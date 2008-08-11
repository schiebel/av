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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvObject.h,v 19.0 2003/07/16 05:47:15 aips2adm Exp $
//
// $Log: AvObject.h,v $
// Revision 19.0  2003/07/16 05:47:15  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:57  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:26  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:33  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:25  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:42:50  hravlin
// Changed className() to return const char *.
//
// Revision 14.0  2000/03/23 16:08:19  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:06  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:50  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:01  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:55  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:27:46  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:58  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:42:27  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.5  1996/06/18  18:43:07  pixton
//  Copyright Update
//
//  Revision 1.4  1995/10/24  17:07:25  pixton
//  CONSTified label() and getLabel().
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
//  Revision 1.2  1995/07/26  19:32:02  hr
//  Added constructor (to initialize label_) and virtual destructor.
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#ifndef _SV_VIS_OBJ_H
#define _SV_VIS_OBJ_H

#include <string.h>
#include <iostream.h>
#include <iomanip.h>

#include <Av.h>

class AvObject {

  friend ostream & operator<< (ostream & o, AvObject & obj);

public:

  AvObject();
  virtual ~AvObject();
  virtual const char * className () const;

  virtual const char * classLabel () const { return ("Object"); } 

  virtual const char * getClassLabel () const { return ("Object"); } 

  void setLabel (char * newLabel) { strcpy (label_, newLabel); }

  char * label () const { return (char *) label_; }

  char * getLabel () const { return (char *) label_; }

protected:

  char label_[128];

private:

};

#endif
