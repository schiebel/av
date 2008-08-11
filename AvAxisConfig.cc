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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvAxisConfig.cc,v 19.0 2003/07/16 05:47:59 aips2adm Exp $
//
// $Log: AvAxisConfig.cc,v $
// Revision 19.0  2003/07/16 05:47:59  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:34  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.3  2002/01/30 22:30:30  hravlin
// Initialized a boolean var to make gcc happy.
//
// Revision 17.2  2002/01/22 21:36:27  hravlin
// Moved PtrToInt() to Av.h.
//
// Revision 17.1  2002/01/07 22:26:19  hravlin
// Removed check for __P.
//
// Revision 17.0  2001/11/12 19:43:02  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:09  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:35  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 18:18:06  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:09:05  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.3  2000/02/29 22:18:53  hravlin
// Under some circumstances, destroy() would delete markers_ twice.
//
// Revision 13.2  1999/09/24 21:01:40  hravlin
// Mostly cleanup + wasn't updating a variable in a callback.
//
// Revision 13.1  1999/08/25 20:01:12  hravlin
// Fixed problem with casting from XtPointer to enum.
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:40:55  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:17  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:02:05  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:56:19  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.8  1998/05/01 21:23:15  hr
// Use Aipsview's colormap when creating widgets.
//
// Revision 9.7  1998/02/26 22:18:57  hr
// Changed some name strings and reoordered panels.
//
// Revision 9.6  1998/02/25 17:29:51  hr
// Changed label string for label color to "Text Color".
//
// Revision 9.5  1998/02/20 20:39:42  hr
// Changed user controls for positioning labels.
//
// Revision 9.4  1997/11/17 17:33:28  hr
// In axisOptions(), exchanged positions of "Set Offset.." and "Place Origin"
// buttons.
//
// Revision 9.3  1997/11/17 17:11:57  hr
// Removed some unused variables.
//
// Revision 9.2  1997/11/17 16:54:58  hr
// Added support for axis offsets and captions.
//
// Revision 9.1  1997/09/23 20:29:20  hr
// Changed the upper range of character sliders to 5.0 from 10.0.
//
// Revision 9.0  1997/08/25 21:31:29  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.9  1997/08/13 20:33:25  hr
// Added readouts to character scale and line width.
//
// Revision 8.8  1997/06/25 16:49:01  hr
// Made the default panel Global instead of XAxis.
//
// Revision 8.7  1997/05/07 21:32:29  hr
// Replaced Gadgets with Widgets.
//
// Revision 8.6  1997/04/28 17:14:30  hr
// Desensitize Marker toggle button when there are no markers.
// Motif 2.x seems to turn on/off radio buttons in a different order from
// 1.x so needed to modify panel management for 2.x.
//
// Revision 8.5  1997/04/25 16:02:30  hr
// Changed layout for curvilinear axis. Added "Use Global Color Selector" button.
//
// Revision 8.4  1997/04/24 20:41:11  hr
// Reformatted axis layout for grid labels.
//
// Revision 8.3  1997/04/23 19:47:55  hr
// Changed grid color toggle label to "Grid Color".
//
// Revision 8.2  1997/04/23 17:46:14  hr
// Cleaned up some DCC compiler warnings.
//
// Revision 8.1  1997/04/23 17:12:19  hr
// Changes to support options for drawing curvilinear axes.
//
// Revision 8.0  1997/02/20 03:19:21  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.7  1997/02/10 21:00:46  hr
// Marker type selector switched from slider to toggles.
//
// Revision 7.6  1997/02/10 16:04:23  hr
// Removed an assignment to an unused widget.
//
// Revision 7.5  1997/02/09 00:30:48  hr
// Removed an unused variable.
//
// Revision 7.4  1997/02/09 00:04:30  hr
// Changed from using internal to external part building routines.
//
//
//---------------------------------------------------------------------------
/* Av AxisConfig.cc
Build setup window to change axis parameters.
*/

#include <iostream.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
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

#include <AvAxisConfig.h>
#include <AvImageWindow.h>
#include <AvImageViewPanel.h>
#include <AvImageViewItem.h>
#include <AvGUIParts.h>
#include <AvWidgetMisc.h>
#include <Av.h>

///	Table to convert between strings and values.

// The different panels that can be displayed (one at a time).
// The ordering must follow the enum in the include file.
static IntMap Panels[] = {
		{ AvAxisConfig::Global, "Global"},
		{ AvAxisConfig::XYAxes, "Axes"},
		{ AvAxisConfig::XAxis, "X Axis & V. Grid"},
		{ AvAxisConfig::YAxis, "Y Axis & H. Grid"},
		{ AvAxisConfig::XLabel, "X Label"},
		{ AvAxisConfig::YLabel, "Y Label"},
		{ AvAxisConfig::Title, "Title"},
		{ AvAxisConfig::Markers, "Markers"},
		{ AvAxisConfig::Captions, "Captions"},
		{ 0, NULL}
	};

// Label strings for the set marker position toggle.
static const char *TRACKMARKER =	"Place Marker";
static const char *TRACKORIGIN =	"Place Origin";
static const char *TRACKING =		"Track Mouse ";
static const char *OFFSETTRACKING =	"Origin Tracks Mouse";

// This is used to help text fields and buttons of a radioTextNotifier
// communicate with each other.
class tfHandler {
  public:
	tfHandler();
	Widget	button;		// Button to turn on text field.
	Widget	field;		// The text field.
	const	AvRadioTextNotifier *tfn;
	static void handleTFCB(Widget, XtPointer, XtPointer);
	// Called when button is pressed.
	static void handleBtnCB(Widget, XtPointer, XtPointer);
	void showString(const char *);
};

/////////////////////////////////
// When we want to be called.
static const int MOUSECHANGE =
	AvConductor::MouseDown | AvConductor::MouseMove | AvConductor::MouseUp;

AvAxisConfig::MarkerData::MarkerData()
{
	panel = NULL;
	win = NULL;
	marker = NULL;
}

AvAxisConfig::MarkerData::~MarkerData()
{
	unlink();
}

// Connect to the image window's mouse tracker.
void AvAxisConfig::MarkerData::link()
{
	if(win != NULL)
		win->getTracker()->addCallback( &AvAxisConfig::markerTrackCB,
						MOUSECHANGE, NULL,
						(XtPointer )this);
}

