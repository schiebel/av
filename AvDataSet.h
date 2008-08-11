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
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvDataSet.h,v 19.1 2003/09/02 21:10:26 hravlin Exp $
//
// $Log: AvDataSet.h,v $
// Revision 19.1  2003/09/02 21:10:26  hravlin
// Added dataSetTypeName(), isDataSet and other support for factory info.
//
// Revision 19.0  2003/07/16 05:47:01  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:45  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:14  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:22  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:35  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:05  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:49  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:20  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:20  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:06  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:26:29  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.1  1997/04/03 20:37:03  hr
// WorldToIJK() now always returns the computed IJK even if it thinks
// the conversion failed.
//
// Revision 8.0  1997/02/20 03:16:08  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.5  1997/01/07 22:21:04  hr
// Changed axisValueInUnits() to match usage in AvFitsDataset and
// AvResidentDataset. (3rd arg is AvString rather than char * and function
// is not const).
//
// Revision 7.4  1996/12/12 08:14:00  droberts
// Added place to set the path.
//
// Revision 1.14  1996/10/07 21:47:49  pixton
// renamed valueInUnits to axisValueInUnits
//
// Revision 1.13  1996/09/20  20:56:37  pixton
// Av prefix added to IPosition, WPosition
//
// Revision 1.12  1996/09/17  16:32:15  hr
// Added start().
//
// Revision 1.11  1996/08/30  20:34:21  pixton
// unvirtualized validIJK because it depends on a consistent
// definition of virtual shape().
//
// Revision 1.10  1996/08/27  14:48:25  pixton
// unvirtualized axisLength() - it is written in terms of virtual shape()
//
// Revision 1.9  1996/08/21  15:11:36  pixton
// added dataUnitsName and dataMeasurementName interface functions
// that were previously missing.
//
// Revision 1.8  1996/08/19  15:07:58  pixton
// touched up comments.
//
// Revision 1.7  1996/08/16  21:59:57  pixton
// added axisPrintFormatString()
// polished comments for html reference
//
// Revision 1.6  1996/08/16  21:14:48  hr
// Changed interface to XYZtoDS and DStoXYZ functions.
//
// Revision 1.5  1996/08/12  21:32:25  pixton
// modified IJKToWorld...
//
// Revision 1.4  1996/08/12  14:49:07  pixton
// widespread changes to support coordinate mapping and labeling.
//
// Revision 1.2  1996/07/23  17:24:16  hr
// Removed const from get{Datum,Row,etc.}.
//
// Revision 1.1  1996/07/18 19:27:28  hr
// Initial revision
//
//
//---------------------------------------------------------------------------

#ifndef AvDataSet_h
#define AvDataSet_h

#include "AvResource.h"
#include "AvConductor.h"
#include "AvIPosition.h"
#include "AvWPosition.h"
#include "AvString.h"
#include <X11/Intrinsic.h>	// Boolean

