//# Copyright (C) 1995-2003 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvDataSet.cc,v 19.2 2003/10/01 21:34:54 hravlin Exp $
//
// $Log: AvDataSet.cc,v $
// Revision 19.2  2003/10/01 21:34:54  hravlin
// Get the blanking value from Av::BlankedValue rather than a constant.
//
// Revision 19.1  2003/09/02 21:09:36  hravlin
// Added dataSetTypeName(), isDataSet and other support for factory info.
//
// Revision 19.0  2003/07/16 05:47:55  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:30  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.2  2002/01/22 21:48:23  hravlin
// Changed some unsigned ints to uInt and added a (uInt) cast for addr.resize().
//
// Revision 17.1  2002/01/07 22:47:10  hravlin
// Removed check for __P.
// Included <float.h> to get FLT_MAX
// replaced reference to FLOATMAX to FLT_MAX.
//
// Revision 17.0  2001/11/12 19:42:58  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:04  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:25  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:09:00  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:50  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:07  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:51  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:56:03  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/02/20 20:47:29  hr
// Added setPath call to attach().
//
// Revision 9.0  1997/08/25 21:31:05  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.1  1997/06/11 18:53:18  hr
// Changed 3 arg version of axisUnits to actually return an abbreviation.
//
// Revision 8.0  1997/02/20 03:16:19  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.6  1997/02/10 16:00:32  hr
// __P problem.
//
// Revision 7.5  1997/01/07 22:17:50  hr
// Changed axisValueInUnits() to match usage in AvFitsDataset and
// AvResidentDataset. (3rd arg is AvString rather than char * and function
// is not const).
//
// Revision 7.4  1996/12/12 08:58:54  droberts
// Added place to set the path.
//
// Revision 1.12  1996/10/07 22:17:41  pixton
// corrected misname of base class function valueInUnits->axisValueInUnits
//
// Revision 1.11  1996/10/07  21:47:49  pixton
// renamed valueInUnits to axisValueInUnits
//
// Revision 1.10  1996/09/20  20:19:35  pixton
// Prefixed all classes with Av
//
// Revision 1.9  1996/09/17  16:32:15  hr
// Added start().
//
// Revision 1.8  1996/09/12  18:22:44  pixton
// Changed dataValue to return "Blanked" instead of "Blank"
//
// Revision 1.7  1996/08/16  21:59:57  pixton
// added axisPrintFormatString()
//
// Revision 1.6  1996/08/16  21:18:58  hr
// Changed interface to XYZtoDS and DStoXYZ functions.
// Added call to AvUnits::initClass to constructor.
// Added check for blankingValue to functions that format data values.
//
// Revision 1.5  1996/08/12  21:32:25  pixton
// modified IJKToWorld...
//
// Revision 1.4  1996/08/12  14:49:07  pixton
// Widespread changes to support correct coordinates and labeling.
//
// Revision 1.3  1996/08/01  21:26:12  hr
// Replaced int by unsigned int and changed function return from DATAFORMAT
// to AvDataSet::DATAFORMAT and some other changes to keep g++ happy.
//
// Revision 1.2  1996/07/23 17:24:16  hr
// Removed const from get{Datum,Row,etc.}.
//
// Revision 1.1  1996/07/18 19:27:28  hr
// Initial revision
//
//
//---------------------------------------------------------------------------

/* AvDataSet.cc
Defines interface to an aipsview data set.
*/
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "AvDataSet.h"
#include "AvUnits.h"
#include "Av.h"

AvDataSet::AvDataSet()
{
	AvUnits::initClass();
}

// Attach the dataset to the data. Returns TRUE on success.
// If already open, calls detach first.
// The base class calls attach_ which should be overridded by
// subclasses. If successful, it then invokes the callback list
// to propagate the change notice.
Boolean AvDataSet::attach(const char *name)
{
  if(attached())
    detach();
  if(attach_(name))
  {	setPath(name);
	invokeCallbacks(ATTACHED);
	return True;
  }
  else
    return False;
}

// Returns TRUE if currently attached.
Boolean AvDataSet::attached()const
{	return FALSE;
}

