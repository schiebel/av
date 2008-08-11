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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvConductor.h,v 19.0 2003/07/16 05:46:51 aips2adm Exp $
//
// $Log: AvConductor.h,v $
// Revision 19.0  2003/07/16 05:46:51  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:36  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:04  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:13  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:01  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:07:52  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:37  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:58  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:49  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:29  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:25:30  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:33  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.4  1997/01/10 22:24:33  hr
// Added addToFront arg to constructor.
//
// Revision 7.3  1996/12/12 06:06:38  droberts
// Final update from monet archive.
//
// Revision 1.4  1996/09/27 16:48:13  hr
// Made conductor items subclass of AvResource. Moved a couple of short
// routines to the include file. Added list locking so calling disconnect
// wouldn't result in removing a currently being used conductor item.
//
// Revision 1.3  1996/09/11 19:09:20  hr
// Added AvConductorItem::remove().
//
// Revision 1.2  1996/07/30 18:41:25  hr
// Changed SV_CONDUCTOR to AV_CONDUCTOR
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.10  1996/06/18  18:42:44  pixton
//  Copyright Update
//
//  Revision 1.9  1996/03/29  20:31:53  baker
//  Documentation.
//
//  Revision 1.8  1996/03/20  17:05:20  hr
//  A lot of semicolons had gotten replaced by colons.
//
//  Revision 1.7  1996/03/14  20:03:19  baker
//  Documentation.
//
//  Revision 1.6  1996/01/17  16:23:35  hr
//  removeCallbacks was having problems with removing the head of the list.
//
//  Revision 1.5  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/08/03  16:28:27  hr
//  Major restructuring.
//
//  Revision 1.3  1995/07/17  21:34:10  hr
//  Added virtual destructor.
//
//  Revision 1.2  1995/03/22  15:05:54  baker
//  Removed leftover "AIX" ifdef
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#ifndef AV_CONDUCTOR
#define AV_CONDUCTOR


/* <summary> Support class for AvConductor. */


#include <X11/Intrinsic.h>		// NULL, TRUE, FALSE.
#include "AvResource.h"

class AvConductor;

typedef void (*AvConductorCB) (XtPointer o, AvConductor * c, XtPointer uData);

/* This is used by AvConductor to maintain a list of callbacks. May be
   subclassed.  (execute will need to be overridden).  */

class AvConductorItem : public AvResource {

public:

  AvConductorItem (AvConductorCB callback=NULL, const int when=0,
		   XtPointer object=NULL, XtPointer userData=NULL);

  virtual void execute ();		// Call callback.

  // Most of these are used only by AvConductor.
  // Returns true if the keys match. (If a bit in keymask is set in when_).
  Boolean keyMatch (const int keymask)const
			{return (when_ & keymask) ? TRUE : FALSE;}

  // Returns True if the arguments are the same as the stored values.
  Boolean matches (const AvConductorItem *) const;
  Boolean matches (AvConductorCB callback, const int when,
		   const XtPointer object, const XtPointer userData) const;

  AvConductorItem * next () { return next_; }

  AvConductorCB callback () const { return callback_; }
  void callback (const AvConductorCB cb) { callback_ = cb; }
  int when ()const { return when_; }
  void when (const int w) { when_ = w; }
  XtPointer object ()const { return object_; }
  void object (const XtPointer o){ object_ = o; }
  XtPointer userData ()const { return userData_; }
  void userData (const XtPointer ud) {  userData_ = ud; }
  AvConductor *conductor ()const { return conductor_; }
  // Have the conductor item remove itself from the conductor's callback list.
  // If the conductor item was created by the conductor, it will be destroyed.
  void remove();

protected:
  virtual ~AvConductorItem ();

  AvConductorCB callback_;

  int when_;

  XtPointer object_;

  XtPointer userData_;

private:

  friend class AvConductor;

  // Managed by AvConductor.
  void next (AvConductorItem *n) { next_=n; }
  void conductor (AvConductor *c) { conductor_ = c; }
  AvConductorItem * next_;
  AvConductor * conductor_;

  // Mark item for later removal.
  static void markForRemoval(AvConductorItem *i);
  Boolean markedForRemoval()const{return marked_;}
  Boolean marked_;		// Marked for removal.

};