// Disconnect from the image window's mouse tracker.
void AvAxisConfig::MarkerData::unlink()
{
	if(win != NULL)
		win->getTracker()->removeCallback(&AvAxisConfig::markerTrackCB,
						  MOUSECHANGE, NULL,
						  (XtPointer )this);
}


////////////////////////////////////////////////////////////////
AvAxisConfig::AvAxisConfig(const char *name, Widget ref, AvAxisOptions *ao,
			   AvImageViewItem *ivi)
{
	name_ = "Setup for ";
	name_ += name;
	name_ += ":";
	name_ += ao->config()->moduleName();
	ref_ = ref;
	dialog = NULL;
	ivi_ = ivi;	// Ivi is is being initialized, so be careful.

	// The different panels that the 'Panels' buttons bring up.
	for(int i=0; i < NUMPANELS; i++)
	{	linPanels_[i] = NULL;
		crvPanels_[i] = NULL;
	}
	panels_ = linPanels_;
	offsetW_ = NULL;
	trackOriginW_ = NULL;
	captions_ = NULL;
	panelNotifier_.init("Panels", NULL, NULL, Panels, 0, Global);
	up_ = FALSE;
	callback_ = NULL;
	cbdata_ = NULL;
	info_ = ao;
	if(info_ != NULL)
	{	info_->ref();
	}
	toggling_ = FALSE;
	markers_ = NULL;
}

AvAxisConfig::~AvAxisConfig()
{
	unlink();
	popdown();
	destroy(TRUE);		// Get rid of any windows.
	info_->unref();
	// ??? Delete marker/captions??
}

// Build an option box for title, X or Y axes or captions.
Widget AvAxisConfig::labelOptions(const char *name, Widget parentW,
				  const AvAxisOptions::axisLabel *options)

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
	XtAddCallback(tfW, XmNactivateCallback, &AvAxisConfig::handleTFCB,
		      (XtPointer) &options->text_);
	// When the text field is activated, the "TextField" button needs to
	// be on. When the "Default" or "TextField" buttons are pressed, the
	// appropriate string should be displayed. tfHandler links them.
	tfHandler *tfh = new tfHandler();
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
			&tfHandler::handleBtnCB,
			(XtPointer) tfh);
		dw = childNameToWidget(textBox, rn->getName(di));
		XtAddCallback(dw, XmNvalueChangedCallback,
			&tfHandler::handleBtnCB,
			(XtPointer) tfh);

		XtAddCallback(tfW, XmNactivateCallback, &tfHandler::handleTFCB,
		      (XtPointer) tfh);
	}

	showEscapeCodes(optionsRC);

	////////////////////////////////////////////////////////////////
	// Axis character scale

	buildDoubleSliderBox(optionsRC, "Character Scale:",
		  .1, 5.0, options->charScale_.getValue(), 1,
		  TRUE,
		  &options->charScale_);

	////////////////////////////////////////////////////////////////
	///		Label color.
	////////////////////////////////////////////////////////////////

	Widget colorFrame = labeledFrame(optionsRC,
					 "Text Color");
	Widget colorRC = XtVaCreateManagedWidget("colorRC",
			xmRowColumnWidgetClass,
			colorFrame,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmVERTICAL,
			NULL);

	Widget colorBtnRC = XtVaCreateManagedWidget("colorBtnRC",
			xmRowColumnWidgetClass,
			colorRC,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmHORIZONTAL,
			NULL);
	// Should grid color track global or use the selector?
	buildToggleButton(colorBtnRC, "Use Global Color Selector",
			  &options->useGlobalColor_);
	buildToggleButton(colorBtnRC, "Opaque Background",
			  &options->opaqueBackground_);

	buildColorSelector(colorRC, "color", NULL,
			   &options->color_);
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	// Text position
	Widget placementFrame = labeledFrame(optionsRC,
					     "Text Placement:", LEFT);

	Widget placementRC = XtVaCreateManagedWidget("placementRC",
			xmRowColumnWidgetClass,
			placementFrame,
			XmNorientation, XmVERTICAL,
//			XmNpacking, XmPACK_TIGHT,
//			XmNpacking, XmPACK_COLUMN,
			NULL);

	buildDoubleSliderBox(placementRC, "X",
		  -2.0, 2.0, options->x_.getValue(), 2,
		  TRUE,
		  &options->x_);

	buildDoubleSliderBox(placementRC, "Y",
		  -2.0, 2.0,  options->y_.getValue(),2,
		  TRUE,
		  &options->y_);

	// Since the angle slider is wider than normal, move the label
	// above it.
	Widget w = buildDoubleSliderBox(placementRC, "Angle:",
		  -180.0, 180.0, options->angle_.getValue(),0,
		  TRUE,
		  &options->angle_);
	{ Widget slider = childNameToWidget(w, "slider");
		if(slider != NULL)
		{		 Dimension	width;
			XtVaGetValues(slider,
				XmNscaleWidth, &width,
				NULL);
			XtVaSetValues(slider,
				XmNscaleWidth, 370,
				NULL);
		}
		XtVaSetValues(w,
			XmNorientation, XmVERTICAL,
			XmNpacking, XmPACK_TIGHT,
			NULL);
	}

	return radioFrame;
}

// Builds the tick mark controls.
void AvAxisConfig::tickMarks(const char *name, Widget parent,
			     const AvAxisOptions::axis *options,
			     const Boolean linearAxis)
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
	buildIntMapRadioBox(majorRC, "majorTicks", NULL,
			linearAxis ? &options->linMajor_ :
				     &options->crvMajor_);
//	int maxticks = (linearAxis) ? 12 : 24;
	int maxticks = 24;
	const AvIntegerNotifier *major = (linearAxis) 
				? &options->linMajorIntervals_
				: &options->crvMajorIntervals_;

	buildIntegerSliderBox(majorRC, NULL,
		  1, maxticks, major->getValue(),
		  FALSE, major);

	/////////////////////////////////////////////////////////////////
	//			Minor tick marks
	if(linearAxis)
	{	XtVaCreateManagedWidget("Minor tick intervals",
					xmLabelWidgetClass,
					boxRC,
					NULL);

		Widget minorRC = XtVaCreateManagedWidget(name,
				xmRowColumnWidgetClass,
				boxRC,
				XmNpacking, XmPACK_TIGHT,
				XmNorientation, XmHORIZONTAL,
				NULL);
		buildIntMapRadioBox(minorRC, "minorTicks", NULL,
				    &options->minor_);

		buildIntegerSliderBox(minorRC, NULL,
				     0, 10,
				     options->linMinorIntervals_.getValue(),
				     FALSE, &options->linMinorIntervals_);
	}
}

