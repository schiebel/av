#if defined(MIRIAD)
//# Copyright (C) 2003 Board of Trustees of the University of Illinois
//#
//# This software, both binary and source, is copyrighted by The
//# Board of Trustees of the University of Illinois.  Ownership
//# remains with the University.  You should have received a copy 
//# of a licensing agreement with this software.  See the file 
//# "AIPSVIEW_COPYRIGHT", or contact the University at this address:
//#
//# 	The NCSA AipsView Visualization System
//# 	National Center for Supercomputing Applications
//# 	University of Illinois
//# 	405 North Mathews Ave.
//# 	Urbana, IL 61801
//# 	aipsview@ncsa.uiuc.edu
//# --------------------------------------------------
//
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvMIRDataSet.cc,v 19.5 2003/10/22 17:20:08 hravlin Exp $
//
// $Log: AvMIRDataSet.cc,v $
// Revision 19.5  2003/10/22 17:20:08  hravlin
// Added hack to get const qualifiers from miriad.h when using Sun's compiler.
//
// Revision 19.4  2003/09/26 16:37:08  hravlin
// Added support for masked data. General cleanup.
//
// Revision 19.3  2003/09/17 21:25:05  hravlin
// For getKeyword (char), made sure we don't try to copy a null string.
//
// Revision 19.2  2003/09/15 20:09:48  hravlin
// Standalone aipsview doesn't look for miriad.h under mirlib.
//
// Revision 19.1  2003/09/02 21:17:30  hravlin
// First pass at a reader for MIRIAD image files.
//

//---------------------------------------------------------------------------
/* AvMIRDataSet.cc
   Handles reading of MIRIAD files for aipsview.
   This was converted from the FITs file reader and may still have some
   of its pecularities.
*/
#include <stdio.h>
#include <math.h>

// It appears that only IRIX and Solaris have ieeefp.h.
#if defined(__sgi) || defined(__sun)
#include <ieeefp.h>
#define isnan(x) isnanf((x))
#endif

#include <iostream.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string>
using namespace std;

#if defined(__APPLE_CC__) && defined(__POWERPC__)
// This needs to be after some above include file.
extern "C" { int isnan(double x); }
#endif

#ifdef __GNUC__
extern "C" {
#endif
#include <sys/mman.h>
#ifdef __GNUC__
}
#endif
#include <sys/stat.h>
     
#include <AvString.h>
#include <AvMIRDataSet.h>
#include <Av.h>

///////////////////////////////////////
// Hack to convince miriad.h that we want "const" qualifiers when using
// Sun's compiler
#if defined(__sun) && (__STDC__ == 0)
#define my__STDC__ __STDC__
#undef __STDC__
#define __STDC__ 1
#endif
///////////////////////////////////////

#if defined(AIPSPLUSPLUS)
#include <mirlib/miriad.h>
#else
#include <miriad.h>
#endif

///////////////////////////////////////
#if defined(my__STDC__)
#undef __STDC__
#define __STDC__ 0
#endif
///////////////////////////////////////

#if !defined(M_PI)
#define M_PI            3.14159265358979323846
#endif

// If this is defined, it will enable turning on tracing. Currently, only
// numerical kewords are traced.
#define MIRTRACE
#ifdef MIRTRACE
static Boolean getBooleanEnv(const char *name, const Boolean def=FALSE)
{

	if(name == NULL)
		return def;
	char *val = getenv(name);
	if(val == NULL)
		return def;
	switch(*val) {
	case 'T':
	case 'Y':
	case 't':
	case 'y':
		return TRUE;
	default:
		return FALSE;
	}
}

static Boolean traceMIR_ = FALSE;

static inline Boolean traceMIR(){return traceMIR_;}
static void checkMIRTrace()
{
	traceMIR_ = getBooleanEnv("AV_TRACEMIR", FALSE);
}

#endif

//================================================================

