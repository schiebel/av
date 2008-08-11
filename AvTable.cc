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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvTable.cc,v 19.0 2003/07/16 05:46:36 aips2adm Exp $
//
// $Log: AvTable.cc,v $
// Revision 19.0  2003/07/16 05:46:36  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:23  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:41:51  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:01  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:09  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:31:58  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:07:37  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/08/25 19:38:57  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:39:19  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:28  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:04  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:51:38  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:24:08  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:14:45  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 07:30:23  droberts
// Final update from monet archive.
//
// Revision 1.3  1996/09/05 20:20:45  hr
// Made str parameter in setCell const to so it would work with accessor's
// functions.
//
// Revision 1.2  1996/09/05  20:08:21  hr
// Made newLabel in setRowLabel() const.
//
// Revision 1.1  1996/07/11  21:31:21  pixton
// Automated Checkin
//
//  Revision 1.5  1996/06/18  18:46:35  pixton
//  Copyright Update
//
//  Revision 1.4  1996/03/28  17:11:46  hr
//  Changed from XmStringCreateSimple to XmStringCreateLocalized and
//  return the strings.
//
//  Revision 1.3  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/08/23  23:41:41  baker
//  Set cells to have 0 as their marginHeight resource.
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------





#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/RowColumn.h>
#include <Xm/LabelG.h>
#include <Xm/TextF.h>

#include "AvTable.h"
#include "AvWidgetMisc.h"

AvTable::AvTable
(Widget parent, int buildInsideParent, int rows, int columns) :
AvFormComp (parent, buildInsideParent)
{
  nRows = rows;
  nColumns = columns;
  precision = 0;

  // we use 1 extra row label to balance out the space
  rowLabel = (WidgetList) XtMalloc ((nRows+1) * (int)sizeof (Widget));
  columnLabel = (WidgetList) XtMalloc (nColumns * (int)sizeof (Widget));
  cell = (WidgetList) XtMalloc (nRows * nColumns * (int)sizeof (Widget));

  // table rows are labelled at left 
  Widget rowLabels = XtVaCreateManagedWidget
    ("TableRowLabels", xmRowColumnWidgetClass, baseWidget_,
     XmNleftAttachment, XmATTACH_FORM,
     XmNtopAttachment, XmATTACH_FORM,
     XmNbottomAttachment, XmATTACH_FORM,
     XmNentryAlignment, XmALIGNMENT_END,
     NULL);

  // make default labels for the rows
  char str[32];
  for (int r=0; r<nRows+1; r++) {
    sprintf (str, "%d", r);
    rowLabel[r] = XtVaCreateWidget
      ("TableRowLabel", xmLabelGadgetClass, rowLabels,
       NULL);
    setLabelString (rowLabel[r], str);
  }

  // set the topmost label to empty string
  setLabelString (rowLabel[0], "   ");

  XtManageChildren (rowLabel, nRows+1);

  Widget columnForm = XtVaCreateManagedWidget
    ("", xmFormWidgetClass, baseWidget_,
     XmNleftAttachment, XmATTACH_WIDGET,
     XmNleftWidget, rowLabels,
     XmNrightAttachment, XmATTACH_FORM,
     XmNverticalSpacing, 0,
     XmNhorizontalSpacing, 0,
     NULL);

  // divide the space by the number of columns plus the row labels
  int space = 100 / (nColumns);

  // make the columns
  Widget column;
  Widget topWidget;
  int indx = 0;
  for (int c=0; c<nColumns; c++) {
    column = XtVaCreateManagedWidget
      ("TableColumn", xmRowColumnWidgetClass, columnForm,
       XmNleftAttachment, XmATTACH_POSITION,
       XmNleftPosition, space*c,
       XmNrightAttachment, XmATTACH_POSITION,
       XmNrightPosition, space*(c+1)-1,
       XmNentryAlignment, XmALIGNMENT_CENTER,
       XmNmarginWidth, 0,
       XmNspacing, 2,
       NULL);

    // attach the last column to the outer edge of form
    if (c == nColumns - 1)
      XtVaSetValues (column, XmNrightAttachment, XmATTACH_FORM, NULL);

    // put a label at the top of the column
    sprintf (str, "%c", 'A'+c);
    columnLabel[c] = XtVaCreateManagedWidget
      (str, xmLabelGadgetClass, column,
       XmNtopAttachment, XmATTACH_FORM,
       XmNalignment, XmALIGNMENT_CENTER,
       NULL);
    
    topWidget = columnLabel[c];

    for (int j=0; j<nRows; j++) {
      cell[indx] = XtVaCreateWidget
	("TableCell", xmTextFieldWidgetClass, column,
	 XmNtopAttachment, XmATTACH_WIDGET,
	 XmNtopWidget, topWidget,
	 XmNeditable, FALSE,
	 XmNcursorPositionVisible, FALSE,
	 XmNhighlightThickness, 0,
	 XmNmarginHeight, 0,
	 XmNshadowThickness, 1,
	 NULL);
      if (c == nColumns - 1)
	XtVaSetValues (cell[indx], XmNrightAttachment, XmATTACH_FORM, NULL);
      topWidget = cell[indx++];
    }
    XtVaSetValues (cell[indx-1], XmNbottomAttachment, XmATTACH_FORM, NULL);
    XtManageChildren (cell+(c*nRows), nRows);
  }
}


void AvTable::setRowLabel (const char * newLabel, int which)
{
  if (which > nRows) return;
  setLabelString (rowLabel[which+1], newLabel);
}


void AvTable::setRowLabels (const char ** newLabels)
{
  for (int i=0; i<nRows; i++)
  { XmString xStr = AvXmStringCreateLocalized (newLabels[i]);
    XtVaSetValues (rowLabel[i], xStr,
		   XmNlabelString, 
		   NULL);
    XmStringFree(xStr);
  }
}


void AvTable::setColumnLabel (const char * newLabel, int which)
{
  if (which > nColumns) return;
  setLabelString (columnLabel[which], newLabel);
} 


void AvTable::setColumnLabels (const char ** newLabels)
{
  for (int i=0; i<nColumns; i++)
  { XmString xStr = AvXmStringCreateLocalized (newLabels[i]);
    XtVaSetValues (columnLabel[i],
		   XmNlabelString, xStr,
		   NULL);
    XmStringFree(xStr);
  }
} 


void AvTable::setCell (int r, int c, float newCell)
{
  char str[128];
  char format[16];
  sprintf (format, "%s%d%s", "%.", precision, "f");
  sprintf (str, format, newCell);
  setCell (r, c, str);
}


void AvTable::setPrecision (int decDigits)
{
  precision = decDigits;
}


void AvTable::setCell (int r, int c, const char * str)
{
  int indx = c * nRows + r;
//  cerr << r << '\t' << c << '\t' << indx << '\t' << str << endl;
// Work around problem of passing a const char *.
  char *buf = new char[strlen(str)+1];
  strcpy(buf, str);
  XmTextFieldSetString (cell[indx], buf);
  delete [] buf;
}


void AvTable::setCellWidth (int sz)
{
  int c, r, indx = 0;
  
  for (c=0; c<nColumns; c++)
    for (r=0; r<nRows; r++)
      XtVaSetValues (cell[indx++], XmNcolumns, sz, NULL);
}


void AvTable::setCellWidth (int sz, int which)
{
  if (which > nColumns) return;
  for (int r=0; r<nRows; r++)
    XtVaSetValues (cell[which * nRows + r], XmNcolumns, sz, NULL);
}


AvTable::~AvTable () 
{
  XtDestroyWidget (baseWidget());
}