// Ignored if not attached.
void AvDataSet::detach()
{
  if(attached())
    {	
      invokeCallbacks(DISCONNECT);	// Tell everyone to let go.
      detach_();
      invokeCallbacks(DETACHED);	// No one should ever get this.
    }
}

//	Get information about the data itself

// Return the value used to indicate blanked data. Must be a valid
// number and not infinity since it causes problems on some machines.
// (Default is MAX_FLOAT).
float AvDataSet::blankingValue()const
{	return defaultBlankingValue();
}

float	AvDataSet::dataMin()const
{	return 0.0;
}

float	AvDataSet::dataMax()const
{	return 1.0;
}

int	AvDataSet::rank()const
{	return shape().nelements();
}

int	AvDataSet::nelements()const
{	return shape().product();
}


// Virtual
AvString AvDataSet::dataValueUnits(float value, int nsigfigs)
{
  AvString retval = dataValue(value, nsigfigs);
  AvString units = dataUnits();
  if ((units != "") && (value != blankingValue()))
    retval += " " + units;
  return retval;
}

// Route various strings to the virtual function above
AvString AvDataSet::dataValueUnits(const AvIPosition &pos, int nsigfigs)
{ return dataValueUnits(getDatum(pos), nsigfigs); }
#if 0
char * AvDataSet::dataValueUnitsStrDup(const AvIPosition &pos, int nsigfigs)
{ return dataValueUnitsStrDup(getDatum(pos), nsigfigs); }
char * AvDataSet::dataValueUnitsStrDup(float value, int nsigfigs)
{ return Strdup(dataValueUnits(value, nsigfigs).chars()); }
#endif

// Virtual
AvString AvDataSet::dataValue(float value, int nsigfigs)
{
  AvString retval;
  if(value == blankingValue())
	return "Blanked";
  AvUnits::roundToNDigits(value, nsigfigs, retval);
  return retval;
}

AvString AvDataSet::dataValue(const AvIPosition &pos, int nsigfigs)
{ return dataValue(getDatum(pos), nsigfigs); }

#if 0
char * AvDataSet::dataValueStrDup(const AvIPosition &pos, int nsigfigs)
{ return dataValueStrDup(getDatum(pos), nsigfigs); }
char * AvDataSet::dataValueStrDup(float value, int nsigfigs)
{ return Strdup(dataValue(value, nsigfigs).chars()); }
#endif

// units along an axis for a given value
AvString AvDataSet::axisUnits(int axis,
				 double value) const
{ 
  AvString retval = "";
  AvUnits::queryUnits(value, 
		      axisUnitsName(axis),
		      axisMeasurementName(axis),
		      retval);
  return retval;
}

// units along an axis for a given value with abbreviated form
AvString AvDataSet::axisUnits(int axis,
			      double value,
			      AvString & abbrev) const
{ 
    AvString units = axisUnits(axis, value);
    abbrev = AvUnits::abbreviate(units);
    return units;
}

#if 0
char * AvDataSet::axisUnitsStrDup(int axis,
				 double value,
				  char *& abbrev) const
{ 
  AvString abbr;
  retval = Strdup(axisUnits(axis, value, abbrev).chars());
  abbrev = Strdup(abbr.chars());
}
#endif

AvString AvDataSet::axisValueUnits(int /*axis*/,
				   double value,
				   int /*nsigfigs*/) const
{
  char buf[80];
  sprintf(buf, "%6.4g", value);
  return buf;
}

// 


// Data Retrieval

/* Return a single data point from the given position.

Base class returns encoded version of the first 4 positions of the address
to help debug readers.
	return = (float) |t|z|y|x|
Where x, y, z and t represent the first four coordinates (or 0). Each is
stored in a separate byte.
*/

float AvDataSet::getDatum(const AvIPosition &pos)
{
  return encodeDatum(pos);
}


