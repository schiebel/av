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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvAccessor.cc,v 19.0 2003/07/16 05:48:04 aips2adm Exp $
//
// $Log: AvAccessor.cc,v $
// Revision 19.0  2003/07/16 05:48:04  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:38  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.2  2002/01/30 22:28:25  hravlin
// Added base class initializers to copy constructor.
//
// Revision 17.1  2002/01/22 21:23:18  hravlin
// Changed NREASONS from int to unsigned int. (use cast for 64 bit sizeof()).
//
// Revision 17.0  2001/11/12 19:43:06  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:12  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:42  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 18:21:31  hravlin
// Replaced a "char *name" by "const char *name" to avoid compiler warnings.
//
// Revision 14.0  2000/03/23 16:09:09  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:41:00  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:24  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:02:17  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:56:33  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1997/11/14 22:42:57  hr
// Added XYZToWorld(IPosition, WPosition) functions.
//
// Revision 9.0  1997/08/25 21:31:51  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.4  1997/08/01 21:05:29  hr
// Added check for x/y/z less than 0. Changed another routine to do validity
// check after returning values. (In case IJK out of data set range).
//
// Revision 8.3  1997/06/09 20:05:54  hr
// In WorldToXYZ(), was checking for rank >= 2 rather than > 2.
//
// Revision 8.2  1997/05/20 19:32:12  hr
// In dataStart(), was testing for rank >= 2.
//
// Revision 8.1  1997/04/15 20:07:15  hr
// XYZToWorld & WorldToXYZ are being converted to always return converted
// value even if result is not within data cube.
//
// Revision 8.0  1997/02/20 03:14:16  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1996/12/12 05:29:09  droberts
// Final update from monet archive.
//
// Revision 1.11  1996/10/01 21:54:13  hr
// setPlane() sometimes set illegal plane values.
//
// Revision 1.10  1996/09/27 16:47:02  hr
// Added ref/unrefs to dscitem_ and accitem_.
//
// Revision 1.9  1996/09/25 21:25:06  hr
// <xyz>Axis() functions now return the constants <XYZ>AXIS. Internal
// functions <xyz>Axis_() created to return mapped axis indexes.
// Some functions that used to return -1 for an invalid axis now return
// 0 (position) or 1 (length).
// Cleared up how plane_ is handled.
//
// Revision 1.8  1996/09/20 20:19:35  pixton
// Prefixed all classes with Av
//
// Revision 1.7  1996/09/20  18:35:03  hr
// Fixed validXYZ().
//
// Revision 1.6  1996/09/18 18:42:18  hr
// More integration with aipsview. Added reasonToString, validIJK. Other
// changes to make it easier to deal with cube or subrange values.
//
// Revision 1.5  1996/09/10 21:07:43  hr
// Comparision of rank to 2 was wrong.
//
// Revision 1.4  1996/09/10 16:48:20  hr
// Many changes during integration with aipsview.
//
// Revision 1.3  1996/08/30 19:01:24  hr
// More initial development and cleanup.
//
//
//---------------------------------------------------------------------------

/* AvAccessor.cc
Interface to a data set.

*/
#include "AvAccessor.h"

// Create an Accessor for a data set.
AvAccessor::AvAccessor( AvDataSet &ds, const int cbMask)
{
	dsinit(ds, cbMask);
}

// Clone an accessor
AvAccessor::AvAccessor( AvAccessor &ac, const int dsMask, const int acMask):
	  AvResource(), AvConductor()
{
	acinit(ac, dsMask, acMask);
}

	// Subrange
// Start and shape are in DS coordinates.
AvAccessor::AvAccessor( AvDataSet &ds, const AvIPosition &start,
		     const AvIPosition &shape, const int cbMask)
{
	dsinit(ds, cbMask);
	start_ = start;
	shape_ = shape;
}

// Make a subrange from an existing Accessor. Start and Shape are in XYZ
// coordinates. Shape is relative to the existing Accessor's start.
AvAccessor::AvAccessor( AvAccessor &ac, const AvIPosition &start,
		  const AvIPosition &shape,
		  const int dsMask, const int acMask)
{
	acinit(ac, dsMask, acMask);
	start_ += XYZtoDS(start);	// New start after converting to DS.
	if(! ds_->validIJK(start_))
	{	errlog("AvAccessor", "Start position is invalid: ", start);
		start_ -= start;
	}
	shape_ = XYZtoDS(shape);
}

