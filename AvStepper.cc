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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvStepper.cc,v 19.0 2003/07/16 05:47:26 aips2adm Exp $
//
// $Log: AvStepper.cc,v $
// Revision 19.0  2003/07/16 05:47:26  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:06  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:35  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:42  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:03  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:32:37  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:08:31  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/08/25 19:42:30  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:40:18  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:13  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:33  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:31  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:28:45  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.1  1997/02/25 17:13:39  hr
// Made sure initial value gets displayed.
//
// Revision 8.0  1997/02/20 03:17:33  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/02/06 18:01:01  hr
// Changes to support 1 relative readouts.
//
// Revision 7.2  1996/12/12 07:11:17  droberts
// Final update from monet archive.
//
// Revision 1.2  1996/08/14 19:09:58  hr
// Changes to remove g++ warnings (from bglenden).
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.10  1996/06/18  18:46:34  pixton
//  Copyright Update
//
//  Revision 1.9  1996/03/29  18:46:10  hr
//  Removed an unused variable from setValue().
//
//  Revision 1.8  1996/03/29  15:08:51  baker
//  Modified editable stepper functionality.
//
//  Revision 1.7  1996/03/27  22:33:54  hr
//  AvStepper now supports user editable min and max.
//
//  Revision 1.6  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.6  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/08/07  15:51:26  baker
//  Added routines for retrieving the min and max values of the stepper.
//
//  Revision 1.4  1995/05/16  16:59:48  hr
//  About 1 1/2 lines got truncated on last ci.
//
//  Revision 1.3  1995/05/16  16:49:48  hr
//  Put back code that was in alpha1 so typeValueCB has a chance of
//  recognizing <return> on systems other than SGIs.
//
//  Revision 1.2  1995/04/12  18:13:23  baker
//  Rearranged layout constraints.  Each part is attached to its neighborleft.
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <Xm/ArrowB.h>
#include <Xm/Form.h>
#include <Xm/LabelG.h>
#include <Xm/TextF.h>

#include "AvStepper.h"


