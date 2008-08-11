//# Copyright (C) 1995-2002 Board of Trustees of the University of Illinois
//#
//# This software, both binary and source, is copyrighted by The
//# Board of Trustees of the University of Illinois. Ownership
//# remains with the University. You should have received a copy 
//# of a licensing agreement with this software. See the file 
//# "AIPSVIEW_COPYRIGHT", or contact the University at this address:
//#
//#     The NCSA AipsView Visualization System
//#     National Center for Supercomputing Applications
//#     University of Illinois
//#     405 North Mathews Ave.
//#     Urbana, IL 61801
//#     aipsview@ncsa.uiuc.edu
//# --------------------------------------------------
//
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvCoordinateSystem.cc,v 19.1 2003/09/10 05:03:21 mcalabre Exp $
//
// $Log: AvCoordinateSystem.cc,v $
// Revision 19.1  2003/09/10 05:03:21  mcalabre
// Modified to use WCSLIB 3.2.
//
// Revision 19.0  2003/07/16 05:47:06  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:49  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/03 19:51:15  hravlin
// Define M_PI if it's not defined in math.h.
//
// Revision 17.0  2001/11/12 19:42:18  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:26  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:52  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:09  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:56  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:31  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:34  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:23  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:26:56  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:24  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.7  1997/02/18 21:25:34  jpixton
// corrected NCP projection map.
//
// Revision 7.6  1997/01/24 18:38:26  hr
// Added definition for M_PI if it isn't defined with the GNU compiler.
//
// Revision 7.5  1996/12/12 09:37:53  droberts
// Added mapping to initialize AipsView CoordinateSystem classes.  Added simple constructor for linear coordinate system.  Added NCP support.
//
// Revision 1.12  1996/11/18 20:40:12  hr
// Added RCS header.
//
//
//---------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

#if !defined(M_PI)
#define M_PI 3.14159265358979323846
#endif

#include <AvCoordinateSystem.h>
#include <AvCelestialProjection.h>
#include <AvLinearMap.h>
#include <AvUnits.h>


AvCoordinateSystem::AvCoordinateSystem(const int nAxes)
  : nAxes_(nAxes), cp_(0), lm_(0), crvals_(0), longpole_(999.0),
    longAxis_(-1), latAxis_(-1), axisNames_(0),
    axisCoordSysNames_(0),
    axisUnits_(0),
    axisMeasurementNames_(0)
{

  axisNames_ = new AvString[nAxes_];
  axisCoordSysNames_ = new AvString[nAxes_];
  axisUnits_ = new AvString[nAxes];
  axisMeasurementNames_ = new AvString[nAxes_];

  crvals_ = new double[nAxes_];
  double * crpixs = new double[nAxes_];
  double * cdelts = new double[nAxes_];
  double * pcmatrix = new double[nAxes_*nAxes_];
  memset(pcmatrix, 0, nAxes_*nAxes_*sizeof(double));

  double projParms[10];
  memset(projParms, 0, 10*sizeof(double));

  for (int a = 0; a < nAxes_; a++)
    {
      char buf[20];
      sprintf(buf, "axis%d", a+1);
      axisNames_[a] = buf;
      axisCoordSysNames_[a] = "pixels";
      axisUnits_[a] = "";
      axisMeasurementNames_[a] = "pixels";

      crvals_[a] = 1.0;
      crpixs[a] = 1.0;
      cdelts[a] = 1.0;
      pcmatrix[a+nAxes_*a] = 1.0;
    }

  lm_ = new AvLinearMap(crpixs,
			cdelts,
			pcmatrix,
			nAxes_);

  delete [] crpixs;
  delete [] cdelts;
  delete [] pcmatrix;
}

