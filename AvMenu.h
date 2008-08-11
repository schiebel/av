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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvMenu.h,v 19.0 2003/07/16 05:47:16 aips2adm Exp $
//
// $Log: AvMenu.h,v $
// Revision 19.0  2003/07/16 05:47:16  aips2adm
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
// Revision 15.0  2000/10/26 17:10:29  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:45:20  hravlin
// Changed some char * to const char *.
//
// Revision 14.0  2000/03/23 16:08:20  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:07  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:52  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:04  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:58  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1997/12/17 21:51:08  hr
// Changes to remove warnings from MIPSpro.7x compiler.
//
// Revision 9.0  1997/08/25 21:27:52  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:17:01  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 08:45:49  droberts
// Final update from monet archive.
//
// Revision 1.3  1996/11/11 18:01:26  hr
// Added name() and widget() functions to AvMenuItem.
//
// Revision 1.2  1996/08/30 17:28:34  hr
// Made string passed to MenuItem a const.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.8  1996/06/18  18:43:06  pixton
//  Copyright Update
//
//  Revision 1.7  1996/02/09  18:20:13  hr
//  Added findMenuInfo().
//
//  Revision 1.6  1996/02/08  18:36:33  hr
//  Added the AvMenuButtonInfo struct.
//
//  Revision 1.5  1996/02/01  21:46:34  hr
//  Added support for button specific creation parameter to allow creating
//  toggle buttons in the on state.
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
//  Revision 1.3  1995/08/28  14:41:07  baker
//  Lenghthened buttonName field.
//
//  Revision 1.2  1995/05/19  21:52:37  baker
//  Removed "AIX" leftovers
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#ifndef SV_MENU_H
#define SV_MENU_H

#include <Xm/Xm.h>
#include "Av.h"

class AvApp;

enum MenuButtons {
  SEPARATOR, 
  SUBMENU_BUTTON,
  PUSH_BUTTON, 
  TOGGLE_BUTTON, 
  RADIO_BUTTON,
  LAUNCH_BUTTON,
  HELP_BUTTON
  };

// This can be used to pass information about how to build menu buttons
// between procedures.
typedef struct AvMenuButtonInfo{
	const char	*name;
	MenuButtons	buttonType;
	int		buttonData;
	}AvMenuButtonInfo;

class AvMenu;

class AvMenuItem {

  friend class AvApp;

public:

  AvMenuItem (const char * name = "Push Me",
	      int typ = PUSH_BUTTON,
	      int d = 0,
	      AvMenu * sub = 0, int bdata=0);

  void set (const char * name = "Push Me",
	    int typ = PUSH_BUTTON,
	    int d = 0,
	    AvMenu * sub = 0, int bdata=0);
  const char *name()const{return buttonName;}
  Widget widget()const{return button;}
private:

  char buttonName[128];
  int buttonType;
  Widget button;
  int data;
  AvMenu * subMenu;
  AvApp * app;
  int	bdata;
};

// Button specific information for creating the button.
// Currently, the only use is for toggle buttons. If bdata is non zero,
// the button is initially in the on state.

class AvMenu {

  friend class AvApp;

public:

  AvMenu (int n);

  void add (const char * name = "Push Me", int typ = PUSH_BUTTON,
	    int data = 0, AvMenu * sub = 0, int bdata=0);

  void insert
    (int indx, const char * name = "Push Me", int typ = PUSH_BUTTON,
     int data = 0, AvMenu * sub = 0);

  int howMany () { return (nItems_); }

  AvMenuItem * items () { return items_; }

  Widget parentWidget () { return (parentWidget_); }

  void addButton (const char * name = "Push Me", int typ = PUSH_BUTTON,
		  int data = 0, AvMenu * sub = 0);
  // Search a MenuButtonInfo array for name and return a pointer to the
  // matching item or NULL if no match.
  static AvMenuButtonInfo *findMenuInfo(char *name,
				AvMenuButtonInfo *mi, const int milen);
private:

  // the widget that holds the menu
  Widget parentWidget_;

  // the items to appear in the menu
  AvMenuItem * items_;

  int nItems_;
  int maxItems_;
};


#endif