// Builds the "Grid and TickMarks panel.
Widget AvAxisConfig::linAxisOptions(const char *name, Widget parentW,
				 const AvAxisOptions::axis *options)
{
	Widget radioFrame = labeledFrame(parentW, name, CENTER);

	// Vertically stack contents
	Widget optionsRC = XtVaCreateManagedWidget(name,
			xmRowColumnWidgetClass,
			radioFrame,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmVERTICAL,
			NULL);


	///	Grid and Tickmarks.
	Widget axisFrame = labeledFrame(optionsRC, "Grid & TickMarks");

	Widget axisRC = XtVaCreateManagedWidget(name,
			xmRowColumnWidgetClass,
			axisFrame,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmVERTICAL,
			NULL);

	Widget axisOptionsRC = XtVaCreateManagedWidget(name,
			xmRowColumnWidgetClass,
			axisRC,
			XmNpacking, XmPACK_COLUMN,
			XmNorientation, XmHORIZONTAL,
			XmNnumColumns, 3,
			NULL);

	buildIntMapRadioBox(axisOptionsRC, "drawzeroline",
		 "Draw Zero Line", &options->drawZeroLine_);

	tickMarks("tickmarks", axisRC, options, TRUE);

	Widget labellingFrame = labeledFrame(optionsRC, "Labels");

	Widget labellingRC = XtVaCreateManagedWidget(name,
			xmRowColumnWidgetClass,
			labellingFrame,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmVERTICAL,
			NULL);

	// This is only relevant for Y axis labels.
	if(options->typeMask() == AvAxisOptions::YAXIS)
		buildToggleButton(labellingRC, "Vertical",
				  &options->vertical_);


	buildIntMapRadioBox(labellingRC, "labelingBox", "Format:",
			    &options->decimal_);

	buildIntMapRadioBox(labellingRC, "position", "Placement",
		 &options->position_);

	return radioFrame;
}

Widget formatFrame=NULL;

// Builds the Panel for selecting options common to both axes.
Widget AvAxisConfig::axesOptions(const char *name, Widget parentW)
{
	Widget axesFrame = labeledFrame(parentW, name, CENTER);
	XtVaSetValues(  axesFrame,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			NULL);

	// Vertically stack contents
	Widget axesRC = XtVaCreateManagedWidget("axesRC",
			xmRowColumnWidgetClass,
			axesFrame,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmVERTICAL,
			XmNcolumns, 1,
			NULL);

	///	Frame and Grid toggles.
	// Horizontally stack buttons.
	Widget axisTogglesRC = XtVaCreateManagedWidget("axisTogglesRC",
			xmRowColumnWidgetClass,
			axesRC,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmHORIZONTAL,
			NULL);

	buildToggleButton(axisTogglesRC, "Draw Frame", &info_->gDrawFrame_);
	buildToggleButton(axisTogglesRC, "Draw Grid", &info_->gDrawGrid_);

	// Currently, the only time we can't use curvilinear mode is when
	// displaying the wedge.
	if(mayUseCurvilinear())
	{	// What type of axis to use.
		Widget w = buildIntMapRadioBox(axesRC, "axisBox", "Axis Type:",
				    &info_->gAxisType_, (XtPointer) this,
				    &AvAxisConfig::handleAxisTypeCB);
		Widget rc= childNameToWidget(w, "axisBox");
		XtVaSetValues(rc,
			      XmNnumColumns, 2,
			      NULL);
		formatFrame = labeledFrame(axesRC, "Offset Format");
		offsetW_ = formatFrame;
		// Vertically stack contents
		Widget offsetRC = XtVaCreateManagedWidget("offsetRC",
			xmRowColumnWidgetClass,
			formatFrame,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmVERTICAL,
			XmNcolumns, 1,
			NULL);
		 buildIntMapRadioBox(offsetRC, "axisFormatBox",
				    "Linear Offset Formats:",
				    &info_->gLinearAxisFormat_,
				    (XtPointer) this);
		Widget miscRC = XtVaCreateManagedWidget("miscRC",
			xmRowColumnWidgetClass,
			offsetRC,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmHORIZONTAL,
//			XmNcolumns, 1,
			NULL);

		Widget resetW = XtVaCreateManagedWidget("Set Offset to Center",
				xmPushButtonWidgetClass,
				miscRC,
				NULL);
		XtAddCallback(  resetW, XmNactivateCallback,
				&AvAxisConfig::resetOffsetCB,
				(XtPointer) this);

		// The "track" toggle. When set, the callback will link
		// the positioning CB to the tracker.
		w = XtVaCreateManagedWidget(TRACKORIGIN,
			xmToggleButtonWidgetClass,
			miscRC,
			XmNset, False,
			XmNborderWidth, 1,
			XmNshadowThickness, 1,
			NULL);
		trackOriginW_ = w;
		XtAddCallback(  w, XmNvalueChangedCallback,
			&AvAxisConfig::handleOffsetCB,
			(XtPointer) this);

		// Turn off offset formats if LINEAROFFSET isn't the default.
		if(info_->gAxisType_.getValue() != AvAxisOptions::LINEAROFFSET)
			XtSetSensitive(offsetW_, FALSE);
		else
		{	makeAxisLabels(OFFSET);
			info_->gAxisType_.notify();	// Force a redraw.
		}

		buildToggleButton(offsetRC, "Offset Only Angular Positions",
				  &info_->gAngularOnly_,
				  (XtPointer) this,
				  &AvAxisConfig::angularCB,
				  GUICB_AFTER); // Put default cb after ours.
	}
	return axesFrame;
}

