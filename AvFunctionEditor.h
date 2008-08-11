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
//# --------------------------------------------------
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvFunctionEditor.h,v 19.0 2003/07/16 05:47:12 aips2adm Exp $
//
// $Log: AvFunctionEditor.h,v $
// Revision 19.0  2003/07/16 05:47:12  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:55  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:24  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:31  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:15  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:54:02  hravlin
// Changed some char * to const char *.
//
// Revision 14.0  2000/03/23 16:08:16  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/09/15 21:35:58  hravlin
// Copyright date update.
//
// Revision 13.0  1999/08/10 18:40:03  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:44  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:53  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:45  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/09 21:31:27  hr
// Fixed several type conversion warnings.
//
// Revision 9.0  1997/08/25 21:27:31  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:45  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:22:19  droberts
// Final update from monet archive.
//
// Revision 1.2  1996/08/19 16:42:52  pixton
// Cleaned up g++ -Wall warnings.
//
// Revision 1.1  1996/07/11  21:31:21  pixton
// Automated Checkin
//
//  Revision 1.8  1996/06/18  18:42:52  pixton
//  Copyright Update
//
//  Revision 1.7  1996/03/05  17:27:37  hr
//  Added init2zero().
//
//  Revision 1.6  1995/11/16  17:33:01  pixton
//  added stuff.
//
//  Revision 1.5  1995/11/14  21:31:14  pixton
//  callback fixes.
//
//  Revision 1.4  1995/11/13  22:22:51  pixton
//  added button release callback.
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
//  Revision 1.2  1995/07/26  19:32:41  hr
//  mode_ wasn't being initialized.
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------



#ifndef __AvFunctionEditor_h__
#define __AvFunctionEditor_h__

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>

#include <Xm/Xm.h>

#include <AvRampFunction.h>
#include <AvPointList2D.h>

typedef void (*AvFunctionEditorCallback)(XtPointer);


//  <summary>
//  Function Editor Interface allows users to design functions to be used
//  in other parts of the program.
//  </summary>
//
//  <synopsis>
//  Responsibility
//
//  AvFunctionEditor performs the following duties:
//
//  [X] Controls an AvRampfunction to provide a discrete function with
//      a selectable number of values.
//  
//  [X] Controls a drawArea widget that may be resized horizontally and vertically.
//
//  [ ] User selection of available functions
//
//  [X] Callback functions for graph data changes and graph mode changes
//
//  [X] Depends on caller for colors - Does not allocate any colors
//
//  [ ] Centers the graph and shows map in reduced form when < 256 colors available
//
//  </synopsis>
//

class AvFunctionEditor
{
//----- User Information/Constants
public:

  enum { DRAW=0, CONSTANT, LINEAR, LOG, 
	 PWCONSTANT, PWLINEAR, SPLINE, REPEAT, NFUNCTIONS };

//----- Functions
public:

  AvFunctionEditor(Widget parent,                    // parent widget
		   unsigned long foregroundPixel,    // pixel to use for foreground
		   unsigned long backgroundPixel,    // pixel to use for background
		   unsigned long pointColorPixel,    // pixel to use for control points
		   unsigned long blackPixel,         // pixel to use for black
		   int width = 256,                  // width of graph
		   int height = 60)                  // height of graph
    : valueChangedCallback_(0), 
      modeChangedCallback_(0), 
      minY_(0), 
      maxY_(0.99999F),
      mode_(DRAW),

      graphWidth_(width), 
      graphHeight_(height),

      pointColorPixel_(pointColorPixel), 
      foregroundPixel_(foregroundPixel), 
      backgroundPixel_(backgroundPixel), 
      blackPixel_(blackPixel),

      parent_(parent)
    {
      display_ = XtDisplay(parent_);
      buildWidgetTree(XtScreen(parent_));
      func_ = new AvRampFunction(width);
    }

private:

