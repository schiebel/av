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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvWidgetMisc.h,v 19.0 2003/07/16 05:47:00 aips2adm Exp $
//
// $Log: AvWidgetMisc.h,v $
// Revision 19.0  2003/07/16 05:47:00  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:44  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:13  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:21  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:33  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:22:36  hravlin
// Added AvXmStringCreateLocalized().
//
// Revision 14.0  2000/03/23 16:08:04  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:48  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:19  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:19  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:04  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:26:26  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:06  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/02/09 00:19:39  hr
// Added childNameToWidget().
//
// Revision 7.2  1996/12/12 07:45:33  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.4  1996/06/18  18:43:32  pixton
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
//  Revision 1.2  1995/03/29  21:35:19  hr
//  add widgetTopShell
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvWidgetMisc.h
Miscellaneous routines for dealing with widgets.
*/
#ifndef AvWidgetMisc_h
#define AvWidgetMisc_h
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <Xm/Xm.h>

Visual *widgetVisual(Widget );
unsigned int widgetDepth(Widget w);
Colormap widgetColormap(Widget w);
XVisualInfo *getWidgetVisualInfo(Widget w);
Widget widgetRoot(Widget w);
Widget widgetTopShell(Widget w);

XColor* queryWidgetColormap(Widget w, int &len);

// Given a child's name and its parent, return the child's widget.
Widget childNameToWidget(Widget parent, const char *childName);

// Motif's XmStringCreateLocalized does not take const char *, this does.
XmString AvXmStringCreateLocalized(const char *text);

#endif
