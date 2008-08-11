//# Copyright (C) 1995-97 Board of Trustees of the University of Illinois
//#
//# This software, both binary and source, is copyrighted by The
//# Board of Trustees of the University of Illinois.  Ownership
//# remains with the University.  You should have received a copy 
//# of a licensing agreement with this software.  See the file 
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvScrollingBufferDataSet.h,v 19.0 2003/07/16 05:47:14 aips2adm Exp $
//
// $Log: AvScrollingBufferDataSet.h,v $
// Revision 19.0  2003/07/16 05:47:14  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:56  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:25  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:32  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:20  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:18  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:05  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:47  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:57  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:50  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:27:39  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:53  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1996/12/12 10:05:50  droberts
// Final update from monet archive.
//
// Revision 1.3  1996/12/11 20:40:12  hr
// Added RCS header and copyright.
//
//
//---------------------------------------------------------------------------

#ifndef __AvScrollingBufferDataSet_h__
#define __AvScrollingBufferDataSet_h__

#include <iostream.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "AvIPosition.h"
#include "AvWPosition.h"

#include "AvVector.h"
#include "AvDataSet.h"
#include "AvResidentDataSet.h"

//
// <summary>
// Scrolling buffer along last dimension (time).  
// </summary>
//
// <synopsis>
// Buffer is created of a certain predetermined size.  The constructor
// declares the number of records to store and the first AvDataSet
// pointer which is put into place.
//
// The datasets are ref'd when put in the buffer and unref'd when
// the buffer no longer has need of them.
//
// </synopsis>
//
// <example>
// 
// 
//
// </example>

class AvScrollingBufferDataSet : public AvDataSet
{
public:
  // construct the scrolling buffer dataset with the size
  // and the first dataset to be placed there.
  AvScrollingBufferDataSet(unsigned int size, AvDataSet * ds);
  
  // return the size of the buffer
  unsigned int size() const { return size_; }

  // scroll and unref replaced value
  void addDataSet(AvDataSet * ds);
  
  // return the index of the time axis
  int timeAxis() const { return timeAxis_; }

  // crap for AvDataSet
  Boolean attached() const { return attached_; }

private:

  // move the window down
  void scroll();

  // replace the origin_'th dataset pointer with this one and
  // return the old value or NULL.  passing NULL illegal, and
  // passing another ScrollingBufferDataSet type is illegal.
  Boolean replace(AvDataSet * ds, int ndx);

public:

  // use first dataset's value
  virtual float blankingValue() const { return buffer_(0)->blankingValue(); }

  // return min/max value over all datasets
  // <group>
  virtual float dataMin() const { return min_; }
  virtual float dataMax() const { return max_; }
  // </group>

  // return rank of first always, assuming that there must be
  // a time axis entry.  This is a good assumption as far as the
  // Parkes Multibeam project is concerned.
  virtual int rank() const { return buffer_(0)->rank(); }

  // return number of data points
  virtual int nelements() const { return size_*buffer_(0)->nelements(); }
  
  // return the size along each axis
  virtual AvIPosition shape() const { return shape_; }

  // return the name of the dataset
  virtual const char * name() const { return buffer_(0)->name(); }

  // since no file name, route call to name
  virtual const char * fileName() const { return name(); }
  
  // return name of dataset
  virtual const char * dataSetType() const { return "Scrolling Buffer"; }
  
  // return "type of data as printable string" (AvDataSet)
  virtual const char * dataName() const { return buffer_(0)->dataName(); }

  // return string containing value and units
  virtual AvString dataValueUnits(float value, int nsigfigs = 4)
  { return buffer_(0)->dataValueUnits(value, nsigfigs); }
 
  // return string containing only value
  virtual AvString dataValue(float value, int nsigfigs = 4)
  { return buffer_(0)->dataValue(value, nsigfigs); }
  
  // return string representing units
  virtual const char * dataUnits() const
  { return buffer_(0)->dataUnits(); }

  // return string containing the full name of the units
  virtual const char * dataUnitsName() const
  { return buffer_(0)->dataUnitsName(); }

  // return string containing the name of the measurement
  virtual const char * dataMeasurementName() const
  { return buffer_(0)->dataMeasurementName(); }

  // return data at a given position
  virtual float getDatum(const AvIPosition & pos);

  // return row of data along any axis
  virtual int getRow(float * buf, const AvIPosition & start,
		     int length, int stride, int axis);

  // map accounting for the extra dimension
  // <group>
  virtual Boolean IJKToWorld_(const AvWPosition& ijk,
			      AvWPosition& world) const;
  virtual Boolean WorldToIJK_(const AvWPosition& world,
			      AvWPosition& ijk) const;
  // </group>
  
  // return the name given to axis
  virtual const char * axisName(int axis) const
  { return buffer_(0)->axisName(axis); }

  // return the full name of the units used for values on the axes
  virtual const char * axisUnitsName(int axis) const
  { return buffer_(0)->axisUnitsName(axis); }

  // return the name of the measurement along an axis
  virtual const char * axisMeasurementName(int axis) const
  { return buffer_(0)->axisMeasurementName(axis); }
       
  // return the units that would be used to print the given value
  // [ ] may have to do something if referencing the time axis
  virtual AvString axisUnits(int axis, double value) const
  { return buffer_(0)->axisUnits(axis, value); }

  // return the units that would be used to print the
  // given value along with the abbreviated form
  virtual AvString axisUnits(int axis, double value, AvString& abbrev) const
  { return buffer_(0)->axisUnits(axis, value, abbrev); }

  // return a string appropriate for printing the value on the given axis
  virtual AvString axisValueUnits(int axis, double value, int nsigfigs=4) const
  { return buffer_(0)->axisValueUnits(axis,value,nsigfigs); }

protected:

  virtual Boolean attach_(const char *) { attached_ = True; return attached_; }
  virtual void detach_() { attached_ = False; }

private:

  // return the dataset corresponding to the nth position
  AvDataSet * dsByNdx(int ndx) const 
  { return buffer_((ndx + origin_) % size_); }

  // return the dataset closest to the given time value
  int timeVal2ndx(double timeVal) const;

  // Scrolling is true if the buffer is full
  Boolean scrolling() const { return nFilled_ == size_; }

  // which axis is the time axis?
  int timeAxis_;

  // recompute min and max in constant amortized time
  void updateMinMax_(AvDataSet * old);

  // size along time axis
  unsigned int size_;

  // Contains the number of datasets occupying the buffer.  This will
  // be zero initially and will be as high as size.
  unsigned int nFilled_;

  // This is the offset origin on the time axis.  
  // When the buffer scrolls, this number is incremented modulo size_;
  unsigned int origin_;

  // the vector of dataset pointers
  AvVector<AvDataSet *> buffer_;

  // shape of the dataset as a whole or unit.
  AvIPosition shape_;

  // shape of a single ds captured from first AvDataSet passed used
  // to enforce conformity among all datasets.
  AvIPosition itemShape_;

  // Running min/max system
  float min_;
  float max_;
  AvDataSet * dsWithMin_;
  AvDataSet * dsWithMax_;

  // Time axis info.  Time values are not constrained to be equally spaced.
  // to speed mapping time values, they are kept here for reference, and
  // the same method of indexing is used
  AvVector<double> timeVals_;

  // Junk to fool AvDataSet interface
  Boolean attached_;
};




#endif
