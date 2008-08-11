//# Copyright (C) 1995-2000 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvProfileOptions.h,v 19.0 2003/07/16 05:46:22 aips2adm Exp $
//
// $Log: AvProfileOptions.h,v $
// Revision 19.0  2003/07/16 05:46:22  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:10  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:41:39  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:41:50  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:07:22  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:34:06  hravlin
// Update copyright.
//
// Revision 14.0  2000/03/23 16:07:23  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:01  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:00  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:57:23  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:50:47  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:22:50  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.2  1997/08/01 21:23:05  hr
// Changed viewport size to a slider from an intmap and added the
// gDrawAxisLabels_ variable.
//
// Revision 8.1  1997/06/23 20:01:10  hr
// Added x/yMinMax_ variables.
//
// Revision 8.0  1997/02/20 03:19:35  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/02/10 21:10:35  hr
// Removed the Removed the unused min/max marker index variables.
//
// Revision 7.2  1997/02/09 00:18:58  hr
// Cleanup and changed some doubleNotifers to integerNotifiers.
//
// Revision 7.1  1997/02/05 17:59:51  hr
// Container to hold profile setup variables.
//
//
//---------------------------------------------------------------------------

/* AvProfileOptions.h
Holds information describing how to draw a profile plot.

Holds global information and options for x, y and title labels
and x and y axis parameters and markers.

This was copied from AvAxisOptions so some of the names may look odd.
Also, many of the variables aren't used. They are still here in case we
someday add an interactive profile window so the user can see the changes
as they are made.
*/

#ifndef AvProfileOptions_h
#define AvProfileOptions_h

#include <iostream.h>
#include <AvResource.h>
#include <AvConductor.h>
#include <AvConfigInfo.h>
#include <AvNotifier.h>

class AvProfileOptions : public AvResource, public AvConductor {
  public:
	typedef AvConfigInfo::IntMap IntMap;
	AvProfileOptions(const char *filename=NULL,
			 const char *configName=NULL);

	// All these enums match up to IntMaps defined in AvProfileOptions.cc
	// Various output formats "_LS" means landscape rather than portrait.
	enum DEVICES { COLORPS, MONOPS, COLORPS_LS, MONOPS_LS};
	enum {Destroyed = AvConductor::Destroyed,
			ValueChanged=AvConductor::ValueChanged};
	// These values are used in AvProfileConfig as values for IntMap vars.
	enum {DEFAULT=-1, OFF, ON};
	enum FONTS { NORMALFONT, ROMAN, ITALIC, SCRIPT};
	// Axis position.
	enum POSITION { OFFP=OFF, NORMALPOSITION, ALTERNATE};
	// Used for Justify and side placement.
	enum PLACEMENT {TOP, BOTTOM, LEFT, RIGHT, CENTER, RIGHTP, LEFTP};
	// Where does text for a label come from?
	enum {DEFAULTLABEL=DEFAULT, TEXTLABEL};
	enum {PARALLEL, PERPENDICULAR };	// Y label.
	enum {LBLDEFEAULT=DEFAULT, DECIMAL, EXPONENTIAL};
	enum ZEROLINE {ZEROOFF=OFF, WITHTICKS, WITHOUTTICKS};
	enum POSITIONFORMAT { FRACTION, WORLD};
	enum LINESTYLE { SOLID, DASHED, DOTDASH, DOTTED, DASHDOTDOT};
	enum PLOTTYPES { NORMALPLOT, HISTOGRAM, MARKERSONLY};
	enum AXISMINMAX { DEFAULTMINMAX=DEFAULT, FLIPPED, MINFIRST, MAXFIRST};
//	// Size of viewport.
//	enum VIEWPORT { NORMALSIZE, VP75, VP50, VP25 };
	// Various profiles.
	//	LOCAL		Print the local profile only. (Default).
	//	INCLUDED	Print local profiles from included windows.
	//	WORLDPROFILE	Print profiles from included windows that
	//			match local profile's position.
//	enum PRINTPROFILE { LOCAL, INCLUDED, WORLDPROFILE};

	void	touch();		// Mark as modified & notify.

	AvConfigInfo	*config()const{return cnfg_;}

	//////////////////////////////////////////////
	enum OPTIONTYPE { XAXIS=1, YAXIS, XLABEL, YLABEL, TITLE, MARKER};

	class options {
	  public:
		options(const int typeMask);
		int typeMask()const{return typeMask_;}
	  private:
		int typeMask_;
	};

