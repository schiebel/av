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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvFormComp.h,v 19.0 2003/07/16 05:47:10 aips2adm Exp $
//
// $Log: AvFormComp.h,v $
// Revision 19.0  2003/07/16 05:47:10  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:53  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:22  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:29  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:08  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:54:51  hravlin
// Changed className() to return const char *.
//
// Revision 14.0  2000/03/23 16:08:13  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:01  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:40  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:47  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:38  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:27:20  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:38  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:18:38  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.5  1996/06/18  18:42:51  pixton
//  Copyright Update
//
//  Revision 1.4  1996/03/14  19:51:32  baker
//  Documentation.
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
//  Revision 1.2  1995/07/17  19:56:12  hr
//  Added 'virtual' to destructor of virtual classes.
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#ifndef _SV_FORM_COMP_H
#define _SV_FORM_COMP_H


/* <g> UserInterface */

/* <s> Abstract class representing a basic Motif Form widget. */

/* <d> Abstract class representing a basic Motif widget with a single
XmForm as the base widget. */


#include <AvUserComp.h>

class AvFormComp : public AvUserComp {

public:

  AvFormComp (Widget parent, int buildInsideParent = TRUE);
  
  virtual const char * className () const;

  // returns the base widget (a Form) for the component
  virtual Widget baseWidget ();

  virtual ~AvFormComp ();

protected:

private:

};

#endif
