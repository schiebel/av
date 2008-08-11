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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvProfileConfig.cc,v 19.0 2003/07/16 05:47:41 aips2adm Exp $
//
// $Log: AvProfileConfig.cc,v $
// Revision 19.0  2003/07/16 05:47:41  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:18  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.2  2002/01/22 22:16:13  hravlin
// PtrToInt() is now in Av.h.
//
// Revision 17.1  2002/01/07 22:38:00  hravlin
// *** empty log message ***
//
// Revision 17.0  2001/11/12 19:42:47  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:54  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:51  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:46  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.2  2000/02/29 22:19:15  hravlin
// Under some circumstances, destroy() would delete markers_ twice.
//
// Revision 13.1  1999/08/25 19:43:51  hravlin
// Fixed problem with casting from XtPointer to enum.
//
// Revision 13.0  1999/08/10 18:40:35  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:40  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:14  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:18  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.2  1998/05/04 16:49:57  hr
// Create shell using aipsview's colormap.
//
// Revision 9.1  1997/10/31 21:58:18  hr
// Enabled code to allow user to adjust label positions.
//
// Revision 9.0  1997/08/25 21:29:56  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.3  1997/08/01 21:17:50  hr
// Viewport size is now a slider. Moved axis flip to profile panel.
//
// Revision 8.2  1997/06/23 20:02:05  hr
// Added buttons to allow user to flip x (or y) axis.
//
// Revision 8.1  1997/05/13 20:56:05  hr
// Changes to panel flipping would work with Motif 2.x. Changed gadgets to
// widgets.
//
// Revision 8.0  1997/02/20 03:19:32  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.5  1997/02/10 16:05:05  hr
// Removed an assignment to an unused widget.
//
// Revision 7.4  1997/02/09 22:27:49  bglenden
// __P and linux problem.
//
// Revision 7.3  1997/02/09 00:29:54  hr
// Removed an unused variable.
//
// Revision 7.2  1997/02/09 00:14:47  hr
// Cleanup and migration of most part building to AvGUIParts.
//
// Revision 7.1  1997/02/05 17:57:35  hr
// GUI for setting up profile printouts.
//
//
//---------------------------------------------------------------------------

#include <iostream.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <Xm/Xm.h>
#include <Xm/PushB.h>
/////////
#include <Xm/TextF.h>
#include <Xm/Frame.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>
#include <Xm/Separator.h>
#include <Xm/ToggleB.h>		// & For XmToggleButtonSetState()
#include <Xm/Form.h>

#include "AvProfileConfig.h"
#include "AvImageWindow.h"
#include "AvImageViewPanel.h"
#include "AvImageViewItem.h"
#include "AvPG2Pixmap.h"
#include "AvPGDriver.h"
#include "AvGUIParts.h"
#include "Av.h"

// This was copied from AvAxisConfig and still has alot of its code in
// case an interactive profile display window ever gets written.

///	Table to convert between strings and values.

// The different panels that can be displayed (one at a time).
// Be careful of commenting out entries. The enum value gets used as
// an index into the panels array.
static AvProfileOptions::IntMap Panels[] = {
		{ AvProfileConfig::Global, "Global"},
		{ AvProfileConfig::Profile, "Profile"},
		{ AvProfileConfig::Output, "Output"},
		{ AvProfileConfig::XLabel, "X Label"},
		{ AvProfileConfig::YLabel, "Y Label"},
		{ AvProfileConfig::Title, "Title"},
		{ AvProfileConfig::Markers, "Markers"},
#if 0
		{ AvProfileConfig::XAxis, "X Axis"},
		{ AvProfileConfig::YAxis, "Y Axis"},
#endif
		{ 0, NULL}
	};

// This is used to help text fields and buttons of a radioTextNotifier
// communicate with each other.
//static class ProfiletfHandler {
class ProfiletfHandler {
  public:
	ProfiletfHandler();
	Widget	button;		// Button to turn on text field.
	Widget	field;		// The text field.
	const	AvRadioTextNotifier *tfn;
	static void handleTFCB(Widget, XtPointer, XtPointer);
	// Called when button is pressed.
	static void handleBtnCB(Widget, XtPointer, XtPointer);
	void showString(const char *);
};

#if 1
/////////////////////////////////
// When we want to be called.
#if 0
static const int MOUSECHANGE =
	AvConductor::MouseDown | AvConductor::MouseMove | AvConductor::MouseUp;
#endif