AvCoordinateSystem::AvCoordinateSystem(const int nAxes,
				       const AvString * ctypes,
				       const double * crvals,
				       const double * crpixs,
				       const double * cdelts,
				       const double * pcmatrix,
				       const double * projps,
				       const double longpole)
     
  : nAxes_(nAxes), cp_(0), lm_(0), crvals_(0), longpole_(longpole), longAxis_(-1), latAxis_(-1), axisNames_(0), axisCoordSysNames_(0), axisUnits_(0),
    axisMeasurementNames_(0)
{
  AvString projSystemName;
  
  axisNames_ = new AvString[nAxes];
  axisCoordSysNames_ = new AvString[nAxes];
  axisUnits_ = new AvString[nAxes];
  axisMeasurementNames_ = new AvString[nAxes];

  // Read and interpret the ctype keywords
  for (int a = 0; a < nAxes_; a++)
    {
      AvString s = upcase(ctypes[a]);
      
      if (ctypes[a].contains(AvRegex("^... \\(Right Ascension\\)\\|\\(Declination\\)")))
	{
	  // AIPS++ ImageCoordinate format
	  axisNames_[a] = ((AvString) ctypes[a]).after(3);
	  axisCoordSysNames_[a] = ((AvString) ctypes[a]).before(3);

	  if (axisNames_[a] == "Right Ascension")
	    {
	      projSystemName = axisCoordSysNames_[a];
	      longAxis_ = a;
	    }
	  else if (axisNames_[a] == "Declination")
	    {
	      latAxis_ = a;
	    }
	}

      if (s.contains("-"))
	{
	  
	  // divide the type string into axis name and coordinate system
	  AvString sn = AvString(s.at(0,4));
	  AvString sc = AvString(s.at(4,4));
	  
	  assert(s.OK());
	  assert(sn.OK());
	  assert(sc.OK());
	  
	  assert(ctypes[0].OK());
	  assert(ctypes[1].OK());
	  assert(ctypes[2].OK());
	  
#ifdef AVWCSDEBUG
#if 0
	  cout << "axis #" << a+1 << ", name = '" << sn << "', coord sys = '" << 
	    sc << "'\n";
	  cout.flush();
#endif
#endif
	  
	  // Strip away all -'s from both
	  sn.gsub("-","");
	  sc.gsub("-","");
	  
	  axisNames_[a] = sn;
	  axisCoordSysNames_[a] = sc;
	  
#ifdef AVWCSDEBUG
#if 0
	  cout << "axis #" << a+1 << ", name = '" << axisNames_[a] << "', coord sys = '" << 
	    axisCoordSysNames_[a] << "'\n";
	  cout.flush();
#endif
#endif
	  
	  if (axisNames_[a].contains(AvRegex("^\\(\\(RA\\)\\|\\(GLON\\)\\|\\(ELON\\)\\)")))
	    {
#ifdef AVWCSDEBUG
	      cout << "Found first projection axis = axis #" << a+1 << endl;
#endif
	      // if RA,GLON,ELON, setup projection and mark axis
	      projSystemName = axisCoordSysNames_[a];
	      longAxis_ = a;
	    }
	  else if (axisNames_[a].contains(AvRegex("^\\(\\(DEC\\)\\|\\(GLAT\\)\\|\\(ELAT\\)\\)")))
	    {
	      // if DEC,GLAT,ELAT, have second of pair, so mark that axis
#ifdef AVWCSDEBUG
	      cout << "Found second projection axis = axis #" << a+1 << endl;
#endif
	      latAxis_ = a;
	    }
	}
      else
	{
	  // have a weird axis
	  axisNames_[a] = ctypes[a];
	  if (ctypes[a].contains("Polarization"))
	    {
	      axisCoordSysNames_[a] = "stokes";
	      axisUnits_[a] = "stoke index";
	      axisMeasurementNames_[a] = "polarization";
	    }
	  else if (axisNames_[a].contains("STOKES"))
	    {
	      axisCoordSysNames_[a] = "stokes";
	      axisUnits_[a] = "stoke index";
	      axisMeasurementNames_[a] = "polarization";
	    }
	  else if (axisNames_[a].contains("SEQ.NUM."))
	    {
	      axisCoordSysNames_[a] = "sequence";
	      axisUnits_[a] = "sequence number";
	      axisMeasurementNames_[a] = "sequence";
	    }
	  else if (axisNames_[a].contains("CHANNEL"))
	    {
	      axisCoordSysNames_[a] = "channel index";
	      axisUnits_[a] = "channel number";
	      axisMeasurementNames_[a] = "channel";
	    }
	  else if (axisNames_[a].contains("TIME"))
	    {
	      axisCoordSysNames_[a] = "time";
	      axisUnits_[a] = "seconds";
	      axisMeasurementNames_[a] = "time";
	    }
	  else if (axisNames_[a].contains("BEAM"))
	    {
	      axisCoordSysNames_[a] = "beam";
	      axisUnits_[a] = "beam index";
	      axisMeasurementNames_[a] = "beam";
	    }
	  else
	    {
	      // don't know what to do with this axis
	      axisCoordSysNames_[a] = "";
	      axisUnits_[a] = "";
	      axisMeasurementNames_[a] = "";
	    }
#ifdef AVWCSDEBUG
#if 0
	  cout << "axis #" << a+1 << ", name = '" << axisNames_[a] << "', coord sys = '" << 
	    axisCoordSysNames_[a] << "'\n";
	  cout.flush();
#endif
#endif
	}
    }
      
  if (longAxis_ >= 0)
    {
      if (projSystemName == "NCP")
	{
	  double pprojps[10];
	  memset(pprojps, 0, 10*sizeof(double));

	  // map NCP to SIN
	  projSystemName = "SIN";
	  double obslat = crvals[latAxis_];
	  
	  if (obslat == 0.0)
	    pprojps[1] = 1e50;  // big number will suffice ??
	  else
	    pprojps[1] = 1.0/tan(M_PI*obslat/180.0);				   
	  //cout << "NCP-->SIN obslat =" << obslat << endl;
	  //cout << "  pp0 is " << pprojps[0] << endl;
	  //cout << "  pp1 is " << pprojps[1] << endl;

	  // Setup the celestial projection with altered projection parameters
	  cp_ = new AvCelestialProjection(projSystemName,
					  crvals[longAxis_],
					  crvals[latAxis_],
					  longpole_, 999.0,
					  pprojps);
	}
      else
	{
	  // Setup the celestial projection
	  cp_ = new AvCelestialProjection(projSystemName,
					  crvals[longAxis_],
					  crvals[latAxis_],
					  longpole_, 999.0,
					  projps);
	}
#ifdef AVWCSDEBUG
      cout << "Contents of celprm struct:" << endl;
      cp_.celprt();
    }
  else
    {
      cout << "Celestial Projection not found, using linear system only\n";
#endif
      
    }
  
  int i;

  double * crpixsm1 = new double[nAxes_];  

  // subtract 1 because WCSLIB uses 1 offsets
  for (i = 0; i < nAxes_; i++)
    crpixsm1[i] = crpixs[i]-1;

  lm_ = new AvLinearMap(crpixsm1,
			cdelts,
			pcmatrix,
			nAxes_);
  
  crvals_ = new double[nAxes_];
  for (i = 0; i < nAxes_; i++)
    {
      crvals_[i] = crvals[i];
    }

  delete [] crpixsm1;
}

