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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvImageDataPanel.cc,v 19.0 2003/07/16 05:48:00 aips2adm Exp $
//
// $Log: AvImageDataPanel.cc,v $
// Revision 19.0  2003/07/16 05:48:00  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:35  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:43:03  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:09  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:36  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:49:57  hravlin
// Replaced XmStringCreateLocalized() by AvXmStringCreateLocalized().
//
// Revision 14.0  2000/03/23 16:09:06  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.2  1999/10/04 20:26:40  hravlin
// Some widgets were being attached to the wrong widgets.
//
// Revision 13.1  1999/10/01 21:56:59  hravlin
// *** empty log message ***
//
// Revision 13.0  1999/08/10 18:40:56  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:17  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:02:07  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:56:21  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1997/12/17 21:50:48  hr
// Changes to remove warnings from MIPSpro.7x compiler.
//
// Revision 9.0  1997/08/25 21:31:32  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.3  1997/05/08 17:53:23  hr
// In constructor readded the XmNbottomAttachment for parts. Now it seems
// to help, esp. when rank > 3.
//
// Revision 8.2  1997/05/07 17:15:42  hr
// In setDataTransfer() call setRange rather than setMin/Max().
//
// Revision 8.1  1997/02/25 16:48:06  hr
// Changes, mostly in buildViewControls() to support the new "View" object.
//
// Revision 8.0  1997/02/20 03:16:27  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.4  1996/12/12 08:25:16  droberts
// Fixed odd quote in log message/comment.  Added dataModified().
//
// Revision 1.6  1996/10/23 15:26:06  hr
// Made the IJK readout text field non editable and turned off its cursor.
// Updated call to AvClipper's constructor to enable editing of min/max fields.
//
// Revision 1.5  1996/09/20 20:19:35  pixton
// Prefixed all classes with Av
//
// Revision 1.4  1996/09/20  19:34:23  hr
// Removed unused variable.
//
// Revision 1.3  1996/09/18  19:02:25  hr
// More integration with AvAccessor.
//
// Revision 1.2  1996/09/10 16:52:17  hr
// Initial changes for integration with AvAccessor & AvDataSet.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.10  1996/06/18  19:15:21  hr
//  Added "Set", "Reset" buttons to data clip. User can set effective min/max
//  of the clipper.
//
//  Revision 1.9  1996/06/18  18:46:11  pixton
//  Copyright Update
//
//  Revision 1.8  1996/04/17  14:56:01  hr
//  Replaced "Slice along:" with "Slice:"
//
//  Revision 1.7  1996/04/16  16:24:28  hr
//  Some compilers can't handle initializations like:
//  	char * buffer[] = { "A","B","C" };
//  Changing to "static char *..." works.
//
//  Revision 1.6  1995/11/01  22:56:26  hr
//  Unmanage basewidget_ when building. This helps prevent the main window
//  from jittering in size. Removed an XmNbottomAttachment from the parts
//  widget. This seems to result in the slider being left out a piece at
//  a time rather than all or nothing when the window is too small.
//
//  Revision 1.5  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/09/13  15:48:09  hr
//  Gave names to some of the manager widgets.
//  attached partsRC to the top and bottom of its parent.
//
//  Revision 1.3  1995/07/18  17:13:31  hr
//  g++ doesn't like constructs like for(int i;;); for(i). Or buf[int &];
//
//  Revision 1.2  1995/07/10  15:07:21  hr
//  Increased length of buildBasicInfo::minMaxText from 32 to 80 characters
//  since unusual values can create long strings.
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <Xm/Xm.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>
#include <Xm/ToggleB.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/Separator.h>
#include <Xm/TextF.h>
#include "AvClipper.h"
#include "AvCLUImage.h"
#include "AvImageDataPanel.h"
#include "AvImageView.h"
#include "AvImageViewPanel.h"
#include "AvImageWindow.h"
#include "AvSlider.h"
#include "AvStepper.h"
#include "AvTable.h"
#include "AvTwoValueSlider.h"
#include "AvAccessor.h"
#include "AvConfigInfo.h"
#include "AvNDConfig.h"
#include "AvWidgetMisc.h"

