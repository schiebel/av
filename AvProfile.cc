//# Copyright (C) 1995-2003 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvProfile.cc,v 19.1 2003/10/01 21:35:32 hravlin Exp $
//
// $Log: AvProfile.cc,v $
// Revision 19.1  2003/10/01 21:35:32  hravlin
// Get the blanking value from Av::BlankedValue rather than a constant.
//
// Revision 19.0  2003/07/16 05:48:08  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:41  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.2  2002/01/30 22:41:06  hravlin
// Ensured that variables initialized in switch statements always get set.
//
// Revision 17.1  2002/01/22 22:15:17  hravlin
// Cast result of sizeof() to int for use with 64 bit compilers.
//
// Revision 17.0  2001/11/12 19:43:09  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:16  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:47  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:35:54  hravlin
// Changed some char * to const char *.
//
// Revision 14.0  2000/03/23 16:09:14  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/09/15 21:08:54  hravlin
// Stopped using DefaultRootWindow().
//
// Revision 13.0  1999/08/10 18:41:05  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:33  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:02:29  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:56:46  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.9  1998/06/09 21:36:17  hr
// Removed an unneeded variable.
//
// Revision 9.8  1998/06/08 21:36:22  hr
// updateProfileXLabels() was creating a 3 long vector instead of one with length 'rank'.
//
// Revision 9.7  1998/05/04 20:37:09  hr
// Removed some unused variables.
//
// Revision 9.6  1997/12/17 21:51:30  hr
// Changes to remove warnings from MIPSpro.7x compiler.
//
// Revision 9.5  1997/11/17 17:04:58  hr
// Removed a couple of unused variables from drawOnlyProfileUsing().
//
// Revision 9.4  1997/10/29 20:09:41  hr
// Removed an unused variable from drawOnlyProfileUsing(). Also, the previous
// edit fixed a problem with overlaying profiles from X and PGPLOT windows.
//
//
//---------------------------------------------------------------------------
/* AvProfile.cc

Draw profiles using X routines (quicker) or PGPLOT (better).
Hardcopy is available via AvPGProfile.
ASCII dumps are available.
Allows overlaying profiles from other windows.

The X version creates something like:
	Max Y label	|
			|
			|	profile window
	Min Y label	|
			-------------------------
			Min X label	Max X label

The labels only get changed/redrawn when the clip min/max (Y) or
subrange (X) changes.

TODO:

redrawProfile could speed redraws up.
(Need to save XPoint list for X, display list for PGPLOT).

Continue moving towards allowing profiles to be along other than Z axis.

Initially, X & Y axis data was going to be separated so both wouldn't
necessarily have to be regenerated each time. However, since the X axis
values (display of the image's Z axis) could change with Y, this isn't
doable. However, the labels probably won't be updated for really non-linear
axes.

Much of this code was moved from AvImageViewPanel. Some comments haven't
been updated yet. Also, some code cleanup can still be done.

*/


#include <string.h>
#include <Xm/Form.h>
#include <Xm/DrawingA.h>
#include <Xm/LabelG.h>
#include <Xm/Frame.h>

#include <AvProfile.h>
#include <AvProfileOptions.h>
#include <AvXPalette.h>
#include <AvGUIParts.h>
#include <AvString.h>
// For included profiles
#include <AvImageView.h>	// For included windows.
#include <AvImageWindow.h>
#include <AvImageViewPanel.h>
//////
#include <AvApp.h>

#ifdef PGPLOT
#include <AvPGCanvas.h>
#include <AvPGDriver.h>
#include <AvXCanvases.h>
#include <AvPGDriver.h>
#include <AvPolyline.h>

extern "C" {
#include <cpgplot.h>
}

// Environment variable.
static const char *PROFILE_FILENAME =	"AV_PROFILE_FILENAME";
#endif

////////////////////////////////////////////////////////////////
// Mask for accessor events we're interested in.
static const int ACCMASK = AvAccessor::DEFAULTMASK | AvDataSet::DEFAULTMASK
			 | AvAccessor::PLANE | AvAccessor::REGION;

// Aipsview uses both X and PGPLOT output windows. Some of the data can
//be shared. This is how.

AvProfile::ProfileData::ProfileData(AvAccessor *a, AvProfileOptions *options)
{
	vItem_ = NULL;		// Used for included profiles.
				// Explicitly set from AvImageViewPanel.
	accessor_ = a;
	accessor_->ref();
	// When this is set TRUE, the current X & Y are copied to xf & yf
	// and xf & yf are used for this profile's position rather than
	// the updated x/y.
	freezeProfile_ = FALSE;
	// NOTE: This needs to be installed before the profile callbacks
	// so the flags/data get updated before they're needed.
	accessor()->addCallback(&AvProfile::ProfileData::accessorCB_,
				 ACCMASK, this, NULL);
	x_ = y_ = 0;		// Current profile position.
	xf_ = yf_ = 0;
	// Set to 1 so copies that track won't match when initialized to 0.
	xSerial_ = ySerial_ = axisSerial_ = 1;
	xDmin_ = 0.0;		// X axis (Z axis of image) min/max.
	xDmax_ = 1.0;
	axis_ = -1;
	setProfileAxis(2);	// Default to view axis.
				// (Some routines always use Z axis).

	// File name w/o path or extension.
	{const char *fnm = accessor()->dataset()->fileName(), *p;
	 char *fname = new char[strlen(fnm)+1];

		// Remove any leading pathname.
		p = strrchr(fnm, '/');
		if(p == NULL)
			strcpy(fname, fnm);
		else
			strcpy(fname, p+1);

#if 0
		// Don't remove extension since AIPS++ uses it.
		// Remove any trailing extension.
		p = strrchr(fname, '.');
		if(p != NULL)
			*p = '\0';
#endif
		fileName_ = fname;
		delete [] fname;
	}

	setXDataScale();

	// Initialize title and axis labels.
	// This is only done if we create profile data since it is assumed
	// to have been done if it already exists.
	// Plot title
	AvString label;
	getDefaultTitle(label);

	// The default title is loaded into the options variable. It will
	// appear on the user's screen the next time 'Default' is pressed.
	options->title_.text_.initialString(label.chars(), TRUE);

	getDefaultXLabel(label);
	options->xLabel_.text_.initialString(label.chars(), TRUE);

	getDefaultYLabel(label);
	options->yLabel_.text_.initialString(label.chars(), TRUE);
	// (No other reference to options in ProfileData).
}

AvProfile::ProfileData::~ProfileData()
{
	// Disconnect from accessor
	if(accessor() != NULL)
	{	accessor()->
			removeCallback( &AvProfile::ProfileData::accessorCB_,
					ACCMASK, this, NULL);
	}
	if(accessor_ != NULL)
		accessor_->unref();
}

////////////////////////////////////////////////////////////////
///		Get profile information			///////
// Return profile information using currently set parameters.
////////////////////////////////////////////////////////////////

// Return 1 row of data from data set at position X/Y along the axis axis.
// (Axis is 0, 1 or 2). 
// Returns the number of entries, 0 for error.
int AvProfile::ProfileData::getProfile( const int x, const int y,
					const int axis,
					AvWPosition &data)const
{
int	depth = accessor()->getAxisLength(axis);

	if(depth <= 1)		// Ignore if too small.
		return 0;

//	if(data.nelements() < depth)
		data.resize(depth);

	// Profile information is retrieved for the entire axis, from
	// 0 to axis length -1. Z0 is always 0. If axis isn't 2 (Z), the
	// corresponding starting value is also set to zero.
	int x0, y0, z0=0;
	switch(axis) {
	case 0:
		x0 = 0;
		y0 = y;		
		break;
	case 1:
		x0 = x;
		y0 = 0;		
		break;
	case 2:
	default:
		x0 = x;
		y0 = y;		
		break;
	}

	// Return data.
	float *d = new float[depth];
	int num = accessor()->getRow(d, x0, y0, z0, depth, 1, axis);
	if(num != depth)
	  cerr << "AvProfile::ProfileData::getProfile: Asked for " << depth
	       << " got " << num << endl;
	for(int i=0; i<num; i++)
		data(i) = d[i];

	delete [] d;

	return num;
}

int AvProfile::ProfileData::getProfile( AvWPosition &data)const
{
	return getProfile(x_, y_, axis_, data);
}


////////////////////////////////////////////////////////////////


// Set position from which to retrieve profile.
void AvProfile::ProfileData::setXY(const int x, const int y)
{Boolean inc = FALSE;

	if((x >= 0) && (x != x_))
	{	x_ = x;
		inc = TRUE;
	}
	if((y >= 0) && (y != y_))
	{	y_ = y;
		inc = TRUE;
	}
	if(inc)
	{	ySerial_++;
		xSerial_++;
	}
}

