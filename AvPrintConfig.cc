//# Copyright (C) 1995-2002 Board of Trustees of the University of Illinois
//#
//# This software, both binary and source, is copyrighted by The
//# Board of Trustees of the University of Illinois. Ownership
//# remains with the University. You should have received a copy 
//# of a licensing agreement with this software. See the file 
//# "AIPSVIEW_COPYRIGHT", or contact the University at this address:
//#
//# The NCSA AipsView Visualization System
//# National Center for Supercomputing Applications
//# University of Illinois
//# 405 North Mathews Ave.
//# Urbana, IL 61801
//# aipsview@ncsa.uiuc.edu
//# --------------------------------------------------
//
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvPrintConfig.cc,v 19.0 2003/07/16 05:48:13 aips2adm Exp $
//
// $Log: AvPrintConfig.cc,v $
// Revision 19.0  2003/07/16 05:48:13  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:46  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.2  2002/01/22 22:14:17  hravlin
// Added (int) cast to strlen() for use with 64 bit compilers.
//
// Revision 17.1  2002/01/07 22:37:55  hravlin
// Removed check for __P.
//
// Revision 17.0  2001/11/12 19:43:13  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:20  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:55  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:36:26  hravlin
// Made OTHERPAPER a const.
//
// Revision 14.0  2000/03/23 16:09:18  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.2  1999/09/24 21:23:23  hravlin
// Added support for user editable filenames.
//
// Revision 13.1  1999/09/15 21:18:23  hravlin
// Initial checkin.
//
//
//---------------------------------------------------------------------------

#include <iostream.h>
#include <stdio.h>
#include <string.h>
//#if defined(__GNUC__) && defined(__P) && !defined(__linux__)
//#undef __P
//#endif
#include <math.h>

#include <Xm/TextF.h>
#include <Xm/PushB.h>
#include <Xm/Frame.h>
#include <Xm/RowColumn.h>
#include <Xm/ToggleB.h>		// & For XmToggleButtonSetState()
#include <Xm/Form.h>
#include <Xm/Text.h>
#include <Xm/ScrolledW.h>

#include "AvPrintConfig.h"
#include "AvGUIParts.h"
#include "AvXColorApp.h"

// A short list of paper sizes. Others may be added by entering their values.
static const char *OTHERPAPER = "Other";
static const int OTHERPAPERVALUE = -1;

static AvPrintOptions::IntMap ShortPaperList[] = {
	{AvPSDriver::LETTER, "Letter"},
	{AvPSDriver::LEGAL, "Legal"},
	{AvPSDriver::ISO_A4, "ISO A4"},
	{OTHERPAPERVALUE, OTHERPAPER},
	{ 0, NULL}
};

////////////////////////////////////////////////////////////////
AvPrintConfig::AvPrintConfig(const char *name, Widget ref, AvPrintOptions *po)
{
	name_ = "Setup for ";
	name_ += name;
	name_ += ":";
	name_ += po->config()->moduleName();
	ref_ = ref;
	dialog = NULL;
	up_ = FALSE;
	callback_ = NULL;
	cbdata_ = NULL;
	info_ = po;
	if(info_ != NULL)
		info_->ref();

	toggling_ = FALSE;
}

AvPrintConfig::~AvPrintConfig()
{
	popdown();
	destroy(TRUE);		// Get rid of any windows.
	info_->unref();
}

