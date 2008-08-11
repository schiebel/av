//# Copyright (C) 1995-2003 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvAipsDataSet.cc,v 19.1 2003/09/02 21:05:11 hravlin Exp $
//
// $Log: AvAipsDataSet.cc,v $
// Revision 19.1  2003/09/02 21:05:11  hravlin
// Added makeDataSet(), dataSetTypeName(), isDataSet and factory support.
//
// Revision 19.0  2003/07/16 05:46:47  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:33  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/02/26 07:17:27  gvandiep
// Conversion String to char*
//
// Revision 17.0  2001/11/12 19:42:01  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:10  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.1  2001/01/24 00:49:14  wbrouw
// Needed Regex.h
//
// Revision 15.0  2000/10/26 17:08:46  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/06/19 10:53:21  gvandiep
// removed end_try
//
// Revision 14.0  2000/03/23 16:07:49  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.2  2000/01/24 18:06:05  gvandiep
// Copped Lattice classes moved from trial to aips
//
// Revision 13.1  1999/08/27 16:29:45  droberts
// Commented message about "Error opening image ...", since it happens when
// reading valid FITS data sets.
//
// Revision 13.0  1999/08/10 18:39:32  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:50  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:36  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:16  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.5  1998/07/10 21:53:38  hr
// In attach_(), added a local test to see if filename is an AIPS++ data set
// since exceptions aren't always handled well.
//
// Revision 9.4  1998/04/02 11:43:34  gvandiep
// removed call to ImageInterface::isMasked
//
// Revision 9.3  1998/03/16 17:00:34  bglenden
// Allow for expression images.
//
// Revision 9.2  1998/03/04 01:02:23  bglenden
// Handle NaN's. Cleanup -Wall messages.
//
// Revision 9.1  1997/11/07 18:58:42  bglenden
// Quiet Purify
//
// Revision 9.0  1997/08/25 21:25:09  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.4  1997/08/12 21:40:07  droberts
// Added return value for getRow.
//
// Revision 8.3  1997/07/11 23:11:19  bglenden
// Need PC and PROJP
//
// Revision 8.2  1997/07/11 16:02:28  bglenden
// Bug fix.
//
// Revision 8.1  1997/05/20 18:41:27  bglenden
// Set up an idle loop to relinquish locks when some other process wants them.
// Relinquish all locks and turn of the idle function if we have been idle
// for long enough. This should let aipsview swap out when it's truly not
// being used.
//
// Revision 8.0  1997/02/20 03:17:40  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.12  1997/01/30 18:02:29  bglenden
// Get rid of debugging print statement.
//
// Revision 7.11  1997/01/30 03:41:32  bglenden
// Improve coordinate handling a bit.
//
// Revision 7.10  1997/01/30 01:12:38  bglenden
// Fix bug in setting shape_
//
// Revision 7.9  1997/01/30 00:04:16  bglenden
// Improve the binding of coordinates to AIPS++ images.
//
// Revision 7.8  1997/01/16 22:25:20  bglenden
// Improve coordination with new AIPS++ coordinate system. Still doesn't
// handle units etc. as well as it should.
//
// Revision 7.7  1997/01/16 02:13:36  bglenden
// Change for new coordinates.
//
// Revision 7.6  1996/12/12 08:38:04  droberts
// Changed double quotes to chevrons in include file specification.
//
// Revision 7.5  1996/10/25 22:42:23  jpixton
// prevented pagedImage from getting a file that ended in a slash
// also added missing error checking.
//
// Revision 7.4  1996/10/23 03:40:19  jpixton
// Now assumes radians for any latitude or longitude axis.
//
// Revision 7.3  1996/10/22 19:02:18  jpixton
// added getSlice() (AipsView's version) and improved getRow() - both
// now use Lattice::getSlice().
//
// Revision 7.2  1996/10/11 21:45:53  jpixton
// Added mapping to initialize AipsView's CoordinateSystem classes.
//
// Revision 7.1  1996/10/10 16:55:12  jpixton
// initial revision, native aips++ images supported.
//
// Revision 7.1  1996/09/24 20:14:01  jpixton
// initial checkin before native aips images supported.
//
// Revision 1.9  1996/09/20 20:19:35  pixton
// Prefixed all classes with Av
//
// Revision 1.8  1996/09/17  16:33:49  hr
// Clear "kword" to 0 if running on a Sun to make memory checker happy.
//
     
