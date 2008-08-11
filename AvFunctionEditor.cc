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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvFunctionEditor.cc,v 19.0 2003/07/16 05:47:23 aips2adm Exp $
//
// $Log: AvFunctionEditor.cc,v $
// Revision 19.0  2003/07/16 05:47:23  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:04  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:33  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:40  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:53  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:53:44  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:08:27  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.3  1999/10/01 21:56:58  hravlin
// *** empty log message ***
//
// Revision 13.2  1999/09/15 21:35:14  hravlin
// Use global X values rather than root's and pass depth value when getting
// pixmaps.
//
// Revision 13.1  1999/08/25 19:52:39  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:40:15  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:06  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:24  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:21  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/09 21:30:53  hr
// Fixed several type conversion warnings.
//
// Revision 9.0  1997/08/25 21:28:29  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:17:24  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:20:17  droberts
// Final update from monet archive.
//
// Revision 1.3  1996/08/14 20:21:36  pixton
// replaced out-of-scope i with its correct value.
//
// Revision 1.2  1996/08/14  18:47:52  hr
// Changes to remove g++ warnings (from bglenden).
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.12  1996/06/18  18:46:02  pixton
//  Copyright Update
//
//  Revision 1.11  1996/03/05  17:26:55  hr
//  Added init2zero() to get rid of some unitialized function pointers.
//
//  Revision 1.10  1995/11/27  16:12:58  baker
//  Cleaned up layout, eliminating some unnecessary widgets.
//
//  Revision 1.9  1995/11/16  17:34:00  pixton
//  no idea.
//
//  Revision 1.8  1995/11/14  21:29:47  pixton
//  callback cleanups
//
//  Revision 1.7  1995/11/13  22:15:56  pixton
//  added buttonrelease callback to satisfy design.
//
//  Revision 1.6  1995/09/20  21:23:18  pixton
//  FunctionEditor reopen bug workaround.
//
//  Revision 1.5  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/09/12  16:10:03  hr
//  Added casts to int in call to linearLocal.
//
//  Revision 1.3  1995/07/18  17:27:17  hr
//  'Fixed' g++ warnings. Mostly don't do "for(int i...)".
//
//  Revision 1.2  1995/05/15  17:18:29  hr
//  Removed 'GraphWidth set' message for alpha release 2.
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------


//
//  AvFunctionEditor.cc
//

#include <draw.icon>
#include <constant.icon>
#include <linear.icon>
#include <log.icon>
#include <pwConstant.icon>
#include <pwLinear.icon>
#include <spline.icon>
#include <sawtooth.icon>

#include <stdlib.h>
#include <Xm/Xm.h>
#include <Xm/Label.h>
#include <Xm/ToggleB.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>
#include <Xm/DrawingA.h>
#include <Xm/Separator.h>
#include <Xm/TextF.h>
#include <AvFunctionEditor.h>

#include <AvApp.h>

static const int CONTROLPOINTRADIUS = 6;

void AvFunctionEditor::doValueChangedCallback()
{
  if (valueChangedCallback_)
    valueChangedCallback_(userData_);
}

void AvFunctionEditor::doModeChangedCallback()
{
  if (modeChangedCallback_)
    modeChangedCallback_(userData_);
}

void AvFunctionEditor::doRepeatChangedCallback()
{
  if (repeatChangedCallback_)
    repeatChangedCallback_(userData_);
}

void AvFunctionEditor::doButtonReleaseCallback()
{
  if (buttonReleaseCallback_)
    buttonReleaseCallback_(userData_);
}

void AvFunctionEditor::doButtonPressCallback()
{
  if (buttonPressCallback_)
    buttonPressCallback_(userData_);
}

static const char *functionsStr_[AvFunctionEditor::NFUNCTIONS] = 
{ "draw", "constant", "linear", "log",
  "pwConstant", "pwLinear", "spline", "repeat"};

void AvFunctionEditor::label(const char * s)
{  
  XtVaSetValues(label_, XmNvalue, s, NULL);
}

void AvFunctionEditor::labelColor(Pixel pixel)
{ XtVaSetValues(label_, XmNforeground, pixel, NULL); }