AvProfileConfig::MarkerData::MarkerData()
{
	panel = NULL;
	win = NULL;
	marker = NULL;
}

AvProfileConfig::MarkerData::~MarkerData()
{
	unlink();
}

// Connect to the image window's mouse tracker.
void AvProfileConfig::MarkerData::link()
{
#if 0
	if(win != NULL)
		win->getTracker()->addCallback(&AvProfileConfig::markerTrackCB,
					MOUSECHANGE, NULL,
					(XtPointer )this);
#endif
}

// Disconnect from the image window's mouse tracker.
void AvProfileConfig::MarkerData::unlink()
{
#if 0
	if(win != NULL)
	  win->getTracker()->removeCallback(&AvProfileConfig::markerTrackCB,
					  MOUSECHANGE, NULL,
					  (XtPointer )this);
#endif
}
#endif

////////////////////////////////////////////////////////////////
AvProfileConfig::AvProfileConfig(const char *name, Widget ref,
				 AvProfileOptions *ao, AvImageViewItem *ivi)
{
	name_ = "Setup for ";
	name_ += name;
	name_ += ":";
	name_ += ao->config()->moduleName();
	ref_ = ref;
	dialog = NULL;
	ivi_ = ivi;	// Ivi is is being initialized, so be careful.

	for(int i=0; i < NUMPANELS; i++)
		panels_[i] = NULL;
	panelNotifier_.init("Panels", NULL, NULL, Panels, 0, Global);
	up_ = FALSE;
	callback_ = NULL;
	cbdata_ = NULL;
	info_ = ao;
	if(info_ != NULL)
		info_->ref();

//	backInfo_ = new AvProfileOptions(*info_);
//	backInfo_->ref();
	toggling_ = FALSE;
	markers_ = NULL;
}

AvProfileConfig::~AvProfileConfig()
{
	popdown();
	destroy(TRUE);		// Get rid of any windows.
	info_->unref();
}


// Build an option box for title, X or Y axes.
Widget AvProfileConfig::labelOptions(const char *name, Widget parentW,
			  const AvProfileOptions::axisLabel *options)

{
	// Frame around everything.
	Widget radioFrame = labeledFrame(parentW, name, CENTER);

	// Vertically stack contents
	Widget optionsRC = XtVaCreateManagedWidget(name,
			xmRowColumnWidgetClass,
			radioFrame,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmVERTICAL,
			NULL);

	XtVaCreateManagedWidget("",
				xmSeparatorWidgetClass,
				optionsRC,
				NULL);


	////////////////////////////////////////////////////////////////
	// Toggles describing how to display axis.
	////////////////////////////////////////////////////////////////
	//		Axis text
	Widget textBox = buildIntMapRadioBox(optionsRC, "label", "Label:",
				  &options->text_);

	Widget tfW = XtVaCreateManagedWidget("axisTF",
			xmTextFieldWidgetClass,
			optionsRC,
			XmNvalue, options->text_.getString(),
			NULL);
	XtAddCallback(tfW, XmNactivateCallback, &AvProfileConfig::handleTFCB,
		      (XtPointer) &options->text_);
	// When the text field is activated, the "TextField" button needs to
	// be on. When the "Default" or "TextField" buttons are pressed, the
	// appropriate string should be displayed. ProfiletfHandler links them.
	ProfiletfHandler *tfh = new ProfiletfHandler();
	tfh->field = tfW;
	tfh->tfn = &options->text_;
	// Pick up the widget for the "TextField" button.
	{ const AvRadioTextNotifier *rn = &options->text_;
	  Widget uw, dw;
	  int uv = rn->userValue();
	  int ui = rn->valueToIndex(uv);
	  int dv = rn->defaultValue();
	  int di = rn->valueToIndex(dv);
		uw = childNameToWidget(textBox, rn->getName(ui));
	  	tfh->button = uw;
		XtAddCallback(uw, XmNvalueChangedCallback,
			&ProfiletfHandler::handleBtnCB,
			(XtPointer) tfh);
		dw = childNameToWidget(textBox, rn->getName(di));
		XtAddCallback(dw, XmNvalueChangedCallback,
			&ProfiletfHandler::handleBtnCB,
			(XtPointer) tfh);

		XtAddCallback(tfW, XmNactivateCallback,
			      &ProfiletfHandler::handleTFCB,
			      (XtPointer) tfh);
	}

	// The 'help' button.
	showEscapeCodes(optionsRC);

	buildIntMapRadioBox(  optionsRC, "sideToggles", "Side:",
			&options->side_);

	////////////////////////////////////////////////////////////////
	// Axis character scale

	buildDoubleSliderBox(	optionsRC, "Character Scale:",
				.1, 10.0, options->charScale_.getValue(), 1,
				TRUE,
				&options->charScale_);

	////////////////////////////////////////////////////////////////
	// Text position
	Widget placementFrame = labeledFrame(optionsRC, "Placement:", LEFT);

	Widget placementRC = XtVaCreateManagedWidget("placementRC",
			xmRowColumnWidgetClass,
			placementFrame,
			XmNorientation, XmVERTICAL,
			XmNpacking, XmPACK_TIGHT,
			NULL);

	buildDoubleSliderBox(placementRC, "Displacement From Edge:",
		  -10.0, 10.0, options->fromEdge_.getValue(), 1,
		  FALSE,
		  &options->fromEdge_);

	////////////////////////////////////////////////////////////////
	// Frame around all the Justification stuff.
	Widget justifyFrame = labeledFrame(optionsRC, "Justification", LEFT);

	Widget justifyRC = XtVaCreateManagedWidget("justifyRC",
			xmRowColumnWidgetClass,
			justifyFrame,
			XmNorientation, XmVERTICAL,
			XmNpacking, XmPACK_TIGHT,
			NULL);

	buildDoubleSliderBox(justifyRC, "Displacement along edge:",
		  0, 1.0, options->alongEdge_.getValue(),2,
		  FALSE,
		  &options->alongEdge_);

	buildIntMapRadioBox(justifyRC, "Justification:",
		 "Justification:",
		 &options->justify_);

	return radioFrame;
}

