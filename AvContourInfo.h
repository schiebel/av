//# Copyright (C) 1995-2000 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvContourInfo.h,v 19.0 2003/07/16 05:46:55 aips2adm Exp $
//
// $Log: AvContourInfo.h,v $
// Revision 19.0  2003/07/16 05:46:55  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:40  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:09  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:17  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:15  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:59:02  hravlin
// Update copyright.
//
// Revision 14.0  2000/03/23 16:07:58  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:42  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:08  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:03  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:46  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.3  1997/09/17 14:42:37  hr
// Added support for emphasized contours.
//
// Revision 9.2  1997/09/15 16:35:12  hr
// Forgot an 'int' in lineWidth() declaration.
//
// Revision 9.1  1997/09/15 16:30:10  hr
// Added support for changing line width.
//
// Revision 9.0  1997/08/25 21:25:56  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:48  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/01/24 20:21:49  hr
// Changes to allow writing into the defaults database.
//
// Revision 7.2  1996/12/12 08:46:29  droberts
// Final update from monet archive.
//
// Revision 1.2  1996/11/05 21:12:07  hr
// Several changes to begin using AvConfigInfo.
//
// Revision 1.1  1996/07/11  21:31:21  pixton
// Automated Checkin
//
//  Revision 1.4  1996/06/18  18:42:46  pixton
//  Copyright Update
//
//  Revision 1.3  1996/01/31  14:56:36  hr
//  Changed default number of contours from 11 to 5.
//
//  Revision 1.2  1996/01/24  22:17:49  hr
//  Extended COLORSTYLE.
//
//  Revision 1.1  1996/01/24  18:07:15  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvContourInfo.h
Holds information describing how to draw contours such as the
contour levels, colors, line type, etc.

*/

#ifndef AvContourInfo_h
#define AvContourInfo_h
#include <AvResource.h>
#include <AvConductor.h>
#include <iostream.h>
#include <AvWPosition.h>
#include <AvIPosition.h>
#include <AvConfigInfo.h>

class LevelInfo;
class AvImageDataItem;

class AvContourInfo : public AvResource, public AvConductor {
  public:
	enum {Destroyed = AvConductor::Destroyed,
			ValueChanged=AvConductor::ValueChanged};
	AvContourInfo(const char *filename=NULL, const int nlevels=5);
	AvContourInfo(const AvContourInfo &);
	// Copy one to another optionally causing to to call its callbacks.
	static void copy(const AvContourInfo &from, AvContourInfo &to,
		const Boolean notify=TRUE);


	// Current number of contour levels to draw.
	inline int numContourLevels()const {return ncontours_;}

	/* Set contour levels.
	   If levelsArefractional is TRUE, level values are fractions (0..1) of
	   the raster range (min..max). AvContourInfo does not check to make
	   sure the values really are in the range of (0..1).
	   If save is TRUE, levels will be written to the defaults area.
	*/
	void setContourLevels(const AvWPosition &levels,
			      const Boolean save=TRUE);
	// Multiplier for levels.
	void scale(const double mult, const Boolean def=FALSE);
	double scale()const {return levelScale_;}
	// Return an array of numContourLevels() unscaled contour levels.
	// If levels is not NULL, it will be used, otherwise an array will be
	// allocated. (Use delete [] to free).
	double *getContourLevels(double *levels=NULL);
	// Return the value of contour level. value = array[level]*scale();
	double getContourValue(const int level)const;

	void generateColors(AvIPosition &colors)const;
	/* Set colors used for contours.
	   colors will be generated (max of 15). If colors.nelements() is
	   less than numLevels above, color values will be repeated.
	*/
	void setContourColors(const AvIPosition &colors,
			      const Boolean write=TRUE);
	/*  Create contour levels.
	   If numLevels > 0 it is used as the length. Otherwise the length
	   of levels is used.
	   If levelsArefractional is TRUE, level values are fractions (0..1) of
	   the raster range (min..max). AvContourInfo does not check to make
	   sure the values really are in the range of (0..1).
	*/
	void generateContourLevels(AvWPosition &levels, const int numLevels=0);
	void generateLineStyles(AvIPosition &lines)const;

	// Return the PGPLOT color value for the contour level.
	int getContourColor(const int level)const;
	int getLinewidth(const int level)const;

	// Return the PGPLOT linestyle to use for the contour level.
	int getLinestyle(const int level)const;

