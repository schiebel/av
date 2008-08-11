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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvResource.h,v 19.0 2003/07/16 05:47:46 aips2adm Exp $
//
// $Log: AvResource.h,v $
// Revision 19.0  2003/07/16 05:47:46  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:22  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/30 22:44:12  hravlin
// Copyright
//
// Revision 17.0  2001/11/12 19:42:51  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:58  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:07  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:52  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:41  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:50  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:28  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:34  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:30:22  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:18:37  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 07:09:05  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.4  1996/06/18  18:43:19  pixton
//  Copyright Update
//
//  Revision 1.3  1996/03/28  22:30:46  hr
//  Added lockResource(), unlockResource(), resourceLocked(), UnRef() and Ref().
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

 
/* AvResource.h
This class is used to implement resource counting.
Other classes should be a subclass of this.

Notes:
	Always use new to create an instance.
	Never explicitly use delete.
	use ref() to increment the reference count.
	use unref() to decrement the reference count.

*/
#ifndef AvResource_h
#define AvResource_h

class AvResource {
  public:
	void ref();
	void unref();
	void unrefNoDelete();
	unsigned int getReferenceCount();
	// If the resource is locked, it will complain if the reference
	// count goes to 0. (Debugging).
	void lockResource();
	void unlockResource();
	inline int resourceLocked(){return locked_;}
	static void UnRef(AvResource *r);
	static void Ref(AvResource *r);
  protected:
	AvResource();			// Base class only.
	virtual ~AvResource();
  private:
	unsigned int referenceCount_;
	int	locked_;
};
#endif