void AvProfile::ProfileData::getXY( int &x, int &y)const
{
	x = x_;
	y = y_;
}

void AvProfile::ProfileData::getFrozenXY( int &x, int &y)const
{
	x = xf_;
	y = yf_;
}

// Change the axis used for profiles.
void AvProfile::ProfileData::setProfileAxis(const int axis)
{	if(axis != axis_)
	{	xSerial_++;	// Flag position as changed.
		ySerial_++;
		axisSerial_++;
		axis_ = axis;
	}
}

// Create a filename based on position and labels.
AvString AvProfile::ProfileData::ofileName()const
{ int x, y;
  char namebuf[1024];

	getXY(x, y);
	sprintf(namebuf, "%s.%s-%s-%03d-%03d",
		 fileName(), accessor()->axisName(0), accessor()->axisName(1),
		 x, y);
	return AvString(namebuf);
}

// Return a pointer to the 'parent' AvImageWindow. Used for 'included'
// processing.
AvImageWindow *AvProfile::ProfileData::imageWindow()const
{ return (vItem_ == NULL) ? (AvImageWindow *)NULL : vItem_->getImageWindow();
}

// Generate an X label to use in profiles.
void AvProfile::ProfileData::getDefaultXLabel( AvString &xAxisLabel )const
{ AvString xUnits;
  AvString xUnitsAbbrev;
  double xScl, repvalue;
  double xmin, xmax;

	getXDataMinMax(xmin, xmax, xScl, repvalue, xUnits, xUnitsAbbrev);
	xAxisLabel = accessor()->axisName(axis_);

	AvDataSet::AXISPRINTFORMAT apfmt = accessor()->axisPrintFormat(axis_);

	if((apfmt != AvDataSet::HMS) && (apfmt != AvDataSet::DMS))
	{	if( xUnitsAbbrev != "")
			xAxisLabel += " (" + xUnitsAbbrev + ")";
		else
			xAxisLabel += " (" + xUnits + ")";
	}
}

void AvProfile::ProfileData::getDefaultYLabel( AvString &yAxisLabel )const
{
  yAxisLabel = accessor()->dataset()->dataName();
  AvString s = accessor()->dataUnits();
  if (s != "")
    yAxisLabel +=  " (" + s + ")";
}

//// ??? Use our filename
void AvProfile::ProfileData::getDefaultTitle( AvString &plotTitle )const
{ AvString shortname = accessor()->dataset()->fileName();
  shortname.gsub(AvRegex("\\.[^\\.]*$"), "");   // strip away filename suffix

  AvWPosition world(accessor()->rank());
  int x,y;
  getXY(x, y);
  accessor()->XYZToWorld(x, y, world);

  plotTitle = "Profile of "
    + shortname + "." 
    + accessor()->axisName(0) + "-"
    + accessor()->axisName(1) + " at "
    + accessor()->axisValueUnits(0, world(0), 3) + " , "
    + accessor()->axisValueUnits(1, world(1), 3);
}

// Scale factors to convert to seconds.
static const double DEG2SEC = 240.0;
static const double HRS2SEC = 3600;

#define ABS(x) (((x)>=0)?(x):-(x))

static inline double DMAX(const double a, const double b)
{
	return (a >= b) ? a : b;
}

// Return the (scaled) data min/max for the profile's X axis.
// Also returns scale and units string.
// xScl will be 1.0 if units aren't DMS or HMS and representativeValue
// will be set to |<largestvalue>| along the z axis. This can be passed
// accessor()->valueInUnits() to help make the real scale factor.
// Otherwise, xScl will be a valid scale factor and representativeValue
// will be 0.0.
// (Needed because scaling is done differently in different places.
//  If scaling is done via axisValueInUnits(), use xScl since xUnits
//  will be empty for DMS or HMS. If using a for loop, xScl will have
//  to be generated if not DMS or HMS. (Use getXDataScale())).

// Note: 'min' means the value at index 0, 'max' at the axis' max index.
// It is very possible that dataMin > dataMax.
void AvProfile::ProfileData::getXDataMinMax(
				double &xDataMin, double &xDataMax,
				double &xScl, double &repvalue,
				AvString &xUnits,
				AvString &xUnitsAbbrev)const
{ AvDataSet::AXISPRINTFORMAT apfmt = accessor()->axisPrintFormat(zAxis());
  AvWPosition minWorld = accessor()->minWorld();
  AvWPosition maxWorld = accessor()->maxWorld();
  double minW = minWorld(zAxis());
  double maxW = maxWorld(zAxis());

	repvalue = 0.0;

	if (apfmt == AvDataSet::HMS)
	{	// PGPlot decides how to print HMS
		xUnits = "arcseconds";
		xScl = DEG2SEC;
		// (valueInUnits uses 'HRS2SEC' to scale so need to do it
		//  here).
		xDataMin = minW*xScl;
		xDataMax = maxW*xScl;
	}
	else if (apfmt == AvDataSet::DMS)
	{	// PGPlot decides how to print DMS
		xUnits = "arcseconds";
		xScl = HRS2SEC;
		xDataMin = minW*xScl;
		xDataMax = maxW*xScl;
	}
	else
	{	// Otherwise, pick representative value on axis, get the units
		// that form the best fit, and use those for tick boundaries

		double min = ABS(minW);
		double max = ABS(maxW);
		double representativeValue = DMAX(min, max);
		repvalue = representativeValue;

		xUnits = accessor()->axisUnits( zAxis(),
						representativeValue,
						xUnitsAbbrev);
		xScl = 1.0;	// valueInUnits() will scale using xUnits.
		xDataMin = accessor()->valueInUnits(zAxis(), minW, xUnits);
		xDataMax = accessor()->valueInUnits(zAxis(), maxW, xUnits);
	}
}

// Set x min, max, scale and xUnits string using current accessor settings.
void AvProfile::ProfileData::setXDataScale()
{// AvString units, abbrev;
 AvString abbrev;
  double xScl, representativeValue;

	getXDataMinMax(xDmin_, xDmax_, xScl, representativeValue,
		       xUnits_, abbrev);

	if(representativeValue != 0.0)
	{ double tmp = accessor()->valueInUnits(zAxis(),
						representativeValue,
						xUnits_.chars());
		xScl = tmp/representativeValue;
	}
	xScale_ = xScl;
}

void AvProfile::ProfileData::freeze(const Boolean freeze)
{
	if(freeze != freezeProfile_)
	{	freezeProfile_ = freeze;
		if(freeze)
		{	xf_ = x_;
			yf_ = y_;
		}
	}
}

 // Callback to receive accessor notices.
void AvProfile::ProfileData::accessorCB_(XtPointer dis, AvConductor *c,
					 XtPointer uD)
{ AvProfile::ProfileData *me = (AvProfile::ProfileData *) dis;
	me->accessorCB(c, uD);
}