#include <aips/Arrays/Vector.h>
#include <AvString.h>
#include <AvAipsDataSet.h>

#include <aips/aips.h>
#include <aips/Tables/Table.h>
#include <aips/Arrays/ArrayMath.h>
#include <aips/Arrays/Matrix.h>
#include <aips/Utilities/Assert.h>
#include <aips/Utilities/String.h>
#include <aips/Utilities/Regex.h>
#include <aips/Mathematics/Constants.h>
#include <aips/Exceptions/Error.h>
#include <aips/Lattices/LatticeStepper.h>
#include <aips/Lattices/LatticeIterator.h>
#include <trial/Images/PagedImage.h>
#include <trial/Coordinates/DirectionCoordinate.h>
#include <aips/Containers/Record.h>

#include <trial/Images/ImageExprParse.h>
#include <trial/Images/ImageExpr.h>
#include <trial/Lattices/LatticeExprNode.h>

////////////////////////////////////////////
// Only need this for isAipsPlusPlus()    //
#include <sys/types.h>
#include <sys/stat.h>
////////////////////////////////////////////

// Taken from AvFitsDataSet.cc
#define isNaNf(x)       (((*(long *)&(x) & 0x7f800000L) == 0x7f800000L) && \
                         ((*(long *)&(x) & 0x007fffffL) != 0x00000000L))


AvDataSet *foofoo()
{
    return new AvAipsDataSet;
}

static IPosition AvIPosition2IPosition(const AvIPosition& aip)
{
  IPosition retval(aip.nelements());
  for (uInt i = 0; i < aip.nelements(); i++)
    retval(i) = aip(i);
  return retval;
}

AvAipsDataSet::AvAipsDataSet()
  : image_(0),
    cs_(0),
    isMasked_(0),
    dataMin_(MAXFLOAT),
    dataMax_(-MAXFLOAT)
{
  start_timer_if_necessary();
}

AvAipsDataSet::~AvAipsDataSet()
{
  detach_();
}

Boolean AvAipsDataSet::attached() const
{	
  start_timer_if_necessary();
  return (image_ != NULL);
}


// Detach from file.
void AvAipsDataSet::detach_()
{
  if (attached())
    {
      if (image_) delete image_;
      if (cs_) delete cs_;
      stop_timer();
    }
}

const char *AvAipsDataSet::dataSetTypeName() { return "AIPS++"; }
const char *AvAipsDataSet::dataSetType() const
{ return AvAipsDataSet::dataSetTypeName();}


const char *AvAipsDataSet::name() const 
{  start_timer_if_necessary();
 return image_ ? image_->name().chars() : ""; }

const char *AvAipsDataSet::fileName() const
{
  start_timer_if_necessary();
  return fileName_.chars(); 
}

// Size along each axis.
AvIPosition AvAipsDataSet::shape() const
{      
  start_timer_if_necessary();
  return shape_;
}

float	AvAipsDataSet::dataMin() const { return dataMin_; }
float	AvAipsDataSet::dataMax() const { return dataMax_; }


/* Make a guess as to whether this is an AIPS++ data set. We'd want the
AIPS++ code to do it for us, but exceptions don't always get caught.

 fname/table.dat must exist.

*/
static const char *TBL = "/table.dat";

