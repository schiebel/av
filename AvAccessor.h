//# Copyright (C) 1995-97 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvAccessor.h,v 19.0 2003/07/16 05:46:37 aips2adm Exp $
//
// $Log: AvAccessor.h,v $
// Revision 19.0  2003/07/16 05:46:37  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:24  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:41:52  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:03  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:14  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:07:38  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:21  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:31  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:08  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:51:43  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.2  1997/12/17 21:48:41  hr
// Changes to remove warnings from MIPSpro.7x compiler.
//
// Revision 9.1  1997/11/14 22:44:32  hr
// Added XYZToWorld(IPosition, WPosition) functions.
//
// Revision 9.0  1997/08/25 21:24:16  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:14:12  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.4  1996/12/12 08:43:57  droberts
// Tweaked to work with AIPS++ native images.
//
// Revision 1.9  1996/10/07 22:16:41  pixton
// made valueInUnits() call AvDataSet::axisValueInUnits()
//
// Revision 1.8  1996/09/25  21:25:06  hr
// <xyz>Axis() functions now return the constants <XYZ>AXIS. Internal
// functions <xyz>Axis_() created to return mapped axis indexes.
// Some functions that used to return -1 for an invalid axis now return
// 0 (position) or 1 (length).
// Cleared up how plane_ is handled.
//
// Revision 1.7  1996/09/20 20:19:35  pixton
// Prefixed all classes with Av
//
// Revision 1.6  1996/09/20  18:35:03  hr
// Fixed validXYZ().
//
// Revision 1.5  1996/09/18 18:42:18  hr
// More integration with aipsview. Added reasonToString, validIJK. Other
// changes to make it easier to deal with cube or subrange values.
//
// Revision 1.4  1996/09/10 16:48:20  hr
// Many changes during integration with aipsview.
//
// Revision 1.3  1996/08/30 19:01:24  hr
// More initial development and cleanup.
//
//
//---------------------------------------------------------------------------

#ifndef AvACCESSOR_H
#define AvACCESSOR_H

#include <AvDataSet.h>

/* AvAccessor.h

Class to interface to a dataset.

It keeps track of subrange selection, current plane, mapping of the
dataset's axes into aipsview's X, Y and Z axis, etc.

Below, dsAxis refers to the dataset axis (0..rank-1). x/y/zAxis refers to
0, 1, 2 which are mapped into the dataset's axis.

The accessor is subclassed from SvConductor so objects using it will be
notified when settings change. (eg. clipmin/max).

The cbMask is used to determine which events the object is interested in.
An accessor will receive all of the data set's events and, if created from
another accessor, the data clip events of the first accessor created for
the dataset. If a 'parent' accessor changes its configuration (e.g. shape,
etc), 'children' won't care. Paying attention to data clipping allows a
single accessor to handle it.

There are at least 3 coordinates systems here.
	IJK	IJK indices into the dataset. They are refered to as "DS"
		here.
	World	World values for the coordinates into the dataset.
	XYZ	aipsview's view of the dataset. The X axis is displayed
		across the image window. The Y axis goes vertically, and
		the Z axis goes into the window (for animations). These
		three may be mapped onto any of the dataset's axis.
*/

class AvAccessor : public AvResource, public AvConductor
{
  public:
	// CLIP		Clip min/max has changed.
	// REGION	Region has changed (start, size, stride, mapping).
	// PLANE	The current plane has changed.
	//	These are extensions of AvDataSet's events.
	enum {	NoReason=AvConductor::NoReason,
		Destroyed=AvConductor::Destroyed,
		DISCONNECT=AvDataSet::DISCONNECT,
		CLIP=AvDataSet::DSFENCE<<1, REGION=CLIP<<1, PLANE=REGION<<1,
		DEFAULTMASK= DISCONNECT|Destroyed|CLIP
	     };
	// Create an Accessor from a data set.
	AvAccessor( AvDataSet &, const int cbMask=AvDataSet::DEFAULTMASK);
	// Clone an Accessor.
	AvAccessor( AvAccessor &, const int dsMask=AvDataSet::DEFAULTMASK,
		  const int acMask=DEFAULTMASK);
	// Subrange start and shape are in DS coords.
	AvAccessor( AvDataSet &, const AvIPosition &startDS,
		  const AvIPosition &shapeDS,
		  const int cbMask=AvDataSet::DEFAULTMASK);
	// Subrange start and shape are in XYZ coords.
	AvAccessor( AvAccessor &, const AvIPosition &startXYZ,
		  const AvIPosition &shapeXYZ,
		  const int dsMask=AvDataSet::DEFAULTMASK,
		  const int acMask=DEFAULTMASK);

