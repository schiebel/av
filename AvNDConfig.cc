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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvNDConfig.cc,v 19.0 2003/07/16 05:48:06 aips2adm Exp $
//
// $Log: AvNDConfig.cc,v $
// Revision 19.0  2003/07/16 05:48:06  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:40  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.2  2002/01/30 22:36:28  hravlin
// Ensured a widget and setAxis always get set.
//
// Revision 17.1  2002/01/22 22:10:03  hravlin
// PtrToInt() is now in Av.h.
//
// Revision 17.0  2001/11/12 19:43:08  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:14  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:45  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:43:49  hravlin
// Changed some char * to const char *.
//
// Revision 14.0  2000/03/23 16:09:12  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/08/25 19:46:53  hravlin
// Edits (mostly casts) to remove compiler warnings.
// Fixed problem with casting from XtPointer to enum.
//
// Revision 13.0  1999/08/10 18:41:03  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:28  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:02:23  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:56:40  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.3  1998/07/16 16:17:37  hr
// Changes to build() to deal with the possibility that axis 2 might not always
// be the default Z axis.
//
// Revision 9.2  1998/06/19 14:14:09  hr
// Added GNUC (for egcs) to list of compilers complaining about a cast to an enum.
//
// Revision 9.1  1998/03/10 22:07:03  hr
// SGI 7.2.0 & 7.2.1 compilers were giving errors on a cast.
//
// Revision 9.0  1997/08/25 21:32:01  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.3  1997/08/19 17:35:12  hr
// Changes to toggle button logic to handle differences in the way Motif 1 & 2
// turn radio buttons on and off.
//
// Revision 8.2  1997/08/18 19:03:42  hr
// Changed toggle buttons from being gadgets to widgets since trying to
// set sensitivity of ow_ in RowEntry() would cause a crash on (some) Linux
// systems.
//
// Revision 8.1  1997/02/25 17:12:56  hr
// Implements GUI for user selection of axis mapping for data sets with
// rank > 3.
//
//
//---------------------------------------------------------------------------

/* AvNDConfig.cc
Implements the user interface for allowing users to select which part
of a multidimensional data set to access.

Aipsview will probably always treat images as only having X, Y and Z
(animation and profile) axes. However, it is possible to allow the user to
specify which of a dataset's axes are treated as X, Y and Z.

Pressing one of the "Set Mapping" buttons would brings up a window
something like:

"RA"   [X][Y][Z] [Start: <slider> [text field]] [End: <slider> [text field]
"DEC"  [X][Y][Z] [Start: <slider> [text field]] [End: <slider> [text field]
"VELO" [X][Y][Z] [Start: <slider> [text field]] [End: <slider> [text field]
"xxx"  [X][Y][Z] [Start: <slider> [text field]] [End: <slider> [text field]

X, Y and Z are buttons to assign axis mapping. Axes not so assigned are
"Other". The start slider is used to specify where along an axis aipsview
should retrive begin retrieving XYZ data. Similarly, the end slider is used
to specify the end of the axis. "Other" axes do not allow the user to
change the End slider. (It can be gotten around, though).

Since a slider might not have enough resolution, a text field is provided
to allow the user to type in an integer explicitly.

Note:
	Currently, there is a problem between the CLUImageSetter and the
Accessor in that they expect the complete DS Z axis to be available. If the
user were allowed to explicitly set the Z range here, the accessor for a
slice could wind up complaining about an illegal (negative) slice.
Therefore, for now, the Z axis is sliders are disabled and any setting are
bypassed by the getStart and getShape routines.
*/

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Xm/Xm.h>
#include <Xm/PushB.h>
#include <Xm/TextF.h>
#include <Xm/RowColumn.h>
#include <Xm/Scale.h>
#include <Xm/Label.h>
#include <Xm/ToggleB.h>
#include "AvNDConfig.h"
#include "AvGUIParts.h"
#include "AvAccessor.h"
#include "Av.h"