// Fill a buffer with length elements from along axis "axis".
// aipsview will use this, via the accessor, for raster information.
// If stride is negative, data is retrieved from decreasing axis
// coordinates. (eg R to L rather than L to R).
// Returns the number of elements actually copied.
int AvDataSet::getRow(	float *buf,
			const AvIPosition &Start, int Length,
			int stride, int axis)
{
  AvIPosition start(Start);
  int i, length=Length, maxlength;
  
  if ((buf == NULL) || (axis < 0) || (axis >= rank()) || (length <= 0))
    return 0;
  
  maxlength = axisElements( start(axis), axisLength(axis), stride);
  if (length > maxlength)
    length = maxlength;
  
  for (i=0; i< length; i++)
    {	*buf++ = getDatum(start);
    start(axis) += stride;
    }
  return length;
}

// Same as above except Start is in XYZ coordinates.
int AvDataSet::getMappedRow(	float *buf,
				const AvIPosition &Start, int Length,
				int stride, int axis,
				const AvIPosition &map)
{
  // Convert starting position to DS coordinates before
  // passing to getRow.
  AvIPosition myStart(XYZtoDS(Start, map));
//  XYZtoDS(Start, myStart, &map);
  return getRow(buf, myStart, Length, stride, axis);
}

/* Return a region of data. If map is not NULL, start, shape and stride
are in XYZ coords. Otherwise, they are in dataset coordinates.

Axis is the index into the AvIPosition that is currently being operated on.
If axis is 1, getSlice_ repeatedly calls getRow to fill the
buffer. Otherwise, it calls itself to iterate along the various
axes.

 getSlice_ returns a pointer to the next position in the buffer.

axis should start at nelements()-1 and will be reduced by one on each
recursive call.

Protected since it doesn't check for valid parameters.
*/
float *AvDataSet::getSlice_(float *buf, const AvIPosition &start,
			    const AvIPosition &shape, const AvIPosition &stride,
			    int axis, const AvIPosition *map)
{
  // If axis is zero, retrieve current row by calling getRow.
  if(axis == 0)
    {// Convert mapped index to real index we're using.
      int raxis = mappedAxis(axis,map);
      int xnum=shape(axis), xstride=stride(axis);
      // Convert starting position to DS coordinates.
      AvIPosition myStart(XYZtoDS(start, map));

      getRow(buf, myStart, xnum, xstride, raxis);
      buf += xnum;
    }
  else	// Interate over current axis.
    {AvIPosition myStart(start);
    int ynum=shape(axis), ystride=stride(axis);

    for(int i=0; i < ynum; i++)
      {	buf = getSlice_(buf, myStart, shape, stride,
			axis-1, map);
	myStart(axis) += ystride;
      }
    }
  return buf;
}

// Fill an array with data points. Any masked/blanked values
// will be replaced by blankingValue().
// Returns FALSE for illegal parameters.
Boolean AvDataSet::getSlice(float *buf, const AvIPosition &start,
			    const AvIPosition &size,
			    const AvIPosition &stride)
{
  if((buf == NULL) || !validSliceValues(start, size, stride))
    return False;
  getSlice_(buf, start, size, stride, start.nelements()-1, NULL);
  return True;
}

// Store values from the current array into the dataset
Boolean AvDataSet::putData(const AvDataSet & /* newData */)
{
  // default is to not allow
  return False;
}

// Fill an array with data points transposed to match buf's layout.
// map[0] gives the dataset axis corresponding to buf's primary
//axis, map[1] corresponds to the secondary axis, etc.
// eg. buf[0] to buf[shape[map[0]]] receives data from along dataset
// axis map[0].
// Start, shape and stride are in XYZ coordinates.
// If stride is negative, data is retrieved from start towards 0.
// Currently, the only thing that might call this is retrieval of
// the 3D region to pass to GLISH.
Boolean AvDataSet::getMappedSlice(float *buf, const AvIPosition &start,
				  const AvIPosition &shape,
				  const AvIPosition &stride,
				  const AvIPosition &map)
{
  //	Not valid since there're mapped.
  //	if((buf == NULL) || !validSliceValues(start, stride, stride))
  //		return False;

  getSlice_(buf, start, shape, stride, start.nelements()-1, &map);
  return True;
}

