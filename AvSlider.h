//# Copyright (C) 1995-97 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvSlider.h,v 19.0 2003/07/16 05:47:14 aips2adm Exp $
//
// $Log: AvSlider.h,v $
// Revision 19.0  2003/07/16 05:47:14  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:56  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:26  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:32  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:22  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:18  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:05  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:48  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:59  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:52  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:27:41  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:55  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 07:10:41  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.8  1996/06/18  18:43:21  pixton
//  Copyright Update
//
//  Revision 1.7  1996/03/29  16:23:15  baker
//  Documentation.
//
//  Revision 1.6  1996/03/14  19:51:41  baker
//  Documentation.
//
//  Revision 1.5  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/07/17  19:56:12  hr
//  Added 'virtual' to destructor of virtual classes.
//
//  Revision 1.3  1995/06/24  00:53:11  baker
//  Modified layout to display in 1, 2, or 3 lines.
//
//  Revision 1.2  1995/05/02  20:45:16  pixton
//  added empty virtual function invokeCallbacks() to satisfy cfront.
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------


#ifndef _SV_SLIDER_H
#define _SV_SLIDER_H


/* <summary> Control a numeric value with a slider or text entry
field.  Motif-based class to show and control a numeric value
(displayed in a text field) through a labeled XmScale or through
editing the text field.  Provides user control over spatial
arrangement, which can take 1, 2, or 3 lines.  Also provides spatial
control over precision to use when displaying numeric vale.  Inherits
from AvConductor to provide callbacks. */


#include <AvConductor.h>
#include <AvFormComp.h>

class AvSlider : public AvFormComp, public AvConductor {

public:

  /* Supply parent Widget, boolean for whether to build within this
     parent or to build a new topLevelshell, a choice of how many rows
     to use in building the widget, and a specification of how many
     digits after the decimal to use in printing the min, max, and
     current values. */
  AvSlider
    (Widget parent, int buildInsideParent = TRUE, int nLines=1, int prec=0);
  
  virtual void setLabel (char * newLabel);
  virtual void setMin (float min);
  virtual void setMax (float max);
  virtual void setRange (float min, float max);

  virtual void setValue (float newVal);

  // Specifies how many digits to print after the decimal.
  virtual void setPrecision (int decDigits);

  virtual void setLabelWidth (int sz);
  virtual void setMinLabelWidth (int sz);
  virtual void setSliderWidth (int sz);
  virtual void setMaxLabelWidth (int sz);
  virtual void setValueFieldWidth (int sz);

  virtual float getValue ();

  virtual ~AvSlider();

protected:

  static void sliderChangedCB (Widget, AvSlider *, XtPointer);
  static void valueFieldChangedCB (Widget, AvSlider *, XtPointer);
  static void sliderDragCB (Widget, AvSlider *, XtPointer);

private:

  float value;
  float min;
  float max;

  int precision;

  // Format string determined from current value of precision.
  char format[16];

  Widget label;
  Widget minLabel;
  Widget slider;
  Widget maxLabel;
  Widget valueField;

};

#endif
