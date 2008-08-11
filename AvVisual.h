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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvVisual.h,v 19.0 2003/07/16 05:46:57 aips2adm Exp $
//
// $Log: AvVisual.h,v $
// Revision 19.0  2003/07/16 05:46:57  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:42  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:10  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:19  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:22  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:23:43  hravlin
// Changed className() to return a const char *.
// /
//
// Revision 14.0  2000/03/23 16:08:00  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:45  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:12  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:08  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:52  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:26:07  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:55  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 07:43:18  droberts
// Final update from monet archive.
//
// Revision 1.2  1996/09/10 16:43:07  hr
// Removed reference to AvGrid.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.3  1996/06/18  18:43:31  pixton
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

#ifndef _SV_VISUAL_H
#define _SV_VISUAL_H

class AvConductor;

#include <AvRepresentation.h>

class AvVisual : public AvRepresentation {

public:

  virtual const char * className () const;

  enum ColorStyle { COLOR_TABLE, RGB, HSV };

  virtual void map () =0;

  static void remapCB (AvObject * obj, AvConductor *, void * flags);

protected:

  virtual void remap (int);

private:

};

#endif