// Normally we use a macro to check for NaN rather than calling a function
// for every data point. If 'USEISNANF' is defined, the function will be used.
// NOTE: If the date are displayed as all WHITE, USEISNANF may need to
//	be defined.
//#define USEISNANF
#ifndef USEISNANF
// Macro from ieeefp.h to test floats for NaN.
//  (replaced 'long' with uInt and removed 'L' suffix on constants since
//   longs are 64 bits in some circumstances).
#define isNaNf(x)       (((*(uInt *)&(x) & 0x7f800000) == 0x7f800000) && \
			 ((*(uInt *)&(x) & 0x007fffff) != 0x00000000))
#else
#define	isNaNf(x)	isnanf(x)
#endif


AvMIRDataSet::AvMIRDataSet()
{
  MIR_init();
}

Boolean AvMIRDataSet::attached()const
{	return (tno_ > 0) ? TRUE : FALSE;
}

// Force variables to 0 value.
void AvMIRDataSet::MIR_init()
{
#ifdef MIRTRACE
	checkMIRTrace();
#endif

  tno_ = -1;
  whichRow_ = 0;
  currentRow_ = 0;
  currentMask_ = 0;

  fileName_ = "";
  dataSetName_ = "";
  dataUnits_ = "";

  isMasked_ = false;	// true if there is a mask file.

  dataMin_ = FLOATMAX;
  dataMax_ = -FLOATMAX;
  cs_ = NULL;
}

AvMIRDataSet::~AvMIRDataSet()
{
  detach_();
}

// Detach from file if attached. Cleanup and reset pointers.
// May be called internally after a failed attach to clean up.
void AvMIRDataSet::detach_()
{
	if (cs_) delete cs_;

	if(tno_ >= 0)
		xyclose_c(tno_);

	delete [] whichRow_;
	delete [] currentRow_;
	delete [] currentMask_;

	MIR_init();	// Reset all the pointers.
}

// Error logging routines.
void AvMIRDataSet::error_out(const char *funcname, const char *msg,
			     const char *param)const
{
  cerr << "AvMIRDataSet::" << funcname << ": ";
  if(fileName()!= NULL)
    cerr << "(" << fileName() << ") ";
  cerr << msg;
  if(param != NULL)
    cerr << param << endl;
  else
    cerr << endl;
} 

void AvMIRDataSet::error_out(const char *funcname, const char *msg,
			     int param)const
{
  cerr << "AvMIRDataSet::" << funcname << ": ";
  if(fileName()!= NULL)
    cerr << "(" << fileName() << ") ";
  cerr << msg << param << endl;
}

void AvMIRDataSet::error_out(const char *funcname, const char *msg,
			     const AvIPosition &param)const
{
  cerr << "AvMIRDataSet::" << funcname << ": ";
  if(fileName()!= NULL)
    cerr << "(" << fileName() << ") ";
  cerr << msg << param << endl;
}

////////////////////////////////////////////////////////////////
//		MIR keyword routines.
////////////////////////////////////////////////////////////////

// Return the value of a keyword. If the keyword doesn't exist,
// returns the default and FALSE.
Boolean  AvMIRDataSet::getKeyword(const char *keyword, int &value,
				  int def)const
{ Boolean	result;

	if(hdprsnt_c(tno_, keyword))
	{	rdhdi_c(tno_, keyword, &value, def);
		result = true;
	}
	else
	{	value = def;
		result = false;
	}

#ifdef MIRTRACE
	if(traceMIR())
	{	cerr	<< "AvMIRDataSet::getKeyword(I): "
			<< (result ? "T" : "F")
			<< " " << keyword
			<< " " << value << endl;
	}
#endif

  return result;
}

Boolean  AvMIRDataSet::getKeyword(const char *keyword, float &fvalue,
				  float def)const
{ Boolean result;
  
	if(hdprsnt_c(tno_, keyword))
	{	rdhdr_c(tno_, keyword, &fvalue, def);
		result = true;
	}
	else
	{	fvalue = def;
		result = false;
	}

#ifdef MIRTRACE
	if(traceMIR())
	{	cerr	<< "AvMIRDataSet::getKeyword(F): "
			<< (result ? "T" : "F")
			<< " " << keyword
			<< " " << fvalue << endl;
	}
#endif

  return result;
}