////////////////////////////////////////////////////////////////
// 			Output Options Panel
////////////////////////////////////////////////////////////////
//Build the "Print" panel.
Widget AvPrintConfig::buildPrintPanel(const char *name, Widget parent)
{
	Widget frame = labeledFrame(parent, name, CENTER);

	XtVaSetValues(  frame,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			NULL);

	// Vertically stack contents
	Widget RC = XtVaCreateManagedWidget("PrintRC",
			xmRowColumnWidgetClass,
			frame,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmVERTICAL,
			XmNcolumns, 1,
			NULL);

	// File name
	Widget filenameframe = labeledFrame(RC, "FileName", CENTER);
	const char *fnStr = info_->filename_.getString();
	filenameW_ = XtVaCreateManagedWidget("filename",
		xmTextFieldWidgetClass,	filenameframe,
		XmNvalue,		fnStr,
		XmNresizeWidth,		True,
		XmNblinkRate,		0,
		XmNuserData,		(XtPointer)this,
		NULL);
	XtAddCallback( filenameW_, XmNvalueChangedCallback,
			&filenameChangedCB,
			(XtPointer) this);

	// Sliders next to each other.
	Widget sliderRC = XtVaCreateManagedWidget("sliderRC",
			xmRowColumnWidgetClass,
			RC,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmHORIZONTAL,
			XmNcolumns, 1,
			NULL);

	buildDoubleSliderBox(sliderRC, "Scale:",
			     .2, 2, info_->scale_.getValue(), 2,
		  	     TRUE, &info_->scale_);

#if 0
	// Not normally shown. (Currently unused).
	buildDoubleSliderBox(sliderRC, "Aspect Ratio:",
			     .2, 2, info_->aspectRatio_.getValue(), 2,
		  	     TRUE, &info_->aspectRatio_);
#endif
	buildIntMapRadioBox(RC, "layout",
		"Layout", &info_->layout_,
		NULL, &AvPrintConfig::layoutChangedCB,
		(XtPointer) this, GUICB_BEFORE
	);

	Widget toggleRC = XtVaCreateManagedWidget("toggleRC",
			xmRowColumnWidgetClass,
			RC,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmHORIZONTAL,
			XmNcolumns, 1,
			NULL);

#if 0
	// These only apply to the image so ignore for now.
	buildToggleButton(toggleRC, "Flip X", &info_->flipX_);
	buildToggleButton(toggleRC, "Flip Y", &info_->flipY_);
#endif
	buildToggleButton(toggleRC, "Smooth Image", &info_->smoothImage_);

	Widget cmtframe = labeledFrame(RC, "Comments", CENTER);

	// Comment text.
	Widget tsw = XtVaCreateManagedWidget( "commentScroller",
		xmScrolledWindowWidgetClass, cmtframe,
//		XmNscrollingPolicy,	XmAPPLICATION_DEFINED,
		XmNscrollingPolicy,	XmAUTOMATIC,
//		XmNscrollBarDisplayPolicy,	XmSTATIC,
		XmNscrollBarDisplayPolicy,	XmAS_NEEDED,
//		XmNvisualPolicy,	XmVARIABLE,
		XmNvisualPolicy,	XmCONSTANT,
		XmNshadowThickness,	0,
		NULL);

	const char *cmtStr = info_->comment_.getString();
	Widget cmntW = XtVaCreateManagedWidget("comment",
		xmTextWidgetClass,	tsw,
		XmNeditMode,		XmMULTI_LINE_EDIT,
		XmNvalue,		cmtStr,
		XmNcolumns,		48,
		XmNrows,		4,
		NULL);

	// Use the widget's idea of its size to resize the scroll window.
	Dimension width, height;

	XtVaGetValues(cmntW,
		XmNwidth, &width,
		XmNheight, &height,
		 NULL);
	if(width > 0)
		XtVaSetValues(tsw,
			XmNwidth, width,
		 NULL);
	if(height > 0)
		XtVaSetValues(tsw,
			XmNheight, height,
			 NULL);

	XtAddCallback( cmntW, XmNvalueChangedCallback,
			&textChangedCB,
			(XtPointer) this);


	////////////////////////////////////////////////////////////////
	// There are two radio buttons for setting paper size. One is
	// a short list of the most frequently used sizes. The other
	// is the full list of supported sizes which is only shown when
	// requested.

	// When one of the buttons is pressed in the short list, the
	// value is actually set in the paperSize_ variable rather than
	// the shortPaperSize_ variable.

	// Form widget to hold the different editing panels.
	Widget paperFormW = XtVaCreateManagedWidget("paperPanels",
			xmFormWidgetClass,
			RC,
			NULL);

	int defaultSize = info_->paperSize_.getValue();
	shortPaperSize_.init("ShortPaperList", NULL, NULL, ShortPaperList,
			    0, defaultSize);
	// Now that it's initialized, see if defaultSize actually exists.
	// If not, turn on "Other".
	if(shortPaperSize_.valueToIndex(defaultSize) < 0)
		shortPaperSize_.setValue(OTHERPAPERVALUE);

	shortPaperSizeW_ = buildIntMapRadioBox(paperFormW, "shortPaperSize",
		"Paper Size", &shortPaperSize_, &shortPaperSize_,
		handlePaperButtonCB, (XtPointer) this);

	XtVaSetValues(shortPaperSizeW_,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			NULL);

	// Create radio buttons using 'this' as the callback data and
	// the notifier as the button info.
	// The long list of paper sizes isn't in a scrolled window because
	// it's annoying trying to find anything in a reasonably size window.
	paperSizeW_ = buildIntMapRadioBox(paperFormW, "paperSize",
		      "Paper Type:", &info_->paperSize_,
		      &info_->paperSize_,
		      handlePaperButtonCB, (XtPointer) this);

	XtVaSetValues(paperSizeW_,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			NULL);

	// Don't show now.
	XtUnmanageChild(paperSizeW_);
	return frame;
}

////////////////////////////////////////////////////////////////