AvCoordinateSystem::~AvCoordinateSystem()
{
  // ?? with this in, dies in _fini trying to delete a
  // const string (go figure!)

  if (lm_) delete lm_;
  if (cp_) delete cp_;
  delete [] crvals_;
  delete [] axisNames_;
  delete [] axisCoordSysNames_;
  delete [] axisUnits_;
  delete [] axisMeasurementNames_;
}

AvCoordinateSystem::AvCoordinateSystem(const AvCoordinateSystem& other)
  : nAxes_(other.nAxes_), 
    cp_(0), 
    lm_(0), 
    crvals_(0), 
    longpole_(other.longpole_), longAxis_(-1), latAxis_(-1), axisNames_(0), axisCoordSysNames_(0), axisUnits_(0),
    axisMeasurementNames_(0)

{
  // forward to op =;
  *this = other;
}

AvCoordinateSystem& AvCoordinateSystem::operator =(const AvCoordinateSystem& other)
{
  if (this != &other)
    {
      // Clean up this class
      if (cp_) delete cp_;
      if (lm_) delete lm_;
      if (crvals_) delete [] crvals_;
      if (axisNames_) delete [] axisNames_;
      if (axisCoordSysNames_) delete [] axisCoordSysNames_;
      if (axisUnits_) delete [] axisUnits_;
      if (axisMeasurementNames_) delete [] axisMeasurementNames_;

      cp_ = NULL;
      lm_ = NULL;

      // copy values from other
      nAxes_ = other.nAxes_;
      if (other.cp_)
	cp_ = new AvCelestialProjection(*other.cp_);
      else
	cp_ = other.cp_;

      lm_ = new AvLinearMap(*other.lm_);
      crvals_ = new double[nAxes_];
      memcpy(crvals_, other.crvals_, nAxes_*sizeof(double));

      longpole_ = other.longpole_;
      longAxis_ = other.longAxis_;
      latAxis_ = other.latAxis_;

      axisNames_ = new AvString[nAxes_];
      axisCoordSysNames_ = new AvString[nAxes_];
      axisUnits_ = new AvString[nAxes_];
      axisMeasurementNames_ = new AvString[nAxes_];
      
      for (int r = 0; r < nAxes_; r++)
	{
	  axisNames_[r] = other.axisNames_[r];
	  axisCoordSysNames_[r] = other.axisCoordSysNames_[r];
	  axisUnits_[r] = other.axisUnits_[r];
	  axisMeasurementNames_[r] = other.axisMeasurementNames_[r];
	}
    }
  return *this;
}

