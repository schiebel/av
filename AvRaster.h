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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvRaster.h,v 19.0 2003/07/16 05:47:41 aips2adm Exp $
//
// $Log: AvRaster.h,v $
// Revision 19.0  2003/07/16 05:47:41  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:19  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:48  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:54  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:53  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:47  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:35  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:41  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:15  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:19  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/09 21:17:06  hr
// Removed unneeded semicolon from DECLARE_HEADER.
//
// Revision 9.0  1997/08/25 21:29:59  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:18:21  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1996/12/12 09:45:07  droberts
// Added HMS_RAD and DMS_RAD types to support radian datasets.
//
// Revision 1.4  1996/11/12 20:17:48  hr
// Removed some commented out code.
// Redefined getRegion().
//
// Revision 1.3  1996/09/20 20:56:37  pixton
// Av prefix added to IPosition, WPosition
//
// Revision 1.2  1996/09/20  18:41:29  hr
// Rewrote axisInfo to be usable with AvAccessor.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.7  1996/06/18  18:43:16  pixton
//  Copyright Update
//
//  Revision 1.6  1996/02/02  20:50:23  hr
//  Added {get,set}UnitString().
//
//  Revision 1.5  1996/01/08  16:26:44  hr
//  Added axisInfo and AvValueFormatter.
//
//  Revision 1.4  1995/10/04  21:13:37  hr
//  Added functions to supply value to use as blanked data.
//
//  Revision 1.3  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/04/24  18:40:57  hr
//  Added beginAccess, endAccess and getRow.
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvRaster.h

Display objects for 2D images.

AvRaster	An abstract object for displaying 2D objects.
		Data is retrieved a line at a time.
		Members that set values may not be functional in all
		subclasses.
		AvRaster does not define how the elements are stored.

		The element at (0,0) is in the lower left corner.
*/

#ifndef AvRaster_H
#define AvRaster_H

#include <AvGraphics2D.h>
#include <AvIPosition.h>

class AvCanvas;	// Don't use include file, causes problems.

class  AvRaster : public AvGraphics2D {
  public:
#if 0
	// Put n elements starting at position x/y. N=0 -> rest of Region.
	virtual void putRegion(float *data, int x=0, int y=0,
			unsigned int w=0, unsigned int h=0);
#endif
	// Get nrows (sub)rows of width width starting at position x/y.
	// Stride > 1 -> skip pixels. stride <= 0 is illegal.
	virtual void getRegion(float *data, const int x, const int y,
			       const unsigned int width,
			       const unsigned int nrows,
			       const int xstride=1, const int ystride=1);
	// Return one data element.
	virtual float getElement(const int x, const int y);
	/* Get one row of data.
	  num = 0 implies width()-x, stride =1 is get every element. 2 is
	  every other. -2 is every other element starting at x+num-1 and
	  going right to left. The number of elements returned will be
	  limited to one row.
	  NOTE: negative stride is not supported everywhere yet.
	*/
	virtual void getRow(float *row, const int x, const int y,
			const int num=0, const int stride=1);
	/* Some storage mechanisms require some setup before accessing the
	   data. The Canvas will call beginAccess() before calling getRegion
	   or getRow.
	   and will call endAccess() after the data has been accessed.
	*/
	virtual void beginAccess();
	virtual void endAccess();
	virtual float min()const;
	virtual void min(const float);
	virtual float max()const;
	virtual void max(const float);
	// Coerce x/y width/height to be valid.
	virtual void rangeCheck(int &x, int &y,
				unsigned int &w, unsigned int &h)const;
	// Size of raster in pixels.
	virtual unsigned int width()const;
	virtual unsigned int height()const;

	virtual void draw(AvCanvas&);
	virtual void draw(AvCanvas*);
	virtual Boolean isHit(AvCanvas &c, const AvAllotment &)const;
	// Is the point inside the raster's boundaries?
	virtual Boolean contains(const AvAllotment &)const;
	virtual void getBoundingBox(AvCanvas&, AvAllotment &);
	// Get/set value used for indicating blanked data. Must be a valid
	// float since on some systems NaN will cause problems.
	virtual float getBlankValue()const;
#ifdef AvACCESSOR_H
	enum	{XAXIS=AvAccessor::XAXIS, YAXIS=AvAccessor::YAXIS,
		 ZAXIS=AvAccessor::ZAXIS};
#else
	enum	{XAXIS=0, YAXIS=1, ZAXIS=2};
#endif

  public:
//////////////////////////////////////////////////////////////
	// Axis world coordinate information.
class axisInfo : public AvResource {
  public:
	axisInfo();
	axisInfo(axisInfo &);
	virtual ~axisInfo();

	//	Below, "axis" is XAXIS or YAXIS (0 or 1).
	// The axis label.
	virtual const char *label(const int axis)const;
	// The units for the axis
	virtual const char *unitString(const int axis)const;
	// How should axis data be displayed?
	enum PRINTFORMAT {UNKNOWN=-1,DEFAULT, HMS, DMS, HMS_RAD, DMS_RAD };
	virtual AvRaster::axisInfo::PRINTFORMAT
					printFormat(const int axis)const;
	// Number of indices along axis.
	virtual int length(const int axis)const;
	// TRUE if i is a valid index. (0..length()-1).
	virtual Boolean validIndex(const int axis, const int i)const;
	// Obtain world value along an axis for a position.
	virtual double XYtoWorld(const int axis, const int x, const int y)
									const;
	// These effectively return value(0) & value(length()-1).
	virtual double minValue(const int axis)const;
	virtual double maxValue(const int axis)const;
	// Return pointer to a buffer containing a string representing the
	// value of the axis.
	virtual const char *valueStr(const int axis, const double val)const;
	virtual const char *title()const;
  private:
};
//////////////////////////////////////////////////////////////

  public:
	axisInfo *axisinfo()const;
	void setAxisInfo(axisInfo *);
  protected:
	AvRaster();
	AvRaster(AvRaster &);
	~AvRaster();
	virtual void scaleBoundingBox(AvCanvas&, AvAllotment &);
  private:
	DECLARE_HEADER
	axisInfo *axisinfo_;
};
#endif