// Initialize with information from the data set.
void AvAccessor::dsinit(AvDataSet &ds, const int dscbMask)
{	ds_ = &ds;
	ds_->ref();

	// Add this to the data set's callback list.
	dscitem_ = new AvConductorItem( &AvAccessor::notifyCB, dscbMask,
			(XtPointer) this, NULL);
	dscitem_->ref();
	ds.addCallbackItem(dscitem_);
	ac_ = NULL;
	accitem_ = NULL;

	int rank = ds_->rank();
	start_.resize(rank);
	start_ = 0;
	dataStart_ = start_;
	one_.resize(rank);
	one_ = 1;
	shape_ = ds_->shape();
	clipmin_ = ds_->dataMin();
	clipmax_ = ds_->dataMax();
	map_.resize(rank);
	// Default to no mapping.
	for(int i=0; i< (int)map_.nelements(); i++)
		map_(i) = i;
	plane_ = getAxisLength(zAxis())/2; // Default to center plane.
}

// Initialize with information from an Accessor and its data set.
void AvAccessor::acinit(AvAccessor &ac, const int dscbMask, const int accbMask)

{
	dsinit( *ac.dataset(), dscbMask);

	// A pointer the the  accessor is kept so we can copy changed
	// data (clipMinMax) when changes occur.
	ac_ = (ac.ac_ != NULL) ? ac.ac_ : &ac;
	ac_->ref();

	accitem_= new AvConductorItem( &AvAccessor::notifyCB,
				accbMask, (XtPointer) this, NULL);
	accitem_->ref();
	ac.addCallbackItem(accitem_);

	clipmin_ = ac.clipmin_;
	clipmax_ = ac.clipmax_;
	start_ = ac.start_;
	shape_ = ac.shape_;
	map_ = ac.map_;
	dataStart_ = ac.dataStart_;
	plane_ = ac.plane_;
}

AvAccessor::~AvAccessor()
{
	// Tell connected objects to disconnect.
	disconnect();
	// Disconnect from objects we're connected to.
	if(dscitem_ != NULL)
	{	dscitem_->remove();
		dscitem_->unref();
		dscitem_ = NULL;
	}
	if(accitem_ != NULL)
	{	accitem_->remove();
		accitem_->unref();
		accitem_ = NULL;
	}
	ds_->unref();
	if(ac_ != NULL)
		ac_->unref();
	// The destructor for AvConductor sends out the destroyed event.
}

// Change the current mapping.
void AvAccessor::map(const AvIPosition &m)
{
	if(map_ != m)
	{	map_ = m;
		regionChanged();
	}
}

// Removes mapping. (xAxis() is DS axis 0).
void AvAccessor::unMap()
{ int	r = ds_->rank();

	map_.resize(r);		// Just in case.
	for(int i=0; i < r; i++)
		map_(i) = i;
	regionChanged();
}

// Use data set axis 'dsAxis' as XYZ axis 'xyzAxis'.
// Both must be in the range 0..rank()-1.
void AvAccessor::setAxisMapping( const int xyzAxis, const int dsAxis)
{
	if(!validAxis(dsAxis))
	{	errlog("setAxisMapping", "Bad DS axis ", dsAxis);
		return;
	}
	if(!validAxis(xyzAxis))
	{	errlog("setAxisMapping", "Bad XYZ axis ", xyzAxis);
		return;
	}
	map_(xyzAxis) = dsAxis;
	regionChanged();
}

//			 Set start position.

// The reason for the variety of start & shape function names is to keep
// from having ambiguous functions.

// Set the starting position of the region, converting to DS coords if
// necessary.
void AvAccessor::setStart(const AvIPosition &S, const Boolean mapped)
{
	if(S.nelements() != start_.nelements())
	{	errlog("setStart", "Start has wrong length: ", S.nelements());
		return;
	}
	AvIPosition s = mapped ? XYZtoDS(S) : S;

	if(start_ != s)
	{	// Make sure the new position is valid.
		// The odd phrasing is because logical operations on
		// IPositions must all succeed for the operation to succeed.
		if( !(s >= 0) || !(s < ds_->shape()))
		{	errlog("setStart", "Invalid start position: ", s);
			return;
		}
		start_ = s;
		dataStart_ = s;
		// Make sure shape is still valid.
		int r = rank();
		for(int i=0; i< r; i++)
			if((shape_(i) + s(i)) > ds_->axisLength(i))
				shape_(i) = ds_->axisLength(i);
		regionChanged();
	}
}

