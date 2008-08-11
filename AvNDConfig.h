//# Copyright (C) 1995-2000 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvNDConfig.h,v 19.0 2003/07/16 05:48:06 aips2adm Exp $
//
// $Log: AvNDConfig.h,v $
// Revision 19.0  2003/07/16 05:48:06  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:40  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:43:08  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:15  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:45  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:44:10  hravlin
// Update copyright.
//
// Revision 14.0  2000/03/23 16:09:12  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:41:03  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:29  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.1  1999/05/21 20:26:36  hravlin
// Added aipsview copyright.
//
//
//---------------------------------------------------------------------------
/* AvNDConfig.h
Let user select how axis is displayed.
Creates several buttons and sliders for changing options.
*/
#ifndef AvNDConfig_h
#define AvNDConfig_h

#include <X11/Intrinsic.h>
#include <AvImageDataItem.h>
#include <AvIPosition.h>
#include <AvString.h>

class RowList;
class AvAccessor;

class AvNDConfig {
  public:
	AvNDConfig(AvImageDataItem::View *view, Widget ref);
	virtual ~AvNDConfig();
	//////////////////////////////////////////////////////////////////
	// Make the window (dis)appear.
	void	popup();
	void	popdown();
	Boolean isUp()const{return up_;}		// Popped up?
	// Change the index for 1/0 relative readouts.
	void	setOffset(const int offset);
	typedef void(*AvNDConfigCallback)
			(AvNDConfig *, XtPointer cbdata,
			 const Boolean destroyed);
	// Only one callback at a time is supported for now.
	void addCallback(AvNDConfigCallback, XtPointer cbdata);
	// Called after done button is pressed to get mapping an start.
	AvIPosition getStart(const Boolean XYZareZero=TRUE)const;
	AvIPosition getShape()const;
	AvIPosition getMap()const;
	const char *getLabel()const;
	Boolean autoDisplay()const;
	int startingPlane()const;
  protected:
  private:
	void build();	// Builds the windows.
	//		Callbacks
	static void doneCB(Widget, XtPointer, XtPointer); // Done button.
	// Called if the popup gets destroyed. (Probably by user).
	static void destroyCB(Widget, XtPointer cbdata, XtPointer);
	// Text field cb
	static void textCB(Widget, XtPointer cbdata, XtPointer);
	// Called to clean up.
	void	destroy(Boolean destroyWidgets);
	void	callCallback(const Boolean destroyed=FALSE);
  private:
	AvString	name_;
	AvImageDataItem::View *view_;
	Widget		ref_;		// Reference widget.
	Widget		dialog;		// Popup dialog widget.
	Widget		text_;
	Widget		autoDisplayW_;
	Boolean		up_;
	RowList		*list_;		// 1 entry for each axis.
	// The callback.
	AvNDConfigCallback	callback_;
	XtPointer	cbdata_;
};

#endif