// Builds the axis panel for curvilinear axes.
Widget AvAxisConfig::crvAxisOptions(const char *name, Widget parentW,
				    const AvAxisOptions::axis *options)
{
	Widget radioFrame = labeledFrame(parentW, name, CENTER);

	// Vertically stack contents
	Widget optionsRC = XtVaCreateManagedWidget(name,
			xmRowColumnWidgetClass,
			radioFrame,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmVERTICAL,
			NULL);


	///	Grid and Tickmarks.
	Widget axisFrame = labeledFrame(optionsRC, "Grid");

	Widget axisRC = XtVaCreateManagedWidget(name,
			xmRowColumnWidgetClass,
			axisFrame,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmVERTICAL,
			NULL);

	tickMarks("tickmarks", axisRC, options, FALSE);

	Widget colorFrame = labeledFrame(optionsRC,
					 "Grid/Tick Color");
	Widget colorRC = XtVaCreateManagedWidget("colorRC",
			xmRowColumnWidgetClass,
			colorFrame,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmVERTICAL,
			NULL);

	// Should grid color track global or use the selector?
	buildToggleButton(colorRC, "Use Global Color Selector",
			  &options->useGlobalColor_);

	// name and label need to be spelled differently.
	buildColorSelector(colorRC, "color", NULL,
			   &options->color_);
	////////////////////////////////////////////////////////////////
	/// Grid/Tick mark labels.
	Widget gridTickFrame = labeledFrame(optionsRC,
					     "Numerical Coordinates");

	Widget gridTickRC = XtVaCreateManagedWidget(name,
			xmRowColumnWidgetClass,
			gridTickFrame,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmVERTICAL,
			NULL);

	buildToggleButton(gridTickRC, "Label Color Tracks Grid/Tick Color",
			  &options->labelTracksGrid_);

	buildIntMapRadioBox(gridTickRC, "format", "Format",
			    &options->format_);

	// The extra RC box helps get the label aligned.
	Widget axisPlacementRC = XtVaCreateManagedWidget(name,
			xmRowColumnWidgetClass,
			gridTickRC,
			XmNpacking, XmPACK_COLUMN,
			XmNorientation, XmVERTICAL,
			XmNpacking, XmPACK_TIGHT,
			NULL);

	XtVaCreateManagedWidget("Placement",
				xmLabelWidgetClass,
				axisPlacementRC,
//				XmNalignment, XmALIGNMENT_CENTER,
//				XmNalignment, XmALIGNMENT_BEGINNING,
				NULL);

	Widget labellingRC = XtVaCreateManagedWidget(name,
			xmRowColumnWidgetClass,
			axisPlacementRC,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmHORIZONTAL,
			NULL);

	buildToggleButton(labellingRC, "Top",
			  &options->top_);

	buildToggleButton(labellingRC, "Bottom",
			  &options->bottom_);

	buildToggleButton(labellingRC, "Left",
			  &options->left_);

	buildToggleButton(labellingRC, "Right",
			  &options->right_);

	return radioFrame;
}

// Build a marker panel for marker 'index'. Since placement is dependent
// on being able to track the mouse, if there is no ImageViewPanel, tracking
// isn't supported.
Widget AvAxisConfig::buildMarkerPanel(const char *, Widget parent,
				      const AvAxisOptions *info,
				      const int index)
{char buf[64];
 const AvAxisOptions::marker *marker = info->getMarkerOptions(index);

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


	buildToggleButton(showPlaceRC, "Show Marker", &marker->show_);

	// The "track" toggle. When set, the callback will link
	// the positioning CB to the tracker.
	// (This button doesn't lend itself to the buildToggleButton rtn).
	if(ivi_ != NULL)
	{	Widget w = XtVaCreateManagedWidget(TRACKMARKER,
			xmToggleButtonWidgetClass,
			showPlaceRC,
			XmNset, False,
			XmNborderWidth, 1,
			XmNshadowThickness, 1,
			NULL);
		XtAddCallback(  w, XmNvalueChangedCallback,
			&AvAxisConfig::handleMarkerCB,
			(XtPointer) &markers_[index]);
		AvAccessor *a = ivi_->accessor();
		// Max cursor value for computing fractional position.
		markers_[index].xMax_ = (double)a->width() -1.0;
		markers_[index].yMax_ = (double)a->height() -1.0;
	}
	else
	{
		markers_[index].yMax_ = 1.0;
		markers_[index].xMax_ = 1.0;
	}

	buildDoubleSliderBox(markerRC, "Scale",
		  .1, 10.0, marker->scale_.getValue(), 1,
		  TRUE, &marker->scale_);

	// Build a radio box for the marker type toggles.
	buildMarkerSelector(markerRC, "Type", "Marker Type:",
			    &marker->type_);

	markers_[index].panel = markerFrame;
	if(ivi_ != NULL)
		markers_[index].win = ivi_->getImageWindow();
	else
		markers_[index].win = NULL;
	markers_[index].marker = marker;


	// name and label need to be spelled differently.
	buildColorSelector(markerRC, "color", "Color", &marker->color_);

	XtManageChild(markerRC);
	return markerFrame;
}

////////////////////////////////////////////////////////////////
// 			Global Options Panel
////////////////////////////////////////////////////////////////

Widget AvAxisConfig::buildGlobalsPanel(const char *name, Widget parent)
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
	buildIntMapRadioBox(genericRC, "fontBox", "Font:", &info_->gFont_);

	// Line Width
	buildIntegerSliderBox(genericRC, "Line Width",
		  1, 20, info_->gLineWidth_.getValue(), 
		  TRUE, &info_->gLineWidth_);

	// Global char scale.
	////////////////////////////////////////////////////////////////
	// Axis character scale

	buildDoubleSliderBox(genericRC, "Character Scale:",
		  .1, 5.0, info_->gCharScale_.getValue(), 1,
		  TRUE,
		  &info_->gCharScale_);

	///	Global color selector. And frame/grid color.
	buildColorSelector(genericRC,
		   "axisColors", "Global Color Selector/Frame & Grid Color",
		   &info_->gAxisColor_);

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

	return globalFrame;
}


// Attach each panel to the 4 sides of the frame.
static void attachPanels(Widget *panels, const int numpanels)
{
	for(int i=0; i< numpanels; i++)
	{	if(panels[i] == NULL)
			continue;
		else
		XtVaSetValues(panels[i],
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			NULL);
	}
}

	// Builds all marker panels.
Widget AvAxisConfig::buildMarkerPanels(const char *name, Widget parent)
{ Widget markersRC = NULL, markerPanel=NULL;
  int numMarkers = info_->numMarkers();

	if(numMarkers > 0)
	{
		markerPanel = labeledFrame(parent, name, CENTER);
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
			buildSliderBox(markersRC, "Marker #",
				  0, numMarkers-1, 0, 0, FALSE,
				  this, &AvAxisConfig::handleChooseMarkerCB);

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
		if(markersRC != NULL)
			XtManageChild(markersRC);
	}
	else
	{
		markers_ = NULL;
	}
	return markerPanel;
}

	// Builds all caption panels.