	class axisLabel : public options {
	  public:
		axisLabel(const int typeMask);
		void init(AvConfigInfo *, AvConductor *,
			  const int sideIndex=0); // entry 0 is default.
		void enableUpdate(){inited_ = 1;}
		void disableUpdate(){inited_ = 0;}
	  public:
		AvIntegerNotifier drawLabel_;	// Should label be drawn?
		AvDoubleNotifier	charScale_;
		AvIntMapNotifier	side_;
		// Value is in units of character height. Negative is inside.
		// Displacement from axis edge.
		AvDoubleNotifier	fromEdge_;
		// Value is a fraction of the length of the edge
		// Displacement along axis edge
		AvDoubleNotifier	alongEdge_;
		AvIntMapNotifier	justify_;	// Left, Center, Right.
		AvRadioTextNotifier text_;	// Label's Text.
	 private:
		int	inited_;
	};

	class axis : public options {
	  public:
		axis(const int typeMask);
		void init(AvConfigInfo *, AvConductor *);
		// Frame & Grid
		AvIntMapNotifier	drawZeroLine_;
		// Tickmarks
		AvIntMapNotifier	major_;		// Off, Default, Slider
		AvIntegerNotifier	majorIntervals_;//# of major intervals.
		AvIntMapNotifier	minor_;		// Off, Default, Slider
		AvDoubleNotifier	minorIntervals_;//# of minor intervals.
		// Axis Labels.
		AvIntMapNotifier	position_;//Off, Normal, Alternate.
		AvIntegerNotifier vertical_;	// Draw Y labels vertically.
						// Ignored for X labels.
		AvIntMapNotifier	decimal_;// Default, decimal, expon.
		void enableUpdate(){inited_ = 1;}
		void disableUpdate(){inited_ = 0;}
	  private:
		int	inited_;

	};

	class marker : public options {
	  public:
		marker();
		void init(AvConfigInfo *, AvProfileOptions *,
			  const int markerIndex);
		int			markerIndex_;
		AvIntegerNotifier	show_;
		AvDoubleNotifier	scale_;
		AvIntMapNotifier	color_;
		AvIntMapNotifier	type_;
	};

	const AvProfileOptions::axisLabel
			*getLabelOptions(const OPTIONTYPE axis)const;
	const AvProfileOptions::axis
			*getAxisOptions(const OPTIONTYPE axis)const;
	const AvProfileOptions::marker
			*getMarkerOptions(const int markerNumber)const;
	int numMarkers()const {return numMarkers_;}
  protected:
	virtual ~AvProfileOptions();	// Use ref/unref.
  private:
	void init(const char *fileName, const char *configName);// Initialize.
  private:
	AvConfigInfo	*cnfg_;
//  private:	// All Notifiers are public.
  public:
	////////////////////////////////////////////////
	// AvProfileConfig and drawProfile directly access these.

	// 'Global' parameters.
	AvIntMapNotifier	gPSDevice_;
	AvIntegerNotifier	gDrawLabels_;
	AvIntMapNotifier	gFont_;
	AvIntegerNotifier	gLineWidth_;
	AvIntMapNotifier	gLineStyle_;
	AvIntMapNotifier	gPlotType_;
	AvDoubleNotifier	gCharScale_;
	AvDoubleNotifier	gAxisCharScale_;
	AvIntegerNotifier	gDrawFrame_;
	AvIntegerNotifier	gDrawGrid_;
	AvIntegerNotifier	gDrawAxisLabels_;
	AvIntMapNotifier	gAxisColor_;
	AvIntMapNotifier	gProfileColor_;
//	AvIntMapNotifier	gViewportSize_;
	AvDoubleNotifier	gViewportSize_;
//	AvIntMapNotifier	gProfiles_;
	AvIntMapNotifier	gDrawZeroLine_;
	AvIntMapNotifier	xMinMax_;
	AvIntMapNotifier	yMinMax_;

	// Per axis, per label options.
	axisLabel	xLabel_;
	axisLabel	yLabel_;
	axisLabel	title_;
//	axis		xAxis_;
//	axis		yAxis_;
	////////////////////////////////////////////////
	int		numMarkers_;
	marker		*markers_;
//	AvIntegerNotifier	gMinMarkerIndex_;
//	AvIntegerNotifier	gMaxMarkerIndex_;
	// Toggle buttons representing differ marker types.
	int			gNumMarkerToggleTypes_;
	IntMap			*gMarkerToggleTypes_;
  private:
	
};
#endif