static Boolean isAipsPlusPlusFile(const char *fname)
{ struct stat filestuff;
  Boolean isAIPS;

	if(fname == NULL)
		return False;
	char *buf = new char[strlen(fname) + strlen(TBL)+1];
	strcpy(buf,fname);
	strcat(buf, TBL);
	if(stat(buf , &filestuff) < 0)
		isAIPS = False;
  	else
		isAIPS = True;

	delete [] buf;
	return isAIPS;

}


//			Attach to AIPS++ image file.
Boolean AvAipsDataSet::attach_(const char *fname)
{
  start_timer_if_necessary();
  char * tname = new char[strlen(fname)+1];
  strcpy(tname, fname);
  
    try {
	int i;
	if (attached())
	    {	
		cerr << "AvAipsDataSet::attach_() -- Already attached.\n";
		return FALSE;
	    }

	{
	    String name(fname);
	    Regex exprreg("^[eE]xpression: *");
	    if (name.contains(exprreg)) {
		// It's an expression, not a "filled" image.
		name.gsub(exprreg, "");
// 		// We cannot handle $'s
// 		if (name.freq('$') > 0) {
// 		    cerr << "Image expressions must contain only file names " <<
// 			"not $ expressions" << endl;
// 		    return FALSE;
// 		}
		LatticeExprNode node = ImageExprParse::command(name);
		image_ = new ImageExpr<Float>(LatticeExpr<Float>(node), fname);
	    } else {
		// Assume it's an ordinary filled image!
		// take out trailing slash if it exists
		if (tname[strlen(tname)-1] == '/') tname[strlen(tname)-1] = 0;

		if(isAipsPlusPlusFile(tname))
			image_ = new PagedImage<Float>(tname);
		else
			image_ = NULL;
	    }
	}


	if (image_==0 || !image_->ok())
	    {
		delete image_;
		image_ = NULL;
//		cerr << "Error opening image - " << fname << endl;
		return FALSE;
	    }

	// take out path
	fileName_ = tname;
	fileName_.gsub(AvRegex(".*/"), "");

	delete [] tname;

	CoordinateSystem coordsys = image_->coordinates();
	const uInt n = coordsys.nWorldAxes();
///	isMasked_ = image_->isMasked();
	shape_.resize(n);

	// won't have more than 16 axes
	AvString ctypes[16];
	double crvals[16];
	double cdelts[16];
	double crpixs[16];


	Vector<String> ctype(n), cunit(n);
	Vector<Double> crval(n), cdelt(n), crpix(n);
	Matrix<Double> pc(n,n);
	Vector<Double> projp;

	// Default to linear
	ctype = "Unknown";
	crval = 0.0;
	cdelt = crpix = crval;
	pc = 0.0; pc.diagonal() = 1.0;

	IPosition shape = image_->shape();
	Record header;
	if (!coordsys.toFITSHeader(header, shape, True, 'c', True)) {
	    cerr << "Error retrieving FITS-like header "
		"from image. Using linear." << endl;
	} else {
	    // OK, we have a header
	    Vector<String> ctype2(n), cunit2(n);
	    Vector<Double> crval2(n), cdelt2(n), crpix2(n);
	    Matrix<Double> pc2(n,n);
	    try {
		if (header.isDefined("projp")) {
		    header.get("projp", projp);
		}
		header.get("ctype", ctype2);
		header.get("cunit", cunit2);
		header.get("crval", crval2);
		header.get("crpix", crpix2);
		header.get("cdelt", cdelt2);
		header.get("pc", pc2);
		AlwaysAssert(ctype2.nelements() == n && 
			     cunit2.nelements() == n &&
			     crval2.nelements() == n &&
			     crpix2.nelements() == n && 
			     cdelt2.nelements() == n && pc2.nrow() == n &&
			     pc2.ncolumn() == n, AipsError);
		ctype = ctype2; 
		cunit = cunit2; 
		crval = crval2; 
		crpix = crpix2; 
		cdelt = cdelt2;
		pc = pc2;
	    } catch (AipsError x) {
		cerr << "Error retrieving FITS-like header from image. "
		    "Using linear." << endl;
	    } 
	}

	for (i=0; i<rank(); i++) {
	    shape_(i) = shape(i);
	    ctypes[i] = ctype(i).chars();
	    crvals[i] = crval(i);
	    cdelts[i] = cdelt(i);
	    crpixs[i] = crpix(i);
	}

	// find out where the real axes are since toFITS might have invented some
	// when nWorldAxes > nPixelAxes.
	real_shape_ = image_->shape();
	axis_map_.resize(coordsys.nPixelAxes());
	uInt count = 0;
	for (i=0; i<Int(coordsys.nPixelAxes()); i++) {
	    Int coord, axis;
	    coordsys.findWorldAxis(coord, axis, i);
	    Int pixaxis = coordsys.pixelAxes(coord)(axis);
	    if (pixaxis >= 0) {
		axis_map_(count) = i;
		count++;
	    }
	}

	// setup min and max
	calcMinMax();

	// Assume default parameters until CoordinateSystem class is
	// available via ImageInterface.
	Double longPole = 999.0;
	Double projps[10];

	Int skyCoord = coordsys.findCoordinate(Coordinate::DIRECTION);
	Int longAxis = -1, latAxis = -1;
	if (skyCoord >= 0) {
	    uInt n = coordsys.nWorldAxes();
	    for (uInt i=0; i<n && (longAxis < 0 || latAxis < 0) ; i++) {
		Int c, a;
		coordsys.findWorldAxis(c, a, i);
		if (c == skyCoord) {
		    if (a == 0) {
			longAxis = i;
		    } else if (a == 1) {
			latAxis = i;
		    }
		}
	    }
	}


	Int stokesCoord = coordsys.findCoordinate(Coordinate::STOKES);
	Int stokesAxis = -1;
	if (stokesCoord >= 0) {
	    for (uInt i=0; i<n; i++) {
		Int c, a;
		coordsys.findWorldAxis(c, a, i);
		if (c == stokesCoord) {
		    stokesAxis = i;
		    break;
		}
	    }
	}

	Int specCoord = coordsys.findCoordinate(Coordinate::SPECTRAL);
	Int specAxis = -1;
	if (specCoord >= 0) {
	    for (uInt i=0; i<n; i++) {
		Int c, a;
		coordsys.findWorldAxis(c, a, i);
		if (c == specCoord) {
		    specAxis = i;
		    break;
		}
	    }
	}

	if (skyCoord >= 0) {
	    Projection::Type t = 
		coordsys.directionCoordinate(skyCoord).projection().type();
	    Int startAt = 1;
	    if (t == Projection::ZPN) {
		startAt = 0;
	    }
	    for (uInt i=0; i<projp.nelements(); i++) {
		projps[startAt + i] = projp(i);
	    }
	}
  

	double * pcmatrix = new double[rank()*rank()];
	for (int j = 0; j < rank(); j++)
	    for (i = 0; i < rank(); i++)
		{
		    pcmatrix[i*rank()+j] = pc(i, j); // This might be the
		    // transpose of what we need!
		}
  
	cs_ = new AvCoordinateSystem(rank(),
				     ctypes,
				     crvals,
				     crpixs,
				     cdelts,
				     pcmatrix,
				     projps,
				     longPole);
  
	// Establish axis units based on FITS conventions.  Add as
	// needed, consulting AvUnits for units and measurement type.
	Regex trailing(" *$"); // Get rid of trailing blanks...
	for (i = 0; i < rank(); i++)  {
	    String unit = downcase(cunit(i)).before(trailing);
	    if (unit == "deg") {
		unit = "degrees";
	    } else if (unit == "rad") {
		unit = "radians";
	    } else if (unit == "hz") {
		unit = "hertz";
	    } else if (unit == "m/s") {
		unit = "meters/second";
	    }

	    if (i == longAxis) {
		cs_->setAxisUnits(i, unit.chars(), "right ascension");
	    } else if (i == latAxis) {
		cs_->setAxisUnits(i, unit.chars(), "declination");
	    } else if (i == stokesAxis) {
		cs_->setAxisUnits(i, "stoke index", "polarization");
	    } else if (i == specAxis && ctype(i).contains("freq")) {
		cs_->setAxisUnits(i, unit.chars(), "frequency");
	    } else if (i == specAxis && ctype(i).contains("felo")) {
		cs_->setAxisUnits(i, unit.chars(), "velocity");
	    } else {
		cs_->setAxisUnits(i, unit.chars(), "");
	    }
	}
	delete [] pcmatrix;
	pcmatrix = 0; // force crash if reused

	// Establish data units (sets dataUnits, dataUnitsAbbrev, and
	// dataMeasurementName).

	dataUnits_ = image_->units().getName().chars();
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

    } catch (AipsError x)
	{
	    delete [] tname;
	    cerr << x.getMesg() << endl;
	    return False;
	}
    
    return TRUE; // NOTREACHED, but make compiler happy
}

