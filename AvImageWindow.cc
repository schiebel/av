//# Copyright (C) 1995-2002 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvImageWindow.cc,v 19.0 2003/07/16 05:47:44 aips2adm Exp $
//
// $Log: AvImageWindow.cc,v $
// Revision 19.0  2003/07/16 05:47:44  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:21  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.3  2002/01/22 21:59:46  hravlin
// Added (uInt) cast for NSCALES. And a cast for ButtonMasks. (For 64 bit cmpls).
//
// Revision 17.2  2002/01/07 22:31:59  hravlin
// Removed check for __P.
//
// Revision 17.1  2002/01/03 22:07:02  hravlin
// Added support for OpenGL/WIREGL.
//
// Revision 17.0  2001/11/12 19:42:50  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:57  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:02  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:45:58  hravlin
// Changed some char * to const char *.
//
// Revision 14.0  2000/03/23 16:08:50  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.5  1999/09/28 21:54:44  hravlin
// Worked on color use in setInclude. Removed references to Gadgets.
//
// Revision 13.4  1999/09/27 15:50:40  hravlin
// Added busy indicator around printImage.
//
// Revision 13.3  1999/09/24 21:14:35  hravlin
// More work on print options: Filename is now from printOptions_, added
// user comments and other PostScript file comment changes.
//
// Revision 13.2  1999/09/15 21:29:48  hravlin
// Added new "Print Options" panels and printing to new PostScript driver.
//
// Revision 13.1  1999/08/25 19:47:29  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:40:38  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:47  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:24  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:29  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.2  1998/01/14 16:24:19  hr
// In mouseMoveCB(), added loop to ignore all but last motion notify event.
//
// Revision 9.1  1997/12/08 22:34:42  hr
// Added ability to draw a cross in drawBox().
//
// Revision 9.0  1997/08/25 21:30:14  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.6  1997/08/01 21:14:56  hr
// Changes to handle rewrite of AvPosition.
//
// Revision 8.5  1997/06/11 19:10:36  hr
// Including math.h brought the _P problem for g++.
// Removed two unused variables.
//
// Revision 8.4  1997/06/11 19:04:56  hr
// Added printing of some simple statistics.
//
// Revision 8.3  1997/05/20 17:41:02  hr
// Added the "Redisplay image" menu item to dump cache, etc. information
// when the user knows the image data have changed.
//
// Revision 8.2  1997/04/15 20:04:21  hr
// Added "Misc" menu and "Raw Position".
//
// Revision 8.1  1997/02/20 14:55:10  hr
// Changed setInclude() to ignore requests to change state if new is same
// as old.
//
// Revision 7.6  1997/02/05 17:41:25  hr
// Changed an unsigned/int comparision. Added calls to IVP's edit menus.
//
// Revision 7.5  1997/01/24 20:24:21  hr
// Added mouse tracker conductor.
// Moved Options:Setup menu to Edit. Added Axis and Wedge setup entries.
// Changed the way AvConfigInfo::writeOptions() gets called.
//
// Revision 7.4  1997/01/09 18:23:38  hr
// Changed init() to check for "DisplaySize" resource and to set "equalScales_"
// to FALSE if the initial window size is not square.
//
// Revision 7.3  1996/12/12 08:50:02  droberts
// Final update from monet archive.
//
// Revision 1.16  1996/11/12 15:45:48  hr
// Was overrunning character buffer in displayTitle().
//
// Revision 1.15  1996/11/11 17:59:46  hr
// Moved equalScales() to here from AvImageViewItem. Added code to
// set "X=Y" toggle if equalScales_ gets changed elsewhere.
//
// Revision 1.14  1996/11/11 16:07:31  hr
// Widespread changes to support independent scaling of X and Y axes.
//
// Revision 1.13  1996/11/06 17:02:01  hr
// Sun's 4.1.3 compiler complains about variables named "end" overriding
// variables of the same name in an outer scope.
//
// Revision 1.12  1996/11/05  21:15:51  hr
// Added menu options to allow user to save configuration settings.
//
// Revision 1.11  1996/10/02  17:22:52  hr
// In mouse movement routines, calls to viewPanel::setSlice needed say
// z value was SR.
//
// Revision 1.10  1996/10/02  16:57:38  hr
// Added setPosition(s) to set included windows to a new AvPosition.
//
// Revision 1.9  1996/09/27  17:04:49  hr
// Removed an unused routine.
//
// Revision 1.8  1996/09/27  16:40:35  hr
// Added a pointer to the imported accessor in order to keep track of
// whether AvCLUImageSetter had already disconnected from it.
//
// Revision 1.7  1996/09/25  21:30:46  hr
// Mouse/cursor handling routines were heavily modified. Several old routines
// were removed. Several variable's names were changed.
// Added code to support tracking of cursor in 3D.
// A couple of classes are no longer friends.
//
// Revision 1.6  1996/09/20  20:19:35  pixton
// Prefixed all classes with Av
//
// Revision 1.5  1996/09/20  18:40:23  hr
// Fixed a number of <xyz>Axis() -> <ZYZ>AXIS problems.
//
// Revision 1.4  1996/09/18  19:27:35  hr
// More integration with AvAccessor.
//
// Revision 1.3  1996/09/10  16:55:28  hr
// Initial changes for integration with AvAccessor & AvDataSet.
//
// Revision 1.2  1996/08/14  18:59:14  hr
// Changes to remove g++ warnings (from bglenden).
//
// Revision 1.1  1996/07/11  21:31:21  pixton
// Automated Checkin
//
//  Revision 1.39  1996/06/18  18:46:16  pixton
//  Copyright Update
//
//  Revision 1.38  1996/04/16  20:29:27  hr
//  Added more scale menu entries. Added call to setAtWindow().
//
//  Revision 1.37  1996/04/11  16:09:57  hr
//  Some minor changes to the scale menus.
//
//  Revision 1.36  1996/04/08  14:44:35  hr
//  Moved print and close image to new "File" menu.
//
//  Revision 1.35  1996/03/27  22:25:56  hr
//  Changed the way the SCALE menu is generated so that really non square
//  images will have a selection of scale factors.
//  If region() is called and the region is not set, the return parameters
//  are set to 0.
//
//  Revision 1.34  1996/03/20  17:35:44  hr
//  Changes to refer to AvXColorApp rather than AvApp.
//
//  Revision 1.33  1996/03/19  22:02:16  hr
//  Changed the print menu string to "Print Image".
//
//  Revision 1.32  1996/03/13  16:00:01  hr
//  Moved "Options" submenu to last.
//
//  Revision 1.31  1996/03/07  21:15:51  hr
//  The filename passed to AvPGCanvas is now retrieved from the imagesetter
//  rather than vItem_.
//
//  Revision 1.30  1996/02/29  17:26:34  hr
//  Added PRINT menu entry.
//
//  Revision 1.29  1996/02/23  17:08:13  hr
//  Replaced use of getRasterOffsets with getMargins.
//
//  Revision 1.28  1996/02/09  18:17:18  hr
//  Moved Region/Profile menu building & handling to AvImageViewPanel.
//
//  Revision 1.27  1996/02/08  18:15:53  hr
//  Added a separator to the show menu. init takes into account potential
//  axis offsets (perhaps pointlessly).
//
//  Revision 1.26  1996/02/05  21:33:05  hr
//  Removed "75%" scaling since the X Canvas can only scale in integral
//  amounts.
//
//  Revision 1.25  1996/02/02  15:35:45  hr
//  Redid Option menu. "Show" now controls all displays. "Setup" has been
//  added to support setup windows (eg. contour user interface).
//
//  Revision 1.24  1996/01/30  22:54:22  hr
//  Added Axis menu item.
//  Changed IJKToWindow and WindowToIJK to handle possible offset of raster
//  image.
//
//  Revision 1.23  1996/01/26  21:45:54  hr
//  raster_ wasn't being initialized to NULL.
//
//  Revision 1.22  1996/01/24  18:16:34  hr
//  Added support for contouring menus.
//  Ignore requests to replace raster_ with itself.
//
//  Revision 1.21  1995/11/13  22:07:34  hr
//  Under certain conditions (small fonts?) showImage() wasn't being called.
//  Now, setDisplayObject gets called when the AvCLUImage object gets
//  set.
//
//  Revision 1.20  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.20  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.19  1995/09/07  14:45:02  hr
//  Wasn't terminating the string copied from buttonName in processMenu.
//
//  Revision 1.18  1995/07/11  21:28:59  hr
//  Added more Scale menu items. Renamed "Info"/"Show" menu & submenus.
//  Added "Invisible" to "Region Draw Mode" menu and renamed the menu.
//  Added 'updateBox() & boxType_ to support disabling box drawing from
//  imageViewPanels toggles.
//  moveMouseCB now checks to make sure x/y are in bounds before doing anything.
//  IJKToWindow - added check for scalefactor > 1.
//
//  Revision 1.17  1995/06/16  18:00:05  hr
//  "Show Profile" menu item is only generated for multiplane datasets.
//  Let AvImageViewPanel hide profile/region when built.
//
//  Revision 1.16  1995/06/12  21:19:14  hr
//  Fixed ghost box problem (I hope).
//  Scale menu will only present 'reasonable' choices.
//  Added viewPanel() function.
//
//  Revision 1.15  1995/06/09  19:37:30  hr
//  Moved creation of the imageViewPanel here from AvImageView.
//  The AvImageViewPanel can be created either inside the imageWindow or in its
//  own shell.
//  The Scale factor is now displayed with the window title.
//
//  Revision 1.14  1995/05/24  14:59:00  hr
//  Worked on getting box drawing to work between windows and with different
//  scales. For now, drawBox takes IJK args.
//
//  Revision 1.13  1995/05/18  20:56:11  hr
//  Turned synchronized picking back on.
//  Changed "Include" to a submenu.
//  Added "Region Draw Mode".
//
//  Revision 1.12  1995/05/12  21:20:09  hr
//  Changes to data clip min/max are propagated to profile display.
//
//  Revision 1.11  1995/05/12  14:24:56  hr
//  Temporarily remove "Draw Mode 1".
//
//  Revision 1.10  1995/05/11  21:51:34  hr
//  Numerous changes dealing with window scaling:
//    Doesn't initiate a redisplay of the display list when window is resized
//    since the canvas will do it.
//    resizeCB may be called from the Canvas callback list.
//    If expose count is > 0, ignore.
//    If the resize callback gets recursively called, ignore.
//
//  Revision 1.9  1995/05/05  15:11:39  hr
//  Check for boxStart, boxEnd in resizeCB.
//
//  Revision 1.8  1995/05/05  14:52:00  hr
//  Added support for synchronized picking.
//
//  Revision 1.7  1995/04/25  16:52:02  hr
//  Added redraw flag to scale.
//
//  Revision 1.6  1995/04/14  19:25:15  hr
//  Changes to allow pixmap caching to work.
//
//  Revision 1.5  1995/04/07  21:06:18  hr
//  Need to ref/unref the canvas
//
//  Revision 1.4  1995/04/03  15:52:16  hr
//  Reset included flag when turning off include toggle
//
//  Revision 1.3  1995/03/30  22:21:56  hr
//  Renamed optionMenu in init to avoid name conflict.
//  Removed fallbacks variable from class.
//
//  Revision 1.2  1995/03/24  15:46:54  baker
//  *** empty log message ***
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <string.h>			// strrchr
#include <X11/cursorfont.h>
#include <math.h>			// for statistics.
#include <Xm/Xm.h>
#include <Xm/ArrowB.h>
#include <Xm/Label.h>
#include <Xm/DrawingA.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Separator.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>
#include <Xm/Command.h>
#include <Xm/MainW.h>