	// SOLID - draw all lines solid. DASHED - use value in LevelInfo.
	// (Which could be set to all solid).
	// DASHEDifNEGATIVE draw positive contours solid, and dash negative.
	enum LINESTYLE { SOLID, DASHED, DASHEDifNEGATIVE};
	LINESTYLE lineStyle()const {return lineStyle_;}
	void lineStyle(const LINESTYLE);
	int lineWidth()const {return lineWidth_;}
	void lineWidth(const int lineWidth);
	// Supported colors (Mixed means a different color/level.
	// (AvContourList uses these as indices, MIXED must be last).
	enum COLORSTYLE { UNKNOWN=-1, BLACK, WHITE, RED, GREEN, BLUE,
			  CYAN, MAGENTA, YELLOW, MIXED};
	COLORSTYLE colorStyle()const {return colorStyle_;}
	void colorStyle(const COLORSTYLE);

	Boolean emphasisEnabled()const {return emphasisEnabled_;}
	void emphasisEnabled(const Boolean enable);
	COLORSTYLE emphasisColor()const {return emphasisColor_;}
	void emphasisColor(const COLORSTYLE);
//	int emphasisOffset()const{return emphasisOffset_;}
//	void emphasisOffset(const int Offset);

	// Set line styles. Valid values are (1..5).
	void setLineStyles(const AvIPosition &styles,
			   const Boolean write=TRUE);

	// If TRUE (default is FALSE, label the contour levels. This may not
	// work well if the raster is visible.
	Boolean labelContours()const {return labelContours_;}
	void labelContours(const Boolean label);
	// Label spacing info (see PGCONL).
	// If the arrays are NULL, defaults will be supplied.
	void setLabelSpacing(const int levels,
				int *minint=NULL, int *intval=NULL);
	void getLabelSpacing(const int level, int &minval, int &intval);
	// The string to use to format the contour labels. eg. "<%.2f>".
	// fmt is copied to an internal array.
	void setFormatString(const char *fmt);
	char *getFormatString()const{ return fmt_;}
	// If TRUE, draw the contour levels. This is the default.
	Boolean drawContours()const {return drawContours_;}
	void drawContours(const Boolean draw);

	// If TRUE, draw a simple title.
	Boolean drawTitle()const {return drawTitle_;}
	void drawTitle(const Boolean draw);

	Boolean levelsAreFractional()const { return levelsAreFractional_;}
	void levelsAreFractional(const Boolean frac);
	// Normally, contours are drawn using a routine that supports
	// blanking. If blanking is not needed, better dashed lines may
	// result by disabling blanking. (PGCONB vs PGCONT).
	void enableBlanking(const Boolean enable);
	Boolean blankingEnabled()const {return blankingEnabled_;}
	// These are only used by some of the routines to help generate
	// default values.
	double min()const {return min_;}
	double max()const {return max_;}
	void setMinMax( const double min, const double max);
	inline Boolean validIndex(const int index)const
		{return ((index >=0) && (index < ncontours_)) ? TRUE : FALSE;}
	unsigned long timestamp()const {return timestamp_;}
	Boolean modifiedSince(const unsigned long ts)const
		{ return (timestamp_ > ts) ? TRUE : FALSE;}
	void	touch();		// Mark as modified & notify.
	Boolean equals(const AvContourInfo &)const;
	// Used by AvContourList to save immediate update toggle state.
	Boolean	immediateUpdate()const {return immediateUpdate_;}
	void immediateUpdate(const Boolean ud){immediateUpdate_ = ud;}
  protected:
	virtual ~AvContourInfo();	// Use ref/unref.
  private:
	void init(const char *fileName, const int nlevels);	// Initialize.
	void checkLevelArray(const int numLevels); // Make sure there is room.
  private:
	AvConfigInfo	*config()const{return cnfg_;}
	AvConfigInfo	*cnfg_;
	void	setD(const char *name, const double val,
		     const Boolean def=FALSE);
	void	setDL(const char *name, const AvWPosition &val,
		      const Boolean def = FALSE);
	void	setI(const char *name, const int val, const Boolean def=FALSE);
	void	setMappedInt(const char *name, const int val,
			     const AvConfigInfo::IntMap *map);
	void	setB(const char *name, const Boolean val);

	int	ncontours_, infolen_;
	LevelInfo	*info_;
	double	levelScale_;		// Multiplier for values.
	Boolean drawContours_;
	Boolean	levelsAreFractional_;	// values are % of raster range.
	LINESTYLE lineStyle_;
	int	  lineWidth_;
	COLORSTYLE colorStyle_;
	Boolean	labelContours_;
	Boolean drawTitle_;
	Boolean blankingEnabled_;
	Boolean	immediateUpdate_;	// Used by AvContourList.
	char	*fmt_;			// Format string to use for labels.
	////// Variables for contour emphasis.
	COLORSTYLE	emphasisColor_;
//	int		emphasisOffset_; // Width = 2*offset+lineWidth.
	Boolean		emphasisEnabled_;
	////
	double	min_, max_;
	unsigned long timestamp_;
};
#endif
