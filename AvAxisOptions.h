//# Copyright (C) 1995-98 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvAxisOptions.h,v 19.0 2003/07/16 05:46:59 aips2adm Exp $
//
// $Log: AvAxisOptions.h,v $
// Revision 19.0  2003/07/16 05:46:59  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:43  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:12  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:20  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:28  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:02  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:47  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:16  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:14  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:59  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.2  1998/02/20 20:45:15  hr
// Changes to support different label positioning.
//
// Revision 9.1  1997/11/17 16:56:42  hr
// Added support for axis offsets and captions.
//
// Revision 9.0  1997/08/25 21:26:18  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.3  1997/05/22 20:32:54  hr
// Added virtual destructors for options and its subclasses.
//
// Revision 8.2  1997/04/25 16:05:13  hr
// Converted to using AvBooleanNotifiers. Added useGlobalColor variable
// objects.
//
// Revision 8.1  1997/04/23 17:14:30  hr
// Changes to support options for drawing curvilinear axes.
//
// Revision 8.0  1997/02/20 03:19:26  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/02/10 21:05:47  hr
// Marker type selector switched from slider to toggles.
//
// Revision 7.2  1997/02/09 00:07:01  hr
// Changed from internally defined Notifiers to AvNotifiers.
//
// Revision 7.1  1997/01/24 20:42:52  hr
// Object to hold axis configuration parameters.
//
//
//---------------------------------------------------------------------------

/* AvAxisOptions.h
Holds information describing how to draw axes.

Holds global information and options for x, y and title labels
and x and y axis parameters and markers.

*/

#ifndef AvAxisOptions_h
#define AvAxisOptions_h

#include "iostream.h"
#include <AvResource.h>
#include <AvConductor.h>
#include <AvNotifier.h>
#include <AvConfigInfo.h>


class AvAxisOptions : public AvResource, public AvConductor {
  public:
	AvAxisOptions(const char *filename=NULL, const char *configName=NULL);

	enum {Destroyed = AvConductor::Destroyed,
			ValueChanged=AvConductor::ValueChanged};
	// These values are used in AvAxisConfig as values for IntMap vars.
	enum {DEFAULT=-1, OFF, ON};
	enum FONTS { NORMALFONT, ROMAN, ITALIC, SCRIPT};
	enum AXISTYPE {LINEAR, LINEAROFFSET, CURVILINEAR, ABSPIXEL};
	enum AXISFORMAT { DEFAULTAXISFORMAT=DEFAULT, PLAIN,
		ANGLE_0_360, ANGLE_M180_180, ANGLE_M180_180U,
		ANGLE_DMS_360, ANGLE_DMS_180, ANGLE_DMS_360U /* 'F' */,
		ANGLE_HMS_24, ANGLE_HMS_12, ANGLE_HMS_12U /*'I' */,
		/* LOG, */ TIME_HMS};

	enum LINEARFORMAT {DEGREES, ARCMIN, ARCSEC, MARCSEC, RELPIXEL};
	// Axis position.
	enum POSITION { OFFP=OFF, NORMALPOSITION, ALTERNATE};
	// Used for Justify and side placement.
	enum PLACEMENT {TOP, BOTTOM, LEFT, RIGHT, CENTER, RIGHTP, LEFTP};
	// Where does text for a label come from?
	enum {DEFAULTLABEL=DEFAULT, TEXTLABEL};
	enum {PARALLEL, PERPENDICULAR };	// Y label.
	enum {LBLDEFEAULT=DEFAULT, DECIMAL, EXPONENTIAL};
//	enum COLORS{ BLACK, WHITE, RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW};
	enum ZEROLINE {ZEROOFF=OFF, WITHTICKS, WITHOUTTICKS};
//	enum POSITIONFORMAT { FRACTION, WORLD};
	unsigned long timestamp()const {return timestamp_;}
	Boolean modifiedSince(const unsigned long ts)const
		{ return (timestamp_ > ts) ? TRUE : FALSE;}
	void	touch();		// Mark as modified & notify.
	AvConfigInfo	*config()const{return cnfg_;}
	//////////////////////////////////////////////
	enum OPTIONTYPE { XAXIS=1, YAXIS, XLABEL, YLABEL, TITLE, MARKER,
			  CAPTION};

	class options {
	  public:
		options(const int typeMask);
		virtual ~options();
		int typeMask()const{return typeMask_;}
	  private:
		int typeMask_;
	};