#include "AvCLUImage.h"
#include "AvCLUImageSetter.h"		// For menus
#include "AvImageDataPanel.h"
#include "AvImageView.h"
#include "AvImageViewPanel.h"
#include "AvImageWindow.h"
#include "AvXCanvases.h"
#ifdef AVOGL
#include "AvGLCanvas.h"
#endif

#include "AvPosition.h"
#include "AvPSCanvas.h"
#ifdef TCL
#include "AvConfigInfo.h"
#endif
#include "AvPrintOptions.h"
#include "AvPrintConfig.h"

// If defined, reports how long statistics calculations take.
//#define TIMEIT
#ifdef TIMEIT
#include <time.h>
#endif

////////////////////////////////////////////////////////////////
/* Object to do calculations on an image plane. It is defined here since
it is used here. It could be anywhere. If a GLISH interface is ever
needed, it will probably be put somewhere else. (Functions are at the
bottom of this file).
*/
// Perform a set of statistics on current image plane of an accessor.
// Calculations are currently done at creation time.
class AvStatistics : public AvResource
{
  public:
	// Creating the object generates the statistics.
	AvStatistics(AvAccessor *ac);
	AvStatistics(AvAccessor *ac,
		     const float clipMin, const float clipMax);
	AvAccessor *accessor()const{return accessor_;}
  class Stats {
    public:
	Stats();
	~Stats();
	// Do calculations for point at x, y.
	// (Currently unused since it takes longer).
	void calc(const float datum, const int x, const int y);
	void init(AvAccessor *ac, const float clipMin, const float clipMax);
	void init(AvAccessor *ac);
	// # of data points involved. (Width()*Height() - #blanked - #outside).
	void	numpoints(const int v){ npoints_ = v;}
	int	numpoints()const{return npoints_;}
	// Sum of values.
	void sum(const double v){ sum_ = v;}
	double sum()const{return sum_;}
	// Sum of square of values.
	void sumsqr(const double v){ sumsqr_ = v;}
	double sumsqr()const{return sumsqr_;}
	// 
	double rms()const;
	double average()const
		{return (numpoints()>0) ? sum()/numpoints() : 0.0 ;}
	Boolean checkClip()const{return checkClip_;}
	void checkClip(const Boolean cc){checkClip_ = cc;}
	void dataMin(const float v, const int x, const int y)
		{ dataMin_ = v; minWhere_(0) = x; minWhere_(1) = y;}
	void dataMax(const float v, const int x, const int y)
		{ dataMax_ = v; maxWhere_(0) = x; maxWhere_(1) = y;}

	void overcount(const int nover){overcount_ = nover;}
	int overcount()const {return overcount_;}
	void undercount(const int nunder){undercount_ = nunder;}
	int undercount()const {return undercount_;}

	float clipMin()const{return clipMin_;}
	float clipMax()const{return clipMax_;}
	float dataMin()const{return dataMin_;}
	float dataMax()const{return dataMax_;}
	AvString clipMinStr()const;
	AvString clipMaxStr()const;
	AvString dataMinStr()const;
	AvString dataMaxStr()const;
	AvIPosition whereMinIJK()const {return minWhere_;}
	AvIPosition whereMaxIJK()const {return maxWhere_;}
	friend ostream &operator<<(ostream &s, const AvStatistics::Stats &x);
    private:
	AvAccessor *accessor_;
	int	npoints_;	// # of data points in calculations.
				// (Width() x Height() - #ignored).
	int	overcount_,	// # of points outside range.
		undercount_;

	Boolean	checkClip_;	// Ignore clipMin/Max if true.
	float	clipMin_,	// Ignore values outside this range.
		clipMax_;

	float	dataMin_,dataMax_;	// min/max in region.
	AvIPosition minWhere_,		// Where in data set. (IJK coords).
		    maxWhere_;
	double	sum_;		// sum(D(i,j))
	double	sumsqr_;	// sum(D(i,j)^2)
	};
	// Display values.
	void	print(ostream &output);
  protected:
	~AvStatistics();
  private:
	void init(AvAccessor *ac, const float clipMin, const float clipMax);

	void	calc();		// Do the calculations.
	AvAccessor	*accessor_;
	void	reset(const float min, const float max);
	Stats	stats_[2];	// Only within clip min/max.
	int	blankcount_;	// # of blanked pixels.
};

////////////////////////////////////////////////////////////////

// If the scale factor is > than this, a box will be drawn around the data
// point as the cursor is drug around the image. Otherwise, it is only drawn
// when the mouse button is released.
static const float MinBoxScale = 6.0;

const char * AvImageWindow::className () const
{
  return ("AvImageWindow");
}

#if 0
// Use of this constructor may result in problems since AvImageWindow
// now assumes it has vItem_ during construction.
AvImageWindow::AvImageWindow
(char * appClass, int & argc, String * argv, String * fallbacks)
: AvDisplayWindow (appClass, argc, argv, fallbacks)
{
cerr << "AvImageWindow::AvImageWindow called with NULL ImageViewItem.\n";
  vItem_ = NULL;
  init ();
}
#endif

// Which accessor actions we're interested in.
static const int ACCMASK = AvAccessor::DEFAULTMASK | AvDataSet::DEFAULTMASK
			 | AvAccessor::PLANE;

// 'v' should not be NULL.
AvImageWindow::AvImageWindow(Widget parent, int buildInsideParent,
    AvImageViewItem * v): AvDisplayWindow (parent, buildInsideParent)
{
	vItem_ = v;
	// Connect to accessor.
	accessor_ = vItem_->accessor();
	accessor()->ref();
	accessor()->addCallback(&AvImageWindow::accessorCB_,
				 ACCMASK, this, NULL);
	init();
}

// List of possible menu scale factors. A separator is inserted where there
// is a NULL. Not all of these will be used since some would result in
// an image of too weird a size.
static const char *scales[] = {
	"10000%","9000%", "8000%", "7000%", "6000%", "5000%", "4000%",
	"3200%", "2000%", "1600%", "1200%", "1000%", "800%", "600%", "500%", 
	"400%", "300%", "200%", "100%",
	NULL,
	"50%", "33%", "25%", "12.5%", "10%", "6.25%", "5%"
	};
#define NSCALES (uInt)(sizeof(scales)/sizeof(*scales))
static const char *EQUALSCALES = "X = Y";

// Minimum size (in pixels) we will allow menu to scale an image.
// Minimum value is probably 1.
static const int minSize = 64;
static	     int maxWidth = 0;		// Filled in later.
static	     int maxHeight = 0;		// Filled in later.

static const char *PRINT = "Print Image";
// Flush caches, etc. Then redisplay. Used if user knows data file has changed.
static const char *REDISPLAY = "Redisplay Image";
static const char *RAWPOSITION = "Raw Position";
static const char *PRINTSTATS = "Print Statistics";

#ifdef TCL
static const char *SAVEINFO = "Update file's rc";
static const char *SAVEALLINFO = "Create filerc.list";
#endif
static const char *PRINTOPTIONS = "Print Options";

  // Build the scale, xscale or yscale menus.
void AvImageWindow::buildScaleMenu(AvMenu *menu, const float estScale,
			   const int Size, const int maxSize, const int option)
{Boolean needSep = FALSE;	// To keep a separator from being last on menu.

	for(int i=0; i< (int)NSCALES; i++)
	if(scales[i] == NULL)
		needSep = TRUE;
	else
	{ Boolean needItem;
	  // Force the scale factor to be included if it is close to the
	  // estimated initial scale size.
	  float scl = atof(scales[i])*.01;
	  float delta = scl - estScale;
	  	if(delta < 0.0) delta = -delta;
		needItem = (delta < .01) ? TRUE: FALSE;

	  int size = (int)((scl*Size) + 0.5);
	  // Only include scale factor if it will generate a reasonable size
	  // image or it is a likely candidate for the initial scale factor.
	  if(((size >= minSize) && (size <= maxSize)) || needItem)
	  {	if(needSep)
		{	needSep = FALSE;
			menu->add ("", SEPARATOR);
		}
		menu->add (scales[i], PUSH_BUTTON, option);
	  }
	}
}

