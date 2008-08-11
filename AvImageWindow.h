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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvImageWindow.h,v 19.0 2003/07/16 05:47:37 aips2adm Exp $
//
// $Log: AvImageWindow.h,v $
// Revision 19.0  2003/07/16 05:47:37  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:16  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.2  2002/01/14 17:34:39  hravlin
// Added Point struct. (Moved from Av.h).
//
// Revision 17.1  2002/01/03 22:07:17  hravlin
// Added support for OpenGL/WIREGL.
//
// Revision 17.0  2001/11/12 19:42:45  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:51  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:43  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:46:12  hravlin
// Changed className() to return const char *.
//
// Revision 14.0  2000/03/23 16:08:43  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/09/15 21:30:35  hravlin
// Added support for print options and panel.
//
// Revision 13.0  1999/08/10 18:40:31  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:35  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:06  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:08  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1997/12/08 22:37:44  hr
// Added support for drawing a cross in drawBox().
//
// Revision 9.0  1997/08/25 21:29:41  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:18:09  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.4  1997/01/24 20:23:05  hr
// Added tracker references.
//
// Revision 7.3  1996/12/12 09:58:04  droberts
// Final update from monet archive.
//
// Revision 1.8  1996/11/11 17:59:46  hr
// Moved equalScales() to here from AvImageViewItem. Added code to
// set "X=Y" toggle if equalScales_ gets changed elsewhere.
//
// Revision 1.7  1996/11/11 16:06:03  hr
// Added buildScaleMenu(), updateScale(), xScaleFactor() and yScaleFactor().
// Changed/added a number of variables during conversion to support
// independent scaling of X and Y axes.
//
// Revision 1.6  1996/10/02 16:57:38  hr
// Added setPosition(s) to set included windows to a new AvPosition.
//
// Revision 1.5  1996/09/27  16:40:35  hr
// Added a pointer to the imported accessor in order to keep track of
// whether AvCLUImageSetter had already disconnected from it.
//
// Revision 1.4  1996/09/25  21:30:46  hr
// Mouse/cursor handling routines were heavily modified. Several old routines
// were removed. Several variable's names were changed.
// Added code to support tracking of cursor in 3D.
// A couple of classes are no longer friends.
//
// Revision 1.3  1996/09/18  19:27:35  hr
// More integration with AvAccessor.
//
// Revision 1.2  1996/09/10  16:55:28  hr
// Initial changes for integration with AvAccessor & AvDataSet.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.19  1996/06/18  18:43:04  pixton
//  Copyright Update
//
//  Revision 1.18  1996/02/02  15:30:43  hr
//  Renamed SV_CONTOUR & SV_AXIS to SV_SHOW and SV_SETUP.
//
//  Revision 1.17  1996/01/31  17:57:32  hr
//  Added SV_AXIS to enum.
//
//  Revision 1.16  1996/01/08  22:11:20  hr
//  Added SV_CONTOUR to enum.
//
//  Revision 1.15  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.15  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.15  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.14  1995/07/17  19:56:12  hr
//  Added 'virtual' to destructor of virtual classes.
//
//  Revision 1.13  1995/07/11  21:33:23  hr
//  Added BOXTYPE and extended DrawModes.
//
//  Revision 1.12  1995/06/16  18:14:53  hr
//  Removed "inline" from drawingRegion.
//
//  Revision 1.11  1995/06/12  21:20:06  hr
//  Added viewPanel() function.
//
//  Revision 1.10  1995/06/09  19:39:55  hr
//  Changes to support internal creation of the view panel and added
//  displayTitle().
//
//  Revision 1.9  1995/05/18  20:57:39  hr
//  Added routines to support Include and Draw mode submenus.
//
//  Revision 1.8  1995/05/12  21:23:38  hr
//  Changes to data clip min/max are propagated to profile display.
//
//  Revision 1.7  1995/05/11  21:55:42  hr
//  Added canvasResizeCB (perhaps temporarily).
//
//  Revision 1.6  1995/05/05  14:50:28  hr
//  Added support for synchronized picking.
//
//  Revision 1.5  1995/04/25  16:52:30  hr
//  Added redraw flag to scale.
//
//  Revision 1.4  1995/04/14  19:35:55  hr
//  Changed canvas_ from AvCanvas to AvXCanvas.
//
//  Revision 1.3  1995/03/30  22:23:08  hr
//  Removed fallbacks variable from class.
//
//  Revision 1.2  1995/03/24  15:45:48  baker
//  Added mechanism for user specification of whether or not this window
//  should be included in a multi-window, synchronized operation, such as
//  blinking or synchronized movies.
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#ifndef _SV_IMAGE_WINDOW_H
#define _SV_IMAGE_WINDOW_H

/*

AvImageWindow

AvImageWindow deals with several coordinates: X, data set relative,
subrange relative and world. Except for X coordinates, all values are
mapped through the accessor. Data set relative here means mapped but
relative to the start of the data set. Subrange coordinates are relative to
the start of the current subrange. World coordinates are world values with
axis name information (AvPosition).

	Coord		Referred to as
	X		??
	Data set	DS or IJK (sorry)
	Subrange	SR
*/

