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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvAxisOptions.cc,v 19.0 2003/07/16 05:46:31 aips2adm Exp $
//
// $Log: AvAxisOptions.cc,v $
// Revision 19.0  2003/07/16 05:46:31  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:19  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/30 22:31:30  hravlin
// Added a default: to a switch to ensure a variable is always set.
//
// Revision 17.0  2001/11/12 19:41:46  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:41:57  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:07:50  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 18:17:14  hravlin
// Replaced "char *axis" by "const char * axis".
//
// Revision 14.0  2000/03/23 16:07:32  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:11  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:17  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:57:50  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:51:20  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.5  1998/06/09 21:25:25  hr
// Removed two unsused variables.
//
// Revision 9.4  1998/02/26 22:20:03  hr
// Changed default value of labelTracksGrid_ to TRUE.
//
// Revision 9.3  1998/02/20 20:44:45  hr
// Changes to support different label positioning.
//
// Revision 9.2  1997/12/17 21:50:06  hr
// Changes to remove warnings from MIPSpro.7x compiler.
//
// Revision 9.1  1997/11/17 16:56:30  hr
// Added support for axis offsets and captions.
//
// Revision 9.0  1997/08/25 21:23:41  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.5  1997/05/22 20:32:31  hr
// Added virtual destructors for options and its subclasses.
//
// Revision 8.4  1997/04/25 16:03:45  hr
// Converted to using AvBooleanNotifiers. Changed axis type to only two entries.
// Changed names of some strings. Added useGlobalColor variable to axis objs.
//
// Revision 8.3  1997/04/24 20:42:01  hr
// Renamed AxisType from "Linear" to "Rectilinear".
//
// Revision 8.2  1997/04/23 19:46:20  hr
// Lower cased a "DEFAULT" string.
//
// Revision 8.1  1997/04/23 17:13:44  hr
// Changes to support options for drawing curvilinear axes.
// Removed marker's positioning format. It's always fractional now.
//
// Revision 8.0  1997/02/20 03:19:24  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.4  1997/02/10 21:03:42  hr
// Marker type selector switched from slider to toggles.
// Removed a bunch of old unused code.
//
// Revision 7.3  1997/02/09 00:06:17  hr
// Changed from internally defined Notifiers to AvNotifiers.
//
// Revision 7.2  1997/02/05 17:27:08  hr
// g++ was complaining about the ordering of variable initialization in
// the constructor.
//
// Revision 7.1  1997/01/24 20:42:11  hr
// Object to hold axis configuration parameters.
//
//
//---------------------------------------------------------------------------
/* AvAxisOptions.cc
Holds information describing how to draw axes and axis labels.
*/

#include "AvAxisOptions.h"
#include <stdio.h>
#include <X11/Intrinsic.h>
#include <Xm/ToggleBG.h>
#include <AvGUIParts.h>		// PGPLOTColors().
#include <math.h>		// sin/cos
//#define DEBUG

#ifdef DEBUG
#include <iostream.h>
#endif
#include <AvString.h>

// TCL Variable name for axis info.
static const char *AXISSTR = "axis";


////////////////////////////////////////////////////////////////
static unsigned long _timestamp = 1;
static inline unsigned long TimeStamp()
{	return _timestamp++;}

