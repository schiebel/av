//# Copyright (C) 1995-98 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvCLUIRaster.h,v 19.0 2003/07/16 05:47:36 aips2adm Exp $
//
// $Log: AvCLUIRaster.h,v $
// Revision 19.0  2003/07/16 05:47:36  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:15  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:44  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:51  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:39  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:41  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:30  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:32  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:03  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:05  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/09 17:18:08  hr
// Removed extra semicolons from the "DECLARE_xxx" macros.
//
// Revision 9.0  1997/08/25 21:29:36  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.2  1997/06/16 21:36:58  hr
// Added units_ variable.
//
// Revision 8.1  1997/04/17 20:44:03  hr
// Added accessor() function.
//
// Revision 8.0  1997/02/20 03:14:39  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 09:28:51  droberts
// Final update from monet archive.
//
// Revision 1.4  1996/11/12 20:12:24  hr
// Added new getRegion() call and removed unneeded {begin,end}Access() calls.
//
// Revision 1.3  1996/09/20 18:36:22  hr
// Changes to work with accessor including rewrite of axisinfo.
//
// Revision 1.2  1996/09/10 16:49:19  hr
// Initial changes for integration with AvAccessor & AvDataSet.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.8  1996/06/18  18:42:38  pixton
//  Copyright Update
//
//  Revision 1.7  1996/04/11  16:12:04  hr
//  Added setSlice & sliceIndex_.
//
//  Revision 1.6  1996/01/31  17:58:09  hr
//  Added sliceindex to constructor. Renamed image_ to imageSetter_ and
//  added width_ and height_.
//
//  Revision 1.5  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/04/25  16:44:35  hr
//  Added array to hold pointers to the start of each row.
//
//  Revision 1.3  1995/04/24  21:56:36  hr
//  Added getRow and getelement functions.
//
//  Revision 1.2  1995/04/24  18:44:26  hr
//  Moved much of the drawing work to CLUIRaster from CLUImageSetter
//  to support beginAccess and endAccess.
//
//  Revision 1.1  1995/04/14  16:29:53  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvCLUIRaster.h
A 2D Raster that gets its data from an SVGriddedData object.
*/
#ifndef AvCLUIRaster_h
#define AvCLUIRaster_h

#include <AvCLUImage.h>
#include <AvRaster.h>
#include <AvString.h>

class AvColormap;

class AvCLUIRaster : public AvRaster {
  public:
	AvCLUIRaster(AvCLUImageSetter *, const int sliceindex=-1);
	~AvCLUIRaster();
	virtual float getElement(const int x, const int y);
	virtual void getRow(float *row, const int x, const int y,
			const int num=0, const int incr=1);
	// Get nrows (sub)rows of width width starting at position x/y.
	// Stride > 1 -> skip pixels. stride <= 0 is illegal.
	virtual void getRegion(float *data, const int x, const int y,
			       const unsigned int width,
			       const unsigned int nrows,
			       const int xstride=1, const int ystride=1);

	virtual float min()const;
	virtual void min(const float){};
	virtual float max()const;
	virtual void max(const float){};
	virtual unsigned int width()const;
	virtual unsigned int height()const;
	virtual void getBoundingBox(AvCanvas &c, AvAllotment &r);
	AvAccessor *accessor()const{return accessor_;}
  public:
	class axisInfo : public AvRaster::axisInfo {
	  public:
		axisInfo(AvAccessor *);
		axisInfo(axisInfo &);
		virtual ~axisInfo();

		// The axis label.
		virtual const char *label(const int axis)const;
		// The units for the axis
		virtual const char *unitString(const int axis)const;
		virtual AvRaster::axisInfo::PRINTFORMAT
					printFormat(const int axis)const;
		// Number of indices along axis.
		virtual int length(const int axis)const;
		// TRUE if i is a valid index. (0..length()-1).
		virtual Boolean validIndex(const int axis, const int i)const;
		// Obtain world value along an axis for a position.
		virtual double XYtoWorld(const int axis,
					 const int x, const int y) const;
		// These effectively return value(0) & value(length()-1).
		virtual double minValue(const int axis)const;
		virtual double maxValue(const int axis)const;
		// Return pointer to a buffer containing a string
		// representing the value of the axis at the given position.
		virtual const char *valueStr(const int axis,
					     const double val)const;
		virtual const char *title()const;
		AvAccessor *accessor()const{return accessor_;}
	  private:
		AvAccessor	*accessor_;
		AvString	units_[2];
		void setUnits(const int i, AvString &units, AvString &abbrev);
	};
  private:
	void setSlice(const int slice);
	AvCLUImageSetter	*imageSetter_;
	int		sliceIndex_;
	AvAccessor	*accessor_;
	DECLARE_HEADER
};

#endif