void AvFunctionEditor::setButtonPixmap(XImage & img, void * data, int w, int h)
{
  img.data = (char *) data;  img.width = w; img.height = h;
  img.xoffset=0; img.format = XYBitmap; img.byte_order = MSBFirst; 
  img.bitmap_pad = 0; img.bitmap_bit_order = LSBFirst; img.bitmap_unit = 8;
  img.depth = 1; img.obdata = NULL; img.bytes_per_line = 3; 
}


void AvFunctionEditor::initButtonPixmaps()
{
  setButtonPixmap(ximage_[DRAW], draw_bits, draw_width, draw_height);
  setButtonPixmap(ximage_[CONSTANT], constant_bits, constant_width, constant_height);
  setButtonPixmap(ximage_[LINEAR], linear_bits, linear_width, linear_height);
  setButtonPixmap(ximage_[LOG], log_bits, log_width, log_height);
  setButtonPixmap(ximage_[PWCONSTANT], pwConstant_bits, pwConstant_width, pwConstant_height);
  setButtonPixmap(ximage_[PWLINEAR], pwLinear_bits, pwLinear_width, pwLinear_height);
  setButtonPixmap(ximage_[SPLINE], spline_bits, spline_width, spline_height);
  setButtonPixmap(ximage_[REPEAT], sawtooth_bits, sawtooth_width, sawtooth_height);
  
  for (int i = 0; i < NFUNCTIONS; i++)
  { char *str;
	str = AvApp::Strdup(functionsStr_[i]);
	XmInstallImage (&ximage_[i], str);
	delete [] str;
  }
} 

void makeCursor()
{
  // Build cursors to use while within the graph window
  // drawCursor_ = ...
  // pointCursor_ = ...
}

// Initialize everything not alread set by constructor.
void AvFunctionEditor::init2zero()
{
	repeatChangedCallback_ = NULL;
	buttonPressCallback_ = NULL;
	buttonReleaseCallback_ = NULL;
	userData_ = NULL;
	lastPointerX_ = 0;
	lastPointerY_ = 0;
	grabbed_ = -1;            // point Ndx or -1 for no point.
	mode_ = 0;               // Function Editor active function
	one2one_ = 0;            // FLAG: T => width of graph_ = width of func
	func_ = NULL;
	gc_ = NULL;
	xorGc_ = NULL;
	workArea_ = NULL;
	topRow_ = NULL;
	graphMgr_ = NULL;
	graph_ = NULL;
	label_ = NULL;
}

