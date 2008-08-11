//# Copyright (C) 1995-99 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvProfile.h,v 19.0 2003/07/16 05:48:08 aips2adm Exp $
//
// $Log: AvProfile.h,v $
// Revision 19.0  2003/07/16 05:48:08  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:42  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:43:10  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:16  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:48  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:09:14  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:41:06  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:34  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.1  1999/05/21 20:27:14  hravlin
// Added aipsview copyright.
//
//
//---------------------------------------------------------------------------
/* AvProfile.h
Display a profile in either an X window or PGPLOT canvas.
*/
#ifndef AvProfile_h
#define AvProfile_h

#include <Xm/Xm.h>
#include <X11/Intrinsic.h>
#include "AvProfileOptions.h"
#include "AvAccessor.h"
#include "AvWPosition.h"
#include "AvPosition.h"

class AvProfileOptions;
class AvXPalette;
class AvPGDriver;
class AvPGCanvas;
class AvXWindow;
class AvViewportSetter;
class AvImageViewPanel;
class AvImageWindow;
class AvImageViewItem;

class AvProfile {
  public:
	typedef unsigned long Serial;
  protected:
	// A lot of information can be shared between paired X & PGPLOT
	// profile objects.
	class ProfileData : public AvResource {
	 public:
		ProfileData(AvAccessor *, AvProfileOptions *opts);
		~ProfileData();
		// Set the X/Y position of the profile.
		// If a value is < 0, it is ignored.
		void setXY(const int x, const int y);
		void getXY( int &x, int &y)const;
		void getFrozenXY( int &x, int &y)const;
		// Each time X &/or Y is updated a counter is incremented.
		Serial xSerial()const{return xSerial_;}
		Serial ySerial()const{return ySerial_;}
		void incXSerial(){xSerial_++;}
		void incYSerial(){ySerial_++;}
		// Which axis to use (0..2) for profiles, Default is 2
		// (along the view axis).
		void	setProfileAxis(const int axis);
		Serial axisSerial()const{return axisSerial_;}
		int getProfileAxis()const{return axis_;}

		void	setSlice(const int slice);
		inline int numPoints()const
				{return accessor_->getAxisLength(axis_);}
		AvAccessor	*accessor()const{return accessor_;}
		// Create a filename based on position and labels.
		AvString ofileName()const;
		const char *fileName()const{return fileName_.chars();}
		double xDmin()const{return xDmin_;}
		double xDmax()const{return xDmax_;}
		// y min/max is accessor->clipMin/Max.
		int getProfile( const int x, const int y, const int axis,
				AvWPosition &data)const;
		// Get profile using current position and axis.
		int getProfile(AvWPosition &data)const;

		void getXDataMinMax(double &xDataMin, double &xDataMax,
				    double &xScl, double &repvalue,
				    AvString &xUnits,
		 		    AvString &xUnitsAbbrev)const;
		void setXDataScale();		// Compute/store x scale.
		double getXDataScale()const {return xScale_;}
		const char *xUnits()const{return xUnits_.chars();}
		Boolean frozen()const{return freezeProfile_;}
		void freeze(const Boolean freeze);
		AvAccessor	*accessor_;
		// Filename w/o path or extension.
		AvString	fileName_;
		// Used to initialize labels in options.
		void getDefaultTitle(AvString &plotTitle )const;
		void getDefaultYLabel(AvString &yAxisLabel )const;
		void getDefaultXLabel(AvString &xAxisLabel )const;
		// Used when dealing with 'included' windows.
		AvImageWindow *imageWindow()const;
		void setViewItem(AvImageViewItem *i){vItem_=i;}
	  protected:
		int xAxis()const{return 0;}	// X axis is 0, Y is 1, ...
		int yAxis()const{return 1;}
		int zAxis()const{return 2;}
	  private:
		int		x_, y_, axis_;
		int		xf_, yf_;	// 'frozen' x & y.
		Serial		xSerial_, ySerial_, axisSerial_;
		AvString	xUnits_;
		double		xDmin_, xDmax_;
		double		xScale_;
		Boolean		freezeProfile_; // If true, use current data
						// when drawing.
		AvImageViewItem	*vItem_;
		// Called by accessorCB_ to handle accessor changes.
		void accessorCB(AvConductor *c, XtPointer uData);
	static void accessorCB_(XtPointer me, AvConductor *c, XtPointer uData);

	};