// This allows changing just one axis.
// The primary reason for this, and the corresponding setShape, is
// to allow for setting a start and end plane.
void AvAccessor::setAxisStart(const int axis, const int begin,
			      const Boolean mapped)
{
int	dsAxis;

	dsAxis = getAxisMapping(axis, mapped);

	if(!validAxis(dsAxis) || !validDSIndex_(begin, dsAxis))
		return;
	start_(dsAxis) = begin;
	// Make sure axis length is still valid.
	if((begin + shape_(dsAxis)) > ds_->axisLength(dsAxis))
		shape_(dsAxis) = ds_->axisLength(dsAxis) - begin;

	regionChanged();
}

// Return starting offset for an axis. Returns 0 if axis is illegal.
#if 0
int AvAccessor::getAxisStart(const int axis, const Boolean mapped)const
{
	int dsAxis = getAxisMapping(axis, mapped);
	return validAxis(dsAxis) ? start_(dsAxis) : -1;
}
#endif

//		Set region size
void AvAccessor::setShape(const AvIPosition &Shape, const Boolean mapped)
{
	if(Shape.nelements() != shape_.nelements())
	{	errlog("setShape", "Shape wrong size: ", Shape.nelements());
		return;
	}
	if(mapped)
		setShape(XYZtoDS(Shape), FALSE);
	else
	if(shape_ != Shape)
		if((Shape > 0) && (Shape + start_ <= ds_->shape()))
		{	shape_ = Shape;
			regionChanged();
		}
		else
			errlog("setShape", "Bad shape parameter: ", Shape);
}

// Set the length of one of the region's axis.
void AvAccessor::setAxisLength( const int axis, const int length,
				const Boolean mapped)
{
	int dsAxis = getAxisMapping(axis, mapped);

	if(validAxis(dsAxis) && validDSIndex_(length-1, dsAxis) &&
	   (length != shape_(dsAxis)))
	{	shape_(dsAxis) = length;
		regionChanged();
	}
}

// Return the length of one of the region's axes.
int AvAccessor::getAxisLength(const int axis, const Boolean mapped)const
{
	int dsAxis = getAxisMapping(axis, mapped);
	return validAxis(dsAxis) ? shape_(dsAxis) : 1;
}

// Change the current plane.
// Plane can be set to any plane within the SR.
void AvAccessor::setPlane( const int plane, const Boolean SR)
{ int slice = plane;
  int start = getAxisStart(zAxis());

	if(!SR)	// Convert from DS to SR;
		slice -= start;
	// Range check.
	if((slice < 0) || (slice > getAxisLength(zAxis())))
	{	cerr	<< "AvAccessor::setPlane: Invalid plane: "
			<< plane << endl;
		return;
	}
	// Convert to DS
	slice += start;
	if(slice != plane_)
	{	plane_ = slice;
		planeChanged();
	}
}

// Return value at location within region.
float AvAccessor::getDatum(const int x, const int y, const int z)
{
	dataStart(x, y, z);
	return ds_->getDatum(dataStart_);
}

// Return a data value from a position. If not mapped, position is in DS
// coordinates. Otherwise, it is in XYZ coordinates. In either case, it
// is relative to current start value.
float AvAccessor::getDatum(const AvIPosition &position,
			   const Boolean mapped)const
{
	AvIPosition pos = mapped ? XYZtoDS(position) : position;
	pos += start_;
	return ds_->getDatum(pos);
}

// Get a (sub) row along the X axis starting at x/y in the current plane.
// Returns the number of elements actually copied.
int AvAccessor::getRow(float *data, const int x, const int y,
			   const int len, const int stride)
{
	return getRow(data, x, y, getPlane(), len, stride, XAXIS);
}

// Get a row of data starting at an arbitrary XYZ point and going
// along one of the XYZ axes. Used for profiles (usually along
// ZAXIS).
// Returns the number of elements actually copied.
int AvAccessor::getRow( float *data,
			const int x0, const int y0, const int z0,
			const int len, const int stride, const int xyzAxis)
{
int	dsAxis = getAxisMapping(xyzAxis);

	if(!validAxis(dsAxis))
		return -1;
	if(data == NULL)
		return -1;
	dataStart(x0, y0, z0);
	return ds_->getRow(data, dataStart_, len, stride, dsAxis);
}