void AvImageWindow::init (int makeMenus)
{
  int marginWidth=0, marginHeight=0;
  xScaleFactor_ = 1.0;
  yScaleFactor_ = 1.0;
  equalScales_ = TRUE;	// X & y scales are initially forced to be the same.
  equalScalesW_ = NULL;

  //			All of these are in SR coordinates.
  // Currently drawn box.
  boxStart_.x = boxStart_.y = boxEnd_.x = boxEnd_.y = 0;
  cursor_.x = cursor_.y = 0;	// Cursor location
  included_ = FALSE;
  track3D_ = TRUE;
  boxStarted_ = FALSE;
  boxDrawn_ = FALSE;
  boxType_ = NoBox;
  drawCross_ = FALSE;		// Don't draw cross in cursor/region box.
  // CENTER is not implemented yet.
  regionType_ = CORNERS;	// Default region type.
  viewPanel_ = NULL;
  raster_ = NULL;
  printConfig_ = NULL;
  printOptions_ = new AvPrintOptions(accessor()->dataset()->fileName(),
				     "print");
  printOptions_->ref();

  // did the user set the width and height in the resources file?
  Dimension ww = getIntegerResource ("DisplayWidth");
  Dimension hh = getIntegerResource ("DisplayHeight");
  Dimension sz = getIntegerResource ("DisplaySize");
  if(sz > 0)	// Overrides width & height
	hh = ww = sz;

  // if not, set to 256 by 256
  if (ww == 0) ww = 256;
  if (hh == 0) hh = 256;

  if(ww != hh)
	equalScales_ = FALSE;

  if (makeMenus) {
    AvMenu *file_Menu = new AvMenu(7);
    file_Menu->add (PRINT, PUSH_BUTTON, SV_OPTION);
    file_Menu->add (REDISPLAY, PUSH_BUTTON, SV_OPTION);
#ifdef TCL
    file_Menu->add ("", SEPARATOR);
    file_Menu->add (SAVEINFO, PUSH_BUTTON, SV_OPTION);
    file_Menu->add (SAVEALLINFO, PUSH_BUTTON, SV_OPTION);
#endif
    file_Menu->add ("", SEPARATOR);
    file_Menu->add ("Close View", PUSH_BUTTON, SV_OPTION);

    editMenu = new AvMenu(10);
    AvCLUImageSetter::buildSetupMenu(editMenu, SV_SETUP);
    AvImageViewPanel::buildEditMenu(editMenu, SV_SETUP, accessor_);
    editMenu->add(PRINTOPTIONS, PUSH_BUTTON, SV_SETUP);

    // The scale menu is built using what are considered the most useful
    // entries of the above list. The scaling that is assumed to be the
    // initial value is always included.
    scaleMenu_ = new AvMenu (NSCALES+4);
    xScaleMenu_ = new AvMenu (NSCALES+2);
    yScaleMenu_ = new AvMenu (NSCALES+2);

    scaleMenu_->add (EQUALSCALES, TOGGLE_BUTTON, SV_SCALE, NULL, equalScales_);
    scaleMenu_->add ("X", SUBMENU_BUTTON, 0, xScaleMenu_);
    scaleMenu_->add ("Y", SUBMENU_BUTTON, 0, yScaleMenu_);
    scaleMenu_->add ("", SEPARATOR);

    // Menus won't scale an image to be greater than about 1 screen size.
    maxWidth = WidthOfScreen(XtScreen(parentWidget()));
    maxHeight = HeightOfScreen(XtScreen(parentWidget()));
    int width = vItem_->getCLUImage()->width();
    int height = vItem_->getCLUImage()->height();

    float estXscale = (float)ww/(float)width;   // Guess of initial scale.
    float estYscale = (float)hh/(float)height;  // Guess of initial scale.
    float estScale = (width+height)/2.0;

    buildScaleMenu(scaleMenu_, estScale, (width+height)/2,
		   (maxWidth+maxHeight)/2, SV_SCALE);

    buildScaleMenu(xScaleMenu_, estXscale, width, maxWidth, SV_XSCALE);
    buildScaleMenu(yScaleMenu_, estYscale, height, maxHeight, SV_YSCALE);

  // Add in any offsets (due to the display of the axis).
  vItem_->getCLUImage()->getMargins(marginWidth, marginHeight);
  ww += marginWidth;
  hh += marginHeight;

  ////////////////////////////////////////////////////////////////
  //			Build other menus			//
  ////////////////////////////////////////////////////////////////
    AvMenu * optionMenu0 = new AvMenu (10);

  AvMenu *showMenu = new AvMenu(16);
  optionMenu0->add ("Show", SUBMENU_BUTTON, 0, showMenu);

  // The profile and region displays can be part of the image window or
  // separate. The related menus are built to reflect that.
  if((panelIsInside_ = AvImageView::buildInside()))
  {  AvImageViewPanel::buildShowMenu(showMenu, SV_OPTION, vItem_);
  }
  else
     showMenu->add ("Show View Info", TOGGLE_BUTTON, SV_OPTION);

  showMenu->add ("", SEPARATOR);
  // Let the image setter add its own menu items.
  AvCLUImageSetter::buildShowMenu(showMenu, SV_SHOW);

    // (Un)Include all windows.
    AvMenu* includeMenu = new AvMenu(6);
	includeMenu->add("This", PUSH_BUTTON, SV_OPTION);
	includeMenu->add("All", PUSH_BUTTON, SV_OPTION);
	includeMenu->add("None", PUSH_BUTTON, SV_OPTION);
	includeMenu->add("Invert", PUSH_BUTTON, SV_OPTION);
	includeMenu->add("", SEPARATOR);
	includeMenu->add ("Track 3D", TOGGLE_BUTTON, SV_OPTION, NULL,
			  track3D());

    optionMenu0->add ("Include", SUBMENU_BUTTON, 0, includeMenu);
    // Different line drawing styles for region.
    AvMenu* drawMenu = new AvMenu(5);
     drawMenu->add("Normal", PUSH_BUTTON, SV_OPTION);
     drawMenu->add("Mode 1", PUSH_BUTTON, SV_OPTION);
     drawMenu->add("Mode 2", PUSH_BUTTON, SV_OPTION);
     drawMenu->add("Invisible", PUSH_BUTTON, SV_OPTION);
     drawMenu->add("Cross", TOGGLE_BUTTON, SV_OPTION);
    optionMenu0->add ("Box Draw Mode", SUBMENU_BUTTON, 0, drawMenu);

    AvMenu *miscMenu = new AvMenu(4);
    miscMenu->add(RAWPOSITION, TOGGLE_BUTTON, SV_OPTION, NULL, 0);
    miscMenu->add(PRINTSTATS, PUSH_BUTTON, SV_OPTION);


    optionMenu0->add ("Misc", SUBMENU_BUTTON, 0, miscMenu);

    AvMenu * mainMenu = new AvMenu (4);
    mainMenu->add ("File", SUBMENU_BUTTON, 0, file_Menu);
    mainMenu->add ("Edit", SUBMENU_BUTTON, 0, editMenu);
    mainMenu->add ("Scale", SUBMENU_BUTTON, 0, scaleMenu_);
    mainMenu->add ("Options", SUBMENU_BUTTON, 0, optionMenu0);

    makeMainMenu (mainMenu);
  }

  XtVaSetValues (parentWidget_,
		 XmNcolormap, AvXColorApp::colormap(),
		 XmNallowShellResize, TRUE,
		 NULL);

  // Create the info panel either inside this window's shell or in its own.
  if(AvImageView::buildInside())
  {	Widget parts = XtVaCreateManagedWidget
	("ImageWindow", xmFormWidgetClass, baseWidget_,
	XmNspacing, 3,
	XmNmarginTop, 10,
	XmNmarginWidth, 8,
	XmNleftAttachment, XmATTACH_FORM,
	XmNrightAttachment, XmATTACH_FORM,
//	XmNtopAttachment, XmATTACH_FORM,
//	XmNbottomAttachment, XmATTACH_FORM,
	NULL);

	// The frame that goes around the data panel.
	Widget infoFrame = XtVaCreateManagedWidget("infoFrame",
	 xmFrameWidgetClass, parts, 
	 XmNleftAttachment, XmATTACH_FORM,
	 XmNrightAttachment, XmATTACH_FORM,
	 XmNtopAttachment, XmATTACH_FORM,
	 NULL);

	if(vItem_ != NULL)
	{	// The panel that shows info: region, profile, etc.
		viewPanel_ = new AvImageViewPanel(infoFrame, TRUE, *vItem_);
		viewPanel_->show();		// Show panel.
	}

	// The frame that goes around the canvas.
	workArea_ = XtVaCreateManagedWidget("DispWin2WorkArea",
		xmFrameWidgetClass, parts, 
	XmNleftAttachment, XmATTACH_FORM,
	XmNrightAttachment, XmATTACH_FORM,
	XmNtopAttachment, XmATTACH_WIDGET,
	XmNtopWidget, infoFrame,
	XmNbottomAttachment, XmATTACH_FORM,
	// If these are set, aipsview under Linux can't resize.
	// (eg. profiles or scale)
    //	XmNwidth, ww,
    //	XmNheight, hh,
	NULL);
  }
  else	// Build imageViewPanel in its own shell.
  {
	if(vItem_ != NULL)
	   viewPanel_ = new AvImageViewPanel(parentWidget_, FALSE, *vItem_);

	// The frame that goes around the canvas.
	workArea_ = XtVaCreateManagedWidget("DispWin2WorkArea",
		xmFrameWidgetClass, mainWindow_,
	XmNleftAttachment, XmATTACH_FORM,
	XmNrightAttachment, XmATTACH_FORM,
	XmNtopAttachment, XmATTACH_FORM,
	XmNbottomAttachment, XmATTACH_FORM,
	NULL);
  }

  // make the X canvas within drawing frame
  int i = 0;
  Arg args[10];

#if 0
  XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
  XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
  XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
  XtSetArg(args[i], XmNbottomAttachment, XmATTACH_FORM); i++;
#endif

  XtSetArg(args[i], XmNwidth, ww); i++;
  XtSetArg(args[i], XmNheight, hh); i++;

#ifdef AVOGL
  AvGLCanvas *canvas = new AvGLCanvas
    ("DisplayWin2Canvas", workArea_, AvXColorApp::palette(), args, i);
  canvas->ref();
#else
  AvXWindow *canvas = new AvXWindow
    ("DisplayWin2Canvas", workArea_, AvXColorApp::palette(), args, i);
  canvas->ref();
#endif
  canvas->setRasterCacheParameters(TRUE, 128);	// Enable Raster caching.

  canvas_ = canvas;
  drawArea_ = canvas->widget();

  XtVaSetValues(workArea_, XmNworkWindow, drawArea_, NULL);

  XtAddCallback (drawArea_, XmNexposeCallback,
		 (XtCallbackProc) &AvImageWindow::redrawCB,
		 (XtPointer) this);

  XtManageChild (baseWidget_);

  XtVaSetValues (parentWidget_, XmNminWidth, 172, NULL);

  XGCValues values;
  // fill in foreground and background
  XtVaGetValues (drawArea_,
		 XmNforeground, &values.foreground,
		 XmNbackground, &values.background,
		 XmNheight, &windowHeight_,
		 XmNwidth, &windowWidth_,
		 NULL);

  values.foreground = values.foreground ^ values.background;
  values.line_style = LineSolid;
  values.function = GXinvert;
  gc_ = XtGetGC (drawArea_,
		GCForeground | GCBackground | GCFunction | GCLineStyle,
		&values);

  feedbackCursor_ = XCreateFontCursor (display_, XC_crosshair);

  if(!externalDrawingArea())
	  XtAddCallback (drawArea_, XmNresizeCallback, 
			 (XtCallbackProc) &AvImageWindow::resizeCB,
			 (XtPointer) this);

  if(vItem_ != NULL)
  {	displayTitle ();
	viewPanel_->setTitle (vItem_->label());
  }
  activateFeedbackHandlers ();
  tracker_ = new AvConductor();
}

static const char * TITLEFMT="%s  %sx%s";

void AvImageWindow::displayTitle()
{
	if(vItem_ == NULL)
		return;

	float xScale = xScaleFactor_*100;
	float yScale = yScaleFactor_*100;
	char xbuf[64], ybuf[64];
	if((float)(int)xScale == xScale)
		sprintf(xbuf, "%d%%", (int)xScale);
	else
		sprintf(xbuf, "%.1f%%", xScale);

	if((float)(int)yScale == yScale)
		sprintf(ybuf, "%d%%", (int)yScale);
	else
		sprintf(ybuf, "%.1f%%", yScale);

	// Currently, some datasets return the path with the fileName() call.
	// So make sure there is no path name.
	const char *ptr = strrchr(vItem_->label(), '/');
	if(ptr == NULL)
		ptr = vItem_->label();
	else
		ptr += 1;

	char *title = new char[ strlen(vItem_->label()) +
				strlen(xbuf) + strlen(ybuf) +
				strlen(TITLEFMT) + 4];		
	sprintf(title, TITLEFMT, ptr, xbuf, ybuf);

	setTitle(title);
	delete [] title;
}

// Handle resize events. Called by resizeCB.
void AvImageWindow::resize (Widget )
{
  // Resizing erases the box
   boxDrawn_ = FALSE;
  // When the window gets resized, we need to ensure that the raster gets
  // scaled to its best fit.
  // Pick up new size.

  if(externalDrawingArea())
  {	windowWidth_ = canvas()->width();
	windowHeight_ = canvas()->height();
  } else
	XtVaGetValues(drawArea_, XmNheight, &windowHeight_,
		      XmNwidth, &windowWidth_, NULL);

#if 0
  printf("AvImageWindow::resize: width %d, height %d\n",
		windowWidth_, windowHeight_);
#endif

  vItem_->fitRasterToWindow ();

  // Then, the window needs to change size to fit the raster.
  // Also, redraw the image and any box.
  vItem_->fitWindowToRaster (TRUE);
}