Boolean  AvMIRDataSet::getKeyword(const char *keyword, double &value,
				  double def)const
{ Boolean	result;

	if(hdprsnt_c(tno_, keyword))
	{	rdhdd_c(tno_, keyword, &value, def);
		result = true;
	}
	else
	{	value = def;
		result = false;
	}

#ifdef MIRTRACE
	if(traceMIR())
	{	cerr	<< "AvMIRDataSet::getKeyword(D): "
			<< (result ? "T" : "F")
			<< " " << keyword
			<< " " << value << endl;
	}
#endif

  return result;
}

Boolean  AvMIRDataSet::getKeyword(const char *keyword, char *value,
				  int len, const char *def)const
{ Boolean result;

	if(value == NULL)		// No place to store value.
		return FALSE;

	if(hdprsnt_c(tno_, keyword))
	{	rdhda_c(tno_, keyword, value, def, len);
		result = true;
	}
	else
	{	if(def != 0)
			strcpy(value, def);
		else
			value[0] = 0;
		result = false;
	}

#ifdef MIRTRACE
	if(traceMIR())
	{	cerr	<< "AvMIRDataSet::getKeyword(S): "
			<< (result ? "T" : "F")
			<< " " << keyword
			<< " " << value << endl;
	}
#endif

	return result;
}

////////////////////////////////////////////////////////////////
///		AvDataSet related functions
////////////////////////////////////////////////////////////////

const char *AvMIRDataSet::dataSetTypeName(){ return "MIRIAD";}
const char *AvMIRDataSet::dataSetType()const
{ return AvMIRDataSet::dataSetTypeName();}

const char *AvMIRDataSet::dataSetName()const {return dataSetName_.c_str();}
const char *AvMIRDataSet::fileName()const
{
	if(fileName_ != "")
		return fileName_.c_str();
	else
		return 0;
}

// Size along each axis.
AvIPosition AvMIRDataSet::shape() const
{      
	return shape_;
}

float	AvMIRDataSet::dataMin()const { return dataMin_; }
float	AvMIRDataSet::dataMax()const { return dataMax_; }

// Type an error message saying a keyword wasn't found then detach
// and return FALSE.
// Used only by attach_.
Boolean AvMIRDataSet::noKW(const char *kw)
{ char buf[128];
  sprintf(buf, "Could not find keyword: %s", kw);
  error_out("attach_", buf);
  detach_();
  return FALSE;
}

Boolean AvMIRDataSet::initattach_(const char* pname)
{
	if(pname == 0)
		return FALSE;

	if(attached())
	{	error_out("attach_", "Already attached.");
		return FALSE;
	}

	// MIRIAD will abort if it can't open the file.
	if(!isDataSet(pname))
		return false;

	MIR_init();

	// open the dataset and get its shape.
	{ int axes[MAXAXES], naxes, i;
		xyopen_c(&tno_, pname, "old", MAXAXES, axes);

	/* Assume that if we get here, the open worked. March
	   backwards through the axis array ignoring trailing entries with
	   a value of 1. Use what's left for the shape.
	 */

		for(i=MAXAXES-1; i > 0; i--)
			if(axes[i] > 1) break;
		naxes = i + 1;
		shape_.resize(naxes);
		for(int j=0; j < naxes; j++)
			shape_(j) = axes[j];
	}

	// Make a copy of the filename w/o path or trailing '/'.
	{ string::size_type pos, len;

		fileName_ = pname;
		// Trim trailing '/'s.
		while((len = fileName_.length()) > 0)
		{	len -= 1;
			if(fileName_[len] == '/')
				fileName_.erase(len);
			else
				break;
		}
		// Remove path part.
		pos = fileName_.find_last_of('/');
		if(pos)
			fileName_.erase(0, pos+1);
	}

	return TRUE;
}

