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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvDisplayList.cc,v 19.0 2003/07/16 05:47:10 aips2adm Exp $
//
// $Log: AvDisplayList.cc,v $
// Revision 19.0  2003/07/16 05:47:10  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:52  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/03 21:21:21  hravlin
// Removed use of AvIVList macros.
//
// Revision 17.0  2001/11/12 19:42:21  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:29  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:06  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:56:55  hravlin
// Changed objname to be const char *.
//
// Revision 14.0  2000/03/23 16:08:13  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/08/25 19:54:19  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:40:00  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:39  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:46  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:37  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/09 21:20:28  hr
// Removed unneeded semicolon after DEFINE_MEMBER_INFO macro.
//
// Revision 9.0  1997/08/25 21:27:17  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:37  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1996/12/12 10:07:35  droberts
// Changed all references to IVList to AvIVList.
//
// Revision 1.2  1996/11/18 21:35:08  hr
// Renamed IVlist to AvIVlist.
//
// Revision 1.1  1996/07/11  21:31:21  pixton
// Automated Checkin
//
//  Revision 1.3  1996/06/18  18:45:58  pixton
//  Copyright Update
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

/* AvDisplayList.C
Holds a list of objects and displays them in first in, first out order.
Normally, AvViewportSetter would be used instead of this since it has
size and position information.

1/3/02
	Removed used of AvIVList macros since bizarre crashing seemed to
start happening. AvDisplayList now only supports adding and removing items
from the end of the list (push/pop).
*/
#include <string.h>
#include "AvDisplayList.h"
#include "AvCanvas.h"
#include "AvPathList.h"

DEFINE_MEMBER_INFO(AvDisplayList)

typedef AvGraphics2D* AvGraphics2DPTR;
static const unsigned int SIZE= 16;	// Size of list increment.

class AvDisplayListImpl {
  public:
	AvDisplayListImpl();
	~AvDisplayListImpl();
	void push(AvGraphics2D *); // Put an object on list.
	void pop();		   // Remove object from list.
	void remove_all();
	unsigned int count() { return count_;}
	void move_down(const unsigned int index);
	void expand();
	AvGraphics2D *getObject(unsigned int i);
	AvGraphics2DPTR	*list_;
	unsigned int	length_;	// Length of list array.
	unsigned int	count_;		// # of items in list.
					// (& index of next free slot).
};

AvDisplayListImpl::AvDisplayListImpl() : list_(NULL), length_(0), count_(0)
{
	expand();
}

AvDisplayListImpl::~AvDisplayListImpl()
{
	remove_all();
	delete list_;
}

void AvDisplayListImpl::push(AvGraphics2D *a)
{
	if(a != NULL)
	{	if(count_ >= length_)
			expand();
		a->ref();
		list_[count_++] = a;
	}
}

void AvDisplayListImpl::pop()
{
	if(count_ > 0)
	{AvGraphics2D *a = list_[--count_];
		list_[count_] = NULL;
		a->unref();
	}
}

// Remove the contents of the list.
void AvDisplayListImpl::remove_all()
{
	for(unsigned int i=0; i< count_; i++)
	{AvGraphics2D *a = list_[i];
		list_[i] = NULL;
		a->unref();
	}
	count_ = 0;
}

// Return a pointer to the i'th object.
AvGraphics2D *AvDisplayListImpl::getObject(unsigned int i)
{
	if(i < count())
		return list_[i];
	else
		return NULL;
}


void AvDisplayListImpl::expand()
{ unsigned int len = length_ + SIZE;

	AvGraphics2DPTR *list = new AvGraphics2DPTR[len];
	if(count_ > 0)
	{	for(unsigned int i=0; i< count_; i++)
			list[i] = list_[i];
	}
	if(list_ != NULL)
		delete [] list_;
	list_ = list;
	length_ = len;
}

////////////////////////////////////////////////////////////////

AvDisplayList::AvDisplayList(
	AvGraphics2D* a, AvGraphics2D* b, AvGraphics2D* c,
	AvGraphics2D* d, AvGraphics2D* e, AvGraphics2D* f,
	AvGraphics2D* g, AvGraphics2D* h, AvGraphics2D* i,
	AvGraphics2D* j)
{
	INIT_CLASS(AvGraphics2D);
	impl_ = new AvDisplayListImpl;

	impl_->push(a);
	impl_->push(b);
	impl_->push(c);
	impl_->push(d);
	impl_->push(e);
	impl_->push(f);
	impl_->push(g);
	impl_->push(h);
	impl_->push(i);
	impl_->push(j);

	mode_ = ALL;
	currentobject_ = 0;
	goingup_ = TRUE;	// Ignored if mode is ALL.
}