void AvFunctionEditor::buildWidgetTree(Screen * screen)
{
  init2zero();
  initButtonPixmaps();
  makeCursor();

  //-----colors for panel
  XGCValues gcv; gcv.background = AvApp::blackPixel();
  gc_ = XCreateGC(display_, AvApp::referenceWindow(), GCBackground, &gcv);
  gcv.function = GXor;
  xorGc_ = XCreateGC(display_, AvApp::referenceWindow(), GCFunction, &gcv);
 
  workArea_ = XtVaCreateManagedWidget
    ("workArea", xmFormWidgetClass, parent_, NULL);
  
  label_ = XtVaCreateManagedWidget
    ("label", xmTextFieldWidgetClass, workArea_,
     XmNresizeWidth, True,
     XmNcolumns, 1,
     XmNeditable, False,
     XmNcursorPositionVisible, False,
     XmNshadowThickness, 0,
     NULL);

  Pixel bg;
  int depth;
  XtVaGetValues(workArea_,
	XmNbackground, &bg,
	XmNdepth, &depth,
	NULL);

  Widget functionSelector = XtVaCreateManagedWidget
    ("functionSelector", xmRowColumnWidgetClass, workArea_,
     XmNradioBehavior, True,
     XmNradioAlwaysOne, True,
     XmNorientation, XmHORIZONTAL,
     XmNtopAttachment, XmATTACH_FORM,
     XmNtopOffset, 2,
     XmNleftAttachment, XmATTACH_WIDGET,
     XmNleftWidget, label_,
     XmNmarginHeight, 0,
     XmNspacing, 0,
     NULL);
  
  for (int i = DRAW; i < NFUNCTIONS - 1; i++) {
    char *str = AvApp::Strdup(functionsStr_[i]);
    uiFuncs_[i] = XtVaCreateManagedWidget
      (functionsStr_[i], xmToggleButtonWidgetClass, functionSelector,
       XmNlabelType, XmPIXMAP,
       XmNlabelPixmap, XmGetPixmapByDepth(screen, str, blackPixel_, bg, depth),
       XmNshadowThickness, 1,
       XmNindicatorOn, False,
       XmNset, (i == DRAW) ? True : False,
       NULL);
    delete [] str;
  }
  
  char *uistr = AvApp::Strdup(functionsStr_[REPEAT]);
  uiFuncs_[REPEAT] = XtVaCreateManagedWidget
    (functionsStr_[REPEAT], xmToggleButtonWidgetClass, workArea_,
     XmNlabelType, XmPIXMAP,
     XmNlabelPixmap,
     XmGetPixmapByDepth(screen, uistr, blackPixel_, bg, depth),
     XmNtopAttachment, XmATTACH_FORM,
     XmNtopOffset, 2,
     XmNleftAttachment, XmATTACH_WIDGET,
     XmNleftWidget, functionSelector,
     XmNleftOffset, 3,
     XmNshadowThickness, 1,
     XmNindicatorOn, False,
     NULL);
  delete [] uistr;
  
  XtAddCallback(uiFuncs_[DRAW], XmNvalueChangedCallback,
		(XtCallbackProc) &AvFunctionEditor::drawModeCB,
		(XtPointer) this);
  XtAddCallback(uiFuncs_[CONSTANT], XmNvalueChangedCallback,
		(XtCallbackProc) &AvFunctionEditor::constantModeCB,
		(XtPointer) this);
  XtAddCallback(uiFuncs_[LINEAR], XmNvalueChangedCallback,
		(XtCallbackProc) &AvFunctionEditor::linearModeCB,
		(XtPointer) this);
  XtAddCallback(uiFuncs_[LOG], XmNvalueChangedCallback,
		(XtCallbackProc) &AvFunctionEditor::logModeCB,
		(XtPointer) this);
  XtAddCallback(uiFuncs_[PWCONSTANT], XmNvalueChangedCallback,
		(XtCallbackProc) &AvFunctionEditor::pwConstantModeCB,
		(XtPointer) this);
  XtAddCallback(uiFuncs_[PWLINEAR], XmNvalueChangedCallback,
		(XtCallbackProc) &AvFunctionEditor::pwLinearModeCB,
		(XtPointer) this);
  XtAddCallback(uiFuncs_[SPLINE], XmNvalueChangedCallback,
		(XtCallbackProc) &AvFunctionEditor::splineModeCB,
		(XtPointer) this);
  XtAddCallback(uiFuncs_[REPEAT], XmNvalueChangedCallback,
		(XtCallbackProc) &AvFunctionEditor::repeatModeCB,
		(XtPointer) this);
  
  static char graphTranslations[] =
    "<BtnDown>:    DrawingAreaInput() \n"
      "<BtnUp>:      DrawingAreaInput() \n" 
	"<BtnMotion>:  DrawingAreaInput()";
  
  graph_ = XtVaCreateManagedWidget
    ("graph", xmDrawingAreaWidgetClass, workArea_,
     XmNtopAttachment, XmATTACH_WIDGET,
     XmNtopWidget, functionSelector,
     XmNtopOffset, 4,
     XmNwidth,  graphWidth(),
     XmNheight, graphHeight(),
     XmNresizePolicy, XmRESIZE_NONE,
     XmNuserData, (XtPointer) NFUNCTIONS,
     XmNmarginHeight, 0,
     XmNmarginWidth, 8,
     XmNbackground, backgroundPixel_,
     NULL);

  XtAddCallback (graph_, XmNexposeCallback, 
		 (XtCallbackProc) &AvFunctionEditor::graphExposedCB,
		 (XtPointer) this);
  
  XtOverrideTranslations (graph_,
			  XtParseTranslationTable (graphTranslations));
  
  XtAddCallback (graph_, XmNinputCallback,
		 (XtCallbackProc) &AvFunctionEditor::graphInputCB,
		 (XtPointer) this);
  
  XtAddCallback (graph_, XmNresizeCallback,
		 (XtCallbackProc) &AvFunctionEditor::graphResizeCB,
		 (XtPointer) this);

  XtAddEventHandler (graph_, EnterWindowMask, 1,
		     (XtEventHandler) &AvFunctionEditor::graphEnterEH,
		     (XtPointer) this);
  XtAddEventHandler (graph_, LeaveWindowMask, 1,
		     (XtEventHandler) &AvFunctionEditor::graphLeaveEH,
		     (XtPointer) this);
} 