void AvImageWindow::resizeCB (Widget w, AvImageWindow * win,
						XmDrawingAreaCallbackStruct *)
{
  XtRemoveCallback (w, XmNresizeCallback, 
		    (XtCallbackProc) &AvImageWindow::resizeCB,
		    (XtPointer) win);

  win->resize(w);
  XtAddCallback (w, XmNresizeCallback, 
		 (XtCallbackProc) &AvImageWindow::resizeCB,
		 (XtPointer) win);
}

// Set/clear the included flag.
void AvImageWindow::setInclude(const Boolean include)
{
	// Ignore if trying to set to current state.
	if(include == included_)
		return;

	if(include)
	{    // save the current color
		XtVaGetValues(mainMenuBar_,
			  XmNbackground, &menuBackgroundColor_,
			  NULL);
		// Change the background color to signify "Included"
		// Child buttons should not be affected since they are
		// Widgets rather than gadgets.
		Pixel incbackground = AvApp::whitePixel();
		if(incbackground == menuBackgroundColor_)
			incbackground = AvApp::blackPixel();

		XtVaSetValues(mainMenuBar_,
			XmNbackground, incbackground,
			NULL);

		// set the "include" flag
		included_ = TRUE;
	}
	else
	{
	    // change the color back to its original state
	    XtVaSetValues (mainMenuBar_,
			   XmNbackground, menuBackgroundColor_,
			   NULL);
	    // reset the "include" flag
	    included_ = FALSE;
	}
}

// Set/clear the included flag for all windows.
void AvImageWindow::setIncludeList(const IncludeMode mode)
{
AvImageViewItem **list;
int listlen;

	if(mode == INCTHIS)
	{	setInclude( ! include());
		return;
	}

	list = AvImageView::instance()->getImageViewList(listlen,
						AvImageView::ALL);
	for(int i=0; i< listlen; i++)
	{AvImageWindow *win = list[i]->getImageWindow();
		switch (mode) {
		case INCALL:
			win->setInclude(TRUE);
			break;
		case INCINVERT:
			win->setInclude(! win->include());
			break;
		case INCNONE:
			win->setInclude(FALSE);
			break;
		default:;
		}
	}
	delete [] list;
}

// Change the way region lines are drawn.
void AvImageWindow::setRegionDrawMode(const DrawMode mode)
{
XGCValues values;
uLong	flags;

	switch(mode) {
	case NORMAL:
		values.function = GXinvert;
		flags = GCFunction;
		break;
	case MODE1:
		values.foreground = 0x5555;
		values.function = GXxor;
		flags = GCFunction | GCForeground;
		break;
	case MODE2:
		values.foreground = 0xAAAA;
		values.function = GXxor;
		flags = GCFunction | GCForeground;
		break;
	case MODEINVISIBLE:
		values.foreground = 0x0;
		values.function = GXxor;
		flags = GCFunction | GCForeground;
		break;
	default:
		flags = 0;
	}
	if(flags > 0)
	{	unDrawBox();
		XChangeGC(display_, gc_, flags, &values);
		drawBox();
	}
}

// Convert a scale menu selection to a floating point number.
float AvImageWindow::convertScaleMenu(const char *buttonName)
{char scaleStr[16];

	// strip away the '%' character
	int len = (int)strlen(buttonName)-1;	// Length w/o '%'.
	strncpy (scaleStr, buttonName, len);
	scaleStr[len] = '\0';
	float sc;
	sc = 0.01 * atof (scaleStr);

	// 33% needs a little help...
	if (!strcmp ("33%", buttonName)) sc = 1.0 / 3.0;
	return sc;
}

// Use the value from processScaleMenu to change the current scale.
void AvImageWindow::updateScale(const float xscl, const float yscl)
{	// is this really a change?
	if((xscl != xScaleFactor_) || (yscl != yScaleFactor_))
	{ scale (xscl, yscl);
	  if(externalDrawingArea())
		vItem_->fitRasterToWindow(TRUE);
	  else
		vItem_->fitWindowToRaster();
	}
}

void AvImageWindow::processMenu (char * buttonName, int choice, Widget mb)
{
  switch (choice){
  case SV_OPTION:
    if (!strcmp ("Close View", buttonName))
      vItem_->deleteView ();
    else
      // (Only one of "Show View Info" and "Show Region"/"Show Profile" will
      // exist at once.)
      if (!strcmp ("Show View Info", buttonName))
	if (XmToggleButtonGetState (mb)) {
	  activateFeedbackHandlers ();
	  viewPanel_->show ();
	}
	else {
	  deactivateFeedbackHandlers ();
	  viewPanel_->hide();
	}
    else
      if( viewPanel_->processShowMenu(buttonName, mb))
		/* Dummy */;
      else
	if (!strcmp ("Include", buttonName)) 
		setInclude(XmToggleButtonGetState (mb));
      else
	// Line drawing mode.
	if (!strcmp ("Normal", buttonName)) 
		setRegionDrawMode(NORMAL);
	else
	if (!strcmp ("Mode 1", buttonName)) 
		setRegionDrawMode(MODE1);
	else
	if (!strcmp ("Mode 2", buttonName))
		setRegionDrawMode(MODE2);
	else
	if (!strcmp ("Invisible", buttonName))
		setRegionDrawMode(MODEINVISIBLE);
	else
	if (!strcmp ("Cross", buttonName))
	{	unDrawBox();
		drawCross_ = XmToggleButtonGetState(mb);
		drawBox();
	}
	else
	// Change include flags for all windows.
	if (!strcmp ("This", buttonName))	// Toggle this window.
		setIncludeList(INCTHIS);
	else
	if (!strcmp ("All", buttonName))
		setIncludeList(INCALL);
	else
	if (!strcmp ("Invert", buttonName))
		setIncludeList(INCINVERT);
	else
	if (!strcmp ("None", buttonName))
		setIncludeList(INCNONE);
	else
	if (!strcmp (RAWPOSITION, buttonName))
		viewPanel_->showRawPosition(XmToggleButtonGetState(mb));
	else
	if (!strcmp (PRINTSTATS, buttonName))
	{ AvStatistics *stats = new AvStatistics(accessor_);
		stats->ref();
		 stats->print(cout);
		stats->unref();
	}
	else
	if (!strcmp (PRINT, buttonName))
		printImage();
	else
	if(!strcmp(REDISPLAY, buttonName)) // Flush caches, etc. Then redisplay
	{AvImageDataItem *idi = vItem_->dataItem();
		idi->dataModified();
		idi->updateViews();
	}
#ifdef TCL
	else
	if (!strcmp (SAVEINFO, buttonName))
	{ // Update configuration information for current file.
	   const char *fn = vItem_->dataItem()->name();
		AvConfigInfo::writeOptions(AvConfigInfo::getRCName(fn).chars(),
					   fn);
	}
	else
	if (!strcmp (SAVEALLINFO, buttonName))
	{  // Save all configuration information for current file.
	   const char *fn = vItem_->dataItem()->name();
	   AvString ofn = AvConfigInfo::getRCName(fn);
		ofn += ".list";
		cout << "Writing to " << ofn << endl;
		AvConfigInfo::writeOptions(ofn.chars(), fn, TRUE);
	}
#endif
    break;
  case SV_SCALE:
	if(strcmp(EQUALSCALES, buttonName) == 0)
	{Boolean eql = XmToggleButtonGetState(mb);
		equalScales(eql);
		break;
	}
	else
	{float scl = convertScaleMenu(buttonName);
		equalScales(TRUE);
		updateScale(scl, scl);
	}
    break;
  case SV_XSCALE:
	{float scl = convertScaleMenu(buttonName);
		equalScales(FALSE);
		updateScale(scl, yScaleFactor_);
	}
    break;
  case SV_YSCALE:
	{float scl = convertScaleMenu(buttonName);
		equalScales(FALSE);
		updateScale(xScaleFactor_, scl);
	}
    break;
  case SV_SHOW:
	// Let the imagesetter handle menu processing.
	raster_->imageSetter()->processShowMenu(buttonName, mb);
    break;
  case SV_SETUP:
		if(strcmp(buttonName, PRINTOPTIONS) == 0)
			showPrintOptions();
		else
		if(!viewPanel_->processEditMenu(buttonName, mb))
		  raster_->imageSetter()->processSetupMenu(buttonName, mb);
    break;
  default:
    AvApp::processMenu (buttonName, choice, mb);
  }
}

// (Un)Display the "Print Options" edit menu selection.
void AvImageWindow::showPrintOptions()
{
	// Will be NULL on first access.
	if(printConfig_ == NULL)
	{	printConfig_ =
			new AvPrintConfig(accessor()->dataset()->fileName(),
				AvImageView::instance()->baseWidget(),
				printOptions_);
		// Needed to delay setting a filename until there was a raster.
		////////////////////////////////////////////////////////////////
		AvCLUImageSetter *is = raster_->imageSetter();
		AvString fileName = is->getPrintName();
		printOptions_->defaultFilename_.setValue(fileName, False);
		if(printOptions_->isEPS())
			fileName += ".eps";
		else
			fileName += ".ps";
		// Insert filename if there wasn't something in an rc file,
		// but don't update anything else.
		if(printOptions_->filename_.getValue().length() == 0)
			printOptions_->filename_.setValue(fileName, False);
		////////////////////////////////////////////////////////////////
	}
	if(printConfig_->isUp())
		printConfig_->popdown();
	else
		printConfig_->popup();
}