	inline int rank()const{return ds_->rank();}
	// Return the number of XYZ dimensions that have length > 1.
	// (Only checks first 3).
	int effectiveRank()const;

	inline Boolean validAxis(const int axis)const
		{return ((axis >= 0) && (axis < rank()))
			? TRUE : FALSE;}

	// Aipsview wants to look at the data as though the X axis goes
	// across the screen, Y up the screen and Z into the screen. The
	// accessor supports mapping of the cube's axis to handle this.
	// dsAxis is a dataset axis.

	enum XYZAXES {XAXIS=0, YAXIS=1, ZAXIS=2};

	// Virtually all outside accesses want to deal with mapped values.
	// These routines return the indexes of the X, Y and Z mapped axes.
	 int xAxis()const{return XAXIS;}
	 int yAxis()const{return YAXIS;}
	 int zAxis()const{return rank()> 2 ? ZAXIS : -1;}

	// Set the axis mapping. If more than one axis will be changed,
	// it would be well to wrap disableNotify/enableNotify around
	// the calls.
	void setAxisMapping( const int xyzAxis, const int DSaxis);
	 void setXaxisMapping(const int dsAxis){setAxisMapping(XAXIS, dsAxis);}
	 void setYaxisMapping(const int dsAxis){setAxisMapping(YAXIS, dsAxis);}
	 void setZaxisMapping(const int dsAxis){setAxisMapping(ZAXIS, dsAxis);}

	// Set/Get the entire mapping array.
	void map(const AvIPosition &newMap);	// Must be of length rank().
	AvIPosition map()const{return map_;}
	// Removes mapping. (xAxis_() is DS axis 0, etc.).
	void unMap();

	// Return the dataset axis used as aipsview axis which may or may
	// not be mapped.
	// Returns -1 for invalid axis values.
	inline int getAxisMapping(const int axis,
				  const Boolean mapped=TRUE)const
				 { if(validAxis(axis))
					return mapped ? map_(axis) : axis;
				   else return -1;
				 }

			// Define a Sub range (slice)
	// Start is in XYZ or DS coords.
	// Valid starting position is anywhere in the data set, not just
	// any currently defined region.
	// If the new starting position invalidates the currently set shape,
	// the shape is quietly reduced.
	AvIPosition start(const Boolean mapped=TRUE)const
		{return mapped ? DStoXYZ(start_) : start_;}
	void start(const AvIPosition &s, const Boolean mapped)
		{setStart(s, mapped);}

	// If mapped is TRUE, the AvIPosition is interpreted as XYZ.
	// If mapped is FALSE, the AvIPosition is used directly (DS).
	void setStart(const AvIPosition &start, const Boolean mapped=TRUE);

	// This allows changing just one axis.
	// The primary reason for this, and the corresponding setShape, is
	// to allow for setting a start and end plane.
	// If the new start value will make the current length invalid,
	// the length is silently reduced to a valid value. Reducing start
	// does not affect the length.
	void setAxisStart(const int axis, const int start,
			const Boolean mapped=TRUE);

	// Return starting offset for an axis. Returns 0 if axis is illegal.
	int getAxisStart(const int axis, const Boolean mapped=TRUE)const
		{ int dsAxis = getAxisMapping(axis, mapped);
			return validAxis(dsAxis) ? start_(dsAxis) : 0;
		}

	// Shape in XYZ or DS coords.
	AvIPosition shape(const Boolean mapped=TRUE)const
		{return mapped ? DStoXYZ(shape_) : shape_;}
	void shape(AvIPosition &s, const Boolean mapped)
		  { setShape(s, mapped);}

	// Set/Return the region's shape in either DS or XYZ coordinates.
	void setShape(const AvIPosition &shape, const Boolean mapped=TRUE);

	// Set length of a particular XYZ axis.
	void setAxisLength(const int axis, const int length,
			   const Boolean mapped=TRUE);

	// Length of subregion
	// zlen is set to 1 for 2D objects.
	int getAxisLength(const int axis, const Boolean mapped=TRUE)const;
	// Length of data set axis.
	int getDSAxisLength(const int axis, const Boolean mapped=TRUE)const
		{return ds_->axisLength(getAxisMapping(axis, mapped));}

	// Return number of entries along the X, Y and Z axes.
	inline int width()const { return shape_(xAxis_()); }
	int height()const { return shape_(yAxis_()); }
	// Depth returns 1 for 2D data sets, otherwise the depth of
	// the currently set region.
	int depth()const { const int axis = zAxis_();
			   if(axis >= 0)
				return shape_(axis);
			   else
				return 1;	// 2D data set.
			 }