// TCL script to initialize defaults.
// (wedge axis is set to 0 markers because a: no one probably wants then.
//  and b: they can't be positioned via the mouse).
static const char *defaultScript = \
	"set aipsview(imageAxis,drawAxes) TRUE\n\
	set aipsview(imageAxis,drawXImageAxis) TRUE\n\
	set aipsview(imageAxis,drawYImageAxis) TRUE\n\
	set aipsview(imageAxis,drawLabels) TRUE\n\
	set aipsview(imageAxis,drawXLabel) TRUE\n\
	set aipsview(imageAxis,drawYLabel) TRUE\n\
	set aipsview(imageAxis,font) Normal\n\
	set aipsview(imageAxis,xLabelFont) Normal\n\
	set aipsview(imageAxis,yLabelFont) Normal\n\
	set aipsview(imageAxis,titleFont) Normal\n\
	set aipsview(imageAxis,numMarkerToggleTypes) 8\n\
	set aipsview(imageAxis,markerToggleType00) 2\n\
	set aipsview(imageAxis,markerToggleType01) 3\n\
	set aipsview(imageAxis,markerToggleType02) 4\n\
	set aipsview(imageAxis,markerToggleType03) 5\n\
	set aipsview(imageAxis,markerToggleType04) 12\n\
	set aipsview(imageAxis,markerToggleType05) 16\n\
	set aipsview(imageAxis,markerToggleType06) 18\n\
	set aipsview(imageAxis,markerToggleType07) -26\n\
	set aipsview(imageAxis,markerToggleType08) -3\n\
	set aipsview(imageAxis,markerToggleType09) 0\n\
	set aipsview(imageAxis,markerToggleType10) 1\n\
	set aipsview(imageAxis,markerToggleType11) 7\n\
	set aipsview(imageAxis,markerToggleType12) 28\n\
	set aipsview(imageAxis,markerToggleType13) 29\n\
	set aipsview(imageAxis,markerToggleType14) 30\n\
	set aipsview(imageAxis,markerToggleType15) 31\n\
	set aipsview(wedgeAxis,drawGrid) FALSE\n\
	set aipsview(wedgeAxis,drawXZeroLine) {Without Ticks} \n\
	set aipsview(wedgeAxis,XlinearMajorTickMarks) Off\n\
	set aipsview(wedgeAxis,XlinearMinorTickMarks) Off\n\
	set aipsview(wedgeAxis,XAxisLabelPosition) Off\n\
	set aipsview(wedgeAxis,YAxisLabelPosition) Alternate\n\
	set aipsview(wedgeAxis,numMarkers) 0\n\
	set aipsview(wedgeAxis,numCaptions) 0\n\
	";

////////////////////////////////////////////////////////////////
////		IntMaps for converting between strings and ints.
////////////////////////////////////////////////////////////////

// Table to convert between font names and their integer values.
static IntMap Fonts[] = {
	{ AvAxisOptions::NORMALFONT, "Normal"},
	{ AvAxisOptions::ROMAN, "Roman"},
	{ AvAxisOptions::ITALIC, "Italic"},
	{ AvAxisOptions::SCRIPT, "Script"},
	{0, NULL}
	};

// AvAxisConfig assumes these names don't change.
static IntMap AxisType[] = {
	{ AvAxisOptions::CURVILINEAR, "Curvilinear"},
	{ AvAxisOptions::LINEAR, "Rectilinear"},
	{ AvAxisOptions::LINEAROFFSET, "Linear Offsets"},
	{ AvAxisOptions::ABSPIXEL, "Abs Pixel"},
	{ 0, NULL}
	};


// How to display curvilinear axis.
static IntMap AxisFormat[] = {
	 { AvAxisOptions::DEFAULT, "Default"},
	 { AvAxisOptions::PLAIN, "Raw"},
#if 0
	// Not really needed.
	 { AvAxisOptions::ANGLE_0_360, "[0, 360)"},		/* 'A' */
	 { AvAxisOptions::ANGLE_M180_180, "(-180 180]"},	/* 'B' */
	 { AvAxisOptions::ANGLE_M180_180U, "[-180, 180)U"},	/* 'C' */
	 { AvAxisOptions::ANGLE_DMS_360, "DMS [0,360)"},
	 { AvAxisOptions::ANGLE_DMS_180, "DMS (-180,180]"},	/* 'E' */
	 { AvAxisOptions::ANGLE_DMS_360U, "DMS [0,360)U"},
	 { AvAxisOptions::ANGLE_HMS_24, "HMS [0,24)"},		// 'G'
	 { AvAxisOptions::ANGLE_HMS_12, "HMS (-12,12]"},	// 'H'
	 { AvAxisOptions::ANGLE_HMS_12U, "HMS (-12, 12]U"},
	 { AvAxisOptions::TIME_HMS, "Time HMS"},
#endif
	 { 0, NULL}
	};

// How to display offsets for linear axis.
static IntMap LinearAxisFormat[] = {
	 { AvAxisOptions::DEFAULT, "Default"},
//	 { AvAxisOptions::DEGREES, "Degrees"},
	 { AvAxisOptions::ARCMIN, "arcmin"},
	 { AvAxisOptions::ARCSEC, "arcsec"},
	 { AvAxisOptions::MARCSEC, "marcsec"},
	 { AvAxisOptions::RELPIXEL, "Pixel"},
	 { 0, NULL}
	};