//			Attach to MIR file.
// Open for reading.
//	Open file
//		Do minmax search if necessary.
Boolean AvMIRDataSet::attach_(const char *pname)
{
  if (! initattach_(pname))
    return FALSE;

  ////////////////////////////////////////////////////////////////
  // OK, It's a MIR file.
  ////////////////////////////////////////////////////////////////
  
  ////////////////////////////////////////////////////////////////
  // Get required keyword/value pairs
  ////////////////////////////////////////////////////////////////
  
  // Number of axes
  int rank = shape_.nelements();

  rowWidth_ = shape_(0);

  if(rank < 2)
    {	
      error_out("attach_", "Rank must be at least 2 : ", rank);
      detach_();
      return FALSE;
    }

  // Initialize variables used by getRowPtr().
  whichRow_ = new int[rank];
  for(int i=0; i< rank; i++)
	whichRow_[i] = 0;
  // Enough space to hold one row.
  currentRow_ = new float[rowWidth_];
  currentMask_ = new int[rowWidth_];
  isMasked_ = isReadable(pname, "mask");	// See if there is a mask file.
  if(!isMasked_)				// If no, all points are good.
	  for(int i=0; i< rowWidth_; i++)
		currentMask_[i] = 1;

  // won't have more than 16 axes
  AvString ctypes[16];
  double crvals[16];
  double cdelts[16];
  double crpixs[16];
  double * pcmatrix = new double[rank*rank];	// Optional PC or CD matrix.
  Boolean foundPC = False;
  int i,j;
   // Initialize to identity.
  memset(pcmatrix, 0, rank*rank*sizeof(double));
  for (i = 0; i < rank; i++) pcmatrix[i*(rank+1)] = 1;

  // To track that there were missing Cxxxx keywords.
  enum {CRPIX=1, CDELT=2, CRVAL=4};
  int cerror=0;

	// To convert from radians to degrees.
	static const double rad2Deg = 180.00/M_PI;

  for(i = 0; i < rank; i++)
    {
      char kword[16], ctype[80], ctypeDefault[20];
      //int nelements;
    
      // get the label
      sprintf(kword, "ctype%d", i+1);
      sprintf(ctypeDefault, "Axis%d", i+1);
      getKeyword(kword, ctype, sizeof(ctype), ctypeDefault);
      ctypes[i] = ctype;

      // get the reference pixel, reference value, cdelt value
      sprintf(kword, "crpix%d", i+1);
      if(!getKeyword(kword, crpixs[i]))
      {	crpixs[i] = 1.0;
	cerror |= CRPIX;
      }
      
      sprintf(kword, "cdelt%d", i+1);
      if(!getKeyword(kword, cdelts[i]))
      {	cdelts[i] = 1.0;
	cerror |= CDELT;
      }
      else
      if(cdelts[i] == 0.0)	// CDELT KW was set to 0!
      {	cdelts[i] = 1.0;
		cerr << kword << " was set to 0.0, resetting to 1.0." << endl;
      }

      sprintf(kword, "crval%d", i+1);
      if(!getKeyword(kword, crvals[i]))
      {	crvals[i] = 1.0;
	cerror |= CRVAL;
      }

	// MIRIAD -> FITS format.
	// Taken from MIRIAD's fits.for.
	if((strncmp("RA---", ctype, 5) == 0) ||
	   (strncmp("DEC--", ctype, 5) == 0) ||
	   (strncmp("GLON-", ctype, 5) == 0) ||
	   (strncmp("GLAT-", ctype, 5) == 0) ||
	   (strncmp("ELON-", ctype, 5) == 0) ||
	   (strncmp("ELAT-", ctype, 5) == 0) ||
	   (strncmp("ANGLE", ctype, 5) == 0))
	{
		cdelts[i] *= rad2Deg;
		crvals[i] *= rad2Deg;
	}
	else
	if((strncmp("VELO-", ctype, 5) == 0) ||
	   (strncmp("FELO-", ctype, 5) == 0))
	{
		cdelts[i] *= 1000.0;
		crvals[i] *= 1000.0;
	}
	else
	if(strncmp("FREQ-", ctype, 5) == 0)
	{
		cdelts[i] *= 1.0E9;
		crvals[i] *= 1.0E9;
	}
  }

    // MIRIAD images don't seem to have these keywords.
  // If no CDELT keywords, check for CD matrix.
  // If ANY CD matrix keyword is found, assume it's all there.
  if(cerror & CDELT)
  { char kword[16];
    for (j = 0; j < rank; j++)
      for (i = 0; i < rank; i++)
      {	  sprintf(kword, "cd%03d%03d", i+1, j+1);
	  if(getKeyword(kword, pcmatrix[j*rank+i], (i == j) ? 1.0 : 0.0))
	    foundPC = True;
	  else
	  { sprintf(kword, "CD%d_%d", i+1, j+1); // alternate KW form.
	    if(getKeyword(kword, pcmatrix[j*rank+i], (i == j) ? 1.0 : 0.0))
		foundPC = True;
	  }
      }
     if(foundPC) cerror &= ~CDELT;	// Turn off CDELT warning.
  }

  // If cerror isn't zero, give a warning message.
  if(cerror != 0)
  {	cerr << "Missing keyword(s) (and defaults): ";
	if(cerror & CRPIX) cerr << "CRPIX(1.0) ";
	if(cerror & CRVAL) cerr << "CRVAL(1.0) ";
	if(cerror & CDELT) cerr << "CDELT(1.0)";
	cerr << endl;
  }

  // (Optional) name of dataset.
  { char label[80];
    
    if (getKeyword("object", label, sizeof(label)))
      dataSetName_ = Strdup(label);
    else
      dataSetName_ = Strdup(fileName());
  }
  
  // Fill in strides used to convert from an AvIPosition to a vector index.
  stride_.resize(rank);
  stride_(0) = 1;
  for(i=1; i< rank; i++)
    stride_(i) = stride_(i-1)*shape_(i-1);

  getMinMax();

  // Setup the World Coordinate System

  {
    char kword[16];

    // (Optional) Get the longitude rotation reference point
    double longPole;
    getKeyword("longpole", longPole, 999.0);

#if 0
#ifdef __sun
      // This is only so the Sun memory checker doesn't give rui
      // messages in later strlen() calls.
	memset(kword, 0, sizeof(kword));
#endif
#endif

    // MIRIAD images don't seem to have these keywords.
    // (Optional) projection parameters
    double projps[10];
    for (i = 0; i < 10; i++)
      {
	sprintf(kword, "projp%d", i+1);
	getKeyword(kword, projps[i], 0.0);
      }

    if(!foundPC)	// If found => have CD matrix.
    { for (j = 0; j < rank; j++)
       for (i = 0; i < rank; i++)
	{
	  sprintf(kword, "pc%03d%03d", i+1, j+1);
	  if (getKeyword(kword, pcmatrix[j*rank+i], (i == j) ? 1.0 : 0.0) == True)
	    foundPC = True;
	}
    }

    // Scan for CROTA on latitude axis if no pc found
    if (foundPC == False)
      {
	int longAxis = -1;
	int latAxis = -1;
	for (int r = 0; r < rank; r++)
	  {
	    // look for longitude axis
	    if (!strncmp(ctypes[r].chars(), "RA--", 4) ||
		!strncmp(ctypes[r].chars(), "GLON", 4) ||
		!strncmp(ctypes[r].chars(), "ELON", 4))
	      {
#ifdef AVFDS_CROTA_DEBUG
		cerr << "found longitude axis on " << r+1 << endl;
#endif
		longAxis = r;
	      }
	    else if (!strncmp(ctypes[r].chars(), "DEC-", 4) ||
		     !strncmp(ctypes[r].chars(), "GLAT", 4) ||
		     !strncmp(ctypes[r].chars(), "ELAT", 4))
	      {
#ifdef AVFDS_CROTA_DEBUG
		cerr << "found latitude axis on " << r+1 << endl;
#endif
		latAxis = r;
	      }
	  }

	if (longAxis >= 0 && latAxis >= 0)
	  {
	    sprintf(kword, "crota%d", latAxis+1);
	    double angle;
	    if (getKeyword(kword, angle, 0.0) == True)
	      {
#ifdef AVFDS_CROTA_DEBUG
		cerr << "found CROTA of "<< angle << " degrees on latitude axis "
		     << latAxis+1 << ".\n";
#endif
		
		// convert to radians and store
		// (Miriad stores angles in radians).
		//angle *= M_PI/180.0;  
		pcmatrix[latAxis*rank+latAxis] = cos(angle);
		pcmatrix[latAxis*rank+longAxis] = -sin(angle);
		pcmatrix[longAxis*rank+latAxis] = sin(angle);
		pcmatrix[longAxis*rank+longAxis] = cos(angle);
	      }
	  }
      }

    cs_ = new AvCoordinateSystem(rank,
				 ctypes,
				 crvals,
				 crpixs,
				 cdelts,
				 pcmatrix,
				 projps,
				 longPole);
    
    // Establish axis units based on MIR conventions.  Add as
    // needed, consulting AvUnits for units and measurement type.
    for (i = 0; i < rank; i++)
      {
	AvString s = cs_->axisName(i);
	if (s == "RA")
	  cs_->setAxisUnits(i, "degrees", "right ascension");
	else if (s == "ELON" || s == "GLON")
	  cs_->setAxisUnits(i, "degrees", "longitude");
	else if (s == "DEC")
	  cs_->setAxisUnits(i, "degrees", "declination");
	else if (s == "ELAT" || s == "GLAT")
	  cs_->setAxisUnits(i, "degrees", "latitude");
	else if (s == "FREQ")
	  cs_->setAxisUnits(i, "hertz", "frequency");
	else if (s == "VELO")
	  cs_->setAxisUnits(i, "meters/second", "velocity");
	else if (s == "FELO")
	  cs_->setAxisUnits(i, "meters/second", "optical velocity");
	else if (s == "STOKES" || s == "Polarization")
	  cs_->setAxisUnits(i, "stoke index", "polarization");
	else if (s == "SEQ NUM")
	  cs_->setAxisUnits(i, "sequence number", "sequence");
	else
	  cs_->setAxisUnits(i, "", "");
      }
//    delete pcmatrix;
//    pcmatrix = 0;
  }
  delete [] pcmatrix;
  pcmatrix = 0;

  // Establish data units (sets dataUnits, dataUnitsAbbrev, and
  // dataMeasurementName).
  char label[80];

  dataUnits_ = (getKeyword("bunit", label, sizeof(label)))
		? downcase(label) : AvString("");

  AvString s = downcase(dataUnits_);

  // [ ] Replace later with AvUnits::lookup(str, ...) that
  // can handle prefixes.
  if (s == "mjy/beam")
    setDataUnits("millijanskys/beam", "specific intensity");
  else if (s == "jy/beam")
    setDataUnits("janskys/beam", "specific intensity");
  else if (s == "gauss")
    setDataUnits("gauss", "magnetic field strength");
  else
    {
      AvString prefix, base, type;
      double multiplier;
      if (AvUnits::lookup(s, prefix, base, type, multiplier))
	setDataUnits(prefix+base, type);
      else
	setDataUnits(s, "");
    }

  return TRUE;
}