	// Aipsview operates on planes of data so it makes sense to make
	// it easy to get pieces of a plane.
	// The current plane is used for data requests that don't include
	// the Z axis. Values can range from 0 to depth()-1.
	// Return can be either DS or subrange relative.
	// Note: If the mapping changes, plane may no longer be valid.
	void setPlane( const int planenum, const Boolean SR=TRUE);
	int getPlane(const Boolean SR=TRUE)const
		{ return SR ? plane_ - getAxisStart(ZAXIS) : plane_;}

	//		Data Access

	float blankingValue()const{return ds_->blankingValue();}

	// Return a data value from a position. If not mapped, position is
	//in DS coordinates. Otherwise, it is in XYZ coordinates. In either
	// case, it is relative to current start value.
	float getDatum(const AvIPosition &pos, const Boolean mapped=TRUE)const;

	// Return value at location within region.
	float getDatum(const int x, const int y, const int z);
	// Use current z plane.
	// (This is likely to get called very often during mouse tracking).
	float getDatum(const int x, const int y)
		{return getDatum(x, y, getPlane());}

	// Get a (sub) row along the X axis starting at x/y in the current
	// plane.
	// This is how the Raster class retrieves its information.
	// Returns the number of elements actually copied.
	int getRow(float *data, const int x, const int y, const int len,
		   const int stride);

	// Get a row of data starting at an arbitrary XYZ point and going
	// along one of the XYZ axes. Used for profiles (usually
	// along ZAXIS). Returns the number of data points actually
	// retrieved.
	int getRow(float *data, const int x0, const int y0, const int z0,
		    const int length, const int stride, const int XYZAxis);

	// If mapped is TRUE, start, shape
	// and stride are mapped into data set coordinates and
	// start is added to the current accessor starting position.
	// Presumably called by Aipsview's region retrieval routine.
	Boolean getSlice(float *buf,
			 const AvIPosition start, const AvIPosition shape,
			 const AvIPosition stride, const Boolean mapped=TRUE
			);

	//	Data clipping
	float clipMin()const{return clipmin_;}
	float clipMax()const{return clipmax_;}
	// Set new data clip values. Ignored if no change otherwise
	// invokes callbacks.
	void setClipMinMax(const float min, const float max);

  // Axis Coordinate conversion.
  //
  // Returns true if successful, else false
  //    
  // Convert positions between XYZ and DS coordinates
  // <group>
#if 0
  // (Not to be confused with XYZtoDS below).
  Boolean XYZToDS(const int x, const int y, AvIPosition& ds)
  { return XYZToDS(x, y, getPlane(), ds); }
  Boolean XYZToDS(const int x, const int y, const int z, AvIPosition& ds);
  AvIPosition XYZToDS(const int x, const int y, const int z);
  AvIPosition XYZToDS(const AvIPosition xyz)
  { return XYZToDS(xyz(0), xyz(1), xyz(2)); }

  Boolean DSToXYZ(const AvIPosition& ds, int & x, int & y, int & z);
  Boolean DSToXYZ(const AvIPosition& ds, int & x, int & y)
  { int z; return DSToXYZ(ds,x,y,z); }
  AvIPosition DSToXYZ(const AvIPosition& ds);
  // </group>

  // Convert Positions between DS and World coordinates
  // <group>
  Boolean DSToWorld(const AvIPosition& ds, AvWPosition& world)
  { return ds_->IJKToWorld(ds, world); }
  Boolean WorldToDS(const AvWPosition& world, AvIPosition& ds)
  { return ds_->WorldToIJK(world, ds); }
  // </group>

  // Convert Positions between XYZ and World Coordinates
  // <group>