/*
AvImageDataPanel builds the controls in the main window:
	Basic info readouts, slice selection and data range control.


Outline of widget use:
		parent (workArea_ = form)
		 baseWidget_ (form)
		  parts (rc)
		    basicinfoform(form)
			info
		    viewform (form)
			viewcontrols.
*/

AvImageDataPanel::AvImageDataPanel 
(Widget parent, int buildInsideParent, AvImageDataItem & d) :
AvFormComp (parent, buildInsideParent), dItem_ (d)
{
  // Unmanage the base widget. This will help keep the main window from
  // jumping around in size and, maybe, help ensure all the parts are
  // viewable. (Sometimes the slider isn't shown).
  // The 'show' below will remanage it.
  // Unfortunately, if we unmanage the widget when there may be more than
  // one row of buttons, the slider doesn't always show.
  if((dItem_.getNumViews() <= 4) && (XtIsManaged(baseWidget_)))
	XtUnmanageChild(baseWidget_);

  // Attach the baseWidget_, a form widget itself, to the parent form.
  XtVaSetValues (baseWidget_,
		 XmNtopAttachment, XmATTACH_FORM,
		 XmNbottomAttachment, XmATTACH_FORM,
		 XmNleftAttachment, XmATTACH_FORM,
		 XmNrightAttachment, XmATTACH_FORM,
		 NULL);

  Widget parts = XtVaCreateManagedWidget
    ("partsRC", xmRowColumnWidgetClass, baseWidget_,
     XmNorientation, XmVERTICAL,
     XmNspacing, 3,
     XmNmarginTop, 10,
     XmNmarginWidth, 8,
     XmNleftAttachment, XmATTACH_FORM,
     XmNrightAttachment, XmATTACH_FORM,
     XmNtopAttachment, XmATTACH_FORM,
	// If this is included, the data range slider is more likely to
	// disappear.
	// (5/97) Putting it back now seems to help, esp for rank > 3 images.
     XmNbottomAttachment, XmATTACH_FORM,
     NULL);

  Widget basicInfoForm = XtVaCreateManagedWidget
    ("basicInfoForm", xmFormWidgetClass, parts,
     NULL);

  buildBasicInfo (basicInfoForm);

  XtVaCreateManagedWidget ("", xmSeparatorWidgetClass, parts, NULL);
  
  Widget viewForm = XtVaCreateManagedWidget
    ("viewForm", xmFormWidgetClass, parts,
     NULL);

  buildViewControls (viewForm);

  XtVaCreateManagedWidget ("", xmSeparatorWidgetClass, parts, NULL);

  Widget transferForm = XtVaCreateManagedWidget
    ("transferForm", xmFormWidgetClass, parts,
     NULL);

  buildDataTransfer (transferForm);
  show ();
}


