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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvOverlayConfig.cc,v 19.0 2003/07/16 05:47:57 aips2adm Exp $
//
// $Log: AvOverlayConfig.cc,v $
// Revision 19.0  2003/07/16 05:47:57  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:32  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.2  2002/01/30 22:38:35  hravlin
// Initialized a boolean that g++ thought might be used unitialized.
//
// Revision 17.1  2002/01/07 22:32:52  hravlin
// Removed check for __P.
// ,
// lookw
//
// Revision 17.0  2001/11/12 19:43:00  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:07  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:30  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:40:15  hravlin
// Replaced XmStringCreateLocalized() by AvXmStringCreateLocalized().
//
// Revision 14.0  2000/03/23 16:09:03  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  2000/02/29 22:20:28  hravlin
// The constructor wasn't ref'ing info_.
//
// Revision 13.0  1999/08/10 18:40:53  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:12  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:58  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:56:11  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.4  1998/05/04 16:49:02  hr
// Create shell using aipsview's colormap.
//
// Revision 9.3  1998/02/25 17:08:01  hr
// Removed color selector and addded filename label.
//
// Revision 9.2  1998/02/20 22:36:53  hr
// Removed g++ compiler warnings.
//
// Revision 9.1  1998/02/20 22:01:26  hr
// User interface for overlays.
//
//
//---------------------------------------------------------------------------
/* Av OverlayConfig.cc
Build setup window to change overlay parameters.
*/

#include <iostream.h>
#include <stdio.h>
#include <string.h>
//#if defined(__GNUC__) && defined(__P) && !defined(__linux__)
//#undef __P
//#endif
//#include <math.h>

#include <sys/types.h>		// For stat call.
#include <sys/stat.h>
#include <errno.h>

#include <Xm/Xm.h>
#include <Xm/PushB.h>
#include <Xm/TextF.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>
#include <Xm/Separator.h>
#include <Xm/ToggleB.h>
#include <Xm/Form.h>
#include <Xm/FileSB.h>

#include <AvOverlayConfig.h>
#include <AvImageWindow.h>
#include <AvImageViewPanel.h>
#include <AvImageViewItem.h>
#include <AvGUIParts.h>
#include <AvWidgetMisc.h>

////////////////////////////////////////////////////////////////
AvOverlayConfig::AvOverlayConfig(const char *name, Widget ref,
				 AvOverlayOptions *info)

{
	name_ = "Setup for ";
	name_ += name;
	name_ += ":";
	ref_ = ref;
	dialog = NULL;
	up_ = FALSE;
	info_ = info;
	if(info != NULL)	// ERROR!!!
	{	info_->ref();
		name_ += info->config()->moduleName();
	}
	callback_ = NULL;
	cbdata_ = NULL;
	toggling_ = FALSE;
}

AvOverlayConfig::~AvOverlayConfig()
{
	popdown();
	destroy(TRUE);		// Get rid of any windows.
	info_->unref();
}