// Builds the tick mark controls.
#if 0
void AvProfileConfig::tickMarks(const char *name, Widget parent,
			     const AvProfileOptions::axis *options)
{
	Widget boxRC = XtVaCreateManagedWidget(name,
			xmRowColumnWidgetClass,
			parent,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmVERTICAL,
			NULL);

	/////////////////////////////////////////////////////////////////
	//			Major tick marks
	XtVaCreateManagedWidget("Major tick intervals",
				xmLabelWidgetClass,
				boxRC,
				NULL);

	Widget majorRC = XtVaCreateManagedWidget(name,
			xmRowColumnWidgetClass,
			boxRC,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmHORIZONTAL,
			NULL);
	// Show tick marks?
	buildIntMapRadioBox(majorRC, "majorTicks",
		      NULL, &options->major_);

	buildIntegerSliderBox( majorRC, NULL,
			1, 16, options->majorIntervals_.getValue(),
			FALSE,
			&options->majorIntervals_);

	/////////////////////////////////////////////////////////////////
	//			Minor tick marks
	XtVaCreateManagedWidget("Minor tick intervals",
				xmLabelWidgetClass,
				boxRC,
				NULL);

	Widget minorRC = XtVaCreateManagedWidget(name,
			xmRowColumnWidgetClass,
			boxRC,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmHORIZONTAL,
			NULL);
	buildIntMapRadioBox(  minorRC, "minorTicks", NULL,
			&options->minor_);
	// Want int slider!!
	buildDoubleSliderBox( minorRC, NULL,
			0.0, 10.0, options->minorIntervals_.getValue(),
			0, FALSE,
			&options->minorIntervals_);
}
#else
void AvProfileConfig::tickMarks(const char *, Widget ,
			     const AvProfileOptions::axis *)
{}
#endif

// Builds the "Grid and TickMarks panel.
Widget AvProfileConfig::axisOptions(const char *, Widget ,
				 const AvProfileOptions::axis *)
{
	return NULL;
}

////////////////////////////////////////////////////////////////


