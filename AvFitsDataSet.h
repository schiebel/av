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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvFitsDataSet.h,v 19.1 2003/09/02 21:11:28 hravlin Exp $
//
// $Log: AvFitsDataSet.h,v $
// Revision 19.1  2003/09/02 21:11:28  hravlin
// Added dataSetTypeName(), isDataSet and other support for factory info.
//
// Revision 19.0  2003/07/16 05:47:08  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:51  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/22 21:50:11  hravlin
// Changed header_len_ from int to size_t.
//
// Revision 17.0  2001/11/12 19:42:20  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:27  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:01  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:12  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/08/25 19:53:10  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:39:59  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:36  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.1  1999/04/30 09:45:09  gvandiep
// added private function initattach_ which contains part of
// the attach_ code
//
// Revision 11.0  1998/10/03 06:59:41  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:32  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/11 17:32:44  hr
// Changes in MapSegment to handle cases where off_t is long long (SGI).
//
// Revision 9.0  1997/08/25 21:27:09  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:32  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/01/07 22:56:47  hr
// Removed close() function.
//
// Revision 7.2  1996/12/12 09:58:59  droberts
// Final update from monet archive.
//
// Revision 1.6  1996/09/25 21:23:19  hr
// Added close() function.
//
// Revision 1.5  1996/09/20 20:19:35  pixton
// Prefixed all classes with Av
//
// Revision 1.4  1996/08/26  19:30:24  pixton
// removed declaration for non-existent buildCoordinateSystem()
// quick first pass at html-ified comments.
//
// Revision 1.3  1996/08/23  17:00:09  pixton
// fixed dataUnits()
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

#ifndef _FITS_READER_H_
#define _FITS_READER_H_


#include <sys/types.h>
#include <AvUnits.h>
#include <AvCoordinateSystem.h>
#include <AvDataSet.h>

//
// <summary>
// Interface to a FITS data file.
// </summary>
//
// <synopsis>
// AvFitsDataSet latches onto a FITS file and uses a memory-mapped
// window into the file for retrieving the data therein.  This allows
// one to work with extremely large files.
//
// This is one of many different 
// <linkto class=AvDataSet>AvDataSet</linkto> subclasses.
// </synopsis>
//
//
//

class MapSegment;
class CValues;

class AvFitsDataSet : public AvDataSet 
{
public:
  AvFitsDataSet ();
  
  // return TRUE if already attached.
  virtual Boolean attached() const;	
  // "FITS".
  static const char *dataSetTypeName() {return "FITS";}
  virtual const char *dataSetType()const
    {return AvFitsDataSet::dataSetTypeName();}

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

  // FITS Logical keyword.
  Boolean getKeyword(const char *keyword, Boolean &value,
		     const Boolean def=FALSE)const;
  // Value should be at least 73 characters long.
  // Also returns FALSE if value is NULL.
  Boolean getKeyword(const char *keyword, char *value,
		     const char *def=NULL)const;
  // Return a pointer to the card corresponding to keyword or NULL
  // if not found.
  // This is used internally and probably isn't needed elsewhere.
  // NOTES::
  //	The string isn't null terminated and is always 80 chars long.
  //	The returned pointer is volatile and can not be saved.
  //	The next call to search or keyword may invalidate it.
  //	(Due to possible changing of the mapping window).
  const char *search(const char * keyword)const;
  
  // Convert a position to a file index.
  long PositionToIndex(const AvIPosition &kp)const;
  // Distance between adjacent points along an axis.
  inline int stride(int axis)const{return stride_(axis);}
  // Return the size of a file.
  // <group>
#if 0
  static long fileSize(int fd);
  static long fileSize(const char *filename);
#else
  static off_t fileSize(int fd);
  static off_t fileSize(const char *filename);
#endif
  // </group>

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

  virtual ~AvFitsDataSet();
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

  int	fd_;		// File descriptor
  char	*fileName_;	// Name of file w/o path.
  char	*dataSetName_;	// Name of data set. (OBJECT keyword).
  size_t header_len_;	// Offset into file where data starts
  size_t filelength_;	// Number of bytes in file.
  
  int	bitpix_;	// bits per pixel ==> data type
  int	type_;		// data type (INT16, INT32, FLOAT)
  size_t bytesPerElement_;
  
  AvIPosition shape_;	// Size of image.
  AvIPosition stride_;	// Used to compute index into linear array.
  
  Boolean	blanking_;	// 0-> no blanking. 1 There are blanked values.
  // Only used for non floating point files.
  int	blankval_;	// Value used for blanking for non fp files.
  // Float data always uses NaN.
  // Blanked values will be converted to
  // blankValue().
  
  AvString * ctypes_;
  double * crpixs_;
  double * cdelts_;
  double * crvals_;

  AvString dataUnits_;
  float	scale_;		// scaling factor for data
  float	zero_;		// pixel value offset
  
  // Min/Max info
  // <group>
  float		dataMin_;
  float		dataMax_;
  // </group>

  // Mapping structs used to access the file.
  // <group>
  MapSegment	*headerseg_;
  MapSegment	*dataseg_;
  // </group>

private:

  // private member functions
  void fits_init();

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

  // Fill a buffer with num bytes starting at index elements into the
  // file. Returns the number of elements read.
  // (start, count and stride are in elements).
  unsigned long getBuffer(float *buf, unsigned long start,
		unsigned long length, unsigned long stride);

  // Used by getBuffer().
  void getBuffer_(float *data, const void *buf, unsigned long numelements,
		  unsigned long stride=1);
};

#endif
