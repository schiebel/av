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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvTwoValueSlider.h,v 19.0 2003/07/16 05:46:46 aips2adm Exp $
//
// $Log: AvTwoValueSlider.h,v $
// Revision 19.0  2003/07/16 05:46:46  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:32  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:01  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:10  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:44  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:07:48  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:32  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:49  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:35  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:14  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:25:06  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:19  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 07:35:15  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.5  1996/06/18  18:43:25  pixton
//  Copyright Update
//
//  Revision 1.4  1996/03/14  19:51:56  baker
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
//  Revision 1.2  1995/06/24  00:52:32  baker
//  *** empty log message ***
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------


#ifndef _SV_TWO_VALUE_SLIDER_H
#define _SV_TWO_VALUE_SLIDER_H


/* <g> UserInterface */

/* <s> Control a value range with a double-control slider. */

/* <d> Motif-based class to show and control a value range (with min
and max displayed in text fields).  Controls can be operated
separately to edit either the min or the max, or they can be 'locked'
together to maintain a constant-sized interval.  Inherits from
AvConductor to provide callbacks. */


#include <AvFormComp.h>
#include <AvConductor.h>

class AvTwoValueSlider : public AvFormComp, public AvConductor {
  
  friend class AvClipper;

public:

  AvTwoValueSlider
    (Widget parent, int buildInsideParent = TRUE, int chWidth = 256);

  enum { Left, Right, Both };

  void getValues (float & min, float & max, int & change);

  void setMinMaxVals (int min, int max);

  void setLock (int trueOrFalse);

  void setLeft (int newPos);
  void setRight (int newPos);

  void setLeftPercentage (float newVal);
  void setRightPercentage (float newVal);

protected:

  Dimension channelWidth;

private:

  Widget leftControl, rightControl, channel;

  Position leftPosition, rightPosition;

  int activeWidth;

  // if "locked", the two controls will maintain a constant
  // separation.  For example, if the separation is 0, they 
  // will be tied together to always have the same value.
  int locked;
  int separation;

  // this stores whether we're moving left or right controls
  XtPointer ctrl;

  static int controlWidth, controlHalfWidth, controlHeight;

  // these data members should be used only when the slider is 
  // controlling integer values.  
  // the min and max values that the slider controls
  int minVal, maxVal;
  // the number of pixels per unit
  float ppuVal;

  static void leftArrowCB (Widget, AvTwoValueSlider *, XtPointer);
  static void rightArrowCB (Widget, AvTwoValueSlider *, XtPointer);

  static void buttonDown (Widget, AvTwoValueSlider *, XEvent *);
  static void buttonMove (Widget, AvTwoValueSlider *, XEvent *);
  static void buttonUp (Widget, AvTwoValueSlider *, XEvent *);
};

#endif