const char * AvMIRDataSet::dataUnits() const
{ return dataUnits_.chars(); }
const char * AvMIRDataSet::dataUnitsName() const
{ return dataUnitsName_.chars(); }
const char * AvMIRDataSet::dataMeasurementName() const
{ return dataMeasurementName_.chars(); }

const char * AvMIRDataSet::axisName(int axis) const
{ return cs_->axisName(axis); }
const char * AvMIRDataSet::axisUnitsName(int axis) const
{ return cs_->axisUnits(axis); }
const char * AvMIRDataSet::axisMeasurementName(int axis) const
{ return cs_->axisMeasurementName(axis); }

AvString AvMIRDataSet::axisValueUnits(int axis,
				      double value,
				      int nsigfigs) const
{
#ifdef AVDSDEBUG 
  cerr << "AvFR::axisValueUnitsStr(axis=" << axis << ",value=" 
       << value << ",nsigfigs=" << nsigfigs << endl;
#endif
  return cs_->valueUnitsStr(axis, value, nsigfigs); 
}

double AvMIRDataSet::axisValueInUnits(int axis,
				      double value,
				      const AvString& units)
{
#ifdef AVDSDEBUG 
  cerr << "AvFR::axisValueInUnitsStr(axis=" << axis << ",value=" 
       << value << ",units=" << units << endl;
#endif
  return cs_->valueInUnits(axis, value, units); 
}

