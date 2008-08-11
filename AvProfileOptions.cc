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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvProfileOptions.cc,v 19.0 2003/07/16 05:48:03 aips2adm Exp $
//
// $Log: AvProfileOptions.cc,v $
// Revision 19.0  2003/07/16 05:48:03  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:37  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/30 22:41:58  hravlin
// Made sure a string pointer always gets set.
//
// Revision 17.0  2001/11/12 19:43:05  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:12  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:41  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:33:54  hravlin
// Changed some char * to const char *.
//
// Revision 14.0  2000/03/23 16:09:09  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:41:00  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:24  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:02:16  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:56:31  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.2  1998/06/09 20:36:38  hr
// Removed a couple of unused variables.
//
// Revision 9.1  1997/12/17 21:51:48  hr
// Changes to remove warnings from MIPSpro.7x compiler.
//
// Revision 9.0  1997/08/25 21:31:48  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.3  1997/08/11 21:10:24  hr
// Changed default profile color to RED so aips++ people can see X profile.
//
// Revision 8.2  1997/08/01 21:21:46  hr
// Changed viewport size to a slider from an intmap and added the
// gDrawAxisLabels_ variable.
//
// Revision 8.1  1997/06/23 20:00:41  hr
// Added x/yMinMax_ variables.
//
// Revision 8.0  1997/02/20 03:19:33  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/02/10 21:07:40  hr
// Edited the default marker types script. Removed the unused min/max
// marker type variables. Generated default marker types start at 0 rather
// than -31 and there is no upper limit for the default type.
//
// Revision 7.2  1997/02/09 00:18:31  hr
// Cleanup and changed some doubleNotifers to integerNotifiers.
//
// Revision 7.1  1997/02/05 17:59:25  hr
// Container to hold profile setup variables.
//
//
//---------------------------------------------------------------------------

/* AvProfileOptions.cc
Holds information describing how to draw profiles.
*/

#include "AvProfileOptions.h"
#include <stdio.h>
#include <X11/Intrinsic.h>
#include <Xm/ToggleBG.h>
#include <AvString.h>
#include <AvGUIParts.h>		// PGPLOTColors().

// TCL Variable name for axis info.
static const char *AXISSTR = "axis";


////////////////////////////////////////////////////////////////

// TCL script to initialize defaults.
// marker 0 is set to be a filled circle.
static const char *defaultScript = \
	"set aipsview(profile,drawAxes) TRUE\n\
	set aipsview(profile,drawXprofile) TRUE\n\
	set aipsview(profile,drawYprofile) TRUE\n\
	set aipsview(profile,drawLabels) TRUE\n\
	set aipsview(profile,drawXLabel) TRUE\n\
	set aipsview(profile,drawYLabel) TRUE\n\
	set aipsview(profile,font) Normal\n\
	set aipsview(profile,xLabelFont) Normal\n\
	set aipsview(profile,yLabelFont) Normal\n\
	set aipsview(profile,titleFont) Normal\n\
	set aipsview(profile,numMarkers) 1\n\
	set aipsview(profile,marker00Symbol) -26\n\
	set aipsview(profile,numMarkerToggleTypes) 8\n\
	set aipsview(profile,markerToggleType00) 2\n\
	set aipsview(profile,markerToggleType01) 3\n\
	set aipsview(profile,markerToggleType02) 4\n\
	set aipsview(profile,markerToggleType03) 5\n\
	set aipsview(profile,markerToggleType04) 12\n\
	set aipsview(profile,markerToggleType05) 16\n\
	set aipsview(profile,markerToggleType06) 18\n\
	set aipsview(profile,markerToggleType07) -26\n\
	set aipsview(profile,markerToggleType08) -3\n\
	set aipsview(profile,markerToggleType09) 0\n\
	set aipsview(profile,markerToggleType10) 1\n\
	set aipsview(profile,markerToggleType11) 7\n\
	set aipsview(profile,markerToggleType12) 28\n\
	set aipsview(profile,markerToggleType13) 29\n\
	set aipsview(profile,markerToggleType14) 30\n\
	set aipsview(profile,markerToggleType15) 31\n\
	";