// Print the image.
void AvImageWindow::printImage()
{ AvCLUImageSetter *is;

	if( (raster_ == NULL) || ((is = raster_->imageSetter())==NULL))
		return;

	AvXColorApp::setBusy();
	////////////////////////////////////////////////////////////////
	// Come up with a filename.
	AvString fileName = printOptions_->filename_.getValue();
	if(fileName.length() == 0)	// User never opened options panel.
	{	fileName = is->getPrintName();
		if(printOptions_->isEPS())
			fileName += ".eps";
		else
			fileName += ".ps";
		printOptions_->filename_.setValue(fileName, False);
	}
	////////////////////////////////////////////////////////////////
	// paper type, eps or not, portrait or landscape.
	AvPSDriver::MediaSize mediasize =
		(AvPSDriver::MediaSize)printOptions_->paperSize_.getValue();
	AvPSDriver::Layout layout =
		(AvPSDriver::Layout)printOptions_->layout_.getValue();

	AvPSDriver::PSInfo *psinfo = new AvPSDriver::PSInfo();

	////////////////////////////////////////////////////////////////
	// Print a bunch of file info as PostScript comments.
	char buf[1024];
	AvString cmt;
	AvAccessor *ac = accessor();
	cmt = "\t\t";
	cmt += ac->dataset()->fileName();
	cmt += "\n";
	sprintf(buf, "Data Min: %.5g, Data Max: %.5g (%s)\n",
			ac->dataMin(),  ac->dataMax(), ac->dataUnits());
	cmt += buf;

	sprintf(buf, "Clip Min: %.5g, Clip Max: %.5g\n",
			ac->clipMin(), ac->clipMax());
	cmt += buf;

	int rank = ac->rank();
	AvWPosition minw = ac->minWorld();
	AvWPosition maxw = ac->maxWorld();
	AvIPosition shape = ac->shape();
	int Indexing = AvImageView::oneIndexing();
	sprintf(buf, "Axes and plane are numbered starting from %d.\n",
		Indexing);
	cmt += buf;

	sprintf(buf, " Axis\tLength\t    Name\t\tFrom\t\t\tTo\n");
	cmt += buf;
	for(int i=0; i< rank; i++)
	{	sprintf(buf,
			"%3d\t%4d\t%8s\t%16s\t%16s\n",
			i + Indexing, shape(i), ac->axisName(i),
			ac->axisValueUnits(i,minw(i)).chars(),
			ac->axisValueUnits(i,maxw(i)).chars()
			);
		cmt += buf;
	}

	if(rank > 2)
	{int planenum = ac->getPlane();
	 AvWPosition pos = ac->XYZToWorld(0, 0, planenum);

		sprintf(buf, "\nPlane %3d %s\n", planenum + Indexing,
			ac->axisValueUnits(2, pos(2)).chars());
		cmt += buf;
	}

	sprintf(buf, "%d colors are available for displaying image.\n",
		AvXColorApp::palette()->availableColors());
	cmt += buf;
	if(printOptions_->smoothImage_.getValue())
		cmt += "Image pixels will be smoothed.\n";
	{	const AvPSDriver::PageInfo *pageinfo =
			AvPSDriver::pageInfo(mediasize);
		cmt += "Output was scaled for ";
		cmt += pageinfo->alias;
		cmt += " ("; cmt += pageinfo->description; cmt += ").\n";
	}

	if(!printOptions_->comment_.getValue().empty())
	{	cmt += "\n Begin User Comments.\n";
		cmt += printOptions_->comment_.getValue();
		cmt += "\n End User Comments.\n";
	}

	psinfo->Title(fileName);
	psinfo->Comment(cmt);

	////////////////////////////////////////////////////////////////

	AvPSDriver *psd = new AvPSDriver(fileName.chars(), mediasize, layout,
					 psinfo);


	////////////////////////////////////////////////////////////////
	// Scale factor to go from window to page.
	double userScale = printOptions_->scale_.getValue();
	float scale;
	float psH, psW;
		psd->pageSize(psW, psH);

	// Choose the scaling that will scale the image to the smaller
	// of the output's width or height.
	if(psH <= psW)
	{	scale = psH/(float)windowHeight_;
	}
	else
	{	scale = psW/(float)windowWidth_;
	}

	// Center the image.
	{ scale *= userScale;
	 float xoff, yoff;
	 float owidth = (float)windowHeight_*scale;
	 float oheight = (float)windowHeight_*scale;

		xoff = (psW - owidth)*0.5;
		yoff = (psH - oheight)*0.5;
		if((xoff != 0.0) || (yoff != 0.0))
			psd->translate( xoff, yoff);
	}		

	////////////////////////////////////////////////////////////////

	AvPSCanvas *c = new AvPSCanvas(AvXColorApp::palette(), psd,
			scale, scale);
	int imageopts = 0;
	if(printOptions_->smoothImage_.getValue())
		imageopts |= AvPSDriver::SMOOTH;
#if 0
	// Only does image, so don't use.
	if(printOptions_->flipX_.getValue())
		imageopts |= AvPSDriver::FLIPX;
	if(printOptions_->flipY_.getValue())
		imageopts |= AvPSDriver::FLIPY;
#endif

	c->imageOptions(imageopts);

	c->ref();
		c->show(is);
	c->unref();
	cout << "Wrote " << fileName << endl;
	AvXColorApp::resetBusy();
}

// Handle mouse movement when this is the window with the cursor.
// If forceBox is TRUE the box will be drawn. Otherwise, the cursor
// box may not be drawn.
// Returns FALSE if xSR &/or ySR are out of range.
Boolean AvImageWindow::processMouse(const int xSR, const int ySR,
				  const Boolean forceBox)
{
  if(!accessor()->validXYZ(xSR, ySR))
	return FALSE;

  cursor_.x = xSR;
  cursor_.y = ySR;

  unDrawBox();

  // If not doing region, draw 'cursor' if scalefactor is large enough for it
  // to be visible under the X cursor.
  if(drawingRegion())
	drawBox(cursor_.x, cursor_.y, RegionBox);
  else
  if(((xScaleFactor_ > MinBoxScale) || (yScaleFactor_ > MinBoxScale)
    || forceBox))
	drawBox(xSR, ySR, CursorBox);
  return TRUE;
}

// Draw a box from x1/y1 to x2/y2
// Degenerate boxes get drawn as lines or points.
// This version assumes SR coordinates.
void AvImageWindow::drawBox(const Point &start, const Point &End)
{
  int x1, y1, x2, y2;
  // rectangle is drawn from minimum point, with size max-min.
  XYToWindow(start.x, start.y, x1, y1);
  XYToWindow(End.x, End.y, x2, y2);

  if(x2 < x1)
  {int tmp = x1;
	x1 = x2;
	x2 = tmp;
  }

  if(y2 < y1)
  {int tmp = y1;
	y1 = y2;
	y2 = tmp;
  }
  // Expand size to enclose scaled pixel.
  int xsf = (int)xScaleFactor_ -1;
  int ysf = (int)yScaleFactor_ -1;
  if(xsf < 0)
	xsf = 0;
  if(ysf < 0)
	ysf = 0;
  x2 += xsf;
  y1 -= ysf;
  // Draw "box" differently depending on how degenerate it is.
  if((x1 == x2) || ( y1 == y2))
  {	if((x1 == x2) && (y1 == y2))
	{	if(!drawCross_)
		  XDrawPoint(display_, XtWindow (drawArea_), gc_, x1, y1);
	}
	else
		XDrawLine(display_, XtWindow (drawArea_), gc_, x1, y1, x2, y2);
  }
  else
      XDrawRectangle
	(display_, XtWindow (drawArea_), gc_, x1, y1, x2-x1, y2-y1);


  if(drawCross_)
  { // Draw a cross at the 'center' of the box. The cross will have a minimum
   // height, but will track the box size otherwise.
   int xl2 = (int) (((float)(x2-x1))/2.0 + 0.5);	// Length of axis/2.
   int yl2 = (int) (((float)(y2-y1))/2.0 + 0.5);
   int xlc = x1 + xl2;		// Center of box
   int ylc = y1 + yl2;
// 1/2 of min cross length.
#define CROSSLENGTH_2	8
	if(xl2 < CROSSLENGTH_2)		// Make minimum size.
		xl2 = CROSSLENGTH_2;
	if(yl2 < CROSSLENGTH_2)
		yl2 = CROSSLENGTH_2;

   int xl0 = xlc-xl2;
   int xl1 = xlc+xl2;
   int yl0 = ylc-yl2;
   int yl1 = ylc+yl2;

	XDrawLine(display_, XtWindow (drawArea_), gc_, xl0, ylc, xl1, ylc);
	XDrawLine(display_, XtWindow (drawArea_), gc_, xlc, yl0, xlc, yl1);
  }

  /////!!
  boxStart_ = start;
  boxEnd_ = End;
  boxDrawn_ = TRUE;
//  XSync(AvApp::display(), FALSE);		// TEST
}

// Erase box if it's drawn.
void AvImageWindow::unDrawBox()
{
	if(boxDrawn())
	{	drawBox(boxStart_, boxEnd_);
		boxDrawn_ = FALSE;
	}
}

/* Draw the box.
	If the box has been drawn undraw it first.
	If the box hasn't been started, start it and draw box around start
	point.
   Values are in SR coordinates.
*/
void AvImageWindow::drawBox(const int xSR, const int ySR,
			    const BOXTYPE boxtype)
{
	// Remove any previous box. It is possible for a box to remain
	// from a previous box drawing session. (boxDrawn_ & ! boxStarted_).
	unDrawBox();
	if(! boxStarted() || (boxtype == CursorBox))
	{	boxStart_.x = xSR;
		boxStart_.y = ySR;
		boxStarted_ = TRUE;
	}
	boxEnd_.x = xSR;
	boxEnd_.y = ySR;
	boxType_ = boxtype;

	drawBox(boxStart_, boxEnd_);
}

// Redraw current box.
void AvImageWindow::drawBox()
{
	switch(boxType_) {
	case NoBox:
		unDrawBox();
		break;
	case CursorBox:
		drawBox(cursor_.x, cursor_.y, CursorBox);
		break;
	case RegionBox:
		unDrawBox();
		drawBox(boxStart_, boxEnd_);
		break;
	default:;
	}
}

// Need to switch between box states. Mostly called externally.
void AvImageWindow::updateBox()
{
Boolean	region = drawingRegion();
Boolean value = viewPanel_->doingValue();

	switch(boxType_) {
	case NoBox:
		if(region)
		{	boxType_ = RegionBox;
			drawBox();	// ????
		}
		else
		if(value)
		{	boxType_ = CursorBox;
			drawBox();
			// Call the ViewPanel back with the current
			viewPanel_->displayInfo (cursor_.x, cursor_.y);
		}
		break;
	case CursorBox:
		if(region)
		{	boxType_ = RegionBox;
//			unDrawBox();
			drawBox();
		}
		else
		if(!value)
		{	boxType_ = NoBox;
			unDrawBox();
		}
		break;
	case RegionBox:
		// Stay in Region mode unless it is turned off.
		if(!region)
		{	if(value)
			{	boxType_ = CursorBox;
				drawBox();
			}
			else
			{	boxType_ = NoBox;
				unDrawBox();
			}
		}
		break;
	default:;
	}
}

// When the mouse button is pressed, check the mode on the
// feedback panel.  If feedback is not activated, then don't 
// do anything inside the window.
void AvImageWindow::mouseDown(const int xSR, const int ySR,
			      const Boolean forceBox)
{
  // does the current feedback mode include drawing the box?
  unsigned short m = viewPanel_->mode();

  if (m > 0) {

    if(processMouse(xSR, ySR, forceBox))
    {	// pass this point to the viewPanel...
	viewPanel_->displayInfo (cursor_.x, cursor_.y);
	// displayInfo sets the cursor position.
	tracker_->invokeCallbacks(AvConductor::MouseDown);
    }
  }
}

void AvImageWindow::mouseDown(const AvPosition &pos)
{
int x, y, z;

	// Convert to X/Y/Z, but only check X/Y. ViewPanel will check Z.
	vItem_->WorldToXYZ(pos, x, y, z);
	if(!accessor()->validIJK(x, y))
		return;
	if(track3D())
		viewPanel_->setSlice(z, TRUE);
	mouseDown(x, y, TRUE);
}

void AvImageWindow::mouseMove(const int xSR, const int ySR,
			      const Boolean forceBox)
{
  if(processMouse(xSR, ySR, forceBox))
  {	  // pass this point to the viewPanel...
	viewPanel_->displayInfo (cursor_.x, cursor_.y);
	// displayInfo sets the cursor position.
	tracker_->invokeCallbacks(AvConductor::MouseMove);
  }
}

void AvImageWindow::mouseMove(const AvPosition &pos)
{
int x, y, z;

	vItem_->WorldToXYZ(pos, x, y, z);
	if(!accessor()->validIJK(x, y))
		return;
	if(track3D())
		viewPanel_->setSlice(z, TRUE);
	mouseMove(x, y, TRUE);
}