// Presumably called by region.

Boolean AvAccessor::getSlice(float *buf,
			     const AvIPosition Start, const AvIPosition Shape,
			     const AvIPosition Stride, const Boolean mapped)
{
	if(!mapped)
		return ds_->getSlice(buf, Start, Shape, Stride);
	else // Convert to DS coordinates first.
	{ AvIPosition start = start_ + XYZtoDS(Start);
	  AvIPosition shape = XYZtoDS(Shape);
	  AvIPosition stride = XYZtoDS(Stride);
		return ds_->getSlice(buf, start, shape, stride);
	}
}

// Set new data clip values. Ignored if no change otherwise
// invokes callbacks.
void AvAccessor::setClipMinMax(const float min, const float max)
{
	if((clipmin_ != min) || (clipmax_ != max))
	{	clipmin_ = min;
		clipmax_ = max;
		clipChanged();
	}
}

//	Coordinate information.

#if 0
Boolean AvAccessor::XYZToDS(const int x, const int y, const int z, AvIPosition& ds)
{
  dataStart(x,y,z);
  ds = dataStart_;
  return True;
}
#endif

#if 0
AvIPosition AvAccessor::XYZToDS(const int x, const int y, const int z)
{
  AvIPosition ds;
  XYZToDS(x,y,z,ds);
  return ds;
}
#endif

#if 0
Boolean AvAccessor::DSToXYZ(const AvIPosition& ds, int & x, int & y, int & z)
{
  AvIPosition xyz = AvDataSet::DStoXYZ(ds, map_);
  x = xyz(0);
  y = xyz(1);
  if(rank() > 2)
	z = xyz(2);
  return TRUE;
}
#endif

#if 0
AvIPosition AvAccessor::DSToXYZ(const AvIPosition& ds)
{
  return AvDataSet::DStoXYZ(ds, map_);
}
#endif

Boolean AvAccessor::XYZToWorld( const AvIPosition& xyz, AvWPosition& world,
				const Boolean SR)const
{ Boolean rtn;
  AvIPosition pos(XYZtoDS(xyz));	// Unmap.

	if(SR)
		pos += start_;
	rtn = ds_->IJKToWorld(pos, world);
	world = mapWorld(world);
	return rtn;
}

AvWPosition AvAccessor::XYZToWorld(const AvIPosition& xyz,
				   const Boolean SR)const
{ AvWPosition world;
  AvIPosition pos(XYZtoDS(xyz));	// Unmap.

	if(SR)
		pos += start_;
	ds_->IJKToWorld(pos, world);
	return mapWorld(world);
}

// Convert between XYZ (mapped) and (mapped) world coordinates.
// Returns TRUE if the input position is valid for the dataset.
Boolean	AvAccessor::XYZToWorld(const int x, const int y, const int z,
		   AvWPosition &world)
{
	dataStart(x,y,z);	// Add offset & do unmapping.
				// (Also picks up any other dimensions).
	AvWPosition wp;
	Boolean rtn = ds_->IJKToWorld(dataStart_, wp);
	world = mapWorld(wp);
	return rtn;
}

AvWPosition AvAccessor::XYZToWorld(const int x, const int y, const int z)
{
  dataStart(x,y,z);			// Add offset & do unmapping.
				// (Also picks up any other dimensions).
  AvWPosition world;
  ds_->IJKToWorld(dataStart_, world);	// world is unmapped.
  return mapWorld(world);		// Map it.
}

// Obtain the value along a single axis in world coordinates for an XYZ
// position.
Boolean	AvAccessor::XYZToWorld(const int x, const int y, const int z,
		   const int xyzaxis, double &value)
{ AvWPosition w;

#if 0
	if(validAxis(xyzaxis) && XYZToWorld(x, y, z, w))
	{	value = w(xyzaxis);
		return TRUE;
	}
	else
		return FALSE;
#else
	Boolean rtn = XYZToWorld(x, y, z, w);
	if(validAxis(xyzaxis))
	{	value = w(xyzaxis);
		return rtn;
	}
	else
		return FALSE;
#endif
}