Widget AvAxisConfig::buildCaptionPanels(const char *name, Widget parent)
{ Widget captionsRC = NULL, captionPanel=NULL;
  int numCaptions = info_->numCaptions();

	if(numCaptions > 0)
	{
		captionPanel = labeledFrame(parent, name, CENTER);
		captionsRC = XtVaCreateWidget("captionsTogglesRC",
			xmRowColumnWidgetClass,
			captionPanel,
			XmNorientation, XmVERTICAL,
			NULL);

		XtVaCreateManagedWidget("",
				xmSeparatorWidgetClass,
				captionsRC,
				NULL);

		// Choose which caption panel to display.
		if(numCaptions > 1)
		{ // Build slider box to select captions. Use the
			buildSliderBox( captionsRC, "Caption #",
					0, numCaptions-1, 0, 0, FALSE,
				  	this,
					&AvAxisConfig::handleChooseCaptionCB);
		}

		// Form widget to hold the different editing panels.
		Widget captionForm = XtVaCreateManagedWidget("CaptionForm",
				xmFormWidgetClass,
				captionsRC,
				NULL);

		// Array to hold references to the sub panels.
		captions_ = new Widget[numCaptions];

		// Turn off all but one of the subpanels.
		// Also fills in captions_[cn].
		for(int cn=0; cn< numCaptions; cn++)
		{char cname[32];

			sprintf(cname, "Caption%02d", cn);
			captions_[cn] = labelOptions(cname,captionForm,
					 &info_->captions_[cn]);
			XtVaSetValues(captions_[cn], XmNuserData, cn,
				      NULL);

			XtUnmanageChild(captions_[cn]);
		}
		XtManageChild(captions_[0]);
		if(captionsRC != NULL)
			XtManageChild(captionsRC);
	}
	else
	{
		captions_ = NULL;
	}
	return captionPanel;
}

static void unmanagePanels(Widget *panels, const int keep, const int numpanels)
{
	for(int k=0; k< numpanels; k++)
		if((k != keep)&&(panels[k] != NULL))
			XtUnmanageChild(panels[k]);
}

/* Build the popup window.
*/
void AvAxisConfig::build()
{
Widget parentW;

	// Building the window takes a long time, so let the user know
	// something is happening.
	AvXColorApp::setBusy();

	xDefLabel_ = info_->xLabel_.text_.defaultString();
	yDefLabel_ = info_->yLabel_.text_.defaultString();

	dialog = XtVaCreatePopupShell(name_.chars(),
		topLevelShellWidgetClass, ref_,
		XmNallowShellResize, TRUE,
		// Use our colormap.
		XmNcolormap,  AvXColorApp::colormap(),
		NULL);

	XtAddCallback(dialog, XmNdestroyCallback, &AvAxisConfig::destroyCB,
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
			&AvAxisConfig::doneCB,
			(XtPointer) this);
	// Panel to choose what to edit.
	Widget panelButtonsW = buildIntMapRadioBox(parentW, "panels",
		"Choose what to edit",
		&panelNotifier_,
		(XtPointer) &panels_, &AvAxisConfig::panelCB);

	// Form widget to hold the different editing panels.
	Widget panelsW = XtVaCreateManagedWidget("optionPanels",
			xmFormWidgetClass,
			parentW,
			NULL);

	panels_ = linPanels_;
	panels_[Global] = buildGlobalsPanel("Global Options", panelsW);
	crvPanels_[Global] = panels_[Global];

	////////////////////////////////////////////////////////////////
	///		The label and axis panels for Linear
	////////////////////////////////////////////////////////////////
	panels_[XLabel] = labelOptions("X Label", panelsW,
		     info_->getLabelOptions(AvAxisOptions::XLABEL));

	panels_[YLabel] = labelOptions("Y Label", panelsW,
		     info_->getLabelOptions(AvAxisOptions::YLABEL));

	panels_[Title] = labelOptions(Panels[Title].name, panelsW,
		     info_->getLabelOptions(AvAxisOptions::TITLE));
	panels_[XAxis] = linAxisOptions(Panels[XAxis].name, panelsW,
		     	info_->getAxisOptions(AvAxisOptions::XAXIS));
	panels_[YAxis] = linAxisOptions(Panels[YAxis].name, panelsW,
		     	info_->getAxisOptions(AvAxisOptions::YAXIS));
	panels_[XYAxes] = axesOptions("Axes", panelsW);
#if 0
	panels_[Captions] = labelOptions("Captions", panelsW,
		     info_->captions_);
#else
	panels_[Captions] = buildCaptionPanels("Captions", panelsW);
#endif

	if(mayUseCurvilinear())
	{	crvPanels_[XLabel] = panels_[XLabel];
		crvPanels_[YLabel] = panels_[YLabel];
		crvPanels_[Title] = labelOptions(Panels[Title].name, panelsW,
			     info_->getLabelOptions(AvAxisOptions::TITLE));
		crvPanels_[XAxis] = crvAxisOptions(Panels[XAxis].name, panelsW,
			     	info_->getAxisOptions(AvAxisOptions::XAXIS));
		crvPanels_[YAxis] = crvAxisOptions(Panels[YAxis].name, panelsW,
			     	info_->getAxisOptions(AvAxisOptions::YAXIS));
		crvPanels_[XYAxes] = panels_[XYAxes];
		crvPanels_[Captions] = panels_[Captions];
	}

	////////////////////////////////////////////////////////////////
	///		Marker sub panels
	////////////////////////////////////////////////////////////////
	panels_[Markers] = buildMarkerPanels("Markers", panelsW);
	if(mayUseCurvilinear())
	{	crvPanels_[Markers] = panels_[Markers];
	}

	// If no markers, desensitize the marker button.
	if(info_->numMarkers() <= 0)
	{	Widget mw = childNameToWidget(panelButtonsW, "Markers");
		if(mw != NULL)
			XtSetSensitive(mw, FALSE);
	}

	// If no captions, desensitize the marker button.
	if(info_->numCaptions() <= 0)
	{	Widget cw = childNameToWidget(panelButtonsW, "Captions");
		if(cw != NULL)
			XtSetSensitive(cw, FALSE);
	}
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	/// Attach each panel to the 4 sides of the frame.
	//

	if(mayUseCurvilinear())
		attachPanels(crvPanels_, NUMPANELS);
	attachPanels(linPanels_, NUMPANELS);

#if 0
	if(markersRC != NULL)
		XtManageChild(markersRC);
#endif
	XtManageChild(parentW);
	// Turn off all but one panel.
	int keep = panelNotifier_.getValue();
	unmanagePanels(linPanels_, -1, NUMPANELS);
	unmanagePanels(crvPanels_, -1, NUMPANELS);
	if(mayUseCurvilinear() &&
	   (info_->gAxisType_.getValue() != AvAxisOptions::LINEAR))
		panels_ = crvPanels_;
	else
		panels_ = linPanels_;

	XtManageChild(panels_[keep]);
	oldPanelW = panels_[keep];

	AvXColorApp::resetBusy();
}

