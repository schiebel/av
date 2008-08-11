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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvXColorApp.h,v 19.0 2003/07/16 05:46:55 aips2adm Exp $
//
// $Log: AvXColorApp.h,v $
// Revision 19.0  2003/07/16 05:46:55  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:40  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.2  2002/01/14 17:54:17  hravlin
// Changed fallbacks arg to be const char *[].
//
// Revision 17.1  2002/01/03 22:00:24  hravlin
// Added externalDrawingArea() (support for wiregl).
//
// Revision 17.0  2001/11/12 19:42:08  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:17  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:13  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:20:57  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:07:58  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.2  1999/09/28 19:36:44  hravlin
// Added nbusyStrings_.
//
// Revision 13.1  1999/09/27 15:47:29  hravlin
// Made busy_ a counter rather than a boolean.
//
// Revision 13.0  1999/08/10 18:39:42  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:07  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:01  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:43  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1997/12/17 21:52:53  hr
// Changes to remove warnings from MIPSpro.7x compiler.
//
// Revision 9.0  1997/08/25 21:25:54  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:47  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/01/09 18:20:51  hr
// Changed constructor arg list to include parameters to allow retrieving
// of command line X args.
//
// Revision 7.2  1996/12/12 07:47:34  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.3  1996/06/18  18:43:35  pixton
//  Copyright Update
//
//  Revision 1.2  1996/04/06  18:28:35  hr
//  Added set/reset busy.
//
//  Revision 1.1  1996/03/14  18:10:10  baker
//  Initial revision
//
//
//---------------------------------------------------------------------------


#ifndef _SV_XCOLOR_APP_H
#define _SV_XCOLOR_APP_H

#include "AvApp.h"

class AvXPalette;

class AvXColorApp : public AvApp {

public:

  virtual const char * className () const;

  static Colormap colormap ();

  static int maxColors ();

  static int availableColors ();

  static void syncColormap ();

  static AvXPalette * palette ();

  // Is aipsview busy?
  static Boolean busy(){return (busy_ > 0)? TRUE : FALSE;}
  // Change busy state of aipsview. These should be placed around functions
  // that could take a long time. Menubars should not be changed during busy
  // periods although changing submenus should be ok.
  static void setBusy();
  static void resetBusy();

  // When linked with wiregl, the drawing area is not part of a widget
  // we control. A number of routines need to know that.
  static Boolean externalDrawingArea();
protected:

  AvXColorApp (Widget parent, int buildInsideParent);

  AvXColorApp
    (const char * appClass, int & argc, String * argv,
     const char * fallbacks[] = NULL);

  AvXColorApp (const char * appClass,
	       XrmOptionDescRec * opt, Cardinal noptions,
	       int & argc, String * argv,
	       const char * fallbacks[],
	       ArgList args, Cardinal nargs,
	       // Arguments to XtGetApplicationResources
	       XtPointer varStruct=NULL,// User defined struct to hold vars.
	       XtResource *resources=NULL, const int nResources=0);

  virtual void initialize ();

  static AvXPalette * palette_;

  virtual ~AvXColorApp ();

  // Set/clear visual busy indicator.
  void setBusyToggle(const Boolean setToBusy);

private:
  static int busy_;		// Is aipsview 'Busy'?
  XmString *busyStrings_;	// Holds list of menu labels while busy.
  Cardinal nbusyStrings_;
};

#endif
