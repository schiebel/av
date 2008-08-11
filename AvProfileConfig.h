//# Copyright (C) 1995-97 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvProfileConfig.h,v 19.0 2003/07/16 05:47:58 aips2adm Exp $
//
// $Log: AvProfileConfig.h,v $
// Revision 19.0  2003/07/16 05:47:58  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:34  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:43:02  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:08  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:32  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:09:04  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:54  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:14  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:02:03  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:56:16  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:31:24  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.1  1997/08/01 21:20:29  hr
// Changed CBReason.
//
// Revision 8.0  1997/02/20 03:19:18  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1997/02/09 00:17:04  hr
// Cleanup and migration of most part building to AvGUIParts.
//
// Revision 7.1  1997/02/05 17:58:22  hr
// GUI for setting up profile printouts.
//
//
//---------------------------------------------------------------------------

/* AvProfileConfig.h
Let user select how axis is displayed.
Creates several buttons and sliders for changing options.
*/
#ifndef AvProfileConfig_h
#define AvProfileConfig_h

#include <X11/Intrinsic.h>
#include <AvProfileOptions.h>

class AvConductor;
class AvImageViewItem;
class AvImageWindow;

class AvProfileConfig {
  public:
	// If ivi is non NULL, markers will be positioned via mouse tracking.
	// Otherwise, markers can only be positioned via the config file.
	// (Since marker positioning isn't supported, ivi is currently
	//  ignored).
	AvProfileConfig(const char *name, Widget ref, AvProfileOptions *,
		     AvImageViewItem *ivi=NULL);
	virtual ~AvProfileConfig();
	//////////////////////////////////////////////////////////////////
	// Make the window (dis)appear.
	void	popup();
	void	popdown();
	Boolean isUp()const{return up_;}		// Popped up?

	////////////////////////////////////////////////////////////////
	///////		Callback support
#if 0
	enum CBReason { ACCEPT, CANCEL, ADD, INVALID, MATCH, DELETE,
							HELP, DESTROYED};
#else
	enum CBReason { ACCEPT=1, CANCEL=2, POPUP=4,
			POPDOWN=8, DESTROYED=0x10};
#endif
	typedef void(*AvProfileConfigCallback)
		(AvProfileConfig *, XtPointer cbdata, const int reason);
	// Only one callback at a time is supported for now.
	void addCallback(AvProfileConfigCallback, XtPointer cbdata);
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	static char *XmStringToString_(XmString string);
	static	Boolean	getToggle(Widget);
	// These are subwindows that are displayed one at a time.
	enum PanelTypes { Global, Profile, Output, XLabel, YLabel, Title,
			  Markers, XAxis, YAxis,  NUMPANELS};

	static char *Strdup(const char *s);
  protected:
	// Called internally
	void	callCallback(const CBReason);
  private:
	void build();	// Builds the windows.
	// Build panel for labels & Title
	Widget labelOptions(const char *name, Widget parentW,
			    const AvProfileOptions::axisLabel *labelOptions);
	// Build panel for axis options.
	Widget axisOptions(const char *name, Widget parent,
			   const AvProfileOptions::axis *axisOptions);
	void tickMarks(const char *name, Widget parent,
			  const AvProfileOptions::axis *axisOptions);
	Widget buildMarkerPanel(const char *name, Widget parent,
				const AvProfileOptions *, const int index);

	Widget buildProfilePanel(const char *name, Widget parent);
	Widget buildOutputPanel(const char *name, Widget parent);

	Widget buildGlobalsPanel(const char *name, Widget parent);

	//		Callbacks
	// 
	static void handleChooseMarkerCB(Widget, XtPointer, XtPointer);
	static void panelCB(Widget, XtPointer, XtPointer);
	static void doneCB(Widget, XtPointer, XtPointer); // Done button.
	static void handleTFCB(Widget, XtPointer, XtPointer);
	// Called if the popup gets destroyed. (Probably by user).
	static void destroyCB(Widget, XtPointer cbdata, XtPointer);
	////
	void	destroy(Boolean destroyWidgets);
	static void setToggle(Widget, const Boolean,
			      const Boolean Notify=FALSE);
  private:
	AvString	name_;
	Widget		ref_;		// Reference widget.
	Widget		dialog;		// Popup dialog widget.
	Widget		panels_[NUMPANELS];
	AvIntMapNotifier	panelNotifier_;

	Boolean		up_;
	AvProfileConfigCallback	callback_;
	XtPointer	cbdata_;
	AvProfileOptions	*info_;		// What gets changed.
	Boolean		toggling_;	// TRUE if changing a toggle that
					// might cause an unwanted callback.
	// Used in turning on/off panels.
	static Widget	oldPanelW;
	static Widget	oldPanelToggleW;

	class MarkerData {
	  public:
		MarkerData();
		~MarkerData();
		void link();
		void unlink();
	  public:
		Widget		panel;
		AvImageWindow	*win;
		const AvProfileOptions::marker *marker;
	};

	MarkerData	*markers_;
	AvImageViewItem	*ivi_;
};

#endif