void AvImageWindow::mouseUp(const int xSR, const int ySR,
			    const Boolean forceBox)
{
  if(processMouse(xSR, ySR, forceBox))
  {	// pass this point to the viewPanel...
	viewPanel_->displayInfo (cursor_.x, cursor_.y);

	// Pass the region info to the view panel.
	if(drawingRegion() && accessor()->validXYZ(xSR, ySR))
	{ int x0 = accessor()->getAxisStart(0);
	  int y0 = accessor()->getAxisStart(1);
		viewPanel_->setRegion(boxStart_.x + x0, boxStart_.y + y0,
				boxEnd_.x + x0, boxEnd_.y + y0);
	}
	// displayInfo sets the cursor position.
	tracker_->invokeCallbacks(AvConductor::MouseUp);
  }
  boxStarted_ = FALSE;
}

void AvImageWindow::mouseUp(const AvPosition &pos)
{
int x, y, z;

	vItem_->WorldToXYZ(pos, x, y, z);
	if(accessor()->validIJK(x, y))
	{	if(track3D())
			viewPanel_->setSlice(z, TRUE);
	}
	else
	{ // Need to call mouseUp to cleanup.
		x = cursor_.x; y = cursor_.y;
	}
	mouseUp(x, y, TRUE);
}
  

// Set position (X, Y, Z) to pos.
// If pos doesn't map into valid position, ignore.
// (forceBox is used to decide whether to always draw the box).
void AvImageWindow::setPosition(AvPosition &pos, const Boolean forceBox)
{int x, y, z;

	// Try to map to our coordinates.
	AvPosition *np = AvPosition::newPosition(accessor(), pos);

	// Valid?
	if(!vItem_->WorldToXYZ( *np, x, y, z))
		return;

	if(track3D())
		viewPanel_->setSlice(z, TRUE);

	// Update cursor and box.
	if(processMouse(x, y, forceBox))
		// pass this point to the viewPanel...
		viewPanel_->displayInfo(cursor_.x, cursor_.y);
	delete np;
}

// Set the position of all included windows to pos.
// If doThis is FALSE, "this" is not changed.
void AvImageWindow::setPositions(AvPosition &pos, const Boolean doThis)
{AvImageViewItem **list;
 int listlen;

	list = AvImageView::instance()->getImageViewList(listlen,
			AvImageView::INCLUDED);
	for(int i=0; i< listlen; i++)
	{AvImageWindow *win = list[i]->getImageWindow();
	 Boolean forceBox;

		// Always draw a box for 'other' windows.
		forceBox = (win == this) ? FALSE : TRUE;

		// Frequently 'this' is handled elsewhere.
		if(!doThis && !forceBox)
			continue;
		win->setPosition(pos, forceBox);
	}
	delete [] list;
}

void AvImageWindow::setPositions(const int xSR, const int ySR, const int zSR,
				 const Boolean doThis)
{
AvPosition pos(accessor());

	vItem_->XYZToWorld(xSR, ySR, zSR, pos);
	setPositions(pos, doThis);
}


// Replace the current raster with a different one and optionally
// redraw the image. The raster is not changed if it's the same as the old,
// but the redraw will take place.
void AvImageWindow::showImage (AvCLUImage * r, const Boolean draw)
{
  if(raster_ != r)
  {	raster_ = r;
	if(canvas_ != NULL)
		canvas_->setDisplayObject(raster_->graphics2d());
  }
  if(draw)
  	showImage();
}

void AvImageWindow::showImage ()
{
  if((raster_ != NULL) && (canvas_ != NULL))
  {	canvas_->show ();
	boxDrawn_ = FALSE;		// Flag box as not there. 
	if(XtIsRealized(drawArea_))
		drawBox();		// Redraw box overlay.
  }
}

// Update display (readout) information and possibly redraw the image.
void AvImageWindow::updateImageInfo(AvCLUImage * r, const Boolean draw)
{
  if(r != NULL)
	showImage(r, FALSE);	// Just set new raster value.
#if 0
  if(XtIsRealized(viewPanel_->baseWidget()))
	viewPanel_->updateDisplayInfo();
#endif

  if(draw)
	showImage();
}

// Set the scalefactors.
void AvImageWindow::scale (const float xsf, const float ysf,
			   const Boolean redraw)
{
  // scale the CLUImage by this factor.  Since we're passing floating
  // point scale factors, but the CLUImage is only going to do pixel 
  // replication or integer subsampling, the scale factor actually used
  // by the CLUImage is returned and stored in the window's scaleFactor.
  // scaleFactor_ is used to convert between IJK and Window coordinates.
  raster_->setScale(xsf, ysf);
  raster_->getScale(xScaleFactor_, yScaleFactor_);
  displayTitle();

  if(redraw)
	canvas_->show();	// Redisplay current list.
}

// Return the widget for the equalScales toggle if it has been set yet.
// Otherwise NULL.
// If the local pointer has not been set, the scale menu is searched
// to see if it has been set there yet.
Widget AvImageWindow::equalScalesW()
{
	if(equalScalesW_ == NULL)
	{ AvMenuItem *mi = scaleMenu_->items();
	  int milen = scaleMenu_->howMany();

		for(int i=0; i< milen; i++,mi++)
			if(strcmp(mi->name(), EQUALSCALES)==0)
			{	equalScalesW_ = mi->widget();
				break;
			}
	}
	return equalScalesW_;
}

Boolean AvImageWindow::equalScales()const
{	return equalScales_;
}

// If the equal scale toggle widget exists, make it track equalScales_.
Boolean  AvImageWindow::equalScales(const Boolean equal)
{Boolean old = equalScales_;
 Widget w;

	equalScales_ = equal;

	if((w = equalScalesW()) != NULL)
		XmToggleButtonSetState(w, equal, False);

	return old;
}

void AvImageWindow::activateFeedbackHandlers ()
{
  XtAddEventHandler (drawArea_,
		     EnterWindowMask, FALSE,
		     (XtEventHandler) &AvImageWindow::enterDrawAreaCB,
		     (XtPointer) this);

  XtAddEventHandler (drawArea_,
		     ButtonPressMask, FALSE,
		     (XtEventHandler) &AvImageWindow::mouseDownCB,
		     (XtPointer) this);

  XtAddEventHandler (drawArea_,
		     ButtonReleaseMask, FALSE,
		     (XtEventHandler) &AvImageWindow::mouseUpCB,
		     (XtPointer) this);

  XtAddEventHandler (drawArea_,
		     ButtonMotionMask, FALSE,
		     (XtEventHandler) &AvImageWindow::mouseMoveCB,
		     (XtPointer) this);
}

void AvImageWindow::deactivateFeedbackHandlers ()
{
  XtRemoveEventHandler (drawArea_,
			EnterWindowMask, FALSE,
			(XtEventHandler) &AvImageWindow::enterDrawAreaCB,
			(XtPointer) this);

  XtRemoveEventHandler (drawArea_,
			ButtonPressMask, FALSE,
			(XtEventHandler) &AvImageWindow::mouseDownCB,
			(XtPointer) this);

  XtRemoveEventHandler (drawArea_,
			ButtonReleaseMask, FALSE,
			(XtEventHandler) &AvImageWindow::mouseUpCB,
			(XtPointer) this);

  XtRemoveEventHandler (drawArea_,
			ButtonMotionMask, FALSE,
			(XtEventHandler) &AvImageWindow::mouseMoveCB,
			(XtPointer) this);
}

// This perhaps should be in the class struct, but it's impossible to need
// more than one at a time.
static AvImageViewItem **list_=NULL;
static int listlen_ = 0;

void AvImageWindow::mouseDownCB (Widget, AvImageWindow * obj, XEvent * event)
{
//AvPosition pos(obj->vItem_);
AvPosition pos(obj->accessor());
int xSR, ySR;

    // grab the cursor and restrict it to drawArea_
#if 0
#ifdef __sun
	if(getenv("dbxprog")==NULL)	// Disable if debugging on Suns.
#endif
#endif
	XGrabPointer(obj->display_, XtWindow(obj->drawArea_), TRUE,
		(unsigned int) (ButtonPressMask | ButtonMotionMask),
		 GrabModeAsync, GrabModeAsync,
		 XtWindow (obj->drawArea_),
		 None, CurrentTime);
    // draw area height is needed to map from window to image ijk
//    XtVaGetValues(drawArea_, XmNheight, &windowHeight_,
//		  XmNwidth, &windowWidth_, NULL);

	list_ = AvImageView::instance()->getImageViewList(listlen_,
			AvImageView::INCLUDED);

	obj->WindowToXY(event->xbutton.x, event->xbutton.y, xSR, ySR);
	obj->vItem_->XYZToWorld(xSR, ySR, obj->accessor()->getPlane(), pos);
	obj->mouseDown(xSR, ySR, FALSE);

	for(int i=0; i< listlen_; i++)
	{AvImageWindow *win = list_[i]->getImageWindow();
	 AvPosition *np;

		if(win == obj)
			continue;
		np  = AvPosition::newPosition(win->accessor(), pos);
		// np == NULL means that pos doesn't map into win's x/y.
		if(np == NULL)
			continue;
		if(!np->validXY())	// X & Y within data set?
		{	delete np;
			return;
		}
		win->mouseDown(*np);
		delete np;
	}
	// list_ will be deleted in mouseUpCB.
}

#if 0
static struct Events {	int	value;
		char	*name;
	} events[] = {
	{ KeyPress, "KeyPress"},
	{ KeyRelease, "KeyRelease"},
	{ ButtonPress, "ButtonPress"},
	{ ButtonRelease, "ButtonRelease"},
	{ MotionNotify, "MotionNotify"},
	{ EnterNotify, "EnterNotify"},
	{ LeaveNotify, "LeaveNotify"},
	{ FocusIn, "FocusIn"},
	{ FocusOut, "FocusOut"},
	{ KeymapNotify, "KeymapNotify"},
	{ Expose, "Expose"},
	{ GraphicsExpose, "GraphicsExpose"},
	{ NoExpose, "NoExpose"},
	{ VisibilityNotify, "VisibilityNotify"},
	{ CreateNotify, "CreateNotify"},
	{ DestroyNotify, "DestroyNotify"},
	{ UnmapNotify, "UnmapNotify"},
	{ MapNotify, "MapNotify"},
	{ MapRequest, "MapRequest"},
	{ ReparentNotify, "ReparentNotify"},
	{ ConfigureNotify, "ConfigureNotify"},
	{ ConfigureRequest, "ConfigureRequest"},
	{ GravityNotify, "GravityNotify"},
	{ ResizeRequest, "ResizeRequest"},
	{ CirculateNotify, "CirculateNotify"},
	{ CirculateRequest, "CirculateRequest"},
	{ PropertyNotify, "PropertyNotify"},
	{ SelectionClear, "SelectionClear"},
	{ SelectionRequest, "SelectionRequest"},
	{ SelectionNotify, "SelectionNotify"},
	{ ColormapNotify, "ColormapNotify"},
	{ ClientMessage, "ClientMessage"},
	{ MappingNotify, "MappingNotify"},
	{ LASTEvent, "LASTEvent"}
	};
