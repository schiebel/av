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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvTimeStamp.h,v 19.0 2003/07/16 05:46:45 aips2adm Exp $
//
// $Log: AvTimeStamp.h,v $
// Revision 19.0  2003/07/16 05:46:45  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:31  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:00  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:09  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:41  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:07:46  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:30  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:46  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:31  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:09  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:24:58  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:14  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 07:33:02  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.5  1996/06/18  18:43:23  pixton
//  Copyright Update
//
//  Revision 1.4  1996/02/23  17:33:48  hr
//  Removed use of any system time routines.
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
//  Revision 1.2  1995/04/04  16:02:51  hr
//  Changed time stamp generation method.
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

 
/* AvTimeStamp.h
Several objects use last modified time rather than a flag to indicate
they have been changed. TimeStamp is used to facilitate timestamps.

TimeStamp does not use actual times. Rather, each "touch" increments an
internal counter.
*/

#ifndef AvTimeStamp_H
#define AvTimeStamp_H
#include <X11/Intrinsic.h>	// Defines Boolean, TRUE, FALSE

typedef unsigned long TimeStamp;

class AvTimeStamp {
  public:
	AvTimeStamp();
	void touch();	// Flag as having been changed.
	// Has this object been modified since another was modified?
	Boolean	modifiedSince(const TimeStamp time)const;
	Boolean	modifiedSince(const AvTimeStamp &)const;
	// When was this last modified.
	inline TimeStamp modifiedTime() const {return lastModifiedTime_;}
  private:
	TimeStamp lastModifiedTime_;
};
#endif