// Return the value of a toggle button.
Boolean AvAxisConfig::getToggle(Widget w)
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
void AvAxisConfig::setToggle(Widget w, const Boolean set, const Boolean Notify)
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
void AvAxisConfig::popup()
{
	if(dialog == NULL)
		build();

	XtPopup(dialog, XtGrabNone);
	up_ = TRUE;
}

// Pop down the dialog.
void AvAxisConfig::popdown()
{
	if((dialog != NULL) && isUp())
	{	XtPopdown(dialog);
		up_ = FALSE;
	}
}

// converts an XmString to a string.
char *AvAxisConfig::XmStringToString_(XmString string)
{
char *text;

	if(XmStringGetLtoR(string, XmSTRING_DEFAULT_CHARSET, &text))
		return text;
	else
		return NULL;
}

char *AvAxisConfig::Strdup(const char *s)
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
void    AvAxisConfig::destroy(Boolean destroyWidgets)
{
	if(destroyWidgets && (dialog != NULL))
	{	XtRemoveCallback(dialog, XmNdestroyCallback,
				 &AvAxisConfig::destroyCB,
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
// If a panel is NULL (ie no widget to manage), the current panel is kept
// and its toggle is turned back on.
// Motif 2.x appears to handle radio buttons differently from 1.x. 2.x
// turns off the radio button after the set button is turned on necessitating
// a rework of the logic.
Widget AvAxisConfig::oldPanelW=NULL;
Widget AvAxisConfig::oldPanelToggleW=NULL;

// Called when a panel button is pressed. Show or hide depending on the
// state of its button.
void AvAxisConfig::panelCB(Widget w, XtPointer cbdata, XtPointer)
{AvIntMapNotifier	*r = (AvIntMapNotifier *)cbdata;
 Boolean isSet = getToggle(w);
// int btn = r->getValue();
 int		index;
 XmString	xmstr;
 XtPointer	userData;
 Widget		panel, *panels, **pnlptr;
 char		*name;

	XtVaGetValues(w,
		XmNlabelString, &xmstr,
		XmNuserData,	&userData,
		NULL);
	name = XmStringToString_(xmstr);
	index = r->nameToIndex(name);		// Index into panels_
	pnlptr = (Widget **) userData;		// List of panels.
	panels = *pnlptr;

	if(panels == NULL)
		return;

	panel = panels[index];
	// Ignore if there is nothing to display.
	if(panel == NULL)
	{
#if XmVERSION < 2
		// Turn on the previous toggle.
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
void AvAxisConfig::handleTFCB(Widget w, XtPointer cbdata, XtPointer )
{AvRadioTextNotifier *r = (AvRadioTextNotifier *)cbdata;
 char *str;

	XtVaGetValues(w, XmNvalue, &str, NULL);
	r->setString(str);
}

// Called when the choose marker slider is changed.
// Same as handleChooseMarkerCB.
void AvAxisConfig::handleChooseCaptionCB(Widget w, XtPointer cbdata, XtPointer)
{AvAxisConfig *me = (AvAxisConfig *)cbdata;
 int   newIndex, oldIndex;
 XtPointer ud;

	if(me == NULL)
	{	cerr	<< "AvAxisConfig::handleChooseSliderCB: "
			<< (char *)XtName(w) << " has no \"this\".\n";
		return;
	}

	// Get marker index.
	XtVaGetValues(w, XmNuserData, &ud,
			 XmNvalue, &newIndex,
			 NULL);

	oldIndex = (int) Av::PtrToInt(ud);
	if(oldIndex == newIndex)
		return;

	XtManageChild(me->captions_[newIndex]);
	XtUnmanageChild(me->captions_[oldIndex]);

	ud = (XtPointer) newIndex;
	XtVaSetValues(w, XmNuserData, ud,
			 NULL);
}

// Called when the choose marker slider is changed.
void AvAxisConfig::handleChooseMarkerCB(Widget w, XtPointer cbdata, XtPointer)
{AvAxisConfig *me = (AvAxisConfig *)cbdata;
 int   newIndex, oldIndex;
 XtPointer ud;

	if(me == NULL)
	{	cerr	<< "AvAxisConfig::handleChooseSliderCB: "
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

// Called when mouse postion changes in the image window and a marker is
// connected to it.
// Updates the marker's position.
void AvAxisConfig::markerTrackCB(XtPointer , AvConductor *, XtPointer ud)
{ AvAxisConfig::MarkerData *data = (AvAxisConfig::MarkerData *)ud;
  AvImageWindow *win = data->win;
  AvImageViewPanel *ivp = win->viewPanel();
  double xp, yp;

	// Pick up cursor position and convert to fraction of window.
	AvIPosition cursor(ivp->cursor());
	xp = (double)cursor(0)/data->xMax_;
	yp = (double)cursor(1)/data->yMax_;

	// Yes, this is a hack to get around the 'const'.
	XtPointer p = (XtPointer) &(data->marker->x_);
	AvDoubleNotifier *x = (AvDoubleNotifier *)p;
	p = (XtPointer) &(data->marker->y_);
	AvDoubleNotifier *y = (AvDoubleNotifier *)p;

	// Temporarily disable notification so it only happens once.
	x->conductor()->disableNotify();
	 x->setValue(xp);
	 y->setValue(yp);
	x->conductor()->enableNotify();
}


// Called when a marker's "Track" button is pressed.
//  Change the label.
//  If set, link marker to image window's tracker.
//  If unset, unlink.
void AvAxisConfig::handleMarkerCB(Widget w, XtPointer cbdata, XtPointer)
{AvAxisConfig::MarkerData *data = (AvAxisConfig::MarkerData *)cbdata;
 Boolean isSet = XmToggleButtonGetState(w);

	if(isSet)
	{XmString str = AvXmStringCreateLocalized(TRACKING);
		XtVaSetValues(w, XmNlabelString, str,
			      NULL);
		XmStringFree(str);
		data->link();
	}
	else
	{XmString str = AvXmStringCreateLocalized(TRACKMARKER);
		XtVaSetValues(w, XmNlabelString, str,
			      NULL);
		XmStringFree(str);
		data->unlink();
	}
}

// Connect to the image window's mouse tracker.
void AvAxisConfig::link()
{	if(ivi_ != NULL)
		ivi_->getImageWindow()->getTracker()
			->addCallback( &AvAxisConfig::offsetTrackCB,
					MOUSECHANGE, NULL,
					(XtPointer )this);
}

// Disconnect from the image window's mouse tracker.
void AvAxisConfig::unlink()
{	if(ivi_ != NULL)
		ivi_->getImageWindow()->getTracker()
			->removeCallback(&AvAxisConfig::offsetTrackCB,
					  MOUSECHANGE, NULL,
					  (XtPointer )this);
}

#if 0
static const char *reasonToString(const int reason)
{static char buf[64];

	switch (reason) {
	case AvConductor::NoReason:
	case AvConductor::Destroyed:
		return "Destroyed";
		break;
	case AvConductor::MouseDown:
		return "MouseDown";
		break;
	case AvConductor::MouseMove:
		return "MouseMove";
		break;
	case AvConductor::MouseUp:
		return "MouseUp";
		break;
	default:
		sprintf(buf, "Unknown: %d", reason);
		return buf;
	}
}
#endif

// Called when mouse postion changes in the image window and offset is
// being changed.
// REQUIRES ivi_ to not be NULL.
void AvAxisConfig::offsetTrackCB(XtPointer , AvConductor *cnd, XtPointer ud)
{ AvAxisConfig *data = (AvAxisConfig *)ud;
  AvImageWindow *win = data->ivi_->getImageWindow();
  AvImageViewPanel *ivp = win->viewPanel();

	// Turn off update notification until we're ready.
	data->info_->gAxisOffsets_.conductor()->disableNotify();
	 // Pick up cursor position.
	 data->info_->gAxisOffsets_.setValue(ivp->cursor());
	 // Don't update axis labels on mouse move since it takes so long.
	 if(cnd->callbackReason()!=AvConductor::MouseMove)
		data->makeAxisLabels(OFFSET);

	data->info_->gAxisOffsets_.conductor()->enableNotify();
	// If MouseUp, need to force a redraw.
	if(cnd->callbackReason()==AvConductor::MouseUp)
		data->info_->gAxisOffsets_.notify();
}


// Called when a marker's "Track" button is pressed.
//  Change the label.
//  If set, link marker to image window's tracker.
//  If unset, unlink.
void AvAxisConfig::handleOffsetCB(Widget w, XtPointer cbdata, XtPointer)
{AvAxisConfig *data = (AvAxisConfig *)cbdata;
 Boolean isSet = XmToggleButtonGetState(w);

	if(isSet)
	{XmString str = AvXmStringCreateLocalized(OFFSETTRACKING);
		XtVaSetValues(w, XmNlabelString, str,
			      NULL);
		XmStringFree(str);
		data->link();
	}
	else
	{XmString str = AvXmStringCreateLocalized(TRACKORIGIN);
		XtVaSetValues(w, XmNlabelString, str,
			      NULL);
		XmStringFree(str);
		data->unlink();
	}
}

// Called if the popup gets destroyed. (Probably by user).
void AvAxisConfig::destroyCB(Widget, XtPointer cbdata, XtPointer)
//void AvAxisConfig::destroyCB(Widget w, XtPointer cbdata, XtPointer)
{
AvAxisConfig	*me = (AvAxisConfig *)cbdata;

//	printf("AvAxisConfig::DestroyCB called for widget: %s\n", XtName(w));
	if(me != NULL)
	{	me->callCallback(DESTROYED);
		me->destroy(FALSE);
		me->up_ = FALSE;
	}
}

// Called when the done text toggle button is pressed.
void AvAxisConfig::doneCB(Widget , XtPointer cbdata, XtPointer)
{AvAxisConfig *me = (AvAxisConfig *)cbdata;

	if(me != NULL)
		me->popdown();
	else
		cerr << "AvAxisConfig::handleHelpCB: NULL CB data.\n";
}

////////////////////////////////////////////////////////////////
///			tfHandler
tfHandler::tfHandler()
{
	button = field = NULL;	// All fields are filled in later.
	tfn = NULL;
}

// Copy the string to the text field.
void tfHandler::showString(const char *str)
{
	if(field == NULL)
		return;
	if(str == NULL)
		str = "";

	char *s = AvAxisConfig::Strdup(str);
	 XmTextFieldSetString(field, s);
	delete [] s;
}

// Called when a radio button is pressed.
// Display the appropriate string in the text field.
void tfHandler::handleBtnCB(Widget w, XtPointer cbdata, XtPointer)
{tfHandler *tfh = (tfHandler *)cbdata;

	if(tfh == NULL)
		return;
	if(!XmToggleButtonGetState(w))	// Ignore resets.
		return;

	const AvRadioTextNotifier *r = tfh->tfn;

	tfh->showString(r->getString());
}

// Called when text field is activated to make sure the "User" button is
// set.
void tfHandler::handleTFCB(Widget , XtPointer cbdata, XtPointer )
{tfHandler *tfh = (tfHandler *)cbdata;

	if(tfh == NULL)
		return;
	if(!XmToggleButtonGetState(tfh->button))
		XmToggleButtonSetState(tfh->button, True, True);
}

// Called when axis type is changed.
// set. Chooses which of linPanels or crvPanels will be drawn.
// REQUIRES the label for the Curvilinear button to not change!
void AvAxisConfig::handleAxisTypeCB(Widget w, XtPointer cbdata, XtPointer)
{AvIntMapNotifier	*r = (AvIntMapNotifier *)cbdata;
 char *name;
 Boolean isSet = getToggle(w);
 XmString	xmstr;
 XtPointer	ud;

	XtVaGetValues(w, XmNlabelString, &xmstr,
				 XmNuserData, &ud,
			      NULL);
	name = XmStringToString_(xmstr);
	AvAxisConfig *me = (AvAxisConfig *)ud;

	if(isSet)
	{
	// Setting the name would result in a redraw so delay until we're
	// done.
	r->disableUpdate();
	r->setName(name);

		// Change which panels will be shown.
		if(strcmp("Curvilinear", name) == 0)
			me->panels_ = me->crvPanels_;
		else
			me->panels_ = me->linPanels_;

		if((strcmp("Linear Offsets", name) == 0) &&
		   (me->offsetW_ != NULL))
		{	XtSetSensitive(me->offsetW_, TRUE);
			me->makeAxisLabels(OFFSET);
			// If turning on linear offsets and we were tracking,
			// need to enable tracking again.
			if(me->getToggle(me->trackOriginW_))
				me->link();
		}
		else
		{	XtSetSensitive(me->offsetW_, FALSE);
			me->makeAxisLabels(DEFAULT);
		}
	// Reenable updates causing a redraw and updating variables.
	r->enableUpdate(True);
	}
	else	// If button is being turned off.
	if(strcmp("Linear Offsets", name) == 0)
		me->unlink();	// Turn off any mouse tracking.

}

Boolean AvAxisConfig::isAngular(const int axis)const
{Boolean isangular;

	if(ivi_ == NULL)
		return FALSE;
	// See axis is in 'angular' units.
	AvDataSet::AXISPRINTFORMAT fmt =
				ivi_->accessor()->axisPrintFormat(axis);
	switch(fmt) {
	case AvDataSet::HMS:
	case AvDataSet::DMS:
		isangular = TRUE;
		break;
	case AvDataSet::DEFAULT:
	default:
		isangular = FALSE;
		break;
	}
	return isangular;
}

// Returns TRUE if axis should be display in 'offset' format.
// TRUE if everything is to be drawn in offset format or axis is angular pos.
Boolean AvAxisConfig::useOffsetLabel(const int axis)const
{
	// See if everything is in offset format.
	if(!info_->gAngularOnly_.getValue())
		return TRUE;
	return isAngular(axis);
}

// Change the X & Y axis labels. If which is OFFSET, use labels for offsets,
// If which is DEFAULT, use std labels.
// This does not result in a redraw.
void AvAxisConfig::makeAxisLabels(const int which)
{ AvAccessor *acc = ivi_->accessor();

	if(which)
	{ AvWPosition wp(acc->XYZToWorld(info_->gAxisOffsets_.getValue()));

		if(useOffsetLabel(0))
		{ AvString xstr = "\\gD ";	// PGPLOT for DELTA.
			xstr += acc->axisName(0);
			xstr += " (Origin: ";
			xstr += acc->axisValueUnits(0, wp(0));
			xstr += ")";
			info_->xLabel_.text_.initialString(xstr.chars(), TRUE);
		}
		else
		  info_->xLabel_.text_.initialString(xDefLabel_.chars(), TRUE);

		if(useOffsetLabel(1))
		{ AvString ystr = "\\gD ";	// PGPLOT for DELTA.
			ystr += acc->axisName(1);
			ystr += " (Origin: ";
			ystr += acc->axisValueUnits(1, wp(1));
			ystr += ")";
			info_->yLabel_.text_.initialString(ystr.chars(), TRUE);
		}
		else
		  info_->yLabel_.text_.initialString(yDefLabel_.chars(), TRUE);
	}
	else	// Return to defaults.
	{ Boolean isAbs =
		(info_->gAxisType_.getValue() == AvAxisOptions::ABSPIXEL);

		if(isAbs)	// For ABSPIXEL, format is "<axisname> (Pixel)"
		{AvString xstr = acc->axisName(0);
		 AvString ystr = acc->axisName(1);
			xstr += " (Pixel)";
			ystr += " (Pixel)";
			info_->xLabel_.text_.initialString(xstr, TRUE);
			info_->yLabel_.text_.initialString(ystr, TRUE);
		}
		else	// Restore to default strings.
		{ info_->xLabel_.text_.initialString(xDefLabel_.chars(), TRUE);
		  info_->yLabel_.text_.initialString(yDefLabel_.chars(), TRUE);
		}
	}
}

// Called when "Offset Only Angular Positions" button is pressed so labels can
// be adjusted.
void AvAxisConfig::angularCB(Widget w, XtPointer cbdata, XtPointer)
{AvAxisConfig	*me = (AvAxisConfig *)cbdata;
 Boolean isSet = getToggle(w);
 Boolean wasSet = me->info_->gAngularOnly_.getValue();

	// Set the value to its new state w/o updating, change labels
	// then change it back so the redraw happens when the default
	// handler gets called.
	me->info_->gAngularOnly_.setValue(isSet, FALSE);
	me->makeAxisLabels(OFFSET);

	me->info_->gAngularOnly_.setValue(wasSet, FALSE);
}

// Called when reset button is pressed. Rests offset position to center
// of image.
void AvAxisConfig::resetOffsetCB(Widget , XtPointer cbdata, XtPointer)
{AvAxisConfig	*me = (AvAxisConfig *)cbdata;
 AvAxisOptions	*opt;
 AvAccessor	*acc;

	if((me == NULL) || ((opt = me->info_)==NULL))
	{	cerr << "AvAxisConfig::resetCB: NULL argument." << endl;
		return;
	}
	if(me->ivi_ == NULL)
		return;
	acc = me->ivi_->accessor();
	opt->gAxisOffsets_.disableUpdate();
	 opt->gAxisOffsets_.setValue(acc->shape()/2);
	 me->makeAxisLabels(OFFSET);
	opt->gAxisOffsets_.enableUpdate();
	opt->gAxisOffsets_.notify();
}
////////////////////////////////////////////////////////////////

// Call the callback for the particular reason.
void AvAxisConfig::callCallback(const CBReason why)
{
Boolean immediate = False;

	switch(why) {
	case ACCEPT:
	case CANCEL:
	case HELP:
	case DESTROYED:
		immediate = TRUE;
		break;
	case ADD:
	case DELETE:
//		saveValues(info_);	// Update the Axis level list.
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
void AvAxisConfig::addCallback(AvAxisConfigCallback cb, XtPointer cbdata)
{
	callback_ = cb;
	cbdata_ = cbdata;
}
