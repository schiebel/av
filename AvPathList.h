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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvPathList.h,v 19.0 2003/07/16 05:47:25 aips2adm Exp $
//
// $Log: AvPathList.h,v $
// Revision 19.0  2003/07/16 05:47:25  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:05  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:34  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:41  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:00  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:30  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:17  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:10  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:30  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:27  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:28:40  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:17:30  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:51:52  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.3  1996/06/18  18:43:11  pixton
//  Copyright Update
//
//  Revision 1.2  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

 
/* AvPathList.h
When a pick is performed, a PathList is created containing a list of
pick paths. Each path is a list of objects from the toplevel viewport
to the last object reporting a hit.

*/
#ifndef AvPathList_h
#define AvPathList_h

#include "AvDisplayList.h"

class AvPathList : public AvResource {
  public:
	AvPathList(int len=0);
	~AvPathList();
	void append(AvGraphics2D*);	// Append an object to current path.
	/* Append the contents of the display list to the current list.
	   If it is desired to append the display list itself, use append.
	*/
	void appendDisplayList(AvDisplayList *dl);
	void remove();			// Remove an object from current path.
	void newList(AvDisplayList * = NULL);	// Start a new list.
	void removeList();		// Remove current list.
	inline int getLength(){return count_;}	// # of lists.
	AvDisplayList *getList(int i);	// Get a pointer to list i.
	AvDisplayList	*current(){return lists_[count_-1];}
  private:
	int	count_;			// # of lists.
	int	size_;			// length of lists_.
	AvDisplayList **lists_;		// list of lists.
	void	expandlists(int =0);
};
// For testing
void printPathList(AvPathList &p);
void printPath(AvDisplayList *l);
#endif