void AvAipsDataSet::calcMinMax()
{
  start_timer_if_necessary();
  DebugAssert(shape_.nelements() >= 2, AipsError);

  RO_LatticeIterator<Float> iter(*image_,
		 LatticeStepper(image_->shape(), image_->niceCursorShape()));
  for (iter.reset(); !iter.atEnd(); iter++) {
    const Array<Float> &arrayRef = iter.cursor();
    Bool delaptr;
    const Float *aptr = arrayRef.getStorage(delaptr);
    uInt n = arrayRef.nelements();
    float val;
    for (uInt i=0; i<n; i++) {
      val = aptr[i];
      if (!isNaNf(val)) {
	if (val < dataMin_) dataMin_ = val;
	if (val > dataMax_) dataMax_ = val;
      }
    }
    arrayRef.freeStorage(aptr, delaptr);
  }
  // cerr << "min/max " << dataMin() << " " << dataMax() << endl;
}

const char * AvAipsDataSet::dataUnits() const
{   start_timer_if_necessary();
return dataUnits_.chars(); }
const char * AvAipsDataSet::dataUnitsName() const
{   start_timer_if_necessary();
return dataUnitsName_.chars(); }
const char * AvAipsDataSet::dataMeasurementName() const
{   start_timer_if_necessary();
return dataMeasurementName_.chars(); }

