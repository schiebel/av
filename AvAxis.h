//# Copyright (C) 1995-2002 Board of Trustees of the University of Illinois
//#
//# This software, both binary and source, is copyrighted by The
//# Board of Trustees of the University of Illinois.  Ownership
//# remains with the University.  You should have received a copy 
//# of a licensing agreement with this software.  See the file 
//# "AIPSVIEW_COPYRIGHT", or contact the University at this address:
//#
//# 	The NCSA AipsView Visualization System
//# 	National Center for Supercomputing Applications
//# 	University of Illinois
//# 	405 North Mathews Ave.
//# 	Urbana, IL 61801
//# 	aipsview@ncsa.uiuc.edu
//---------------------------------------------------------------------------
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvAxis.h,v 19.0 2003/07/16 05:48:07 aips2adm Exp $
//
// $Log: AvAxis.h,v $
// Revision 19.0  2003/07/16 05:48:07  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:41  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/07 22:26:28  hravlin
// Removed check for __P.
//
// Revision 17.0  2001/11/12 19:43:09  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:15  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:46  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:09:13  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:41:04  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:31  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:02:26  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:56:43  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.3  1998/06/09 21:09:29  hr
// Removed unneeded semicolon from DECLARE_HEADER.
//
// Revision 9.2  1998/02/20 20:33:22  hr
// Changed label handling to use position and angle controls rather than
// old method.
//
// Revision 9.1  1997/11/17 16:54:14  hr
// Added support for axis offsets and captions.
//
// Revision 9.0  1997/08/25 21:32:05  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.1  1997/04/23 17:11:01  hr
// Changes to support use of pgcrvl including globalOptions.
//
// Revision 8.0  1997/02/20 03:14:27  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.6  1997/02/11 20:38:12  hr
// Added a pointer to the raster.
//
// Revision 7.5  1997/02/05 17:22:29  hr
// The __P problem with g++ and math.h.
//
// Revision 7.4  1997/01/24 20:37:54  hr
// Widespread changes to convert to using AvAxisOptions for settable parameters.
//
// Revision 7.3  1996/12/12 09:18:33  droberts
// added HMS_RAD and DMS_RAD types to support radian datasets.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.5  1996/06/18  18:42:36  pixton
//  Copyright Update
//
//  Revision 1.4  1996/02/23  17:37:31  hr
//  Several changes to cleanup, to incorporate code from AvCLUImageSetter
//  and to help display text and tick marks better.
//
//  Revision 1.3  1996/02/08  18:41:04  hr
//  Added charSize().
//
//  Revision 1.2  1996/01/25  17:43:17  hr
//  Needed to add Graphics2D initializations.
//
//  Revision 1.1  1996/01/24  18:05:51  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvAxis.h

Subclass of AvPolyline that generates a labeled axis.

The drawing area for the axis information is greater than for the raster
so this assumes the raster is contained within a smaller viewport that
is offset from the one into which the axis information will be drawn.
The surrounding viewport is assumed to be the size of the raster's viewport
plus margins.

	VPS	->VPR(at xoffset, yoffset)	->Raster
		->Axis: using xoffset, yoffset

VPS is the enclosing viewport containing VPR and the AvAxis object.

VPR should be a viewportsetter containing the raster and should be offset
from VPS' origin by the given offsets.

--------------
Example:

	static const int OFFSET=150;
	vpr = new AvViewportSetter(OFFSET, OFFSET, raster);

	// Viewport to enclose raster's vp and have room for grid & labels.
	vps = new AvViewportSetter(0.0, 0.0,
					width+OFFSET*2, height+OFFSET*2, vpr);
	// (width, height are the dimensions of the scaled raster).
	AvAxis *axis = new AvAxis(raster, axisoptions);
	// The box will be drawn around a box with these margins
	// (in pixels) from the surrounding viewport.
	// If lm = rm and tm = bm, then the following can be used:
	axis->offset(OFFSET, OFFSET);
	// If the margins are different, use:
		axis->setMargins(lm, rm, tm, bm);

	vps->append( axis);

	// If a raster is not used, it will be necessary to make calls to
	// these routines:
	xLabel(), yLabel(), title() and setWorldWindow().

--------------

Various options to change the appearance of the plot may be set.
These are set via the AvAxisOptions object.

