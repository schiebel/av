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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvDisplayList.h,v 19.0 2003/07/16 05:46:58 aips2adm Exp $
//
// $Log: AvDisplayList.h,v $
// Revision 19.0  2003/07/16 05:46:58  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:42  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/03 21:21:43  hravlin
// Removed removeIndex() and replaceIndex().
//
// Revision 17.0  2001/11/12 19:42:11  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:19  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:25  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:01  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:46  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:14  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:11  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:56  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/09 21:13:37  hr
// Removed unneeded semicolon from DECLARE_HEADER.
//
// Revision 9.0  1997/08/25 21:26:12  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:58  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:09:58  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.3  1996/06/18  18:42:47  pixton
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

/* AvDisplayList2D.h
Base class for display objects.
Implements a list of objects. Normally the entire list is drawn when
draw() is called, however, it is possible to set the display mode to
choose one object from the list.

The display list does not explicitly animate. To animate, it is placed
in 'single step' mode and nextObject() is called on every time step.

To animate, setDisplayMode with one of: ALL, UP, DOWN, REVERSE, WRAP.
	ALL	Entire display list will be shown (default).
	UP	Objects will be displayed from 0 to max.
	DOWN	Objects will be displayed from max to 0.
	REVERSE	Objects will be displayed from 0 to max, max to 0, ...
	WRAP	Objects will be displayed from 0 to max, 0 to max...
In modes other than ALL only the currently selected object will be displayed.
To display the next object, call nextObject() and redisplay the list.
If the selected object is set to 'ignore', nothing will be drawn.
*/

#ifndef AvDisplayList2D_H
#define AvDisplayList2D_H

#include "AvGraphics2D.h"

// List of Display objects. Interface is subject to change.
class AvDisplayListImpl;

class AvDisplayList : public AvGraphics2D {
  public:
	AvDisplayList(
	 AvGraphics2D* =NULL,AvGraphics2D* =NULL,AvGraphics2D* =NULL,
	 AvGraphics2D* =NULL,AvGraphics2D* =NULL,AvGraphics2D* =NULL,
	 AvGraphics2D* =NULL,AvGraphics2D* =NULL,AvGraphics2D* =NULL,
	 AvGraphics2D * =NULL);
	//////////////////////////////////////////////////
	//		List manipulation		//
	//////////////////////////////////////////////////
	// Append the object to the list. The object is ref'd.
	void append(AvGraphics2D *);
	// The current display list is removed. The Graphics objects
	// are unrefed.
	void removeList();
	// # of items on list.
	int	getLength()const;
	// Remove the i'th entry.
	//	void removeIndex(const int i);
	// Replace the i'th object. Ignored if obj is NULL.
	//	void replaceIndex(const int i, AvGraphics2D *obj);
	// Return a pointer to the i'th object.
	AvGraphics2D *getObject(const int i)const;
	// above, but from the end of the list
	AvGraphics2D *getObjectFromEnd(const int i)const;
	// Make a copy of the list.
	AvDisplayList *copyList(const int start=0, const int num=0,
				AvDisplayList * =NULL)const;
	void push(AvGraphics2D *);	// Same as append
	void pop();			// remove last.
	/* See if two lists are the same within the first 'len' items. If len
	   is 0, checks the entire list. Only checks items in the immediate
	   list, does not look into any sublists. Returns FALSE if either list
	   isn't at least as long as the number of items to be checked.
	*/
	Boolean equals(const AvDisplayList *, const int len=0)const;
	// Look for the nth occurance of an object with the name.
	// (0 - first, 1 next, etc).
	AvGraphics2D *searchByName(const char *, const int n=0)const;
	AvGraphics2D *searchByNameFromEnd(const char *, const int n=0)const;
	AvGraphics2D *searchByClassName(const char *, const int n=0)const;
	AvGraphics2D *searchByClassNameFromEnd(const char *,
							const int n=0)const;
	// Search for an object with the given id.
	AvGraphics2D *searchByID(const AvTypeID, const int n=0)const;
	AvGraphics2D *searchByIDFromEnd(const AvTypeID, const int n=0)const;
	// Search for an object derived from the given id.
	AvGraphics2D *searchByParentID(const AvTypeID, const int n=0)const;
	AvGraphics2D *searchByParentIDFromEnd(const AvTypeID,
							const int n=0)const;
	//////////////////////////////////////////////////
	virtual void draw(AvCanvas &);
	virtual void draw(AvCanvas *);
	virtual void getBoundingBox(AvCanvas &c, AvAllotment &r);
	virtual void handleEvent(AvCanvas &c, AvEvent *);
	// build list of paths to hit objects.
	virtual void getPickPaths(AvCanvas &c, const AvAllotment &,
							AvPathList &);
	/* Animation support. ALL means show everything (no animation).
	   The others indicate how animation is done.
	*/
	enum DisplayMode { ALL, UP, DOWN, REVERSE, WRAP};
	void setDisplayMode(const DisplayMode m);
	DisplayMode getDisplayMode()const;
	// Select the next Object for display. (Not used if mode is ALL).
	void nextIndex();
	// Previous object.
	void previousIndex();
	// Index of current object or -1 if no more (ALL mode).
	// Not necessarily the object index and so isn't used other
	// than internally.
	int getCurrentIndex()const;
	// Sets next object to display when mode isn't ALL. 0 is 'first' object
	// on the list. (Regardless of current direction (up or down).
	void setCurrentIndex(const int cf);
	// Returns the object to display or NULL. It can be NULL if in ALL
	// mode and the list has been completed.
	AvGraphics2D *currentObject()const;
	// Returns the length of list if mode is ALL, otherwise 1.
	int	numObjects()const;
  protected:
	~AvDisplayList();		// Use unref().
  private:
	AvDisplayListImpl *impl_;
	DisplayMode	mode_;
	// This the index of the current display object only if direction
	// is UP. If DOWN, the index is getLength()-currentobject-1.
	int		currentobject_;
	Boolean		goingup_;
	AvGraphics2D *searchByName_(const char *, const int n,
	  const Boolean useclassname,const Boolean fromend)const;
	AvGraphics2D *searchByID_(const AvTypeID id, const int n,
		const Boolean checkParents, const Boolean fromend)const;

	DECLARE_HEADER
};
#endif