#ifdef AVOGL
#include <AvGLCanvas.h>
#else
#include <AvXCanvases.h>
#endif
class AvCLUImage;
class AvImageDataPanel;
class AvImageViewPanel;
class AvVisRep2D;
class AvPosition;
class AvAccessor;
class AvImageViewItem;
class AvPrintConfig;
class AvPrintOptions;

#include <AvDisplayWindow.h>
#include <AvTable.h>
#include <AvEvent.h>
#include <AvConductor.h>

class AvImageWindow : public AvDisplayWindow {

public:

  AvImageWindow (Widget parent, int buildInsideParent,
				AvImageViewItem *v=NULL);

  virtual const char * className () const;

  virtual const char * getClassLabel () const { return "2D Window"; }
  virtual const char * classLabel () const { return "2D Window"; }

  // Sets the current raster to raster and tells the canvas to draw it if
  // redraw is TRUE.
  void showImage (AvCLUImage * raster, const Boolean draw=TRUE);
  void showImage ();
  // Tells the window to update view information.
  void updateImageInfo(AvCLUImage * r, const Boolean draw=TRUE);
  // ImageViewPanel's Value or Region button has changed and we need to
  // redisplay the box.
  void updateBox();
  // Various box types. Doesn't necessarily imply it is drawn.
  enum BOXTYPE { NoBox=1, CursorBox=2, RegionBox=3};
  // Region drawing is from corner to corner or from center to mouse.
  // (Future).
  enum REGIONTYPE {CORNERS, CENTER};
  // Returns whether or not this window has been selected by the user
  // to participate in a synchronized operation.
  int include () const { return (included_); }
  // Set/clear include flag.
  void setInclude(const Boolean include);
  // INCTHIS this window, INCALL include all windows, INCINVERT flip state
  // INCNONE clear include flag for all windows.
  enum IncludeMode { INCTHIS, INCALL, INCINVERT, INCNONE};
  // Set/clear include flag for all windows.
  void setIncludeList(const IncludeMode mode);
  Boolean track3D()const{return track3D_;}

  // Set the title using existing info.
  void displayTitle();
  // scale the contents of the window. If redraw it TRUE, redisplay the image.
  void scale (const float xsf, const float ysf, const Boolean redraw=FALSE);
  float xScaleFactor()const{return xScaleFactor_;}
  float yScaleFactor()const{return yScaleFactor_;}
  // Are the x & y scale factors forced to be the same?
  Boolean equalScales()const;
  // Note: this only sets a flag it doesn't force a redraw.
  // (call AvImageViewItem::fitRasterToWindow).
  // Returns the old value.
  Boolean equalScales(const Boolean equal);

  AvImageViewPanel *viewPanel()const {return viewPanel_;}
#ifdef AVOGL
  AvGLCanvas *canvas()const { return canvas_;}
#else
  AvXWindow *canvas ()const { return canvas_; }
#endif
  AvCanvas *avcanvas()const{return canvas_;}

  Widget drawArea()const{return drawArea_;}

  // Convert Window position in pixels to World coordinates.
  // The current slice number is used to compute the Z value.
  // Returns FALSE if x or y is out of range.
  Boolean WindowToWorld(const int x, const int y, AvPosition &pos);
  // Convert window position in pixels to XYZ coordinates.
  // Returns FALSE if x or y is out of range.
  // If sr is TRUE, x/y are relative to the start of the current subrange.
  // Otherwise, the start of the data set.
  Boolean WindowToXY(const int x, const int y, int &i, int &j,
		     const Boolean sr=TRUE)const;
  // Converts World to window coordinates.
  Boolean WorldToWindow(const AvPosition &pos, int &x, int &y)const;
  // Convert an X/Y position within the current subrange to window coords.
  Boolean XYToWindow(const int x, const int y, int &xw, int &yw)const;
  virtual ~AvImageWindow ();

  // Set the position of all included windows to pos.
  // If doThis is FALSE, "this" is not changed.
  void setPositions(AvPosition &pos, const Boolean doThis=FALSE);
  void setPositions(const int xSR, const int ySR, const int zSR,
		    const Boolean doThis=FALSE);

  // Set position (X, Y, Z) to pos.
  // If pos doesn't map into valid position, ignore.
  // (forceBox is used to decide whether to always draw the box).
  void setPosition(AvPosition &pos, const Boolean forceBox);

  // Whenever the mouse position changes, objects connected to the tracker
  // will be notified. Valid masks are AvConductor::Mouse{Down,Move,Up}.
  AvConductor *getTracker()const{return tracker_;}
  typedef struct {int x;int y;} Point;

protected:

  virtual void processMenu (char * buttonName, int choice, Widget);

  // When making an instance of this class, should we make the menus or not?
  // It is anticipated that classes derived from this will want to make 
  // their own menus.
  void init (int makeMenus = TRUE);

private:

  AvMenu * scaleMenu_, *xScaleMenu_, *yScaleMenu_;

  Pixel menuBackgroundColor_;

