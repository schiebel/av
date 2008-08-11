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
//---------------------------------------------------------------------------
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvContour.h,v 19.0 2003/07/16 05:46:53 aips2adm Exp $
//
// $Log: AvContour.h,v $
// Revision 19.0  2003/07/16 05:46:53  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:38  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:07  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:16  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:08  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:07:57  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:40  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:04  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:57  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:39  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.2  1998/06/09 21:13:25  hr
// Removed unneeded semicolon from DECLARE_HEADER.
//
// Revision 9.1  1997/09/15 16:28:55  hr
// Added support for changing line width. (getLinewidth()).
//
// Revision 9.0  1997/08/25 21:25:46  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:43  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:09:21  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.3  1996/06/18  18:42:45  pixton
//  Copyright Update
//
//  Revision 1.2  1996/01/25  17:43:17  hr
//  Needed to add Graphics2D initializations.
//
//  Revision 1.1  1996/01/24  18:06:45  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvContour.h

Subclass of AvPolyline that generates contours of a raster.

Negative contours are dashed by default.

Each contour may have a different color with a maximum of 15.
Each contour may have a different linestyle with a maximum of 5 if dashing
of negative contours is not requested.
Contour lines may be labeled with the value of the contour.
Contour values may be either absolute values or fractions of the range
between the raster's min and max.

The raster is used to supply size and labeling information. The
contour is only drawn on the raster if they are children of overlapping
viewports.

*/

#ifndef AvContour_h
#define AvContour_h

#include <AvPolyline.h>
#include <AvContourInfo.h>
class AvRaster;
class AvContourInfo;

class AvContour : public AvPolyline {
  public:
	AvContour(AvRaster *, AvContourInfo *cinfo=NULL);
	// Change raster in mid stream. Does not change contour or color arrays
	void setRaster(AvRaster *);
	AvContourInfo *contourInfo()const {return info_;}
	void contourInfo(AvContourInfo *info);

	// Return the value of contour level.
	double getContourValue(const int level)const
				{return info_->getContourValue(level);}

	// Return the PGPLOT color value for the contour level.
	int getContourColor(const int level)const
			{return info_->getContourColor(level);}

	// Return the PGPLOT linestyle to use for the contour level.
	int getLinestyle(const int level)const
				{return info_->getLinestyle(level);}

	int getLinewidth(const int level)const
				{return info_->getLinewidth(level);}
#if 0
	// If TRUE (the default), negative contours will be drawn with dashes,
	// positive with solid. If FALSE, use value from setLinestyles().
	Boolean dashNegativeContours()const
			{return info_->dashNegativeContours();}
#endif
	// Current number of contour levels to draw.
	inline int numContourLevels()const
			{return info_->numContourLevels();}

	// If TRUE (default is FALSE, label the contour levels. This may not
	// work well if the raster is visible.
	Boolean labelContours()const
			{return info_->labelContours();}

	// Label spacing info (see PGCONL).
	void getLabelSpacing(const int level, int &minval, int &intval)
		{info_->getLabelSpacing(level, minval, intval);}
	// The string to use to format the contour labels. eg. "<%.2f>".
	char *getFormatString()const{ return info_->getFormatString();}
	// If TRUE, draw the contour levels. This is the default.
	Boolean drawContours()const {return info_->drawContours();}

	// If TRUE, draw a simple title.
	Boolean drawTitle()const {return info_->drawTitle();}

	Boolean levelsAreFractional()const
			{ return info_->levelsAreFractional();}

	// Normally, contours are drawn using a routine that supports
	// blanking. If blanking is not needed, better dashed lines may
	// result by disabling blanking. (PGCONB vs PGCONT).
	Boolean blankingEnabled()const {return info_->blankingEnabled();}

  public:	// These routines are called internally.
	virtual void draw(AvCanvas &);
	virtual void draw(AvCanvas *);
	inline Boolean haveContour()const{return haveContour_;}
	virtual void touch();	// Mark as modified.
	// Return the string to use for the level's label or NULL if none.
	// The return points to a static buffer that gets reused each call.
	char *getLabel(const int level)const;
	// Get rid of current contour.
	void purgePoints();
  protected:
	virtual ~AvContour();		// Use ref/unref.
	void generate(AvCanvas *c);	// Create contour vector list.
  private:
	DECLARE_HEADER
	AvRaster *raster_;
	Boolean	haveContour_;
	AvContourInfo	*info_;
	unsigned long infostamp_;	// Last "time" info_ changed.
};

#endif