  AvWPosition XYZToWorld(const int x, const int y)
  { return XYZToWorld(x, y, getPlane()); }
  AvWPosition XYZToWorld(const AvIPosition& xyz)
  { return XYZToWorld(xyz(0), xyz(1), xyz(2)); }

//  Boolean WorldToXYZ(const AvWPosition& world, AvIPosition& xyz);
//  AvIPosition WorldToXYZ(const AvWPosition& world);
#endif
  // Convert XYZ (mapped IJK) coordinates into (mapped) world coordinates.
  Boolean XYZToWorld(const int x, const int y, const int z, AvWPosition& world);
  // As above, but use current plane.
  Boolean XYZToWorld(const int x, const int y, AvWPosition& world)
  { return XYZToWorld(x, y, getPlane(), world); }
  AvWPosition XYZToWorld(const int x, const int y, const int z);
  // Obtain the value along a single axis in world coordinates for an XYZ
  // position.
  Boolean XYZToWorld(const int x, const int y, const int z,
		     const int xyzaxis, double& value);
  // As above, but use current plane.
  Boolean XYZToWorld(const int x, const int y,
		     const int xyzaxis, double& value)
  { return XYZToWorld(x,y,getPlane(),xyzaxis, value); }

#if 0
  Boolean AvAccessor::XYZToWorld(const AvIPosition& xyz, AvWPosition& world)
  { int z = (xyz.nelements() >= 3) ? xyz(2) : 0;
	return XYZToWorld(xyz(0), xyz(1), z, world);
  }
#else
  // Convert from mapped integer indices to mapped world position. If SR
  // is true, value is relative to current subregion start. Returns FALSE
  // if conversion failed. (xyz out of array bounds doesn't necessarily
  // imply a failure).
  Boolean XYZToWorld(const AvIPosition& xyz, AvWPosition& world,
		     const Boolean SR=FALSE)const;
  AvWPosition XYZToWorld(const AvIPosition& xyz, const Boolean SR=TRUE)const;
#endif
  // Convert a mapped world vector into XYZ values.
  Boolean WorldToXYZ(const AvWPosition& world, AvIPosition &xyz);
  Boolean WorldToXYZ(const AvWPosition& world, int & x, int & y, int & z);

  // </group>

  //
  // Data Axis Unit Information/functions
  //

  AvString dataValueUnits(float value, int nsigfigs=4)
  { return ds_->dataValueUnits(value, nsigfigs); }
  AvString dataValue(float value, int nsigfigs=4)
  { return ds_->dataValue(value, nsigfigs); }
  const char * dataUnits() const 
  { return ds_->dataUnits(); }
  
  // Coordinate axis unit information and functions

  AvDataSet::AXISPRINTFORMAT axisPrintFormat(int axis) const
  { return ds_->axisPrintFormat(map_(axis)); }
  AvString axisPrintFormatString(int axis) const
  { return ds_->axisPrintFormatString(map_(axis)); }

  const char * axisName(int axis) const
  { return ds_->axisName(map_(axis)); }
  const char * axisUnitsName(int axis) const
  { return ds_->axisUnitsName(map_(axis)); }
  const char * axisMeasurementName(int axis) const
  { return ds_->axisMeasurementName(map_(axis)); }

  AvString axisUnits(int axis, double value) const
  { return ds_->axisUnits(map_(axis), value); }
  AvString axisUnits(int axis, double value, AvString& abbrevUnits) const
  { return ds_->axisUnits(map_(axis), value, abbrevUnits); }

  double valueInUnits(int axis, double value, const char * units) const
  { return ds_->axisValueInUnits(map_(axis), value, units); }
  AvString axisValueUnits(int axis, double value, int nsigfigs=4) const
  { return ds_->axisValueUnits(map_(axis), value, nsigfigs); }

	const char *dataName()const {return ds_->dataName();}
	float dataMin()const {return ds_->dataMin();}
	float dataMax()const {return ds_->dataMax();}

	// Is the position a valid one for the current subrange?
	// (x, y and z are offsets from start()).
	Boolean validXYZ(const int x, const int y, const int z)const
	 {return ( (x>=0)&&(x<width())&&(y>=0)&&(y<height())&&
		   (x>=0)&&(z<depth()))?TRUE : FALSE;}
	Boolean validXYZ(const int x, const int y)const
	  { return ((x>=0)&&(x<width())&&(y>=0)&&(y<height()))?TRUE:FALSE;}
	// Is the position a valid one for the data set?
	// x, y, z are mapped, but refer to the data set, not the subregion.
	Boolean validIJK(const int x, const int y, const int z);
	Boolean validIJK(const int x, const int y);

	// Return a pointer to the dataset.
	AvDataSet *dataset()const {return ds_;}
	// Utility to return the number of elements available along an
	// axis from a given starting position.
	int axisElements(int start, int stride,
			 const int axis, const Boolean mapped=TRUE);

	// World values of coordinates at the beginning/end of the data set
	// or the current subrange.
	AvWPosition minWorld(const Boolean subregion=TRUE)const
		{return subregion ? minRegion() : mapWorld(ds_->minWorld());}
	AvWPosition maxWorld(const Boolean subregion=TRUE)const
		{return subregion ? maxRegion() : mapWorld(ds_->maxWorld());}
	// World position of the corners of the subregion.
	AvWPosition minRegion()const;
	AvWPosition maxRegion()const;