// Set the repeat wave function
void AvFunctionEditor::repeatMode(int onOff, int cb)
{
  if (onOff) {
    XmToggleButtonSetState(uiFuncs_[REPEAT], 1, 0);
    func_->waveMode(AvRampFunction::REPEAT);
    func_->recalc();
    if (cb) {
      doRepeatChangedCallback();
      doValueChangedCallback();
      doButtonReleaseCallback();
    }
    redrawGraph();

  }
  else {
    XmToggleButtonSetState(uiFuncs_[REPEAT], 0, 0);
    func_->waveMode(AvRampFunction::EXTEND);
    func_->recalc();
    if (cb) {
      doRepeatChangedCallback();
      doValueChangedCallback();
      doButtonReleaseCallback();
    }
    redrawGraph();
  }
}

void AvFunctionEditor::graphEnterEH
(Widget, AvFunctionEditor * , XEvent * , Boolean *)
{
  // printf("FE::graphEnterEH()\n");

  // Switch to the appropriate cursor model
  //
  // if (mode() == DRAW) [use pencil cursor]
  // else [use crosshair cursor]
  //
  // // The cursor color was already defined.
  //
}

void AvFunctionEditor::graphLeaveEH
(Widget, AvFunctionEditor *, XEvent *, Boolean *)
{
  // printf("FE::graphLeaveEH()\n");
}

//-----Switch to new mode
void AvFunctionEditor::gotoMode(int m, int cb)
{
  XmToggleButtonSetState(uiFuncs_[mode()], 0, 0);
  XmToggleButtonSetState(uiFuncs_[m], 1, cb);
  mode(m, cb);
  redrawGraph();
}

//---------------------------------------------------------------------
//
//  Resize Callback - issued by the graph_ widget (attached to form)
//
//  Purpose is to set the cached values graphWidth_ and graphHeight_
//
//---------------------------------------------------------------------

void AvFunctionEditor::graphResizeCB(Widget w, AvFunctionEditor * fe, XmDrawingAreaCallbackStruct *)
{
  // Reset the current width and height so the function will be drawn
  // correctly within the graph window.
  //

  Dimension width, height;
  XtVaGetValues(w, XmNwidth, &width, XmNheight, &height, NULL);
  fe->resizeGraph(width, height);
}

void AvFunctionEditor::resizeGraph(int width, int height, int cb)
{
  XResizeWindow(display_, XtWindow(graph_), width, height);
  XtVaSetValues(graph_, XmNwidth, width, XmNheight, height, NULL);
  graphWidth_ = width;
  graphHeight_ = height;
//  printf("GraphWidth set to %d, h to %d\n", width, height);

  if (cb) redrawGraph();
}

//---------------------------------------------------------------------
//
//  Mode Change Callbacks
//
//---------------------------------------------------------------------
void AvFunctionEditor::drawModeCB(Widget , AvFunctionEditor * fe,
				  XEvent *)
{
  fe->mode(DRAW);
  fe->redraw();
}

void AvFunctionEditor::constantModeCB(Widget , AvFunctionEditor * fe, XEvent *)
{
  fe->mode(CONSTANT);
  AvPointList2D * pl = fe->func()->pts();
  pl->lock(0);
  if (pl->length() >= 1)
    {
      while (pl->length() > 1) pl->remove(1);
    }
  else
    {
      pl->clear();
      pl->add((float)(fe->func()->length()/2), 0.5);
    }
  pl->lock(1);
  fe->repeatMode(0);
  fe->func()->orderMode(AvRampFunction::CONSTANT);
  fe->func()->recalc();
  fe->doValueChangedCallback();
  fe->doButtonReleaseCallback();
  fe->redraw();
}

