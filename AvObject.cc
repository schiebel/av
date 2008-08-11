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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvObject.cc,v 19.0 2003/07/16 05:47:32 aips2adm Exp $
//
// $Log: AvObject.cc,v $
// Revision 19.0  2003/07/16 05:47:32  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:11  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:40  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:47  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:24  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:42:36  hravlin
// Changed className() to return const char *.
//
// Revision 14.0  2000/03/23 16:08:37  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:25  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:25  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:50  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:50  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:29:11  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:17:53  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:42:02  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.4  1996/06/18  18:46:20  pixton
//  Copyright Update
//
//  Revision 1.3  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/07/26  19:31:37  hr
//  Label wasn't being initialized.
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include <AvObject.h>


const char * AvObject::className () const
{
  return ("AvObject");
}

AvObject::AvObject()
{
	label_[0] = '\0';
}

AvObject::~AvObject(){}


ostream & operator<< (ostream & o, AvObject & obj)
{
  o << "{ AvObj\nLabel " << obj.getLabel() << " }" << endl;
  return o;
}
