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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvClipper.h,v 19.0 2003/07/16 05:46:42 aips2adm Exp $
//
// $Log: AvClipper.h,v $
// Revision 19.0  2003/07/16 05:46:42  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:29  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:41:57  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:07  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:32  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 18:13:22  hravlin
// Changed some char * to const char *.
//
// Revision 14.0  2000/03/23 16:07:44  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/08/25 19:59:20  hravlin
// Updated copyright date.
//
// Revision 13.0  1999/08/10 18:39:27  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:40  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:23  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:51:59  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:24:43  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:05  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1996/12/12 05:44:27  droberts
// Final update from monet archive.
//
// Revision 1.2  1996/10/23 15:27:47  hr
// Added ability to allow user to edit min/max fields from the keyboard.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.5  1996/06/18  18:42:41  pixton
//  Copyright Update
//
//  Revision 1.4  1996/03/14  19:51:17  baker
//  Documentation.
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
//  Revision 1.2  1995/07/17  19:56:12  hr
//  Added 'virtual' to destructor of virtual classes.
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#ifndef SV_CLIPPER_H
#define SV_CLIPPER_H


/* <g> UserInterface */

/* <s> Control a numeric value with a slider or text entry field. */

/* <d> Motif-based class to show and control a value range (with min
and max displayed in text fields).  Provides user control over display
of text fields to show min and max values of overall range and min and
max values of clipped range.  Also provides spatial control over
precision to use when displaying numeric vale.  Inherits from
AvConductor to provide callbacks. */


#include "AvTwoValueSlider.h"

class AvConductor;
class AvObject;

#include "AvFormComp.h"

class AvClipper : public AvFormComp {

public:

  enum { Left, Right, Both };

  /* Supply parent Widget, boolean for whether to build within this
     parent or to build a new topLevelshell, and a specification of
     how many digits after the decimal to use in printing the min,
     max, and current values. If editable is 1, then the user may change
     the min/max fields. Otherwise, they are read only.
  */
  AvClipper
    (Widget parent, int buildInsideParent = TRUE, int width=256, int prec=0,
     const Boolean editable=0);

  void setPrecision (int p);

  void setMin (float m);
  void setMax (float m);
  void setRange (float m1, float m2);
  void setClipMin (float m);
  void setClipMax (float m);
  void setClipRange (float m1, float m2);

  void setValue (float newVal);

  void setLock (int trueOrFalse);

  void setLabels (const char * newMinLabel, const char * newMaxLabel);
  void setClipLabels (const char * newMinLabel, const char * newMaxLabel);

  void showLabels (int);
  void showClipLabels (int);

  void getValues (float & min, float & max, int & change);

  float min () { return (min_); }
  float max () { return (max_); }

  float getMin () { return (min_); }
  float getMax () { return (max_); }

  float clipMin () { return (clipMin_); }
  float clipMax () { return (clipMax_); }

  float getClipMin () { return (clipMin_); }
  float getClipMax () { return (clipMax_); }

  virtual ~AvClipper ();

  virtual void addCallback
    (AvConductorCB func, int when, XtPointer o, XtPointer uData)
    {
      slider_->addCallback (func, when, o, uData);
    }

private:

  // The slider used to set clipMin_ and clipMax_
  AvTwoValueSlider * slider_;

  // The range for this clipper
  float min_, max_;

  // The current min and max values specified by the clipper
  float clipMin_, clipMax_;

  // How wide to make the slider
  int sliderWidth_;

  // The number of places after the decimal to show in displaying values
  int precision_;
  // Based on precision, the format string to use in a printf statement
  char format_[16];

  Widget minLabel_, maxLabel_, minNum_, maxNum_;

  Widget clipMinLabel_, clipMaxLabel_, clipMinNum_, clipMaxNum_;

  // Sets the number of pixels the slider move when an arrow button is binked
  void setArrowJumps ();

  // Convenience routine for setting either readout values for 
  // min, max, clipMin, clipMax, etc.
  void setMinMax (Widget w, float m);

  static void updateCB (AvObject *, AvConductor *, void *);
  static void minNumChangedCB (Widget, AvClipper * p, XKeyEvent * e);
  static void maxNumChangedCB (Widget, AvClipper * p, XKeyEvent * e);
  // Convert a text field value to a double. Returns 1 for a valid number
  // otherwise 0.
  static int getTextFField(Widget w, double &value);
};

#endif