void AvFunctionEditor::linearModeCB(Widget , AvFunctionEditor * fe, XEvent *)
{
  fe->mode(LINEAR);
  AvPointList2D * pl = fe->func()->pts();
  pl->lock(0);
  
  if (pl->length() == 1)
    {
      // Reflect point
      float x1,y1; pl->getValue(0, &x1, &y1);
      pl->add((float)fe->func()->length() - x1, 1.0 - y1);
      pl->xSort();
    }

  if (pl->length() >= 2)
    {
      while (pl->length() > 2) pl->remove(1);
    }
  else
    {
      pl->clear();
      pl->add((int) (0.1 * (int)(fe->func()->length()-1)),0);
      pl->add((int) (0.9 * (int)(fe->func()->length()-1)),1.0);
      pl->xSort();
    }
  pl->lock(1);
  fe->func()->orderMode(AvRampFunction::LINEAR);
  fe->func()->recalc();
  fe->doValueChangedCallback();
  fe->doButtonReleaseCallback();
  fe->redraw();
}

void AvFunctionEditor::logModeCB(Widget, AvFunctionEditor * fe, 
				   XEvent *)
{
  fe->mode(LOG);
  AvPointList2D * pl = fe->func()->pts();
  pl->lock(0);

  if (pl->length() == 2)
    {
      // add third point in middle
      float x1,y1,x3,y3;
      pl->getValue(0, &x1, &y1);
      pl->getValue(1, &x3, &y3);
      pl->add((x1 + x3) / 2, (y1 + y3)/2);
      pl->xSort();
    }

  if (pl->length() >= 3)
    while (pl->length() > 3) pl->remove(2);
  else
    {
      pl->clear();
      pl->add(0,0);
      pl->add((float)(fe->func()->length()/2), 0.3);
      pl->add((float)(fe->func()->length()-1), 1.0);
      pl->xSort();
    }
  pl->lock(1);

  fe->func()->orderMode(AvRampFunction::EXPONENTIAL);
  fe->func()->recalc();
  fe->doValueChangedCallback();
  fe->doButtonReleaseCallback();
  fe->redraw();
}

void AvFunctionEditor::pwConstantModeCB(Widget , AvFunctionEditor * fe, 
				      XEvent *)
{
  fe->mode(PWCONSTANT);
  AvPointList2D * pl = fe->func()->pts();
  pl->lock(0);
  fe->func()->orderMode(AvRampFunction::CONSTANT);
  fe->func()->recalc();
  fe->doValueChangedCallback();
  fe->doButtonReleaseCallback();
  fe->redraw();
}

void AvFunctionEditor::pwLinearModeCB(Widget , AvFunctionEditor * fe, 
				      XEvent *)
{
  fe->mode(PWLINEAR);
  fe->func()->pts()->lock(0);
  fe->func()->orderMode(AvRampFunction::LINEAR);
  fe->func()->recalc();
  fe->doValueChangedCallback();
  fe->doButtonReleaseCallback();
  fe->redraw();
}

void AvFunctionEditor::splineModeCB(Widget , AvFunctionEditor * fe, 
				   XEvent *)
{
  fe->mode(SPLINE);
  fe->func()->pts()->lock(0);
  fe->func()->orderMode(AvRampFunction::SPLINE);
  fe->func()->recalc();
  fe->doValueChangedCallback();
  fe->doButtonReleaseCallback();
  fe->redraw();
}

//
//  Wave Modifier
//

void AvFunctionEditor::repeatModeCB(Widget tbg, AvFunctionEditor * fe, 
				    XEvent *)
{
  int dontApply = 
    (fe->mode() == AvFunctionEditor::DRAW  ||
     fe->mode() == AvFunctionEditor::CONSTANT);

  if (!dontApply)
    {
      if (XmToggleButtonGetState(tbg))  // ON
	{
	  fe->repeatMode(1);
	}
      else  // OFF
	{
	  fe->repeatMode(0);
	}
    }
  else
    {
      XmToggleButtonSetState(tbg, 0, 0);
    }
}

//=====================
//
//  CONTEXT SWITCHING - Event Handlers responding to widget entry by pointer
//
//=====================

//=================================================================
//
// UTILITIES
//
// Accepts a window and two x,y pointers.  Returns in x and y the
// local pointer location as well as the returned window from 
// XQueryPointer
//
//=================================================================

