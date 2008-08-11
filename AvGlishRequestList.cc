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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvGlishRequestList.cc,v 19.0 2003/07/16 05:47:16 aips2adm Exp $
//
// $Log: AvGlishRequestList.cc,v $
// Revision 19.0  2003/07/16 05:47:16  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:57  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:27  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:34  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:27  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:19  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  2000/03/09 17:17:15  hravlin
// Removed some compiler warnings.
//
// Revision 13.0  1999/08/10 18:40:07  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:51  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:03  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:57  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:27:49  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.1  1997/06/11 15:30:00  hr
// Removed a printf from applyCB().
//
// Revision 8.0  1997/02/20 03:16:59  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1996/12/12 09:23:47  droberts
// First pass, glish events enabled.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.5  1996/06/18  18:46:04  pixton
//  Copyright Update
//
//  Revision 1.4  1995/10/03  19:03:29  hr
//  Adjustment needed to the previous ifdef.
//
//  Revision 1.3  1995/10/03  18:32:58  hr
//  Added ifdef so code is only compiled if the appropriate flag is defined.
//  Added copyright.
//
//  Revision 1.2  1995/09/19  23:19:08  baker
//  *** empty log message ***
//
//
//---------------------------------------------------------------------------

//
//
//  AvGlishRequestList.cc
//
//

#ifdef GLISH

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/PushBG.h>
#include <Xm/LabelG.h>
#include <Xm/RowColumn.h>
#include <Xm/Frame.h>

#include "AvList.h"
#include "AvGlishRequestList.h"
#include "AvImageView.h"

//---------------------------------------------------------------------------
//
//  GlishRequestListEntry
//
//---------------------------------------------------------------------------

//
//  Make the Widget tree off of parent_ 
//
void AvGlishRequestListEntry::build(const char * imageName,
				    void * data,
				    XtCallbackProc applyCB,
				    XtCallbackProc cancelCB)
{
  entry_ = XtVaCreateManagedWidget("frame", xmFrameWidgetClass, parent_,
				   XmNuserData, this,
				   NULL);

  // Request Entry Widget
  Widget form = XtVaCreateManagedWidget
    ("GlishRequest", xmFormWidgetClass, entry_,
     NULL);

  // form | apply  -  Apply the last value to the selection
  Widget apply = XtVaCreateManagedWidget("Apply",
					 xmPushButtonGadgetClass, form,
					 XmNtopAttachment, XmATTACH_FORM,
					 XmNrightAttachment, XmATTACH_FORM,
					 XmNshadowThickness, 1,
					 XmNuserData, this,
					 NULL);
  XtAddCallback(apply, XmNactivateCallback, applyCB, data);
		

  // form | cancel  -  Cancel the request
  Widget cancel = XtVaCreateManagedWidget("Cancel",
					  xmPushButtonGadgetClass, form,
					  XmNtopAttachment, XmATTACH_WIDGET,
					  XmNtopWidget, apply,
					  XmNrightAttachment, XmATTACH_FORM,
					  XmNbottomAttachment, XmATTACH_FORM,
					  XmNshadowThickness, 1,
					  XmNuserData, this,
					  NULL);
  XtAddCallback(cancel, XmNactivateCallback, cancelCB, data);

  // form | requestText  -  Shows the data name and pending request
  char buf[512];
  sprintf(buf, "%s :" , imageName);
  XmString s = XmStringCreateLocalized(buf);
  Widget name = XtVaCreateManagedWidget("FontBoldItalic",
					xmLabelGadgetClass, form,
					XmNlabelString, s,
					XmNleftAttachment, XmATTACH_FORM,
					XmNtopAttachment, XmATTACH_FORM,
					NULL);
  XmStringFree(s);

  sprintf(buf, "<%s>", eventName_);
  s = XmStringCreateLocalized(buf);
/*  Widget event = */ XtVaCreateManagedWidget("event",
					 xmLabelGadgetClass, form,
					 XmNlabelString, s,
					 XmNleftAttachment, XmATTACH_WIDGET,
					 XmNleftWidget, name,
					 XmNtopAttachment, XmATTACH_FORM,
					 XmNrightAttachment, XmATTACH_WIDGET,
					 XmNrightWidget, apply,
					 NULL);
					 

  // form | reason  -  Shows the reason for the request
  s = XmStringCreateLocalized(reason_);
/*  Widget reason = */ XtVaCreateManagedWidget("reason",
					  xmLabelGadgetClass, form,
					  XmNlabelString, s,
					  XmNleftAttachment, XmATTACH_FORM,
					  XmNbottomAttachment, XmATTACH_FORM,
					  XmNtopAttachment, XmATTACH_WIDGET,
					  XmNtopWidget, name,
					  XmNrightAttachment, XmATTACH_WIDGET,
					  XmNrightWidget, cancel,
					  NULL);
  XmStringFree(s);
}