// How to justify text.
static IntMap Justify[] = {
	{ AvAxisOptions::LEFT, "Start"},
	{ AvAxisOptions::CENTER, "Center"},
	{ AvAxisOptions::RIGHT, "End"},
	{0, NULL}
	};
//static const int NJUSTIFY = sizeof(Justify)/sizeof(*Justify) -1;

// Axis label positioning.
static IntMap AxisLabelPosition[] = {
	{ AvAxisOptions::OFF, "Off"},
	{ AvAxisOptions::NORMALPOSITION, "Normal"},
	{ AvAxisOptions::ALTERNATE, "Alternate"},
	{0, NULL}
	};

// Which side should a label be on? TopBottom & LeftRight are setup so
// entry 0 is the default.
static IntMap TopBottom[] = {
	{ AvAxisOptions::BOTTOM, "Bottom"},
	{ AvAxisOptions::TOP, "Top"},
	{ 0, NULL},
};

static IntMap LeftRight[] = {
	{ AvAxisOptions::LEFT, "Left"},
	{ AvAxisOptions::RIGHT, "Right"},
	{ AvAxisOptions::LEFTP, "LeftP"},
	{ AvAxisOptions::RIGHTP, "RightP"},
	{ 0, NULL},
};

static IntMap LR_TB[] = {
	{ AvAxisOptions::BOTTOM, "Bottom"},
	{ AvAxisOptions::TOP, "Top"},
	{ AvAxisOptions::LEFT, "Left"},
	{ AvAxisOptions::RIGHT, "Right"},
	{ AvAxisOptions::LEFTP, "LeftP"},
	{ AvAxisOptions::RIGHTP, "RightP"},
	{ 0, NULL},
};

static IntMap Label[] = {
	{ AvAxisOptions::DEFAULTLABEL, "Default"},
	{ AvAxisOptions::TEXTLABEL, "Text Field"},
	{ 0, NULL}
	};

static IntMap TickMarks[] = {
	{ AvAxisOptions::OFF, "Off"},
	{ AvAxisOptions::DEFAULT, "Default"},
	{ AvAxisOptions::ON, "Slider"},
	{ 0, NULL}
	};

static IntMap LabelFormat[] = {
	{ AvAxisOptions::DEFAULT, "Default"},
	{ AvAxisOptions::DECIMAL, "Decimal"},
	{ AvAxisOptions::EXPONENTIAL, "Exponential"},
	{ 0, NULL}
	};

#if 0
// Color information for all the color toggles.
static IntMap Colors[] = {
	{AvAxisOptions::RED, "Red"},
	{AvAxisOptions::GREEN, "Green"},
	{AvAxisOptions::BLUE, "Blue"},
	{AvAxisOptions::CYAN, "Cyan"},
	{AvAxisOptions::MAGENTA, "Magenta"},
	{AvAxisOptions::YELLOW, "Yellow"},
	{AvAxisOptions::BLACK, "Black"},
	{AvAxisOptions::WHITE, "White"},
	{ 0, NULL}
	};
#endif
// Color information for all the color toggles.
static const IntMap *Colors=NULL;

static IntMap ZeroLine[] = {
	{AvAxisOptions::OFF, "Off"},
	{AvAxisOptions::WITHTICKS, "With Ticks"},
	{AvAxisOptions::WITHOUTTICKS, "Without Ticks"},
	{ 0, NULL}
	};
	
#if 0
static IntMap xyFormat[] = {
	{AvAxisOptions::WORLD, "World"},
	{AvAxisOptions::FRACTION, "Fraction"},
	{0, NULL}
	};
#endif

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

AvAxisOptions::AvAxisOptions(const char *fileName, const char *tclname) :
	xLabel_(XLABEL), yLabel_(YLABEL), title_(TITLE),
	xAxis_(XAXIS), yAxis_(YAXIS)

{
	init(fileName, tclname);
}