Window AvFunctionEditor::queryPointer(Widget w, int * x, int * y)
{
  Window root,win; int rootX, rootY; unsigned int mask;
  XQueryPointer(display_, XtWindow(w),  
		&root, &win, &rootX, &rootY, x, y, &mask);
  return win;
}

//======================
//
//  EXPOSURE CALLBACKS
//
//======================

void AvFunctionEditor::graphExposedCB
(Widget, AvFunctionEditor *fe, XmDrawingAreaCallbackStruct *)
{
  fe->redrawGraph();
}

//================================
//
//  GRAPH MODIFICATION CALLBACKS
//
//================================

//
//  GRAPH ARM
//
void AvFunctionEditor::graphArmCB
(Widget widget, AvFunctionEditor *fe, XEvent *)
{
  if (fe->drawMode())
    {
      fe->drawModeArm(widget);
    }
  else  // grab OR ADD control point
    {
      fe->grabControlPoint();
    }
}

// [ ] Fix Hard Code Consts
void AvFunctionEditor::drawModeArm(Widget widget)
{
  queryPointer(widget, &lastPointerX_, &lastPointerY_);
  func()->setValue(int(graph2FuncX(lastPointerX_)),
			 graph2FuncY(lastPointerY_));
  doValueChangedCallback();
  redrawGraph();
}

void AvFunctionEditor::grabControlPoint()
{
  AvPointList2D * pl = func_->pts();
  if (!pl->empty())
    {
      int x,y;
      queryPointer(graph(), &x, &y);
      int ndx = pl->nearestXNdx(graph2FuncX(x));
      
      float px, py;
      pl->getValue(ndx, &px, &py); 

      if ((abs(func2GraphX(px) - x)) < CONTROLPOINTRADIUS)
	grabbed_ = ndx;
      else
	grabbed_ = addControlPoint(graph2FuncX(x),
				   graph2FuncY(y));

      if (grabbed_ >= 0) pl->setValue(grabbed_, 
				      graph2FuncX(x),
				      graph2FuncY(y)); 
    }
  else
    {
      int x,y;
      queryPointer(graph(), &x, &y);
      pl->add(graph2FuncX(x), graph2FuncY(y));
    }
  func()->recalc();
  doValueChangedCallback();
  redrawGraph();
}

// FUNC COORD SYSTEM!
int AvFunctionEditor::addControlPoint(float x, float y)
{
  AvPointList2D * pl = func()->pts();
  pl->add(x, y);
  pl->xSort();
  return pl->find(x, y);
}


//  POINTER MOTION
void AvFunctionEditor::graphInputCB
(Widget w, AvFunctionEditor *fe, XmDrawingAreaCallbackStruct * cb)
{
  switch (cb->event->type)
    {
    case ButtonPress: 
      fe->doButtonPressCallback();
      graphArmCB(w, fe, (XEvent *) cb); 
      break;
    case MotionNotify:
      graphMotionCB(w, fe, (XEvent *) cb); break;
    case ButtonRelease:
      fe->doButtonReleaseCallback();
      break;
    }
}


void AvFunctionEditor::graphMotionCB
(Widget, AvFunctionEditor * fe, XEvent *)
{
  //  If in freehand mode, read new location and do linear
  //  fill between new and old points
  if (fe->drawMode()) {
    fe->drawModeMotion();
    fe->redraw();
  }
  else {
    if (fe->grabbed() >= 0) {
      fe->dragControlPoint();
      fe->func()->recalc();
      fe->doValueChangedCallback();
      fe->redrawGraph();
    }
  }
}

void AvFunctionEditor::drawModeMotion ()
{
  int x,y;
  queryPointer(graph(), &x, &y);

  float x1 = CLAMP(graph2FuncX(lastPointerX_), 0, (float)(func()->length()-1));
  float y1 = CLAMP(graph2FuncY(lastPointerY_), 0, .99999F);
  float x2 = CLAMP(graph2FuncX(x), 0, (float)(func()->length()-1));
  float y2 = CLAMP(graph2FuncY(y), 0, .99999F);

  func()->linearLocal((int)x1, y1, (int)x2, y2);
  doValueChangedCallback();

  lastPointerX_ = x;
  lastPointerY_ = y;
}