/* <summary>Base class for all aipsview data classes</summary>

<synopsis>This is the base class for the aipsview image class. This will need to be
subclassed for each "reader".

Most of the virtual functions need to be overridden. However, it should be
possible to get away with just getDatum and leaving getRow and getSlice for
later.

putRow and getSlicePtr are not currently used by aipsview.

AvDataSet is subclassed from <linkto class=AvResource>AvResource</linkto>
 since a lot of objects are likely
to refer to it. AvDataSet objects must be created by "new". Use obj->ref()
to increase the reference count and obj->unref() to decrease it. When
the count goes to 0, the object will self destruct. Don't call the destructor
explicitly.

AvDataSet is also subclassed from <linkto class=AvConductor>AvConductor</linkto>
so changes can be propagated to
interested parties. While there is currently no standard mechanism to
change a data set, subclasses are free to provide one, if needed.

A double version of <linkto class=AvIPosition>IPosition</linkto> called
<linkto class=AvWPosition>WPosition</linkto> (World Position) was built to 
satisfy our needs of manipulating precise coordinates without having to
use AIPS++ Vector<Double> right away.

For functions retrieving/storing data, a negative stride means transfer
from the starting point towards 0, rather than towards the end of the axis.
This probably isn't supported by any reader and aipsview doesn't currently
need it. (The only time aipsview will use a stride that isn't 1 is when the
canvas is displaying a raster with a scale factor less than 1).

There are three coordinate systems used in AvDataSet:
<dl>

<dt><b>IJK</b> or <b>DS</b> (dataset) coordinates
<dd>		Integer indexes into the data set cube.
<dt><b>World</b>
<dd>World coordinates for the dataset.  Celestial mapping is
carried out by <linkto class=AvCoordinateSystem>AvCoordinateSystem</linkto>
using the wcslib written by Mark Calebretta.
<dt><b>XYZ</b>
<dd>Aipsview's 3D coordinate system with the X axis going across the screen, Y axis going up and Z axis going into the screen. XYZ coordinates allow for any IJK axis to be
 mapped into an X, Y or Z value. Routines that support mapping
 take a map argument that describes the mapping between XYZ
 and IJK. Non orthogonal axis are not supported.

</dl>
</synopsis>
*/

class AvDataSet;

typedef struct {
  AvDataSet *(*makeDataSet)();		// New data set.
  bool (*isDataSet)(const char *f);
  const char *(*dataSetTypeName)(void);
 } AvDataSetFactoryInfo;

typedef const AvDataSetFactoryInfo *(AvDataSetFactoryInfoPtr)();


class AvDataSet : public AvResource, public AvConductor 
{
public:

  // Different ways the dataset can change that other objects might
  // need to be notified about.
  // Destroyed	Dataset's destructor has been called.
  //		(Since the dataset is an AvResource, everything connected
  //		 to the dataset should have disconnected already).
  // DISCONNECT	This is a warning that anything connected should
  //		disconnect immediately so the dataset can be destroyed.
  //		(The user is finished with the dataset).
  // ATTACHED	Dataset has just attached to the data. (eg. opened).
  // DETACHED	Dataset was detached.
  // DATA		Data value or dataset size has changed.
  //		This won't happen unless the data set is somehow
  //		dynamic.
  enum { NoReason=AvConductor::NoReason,
	 Destroyed=AvConductor::Destroyed,
	 DISCONNECT=AvConductor::USER0,
	 ATTACHED=AvConductor::USER1, 
	 DETACHED=AvConductor::USER2,
	 DATA=AvConductor::USER3,
	 DSFENCE=DATA,
	 DEFAULTMASK=DISCONNECT|ATTACHED|DETACHED|DATA
  };
  
  // Attach the dataset to the data. Returns TRUE on success.
  // If already open, calls detach first.
  // The base class calls attach_ which should be overridded by
  // subclasses. If successful, it then invokes the callback list
  // to propagate the change notice.
  Boolean attach(const char *name);

  // Returns TRUE if currently attached.
  virtual Boolean attached()const;	
  
  // Detach the dataset from the class if it is attached, else
  // do nothing
  void detach();
  
  //
  //   Data Value Access
  //	
  //
  // Return the value used to indicate blanked data. Must be a valid
  // number and not infinity since it causes problems on some machines.
  // (Default is MAX_FLOAT).
  virtual float blankingValue() const;
  
  // Return the minimum data value present in the data set
  virtual float	dataMin() const;
  // Return the maximum data value present in the data set
  virtual float	dataMax() const;

  // Number of axis in data set
  virtual int   rank() const;		
  // Number of data points.
  virtual int   nelements() const;	

  // Starting position. (Always 0).
  virtual AvIPosition start() const;

  // Size along each axis.
  virtual AvIPosition shape() const;
  
  // Name of dataset.
  // <group>
  const char * dataSetName() const { return name(); }
  virtual const char *name() const;
  // </group>

  // Name of "file" without path
  virtual const char *fileName() const;