  public:
	AvProfile(Widget parent, AvAccessor *a, AvProfileOptions *o);
	AvProfile(Widget parent, ProfileData *d, AvProfileOptions *o);
	virtual ~AvProfile();
	// Draw profile using currently set parameters.
	void drawProfile();
	// Redraw defaults to calling drawProfile, but may be overridden
	// if it can be made quicker.
	virtual void redrawProfile();	// Redraw using current values.
	// Get profile information. Draw profile.
	void drawProfileAt(const int x, const int y);
	// Draw profile at position using the supplied profile's drawing
	// routines.
	// Only the Z axis value is used in 'to'.
	void drawProfileAtUsing(const AvPosition &from,
				const AvPosition &to, AvProfile *);
	// Set/Get the X/Y position of the profile.
	void setXY(const int x, const int y);
	void getXY( int &x, int &y)const;
	AvAccessor	*accessor()const{return data_->accessor();}
	virtual	void open();
	virtual	void close();
	// Draw only the profile line.
	virtual void drawProfileOnly(const AvWPosition &x,
				     const AvWPosition &y);
	// Draw only the profile line using scaling and device information
	// from 'profile', but options (eg. color, line width) from this.
	// Used for implementing overlayed lines.
	virtual void drawProfileOnlyUsing(const AvWPosition &x,
					  const AvWPosition &y,
					  AvProfile *profile);
	// As above, but use current values.
	virtual void drawOnlyProfileUsing(AvProfile *profile);
	void drawIncludedProfiles();
	virtual void  drawBoxAndLabels();

	// Postscript output.
	virtual void printProfile(const char *fileName);
	// ASCII output.
	void dumpProfile();
	short  winWidth()const{return winWidth_;}	// Window size.
	short  winHeight()const{return winHeight_;}
	AvProfileOptions *options()const{return options_;}
	Boolean showIncluded()const {return showIncluded_;}
	void showIncluded(const Boolean show);
	void show(const Boolean);	// Show/hide widget.
	Boolean isUp()const;	// Is main widget managed?
	int xAxis()const{return 0;}	// X axis is 0, Y is 1, ...
	int yAxis()const{return 1;}
	int zAxis()const{return 2;}
	Boolean haveOptions()const{return haveOptions_;}// Are options current?
	Widget	profileWidget()const{return profileW_;}
	ProfileData *profileData()const{return data_;}
	// Retrieve X & Y values.
	void getXData();	// Fill X axis array.
	void getYData();	// Fill Y axis array.
	// Fill X & Y axis arrays.
	virtual void getXData(AvWPosition &d)const;
	virtual void getYData(AvWPosition &d)const;
	inline Boolean haveYData()const{return (ySerial_ == data_->ySerial());}
	inline Boolean haveXData()const{return (xSerial_ == data_->xSerial());}
	inline Boolean dataChanged()const {return !haveOptions()
					    || !haveXData() || !haveYData();}
	inline Boolean freezeProfile()const{return data_->frozen();}
	void freezeProfile(const Boolean );
  protected:
	////////////////////////////////////////////////
	//			Profile data.
	////////////////////////////////////////////////

	virtual void	updateOptions();// Update color, etc.
	// Returns TRUE if widget exists and is realized.
	inline	Boolean validWidget(Widget w)const
		{return ((w!=NULL) && XtIsRealized(w)) ? TRUE : FALSE;}
	// Called when start or stop plane changes.
//	void regionChanged();
	void setProfileWidget(Widget );	// Widget to be used for drawing.
	// return surrounding widget. (NULL for PGPLOT, a Form for X).
	virtual Widget mainWidget()const;
  private:	// Variables.
	ProfileData	*data_;
	AvWPosition	xdata_;		// X axis data.
	AvWPosition	ydata_;		// Y axis data.
	// Used to track X/Y changes.
	Serial		xSerial_;
	Serial		ySerial_;
	Serial		axisSerial_;
	AvProfileOptions *options_;	// Line style, etc.
	Boolean		haveOptions_;	// FALSE if options need updating.
					// (info_ has changed).
	Boolean		showIncluded_;
	Widget		profileW_;	// Where to draw profile.
	Widget		parentW_;	// The parent passed to this.
	short		winWidth_,	// Window size.
			winHeight_;
	///////
  private:
	void	init(Widget parent, ProfileData *d, AvProfileOptions *o);
	// XProfiles return the XProfile, PGProfiles return the PGProfile.
	// Used when drawing included profiles.
	virtual AvProfile *getIVPProfile(AvImageViewPanel *)const;