void AvImageDataPanel::buildBasicInfo (Widget parent)
{
  XmString xStr = AvXmStringCreateLocalized ("Data File");
  Widget objType = XtVaCreateManagedWidget
    ("FontBoldItalic", xmLabelWidgetClass, parent,
     XmNtopAttachment, XmATTACH_FORM,
     XmNtopOffset, 2,
     XmNlabelString, xStr,
     NULL);
  XmStringFree (xStr);

  Widget objName = XtVaCreateManagedWidget
    ("FontBoldItalic", xmTextFieldWidgetClass, parent,
     XmNleftAttachment, XmATTACH_WIDGET,
     XmNleftWidget, objType,
     XmNrightAttachment, XmATTACH_FORM,
     XmNcolumns, 10,
     XmNeditable, FALSE,
     XmNcursorPositionVisible, FALSE,
     XmNhighlightThickness, 0,
     XmNshadowThickness, 1,
     XmNvalue, dItem_.accessor()->dataset()->fileName(),
     NULL);

  // show the 'ijk' sizes of this data set
  Widget sizeForm = XtVaCreateManagedWidget
    ("SizeForm", xmFormWidgetClass, parent,
     XmNtopAttachment, XmATTACH_WIDGET,
     XmNtopWidget, objName,
     XmNtopOffset, 4,
     NULL);

  xStr = AvXmStringCreateLocalized ("Size");
  Widget dimsLabel = XtVaCreateManagedWidget
    ("SizeLabel", xmLabelWidgetClass, sizeForm,
     XmNbottomAttachment, XmATTACH_FORM,
     XmNeditable, FALSE,
     XmNcursorPositionVisible, FALSE,
     XmNlabelString, xStr,
     NULL);
  XmStringFree (xStr);

  // make a string for the dimensions: e.g., 'i x j x k'
  char str[128];
  int tmp = accessor()->width();
  int i;
  char tmpc[16];
  sprintf (str, "%d", tmp);
  int rank = accessor()->rank();
  for (i=1; i<rank; i++) {
    tmp = accessor()->getAxisLength(i);
    sprintf (tmpc, " x %d", tmp);
    strcat (str, tmpc);
  }
  
  XtVaCreateManagedWidget
    ("FontPlain", xmTextFieldWidgetClass, sizeForm,
     XmNleftAttachment, XmATTACH_WIDGET,
     XmNleftWidget, dimsLabel,
     XmNrightAttachment, XmATTACH_FORM,
     XmNshadowThickness, 1,
     XmNeditable, FALSE,
     XmNcursorPositionVisible, FALSE,
     XmNvalue, str,
     NULL);

  // make the table showing data set extent
  static const char * cLabels[] = { "From", "To" };
  
  // make a Table and fill in its row and column labels
  AvTable * regionSize = new AvTable (parent, TRUE, rank, 2);

  XtVaSetValues (regionSize->baseWidget(),
		 XmNtopAttachment, XmATTACH_WIDGET,
		 XmNtopWidget, sizeForm,
		 XmNleftAttachment, XmATTACH_FORM,
		 XmNrightAttachment, XmATTACH_FORM,
		 NULL);

  for (i=0; i<rank; i++)
    regionSize->setRowLabel (accessor()->axisName(i), i);
  regionSize->setColumnLabels (cLabels);
  
//  maxLen = 0;

  // fill in the "from" column
  AvWPosition m = accessor()->minWorld();
  for (i=0; i<rank; i++) {
	regionSize->setCell(i, 0, accessor()->axisValueUnits(i, m(i)).chars());
  }

  // fill in the "to" column
  m = accessor()->maxWorld();
  for (i=0; i<rank; i++) {
	regionSize->setCell(i, 1, accessor()->axisValueUnits(i, m(i)).chars());
  }
  regionSize->setCellWidth (10);
  regionSize->show();
}