void AvFunctionEditor::dragControlPoint()
{
  AvPointList2D * pl = func()->pts();
  if (!pl->empty()) {
      float ox,oy;
      pl->getValue(grabbed(), &ox, &oy);

      int x,y;
      queryPointer(graph(), &x, &y);
      
      float gx = graph2FuncX(x);
      float gy = graph2FuncY(y);

      if (mode() == LOG && grabbed())
	{
	  float x0,y0,x1,y1,x2,y2; 
	  pl->getValue(0, &x0, &y0);
	  pl->getValue(1, &x1, &y1);
	  pl->getValue(2, &x2, &y2);
	  
	  if (y0 < y1 && y2 < y1)
	    {
	      if (grabbed() == 0)
		pl->setValue(2, x2, (y1+1)/2);
	      else
		pl->setValue(0, x0, (y1+1)/2);
	    }
	  
	  if (y0 > y1 && y2 > y1)
	    {
	      if (grabbed() == 0)
		pl->setValue(2, x2, (y1)/2);
	      else
		pl->setValue(0, x0, (y1)/2);
	    }

	  switch(grabbed())
	    {
	    case 0:
	      gx = CLAMP(gx, 0, x1);     
	      break;

	    case 1:
	      gx = CLAMP(gx, x0+1, x2-1);
	      gy = CLAMP(gy, MIN(y0,y2), MAX(y0,y2));
	      break;

	    case 2:
	      gx = CLAMP(gx, x1, (float)(func()->length()-1));
	      break;
	    }
	  pl->setValue(grabbed(),
		       CLAMP(gx, 0, (float)(func()->length()-1)),
		       CLAMP(gy, 0, 0.99999F));
	}
      else
	{
	  pl->deleteBetweenXRange(ox, graph2FuncX(x), &grabbed_); 
	  pl->setValue(grabbed(),
		       CLAMP(gx, 0, (float)(func()->length()-1)),
		       CLAMP(gy,0,0.999999F));
	}
      lastPointerX_ = x;
      lastPointerY_ = y;
    }
}

void AvFunctionEditor::XDraw(Widget w, GC gc, int border)
{
  Dimension width, height;
  XtVaGetValues(w, XmNwidth, &width, XmNheight, &height, NULL);
 
  int rwidth = width - 2*border;
  int rheight = height - 2*border;
  
  XClearArea(XtDisplay(w), XtWindow(w), border, border, rwidth, rheight, 0);
 
  int len = (int)func_->length();
  float * array = func_->array();
  XPoint * newPoints = new XPoint[rwidth];
  for (int i = 0; i < rwidth; i++) {
    newPoints[i].x = border + i;
    newPoints[i].y = border + rheight - 1 -
      (int)(rheight*.999*array[(int)(len*i/rwidth)]);
    }

  XDrawLines(XtDisplay(w), XtWindow(w), gc,
	     newPoints, rwidth, CoordModeOrigin);

  delete [] newPoints;  
}


//  Redraws the graph ndx in its own drawable
void AvFunctionEditor::redrawGraph()
{
  XClearWindow (display_, XtWindow (graph_));

  float *array = func_->array();
  XPoint * newPoints = new XPoint[func()->length()];

  for (int i = 0; i < func()->length(); i++) 
    {
      newPoints[i].x = func2GraphX(i);
      newPoints[i].y = func2GraphY(array[i]);
    }
  
  XSetForeground (display_, gc_, foregroundPixel_);
  XDrawLines (display_, XtWindow (graph_),
	      gc_, newPoints, (int)func()->length(), CoordModeOrigin);

  if (mode() != DRAW) redrawControlPoints();

  delete [] newPoints;
}

void AvFunctionEditor::redrawControlPoints()
{
  AvPointList2D *pl = func()->pts();
  XSetForeground (display_, gc_, pointColorPixel_);

  for (int i = 0; i < pl->length(); i++)
    {
      float x,y;
      pl->getValue(i, &x, &y);
      float gx = func2GraphX(x) - (CONTROLPOINTRADIUS/2);
      float gy = func2GraphY(y) - (CONTROLPOINTRADIUS/2);
      XFillArc (display_, XtWindow (graph_), gc_, 
		int(gx),
		int(gy),
		CONTROLPOINTRADIUS, 
		CONTROLPOINTRADIUS, 0, 64*360);
    }
}