////////////////////////////////////////////////////////////////
////		IntMaps for converting between strings and ints.
////////////////////////////////////////////////////////////////
static AvProfileOptions::IntMap PSDevices[] = {
	{ AvProfileOptions::MONOPS, "Monochrome, Portrait"},
	{ AvProfileOptions::COLORPS, "Color, Portrait"},
	{ AvProfileOptions::COLORPS_LS, "Color, Landscape"},
	{ AvProfileOptions::MONOPS_LS, "Monochrome, Landscape"},
	{ 0, NULL}
	};

// Table to convert between font names and their integer values.
static AvProfileOptions::IntMap Fonts[] = {
	{ AvProfileOptions::NORMALFONT, "Normal"},
	{ AvProfileOptions::ROMAN, "Roman"},
	{ AvProfileOptions::ITALIC, "Italic"},
	{ AvProfileOptions::SCRIPT, "Script"},
	{0, NULL}
	};

// How to justify text.
static AvProfileOptions::IntMap Justify[] = {
	{ AvProfileOptions::LEFT, "Start"},
	{ AvProfileOptions::CENTER, "Center"},
	{ AvProfileOptions::RIGHT, "End"},
	{0, NULL}
	};

// Axis label positioning.
static AvProfileOptions::IntMap AxisLabelPosition[] = {
	{ AvProfileOptions::OFF, "Off"},
	{ AvProfileOptions::NORMALPOSITION, "Normal"},
	{ AvProfileOptions::ALTERNATE, "Alternate"},
	{0, NULL}
	};

// Which side should a label be on? TopBottom & LeftRight are setup so
// entry 0 is the default.
static AvProfileOptions::IntMap TopBottom[] = {
	{ AvProfileOptions::BOTTOM, "Bottom"},
	{ AvProfileOptions::TOP, "Top"},
	{ 0, NULL},
};

static AvProfileOptions::IntMap LeftRight[] = {
	{ AvProfileOptions::LEFT, "Left"},
	{ AvProfileOptions::RIGHT, "Right"},
	{ AvProfileOptions::LEFTP, "LeftP"},
	{ AvProfileOptions::RIGHTP, "RightP"},
	{ 0, NULL},
};

static AvProfileOptions::IntMap Label[] = {
	{ AvProfileOptions::DEFAULTLABEL, "Default"},
	{ AvProfileOptions::TEXTLABEL, "TextField"},
	{ 0, NULL}
	};

static AvProfileOptions::IntMap TickMarks[] = {
	{ AvProfileOptions::OFF, "Off"},
	{ AvProfileOptions::DEFAULT, "Default"},
	{ AvProfileOptions::ON, "Slider"},
	{ 0, NULL}
	};

static AvProfileOptions::IntMap LabelFormat[] = {
	{ AvProfileOptions::DEFAULT, "Default"},
	{ AvProfileOptions::DECIMAL, "Decimal"},
	{ AvProfileOptions::EXPONENTIAL, "Exponential"},
	{ 0, NULL}
	};

// Color information for all the color toggles.
static AvProfileOptions::IntMap ZeroLine[] = {
	{AvProfileOptions::OFF, "Off"},
	{AvProfileOptions::WITHTICKS, "With Ticks"},
	{AvProfileOptions::WITHOUTTICKS, "Without Ticks"},
	{ 0, NULL}
	};
	
// Are axes displayed as found, min first, max first or flipped?
static AvProfileOptions::IntMap minmax[] = {
	{AvProfileOptions::DEFAULT, "Default"},
	{AvProfileOptions::FLIPPED, "Flipped"},
	{AvProfileOptions::MINFIRST, "Min First"},
	{AvProfileOptions::MAXFIRST, "Max First"},
	{0, NULL}
	};

#if 0
static AvProfileOptions::IntMap xyFormat[] = {
	{AvProfileOptions::FRACTION, "Fraction"},
	{AvProfileOptions::WORLD, "World"},
	{0, NULL}
	};
#endif

static AvProfileOptions::IntMap lineStyle[] = {
	{ AvProfileOptions::SOLID, "SOLID"},
	{ AvProfileOptions::DASHED, "DASHED"},
	{ AvProfileOptions::DOTDASH, "DOTDASH"},
	{ AvProfileOptions::DOTTED, "DOTTED"},
	{ AvProfileOptions::DASHDOTDOT, "DASHDOTDOT"},
	{ 0, NULL}
	};