// Return a pointer to an array of data points. If needDelete is True,
// caller will need to "delete []" the array.
// Current aipsview does not need this. This is potentially for
// new 3D algorithms.
// If map is NULL, to mapping is done.
float *AvDataSet::getSlicePtr(Boolean &needDelete,
			      const AvIPosition &start,
			      const AvIPosition &shape,
			      const AvIPosition &stride,
			      const AvIPosition *map)
{
  int size = shape.product();
  float *buf = new float[size];
  
  if(!validSliceValues(start, stride, stride))
    return False;
  
  getSlice_(buf, start, shape, stride, start.nelements()-1, map);
  needDelete = True;
  return buf;
}

// Name of dataset.
const char *AvDataSet::name() const
{	return "NoName";
}

// Name of "file", probably w/o path.
const char *AvDataSet::fileName() const
{	return "NoFile";
}

// "Type" of dataset: eg. "AIPS++", "FITS", etc.
const char *AvDataSet::dataSetTypeName()
{	return "NoDataType";
}

const char *AvDataSet::dataSetType()const
{	return AvDataSet::dataSetTypeName();
}

// Type of data as printable string.
const char *AvDataSet::dataName() const
{	return "Value";
}

// Unit information
const char *AvDataSet::dataUnits() const
{	return NULL;
}

const char *AvDataSet::axisName(int axis) const
{
  static char buf[30];
  sprintf(buf, "axisName %d", axis);
  return buf;
}

const char *AvDataSet::axisUnitsName(int axis) const
{
  static char buf[30];
  sprintf(buf, "axisUnitsName %d", axis);
  return buf;
}

double AvDataSet::axisValueInUnits(int, double val, const AvString&)
{
  cerr << "Warning -- AvDataSet::axisValueInUnits() not overridden" << endl;
  return val;
}

const char *AvDataSet::axisMeasurementName(int axis) const
{
  static char buf[40];
  sprintf(buf, "axisMeasurementName %d", axis);
  return buf;
}

// Returns the world coordinates of position 0.
AvWPosition AvDataSet::minWorld() const
{
  int r = rank();
  AvWPosition world(r, 0.0);
  AvWPosition zero(r, 0.0);
  if (!IJKToWorld_(zero, world))
    { cerr << "AvDataSet::minWorld() - IJKToWorld failed\n"; }
#ifdef AVDSDEBUG
  cout << "AvDataSet::minWorld() returning" << world << endl;
#endif
  return world;
}

// Returns the world coordinates of position shape()-1.
AvWPosition AvDataSet::maxWorld()const
{
  int r = rank();
  AvWPosition world(r);
  IJKToWorld(AvIPosition(shape()-1), world);
  return world;
}

// Convert a position between IJK and world coordinates.
// Returns FALSE if the input position is invalid for the dataset.
// Base class scales position so ijkMIN->worldMin, ijkMAX->worldMax.
Boolean	AvDataSet::IJKToWorld_(const AvWPosition &ijk, AvWPosition &world) const
{
  unsigned int ilen;
  
  ilen = ijk.nelements();
  
  if(ilen != world.nelements())
    world.resize(ilen, 0);	// Make same size as world, don't
				// bother copying what's there.
  
  // world = (ijk/(ijkMax-ijkMin))*(worldMax-worldMin) + worldMin.
  // ijkMax-ijkMin = shape() -1 since min is always 0.
  // Have to do conversion manually since shape=1 results in a divide
  // by zero.
  AvWPosition minW(minWorld()), maxW(maxWorld());
  for(unsigned int i=0; i< ilen; i++)
    {
      int s;
      if((s = axisLength(i)-1) <= 0)
	world(i) = minW(i);
      else
	world(i) =
	(float)(ijk(i))*(maxW(i)-minW(i))/s + minW(i);
    }
  return TRUE;
}

