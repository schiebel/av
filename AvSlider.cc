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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvSlider.cc,v 19.0 2003/07/16 05:47:25 aips2adm Exp $
//
// $Log: AvSlider.cc,v $
// Revision 19.0  2003/07/16 05:47:25  aips2adm
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
// Revision 15.0  2000/10/26 17:11:02  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:33:07  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:08:30  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:18  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:11  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:31  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:29  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:28:43  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:17:32  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 07:09:21  droberts
// Final update from monet archive.
//
// Revision 1.2  1996/08/14 19:09:14  hr
// Changes to remove g++ warnings (from bglenden).
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.9  1996/06/18  18:46:33  pixton
//  Copyright Update
//
//  Revision 1.8  1996/03/29  16:23:32  baker
//  Small change to layout.
//
//  Revision 1.7  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.7  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.6  1995/08/24  21:17:15  baker
//  Modified layout.
//
//  Revision 1.5  1995/08/08  16:43:02  baker
//  *** empty log message ***
//
//  Revision 1.4  1995/08/08  16:36:55  baker
//  Added destructor.
//
//  Revision 1.3  1995/07/17  21:32:53  hr
//  Added destructor.
//
//  Revision 1.2  1995/06/24  00:36:48  baker
//  Rearranged parts to support display on 1 line, 2 lines, or 3 lines.
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/LabelG.h>
#include <Xm/Scale.h>
#include <Xm/TextF.h>

#include "AvSlider.h"


AvSlider::AvSlider
(Widget parent, int buildInsideParent, int nLines, int prec) :
AvFormComp (parent, buildInsideParent)
{
  min = 1.0;
  max = 100.0;
  value = 50.0;
  precision = prec;
  sprintf (format, "%s%d%s", "%.", precision, "f");
  
  char emptystr[] = "";
  label = XmCreateLabelGadget(baseWidget_, emptystr, NULL, 0);
  XtVaSetValues(label,
		XmNalignment, XmALIGNMENT_END,
		NULL);

  minLabel = XmCreateLabelGadget(baseWidget_, emptystr, NULL, 0);
  XtVaSetValues(minLabel,
		XmNmarginHeight, 0,
		XmNalignment, XmALIGNMENT_END,
		NULL);

  valueField = XmCreateTextField(baseWidget_, emptystr, NULL, 0);
  XtVaSetValues(valueField,
		XmNcolumns, 10,
		XmNmarginHeight, 0,
		NULL);

  maxLabel = XmCreateLabelGadget(baseWidget_, emptystr, NULL, 0);
  XtVaSetValues(maxLabel,
		XmNmarginHeight, 0,
		XmNalignment, XmALIGNMENT_BEGINNING,
		NULL);

  slider = XmCreateScale(baseWidget_, emptystr, NULL, 0);
  XtVaSetValues(slider,
		XmNminimum, 0,
		XmNmaximum, 100,
		XmNvalue, 50,
		XmNorientation, XmHORIZONTAL,
		NULL);

  switch (nLines) {
  case 1:
    XtVaSetValues(label,
		  XmNtopAttachment, XmATTACH_FORM,
		  XmNleftAttachment, XmATTACH_FORM,
		  NULL);

    XtVaSetValues(valueField,
		  XmNrightAttachment, XmATTACH_FORM,
		  NULL);

    XtVaSetValues(minLabel,
		  XmNleftAttachment, XmATTACH_WIDGET,
		  XmNleftWidget, label,
		  XmNbottomAttachment, XmATTACH_FORM,
		  NULL);

    XtVaSetValues(maxLabel,
		  XmNrightAttachment, XmATTACH_WIDGET,
		  XmNrightWidget, valueField,
		  XmNbottomAttachment, XmATTACH_FORM,
		  NULL);

    XtVaSetValues(slider,
		  XmNleftAttachment, XmATTACH_WIDGET,
		  XmNleftWidget, minLabel,
		  XmNrightAttachment, XmATTACH_WIDGET,
		  XmNrightWidget, maxLabel,
		  XmNbottomAttachment, XmATTACH_FORM,
		  NULL);
    break;
  case 2:
    XtVaSetValues(label,
		  XmNalignment, XmALIGNMENT_BEGINNING,
		  XmNtopAttachment, XmATTACH_FORM,
		  XmNleftAttachment, XmATTACH_FORM,
		  NULL);

    XtVaSetValues(valueField,
		  XmNtopAttachment, XmATTACH_FORM,
		  XmNleftAttachment, XmATTACH_WIDGET,
		  XmNleftWidget, label,
		  XmNrightAttachment, XmATTACH_FORM,
		  NULL);

    XtVaSetValues(minLabel,
		  XmNleftAttachment, XmATTACH_FORM,
		  XmNbottomAttachment, XmATTACH_FORM,
		  NULL);

    XtVaSetValues(maxLabel,
		  XmNrightAttachment, XmATTACH_FORM,
		  XmNbottomAttachment, XmATTACH_FORM,
		  NULL);

    XtVaSetValues(slider,
		  XmNtopAttachment, XmATTACH_WIDGET,
		  XmNtopWidget, label,
		  XmNtopOffset, 3,
		  XmNleftAttachment, XmATTACH_WIDGET,
		  XmNleftWidget, minLabel,
		  XmNrightAttachment, XmATTACH_WIDGET,
		  XmNrightWidget, maxLabel,
		  XmNbottomAttachment, XmATTACH_FORM,
		  NULL);
    break;
  case 3:
    XtVaSetValues(label,
		  XmNalignment, XmALIGNMENT_BEGINNING,
		  XmNtopAttachment, XmATTACH_FORM,
		  XmNleftAttachment, XmATTACH_FORM,
		  NULL);

    XtVaSetValues(valueField,
		  XmNtopAttachment, XmATTACH_FORM,	
		  XmNleftAttachment, XmATTACH_WIDGET,
		  XmNleftWidget, label,
		  XmNrightAttachment, XmATTACH_FORM,
		  NULL);

    XtVaSetValues(slider,
		  XmNtopAttachment, XmATTACH_WIDGET,
		  XmNtopWidget, label,
		  XmNtopOffset, 3,
		  XmNleftAttachment, XmATTACH_FORM,
		  XmNrightAttachment, XmATTACH_FORM,
		  NULL);

    XtVaSetValues(minLabel,
		  XmNtopAttachment, XmATTACH_WIDGET,
		  XmNtopWidget, slider,
		  XmNleftAttachment, XmATTACH_FORM,
		  XmNbottomAttachment, XmATTACH_FORM,
		  NULL);

    XtVaSetValues(maxLabel,
		  XmNtopAttachment, XmATTACH_WIDGET,
		  XmNtopWidget, slider,
		  XmNrightAttachment, XmATTACH_FORM,
		  XmNbottomAttachment, XmATTACH_FORM,
		  NULL);
    break;
  }

  XtAddCallback (slider, XmNvalueChangedCallback,
		 (XtCallbackProc) &AvSlider::sliderChangedCB, this);

  XtAddCallback (valueField, XmNactivateCallback,
		 (XtCallbackProc) &AvSlider::valueFieldChangedCB, this);

  XtAddCallback (slider, XmNdragCallback,
		 (XtCallbackProc) &AvSlider::sliderDragCB, this);

  XtManageChild(label);
  XtManageChild(minLabel);
  XtManageChild(slider);
  XtManageChild(maxLabel);
  XtManageChild(valueField);
}


