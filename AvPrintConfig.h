//# Copyright (C) 1995-99 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvPrintConfig.h,v 19.0 2003/07/16 05:48:12 aips2adm Exp $
//
// $Log: AvPrintConfig.h,v $
// Revision 19.0  2003/07/16 05:48:12  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:45  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:43:13  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:20  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:54  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:09:18  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.2  1999/09/24 21:23:36  hravlin
// Added support for user editable filenames.
//
// Revision 13.1  1999/09/15 21:12:43  hravlin
// Initial checkin.
//
//
//---------------------------------------------------------------------------

/* AvPrintConfig.h
Let user select how axis is displayed.
Creates several buttons and sliders for changing options.
*/
#ifndef AvPrintConfig_h
#define AvPrintConfig_h

#include <X11/Intrinsic.h>
#include <AvPrintOptions.h>

class AvPrintConfig {
  public:
	AvPrintConfig(const char *name, Widget ref, AvPrintOptions *);
	virtual ~AvPrintConfig();
	//////////////////////////////////////////////////////////////////
	// Make the window (dis)appear.
	void	popup();
	void	popdown();
	Boolean isUp()const{return up_;}		// Popped up?

	////////////////////////////////////////////////////////////////
	///////		Callback support
	enum CBReason { ACCEPT=1, CANCEL=2, POPUP=4,
			POPDOWN=8, DESTROYED=0x10};
	typedef void(*AvPrintConfigCallback)
		(AvPrintConfig *, XtPointer cbdata, const int reason);
	// Only one callback at a time is supported for now.
	void addCallback(AvPrintConfigCallback, XtPointer cbdata);
	//////////////////////////////////////////////////////////////////
	static char *XmStringToString_(XmString string);
	static	Boolean	getToggle(Widget);
  protected:
	// Called internally
	void	callCallback(const CBReason);
  private:
	void build();	// Builds the windows.
	void handlePaperButton(Widget w);
	// Build panel for labels & Title
	Widget buildPrintPanel(const char *name, Widget parent);
	//		Callbacks
	// 
	static void doneCB(Widget, XtPointer, XtPointer); // Done button.
	static void textChangedCB(Widget, XtPointer, XtPointer);
	static void filenameChangedCB(Widget, XtPointer, XtPointer);
	static void layoutChangedCB(Widget, XtPointer, XtPointer);
	static void handlePaperButtonCB(Widget w, XtPointer cbdata,
					XtPointer);
	// Called if the popup gets destroyed. (Probably by user).
	static void destroyCB(Widget, XtPointer cbdata, XtPointer);
	////
	void	destroy(Boolean destroyWidgets);
	void	textChanged(Widget);
	void	filenameChanged(Widget textf); // Called when filename changes.
	void	layoutChanged(Widget textf); // Called when filename changes.

  private:
	AvString	name_;
	Widget		ref_;		// Reference widget.
	Widget		dialog;		// Popup dialog widget.
	Boolean		up_;
	AvPrintConfigCallback	callback_;
	XtPointer	cbdata_;
	AvPrintOptions	*info_;		// What gets changed.
	Boolean		toggling_;	// TRUE if changing a toggle that
					// might cause an unwanted callback.
	AvIntMapNotifier	shortPaperSize_;// Only Letter, Legal, A4.
	Widget paperSizeW_;		// IntMapRadioBox container widgets.
	Widget shortPaperSizeW_;
	Widget filenameW_;
};

#endif