  // "Type" of dataset: eg. "AIPS++", "FITS", etc.
  static const char *dataSetTypeName();
  virtual const char *dataSetType()const;

  // Type of data as printable string 
  virtual const char *dataName() const;
  
  // return string containing value and units e.g., "12.50 JY/BEAM"
  // <group>
  virtual AvString dataValueUnits(float value, int nsigfigs = 4);
  AvString dataValueUnits(const AvIPosition &pos, int nsigfigs = 4);
  // </group>

  // return string without units e.g., "12.50" 
  // <group>
  virtual AvString dataValue(float value, int nsigfigs = 4);
  AvString dataValue(const AvIPosition &pos, int nsigfigs = 4);
  // </group>

  // return string representing units e.g., "JY/BEAM"
  virtual const char *dataUnits() const;

  // return string containing the full name of the data units
  virtual const char * dataUnitsName() const { return ""; }
  // return string containing the name of the measurement e.g., "specific intensity"
  virtual const char * dataMeasurementName() const { return ""; }

  // <group>
  // Routines to return chunks of the data set
  //
  // Return the value at the position in IJK coordinates.
  virtual float getDatum(const AvIPosition &pos);

  // Fill a buffer with length elements from along axis "axis".
  // aipsview will use this, via the accessor, for raster information.
  // If stride is negative, data is retrieved from decreasing axis
  // coordinates. (eg R to L rather than L to R).
  // Returns the number of elements actually copied.
  virtual int getRow(float *buf,
		     const AvIPosition &start, int length,
		     int stride, int axis);
  // Same as above, but start is in XYZ coordinates.
  virtual int getMappedRow(float *buf,
			   const AvIPosition &start, int length,
			   int stride, int axis,
			   const AvIPosition &map);

  // Store data into dataset. Default is to just call dataChanged().
  // This not currently needed by aipsview.
  //	virtual int putRow(float *buf,
  //			   const AvIPosition &start, const int length,
  //			   const int stride, const int axis)const;

  // Fill an array with data points. Any masked/blanked values
  // will be replaced by blankingValue().
  // Returns FALSE for illegal values.
  // Note that aipsview will call getMappedSlice instead.
  virtual Boolean getSlice(float *buf, const AvIPosition &start,
			   const AvIPosition &shape,
			   const AvIPosition &stride);
  
  // copy newData's data into this dataset's data
  virtual Boolean putData(const AvDataSet & newData);

  // Fill an array with data points transposed to match buf's layout.
  // map[0] gives the dataset axis corresponding to buf's primary ('X')
  // axis, map[1] corresponds to the secondary axis ('Y'), etc.
  // eg. buf[0] to buf[shape[map[0]]] receives data from along dataset
  // axis map[0].
  // Start, shape and stride are in XYZ coordinates.
  // If stride is negative, data is retrieved from start towards 0.
  // (Not implemented).
  // Currently, the only thing that might call this is retrieval of
  // the 3D region to pass to GLISH?
  // Returns FALSE if any parameter values are bad.
  virtual Boolean getMappedSlice(float *buf, const AvIPosition &start,
				 const AvIPosition &shape,
				 const AvIPosition &stride,
				 const AvIPosition &map);

  // Return a pointer to an array of data points. If needDelete is True,
  // caller will need to "delete []" the array.
  // Current aipsview does not need this. This is potentially for
  // new 3D algorithms.
  // If map is NULL, no mapping is done and start shape and stride
  // are in XYZ coordinates. Otherwise, they are in IJK coordinates.
  virtual float *getSlicePtr(Boolean &needDelete,
			     const AvIPosition &start,
			     const AvIPosition &shape,
			     const AvIPosition &stride,
			     const AvIPosition *map);
  

  // </group>

  //	          Coordinate information.

  // Return the name of the projection part of the system or
  // NULL if there is none.
  const char * projectionCoordinateSystemName() const;

