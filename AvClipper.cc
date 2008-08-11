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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvClipper.cc,v 19.0 2003/07/16 05:47:07 aips2adm Exp $
//
// $Log: AvClipper.cc,v $
// Revision 19.0  2003/07/16 05:47:07  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:50  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:19  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:26  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:55  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 18:13:12  hravlin
// Changed some char * to const char *.
//
// Revision 14.0  2000/03/23 16:08:10  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/08/25 19:58:23  hravlin
// Changed string format from 'f' to 'G' to minimize chance of buffer overflow.
//
// Revision 13.0  1999/08/10 18:39:57  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:33  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:37  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:27  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/09 20:31:05  hr
// Removed an extra semicolon.
//
// Revision 9.0  1997/08/25 21:27:01  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.1  1997/05/07 17:14:05  hr
// In setClipMin/Max(), check for divide by 0.
// in setRange() preset max_ so setClipMin() won't try divide by 0 the first
// time.
//
// Revision 8.0  1997/02/20 03:15:21  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1996/12/12 09:04:44  droberts
// Final update from monet archive.
//
// Revision 1.5  1996/10/28 20:38:17  hr
// Removed check for 'bad' values from set{Min,Max} since it prevented
// changing values under certain circumstances.
//
// Revision 1.4  1996/10/23 15:38:47  hr
// getTextFField() was converting result to an int.
//
// Revision 1.3  1996/10/23 15:27:47  hr
// Added ability to allow user to edit min/max fields from the keyboard.
//
// Revision 1.2  1996/08/14 17:25:28  hr
// Changes to remove g++ warnings (from bglenden).
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.3  1996/06/18  18:45:51  pixton
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

#include <stdio.h>
#include <stdlib.h>		// strtod
#include <iostream.h>
#include <Xm/LabelG.h>
#include <Xm/Form.h>
#include <Xm/TextF.h>

#include <AvTwoValueSlider.h>

#include <AvClipper.h>

AvClipper::AvClipper
(Widget parent, int buildInsideParent, int width, int prec,
 const Boolean editable)
: AvFormComp (parent, buildInsideParent)
{
  sliderWidth_ = width;
  setPrecision (prec);
  min_ = max_ = clipMin_ = clipMax_ = 0.0;

  WidgetClass wc;
  if(editable)
	wc = xmTextFieldWidgetClass;
  else
	wc = xmLabelGadgetClass;

  minLabel_ = XtVaCreateManagedWidget
    ("Minimum", xmLabelGadgetClass, baseWidget_,
     XmNtopAttachment, XmATTACH_FORM,
     XmNleftAttachment, XmATTACH_FORM,
     NULL);
  
  // Each textfield is 40% of the width of the bar.
  int fs = (width*4)/10;
  minNum_ = XtVaCreateManagedWidget
//    ("clip", xmLabelGadgetClass, baseWidget_,
    ("clip", wc, baseWidget_,
     XmNtopAttachment, XmATTACH_WIDGET,
     XmNtopWidget, minLabel_,
     XmNleftAttachment, XmATTACH_FORM,
     XmNwidth, fs,
     NULL);
  

  maxLabel_ = XtVaCreateManagedWidget
    ("Maximum", xmLabelGadgetClass, baseWidget_,
     XmNalignment, XmALIGNMENT_END,
     XmNtopAttachment, XmATTACH_FORM,
     XmNrightAttachment, XmATTACH_FORM,
     NULL);
  
  maxNum_ = XtVaCreateManagedWidget
    ("clip", wc, baseWidget_,
     XmNrightAttachment, XmATTACH_FORM,
     XmNtopAttachment, XmATTACH_WIDGET,
     XmNtopWidget, maxLabel_,
     XmNwidth, fs,
     NULL);

  slider_ = new AvTwoValueSlider (baseWidget_, TRUE, width);
  slider_->addCallback ((AvConductorCB) &AvClipper::updateCB,
		       AvConductor::MouseMove,
		       (XtPointer) this, NULL);
  slider_->show ();
  
  Widget sw = slider_->baseWidget_;
  XtVaSetValues (sw,
		 XmNtopAttachment, XmATTACH_WIDGET,
		 XmNtopWidget, maxNum_,
		 XmNleftAttachment, XmATTACH_FORM,
//		 XmNrightAttachment, XmATTACH_FORM,
		 NULL);
  
  clipMinNum_ = XtVaCreateManagedWidget
    ("clip", xmLabelGadgetClass, baseWidget_,
     XmNleftAttachment, XmATTACH_FORM,
     XmNtopAttachment, XmATTACH_WIDGET,
     XmNtopWidget, sw,
     NULL);

  clipMaxNum_ = XtVaCreateManagedWidget
    ("clip", xmLabelGadgetClass, baseWidget_,
     XmNrightAttachment, XmATTACH_FORM,
     XmNtopAttachment, XmATTACH_WIDGET,
     XmNtopWidget, sw,
     NULL);

  clipMaxLabel_ = XtVaCreateManagedWidget
    ("Clip Maximum", xmLabelGadgetClass, baseWidget_,
     XmNrightAttachment, XmATTACH_FORM,
     XmNtopAttachment, XmATTACH_WIDGET,
     XmNtopWidget, clipMaxNum_,
     NULL);

  clipMinLabel_ = XtVaCreateManagedWidget
    ("Clip Minimum", xmLabelGadgetClass, baseWidget_,
     XmNleftAttachment, XmATTACH_FORM,
     XmNtopAttachment, XmATTACH_WIDGET,
     XmNtopWidget, clipMinNum_,
     NULL);

  if(editable) {
    XtAddEventHandler(minNum_, KeyReleaseMask, FALSE,
		      (XtEventHandler) AvClipper::minNumChangedCB, this);
    XtAddEventHandler(maxNum_, KeyReleaseMask, FALSE,
		      (XtEventHandler) AvClipper::maxNumChangedCB, this);
  }
}