const char * AvAipsDataSet::axisName(int axis) const
{   start_timer_if_necessary();
return cs_->axisName(axis); }
const char * AvAipsDataSet::axisUnitsName(int axis) const
{   start_timer_if_necessary();
return cs_->axisUnits(axis); }
const char * AvAipsDataSet::axisMeasurementName(int axis) const
{   start_timer_if_necessary();
return cs_->axisMeasurementName(axis); }

AvString AvAipsDataSet::axisValueUnits(int axis,
				       double value,
				       int nsigfigs) const
{
  start_timer_if_necessary();
#ifdef AVDSDEBUG 
  cout << "AvFR::axisValueUnitsStr(axis=" << axis << ",value=" 
       << value << ",nsigfigs=" << nsigfigs << endl;
#endif
  return cs_->valueUnitsStr(axis, value, nsigfigs); 
}

double AvAipsDataSet::axisValueInUnits(int axis,
				       double value,
				       const AvString& units)
{
  start_timer_if_necessary();
#ifdef AVDSDEBUG 
  cout << "AvFR::axisValueInUnitsStr(axis=" << axis << ",value=" 
       << value << ",units=" << units << endl;
#endif
  return cs_->valueInUnits(axis, value, units); 
}

Boolean AvAipsDataSet::IJKToWorld_(const AvWPosition &ijk,
				   AvWPosition &world) const
{
  start_timer_if_necessary();
  Boolean retval = cs_->ijk2world(ijk, world);
  return retval;
}