Option		Function			Default
GRID		Draw grid line.			Yes
LOG		Logarithmic axis.		No
MAJORTICKS	Draw Major tick marks		Yes
MINORTICKS	Draw Minor tick marks		Yes
VERTLABELS	Draw Y axis labels vertically	Yes
		rather than parallel to Y axis.
STDLABELPOSITION
		Draw labels in 'standard'	Yes
		position.
NONSTDLABELPOSITION
		Draw labels in 'nonstandard'	No
		position.
		(Don't set both of the above).
SHOWZEROLINE
		Show the zero line.		No
HMS		Use hours minutes seconds	No	(Print as 'floats').
		labeling.
DMS		Use degrees minutes seconds	No
		labeling.
		(Don't set both of the above).
*/

#ifndef AvAxis_h
#define AvAxis_h
//#if defined(__GNUC__) && defined(__P) && !defined(__linux__)
//#undef __P
//#endif
#include <math.h>
#include "AvPolyline.h"
#include "AvAxisOptions.h"

class AvRaster;
class AxisGlobalOptions;

class AvAxis : public AvPolyline {
  public:
	// The raster is used to initialize labels and label formats.
	// If NULL, they will need to be set separately.
	AvAxis(AvRaster * = NULL, AvAxisOptions * = NULL);
	// Use a different options var.
	void setOptions(AvAxisOptions *o);
	AvAxisOptions *getOptions()const{return options_;}

	// Use the raster to set labels and axis types.
	void useRasterInfo(AvRaster *);
	//		Set X/Y label, title.
	void xLabel(const char *);
	void yLabel(const char *);
	void title(const char *);
	// Offsets within viewport to start drawing. (Pixels).
	void setMargins(const int lm, const int rm,
			const int tm, const int bm);
	// Different axis options. (Used internally).
	enum OPTIONS {	NOOPTIONS=0, 
			// Axis, grid, axis labels.
			DECIMAL=1, EXPONENTIAL=2, // If neither, use default.
			FRAME=0x10, GRID=0x20, HMS=0x40, DMS=0x80,
			STDLABELPOSITION=0x100, NONSTDLABELPOSITION=0x200,
			NOAXISLABEL=STDLABELPOSITION|NONSTDLABELPOSITION,
			VERTYLABELS=0x400,
			ZEROLINENOTICKS=0x1000, ZEROLINEWITHTICKS=0x2000,
			ZEROLINE = ZEROLINENOTICKS|ZEROLINEWITHTICKS,
			// Labels
			SHOWLABEL=0x10000,
			MAJORTICKS=0x100000, USERMAJORINTERVALS=0x200000,
			MINORTICKS=0x400000, USERMINORINTERVALS=0x800000
			};

	// AXISTYPE gives the input and output formats.
	enum AXISFORMAT { FLOAT=0, HOURS, DEGREES, RADIANS};
	// Size of window in world coordinates.
	void setWorldWindow(const float x0, const float x1,
			    const float y0, const float y1);
	void getWorldWindow(const int xAxisInput, float &x0, float &x1,
			    const int yAxisInput, float &y0, float &y1)const;

	// Desired max size in pixels for characters. (rough approximation).
	void maxCharSize(const float size);
	float maxCharSize()const { return charsize_;}
	virtual void draw(AvCanvas &);
	virtual void draw(AvCanvas *);
	// Returns TRUE if the vector list exists.
	inline Boolean haveAxis()const{return haveAxis_;}
	// Get rid of current Axis. (clears haveAxis_).
	virtual void touch();
	/* Compute the (approximate) scalefactor needed to fit n chars into
	  space # of pixels. If vertical is FALSE, scale is for
	  horizontal drawing, else vertical. The space needed for a
	  character string is:
		#chars * charsize * charscale * viewportsize

	   The viewport size is needed since characters are drawn with
	  fractional vectors: increasing the viewport size increases
	  the size of the characters.
	  nchrs	# of characters to draw.
	  space	# of pixels to draw into
	  vpsize Size of drawing surface (height for vertical=TRUE,
		 width otherwise).
	  vertical Whether text will be drawn vertically.
	*/
	float computeCharScale( const float nchrs, const float space,
			const int vpsize, const Boolean vertical);
	void showAxis(const Boolean);
	Boolean showAxis()const;
	// PGPLOT's pgrnd.
	static double pgrnd(double x, int &nsub);
	static inline float hours2sec(const float hrs){return hrs *3600;}
	//[ (1day/360deg) * (24hrs/day) * (3600sec/hr) = 240.0sec/deg].
	static inline float degrees2sec(const float deg){return deg *240.0;}
	// 360deg/2PIradians = 57.2957795130823208768463.
	static inline float radians2deg(const float r)
					{return (float)(r*57.29577951308232);}
	static inline float radians2sec(const float r)
					{return degrees2sec(radians2deg(r));}

	// Given a value in world coordinates, convert it to what PGPLOT
	// expects ('Time' related values in seconds).
#ifndef __sgi
	inline double worldX2PG(const double value)
		{ switch (xAxisInput_) {
			case DEGREES:
				return degrees2sec(value);
			case HOURS:
				return hours2sec(value);
			case RADIANS:
				return radians2sec(value);
			default:
				return value;
			}
		}
	inline double worldY2PG(const double value)
		{ switch (yAxisInput_) {
			case DEGREES:
				return degrees2sec(value);
			case HOURS:
				return hours2sec(value);
			case RADIANS:
				return radians2sec(value);
			default:
				return value;
			}
		}
#else
	//  The sgi compiler under 5.3 gives a
	//"number outside range for double precision floating point values"
	// warning in the assemble when compiled with -g and inlined.
	double worldX2PG(const double value);
	double worldY2PG(const double value);
#endif

	// Global options are shared between AvAxis objects of the same
	// data set and view axis.
	AxisGlobalOptions *globalOptions()const{return goptions_;}
	void globalOptions(AxisGlobalOptions *opt);
	static AxisGlobalOptions *makeGlobalOptions(AvRaster *, const int s=0);
  protected:
	virtual ~AvAxis();
	// Return character scale needed for the box. Uses charSize if set
	// or else computes a scale factor for estimated label sizes.
	// If there is no space, for a label in the requested position,
	// the corresponding option will be turned off.
	float getBoxCharScale(  const int vpwidth, const int xchrs,
				int &xoptions, int &yoptions);
  private:
	void initToZero();
	void generate(AvCanvas *c);	// Generate the Axis vector list.
	void drawLinearAxis(AvCanvas *c, const int xMask, const int yMask,
			    const int lineColor);
	void drawCurviLinearAxis(const int xMask, const int yMask,
			    const int lineColor);
	char *strDup(const char *str)const;
	int getGlobalOptionMask(AvAxisOptions *options)const;
	int getAxisOptionMask(const AvAxisOptions::axis *axis,
			      const AXISFORMAT af,
			      const Boolean useCurvilinearAxis)const;

	// Convert option var to PGPLOT option string.
	// Return a pointer to the buffer.
	char	*makeOptionString(const int o, char *buf)const;
	// As above, but only turns on grid, frame and tickmarks.
	char	*makeLineOptionString(const int o, char *buf)const;

	void	drawLabel(const AvAxisOptions::axisLabel *Label,
			  float charScale);
	void	drawMarker(const AvAxisOptions::marker *Marker,
			   float charScale, const float x0, const float x1,
			   const float y0, const float y1);
  private:
	DECLARE_HEADER
	Boolean	haveAxis_;		// Set when the axis has been generated
	float	xwin0_, xwin1_;		// World coordinates of window...
	float	ywin0_, ywin1_;		// Set in useRasterWindow.
	float	Xwin0_, Xwin1_;		// What gets used. Either xwin0_, etc.
	float	Ywin0_, Ywin1_;		// Or the 'offset' version.
	Boolean showOffsets_;		// If true, show delta axis.
					// Otherwise, standard.
	int	lm_, bm_, tm_, rm_;	// Margins within viewport to draw
					// axis' box.
	float	charsize_;
	float	vpw_, vph_;		// Size of canvas' viewport.
	AvAxisOptions	*options_;
	AvRaster	*raster_;
	// Axis input/output data format. Set from raster info.
	AXISFORMAT xAxisInput_;
	AXISFORMAT yAxisInput_;
	AXISFORMAT xAxisOutput_;
	AXISFORMAT yAxisOutput_;
	AxisGlobalOptions	*goptions_;
};

#endif