AvDisplayList::~AvDisplayList()
{
	delete impl_;
}

void AvDisplayList::removeList()
{
	impl_->remove_all();
}


void AvDisplayList::append(AvGraphics2D *obj)
{
	if(obj != NULL)
	{	impl_->push(obj);
		touch();
	}
}

// # of items on list.
int AvDisplayList::getLength()const { return impl_->count();}

#if 0
// Remove the i'th entry.
void AvDisplayList::removeIndex(const int i)
{
	impl_->removeIndex(i);
	touch();
}
#endif

#if 0
// Replace the i'th entry.
void AvDisplayList::replaceIndex(const int i, AvGraphics2D *obj)
{
	if(obj != NULL)
	{	impl_->replaceIndex(i, obj);
		touch();
	}
}
#endif

AvGraphics2D *AvDisplayList::getObject(const int i)const
{
	return impl_->getObject(i);
}

// Return a pointer to the i'th object from end of list
AvGraphics2D *AvDisplayList::getObjectFromEnd(const int i)const
{
	return impl_->getObject(impl_->count()-i-1);
}

void AvDisplayList::push(AvGraphics2D *obj)
{
	if(obj != NULL)
	{	impl_->push(obj);
		touch();
	}
}

void AvDisplayList::pop()
{
	impl_->pop();
	touch();
}

/* Draw the display list. If display mode is ALL, everything in the list
not set to ignore is drawn. For other modes, only the next item is drawn.

In ALL mode, reset current object to 0 when done. It needs to be
valid before the next draw call since getBoundingBox is called first.
*/
void AvDisplayList::draw(AvCanvas &d)
{
int nobjects;
AvGraphics2D *obj;

	if(ignore()) return;	// Ignore me?

	nobjects = numObjects();	// DL length if ALL, else 1.
	for(int i=0; i < nobjects; i++)
	{	obj = currentObject();
		if((obj != NULL) && (! obj->ignore()))
			obj->draw(d);
		if(mode_ == ALL)
			nextIndex();
	}

	if(mode_ == ALL)
		setCurrentIndex(0);	// Reset.
}

void AvDisplayList::draw(AvCanvas *d)
{
	if(d != NULL)
		draw(*d);
}

// Compute bounding box for list.
void AvDisplayList::getBoundingBox(AvCanvas &c, AvAllotment &r)
{
AvAllotment req;	// Is initialized to 0.

	if(ignore())
	{	r = AvAllotment::zero();	// set to empty;
		return;
	}

	// If mode is all, get bounding box for everything, otherwise
	// only for currentObject.
	if(mode_ == ALL)
	{	for(unsigned int i=0; i< impl_->count(); i++)
		{ AvGraphics2D *obj = impl_->getObject(i);
			if((obj != NULL) && (! obj->ignore()))
			{	req = AvAllotment::zero();	// reset.
				obj->getBoundingBox(c, req);
				r.maximum(req);
			}
		}
	}
	else
	{ AvGraphics2D *obj = currentObject();
		if((obj != NULL) && (! obj->ignore()))
		{	r = AvAllotment::zero();	// reset.
			obj->getBoundingBox(c, r);
		}
		else
			r = AvAllotment::zero();	// set to empty;
	}
}

// Call each object until one handles the event.
void AvDisplayList::handleEvent(AvCanvas &c, AvEvent *e)
{
	if(e == NULL) return;
	if(ignore()) return;

	if(mode_ == ALL)
	{	for(unsigned int i=0; i< impl_->count(); i++)
		{ AvGraphics2D *obj = impl_->getObject(i);
			if((obj != NULL) && (! obj->ignore()))
			{	obj->handleEvent(c,e);
				// Quit when the event is dealt with.
				if( e->isHandled())
					break;
			}
		}
	}
	else
	{ AvGraphics2D *obj = currentObject();
		if((obj != NULL) && (! obj->ignore()))
			obj->handleEvent(c,e);
	}		
}