Boolean AvAipsDataSet::WorldToIJK_(const AvWPosition &world,
				   AvWPosition &ijk) const
{
  start_timer_if_necessary();
  return cs_->world2ijk(world, ijk);
}


Boolean AvAipsDataSet::getSlice(float *buf,
			    const AvIPosition& start,
			    const AvIPosition& shape,
			    const AvIPosition& stride)
{
  start_timer_if_necessary();
  try
    {
      if(!validIJK(start))
	{	
	  cerr << "AvAipsDataSet::getSlice() - Invalid start position : " <<
	    start << endl;
	  return 0;
	}

      const uInt ndim = real_shape_.nelements();
      IPosition ip_start(ndim);
      IPosition ip_shape(ndim);
      IPosition ip_stride(ndim);
      for (uInt i=0; i<ndim; i++) {
	  ip_start(i) = start(axis_map_(i));
	  ip_shape(i) = shape(axis_map_(i));
	  ip_stride(i) = stride(axis_map_(i));
      }

#if 1
      // GetSlice Method
      Array<Float> array(ip_shape, buf, SHARE);
      image_->getSlice(array, ip_start, ip_shape, ip_stride);
      uInt n = array.nelements();
      float *last = buf + n;
      float blank = blankingValue();
      for (float *where = buf; where != last; where++) {
	if (isNaNf(*where)) {
	  *where = blank;
	}
      }
#endif

#if 0
      // Lattice Iterator Method
      LatticeStepper stepper(image_->shape(), ip_shape);
      RO_LatticeIterator<Float> iter(*image_, stepper);

      // How to position the iterator directly?????
#endif
    }
  catch (AipsError x)
    {
      cerr << "AvAipsDataSet::getSlice()\n";
      cerr << "start  = " << start << endl;
      cerr << "shape  = " << shape << endl;
      cerr << "stride = " << stride << endl;
      cerr << "------------------------------------\n";
      cerr << x.getMesg();
      cerr.flush();
      abort();
    }
  
  return TRUE;
}

int AvAipsDataSet::getRow(float *buf, const AvIPosition &start, int count,
			  int stride, int axis)
{
  start_timer_if_necessary();
  try
    {
      if(!validIJK(start))
	{	
	  cerr << "AvAipsDataSet::getRow() - Invalid start position : " << start << endl;
	  return 0;
	}
      if(count <= 0)
	{
	  cerr << "AvAipsDataSet::getRow() - Invalid count : " << count << endl;
	  return 0;
	}
      
      if(!validAxis(axis))
	{	
	  cerr << "AvAipsDataSet::getRow() - Invalid axis : " << axis << endl;
	  return 0;
	}
      
      if (stride <= 0)
	{
	  cerr << "AvAipsDataSet::getRow() - Invalid stride " << stride << " set to 1\n";
	  stride = 1;
	}
      
      int nAvail = 1 + (shape_(axis) - start(axis) - 1) / stride;
      if (count > nAvail)
	{ 
	  cerr << "AvResidentDataSet::getRow: only " << nAvail <<
	    " elements available (requested " << count << ").\n";  
	  count = nAvail;
	}

#if 1
      // Row at a time access via getSlice
      IPosition ip_start = AvIPosition2IPosition(start);
      IPosition ip_shape(start.nelements(), 1);
      ip_shape(axis) = nAvail;
      IPosition ip_stride(start.nelements(), 1);
      ip_stride(axis) = stride;

      Array<Float> array(ip_shape, buf, SHARE);
      image_->getSlice(array, ip_start, ip_shape, ip_stride);
      uInt n = array.nelements();
      float *last = buf + n;
      float blank = blankingValue();
      for (float *where = buf; where != last; where++) {
	if (isNaNf(*where)) {
	  *where = blank;
	}
      }
#endif
#if 0
      // Row at a time access with lattice iterator
      IPosition ip_start = AvIPosition2IPosition(start);
      IPosition ip_shape(start.nelements(), 1);
      ip_shape(axis) = nAvail;
      IPosition ip_stride(start.nelements(), 1);
      ip_stride(axis) = stride;

      LatticeStepper stepper(image_->shape(), ip_shape);
      RO_LatticeIterator<Float> iter(*image_, stepper);
#endif
#if 0
      // Pixel-by-pixel access using getAt()
      IPosition ip_start = AvIPosition2IPosition(start);
      for (uInt i = 0; i < nAvail; i++, ip_start(axis) += stride)
	{
	  buf[i] = image_->getAt(p);
	}
#endif

    }
  catch (AipsError x)
    {
      cerr << "AvAipsDataSet::getRow()\n";
      cerr << "start  = " << start << endl;
      cerr << "count  = " << count << endl;
      cerr << "stride = " << stride << endl;
      cerr << "axis   = " << axis << endl;
      cerr << "------------------------------------\n";
      cerr << x.getMesg();
      cerr.flush();
      abort();
    }
  
  // If we get this far, assume that we have everything.
  return count;
}