	// Tell anything connected that they should disconnect now.
	void disconnect();
	// Convert a callback reason mask to a string.
	static AvString reasonToString(const int reason);

	// Convert from mapped to unmapped AvIPosition.
	// from[0] -> to[map[0]], etc.
	AvIPosition XYZtoDS(const AvIPosition &xyz)const
		{ return AvDataSet::XYZtoDS(xyz, map_);}
	// Convert from unmapped to mapped AvIPosition.
	// ds is copied to xyz such that xyz[0] = ds[map[0]].
	AvIPosition DStoXYZ(const AvIPosition &ds)const
		{ return AvDataSet::DStoXYZ(ds, map_);}

	// Utility routines to call invokeCallbacks.
	void clipChanged();		// Data min/max was reset.
	void regionChanged();		// start, shape or map changed.
	void planeChanged();		// Plane changed.

  protected:
	~AvAccessor();		// Use unref().
	// Dataset axis corresponding to XYZ axis.
	 int xAxis_()const{return map_(XAXIS);}
	 int yAxis_()const{return map_(YAXIS);}
	 int zAxis_()const{return rank()> 2 ? map_(ZAXIS) : -1;}

	// Called by notifyCB to handle local updates.
	// If ds is TRUE, event came from dataset, else base accessor
	int notify(const int whymask, const Boolean ds);
	// Check to make sure that an index is valid. Protected so we don't
	// have to check the axis.
	inline Boolean validDSIndex_(const int index, const int axis)const
		{ return ((index>=0) && (index < ds_->axisLength(axis)))
			? TRUE : FALSE;}
	inline Boolean validXYZIndex_(const int index, const int axis)const
		{ return ((index>=0) &&
			  (index < ds_->axisLength(getAxisMapping(axis))))
				? TRUE : FALSE;
		}
	inline Boolean validIndex_(const int index, const int axis,
				   const Boolean mapped=TRUE)const
		{ return ((index>=0) &&
			(index < ds_->axisLength(getAxisMapping(axis,mapped))))
				? TRUE : FALSE;
		}

  private:
	// Constructor calls the dataset's and
	// "base" addCallback functions with this so that when
	// information in the other object changes, the new object will be
	// notified.
	// notifyCB first calls the local notify routine and then
	// calls this object's callCallback routine
	// to propagate the change notice to its "children", if any.
	static void notifyCB(XtPointer obj, AvConductor *c, XtPointer d);
	void dsinit(AvDataSet &ds, const int dscbMask);
	void acinit(AvAccessor &ac, const int dscbMask, const int accbMask);

	// Convert from mapped to unmapped AvWPosition.
	AvWPosition unMapWorld(const AvWPosition &xyz)const;
	// Convert from unmapped to mapped AvWPosition.
	AvWPosition mapWorld(const AvWPosition &ds)const;

	// Sets dataStart with start_ + <x,y,z>.
	// Global variable used to send a position to the dataset.
	// x, y, and z are in XYZ (mapped) coordinates.
	// If subregion is TRUE, x, y and z are relative to the start of
	// the current subregion (offsets from start_) otherwise they are
	// offsets from the start of the data set.
	void dataStart( const int x, const int y, const int z,
			const Boolean subregion=TRUE);

	// Standardized error logging functions.
	// func	- Name of function making call to error_out.
	// msg	- Error message.
	// param- Optional parameter to be appended. 
	void errlog(const char * func, const char * msg,
		 const char * param=NULL)const;
	void errlog(const char * func, const char * msg,
		 int param)const;
	void errlog(const char * func, const char * msg,
		 const AvIPosition &param)const;

  private:
	AvDataSet	*ds_;
	AvAccessor	*ac_;		// Not NULL if 'parent' is accessor.
	float		clipmin_,	// Data min/max to be used.
			clipmax_;
	// These are in DS coordinates.
	AvIPosition	start_,		// Start of subregion.
			shape_,
			map_;		// To convert X/Y/Z to IJK
	// It is necessary to pass a (start) position to the ds for data
	// access. Since only x & y change often, a start position is
	// kept up to date with the current plane. Only x & y need to be
	// set.
	AvIPosition	dataStart_;
	AvIPosition	one_;		// Holds a vector of ones.
	int		plane_;		// Current slice (DS, not SR).
	AvConductorItem *dscitem_;	// Local ConductorItems so it would
	AvConductorItem *accitem_;	// be easy to change the mask.
};

#endif
