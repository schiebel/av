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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvGlishRequestList.h,v 19.0 2003/07/16 05:47:18 aips2adm Exp $
//
// $Log: AvGlishRequestList.h,v $
// Revision 19.0  2003/07/16 05:47:18  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:59  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:28  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:35  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:34  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:21  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  2000/03/09 17:17:29  hravlin
// Removed some compiler warnings.
//
// Revision 13.0  1999/08/10 18:40:09  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:56  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:09  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:03  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:28:00  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:17:06  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:26:27  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.4  1996/06/18  18:42:54  pixton
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
//  Revision 1.2  1995/09/19  22:47:30  baker
//  *** empty log message ***
//
//
//---------------------------------------------------------------------------

/*
GlishRequestList.h

Behaviour -

Creation:

  Make an empty request list when aipsView is compiled, sending
  the glish client pointer as the only parameter.

Glish Control:

  Glish events which require user intervention are accumulated
  in a queue and displayed to the user, but the user is not forced
  to do anything about it.  Whenever a glish event arrives, the
  pending request window is shown (if previously empty) and the
  window is raised, with the new event at the end of the list.

User Control:

  Whenever the user presses the apply key on one of the events,
  the selection is made available to the request queue, and the
  event response is sent as a response.  The cancel key sends the
  cancelled event for the request.

Deletion:

  Only when the app exits.
*/


#ifndef __AvGlishRequestList_h__
#define __AvGlishRequestList_h__

#include <X11/Intrinsic.h>
#include <Xm/Xm.h>

#include <AvList.h>
#include <AvXGlishClient.h>

class AvGlishRequestList;

class AvGlishRequestListEntry {
public:

  AvGlishRequestListEntry(Widget parent,
			  void * data,
			  XtCallbackProc applyCB,
			  XtCallbackProc cancelCB,
			  const char * eventName, int imageID, 
			  const char * imageName, const char * reason)
    : parent_(parent), imageID_(imageID)
      {
	if (!eventName) abort();
	eventName_ = new char[strlen(eventName)+1];
	strcpy(eventName_, eventName);

	if (!reason) abort();
	reason_ = new char[strlen(reason)+1];
	strcpy(reason_, reason);
	
	// Sets "entry_"
	build(imageName, data, applyCB, cancelCB);

	// pack "this" into the user data field so all callbacks
	// can be handled through AvGlishRequestList
	XtVaSetValues(entry_, XmNuserData, this, NULL);
      }

  ~AvGlishRequestListEntry() { XtDestroyWidget(entry_); }

  // eventName - CG access
  const char * eventName() const { return (const char *) eventName_; }

  // imageID - CG access
  int imageID() const { return imageID_; }

  // Construct the Widget tree
  void build(const char * iname, void * data, 
	     XtCallbackProc applyCB, XtCallbackProc cancelCB);

  // Widget access
  Widget widget() const { return entry_; }

private:

  char * reason_; 
  char * eventName_;

  Widget parent_;  // Parent Widget
  int imageID_;
  Widget entry_;   // Form for entry area
};

//
//  Request list is attached to the open client (if available)
//
class AvGlishRequestList
{
public:

  AvGlishRequestList(Widget parent, AvXGlishClient * xgclient) 
    : parent_(parent), xgclient_(xgclient) 
      {
	build();
      }

  // Called by list entries.  Entry pointer packed into w's userData field
  static void applyCB(Widget w, AvGlishRequestList * list, XtPointer);
  static void cancelCB(Widget w, AvGlishRequestList * list, XtPointer);

  void addRequest(const char * eventName, int imageID, 
		  const char * imageName, const char * reason);
  void addRequest(AvGlishRequestListEntry * e);
  void deleteRequest(AvGlishRequestListEntry * e);

  AvXGlishClient * xgclient() const { return xgclient_; }

private:

  void build();

  // Inhibit default constructor
  AvGlishRequestList();


  Widget main_;    // Rowcolumn managing the request entries
  Widget parent_;  // parent
  Widget shell_;   // shell window
  AvXGlishClient * xgclient_;

  AvList<AvGlishRequestListEntry *> requestList_;
};

#endif