/* <summary> An abstract class for establishing dependencies between
   objects.  A conductor maintains a list of callback functions.  When
   the conductor object changes in some way it invokes its list of
   callbacks so changes to the Conductor object are passed along to
   generate activity in other objects.

   Objects wishing to be informed of changes can either provide a
   callback (ala X11) or be a subclass of AvConductorItem.

   To notify objects on the list, call 'invokeCallbacks' with a mask
   indicating the reason for the callback.  Callbacks with a mask
   containing a bit from the supplied mask will be called.  (if ( (mask &
   key) != 0) call_callbacks).

   It is possible to 'batch' up requests and send them all at once.
   Each call to disableNotify increments a counter.  If the counter is
   > 0, invokeCallbacks will save the mask by 'OR'ing it into a saved
   copy.  Each call to enableNotify decrements the counter.  When the
   counter reaches 0, invokeCallbacks is called with the saved mask.
   Note: calling invokeCallbacks with the same mask will result in 1
   call, not one for each invokeCallbacks.

   It is the object's responsibility to remove the callback from the
   callback list if the callback is ever destroyed.  (AvConductorItems
   will remove themselves when deleted).
*/


class AvConductor {

public:

  AvConductor ();

  // Add a callback. Normally, entries are added to the end of the list.
  // If addToFront is TRUE, the entry is added to the front of the list.
  virtual void addCallback
    (AvConductorCB func, int when, XtPointer o, XtPointer uData,
     const Boolean addToFront=FALSE);

  virtual void addCallbackItem (AvConductorItem *,
				const Boolean addToFront=FALSE);

  // Events that elicit a callback. May be OR'd together.
  // Subclasses are free to redefine values >= USER0.
  // AvConductor uses Destroyed to notify connected objects it is being
  // deleted. NoReason is returned by callbackReason if invokeCallbacks is
  // not currently executing. The meaning of other values is assigned by the
  // caller.
  enum { NoReason=  0,   Destroyed=1,
	 MouseDown= 2,   MouseMove= 4,   MouseUp=8, 
	 Freq1=0x10,     Freq2=   0x20,  Freq3=  0x40,   ValueChanged=0x80,
	 RSRV1= 0x100,   RSRV2=  0x200,  RSRV3=  0x400,  RSRV4=  0x800,
	 RSRV5= 0x1000,  RSRV6= 0x2000,  RSRV7= 0x4000,  RSRV8= 0x8000,
	 USER0=0x10000,  USER1=0x20000,  USER2=0x40000,  USER3=0x80000,
	 USER4=0x100000, USER5=0x200000, USER6=0x400000, USER7=0x800000
	   // etc.
       };

  // Remove callback ci from the list.
  void removeCallback (AvConductorItem *ci);

  // Remove a callback which matches these parameters.
  virtual void removeCallback
    (AvConductorCB func, int when, XtPointer o, XtPointer uData);

  // A conductor item being destroyed will call this to inform the
  // conductor to remove it from the callback list.
  void callbackDestroyed (AvConductorItem *ci);

  // Call callbacks which match (share a bit with) which.
  virtual void invokeCallbacks (const int which);

  // If invokeCallbacks is being executed, returns 'which' otherwise
  // returns NoReason.
  int callbackReason () const { return reason_; }

  // Enable/disable callback notification.
  // Each call to disableNotify increments a counter. Each call to
  // enableNotify decrements it. When the count reaches 0, the callbacks
  // will be called unless notify is FALSE.
  void enableNotify (const Boolean notify=TRUE);
  void disableNotify ();

  void clearCallbackMask ();	// Clears callback mask generated by
				// calls to invokeCallbacks while notification
				// is off. (Seldom needed).

  Boolean notifyEnabled () const {return (disableCount_ == 0) ? TRUE : FALSE;}

  virtual ~AvConductor ();

protected:

  // Returns true if item is found. If prev isn't null it is set to the
  // previous item in the list. (NULL if item is the head).
  // If needMatch is TRUE, comparison is done on the contents, otherwise,
  // the pointers are compared.
  Boolean findItem (AvConductorItem *ci, const Boolean needMatch,
		    AvConductorItem **prev=NULL)const;

  AvConductorItem * head, * tail;

private:

  int reason_;				// Reason for calling callbacks.
  int disableCount_;			// Count of # disableNotify calls.
  int reasons_;				// Mask containing batched requests.

  int empty () const { return ( (head == 0) ? TRUE : FALSE); }

  // Remove an item or its successor.
  void removeItem (AvConductorItem *ci);
  void removeNextItem (AvConductorItem *prev);

  virtual void removeCallbacks ();	// Remove list.
  // We don't want the list to change while we're traversing it.
  Boolean listLocked()const{return listLocked_;}
  void lockList(){listLocked_ = TRUE;}
  void unlockList();		// Will do a sweep and removed marked items.
  Boolean listLocked_;
};

#endif