void AvClipper::setPrecision (int prec)
{
  precision_ = prec;
//  sprintf (format_, "%s%d%s", "%.", precision_, "f");
  sprintf(format_, "%%.%dG", precision_);
}


void AvClipper::setLock (int trueOrFalse)
{
    slider_->setLock (trueOrFalse);
}


void AvClipper::getValues (float & a, float & b, int & change)
{
  // let the slider fill in the change parameter
  slider_->getValues (a, b, change);
  // set a and b from the clipper's variables
  a = clipMin_;
  b = clipMax_;
}


void AvClipper::updateCB (AvObject * obj, AvConductor *, void *)
{
  AvClipper * clip = (AvClipper *) obj;
  float a, b;
  int mostRecentChange;
  clip->slider_->getValues (a, b, mostRecentChange);
  float range = clip->max_ - clip->min_;
  if (mostRecentChange == AvTwoValueSlider::Left) {
    clip->clipMin_ = clip->min_ + a * range;
    clip->setMinMax (clip->clipMinNum_, clip->clipMin_);
  }
  else if (mostRecentChange == AvTwoValueSlider::Right) {
    clip->clipMax_ = clip->min_ + b * range;
    clip->setMinMax (clip->clipMaxNum_, clip->clipMax_);
  }
  else {
    // must be locked
    clip->clipMin_ = clip->min_ + a * range;
    clip->clipMax_ = clip->min_ + b * range;
    clip->setMinMax (clip->clipMinNum_, clip->clipMin_);
    clip->setMinMax (clip->clipMaxNum_, clip->clipMax_);
  }
}

// Convert a float value to a string and write it into the widget.
void AvClipper::setMinMax (Widget w, float v)
{
  char str[32];
  sprintf (str, format_, v);
  if(XmIsTextField(w))
	XmTextFieldSetString (w, str);
  else
	setLabelString (w, str);
}


/*
void AvClipper::setMinMaxText (Widget w, float v)
{
  char str[32];
  sprintf (str, format, v);
  XmString xStr = XmStringCreateLocalized (str);
  XtVaSetValues (w, XmNvalue, str, NULL);
  XmStringFree (xStr);
}
*/


