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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvConductor.cc,v 19.0 2003/07/16 05:46:58 aips2adm Exp $
//
// $Log: AvConductor.cc,v $
// Revision 19.0  2003/07/16 05:46:58  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:42  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:11  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:19  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:23  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:01  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:45  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:13  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:10  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:54  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:26:09  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:56  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.4  1997/01/10 22:24:15  hr
// Added addToFront arg to constructor.
//
// Revision 7.3  1996/12/12 06:04:08  droberts
// Final update from monet archive.
//
// Revision 1.4  1996/09/27 16:50:02  hr
// Made conductor items subclass of AvResource. Moved a couple of short
// routines to the include file. Added list locking so calling disconnect
// wouldn't result in removing a currently being used conductor item.
//
// Revision 1.3  1996/09/11 19:09:20  hr
// Added AvConductorItem::remove().
//
// Revision 1.2  1996/07/30 18:40:27  hr
// Added casts to NULL to keep g++ happy.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.7  1996/06/18  18:45:54  pixton
//  Copyright Update
//
//  Revision 1.6  1996/01/17  16:23:02  hr
//  removeCallbacks was having problems with removing the head of the list.
//
//  Revision 1.5  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/08/03  16:26:55  hr
//  (Missed a newline during last checkin).
//
//  Revision 1.3  1995/08/03  16:25:08  hr
//  Major changes including making callback reason a bit mask rather than
//  a single value. Current references to conductor should still work.
//
//  Revision 1.2  1995/07/17  21:32:53  hr
//  Added destructor.
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------
#include <iostream.h>		// For the cerr.

#include "AvConductor.h"

AvConductorItem::AvConductorItem(AvConductorCB callback, const int when,
				 XtPointer object, XtPointer userData)
{
	callback_ = callback;
	conductor_ = NULL;
	when_ = when;
	object_ = object;
	userData_ = userData;
	next_ = NULL;
	marked_ = FALSE;
}

AvConductorItem::~AvConductorItem()
{
	// Notify conductor of the destruction. (conductor_ will be NULL if
	// AvConductor is destroying this).
	if(conductor_ != NULL)
		conductor_->callbackDestroyed(this);
}

// Call the callback with current data.
void AvConductorItem::execute()
{
	if(callback_ != NULL)
		callback_ (object_, conductor_, userData_);
}

Boolean AvConductorItem::matches(AvConductorCB callback, const int when,
		  const XtPointer object, const XtPointer userData)const
{
	return  (callback == callback_) && (when == when_) &&
		(object == object_) && (userData == userData_);
}

Boolean AvConductorItem::matches(const AvConductorItem *ci)const
{
	if(ci == NULL)
		return FALSE;

	return matches(ci->callback_, ci->when_, ci->object_, ci->userData_);
}

void AvConductorItem::remove()
{
	if(conductor_ != NULL)
		conductor_->removeCallback(this);
}

void AvConductorItem::markForRemoval(AvConductorItem *i)
{
	if(i != NULL)
		i->marked_ = TRUE;
}


////////////////////////////////////////////////////////////////////////////

AvConductor::AvConductor ()
{
  head = tail = 0;
  reason_ = NoReason;
  reasons_ = 0;
  disableCount_ = 0;
  listLocked_ = FALSE;
}

AvConductor::~AvConductor ()
{
	invokeCallbacks(Destroyed);
	removeCallbacks();
}

// Add callback item to list.
void AvConductor::addCallbackItem(AvConductorItem *ci, const Boolean atFront)
{
  if(ci == NULL)
	return;
  ci->conductor(this);
  ci->ref();
  ci->lockResource();
  // is this the first item?
  if (empty ())
    head = tail = ci;
  else
  { if(atFront)
    {
	ci->next(head);
	head = ci;
    }
    else
    {
	tail->next(ci);
	tail = ci;
    }
 }
}

/* Search for item. If needMatch is TRUE, comparison is done by checking
the components. If FALSE, the pointers are checked instead.
On return, prev will point to the predecessor of the node that matched or
NULL if node was at the head.
*/
Boolean AvConductor::findItem(AvConductorItem *ci, const Boolean needMatch,
				AvConductorItem **prev)const
{
AvConductorItem *p, *pp;
Boolean found = FALSE;

  if((ci == NULL) || (empty()))
	return FALSE;

  for(pp=NULL, p = head; p != NULL; p = p->next())
  {	if( (!ci->markedForRemoval()) && ((needMatch && ci->matches(p)) ||
	    (!needMatch && (p == ci))))
	{	found = TRUE;
		break;
	}
	pp = p;
  }
  if((prev != NULL) && found)
	*prev = pp;

  return found;
}