float AvAipsDataSet::getDatum(const AvIPosition &pos)
{ 
  start_timer_if_necessary();
  try
    {
      if (attached())
	{
	  if (!validIJK(pos))
	    return 0;
	  else {
	    float tmp = image_->getAt(AvIPosition2IPosition(pos));
	    if (isNaNf(tmp)) tmp = blankingValue();
	    return tmp;
	  }
	}
      else
	cerr << "AvAipsDataSet::getDatum() -- not attached\n";
    }
  catch (AipsError x)
    {
      cerr << "AvAipsDataSet::getDatum(" << pos << ")\n";
      cerr.flush();
      cerr << x.getMesg();
      abort();
    }
  
  return blankingValue(); // NOTREACHED, but quiet compiler
}

Boolean AvAipsDataSet::putData(const AvDataSet & newData)
{
  start_timer_if_necessary();
  uInt i;

  // Ok - need to ensure that datasets conform
  if (newData.shape() != shape())
    {
      cerr << "AvAipsDataSet::putData() datasets don't conform\n";
      return False;
    }

  // Ok, now we must copy the new data onto the old data.  Do this by
  // slicing along the last axis and using a buffer.

  IPosition ip_shape = AvIPosition2IPosition(shape());

  uInt ndim = ip_shape.nelements();

  uInt nelements = 1;
  for (i = 0; i < ndim; i++) nelements *= ip_shape(i);

  // just copy the damn thing
  float * arr = new float[nelements];

  if (!arr)
    {
      cerr << "AvAipsDataSet::putData - Not enough memory for putData\n";
      return False;
    }

  Array<Float> array(ip_shape, arr, SHARE);
  
  AvIPosition avi_start(ndim, 0);
  AvIPosition avi_stride(ndim, 1);
  
  AvDataSet * hack1 = &((AvDataSet &) newData);
  hack1->getSlice(arr, avi_start, shape(), avi_stride);
  
  IPosition start(ndim, 0);
  IPosition stride(ndim, 1);
  
  // putSlice returns void, not bool, so must assume it worked
  image_->putSlice(array, start, stride);

  delete [] arr;

  return True;

  // data copied, so now get coordinate system
}

/* Returns true if file maybe a Miriad data set directory. Otherwise false.
*/
bool AvAipsDataSet::isDataSet(const char *filename)
{
  return isAipsPlusPlusFile(filename);
}

static AvDataSetFactoryInfo factoryInfo_ =
	{ &AvAipsDataSet::makeDataSet,
	  &AvAipsDataSet::isDataSet,
	  &AvAipsDataSet::dataSetTypeName
	};

const AvDataSetFactoryInfo *AvAipsDataSet::getFactoryInfo()
{
	return &factoryInfo_;
}


// Open a dataset and return a pointer to the object.
// Returns NULL on failure.
AvDataSet * AvAipsDataSet::makeDataSet()
{  AvDataSet *ds = new AvAipsDataSet();
	return ds;
}