	class axisLabel : public options {
	  public:
		axisLabel(const int typeMask);
		axisLabel();
		virtual ~axisLabel();
		void init(AvConfigInfo *, AvConductor *,
			  // 0 is default side, 1 is alternate.
			  const int sideIndex=0,
			  const int index=-1); // Only captions are >= 0.
//		void enableUpdate(){inited_ = 1;}
//		void disableUpdate(){inited_ = 0;}
	  public:
		AvBooleanNotifier	drawLabel_;   // Should label be drawn?
		AvDoubleNotifier	charScale_;
		AvIntMapNotifier	side_;		//
#if 0
		// Value is in units of character height. Negative is inside.
		AvDoubleNotifier	fromEdge_;	// Displacement
							// from axis edge.
		// Value is a fraction of the length of the edge
		AvDoubleNotifier	alongEdge_;	// Displacement
							// along axis edge
#else
		AvDoubleNotifier	x_;
		AvDoubleNotifier	y_;
		AvDoubleNotifier	angle_;
#endif
		AvIntMapNotifier	justify_;	// Left, Center, Right.
		AvRadioTextNotifier	text_;		// Label's Text.
		AvBooleanNotifier	useGlobalColor_;
		AvIntMapNotifier	color_;
		AvBooleanNotifier	opaqueBackground_;
	 private:
		int	index_;
	};

	class axis : public options {
	  public:
		axis(const int typeMask);
		virtual ~axis();
		void init(AvConfigInfo *, AvConductor *);
		// Some of the options pertain to both curvilinear and linear
		// formats. Some to one or the other.
		// Frame & Grid
		AvIntMapNotifier	drawZeroLine_;
		// Tickmarks
		AvIntMapNotifier	linMajor_;	// Off, Default, Slider
		//# of major intervals.
		AvIntegerNotifier	linMajorIntervals_;
		AvIntMapNotifier	minor_;		// Off, Default, Slider
		//# of minor intervals.
		AvIntegerNotifier	linMinorIntervals_;
		// Axis Labels.
		AvIntMapNotifier	position_; // Off, Normal, Alternate.
		AvBooleanNotifier	vertical_; // Draw Y labels vertically.
						   // Ignored for X labels.
		AvIntMapNotifier	decimal_; // Default, decimal, expon.
		///////// Curvilinear
		AvBooleanNotifier	useGlobalColor_;
		AvIntMapNotifier	color_;
		//# of major intervals.
		AvIntegerNotifier	crvMajorIntervals_;
		AvIntMapNotifier	crvMajor_;	// Off, Default, Slider
		// Whether the grid label is same color as the grid.
		AvBooleanNotifier	labelTracksGrid_;
		AvIntMapNotifier	format_;  // DMS, HMS, etc.
		// Grid labels can be in all four places.
		AvBooleanNotifier	top_,bottom_,left_,right_;
	  private:

	};

	class marker : public options {
	  public:
		marker();
		virtual ~marker();
		void init(AvConfigInfo *, AvAxisOptions *,
			  const int markerIndex);
		int		markerIndex_;
		AvIntegerNotifier	show_;
		AvDoubleNotifier	scale_;
		AvIntMapNotifier	color_;
		AvIntMapNotifier	type_;
		AvDoubleNotifier	x_;
		AvDoubleNotifier	y_;
	};

	const AvAxisOptions::axisLabel
			*getLabelOptions(const OPTIONTYPE axis)const;
	const AvAxisOptions::axis
			*getAxisOptions(const OPTIONTYPE axis)const;
	const AvAxisOptions::marker
			*getMarkerOptions(const int markerNumber)const;
	const AvAxisOptions::axisLabel
			*getCaptionOptions(const int captionNumber)const;
	int numMarkers()const {return numMarkers_;}
	int numCaptions()const {return numCaptions_;}
  protected:
	virtual ~AvAxisOptions();	// Use ref/unref.
  private:
	void init(const char *fileName, const char *configName);// Initialize.
  private:
	AvConfigInfo	*cnfg_;
//  private:
  public:
	////////////////////////////////////////////////
	// AvAxisConfig and Axis directly access these.

	// 'Global' parameters to be used unless overridden.
	AvBooleanNotifier	gDrawLabels_;
	AvIntMapNotifier	gFont_;
	AvIntMapNotifier	gAxisType_;
	AvIntegerNotifier	gLineWidth_;
	AvDoubleNotifier	gCharScale_;
	AvBooleanNotifier	gDrawFrame_;
	AvBooleanNotifier	gDrawGrid_;
	AvIntMapNotifier	gAxisColor_;
	// In Linear offset mode, gAxisOffsets is converted to world positions
	// and used as the axis origin.
	AvIntegerArrayNotifier	gAxisOffsets_;	// Cursor position in IJK.
	// How to display axes when doing offsets.
	AvIntMapNotifier	gLinearAxisFormat_;
	// If this is TRUE (default), only angular positions (RA, DEC, etc.)
	// are displayed as offsets. Otherwise, everything (eg. Velocity) is.
	AvBooleanNotifier	gAngularOnly_;
	// Per axis, per label options.
	axisLabel	xLabel_;
	axisLabel	yLabel_;
	axisLabel	title_;
	axis		xAxis_;
	axis		yAxis_;
	////////////////////////////////////////////////
	int		numCaptions_;
	axisLabel	*captions_;
	////////////////////////////////////////////////
	int		numMarkers_;
	marker		*markers_;
	////////////////////////////////////////////////
	// Toggle buttons representing different marker types.
	int		gNumMarkerToggleTypes_;
	IntMap		*gMarkerToggleTypes_;

  private:
	
	unsigned long timestamp_;
};
#endif
