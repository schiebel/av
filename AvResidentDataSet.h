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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvResidentDataSet.h,v 19.0 2003/07/16 05:47:45 aips2adm Exp $
//
// $Log: AvResidentDataSet.h,v $
// Revision 19.0  2003/07/16 05:47:45  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:22  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:51  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:57  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:05  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:51  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:40  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:49  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:27  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:33  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:30:20  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:18:35  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1996/12/12 07:07:01  droberts
// Final update from monet archive.
//
// Revision 7.2  1996/10/25 22:43:22  jpixton
// added putData(AvDataSet)
//
// Revision 7.1  1996/09/26 20:24:16  jpixton
//  initial checkin before native aips images supported.
//
// Revision 1.3  1996/09/20 20:19:35  pixton
// Prefixed all classes with Av
//
// Revision 1.2  1996/08/28  22:16:31  pixton
// bug fix - failed to copy passed data array if takeover is false.
//
// Revision 1.1  1996/08/23  19:32:19  pixton
// Initial revision
//
// Revision 1.2  1996/08/12  21:32:25  pixton
// modified IJKToWorld...
//
// Revision 1.1  1996/08/12  14:49:07  pixton
// Initial revision
//
// Revision 1.1  1996/08/02  16:50:20  hr
// Initial revision
//
//
//---------------------------------------------------------------------------

#if !defined(_AvResidentDataSet_h_)
#define _AvResidentDataSet_h_

#include <AvUnits.h>
#include <AvCoordinateSystem.h>
#include <AvDataSet.h>

//
// <summary>
// dataset that is resident in memory.
// </summary>
//
// <synopsis>
// AvResidentDataSet is a data set that resides in memory.  It is constructed
// by passing an array, along with specifications, and a coordinate system.
// </synopsis>
//
// <motivation>
// <ol>
// <li>We need a glish data set, but I wanted to separate the mechanisms for
//     parsing the glish record from the dataset access.
// <li>We need a simple dataset that is capable of being generated from
//     an expression of other datasets.
// </ol>
// </motivation>
//

class AvResidentDataSet : public AvDataSet 
{
public:

  static AvDataSet * makeExample(int sizex, int sizey, int sizez);

  AvResidentDataSet (const float * array,
		     const AvIPosition& shape,
		     const char * bunit,
		     const AvCoordinateSystem& cs,
		     const char * name,
		     const float blankval,
		     Boolean manageArray = 0);

  AvResidentDataSet (const float * array,
		     const AvIPosition& shape,
		     const char * bunit,
		     const AvCoordinateSystem& cs,
		     const char * name,
		     const float blankval,
		     Boolean manageArray,
		     const float datamin,
		     const float datamax);

  // Resident datasets are always "attached"
  virtual Boolean attached() const { return attached_; }
  // "Resident".
  virtual const char *dataSetType() const { return "Resident"; }
  // Size along each axis.
  virtual AvIPosition shape() const { return shape_; }
  // Name of dataset.
  virtual const char * name() const { return name_.chars(); }
  // Name of "file" w/o path.
  virtual const char * fileName() const { return name(); }

  // Return the value that represents blank data for this dataset
  virtual float blankingValue() const { return blankVal_; }

  // Return the minimum value in the array
  virtual float dataMin() const { return dataMin_; }
  // Return the maximum value in the array
  virtual float dataMax() const { return dataMax_; }

  // [] Return a single data point from the given position.
  virtual float getDatum(const AvIPosition &start);

  // [] Get a row of data from along an axis.
  virtual int getRow(float *buf,
		     const AvIPosition &start, int length,
		     int stride, int axis);
  
  // replace the data and coordinate system with newData's
  virtual Boolean putData(const AvDataSet& newData);

  // Return the abbreviated name of the data units
  virtual const char * dataUnits() const { return dataUnits_.chars(); }
  // Return the name of the units of data if any
  virtual const char * dataUnitsName() const { return dataUnitsName_.chars(); }
  // Return the measurement name ascribed to the data
  virtual const char * dataMeasurementName() const 
  { return dataMeasurementName_.chars(); }

  // Return the name of an axis
  virtual const char * axisName(int axis) const;
  // Return the name of the units to use for a given axis e.g., "hertz"
  virtual const char * axisUnitsName(int axis) const;
  // Return the measurement type of an axis e.g., "right ascension"
  virtual const char * axisMeasurementName(int axis) const;
  // Return the string printed nicely
  virtual AvString axisValueUnits(int axis, 
				  double value,
				  int nsigfigs = 4) const;
  // return the value given
  virtual double axisValueInUnits(int axis,
				  double value,
				  const AvString& units);
  // Convert position from IJK coords to World coords
  // returns FALSE if conversion failed
  virtual Boolean IJKToWorld_(const AvWPosition &ijk,
			      AvWPosition &world) const;
  // Convert position from World coords to IJK coords
  // returns FALSE if conversion failed
  virtual Boolean WorldToIJK_(const AvWPosition &world,
			      AvWPosition &ijk) const;

protected:

  virtual ~AvResidentDataSet();
  virtual Boolean attach_(const char * /*pname*/) { return (attached_ = True); }
  virtual void detach_() { attached_ = False; }

  void setDataUnits(const AvString& dataUnitsName,
		    const AvString& dataMeasurementName)
  { 
    dataUnitsName_ = dataUnitsName;
    dataMeasurementName_ = dataMeasurementName;
  }

private:

   // does most of the constructor work
  void init();  
 
  // Just here to fool AvDataSet - attach does nothing
  Boolean attached_;

  // Coordinate system
  AvCoordinateSystem * cs_;

  // abbreviated form of units e.g., "mjy/beam"
  AvString dataUnits_;
  // full units name e.g., "millijanksys/beam"
  AvString dataUnitsName_;
  // measurement type name e.g., "specific intensity"
  AvString dataMeasurementName_;

  // name of the dataset
  AvString name_;

  // pointer to array
  float * array_;
  // True if this class responsible for deleting
  Boolean arrayOwner_;
  // shape of the array
  AvIPosition shape_;

  // arrayStep_[a] is the linear distance in the array between 
  // adjacent cells along axis a. 
  // arrayStep_(0) is 1, 
  // arrayStep_(i) is arrayStep_(i-1)*shape_(i-1).
  AvIPosition arrayStep_;
  
  // blank value of dataset
  float blankVal_;
 
  // Min/Max info
  float	dataMin_;
  float	dataMax_;

private:

};

#endif