// Build the toggle buttons that allow selection of which view to show.
void AvImageDataPanel::buildViewControls (Widget parent)
{

    XmString xStr = AvXmStringCreateLocalized ("Slice:");
    Widget axisLabel = XtVaCreateManagedWidget
      ("FontBoldItalic", xmLabelWidgetClass, parent, 
       XmNtopAttachment, XmATTACH_FORM,
//       XmNbottomAttachment, XmATTACH_FORM,
       XmNlabelString, xStr,
       NULL);
    XmStringFree (xStr);

    int nviews = dItem_.getNumViews();
    short nrows = nviews/4 + ((nviews % 4) ? 1 : 0);

    Widget axes = XtVaCreateWidget
      ("axesRC", xmRowColumnWidgetClass, parent,
       XmNcolumns, nrows,
       XmNorientation, XmHORIZONTAL,
       XmNpacking, XmPACK_COLUMN,
       XmNspacing, 1,
       XmNleftAttachment, XmATTACH_WIDGET,
       XmNleftWidget, axisLabel,
       XmNleftOffset, 2,
       NULL);


    int i=0;
    AvImageDataItem::View *view;
    while((view = dItem_.getView(i)) != NULL)
    {    char axisName[80];
	// Copying to axisName is to get around the problem of passing a
	// const char * to a routine w/o a const char * argument.
	if(view->haveMapping())
		strcpy(axisName, view->label().chars());
	else
		strcpy(axisName, "Set Map");
	xStr = AvXmStringCreateLocalized (axisName);
	Widget tb = XtVaCreateManagedWidget
		("FontPlain", xmToggleButtonWidgetClass, axes,
		XmNuserData, i++,	// The index into the views array.
		XmNlabelString, xStr,
		XmNshadowThickness, 1,
		NULL);
	XmStringFree (xStr);

      // If a mapping vector has been defined (by default or the user),
      // link button to the display view routine. Otherwise link to the
      // routine to have user configure mapping.
      if(view->haveMapping())
	XtAddCallback(tb, XmNvalueChangedCallback,
		     (XtCallbackProc) &AvImageDataPanel::changeAxisCB,
		     (XtPointer) this);

      else
	XtAddCallback (tb, XmNvalueChangedCallback,
		     (XtCallbackProc) &AvImageDataPanel::doMapCB,
		     (XtPointer) this);
      view->tb = tb;
      view->idp = this;
      // The window is actually created in AvImageDataItem::makeview.
      if(view->autoDisplay())
	XmToggleButtonSetState(view->tb, TRUE, FALSE);
    }

  // Without this, axes winds up being 1 row.
  XtVaSetValues (axes,
		 XmNnumColumns, nrows,
		 NULL);
  XtManageChild(axes);
}

void AvImageDataPanel::dataModified()
{
  // data modified, so min/max could be different.
  // adjust clip limits accordingly

  float dmin = accessor()->dataMin();
  float cmin = dataClip_->clipMin();
  if (cmin < dmin) cmin = dmin;
  dataClip_->setMin(accessor()->dataMin());
  dataClip_->setClipMin(cmin);

  float dmax = accessor()->dataMax();
  float cmax = dataClip_->clipMax();
  if (cmax > dmax) cmax = dmax;
  dataClip_->setMax(accessor()->dataMax());
  dataClip_->setClipMax(cmax);
}

// !! Check to make sure this isn't at least partially obsolete.
void AvImageDataPanel::setDataTransfer ()
{
  float min, max;
  min = accessor()->clipMin();
  max = accessor()->clipMax();
#if 0
  dataClip_->setMin (min);
  dataClip_->setMax (max);
#else
  dataClip_->setRange(min, max);
#endif

  dataClip_->setClipMin (min);
  dataClip_->setClipMax (max);
}

// Use the current settings of the clipper to set new min/max range of
// the clipper.
void AvImageDataPanel::setRangeCB(Widget, AvImageDataPanel *idp, XtPointer)
{
//float min, max, cmin, cmax;
float cmin, cmax;

	cmin = idp->dataClip_->getClipMin();
	cmax = idp->dataClip_->getClipMax();
//	min = idp->dataClip_->getMin();
//	max = idp->dataClip_->getMax();
	idp->dataClip_->setRange(cmin, cmax);
	// This is to get the slider in the correct position.
	idp->dataClip_->setClipRange(cmin, cmax);
//	if((cmin != min) || (cmax != max))
//		idp->dataClip_->invokeCallbacks(AvConductor::MouseMove);
}