  static void graphExposedCB
    (Widget widget, AvFunctionEditor * fe, XmDrawingAreaCallbackStruct *cbs);
  
  static void drawModeCB
    (Widget widget, AvFunctionEditor * fe, XEvent *);
  
  static void constantModeCB
    (Widget widget, AvFunctionEditor * fe, XEvent *);
  
  static void linearModeCB
    (Widget widget, AvFunctionEditor * fe, XEvent *);
  
  static void logModeCB
    (Widget widget, AvFunctionEditor * fe, XEvent *);
  
  static void pwConstantModeCB
    (Widget widget, AvFunctionEditor * fe, XEvent *);
  
  static void pwLinearModeCB
    (Widget widget, AvFunctionEditor * fe, XEvent *);
  
  static void splineModeCB
    (Widget widget, AvFunctionEditor * fe, XEvent *);
  
  static void repeatModeCB
    (Widget widget, AvFunctionEditor * fe, XEvent *);
  
  static void motionCB
    (Widget widget, AvFunctionEditor * fe, XEvent *);

  static void graphEnterEH  // Event Handler
    (Widget widget, AvFunctionEditor * fe, XEvent *, Boolean *);

  static void graphLeaveEH
    (Widget widget, AvFunctionEditor * fe, XEvent *, Boolean *);

  static void graphArmCB
    (Widget widget, AvFunctionEditor * fe, XEvent *);

  static void graphMotionCB
    (Widget widget, AvFunctionEditor * fe, XEvent *);
   
  static void graphInputCB
    (Widget widget, AvFunctionEditor * fe, XmDrawingAreaCallbackStruct * cb);

  static void graphResizeCB
    (Widget widget, AvFunctionEditor * fe, XmDrawingAreaCallbackStruct * cb);

  // Prep the pixmap for use with a button

private:

  void buildWidgetTree(Screen * screen);
  void setButtonPixmap(XImage& img, void * data, int w, int h);
  void initButtonPixmaps();
  void init2zero();

public:

  // Map a function x value to the displayed pixel value
  short func2GraphX(float x)
//    { return ((int) (x * graphWidth_) / func_->length()); }
    { return (short)( (x * graphWidth_) / (float)func_->length()); }

  // Map a function y value to the displayed pixel value
  short func2GraphY(float y)
    { return (graphHeight_ - 1 - ((int) (MIN (maxY_ - minY_, y - minY_) * graphHeight_))); }

  // Map an x pixel in the widget to a function value
  float graph2FuncX(short x)
    { return (((float) x * (float)func_->length()) / graphWidth_); }

  // Map a y pixel in the widget to a function value
  float graph2FuncY(short y)
    { return (minY_ + (graphHeight_ - 1 - y) * ((maxY_ - minY_) / (graphHeight_ - 1))); }

  // Return/Set the maximum Y values the graph represents
  float maxY() const { return maxY_; }
  void maxY(float y) { maxY_ = y;    }  

public:

  void drawModeMotion();
  int addControlPoint(float x, float y);
  void grabControlPoint();
  void dragControlPoint();
  void drawControlPoint();

  int grabbed() const { return grabbed_; }
  void grabbed(int n) { grabbed_ = n;    }

  Window queryPointer(Widget w, int * x, int * y);

  void drawModeArm(Widget w);

  int drawMode() const { return (mode_ == DRAW);   }
  int mode()     const { return mode_;             }

  void mode(int n, int cb = 1)     
    { mode_ = CLAMP(n, 0, SPLINE); if (cb) doModeChangedCallback(); }

  void gotoMode(int n, int cb = 1);

  void repeatMode(int onOff, int cb = 1);
  int repeatMode() const { return (func_->waveMode() == AvRampFunction::REPEAT); }
  
  AvRampFunction * func() const { return func_; }

  void redraw() { redrawGraph(); if (!drawMode()) redrawControlPoints(); }
  void redrawGraph(); 
  void resizeGraph(int width, int height, int cb = 1);
  void redrawControlPoints();