static const int SHADOWTHICKNESS = 1;
static const int BORDERWIDTH = 1;
class RowList;
class Valuator;

enum AXIS {NONE=0, X=1, Y=2, Z=4, XYZ= X|Y|Z, OTHER=0x10};

// Return the value of a toggle button.
static Boolean getToggle(Widget w)
{
	if(w != NULL)
		return XmToggleButtonGetState(w) ? TRUE : FALSE ;
	else
		return FALSE;
}


// Object to contain the X/Y/Z/Other toggles and the start/end sliders.
class RowEntry {
  public:
	RowEntry(const char *name, Widget parent, const AXIS which,
		 const int start, const int maxIndex, const int labelLength=0);
	// If zCheck is TRUE, z is handled differently.
	int getStart(const Boolean zCheck=TRUE)const;
	int getLength()const;
	// Returns which of X, Y, Z or Other is set.
	AXIS whichAxis()const{return which_;}
	// If axis is turned on, turn on Other instead.
	void checkFor(const AXIS axis);
	// Set one of the buttons.
	void setAxis(const AXIS axis);
	void setRowList(RowList *l){list_=l;}
	// Changes 0/1 relative readouts.
	void setOffset(const int offset);
	// Make the slider/text area (in)sensitive.
	// start refers to the start field.
	void setSensitive(const Boolean sensitive, const Boolean start=TRUE);
  private:
	void setTextString(char *);
  private:
	int	max_;
	// Turn on the Other button.
	void setOther();
	void setTextValue(const int);
	int getTextValue()const;
	Widget buildToggle(const char *name, Widget parent,
			   const Boolean isSet, XtPointer ud);
	// Called when a row choice (X/Y/Z/Other) toggle changes.
	static void rowToggleCB(Widget, XtPointer cbdata, XtPointer);
	Widget	xw_;
	Widget	yw_;
	Widget	zw_;
	Widget	ow_;
	AXIS	which_;		// Which of x, y, z or other is set.
	Valuator	*start_;
	Valuator	*end_;
	RowList	*list_;
	int	lbllen_;	// If non zero, make the label this wide.
};

// Contains and manages a list of row entries.
class RowList {
  public:
	RowList(const int maxLen);
	~RowList();
	void addEntry(RowEntry *e);
	// Scan list and change any entries that are set the same as
	// the argument. This implements the '1 of many' in the vertical
	// column.
	void scan(RowEntry *);
	void setOffset(const int offset);
	AvIPosition getStart(const Boolean XYZareZero=TRUE)const;
	AvIPosition getShape()const;
	AvIPosition getMap()const;
	void setSensitive(const Boolean sensitive);
	int currentZ()const;
  private:
	int		maxLen_;
	int		next_;
	RowEntry	**list_;
	Boolean		scanning_;
};

// Implement a slider/text field pair. The text field exists because the
// slider may not have enough resolution.
class Valuator {
  public:
	Valuator(const char *name, Widget parent,
		 const int max, const int val0);
	int getValue()const;
	void setValue(const int);
	void setValuator(Valuator *, const Boolean isMin_);
	void setSensitive(const Boolean isSensitive);
	void setOffset(const int off);
  private:
	void setTextValue(const int val0);
	void setTextString(const char *str);
	int getTextValue()const;
	void setSliderValue(const int value);
	int getSliderValue()const;
	// Compare the check value to the current setting. If current
	// value is out of range, adjust it.
	void checkValue(const int checkValue);
	static void sliderCB(Widget, XtPointer cbdata, XtPointer);
	static void textCB(Widget, XtPointer cbdata, XtPointer);
	int	max_;
	Widget	box_;
	Widget	slider_;
	Widget	text_;
	int	offset_;
	Boolean	isMin_;
	Valuator *val_;
};