int AvCoordinateSystem::world2ijk(const AvWPosition & world,
				  AvWPosition & ijk)
{
  double d_world[16];
  double d_ijk[16];
  unsigned int i;

  ijk.resize(world.nelements());

  for (i = 0; i < world.nelements(); i++)
    d_world[i] = world(i);
  int retval = world2ijk(d_world, d_ijk);
  for (i = 0; i < world.nelements(); i++)
    ijk(i) = d_ijk[i];
  return retval;
}
				  

int AvCoordinateSystem::world2ijk(const double * world,
		double * ijk)
{
  double worldvals[16];

  // apply crval offset to non-celestial coordinates
  int i;
  for (i = 0; i < nAxes_; i++)
    {
      if (i == longAxis_) continue;
      if (i == latAxis_) continue;
      worldvals[i] = world[i] - crvals_[i];
    }

  // map the celestial projection   
  if (cp_)
    {
      double lng = world[longAxis_];
      double lat = world[latAxis_];      
      
      cp_->celestial2world(lng, lat, worldvals[longAxis_], worldvals[latAxis_]);
    }

  // then map them all linearly
  lm_->world2pixel(worldvals, ijk);

  return 1;
}

int AvCoordinateSystem::ijk2world(const AvWPosition & ijk,
				  AvWPosition & world)
{
  double d_ijk[16];
  double d_world[16];
  unsigned int i;

  world.resize(ijk.nelements());

  for (i = 0; i < ijk.nelements(); i++)
    d_ijk[i] = ijk(i);
  int retval = ijk2world(d_ijk, d_world);
  for (i = 0; i < ijk.nelements(); i++)
    world(i) = d_world[i];
  return retval;
}
				  

// a.k.a pixel2celestial
int AvCoordinateSystem::ijk2world(const double * ijk,
				  double * world)
{
#ifdef AVWCSDEBUG
  cout << "AvCS:i2w ijk = <" << ijk[0] << "," << ijk[1] << "," << ijk[2] << ">\n";
#endif

  lm_->pixel2world(ijk, world);

  if (cp_)
    {
      double x = world[longAxis_];
      double y = world[latAxis_];
#ifdef AVWCSDEBUG
      cout << "longAxis_ = " << longAxis_ << ", latAxis_ = " << latAxis_ << endl;
      cout << "AvCS:x,y = " << x << "," << y << endl;
#endif
      
      cp_->world2celestial(x,
			   y,
			   world[longAxis_],
			   world[latAxis_]);
#ifdef AVWCSDEBUG
      cout << "AvCS:lng,lat = " << world[longAxis_] << "," << world[latAxis_] << endl;
#endif
    }

  // add the offset crvals for non-projection axes
  int i;
  for (i = 0; i < nAxes_; i++)
    {
      if (i == longAxis_) continue;
      if (i == latAxis_) continue;
      world[i] += crvals_[i];
    }

#ifdef AVWCSDEBUG
  cout << "AvCS:i2w world = <" << world[0] << "," << world[1] << "," << world[2] << ">\n";
#endif
  return 1;
}

void AvCoordinateSystem::setAxisUnits(int axis, 
				      const AvString& units, 
				      const AvString& mname)
{
  assert(axis>=0 && axis<nAxes_);

  axisUnits_[axis] = units; 
  axisMeasurementNames_[axis] = mname;
}

AvString AvCoordinateSystem::valueUnitsStr(int axis,
					   double value,
					   int nsigfigs) const
{
  assert(axis>=0 && axis<nAxes_);
  return AvUnits::makeNiceString(value,
				 axisUnits_[axis],
				 nsigfigs,
				 axisMeasurementNames_[axis]);
}

double AvCoordinateSystem::valueInUnits(int axis,
					double value,
					const AvString& units) const
{
  double retval = value;
  assert(axis>=0 && axis<nAxes_);
  AvUnits::convertValue(value, 
			axisUnits_[axis],
			units,
			retval,
			axisMeasurementNames_[axis]);
  return retval;
}
