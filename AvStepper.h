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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvStepper.h,v 19.0 2003/07/16 05:47:17 aips2adm Exp $
//
// $Log: AvStepper.h,v $
// Revision 19.0  2003/07/16 05:47:17  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:58  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:27  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:34  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:31  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:20  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:08  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:53  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:06  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:00  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:27:54  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:17:02  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/02/06 18:01:28  hr
// Changes to support 1 relative readouts.
//
// Revision 7.2  1996/12/12 07:13:14  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.8  1996/06/18  18:43:22  pixton
//  Copyright Update
//
//  Revision 1.7  1996/03/29  15:09:25  baker
//  Modified editable stepper functionality.
//
//  Revision 1.6  1996/03/27  22:39:19  hr
//  AvStepper now supports user editable min and max fields.
//
//  Revision 1.5  1996/03/14  19:52:12  baker
//  Documentation.
//
//  Revision 1.4  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/08/07  15:51:59  baker
//  Added routines for retrieving the min and max values of the stepper.
//
//  Revision 1.2  1995/07/17  19:56:12  hr
//  Added 'virtual' to destructor of virtual classes.
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#ifndef _SV_STEPPER_H
#define _SV_STEPPER_H


/* <summary> A counter with up and down arrows to increment/decrement.
This is a Motif-based class to show and control an integer counter
(displayed in a text field) with left-pointing and right-pointing
arrows.  User can manipulate the current value with the arrows or by
text-entry in the value display field.  Optionally, the user can edit
the text fields that display the minimum and maximum values that make
up the range of the stepper. */


#include <AvFormComp.h>
#include <AvConductor.h>

class AvStepper : public AvFormComp, public AvConductor {

public:

  enum {ValueChanged = AvConductor::ValueChanged,
	RangeChanged = AvConductor::USER0};
  
  AvStepper (Widget parent, int buildInsideParent, int min, int max,
	     int userEdit=FALSE);

  virtual void setLabel (char * newLabel);
  virtual void setValue (int newVal);
  virtual void setMinMax (int newMin, int newMax);
  virtual void setLabelWidth (int sz);
  virtual void setValueFieldWidth (int sz);
  virtual void setId (int i) { id_ = i; }

  virtual int value () { return (value_); }
  virtual int min () { return (min_); }
  virtual int max () { return (max_); }
  virtual int id () { return id_; }

  virtual int getValue () { return (value_); }
  virtual int getMin () { return (min_); }
  virtual int getMax () { return (max_); }
  virtual int getId () { return id_; }
  // Displayed strings have this number added to them.
  void setDisplayOffset(const int n);
  int getDisplayOffset()const {return displayOffset_;}
  virtual ~AvStepper();

protected:

private:

  int value_;
  int displayOffset_;	// Displayed text = value_ + displayOffset_;
  // The full range of values supported by the stepper.
  int fullMin_;
  int fullMax_;

  // Can the range of values be edited by the user?
  int userEdit_;

  // User-settable min and max values.
  int min_;
  int max_;

  Widget label_;
  Widget leftLabel_, rightLabel_;
  Widget valueField_;
  Widget leftArrow_;
  Widget rightArrow_;

  int id_;

  void setMin (int, int &, Widget);
  void setMax (int, int &, Widget);

  static void leftArrowChangedCB (Widget, AvStepper *, XtPointer);
  static void rightArrowChangedCB (Widget, AvStepper *, XtPointer);
  static void typeValueCB (Widget, AvStepper *, XKeyEvent *);
  static void leftLabelChangedCB (Widget, AvStepper *, XKeyEvent *);
  static void rightLabelChangedCB (Widget, AvStepper *, XKeyEvent *);

};

#endif