static AvProfileOptions::IntMap plotTypes[] = {
	{ AvProfileOptions::NORMALPLOT, "Normal"},
	{ AvProfileOptions::HISTOGRAM, "Histogram"},
	{ AvProfileOptions::MARKERSONLY, "Markers Only"},
	{ 0, NULL}
	};

#if 0
static AvProfileOptions::IntMap VPSizes[] = {
	{ AvProfileOptions::NORMALSIZE, "Normal"},
	{ AvProfileOptions::VP75, "0.75"},
	{ AvProfileOptions::VP50, "0.50"},
	{ AvProfileOptions::VP25, "0.25"},
	{ 0, NULL}
	};
#endif

#if 0
static AvProfileOptions::IntMap Profiles[] = {
	{ AvProfileOptions::LOCAL, "Local"},
	{ AvProfileOptions::WORLDPROFILE, "World"},
	{ AvProfileOptions::INCLUDED, "Included"},
	{ 0, NULL}
	};
#endif

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

// ( Initializers with "TRUE" are integerNotifiers configured as Boolean).
AvProfileOptions::AvProfileOptions(const char *fileName, const char *tclname) :
	gDrawLabels_(TRUE),
	gDrawFrame_(TRUE), gDrawGrid_(TRUE), gDrawAxisLabels_(TRUE),
	xLabel_(XLABEL), yLabel_(YLABEL), title_(TITLE)
	/*, xAxis_(XAXIS), yAxis_(YAXIS) */ 
{
	init(fileName, tclname);
}

void AvProfileOptions::init(const char *filename, const char *tclname)
{
	if(tclname == NULL)
		tclname = AXISSTR;
	cnfg_ = new AvConfigInfo(filename, tclname, defaultScript);
	cnfg_->ref();

	gPSDevice_.init("outputDevice", cnfg_, this, PSDevices, 0, MONOPS);
	gDrawLabels_.init("drawLabels", cnfg_, this, TRUE);
	gFont_.init("font", cnfg_, this, Fonts, 0, NORMALFONT);
	gLineWidth_.init("lineWidth", cnfg_, this, 2);
	gLineStyle_.init("lineStyle", cnfg_, this, lineStyle, 0, SOLID);
	gPlotType_.init("plotType", cnfg_, this, plotTypes, 0, NORMALPLOT);
	gCharScale_.init("charScale", cnfg_, this, 1.0);
	gAxisCharScale_.init("axisCharScale", cnfg_, this, 1.0);
	gDrawFrame_.init("drawFrame", cnfg_, this, TRUE);
	gDrawGrid_.init("drawGrid", cnfg_, this, FALSE);
	gDrawAxisLabels_.init("drawAxisLabels", cnfg_, this, TRUE);
	const IntMap *Colors = PGPLOTColors();
	gAxisColor_.init("axisColor", cnfg_, this, Colors, 0, WHITE);
	gProfileColor_.init("profileColor", cnfg_, this, Colors, 0, RED);
#if 0
	gViewportSize_.init("viewportSize", cnfg_, this, VPSizes, 0,
			    NORMALSIZE);
#else
	gViewportSize_.init("viewportSize", cnfg_, this, 0.75);
#endif

#if 0
	gProfiles_.init("profiles", cnfg_, this, Profiles, 0,
			LOCAL);
#endif
	gDrawZeroLine_.init("drawZeroLine", cnfg_, this, ZeroLine, 0,
			    WITHOUTTICKS);

	// In lieu of individual axis instances.
	// 
	xMinMax_.init("XaxisLayout", cnfg_, this, minmax, 0, DEFAULT);
	yMinMax_.init("YaxisLayout", cnfg_, this, minmax, 0, DEFAULT);

	xLabel_.init(cnfg_, this, 0);
	yLabel_.init(cnfg_, this);
	title_.init(cnfg_, this, 1);
//	xAxis_.init(cnfg_, this);
//	yAxis_.init(cnfg_, this);


//	gMinMarkerIndex_.init("minMarkerIndex", cnfg_, this, -31);
//	gMaxMarkerIndex_.init("maxMarkerIndex", cnfg_, this, 126);

	// Build an IntMap for the toggles
	// (The user can choose how many marker toggles there are and
	// what marker type is associated with each one.
	cnfg_->getInt("numMarkerToggleTypes", gNumMarkerToggleTypes_, 8);
	gMarkerToggleTypes_ = new IntMap[gNumMarkerToggleTypes_ +1];

	for(int mt=0; mt< gNumMarkerToggleTypes_; mt++)
	{char *name, vstring[32];
	 int val;
//	 int deftype = (mt-31) % 126;
	 int deftype = mt;

		sprintf(vstring, "markerToggleType%02d", mt);
		cnfg_->getInt(vstring, val, deftype);
		gMarkerToggleTypes_[mt].value = val;
		name = new char[32];
		// Since the label is never shown, the name may be pointless.
		sprintf(name, "Marker Toggle %d", mt);
		gMarkerToggleTypes_[mt].name = name;
	}
	// Null terminate the table.
	gMarkerToggleTypes_[gNumMarkerToggleTypes_].value = 0;
	gMarkerToggleTypes_[gNumMarkerToggleTypes_].name = NULL;
	

	// Build the marker objects.
//	cnfg_->getInt("numMarkers", numMarkers_, 1);
	// The profile has 1 and only one marker.
	numMarkers_ = 1;
	markers_ = new marker[numMarkers_];
	for(int n=0; n< numMarkers_; n++)
		markers_[n].init(cnfg_, this, n);
}

