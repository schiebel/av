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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvTwoValueSlider.cc,v 19.0 2003/07/16 05:46:41 aips2adm Exp $
//
// $Log: AvTwoValueSlider.cc,v $
// Revision 19.0  2003/07/16 05:46:41  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:27  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.2  2002/01/22 22:22:08  hravlin
// PtrToInt() is now in Av.h.
//
// Revision 17.1  2002/01/07 22:40:29  hravlin
// Removed check for __P.
//
// Revision 17.0  2001/11/12 19:41:55  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:05  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:25  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:28:11  hravlin
// Changed from using XmStringCreateLocalized() to AvXmStringCreateLocalized().
//
// Revision 14.0  2000/03/23 16:07:41  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/08/25 19:37:15  hravlin
// Fixed problem with casting from XtPointer to enum.
//
// Revision 13.0  1999/08/10 18:39:25  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:36  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:17  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:51:53  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:24:32  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:14:59  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/02/05 18:00:54  hr
// The __P problem.
//
// Revision 7.2  1996/12/12 07:33:56  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.3  1996/06/18  18:46:38  pixton
//  Copyright Update
//
//  Revision 1.2  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#include <iostream.h>
#include <string.h>
#include <math.h>
#include <Xm/Form.h>
#include <Xm/BulletinB.h>
#include <Xm/ArrowB.h>
#include <Xm/PushB.h>

#include "AvTwoValueSlider.h"
#include "AvWidgetMisc.h"
#include "Av.h"

int AvTwoValueSlider::controlWidth = 12;
int AvTwoValueSlider::controlHalfWidth = 6;
int AvTwoValueSlider::controlHeight = 16;

AvTwoValueSlider::AvTwoValueSlider 
(Widget parent, int buildInsideParent, int chWidth) 
: AvFormComp (parent, buildInsideParent)
{
  locked = FALSE;
  separation = 0;
  minVal = maxVal = 0;
  ppuVal = 0.0;

  Widget overall = XtVaCreateManagedWidget
    ("", xmFormWidgetClass, baseWidget(),
     XmNheight, controlHeight+4,
     XmNwidth, chWidth,
     XmNshadowType, XmSHADOW_IN,
     XmNshadowThickness, 2,
     XmNmarginHeight, 2, 
     XmNmarginWidth, 2,
     NULL);

  Widget leftArrow = XtVaCreateManagedWidget
    ("ArrowButton", xmArrowButtonWidgetClass, overall,
     XmNarrowDirection, XmARROW_LEFT,
     XmNleftAttachment, XmATTACH_FORM,
     XmNy, 2,
     XmNshadowThickness, 0,
     NULL);

  Dimension arrowWidth;
  XtVaGetValues (leftArrow, XmNwidth, &arrowWidth, NULL);

  channelWidth = chWidth - 6 - arrowWidth * 2;

  activeWidth = channelWidth - controlWidth;

  leftPosition = 0;
  rightPosition = activeWidth;

  channel = XtVaCreateManagedWidget
    ("", xmBulletinBoardWidgetClass, overall,
     XmNwidth, channelWidth,
     XmNleftAttachment, XmATTACH_WIDGET,
     XmNleftWidget, leftArrow,
     XmNheight, controlHeight,
     // move it off overall's top shadow
     XmNy, 2,
     XmNmarginHeight, 0, 
     XmNmarginWidth, 0,
     XmNresizePolicy, XmRESIZE_NONE,
     NULL);

  Widget rightArrow = XtVaCreateManagedWidget
    ("ArrowButton", xmArrowButtonWidgetClass, overall,
     XmNarrowDirection, XmARROW_RIGHT,
     XmNleftAttachment, XmATTACH_WIDGET,
     XmNleftWidget, channel,
     XmNy, 2,
     XmNshadowThickness, 0,
     NULL);

  XtAddCallback (leftArrow, XmNactivateCallback,
		 (XtCallbackProc) &AvTwoValueSlider::leftArrowCB,
		 (XtPointer) this);

  XtAddCallback (rightArrow, XmNactivateCallback,
		 (XtCallbackProc) &AvTwoValueSlider::rightArrowCB,
		 (XtPointer) this);

  XmString xStr = AvXmStringCreateLocalized ("");
  leftControl = XtVaCreateManagedWidget
    ("", xmPushButtonWidgetClass, channel,
     XmNuserData, Left,
     XmNlabelString, xStr,
     XmNwidth, controlWidth,
     XmNheight, controlHeight,
     XmNshadowThickness, 2,
     XmNx, leftPosition,
     NULL);

  rightControl = XtVaCreateManagedWidget
    ("", xmPushButtonWidgetClass, channel,
     XmNrightAttachment, XmATTACH_FORM,
     XmNuserData, Right,
     XmNlabelString, xStr,
     XmNwidth, controlWidth,
     XmNheight, controlHeight,
     XmNshadowThickness, 2,
     XmNx, rightPosition,
     NULL);

  XmStringFree (xStr);

  XtAddEventHandler
    (leftControl, ButtonPressMask, FALSE,
     (XtEventHandler) &AvTwoValueSlider::buttonDown,
     (XtPointer) this);

  XtAddEventHandler
    (leftControl, Button1MotionMask | PointerMotionHintMask, FALSE,
     (XtEventHandler) &AvTwoValueSlider::buttonMove,
     (XtPointer) this);

  XtAddEventHandler
    (leftControl, ButtonReleaseMask, FALSE,
     (XtEventHandler) &AvTwoValueSlider::buttonUp,
     (XtPointer) this);

  XtAddEventHandler
    (rightControl, ButtonPressMask, FALSE,
     (XtEventHandler) &AvTwoValueSlider::buttonDown,
     (XtPointer) this);

  XtAddEventHandler
    (rightControl, Button1MotionMask | PointerMotionHintMask, FALSE,
     (XtEventHandler) &AvTwoValueSlider::buttonMove,
     (XtPointer) this);

  XtAddEventHandler
    (rightControl, ButtonReleaseMask, FALSE,
     (XtEventHandler) &AvTwoValueSlider::buttonUp,
     (XtPointer) this);
}