  // Min/max world coordinates. Note that it is likely that a 'max'
  // coordinate will actually be less than the 'min' since values
  // may decrease with increasing axis index.  
  // 
  // For projection axes, min/maxWorld is not likely to represent
  // the minimum and maximum world values present.
  // Returns the world coordinates of position 0 and shape()-1.
  // <group>
  AvWPosition minWorld() const;
  AvWPosition maxWorld() const;
  // </group>

  // Convert a position between IJK and world coordinates.
  // Conversion may depend on more than one axis.
  // Returns FALSE if the conversion failed
  // <group>
  Boolean IJKToWorld(const AvIPosition &ijk, AvWPosition &world) const
  { return IJKToWorld_(AvWPosition(ijk), world); }
  virtual Boolean IJKToWorld_(const AvWPosition &ijk,
			       AvWPosition &world) const;
  // </group>

  // Returns the indices closest to the world value.
  // Returns FALSE if the conversion failed
  // <group>
  Boolean WorldToIJK(const AvWPosition &world, AvIPosition &ijk) const
  { 
    AvWPosition Wijk(world.nelements());
    Boolean retval = WorldToIJK_(world, Wijk);
    ijk = AvIPosition(Wijk);
    return retval;
  }
  virtual Boolean WorldToIJK_(const AvWPosition &world, AvWPosition &ijk) const;
  // </group>

  // Is the position a valid one for the data set?
  Boolean validIJK(const AvIPosition &position)const;

  // Map to IJK and ask validIJK's opinion
  Boolean validWorld(const AvWPosition &position)const;

  enum AXISPRINTFORMAT { DEFAULT, HMS, DMS };
  //
  // Return an enumerator suitable for comparing the various
  // formats used to display values.  Expressions between //'s
  // are regular expressions.  The number of significant digits
  // used is controlled separately.
  //
  // <dl>
  // <dt><b>DEFAULT</b> - format is <value> <units>.  
  // <dd>e.g., 12.3 Mhz (for 3 sig figs)
  // <dt><b>HMS</b>       - format is HHh MMm /SS(.S+)?/s
  // <dd>   e.g., 05h 30m 12.3456s (6 sig figs)
  // <dt>DMS       - format is DDd MMm /SS(.S+)?/s 
  // <dd> e.g., 09d 45m 12.3s (3 sig figs)
  // </dl>
  //
  // Planned:
  //
  // <dl>
  // <dt>DATE      - DD MM D HH:MM:SS TZ YEAR
  //    e.g., Mon Aug  5 15:16:48 CDT 1996
  // <dd>
  // </dl>
  //
  // Return one of the above formats in either enumerator format
  // or AvString format.
  // <group>
  AXISPRINTFORMAT axisPrintFormat(int axis) const;
  AvString axisPrintFormatString(int axis) const;
  // </group>

  // Return True if the given axis is valid, otherwise return False
  inline Boolean validAxis(int axis) const
  { return ((axis >= 0) && (axis < rank())) ? TRUE : FALSE;}

  // Length of an axis (eg. shape[axis]);
  int axisLength(int axis) const;

  // Utility routine to return the number of elements retrievable
  // from an axis of length 'length' given a starting offset and a
  // stride. Stride may be < 0.   
  //
  static int axisElements(int start, int length, int stride);

  // Type of axis as printable string. e.g. "RA", "VELO" etc.
  virtual const char *axisName(int axis) const;

  // Return the full name of the units used for values on the axes
  virtual const char *axisUnitsName(int axis) const;

  // Return the name of the measurement along an axis
  virtual const char *axisMeasurementName(int axis) const;

  // Return the units that would be used to print the given
  // value  e.g., "megahertz","million lightyears"
  // <group>
  virtual AvString axisUnits(int axis, double value) const;
  // </group>

  // Return the units that would be used to print the given
  // value along with the abbreviated form "MHz","M ly"
  // <group>
  virtual AvString axisUnits(int axis, 
			     double value, 
			     AvString & abbrevUnits) const;
  // </group>

  // Return the value expressed in the units requested
  virtual double axisValueInUnits(int axis, double value,
				  const AvString& units);

