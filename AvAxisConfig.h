//# Copyright (C) 1995-98 Board of Trustees of the University of Illinois
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
//---------------------------------------------------------------------------
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvAxisConfig.h,v 19.0 2003/07/16 05:46:34 aips2adm Exp $
//
// $Log: AvAxisConfig.h,v $
// Revision 19.0  2003/07/16 05:46:34  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:21  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:41:50  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:41:59  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:02  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:07:35  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:17  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:24  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:57:59  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:51:30  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.3  1998/02/26 22:17:18  hr
// Reordered PanelTypes.
//
// Revision 9.2  1998/02/20 20:40:08  hr
// Minor changes done at same time label controls were changed.
//
// Revision 9.1  1997/11/17 16:55:14  hr
// Added support for axis offsets and captions.
//
// Revision 9.0  1997/08/25 21:23:58  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.2  1997/04/23 20:50:35  hr
// Added a 'friend class ..' to make SunOS compiler happy.
//
// Revision 8.1  1997/04/23 17:13:03  hr
// Changes to support options for drawing curvilinear axes.
//
// Revision 8.0  1997/02/20 03:19:22  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1997/02/09 00:05:09  hr
// Changed from using internal to external part building routines.
//
// Revision 7.1  1997/01/24 20:40:51  hr
// Supplies gui for setting axis parameters.
//
//
//---------------------------------------------------------------------------

/* AvAxisConfig.h
Let user select how axis is displayed.
Creates several buttons and sliders for changing options.
*/
#ifndef AvAxisConfig_h
#define AvAxisConfig_h
#include <X11/Intrinsic.h>
#include "AvAxisOptions.h"
#include "AvAccessor.h"
#include "AvWPosition.h"

class AvConductor;
class AvImageViewItem;
class AvImageWindow;

class AvAxisConfig {
  public:
	// If ivi is non NULL, markers will be positioned via mouse tracking.
	// Otherwise, markers can only be positioned via the config file.
	AvAxisConfig(const char *name, Widget ref, AvAxisOptions *,
		     AvImageViewItem *ivi=NULL);
	virtual ~AvAxisConfig();
	//////////////////////////////////////////////////////////////////
	// Make the window (dis)appear.
	void	popup();
	void	popdown();
	Boolean isUp()const{return up_;}		// Popped up?

	////////////////////////////////////////////////////////////////
	///////		Callback support
	enum CBReason { ACCEPT, CANCEL, ADD, INVALID, MATCH, DELETE,
							HELP, DESTROYED};
	typedef void(*AvAxisConfigCallback)
			(AvAxisConfig *, XtPointer cbdata, const int reason);
	// Only one callback at a time is supported for now.
	void addCallback(AvAxisConfigCallback, XtPointer cbdata);
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	static char *XmStringToString_(XmString string);
	static	Boolean	getToggle(Widget);
	// These are subwindows that are displayed one at a time.
	// The values are used as indexes into an array.
	enum PanelTypes { Global,
			  XYAxes, XAxis, YAxis,
			  XLabel, YLabel, Title,
			  Markers, Captions, NUMPANELS};
	enum AXISFORMATS { DEFAULT, OFFSET};
	static char *Strdup(const char *s);
  protected:
	// Called internally
	void	callCallback(const CBReason);
  private:
	Boolean mayUseCurvilinear()const{return (ivi_ != NULL) ? TRUE : FALSE;}

	void build();	// Builds the windows.
	////////////////////////////////////////////////////////////////
	//		For linear axes
	// Build panel for labels & Title
	Widget labelOptions(const char *name, Widget parentW,
			    const AvAxisOptions::axisLabel *labelOptions);
	// Build panel for axis options.
	Widget linAxisOptions(const char *name, Widget parent,
			      const AvAxisOptions::axis *axisOptions);
	void tickMarks( const char *name, Widget parent,
			const AvAxisOptions::axis *axisOptions,
			const Boolean minorTicks=TRUE);
	////////////////////////////////////////////////////////////////
	Widget crvAxisOptions(const char *name, Widget parent,
			      const AvAxisOptions::axis *axisOptions);
	Widget axesOptions(const char *name, Widget parent);