void AvTwoValueSlider::setMinMaxVals (int min, int max)
{
  // we only want to use these if client has set min and max
  // to different values
  minVal = min;
  maxVal = max;
  if (minVal == maxVal)
    ppuVal = 0.0;
  else
    ppuVal = activeWidth / (float) (max - min + 1);
}


void AvTwoValueSlider::leftArrowCB (Widget, AvTwoValueSlider * s, XtPointer)
{
  if (s->ppuVal > 0.0) {
    float p = s->leftPosition / s->ppuVal;
    int cPos = (int) (floor ((double) p));
    s->setLeft ((int) (cPos * s->ppuVal));
  }
  else
    s->setLeft (s->leftPosition - 1);

  s->invokeCallbacks (AvConductor::MouseMove);
  s->invokeCallbacks (AvConductor::MouseUp);
}


void AvTwoValueSlider::rightArrowCB (Widget, AvTwoValueSlider * s, XtPointer)
{
  if (s->ppuVal > 0.0) {
    float p = (s->rightPosition - controlWidth) / s->ppuVal;
    int cPos = (int) (ceil ((double) p) + 1);
    s->setRight ((int) (controlWidth + cPos * s->ppuVal));
  }
  else
    s->setRight (s->rightPosition + 1);

  s->invokeCallbacks (AvConductor::MouseMove);
  s->invokeCallbacks (AvConductor::MouseUp);
}

void AvTwoValueSlider::getValues (float & min, float & max, int & change)
{
  change = Av::PtrToInt(ctrl);
  min = leftPosition / ((float) activeWidth - controlWidth);
  max = (rightPosition - controlWidth) / ((float) activeWidth - controlWidth);
}


void AvTwoValueSlider::setLock (int trueOrFalse)
{
  if (locked != trueOrFalse)
    locked = trueOrFalse;
  if (locked) {
    ctrl = (XtPointer) Both;
    separation = rightPosition - leftPosition;
  }
  else
    separation = 0;
}


void AvTwoValueSlider::buttonDown (Widget w, AvTwoValueSlider * s, XEvent *)
{
  // if not locked, then record whether left or the right control is active
  if (!s->locked)
    XtVaGetValues (w, XmNuserData, &s->ctrl, NULL);
}


void AvTwoValueSlider::buttonUp (Widget, AvTwoValueSlider * s, XEvent *)
{
  s->invokeCallbacks (AvConductor::MouseUp);
}


void AvTwoValueSlider::setLeftPercentage (float newVal)
{
  float newPos = newVal * (activeWidth);
//  cerr << "setL: " << newVal << '\t' << newPos << endl;
  setLeft ((int) newPos);
}


void AvTwoValueSlider::setRightPercentage (float newVal)
{
  float newPos = newVal * (activeWidth);
//  cerr << "setR: " << newVal << '\t' << newPos << endl;
  setRight ((int) newPos);
}


void AvTwoValueSlider::setLeft (int newPos)
{
  // if the position is outside the range of the slider, reset
  if (newPos < 0) newPos = 0;
  if (newPos > activeWidth) newPos = activeWidth;
  
  // if no change, get out
  if (newPos == leftPosition) return;
  
  // don't run over the right control or push it off the right edge
  if (!locked) {
    leftPosition = MIN(newPos, rightPosition - controlWidth);
    XtVaSetValues (leftControl, XmNx, leftPosition, NULL);
  }
  else {
    leftPosition = MIN(newPos, activeWidth - separation);
    rightPosition = leftPosition + separation;
    XtVaSetValues (leftControl, XmNx, leftPosition, NULL);
    XtVaSetValues (rightControl, XmNx, rightPosition, NULL);
  }
}


void AvTwoValueSlider::setRight (int newPos)
{
  // if the position is outside the range of the slider, reset
  if (newPos < 0) newPos = 0;
  if (newPos > activeWidth) newPos = activeWidth;
  
  // if no change, get out
  if (newPos == rightPosition) return;
  
  // don't run over the left control or push it off the left edge
  if (!locked) {
    rightPosition = MAX(newPos, leftPosition + controlWidth);
    XtVaSetValues (rightControl, XmNx, rightPosition, NULL);
  }
  else {
    rightPosition = MAX(newPos, separation);
    leftPosition = rightPosition - separation;
    XtVaSetValues (leftControl, XmNx, leftPosition, NULL);
    XtVaSetValues (rightControl, XmNx, rightPosition, NULL);
  }
}


void AvTwoValueSlider::buttonMove (Widget w, AvTwoValueSlider * s, XEvent *)
{
  Window root, win;
  int rootX, rootY, winX, winY;
  unsigned int mask;

  XQueryPointer (XtDisplay (w), XtWindow (s->channel), &root, &win,
		 &rootX, &rootY, &winX, &winY, &mask);

  Position newPos = winX - controlHalfWidth;

  if (w == s->leftControl) 
    s->setLeft (newPos);
  else
    s->setRight (newPos);

  s->invokeCallbacks (AvConductor::MouseMove);
}