/* Build the popup window.
*/
void AvPrintConfig::build()
{
Widget parentW;

	dialog = XtVaCreatePopupShell(name_.chars(),
		topLevelShellWidgetClass, ref_,
		XmNallowShellResize, TRUE,
		XmNcolormap,	AvXColorApp::colormap(),
		NULL);

	XtAddCallback(dialog, XmNdestroyCallback, &AvPrintConfig::destroyCB,
							(XtPointer) this);

	// Container for everything.
	parentW = XtVaCreateWidget("parentRC",
			xmRowColumnWidgetClass,
			dialog,
			XmNorientation, XmVERTICAL,
			XmNpacking, XmPACK_TIGHT,
			XmNnumColumns, 1,
			XmNisAligned, FALSE,
			NULL);

	////////////////////////////////////////////////////////////////
	// Done button
	Widget doneW = XtVaCreateManagedWidget("Done",
			xmPushButtonWidgetClass,
			parentW,
			NULL);
	XtAddCallback(  doneW, XmNactivateCallback,
			&AvPrintConfig::doneCB,
			(XtPointer) this);
	buildPrintPanel("Print Options", parentW);

	XtManageChild(parentW);
}

// Return the value of a toggle button.
Boolean AvPrintConfig::getToggle(Widget w)
{
	if(w != NULL)
		return XmToggleButtonGetState(w) ? TRUE : FALSE ;
	else
		return FALSE;
}

// Popup the dialog rebuilding it first if necessary.
void AvPrintConfig::popup()
{
	if(dialog == NULL)
		build();

	XtPopup(dialog, XtGrabNone);
	up_ = TRUE;
	callCallback(POPUP);
}

// Pop down the dialog.
void AvPrintConfig::popdown()
{
	if((dialog != NULL) && isUp())
	{	XtPopdown(dialog);
		up_ = FALSE;
		callCallback(POPDOWN);
	}
}

// converts an XmString to a string.
char *AvPrintConfig::XmStringToString_(XmString string)
{
char *text;

	if(XmStringGetLtoR(string, XmSTRING_DEFAULT_CHARSET, &text))
		return text;
	else
		return NULL;
}