AvStepper::AvStepper
(Widget parent, int buildInsideParent, int minimum, int maximum, int edit) :
AvFormComp (parent, buildInsideParent)
{
  fullMin_ = minimum;  min_ = fullMin_;
  fullMax_ = maximum;  max_ = fullMax_;

  // Figure out what the longest 'value' string will be .
  char str[32];
  sprintf (str, "%d", minimum);
  int minLabelLength = (int)strlen (str);
  sprintf (str, "%d", maximum);
  int maxLabelLength = (int)strlen (str);
  int largerLabel = MAX (minLabelLength, maxLabelLength);

  value_ = minimum;
  userEdit_ = edit;
  displayOffset_ = 0;

  char emptystr[] = "";
  label_ = XmCreateLabelGadget (baseWidget_, emptystr, NULL, 0);
  XtVaSetValues (label_,
		 XmNalignment, XmALIGNMENT_END,
		 XmNbottomAttachment, XmATTACH_FORM,
		 NULL);

  if (!userEdit_) {
    leftLabel_ = XtVaCreateManagedWidget
      ("FontPlain", xmLabelGadgetClass, baseWidget_,
       XmNleftAttachment, XmATTACH_WIDGET,
       XmNleftWidget, label_,
       XmNbottomAttachment, XmATTACH_FORM,
       NULL);
    setLabelString (leftLabel_, min_);
  }
  else {
    leftLabel_ = XtVaCreateManagedWidget
      ("FontPlain", xmTextFieldWidgetClass, baseWidget_,
       XmNcolumns, largerLabel,
       XmNleftAttachment, XmATTACH_WIDGET,
       XmNleftWidget, label_,
       XmNbottomAttachment, XmATTACH_FORM,
       XmNbottomOffset, 2,
       XmNmarginHeight, 0,
       XmNshadowThickness, 0,
       NULL);
  }

  leftArrow_ = XtVaCreateManagedWidget
    ("ArrowButton", xmArrowButtonWidgetClass, baseWidget(),
     XmNarrowDirection, XmARROW_LEFT,
     XmNshadowThickness, 0,
     XmNleftAttachment, XmATTACH_WIDGET,
     XmNleftWidget, leftLabel_,
     XmNbottomAttachment, XmATTACH_FORM,
     XmNbottomOffset, 3,
     NULL);

  valueField_ = XtVaCreateManagedWidget
    ("FontPlain", xmTextFieldWidgetClass, baseWidget_,
     XmNcolumns, largerLabel,
     XmNleftAttachment, XmATTACH_WIDGET,
     XmNleftWidget, leftArrow_,
     XmNbottomAttachment, XmATTACH_FORM,
     XmNbottomOffset, 2,
     XmNmarginHeight, 0,
     XmNshadowThickness, 0,
     XmNvalue,	"0",		// Default is "".
   NULL);

  rightArrow_ = XtVaCreateManagedWidget
    ("ArrowButton", xmArrowButtonWidgetClass, baseWidget(),
     XmNarrowDirection, XmARROW_RIGHT,
     XmNshadowThickness, 0,
     XmNleftAttachment, XmATTACH_WIDGET,
     XmNleftWidget, valueField_,
     XmNbottomAttachment, XmATTACH_FORM,
     XmNbottomOffset, 3,
     NULL);

  if (!userEdit_) {
    rightLabel_ = XtVaCreateManagedWidget
      ("FontPlain", xmLabelGadgetClass, baseWidget_,
       XmNleftAttachment, XmATTACH_WIDGET,
       XmNleftWidget, rightArrow_,
       XmNbottomAttachment, XmATTACH_FORM,
       NULL);
    setLabelString (rightLabel_, max_);
  }
  else {
    rightLabel_ = XtVaCreateManagedWidget
      ("FontPlain", xmTextFieldWidgetClass, baseWidget_,
       XmNcolumns, largerLabel,
       XmNleftAttachment, XmATTACH_WIDGET,
       XmNleftWidget, rightArrow_,
       XmNbottomAttachment, XmATTACH_FORM,
       XmNbottomOffset, 2,
       XmNmarginHeight, 0,
       XmNshadowThickness, 0,
       NULL);
  }

  setMinMax (min_, max_);
  setValue (min_);

  XtAddCallback (leftArrow_, XmNactivateCallback,
                 (XtCallbackProc) AvStepper::leftArrowChangedCB, this);
  
  XtAddCallback (rightArrow_, XmNactivateCallback,
                 (XtCallbackProc) AvStepper::rightArrowChangedCB, this);

  if (userEdit_) {
    XtAddEventHandler(leftLabel_, KeyReleaseMask, FALSE,
		      (XtEventHandler) AvStepper::leftLabelChangedCB, this);
    XtAddEventHandler(rightLabel_, KeyReleaseMask, FALSE,
		      (XtEventHandler) AvStepper::rightLabelChangedCB, this);
  }
  
XtAddEventHandler (valueField_, KeyReleaseMask, FALSE,
		     (XtEventHandler) &AvStepper::typeValueCB,
		     (XtPointer) this);

  XtManageChild (label_);
  XtManageChild (valueField_);
}


void AvStepper::setLabel (char * newLabel)
{
  setLabelString (label_, newLabel);
}


void AvStepper::setLabelWidth (int sz)
{
  XtVaSetValues (label_, XmNwidth, sz, XmNrecomputeSize, FALSE, NULL);
}


void AvStepper::setValueFieldWidth (int sz)
{
  XtVaSetValues (valueField_, XmNwidth, sz, NULL);
}


static void setValueString(Widget w, const int value)
{
    char str[16];
    sprintf (str, "%d", value);
    XmTextFieldSetString (w, str);
}

void AvStepper::setValue (int v)
{

  v = v < min_ ? min_ : v;
  v = v > max_ ? max_ : v;

  if (value_ != v) {
    value_ = v;
    invokeCallbacks (ValueChanged);
  }

  if (value_ != getTextField (valueField_) - displayOffset_) {
#if 0
    char str[8];
    sprintf (str, "%d", value_ + displayOffset_);
    XmTextFieldSetString (valueField_, str);
#else
    setValueString(valueField_, value_ + displayOffset_);
#endif
  }
}