Valuator::Valuator(const char *label, Widget parent,
		   const int max, const int Val0)
{
	max_ = max;
	val_ = NULL;
	isMin_ = TRUE;
	offset_ = 0;
	int val0 = Val0;

	box_ = labeledFrame(parent, label, LEFT);
	Widget rc = XtVaCreateManagedWidget(label,
					xmRowColumnWidgetClass,
					box_,
					XmNpacking, XmPACK_TIGHT,
					XmNorientation, XmHORIZONTAL,
					NULL);
	if(val0 < 0)
		val0 = 0;
	else
	if(val0 >max)
	{	//cout << "max " << max << " val0 = " << val0 << endl;
		val0 = max;
	}

	slider_ = XtVaCreateManagedWidget("slider",
		xmScaleWidgetClass,
		rc,
		XmNminimum, 0,
		XmNmaximum, max,
		XmNvalue, val0,
		XmNdecimalPoints, 0,
		XmNorientation, XmHORIZONTAL,
		XmNpositionIndex, XmLAST_POSITION,
//		XmNscaleWidth, width,
		XmNshowValue, False ,
		NULL);

	XtAddCallback(  slider_, XmNdragCallback,
			&Valuator::sliderCB, (XtPointer)this);

	XtAddCallback(  slider_, XmNvalueChangedCallback,
			&Valuator::sliderCB, (XtPointer) this);

	// The text field exists because the slider may not have the resolution
	// to resolve all values.
	text_ = XtVaCreateManagedWidget("textField",
			xmTextFieldWidgetClass,
			rc,
			XmNcolumns, 4,
			XmNblinkRate, 0,
			NULL);
	setTextValue(val0);

	XtAddCallback(text_, XmNactivateCallback, &Valuator::textCB,
		      (XtPointer) this);

}

// Make the slider/text area (in)sensitive.
void  Valuator::setSensitive(const Boolean sensitive)
{
	if(box_ != NULL)
		XtSetSensitive(box_, sensitive);
}

void Valuator::setTextString(const char *str)
{
	if(text_ == NULL)
		return;
        if(str == NULL)
                str = "";

        XtVaSetValues(text_, XmNvalue, str, NULL);
}

void Valuator::setTextValue(const int value)
{char buf[64];
 int val;
	if(value < 0)
		val = 0;
	else
	if(value > max_)
		val = max_;
	else
		val = value;

	sprintf(buf, "%d", val+offset_);
	setTextString(buf);
}

int Valuator::getTextValue()const
{ int val;
	if(text_ != NULL)
	{ char *str;
		XtVaGetValues(text_, XmNvalue, &str, NULL);
		val = atoi(str) - offset_;
	}
	else
		val = 0;
	return val;
}

int Valuator::getValue()const
{
	return getTextValue();
}

// Change the 1/0 offset and update the text field readout.
void Valuator::setOffset(const int offset)
{int val = getTextValue();
	offset_ = offset;
	setTextValue(val);
}

void Valuator::setSliderValue(const int value)
{int val;

	if(slider_ == NULL)
		return;

	if(value < 0)
		val = 0;
	else
	if(value > max_)
		val = max_;
	else
		val = value;

        XtVaSetValues(  slider_,
			XmNvalue, val,
			NULL);
}

int Valuator::getSliderValue()const
{int value;

	if(slider_ == NULL)
		return 0;

        XtVaGetValues(  slider_,
			XmNvalue, &value,
			NULL);
	return value;
}

void Valuator::setValuator(Valuator *v, const Boolean isMin)
{
	val_ = v;
	isMin_ = isMin;
}

// Compare check value to current value and adjust if necessary. This
// is called by the other slider to make sure the start value is never
// greater than the end value.
void Valuator::checkValue(const int cv)
{int value = getValue();

	if(isMin_ && (value > cv))
	{	setSliderValue(cv);
		setTextValue(cv);
	}
	else
	if(!isMin_ && (value < cv))
	{	setSliderValue(cv);
		setTextValue(cv);
	}
}