AvProfileOptions::~AvProfileOptions()
{
	delete [] markers_;
	for(int mt=0; mt< gNumMarkerToggleTypes_; mt++)
		delete [] gMarkerToggleTypes_[mt].name;
	delete [] gMarkerToggleTypes_;
	gMarkerToggleTypes_ = NULL;
	AvResource::UnRef(cnfg_);
}

void AvProfileOptions::touch()
{
	if(notifyEnabled())
	{
		invokeCallbacks(ValueChanged);
	}
}

const AvProfileOptions::axisLabel
	*AvProfileOptions::getLabelOptions(const OPTIONTYPE axis)const
{
	switch(axis) {
	case XLABEL:
		return &xLabel_;
	case YLABEL:
		return &yLabel_;
	case TITLE:
		return &title_;
	default:
		return NULL;
	};
}

const AvProfileOptions::axis
	*AvProfileOptions::getAxisOptions(const OPTIONTYPE axis)const
{
	switch(axis) {
#if 0
	case XAXIS:
		return &xAxis_;
	case YAXIS:
		return &yAxis_;
#endif
	default:
		return NULL;
	};
}

const AvProfileOptions::marker *AvProfileOptions::getMarkerOptions(
					const int markerNumber)const
{
	if((markerNumber>= 0) && (markerNumber < numMarkers_))
		return &markers_[markerNumber];
	else
		return NULL;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
///			Options
///		(Holds a group of notifier variables)
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

AvProfileOptions::options::options(const int typeMask)
{
	typeMask_ = typeMask;
}

AvProfileOptions::axisLabel::axisLabel(const int typeMask): options(typeMask),
	drawLabel_(TRUE)
{

}

void AvProfileOptions::axisLabel::init(AvConfigInfo *cnfg, AvConductor *cond,
				    const int side)
{char buf[64];
 const char *axis = "";

	switch(typeMask()) {
	case AvProfileOptions::XLABEL:
			axis = "X";
			break;
	case AvProfileOptions::YLABEL:
			axis = "Y";
			break;
	case AvProfileOptions::TITLE:
			axis = "Title";
			break;

	}

	sprintf(buf, "draw%sLabel", axis);
	drawLabel_.init(buf, cnfg, cond, TRUE);

	sprintf(buf, "%scharacterScale", axis);
	charScale_.init(buf, cnfg, cond, 1.0);

	sprintf(buf, "%sSide", axis);
	IntMap *map = (typeMask()==YLABEL) ? LeftRight : TopBottom;
	side_.init(buf, cnfg, cond, map, 0, map[side].value);

	sprintf(buf, "from%sEdge", axis);
	float from;
	switch(typeMask()){
		case TITLE:
			from = 2.0;
			break;
		case YLABEL:
			from = 2.2;
			break;
		default:
		case XLABEL:
			from = 3.2;
			break;
	};
	fromEdge_.init(buf, cnfg, cond, from);

	sprintf(buf, "along%sEdge", axis);
	alongEdge_.init(buf, cnfg, cond, 0.5);

	sprintf(buf, "%sJustify", axis);
	justify_.init(buf, cnfg, cond, Justify, 0, CENTER);

	sprintf(buf, "%sLabel", axis);
	text_.init(buf, cnfg, cond, Label, 0, DEFAULTLABEL, TEXTLABEL,
		   "", DEFAULTLABEL);
}

AvProfileOptions::axis::axis(const int mask): options(mask),
	vertical_(TRUE)
{
}


void AvProfileOptions::axis::init(AvConfigInfo *cnfg, AvConductor *cond)
{ char buf[64];
 const char *axis;

	switch(typeMask()) {
	case AvProfileOptions::XAXIS:
			axis = "X";
			break;
	case AvProfileOptions::YAXIS:
			axis = "Y";
			break;
	default:
			axis = "X";		// Error.
	}

	sprintf(buf, "draw%sZeroLine", axis);
	drawZeroLine_.init(buf, cnfg, cond, ZeroLine, 0, OFF);

	sprintf(buf, "%sMajorTickMarks", axis);
	major_.init(buf, cnfg, cond, TickMarks, 0, DEFAULT);

	sprintf(buf, "%sMajorTicMarkIntervals", axis);
	majorIntervals_.init(buf, cnfg, cond, 4);

	sprintf(buf, "%sMinorTickMarks", axis);
	minor_.init(buf, cnfg, cond, TickMarks, 0, DEFAULT);

	sprintf(buf, "%sMinorTicMarkIntervals", axis);
	minorIntervals_.init(buf, cnfg, cond, 4.0);

	sprintf(buf, "%sAxisLabelPosition", axis);
	position_.init(buf, cnfg, cond, AxisLabelPosition, 0, NORMALPOSITION);

	sprintf(buf, "vertical%sLabels", axis);
	vertical_.init(buf, cnfg, cond, TRUE);

	// Label Format (Decimal, Exponential).
	sprintf(buf, "vertical%sLabelFormat", axis);
	decimal_.init(buf, cnfg, cond, LabelFormat, 0, DEFAULT);
}

AvProfileOptions::marker::marker(): options(MARKER), show_(TRUE)
{
}

void AvProfileOptions::marker::init(AvConfigInfo *cnfg, AvProfileOptions *cond,
				   const int markerNum)
{char buf[64];

	markerIndex_ = markerNum;

	sprintf(buf, "showMarker%02d", markerNum);
	show_.init(buf, cnfg, cond, FALSE);

	sprintf(buf, "marker%02dScale", markerNum);
	scale_.init(buf, cnfg, cond, 1.0);

	sprintf(buf, "marker%02dColor", markerNum);
	const IntMap *Colors = PGPLOTColors();
	color_.init(buf, cnfg, cond, Colors, 0, WHITE);

	sprintf(buf, "marker%02dSymbol", markerNum);
#if 0
	type_.init(buf, cnfg, cond, markerNum);
#else
	int mt, nmt = cond->gNumMarkerToggleTypes_;
	if( nmt> 0)
		mt = cond->gMarkerToggleTypes_[markerNum % nmt].value;
	else
		mt = 0;
	type_.init(buf, cnfg, cond,
		   cond->gMarkerToggleTypes_,
		   cond->gNumMarkerToggleTypes_, mt);
#endif

#if 0
	sprintf(buf, "marker%02dXPosition", markerNum);
	x_.init(buf, cnfg, cond, 0.5);

	sprintf(buf, "marker%02dYPosition", markerNum);
	y_.init(buf, cnfg, cond, 0.5);

	sprintf(buf, "marker%02dxyFormat", markerNum);
	xyFormat_.init(buf, cnfg, cond, xyFormat, 0, WORLD);
#endif
}
