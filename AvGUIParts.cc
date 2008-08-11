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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvGUIParts.cc,v 19.0 2003/07/16 05:46:28 aips2adm Exp $
//
// $Log: AvGUIParts.cc,v $
// Revision 19.0  2003/07/16 05:46:28  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:16  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.2  2002/01/22 21:56:40  hravlin
// PtrToInt() is now in Av.h. cast return of strlen() to (int).
//
// Revision 17.1  2002/01/07 22:27:48  hravlin
// Removed check for __P.
//
// Revision 17.0  2001/11/12 19:41:44  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:41:54  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.1  2001/03/21 17:43:07  hravlin
// Fixed ambiguous arg to pow().
//
// Revision 15.0  2000/10/26 17:07:37  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:52:51  hravlin
// Made escapeCodes a const.
//
// Revision 14.0  2000/03/23 16:07:28  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.2  2000/03/06 16:19:08  hravlin
// Reduced length of FRAMENAME string to get around lesstif buffer overflow
// problem.
//
// Revision 13.1  1999/08/25 19:51:58  hravlin
// Fixed problem with casting from XtPointer to enum.
//
// Revision 13.0  1999/08/10 18:39:07  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:10  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:57:40  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:51:08  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.2  1998/06/09 21:26:55  hr
// Fixed some signed/unsigned/long, etc. type conversion warnings.
//
// Revision 9.1  1997/11/17 16:49:18  hr
// Added ability to have internal callback come before or after caller's.
//
// Revision 9.0  1997/08/25 21:23:22  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.3  1997/05/07 21:33:10  hr
// Replaced Gadgets with Widgets.
// buildIntMapPMRadioBox() (called by buildColorSelector()) would hang
// in the XtVaSetValues call under Linux 1.2.13 and our Motif 2.0.2
// installation. The problem went away after converting to toggleButonWidgets.
//
// Revision 8.2  1997/04/28 17:18:12  hr
// labeledFrame(): Generate frame's name from label and "LabeledFrame".
//
// Revision 8.1  1997/04/25 16:06:40  hr
// In buildIntMapPMRadioBox() if label is NULL realize that box is the
// radio box.
//
// Revision 8.0  1997/02/20 03:19:37  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1997/02/09 22:34:33  bglenden
// __P nad linux, and *pixmaps must compare to an integer, not a pointer.
//
// Revision 7.1  1997/02/09 00:08:33  hr
// Miscellaneous routines for building user interface, particularly with
// Notifiers.
//
//
//---------------------------------------------------------------------------

/* AvGUIParts.cc
Miscellaneous routines for building user interface, particularly with
Notifiers.
*/
#include <string.h>
#include <math.h>
#include <Xm/Scale.h>
#include <Xm/Frame.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>
#include <Xm/ToggleB.h>
#include <Xm/Text.h>

#include <AvPGDriver.h>
#include <AvPG2Pixmap.h>
#include <AvXColorApp.h>
#include <AvGUIParts.h>
#include <Av.h>