Boolean AvMIRDataSet::IJKToWorld_(const AvWPosition &ijk,
				  AvWPosition &world) const
{
  Boolean retval = cs_->ijk2world(ijk, world);
  return retval;
}

Boolean AvMIRDataSet::WorldToIJK_(const AvWPosition &world,
				  AvWPosition &ijk) const
{
  return cs_->world2ijk(world, ijk);
}

// Recursively iterate through the dataset looking for min/max.
// Note the lack of a '&' on the position. We want it to be copied.
void AvMIRDataSet::doMinMax(AvIPosition position, int axis,
			    float &min, float &max)
{
	if(axis == 1)		// Do plane.
	{ int nrows = shape()(1);
	  int ncols = shape()(0);

		for(int r=0; r < nrows; r++)
		{ const float *buf = getRowPtr(position);
			for(int c=0; c<ncols; c++)
			{ float v = buf[c];
				if(v < min)
					min = v;
				else
				if(v > max)
					max = v;
			}
			position(1) += 1;		// Next row.
		}
	}
	else			// Iterate through higher dimensions.
	{ int size = shape()(axis);
		for(int i=0; i< size; i++)
		{	doMinMax(position, axis-1, min, max);
			position(axis) += 1;
		}
	}
}

// Get the min/max data values.
// First try the keywords. If that fails, search entire file.
void AvMIRDataSet::getMinMax()
{
	if(getKeyword("DATAMIN", dataMin_) && getKeyword("DATAMAX", dataMax_))
		return;
	if(getKeyword("datamin", dataMin_) && getKeyword("datamax", dataMax_))
		return;

	dataMin_ = FLOATMAX;
	dataMax_ = -dataMin_;

	AvIPosition start = AvDataSet::start();
	int axis = rank() -1;	// rank will always be >= 2.
	doMinMax(start, axis, dataMin_, dataMax_);
}