	////////////////////////////////////////////////////////////////
	//	For either/neither
	// Builds all marker panels.
	Widget buildMarkerPanels(const char *name, Widget parent);
	// Builds individual marker panels.
	Widget buildMarkerPanel(const char *name, Widget parent,
				const AvAxisOptions *, const int index);

	// Builds all caption panels.
	Widget buildCaptionPanels(const char *name, Widget parent);

	Widget buildGlobalsPanel(const char *name, Widget parent);

	// Generates labels for offsets if fmt is 1. Restores default labels
	// if fmt is 0.
	void makeAxisLabels(const int fmt);
	// Is axis type 'angular' (ie RA, DEC).
	Boolean isAngular(const int axis)const;
	// Should axis be displayed in OFFSET format?
	Boolean useOffsetLabel(const int axis)const;
	//		Callbacks
	// 
	static void panelCB(Widget, XtPointer, XtPointer);
	static void handleTFCB(Widget, XtPointer, XtPointer);
	static void handleChooseMarkerCB(Widget, XtPointer, XtPointer);
	static void handleChooseCaptionCB(Widget, XtPointer, XtPointer);
	// Called when Axis Type changes.
	static void handleAxisTypeCB(Widget, XtPointer, XtPointer);
	static void doneCB(Widget, XtPointer, XtPointer); // Done button.
	// Marker track/don't track button.
	static void handleMarkerCB(Widget, XtPointer, XtPointer);
	static void markerTrackCB(XtPointer , AvConductor *, XtPointer);
	// Offset track/don't track button.
	static void handleOffsetCB(Widget, XtPointer, XtPointer);
	// Called when adjusting offset via mouse.
	static void offsetTrackCB(XtPointer , AvConductor *, XtPointer);
	// Called when an offset format button gets pushed.
	static void handleAxisFormatCB(Widget w, XtPointer cbdata, XtPointer);
	// Reset offset to center of image.
	static void resetOffsetCB(Widget w, XtPointer cbdata, XtPointer);
	// Called when "Only Angular Positions" button is pressed.
	static void angularCB(Widget w, XtPointer cbdata, XtPointer);
	void link();
	void unlink();
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
	// The various panels that a user can see. Curvilinear and Linear
	// axes use (mostly) different sets.
	Widget		linPanels_[NUMPANELS];
	Widget		crvPanels_[NUMPANELS];
	Widget		*panels_;
	AvIntMapNotifier	panelNotifier_;
	Boolean		up_;
	AvAxisConfigCallback	callback_;
	XtPointer	cbdata_;
	AvAxisOptions	*info_;		// What gets changed.
	Boolean		toggling_;	// TRUE if changing a toggle that
					// might cause an unwanted callback.
	Widget		offsetW_;	// Parent of axis offset buttons.
					// Used to enable/disable them.
	AvString	xDefLabel_;	// Saved default labels
	AvString	yDefLabel_;
	Widget		trackOriginW_;	// Track origin widget.

	// Used in turning on/off panels.
	static Widget	oldPanelW;
	static Widget	oldPanelToggleW;

	class MarkerData {
	  public:
		MarkerData();
		virtual ~MarkerData();
		void link();
		void unlink();
	  public:
		Widget		panel;
		AvImageWindow	*win;
		const AvAxisOptions::marker *marker;
		// Max pixel values for FRACTION mode.
		double		xMax_, yMax_;
	};
	friend class AvAxisConfig::MarkerData;	// For Sun CC 3.0.1
	MarkerData	*markers_;
	AvImageViewItem	*ivi_;
	Widget		*captions_;
};

#endif
