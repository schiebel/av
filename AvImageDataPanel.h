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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvImageDataPanel.h,v 19.0 2003/07/16 05:47:33 aips2adm Exp $
//
// $Log: AvImageDataPanel.h,v $
// Revision 19.0  2003/07/16 05:47:33  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:12  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:42  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:48  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:29  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:38  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:27  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:28  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:54  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:54  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:29:19  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.1  1997/02/25 17:06:23  hr
// Added callbacks for AvNDConfig support.
// Removed unused effectiveRank() function.
//
// Revision 8.0  1997/02/20 03:17:58  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1996/12/12 08:36:42  droberts
// Upgrades to support dynamic datasets.
//
// Revision 1.3  1996/09/18 19:02:25  hr
// More integration with AvAccessor.
//
// Revision 1.2  1996/09/10 16:52:17  hr
// Initial changes for integration with AvAccessor & AvDataSet.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.5  1996/06/18  19:17:33  hr
//  Added setRangeCB, resetRangeCB.
//
//  Revision 1.4  1996/06/18  18:43:00  pixton
//  Copyright Update
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

#ifndef _IMAGE_DATA_PANEL_H
#define _IMAGE_DATA_PANEL_H


#include "AvConductor.h"
#include "AvFormComp.h"
#include "AvImageView.h"


class AvClipper;
class AvConductor;
class AvImageView;
class AvImageViewItem;
class AvTable;
class AvAccessor;
class AvNDConfig;

class AvImageDataPanel : public AvConductor, public AvFormComp {

public:

  AvImageDataPanel
    (Widget parent, int buildInsideParent, AvImageViewItem & a);

  AvImageDataPanel
    (Widget parent, int buildInsideParent, AvImageDataItem & a);

  AvImageDataItem * item () { return (&dItem_); }

  // update the panel to reflect the current data
  // Currently, this updates the min and max values shown on the
  // transfer function editor.  Is that sufficient?  *** pb 1/15/95
  void update ();

  // update the panel when data is modified and for example
  // the min and max clip values are different
  void dataModified();

  void setAxisViewIndicator (int axis, int status);

  virtual ~AvImageDataPanel ();
  AvAccessor *accessor()const{return dItem_.accessor();}
protected:
  
private:

  AvImageDataItem & dItem_;


  int rank()const{return accessor()->rank();}
//  int effectiveRank()const{return accessor()->effectiveRank();}

//  WidgetList axis_;

//  AvTable * regionSize_;

  AvClipper * dataClip_;

  void buildBasicInfo (Widget parent);
  void buildViewControls (Widget parent);
  void buildDataTransfer (Widget parent);

  void updateDisplay ();
  void updateClipMinMax ();
  void changeAxis (Widget w);

  void setDataTransfer ();

  static void updateClipCB (AvImageDataPanel *, AvConductor *, int);
  // Call when a view button is pressed to cause a view to be shown.
  static void changeAxisCB (Widget, AvImageDataPanel * p, XtPointer);
  // Called when a view toggle is pressed that needs to have its axis
  // mapping set.
  static void doMapCB (Widget, AvImageDataPanel * p, XtPointer);
  // Called when the mapping window is closed via 'done' button.
  // If destroyed is TRUE, the window was destroyed instead.
  static void haveMapCB (AvNDConfig *, XtPointer, const Boolean destroyed);

  static void setRangeCB(Widget, AvImageDataPanel *, XtPointer);
  static void resetRangeCB(Widget, AvImageDataPanel *, XtPointer);
};

#endif