// build list of paths to hit objects.
void AvDisplayList::getPickPaths(AvCanvas &c, const AvAllotment &a,
							AvPathList &pl)
{
int		len;
AvDisplayList	*orig;
int		origlen;
int		numadded = 0;

	orig = pl.current();
	origlen = pl.getLength();

	if(!ignore() && (len = getLength()) > 0)
	{	// Build local path lists, one for each hit item.
		for(int i = 0; i < len; i++)
		{ AvGraphics2D *obj = getObject(i);
		  AvPathList	*lpl;		// Local pathlist.
		  int lplen;

			if((obj != NULL) && (! obj->ignore()))
			{	lpl = new AvPathList();
				obj->getPickPaths(c, a, *lpl);
				lplen = lpl->getLength();
				for(int k=0; k < lplen; k++)
				{AvDisplayList *ldl;
					// Ignore any empty paths.
					if(((ldl = lpl->getList(k)) == NULL) ||
						(ldl->getLength() <= 0))
						continue;
					/* If we've already added a path,
					   need to create a new one.
					*/
					if(numadded > 0) // Replicate original.
					 pl.newList(orig->copyList(0, origlen));
					pl.appendDisplayList(ldl);
					numadded += 1;
				}
				delete lpl;
			}
		}
	}
}


/* Return a (sub ranged) copy of this list. If list is NULL a new
DisplayList will be created, otherwise elements will be appended to
it.

Note:
	Only the pointers to the objects are copied, new versions of
the objects are not made.

*/
AvDisplayList *AvDisplayList::copyList(const int start, const int n,
			AvDisplayList *lst)const
{
int	len, maxn, num;
AvDisplayList *list;

	list = (AvDisplayList *)lst;
	num = n;
	len = getLength();
	if((start < 0) || (start > len -1))
		return NULL;

	maxn = len-start;
	if((num <= 0) || (num > maxn))
		num = maxn;

	if(list == NULL)
		list = new AvDisplayList();


	for(int i=0; i< num; i++)
		list->push(getObject(i+start));

	return list;
}

Boolean AvDisplayList::equals(const AvDisplayList *list, const int l)const
{
int	len = l;

	if(list == NULL)
		return FALSE;

	if(len <= 0)
		len = getLength();
	else
	if( len > getLength())
		return FALSE;	// This list is too short.

	if(len > list->getLength())
		return FALSE;	// That list is too short.

	for(int i=0; i< len; i++)
		if(getObject(i) != list->getObject(i))
			return FALSE;

	return TRUE;
}

AvGraphics2D *AvDisplayList::searchByName_(const char *name, const int n,
	const Boolean useclassname, const Boolean fromend)const
{
int	listlen, num;
AvGraphics2D *obj;
const char *objname;

	if(name == NULL)		// Nothing to look for.
		return NULL;

	num = n;

	listlen = getLength();
	for(int i=0; i< listlen; i++)
	{	obj = (fromend) ? getObjectFromEnd(i) : getObject(i);
		if(obj == NULL) continue;	// This shouldn't happen.
		objname = (useclassname) ? obj->getClassName() :obj->getName();
		if( strcmp(objname, name) == 0)
		  if( --num < 0)
			return obj;
	}
	return NULL;
}

AvGraphics2D *AvDisplayList::searchByName(const char *name, const int n)const
{
	return searchByName_(name, n, FALSE, FALSE);
}

AvGraphics2D *AvDisplayList::searchByNameFromEnd(const char *name,
							const int n)const
{
	return searchByName_(name, n, FALSE, TRUE);
}

AvGraphics2D *AvDisplayList::searchByClassName(const char *name,
							const int n)const
{
	return searchByName_(name, n, TRUE, FALSE);
}

AvGraphics2D *AvDisplayList::searchByClassNameFromEnd(const char *name,
		const int n)const
{
	return searchByName_(name, n, TRUE, TRUE);
}

/* Search display list for an object of type id
Search can start at either end. If checkParents is TRUE the match succeeds
if the object is derived from id.
*/
AvGraphics2D *AvDisplayList::searchByID_(const AvTypeID id, const int n,
	const Boolean checkParents, const Boolean fromend)const
{
int	listlen, num;
AvGraphics2D *obj;

	num = n;

	listlen = getLength();
	for(int i=0; i< listlen; i++)
	{	obj = (fromend) ? getObjectFromEnd(i) : getObject(i);
		if(obj == NULL) continue;	// This shouldn't happen.
		if( (checkParents && obj->isDerivedFrom(id)) ||
		    (!checkParents && (id == obj->getTypeID())))
		  	if( --num < 0)
				return obj;
	}
	return NULL;
}