  // Returns a string appropriate for printing the value on
  // the given axis.
  virtual AvString axisValueUnits(int axis, 
				  double value,
				  int nsigfigs = 4) const;
  
  // Support routines.

  // These routines just call invokeCallbacks with the appropriate
  // mask. Subclasses should make sure they call these when changing
  // data.
  // <group>
  void disconnect();
  void dataChanged();
  // </group>

  // Return a pointer to a copy of the string. If NULLSTRING is TRUE,
  // a NULL input will result in a NULL return. Otherwise, an empty
  // string is returned. Use delete [] to free.
  static char *Strdup(const char *, const Boolean USENULLSTRING=FALSE);

  // <group>
  // Convert between XYZ (mapped) and DS (data set) coordinates.
  // If map is NULL, conversion is via a straight copy.
  // The output is forced to be the same length as the input.
  // Output can't be the same as input. (Variables can't be mapped
  // on top of themselves).

  // xyz is copied to ds such that ds(map(0)) = xyz(0).
  // <group>
  static AvIPosition XYZtoDS(const AvIPosition &xyz, const AvIPosition *map);
  static AvIPosition XYZtoDS(const AvIPosition &xyz, const AvIPosition &map);
  // </group>

  // ds is copied to xyz such that xyz(0) = ds(map(0)).
  // <group>
  static AvIPosition DStoXYZ(const AvIPosition &ds, const AvIPosition *map);
  static AvIPosition DStoXYZ(const AvIPosition &ds, const AvIPosition &map);
  // </group>
  // </group>

  // Validity check for slice parameters.
  Boolean validSliceValues(const AvIPosition &start,
			   const AvIPosition &shape,	
			   const AvIPosition &stride)const;

  // Debugging routine. Convert a value returned by the base class'
  // getDatum function into an address. addr is resized to be 4 long.
  static AvIPosition decodeDatum(float value);

  // Encode the first four values of address as a floating point #.
  // address(0) is placed in the low byte. Use decodeDatum to invert.
  // Since floating values are created from integers, it probably isn't
  // a good idea to try to encode a 4th axis with large (>128) values.
  static float encodeDatum(const AvIPosition &address);

  // Returns the default blankingValue (MAX_FLOAT)
  static float defaultBlankingValue();

  void setPath(const char * path) { path_ = path; }
  const char * path() const { return path_.chars(); }

protected:

  // Base class doesn't want to be explicitly created.
  AvDataSet();

  // Destructors for subclasses of AvResource should be protected so
  // they are not accidentally called explicitly.
  // Use unref().
  virtual ~AvDataSet();	

  // These are the real attach/detach functions.
  // <group>
  virtual Boolean attach_(const char *filename);
  virtual void detach_();
  // </group>

  // Return a mapped index. If map is NULL axis is returned.
  inline int mappedAxis(int axis, const AvIPosition *map=NULL) const
  { return (map != NULL) ? (*map)(axis) : axis; }

  // Return a region of data in IJK or XYZ order.
  // getSlice_ returns a pointer to the next position in the
  // buffer. Protected because it does not check parameter validity.
  float *getSlice_(float *buf,
		   const AvIPosition &start,
		   const AvIPosition &shape, 
		   const AvIPosition &stride,
		   int axis, const AvIPosition *map);

  // Each reader needs to implement these as public interfaces.
  // They are called from AvMakeDataSet.
  // They're protected here since the base class doesn't do anything.
  // <group>
  // Returns true if filename is a potential valid data set.
  // This version returns true if filename is a regular file and is
  // readable.
  static bool isDataSet(const char *filename);
  // Returns a pointer to this reader's factory info struct.
  static const AvDataSetFactoryInfo *getFactoryInfo() { return 0;}
  //
  // Open a dataset and return a pointer to the object.
  // Returns NULL on failure.
  static AvDataSet * makeDataSet() { return 0;}
  // </group>

private:

  AvString path_;

};

#endif