// Convert a mapped world vector into XYZ values.
Boolean AvAccessor::WorldToXYZ( const AvWPosition& world,
				int & x, int & y, int & z)
{ AvWPosition wp = unMapWorld(world);
  AvIPosition ijk;
  Boolean rtn = ds_->WorldToIJK(wp, ijk);

	x = ijk(xAxis_()) - start_(xAxis_());
	y = ijk(yAxis_()) - start_(yAxis_());
	if(rank() > 2)
		z = ijk(zAxis_()) - start_(zAxis_());
	else
		z = 0;

	if((x>=width())||(y>=height())||(z >= depth()) ||
	   (x< 0) || ( y< 0 ) || (z < 0))
		rtn = FALSE;

	return rtn;
}

// Convert a mapped world vector into XYZ values.
// Returns TRUE if XYZ are in range, FALSE if any value is outside of range.
Boolean AvAccessor::WorldToXYZ( const AvWPosition& world, AvIPosition &xyz)
{ AvWPosition wp = unMapWorld(world);
  AvIPosition ijk;
  Boolean rtn = ds_->WorldToIJK(wp, ijk);

	ijk -= start_;		// Convert to subrange.
	xyz = AvDataSet::DStoXYZ(ijk, map_);	// Remap
	if(rtn && (ijk >= 0) && (ijk < shape_))	// Check for in range.
		return TRUE;
	else
		return FALSE;
}

// Is the position a valid one for the data set?
// x, y, z are mapped, but refer to the data cube, not the subregion.
Boolean AvAccessor::validIJK(const int x, const int y, const int z)
{
	dataStart(x, y, z, FALSE);
	return ds_->validIJK(dataStart_);
}

Boolean AvAccessor::validIJK(const int x, const int y)
{
	return validIJK(x, y, 0);
}

// World position of the corners of the subregion.
AvWPosition AvAccessor::minRegion()const
{AvWPosition wp;

	ds_->IJKToWorld(start_, wp);
	return mapWorld(wp);
}

AvWPosition AvAccessor::maxRegion()const
{AvWPosition wp;
 AvIPosition end = start_ + shape_ - 1;

	ds_->IJKToWorld(end, wp);
	return mapWorld(wp);
}

// Sets dataStart with start_ + <x,y,z>.
// (dataStart_ is in DS coords).
void AvAccessor::dataStart(const int x, const int y, const int z,
			   const Boolean subregion)
{
  int xaxis = xAxis_(), yaxis = yAxis_(), zaxis = zAxis_();
  Boolean haveZ = (rank() > 2);

	// It is assumed that the data set will check the validity of
	// the position.

	dataStart_(xaxis) = x;
	dataStart_(yaxis) = y;
	if(haveZ)
		dataStart_(zaxis) = z;

	if(subregion)
	{	dataStart_(xaxis) += start_(xaxis);
		dataStart_(yaxis) += start_(yaxis);
		if(haveZ)
			dataStart_(zaxis) += start_(zaxis);
	}
	// The rest of dataStart_ (if any) only changes when start_ changes.
}

// Error logging routines.
void AvAccessor::errlog(const char *funcname, const char *msg,
			     const char *param)const
{
  cerr << "AvAccessor::" << funcname << ": ";
  if(ds_->fileName()!= NULL)
    cerr << "(" << ds_->fileName() << ") ";
  cerr << msg;
  if(param != NULL)
    cerr << param << endl;
  else
    cerr << endl;
} 

void AvAccessor::errlog(const char *funcname, const char *msg,
			     int param)const
{
  cerr << "AvAccessor::" << funcname << ": ";
  if(ds_->fileName()!= NULL)
    cerr << "(" << ds_->fileName() << ") ";
  cerr << msg << param << endl;
}

void AvAccessor::errlog(const char *funcname, const char *msg,
			     const AvIPosition &param)const
{
  cerr << "AvAccessor::" << funcname << ": ";
  if(ds_->fileName()!= NULL)
    cerr << "(" << ds_->fileName() << ") ";
  cerr << msg << param << endl;
}

void AvAccessor::regionChanged()
{
	invokeCallbacks(REGION);
}

void AvAccessor::clipChanged()
{
	invokeCallbacks(CLIP);
}

void AvAccessor::planeChanged()
{
	invokeCallbacks(PLANE);
}