static const int NEVENTS = sizeof(events)/sizeof(*events);
static const char *v2s(const int v)
{	for(int i=0; i< NEVENTS; i++)
		if(v == events[i].value)
			return events[i].name;
	return "Unknown";
}
#endif

void AvImageWindow::mouseMoveCB (Widget w, AvImageWindow * obj, XEvent * event)
{
AvPosition pos(obj->accessor());
int xSR, ySR;

	// The cursor can get way ahead of the updates, especially if there
	// are included windows. In order to reduce lag, all but the last
	// motion event are discarded.
	Display *dpy = XtDisplay(w);
	while(XEventsQueued(dpy, QueuedAlready) > 0)
	{XEvent e;
		XPeekEvent(dpy, &e);
		if(e.type == NoExpose)// Should look into getting rid of these.
			XNextEvent(dpy, &e);	// Pitch NoExpose events.
		else
		if(e.type == MotionNotify)
			XNextEvent(dpy, event);
		else
			break;
	}
	if(event->type != MotionNotify)	// Just in case.
		return;

  // If mouse is out of bounds, ignore. (It can be out if no buttons are
  // turned on so no grab was done.
  if(!obj->WindowToXY(event->xbutton.x, event->xbutton.y, xSR, ySR))
	return;
  obj->vItem_->XYZToWorld(xSR, ySR, obj->accessor()->getPlane(), pos);
  obj->mouseMove(xSR, ySR, FALSE);

  if(listlen_ <= 0)
	return;

	for(int i=0; i< listlen_; i++)
	{AvImageWindow *win = list_[i]->getImageWindow();
	 AvPosition *np;

		if(win == obj)
			continue;
		np  = AvPosition::newPosition(win->accessor(), pos);
		// np == NULL means that pos doesn't map into win's x/y.
		if(np == NULL)
			continue;
		if(!np->validXY())
		{	delete np;
			continue;
		}
		win->mouseMove(*np);
		delete np;
	}
}
  
void AvImageWindow::mouseUpCB (Widget, AvImageWindow * obj, XEvent * event)
{
AvPosition pos(obj->accessor());
int xSR, ySR;

  XUngrabPointer (obj->display_, CurrentTime);

  // Convert to XY coords
  if(!obj->WindowToXY(event->xbutton.x, event->xbutton.y, xSR, ySR))
	return;
  obj->mouseUp(xSR, ySR, TRUE);		// Always draw box on button up.
  obj->vItem_->XYZToWorld(xSR, ySR, obj->accessor()->getPlane(), pos);

  if(listlen_ <= 0)
	return;

	for(int i=0; i< listlen_; i++)
	{AvImageWindow *win = list_[i]->getImageWindow();
	 AvPosition *np;

		if(win == obj)
			continue;
		// Don't check for valid position since we need to make
		// sure 'regioning' gets turned off.
		np  = AvPosition::newPosition(win->accessor(), pos);
		// np == NULL means that pos doesn't map into win's x/y.
		if(np == NULL)
			continue;
		win->mouseUp(*np);
		delete np;
	}
	delete list_;
	list_ = NULL;
	listlen_ = 0;
}

void AvImageWindow::enterDrawAreaCB (Widget, AvImageWindow * obj, XEvent *)
{
  if (obj->viewPanel_->mode())
    XDefineCursor
      (obj->display_, XtWindow (obj->drawArea_), obj->feedbackCursor_);
  else
    XUndefineCursor (obj->display_, XtWindow (obj->drawArea_));
}  

void AvImageWindow::redrawCB (Widget, AvImageWindow * obj, XtPointer ptr)
{
	// Expose events can come in bunches. Ignore all but the last since
	// we're always repainting the entire window.
	if(ptr != NULL)
	{XmDrawingAreaCallbackStruct *das = (XmDrawingAreaCallbackStruct *)ptr;
		XExposeEvent *event = (XExposeEvent *)das->event;;

		  if((event != NULL) &&( event->type == Expose))
		  {
			if(event->count != 0)
				return;
		  }
	}

	if(!externalDrawingArea())
		XtVaGetValues ( obj->drawArea_, XmNheight, &obj->windowHeight_,
				XmNwidth, &obj->windowWidth_, NULL);

	// If object has been exposed, the box probably got erased.
	//  if(obj->boxDrawn_)
	obj->boxDrawn_ = FALSE;
	obj->drawBox();
}

AvImageWindow::~AvImageWindow ()
{
  // Disconnect from accessor
  if(accessor() != NULL)
  {	accessor()->removeCallback(&AvImageWindow::accessorCB_,
			ACCMASK, this, NULL);
	accessor()->unref();
  }
  delete scaleMenu_;
  delete xScaleMenu_;
  delete yScaleMenu_;

  canvas_->unref();
#if 0
  // Should these be deleted? Or are they handled elsewhere??
  delete vItem_;
  delete raster_;		// Deleted in ~AvImageViewItem();
#endif
  delete viewPanel_;
  delete tracker_;
}

// Convert a window position to world coordinates.
Boolean AvImageWindow::WindowToWorld(const int x, const int y, AvPosition &pos)
{
int	i, j;
Boolean	valid;

	valid = WindowToXY(x, y, i, j);
	valid &= vItem_->XYZToWorld(i, j, accessor()->getPlane(), pos);

	return valid;
}

// Convert an XY position in window (window) coordinates to indices
// for the current subrange (data set relative).
// Returns TRUE if XW/YW map into valid subrange indices.
Boolean AvImageWindow::WindowToXY(const int XW, const int YW, int &i, int &j,
				  const Boolean sr)const
{
int	scrnY, minI, minJ;
int	lm, rm, tm, bm, x;

	// flip the vertical coordinate system 
	scrnY = windowHeight_ - YW - 1;

	// Get rid of margins
	vItem_->getCLUImage()->getMargins(lm, rm, tm, bm);
	x = XW - lm;
	scrnY = scrnY - bm;	// tm or bm??

	float xsf = xScaleFactor_;
	if (xScaleFactor_ > 1.0) 
		xsf = (int) xScaleFactor_;
	float ysf = yScaleFactor_;
	if (yScaleFactor_ > 1.0) 
		ysf = (int) yScaleFactor_;

	if(sr)
	{	minI = minJ = 0;
	}
	else
	{	minI = accessor()->getAxisStart(AvAccessor::XAXIS);
		minJ = accessor()->getAxisStart(AvAccessor::YAXIS);
	}

	i = int(x / xsf + minI);
	j = int(scrnY / ysf + minJ);

	return sr ? accessor()->validXYZ(i, j) : accessor()->validIJK(i, j);
}

// Convert a world position to window coordinates.
Boolean AvImageWindow::WorldToWindow(const AvPosition &pos, int &x, int &y)
									const
{
Boolean	valid;
int	i,j,k;

	valid = vItem_->WorldToXYZ(pos, i, j, k);

	valid &= XYToWindow(i, j, x, y);
	return valid;
}

// Convert XY values (for the current subrange) into window coordinates.
// Returns TRUE if XY maps into valid subrange values.
// Assumes scaleFactor_ is the same as used by the display canvas.
Boolean AvImageWindow::XYToWindow(const int x, const int y, int &xw, int &yw)
									const
{
int	lm, rm, tm, bm;

	if(!accessor()->validXYZ(x, y))
		return FALSE;
	float xsf = xScaleFactor_;
	if (xScaleFactor_ > 1.0) 
		xsf = (int) xScaleFactor_;
	float ysf = yScaleFactor_;
	if (yScaleFactor_ > 1.0) 
		ysf = (int) yScaleFactor_;
	xw = int(x*xsf);
	yw = int(y*ysf);

	// Add in window margins.
	vItem_->getCLUImage()->getMargins(lm, rm, tm, bm);
	xw += lm;
	yw += bm;

	// flip the vertical coordinate system 
	yw = windowHeight_ - yw - 1;

	return TRUE;
}

// This is here rather than the include file so the include file doesn't have
// to include AvImageViewPanel.h.
Boolean AvImageWindow::drawingRegion()const
{
	return viewPanel_->doingRegion();
}

 // Callback to receive accessor notices.
void AvImageWindow::accessorCB_(XtPointer dis, AvConductor *c, XtPointer uD)
{ AvImageWindow *me = (AvImageWindow *) dis;
	me->accessorCB(c, uD);
}

 // Callback to actually handle accessor notices.
//void AvImageWindow::accessorCB(AvConductor *c, XtPointer uData)
void AvImageWindow::accessorCB(AvConductor *c, XtPointer )
{ int r = c->callbackReason();
  static int rmask = AvAccessor::CLIP | AvAccessor::PLANE | AvAccessor::REGION;

	if(r & rmask)
		showImage();
	if(r & AvAccessor::DISCONNECT)
	{  // Disconnect from accessor
	  if(accessor_ != NULL)
	  {	accessor_->removeCallback(&AvImageWindow::accessorCB_,
			ACCMASK, this, NULL);
		accessor_->unref();
		accessor_ = NULL;
	  }
	}
}

////////////////////////////////////////////////////////////////
////		AvStatistics			////////////////
////////////////////////////////////////////////////////////////
AvStatistics::AvStatistics(AvAccessor *ac)
{
	init(ac, ac->clipMin(), ac->clipMax());
	calc();
}

AvStatistics::AvStatistics(AvAccessor *ac,
			   const float clipMin, const float clipMax)
{
	init(ac, clipMin, clipMax);
	calc();
}

void AvStatistics::init(AvAccessor *ac,
			const float clipMin, const float clipMax)
{
	accessor_ = ac;
	if(accessor_ != NULL)
		accessor_->ref();	// ERROR if NULL!!!!!

	stats_[0].init(ac);
	stats_[1].init(ac, clipMin, clipMax);
	blankcount_ = 0;
}

AvStatistics::~AvStatistics()
{
	accessor_->unref();
}