// Build a marker panel for marker 'index'. Since placement is dependent
// on being able to track the mouse, if there is no ImageViewPanel, tracking
// isn't supported.
Widget AvProfileConfig::buildMarkerPanel(const char *, Widget parent,
				      const AvProfileOptions *info,
				      const int index)
{char buf[64];
 const AvProfileOptions::marker *marker = info->getMarkerOptions(index);

	sprintf(buf, "Marker_%02d", marker->markerIndex_);
	Widget markerFrame = labeledFrame(parent, buf);
	XtVaSetValues(markerFrame,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			NULL);

	Widget markerRC = XtVaCreateWidget("markerRC",
			xmRowColumnWidgetClass,
			markerFrame,
			XmNorientation, XmVERTICAL,
			XmNpacking, XmPACK_TIGHT,
//			XmNpacking, XmPACK_COLUMN,
//			XmNpacking, XmPACK_NONE,
			XmNnumColumns, 1,
			NULL);

	Widget showPlaceRC = XtVaCreateManagedWidget("showPlaceRC",
			xmRowColumnWidgetClass,
			markerRC,
			XmNorientation, XmHORIZONTAL,
			XmNpacking, XmPACK_TIGHT,
//			XmNpacking, XmPACK_COLUMN,
//			XmNpacking, XmPACK_NONE,
			XmNnumColumns, 1,
			NULL);


	buildToggleButton(showPlaceRC, "Show Markers", &marker->show_);

	buildDoubleSliderBox(markerRC, "Scale",
			     .1, 10.0, marker->scale_.getValue(), 1,
		  	     TRUE, &marker->scale_);

	// Build a radio box for the marker type toggles.
	buildMarkerSelector(  markerRC, "Type", "Marker Type:",
					      &marker->type_);

////////////////////////////////////////////////////////////////
	markers_[index].panel = markerFrame;
	if(ivi_ != NULL)
		markers_[index].win = ivi_->getImageWindow();
	else
		markers_[index].win = NULL;
	markers_[index].marker = marker;

	buildColorSelector(markerRC, "Color", "Marker Color", &marker->color_);

	XtManageChild(markerRC);
	return markerFrame;
}

////////////////////////////////////////////////////////////////
// 			Output Options Panel
////////////////////////////////////////////////////////////////

Widget AvProfileConfig::buildOutputPanel(const char *name, Widget parent)
{
	Widget outputFrame = labeledFrame(parent, name, CENTER);
	XtVaSetValues(  outputFrame,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			NULL);

	// Vertically stack contents
	Widget outputRC = XtVaCreateManagedWidget("outputRC",
			xmRowColumnWidgetClass,
			outputFrame,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmVERTICAL,
			XmNcolumns, 1,
			NULL);


	buildIntMapRadioBox(outputRC, "OutputFormat", "Output Format:",
			&info_->gPSDevice_);
#if 0
	buildIntMapRadioBox(outputRC, "OutputSize", "Output Size:",
			&info_->gViewportSize_);
#else
	buildDoubleSliderBox(outputRC, "Output Size",
			     .2, 1.5, info_->gViewportSize_.getValue(), 2,
		  	     TRUE, &info_->gViewportSize_);

#endif

#if 0
	buildIntMapRadioBox(	outputRC, "ProfilesToPrint",
			"Profiles to Print:",
			&info_->gProfiles_);
#endif
	return outputFrame;
}

//Build the "Profile" panel. Items here affect only the profile display,
// not axes, etc.
Widget AvProfileConfig::buildProfilePanel(const char *name, Widget parent)
{
	Widget frame = labeledFrame(parent, name, CENTER);

	XtVaSetValues(  frame,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			NULL);

	// Vertically stack contents
	Widget RC = XtVaCreateManagedWidget("profileRC",
			xmRowColumnWidgetClass,
			frame,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmVERTICAL,
			XmNcolumns, 1,
			NULL);

	buildIntMapRadioBox(RC, "lineStyleBox",
		      "Line Style:", &info_->gLineStyle_);

	buildIntMapRadioBox(RC, "PlotType", "Plot Type:", &info_->gPlotType_);
	////////////////////////////////////////////////////////////////
	//	Color selection toggles.
	buildColorSelector(RC, "ProfileColor", "Profile Color:",
			   &info_->gProfileColor_);

	// Build buttons to decide whether axis should be flipped.
	Widget w = buildIntMapRadioBox(RC, "XaxisMinMax",
		 "X Axis Min/Max", &info_->xMinMax_);
	// Reset so all buttons are on 1 row.
	{ Widget wm = childNameToWidget( w, "XaxisMinMax");
		XtVaSetValues(wm,
			XmNnumColumns, 1,
			NULL);
	}

	w = buildIntMapRadioBox(RC, "YaxisMinMax",
		 "Y Axis Min/Max", &info_->yMinMax_);
	{ Widget wm = childNameToWidget( w, "YaxisMinMax");
		XtVaSetValues(wm,
			XmNnumColumns, 1,
			NULL);
	}

	return frame;
}

////////////////////////////////////////////////////////////////
// 			Global Options Panel
////////////////////////////////////////////////////////////////