// Returns the indices closest to the world value.
Boolean	AvDataSet::WorldToIJK_(const AvWPosition &world, AvWPosition &ijk)const
{
  unsigned int wlen;
  
  // ijk = (world - minWorld)/(maxWorld-minWorld)
  //		*(maxIJK-minIJK) + minIJK
  //		 =(shape()-1) + 0
  wlen = world.nelements();
  if(ijk.nelements() != wlen)
    ijk.resize(wlen, 0);	// Make same size as world, don't
  // bother copying what's there.
  AvWPosition wMax(maxWorld()), wMin(minWorld());
  AvIPosition shp(shape());
  
  for(unsigned int i=0; i< wlen; i++)
    {
      int s;
      if((s = shp(i)-1) <= 0)
	ijk(i) = 0;
      else
	{ 
	  float wM = wMin(i);
	  ijk(i) = (int)((world(i)-wM)*s/(wMax(i)-wM) + 0.5);
	}
    }
  
  return TRUE;
}

// Is the position a valid one for the data set?
Boolean AvDataSet::validIJK(const AvIPosition &position) const
{
  return (position >= 0 && position < shape());
}

Boolean AvDataSet::validWorld(const AvWPosition &position)const
{
#if 1
  AvIPosition ijk(position.nelements());
  if (!WorldToIJK(position, ijk)) return FALSE;
  return validIJK(ijk);
#else
  //
  // World is not a linear coord system --> not possible to do this 
  //
  return ((position >= minWorld()) && (position <= maxWorld()))
    ? True : False;
#endif
}

AvDataSet::AXISPRINTFORMAT AvDataSet::axisPrintFormat(int axis) const
{	
  // Switch on axis measurement name
  if (!strcmp(axisMeasurementName(axis), "right ascension"))
    return HMS;
  else if (!strcmp(axisMeasurementName(axis), "declination"))
    return DMS;
  else
    return DEFAULT;
}

AvString AvDataSet::axisPrintFormatString(int axis) const
{
  switch(axisPrintFormat(axis))
    {
    case HMS:
      return "HMS";
    case DMS:
      return "DMS";
    case DEFAULT:
      break;
    }
  return "DEFAULT";
}

////////////////////////////////////////////////////////////////
//		Axis information
// Return NULL ( or -1 for ints) if axis is illegal.

// We assume origin is at 0.
AvIPosition AvDataSet::start()const
{	return AvIPosition(rank(), 0);
}

AvIPosition AvDataSet::shape()const
{	return AvIPosition(0,0);
}
// Size

// Length of an axis (eg. shape[axis])
int AvDataSet::axisLength(int axis) const
{
  if(validAxis(axis))
    return shape()(axis);
  else
    return -1;
}

// Return the number of values for an axis length long and stride > 0
static inline int numValues(int len, int stride)
{	return (stride >= len) ? 1 :
  len/stride + (((len % stride) != 0) ? 1 : 0);
}

// Utility routine to return the number of elements retrievable
// from an axis of length 'length' given a starting offset and a
// stride. Stride may be < 0. 
int AvDataSet::axisElements(int start,
			    int length, int stride)
{
  if( (length <= 0) || (start < 0) || (start >= length)
      || (stride == 0)
      )
    return 0;

  if(stride > 0)
    return numValues(length-start, stride);
  else
    return numValues(start+1, -stride);
}

// Destructors for subclasses of AvResource should be protected so
// they are not accidentally called explicitly.
// (Note that SvConductor calls attached objects with the Destroyed message).
AvDataSet::~AvDataSet()
{	
  if(attached())
    detach();
}

// These routines actually do the attach/detach and need to be overridden
// by each sub class.
Boolean AvDataSet::attach_(const char * /*filename*/)
{
  return False;
}

void AvDataSet::detach_()
{
}

////////////////////////////////////////////////////////////////
///		Utility/Support Routines.
////////////////////////////////////////////////////////////////


// These routines just call invokeCallbacks with the appropriate
// mask. Subclasses should make sure they call these when changing
// data.
void AvDataSet::disconnect()
{
  invokeCallbacks(DISCONNECT);
}

void AvDataSet::dataChanged()
{
  invokeCallbacks(DATA);
}

// Return a pointer to a copy of the string. If NULLSTRING is TRUE,
// a NULL input will result in a NULL return. Otherwise, an empty
// string is returned.
char *AvDataSet::Strdup(const char *s, const Boolean ns)
{
  if(s == NULL)
    {	if(ns)
      return NULL;
    else
      return Strdup("");
    }

  char *str = new char[strlen(s)+1];
  strcpy(str, s);
  return str;
}