void Valuator::sliderCB(Widget , XtPointer client_data, XtPointer)
{ Valuator *me = (Valuator *)client_data;
  int value = me->getSliderValue();

	me->setTextValue(value);
	if(me->val_ != NULL)
		me->val_->checkValue(value);
}

// Called when the text field changes. Copies the text field to the notifier.
void Valuator::textCB(Widget , XtPointer cbdata, XtPointer )
{ Valuator *me = (Valuator *)cbdata;

	int val = me->getTextValue();
	int value;
	// Make sure user typed in a correct value.
	if(val < 0)
		value = 0;
	else
	if(val > me->max_)
		value = me->max_;
	else
		value = val;
	if(value != val)
		me->setTextValue(value);

	me->setSliderValue(value);
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

Widget RowEntry::buildToggle(const char *name, Widget parent,
			     const Boolean isSet, XtPointer ud)
{
	Widget w = XtVaCreateWidget(  name,
				xmToggleButtonWidgetClass, parent,
				XmNuserData, NULL,
				XmNset, isSet,
				XmNborderWidth, BORDERWIDTH,
				XmNshadowThickness, SHADOWTHICKNESS,
				XmNuserData, ud,
				NULL);
	XtAddCallback(w, XmNvalueChangedCallback,
			&RowEntry::rowToggleCB, this);
	XtManageChild(w);
	return w;
}

static inline Boolean isSet(const AXIS me, const AXIS what)
{
	return (me == what) ? TRUE : FALSE;
}

RowEntry::RowEntry(const char *name, Widget parent, const AXIS which,
		   const int start, const int max, const int lbllen)
{
	list_ = NULL;
	lbllen_ = lbllen;
	max_ = max;
	start_ = end_ = NULL;

	Widget rowRC = XtVaCreateManagedWidget(name,
			xmRowColumnWidgetClass,
			parent,
			XmNpacking, XmPACK_TIGHT,
			XmNorientation, XmHORIZONTAL,
			NULL);

	// If lbllen_ > 0, create label with a dummy label then fix its
	// size before changing to the correct label. This is done to
	// get around problems of variable sized fonts making the labels
	// slightly different sizes.
	if(lbllen_ > 0)
	{char *buf = new char[lbllen_+1];
		for(int i=0; i< lbllen_; i++)
			buf[i] = 'W';
		buf[lbllen_] = '\0';
		Widget l = XtVaCreateManagedWidget(buf,
				xmLabelWidgetClass,
				rowRC,
				NULL);
		XtVaSetValues(	l,
			      	XmNrecomputeSize, FALSE,
				NULL);
		char *nam = new char[strlen(name)+1];
		strcpy(nam, name);
		XmString xStr = XmStringCreateLocalized(nam);
		XtVaSetValues(l, XmNlabelString, xStr, NULL);
		XmStringFree (xStr);
		delete [] nam;
		delete [] buf;
	}
	else
		XtVaCreateManagedWidget(name,
					xmLabelWidgetClass,
					rowRC,
					NULL);


	char ac[] = "axisChoice";
	Widget toggleBox = XmVaCreateSimpleRadioBox(rowRC,
			ac, 0, NULL,
			XmNorientation, XmHORIZONTAL,
			XmNpacking, XmPACK_TIGHT,
//			XmNnumColumns, numColumns,
			NULL);

	xw_ = buildToggle("X", toggleBox, isSet(X, which), (XtPointer)X);
	yw_ = buildToggle("Y", toggleBox, isSet(Y, which), (XtPointer)Y);
	zw_ = buildToggle("Z", toggleBox, isSet(Z, which), (XtPointer) Z);
	ow_ = buildToggle("", toggleBox, isSet(OTHER, which),
			  (XtPointer)OTHER);
	switch(which){
	case X:	which_ = X;
		break;
	case Y:	which_ = Y;
		break;
	case Z:	which_ = Z;
		break;
	case OTHER:
		which_ = OTHER;	// Fall through.
	default:
		break;
	}

	// This makes the ow_ toggle more or less invisible and
	// unchangeable by the user.
	Pixel bg;
	XtVaGetValues(parent,
		XmNbackground, &bg,
		NULL);

	XtVaSetValues(ow_,
		XmNwidth, 1,
		XmNheight, 1,
		XmNindicatorOn, FALSE,
		// Causes a crash if ow_ is a Gadget on (some) Linux systems.
		XmNsensitive, FALSE,
		XmNborderWidth, 0,
		XmNshadowThickness, 0,
		XmNselectColor, bg,
		NULL);


	XtManageChild(toggleBox);

	if(max <= 0)
		return;
	start_ = new Valuator("Start", rowRC, max, start);
	end_ = new Valuator("End", rowRC, max, max);
	start_->setValuator(end_, TRUE);
	end_->setValuator(start_, FALSE);
	if((which == OTHER) || (which == Z))
		setSensitive(FALSE);
}

// Returns which of X, Y, Z or Other is set.
#if 0
AXIS RowEntry::whichAxis()const
{	if(getToggle(xw_))
		return X;
	else
	if(getToggle(yw_))
		return Y;
	else
	if(getToggle(zw_))
		return Z;
	else
		return OTHER;	// One has to be set.
}
#endif

// Change the text field readout.
void RowEntry::setOffset(const int offset)
{
	if(start_ != NULL)
		start_->setOffset(offset);
	if(end_ != NULL)
		end_->setOffset(offset);
}

// If axis is turned on, turn on Other instead.
void RowEntry::checkFor(const AXIS axis)
{
	if((axis != OTHER) && (axis == whichAxis()))
		setOther();
}

// The toggle callback will make it sensitive.
void RowEntry::setOther()
{
	XmToggleButtonSetState(ow_, TRUE, TRUE);
	which_ = OTHER;
}

// Turn on button for axis.
void RowEntry::setAxis(const AXIS axis)
{ Widget w;

	switch(axis){
	case X:
		w = xw_;
		break;
	case Y:
		w = yw_;
		break;
	case Z:
		w = zw_;
		break;
	case OTHER:		// Never called for this.
		w = ow_;
		break;
	default:
		w = 0;
		break;
	}
	if(w != 0)
	{	which_ = axis;
		XmToggleButtonSetState(w, TRUE, TRUE);
	}
}

int RowEntry::getStart(const Boolean zCheck)const
{
	if((whichAxis() == Z) && zCheck)
		return 0;
	else
	if(start_ != NULL)
		return start_->getValue();
	else
		return 0;
}

int RowEntry::getLength()const
{
	if(whichAxis() == Z)
		return max_+1;
	else
	if((start_ != NULL) && (end_ != NULL))
	{ int endv = end_->getValue() - start_->getValue() + 1;
		if(endv <= 0)
			return 1;
		else
			return endv;
	}
	else
		return 1;
}

#if 0
static const char *axisName(AXIS axis)
{
	switch(axis) {
	case X:
		return "X";
		break;
	case Y:
		return "Y";
		break;
	case Z:
		return "Z";
		break;
	case OTHER:
		return "OTHER";
		break;
	default:
		return "Unknown";
		break;
	}
}
#endif

// Called when an X, Y or Z button is pressed. Ignores resets.
// Turns on/off the slider sensitivity depending on what button was pressed.
// Makes sure only one of X, Y or Z is on at a time.
// (Note: Motif 1 and Motif 2 differ in the order radio buttons get set/reset).
void RowEntry::rowToggleCB(Widget w, XtPointer client_data, XtPointer)
{ RowEntry *me = (RowEntry *)client_data;
  Boolean isSet = getToggle(w);
  AXIS axis;		// Which axis BUTTON was just pushed.
  Boolean scan;
  XtPointer ud;

	if(!isSet)
		return;		// Ignore turn offs.

	XtVaGetValues(w, XmNuserData, &ud, NULL);
	axis = (AXIS) Av::PtrToInt(ud);

#if 0
cout	<< XtName(w) << " was " << axisName(me->which_) << " new "
	<< axisName(axis) << endl;

cout << XtName(w) << (isSet ? " Set" : " Reset") << endl;
#endif

	me->which_ = axis;	// Keeps track of which button is currently
				// set.

	switch( axis) {
	case X:
	case Y:
		me->setSensitive(TRUE);
		scan = TRUE;
		break;
	case Z:
		scan = TRUE;
		me->setSensitive(FALSE);
		break;
	case OTHER:
		me->setSensitive(FALSE);
	default:
		scan = FALSE;
	}

	if(scan && (me->list_ != NULL))
		me->list_->scan(me);	// Turn off other buttons of same type.
}


// Make the slider/text area (in)sensitive.
void  RowEntry::setSensitive(const Boolean sensitive, const Boolean start)
{
#if 1
	if(start_ != NULL)
		start_->setSensitive(start);
#endif
	if(end_ != NULL)
		end_->setSensitive(sensitive);
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

RowList::RowList(const int maxLen)
{	maxLen_ = maxLen;
	next_ = 0;
	list_ = new RowEntry *[maxLen_];
	for(int i=0; i< maxLen_; i++)
		list_[i] = NULL;
	scanning_ = FALSE;
}

RowList::~RowList()
{
	for(int i=0; i< next_; i++)
		delete list_[i];
	delete list_;
}

void RowList::addEntry(RowEntry *e)
{
	if((next_ < maxLen_) && (e != NULL))
	{	list_[next_++] = e;
		e->setRowList(this);
	}
}

// Scan list and change any entries that are set the same as
// the argument. This implements the '1 of many' in the vertical
// column.
// Scan enforces the requirement that at least one of each of X, Y and Z
// must always be on.
void RowList::scan(RowEntry *e)
{AXIS axis = e->whichAxis();
 int have = axis;
 int i;

	if(scanning_)	// Ignore if called as a result of scanning.
		return;
	scanning_ = TRUE;

	if(axis == OTHER)
		return;
	for(i=0; i< next_; i++)
	{RowEntry *le = list_[i];
		if(le != e)
		{	le->checkFor(axis);
			// Keep track of existence of an X, Y and Z axis.
			have |= (le->whichAxis() & XYZ);
		}
	}

	int need = ~have & XYZ;		// Which buttons aren't set.
	i = 0;
	while((need != 0) && (i < next_))
	{RowEntry *le = list_[i++];
	 int Axis = le->whichAxis();
	 AXIS setAxis = NONE;

		if(Axis != OTHER)
			continue;
		if(need & X)
			setAxis = X;
		else
		if(need & Y)
			setAxis = Y;
		else
		if(need & Z)
			setAxis = Z;

		need &= ~setAxis;
		le->setAxis(setAxis);
	}
	scanning_ = FALSE;
}


// Change the text field readout.
void RowList::setOffset(const int offset)
{
	for(int i=0; i< next_; i++)
		list_[i]->setOffset(offset);
}


// Go through the list and build the start vector. If XYZareZero is TRUE,
// the X, Y and Z start are forced to zero, otherwise the are the value
// of the text field. (Not slider since it may not be accurate).
AvIPosition RowList::getStart(const Boolean XYZareZero)const
{AvIPosition start(next_);

	for(int i=0; i< next_; i++)
	{ RowEntry *re = list_[i];
	  AXIS which = re->whichAxis();
		if(XYZareZero && (which != OTHER))
			start(i) = 0;
		else
			start(i) = re->getStart();
	}
	return start;
}

// Go through the list and build the shape vector.
AvIPosition RowList::getShape()const
{AvIPosition shape(next_);

	for(int i=0; i< next_; i++)
	{ RowEntry *re = list_[i];
		shape(i) = re->getLength();
	}
	return shape;
}

// Go through the list and build the map vector.
AvIPosition RowList::getMap()const
{AvIPosition map(next_);
 int oi=3;	// Starting index for axes that aren't labeled X, Y or Z.

	for(int i=0; i< next_; i++)
	{ RowEntry *re = list_[i];
	  AXIS which = re->whichAxis();

		switch(which) {
		case X:
			map(0) = i;
			break;
		case Y:
			map(1) = i;
			break;
		case Z:
			map(2) = i;
			break;
		case OTHER:
			map(oi++) = i;
			break;
		default:;
		}
	}
	return map;
}

// Return the current Z starting value so it can be used as current plane #.
int RowList::currentZ()const
{
	for(int i=0; i< next_; i++)
	{ RowEntry *re = list_[i];
		if(re->whichAxis()==Z)
			return re->getStart(FALSE);
	}
	return 0;		// Error??
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//AvNDConfig::AvNDConfig(const AvString &name, Widget ref, AvAccessor *a)
AvNDConfig::AvNDConfig(AvImageDataItem::View *view, Widget ref)
{
	name_ = view->label();
	ref_ = ref;
	dialog = NULL;
	up_ = FALSE;
	view_ = view;
	list_ = new RowList(view->accessor()->rank());
}

AvNDConfig::~AvNDConfig()
{
	popdown();
	destroy(TRUE);		// Get rid of any windows.
}

void AvNDConfig::setOffset(const int offset)
{
	list_->setOffset(offset);
}


AvIPosition AvNDConfig::getStart(const Boolean XYZareZero)const
{
	return list_->getStart(XYZareZero);
}

AvIPosition AvNDConfig::getShape()const
{
	return list_->getShape();
}

AvIPosition AvNDConfig::getMap()const
{
	return list_->getMap();
}

int AvNDConfig::startingPlane()const
{
	return list_->currentZ();
}

// As long as the window is open, the label is whatever is showing in
// the text field. If it isn't open, the label is stored in name_.
const char *AvNDConfig::getLabel()const
{
	if(isUp())
	{ char *value;
		XtVaGetValues(text_, XmNvalue, &value, NULL);
		return value;
	}
	else
		return name_.chars();
}

Boolean AvNDConfig::autoDisplay()const
{
	return getToggle(autoDisplayW_);
}


/* Build the popup window.
*/
void AvNDConfig::build()
{
Widget parentW;

	{AvString name = "Setup for ";
	 char buf[128];

		name += view_->accessor()->dataset()->fileName();
		sprintf(buf, ": slice button %d", view_->index());
		name += buf;

		dialog = XtVaCreatePopupShell(name.chars(),
			topLevelShellWidgetClass, ref_,
			XmNallowShellResize, TRUE,
			NULL);	// (It isn't a dialog, the name is historical).
	}

	XtAddCallback(dialog, XmNdestroyCallback, &AvNDConfig::destroyCB,
							(XtPointer) this);

	// Container for everything.
	parentW = XtVaCreateWidget("NDRC",
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
			&AvNDConfig::doneCB,
			(XtPointer) this);

	// Container for label, text field and autodisplay toggle
	Widget textRC = XtVaCreateWidget("textRC",
			xmRowColumnWidgetClass,
			parentW,
			XmNorientation, XmHORIZONTAL,
//			XmNpacking, XmPACK_TIGHT,
//			XmNnumColumns, 1,
//			XmNisAligned, FALSE,
			NULL);

	XtVaCreateManagedWidget("Label",
				xmLabelWidgetClass,
				textRC,
				NULL);

	text_ = XtVaCreateManagedWidget("textField",
			xmTextFieldWidgetClass,
			textRC,
			XmNvalue, name_.chars(),
			XmNcolumns, 16,
			XmNblinkRate, 0,
			NULL);

	autoDisplayW_ = XtVaCreateManagedWidget(  "Autodisplay",
				xmToggleButtonWidgetClass, textRC,
				XmNset, view_->autoDisplay(),
				XmNborderWidth, BORDERWIDTH,
				XmNshadowThickness, SHADOWTHICKNESS,
				NULL);

	AvAccessor *accessor = view_->accessor();
	// Use data set instead of accessor since we want unmapped values
	// but they may be mapped if the 3rd axis has length 1.
	AvDataSet *ds = accessor->dataset();
//	int rank = accessor->rank();
	int rank = ds->rank();

	// Go through list, find the length of the longest label and use
	// that for all the labels.
	int maxlen = 0;
	for(int k=0; k< rank; k++)
	{int len = (int)strlen(ds->axisName(k));
		if(len > maxlen)
			maxlen = len;
	}

	// Which DS axis is used as current Z?
	int axis3 = view_->accessor()->getAxisMapping(2, TRUE);
	for(int i=0; i< rank; i++)
	{AXIS axis;
	 const char *name = ds->axisName(i);
	 int max = accessor->getAxisLength(i, FALSE)-1;
	 int start = 0;
		switch(i) {
		case 0:
			axis = X;
			break;
		case 1:
			axis = Y;
			break;
		case 2:			// Can't assume it's Z anymore.
			axis = OTHER;
//			axis = Z;
//			start = accessor->getPlane();
			break;
		default:
			axis = OTHER;
			start = 0;
			break;
		}
		// Flag current Z axis as default Z axis.
		if(i == axis3)
		{	start = accessor->getPlane();
			// (Start will only be non zero if set via config var).
			axis = Z;
		}

		list_->addEntry(new RowEntry(name, parentW, axis, start, max,
					     maxlen));
	}

	XtManageChild(textRC);
	XtManageChild(parentW);
}

// Popup the dialog rebuilding it first if necessary.
void AvNDConfig::popup()
{
	if(dialog == NULL)
		build();

	XtPopup(dialog, XtGrabNone);
	up_ = TRUE;
}

// Pop down the dialog.
void AvNDConfig::popdown()
{
	if((dialog != NULL) && isUp())
	{ // Get the value of the label field.
	  char *value;
		XtVaGetValues(text_, XmNvalue, &value, NULL);
		name_ = value;
		XtPopdown(dialog);
		up_ = FALSE;
	}
}

void AvNDConfig::addCallback(AvNDConfigCallback cb, XtPointer cbdata)
{	callback_ = cb;
	cbdata_ = cbdata;
}

void AvNDConfig::callCallback(const Boolean destroyed)
{
	if(callback_ != NULL)
		(callback_)(this, cbdata_, destroyed);
}

// Destroy the dialog widget if it exists.
// If called with destryWidgets = FALSE, the widget isn't destroyed, but
// dialog is still set to NULL. (It was called by destroyCB() because
// dialog was already being destroyed.
void    AvNDConfig::destroy(Boolean destroyWidgets)
{
	if(destroyWidgets && (dialog != NULL))
	{	XtRemoveCallback(dialog, XmNdestroyCallback,
				 &AvNDConfig::destroyCB,
				 (XtPointer) this);
		XtDestroyWidget(dialog);
	}
	dialog = NULL;

	// The other widgets belong to dialog.
}

// Called if the popup gets destroyed. (Probably by user).
void AvNDConfig::destroyCB(Widget, XtPointer cbdata, XtPointer)
//void AvNDConfig::destroyCB(Widget w, XtPointer cbdata, XtPointer)
{
AvNDConfig	*me = (AvNDConfig *)cbdata;

//printf("AvNDConfig::DestroyCB called for widget: %s\n", XtName(w));
	if(me != NULL)
	{	me->callCallback(TRUE);
		me->destroy(FALSE);
		me->up_ = FALSE;
	}
}

// Called when the done text toggle button is pressed.
void AvNDConfig::doneCB(Widget , XtPointer cbdata, XtPointer)
{AvNDConfig *me = (AvNDConfig *)cbdata;

	if(me != NULL)
	{	me->view_->updateConfig();
		me->popdown();
		me->callCallback(FALSE);
	}
	else
		cerr << "AvNDConfig::doneCB: NULL CB data.\n";
}