// Remove prev's next item from the list or the head if prev is NULL.
// If the list is locked, just mark it.
void AvConductor::removeNextItem(AvConductorItem *prev)
{
AvConductorItem *ci, *next;	// Item to be removed and the one after it.

  // If the list is locked, only mark the item.
  if(listLocked())
  {	if(prev != NULL)
		AvConductorItem::markForRemoval(prev->next());
	else
		AvConductorItem::markForRemoval(head);
	return;
  }
  // Remove from list, updating the pointers.
  if(prev != NULL)
  {	ci = prev->next();
	next = (ci != (AvConductorItem *)NULL) ? ci->next() :
		(AvConductorItem *)NULL;
	prev->next(next);
  }
  else	// Remove head of list.
  {	ci = head;
	next = (ci != (AvConductorItem *)NULL) ? ci->next() :
		(AvConductorItem *)NULL;
	head = next;
  }

  // If tail was removed, the new tail is the predecessor or NULL
  // if the head was the only item on the list.
  if(tail == ci)
	tail = prev;

  // Once removed, ensure it won't try to access conductor again.
  if(ci != NULL)
  {	ci->conductor(NULL);
	ci->unlockResource();
	ci->unref();
  }
}

// Remove item pointed to by ci.
void AvConductor::removeItem(AvConductorItem *ci)
{
AvConductorItem *prev;
  if(findItem(ci, FALSE, &prev))
	removeNextItem(prev);
}

/* Called by a conductor item when it is destroyed. Remove item from list.
Conductor items we're destroying can't call this since the pointer to 'this'
is removed before the delete.
*/
void AvConductor::callbackDestroyed(AvConductorItem *ci)
{
	// This shouldn't happen since we've ref'd the item.
	cerr << "AvConductor::callbackDestroyed called\n";
	removeItem(ci);
}

void AvConductor::removeCallback(AvConductorItem *ci)
{
	removeItem(ci);
}

// Remove callback whose info matches these args.
void AvConductor::removeCallback(AvConductorCB func, int w,
						XtPointer o, XtPointer d)
{
AvConductorItem *ci = new AvConductorItem(func, w, o, d), *prev;

  ci->ref();
   if(findItem(ci, TRUE, &prev))	// Compare using info, not pointers.
	removeNextItem(prev);
  ci->unref();
}

// Get rid of entire list.
void AvConductor::removeCallbacks()
{
  while(head != NULL)
	removeNextItem(NULL);

  head = tail = NULL;
}

void AvConductor::addCallback
(AvConductorCB func, int w, XtPointer o, XtPointer d, const Boolean atFront)
{
  AvConductorItem * ci = new AvConductorItem(func, w, o, d);

  // Is this callback already on the list?
  if(findItem(ci, TRUE))	// Ignore if it's already there.
	ci->unref();
  else
	addCallbackItem(ci, atFront);
}

// Call any callbacks which match key or previous bits we've saved.
// If notifyEnabled is FALSE, just remember the key.
void AvConductor::invokeCallbacks (const int key)
{
  AvConductorItem * p;

  reasons_ |= key;
  if(reasons_ == 0)
	return;				// Nothing to do.

  if(!notifyEnabled())
	return;

	reason_ = reasons_;			// For callbackReason.
  // If reaon is DISCONNECT, calling the callback will result in the
  // conductor item being removed out from under us. The list is locked
  // so this won't happen.
  lockList();
	for (p=head; p; p=p->next())
	if (p->keyMatch(reason_))
	{	// Execute could result in removing p from list.
	p->ref();
		p->execute();
		
		p->unref();
	}
  unlockList();
  reason_ = NoReason;
  reasons_ = 0;
}

// Increment disable counter. Masks passed to invokeCallbacks will be
// saved until enableNotify reduces the count to 0 or an explicit
// invokeCallbacks is called.
void AvConductor::disableNotify()
{
	disableCount_ += 1;
}

// Decrement disableCount. If 0, callbacks will be called unless notify
// is false.
void AvConductor::enableNotify(const Boolean notify)
{	if(disableCount_ > 0)
		disableCount_ -= 1;
	if((disableCount_ == 0) && notify)
		invokeCallbacks(0);
}

// Clears callback mask generated by batched calls to invokeCallbacks.
// This should only be used if it is desired that the callback mask generated
// while notification was disabled be discarded rather than used after
// notification is reenabled.
void AvConductor::clearCallbackMask()
{
	reasons_ = 0;
}

// Scan the list removing any callback items that have been marked for removal.
void AvConductor::unlockList()
{
	listLocked_ = FALSE;
	while( (head != NULL) && head->markedForRemoval())
		removeNextItem(NULL);	// Remove head.

	AvConductorItem *ci = head, *next;
	while(ci != NULL)
	{	next = ci->next();
		if((next != NULL)&&next->markedForRemoval())
			removeNextItem(ci);
		else
			ci = next;
	}
}