Widget AvProfileConfig::buildGlobalsPanel(const char *name, Widget parent)
{
	Widget globalFrame = labeledFrame(parent, name, CENTER);
	XtVaSetValues(  globalFrame,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			NULL);

	// Vertically stack contents
	Widget globalRC = XtVaCreateManagedWidget("globalRC",
			xmRowColumnWidgetClass,
			globalFrame,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmVERTICAL,
			XmNcolumns, 1,
			NULL);

	////////////////////////////////////////////////////////////////
	///	Generic
	Widget gFrame = labeledFrame(globalRC, "Generic", LEFT);
	// Vertically stack contents
	Widget genericRC = XtVaCreateManagedWidget("genericRC",
			xmRowColumnWidgetClass,
			gFrame,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmVERTICAL,
			NULL);

	// Axis Font
	buildIntMapRadioBox( genericRC, "fontBox", "Font:", &info_->gFont_);

	// Line Width
	buildIntegerSliderBox( genericRC, "Line Width",
			1, 20, info_->gLineWidth_.getValue(), 
			TRUE, &info_->gLineWidth_);

	// Global char scale.
	////////////////////////////////////////////////////////////////
	// Axis character scale

	buildDoubleSliderBox( genericRC, "Character Scale:",
			.1, 10.0, info_->gCharScale_.getValue(), 1,
			TRUE,
			&info_->gCharScale_);
	////////////////////////////////////////////////////////////////
	////	Label specific global parameters.

	Widget labelFrame = labeledFrame(globalRC, "Label Specific", LEFT);

	Widget labelRC = XtVaCreateManagedWidget("labelRC",
			xmRowColumnWidgetClass,
			labelFrame,
			XmNorientation, XmVERTICAL,
			NULL);

	///	A couple of toggles. OK, only 1 for now.
	// Horizontally stack buttons.
	Widget labelTogglesRC = XtVaCreateManagedWidget("labelTogglesRC",
			xmRowColumnWidgetClass,
			labelRC,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmHORIZONTAL,
			NULL);

	buildToggleButton(labelTogglesRC, "Show Labels", &info_->gDrawLabels_);


	////////////////////////////////////////////////////////////////
	////	Axis specific global parameters.

	Widget axisFrame = labeledFrame(globalRC, "Axis Specific", LEFT);

	Widget axisRC = XtVaCreateManagedWidget("axisRC",
			xmRowColumnWidgetClass,
			axisFrame,
			XmNorientation, XmVERTICAL,
//			XmNpacking,	XmPACK_TIGHT,
			NULL);

	///	A couple of toggles.
	// Horizontally stack buttons.
	Widget axisTogglesRC = XtVaCreateManagedWidget("axisTogglesRC",
			xmRowColumnWidgetClass,
			axisRC,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmHORIZONTAL,
			NULL);

	buildToggleButton(axisTogglesRC, "Draw Frame", &info_->gDrawFrame_);
	buildToggleButton(axisTogglesRC, "Draw Grid", &info_->gDrawGrid_);
	buildToggleButton(axisTogglesRC, "Draw Labels",
			  &info_->gDrawAxisLabels_);

	buildIntMapRadioBox(axisRC, "drawzeroline",
		 "Draw Zero Line", &info_->gDrawZeroLine_);

#if 0
	// This causes interactions with the box. It could come back if
	// wanted.
	// Axis character scale
	buildDoubleSliderBox( axisRC, "Character Scale:",
			.1, 10.0, info_->gAxisCharScale_.getValue(), 1,
			TRUE,
			&info_->gAxisCharScale_);
#endif

#if 0
	///	Axis color.
#if 0
	Widget colors = buildRadioBox(axisRC, "axisColors", "Axis Color",
				 &info_->gAxisColor_);
#else
	buildColorSelector(axisRC, "axisColors", "Axis Color",
			   &info_->gAxisColor_);
#endif
#endif

	return globalFrame;
}


