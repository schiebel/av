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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvGraphics2D.h,v 19.0 2003/07/16 05:47:18 aips2adm Exp $
//
// $Log: AvGraphics2D.h,v $
// Revision 19.0  2003/07/16 05:47:18  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:00  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:29  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:36  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:38  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:52:09  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:08:22  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:10  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:58  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:11  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:06  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/09 21:15:53  hr
// Fixed up unneeded semicolon warnings in DECLARE_xxx macros.
//
// Revision 9.0  1997/08/25 21:28:05  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:17:09  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:27:54  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.3  1996/06/18  18:42:55  pixton
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

/* AvGraphics2D.h
Base class for display objects.

All classes subclassed either directly or indirectly from AvGraphics2D
need to have some particular variables and functions defined. Here is a
boiler plate to make things easier:

class MyClass : public <parent> {
 ...
 // This declares some public functions and some private variables.
 // It can go anywhere in the class definition.
 // Afterwards the header is in 'private' mode.
 DECLARE_HEADER;
};

// Put these in the class source file:
// Near the beginning of the file.
DEFINE_MEMBER_INFO(MyClass);

MyClass::MyClass(...)
{
	// Inside the constructor(s);
	INIT_CLASS(<parent>);
}



Several files have lines like "#ifdef OLDCC"
These are for old old cfront compilers, specifically SunOS SC1.0.
While the routines should compile if OLDCC is defined, thorough testing
has not been done.
*/

#ifndef AvGraphics2D_H
#define AvGraphics2D_H
#include <X11/Intrinsic.h>	// Defines Boolean, TRUE, FALSE
#include "AvResource.h"
#include "AvGeometry.h"
#include "AvTimeStamp.h"
#include <sys/types.h>

class AvCanvas;
class AvEvent;
class AvPathList;

#ifndef NULL
#define NULL 0
#endif

typedef	unsigned int AvTypeID;

/*	****************************************************************
	*****************************************************************
	*****************************************************************
	All classes subclassed from AvGraphics2D need some common boiler plate.
These are some macros to help.
*/

//			These go into the header file
//	Public member functions
#define DECLARE_HEADER_FUNCS					\
	virtual const char *getClassName()const;		\
	static const char *className();				\
	static AvTypeID getClassTypeID(){return classTypeID_;}
//	Private variables
// classTypeID	- The generic class has this id.
// typeID	- class id for this object.
#define DECLARE_HEADER_VARS					\
	static	AvTypeID classTypeID_;				\
	static	const char* Name;

// This calls the above.
#define	DECLARE_HEADER						\
  public:							\
	DECLARE_HEADER_FUNCS					\
  private:							\
	DECLARE_HEADER_VARS

//			These go into the source file.
//	Static variables for the Class.
#ifdef OLDCC
#define DEFINE_STATIC_VARS(className)				\
	const char *className::Name = "className";		\
	AvTypeID className::classTypeID_ = 0		// id for this class.
#else
#define DEFINE_STATIC_VARS(className)				\
	const char *className::Name = #className;		\
	AvTypeID className::classTypeID_ = 0;		// id for this class.
#endif

//	Public member functions.
#define	DEFINE_MEMBER_FUNCS(classname)				\
  const char *classname::getClassName()const{ return classname::Name;}	\
  const char *classname::className(){ return classname::Name;}

// Calls the above two.
#define	DEFINE_MEMBER_INFO(classname)				\
	DEFINE_STATIC_VARS(classname)				\
	DEFINE_MEMBER_FUNCS(classname)

//	This goes into the constructor(s).
#ifdef OLDCC
#define	INIT_CLASS(parent)					\
	if(classTypeID_ == 0)					\
	 classTypeID_ = createTypeID(AvGraphics2D::getIDfromName("parent"),\
						className());	\
	 typeID_ = classTypeID_
#else
#define	INIT_CLASS(parent)					\
	if(classTypeID_ == 0)					\
	 classTypeID_ = createTypeID(AvGraphics2D::getIDfromName(#parent),\
						className());	\
	 typeID_ = classTypeID_
#endif

/*	****************************************************************
	*****************************************************************
	*****************************************************************
*/

class  AvGraphics2D : public AvTimeStamp , public AvResource {
  public:
	// The Canvas calls draw with a pointer to itself to tell the object
	// to display itself.
	virtual void draw(AvCanvas &);
	virtual void draw(AvCanvas *);

	/* Return the desired size in pixels.
	   Returned sizes are from '0'. ie. an object at position 10 that was
	   20 long would return 30.
	*/
	virtual void getBoundingBox(AvCanvas &c, AvAllotment &r);
	// If this is set, this object won't be drawn. A way of temporarily
	// turning something off w/o deleting it from a display list.
	void ignore(const Boolean);
	inline Boolean ignore()const{return ignore_;}
	// Is the object pickable?
	void pickable(const Boolean);
	inline Boolean pickable()const{return pickable_;}
	// Called when event is received.
	virtual void handleEvent(AvCanvas &c, AvEvent *);
	// Returns TRUE if the point intersects this object.
	virtual Boolean isHit(AvCanvas &c, const AvAllotment &)const;
	// build list of paths to hit objects.
	virtual void getPickPaths(AvCanvas &, const AvAllotment &,
								AvPathList &);
	// Name of this object
	const char *getName() const;
	void name(const char *newname);

	AvTypeID getTypeID()const {return typeID_;}
	int getSerial()const {return serial_;}	// Unique to instance.
	static AvTypeID getIDfromName(const char *name);
	static char *getNameFromID(const AvTypeID id);
	AvTypeID getParentID()const;
	Boolean	isDerivedFrom(const AvTypeID id)const;
	// Every class should define these.
	//  virtual const char *getClassName() returns name of this class.
	//  static const char *className() returns generic class name.
	DECLARE_HEADER_FUNCS;
  protected:
	AvGraphics2D();		// Base class.
	~AvGraphics2D();	// Use unref() to get rid of an instance.
	static	AvTypeID createTypeID(const AvTypeID parentID,
							const char *name);
	AvTypeID typeID_;	// This object is of class.
  private:
	Boolean	ignore_;
	Boolean pickable_;
	char	*name_;
	int	serial_;	// Unique to this instance.
	static	int nextserial_;
	DECLARE_HEADER_VARS
};

#endif
