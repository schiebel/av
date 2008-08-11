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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvAipsDataSet.h,v 19.1 2003/09/02 21:05:51 hravlin Exp $
//---------------------------------------------------------------------------

#if !defined(_AvAipsDataSet_h_)
#define _AvAipsDataSet_h_

#include <aips/aips.h>

#include <XWin_enter.h>

#include <AvUnits.h>
#include <AvCoordinateSystem.h>
#include <AvDataSet.h>

#include <XWin_exit.h>


#include <aips/Utilities/String.h>
#include <trial/Images/PagedImage.h>

class AvTimer;

//
// <summary>
// Interface to a Aips++ data file.
// </summary>
//
// <synopsis>
// Wrapper class that interfaces ImageInterface to AvDataSet
//
// This is one of many different 
// <linkto class=AvDataSet>AvDataSet</linkto> subclasses.
// </synopsis>
//
//
//

class AvAipsDataSet : public AvDataSet 
{
public:
  AvAipsDataSet ();
  
  // return TRUE if already attached.
  virtual Boolean attached() const;	
  // Returns "AIPS++".
  static const char *dataSetTypeName();
  virtual const char *dataSetType() const;
  // Size along each axis.
  virtual AvIPosition shape() const;
  // Name of dataset.
  virtual const char *name() const;
  // Name of "file" w/o path.
  virtual const char * fileName() const;

  // return min and max of dataset
  virtual float dataMin()const;
  virtual float dataMax()const;

	// Return a single data point from the given position.
  virtual float getDatum(const AvIPosition &start);
  // Get a row of data from along an axis.
  virtual int getRow(float *buf,
		     const AvIPosition &start, int length,
		     int stride, int axis);

  // Get a 2D slice of data along an axis
  virtual Boolean getSlice(float * buf,
			   const AvIPosition & start,
			   const AvIPosition & shape,
			   const AvIPosition & stride);

  // copy newData's data into this dataset's data
  virtual Boolean putData(const AvDataSet & newData);

  // Return the exactly the BUNIT keyword
  virtual const char * dataUnits() const;
  // Return the name of the units of data if any
  virtual const char * dataUnitsName() const;
  // Return the measurement name ascribed to the data
  virtual const char * dataMeasurementName() const;

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

  // Static member functions defined by all data set readers.
  // <group>
  // Returns true if filename might refer to a valid dataset.
  static bool isDataSet(const char *filename);
  // Returns a pointer to this reader's factory info struct.
  static const AvDataSetFactoryInfo *getFactoryInfo();
  // Open a dataset and return a pointer to the object.
  // Returns NULL on failure.
  static AvDataSet * makeDataSet();
  // </group>

protected:

  void calcMinMax();

  Boolean isMasked() const { return isMasked_; }

  virtual ~AvAipsDataSet();
  virtual Boolean attach_(const char * pname);
  virtual void detach_();


  void setDataUnits(const AvString& dataUnitsName,
		    const AvString& dataMeasurementName)
  { 
    dataUnitsName_ = dataUnitsName;
    dataMeasurementName_ = dataMeasurementName;
  }

private:
  
  AvString fileName_;

  // Pointer to the AIPS paged image class
  ImageInterface<Float> * image_;

  AvCoordinateSystem * cs_;

  AvString dataUnitsName_;
  AvString dataMeasurementName_;

  AvIPosition shape_;	// Size of image.

  AvString dataUnits_;
  
  // True if the data is masked
  Boolean isMasked_;

  IPosition real_shape_;
  IPosition axis_map_;

  // Min/Max info
  // <group>
  float		dataMin_;
  float		dataMax_;
  // </group>

  // Used for timing so we can relinquish locks if necessary. We only have one
  // timer for all datasets.
  // <group>
  static AvTimer *timer_;
  static int idle_count_;
  void start_timer_if_necessary() const;
  static void stop_timer();
  // The actual callback function
  static void timerCB(AvTimer *, XtPointer cbdata, XtPointer );
  // <group>

};

#endif