/* Build the popup window.
*/
void AvProfileConfig::build()
{
Widget parentW;

	dialog = XtVaCreatePopupShell(name_.chars(),
		topLevelShellWidgetClass, ref_,
		XmNallowShellResize, TRUE,
		XmNcolormap,	AvXColorApp::colormap(),
		NULL);

	XtAddCallback(dialog, XmNdestroyCallback, &AvProfileConfig::destroyCB,
							(XtPointer) this);

	// Container for everything.
	parentW = XtVaCreateWidget("AxisRC",
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
			&AvProfileConfig::doneCB,
			(XtPointer) this);
	// Panel to choose what to edit.
	buildIntMapRadioBox(  parentW, "panels", "Choose what to edit",
			&panelNotifier_,
			(XtPointer) &panels_, &AvProfileConfig::panelCB);

	// Form widget to hold the different editing panels.
	Widget panelsW = XtVaCreateManagedWidget("optionPanels",
			xmFormWidgetClass,
			parentW,
			NULL);

	panels_[Global] = buildGlobalsPanel("Global Options", panelsW);

	panels_[Profile] = buildProfilePanel("Profile", panelsW);
	panels_[Output] = buildOutputPanel("Output", panelsW);
	////////////////////////////////////////////////////////////////
	///			The label and axis panels
	////////////////////////////////////////////////////////////////
	panels_[XLabel] = labelOptions("X Label", panelsW,
		     info_->getLabelOptions(AvProfileOptions::XLABEL));

	panels_[YLabel] = labelOptions("Y Label", panelsW,
		     info_->getLabelOptions(AvProfileOptions::YLABEL));

	panels_[Title] = labelOptions("Title", panelsW,
		     info_->getLabelOptions(AvProfileOptions::TITLE));

#if 0
	panels_[XAxis] = axisOptions("X Axis", panelsW,
		     	info_->getAxisOptions(AvProfileOptions::XAXIS));
	panels_[YAxis] = axisOptions("Y Axis", panelsW,
		     	info_->getAxisOptions(AvProfileOptions::YAXIS));
#else
	panels_[XAxis] = NULL;
	panels_[YAxis] = NULL;
#endif
	////////////////////////////////////////////////////////////////
	///		Marker sub panels
	////////////////////////////////////////////////////////////////
	Widget markersRC = NULL;
#if 1
	int numMarkers = info_->numMarkers();

	if(numMarkers > 0)
	{	Widget markerPanel = labeledFrame(panelsW, "Markers", CENTER);
		panels_[Markers] = markerPanel;

		markersRC = XtVaCreateWidget("markersTogglesRC",
			xmRowColumnWidgetClass,
			markerPanel,
			XmNorientation, XmVERTICAL,
			NULL);

		XtVaCreateManagedWidget("",
				xmSeparatorWidgetClass,
				markersRC,
				NULL);

		// Choose which marker panel to display.
		if(numMarkers > 1)
			buildSliderBox( markersRC, "Marker #",
				0, numMarkers-1, 0, 0, FALSE,
				this,
				&AvProfileConfig::handleChooseMarkerCB);

		// Form widget to hold the different editing panels.
		Widget markerForm = XtVaCreateManagedWidget("MarkerForm",
				xmFormWidgetClass,
				markersRC,
				NULL);

		// Array to hold references to the sub panels.
		markers_ = new MarkerData[numMarkers];

		// Turn off all but one of the subpanels.
		for(int mn=0; mn< numMarkers; mn++)
		{	// Also fills in markers_[mn].
			buildMarkerPanel("marker", markerForm, info_, mn);
			XtUnmanageChild(markers_[mn].panel);
		}
		XtManageChild(markers_[0].panel);
	}
	else
#endif
	{
		panels_[Markers] = NULL;
		markers_ = NULL;
	}

	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	/// Attach each panel to the 4 sides of the frame.
	//

	for(int i=0; i< NUMPANELS; i++)
	{	if(panels_[i] == NULL)
			continue;
		else
		XtVaSetValues(panels_[i],
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			NULL);
	}

	if(markersRC != NULL)
		XtManageChild(markersRC);
	XtManageChild(parentW);

	// Turn off all but one panel.
	int keep = panelNotifier_.getValue();
	for(int k=0; k< NUMPANELS; k++)
		if((k != keep)&&(panels_[k] != NULL))
			XtUnmanageChild(panels_[k]);
	oldPanelW = panels_[keep];
}

// Return the value of a toggle button.
Boolean AvProfileConfig::getToggle(Widget w)
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
void AvProfileConfig::setToggle(Widget w, const Boolean set,
				const Boolean Notify)
{
	if(w != NULL)
	{	XmToggleButtonSetState(w, set, Notify);
		if(XmToggleButtonGetState(w) != set)
		{	XmToggleButtonSetState(w, set, Notify);
			printf("%d %d\n", set, XmToggleButtonGetState(w));
		}
	}
}