// Accessor has received notification that something in the dataset or
// the base accessor has changed. Update any local fields necessary.
// Callbacks are disabled until after all changes have been made.
// Removes any masks that are handled and returns what's left.
int AvAccessor::notify(const int Reason, const Boolean ds)
{ int reason = Reason;

	// The disable is done so, should more events be monitored, only
	// one call to invokeCallbacks will be made.
	disableNotify();

	if(reason & CLIP)
	{	reason &= ~CLIP;	// Remove the reason.
		setClipMinMax(ac_->clipMin(), ac_->clipMax());
	}

	// The reason isn't removed here since it needs to be propagated.
	if((reason & DISCONNECT) || (reason & Destroyed))
	{	if(ds)
		{	if(dscitem_ != NULL)
				dscitem_->remove();
			AvResource::UnRef(ds_);
			ds_ = NULL;
		}
		else
		if(accitem_ != NULL)
		{	accitem_->remove();
			AvResource::UnRef(ac_);
			ac_ = NULL;
		}
	}

	// Should check for data set reconfiguration.
	enableNotify();		// Enable notification & call callbacks.
	return reason;
}

void AvAccessor::notifyCB(XtPointer obj, AvConductor *c, XtPointer )
{ AvAccessor *me = (AvAccessor *) obj;
  int reason = c->callbackReason();

	if(me != NULL)
	{	reason = me->notify(reason, (c == me->ds_) ? TRUE : FALSE);
		// Pass anything left on.
		me->invokeCallbacks(reason);
	}
}

// Utility to return the number of elements available along an
// axis from a given starting position.
int AvAccessor::axisElements(int Start, int stride,
			     const int axis, const Boolean mapped)
{
	// Convert to ds coordinates.
	int dsAxis = getAxisMapping(axis, mapped);
	if(dsAxis < 0)
		return -1;
	int start = Start + start_(dsAxis);
	int length = shape_(dsAxis);
	return ds_->axisElements(start, length, stride);
}

// Convert from mapped to unmapped World coordinates.
// xyz is copied to world such that world[map[0]] = xyz[0].
// The output is forced to be the same size as the input.
AvWPosition AvAccessor::unMapWorld(const AvWPosition &xyz)const
{ int xyzlen = xyz.nelements(), mlen = map_.nelements(), mindex;
  AvWPosition world(xyzlen);

	if(mlen > xyzlen)
		mlen = xyzlen;		// xyz is shorter than map.
	for(int i=0; i< mlen; i++)
	{	if((mindex = map_(i)) >= xyzlen)
		break;		// Bad map index.
		world(mindex) = xyz(i);
	}
	return world;
}

// Convert from unmapped to mapped world coordinates.
// ds is copied to xyz such that xyz[0] = world[map[0]].
// The output is forced to be the same size as the input.
AvWPosition AvAccessor::mapWorld(const AvWPosition &ds)const
{int worldlen = ds.nelements(), mlen = map_.nelements(), mindex;
 AvWPosition xyz(worldlen);
 
	if(mlen > worldlen)
		mlen = worldlen;		// world is shorter than map.
	for(int i=0; i< mlen; i++)
	{	if((mindex = map_(i)) >= worldlen) // Bad map index.
		break;
		xyz(i) = ds(mindex);
	}
	return xyz;
}

// Tell anything connected that they should disconnect now.
void AvAccessor::disconnect()
{
	invokeCallbacks(DISCONNECT);
}

// Return the number of XYZ dimensions that have length > 1.
// Only checks first 3 dimensions.
// 
int AvAccessor::effectiveRank()const
{ int er=0, r;

	r = (rank() <= 3) ? rank() : 3;

	for(int i=0; i < r; i++)
		if(getAxisLength(i) > 1) er++;
	return er;
}

static struct reasons {
	const char *name;
	int	reason;
	} reasons[] = {
		{ "NoReason", AvDataSet::NoReason},
		{ "Destroyed",AvDataSet::Destroyed},
		{ "DISCONNECT",AvDataSet::DISCONNECT},
		{ "ATTACHED", AvDataSet::ATTACHED},
		{ "DETACHED",AvDataSet::DETACHED},
		{ "DATA", AvDataSet::DATA},
		{ "CLIP", AvAccessor::CLIP},
		{ "REGION", AvAccessor::REGION},
		{ "PLANE", AvAccessor::PLANE}
	};
static const unsigned int NREASONS =
			(unsigned int)(sizeof(reasons)/sizeof(*reasons));

// Convert a callback reason mask to a string.
AvString AvAccessor::reasonToString(const int reason)
{ AvString s;

	for(unsigned int j=0; j< NREASONS; j++)
		if( (reason & reasons[j].reason))
		{	if(s.length() > 0)
				s += " | ";
			s += reasons[j].name;
		}
	return s;
}