void AvAxisOptions::init(const char *filename, const char *tclname)
{
	if(Colors == NULL)
		Colors = PGPLOTColors();
	if(tclname == NULL)
		tclname = AXISSTR;
	cnfg_ = new AvConfigInfo(filename, tclname, defaultScript);
	cnfg_->ref();

	gDrawLabels_.init("drawLabels", cnfg_, this, TRUE);
	gFont_.init("font", cnfg_, this, Fonts, 0, NORMALFONT);
	gAxisType_.init("axis", cnfg_, this, AxisType, 0, CURVILINEAR);
	gLineWidth_.init("lineWidth", cnfg_, this, 2);
	gCharScale_.init("charScale", cnfg_, this, 1.0);
	gDrawFrame_.init("drawFrame", cnfg_, this, TRUE);
	gDrawGrid_.init("drawGrid", cnfg_, this, TRUE);
	gAxisOffsets_.init("linearAxisOffsets", cnfg_, this, 0);
	gLinearAxisFormat_.init("linearAxisFormat", cnfg_, this,
				LinearAxisFormat, 0, DEFAULT);
	gAngularOnly_.init("onlyAngularPositions", cnfg_, this, TRUE);
	gAxisColor_.init("axisColor", cnfg_, this, Colors, 0, WHITE);

	xLabel_.init(cnfg_, this, 0);
	yLabel_.init(cnfg_, this);
	title_.init(cnfg_, this, 1);
	xAxis_.init(cnfg_, this);
	yAxis_.init(cnfg_, this);

	int n;
	// Initialize any captions.
	cnfg_->getInt("numCaptions", numCaptions_, 2);
	captions_ = new axisLabel[numCaptions_];
	for(n=0; n< numCaptions_; n++)
		captions_[n].init(cnfg_, this, 0, n);

	// Build an IntMap for the toggles
	// (The user can choose how many marker toggles there are and
	// what marker type is associated with each one.
	cnfg_->getInt("numMarkerToggleTypes", gNumMarkerToggleTypes_, 8);
	gMarkerToggleTypes_ = new IntMap[gNumMarkerToggleTypes_ +1];

	for(int mt=0; mt< gNumMarkerToggleTypes_; mt++)
	{char *name, vstring[32];
	 int val;
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

	cnfg_->getInt("numMarkers", numMarkers_, 8);
	markers_ = new marker[numMarkers_];
	for(n=0; n< numMarkers_; n++)
		markers_[n].init(cnfg_, this, n);

//	gMinMarkerIndex_.init("minMarkerIndex", cnfg_, this, -31);
//	gMaxMarkerIndex_.init("maxMarkerIndex", cnfg_, this, 126);

	timestamp_ = TimeStamp();
}

AvAxisOptions::~AvAxisOptions()
{
	delete [] markers_;
	for(int mt=0; mt< gNumMarkerToggleTypes_; mt++)
		delete [] gMarkerToggleTypes_[mt].name;
	delete [] gMarkerToggleTypes_;
	gMarkerToggleTypes_ = NULL;
	AvResource::UnRef(cnfg_);
}

#if 0
// Copy from one to another. The target is assumed to already have been
// initialized.
void AvAxisOptions::copy(const AvAxisOptions &from, AvAxisOptions &to,
			const Boolean update)
{
	if(&to == &from)
	{	fprintf(stderr, "AvAxisOptions::copy: to = from\n");
		return;
	}
	if(update)
		to.touch();
	else
		to.timestamp_ = from.timestamp_;
}
#endif

#if 0
// Compares the two AxisOptions objects excluding timestamps.
Boolean AvAxisOptions::equals(const AvAxisOptions &i)const
{
	return TRUE;
}
#endif

#if 0
static char *Strdup(const char *str)
{
int	len;
char	*s;

	if(str == NULL)
		return NULL;

	len = strlen(str);
	s = new char[len+1];
	strcpy(s, str);

	return s;
}
#endif

void AvAxisOptions::touch()
{
	timestamp_ = TimeStamp();

	if(notifyEnabled())
	{
		invokeCallbacks(ValueChanged);
	}
}

const AvAxisOptions::axisLabel
	*AvAxisOptions::getLabelOptions(const OPTIONTYPE axis)const
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

const AvAxisOptions::axis
	*AvAxisOptions::getAxisOptions(const OPTIONTYPE axis)const
{
	switch(axis) {
	case XAXIS:
		return &xAxis_;
	case YAXIS:
		return &yAxis_;
	default:
		return NULL;
	};
}

const AvAxisOptions::marker *AvAxisOptions::getMarkerOptions(
					const int markerNumber)const
{
	if((markerNumber>= 0) && (markerNumber < numMarkers_))
		return &markers_[markerNumber];
	else
		return NULL;
}

const AvAxisOptions::axisLabel *AvAxisOptions::getCaptionOptions(
					const int captionNumber)const
{
	if((captionNumber>= 0) && (captionNumber < numCaptions_))
		return &captions_[captionNumber];
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

AvAxisOptions::options::options(const int typeMask)
{
	typeMask_ = typeMask;
}

AvAxisOptions::options::~options()
{
}


AvAxisOptions::axisLabel::axisLabel(const int typeMask): options(typeMask)
{

}

AvAxisOptions::axisLabel::axisLabel(): options(CAPTION)
{
	index_ = -1;
}

AvAxisOptions::axisLabel::~axisLabel()
{
}

void AvAxisOptions::axisLabel::init(AvConfigInfo *cnfg, AvConductor *cond,
				    const int side, const int index)
{char buf[64];
 char capbuf[64];
 const char *axis;
 float x = 0.5, y=0.5, angle=0.0;

	index_ = index;

	int typemask = typeMask();
	IntMap *map;

	switch(typemask) {
	default:
	case AvAxisOptions::XLABEL:
			axis = "X";
			map = TopBottom;
			x = 0.5;
			y = -0.20;
			break;
	case AvAxisOptions::YLABEL:
			axis = "Y";
			map = LeftRight;
			x = -0.20;
			y = 0.5;
			angle = 90.0;
			break;
	case AvAxisOptions::TITLE:
			axis = "Title";
			map = TopBottom;
			x = 0.5;
			y = 1.10;
			break;
	case AvAxisOptions::CAPTION:
			sprintf(capbuf, "Caption%d", index);
			axis = capbuf;
			map = LR_TB;
			x = 0.5;
			y = 0.5;
			angle = 0.0;
			break;
	}

	sprintf(buf, "draw%sLabel", axis);
	drawLabel_.init(buf, cnfg, cond, TRUE);

	sprintf(buf, "%scharacterScale", axis);
	charScale_.init(buf, cnfg, cond, 1.0);

	sprintf(buf, "%sSide", axis);
//	IntMap *map = (typeMask()==YLABEL) ? LeftRight : TopBottom;
	side_.init(buf, cnfg, cond, map, 0, map[side].value);

#if 0
	sprintf(buf, "from%sEdge", axis);
	fromEdge_.init(buf, cnfg, cond, 5.0);

	sprintf(buf, "along%sEdge", axis);
	alongEdge_.init(buf, cnfg, cond, 0.5);
#else
	sprintf(buf, "%sX", axis);
	x_.init(buf, cnfg, cond, x);

	sprintf(buf, "%sY", axis);
	y_.init(buf, cnfg, cond, y);

	sprintf(buf, "%sAngle", axis);
	angle_.init(buf, cnfg, cond, angle);
#endif
	sprintf(buf, "%sJustify", axis);
	justify_.init(buf, cnfg, cond, Justify, 0, CENTER);

	sprintf(buf, "%sLabel", axis);
	text_.init(buf, cnfg, cond, Label, 0, DEFAULTLABEL, TEXTLABEL,
		   "", DEFAULTLABEL);

	sprintf(buf, "%sColor", axis);
	color_.init(buf, cnfg, cond, Colors, 0, WHITE);
	sprintf(buf, "%sUseGlobalColor", axis);
	useGlobalColor_.init(buf, cnfg, cond, TRUE);

	sprintf(buf, "%sopaqueTextBackground", axis);
	opaqueBackground_.init(buf, cnfg, cond, FALSE);
}

AvAxisOptions::axis::axis(const int mask): options(mask)
{
}

AvAxisOptions::axis::~axis()
{
}

void AvAxisOptions::axis::init(AvConfigInfo *cnfg, AvConductor *cond)
{ char buf[64];
 const char *axis;

	switch(typeMask()) {
	case AvAxisOptions::XAXIS:
			axis = "X";
			break;
	case AvAxisOptions::YAXIS:
			axis = "Y";
			break;
	default:
			axis = "X";		// Error.
	}

	sprintf(buf, "draw%sZeroLine", axis);
	drawZeroLine_.init(buf, cnfg, cond, ZeroLine, 0, OFF);

	sprintf(buf, "%slinearMajorTickMarks", axis);
	linMajor_.init(buf, cnfg, cond, TickMarks, 0, DEFAULT);

	sprintf(buf, "%slinearMajorTicMarkIntervals", axis);
	linMajorIntervals_.init(buf, cnfg, cond, 4);

	sprintf(buf, "%slinearMinorTickMarks", axis);
	minor_.init(buf, cnfg, cond, TickMarks, 0, DEFAULT);

	sprintf(buf, "%slinearMinorTicMarkIntervals", axis);
	linMinorIntervals_.init(buf, cnfg, cond, 4);

	sprintf(buf, "%sAxisLabelPosition", axis);
	position_.init(buf, cnfg, cond, AxisLabelPosition, 0, NORMALPOSITION);

	sprintf(buf, "vertical%sLabels", axis);
	vertical_.init(buf, cnfg, cond, TRUE);

	// Label Format (Decimal, Exponential).
	sprintf(buf, "vertical%sLabelFormat", axis);
	decimal_.init(buf, cnfg, cond, LabelFormat, 0, DEFAULT);

	// Curvilinear related
	sprintf(buf, "%scurvilinearMajorTickMarks", axis);
	crvMajor_.init(buf, cnfg, cond, TickMarks, 0, DEFAULT);

	sprintf(buf, "%scurvilinearMajorTicMarkIntervals", axis);
	crvMajorIntervals_.init(buf, cnfg, cond, 8);

	// Curvilinear only, ignored for Linear axis.
	sprintf(buf, "%sColor", axis);
	color_.init(buf, cnfg, cond, Colors, 0, WHITE);

	sprintf(buf, "%sUseGlobalColor", axis);
	useGlobalColor_.init(buf, cnfg, cond, TRUE);

	sprintf(buf, "%slabelTracksGrid", axis);
	labelTracksGrid_.init(buf, cnfg, cond, TRUE);

	sprintf(buf, "%sFormat", axis);
	format_.init(buf, cnfg, cond, AxisFormat, 0, DEFAULT);

	sprintf(buf, "%sLabelOnTop", axis);
	top_.init(buf, cnfg, cond, FALSE);

	if(typeMask() == AvAxisOptions::XAXIS)
	{	sprintf(buf, "%sLabelOnBottom", axis);
		bottom_.init(buf, cnfg, cond, TRUE);

		sprintf(buf, "%sLabelOnLeft", axis);
		left_.init(buf, cnfg, cond, FALSE);
	}
	else
	{	sprintf(buf, "%sLabelOnBottom", axis);
		bottom_.init(buf, cnfg, cond, FALSE);

		sprintf(buf, "%sLabelOnLeft", axis);
		left_.init(buf, cnfg, cond, TRUE);
	}

	sprintf(buf, "%sLabelOnRight", axis);
	right_.init(buf, cnfg, cond, FALSE);

}

AvAxisOptions::marker::marker(): options(MARKER), show_(TRUE)
{
}

AvAxisOptions::marker::~marker()
{
}

void AvAxisOptions::marker::init(AvConfigInfo *cnfg, AvAxisOptions *cond,
				   const int markerNum)
{char buf[64];

	markerIndex_ = markerNum;

	sprintf(buf, "showMarker%02d", markerNum);
	show_.init(buf, cnfg, cond, FALSE);

	sprintf(buf, "marker%02dScale", markerNum);
	scale_.init(buf, cnfg, cond, 1.0);

	sprintf(buf, "marker%02dColor", markerNum);
	color_.init(buf, cnfg, cond, Colors, 0, WHITE);

	sprintf(buf, "marker%02dSymbol", markerNum);
#if 0
	type_.init(buf, cnfg, cond, markerNum & 0xFF);
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

	sprintf(buf, "marker%02dXPosition", markerNum);
	x_.init(buf, cnfg, cond, 0.5);

	sprintf(buf, "marker%02dYPosition", markerNum);
	y_.init(buf, cnfg, cond, 0.5);

//	sprintf(buf, "marker%02dxyFormat", markerNum);
//	xyFormat_.init(buf, cnfg, cond, xyFormat, 0, WORLD);
}