static const int SHADOWTHICKNESS = 1;
static const int BORDERWIDTH = 1;
// 'Standard' size for toggle button pixmaps.
static const int PIXMAPSIZE=16;

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// Create a frame with a title that is either left, center or right
// justified. (The title does not count as the frame's 'only' child).
static const char *FRAMENAME = "Frame";
Widget labeledFrame(Widget parent, const char *label,
		    const TITLEJUSTIFY justify)
{char *frameName;

	// Build a frame name from the label. (Helpful for debugging).
	{	if(label != NULL)
		{	frameName= new char[strlen(label)+strlen(FRAMENAME)+1];
			strcpy(frameName, label);
		}
		else
		{	frameName = new char[strlen(FRAMENAME)+1];
			frameName[0] = '\0';
		}
		strcat(frameName, FRAMENAME);
	}

	Widget frame = XtVaCreateManagedWidget(frameName,
			xmFrameWidgetClass,
			parent,
			NULL);
	delete [] frameName;

	unsigned char alignment;
	switch(justify) {
	case CENTER:
		alignment = XmALIGNMENT_CENTER;
		break;
	case RIGHT:
		alignment = XmALIGNMENT_END;
		break;
	case LEFT:
	default:
		alignment = XmALIGNMENT_BEGINNING;
		break;
	}

	XtVaCreateManagedWidget(label,
			xmLabelWidgetClass,
			frame,
			XmNchildType, XmFRAME_TITLE_CHILD,
			XmNchildHorizontalAlignment, alignment,
			XmNchildVerticalAlignment, XmALIGNMENT_WIDGET_BOTTOM,
			NULL);
	return frame;
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	///		Utility routines.
	////////////////////////////////////////////////////////////////

// Convert a value to a scale value arg.
static inline int convertValue(const double value, const int dp)
{
	if(dp == 0)
		return (int)value;
	else
		return (int) (value*pow(10.0, dp));
}

// converts an XmString to a string.
static char *XmStringToString_(XmString string)
{
char *text;

	if(XmStringGetLtoR(string, XmSTRING_DEFAULT_CHARSET, &text))
		return text;
	else
		return NULL;
}

// Return the value of a toggle button.
static Boolean getToggle(Widget w)
{
	if(w != NULL)
		return XmToggleButtonGetState(w) ? TRUE : FALSE ;
	else
		return FALSE;
}

////////////////////////////////////////////////////////////////
//			Callbacks
////////////////////////////////////////////////////////////////

// Default callback routine for double sliders.
// Transfer the slider value to the notifier.
static void handleDoubleSliderCB(Widget w, XtPointer cbdata, XtPointer)
{AvDoubleNotifier	*d = (AvDoubleNotifier *)cbdata;
 short ndp;
 int   value;
 double dvalue;

	if(d == NULL)
	{	cerr	<< "AvGUIParts::handleDoubleSliderCB: "
			<< (char *)XtName(w) << " has no notifier.\n";
		return;
	}

	XtVaGetValues(w, XmNdecimalPoints, &ndp,
			 XmNvalue, &value,
			 NULL);
	// out = val*10^-dp.
	if(ndp == 0)
		dvalue = (double) value;
	else
		dvalue = ((double) value)*pow(10.0, -ndp);
	d->setValue(dvalue);
}

// Default callback routine for integer sliders.
// Transfer the slider value to the notifier.
static void handleIntegerSliderCB(Widget w, XtPointer cbdata, XtPointer)
{AvIntegerNotifier	*n = (AvIntegerNotifier *)cbdata;
 int   value;

	if(n == NULL)
	{	cerr	<< "AvGUIParts::handleIntegerSliderCB: "
			<< (char *)XtName(w) << " has no notifer.\n";
		return;
	}

	XtVaGetValues(w, XmNvalue, &value,
			 NULL);
	n->setValue(value);
}

// Called when a radio button is pressed.
static void handleRadioCB(Widget w, XtPointer cbdata, XtPointer)
{AvIntMapNotifier	*r = (AvIntMapNotifier *)cbdata;
 char *name;
 Boolean isSet = getToggle(w);
 XmString	xmstr;

	if(isSet)
	{	XtVaGetValues(w, XmNlabelString, &xmstr, NULL);
		name = XmStringToString_(xmstr);
		r->setName(name);
	}
}

// Called when a toggle button is pressed.
// Ignores toggles being turned off.
static void handleToggleCB(Widget w, XtPointer cbdata, XtPointer)
{AvIntegerNotifier *t = (AvIntegerNotifier *)cbdata;

	if(t != NULL)
		t->setValue(getToggle(w));
	else
		cerr	<< "handleToggleCB: "
			<< (char *)XtName(w) << " has no notifer.\n";
}

// Called when the help text toggle button is pressed.
void handleHelpCB(Widget w, XtPointer cbdata, XtPointer)
{Widget help = (Widget )cbdata;
 Boolean isSet = XmToggleButtonGetState(w);

	if(help == NULL)
		return;

	if(isSet)
		XtManageChild(help);
	else
		XtUnmanageChild(help);
}

////////////////////////////////////////////////////////////////
// Routines to build sliders (XmScale widgets).
////////////////////////////////////////////////////////////////

/* Returns a RowColumn widget around a label and slider (XmScale).
This is the 'base' routine to generate a slider and is primarily called
by the other slider generation routines.

The XtPointer cbdata helps to cast away the const'ness of most notifiers.
*/
Widget buildSliderBox( Widget parent, const char *label,
			const int min, const int max,
			const int value0, const int dp,
			const Boolean showValue,
			XtPointer cbdata, XtCallbackProc rtn)
{
	Widget sliderRC = XtVaCreateWidget("sliderRC",
			xmRowColumnWidgetClass,
			parent,
			XmNorientation, XmHORIZONTAL,
//			XmNpacking, XmPACK_NONE,
			NULL);

	if(label != NULL)
		XtVaCreateManagedWidget(label,
				xmLabelWidgetClass,
				sliderRC,
				XmNpositionIndex, 0,
				NULL);

	// Make sure initial position is within range.
	int value = value0;
	if(value < min)
		value = min;
	else
	if(value > max)
		value = max;

	// Find the width of the slider.
	Dimension width = max-min+1 + 48;	// Plus some for the knob.
						// Empirical.
		// (The marker slider (-31...126) misses some values).
	// Limit its size.
	if(width < 128)
		width = 128;
	if(width > 256)
		width = 256;

	Widget slider = XtVaCreateManagedWidget("slider",
		xmScaleWidgetClass,
		sliderRC,
		XmNminimum, min,
		XmNmaximum, max,
		XmNvalue, value,
		XmNdecimalPoints, dp,
		XmNorientation, XmHORIZONTAL,
		XmNpositionIndex, XmLAST_POSITION,
		XmNscaleWidth, width,
		XmNshowValue, showValue ,
//		XmNprocessingDirection, XmMAX_ON_RIGHT,
		NULL);

	XtAddCallback(  slider, XmNdragCallback,
			rtn, cbdata);

	XtAddCallback(  slider, XmNvalueChangedCallback,
			rtn, cbdata);

	XtManageChild(sliderRC);
	return sliderRC;
}

// Create a slider box to control a double notifier.
Widget buildDoubleSliderBox(Widget parent, const char *label,
			    const double Min, const double Max,
			    const double Value0, const int dp,
			    const Boolean showValue,
			    const AvDoubleNotifier *ntfr)
{
	int value = convertValue(Value0, dp);
	int min = convertValue(Min, dp);
	int max = convertValue(Max, dp);

	return buildSliderBox(parent, label, min, max, value, dp,
			      showValue,
			      (XtPointer) ntfr, &handleDoubleSliderCB);
}

// Create a slider box to control an integer notifier.
Widget buildIntegerSliderBox(   Widget parent, const char *label,
				const int Min, const int Max,
				const int Value0,
				const Boolean showValue,
				const AvIntegerNotifier *ntfr)
{
	return buildSliderBox(parent, label,
			      Min, Max, Value0, 0,
			      showValue,
			      (XtPointer) ntfr, &handleIntegerSliderCB);
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

// Create a radio box to set an IntMapNotifier. A box with a label and a
// number of radio buttons.

Widget buildIntMapRadioBox(Widget parentW, const char *name,
			   const char *label,
			   const AvIntMapNotifier *imn,
			   XtPointer userData,
			   XtCallbackProc rtn,
			   XtPointer cbdata,
			   const GUICBPOSITION cbpos)
{Widget box, parent;
 int nlabels, i;

	if(imn == NULL)
		return NULL;
	nlabels = imn->mapLength();

	// Empirically, 40 chars will fit. If that doesn't work, it will
	// be necessary to reset the packing and numcolumns externally.
	// Use childNameToWidget(<returned widget>, <name>) to
	// get the radio box widget.
	// If the number of rows is > 1, the label is placed above the
	// toggles rather than to the left.

	// Use the number of characters in the labels to estimate the
	// number of columns.
	int ccount = 0;
	for(int j=0; j< nlabels; j++)
	{const char *lbl = imn->getName(j);
		if(lbl != NULL)
			ccount += (int)strlen(lbl);
	}

	ccount += nlabels*3;	// Count button + 2 spaces.

	unsigned char packing, orientation;

#define MAXC 40
	// Number of 40 char 'columns' (rows).
	short numColumns = ccount/MAXC + ((ccount %MAXC) ? 1 : 0);
	if(numColumns > 1)
	{	packing = XmPACK_COLUMN;
		orientation = XmVERTICAL;
	}
	else
	{	packing = XmPACK_TIGHT;
		orientation = XmHORIZONTAL;
	}

	// If there is a label create an RC box to hold everything.
	// If not, don't bother.
	if(label != NULL)
	{	box = XtVaCreateWidget("radioBoxRC",
				xmRowColumnWidgetClass,
				parentW,
				XmNpacking, XmPACK_TIGHT,
				XmNorientation, orientation,
				NULL);
		XtVaCreateManagedWidget(label,
				xmLabelWidgetClass,
				box,
				NULL);

		parent = box;
	}
	else
	{	parent = parentW;
		box = NULL;
	}

	// Create a simple radio box, but populate it manually.
	char *nam;
	// XmVaCreateSimpleRadioBox won't take a const char *.
	if(name != NULL)
	{	nam = new char[strlen(name)+1];
		strcpy(nam, name);
	}
	else
	{	nam = new char[16];
		strcpy(nam, "radioBox");
	}

	Widget rcbox = XmVaCreateSimpleRadioBox( parent,
			nam, 0, NULL,
			XmNorientation, XmHORIZONTAL,
			XmNpacking, packing,
			XmNnumColumns, numColumns,
			NULL);

	delete [] nam;

#if 0
	// The default callback.
	if(rtn == NULL)
		rtn = handleRadioCB;
#endif
	if(cbdata == NULL)
		cbdata = (XtPointer)imn;

	// Which button should be on?
	int onButton = imn->valueToIndex(imn->getValue());

	for(i=0; i< nlabels; i++)
	{ Widget w = XtVaCreateWidget(  imn->getName(i),
					xmToggleButtonWidgetClass, rcbox,
					XmNuserData, userData,
					XmNset, (i == onButton),
					XmNborderWidth, BORDERWIDTH,
					XmNshadowThickness, SHADOWTHICKNESS,
					NULL);
		// Add internal cb either before caller's or if caller's
		// is NULL.
		if( (rtn == NULL) || (cbpos & GUICB_BEFORE))
			XtAddCallback(w, XmNvalueChangedCallback,
				      handleRadioCB, (XtPointer) imn);
		if(rtn != NULL)	// Add caller's callback
			XtAddCallback(w, XmNvalueChangedCallback,
				      rtn, cbdata);
		// Add internal CB after caller's
		if((rtn != NULL) && (cbpos & GUICB_AFTER))
			XtAddCallback(w, XmNvalueChangedCallback,
				      handleRadioCB, (XtPointer) imn);
		XtManageChild(w);
	}

	XtManageChild(rcbox);
	if(box != NULL)
		XtManageChild(box);

	return (box == NULL) ? rcbox : box;
}

// Create a radio box using Pixmaps for labels.
Widget buildIntMapPMRadioBox(Widget parentW, const char *name,
		       const char *label,
		       const AvIntMapNotifier *imn,
		       Pixmap *pixmaps,
		       XtPointer userData,
		       XtCallbackProc rtn)
{Widget box;
 int nlabels;

	if(imn == NULL)
		return NULL;

	box = buildIntMapRadioBox(parentW, name, label, imn, userData, rtn);
	if(box == NULL)
		return box;

	nlabels = imn->mapLength();
	const IntMap *map = imn->getMap();

	// For each toggle, set the cooresponding Pixmap if it exists.
	for(int mk =0 ; mk < nlabels; mk++, pixmaps++)
	{ Widget toggle = childNameToWidget(box, map[mk].name);

		if(toggle == NULL)
			continue;
		if(*pixmaps == 0)
			continue;

		XtVaSetValues(toggle,
				XmNlabelType, XmPIXMAP,
				XmNlabelPixmap, *pixmaps,
				NULL);
	}

	// Reset the number of columns.
	int ncols = nlabels/8 + ((nlabels % 8) ? 1 : 0);
	Widget tbox;
	if(label != NULL)
		tbox = childNameToWidget(box, name);
	else
		tbox = box;

	XtVaSetValues(tbox,
		      XmNnumColumns, ncols,
		      NULL);
	return box;
}


////////////////////////////////////////////////////////////////
// Create a toggle button and wire it to the callback.
// Use the default callback if none is supplied.
Widget buildToggleButton(Widget parent, const char *label,
			 const AvIntegerNotifier *in,
			 XtPointer cbData, XtCallbackProc cb,
			 const GUICBPOSITION cbpos)
{
	Widget w = XtVaCreateManagedWidget(label,
			xmToggleButtonWidgetClass,
			parent,
			XmNset, in->getValue(),
			XmNborderWidth, BORDERWIDTH,
			XmNshadowThickness, SHADOWTHICKNESS,
			NULL);
	if(cbData == NULL)
		cbData = (XtPointer) in;

#if 0
	if(cb == NULL)
		cb = &handleToggleCB;
#endif

	if((cb == NULL) || (cbpos & GUICB_BEFORE))
		XtAddCallback(w, XmNvalueChangedCallback,
			      &handleToggleCB, (XtPointer) in);
	if(cb != NULL)
		XtAddCallback(w, XmNvalueChangedCallback, cb, cbData);

	if((cb != NULL) && (cbpos & GUICB_AFTER))
		XtAddCallback(w, XmNvalueChangedCallback,
			      &handleToggleCB, (XtPointer) in);
	return w;

}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
////////////////	Special purpose varients	//////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

//		Build a standardized color selector for choosing
//		PGPLOT colors

/// Clears a pixmap to a color.
class AvPGColor : public AvPG2Pixmap {
  public:
	AvPGColor(Widget reference, AvXPalette *pal,
		  const unsigned int width, const unsigned int height,
		  const unsigned depth=0);	// depth=0 -> use reference's.
	virtual ~AvPGColor();
	virtual void execute(AvPG2Pixmap *);
};

AvPGColor::AvPGColor( Widget ref, AvXPalette *pal,
			const unsigned int w, const unsigned int h,
			const unsigned int  d) : AvPG2Pixmap(ref, pal, w, h, d)
{
}

AvPGColor::~AvPGColor() {}

void AvPGColor::execute(AvPG2Pixmap *pm)
{ int color = Av::PtrToInt(pm->uData());
	setBackgroundColor(color);
	clear();	// Clear pixmap to correct color.
}

Widget buildColorSelector(Widget parent, const char *name,
			  const char *label, const AvIntMapNotifier *imn)
{
	if(imn == NULL)
		return NULL;
	int mapLength = imn->mapLength();
	AvXPalette *pal = AvXColorApp::palette();
	const IntMap *map = imn->getMap();

	int tbllen;
	PGColormap *pgpal = AvPGDriver::getPGColormappings(pal, tbllen);

	Pixmap *pixmaps = new Pixmap[mapLength];

	// For each entry in the toggle color table, build an AvPGColor
	// object and have it clear its pixmap to the color.
	for(int mk =0 ; mk < mapLength; mk++)
	{
		AvPGColor *pm = new AvPGColor(parent, pal,
					      PIXMAPSIZE, PIXMAPSIZE);
		pm->ref();
		int value = (int)map[mk].value;	// PGPLOT color index.
		// Convert PGPLOT index to X index. We assume color values
		// are indexes rather than TRUECOLOR values!!
		if(value < tbllen)
			value = (int) pgpal[value].pindex;
		// (We loose if it isn't <= tbllen).
		pm->display((XtPointer)value);
		pixmaps[mk] = pm->getPixmap();
		pm->unref();
	}
	delete pgpal;

	Widget box = buildIntMapPMRadioBox(parent, name, label, imn, pixmaps);
	delete [] pixmaps;
	return box;
}

	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	/// Build a selector for PGPLOT markers.
	////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//			Used to draw markers into pixmaps.
////////////////////////////////////////////////////////////////

class AvPGMarker : public AvPG2Pixmap {
  public:
	AvPGMarker(Widget reference, AvXPalette *pal,
		   const unsigned int width, const unsigned int height,
		   const unsigned depth=0);	// depth=0 -> use reference's.
	virtual ~AvPGMarker();
	virtual void execute(AvPG2Pixmap *);
	////////////////////
	int	ci_;	// PGPLOT colorindex.
	int	lineWidth_;
	float	scale_;
};

AvPGMarker::AvPGMarker( Widget ref, AvXPalette *pal,
			const unsigned int w, const unsigned int h,
			const unsigned int  d) : AvPG2Pixmap(ref, pal, w, h, d)
{
	ci_ = 3;
	scale_ = 60;	// Needs to be big for small pixmaps.
	lineWidth_ = 1;
}

AvPGMarker::~AvPGMarker() {}

void AvPGMarker::execute(AvPG2Pixmap *pm)
{float x = 0.5, y=0.5;
 int symbol = Av::PtrToInt(pm->uData());
	clear();	// Clear pixmap to correct color.

	cpgslw(lineWidth_);
	cpgsci(ci_);
	cpgsch(scale_);
	cpgpt(1, &x, &y, symbol);
}

Widget buildMarkerSelector(Widget parent, const char *name,
			   const char *label, const AvIntMapNotifier *imn)
{
	if(imn == NULL)
		return NULL;
	int mapLength = imn->mapLength();
	const IntMap *map = imn->getMap();

	// Get the foreground and background colors. Parent is used since
	// it may be available already.
	// The pixmap will be cleared to the background color and the
	// marker symbol will be written with the foreground color.
	Pixel bg=0, fg=0;
	XtVaGetValues(  parent,
			XmNbackground, &bg,
			XmNforeground, &fg,
			NULL);

	AvXPalette *pal = AvXColorApp::palette();

	// Try to convert the foreground color into a PGPLOT color index.
	{int tbllen, pic=-1;
	 PGColormap *pgpal = AvPGDriver::getPGColormappings(pal, tbllen);
		for(int ip =0; ip< tbllen; ip++)
			if(pgpal[ip].pindex == fg)
			{	pic = ip;
				break;
			}
		delete pgpal;
		if(pic >= 0)
			fg = pic;
		// If it wasn't in the table, just try with what we got.
	}

	// For each entry in the toggle type table, build an AvPGMarker
	// object and have it write the symbol into the pixmap which we
	// grab for our use.
	Pixmap *pixmaps = new Pixmap[mapLength];

	for(int mk =0 ; mk < mapLength; mk++)
	{	AvPGMarker *pm = new AvPGMarker(parent, pal,
						PIXMAPSIZE, PIXMAPSIZE);
		pm->ref();
		int value = (int)map[mk].value;	// Marker type.
		pm->ci_ = (int)fg;			// Marker color.
		pm->setBackgroundColor((int)bg);
		pm->display((XtPointer)value);
		pixmaps[mk] = pm->getPixmap();
		pm->unref();
	}

	Widget markerBox = buildIntMapPMRadioBox(parent, name,
						 label, imn,
						 pixmaps);
	delete [] pixmaps;

	// How many rows (called columns) should there be?
	int ncols = mapLength/8 + ((mapLength % 8) ? 1 : 0);
	Widget tbox = childNameToWidget(markerBox, "Type");
	XtVaSetValues(tbox,
		      XmNnumColumns, ncols,
		      NULL);

	return markerBox;

}

// Text that is displayed when "Show Escape Codes" is pressed.
static const char *escapeCodes = "\
                 From the PGPLOT manual:\n\
 \\u      start a superscript, or end a subscript\n\
 \\d      start a subscript, or end a superscript\n\
 \\b      backspace (overwrite previous character)\n\
 \\fn     switch to Normal font (1)\n\
 \\fr     switch to Roman font (2)\n\
 \\fi     switch to Italic font (3)\n\
 \\fs     switch to Script font (4)\n\
 \\\\      backslash character (\\)\n\
 \\x      multiplication sign (×)\n\
 \\.      centered dot (·)\n\
 \\A      Angstrom symbol (Å)\n\
 \\gx     greek letter corresponding to roman letter x.\n\
 \\mn\n\
 \\mnn    graph marker number n or nn (1-31)\n\
 \\(nnnn) character number nnnn (1 to 4 decimal digits) from the Hershey \
character set.\
	";

// Creates a toggle button with the label "Show Escape Codes". When pressed,
// window below the toggle will expand listing the PGPLOT escape codes.
Widget showEscapeCodes(Widget parent)
{
	// The one help button.
	Widget helpB = XtVaCreateManagedWidget("Show Escape Codes",
			xmToggleButtonWidgetClass,
			parent,
			XmNset, FALSE,
			NULL);

	// The help.
	Widget helpW = XtVaCreateWidget("escapeCodes",
		xmTextWidgetClass, parent,
		XmNeditable, FALSE,
		XmNcursorPositionVisible, FALSE,
		XmNeditMode, XmMULTI_LINE_EDIT,
		XmNtopOffset, 20,
		XmNbottomOffset, 20,
		XmNleftOffset, 20,
		XmNrightOffset, 20,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
//		XmNheight, 280,
//		XmNwidth, 382,
		XmNrows, 18,
		XmNscrollVertical, True,
		XmNwordWrap, TRUE,
		XmNvalue, escapeCodes,
		NULL);

	XtAddCallback(  helpB, XmNvalueChangedCallback,
			&handleHelpCB,
			(XtPointer) helpW);

	return helpB;
}

// Color information for all the color toggles.
static IntMap Colors[] = {
	{RED, "Red"},
	{GREEN, "Green"},
	{BLUE, "Blue"},
	{CYAN, "Cyan"},
	{MAGENTA, "Magenta"},
	{YELLOW, "Yellow"},
	{BLACK, "Black"},
	{WHITE, "White"},
	{ 0, NULL}
	};

const IntMap *PGPLOTColors()
{
	return Colors;
}
