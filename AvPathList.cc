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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvPathList.cc,v 19.0 2003/07/16 05:47:49 aips2adm Exp $
//
// $Log: AvPathList.cc,v $
// Revision 19.0  2003/07/16 05:47:49  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:25  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:53  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:00  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:14  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:54  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:43  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:55  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:35  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:43  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:30:35  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:18:44  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:51:30  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.4  1996/06/18  18:46:24  pixton
//  Copyright Update
//
//  Revision 1.3  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/09/01  14:19:34  hr
//  Replaced '"' around system includes with '<,>'.
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvPathList.cc
When a pick is performed, a PathList is created containing a list of
pick paths. Each path is a list of objects from the toplevel viewport
to the last object reporting a hit.
*/
#include <AvPathList.h>
#include <stdio.h>
#include <string.h>

// # of lists to increase by if we have to lengthen lists_.
#define LISTSINCR	8

AvPathList::AvPathList(int len)
{
	lists_ = NULL;
	size_ = 0;
	count_ = 0;
	expandlists(len);	// Create the list of lists.
	newList();		// Make sure one exists.
}

// Lengthen the lists_ array by the given amount.
void AvPathList::expandlists(int n)
{
AvDisplayList **tmp;
int	newsize;

	if(n <= 0) n = LISTSINCR;
	newsize = size_ + n;
	tmp =  new (AvDisplayList*[newsize]);
	if( lists_ != NULL)
	{	memcpy(tmp, lists_, sizeof(*lists_)*count_);
		delete [] lists_;
	}
	lists_ = tmp;
	size_ = newsize;
}

AvPathList::~AvPathList()
{
	while(getLength() > 0)
		removeList();
	delete [] lists_;
}

// Append the object to the current list.
void AvPathList::append(AvGraphics2D *obj)
{
	current()->append(obj);
}

// Append the contents of the display list to the current list.
// If it is desired to append the display list itself, use append.
void AvPathList::appendDisplayList(AvDisplayList *dl)
{
AvDisplayList *l;
int	n;

	if((dl == NULL) || ( (n=dl->getLength()) <= 0))
		return;
	l = current();
	for(int i=0; i< n; i++)
		l->append(dl->getObject(i));
}

void AvPathList::remove()
{
	current()->pop();
}

void AvPathList::newList(AvDisplayList *list)
{
	if(count_ >= size_)
		expandlists();
	if(list == NULL)
		list = new AvDisplayList();
	list->ref();
	lists_[count_] = list;
	count_++;
}

void AvPathList::removeList()
{
	if(count_ > 0)
	{	current()->unref();
		count_--;
	}
}

AvDisplayList *AvPathList::getList(int i)
{
	if((i >= 0) && (i < getLength()))
		return lists_[i];
	else
		return NULL;
}

void printPath(AvDisplayList *l)
{
int	len;

	if(l == NULL)
	{	printf("NULL path\n");
		return;
	}
	len = l->getLength();
	printf("%s::%s %d items\n", l->getClassName(), l->getName(), len);
	for(int i=0; i< len; i++)
	{ AvGraphics2D *o;

		o = l->getObject(i);
		if(o == NULL)
			printf("  %d	NULL OBJECT\n", i);
		else
			printf("  %d	%s:%s\n",
				i, o->getClassName(), o->getName());
	}
}

void printPathList(AvPathList &p)
{
	printf("	%d paths\n", p.getLength());
	for(int i=0; i< p.getLength(); i++)
	{AvDisplayList *l = p.getList(i);
		if(l->getLength() > 0)
			printPath(l);
		else
			printf("Empty path\n");
	}
}