// Reset clipper to data's min/max range.
void AvImageDataPanel::resetRangeCB(Widget, AvImageDataPanel *idp, XtPointer)
{
//float min, max, cmin, cmax;
float cmin, cmax;

	cmin = idp->accessor()->dataMin();
	cmax = idp->accessor()->dataMax();
	idp->dataClip_->setRange(cmin, cmax);
//	min = idp->dataClip_->getMin();
//	max = idp->dataClip_->getMax();
	idp->dataClip_->setRange(cmin, cmax);
	// This is to get the slider in the correct position.
	idp->dataClip_->setClipRange(cmin, cmax);
	idp->accessor()->setClipMinMax(cmin, cmax);
}

void AvImageDataPanel::buildDataTransfer (Widget parent)
{
  XmString xStr = AvXmStringCreateLocalized ("Data range:");
#define SETRANGE
#ifdef SETRANGE
  
	// This section allows the user to set/reset the data clip range.
	Widget partsRC = XtVaCreateManagedWidget
				("partsRC", xmRowColumnWidgetClass, parent,
				 XmNorientation, XmHORIZONTAL,
#if 0
     XmNrightAttachment, XmATTACH_FORM,
     XmNleftAttachment, XmATTACH_FORM,
#endif
				 NULL);

	XtVaCreateManagedWidget
			("FontBoldItalic", xmLabelWidgetClass, partsRC,
			 XmNlabelString, xStr,
			 NULL);
	XmStringFree (xStr);

	xStr = AvXmStringCreateLocalized("Set");
	Widget set = 
	  XtVaCreateManagedWidget("setPB", xmPushButtonWidgetClass, partsRC,
			    XmNlabelString, xStr,
			    NULL);
	XmStringFree (xStr);
	XtAddCallback(set, XmNactivateCallback,
	   (XtCallbackProc) &AvImageDataPanel::setRangeCB, (XtPointer) this);


	xStr = AvXmStringCreateLocalized("Reset");
	Widget reset = 
	  XtVaCreateManagedWidget("resetPB", xmPushButtonWidgetClass, partsRC,
			    XmNlabelString, xStr,
			    NULL);
	XtAddCallback(reset, XmNactivateCallback,
	   (XtCallbackProc) &AvImageDataPanel::resetRangeCB, (XtPointer) this);
	XmStringFree (xStr);
#else
  Widget label = XtVaCreateManagedWidget
    ("FontBoldItalic", xmLabelWidgetClass, parent,
     XmNlabelString, xStr,
     NULL);
  XmStringFree (xStr);
#endif

  // make some guess about how many decimal places to use in
  // displaying the data range
  int precision;
  if (accessor()->dataMax() < 1.0)
    precision = 6;
  else
    precision = 3;
  
  dataClip_ = new AvClipper (parent, TRUE, 256, precision, 1);
  XtVaSetValues (dataClip_->baseWidget(),
		 XmNtopAttachment, XmATTACH_WIDGET,
		 XmNtopWidget, partsRC,
		 NULL);

  dataClip_->setLabels ("Data Minimum", "Data Maximum");
  dataClip_->showLabels (FALSE);
  dataClip_->showClipLabels (FALSE);
  
  setDataTransfer ();

  dataClip_->show();
  
  dataClip_->addCallback 
    ((AvConductorCB) &AvImageDataPanel::updateClipCB, AvConductor::MouseUp,
     (XtPointer) this, (XtPointer) dataClip_);
}

#if 0
void AvImageDataPanel::setAxisViewIndicator (int a, int status)
{
  // if this data is more than 2-dimensional, 'a' is the toggle index
  if (effectiveRank() > 2)
    XmToggleButtonSetState (axis_[a], status, FALSE);
  else
    // if this is only a 2-dimensional data set, the toggle is in axis_[0],
    // despite whatever the value of 'a'
    XmToggleButtonSetState (axis_[0], status, FALSE);
}

#endif

void AvImageDataPanel::update ()
{
  setDataTransfer ();
}


