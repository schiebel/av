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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvDisplayWindow.cc,v 19.0 2003/07/16 05:47:13 aips2adm Exp $
//
// $Log: AvDisplayWindow.cc,v $
// Revision 19.0  2003/07/16 05:47:13  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:55  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/14 17:06:31  hravlin
// Changed fallbacks arg to be const char *[].
//
// Revision 17.0  2001/11/12 19:42:24  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:31  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:17  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:56:19  hravlin
// Changed className() to return const char *.
//
// Revision 14.0  2000/03/23 16:08:17  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:04  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:45  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:55  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:47  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/09 20:30:29  hr
// Removed a couple of extra semicolons.
//
// Revision 9.0  1997/08/25 21:27:34  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:46  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:11:54  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.4  1996/06/18  18:45:59  pixton
//  Copyright Update
//
//  Revision 1.3  1996/03/20  17:35:15  hr
//  Changed to be derived from AvXColorApp.
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

#include <AvDisplayWindow.h>

const char * AvDisplayWindow::className () const
{
  return ("AvDisplayWindow");
}


AvDisplayWindow::AvDisplayWindow
(char * appClass, int & argc, String * argv, const char * fallbacks[])
: AvXColorApp (appClass, argc, argv, fallbacks)
{
  nVisReps = 0;
}


AvDisplayWindow::AvDisplayWindow (Widget parent, int buildInsideParent)
: AvXColorApp (parent, buildInsideParent)
{
  nVisReps = 0;
}

AvDisplayWindow::~AvDisplayWindow () {}

AvMenu * AvDisplayWindow::constructMainMenu (int & nDefault, int howMany)
{
  nDefault = 4;
  howMany = MAX(howMany, nDefault);
  AvMenu * menu = new AvMenu(howMany);
//  menu->add ("File", SUBMENU_BUTTON, 0, fileMenu);
//  menu->add ("Edit", SUBMENU_BUTTON, 0, editMenu);
  menu->add ("Options", SUBMENU_BUTTON, 0, optionMenu);
//  menu->add ("Help", HELP_BUTTON, 0, helpMenu);
  return (menu);
}


AvMenu * AvDisplayWindow::constructFileMenu (int & nDefault, int howMany)
{
  nDefault = 4;
  howMany = MAX(howMany, nDefault);
  AvMenu * menu = new AvMenu(howMany);
  menu->add ("Save screen...", PUSH_BUTTON, SV_SAVE_SCREEN);
  menu->add ("Print...", PUSH_BUTTON, SV_PRINT_SCREEN);
  return (menu);
}


AvMenu * AvDisplayWindow::constructEditMenu (int & nDefault, int howMany)
{
  nDefault = 2;
  howMany = MAX(howMany, nDefault);
  AvMenu * menu = new AvMenu (howMany);
  menu->add ("Axes...",  PUSH_BUTTON, SV_AXES);
  menu->add ("Background...", PUSH_BUTTON, SV_BACKGROUND);
  return (menu);
}


AvMenu * AvDisplayWindow::constructOptionMenu (int & nDefault, int howMany)
{
  nDefault = 2;
  howMany = MAX(howMany, nDefault);
  AvMenu * menu = new AvMenu (howMany);
  return (menu);
}


AvMenu * AvDisplayWindow::constructHelpMenu (int & nDefault, int howMany)
{
  nDefault = 1;
  howMany = MAX(howMany, nDefault);
  AvMenu * menu = new AvMenu (howMany);
  menu->add ("", PUSH_BUTTON);
  return (menu);
}