	// Called by widgetChangedCB when widget gets resized.
	virtual void widgetChanged(Widget w, XmDrawingAreaCallbackStruct *);
	// There was input on the widget.
	virtual void widgetInput(Widget w, XmDrawingAreaCallbackStruct *);
	// Called by accessorCB_ to handle accessor changes.
	void accessorCB(AvConductor *c, XtPointer uData);
	// Before accessorCB redraws the profile, it calls this for profile
	// specific updates.
	virtual void accessorCB2(const int reason, XtPointer uData);
	// ************************************************************
	// Callbacks.
	// expose or resize events.
	static void widgetChangedCB(Widget, XtPointer, XtPointer);
	static void redrawCB(Widget, AvProfile *, XtPointer);
	// Called when mouse is moved in profile window.
	static void profileCB(Widget, AvProfile *, XtPointer);
	static void optionsInfoChangedCB(XtPointer o, AvConductor *,
					 XtPointer uData);
	static void accessorCB_(XtPointer me, AvConductor *c, XtPointer uData);
};

class AvXProfile : public AvProfile {
  public:
	AvXProfile(const char *name, Widget parent, AvAccessor *a,
		   AvProfileOptions *, AvXPalette *);
	AvXProfile(const char *name, Widget parent, AvProfile::ProfileData *d,
		   AvProfileOptions *, AvXPalette *);
	virtual ~AvXProfile();
	virtual Widget mainWidget()const{return mainWidget_;}
	enum PLOTTYPE { LINES, DOTS};
  private:
	void xinit(const char *name, Widget parent, AvXPalette *);

	Widget buildProfile(Widget parent);
	virtual	void open();
	// Display profile using local color, lw, etc and profile's
	// scaling and output.
	virtual void drawProfileOnlyUsing(const AvWPosition &x,
					  const AvWPosition &y,
					  AvProfile *profile);
	virtual void  drawBoxAndLabels();
	void	updateProfileXLabels();
	void	updateProfileYLabels();
	void	setLabelString(Widget w, const char *str);
	virtual void	updateOptions();// Update color, etc.
	// Draw an array of XPoints as either lines or points.
	void	drawXPoints(Widget w, XPoint *points, const int len,
			    const Boolean drawPoints=FALSE);
	// Called by AvProfile::accessorCB.
	virtual void accessorCB2(const int reason, XtPointer uData);
	// Operations on local X profile window.
	void	clearWindow();
	void	drawSliceLine();	// Draw current slice indicator line.
	void	drawZeroLine();		// Draw Y=0 zero line.
	// Get X color index from options.
	unsigned long colorIndex()const;
	// Returns TRUE if widget exists and is realized.
	inline	Boolean validWidget(Widget w)const
		{return ((w!=NULL) && XtIsRealized(w)) ? TRUE : FALSE;}
	virtual AvProfile *getIVPProfile(AvImageViewPanel *)const;
	short	winWidth_, winHeight_; // Size of window.

	virtual void widgetInput(Widget w, XmDrawingAreaCallbackStruct *);
	GC	linesGC_;
	GC	boxGC_;
	AvXPalette *palette_;
	Boolean	showZeroLine_;
	Boolean	haveXLabels_;
	Boolean	haveYLabels_;
	int	xMinMax_;	// Whether to flip axis or not.
	int	yMinMax_;
	int	plotType_;
	Widget	profileYMaxLabel_;
	Widget	profileXMaxLabel_;
	Widget	profileXMinLabel_;
	Widget	profileYMinLabel_;
	Widget	mainWidget_;		// Enclosing widget.
};

#ifdef PGPLOT
class AvPGProfile : public AvProfile {
  public:
	AvPGProfile(Widget parent, AvAccessor *a, AvProfileOptions *o,
		    AvXPalette *pal);
	AvPGProfile(Widget parent, AvProfile::ProfileData *d,
		    AvProfileOptions *o, AvXPalette *pal);
	virtual ~AvPGProfile();
	// Open/close profile device
	virtual	void open();
	virtual	void close();
	virtual void drawProfileOnlyUsing(const AvWPosition &x,
					  const AvWPosition &y,
					  AvProfile *profile);
	virtual void  drawBoxAndLabels();
	virtual void printProfile(const char *fileName);
//	virtual void redrawProfile();
  private:	// Variables.
	Boolean		hardCopy_;
	AvXWindow	*xCanvas_;
	AvPGCanvas	*pgCanvas_;
	AvPGDriver	*driver_;
	AvViewportSetter *vps_;
	Boolean		haveProfile_;	// TRUE if display list is valid.

	void pginit(Widget parent, AvXPalette *pal);

	Boolean openProfileDevice(const Boolean hardCopy);
	void closeProfileDevice();
	// Print X/Y/Z labels.
	void printLabel(const AvProfileOptions::axisLabel *Label,
			const float charScale);
	virtual AvProfile *getIVPProfile(AvImageViewPanel *)const;
};
#endif

#endif