// Popup the dialog rebuilding it first if necessary.
void AvProfileConfig::popup()
{
	if(dialog == NULL)
		build();

	// Make a copy of current settings for undos.
//	*backInfo_ = *info_;
	XtPopup(dialog, XtGrabNone);
	up_ = TRUE;
	callCallback(POPUP);
}

// Pop down the dialog.
void AvProfileConfig::popdown()
{
	if((dialog != NULL) && isUp())
	{	XtPopdown(dialog);
		up_ = FALSE;
		callCallback(POPDOWN);
	}
}

// converts an XmString to a string.
char *AvProfileConfig::XmStringToString_(XmString string)
{
char *text;

	if(XmStringGetLtoR(string, XmSTRING_DEFAULT_CHARSET, &text))
		return text;
	else
		return NULL;
}

char *AvProfileConfig::Strdup(const char *s)
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
void    AvProfileConfig::destroy(Boolean destroyWidgets)
{
	if(destroyWidgets && (dialog != NULL))
	{	XtRemoveCallback(dialog, XmNdestroyCallback,
				 &AvProfileConfig::destroyCB,
				 (XtPointer) this);
		XtDestroyWidget(dialog);
	}
	if(markers_ != NULL)
	{	delete [] markers_;
		markers_ = NULL;
	}
	dialog = NULL;
	oldPanelW=NULL;
	oldPanelToggleW=NULL;

	// The other widgets belong to dialog.
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//		Callbacks for the various buttons.
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

// Rather than turning off a panel when its toggle goes FALSE, a pointer to
// it is saved here. By unmanaging the widget after the new one is managed,
// we save on drastic window size changes. Since we're dealing with radio
// toggles, we can't get out of sync by having an oldW for one set and a
// new widget for another.
// Motif 2.x appears to handle radio buttons differently from 1.x. 2.x
// turns off the radio button after the set button is turned on necessitating
// a rework of the logic.
Widget AvProfileConfig::oldPanelW=NULL;
Widget AvProfileConfig::oldPanelToggleW=NULL;

// Called when a panel button is pressed. Show or hide depending on the
// state of its button.
void AvProfileConfig::panelCB(Widget w, XtPointer cbdata, XtPointer)
{AvIntMapNotifier	*r = (AvIntMapNotifier *)cbdata;
 Boolean isSet = getToggle(w);
 int		index;
 XmString	xmstr;
 XtPointer	userData;
 Widget		panel, *panels;
 char		*name;

	XtVaGetValues(w,
		XmNlabelString, &xmstr,
		XmNuserData,	&userData,
		NULL);
	name = XmStringToString_(xmstr);
	index = r->nameToIndex(name);		// Index into panels_
	panels = (Widget *) userData;		// List of panels.
	if(panels == NULL)
		return;

	panel = panels[index];
	// Ignore if there is nothing to display.
	if(panel == NULL)
	{	// Turn on the previous toggle.
#if XmVERSION < 2
		if(oldPanelToggleW != NULL)
			XmToggleButtonSetState(oldPanelToggleW, TRUE, TRUE);
		oldPanelW = NULL;
#else
		if(isSet)	// Clear current panel.
		{
			oldPanelW = NULL;
		}
#endif
		return;
	}

	if(isSet)	// Change current value and display panel.
	{	r->setName(name);
		XtManageChild(panel);
#if XmVERSION < 2
		// Turn off any previous panel after new one is managed.
		if(oldPanelW != NULL)
		{	XtUnmanageChild(oldPanelW);
			oldPanelW = NULL;
			oldPanelToggleW = NULL;
		}
#else
		// Save current info.
		{	oldPanelW = panel;
			oldPanelToggleW = w;
		}
#endif
	}
	else	// Turn off display of panel.
#if XmVERSION < 2
	{	oldPanelW = panel;
		oldPanelToggleW = w;
	}
#else
	{	// Turn off current panel only if there is a new one.
		// If there isn't, turn the old button back on.
		if(oldPanelW != NULL)
			XtUnmanageChild(panel);
		else
		{	oldPanelW = panel;
			XmToggleButtonSetState(oldPanelToggleW, TRUE, TRUE);
		}
	}
#endif
}

// Called when the text field changes. Copies the text field to the notifier.
void AvProfileConfig::handleTFCB(Widget w, XtPointer cbdata, XtPointer )
{AvRadioTextNotifier *r = (AvRadioTextNotifier *)cbdata;
 char *str;

	XtVaGetValues(w, XmNvalue, &str, NULL);
//	cout << "handleTCFB " << str << endl;
	r->setString(str);
}

// Called when the choose marker slider is changed.
void AvProfileConfig::handleChooseMarkerCB(Widget w, XtPointer cbdata, XtPointer)
{AvProfileConfig *me = (AvProfileConfig *)cbdata;
 int   newIndex, oldIndex;
 XtPointer ud;

	if(me == NULL)
	{	cerr	<< "AvProfileConfig::handleChooseSliderCB: "
			<< (char *)XtName(w) << " has no \"this\".\n";
		return;
	}

	// Get marker index.
	XtVaGetValues(w, XmNuserData, &ud,
			 XmNvalue, &newIndex,
			 NULL);
	oldIndex = Av::PtrToInt(ud);

	if(oldIndex == newIndex)
		return;
	XtManageChild(me->markers_[newIndex].panel);
	XtUnmanageChild(me->markers_[oldIndex].panel);

	ud = (XtPointer) newIndex;
	XtVaSetValues(w, XmNuserData, ud,
			 NULL);
}

// Called if the popup gets destroyed. (Probably by user).
void AvProfileConfig::destroyCB(Widget, XtPointer cbdata, XtPointer)
//void AvProfileConfig::destroyCB(Widget w, XtPointer cbdata, XtPointer)
{
AvProfileConfig	*me = (AvProfileConfig *)cbdata;

//printf("AvProfileConfig::DestroyCB called for widget: %s\n", XtName(w));
	if(me != NULL)
	{	me->callCallback(DESTROYED);
		me->destroy(FALSE);
		me->up_ = FALSE;
	}
}

// Called when the done text toggle button is pressed.
void AvProfileConfig::doneCB(Widget , XtPointer cbdata, XtPointer)
{AvProfileConfig *me = (AvProfileConfig *)cbdata;

	if(me != NULL)
		me->popdown();
	else
		cerr << "AvProfileConfig::handleHelpCB: NULL CB data.\n";
}

////////////////////////////////////////////////////////////////
///			ProfiletfHandler
ProfiletfHandler::ProfiletfHandler()
{
	button = field = NULL;	// All fields are filled in later.
	tfn = NULL;
}

// Copy the string to the text field.
void ProfiletfHandler::showString(const char *str)
{
	if(field == NULL)
		return;
	if(str == NULL)
		str = "";

	char *s = AvProfileConfig::Strdup(str);
	 XmTextFieldSetString(field, s);
	delete [] s;
}

// Called when a radio button is pressed.
// Display the appropriate string in the text field.
void ProfiletfHandler::handleBtnCB(Widget w, XtPointer cbdata, XtPointer)
{ProfiletfHandler *tfh = (ProfiletfHandler *)cbdata;

	if(tfh == NULL)
		return;
	if(!XmToggleButtonGetState(w))	// Ignore resets.
		return;

	const AvRadioTextNotifier *r = tfh->tfn;

	tfh->showString(r->getString());
}

// Called when text field is activated to make sure the "User" button is
// set.
void ProfiletfHandler::handleTFCB(Widget , XtPointer cbdata, XtPointer )
{ProfiletfHandler *tfh = (ProfiletfHandler *)cbdata;

	if(tfh == NULL)
		return;
	if(!XmToggleButtonGetState(tfh->button))
		XmToggleButtonSetState(tfh->button, True, True);
}
////////////////////////////////////////////////////////////////

// Call the callback for the particular reason.
void AvProfileConfig::callCallback(const CBReason why)
{
#if 0
Boolean immediate;

	switch(why) {
	case ACCEPT:
	case CANCEL:
//	case HELP:
	case DESTROYED:
	case POPUP:
	case POPDOWN:
		immediate = TRUE;
		break;
//	case ADD:
//	case DELETE:
//		saveValues(info_);	// Update the Axis level list.
//	case INVALID:
		break;
	default:
		immediate = FALSE;		// Oops.
		break;
	}

	if((callback_ != NULL) && immediate)
		(callback_)(this, cbdata_, why);
#else
	if(callback_ != NULL)
		(callback_)(this, cbdata_, why);
#endif
}

// Add a callback.
void AvProfileConfig::addCallback(AvProfileConfigCallback cb, XtPointer cbdata)
{
	callback_ = cb;
	cbdata_ = cbdata;
}