// Convert from XYZ coordinates to DS (data set) coordinates.
// xyz is copied to ds such that ds[map[0]] = xyz[0].
// If map is NULL, conversion is a straight copy.
// The output is forced to be the same size as the input.
AvIPosition AvDataSet::XYZtoDS(const AvIPosition &xyz, const AvIPosition &map)
{ int xyzlen = xyz.nelements(), mlen = map.nelements(), mindex;
  AvIPosition ds(xyzlen);

    if(mlen > xyzlen)
      mlen = xyzlen;		// xyz is shorter than map.
    for(int i=0; i< mlen; i++)
      {	if((mindex = map(i)) >= xyzlen)
		break;		// Bad map index.
	ds(mindex) = xyz(i);
      }
	return ds;
}

AvIPosition AvDataSet::XYZtoDS(const AvIPosition &xyz, const AvIPosition *map)
{

	if(map == NULL)
		return xyz;
	else
		return XYZtoDS(xyz, *map);
}

// Convert from DS coordinates to XYZ (mapped) coordinates.
// ds is copied to xyz such that xyz[0] = ds[map[0]].
// If map is NULL, conversion is a straight copy.
// The output is forced to be the same size as the input.
AvIPosition AvDataSet::DStoXYZ(const AvIPosition &ds, const AvIPosition &map)
{int dslen = ds.nelements(), mlen = map.nelements(), mindex;
 AvIPosition xyz(dslen);
 
    if(mlen > dslen)
      mlen = dslen;		// ds is shorter than map.
    for(int i=0; i< mlen; i++)
      {	if((mindex = map(i)) >= dslen) // Bad map index.
		break;
	xyz(i) = ds(mindex);
    }
	return xyz;
}

AvIPosition AvDataSet::DStoXYZ(const AvIPosition &ds, const AvIPosition *map)
{
	if(map == NULL)
		return ds;
	else
		return DStoXYZ(ds, *map);
}

// Check for slice parameter validity.
Boolean AvDataSet::validSliceValues(const AvIPosition &start,
		    const AvIPosition &shape, const AvIPosition &stride)const
{
  unsigned int nel = start.nelements();

  if( (nel != shape.nelements()) || (nel != stride.nelements()))
    return False;
  if((start < AvIPosition(start.nelements(), 0)) ||
     (start+shape > this->shape()) || (stride == 0))
    return False;
  else
    return True;
}

// Debugging routine.
// Encode an address into a floating point number.
// Each address index is encoded as an 8 bits. Use decodeDatum to convert
// back to an address. Only the first 4 indexes are encoded.
float AvDataSet::encodeDatum(const AvIPosition &address)
{
  uInt val=0, len=address.nelements(), shift, i;
  uInt max = (uInt)sizeof(int);		// # of bytes in an int.
  
  if(max > len)		// Min of the 2.
    max = len;
  for(shift=0, i=0; i< max; i++, shift+=8)
    val |= (address(i) & 0xFF) << shift;
  
  return (float) val;
}

// Debugging routine. Convert a value returned by the base class' getDatum
// function into an address. addr is resized to be 4 long.
AvIPosition AvDataSet::decodeDatum(float value)
{
  unsigned int val = (int)value, i,shift;
  AvIPosition addr;
  
  addr.resize((uInt)sizeof(int), False);
  for(shift=0, i=0; i< sizeof(int); i++, shift += 8)
    addr(i) = (val>>shift) & 0xFF;
  return addr;
}

float AvDataSet::defaultBlankingValue()
{
  return Av::BlankedValue;
}

/* Returns 1 if file is a regular file or a special (ie Miriad or AIPS++
   data set) directory. Otherwise 0.
*/
bool AvDataSet::isDataSet(const char *filename)
{ struct stat s_buf;

	if(filename == 0)
		return false;

	if(stat(filename, &s_buf) == -1)
		return false;		/* Can't stat file. */

	if(S_ISREG(s_buf.st_mode) && (access(filename, R_OK) == 0))
		return true;		/* Regular files are OK. */

	return false;			/* Anything else. */
}