void AvStepper::setDisplayOffset(const int o)
{
	if(o != displayOffset_)
	{	displayOffset_ = o;
		setMinMax(min_, max_);
		setValueString(valueField_, value_ + displayOffset_);
	}
}

void AvStepper::setMinMax (int newMin, int newMax)
{
  if (newMin > newMax) return;

  newMin = newMin < fullMin_ ? fullMin_ : newMin;
  newMin = newMin > fullMax_ ? fullMax_ : newMin;
  newMax = newMax > fullMax_ ? fullMax_ : newMax;
  newMax = newMax < fullMin_ ? fullMin_ : newMax;

  min_ = newMin;
  max_ = newMax;

  char str[8];

  if (!userEdit_) {
    setLabelString (leftLabel_, min_ + displayOffset_);
    setLabelString (rightLabel_, max_ + displayOffset_);
  }
  else {
    sprintf (str, "%d", min_ + displayOffset_);
    XmTextFieldSetString (leftLabel_, str);
    sprintf (str, "%d", max_ + displayOffset_);
    XmTextFieldSetString (rightLabel_, str);
  }

  // If the current 'value' is now out of range, then reset it
  // to lie in the middle of the new values.
  if (value_ < min_ || value_ > max_) {
    value_ = int(min_ + 0.5 * (max_ - min_));
    sprintf (str, "%d", value_ + displayOffset_);
    XmTextFieldSetString (valueField_, str);
  }
}


void AvStepper::setMin (int newVal, int & val, Widget w)
{
  if (!userEdit_) return;

  newVal = newVal < fullMin_ ? fullMin_ : newVal;
  newVal = newVal > max_ ? max_ : newVal;

  if (newVal != val) {
    val = newVal;
    char str[8];
    sprintf (str, "%d", newVal + displayOffset_);
    XmTextFieldSetString (w, str);
    invokeCallbacks (RangeChanged);
  }
}


void AvStepper::setMax (int newVal, int & val, Widget w)
{
  if (!userEdit_) return;

  newVal = newVal < min_ ? min_ : newVal;
  newVal = newVal > fullMax_ ? fullMax_ : newVal;

  if (newVal != val) {
    val = newVal;
    char str[8];
    sprintf (str, "%d", newVal + displayOffset_);
    XmTextFieldSetString (w, str);
    invokeCallbacks (RangeChanged);
  }
}


void AvStepper::leftArrowChangedCB (Widget, AvStepper * obj, XtPointer)
{
  obj->setValue (obj->getValue () - 1);
}


void AvStepper::rightArrowChangedCB (Widget, AvStepper * obj, XtPointer)
{
  obj->setValue (obj->getValue () + 1);
}


void AvStepper::typeValueCB (Widget, AvStepper * p, XKeyEvent * e)
{
  if (isReturnKey (e)) 
    p->setValue (getTextField (p->valueField_) - p->displayOffset_);
}


void AvStepper::leftLabelChangedCB (Widget, AvStepper * p, XKeyEvent * e)
{
  if (isReturnKey (e)) {
    int newVal = getTextField (p->leftLabel_) - p->displayOffset_;
    p->setMin (newVal, p->min_, p->leftLabel_);
    // If the current value is now out of range, set it.
    if (p->value_ < newVal)
      p->setValue (newVal);
  }
}


void AvStepper::rightLabelChangedCB (Widget, AvStepper * p, XKeyEvent * e)
{
  if (isReturnKey (e)) {
    int newVal = getTextField (p->rightLabel_) - p->displayOffset_;
    p->setMax (newVal, p->max_, p->rightLabel_);
    // If the current value is now out of range, set it.
    if (p->value_ > newVal)
      p->setValue (newVal);
  }
}


AvStepper::~AvStepper ()
{
  XtDestroyWidget (baseWidget_);
}