void AvImageDataPanel::changeAxis (Widget w)
{
  int i;
  XtVaGetValues (w, XmNuserData, &i, NULL);
  Boolean isSet = XmToggleButtonGetState(w);
  AvImageDataItem::View *view = dItem_.getView(i);

  // If the user manually turns on or off a view, update the autoDisplay
  // configuration variable.
	if(view != NULL)
		view->autoDisplay(isSet);

  // activate this axis view
  if (isSet)
    dItem_.makeView (i);
  else
    dItem_.deleteView (i);
}


void AvImageDataPanel::updateClipMinMax ()
{
  float min, max;
  min = dataClip_->getClipMin ();
  max = dataClip_->getClipMax ();
  accessor()->setClipMinMax(min, max);
  // Update config.
  AvConfigInfo *info = dItem_.getView(0)->info();
  info->setDouble("clipMin", (double) min);
  info->setDouble("clipMax", (double) max);
}


void AvImageDataPanel::updateDisplay ()
{
  dItem_.updateViews ();
}

  
void AvImageDataPanel::changeAxisCB (Widget w, AvImageDataPanel * p, XtPointer)
{
  p->changeAxis (w);
}

// Called when a view toggle is pressed that needs to have its axis
// mapping set.
// Create and show the configuration window for the view.
void AvImageDataPanel::doMapCB(Widget w, AvImageDataPanel * p, XtPointer)
{
  int i;		// View index.
  Boolean isSet = XmToggleButtonGetState(w);
  if(!isSet)
	return;
  XtVaGetValues (w, XmNuserData, &i, NULL);

  AvImageDataItem::View *view = p->dItem_.getView(i);
  if(view == NULL)
	return;		// Oops.
  if(view->config == NULL)
    {
	{	view->config = new AvNDConfig(view, p->baseWidget_);
		view->config->addCallback(&AvImageDataPanel::haveMapCB,
					  (XtPointer)view);
		view->config->setOffset(AvImageView::oneIndexing());
	}
   }
   view->config->popup();
}

// Called when the mapping window is closed via 'done' button.
// Remove the callback to this callback, add the callback to axisChangeCB,
// and copy relevent data.
// Turn off button.
void AvImageDataPanel::haveMapCB(AvNDConfig *, XtPointer v,
				 const Boolean destroyed)
{ 

  AvImageDataItem::View *view = (AvImageDataItem::View *)v;
  if(view == NULL)
	return;		// Oops.
  Widget tb = view->tb;
  // If the widget was destroyed (presumably by the user), just turn off
  // the button.
  if(destroyed)
  {	XmToggleButtonSetState (tb, FALSE, FALSE);
	return;
  }

  // Remove this callback and replace with the one to display the view.
  XtRemoveCallback(tb, XmNvalueChangedCallback,
		   (XtCallbackProc) &AvImageDataPanel::doMapCB, view->idp);
  // Make sure button is off.
  XmToggleButtonSetState (tb, FALSE, FALSE);
  XtAddCallback(tb, XmNvalueChangedCallback,
		(XtCallbackProc) &AvImageDataPanel::changeAxisCB,
		(XtPointer) view->idp);

  // Change label of toggle to indicate mapping has been done.
  setLabelString(tb, view->label().chars());

  Boolean autoDisplay = view->autoDisplay();

#if 0
  AvAccessor *a = view->accessor();
cout << "Map:  " << a->map() << endl;
cout << "Start " << a->start(FALSE) << endl;
cout << "Shape " << a->shape(FALSE) << endl;
cout << "Auto Display " << ((autoDisplay) ? "TRUE" : "FALSE") << endl;
#endif

  delete view->config;
  view->config = NULL;

  if(autoDisplay)	// Turning on the button will trigger the display.
	XmToggleButtonSetState(tb, TRUE, TRUE);
}



void AvImageDataPanel::updateClipCB (AvImageDataPanel * p, AvConductor *, int)
{
  p->updateClipMinMax ();
  p->updateDisplay ();
}


AvImageDataPanel::~AvImageDataPanel ()
{

}