void AvSlider::sliderChangedCB (Widget, AvSlider * s, XtPointer)
{
  int val;
  XmScaleGetValue (s->slider, &val);
  float vv = s->min + (val / 100.0) * (s->max - s->min);
  s->setValue (vv);
  s->invokeCallbacks (AvConductor::MouseMove);
}


void AvSlider::valueFieldChangedCB (Widget, AvSlider * obj, XtPointer)
{
  obj->setValue (atof (XmTextFieldGetString (obj->valueField)));
  obj->invokeCallbacks (AvConductor::ValueChanged);
}


void AvSlider::sliderDragCB (Widget, AvSlider * s, XtPointer)
{
  int val;
  XmScaleGetValue (s->slider, &val);
  float vv = s->min + (val / 100.0) * (s->max - s->min);
  s->setValue (vv);
  s->invokeCallbacks (AvConductor::MouseMove);
}


void AvSlider::setLabel (char * newLabel)
{
  setLabelString (label, newLabel);
}


void AvSlider::setMin (float newMin)
{
  min = newMin;
  char newLabel[128];
  sprintf (newLabel, format, newMin);
  setLabelString (minLabel, newLabel);
}


void AvSlider::setMax (float newMax)
{
  max = newMax;
  char newLabel[128];
  sprintf (newLabel, format, newMax);
  setLabelString (maxLabel, newLabel);
}


void AvSlider::setRange (float minn, float maxx)
{
  setMin (minn);
  setMax (maxx);
}


void AvSlider::setValue (float newVal)
{
  if (value != newVal) {
    newVal = newVal > max ? max : newVal;
    newVal = newVal < min ? min : newVal;
    value = newVal;

    // Set the text field.
    char str[128];
    sprintf (str, format, newVal);
    XmTextFieldSetString (valueField, str);

    // Convert to slider units and set the slider.
    int s = int(100 * (newVal - min) / (max - min));
    XmScaleSetValue (slider, s);
  }
}


void AvSlider::setPrecision (int decDigits)
{
  precision = decDigits;
  sprintf (format, "%s%d%s", "%.", precision, "f");
}


float AvSlider::getValue ()
{
  return (value);
}


void AvSlider::setLabelWidth (int sz)
{
  XtVaSetValues (label, XmNwidth, sz, NULL);
}


void AvSlider::setMinLabelWidth (int sz)
{
  XtVaSetValues (minLabel, XmNwidth, sz, NULL);
}


void AvSlider::setSliderWidth (int sz)
{
  XtVaSetValues (slider, XmNscaleWidth, sz, NULL);
}


void AvSlider::setMaxLabelWidth (int sz)
{
  XtVaSetValues (maxLabel, XmNwidth, sz, NULL);
}


void AvSlider::setValueFieldWidth (int sz)
{
  XtVaSetValues (valueField, XmNcolumns, sz, NULL);
}


AvSlider::~AvSlider ()
{
}
