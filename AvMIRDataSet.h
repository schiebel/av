#if defined(MIRIAD)
//# Copyright (C) 2003 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvMIRDataSet.h,v 19.3 2003/09/26 16:37:25 hravlin Exp $
//
// $Log: AvMIRDataSet.h,v $
// Revision 19.3  2003/09/26 16:37:25  hravlin
// Added support for masked data. General cleanup.
//
// Revision 19.2  2003/09/17 21:25:37  hravlin
// Made the default value of getKeyword(char) an empty string instead of null.
//
// Revision 19.1  2003/09/02 21:17:59  hravlin
// First pass at a reader for MIRIAD image files.
//
//---------------------------------------------------------------------------

#ifndef _MIR_READER_H_
#define _MIR_READER_H_


#include <sys/types.h>
#include <AvUnits.h>
#include <AvCoordinateSystem.h>
#include <AvDataSet.h>
#include <string>
using namespace std;

//
// <summary>
// Interface to a MIR data file.
// </summary>
//
// <synopsis>
// AvMIRDataSet latches onto a MIRIAD file.
//
// This is one of many different 
// <linkto class=AvDataSet>AvDataSet</linkto> subclasses.
// </synopsis>
//
//
//

class AvMIRDataSet : public AvDataSet 
{
public:
  AvMIRDataSet ();
  
  // return TRUE if already attached.
  virtual Boolean attached() const;	
  // Size along each axis.
  virtual AvIPosition shape() const;
  // Name of dataset.
  virtual const char *dataSetName() const;
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


  ///////////////////////////////////////////////////////////////
  // Return the value of a keyword. If the keyword doesn't exist,
  // returns the default and FALSE.
  // <group>
  Boolean getKeyword(const char *keyword, int &value,
		     int def=0)const;
  Boolean getKeyword(const char *keyword, float &value,
		     float def=(float)0.0)const;
  Boolean getKeyword(const char *keyword, double &value,
		     double def=(double)0.0)const;
  // </group>

  // Also returns FALSE if value is NULL.
  Boolean getKeyword(const char *keyword, char *value, int len,
		     const char *def="")const;

  // Distance between adjacent points along an axis.
  inline int stride(int axis)const{return stride_(axis);}

  // Static member functions defined by all data set readers.
  // <group>
  // Returns true if filename is a potentially valid data set.
  // This version returns true if filename is a directory and contains
  // the readable files "header" and "image".
  static bool isDataSet(const char *filename);
  // Returns true if dir/file is a readable regular file.
  static bool isReadable(const char *dir, const char *file);
  // Returns a pointer to this reader's factory info struct.
  static const AvDataSetFactoryInfo *getFactoryInfo();
  //
  // Returns a pointer to a new reader.
  static AvDataSet * makeDataSet();
  // "MIRIAD".
  static const char *dataSetTypeName();
  virtual const char *dataSetType()const;
  // </group>

protected:

  virtual ~AvMIRDataSet();
  virtual Boolean attach_(const char * pname);	// For reading.
  virtual void detach_();

  void setDataUnits(const AvString& dataUnitsName,
		    const AvString& dataMeasurementName)
  { 
    dataUnitsName_ = dataUnitsName;
    dataMeasurementName_ = dataMeasurementName;
  }

private:

  Boolean initattach_ (const char * pname);
  
  AvCoordinateSystem * cs_;

  AvString dataUnitsName_;
  AvString dataMeasurementName_;

  string fileName_;	// Name of file w/o path.
  string dataSetName_;	// Name of data set. (OBJECT keyword).
  
  enum  AxisInfo {MAXAXES=16};

  AvIPosition shape_;	// Size of image.
  AvIPosition stride_;	// Used to compute index into linear array.
  
  
  AvString * ctypes_;
  double * crpixs_;
  double * cdelts_;
  double * crvals_;

  AvString dataUnits_;

  // Min/Max info
  // <group>
  float		dataMin_;
  float		dataMax_;
  // </group>

  // MIRIAD image related information.
  // <group>
  int	tno_;		// MIRIAD dataset handle.
  // These will change when/if row caching is done. They should only be
  // accessed by getRowPtr().
  int	rowWidth_;	// used a lot.
  int	*whichRow_;	// ranks-1 array giving position of current row.
			// Set by getRow().
  float	*currentRow_;	// Current row cache. Holds an entire row.
			// (Stride = 1).
  int   *currentMask_;  // Row of mask values corresponding to currentRow_.
  bool isMasked_;	// f-> no blanking. t-> There are blanked values.
  // </group>

private:

  // private member functions
  void MIR_init();

  // <group>
  // Standardized error logging functions.
  // <ul>
  // <li><b>func</b> : Name of function making call to error_out.
  // <li><b>msg</b> : Error message.
  // <li><b>param</b> : Optional parameter to be appended. 
  // </ul>
  void error_out(const char * func, const char * msg,
		 const char * param=NULL)const;
  void error_out(const char * func, const char * msg,
		 int param)const;
  void error_out(const char * func, const char * msg,
		 const AvIPosition &param)const;
  // </group>
  
  // Utility to type a message saying a keyword wasn't found.
  // (Used only by attach_()).
  Boolean noKW(const char *kw);

  // get the min/max data values from keywords or the data as necessary.
  void getMinMax();
  // Actually do the min/max.
  void doMinMax(AvIPosition position, int axis,
		float &dataMin, float &dataMax);

  // Return a pointer to the requested row. rownum 0 is ignored.
  // Any masked data points are converted to blankingValue().
  const float *getRowPtr(const AvIPosition &rownum);
};

#endif
#endif