//---------------------------------------------------------------------------
//
//  AvGlishRequestList
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
//  Callbacks - list is the data parameter, and the entry is contained in
//              the widget userData field
//

void AvGlishRequestList::applyCB(Widget w, AvGlishRequestList * list, XtPointer)
{
  XtPointer ep;
  XtVaGetValues(w, XmNuserData, &ep, NULL);
  AvGlishRequestListEntry * e = (AvGlishRequestListEntry *) ep;  

//  printf("GRL::applyCB\n");
  //
  //  1.  Get the image data item pointer.  Cancel the event if invalid ID.
  //    
  AvImageDataItem * idi = AvImageView::instance()->search(e->imageID());
  if (!idi)
    { 
      // DataSet must have been deleted
      list->xgclient()->PostEvent("error", "Data No Longer Available");
      list->deleteRequest(e); 
      return; 
    }
  
  //
  //  2.  Switch on the event name and generate the requested values.  If the
  //      user pressed apply without having selected the request value, ignore
  //      the press.
  //
  if ((!strcmp(e->eventName(), "selectImageLocation")) ||
      (!strcmp(e->eventName(), "queryImageLocation")))
    {
      if (idi->lastLocationValid())
	{
	  AvIPosition ipos = idi->lastLocation();
	  int n = ipos.nelements();

	  int * iLoc = new int[n];
	  double * wLoc = new double[n];

	  AvWPosition wpos(n);
	  idi->dataset()->IJKToWorld(ipos, wpos);

	  for (int i = 0; i < n; i++)
	    {
	      iLoc[i] = ipos(i);
	      wLoc[i] = wpos(i);
	    }

	  Value * v = create_record();
	  v->SetField("imageID", idi->id());
	  v->SetField("pixelLocation", iLoc, n, PRESERVE_ARRAY);
	  v->SetField("worldLocation", wLoc, n, PRESERVE_ARRAY);
	  v->SetField("value", idi->lastValue());
	  list->xgclient()->PostEvent("imageLocation", v);
	  Unref(v);
	  
	  delete [] iLoc;
	  delete [] wLoc;

	  list->deleteRequest(e);
	}
    }
  else if ((!strcmp(e->eventName(), "selectImageRegion")) ||
	   (!strcmp(e->eventName(), "queryImageRegion")))
    {
      if (idi->lastRegionValid())
	{
	  int * v1 = idi->lastRegion();  // lastRegion format: x1 y1 z1 x2 y2 z2
	  int * v2 = v1 + 3;
	  Value * v = create_record();
	  v->SetField("imageID", idi->id());
	  Value * vert1 = new Value(v1, 3, COPY_ARRAY);
	  Value * vert2 = new Value(v2, 3, COPY_ARRAY);
	  Value * verts = create_record();
	  verts->SetField("1", vert1);
	  verts->SetField("2", vert2);
	  v->SetField("vertices", verts);
	  list->xgclient()->PostEvent("imageRegion", v);
	  Unref(vert1);
	  Unref(vert2);
	  Unref(verts);
	  Unref(v);
	  
	  list->deleteRequest(e);
	}
    }
  else if ((!strcmp(e->eventName(), "getImageRegion")))
    {
      cerr << "GLISH event 'getImageRegion' is disabled...\n";
#if 0
      if (idi->lastRegionValid())
	{
	  // Get region parameters
	  int * v1 = idi->lastRegion();
	  int * v2 = v1 + 3;

	  // Fetch the data
	  int n[3];
	  n[0] = (v2[0] - v1[0] + 1);
	  n[1] = (v2[1] - v1[1] + 1);
	  n[2] = (v2[2] - v1[2] + 1);
	  int size = n[0]*n[1]*n[2];
	  float * values = new float[size];  // Glish Deletes!
	  idi->dataset()->fetchRegion(values, v1, v2, 3);

	  // Wrap with glish values
	  Value * v = create_record();
	  v->SetField("imageID", idi->id());

	  Value * vert1 = new Value(v1, 3, PRESERVE_ARRAY);
	  Value * vert2 = new Value(v2, 3, PRESERVE_ARRAY);
	  Value * verts = create_record();
	  verts->SetField("1", vert1);
	  verts->SetField("2", vert2);
	  v->SetField("vertices", verts);

	  Value * shapeV = new Value(n, 3, PRESERVE_ARRAY);
	  Value * dataV = new Value(values, size, TAKE_OVER_ARRAY);
	  dataV->AssignAttribute("shape", shapeV);
	  v->SetField("array", dataV);

	  list->xgclient()->PostEvent("filledImageRegion", v);
	  Unref(verts);
	  Unref(vert1);
	  Unref(vert2);
	  Unref(dataV);
	  Unref(shapeV);
	  Unref(v);

	  list->deleteRequest(e);
	}
#endif
    }
}