void AvProfile::ProfileData::accessorCB(AvConductor *c, XtPointer /* uData */)
{ int r = c->callbackReason();

	if(r & AvAccessor::CLIP)
	{
		incXSerial();	// Flag data as old.
		incYSerial();
	}

	if(r & AvAccessor::REGION)
	{
		incXSerial();	// Flag data as old.
		incYSerial();
		setXDataScale();
	}

#if 0
	if(r & AvAccessor::PLANE)
	{
	}
#endif
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

AvProfile::AvProfile(Widget parent, AvAccessor *a, AvProfileOptions *options)
{
	AvProfile::ProfileData *data = new AvProfile::ProfileData(a, options);
	init(parent, data, options);
}

AvProfile::AvProfile(Widget parent, AvProfile::ProfileData *data,
		     AvProfileOptions *options)
{
	init(parent, data, options);
}

void AvProfile::init(Widget parent, AvProfile::ProfileData *d,
		     AvProfileOptions *options)
{
	data_ = d;
	data_->ref();
	options_ = options;
	options_->ref();
	// Connect so we know when options change.
	options->addCallback(&AvProfile::optionsInfoChangedCB,
			     AvProfileOptions::ValueChanged, this, NULL);
	int n = data_->numPoints();
	xdata_.resize(n);
	ydata_.resize(n);
	xSerial_ = ySerial_ = 0;
	axisSerial_ = 0;
	haveOptions_ = FALSE;
	showIncluded_ = FALSE;
	parentW_ = parent;
	winWidth_ = winHeight_ = 0;
	accessor()->addCallback(&AvProfile::accessorCB_,
				 ACCMASK, this, NULL);
}

AvProfile::~AvProfile()
{
	if(options_ != NULL)
	{	options_->removeCallback(
				&AvProfile::optionsInfoChangedCB,
				AvProfileOptions::ValueChanged, this, NULL);
		options_->unref();
	}
	// Disconnect from accessor.
	if(accessor() != NULL)
	{	accessor()->removeCallback(&AvProfile::accessorCB_,
					   ACCMASK, this, NULL);
	}
	data_->unref();
}

void AvProfile::drawProfileAt(const int x, const int y)
{
	data_->setXY(x, y);
	drawProfile();
}

// Get profile using current position.
void AvProfile::getYData(AvWPosition &ydata)const
{
	data_->getProfile(ydata);
}

void AvProfile::getYData()
{
	getYData(ydata_);
	ySerial_ = data_->ySerial();
}

// Generate X data (compute world coordinates along axis).
// xdata is resized to the number of points in the profile.
void AvProfile::getXData(AvWPosition &xdata)const
{int l = profileData()->numPoints();

	xdata.resize(l);
	int x0, y0, z0;
	getXY(x0, y0);

	double xScl = data_->getXDataScale();

	for(z0=0; z0 < l; z0++)
	{double z;

		accessor()->XYZToWorld( x0, y0, z0, 2, z);
		xdata(z0) = z*xScl;
	}
}

void AvProfile::getXData()	// Profile info
{
	getXData(xdata_);
	xSerial_ = data_->xSerial();
}

// Set position from which to retrieve profile.
void AvProfile::setXY(const int x, const int y)
{
	data_->setXY(x, y);
}

void AvProfile::getXY( int &x, int &y)const
{
	data_->getXY(x, y);
}

// Update internal options if info struct has changed, get data if needed
// then draw complete profile.
void AvProfile::drawProfile()
{
	// Update options (line color, etc. it necessary).
	if(!haveOptions())
	{	updateOptions();
		haveOptions_ = TRUE;
	}
	// Grab data if it has changed.
	if(!haveXData())
		getXData();
	if(!haveYData())
		getYData();
	open();
	 if(freezeProfile())
		drawOnlyProfileUsing(this);
	 else
		drawProfileOnly(xdata_, ydata_);
	 if(showIncluded())
		drawIncludedProfiles();
	 drawBoxAndLabels();
	close();
}

void AvProfile::open(){}
void AvProfile::close(){}

void AvProfile::drawProfileOnly(const AvWPosition &x, const AvWPosition &y)
{
	drawProfileOnlyUsing(x, y, this);
}

void AvProfile::drawProfileOnlyUsing(const AvWPosition &,
				     const AvWPosition &,
				     AvProfile *) {}

void AvProfile::redrawProfile()
{
	drawProfile();
}

void AvProfile::showIncluded(const Boolean show)
{	if(show != showIncluded_)
	{	showIncluded_ = show;
		redrawProfile();
	}
}

void AvProfile::freezeProfile(const Boolean freeze)
{
	if(freeze != data_->frozen())
	{	data_->freeze(freeze);
		redrawProfile();
	}
}


// Draw profiles from all windows that have their 'include' flag set.
void AvProfile::drawIncludedProfiles()
{
AvImageViewItem **list=NULL;
static int listlen = 0;

	list = AvImageView::instance()->getImageViewList(listlen,
							AvImageView::INCLUDED);
	if(listlen <= 0)
		return;

	// Create Position args for profile start and stop.
	int x, y, z=0;
	getXY(x, y);
	AvPosition from(accessor());	// Current position.
	from.XYZToWorld(x, y, z);
	AvPosition to(from);
	z = accessor()->getAxisLength(profileData()->getProfileAxis())-1;
	to.XYZToWorld(x, y, z);

	// Tell each included window that isn't this one to draw a profile
	// using our clip min/max and output window, but its color, etc.
	// parameters.
	for(int i=0; i< listlen; i++)
	{AvImageWindow *win = list[i]->getImageWindow();
	 AvImageViewPanel *ivp = win->viewPanel();
	 AvProfile *profile = getIVPProfile(ivp);

		if((profile == this) || (profile == NULL))
			continue;
		if(profile->freezeProfile())
			profile->drawOnlyProfileUsing(this);
		else
			profile->drawProfileAtUsing(from, to, this);
	}

	delete [] list;
}

AvProfile *AvProfile::getIVPProfile(AvImageViewPanel *)const
{
	return NULL;
}

void AvProfile::drawBoxAndLabels(){}
void AvProfile::updateOptions() {}

void AvProfile::printProfile(const char *)
{
}

static const char *CMT = "# ";

// Dump the profile to an ASCII file.
// Includes a few lines of header info
// and two columns: X Y
void AvProfile::dumpProfile()
{
  float nslices;

	// Open file.
	AvString oname = data_->ofileName();
	oname += ".profile.dump";

	fstream out;
	out.open(oname.chars(), ios::out);
	if(out.rdstate() != ios::goodbit)
		return;
	printf("Dumping to %s\n", oname.chars());

//	AvString xUnits;
//	AvString xUnitsAbbrev;
	AvString xAxisLabel;

//	AvWPosition minW = accessor()->minWorld();
//	AvWPosition maxW = accessor()->maxWorld();

	AvWPosition world(accessor()->rank());
	int x, y;
	getXY(x, y);
	accessor()->XYZToWorld(x, y, world);

	data_->getDefaultXLabel(xAxisLabel);

	AvString yAxisLabel = accessor()->dataset()->dataName();
	AvString s = accessor()->dataUnits();
	if (s != "")
		yAxisLabel +=  " (" + s + ")";

	// Plot title

	AvString shortname = accessor()->dataset()->fileName();
	// strip away filename suffix
	shortname.gsub(AvRegex("\\.[^\\.]*$"), "");

	AvString plotTitle = "Profile of "
		+ shortname + "." 
		+ accessor()->axisName(xAxis()) + "-"
		+ accessor()->axisName(yAxis()) + " at "
		+ accessor()->axisValueUnits(xAxis(), world(xAxis()), 3)
		+ " , "
		+ accessor()->axisValueUnits(yAxis(), world(yAxis()), 3);


	// Dump a header.
	out << CMT << plotTitle << endl;
	out << CMT << "X axis: " << xAxisLabel << endl;
	out << CMT << "Y axis: " << yAxisLabel << endl;
	nslices = accessor()->getAxisLength(zAxis());
	out << CMT << nslices << " slices\n";

	// Dump the profile.
	for(int ijk=0; ijk< nslices; ijk++)
		out << (float) xdata_(ijk) << "\t" << ydata_(ijk) << endl;

	out.close();
}

// Widget to be used for drawing.
void AvProfile::setProfileWidget(Widget w)
{
	profileW_ = w;
	XtAddCallback(profileW_, XmNexposeCallback,
		      &AvProfile::widgetChangedCB, this);

	XtAddCallback(profileW_, XmNresizeCallback,
		      &AvProfile::widgetChangedCB, this);

	XtAddCallback(profileW_, XmNinputCallback,
		      &AvProfile::widgetChangedCB, this);
}

Widget AvProfile::mainWidget()const {return NULL;}
void AvProfile::widgetChanged(Widget , XmDrawingAreaCallbackStruct *) {}
void AvProfile::widgetInput(Widget , XmDrawingAreaCallbackStruct *) {}

// Called when window is exposed, changes size or there is input.
void AvProfile::widgetChangedCB(Widget w, XtPointer cbdata, XtPointer data)
{ AvProfile *p = (AvProfile *) cbdata;
  XmDrawingAreaCallbackStruct *cbs = (XmDrawingAreaCallbackStruct *) data;

	if((data == NULL) || (p == NULL))
		return;
	Dimension width, height;
	XtVaGetValues(w, XmNheight, &height, XmNwidth, &width, NULL);
	p->winWidth_ = width;
	p->winHeight_ = height;


	int reason = cbs->reason;
	switch (reason) {
	case XmCR_RESIZE:
		p->widgetChanged(w, cbs);// Do any updates depending on size.
					// fall through.
	case XmCR_EXPOSE:
		p->redrawProfile();
		break;
	case XmCR_INPUT:
		p->widgetInput(w, cbs);
		break;
	default:;
//		cout << "AvProfile::widgetChangedCB: Unhandled reason = " << reason << endl;
	}
}

// Called when profile options change.
// Flags options as having changed then calls drawProfile().
void AvProfile::optionsInfoChangedCB(XtPointer o, AvConductor *,
				     XtPointer )
{ AvProfile *me = (AvProfile *) o;

	me->haveOptions_ = FALSE;	// Mark options as changed.
	if(me->isUp())			// Only call redraw if active.
		me->drawProfile();

	// If this window has its 'Include' flag set, tell any other
	// windows with include set to redraw their profiles.
	AvImageWindow *mywin = me->profileData()->imageWindow();
	if((mywin != NULL) && (mywin->include()))
	{AvImageViewItem **list;
	 int listlen;

		list = AvImageView::instance()->getImageViewList(listlen,
				AvImageView::INCLUDED);
		for(int i=0; i< listlen; i++)
		{AvImageWindow *win = list[i]->getImageWindow();

			if(win == mywin) continue;
			AvImageViewPanel *ivp = win->viewPanel();
			ivp->redrawProfile();
		}
		delete [] list;
	}
}

// Manage/Unmanage the widget.
void AvProfile::show(const Boolean shw)
{ Widget w = mainWidget();

	if(w == NULL)
		w = profileWidget();

	if(shw)
	{	XtManageChild(w);
		if(dataChanged())
			drawProfile();
	}
	else
		XtUnmanageChild(w);
}

// Is main widget managed?
Boolean AvProfile::isUp()const
{ Widget w = mainWidget();

	if(w == NULL)
		w = profileWidget();

	return XtIsManaged(w);
}

 // Callback to receive accessor notices.
void AvProfile::accessorCB_(XtPointer dis, AvConductor *c, XtPointer uD)
{ AvProfile *me = (AvProfile *) dis;
	me->accessorCB(c, uD);
}

void AvProfile::accessorCB(AvConductor *c, XtPointer uData)
{ int r = c->callbackReason();

	accessorCB2(r, uData);		// In case something special is needed.

	if(r & AvAccessor::CLIP)
	{
		drawProfile();
	}

	if(r & AvAccessor::REGION)
	{
		drawProfile();
	}

	if(r & AvAccessor::PLANE)
	{
		redrawProfile();
	}
}

void AvProfile::accessorCB2(const int /* reason*/, XtPointer /*uData */)
{
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

AvXProfile::AvXProfile(const char * name, Widget parent,
		       AvAccessor *a, AvProfileOptions *o, AvXPalette *pal) :
	    AvProfile(parent, a, o)
{
	xinit(name, parent, pal);
}

AvXProfile::AvXProfile( const char * name, Widget parent,
			AvProfile::ProfileData *d,
		        AvProfileOptions *o, AvXPalette *pal) :
			AvProfile(parent, d, o)
{
	xinit(name, parent, pal);
}

void AvXProfile::xinit(const char * /*name*/, Widget parent, AvXPalette *pal)
{
	palette_ = pal;
	palette_->ref();

	showZeroLine_ = TRUE;
	plotType_ = LINES;

	haveXLabels_ = haveYLabels_ = FALSE;	// Need to draw X/Y labels.
	AvProfileOptions *optns = options();
	// Whether to flip axes.
	xMinMax_ = optns->xMinMax_.getValue();
	yMinMax_ = optns->yMinMax_.getValue();

	mainWidget_ = buildProfile(parent);

	// Create GCs used for lines and other things.
	XGCValues values;

	// Set Foreground and background.
	XtVaGetValues(profileWidget(),
		      XmNforeground, &values.foreground,
		      XmNbackground, &values.background,
	 	      NULL);

	// fill in other parts of graphics context
	values.line_style = LineSolid;
	values.function = GXcopy;

	Display *dpy = XtDisplay(parent);
	Window win = XtWindow(parent);
	if(win == 0)
		win = AvApp::referenceWindow();

	// GC used for drawing the zero and slice lines.
	boxGC_ = XCreateGC(dpy, win,
		      GCForeground | GCBackground | GCFunction | GCLineStyle,
		      &values);

	values.foreground = colorIndex();
	linesGC_ = XCreateGC(dpy, win,
//		      GCForeground | GCBackground | GCFunction | GCLineStyle,
		      GCForeground | GCFunction | GCLineStyle,
		      &values);
}

AvXProfile::~AvXProfile()
{
	XFreeGC(XtDisplay(mainWidget_), linesGC_);
	XFreeGC(XtDisplay(mainWidget_), boxGC_);
	palette_->unref();
}

// Builds display for X. Four label widgets and on drawing area.
Widget AvXProfile::buildProfile(Widget parent)
{
#if 0
	Widget frame = XtVaCreateManagedWidget("xprofile",
			xmFrameWidgetClass,
			parent,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
			NULL);
#endif
	Widget formW = XtVaCreateManagedWidget("", xmFormWidgetClass,
//		frame,
		parent,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		NULL);

	// Y axis labels.
	profileYMaxLabel_ = XtVaCreateManagedWidget("FontPlain",
				xmLabelGadgetClass, formW,
				XmNtopAttachment, XmATTACH_FORM,
				XmNleftAttachment, XmATTACH_FORM,
				NULL);

	Widget da = XtVaCreateManagedWidget("xProfile",
		xmDrawingAreaWidgetClass, formW,
		XmNtopAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_WIDGET,
		XmNleftWidget, profileYMaxLabel_,
		XmNrightAttachment, XmATTACH_FORM,
//		XmNbottomAttachment, XmATTACH_FORM,
		// height is set in fallback resources so users can override.
//		XmNheight, 128,
		XmNborderWidth, 1,
		NULL);
	setProfileWidget(da);

	profileXMaxLabel_ = XtVaCreateManagedWidget("FontPlain",
		xmLabelGadgetClass, formW,
		XmNtopAttachment, XmATTACH_WIDGET,
		XmNtopWidget, da,
		XmNrightAttachment, XmATTACH_FORM,
		NULL);

	profileXMinLabel_ = XtVaCreateManagedWidget("FontPlain",
		xmLabelGadgetClass, formW,
		XmNtopAttachment, XmATTACH_WIDGET,
		XmNtopWidget, da,
		XmNleftAttachment, XmATTACH_WIDGET,
		XmNleftWidget, profileYMaxLabel_,
		NULL);

	profileYMinLabel_ = XtVaCreateManagedWidget("FontPlain",
		xmLabelGadgetClass, formW,
		XmNbottomAttachment, XmATTACH_WIDGET,
		XmNbottomWidget, profileXMinLabel_,
		NULL);

	updateProfileXLabels();

	// Since we're using variable width fonts, the size of the label
	// can change when its contents change. This can result in the
	// window changing sizes. We choose the largest likely width and
	// lock it down.

	setLabelString(profileYMinLabel_, "99999999");
	XtVaSetValues(  profileYMinLabel_,
			XmNrecomputeSize, FALSE,
			NULL);
	setLabelString(profileYMinLabel_, "");

	setLabelString(profileYMaxLabel_, "99999999");
	XtVaSetValues(  profileYMaxLabel_,
			XmNrecomputeSize, FALSE,
			NULL);
	setLabelString(profileYMaxLabel_, "");
	updateProfileYLabels();
	return formW;
//	return frame;
}

// Used by routines that need to flip axis components.
static inline void flip(double &a, double &b)
{ double t = a;
	a = b;
	b = t;
}

// Redisplays the Y axis labels for the X11 profile.
void AvXProfile::updateProfileYLabels()
{
double min, max;
char str[24];

	min = accessor()->clipMin();
	max = accessor()->clipMax();

	switch( yMinMax_ ) {
	case AvProfileOptions::DEFAULTMINMAX:	// Right out of data set.
		break;
	case AvProfileOptions::FLIPPED:		// flip it.
		flip(min, max);
		break;
	case AvProfileOptions::MINFIRST:
		if(max < min)
			flip(min, max);
		break;
	case AvProfileOptions::MAXFIRST:
		if(max > min)
			flip(min, max);
		break;
	}

	sprintf(str, "%8.5g", min);
	setLabelString(profileYMinLabel_, str);

	sprintf (str, "%8.5g", max);
	setLabelString (profileYMaxLabel_, str);

	haveYLabels_ = TRUE;
}

// Draw X axis labels.
void AvXProfile::updateProfileXLabels()
{
  int rank = accessor()->rank();
  AvWPosition world(rank);
//  AvIPosition ip(3,0,0,0);
  AvIPosition ip(rank);

	ip = 0;
	accessor()->XYZToWorld(ip, world);
	double min = world(zAxis());

	int endSlice = accessor()->getAxisLength(zAxis()) -1;
	ip(2) = endSlice;
	accessor()->XYZToWorld(ip, world);
	double max = world(zAxis());

	switch( xMinMax_ ) {
	case AvProfileOptions::DEFAULTMINMAX:	// Right out of data set.
		break;
	case AvProfileOptions::FLIPPED:		// flip it.
		flip(min, max);
		break;
	case AvProfileOptions::MINFIRST:
		if(max < min)
			flip(min, max);
		break;
	case AvProfileOptions::MAXFIRST:
		if(min < max)
			flip(min, max);
		break;
	}

	setLabelString( profileXMinLabel_,
			accessor()->axisValueUnits(zAxis(), min).chars());

	setLabelString( profileXMaxLabel_,
			accessor()->axisValueUnits(zAxis(), max).chars());
	haveXLabels_ = TRUE;
}

// Change the label's string.
void AvXProfile::setLabelString(Widget w, const char * strng)
{ char *str;

	if(strng == NULL)
		return;
	str = new char[strlen(strng)+1];
	strcpy(str, strng);
	XmString xStr = XmStringCreateLocalized(str);
	XtVaSetValues(w, XmNlabelString, xStr, NULL);
	XmStringFree(xStr);
	delete [] str;
}

// Called by AvProfile::accessorCB to do processing needed when accessor
// parameters change.
void AvXProfile::accessorCB2(const int reason, XtPointer /*uData*/)
{
	if(reason & AvAccessor::CLIP)
	{
		haveYLabels_ = FALSE;
	}

	if(reason & AvAccessor::REGION)
	{
		haveXLabels_ = FALSE;
	}

#if 0
	// Nothing special for X.
	if(reason & AvAccessor::PLANE)
	{
	}
#endif
}

// Draw an array of XPoints as either lines or dots.
// fmt==0, draw as points, else lines.
// For lines, draws len-1 line segments.
void AvXProfile::drawXPoints(Widget w,
			    XPoint *points, const int len,
			    const Boolean drawPoints)
{
	if(len <= 0)
		return;
	if((len == 1) || drawPoints)
		XDrawPoints(XtDisplay(w), XtWindow(w), linesGC_,
			    points, len, CoordModeOrigin);
	else
		XDrawLines(XtDisplay(w), XtWindow(w), linesGC_,
			    points, len, CoordModeOrigin);
}

// Draw profile (only) using options from this, but using output and scales
// from profile. (Useful for overlays).
// Does not clear window.
void AvXProfile::drawProfileOnlyUsing(const AvWPosition &xdata,
				      const AvWPosition &ydata,
				      AvProfile *profile)
{ int i;
  double xmin, xmax, ymin, xscl, yscl, ymax;
  Dimension maxH;

	if(profile == NULL)
		profile = this;

	Widget w = profile->profileWidget();

	if((w == NULL) || !XtIsRealized(w))
		return;

	maxH = profile->winHeight() -1;		// Max Y value.
	AvProfile::ProfileData *data = profile->profileData();
	xmin = data->xDmin();
	xmax = data->xDmax();

	ymin = profile->accessor()->clipMin();
	ymax = profile->accessor()->clipMax();

	AvProfileOptions *optns = profile->options();

	// Whether to flip the axis.
	switch(optns->xMinMax_.getValue()) {
	case AvProfileOptions::DEFAULTMINMAX:	// Right out of data set.
		break;
	case AvProfileOptions::FLIPPED:		// flip it.
		flip(xmin, xmax);
		break;
	case AvProfileOptions::MINFIRST:
		if(xmax < xmin)
			flip(xmin, xmax);
		break;
	case AvProfileOptions::MAXFIRST:
		if(xmin < xmax)
			flip(xmin, xmax);
		break;
	}

	switch(optns->yMinMax_.getValue()) {
	case AvProfileOptions::DEFAULTMINMAX:	// Right out of data set.
		break;
	case AvProfileOptions::FLIPPED:		// flip it.
		flip(ymin, ymax);
		break;
	case AvProfileOptions::MINFIRST:
		if(ymax < ymin)
			flip(ymin, ymax);
		break;
	case AvProfileOptions::MAXFIRST:
		if(ymin < ymax)
			flip(ymin, ymax);
		break;
	}

	xscl = (profile->winWidth()-1)/(xmax-xmin);
	yscl = (maxH)/(ymax-ymin);

	int xlen=xdata.nelements(), ylen = ydata.nelements();

	int len = xlen;
	if(len > ylen)
		len = ylen;	// Use shorter.
	if(len <= 1)
		return;		// Too short;

	const double BlankedValue = (double)accessor()->blankingValue();

	XPoint *points = new XPoint[len];
	int npoints = 0;
	// If plot type is markersonly, draw dots.
	Boolean drawDots = (plotType_ == DOTS);

	// Generate an array of xpoints to draw the line. If a datum is flagged
	// as being blanked, dump any current array and proceed with the next.
	for(i=0; i<len; i++)
	{ double x, y;
	  short ix, iy;

		x = xdata(i);
		y = ydata(i);
		if(((x == BlankedValue) || (y == BlankedValue)) && (npoints>0))
		{	drawXPoints(w, points, npoints, drawDots);
			npoints = 0;
			continue;
		}

		ix = (int)((x-xmin)*xscl + 0.5);
		// Remember to flip the Y value.
		iy = maxH - (int)((y-ymin)*yscl + 0.5);
		points[npoints].x = ix;
		points[npoints].y = iy;
		npoints++;
	}

	if(npoints > 0)		// Flush extras.
	{	drawXPoints(w, points, npoints, drawDots);
#if 0
		for(int k=0; k< 16; k++)
		{	cout << points[k].y << " ";
		}
		cout << endl;
#endif
	}
	delete [] points;
}

//Draw the vertical line indicating the current slice.
// The vertical bar doesn't go all the way from top to bottom.
void AvXProfile::drawSliceLine()
{ Dimension lpw, lph;
  int npoints = profileData()->numPoints();
  Widget w = profileWidget();

	if(! validWidget(w))
		return;
	XtVaGetValues(w, XmNwidth, &lpw, XmNheight, &lph, NULL);
	if((lpw == 0) || (lph == 0))
		return;
	float slicePosition =
	 (float)(accessor()->getPlane())*(lpw-1)/(float)(npoints-1);
	int xslice = (int)(slicePosition + 0.5);
	if(xslice >= 0)
		XDrawLine(XtDisplay(w), XtWindow(w),
			  boxGC_, xslice, 1, xslice, lph-1-1);
}

// erase the current profile.
void AvXProfile::clearWindow()
{
 Widget w = profileWidget();

#if 0
 XGCValues values;
	XtVaGetValues(w,
		      XmNforeground, &values.foreground,
		      XmNbackground, &values.background,
	 	      NULL);
#endif
	if(validWidget(w))
		XClearWindow(XtDisplay(w), XtWindow(w));
}

// Draw the Zero line.
void AvXProfile::drawZeroLine()
{ int npoints = profileData()->numPoints();
  Dimension lpw, lph;
  double min, max;
  Widget w = profileWidget();

	if(! showZeroLine_)
		return;
	if(! validWidget(w))
		return;
	if(npoints <= 0)
		return;

	min = accessor()->clipMin();
	max = accessor()->clipMax();
	XtVaGetValues(w, XmNwidth, &lpw, XmNheight, &lph, NULL);
	int y, x0, x1;
	x0 = 0;
	x1 = lpw-1;
	y = int(lph + min*lph/(max-min));
	XDrawLine(XtDisplay(w), XtWindow(w), boxGC_,
		  x0, y, x1, y);
}

void AvXProfile::open()
{
	clearWindow();
	// These are done here rather than in drawBoxAndLabels so the
	// profile will be on top of the lines.
	drawZeroLine();
	drawSliceLine();
}

void AvXProfile::drawBoxAndLabels()
{
//	drawZeroLine();
//	drawSliceLine();
	// These could also be in open().
	if(!haveXLabels_)
		updateProfileXLabels();
	if(!haveYLabels_)
		updateProfileYLabels();
}

// Convert pos into X/Y, checking for z range. If legal, draw the
// profile (only) using the supplied profile arg.
// Pos gives the starting position, zend the end position along the z axis.
void AvProfile::drawProfileAtUsing(const AvPosition &from,
				   const AvPosition &to,
				   AvProfile *profile)
{	AvAccessor *ac = accessor();

	int rank = ac->rank();
	// Need 3 dimensions.
	if(rank <= 2)
		return;
	if((from.length() <= 2) || (to.length() <= 2))
		return;

	// Create new positions using our ordering, but values from
	// the supplied arguments. The supplied values must have our
	// first three axis units (X, Y, Z) or there is no point in
	// continuing. (To & from may be longer or shorter than rank,
	// since only X, Y and Z are used).
	AvPosition *begin = AvPosition::newPosition(ac, from);
	if(begin == NULL)
		return;		// No units match X, Y.

	const char *zName = ac->axisName(2);
	if(strcmp(begin->units(2), zName) != 0)
	{	delete begin;
		return;		// Z didn't match.
	}

	AvPosition *stop = AvPosition::newPosition(ac, to);
	if(stop == NULL)
	{	delete begin;
		return;
	}
	if(strcmp(stop->units(2), zName) != 0)
	{	delete begin;
		delete stop;
		return;		// Z didn't match.
	}

	// The 1st three axis units of the supplied arguments match ours.
	// Now make sure the values are within the data set.
	AvIPosition ijk0(rank), ijk1(rank);
	begin->WorldToXYZ(ijk0);
	stop->WorldToXYZ(ijk1);

	delete begin;		// Done with Positions.
	delete stop;

	int xLen = ac->getAxisLength(0);
	int yLen = ac->getAxisLength(1);
	int zLen = ac->getAxisLength(2);
	int x0 = ijk0(0), y0 = ijk0(1), z0 = ijk0(2);
	int x1 = ijk1(0), y1 = ijk1(1), z1 = ijk1(2);
	if((x0 < 0) || (x0 >= xLen) || (y0 < 0) || (y0 >= yLen)
				 ||
	   (x1 < 0) || (x1 >= xLen) || (y1 < 0) || (y1 >= yLen)
	  )
		return;

	// Z values are a different. If possible, they will be clipped
	// to fit.
	if(z0 > z1)
	{ int t = z0;
		z0 = z1;
		z1 = t;
	}

	// If the range z0..z1 is entirely on one side of the valid
	// range, there is no overlap so we lose.
	if((z1 < 0) || (z0 >= zLen))
		return;

	// Clip, if necessary.
	if(z0 < 0)
		z0 = 0;
	if(z1 >= zLen)
		z1 = zLen-1;

	// We also punt if there is no effective range after clipping.
	if(z0 >= z1)
		return;

	// Get profile data.
	double xScl = data_->getXDataScale();
	int depth = z1-z0+1;
	float *d = new float[depth];
	int num = accessor()->getRow(d, x0, y0, z0, depth, 1, 2);
	if(num != depth)
	  cerr << "AvXProfile::drawProfileAtUsing: Asked for " << depth
	       << " got " << num << endl;
	AvWPosition x(depth), y(depth);

	for(int i=0; i<num; i++)
	{double z;
		y(i) = d[i];
		accessor()->XYZToWorld( x0, y0, z0+i, 2, z);
		x(i) = (double) z*xScl;
	}

	delete [] d;

	drawProfileOnlyUsing(x, y, profile);
}

// Draw profile using current X/Y.
void AvProfile::drawOnlyProfileUsing( AvProfile *profile)
{	AvAccessor *ac = accessor();

	int rank = ac->rank();
	// Need 3 dimensions.
	if(rank <= 2)
		return;

	int zLen = ac->getAxisLength(2);
	int x0, y0, z0=0;
	if(freezeProfile())
		data_->getFrozenXY(x0, y0);	// Use frozen position.
	else	// Currently, this routine is only called if frozen.
		getXY(x0, y0);	// Use current position.

	// Get profile data.
	double xScl = data_->getXDataScale();
	int depth = zLen;
	float *d = new float[depth];
	int num = accessor()->getRow(d, x0, y0, z0, depth, 1, 2);
	if(num != depth)
	  cerr << "AvXProfile::drawOnlyProfileUsing: Asked for " << depth
	       << " got " << num << endl;
	AvWPosition x(depth), y(depth);

	for(int i=0; i<num; i++)
	{double z;
		y(i) = d[i];
		accessor()->XYZToWorld( x0, y0, z0+i, 2, z);
		x(i) = (double) z*xScl;
	}

	delete [] d;

	drawProfileOnlyUsing(x, y, profile);
}

// There was input on the widget. (Mouse button press).
void AvXProfile::widgetInput(Widget , XmDrawingAreaCallbackStruct *info)
{ int xw;

	int reason = info->reason;
	if(reason != XmCR_INPUT)
		return;
	XEvent *event = info->event;
	if( event == NULL)
		return;

	if(event->type == ButtonPress)
	{XButtonPressedEvent *e = (XButtonPressedEvent *)event;
		if(e->state != 0)	// Ignore Button Release
			return;
		xw = e->x;
	}
	else	// Motif doesn't seem to pass these.
	if(event->type == MotionNotify)
	{XMotionEvent *e = (XMotionEvent *)event;
		xw = e->x;
	}
	else
		return;
	// Convert xw into a slice index.
	Dimension width;
	XtVaGetValues(profileWidget(), XmNwidth, &width, NULL);
	float xScale = (accessor()->depth() - 1) /(float) (width-1);
	int sliceIndex = (int)(xw *xScale + 0.5);
	// Set this slice
	accessor()->setPlane(sliceIndex);
	redrawProfile();

	// Update included windows.
	AvImageWindow *win = profileData()->imageWindow();
	if((win != NULL) && (win->include()))
	{	int x,y, z;
		getXY(x, y);
		z = accessor()->getPlane();
		win->setPositions(x, y, z, FALSE);
	}
}

// Patterns to use for non-solid lines.
static const char Dash[] = {4, 2};
static const int DashL = (int)(sizeof(Dash)/sizeof(*Dash));
static const char DotDash[] = {2, 2, 4, 2};
static const int DotDashL = (int)(sizeof(DotDash)/sizeof(*DotDash));
static const char Dot[] = {2, 2};
static const int DotL = (int)(sizeof(Dot)/sizeof(*Dot));
static const char DashDotDot[] = {4, 2, 2, 2, 2, 2};
static const int DashDotDotL = (int)(sizeof(DashDotDot)/sizeof(*DashDotDot));

// Update color, etc. Changes are made to the linesGC_ here so redraws,
// etc. don't have to access info or change gc.
void AvXProfile::updateOptions()
{ XGCValues vals;
  Widget w = profileWidget();
  static unsigned long mask = GCForeground | GCLineWidth | GCLineStyle;
  AvProfileOptions *optns = options();
  Display *dpy = XtDisplay(w);

	// Line style: solid, dotted, etc.
	int lineStyle = optns->gLineStyle_.getValue();
	{	int xLineStyle;
		switch (lineStyle) {
		 case AvProfileOptions::DASHED:
				xLineStyle = LineOnOffDash;
				XSetDashes(dpy, linesGC_, 0, Dash, DashL);
				break;
		 case AvProfileOptions::DOTDASH:
				xLineStyle = LineOnOffDash;
				XSetDashes(dpy, linesGC_, 0,
					   DotDash, DotDashL);
				break;
		 case AvProfileOptions::DOTTED:
				xLineStyle = LineOnOffDash;
				XSetDashes(dpy, linesGC_, 0, Dot, DotL);
				break;
		 case AvProfileOptions::DASHDOTDOT:
				xLineStyle = LineOnOffDash;
//				xLineStyle = LineDoubleDash;
				XSetDashes(dpy, linesGC_, 0,
					   DashDotDot, DashDotDotL);
				break;
		 default:
		 case AvProfileOptions::SOLID:
				xLineStyle = LineSolid;
				break;
		}
		vals.line_style = xLineStyle;
	}
	// Line color.
	vals.foreground = colorIndex();
	// Line width;
	int lineWidth = (int) optns->gLineWidth_.getValue();
	{ Screen *s = XtScreen(w);	// To be compatible with PGPLOT.
	  int wp = WidthOfScreen(s);
	  int wmm = WidthMMOfScreen(s);
	  float xdpi = (25.4*wp)/(float)wmm;
	  lineWidth = (int)(lineWidth*xdpi*.005);
	}
	if(lineWidth < 0)
		lineWidth = 0;
	vals.line_width = lineWidth;
	XChangeGC(XtDisplay(w), linesGC_, mask, &vals);

	// Non-GC options. These are stored internally so we don't have to
	// access the options object on each redraw.
	// Zero line can be on or off.
	if(options()->gDrawZeroLine_.getValue() == AvProfileOptions::OFF)
		showZeroLine_ = FALSE;
	else
		showZeroLine_ = TRUE;

	// For now, X profiles are either lines or dots.
	int plotType = options()->gPlotType_.getValue();
	if(plotType == AvProfileOptions::MARKERSONLY)
		plotType_ = DOTS;
	else
		plotType_ = LINES;

	int minmax = options()->xMinMax_.getValue();
	if( minmax != xMinMax_)
	{	xMinMax_ = minmax;
		haveXLabels_ = FALSE;
	}

	minmax = options()->yMinMax_.getValue();
	if( minmax != yMinMax_)
	{	yMinMax_ = minmax;
		haveYLabels_ = FALSE;
	}
}

// Convert current color selection from options into an X color value.
unsigned long AvXProfile::colorIndex()const
{ int profileColor= options()->gProfileColor_.getValue();
  const char *color;

	switch(profileColor) {
	 case BLACK:
		color = "BLACK";
		break;
	 case WHITE:
		color = "WHITE";
		break;
	 case RED:
		color = "RED";
		break;
	 case GREEN:
		color = "GREEN";
		break;
	 case BLUE:
		color = "BLUE";
		break;
	 case CYAN:
		color = "CYAN";
		break;
	 case MAGENTA:
		color = "MAGENTA";
		break;
	 case YELLOW:
		color = "YELLOW";
		break;
	default:
		color = "black";
	}
	unsigned long vindex, pindex;
	palette_->allocNamedColor(color, vindex, pindex);
	return pindex;
}

AvProfile *AvXProfile::getIVPProfile(AvImageViewPanel *ivp)const
{
	return ivp->xProfile();
}


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// All functions are wrapped with #ifdef PGPLOT since if it isn't defined,
// AvPGProfile doesn't exist.

#ifdef PGPLOT
AvPGProfile::AvPGProfile(Widget parent, AvAccessor *a, AvProfileOptions *o,
			 AvXPalette *pal): AvProfile(parent, a, o)
{
	pginit(parent, pal);
}

AvPGProfile::AvPGProfile(Widget parent, AvProfile::ProfileData *d,
			 AvProfileOptions *o,
			 AvXPalette *pal): AvProfile(parent, d, o)
{
	pginit(parent, pal);
}

void AvPGProfile::pginit(Widget parent, AvXPalette *pal)
{
	hardCopy_ = FALSE;
	haveProfile_ = FALSE;
	xCanvas_ = new AvXWindow("pgProfile", parent, pal, NULL, 0);
	xCanvas_->ref();
	pgCanvas_ = NULL;
	driver_ = NULL;
	vps_ = NULL;

	Widget da = xCanvas_->widget();
	XtVaSetValues(da,
	 XmNtopAttachment, XmATTACH_FORM,
	 XmNleftAttachment, XmATTACH_FORM,
	 XmNrightAttachment, XmATTACH_FORM,
	 XmNbottomAttachment, XmATTACH_FORM,
	// Retrieved via fallback resource.
//	 XmNheight, 384,
	 NULL);
	XtUnmanageChild(da);

	setProfileWidget(da);
}
#endif

#ifdef PGPLOT
AvPGProfile::~AvPGProfile()
{
	xCanvas_->unref();
	if(vps_ != NULL)
		vps_->unref();
}
#endif

#ifdef PGPLOT
void AvPGProfile::open()
{
	openProfileDevice(hardCopy_);
}
#endif

#ifdef PGPLOT
void AvPGProfile::close()
{
	closeProfileDevice();
}
#endif

#ifdef PGPLOT
// Draw profile using given data. 'profile' arg isn't used since PGPLOT
// device is already global.
void AvPGProfile::drawProfileOnlyUsing(  const AvWPosition &X,
					 const AvWPosition &Y,
					 AvProfile * /*profile*/)
{int npts = X.nelements();
 float *x = new float[npts];
 float *y = new float[npts];
 int i;
 float BlankedValue = accessor()->blankingValue();

	for(i=0; i< npts; i++)
	{	x[i] = (float) X(i);
		y[i] = (float) Y(i);
	}

	AvProfileOptions *Options = options();
	// Plot type: NORMAL (continous line, histogram, only markers).
	int plotType = Options->gPlotType_.getValue();

	// Line style: solid, dotted, etc.
	int lineStyle = Options->gLineStyle_.getValue() + 1;
	int lw = (int) Options->gLineWidth_.getValue();

	////////////////////////////////////////////////////////////////
	// Draw profile ignoring any blanked data points.
	////////////////////////////////////////////////////////////////
	if(plotType != AvProfileOptions::MARKERSONLY)
	{int start = 0;
	 int profileColor= Options->gProfileColor_.getValue();
	  cpgsave();
	  cpgsci(profileColor);
	  cpgsls(lineStyle);
	  cpgslw(lw);

	  for(i=0; i<npts; i++)
		if(y[i] == BlankedValue)
		{	if(start < i)
				if(plotType == AvProfileOptions::HISTOGRAM)
				       cpgbin(i-start, x+start, y+start, TRUE);
				else
				if(plotType == AvProfileOptions::NORMALPLOT)
					cpgline(i-start, x+start, y+start);
			start = i+1;// Don't count this blanked value.
		}

	  // Draw any lines at the end.
	  if(npts-start > 0)
		if(plotType == AvProfileOptions::HISTOGRAM)
	 		cpgbin(int(npts-start), x+start, y+start, TRUE);
		else
		if(plotType == AvProfileOptions::NORMALPLOT)
	 		cpgline(int(npts-start), x+start, y+start);
	 cpgunsa();
	}
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	// Show markers at all non blanked points.
	// Should markers be shown?
	const AvProfileOptions::marker *marker =
					Options->getMarkerOptions(0);

	Boolean showMarker;
	if(plotType == AvProfileOptions::MARKERSONLY)
		showMarker = TRUE;
	else
	if(marker != NULL)
		showMarker = marker->show_.getValue();
	else
		showMarker = FALSE;

	if(showMarker)
	{	int mcolor =  marker->color_.getValue();
		int symbol =  (int)marker->type_.getValue();
		float scale = marker->scale_.getValue();

		cpgsave();
		 cpgsci(mcolor);
		 cpgsch(scale);
		 cpgslw(lw);
		 for(i=0; i<npts; i++)
			if(y[i] != Av::BlankedValue)
		 		cpgpt(1, &x[i], &y[i], symbol);
		cpgunsa();
	}
	////////////////////////////////////////////////////////////////


	delete [] x;
	delete [] y;
}
#endif

#ifdef PGPLOT
// Print the box, zero line labels etc. Everything that isn't the profile.
void AvPGProfile::drawBoxAndLabels()
{ AvDataSet::AXISPRINTFORMAT apfmt = accessor()->axisPrintFormat(zAxis());
  AvString pgtboxXOptions = "ST";
  AvString pgtboxYOptions = "ST";
  Boolean drawZeroLine;		// Draw zero line W/O tickmarks.
  AvProfileOptions *Options = options();
  // Global character scale affecting all charcters. Labels and axis chars
  // also have individual scales.
  float charscale = Options->gCharScale_.getValue();

	if(Options->gDrawAxisLabels_.getValue())
	{	pgtboxXOptions += 'N';
		pgtboxYOptions += 'N';
	}

	// Draw frame?
	if(Options->gDrawFrame_.getValue())
	{	pgtboxXOptions += "BC";
		pgtboxYOptions += "BC";
	}

	// Draw grid?
	if(Options->gDrawGrid_.getValue())
	{	pgtboxXOptions += "G";
		pgtboxYOptions += "G";
	}

	switch(Options->gDrawZeroLine_.getValue()) {
	case AvProfileOptions::OFF:
		drawZeroLine = FALSE;
		break;
	case AvProfileOptions::WITHTICKS:
		drawZeroLine = FALSE;
		pgtboxXOptions += "A";	// Have PGPLOT draw zero line;
		break;
	case AvProfileOptions::WITHOUTTICKS:
	default:
		drawZeroLine = TRUE;
		break;
	}


	if (apfmt == AvDataSet::HMS)
	{ // PGPlot decides how to print HMS
		pgtboxXOptions += "ZHY";
	}
	else if (apfmt == AvDataSet::DMS)
	{ // PGPlot decides how to print DMS
		pgtboxXOptions += "ZD";
	}

	AvProfile::ProfileData *data = profileData();
	float xTickMin=data->xDmin(), xTickMax=data->xDmax();

	// Character font
	int fontNum = Options->gFont_.getValue();
	fontNum += 1;		// Goes from 1..n.
	cpgscf(fontNum);

	////////////////////////////////////////////////////////////////
	///			Draw Box and Zero Line
	////////////////////////////////////////////////////////////////
	cpgsls(1);	// Make sure box is solid.
	float axisCharScale =
			(float)Options->gAxisCharScale_.getValue();
	// PGPLOT option string.
	char xOptions[16], yOptions[16];
	strcpy(xOptions, pgtboxXOptions);
	strcpy(yOptions, pgtboxYOptions);
//	cpgsave();
	 cpgsch(axisCharScale*charscale);
	 cpgtbox(xOptions, 0.0, 0, yOptions, 0.0, 0);	// Draw axis.
//	cpgunsa();

	// Draw the 0 line. It's done 'manually' since letting pgtbox
	// do it results in tick marks on the line.
	if(drawZeroLine)
	{float x0[2], y0[2];	// Draw 0 line.
		x0[0] = xTickMin;
		x0[1] = xTickMax;
		y0[0] = 0.0;
		y0[1] = 0.0;
		cpgline(2, x0, y0);
	}
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	// Draw labels.
	////////////////////////////////////////////////////////////////
	if(Options->gDrawLabels_.getValue())
	{	printLabel(
		    Options->getLabelOptions(AvProfileOptions::XLABEL),
		    charscale);
		printLabel(
		    Options->getLabelOptions(AvProfileOptions::YLABEL),
		    charscale);
		printLabel(
		    Options->getLabelOptions(AvProfileOptions::TITLE),
		    charscale);
	}
	////////////////////////////////////////////////////////////////
}
#endif

#ifdef PGPLOT
void AvPGProfile::printProfile(const char * /*fileName*/ )
{
	hardCopy_ = TRUE;
	 drawProfile();
	hardCopy_ = FALSE;
}
#endif

#ifdef PGPLOT
// Open PGPLOT device, set viewport and world window.
// Returns TRUE for success, FALSE for failure.
Boolean AvPGProfile::openProfileDevice(const Boolean hardcopy)
{ int opencode;
  AvProfile::ProfileData *data = profileData();

	////////////////////////////////////////////////////////////////
	// Generate device and filenames and open PGPLOT device
	////////////////////////////////////////////////////////////////
	if(hardcopy)
	{AvString oname = data->ofileName();
	 AvString fn = AvPGCanvas::getPGfilename(PROFILE_FILENAME,
						 oname.chars());
		fn += ".profile.ps";

	 // Get output device.
	 AvString dev;
	 AvProfileOptions *optns = options();
	 int odevice = optns->gPSDevice_.getValue();

		switch (odevice) {
		case AvProfileOptions::MONOPS_LS:
			dev = "/PS";
			break;
		case AvProfileOptions::MONOPS:
			dev = "/VPS";
			break;
		case AvProfileOptions::COLORPS_LS:
			dev = "/CPS";
			break;
		default:
		case AvProfileOptions::COLORPS:
			dev = "/VCPS";
			break;
		}

		AvString name = AvPGCanvas::makePGname( fn.chars(),
							dev.chars());
		// Open device using PGCanvas' open because of the const
		// string.
		opencode = AvPGCanvas::cpgbeg(0, name, 1, 1);
		if(opencode != 1)	// PGPLOT open failed.
			return FALSE;

		printf("Printing to device \"%s\", file: %s\n",
			dev.chars(), fn.chars());
	}
	else	// Draw PGPLOT profile to an X window.
	{	driver_ = new AvPGDriver();
		driver_->setCanvas(xCanvas_);
		AvPolyline *pl = new AvPolyline();
		pl->name("profilePoly");
		if(vps_ == NULL)
		{	// The PGDriver will reset the size to 1x1.
			vps_ = new AvViewportSetter(0.0, 0.0);
			vps_->name("profileVPS");
			vps_->ref();	// Keep around.
			xCanvas_->setDisplayObject(vps_);
		}

		vps_->removeList();	// Clean any old crud.
		vps_->append(pl);	// Add new.
		driver_->setVPS(vps_);
		driver_->newPolyline(pl);
		AvPGDriver::currentDriver(driver_);

		opencode = AvPGCanvas::cpgbeg(0, "/canvas", 1, 1);

		if(opencode != 1)	// PGPLOT open failed.
		{	delete driver_;
			driver_ = NULL;
			return FALSE;
		}
	}

	// PGPLOT device has now been opened.

	cpgask(0);		// Don't ask.
	cpgbbuf();		// Buffer calls.
	////////////////////////////////////////////////////////////////
	// Set world window after, possibly, convert xaxis values to secs.
	////////////////////////////////////////////////////////////////

	float xDataMin=(float)data->xDmin(), xDataMax = (float)data->xDmax();

	float yDataMin = accessor()->clipMin();
	float yDataMax = accessor()->clipMax();

	// On which side of the viewport do we place the min value?
	float x0, x1, y0, y1;
	AvProfileOptions *optns = options();

	switch(optns->xMinMax_.getValue()) {
	default:
	case AvProfileOptions::DEFAULTMINMAX:	// Right out of data set.
		x0 = xDataMin;
		x1 = xDataMax;
		break;
	case AvProfileOptions::FLIPPED:		// flip it.
		x0 = xDataMax;
		x1 = xDataMin;
		break;
	case AvProfileOptions::MINFIRST:
		if( xDataMin <= xDataMax)
		{	x0 = xDataMin;
			x1 = xDataMax;
		}
		else
		{	x0 = xDataMax;
			x1 = xDataMin;
		}
		break;
	case AvProfileOptions::MAXFIRST:
		if( xDataMin >= xDataMax)
		{	x0 = xDataMin;
			x1 = xDataMax;
		}
		else
		{	x0 = xDataMax;
			x1 = xDataMin;
		}
		break;
	}

	switch(optns->yMinMax_.getValue()) {
	default:
	case AvProfileOptions::DEFAULTMINMAX:
		y0 = yDataMin;
		y1 = yDataMax;
		break;
	case AvProfileOptions::FLIPPED:
		y0 = yDataMax;
		y1 = yDataMin;
		break;
	case AvProfileOptions::MINFIRST:
		if( yDataMin <= yDataMax)
		{	y0 = yDataMin;
			y1 = yDataMax;
		}
		else
		{	y0 = yDataMax;
			y1 = yDataMin;
		}
		break;
	case AvProfileOptions::MAXFIRST:
		if( yDataMin >= yDataMax)
		{	y0 = yDataMin;
			y1 = yDataMax;
		}
		else
		{	y0 = yDataMax;
			y1 = yDataMin;
		}
		break;
	}

	cpgswin(x0, x1, y0, y1);

	// Line width is global, so set it here.
	int lw = (int) optns->gLineWidth_.getValue();
	if(lw > 0)
		cpgslw(lw);

	////////////////////////////////////////////////////////////////
	// Set desired viewport size.
	////////////////////////////////////////////////////////////////
	{ float scale = (float)optns->gViewportSize_.getValue();
	  float offset;

		offset = (1.0 - scale)/2.0;	// Center;
		cpgsvp(offset, offset + scale, offset, offset+scale);
	}
	////////////////////////////////////////////////////////////////

	return TRUE;
}
#endif

#ifdef PGPLOT
	// Close PGPLOT device.
void AvPGProfile::closeProfileDevice()
{
	cpgebuf();			// Stop buffering.
	AvPGCanvas::cpgend();
	// If output was to display list, need to send it to window.
	if(! hardCopy_)			// Output to window.
	{Dimension vph, vpw;
	 Widget w = profileWidget();

		// It is necessary to reset the size since the PGDriver
		// sets it to 1x1.
		XtVaGetValues (w,
		 XmNheight, &vph,
		 XmNwidth, &vpw,
		 NULL);
		vps_->setSize(vpw, vph);

		xCanvas_->show();
		haveProfile_ = TRUE;
		delete driver_;
		driver_ = NULL;
//cout << "PG width = " << vpw << " H = " << vph << endl;
	}
}
#endif

#ifdef PGPLOT

// Print X, Y or Title.
void AvPGProfile::printLabel(const AvProfileOptions::axisLabel *Label,
			   const float charScale)
{const char *side;
 float cscl;
 float from = Label->fromEdge_.getValue();
 float along = Label->alongEdge_.getValue();
 float justify;
 const char *label;

	// Show this label?
	if(!Label->drawLabel_.getValue())
		return;

	label = Label->text_.getString();
	if((label == NULL) || (strlen(label) == 0))
		return;

	switch(Label->justify_.getValue()){
	case AvProfileOptions::LEFT:
			justify = 0.0;
			break;
	case AvProfileOptions::RIGHT:
			justify = 1.0;
			break;
	default:
	case AvProfileOptions::CENTER:
			justify = 0.5;
			break;
	}

	switch(Label->side_.getValue()){
	case AvProfileOptions::LEFT:
			side = "L";
			break;
	case AvProfileOptions::LEFTP:
			side = "LV";
			break;
	case AvProfileOptions::RIGHT:
			side = "R";
			break;
	case AvProfileOptions::RIGHTP:
			side = "RV";
			break;
	case AvProfileOptions::BOTTOM:
			side = "B";
			break;
	case AvProfileOptions::TOP:
			side = "T";
			break;
	default:
			side = "L";
	}
	// The resultant scale is the product of the global character scale
	// and the per label scale.
	cscl= (float)(charScale*Label->charScale_.getValue());

	float scl;
	cpgqch(&scl);
	cpgsave();
	 cpgsch(cscl);	// Adjust character size.
	 from /= cscl;	// Keeps distance constant when char scale varies.
	 from *= scl;
	 cpgmtxt(side, from, along, justify, label);
	cpgunsa();
}
#endif

#ifdef PGPLOT
#if 0
void AvPGProfile::redrawProfile()
{
//	if(haveProfile_)
//		xCanvas_->show();
//	else
		drawProfile();
}
#endif
#endif


#ifdef PGPLOT
AvProfile *AvPGProfile::getIVPProfile(AvImageViewPanel *ivp)const
{
	return ivp->pgProfile();
}
#endif
