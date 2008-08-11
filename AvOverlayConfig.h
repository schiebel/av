//# Copyright (C) 1995-98 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvOverlayConfig.h,v 19.0 2003/07/16 05:47:52 aips2adm Exp $
//
// $Log: AvOverlayConfig.h,v $
// Revision 19.0  2003/07/16 05:47:52  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:28  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:56  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:03  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:23  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:58  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:47  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:02  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:45  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:55  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.2  1998/02/25 17:08:41  hr
// Added widgets for filename and file selector.
//
// Revision 9.1  1998/02/20 22:01:49  hr
// User interface for overlays.
//
//
//---------------------------------------------------------------------------
/* AvOverlayConfig.h
User interface for overlays.
Creates several buttons and sliders for changing options.
*/
#ifndef AvOverlayConfig_h
#define AvOverlayConfig_h
#include <X11/Intrinsic.h>
#include "AvOverlayOptions.h"
#include "AvAccessor.h"
#include "AvWPosition.h"

class AvConductor;
//class AvImageViewItem;
//class AvImageWindow;

class AvOverlayConfig {
  public:
	AvOverlayConfig(const char *name, Widget ref, AvOverlayOptions *);
	virtual ~AvOverlayConfig();
	//////////////////////////////////////////////////////////////////
	// Make the window (dis)appear.
	void	popup();
	void	popdown();
	Boolean isUp()const{return up_;}		// Popped up?

	////////////////////////////////////////////////////////////////
	///////		Callback support
	enum CBReason { ACCEPT, CANCEL, ADD, INVALID, MATCH, DELETE,
							HELP, DESTROYED};
	typedef void(*AvOverlayConfigCallback)
		(AvOverlayConfig *, XtPointer cbdata, const int reason);
	// Only one callback at a time is supported for now.
	void addCallback(AvOverlayConfigCallback, XtPointer cbdata);
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	static char *XmStringToString_(XmString string);
	static	Boolean	getToggle(Widget);
	static char *Strdup(const char *s);
  protected:
	// Called internally
	void	callCallback(const CBReason);
  private:
	void build();	// Builds the windows.
	Widget buildOverlayPanel(const char *, Widget parent);

	////////////////////////////////////////////////////////////////
	//	For either/neither
	static void doneCB(Widget, XtPointer, XtPointer); // Done button.
	// Called if the popup gets destroyed. (Probably by user).
	static void destroyCB(Widget, XtPointer cbdata, XtPointer);
	////
	void	destroy(Boolean destroyWidgets);
	static void setToggle(Widget, const Boolean,
			      const Boolean Notify=FALSE);
	// Called when "Select Input File" button is pressed. Manages/unmanages
	// the file requester.
	static void selectfilePBCB(Widget, XtPointer ud, XtPointer cbreason);
	// Called when user selects an input filename for vectors.
	static void filenameCB(Widget, XtPointer ud, XtPointer cbreason);
	static void reloadCB(Widget, XtPointer ud, XtPointer cbreason);
  private:
	AvString	name_;
	Widget		ref_;		// Reference widget.
	Widget		dialog;		// Popup dialog widget.
	Widget		fileSelectorW_;	// File selector.
	Widget		filenameW_;	// Holds current filename.
	Boolean		up_;
	AvOverlayConfigCallback	callback_;
	XtPointer	cbdata_;
	AvOverlayOptions	*info_;		// What gets changed.
	Boolean		toggling_;	// TRUE if changing a toggle that
					// might cause an unwanted callback.
};

#endif