void AvClipper::showLabels (int trueOrFalse)
{
  if (trueOrFalse) {
    XtManageChild (minLabel_);
    XtManageChild (maxLabel_);
  }
  else {
    XtUnmanageChild (minLabel_);
    XtUnmanageChild (maxLabel_);
  }
}


void AvClipper::showClipLabels (int trueOrFalse)
{
  if (trueOrFalse) {
    XtManageChild (clipMinLabel_);
    XtManageChild (clipMaxLabel_);
  }
  else {
    XtUnmanageChild (clipMinLabel_);
    XtUnmanageChild (clipMaxLabel_);
  }
}


//----------------------------------
void AvClipper::setLabels (const char * newMinL, const char * newMaxL)
{
  setLabelString (minLabel_, newMinL);
  setLabelString (maxLabel_, newMaxL);
}


void AvClipper::setClipLabels (const char * newMinL, const char * newMaxL)
{
  setLabelString (clipMinLabel_, newMinL);
  setLabelString (clipMaxLabel_, newMaxL);
}


// how many pixels should the slider move when an arrow button is binked?
void AvClipper::setArrowJumps ()
{
  if (precision_ == 0)
    slider_->setMinMaxVals (int(min_), int(max_));
  else
    slider_->setMinMaxVals (0, 0);
}     


// Set the min value for the clipper.
void AvClipper::setMin (float m)
{
  min_ = m;
  setMinMax (minNum_, m);
  if(m > clipMin_)
  {	setClipRange(m, clipMax_);
	slider_->invokeCallbacks(AvConductor::MouseUp);
  }
  else
	setClipRange(clipMin_, clipMax_);		// Set slider bar.

  setArrowJumps ();
}


void AvClipper::setMax (float m)
{
  max_ = m;
  setMinMax (maxNum_, m);
  setArrowJumps ();
  if(m < clipMax_)
  {	setClipRange(clipMin_, m);
	slider_->invokeCallbacks(AvConductor::MouseUp);
  }
  else
	setClipRange(clipMin_, clipMax_);		// Set slider bar.
}


// Change the min/max range of the clipper.
void AvClipper::setRange (float m1, float m2)
{
  if((m1 != min_) || (m2 != max_))
  {	slider_->disableNotify();
	  max_ = m2;		// For setClipMin() called via setMin().
	  setMin (m1);
	  setMax (m2);
	slider_->enableNotify();
  }
}


void AvClipper::setClipMin (float m)
{
  clipMin_ = m;
  setMinMax (clipMinNum_, m);
  if(max_ != min_)
     slider_->setLeftPercentage ((m - min_) / (max_ - min_));
}


void AvClipper::setClipMax (float m)
{
  clipMax_ = m;
  setMinMax (clipMaxNum_, m);
  if(max_ != min_)
     slider_->setRightPercentage ((m - min_) / (max_ - min_));
}


void AvClipper::setClipRange (float m1, float m2)
{
  setClipMin (m1);
  setClipMax (m2);
}


AvClipper::~AvClipper ()
{
  delete slider_;
  XtDestroyWidget (baseWidget_);
}

// When the user types in a new min/max value followed by a return,
// set the new value.
void AvClipper::minNumChangedCB (Widget, AvClipper * p, XKeyEvent * e)
{
  if (isReturnKey (e)) {
    double newVal;
    if(getTextFField (p->minNum_, newVal))
	p->setMin(newVal);
    else
	p->setMinMax(p->minNum_, p->min_);
  }
}

void AvClipper::maxNumChangedCB (Widget, AvClipper * p, XKeyEvent * e)
{
  if (isReturnKey (e)) {
    double newVal;
    if(getTextFField (p->maxNum_, newVal))
	p->setMax(newVal);
    else
	p->setMinMax(p->maxNum_, p->max_);
  }
}

// Convert a text field value to a double. Returns 1 for a valid number
// otherwise 0. (The check is rather simple and is not guaranteed).
int AvClipper::getTextFField(Widget w, double &value)
{
  char * val = XmTextFieldGetString (w);
  char *endptr;
  value  = strtod(val, &endptr);
  XtFree (val);
  return (val == endptr) ? 0 : 1;
}
