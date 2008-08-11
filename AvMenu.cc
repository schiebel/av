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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvMenu.cc,v 19.0 2003/07/16 05:47:43 aips2adm Exp $
//
// $Log: AvMenu.cc,v $
// Revision 19.0  2003/07/16 05:47:43  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:20  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:49  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:56  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:59  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:45:01  hravlin
// Changed some char * to const char *.
//
// Revision 14.0  2000/03/23 16:08:49  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/10/01 22:53:37  hravlin
// Changed Gadgets to Widgets.
//
// Revision 13.0  1999/08/10 18:40:37  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:45  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:21  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:26  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:30:09  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:18:29  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:41:40  droberts
// Final update from monet archive.
//
// Revision 1.2  1996/08/30 17:28:34  hr
// Made string passed to MenuItem a const.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.5  1996/06/18  18:46:19  pixton
//  Copyright Update
//
//  Revision 1.4  1996/02/09  18:18:09  hr
//  Added findMenuInfo().
//
//  Revision 1.3  1996/02/01  21:44:45  hr
//  Added support for button specific creation parameter to allow creating
//  toggle buttons in the on state.
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




#include <iostream.h>
#include <string.h>
#include <Xm/PushB.h>

#include "AvMenu.h"


AvMenu::AvMenu (int n)
{
  parentWidget_ = NULL;
  items_ = new AvMenuItem[n];
  nItems_ = 0;
  maxItems_ = n;
}


void AvMenu::add (const char * name, int typ, int data, AvMenu * sub, int bdata)
{
  if(nItems_ < maxItems_)
    items_[nItems_++].set (name, typ, data, sub, bdata);
  else
    cerr << "AvMenu::add too many menu entries(" << name << ")" << endl;
}


void AvMenu::addButton (const char * name, int typ, int data, AvMenu * sub)
{
  items_[nItems_++].set (name, typ, data, sub);
  char *nam = NULL;
  if(name != NULL)
  {	nam = new char[strlen(name)+1];
	strcpy(nam, name);
  }
  else
  {	nam = new char[strlen("button")+1];
	strcpy(nam, "button");
  }
  XmString xStr = XmStringCreateLocalized (nam);
  delete [] nam;
  XtVaCreateManagedWidget ("", xmPushButtonWidgetClass, parentWidget_,
			   XmNlabelString, xStr,
			   NULL);
  XmStringFree (xStr);
}


void AvMenu::insert (int indx, const char * name, int typ, int data, AvMenu * sub)
{
  for (int i=nItems_; i>indx; i--) 
    items_[i] = items_[i-1];
  items_[indx].set (name, typ, data, sub);
  nItems_++;
}


AvMenuItem::AvMenuItem (const char * name, int typ, int d, AvMenu * sub,
			int bd)
{
  strcpy (buttonName, name);
  buttonType = typ;
  data = d;
  subMenu = sub;
  bdata = bd;
}


void AvMenuItem::set (const char * name, int typ, int d, AvMenu * sub, int bd)
{
  strcpy (buttonName, name);
  buttonType = typ;
  data = d;
  subMenu = sub;
  bdata = bd;
}

// Search a MenuButtonInfo array for name and return a pointer to the
// matching item or NULL if no match.
AvMenuButtonInfo *AvMenu::findMenuInfo(char *name,
				AvMenuButtonInfo *mi, const int milen)
{
	if((mi == NULL) || (name == NULL))
		return NULL;

	for(int i=0; i< milen; i++, mi++)
		if(strcmp(name, mi->name) == 0)
			return mi;
	return NULL;
}