  // This flag indicates whether or not this window is to be included in
  // an operation involving more than one window (e.g., synchronized picking
  // or movies, etc.)  The flag is set by user interaction with the "Options"
  // menu.
  int included_;
  Boolean track3D_;	// If set (default), included windows track in 3D.
  // Is the imageViewPanel inside or out?
  Boolean panelIsInside_;

#ifdef AVOGL
  AvGLCanvas *canvas_;
#else
  AvXWindow * canvas_;
#endif

  // the drawing area created by and holding the canvas
  Widget drawArea_;

  // The item that is being displayed
  AvImageViewItem * vItem_;

  // The raster being displayed
  AvCLUImage * raster_;

  // the user interface panel with info (physical coordinates, etc.)
  // about this view
  AvImageViewPanel * viewPanel_;

  // the factor by which to scale the contents of this window
  float xScaleFactor_;
  float yScaleFactor_;
  Boolean equalScales_;	// If true x & y scales are forced to be the same
			// during drawing.
  Widget  equalScalesW_; // Widget for equal scale toggle.

  AvAccessor *accessor_;
  // Current size of window in pixels.
  Dimension windowHeight_, windowWidth_;


  // --------------------
  // items relating to interacting with the image, i.e., 
  // drawing the region, getting data values, etc.

  GC gc_;

  Cursor feedbackCursor_;

  //	(See note at start of file for what "SR" means).
  // SR coordinates (indices into the data set) of the current point.
  Point cursor_;

  // SR coordinates for start and end x and y of the current box's outline.
  Point boxStart_, boxEnd_;
  // Has box been started?
  inline Boolean boxStarted()const {return boxStarted_;}
  Boolean boxStarted_;
  // Has a box been drawn?
  inline Boolean boxDrawn()const {return boxDrawn_;}
  Boolean boxDrawn_;
  BOXTYPE boxType_;
  Boolean drawCross_;	// If true, a cross is drawn with the box.
  REGIONTYPE regionType_;

  AvConductor *tracker_;	// For mouse change notification.

  // Update cursor position, draw box.
  // Local means X cursor is in this window.
  Boolean processMouse (const int xSR, const int ySr, const Boolean local);

  void mouseDown(const int xSR, const int ySR, const Boolean local);
  void mouseDown (const AvPosition &p);
  void mouseMove (const int xSR, const int ySR, const Boolean local);
  void mouseMove (const AvPosition &p);
  void mouseUp (const int xSR, const int ySR, const Boolean local);
  void mouseUp (const AvPosition &pos);
  // Convenience routine that returns TRUE if the region toggle button
  // (in IVP) is on.
  Boolean drawingRegion()const;

  // Draw a box at these coordinates.
  // Values are SR.
  void drawBox(const Point &start, const Point &end);
  // Draw box using x/y as endpoints. If box hasn't been started, x & y
  // will also be used as endpoints. Values are SR coordinates.
  void drawBox(const int x, const int y, const BOXTYPE boxtype);
  // Redraw box (Region or SW cursor) using current values.
  void drawBox ();
  void unDrawBox ();
  // This could be public.
  enum DrawMode { NORMAL, MODE1, MODE2, MODEINVISIBLE};
  void setRegionDrawMode(const DrawMode);
  // Call these when the view info panel is displayed, to handle
  // user interaction
  void activateFeedbackHandlers ();
  void deactivateFeedbackHandlers ();

  static void redrawCB (Widget, AvImageWindow *, XtPointer);

  static void enterDrawAreaCB (Widget, AvImageWindow * w, XEvent * event);
  static void mouseDownCB (Widget w, AvImageWindow * obj, XEvent * event);
  static void mouseMoveCB (Widget w, AvImageWindow * obj, XEvent * event);
  static void mouseUpCB (Widget w, AvImageWindow * obj, XEvent * event);

  void resize (Widget w);
  static void resizeCB (Widget w, AvImageWindow * obj,
					XmDrawingAreaCallbackStruct *);
  static void canvasResizeCB (AvEvent &, AvPointer);

  AvAccessor *accessor()const{return accessor_;}

  // Callback to receive accessor notices.
  static void accessorCB_(XtPointer me, AvConductor *c, XtPointer uData);
  // Callback to actually handle accessor notices.
  void accessorCB(AvConductor *c, XtPointer uData);
  // Build the scale, xscale or yscale menus.
  void buildScaleMenu(AvMenu *menu, const float estScale, const int size,
		      const int maxSize, const int option);
  // Convert a scale menu selection to a floating point number.
  float convertScaleMenu(const char *value);
  // Use the value from convertScaleMenu to change the current scale
  // and update image.
  void updateScale(const float xscl, const float yscl);
  Widget equalScalesW();
  // Printing support.
  void printImage();
  void showPrintOptions();
  AvPrintConfig	*printConfig_;
  AvPrintOptions *printOptions_;

  // For menus
enum {
  SV_OPTION = 200,
  SV_SCALE, SV_XSCALE, SV_YSCALE,
  SV_SHOW,
  SV_SETUP
};

};

#endif