////////////////////////////////////////////////////////////////
// Build a overlay panel.
////////////////////////////////////////////////////////////////
Widget AvOverlayConfig::buildOverlayPanel(const char *, Widget parent)
{AvOverlayOptions *info = info_;

	Widget overlayFrame = labeledFrame(parent, "Overlays", CENTER);
	XtVaSetValues(overlayFrame,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			NULL);

	Widget overlayRC = XtVaCreateWidget("overlayRC",
			xmRowColumnWidgetClass,
			overlayFrame,
			XmNorientation, XmVERTICAL,
			XmNpacking, XmPACK_TIGHT,
			XmNnumColumns, 1,
			NULL);

	////////////////////////////////////////////////////////////////
	//		Filename

	XtVaCreateManagedWidget("Filename:",
		xmLabelWidgetClass,
		overlayRC,
		NULL);
	{	// Create a file requester with initial path that of the
		// default filename.
	  char *dir = Strdup(info->filename_.getString());
	  char *name = strrchr(dir, '/');	// Look for last '/';
	  XmString fullname = XmStringCreateLocalized(dir);

		if(name != NULL)
			*++name = '\0';	// NULL terminate dir after '/'.
		else	// Current directory.
		{	delete [] dir;
			dir = Strdup("./");
		}
		XmString dirspec = XmStringCreateLocalized(dir);
		XmString pat = AvXmStringCreateLocalized("*.overlay");

		filenameW_ =  XtVaCreateManagedWidget("fileNameW",
				xmLabelWidgetClass,
				overlayRC,
				XmNlabelString, fullname,
				XmNalignment, XmALIGNMENT_END,
				NULL);

	 	fileSelectorW_ = XtVaCreateWidget("fileSelectorW_",
			xmFileSelectionBoxWidgetClass,
			overlayRC,
			XmNdirectory, dirspec,
			XmNpattern, pat,
			NULL);

		XtVaSetValues(fileSelectorW_,
			XmNdirSpec, fullname,
			NULL);

		XmStringFree(fullname);
		XmStringFree(dirspec);
		XmStringFree(pat);
		delete [] dir;
	}

	XtAddCallback(fileSelectorW_, XmNokCallback,
                 (XtCallbackProc) &AvOverlayConfig::filenameCB,
                 (XtPointer) this);
	XtAddCallback(fileSelectorW_, XmNcancelCallback,
                 (XtCallbackProc) &AvOverlayConfig::filenameCB,
                 (XtPointer) this);

	////////////////////////////////////////////////////////////////
	Widget showRC = XtVaCreateManagedWidget("showRC",
			xmRowColumnWidgetClass,
			overlayRC,
			XmNorientation, XmHORIZONTAL,
			XmNpacking, XmPACK_TIGHT,
			XmNnumColumns, 1,
			NULL);

	// filePBW is used to manage/unmanage fileSelectorW_.
	Widget filePBW = XtVaCreateManagedWidget("Select Input File",
			xmPushButtonWidgetClass,
			showRC,
			NULL);
	XtAddCallback(  filePBW, XmNactivateCallback,
			&AvOverlayConfig::selectfilePBCB,
			(XtPointer) this);

	// Push button to force a reload of current file.
	Widget reloadW = XtVaCreateManagedWidget("Reload",
				xmPushButtonWidgetClass,
				showRC,
				NULL);
	XtAddCallback(  reloadW, XmNactivateCallback,
			&AvOverlayConfig::reloadCB,
			(XtPointer) info_);

	Widget sizeRC = XtVaCreateManagedWidget("sizeRC",
			xmRowColumnWidgetClass,
			overlayRC,
			XmNorientation, XmHORIZONTAL,
			XmNpacking, XmPACK_TIGHT,
			XmNnumColumns, 1,
			NULL);

#if 0
	buildIntegerSliderBox(sizeRC, "Width",
		  	      1, 10,
		  	      info->width_.getValue(),
		  	      TRUE, &info->width_);

#endif
	buildDoubleSliderBox(sizeRC, "Vector Scale",
		  .1, 10.0, info->scale_.getValue(), 1,
		  TRUE, &info->scale_);

#if 0
	buildColorSelector(overlayRC, "color", "Color", &info->color_);
	buildIntMapRadioBox(overlayRC, "ColorChoice", "ColorChoice:",
			    &info->colorChoice_);
#endif
	XtManageChild(overlayRC);
	return overlayFrame;
}


/* Build the popup window.
*/
void AvOverlayConfig::build()
{
Widget parentW;

	// Building the window takes a long time, so let the user know
	// something is happening.
	AvXColorApp::setBusy();

	dialog = XtVaCreatePopupShell(name_.chars(),
		topLevelShellWidgetClass, ref_,
		XmNallowShellResize, TRUE,
		XmNcolormap,	AvXColorApp::colormap(),
		NULL);

	XtAddCallback(dialog, XmNdestroyCallback, &AvOverlayConfig::destroyCB,
							(XtPointer) this);

	// Container for everything.
	parentW = XtVaCreateWidget("OverlayRC",
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
			&AvOverlayConfig::doneCB,
			(XtPointer) this);

	buildOverlayPanel("overlays", parentW);
	XtManageChild(parentW);
	AvXColorApp::resetBusy();
}

// Return the value of a toggle button.
Boolean AvOverlayConfig::getToggle(Widget w)
{
	if(w != NULL)
		return XmToggleButtonGetState(w) ? TRUE : FALSE ;
	else
		return FALSE;
}

// Change a toggle and optionally notify its callbacks. Usually Notify
// is FALSE, but radio toggles seem to need the notification in order to
// turn off the others in the list. However, this will cause our callbacks
// to be called too. "toggling_" is used to make sure we don't try to
// change info_ unnecessarily.
void AvOverlayConfig::setToggle(Widget w, const Boolean set, const Boolean Notify)
{
	if(w != NULL)
	{	XmToggleButtonSetState(w, set, Notify);
		if(XmToggleButtonGetState(w) != set)
		{	XmToggleButtonSetState(w, set, Notify);
//			printf("%d %d\n", set, XmToggleButtonGetState(w));
		}
	}
}

// Popup the dialog rebuilding it first if necessary.
void AvOverlayConfig::popup()
{
	if(dialog == NULL)
		build();

	// Make a copy of current settings for undos.
//	*backInfo_ = *info_;
	XtPopup(dialog, XtGrabNone);
	up_ = TRUE;
}

// Pop down the dialog.
void AvOverlayConfig::popdown()
{
	if((dialog != NULL) && isUp())
	{	XtPopdown(dialog);
		up_ = FALSE;
	}
}

// converts an XmString to a string.
char *AvOverlayConfig::XmStringToString_(XmString string)
{
char *text;

	if(XmStringGetLtoR(string, XmSTRING_DEFAULT_CHARSET, &text))
		return text;
	else
		return NULL;
}

char *AvOverlayConfig::Strdup(const char *s)
{
char	*str;

	if(s == NULL)
		return NULL;
	str = new char [strlen(s) + 1];
	strcpy(str, s);
	return str;
}