int AvMIRDataSet::getRow(float *buf,
			 const AvIPosition &start, int count,
			 int Stride0, int axis)
{int knt = 0;
 int stride, Stride=Stride0;

	if(!validIJK(start))
	{	error_out("getRow", "Invalid start position : ", start);
		return 0;
	}
	if(count <= 0)
	{	error_out("getRow", "Invalid count : ", count);
		return 0;
	}

	if(!validAxis(axis))
	{	error_out("getRow", "Invalid axis: ", axis);
		return 0;
	}

	if(Stride <= 0)
	{	error_out("getRow", "Stride was <= 0, using 1.\n");
		Stride = 1;
	}
	//stride = this->stride(axis)*Stride;
	stride = Stride;

	// Make sure that there are count elements available along the
	// axis. If not, clip and give a warning message.
	int	slen = axisElements(start(axis), axisLength(axis), Stride);
	if(slen < count)
	{ char cbuf[128];
		// Use supplied stride for printout rather than computed.
		sprintf(cbuf,
  "\nAxis %d has %d elements with start=%d, stride=%d. %d were requested.",
			axis, slen, start(axis), Stride, count);
		error_out("getRow", cbuf);
	}
	else
		slen = count;

	// slen now contains the number of elements to return.

	// If the requested axis is 0, just get the rowPtr and copy data.
	// Otherwise, repeatedly call getDatum to fill buffer.
	{	if(axis == 0)		// Get from axis 0.
		{ const float *rowPtr = getRowPtr(start);
		  int index = 0;

			if(rowPtr != 0)
			 for(int i=0; i< slen; i++)
			 {	buf[i] = rowPtr[index];
				index += stride;
				knt ++;
			 }
		}
		else			// Some other axis.
		{ AvIPosition pos = start;

			for(int i=0; i < slen; i++)
			{ float v  = getDatum(pos);
				buf[knt++] = v;
				pos(axis) += stride;
			}
		}
	}
	return knt;
}

