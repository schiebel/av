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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvTable.h,v 19.0 2003/07/16 05:46:44 aips2adm Exp $
//
// $Log: AvTable.h,v $
// Revision 19.0  2003/07/16 05:46:44  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:30  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:41:58  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:08  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:37  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:32:04  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:07:45  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:28  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:43  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:27  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:04  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:24:50  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:10  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 07:31:58  droberts
// Final update from monet archive.
//
// Revision 1.3  1996/09/05 20:22:09  hr
// Made str parameter in setCell const.
//
// Revision 1.2  1996/09/05  20:09:15  hr
// Made newLabel in setRowLabel() const.
//
// Revision 1.1  1996/07/11  21:31:21  pixton
// Automated Checkin
//
//  Revision 1.5  1996/06/18  18:43:22  pixton
//  Copyright Update
//
//  Revision 1.4  1996/03/14  19:51:49  baker
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

#ifndef _SV_TABLE_H
#define _SV_TABLE_H


/* <g> UserInterface */

/* <s> Organizes text into a table of labeled rows and columns. */

/* <d> Motif-based class to put short text items into a table. */


#include <AvFormComp.h>

class AvTable : public AvFormComp {

public:

  AvTable (Widget parent, int buildInsideParent, int rows, int columns);

  virtual void setRowLabels (const char ** newLabels);
  virtual void setRowLabel (const char * newLabel, int which);
  virtual void setColumnLabels (const char ** newLabels);
  virtual void setColumnLabel (const char * newLabels, int which);
  virtual void setPrecision (int decDigits);
  virtual void setCell (int r, int c, const char * newValue);
  virtual void setCell (int r, int c, float newValue);

  virtual void setCellWidth (int sz);
  virtual void setCellWidth (int sz, int column);

  virtual ~AvTable();

protected:

private:

  int nRows;
  int nColumns;
  int precision;
  WidgetList rowLabel, columnLabel, cell;

};

#endif