void AvGlishRequestList::cancelCB(Widget w, AvGlishRequestList * list, XtPointer)
{
  XtPointer ep;
  XtVaGetValues(w, XmNuserData, &ep, NULL);
  AvGlishRequestListEntry * e = (AvGlishRequestListEntry *) ep;  

  //
  //  1.  Send the cancel message to the glish bus
  //
  
  Value * v = create_record();
  v->SetField("imageID", e->imageID());
  v->SetField("event", e->eventName());
  list->xgclient()->PostEvent("cancelled", v);
  Unref(v);

  //
  //  2.  Delete the request entry
  //
  list->deleteRequest(e);
}

//---------------------------------------------------------------------------
//
// Build the Widget tree for the empty "pending requests" list.
//
// parent is the application base pointer
//

void AvGlishRequestList::build()
{
  // Make a popup shell and use if for glish request reporting
  shell_ = XtVaCreatePopupShell("Glish Request List",
				topLevelShellWidgetClass, parent_,
				XmNiconName, "Glish Request List",
				XmNwidth, 280,
				XmNheight, 50,
				NULL);

  main_ = XtVaCreateManagedWidget("requestList",
					 xmRowColumnWidgetClass, shell_,
					 NULL);
}

// Called from aipsView's glish event handler
void AvGlishRequestList::addRequest(const char * eventName,
				    int imageID,
				    const char * imageName,
				    const char * reason)
{
  AvGlishRequestListEntry * e = new AvGlishRequestListEntry(main_,
							    this,
							    (XtCallbackProc) applyCB,
							    (XtCallbackProc) cancelCB,
							    eventName,
							    imageID,
							    imageName,
							    reason);
  addRequest(e);
}

void AvGlishRequestList::addRequest(AvGlishRequestListEntry * e)
{
  // Popup the window if the list was empty
  if (requestList_.empty()) 
    {
      XtPopup(shell_, XtGrabNone);
      XtManageChild(main_);
    }

  // Raise the window so it will be visible
  //XRaiseWindow(XtDisplay(shell_), XtWindow(shell_));

  // Add to the internal list
  requestList_.add(e);

  // Show the widget
  XtManageChild(e->widget());

  // Resize the window to show all the new entries
  XtVaSetValues(shell_, XmNheight, 50*requestList_.length()-1, NULL);  // hack
  XtVaSetValues(shell_, XmNheight, 50*requestList_.length(), NULL);
}
					 
void AvGlishRequestList::deleteRequest(AvGlishRequestListEntry * e)
{
  requestList_.remove(e);
  XtUnmanageChild(e->widget());
  delete e;
  if (requestList_.empty())
    XtPopdown(shell_);
  else
    {
      XtVaSetValues(shell_, XmNheight, 50*requestList_.length(), NULL);
    }
}

#endif