// Return data value at requested position.
// Currently does not check for a proper position.
float AvMIRDataSet::getDatum(const AvIPosition &start)
{ float value;
  int index;

	// Get pointer to row.
	const float *bufr = getRowPtr(start);
	index = start(0);		// Which column.
	value = bufr[index];
	return value;
}

// Return a pointer to a buffer containing the requested row data.
// rownum is zero based.
// rownum(0) (the column) is ignored.
const float *AvMIRDataSet::getRowPtr(const AvIPosition &rownum)
{ int rnk = rank();
  bool haveRow = true;

	// See if current row happens to be what we're looking for.
	// NOTE: aipsview indexes from 0, MIRIAD starts at 1 so we
	// add or subtract 1 as necessary.
	// (whichRow_ is 1 based since it is passed to MIRIAD).
	for(int i=1; i < rnk; i++)	// Ignore column #.
		if(rownum(i) != (whichRow_[i]-1))
		{	haveRow = false;
			break;
		}

	if(!haveRow)
	{int rowNum;
		for(int i=0; i<rnk; i++)	// Save new position.
			whichRow_[i] = rownum(i)+1;
		rowNum = whichRow_[1];
		xysetpl_c(tno_, rnk-2, &whichRow_[2]);
		xyread_c(tno_, rowNum, currentRow_);
		// If we're doing blanking, read the mask values and
		// adjust the data buffer as needed.
		if(isMasked_)
		{ float bv = blankingValue();
			xyflgrd_c(tno_, rowNum, currentMask_);
			for(int i=0; i< rowWidth_; i++)
				if(!currentMask_[i])
					currentRow_[i] = bv;
		}
	}
	return currentRow_;
}

static AvDataSetFactoryInfo factoryInfo_ =
	{ &AvMIRDataSet::makeDataSet,
	  &AvMIRDataSet::isDataSet,
	  &AvMIRDataSet::dataSetTypeName
	};

const AvDataSetFactoryInfo *AvMIRDataSet::getFactoryInfo()
{
	return &factoryInfo_;
}

// Open a dataset and return a pointer to the object.
AvDataSet * AvMIRDataSet::makeDataSet()
{  AvDataSet *ds = new AvMIRDataSet();
	return ds;
}

// See if the file <dir>/<file> exists, is a regular file and is readable.
bool AvMIRDataSet::isReadable(const char *dir, const char *file)
{  char buf[BUFSIZ];
   struct stat s_buf;

	if(dir != 0)
	{	strcpy(buf, dir);
		strcat(buf, "/");
	}
	else
		buf[0] = 0;

	if(file != 0)
		strcat(buf, file);

	if(stat(buf, &s_buf) == -1)
		return false;

	if(!S_ISREG(s_buf.st_mode))
		return false;

	if(access(buf, R_OK) == -1)
		return false;

	return true;
}

/* Returns true if file maybe a Miriad data set directory. Otherwise false.
*/
bool AvMIRDataSet::isDataSet(const char *filename)
{
	if(!isReadable(filename, "header"))
		return false;
	if(!isReadable(filename, "image"))
		return false;

	return true;
}
#endif