// Destroy the dialog widget if it exists.
// If called with destroyWidgets = FALSE, the widget isn't destroyed, but
// dialog is still set to NULL. (It was called by destroyCB() because
// dialog was already being destroyed.
void    AvPrintConfig::destroy(Boolean destroyWidgets)
{
	if(destroyWidgets && (dialog != NULL))
	{	XtRemoveCallback(dialog, XmNdestroyCallback,
				 &AvPrintConfig::destroyCB,
				 (XtPointer) this);
		XtDestroyWidget(dialog);
	}
	dialog = NULL;
	// The other widgets belong to dialog.
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//		Callbacks for the various buttons.
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

// Called if the popup gets destroyed. (Probably by user).
void AvPrintConfig::destroyCB(Widget, XtPointer cbdata, XtPointer)
//void AvPrintConfig::destroyCB(Widget w, XtPointer cbdata, XtPointer)
{
AvPrintConfig	*me = (AvPrintConfig *)cbdata;

	if(me != NULL)
	{	me->callCallback(DESTROYED);
		me->destroy(FALSE);
		me->up_ = FALSE;
	}
}

// Called when the done text toggle button is pressed.
void AvPrintConfig::doneCB(Widget , XtPointer cbdata, XtPointer)
{AvPrintConfig *me = (AvPrintConfig *)cbdata;

	if(me != NULL)
		me->popdown();
	else
		cerr << "AvPrintConfig::handleHelpCB: NULL CB data.\n";
}

// Call the callback for the particular reason.
void AvPrintConfig::callCallback(const CBReason why)
{
	if(callback_ != NULL)
		(callback_)(this, cbdata_, why);
}

// Add a (the) callback.
void AvPrintConfig::addCallback(AvPrintConfigCallback cb, XtPointer cbdata)
{
	callback_ = cb;
	cbdata_ = cbdata;
}

// Called when user presses a paper size toggle.
//  If a valid paper size is selected in the short list, also set it
// in the long list and the paper size variable. If the "Other" button
// is pressed, unmanage the short list and manage the long list.
//  If a button in the long list is pressed that appears in the short list,
// set it in the short list and unmanage.
void AvPrintConfig::handlePaperButton(Widget w)
{Boolean isSet = getToggle(w);
 XmString	xmstr;
 AvIntMapNotifier *nv;
 XtPointer	xtp;

	if(! isSet)
		return;

	// In order to get radio buttons to turn off 'old' buttons, it
	// is necessary to enable notification when setting a toggle button.
	// However, this will result in a recursive call. toggling_ is used
	// to ignore any callbacks caused by our setting a button.
	if(toggling_)
		return;
	toggling_ = TRUE;

	// Get paper name and the notifier variable
	XtVaGetValues(w,
		XmNlabelString, &xmstr,
		XmNuserData, &xtp,
		 NULL);
	char *wname = XmStringToString_(xmstr);
	nv = (AvIntMapNotifier *)xtp;

	// Set selection in the current notifier.
	nv->setName(wname);
	int value = nv->getValue();	// Value corresponding to name.

	if(value == OTHERPAPERVALUE)	// "Other"
	{	// If yes, turn off short list, turn on long list.
		XtManageChild(paperSizeW_);
		XtUnmanageChild(shortPaperSizeW_);
		toggling_ = FALSE;
		return;
	}

	if( nv == &shortPaperSize_)
	{	// Set value into long list.
		info_->paperSize_.setValue(value);
		// Set corresponding toggle by finding its widget using
		// the widget's name (label).
		const char *name = info_->paperSize_.getName();
		Widget tb = childNameToWidget(paperSizeW_, name);
		XmToggleButtonSetState(tb, True, True);
	}
	else
	{	// Must be a selection in the long list.
		int indx = shortPaperSize_.valueToIndex(value);
		if(indx >= 0)	// Also appears in short list.
		{	shortPaperSize_.setIndex(indx);
			const char *name = shortPaperSize_.getName();
			Widget tb = childNameToWidget(shortPaperSizeW_, name);
			XmToggleButtonSetState(tb, True, True);
		}
		// Go back to short list after any choice.
		XtManageChild(shortPaperSizeW_);
		XtUnmanageChild(paperSizeW_);
	}
	toggling_ = FALSE;
}

void AvPrintConfig::handlePaperButtonCB(Widget w, XtPointer cbdata, XtPointer)
{AvPrintConfig *pc = (AvPrintConfig *) cbdata;

	pc->handlePaperButton(w);
}


// Called whenever the comment widget is changed.
void AvPrintConfig::textChanged(Widget w)
{ char *str;

	str = XmTextGetString(w);
	info_->comment_.setValue(str);
	XtFree(str);
}

void AvPrintConfig::textChangedCB(Widget w, XtPointer cbdata, XtPointer)
{AvPrintConfig *pc = (AvPrintConfig *) cbdata;

	pc->textChanged(w);
}

// Called when the layout (PS/EPS, PORTRAIT/LANDSCAPE) buttons get
// pressed. The sole function is to toggle the filname extension
// between ".ps" and ".eps". Nothing is done if the filename has been
// edited by the user.
void AvPrintConfig::layoutChangedCB(Widget w, XtPointer cbdata, XtPointer)
{AvPrintConfig *pc = (AvPrintConfig *) cbdata;

	pc->layoutChanged(w);
}

void AvPrintConfig::layoutChanged(Widget w)
{Boolean isSet = getToggle(w);

	if(! isSet)
		return;

  int layout = info_->layout_.getValue();
  const Boolean isEPS = ((layout == AvPSDriver::EPS_LANDSCAPE) ||
			 (layout == AvPSDriver::EPS_PORTRAIT));

  const char *fn = info_->filename_.getString();
  int fnlen = (int)strlen(fn);
  const char *dfn = info_->defaultFilename_.getString();
  int deflen = (int)strlen(dfn);

	// If filename's length doesn't match deflen + strlen(".ps")
	// or deflen + strlen(".eps"), filename has been changed.
	// (This check also ensures extension check below doesn't overrun
	//  filename).
	if( (fnlen != (deflen+3)) && (fnlen != (deflen+4)))
		return;

	// If the filename has been changed, don't diddle it.
	if( strncmp(fn, dfn, deflen) != 0)
		return;
  const char *fnextptr = fn+deflen;	// Point to the extension.
  const char *ext = isEPS ? ".eps" : ".ps";

	if(strcmp(fnextptr, ext) == 0)	// Is extension already OK?
		return;

	// Make sure user hasn't supplied a different extension.
	if((strcmp(fnextptr, ".ps") != 0) && ((strcmp(fnextptr, ".eps") != 0)))
		return;
	// Can't use AvString since Motif call want non const char *.
	char *buf = new char[deflen + strlen(ext) +1];
	strcpy(buf,info_->defaultFilename_.getString());
	strcat(buf, ext);
	// Change string. The widget's callback will handle updating the
	// notifier variable.
	XmTextSetString(filenameW_, buf);
}

// Called when the user edits the filename.
void AvPrintConfig::filenameChangedCB(Widget w, XtPointer cbdata, XtPointer)
{AvPrintConfig *pc = (AvPrintConfig *) cbdata;

	pc->filenameChanged(w);
}

void AvPrintConfig::filenameChanged(Widget w)
{ char *str;

	str = XmTextGetString(w);
	info_->filename_.setValue(str);
	XtFree(str);
}