AvGraphics2D *AvDisplayList::searchByParentID(const AvTypeID id,
							const int n)const
{
	return searchByID_(id, n, FALSE, FALSE);
}

AvGraphics2D *AvDisplayList::searchByIDFromEnd(const AvTypeID id,
							const int n)const
{
	return searchByID_(id, n, FALSE, TRUE);
}

AvGraphics2D *AvDisplayList::searchByID(const AvTypeID id,
							const int n)const
{
	return searchByID_(id, n, TRUE, FALSE);
}

AvGraphics2D *AvDisplayList::searchByParentIDFromEnd(const AvTypeID id,
							const int n)const
{
	return searchByID_(id, n, TRUE, TRUE);
}

void AvDisplayList::setDisplayMode(const DisplayMode m)
{
	switch (m) {
	case ALL:
	case UP:
	case DOWN:
	case REVERSE:
	case WRAP:
		mode_ = m;
		setCurrentIndex(0);
		goingup_ = (m != DOWN);
		break;
	default:;
	}
}

AvDisplayList::DisplayMode AvDisplayList::getDisplayMode()const {return mode_;}

/* Increment the currentObject index. If it is at the end, decide what to
do based on the current display mode.
*/
void AvDisplayList::nextIndex()
{
int cf, len, delta = 1;

	{	len = getLength();
		cf = currentobject_ + delta;
		if(cf >= len)
			switch( mode_ ) {
			case ALL:
				cf = -1;	// Done.
				break;
			case UP:
			case DOWN:
				cf = currentobject_;	// Stop at end
				break;
			case REVERSE:
				goingup_ = ! goingup_;
				cf = 0;
				break;
			case WRAP:
				cf = 0;
				break;
			default:;
			}
		currentobject_ = cf;
	}
}

/* Decrement the currentObject index. If it is at the end, decide what to
do based on the current display mode.
*/
void AvDisplayList::previousIndex()
{
int cf, len, delta = 1;

//	if(delta < 0)
//		nextIndex(-delta);
//	else
	{	len = getLength();
		cf = currentobject_ - delta;
		if(cf < 0)
			switch( mode_ ) {
			case ALL:
				cf = -1;
				break;
			case UP:
			case DOWN:
				cf = currentobject_;	// Stop at end
				break;
			case REVERSE:
				goingup_ = ! goingup_;
				cf = 0;
				break;
			case WRAP:
				cf = len-1;
				break;
			default:;
			}
		currentobject_ = cf;
	}
}

/* Return object index of current object (to be displayed).
If currentobject_ is negative, assume "EOF" condition.
*/
int AvDisplayList::getCurrentIndex()const
{
	if(currentobject_ < 0)		// Invalid/EOF flag.
		return currentobject_;

	if(goingup_)
		return currentobject_;
	else
		return getLength() - currentobject_ -1;
}

// Set the current object to display to be cf. CF is the index from the
// beginning of the list.
void AvDisplayList::setCurrentIndex(const int cf)
{
	if( (cf >= 0) && (cf < getLength()))
	{  if(goingup_)
		currentobject_ = cf;
	   else
		currentobject_ = getLength() - cf -1;
	}
}

// Returns object referenced by currentObject() or NULL.
AvGraphics2D *AvDisplayList::currentObject()const
{
	return getObject(getCurrentIndex());
}

// If mode is ALL, return display list length, else 1 (unless list is empty).
int AvDisplayList::numObjects()const
{
int len = getLength();

	return (mode_ == ALL) ? len : (len > 0) ? 1 : 0;
}

#if 0
// Set all entries in the display list to be (not) ignored.
// We should really keep track of this separately rather than poking the
// objects directly.
void AvDisplayList::setAllIgnore(const Boolean ignore)
{
int	i, len;
AvGraphics2D *o;

	len = getLength();
	for(i = 0; i < len; i++)
		if( (o = getObject(i)) != NULL)
			o->ignore(ignore);
}
#endif