// Do the calculations on the current plane. Initially, the calculations
// were done by calling Stats::calc(), however, there can be a 2X difference
// in the amount of time taken, so the work is done here.
void AvStatistics::calc()
{
	if(accessor_ == NULL)
	{	cerr << "AvStatistics::calc: NULL accessor." << endl;
		return;
	}
 int width = accessor_->width(), height = accessor_->height();
 long npixels = width*height;
 int rank = accessor_->rank();
 float *data = new float[npixels], *ptr=data;
 float blank = accessor_->blankingValue();
 AvIPosition start(rank, 0);
 AvIPosition shape(rank, 1);
 AvIPosition stride(rank, 1);
 // Data values.
 double clipsum, clipsumsqr, datasum, datasumsqr;
 float dataMin, dataMax, clipMin, clipMax;
 float clipMin_ = accessor_->clipMin();
 float clipMax_ = accessor_->clipMax();
 int dataMinX, dataMinY, clipMinX, clipMinY;
 int dataMaxX, dataMaxY, clipMaxX, clipMaxY;
 int nclippoints=0, ndatapoints=0;
 int x, y;
#ifdef TIMEIT
 clock_t timeread0 , timeread1, timecalc0, timecalc1;

	timeread0 = clock();
#endif
	////////////////////////////////////////////////////////////////
	//		Get the current slice.
	////////////////////////////////////////////////////////////////
	if(rank > 2)
		start(2) = accessor_->getPlane();

	shape(0) = width;
	shape(1) = height;

	if(! accessor_->getSlice(data, start, shape, stride))
	{	cerr << "AvStatistics::calc: Could not read data." << endl;
		return;
	}


#ifdef TIMEIT
	timeread1 = clock();
	timecalc0 = clock();
#endif
	////////////////////////////////////////////////////////////////
	//	Iterate over the slice keeping track of min/max and adding
	//	things up.
	////////////////////////////////////////////////////////////////
	dataMinX = dataMinY = clipMinX = clipMinY = 0;
	dataMaxX = dataMaxY = clipMaxX = clipMaxY = 0;
	clipsum = clipsumsqr = datasum = datasumsqr = 0.0;
	ptr = data;

	dataMin = clipMin = stats_[0].clipMax();
	dataMax = clipMax = stats_[0].clipMin();

	blankcount_ = 0;
	int overcount=0, undercount=0;

	for( y = 0; y < height; y++)
	 for( x = 0; x < width; x++)
	 {float datum = *ptr++;
		if(datum == blank)
		{	blankcount_ += 1;
			 continue;
		}

		// Calculations and checks over entire data range.
		double d = (double)datum;
		double dd = d*d;
		datasum += d;
		datasumsqr += dd;
		ndatapoints += 1;

		if(datum > dataMax)
		{	dataMax = datum;
			dataMaxX = x;
			dataMaxY = y;
		}

		if(datum < dataMin)
		{	dataMin = datum;
			dataMinX = x;
			dataMinY = y;
		}

		// Calculations and checks over clipping range.
		if(datum < clipMin_)		// Range checks
		{	undercount += 1;
			continue;
		}
		else
		if(datum > clipMax_)
		{	overcount += 1;
			continue;
		}

		if(datum < clipMin)		// Min/Max checks.
		{	clipMin = datum;
			clipMinX = x;
			clipMinY = y;
		}

		if(datum > clipMax)
		{	clipMax = datum;
			clipMaxX = x;
			clipMaxY = y;
		}

		nclippoints += 1;
		clipsum += d;
		clipsumsqr += dd;
	}

	////////////////////////////////////////////////////////////////
	// Copy data to stats structs.
	////////////////////////////////////////////////////////////////
	stats_[0].numpoints(ndatapoints);
	stats_[0].sum(datasum);
	stats_[0].sumsqr(datasumsqr);
	stats_[0].dataMin(dataMin, dataMinX, dataMinY);
	stats_[0].dataMax(dataMax, dataMaxX, dataMaxY);

	stats_[1].numpoints(nclippoints);
	stats_[1].dataMin(clipMin, clipMinX, clipMinY);
	stats_[1].dataMax(clipMax, clipMaxX, clipMaxY);
	stats_[1].sum(clipsum);
	stats_[1].sumsqr(clipsumsqr);
	stats_[1].overcount(overcount);
	stats_[1].undercount(undercount);

#ifdef TIMEIT
	timecalc1 = clock();
	double readtime = timeread1-timeread0;
	double calctime = timecalc1-timecalc0;

	cout << "Reading data took " << readtime/1000.0 << " ms." << endl;
	cout << "Calculations took " << calctime/1000.0 << " ms." << endl;
#endif

	delete [] data;
}

// Print the formatted position in a manner similar to Av?Position.
// If showAll is FALSE, only the first 2 entries (X & Y) are displayed.
static void printPos(const AvWPosition &wp, AvAccessor *a, ostream &out,
		     const Boolean showAll=TRUE, const int nsfig=4)
{ int rank = a->rank();
  AvString str = "[ ";

	if(!showAll)
		rank = 2;
	for(int i=0; i< rank; i++)
	{	str += a->axisValueUnits(i, wp(i), nsfig);
		if( i != rank-1)	// Last time.
		{ if(str.length() < 64)
			str += ", ";
		 else
		 {	out << str << endl;
			str = "  ";
		 }
		}
		else
			str += " ]";
	}
	out << str;
}

// Print the 1st two entries of an AvIPosition.
static void printXYPos(const AvIPosition &ip, ostream &out)
{ AvIPosition i(2);
	i(0) = ip(0);
	i(1) = ip(1);
	out << i;
}

// Print the current numbers.
// Values are printed to terminal rather than being displayed in a window
// to avoid hassles. Depending on where they were, they might need to be
// updated when the slice changed. They would be expected to be updated
// if/when the dataset was modified.
void AvStatistics::print(ostream &out)
{
	out <<
 "================================================================" << endl;
	out << "File: " << accessor_->dataset()->fileName() << endl;
	int rank = accessor_->rank();
	AvWPosition wp(rank);
	accessor_->XYZToWorld(0, 0, wp);

	////////////////////////////////
	// Size of Region.
	////////////////////////////////
	// In World values.
	out	<< "\t\tRegion: " << endl;
	out << "From:" << endl;
	printPos(wp, accessor_, out);
	out << endl;

	accessor_->XYZToWorld(accessor_->width()-1, accessor_->height()-1, wp);
	out << "To:" << endl;
	printPos(wp, accessor_, out);
	out << endl;

	// In pixel/index values.
	out	<< " (" << accessor_->width() << " X " << accessor_->height()
		<< " = " << accessor_->width()*accessor_->height()
		<< " points, " << blankcount_ << " blanked.)" << endl;
	out << endl;

	// Print numbers for entire range of data.
	out << stats_[0];

	// If different, print for data within clip min/max
	if(stats_[0].numpoints() != stats_[1].numpoints())
	{	out <<
 "====================================" << endl;

	out << stats_[1];
	}

	out <<
 "================================================================" << endl;
}

#if 0
ostream& operator<<(ostream& out, const AvStatistics &s)
{
	out << s.stats_[0];
	if( (s.stats_[0].clipMin() != s.stats_[1].clipMin())
				  ||
	    (s.stats_[0].clipMax() != s.stats_[1].clipMax()))
	{	out << s.stats_[1];
	}
	return out;
}
#endif

////////////////////////////////////////////////////////////////
///		AvStatistics::Stats
// (This wound up being just a struct holding values calculated elsewhere).

// Initialize a Stats entry.
AvStatistics::Stats::Stats()
{
	init(NULL);
}

AvStatistics::Stats::~Stats()
{
	if(accessor_ != NULL)
		accessor_->unref();
}

void AvStatistics::Stats::init( AvAccessor *ac,
				const float clipMin, const float clipMax)
{
	accessor_ = ac;
	int rank;
	if(accessor_ != NULL)
	{	accessor_->ref();
		rank = accessor_->rank();
	}
	else
		rank = 3;

	checkClip_ = TRUE;
	clipMin_ = clipMin;
	clipMax_ = clipMax;

	npoints_ = 0;
	overcount_ = undercount_ = 0;
	minWhere_.resize(rank);
	minWhere_ = 0;
	maxWhere_.resize(rank);
	maxWhere_ = 0;
	if((accessor_ != NULL) && (rank > 2))
	{ int plane = accessor_->getPlane();
		minWhere_(2) = plane;
		maxWhere_(2) = plane;
	}

	dataMin_ = clipMax;
	dataMax_ = clipMin;
	sumsqr_ = 0.0;
	sum_ = 0.0;
}

void AvStatistics::Stats::init( AvAccessor *ac)
{float cmin, cmax;

	if(ac != NULL)
	{	cmin = ac->clipMin();
		cmax = ac->clipMax();
	}
	else
		cmin = cmax = 0.0;

	init(ac, cmin, cmax);
	checkClip_ = FALSE;
}

// Do calculations. The x, y are to keep track of where the min and max
// points are.
 void AvStatistics::Stats::calc(register const float datum,
				 const int x, const int y)
{
	if(checkClip_ && ((datum < clipMin_) || (datum > clipMax_)))
	{	if(datum < clipMin_)
			undercount_ += 1;
		else
			overcount_ += 1;
		return;
	}

	if(datum < dataMin_)
	{	dataMin_ = datum;
		minWhere_(0) = x;
		minWhere_(1) = y;
	}
	if(datum > dataMax_)
	{	dataMax_ = datum;
		maxWhere_(0) = x;
		maxWhere_(1) = y;
	}
	npoints_ += 1;
	double d = (double)datum;
	sum_ += d;
	sumsqr_ += d*d;
}

// RMS = sqrt( (<sumOfSquares> - <squareOfSum>/n)/(n-1));
static inline double rms(const double sumsqr, const double sum, const int n)
{
	if(n <= 1)
		return -1.0;		// Negative means error.

	double s2 = sum*sum;
	return sqrt((sumsqr - s2/n)/(n-1));
}

// Compute the rms value. Returns -1 if there is an error. (Number of
// points is <= 1).
// This isn't inline so math.h doesn't have to be in the include file if
// AvStatistics is moved.
// (The formula was given to me).

double AvStatistics::Stats::rms()const
{	return ::rms(sumsqr(), sum(), numpoints());
}

// The data range. If checkClip is FALSE, the contents don't mean anything.
AvString AvStatistics::Stats::clipMinStr()const
{
	return accessor_->dataValueUnits(clipMin(), 8);
}

AvString AvStatistics::Stats::clipMaxStr()const
{
	return accessor_->dataValueUnits(clipMax(), 8);
}

// Min and Max data values as strings.
AvString AvStatistics::Stats::dataMinStr()const
{
	return accessor_->dataValueUnits(dataMin(), 8);
}

AvString AvStatistics::Stats::dataMaxStr()const
{
	return accessor_->dataValueUnits(dataMax(), 8);
}

// Print the values.
ostream& operator<<(ostream& out, const AvStatistics::Stats &s)
{AvAccessor *ac = s.accessor_;

	AvWPosition w(ac->rank());

	////////////////
	// Numbers for what range of data?
	if( s.checkClip())
		out	<< "Looking at Data in Range: " << s.clipMinStr()
			<< " to " << s.clipMaxStr() << endl;
	else
		out	<< "Looking at entire data range." << endl;

	// The min and max data value seen in the range.
	ac->XYZToWorld(s.whereMinIJK(), w);
	out	<< "Data min: " << s.dataMinStr() << endl;
	out << " At: ";
	printPos(w, ac, out, FALSE);
	out << " ";
	printXYPos(s.whereMinIJK(), out);
	out << endl;

	ac->XYZToWorld(s.whereMaxIJK(), w);
	out	<< "Data max: " << s.dataMaxStr() << endl;
	out << " At: ";
	printPos(w, ac, out, FALSE);
	out << " ";
	printXYPos(s.whereMaxIJK(), out);
	out << endl << endl;

	// Number of points used and ignored, if any (excluding blanked).
	out	<< "Points used: " << s.numpoints();
	if(s.checkClip() && (s.overcount() || s.undercount()))
	{	out	<< " (Ignored " << s.undercount() << " under range, "
			<< s.overcount() << " over range.)";
	}
	out	<< endl;

	// Sum, Sum of squares.
	out	<< "Sum = " << s.sum()
		<< "; Sum of Squares = " << s.sumsqr() << endl;
	// Average and RMS.
	out	<< "Average = " << s.average();
	if(s.numpoints() > 1)
		out	<< "; RMS " << s.rms() << endl;
	else
		out	<< endl;

	return out;
}