  void label(const char * s);
  void labelColor(Pixel pixel);

  Widget graph() const { return graph_; }
  int graphWidth() const { return graphWidth_; }
  int graphHeight() const { return graphHeight_; }

  void graphWidth(int w, int cb = 1)
    {
      Dimension h;
      XtVaGetValues(graph_, XmNheight, &h, NULL);
      resizeGraph(w, h, cb);
    }

  void graphHeight(int h, int cb = 1)
    {
      Dimension w;
      XtVaGetValues(graph_, XmNwidth, &w, NULL);
      resizeGraph(w, h, cb);
    }
  
  void valueChangedCallback(AvFunctionEditorCallback f) 
    { valueChangedCallback_ = f; }
  AvFunctionEditorCallback valueChangedCallback()      
    { return valueChangedCallback_; }

  void modeChangedCallback(AvFunctionEditorCallback f) 
    { modeChangedCallback_ = f; }
  AvFunctionEditorCallback modeChangedCallback()       
    { return modeChangedCallback_; }

  void repeatChangedCallback(AvFunctionEditorCallback f) 
    { repeatChangedCallback_ = f; }
  AvFunctionEditorCallback repeatChangedCallback()       
    { return repeatChangedCallback_; }

  void buttonPressCallback(AvFunctionEditorCallback f)
    { buttonPressCallback_ = f; }
  AvFunctionEditorCallback buttonPressCallback()
    { return buttonPressCallback_; }

  void buttonReleaseCallback(AvFunctionEditorCallback f)
    { buttonReleaseCallback_ = f; }
  AvFunctionEditorCallback buttonReleaseCallback()
    { return buttonReleaseCallback_; }

  void doValueChangedCallback();
  void doModeChangedCallback();
  void doRepeatChangedCallback();
  void doButtonPressCallback();
  void doButtonReleaseCallback();
  void userData(XtPointer v) { userData_ = v; }

  // Draw the graph only in the (drawable) widget provided
  void XDraw(Widget w, GC gc, int border = 0);

private:

  int graphXOffset()
    {
      Dimension w, mw;
      XtVaGetValues(graph_, XmNwidth, &w, NULL);
      XtVaGetValues(graphMgr_, XmNwidth, &mw, NULL);
      return MAX(0, (mw - w)/2);
    }

  AvFunctionEditorCallback valueChangedCallback_;  // value changed callback
  AvFunctionEditorCallback modeChangedCallback_;   // called when graph changes mode
  AvFunctionEditorCallback repeatChangedCallback_; // ditto for wavify button
  AvFunctionEditorCallback buttonPressCallback_;   // called when button pressed in editor
  AvFunctionEditorCallback buttonReleaseCallback_; // called when button released in editor

  XtPointer userData_;

  float minY_, maxY_;

  int lastPointerX_;
  int lastPointerY_;
  int grabbed_;            // point Ndx or -1 for no point.
  int mode_;               // Function Editor active function

  int one2one_;            // FLAG: T => width of graph_ = width of func
  int graphWidth_;         // Cached value from graph_ Widget width
  int graphHeight_;        // ditto, height

  AvRampFunction * func_;  // function data manager for the graph

  Display * display_;
  GC gc_;                  // Context for graphs 
  GC xorGc_;               

  unsigned long pointColorPixel_;
  unsigned long foregroundPixel_;
  unsigned long backgroundPixel_;
  unsigned long blackPixel_; 

  Widget parent_;          // Parent class of this widget
  Widget workArea_;        // xmForm for func editor 
  Widget topRow_;
  Widget uiFuncs_[NFUNCTIONS];  // User Interface Function Buttons
  Widget graphMgr_;
  Widget graph_;           // 
  Widget label_;           // XmLable for graph color

  XImage ximage_[NFUNCTIONS];

};

#endif
