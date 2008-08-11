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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvGraphics2D.cc,v 19.0 2003/07/16 05:47:27 aips2adm Exp $
//
// $Log: AvGraphics2D.cc,v $
// Revision 19.0  2003/07/16 05:47:27  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:07  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:37  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:43  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:08  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:52:00  hravlin
// Changed some char * to const char *.
//
// Revision 14.0  2000/03/23 16:08:33  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/08/25 19:51:20  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:40:20  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:16  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:37  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:36  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.2  1998/06/09 21:27:37  hr
// Removed unneeded semicolon from DEFINE_xxx macros.
// Fixed some signed/unsigned/long, etc. type conversion warnings.
//
// Revision 9.1  1997/11/07 18:57:19  bglenden
// Replace a call with a call to new and an explicit loop since it is deleted
// with delete later.
//
// Revision 9.0  1997/08/25 21:28:51  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:17:37  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.4  1997/02/09 00:42:41  hr
// A couple of signed/unsigned comparisions.
//
// Revision 7.3  1996/12/12 09:00:15  droberts
// Changed all references to IVList to AvIVList.
//
// Revision 1.2  1996/11/18 21:35:08  hr
// Renamed IVlist to AvIVlist.
//
// Revision 1.1  1996/07/11  21:31:21  pixton
// Automated Checkin
//
//  Revision 1.3  1996/06/18  18:46:04  pixton
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

/* AvGraphics2D.C
Base class for display objects.
*/
#include <stdio.h>

#include <AvGraphics2D.h>
#include <AvPathList.h>
#include <AvIVList.h>
#include <string.h>

void AvGraphics2D::draw(AvCanvas &){}
void AvGraphics2D::draw(AvCanvas *){}

//static char *Name = "AvGraphics2D";
DEFINE_STATIC_VARS(AvGraphics2D)
int AvGraphics2D::nextserial_ = 1;

static const char *NoName = "";

class AvType;
declarePtrList(TypeList, AvType)
implementPtrList(TypeList, AvType)
static TypeList	*idlist=NULL;

class AvType {
  friend class AvGraphics2D;
  public:
	AvType();
	~AvType();
	AvTypeID getID()const { return id_;}
	AvTypeID parentID()const { return parent_;}
	char *getName()const { return name_;}
	static unsigned int createID(const unsigned int parent,
							const char *name);
	static AvType *getEntry(AvTypeID id) { return idlist->item((long)id);}
  private:
	AvTypeID	id_;
	AvTypeID	parent_;
	char	*name_;
	static	void init();
};


AvType::AvType()
{
	id_ = 0;
	parent_ = 0;
	name_ = NULL;
}

unsigned int AvType::createID(const unsigned int parent, const char *name)
{
AvType	*t;
unsigned int id;

	AvType::init();
	t = new AvType();
	t->parent_ = parent;
	if(name == NULL)
	{	t->name_ = new char[strlen(NoName)+1];
		strcpy(t->name_, NoName);
	}
	else
	{	t->name_ = new char[strlen(name)+1];
		strcpy(t->name_, name);
	}
	idlist->append(t);
	id = (AvTypeID) idlist->count()-1;
	t->id_ = id;
#if 0
{static int once=0;
	if(once++ == 0)
		printf("ID	ParentID	Name\n");
}
printf("%2d	%d	%s\n", id, t->parent_, t->getName());
#endif
	return id;
}

AvType::~AvType()
{
	if(name_ != NULL)
		delete [] name_;
}

void AvType::init()
{
	if(idlist == NULL)
	{	idlist = new TypeList(30);
		// This had better be in index 0.
		AvType::createID(0, "NoParent");
		// createID will call init, but since idlist is no longer NULL,
		// Nothing bad will happen.
	}
}

/*****************************************************************/
/*****************************************************************/


AvGraphics2D::AvGraphics2D()
{
	ignore_ = FALSE;
	pickable_ = TRUE;
	name_ = NULL;
	serial_ = nextserial_++;
	AvType::init();			// Initialize AvType.
	INIT_CLASS(NoParent);
}

AvGraphics2D::~AvGraphics2D()
{
	name(NULL);
}

void AvGraphics2D::ignore(const Boolean ignore)
{
	ignore_ = ignore;
}

void AvGraphics2D::pickable(const Boolean pickable)
{
	pickable_ = pickable;
}

void AvGraphics2D::getBoundingBox(AvCanvas &, AvAllotment &r)
{
	r.value(AvDimensionX, 0.);
	r.value(AvDimensionY, 0.);
}

// Name of this instance.
const char *AvGraphics2D::getName() const
{
	return (name_ == NULL) ? NoName : name_;
}

// Set name. If name is NULL remove any current name.
void AvGraphics2D::name(const char *name)
{
	if(name_ != NULL)	// Remove any old.
	{	delete [] name_;
		name_ = NULL;
	}
	if(name != NULL) {
	        int len = (int)strlen(name) + 1; // +1 for trailing \0
		// The following will throw exception on failure at some point
		// when compilers are standard conforming.
		name_ = new char[len];
		for (int i=0; i<len; i++) {
		    name_[i] = name[i];
		}
	} else {
		name_ = NULL;
	}
}

DEFINE_MEMBER_FUNCS(AvGraphics2D)

#if 0
char *AvGraphics2D::className()
{
	return Name;
}
#endif

void AvGraphics2D::handleEvent(AvCanvas &, AvEvent *)
{
}

Boolean AvGraphics2D::isHit(AvCanvas &, const AvAllotment &)const
{
	return FALSE;
}

void AvGraphics2D::getPickPaths(AvCanvas &c, const AvAllotment &a,
							AvPathList &pl)
{
	if(isHit(c, a))
	{	pl.append(this);
	}
}

AvTypeID AvGraphics2D::getIDfromName(const char *name)
{
unsigned int len = (unsigned int) idlist->count();
unsigned int i;

	if(name == NULL)
		return 0;

	for(i = 1; i< len; i++)
	{ AvType *t = idlist->item((long)i);
		if((t != NULL) && (strcmp(t->getName(), name) == 0))
			return i;
	}
	return 0;
}

char *AvGraphics2D::getNameFromID(const AvTypeID id)
{
	if((id > 0) && (id < (unsigned int)idlist->count()))
		return idlist->item((long)id)->getName();
	else
		return NULL;
}


AvTypeID AvGraphics2D::createTypeID(const AvTypeID parentID,
							const char *name)
{
	return AvType::createID(parentID, name);	
}

AvTypeID AvGraphics2D::getParentID()const{

	return AvType::getEntry(getTypeID())->parentID();
}

// See if the object is derived from the given id.
// If the object is of type id, that is also considered a match.
Boolean	AvGraphics2D::isDerivedFrom(const AvTypeID idtst)const
{
AvTypeID id;

	id = getTypeID();

	while(id != idtst)
		if( (id = AvType::getEntry(id)->parentID()) == 0)
			return FALSE;

	return TRUE;
}