// Destroy the dialog widget if it exists.
// If called with destryWidgets = FALSE, the widget isn't destroyed, but
// dialog is still set to NULL. (It was called by destroyCB() because
// dialog was already being destroyed.
void    AvOverlayConfig::destroy(Boolean destroyWidgets)
{
	if(destroyWidgets && (dialog != NULL))
	{	XtRemoveCallback(dialog, XmNdestroyCallback,
				 &AvOverlayConfig::destroyCB,
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
void AvOverlayConfig::destroyCB(Widget, XtPointer cbdata, XtPointer)
//void AvOverlayConfig::destroyCB(Widget w, XtPointer cbdata, XtPointer)
{
AvOverlayConfig	*me = (AvOverlayConfig *)cbdata;

//	printf("AvOverlayConfig::DestroyCB called for widget: %s\n", XtName(w));
	if(me != NULL)
	{	me->callCallback(DESTROYED);
		me->destroy(FALSE);
		me->up_ = FALSE;
	}
}

// Called when the done text toggle button is pressed.
void AvOverlayConfig::doneCB(Widget , XtPointer cbdata, XtPointer)
{AvOverlayConfig *me = (AvOverlayConfig *)cbdata;

	if(me != NULL)
		me->popdown();
	else
		cerr << "AvOverlayConfig::handleHelpCB: NULL CB data.\n";
}

////////////////////////////////////////////////////////////////
// Called when "Select Input File" button is pressed. Toggles whether
// the file requester is managed. Also toggles showing of the filename widget.
void AvOverlayConfig::selectfilePBCB(Widget , XtPointer cbdata, XtPointer )
{AvOverlayConfig *me = (AvOverlayConfig *) (Widget) cbdata;

	if(XtIsManaged(me->fileSelectorW_))
	{
		XtManageChild(me->filenameW_);
		XtUnmanageChild(me->fileSelectorW_);
	}
	else
	{	XtManageChild(me->fileSelectorW_);
		XtUnmanageChild(me->filenameW_);
	}
}

// Called when user selects an overlay input file.
void AvOverlayConfig::filenameCB(Widget w, XtPointer cbdata, XtPointer cbd_)
{AvOverlayConfig *me = (AvOverlayConfig *)cbdata;
 AvOverlayOptions *v = me->info_;

 XmFileSelectionBoxCallbackStruct *cbd =
 			(XmFileSelectionBoxCallbackStruct *)cbd_;
 char *filename;

	if(cbd->reason == XmCR_CANCEL)	// User pressed Cancel.
	{	XtManageChild(me->filenameW_);
		XtUnmanageChild(w);
		return;
	}
	if(! XmStringGetLtoR(cbd->value, XmFONTLIST_DEFAULT_TAG, &filename))
	{	cout
		    << "AvOverlayConfig::filenameCB Could not decode filename."
		    << endl;
		return;
	}

	// Stat the file to make sure it's not a directory.
	struct stat buf;
	if( stat(filename, &buf) < 0)	// Couldn't stat.
	{	perror(filename);
		XtFree(filename);
		return;
	}
	if((buf.st_mode & S_IFREG) == 0)
	{	cerr << filename << " is not a regular file." << endl;
		XtFree(filename);
		return;
	}
	// Try to open the file. If it's readable continue. Otherwise warn user
	// and return.
	FILE *fptr = fopen(filename, "r");
	if(fptr == NULL)
	{	cerr << "Could not open " << filename << endl;
		return;
	}
	fclose(fptr);

	// File is at least readable. Set the variable then unmanage the
	// Widget.
	// Set filename w/o path into filename widget.
	char *fn = strrchr(filename, '/');
	XmString xmfn;
	if(fn == NULL)
		xmfn = cbd->value;			// No path components.
	else
		xmfn = XmStringCreateLocalized(fn+1);	// Skip '/'.

	XtVaSetValues(me->filenameW_,
		XmNlabelString, xmfn,
		NULL);
	if(fn != NULL)
		XmStringFree(xmfn);

	// Fullpath name goes into notifier variable.
	v->filename_.setValue(filename);
	XtFree(filename);
	XtManageChild(me->filenameW_);
	XtUnmanageChild(w);
}

void AvOverlayConfig::reloadCB(Widget , XtPointer cbdata, XtPointer )
{ AvOverlayOptions * ovr = (AvOverlayOptions *)cbdata;

	if(ovr != NULL)
		ovr->touch();
}

////////////////////////////////////////////////////////////////

// Call the callback for the particular reason.
void AvOverlayConfig::callCallback(const CBReason why)
{
Boolean immediate = FALSE;

	switch(why) {
	case ACCEPT:
	case CANCEL:
	case HELP:
	case DESTROYED:
		immediate = TRUE;
		break;
	case ADD:
	case DELETE:
//		saveValues(info_);	// Update the Overlay level list.
	case INVALID:
		break;
	default:
		immediate = FALSE;		// Oops.
		break;
	}

	if((callback_ != NULL) && immediate)
		(callback_)(this, cbdata_, why);
}

// Add a callback.
void AvOverlayConfig::addCallback(AvOverlayConfigCallback cb, XtPointer cbdata)
{
	callback_ = cb;
	cbdata_ = cbdata;
}
