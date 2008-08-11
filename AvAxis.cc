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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvAxis.cc,v 19.0 2003/07/16 05:46:30 aips2adm Exp $
//
// $Log: AvAxis.cc,v $
// Revision 19.0  2003/07/16 05:46:30  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:18  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.2  2002/03/11 16:54:14  hravlin
// Changed NEED_FORTRAN_UNDERSCORES to be false for HP systems and true
// otherwise. It can now be defined externally to the file.
//
// Revision 17.1  2002/01/25 21:41:58  hravlin
// nlfunc() never initialized cntrl. Neither does pgsbox. This could cause
// a 1000 times increase in axis drawing times.
//
// Revision 17.0  2001/11/12 19:41:46  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:41:56  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:07:46  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 18:18:41  hravlin
// Replaced "char *idents" by "char idents[]" to avoid compiler warnings.
//
// Revision 14.0  2000/03/23 16:07:31  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/08/25 20:01:44  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:39:10  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:15  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:57:47  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:51:17  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.6  1998/06/09 21:19:31  hr
// Removed unneeded semicolon after DEFINE_MEMBER_INFO macro.
//
// Revision 9.5  1998/02/26 22:15:24  hr
// In drawLabel(), make use of useGlobalColor variable to choose color to draw with.
//
// Revision 9.4  1998/02/20 20:32:36  hr
// Changed label handling to use position and angle controls rather than
// old method.
//
// Revision 9.3  1997/12/17 21:49:45  hr
// Changes to remove warnings from MIPSpro.7x compiler.
//
// Revision 9.2  1997/11/17 16:53:27  hr
// Added support for axis offsets and captions.
//
// Revision 9.1  1997/09/23 20:27:21  hr
// In draw{recti/curvi}linear(), use global character scale to change size
// of numeric axis labels.
//
// Revision 9.0  1997/08/25 21:23:35  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.13  1997/08/11 20:37:15  hr
// Removed unsused code found by gcc -Wall.
//
// Revision 8.12  1997/08/01 21:40:06  hr
// DCC found some unused code.
//
// Revision 8.11  1997/08/01 21:30:31  hr
// Changed drawMarker() to set window to (0..1) rather than actual values.
//
// Revision 8.10  1997/06/16 21:53:06  hr
// Removed a couple of unused static variables.
//
// Revision 8.9  1997/06/16 15:53:29  hr
// Changes to support m/s <-> km/s, mainly the ScaleInfo class.
//
// Revision 8.8  1997/05/22 19:50:48  hr
// In drawLinearAxis(), y intrvls was being set from crvMajorIntervals.
//
// Revision 8.7  1997/05/21 21:40:29  hr
// AxisGlobalOptions::WorldToPixel() needed to be subtracting start position.
//
// Revision 8.6  1997/04/30 20:05:53  hr
// (Re)initialize cache statistics in resetCache().
//
// Revision 8.5  1997/04/28 14:55:53  bglenden
// Unilaterally declare that we need underscores for now.
//
// Revision 8.4  1997/04/25 16:00:37  hr
// Changed color handling for curvilinear axes. Added global color boolean and
// made labels default to 'default' color rather than global color.
//
// Revision 8.3  1997/04/23 17:46:34  hr
// Cleaned up some DCC compiler warnings.
//
// Revision 8.2  1997/04/23 17:33:09  hr
// Mistyped a directive.
//
// Revision 8.1  1997/04/23 17:10:31  hr
// Changes to support use of pgcrvl.
//
// Revision 8.0  1997/02/20 03:14:30  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.8  1997/02/11 20:35:16  hr
// In generate(), reset the title to that of the raster. This gets around
// the problem that there is only one option title variable, but many
// axis titles.
//
// Revision 7.7  1997/02/09 00:02:32  hr
// Minor changes to track changing some of AxisOption's variables to
// IntegerNotifiers from DoubleNotifiers.
//
// Revision 7.6  1997/02/05 17:21:24  hr
// Removed unused variable in yLabel().
//
// Revision 7.5  1997/01/24 20:36:55  hr
// Widespread changes to convert to using AvAxisOptions for settable parameters.
//
// Revision 7.4  1996/12/12 09:07:39  droberts
// Final update from monet archive.
//
// Revision 1.5  1996/10/30 21:40:14  hr
// Unit strings are never displayed for an axis.
//
// Revision 1.4  1996/09/25 18:40:18  hr
// Axis labels now include unit information only if print format is DEFAULT.
//
// Revision 1.3  1996/09/20 18:35:42  hr
// Changes to to work with AvAccessor.
//
// Revision 1.2  1996/08/14 17:15:18  hr
// Changes to remove g++ warnings (from bglenden).
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.12  1996/06/18  18:45:46  pixton
//  Copyright Update
//
//  Revision 1.11  1996/03/05  17:33:15  hr
//  Removed a call to cpgsci that set the current color to the background color.
//
//  Revision 1.10  1996/02/29  17:12:46  hr
//  Some variables weren't being initialized. xtype in useRasterInfo
//  wasn't being set if axis type was floating.
//
//  Revision 1.9  1996/02/23  17:28:27  hr
//  Included axis setup code that was in AvCLUImageSetter.
//  Removed need to always have a raster.
//  Many changes to get text and tick marks to come out reasonably.
//
//  Revision 1.8  1996/02/08  18:47:05  hr
//  Removed some unused constants.
//
//  Revision 1.7  1996/02/08  18:34:39  hr
//  Some changes to get labels to display better. The title is scaled to
//  fit.
//
//  Revision 1.6  1996/02/04  21:18:00  hr
//  Added test version of display of "id".
//
//  Revision 1.5  1996/02/02  20:58:56  hr
//  Moved axis labeling from AvCLUImageSetter to AvAxis::setRaster.
//
//  Revision 1.4  1996/01/31  17:53:59  hr
//  Removed Grid and 0 line from default display.
//
//  Revision 1.3  1996/01/25  17:42:43  hr
//  Needed to add Graphics2D initializations.
//
//  Revision 1.2  1996/01/24  18:32:15  hr
//  DCC didn't like having static const variables in a static function.
//
//  Revision 1.1  1996/01/24  18:02:54  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvAxis.cc

Subclass of AvPolyline that generates a labeled axis. Its primary use
is to draw a labeled axis for a raster.

Uses PGPLOT to draw a 'box' around the object.

*/
#include <stdio.h>
#include "AvAxis.h"
#include "AvViewportSetter.h"
#include "AvRaster.h"
#include "AvPGDriver.h"
#include "AvCanvas.h"
///// Needed for curvilinear axis support.
#include "AvCLUIRaster.h"
#include "AvAccessor.h"
#include "AvUnits.h"

#if defined(AIPSPLUSPLUS) && defined(GLISH)
#include "AvScrollingBufferDataSet.h"
#endif

// If defined, prints out times for pgcrvl.
//#define TIMEIT
#ifdef TIMEIT
#include <time.h>
#endif

extern "C" {
#include <cpgplot.h>
}

// Global options are shared between AvAxis objects of the same
// data set and view axis. Currently, the primary purpose is to support
// parameters used to draw curvilinear axes using pgcrvl.
class AxisGlobalOptions : public AvResource {
  public:
	AxisGlobalOptions(AvRaster *, const int cachesize=8192);
	~AxisGlobalOptions();
	void WorldToPixel( const double world[], double pixel[]);
	void PixelToWorld( const double pixel[], double world[]);
	////////////////////////////////////////////////////////////////
	// pgcrvl will cache the extrema values from pgcrvl since it takes
	// a long time to generate, we save them.
	Boolean haveExtrema()const{return haveExtrema_;}
	// Can't save extrema between AvAxis calls if data set is variable.
	Boolean mayUseExtrema()const{return mayUseExtrema_;}
	void haveExtrema(const Boolean he){haveExtrema_ = he;}
	void setExtrema(const double *);
	void getExtrema( double *)const;
	////////////////////////////////////////////////////////////////
	// May only use curvilinear axis routine if there is an accessor.
	//  (to do pixel to/from world conversions).
	// Currently, the only raster that supports one is AvCLUIRaster.
	Boolean mayUseCurvilinearAxis()const{return mayUseCurvilinear_;}
	// Returns NULL if raster is not subclassed from AvCLUIRaster.
	AvAccessor *accessor()const{return accessor_;}
	AvRaster *raster()const{return raster_;}
	////////////////////////////////////////////////////////////////
	// Caching mechanism to minimize world to/from pixel calls.
	void resetCache();	// Flush cache.
	// Store values in slot.
	int storeValues(const int index, const double wx, const double wy,
					 const double px, const double py);
	// Return slot index for args. May or may not be used.
	// Returns -1 if no slot was found. (Table full).
	int findEntry(const double wx, const double wy);
	// Return pixel values from slot.
	void getPValues(const int index, double &px, double &py);
	// Does slot have an entry?
	Boolean isHit(const int indx)const{return inUse_(indx) ? TRUE : FALSE;}
	Boolean validIndex(const int index)const
		{ return ((index>=0)&&(index<cacheSize_))?TRUE : FALSE;}
	// "labden" values. Used in generating hash index.
	void setSteps(const int xsteps, const int ysteps);
	Boolean cacheEnabled()const{return cacheEnabled_;}
	void enableCache(const Boolean enable){cacheEnabled_ = enable;}
	int getCacheIndex(const double x, const double y);
	void resetCounts();	// Reset monitoring counters.
	void getCounts( int &cacheHits, int &cacheSeeks);
	////////////////////////////////////////////////////////////////
	// Info to convert an axis value into its 'scaled' form (eg m/s->km/s).
	// Conversion can be quick via a scale factor or via a call to
	// AvUnits::convert. Since the later appears to only do a multiply,
	// it is disabled until/unless it is found to be needed. (It's
	// an order of magnitude slower).
	class ScaleInfo {
	  public:
		ScaleInfo();
		~ScaleInfo();
		void init(AvAccessor *accessor, const int axis);
		// Do (un)scaling, avoiding subroutine calls when possible.
		inline double scale(const double val)const
			{ return (needscale_) ?
			     ((QUICKSCALE) ? val*scaleFactor_ : scale_(val))
					   : val;}
		inline double unscale(const double val)const
			{ return (needscale_) ?
			   ((QUICKSCALE) ? val/scaleFactor_ : unscale_(val))
				 : val;}
		const AvString &abbrev()const{return abbrev_;}
	  private:
		// If TRUE, scaling is done via multiply/divide. If FALSE,
		// scale in done via call to a conversion routine.
		static Boolean QUICKSCALE;
		double scale_(const double val)const;
		double unscale_(const double val)const;
		Boolean		needscale_;	// If FALSE, don't scale.
		double		scaleFactor_;
		AvString	units_;		// 'Base' units for axis.
		AvString	scaledUnits_;	// Units to convert to.
		AvString	abbrev_;	// Short form of scaledUnits_.
		AvString	measurementName_;
		};
	const ScaleInfo *scaleInfo(const int i)
		const{return (i == 0) ? &xScale_ : &yScale_;}
  private:
	AvRaster	*raster_;
	AvAccessor	*accessor_;
	Boolean		haveExtrema_;
	Boolean		mayUseExtrema_;
	Boolean		mayUseCurvilinear_;
	Boolean		cacheEnabled_;	// If false, don't cache.
	double		extrema_[4];	// Extrema result from pgcrvl.
	////////////////////////////////////////////////////////////////
	// Cache used to store results of world to pixel conversions.
	AvWPosition	wx_;		// world x,
	AvWPosition	wy_;		// world y,
	AvWPosition	px_;		// pixel x,
	AvWPosition	py_;		// pixel y.
	AvIPosition	inUse_;		// Non 0 means entry is used.
	int		cacheSize_;	// # of slots available.
	int		cacheSeeks_;	// Usage stats
	int		cacheHits_;
	// Used in generating hash index.
	double		wXMin_, wXMax_,
			wYMin_,	wYMax_,
			wXStep_, wYStep_;
	int		XSteps_, YSteps_;
	////////////////////////////////////////////////////////////////
	AvWPosition	world_;		// World & ijk are used to more
	AvWPosition	ijk_;		// efficiently access low lvl rtns.
	AvWPosition	start_;
	int		xAxis_, yAxis_;	// DS axis used for X & Y.
	int		zAxis_;		// If >= 0, then the z axis.
	ScaleInfo	xScale_;
	ScaleInfo	yScale_;
};


DEFINE_MEMBER_INFO(AvAxis)

				   // Assume a bunch of points (arbitrary).
AvAxis::AvAxis(AvRaster *r, AvAxisOptions *o) : AvPolyline(1024)
{
	INIT_CLASS(AvPolyline);

	initToZero();
	// o Should not be NULL.
	if(o == NULL)
		o = new AvAxisOptions(NULL);
	setOptions(o);

	// setRaster will (re)initialize many parameters.
	useRasterInfo(r);
}

// Initialize things to 0.
void AvAxis::initToZero()
{
	lm_ = rm_ = tm_ = bm_ = 0;
	charsize_ = 0.0;	// 0 means use whatever we want.
	xAxisInput_ = yAxisInput_ = xAxisOutput_ = yAxisOutput_ = FLOAT;
	// World window size from DS.
	xwin0_ = xwin1_ = ywin0_ = ywin1_ = 0.0;
	// Above or massaged for offsets.
	Xwin0_ = Xwin1_ = Ywin0_ = Ywin1_ = 0.0;
	haveAxis_ = FALSE;
	showOffsets_ = FALSE;
	// Due to the PG offsets being fractions of a window, axes don't scale
	// correctly if the viewport changes size. If the viewport size
	// changes, the axis will be recomputed.
	vpw_ = vph_ = 0.0;
	options_ = NULL;
	raster_ = NULL;
//	haveExtrema_ = FALSE;
	goptions_ = NULL;
}

// When the options object changes mark as needing to regenerate.
static void optionsCB(XtPointer , AvConductor *, XtPointer data)
{AvAxis *me = (AvAxis *)data;

	me->touch();
}

// Change current options pointer.
void AvAxis::setOptions(AvAxisOptions *o)
{
	if(o == options_)
		return;
	if(options_ != NULL)
	{	options_->removeCallback(&optionsCB, AvConductor::ValueChanged,
					 this, this);
		options_->unref();
	}
	options_ = o;
	if(options_ != NULL)
	{	options_->ref();
		// Add to front of list so the imageSetter gets called after
		// this touches itself.
		options_->addCallback(&optionsCB, AvConductor::ValueChanged,
				      this, this, TRUE);
	}
//	setOptionMasks(o);
}

AvAxis::~AvAxis()
{
	if(options_ != NULL)
		options_->unref();
	options_ = NULL;
	if(goptions_ != NULL)
		goptions_->unref();
	if(raster_ != NULL)
		raster_->unref();
}

#if 0
static const double DEG2SEC = 240.0;
static inline double degrees2sec(const double deg)
{
// (360deg/day * 1day/24hrs * 1hr/3600sec = .004 deg/sec)
	return deg *DEG2SEC;
}

static const double HRS2SEC = 3600;
static inline double hours2sec(const double hrs)
{
	return hrs *HRS2SEC;
}
#endif

// Use the raster to supply label strings and formats.
void AvAxis::useRasterInfo( AvRaster *r)
{
double	xmin, ymin, xmax, ymax;

	if(r == NULL)
		return;		// Nothing to be done.
	if(raster_ != r)
	{	if(raster_ != NULL)
			raster_->unref();
		raster_ = r;
		if(raster_ != NULL)
			raster_->ref();
	}

	AvRaster::axisInfo *info = r->axisinfo();
	if(info == NULL)
		return;		// Oops.

	// Generate labels.
	char label[256];

#if 0
	// Get around problem with Sun's memory checker.
#if defined(__sun) && defined(__SunOS_5_5)
	  memset(label, 0, sizeof(label));
#endif
#endif

	const int XAXIS = AvRaster::XAXIS;
	const int YAXIS = AvRaster::YAXIS;

	sprintf(label, "%s",info->label(XAXIS));
	AxisGlobalOptions *opts = globalOptions();
	// Append units abbreviation, if any.
	if(opts != NULL)
	{	const AxisGlobalOptions::ScaleInfo *inf = opts->scaleInfo(0);
		const char *chrs = inf->abbrev().chars();
		if((chrs != NULL) && (strlen(chrs) > 0))
		{	strcat(label, " (");
			strcat(label, chrs);
			strcat(label, ")");
		}
	}

	xLabel( label);

	sprintf(label, "%s",info->label(YAXIS));
	if(opts != NULL)
	{	const AxisGlobalOptions::ScaleInfo *inf = opts->scaleInfo(1);
		const char *chrs = inf->abbrev().chars();
		if((chrs != NULL) && (strlen(chrs) > 0))
		{	strcat(label, " (");
			strcat(label, chrs);
			strcat(label, ")");
		}
	}

	yLabel( label);

	title(r->getName());

	// Supply a name for debugging.
	sprintf(label, "Axis for %s", r->getName());
	name(label);

	// Initial values for world coordinate window.
	xmin = info->minValue(XAXIS);
	xmax = info->maxValue(XAXIS);
	ymin = info->minValue(YAXIS);
	ymax = info->maxValue(YAXIS);

	// RA: display in HMS. Input format is degrees or RADIANS.
	switch(info->printFormat(XAXIS)) {
	case AvRaster::axisInfo::HMS:
		xAxisInput_ = DEGREES;
		xAxisOutput_ = HOURS;
		break;
	case AvRaster::axisInfo::HMS_RAD:// the _RAD parts haven't been tested.
		xAxisInput_ = RADIANS;
		xAxisOutput_ = HOURS;
		break;
	case AvRaster::axisInfo::DMS:
		xAxisInput_ = HOURS;
		xAxisOutput_ = DEGREES;
		break;
	case AvRaster::axisInfo::DMS_RAD:
		xAxisInput_ = RADIANS;
		xAxisOutput_ = DEGREES;
		break;
	default:
		xAxisInput_ = FLOAT;
		xAxisOutput_ = FLOAT;
		break;
	}

	// DEC:display in DMS. Input format is degrees or RADIANS.
	switch(info->printFormat(YAXIS)) {
	case AvRaster::axisInfo::HMS:
		yAxisInput_ = DEGREES;
		yAxisOutput_ = HOURS;
		break;
	case AvRaster::axisInfo::HMS_RAD:
		yAxisInput_ = RADIANS;
		yAxisOutput_ = HOURS;
		break;
	case AvRaster::axisInfo::DMS:
//		yAxisInput_ = DEGREES;
		yAxisInput_ = HOURS;	// This works, DEGREES doesn't.
		yAxisOutput_ = DEGREES;
		break;
	case AvRaster::axisInfo::DMS_RAD:
		yAxisInput_ = RADIANS;
		yAxisOutput_ = DEGREES;
		break;
	default:
		yAxisInput_ = FLOAT;
		yAxisOutput_ = FLOAT;
		break;
	}

	// Set the default World coordinate window values.
	setWorldWindow(xmin, xmax, ymin, ymax);

//	touch();	// Done in setWorldWindow.
}

void AvAxis::touch()
{
	haveAxis_ = FALSE;
	reset(-1);	// tell AvPolyline to get rid of its point list.
	AvPolyline::touch();
}

// Return the suggested number of tick to display along the X axis
static int numXTicks(const int size)
{
int nticks;

	if(size < 128)
		nticks = 3;
	else
	if(size <= 192)
		nticks = 5;
	else
	if(size <= 256)
		nticks = 7;
	else
	if(size <= 320)
		nticks = 9;
	else
		nticks = 11;
	return nticks;
}

static const float CHARHEIGHT = 0.025;	// Character height w/o scaling.
					// (= 1/40 max height (1.0)).
// Width of a character in pixels = CHARWIDTH*cscl*<width of drawing surface>.
// Characters are not constant width so this is just a gross estimate.
static const float CHARWIDTH = 0.014;	

static const int NUMCHRS = 14;	     // Max # of chars in a horizontal label.
					// "?? -05<deg>24'48"< >" (est.)
static const int CHARS_PER_TICK= 7;	// Assumed max # of character/tick
					// (From pgbox.f).

/* Compute the (approximate) scalefactor needed to fit n chars into
space # of pixels. If vertical is FALSE, scale is for horizontal
drawing, else vertical. The space needed for a character string is:
	#chars * charsize * charscale * width

The width is needed since characters are drawn with fractional vectors:
increasing the viewport size increases the size of the characters.

nchrs	# of characters to draw.
space	# of pixels to draw into
vpsize	Size of drawing surface (height for vertical=TRUE, width otherwise).
vertical Whether text will be drawn vertically.
*/

float AvAxis::computeCharScale( const float nchrs, const float space,
			const int vpsize, const Boolean vertical)
{
float cscl, appc, chrsize, uppc;

	if((nchrs == 0) || (vpsize == 0))
		return 0.0;
	appc = space/nchrs;		// # of pixels/char available.
	chrsize = (vertical) ? CHARHEIGHT : CHARWIDTH;
	// Pixels/character for unit scaling.
	uppc = chrsize*vpsize;
	cscl = appc/uppc;		// scale needed.
#if 0
printf("chrsize %.3f, appc %.2f, nchars %.1f space %.1f vpsize %d\n",
	chrsize, appc, nchrs, space, vpsize);
#endif
	return cscl;
}

#define BIGN 1000.0

// Return character scale needed for the box.
// Computes a scale factor for estimated label sizes.
// If there is no space, for a label in the requested position,
// the corresponding option will be turned off.
// Caller will need to check the return. If it is zero, there
// wasn't enough room and/or printing is turned off.
float AvAxis::getBoxCharScale(  const int vpwidth, const int nxchars,
				int &xoptions, int &yoptions)
{
float cscale, maxScale;
float space, vcscale, hcscale;

	// Max char scale to allow.
	maxScale = maxCharSize()/(CHARHEIGHT*vpwidth);
	if(maxScale == 0.0)
		maxScale = BIGN;	// Default to "no" limt.

	//Compute horizontal space needed for vertical labels.
	if(yoptions & STDLABELPOSITION)
		space = (float) lm_;
	else
	if(yoptions & NONSTDLABELPOSITION)
		space = (float) rm_;
	else
		space = 0.0;		// Not printing

	// If there is no room, turn off labels.
	if(space == 0.0)
	{	yoptions &= ~NOAXISLABEL;
		vcscale = BIGN;	// Something large.
	}
	else
		vcscale = computeCharScale(NUMCHRS, space, vpwidth, FALSE);

	// Horizontal space needed by horizontal labels.
	if(xoptions & STDLABELPOSITION)
		space = (float) bm_;
	else
	if(xoptions & NONSTDLABELPOSITION)
		space = (float) tm_;
	else
		space = 0.0;		// Not printing
	// If there is no room, turn off labels.
	if(space == 0.0)
	{	xoptions &= ~NOAXISLABEL;
		hcscale = BIGN;	// Something large.
	}
	else
	{	// Space was used to see if there was vertical room.
		// Compute scaling needed to fit an estimated number
		// of chars directly under the raster's viewport.
		// (There is usually some overhang on the left).
		float hsize = nxchars*CHARWIDTH*vpwidth;
		hcscale = (vpwidth - (lm_ + rm_))/hsize;
	}
	cscale = (vcscale <= hcscale) ? vcscale : hcscale;
	if(cscale == BIGN)
		cscale = 0.0;
	else
	if(cscale > maxScale)
		cscale = maxScale;
#if 0
printf("vcscale %f, hcscale %f scale %f maxScale %f\n",
		vcscale, hcscale, cscale, maxScale);
#endif
	return cscale;
}

///////////////////////////////////////////////
/* Function to round a value to a "NICE" number. This is PGPLOT's pgrnd
function translated to C because the f2c'd version doesn't seem to work
correctly with aipsview.
*/
static double nice_[] = { 2.0, 5.0, 10.0};

#define ABS(x) ( ((x) >= 0)? (x) : -(x))

double	AvAxis::pgrnd(double x, int &nsub)
{
double	frac, pwr,xlog, xx, value;
int	i, ilog;

	if(x == 0.0)
	{	nsub = 2;
		return 0.0;
	}

	xx = ABS(x);
	xlog = log10(xx);
	ilog = (int)xlog;
	if(xlog < 0.0)
		ilog -= 1;
	pwr = pow(10.0, ilog);
	frac = xx/pwr;

	if(frac <= nice_[0])
		i = 0;
	else
	if(frac <= nice_[1])
		i = 1;
	else
		i = 2;

	value = pwr*nice_[i];
	value = ABS(value);
	if(x < 0.0)
		value = -value;
	if(i == 0)
		nsub = 2;
	else
		nsub = 5;

	return value;
}

///////////////////////////////////////////////
#if 0
// Draw X, Y or Title.
void AvAxis::drawLabel( const AvAxisOptions::axisLabel *Label, float charScale)
{char *side;
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
	case AvAxisOptions::LEFT:
			justify = 0.0;
			break;
	case AvAxisOptions::RIGHT:
			justify = 1.0;
			break;
	default:
	case AvAxisOptions::CENTER:
			justify = 0.5;
			break;
	}

	switch(Label->side_.getValue()){
	case AvAxisOptions::LEFT:
			side = "L";
			break;
	case AvAxisOptions::LEFTP:
			side = "LV";
			break;
	case AvAxisOptions::RIGHT:
			side = "R";
			break;
	case AvAxisOptions::RIGHTP:
			side = "RV";
			break;
	case AvAxisOptions::BOTTOM:
			side = "B";
			break;
	case AvAxisOptions::TOP:
			side = "T";
			break;
	default:
			side = "L";
	}
	// The resultant scale is the product of the scale computed by
	// AvAxis, the global character scale and the per label scale.
	cscl= (float)(charScale*Label->charScale_.getValue());
	cscl *= (float) options_->gCharScale_.getValue();

	cpgsave();
	 cpgsch(cscl);	// Adjust character size.
	 from /= cscl;	// Keeps distance constant when char scale varies.
#if 0
	 if(!Label->useGlobalColor_.getValue())
	 {int color = Label->color_.getValue();
		cpgsci(color);
		// Assume that colored text will overlay the image and that
		// we want to clear its background.
		if(color == 0)
			cpgstbg(1);
		else
			cpgstbg(0);
	 }
#else
	// Opaque background?
	 {int color = Label->color_.getValue();
		cpgsci(color);
		if(Label->opaqueBackground_.getValue())
		{	if(color == 0)
				cpgstbg(1);
			else
				cpgstbg(0);
		}
	 }
#endif

	 cpgmtxt(side, from, along, justify, label);
	cpgunsa();
}
#else
// Draw X, Y or Title.
void AvAxis::drawLabel( const AvAxisOptions::axisLabel *Label, float charScale)
{float cscl;
 float x = Label->x_.getValue();
 float y = Label->y_.getValue();
 float angle = Label->angle_.getValue();
 float justify;
 const char *label;

	// Show this label?
	if(!Label->drawLabel_.getValue())
		return;

	label = Label->text_.getString();
	if((label == NULL) || (strlen(label) == 0))
		return;
	
	switch(Label->justify_.getValue()){
	case AvAxisOptions::LEFT:
			justify = 0.0;
			break;
	case AvAxisOptions::RIGHT:
			justify = 1.0;
			break;
	default:
	case AvAxisOptions::CENTER:
			justify = 0.5;
			break;
	}

	// The resultant scale is the product of the scale computed by
	// AvAxis, the global character scale and the per label scale.
	cscl= (float)(charScale*Label->charScale_.getValue());
	cscl *= (float) options_->gCharScale_.getValue();

	cpgsave();
	 cpgsch(cscl);	// Adjust character size.
	// Opaque background?
	 {//int color = Label->color_.getValue();
	  int color;
		if(Label->useGlobalColor_.getValue())
			color = options_->gAxisColor_.getValue();
		else
			color = Label->color_.getValue();

		cpgsci(color);
		if(Label->opaqueBackground_.getValue())
		{	if(color == 0)
				cpgstbg(1);
			else
				cpgstbg(0);
		}
	 }

	 cpgswin(0.0, 1.0, 0.0, 1.0);
#if 0
	 from /= cscl;	// Keeps distance constant when char scale varies.
	 cpgmtxt(side, from, along, justify, label);
#else
	 cpgptxt(x, y, angle, justify, label);
//cout << "X: " << x << " Y: " << y << " Angle: " << angle << endl;
#endif
	cpgunsa();
}
#endif

#if 0
// Return the position along a line given its fractional distance along it.
static inline float windowPosition(const double percent,
				   const float start, const float end)
{
	return (float)percent*(end-start) + start;
}
#endif

#ifdef __sgi
double AvAxis::worldX2PG(const double value)
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

double AvAxis::worldY2PG(const double value)
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
#endif

// Draw the marker at the given location.
// extraScale is used to help set marker scale factor. (Usually its = 1.0).
// The x & y positions give the size and position of the viewport in
// world coordinates.
#if 0
void AvAxis::drawMarker(const AvAxisOptions::marker *Marker,
			float extraScale, const float x0, const float x1,
			const float y0, const float y1)
{float x,y;

	if(Marker->show_.getValue() == 0)
		return;
	float scale = extraScale*Marker->scale_.getValue();
	int color = Marker->color_.getValue();
	int symbol = Marker->type_.getValue();

	cpgsave();

	// Is the position stored in world coords or as a fraction of the
	// size of the viewport?
	int format = Marker->xyFormat_.getValue();
	if(format == AvAxisOptions::WORLD)
	{	x = (float)worldX2PG(Marker->x_.getValue());
		y = (float)worldY2PG(Marker->y_.getValue());
	}
	else
	{	x = Marker->x_.getValue();
		y = Marker->y_.getValue();
	x = windowPosition(x, x0, x1-1) + 0.5;
	y = windowPosition(y, y0, y1-1) + 0.5;
		cpgswin(x0, x1, y0, y1);
	}
//	cpgsave();
	 cpgsci(color);
	 cpgsch(scale);
	 cpgpt(1, &x, &y, symbol);
	cpgunsa();
}
#else
#if 0
void AvAxis::drawMarker(const AvAxisOptions::marker *Marker,
			float extraScale, const float x0, const float x1,
			const float y0, const float y1)
#else
void AvAxis::drawMarker(const AvAxisOptions::marker *Marker,
			float extraScale, const float , const float ,
			const float , const float )
#endif
{float x,y;

	if(Marker->show_.getValue() == 0)
		return;
	float scale = extraScale*Marker->scale_.getValue();
	int color = Marker->color_.getValue();
	int symbol = Marker->type_.getValue();


	// Get position as fraction of window.
	x = Marker->x_.getValue();
	y = Marker->y_.getValue();
	// Convert to IJK position. The 0.5 moves it to the center of
	// the pixel.
#if 0
	x = windowPosition(x, x0, x1-1) + 0.5;
	y = windowPosition(y, y0, y1-1) + 0.5;
#endif
	//cout << "Marker X,Y = " << x << ", " << y << endl;

	cpgsave();
//	 cpgswin(x0, x1, y0, y1);
	 cpgswin(0.0, 1.0, 0.0, 1.0);
	 cpgsci(color);
	 cpgsch(scale);
	 cpgpt(1, &x, &y, symbol);
	cpgunsa();
}
#endif

// For now, force quicker scaling.
Boolean AxisGlobalOptions::ScaleInfo::QUICKSCALE = TRUE;

////////////////////////////////////////////////////////////////////////
/////		pgcrvl interface
////////////////////////////////////////////////////////////////////////
extern "C" {

typedef void(*S_fp)(int*, int*, int*, int*, char*, int*, double*, double*,
		   double*, int*, double*, int *, int);

// Decide whether external FORTRAN references need an underscore appended
// (they usually do).
#if !defined(NEED_FORTRAN_UNDERSCORES)
#if defined(__hpux__)
#define NEED_FORTRAN_UNDERSCORES 0
#else
#define NEED_FORTRAN_UNDERSCORES 1
#endif
#endif

/*
#if defined(AIPSPLUSPLUS) && defined(NEED_FORTRAN_UNDERSCORES)
#define PGCRVL pgcrvl_
#elif defined(AIPSPLUSPLUS) && !defined(NEED_FORTRAN_UNDERSCORES)
#define PGCRVL pgcrvl
#elif !defined(AIPSPLUSPLUS)
#define PGCRVL pgcrvl_
#endif
*/
#if NEED_FORTRAN_UNDERSCORES
#define PGCRVL pgcrvl_
#else
#define PGCRVL pgcrvl
#endif

extern void PGCRVL (int *axen, char *idents, char *opt,
	int *labctl, int *labden, int *ci, int *gcode, 
	double *tiklen, int *ng1, double *grid1, int *ng2, 
	double *grid2, int *doeq, S_fp nlfunc, int *nlc, int *
	nli, int *nld, char *nlcprm, int *nliprm, double *nldprm, 
	int *nc, int *ic, double *cache, int *ierr, int idents_len,
	int opt_len, int nlcprm_len);

// Counters for stats.
static int oc1=0, oc2=0, ocm1 = 0;
#ifdef TIMEIT
static clock_t w2p0=0, w2p1=0, p2w0=0, p2w1=0;
#endif

// In case ints are smaller than pointers, just pass the pointer to the
// global options the BASIC way. Since aipsview isn't multi-threaded,
// multiprocessing, etc. we can get away with it.
static AxisGlobalOptions *gOptions_=NULL;

// nlfunc is called from pgcrvl to do world to/from pixel conversions.
// The version used here just passes the requests to aipsview's routines
// via the global options variable.
// The return code is supposed to indicate whether the pixel or world
// values were invalid. We ignore that possibility since it isn't clear
// what that means. pgcrvl intentionally calls with pixel/world values
// outside the range of the data set.
#if 0
static void nlfunc(	int *opcode, int *nlc, int *nli, int *nld,
			char *nlcprm, int *nliprm, double *nldprm,
			double *world, double *pixel, int *contrl,
			double *contxt, int *ierr, int)
#else
static void nlfunc(	int *opcode, int *, int *, int *,
			char *, int *, double *,
			double *world, double *pixel, int *contrl,
			double *, int *ierr, int)
#endif
{ int err=0;

	// Make sure everything has been set up correctly.
	if((gOptions_ == NULL) || (!gOptions_->mayUseCurvilinearAxis()))
	{	*ierr = 1;
		return;
	}

	switch(*opcode) {
	case 0:		// Initialize.
		// Usage counts.
		oc1 = oc2 = ocm1 = 0;
		*contrl = 0;		// Make sure it's set.
#ifdef TIMEIT
		// Time counts.
		w2p0 = w2p1 = p2w0 = p2w1=0;
#endif
		err = 0;
		break;
	case 1:		// Compute pixel coordinates.
		oc1++;
	case 2:		// Similar.
		if(*opcode == 2) oc2++;
		{
#ifdef TIMEIT
w2p0 = clock();
#endif
			gOptions_->WorldToPixel(world, pixel);
#ifdef TIMEIT
w2p1 += clock() - w2p0;
#endif
		}
		break;
	case -1:	// Compute World coordinates
		ocm1++;
		gOptions_->PixelToWorld(pixel, world);
		break;
	default:
		err = 1;
	}
	*ierr = err;
}

}	// extern "C"

static void cpgcrvl(int *axen, const char *, char *opt,
	int labctl, int labden, int *ci, int *gcode,
	double tiklen, int ng1, double *grid1, int ng2,
	double *grid2, int doeq, S_fp nlfunc, int nlc, int nli,
	int nld, char *nlcprm, int *nliprm, double *nldprm, 
	int nc, int &ic, double *cache, int &ierr)
{int idents_len=1, opt_len=1, nlcprm_len = 1;
 char idents[] = "   ";

	PGCRVL( axen, idents, opt,
	 &labctl, &labden, ci, gcode,
	 &tiklen, &ng1, grid1, &ng2, 
	 grid2,  &doeq, nlfunc, &nlc, &nli,
	 &nld,  nlcprm, nliprm, nldprm,
	 &nc, &ic, cache, &ierr, idents_len, opt_len, nlcprm_len);
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

// Given an option mask generate an AvAxisOptions code for a curvilinear
// axis format.
static int axisCode(const int mask)
{ int c;

	if(mask & AvAxis::HMS)
		c = AvAxisOptions::ANGLE_HMS_24;	// 'G'
	else
	if(mask & AvAxis::DMS)
		c = AvAxisOptions::ANGLE_DMS_180;	// 'E'
	else
		c = AvAxisOptions::PLAIN;
	return c;
}

// Generate a pgcrvl 'opt' character for an axis.
static char axisCode(const int code, const int mask)
{ char c;

	switch( code) {

	case AvAxisOptions::DEFAULT:
		c = axisCode(axisCode(mask), 0);
		break;
	case AvAxisOptions::PLAIN:
		c = ' ';
		break;
	case AvAxisOptions::ANGLE_0_360:
		c = 'A';
		break;
	case AvAxisOptions::ANGLE_M180_180:
		c = 'B';
		break;
	case AvAxisOptions::ANGLE_M180_180U:
		c = 'C';
		break;
	case AvAxisOptions::ANGLE_DMS_360:
		c = 'D';
		break;
	case AvAxisOptions::ANGLE_DMS_180:
		c = 'E';
		break;
	case AvAxisOptions::ANGLE_DMS_360U:
		c = 'F';
		break;
	case AvAxisOptions::ANGLE_HMS_24:
		c = 'G';
		break;
	case AvAxisOptions::ANGLE_HMS_12:
		c = 'H';
		break;
	case AvAxisOptions::ANGLE_HMS_12U:
		c = 'I';
		break;
	case AvAxisOptions::TIME_HMS:
		c = 'T';
		break;
	default:
		c = ' ';
	}

	return c;
}

void AvAxis::getWorldWindow(const int xAxisInput, float &x0, float &x1,
			    const int yAxisInput, float &y0, float &y1)const
{
	switch (xAxisInput) {
	case HOURS:
		x0 = hours2sec(x0);
		x1 = hours2sec(x1);
		break;
	case DEGREES:
		x0 = degrees2sec(x0);
		x1 = degrees2sec(x1);
		break;
	case RADIANS:
		x0 = radians2sec(x0);
		x1 = radians2sec(x1);
		break;
	case FLOAT:
	default:
		x0 = x0;
		x1 = x1;
		break;
	}

	switch(yAxisInput) {
	case HOURS:
		y0 = hours2sec(y0);
		y1 = hours2sec(y1);
		break;
	case DEGREES:
		y0 = degrees2sec(y0);
		y1 = degrees2sec(y1);
		break;
	case RADIANS:
		y0 = radians2sec(y0);
		y1 = radians2sec(y1);
		break;
	case FLOAT:
	default:
		y0 = y0;
		y1 = y1;
		break;
	}
}

#if 0
// Convert internal (PGPLOT) format 
void AvAxis::PGToWorld( const int xAxisInput, float &x0, float &x1,
			const int yAxisInput, float &y0, float &y1)const
{
	switch (xAxisInput) {
	case HOURS:
		x0 = hours2sec(x0);
		x1 = hours2sec(x1);
		break;
	case DEGREES:
		x0 = degrees2sec(x0);
		x1 = degrees2sec(x1);
		break;
	case RADIANS:
		x0 = radians2sec(x0);
		x1 = radians2sec(x1);
		break;
	case FLOAT:
	default:
		x0 = x0;
		x1 = x1;
		break;
	}

	switch(yAxisInput) {
	case HOURS:
		y0 = hours2sec(y0);
		y1 = hours2sec(y1);
		break;
	case DEGREES:
		y0 = degrees2sec(y0);
		y1 = degrees2sec(y1);
		break;
	case RADIANS:
		y0 = radians2sec(y0);
		y1 = radians2sec(y1);
		break;
	case FLOAT:
	default:
		y0 = y0;
		y1 = y1;
		break;
	}
}
#endif

// Draw a traditional PGPLOT linear axis. Much faster, but way off
// near a pole.
//static float xwin0, xwin1, ywin0, ywin1;

void AvAxis::drawLinearAxis(AvCanvas *c, const int XMASK, const int YMASK,
			    const int lineColor)
{ int xMask = XMASK;
  int yMask = YMASK;
  AvViewport *vp = c->viewport();
  const AvAxisOptions::axis *Xaxis = options_->getAxisOptions(
							AvAxisOptions::XAXIS);
  const AvAxisOptions::axis *Yaxis = options_->getAxisOptions(
							AvAxisOptions::YAXIS);

	// If we're showing raw positions, turn off any time conversions.
	if(showOffsets_)
	{	xMask &= ~(HMS | DMS);
		yMask &= ~(HMS | DMS);
	}

	// Decide how many axis labels to draw by default.
	float cvpw = vp->width();
	int rvpw = (int)(cvpw - (lm_ + rm_));	// width of scaled raster.
	float xMajorTickInterval;		// Interval between major ticks
	int xMinorIntervals;			// # minor ticks per major tick
	float yMajorTickInterval;
	int   yMinorIntervals;
	int nticks = numXTicks(rvpw);		// # of tick marks to draw.
#if 0
	float worldWidth = xwin1_ - xwin0_;	// Width in world coords.
	float worldHeight= ywin1_ - ywin0_;	// Height. (may be negative).
#else
	float worldWidth = Xwin1_ - Xwin0_;	// Width in world coords.
	float worldHeight= Ywin1_ - Ywin0_;	// Height. (may be negative).
#endif
	float xtick0 = worldWidth/nticks;	// X interval in world coords.
	// Rounded to something nice.
	xMajorTickInterval = pgrnd(xtick0, xMinorIntervals);

	int charspertic = CHARS_PER_TICK;
	if(showOffsets_)
	{char tmp0[64], tmp1[64];
		// Approximate how long the label will be.
		sprintf(tmp0, "%#.4g", Xwin0_);
		sprintf(tmp1, "%#.4g", Xwin1_);
		int l0 = (int)strlen(tmp0);
		int l1 = (int)strlen(tmp1);
		charspertic = (l0 > l1) ? l0 : l1;
		charspertic += 3;	// Empirical fudge factor.
//		printf("%s %s\n", tmp0, tmp1);
	}
	int nxchars = charspertic*nticks;	// # chars along X axis.

	float charscl = getBoxCharScale(int(cvpw), nxchars, xMask, yMask);
	// Use global character scale to adjust numeric labels.
	float cscl = (float) options_->gCharScale_.getValue();
	charscl *= cscl;
	cpgsch(charscl);		// Set base character scale.

	// Override computed values for tickmarks?
	if(xMask & USERMAJORINTERVALS)
	{int intrvl = Xaxis->linMajorIntervals_.getValue();
#if 0
		if(intrvl == 1)
			xMajorTickInterval = worldWidth*0.999999;
		else
		if(intrvl > 0)
#endif
			xMajorTickInterval = worldWidth/(float)intrvl;

		xMajorTickInterval = pgrnd(xMajorTickInterval,xMinorIntervals);
		// When displaying a time axis,if the tickmark interval is
		// the same as the world size, PGPLOT says it's larger.
		// This twiddle shouldn't affect non-time results too much.
		if(ABS(xMajorTickInterval) >= ABS(worldWidth))
			xMajorTickInterval = worldWidth*0.999999;
	}

	if(xMask & USERMINORINTERVALS)
		xMinorIntervals = Xaxis->linMinorIntervals_.getValue();

	// Get the number of intervals and convert it into a width.
	if(yMask & USERMAJORINTERVALS)
	{int intrvls = Yaxis->linMajorIntervals_.getValue();
		if(intrvls == 1)
			yMajorTickInterval = worldHeight*0.999999;
		else
		if(intrvls > 0)
			yMajorTickInterval = worldHeight/(float)intrvls;
		else
			yMajorTickInterval = 0.0;

		if(yMajorTickInterval != 0.0)
		yMajorTickInterval = pgrnd(yMajorTickInterval,yMinorIntervals);
	}
	else
		yMajorTickInterval = 0.0; // Default is to let PGPLOT decide.

	if(yMask & USERMINORINTERVALS)
		yMinorIntervals = Yaxis->linMinorIntervals_.getValue();
	else
		yMinorIntervals = 0;

#if 0
#if 0
	int nxchars = CHARS_PER_TICK*nticks;	// # chars along axis.
	// Adjusted # of ticks to draw.
	int nticks2 = int(worldWidth/xMajorTickInterval);
	nxchars = CHARS_PER_TICK*nticks2;	// Adjusted # of chars
#else
	int charspertic = CHARS_PER_TICK;
	if(showOffsets_)
	{char tmp0[64], tmp1[64];
		// Approximate how long the label will be.
		sprintf(tmp0, "%#.4g", Xwin0_);
		sprintf(tmp1, "%#.4g", Xwin1_);
		int l0 = strlen(tmp0);
		int l1 = strlen(tmp1);
		charspertic = (l0 > l1) ? l0 : l1;
		charspertic += 3;	// Empirical fudge factor.
//		printf("%s %s\n", tmp0, tmp1);
	}
	int nxchars = charspertic*nticks;	// # chars along axis.
	// Adjusted # of ticks to draw.
	int nticks2 = int(worldWidth/xMajorTickInterval);
	nxchars = charspertic*nticks2;	// Adjusted # of chars
#endif
#endif
	char xopts[80], yopts[80];

	// Draw the axis labels separately so that the lines can be
	// a different color.
	char * xstr = makeOptionString(xMask, xopts);
	char * ystr = makeOptionString(yMask, yopts);
	cpgtbox( xstr, xMajorTickInterval, xMinorIntervals,
		 ystr, yMajorTickInterval, yMinorIntervals);
	
	cpgsci(lineColor);
	
	// Draw the box and grid.
	// If zero lines with tickmarks was requested,
	// they are drawn here.
	xstr = makeLineOptionString(xMask, xopts);
	ystr = makeLineOptionString(yMask, yopts);

	cpgtbox(xstr, xMajorTickInterval, xMinorIntervals,
		ystr, yMajorTickInterval, yMinorIntervals);
}

// Curvilinear - more accurate, but slower.
void AvAxis::drawCurviLinearAxis(const int xMask, const int yMask,
				 const int lineColor)

{const AvAxisOptions::axis *Xaxis = options_->getAxisOptions(
							AvAxisOptions::XAXIS);
 const AvAxisOptions::axis *Yaxis = options_->getAxisOptions(
							AvAxisOptions::YAXIS);
 int axen[2];
 int labctl, labden=0, ci[7], gcode[2];
 char opt[] = "  ";
// const char *idents[3];	// Axis IDs. (Unused).
 int ng1=0, ng2=0;
 double tiklen= 0.01, grid1[1]={0}, grid2[1]={0};
 int doeq = 0;
 static const int NLC=10, NLI=10, NLD=10, NCACHE=100;
 int nlc=NLC, nli=NLI, nld=NLD, nc=NCACHE, ic=0;
 char nlcprm[NLC+1];
 int nliprm[NLI];
 double nldprm[NLD];
 double cache[4*(NCACHE+1)];	// cache indexes go from 0 to NCACHE.
 int ierr;

#if 0
	// Get label strings.
	if(Xlabel->drawLabel_.getValue())
		idents[0] = Xlabel->text_.getString();
	else
		idents[0] = NULL;

	if(Ylabel->drawLabel_.getValue())
		idents[1] = Ylabel->text_.getString();
	else
		idents[1] = NULL;

	if(Title->drawLabel_.getValue())
		idents[2] = Title->text_.getString();
	else
		idents[2] = NULL;
//#else
//		idents[0] = idents[1] = idents[2] = NULL;

#endif

	nlcprm[0] = '\0';

	// drawCurviLinearAxis should only have been called if raster
	// is a subclass of AvCLUIRaster.
	AvCLUIRaster *ras = (AvCLUIRaster *) raster_;

	axen[0] = ras->width();
	axen[1] = ras->height();

	// How to display each axis. (DMS, HMS, etc.)
	int xcode = Xaxis->format_.getValue();
	int ycode = Yaxis->format_.getValue();
	opt[0] = axisCode(xcode, xMask);
	opt[1] = axisCode(ycode, yMask);

	// Label density.
	{ int xd, yd;		// X,Y label densities.

		if(xMask & USERMAJORINTERVALS)
			xd = Xaxis->crvMajorIntervals_.getValue();
		else
				xd = 0;

		if(yMask & USERMAJORINTERVALS)
			yd = Yaxis->crvMajorIntervals_.getValue();
		else
				yd = 0;

		labden = yd*100 + xd;
		globalOptions()->setSteps(xd, yd);
	}

	// Where to put labels. pgcrvl can display each of X&Y labels on
	// all four sides depending on how curvy the grids are.
	labctl = 0;

	if(Xaxis->top_.getValue())
		labctl += 100;
	if(Xaxis->bottom_.getValue())
		labctl += 1;
	if(Xaxis->left_.getValue())
		labctl += 10;
	if(Xaxis->right_.getValue())
		labctl += 1000;

	if(Yaxis->top_.getValue())
		labctl += 200;

	if(Yaxis->bottom_.getValue())
		labctl += 2;

	if(Yaxis->left_.getValue())
		labctl += 20;

	if(Yaxis->right_.getValue())
		labctl += 2000;

	// Turn off labeling if nothing was enabled.
	// We don't support pgcrvl figuring out the positions.
	if(labctl == 0)
		labctl = -1;

	//cout << "Labctl " << labctl << endl;

	// Grid lines and labels for an axis are the same color.
	// The default color is used for the frame.
	// If the chosen color is the background color (0), then make labels
	// the default color.
	int defaultColor;
	cpgqci(&defaultColor);
	if(Xaxis->useGlobalColor_.getValue())
		ci[0] = lineColor;
	else
		ci[0] = Xaxis->color_.getValue();

	if((ci[0] == 0) || !Xaxis->labelTracksGrid_.getValue())
		ci[2] = defaultColor;
	else
		ci[2] = ci[0];

	if(Yaxis->useGlobalColor_.getValue())
		ci[1] = lineColor;
	else
		ci[1] = Yaxis->color_.getValue();

	if((ci[1] == 0) || !Yaxis->labelTracksGrid_.getValue())
		ci[3] = defaultColor;
	else
		ci[3] = ci[1];

	// label string aren't shown, but fill in a color anyway.
	ci[4] = defaultColor;
	ci[5] = defaultColor;
	ci[6] = defaultColor;

	// Decide on whether to draw grids or tick marks.
	gcode[0] = gcode[1] = 0;	// Don't draw either.
	{int cmd;
		if(xMask & GRID)	// Is same for x and y.
			cmd = 2;	// Draw grid
		else
			cmd = 1;	// Just tickmarks.

		if(xMask & MAJORTICKS)
			gcode[0] = cmd;

		if(yMask & MAJORTICKS)
			gcode[1] = cmd;
	}

//cout << "gcode[0] " << gcode[0] << endl;

	// pgcrvl takes a long time to calculate the coordinate value
	// extrema. However, the values may be saved and reused. This
	// saves a LOT of time. However, if the data itself changes, this
	// could mess up.

	if(globalOptions()->haveExtrema())
	{	ic = 0;
		globalOptions()->getExtrema(cache);
	}
	else
		ic = -1;

#ifdef TIMEIT
static clock_t time0, time1;

	time0 = clock();
#endif
	ierr = 0;

	// Use global character scale for numeric labels.
	float cscl = (float) options_->gCharScale_.getValue();
	cpgsch(cscl);

	// There isn't really a good way to pass a pointer arg via FORTRAN
	// so we just go around and make it globally available.
	gOptions_ = globalOptions();
	cpgcrvl(axen, NULL, opt,
		labctl, labden, ci, gcode,
		tiklen, ng1, grid1, ng2,
		grid2, doeq, nlfunc, nlc, nli,
		nld, nlcprm, nliprm, nldprm,
		nc, ic, cache, ierr);
#ifdef TIMEIT
	time1 = clock();
	cout << "pgcrvl took " << (time1 - time0)/1000 << " ms." << endl;
	cout << "World to Pixel took " << w2p1/1000 << " ms." << endl;

#if 0
	// Print cache hit/seek & nlfunc call info.
	{ int ch, cs;
		globalOptions()->getCounts(ch, cs);
		cout << "cache hits " << ch << " cache seeks " << cs << endl;
	cout	<< "nlfunc calls: opcode:1 " << oc1 << " opcode:2 " << oc2
		<< " opcode:-1 " << ocm1 << endl;
		globalOptions()->resetCounts();
	}
#endif
#endif
	if(!globalOptions()->haveExtrema())
		globalOptions()->setExtrema(cache);

	if( xMask & FRAME)	// same for both x & y.
	{	cpgsci(lineColor);
		cpgbox("BC", 0.0, 0, "BC", 0.0, 0);	// Draw frame.
	}

}

static inline double offset(const double frac,
			    const double win0, const double win1)
{	return (win1-win0)*frac;
}


// Generate the Axis.
void AvAxis::generate(AvCanvas *c)
{
AvViewport *vp;
AvViewportSetter *myvps;
float cvpw, cvph;	// Canvas' viewport size.
const AvAxisOptions::axisLabel *Xlabel = options_->getLabelOptions(
							AvAxisOptions::XLABEL);
const AvAxisOptions::axisLabel *Ylabel = options_->getLabelOptions(
							AvAxisOptions::YLABEL);
const AvAxisOptions::axisLabel *Title = options_->getLabelOptions(
							AvAxisOptions::TITLE);
const AvAxisOptions::axis *Xaxis = options_->getAxisOptions(
							AvAxisOptions::XAXIS);
const AvAxisOptions::axis *Yaxis = options_->getAxisOptions(
							AvAxisOptions::YAXIS);

	touch();
//cout << "Generate: " << raster_->getName() << endl;

	// Use the current canvas viewport to get size and raster scaling
	// information.
	vp = c->viewport();
	cvpw = vp->width();
	cvph = vp->height();

//****************************************************************
//****************************************************************
	//			Make Axis.

	// Create a PGPLOT driver object and tell it to draw into this.
	AvPGDriver *pgdriver = new AvPGDriver();
	pgdriver->setCanvas(c);
	// Create a viewport the same size as the current one.
	myvps = new AvViewportSetter(0.0, 0.0, cvpw, cvph);
	pgdriver->setVPS(myvps);
	pgdriver->newPolyline(this);
	AvPGDriver::currentDriver(pgdriver);

//****************************************************************
//			PGPLOT code
	// Open device.
	if(cpgbeg(0, "/CANVAS", 1, 1) != 1)
		return;
	cpgask(0);		// Don't ask.

	// Create a PGPLOT viewport the size of the current viewport-offsets.
	// Values are fractional (in the range (0..1)).
	// Drawing will be done inside this viewport.
//	float xm=cvpw-1, ym=cvph-1;		// Position, not size.
	float xm=cvpw, ym=cvph;
	float x0= lm_/xm,	x1=(xm-rm_)/xm;
	float y0= bm_/ym,	y1=(ym-tm_)/ym;
	cpgsvp(x0, x1, y0, y1);
#if 0
cout << "Axis X0/X1 " << x0 << " / " << x1
     << " Y0/Y1 " << y0 << "/" << y1 << endl;
#endif
	////////////////////////////////////////////////////////////////
	// Decide which type of axis drawing routine to use.
	Boolean mayUseCurvilinear =
		((goptions_ !=NULL) && goptions_->mayUseCurvilinearAxis());

	int axisType = options_->gAxisType_.getValue();
	Boolean useCurvilinear = (mayUseCurvilinear &&
		(axisType == AvAxisOptions::CURVILINEAR));
	////////////////////////////////////////////////////////////////
	int linmode = options_->gLinearAxisFormat_.getValue();

#if 0
	showOffsets_ = (axisType == AvAxisOptions::LINEAR) &&
		  (linmode != AvAxisOptions::DEFAULT);
#else
	showOffsets_ = (axisType == AvAxisOptions::LINEAROFFSET)
				||
		  (axisType == AvAxisOptions::ABSPIXEL);
#endif
	// Axes in seconds.
	Xwin0_=xwin0_; Xwin1_=xwin1_; Ywin0_=ywin0_; Ywin1_=ywin1_;
	float scl = 1.0;
	AxisGlobalOptions *opts = globalOptions();
	AvAccessor *acc = NULL;
	if(opts != NULL)
	{	acc = opts->accessor();
	}

	if(showOffsets_ && (acc != NULL))
	{  // Cursor position in array indices.
	   AvIPosition cursor(options_->gAxisOffsets_.getValue());
	   AvIPosition shape(acc->shape());
	   float xfrac = (float)cursor(0)/(float)(shape(0)-1);
	   float yfrac = (float)cursor(1)/(float)(shape(1)-1);

		double dsx0, dsx1, dsy0, dsy1;
		acc->XYZToWorld(0, 0, 0, 0, dsx0);
		acc->XYZToWorld(acc->getAxisLength(0)-1,
				acc->getAxisLength(1)-1,
				0,
				dsx1);
		acc->XYZToWorld(0, 0, 0, 1, dsy0);
		acc->XYZToWorld(acc->getAxisLength(0)-1,
				acc->getAxisLength(1)-1,
				1,
				dsy1);
		
		float xWin0, xWin1, yWin0, yWin1;
		xWin0 = -offset(xfrac, dsx0, dsx1);
		xWin1 = xWin0 + (dsx1-dsx0);
		yWin0 =  -offset(yfrac, dsy0, dsy1);
		yWin1 = yWin0 + (dsy1 - dsy0);

		Boolean allOffsets = !options_->gAngularOnly_.getValue();
		Boolean angXPos = (xAxisInput_ != FLOAT);
		Boolean angYPos = (yAxisInput_ != FLOAT);


		// ABSPIXEL is labeled 1..N.
		if(axisType == AvAxisOptions::ABSPIXEL)
		{	Xwin0_ = 1.0; Xwin1_ = acc->getAxisLength(0);
			Ywin0_ = 1.0; Ywin1_ = acc->getAxisLength(1);
		}
		else
		switch(linmode) {
		case AvAxisOptions::ARCMIN:
		case AvAxisOptions::ARCSEC:
		case AvAxisOptions::MARCSEC:
			scl = (linmode == AvAxisOptions::ARCMIN)
				? 60 : (linmode == AvAxisOptions::ARCSEC)
					? 3600.0 : 3600000.0;

			if(angXPos)
			{	Xwin0_ = xWin0*scl;
				Xwin1_ = xWin1*scl;
			}
			else
			if(allOffsets)
			{	Xwin0_ = -offset(xfrac, xwin0_, xwin1_);
				Xwin1_ = Xwin0_ + xwin1_ - xwin0_;
			}

			if(angYPos)
			{	Ywin0_ = yWin0*scl;
				Ywin1_ = yWin1*scl;
			}
			else
			if(allOffsets)
			{	Ywin0_ = -offset(yfrac, ywin0_, ywin1_);
				Ywin1_ = Ywin0_ + ywin1_ - ywin0_;
			}
				break;
		case AvAxisOptions::RELPIXEL:
			if(angXPos || allOffsets)
			{	Xwin0_ = -cursor(0);
				Xwin1_ = Xwin0_ + acc->getAxisLength(0) - 1;
			}
			if(angYPos || allOffsets)
			{	Ywin0_ = -cursor(1);
				Ywin1_ = Ywin0_ + acc->getAxisLength(1) - 1;
			}
				break;
		default:;
		case AvAxisOptions::DEFAULT:
			if(angXPos)
			{	Xwin0_ = xWin0;
				Xwin1_ = xWin1;
			}
			else
			if(allOffsets)
			{	Xwin0_ = -offset(xfrac, xwin0_, xwin1_);
				Xwin1_ = Xwin0_ + xwin1_ - xwin0_;
			}

			if(angYPos)
			{	Ywin0_ = yWin0;
				Ywin1_ = yWin1;
			}
			else
			if(allOffsets)
			{	Ywin0_ = -offset(yfrac, ywin0_, ywin1_);
				Ywin1_ = Ywin0_ + ywin1_ - ywin0_;
			}
				break;
		}
	}
	
	////////////////////////////////////////////////////////////////
	// Set world coordinate window.
	cpgswin( Xwin0_, Xwin1_, Ywin0_, Ywin1_);
	float worldWidth = Xwin1_ - Xwin0_;	// Width in world coords.
	////////////////////////////////////////////////////////////////
	/////		Global variables			///////
	////////////////////////////////////////////////////////////////

	// Line width
	int lineWidth = (int) options_->gLineWidth_.getValue();
	cpgslw(lineWidth);

	// Character font
	int fontNum = options_->gFont_.getValue();
	fontNum += 1;		// Goes from 1..n.
	cpgscf(fontNum);


	// Color used for drawing grid lines.
	int lineColor = options_->gAxisColor_.getValue();
	// (line color will get set when needed).

	// Pick up copies of the option masks since they may get changed.
	int xMask = getGlobalOptionMask(options_);
	int yMask = xMask;
	xMask |= getAxisOptionMask(Xaxis, xAxisOutput_, useCurvilinear);
	yMask |= getAxisOptionMask(Yaxis, yAxisOutput_, useCurvilinear);

	////////////////////////////////////////////////////////////////
	// Default character scale.
	int rvpw = (int)(cvpw - (lm_ + rm_));	// width of scaled raster.
	int nticks = numXTicks(rvpw);		// # of tick marks to draw.
	double xtick0 = worldWidth/nticks;	// X interval in world coords.
	// Rounded to something nice.
	int xMinorIntervals;
	float xMajorTickInterval = pgrnd(xtick0, xMinorIntervals);

	int nxchars = CHARS_PER_TICK*nticks;	// # chars along axis.
	// Adjusted # of ticks to draw.
	int nticks2 = int(worldWidth/xMajorTickInterval);
	nxchars = CHARS_PER_TICK*nticks2;	// Adjusted # of chars

	float charscl = getBoxCharScale(int(cvpw), nxchars, xMask, yMask);
	cpgsch(charscl);		// Set base character scale.
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	///		Draw box, grid and axis labels.
	////////////////////////////////////////////////////////////////

	cpgsave();
	// The method used to do the drawing depends on the type of raster.
	if(useCurvilinear)
		drawCurviLinearAxis(xMask, yMask, lineColor);
	else
		drawLinearAxis(c, xMask, yMask, lineColor);

	////////////////////////////////////////////////////////////////
	// Draw zero line(s) w/o any tickmarks.
	// This won't be accurate for near polar plots.
	if(xMask & ZEROLINENOTICKS)
	{ float x[2], y[2];
		x[0] = Xwin0_;
		x[1] = Xwin1_;
		y[0] = 0.0;
		y[1] = 0.0;
		cpgline(2, x, y);
	}
	if(yMask & ZEROLINENOTICKS)
	{ float x[2], y[2];
		y[0] = Ywin0_;
		y[1] = Ywin1_;
		x[0] = 0.0;
		x[1] = 0.0;
		cpgline(2, x, y);
	}
	cpgunsa();

	////////////////////////////////////////////////////////////////
	///		Draw Labels
	////////////////////////////////////////////////////////////////
	// This is a hack to get around the fact that each axis needs a
	// separate title, but there is only one variable to store it.
	options_->title_.text_.initialString(raster_->getName(), TRUE);

	if(xMask & SHOWLABEL)	// Either x or y Mask would work.
	{const char *title = Title->text_.getString();
	 int n;
		drawLabel(Xlabel, charscl);
		drawLabel(Ylabel, charscl);
		if((title != NULL) && ((n = strlen(title) > 0)))
		{
			// Force title to fit in area above the raster's
			// viewport. The title will take up about 75% of
			// the width of cvpw.
			float cscl = computeCharScale(n, cvpw*0.75, int(cvpw),
						      FALSE);
			
			if(cscl > 1.5*charscl)		// Limit size.
				cscl = 1.5*charscl;
			drawLabel(Title, cscl);
		}

		// Draw any captions.
		for(int nc=0; nc < options_->numCaptions_; nc++)
			drawLabel(options_->getCaptionOptions(nc), charscl);

	}

	////////////////////////////////////////////////////////////////
	///	Draw Markers
	////////////////////////////////////////////////////////////////
	int numMarkers = options_->numMarkers();
	int rWidth = raster_->width();
	int rHeight = raster_->height();
	for(int Marker=0; Marker < numMarkers; Marker++)
	{ const AvAxisOptions::marker *m = options_->getMarkerOptions(Marker);
		drawMarker(m, 1.0, 0, rWidth, 0, rHeight);
	}
	cpgend();

//****************************************************************
//****************************************************************
//****************************************************************

	delete pgdriver;
	haveAxis_ = TRUE;
}

void AvAxis::draw(AvCanvas &c)
{
	if(ignore())
		return;

	// If the viewport being drawn into has changed size, it is necessary
	// to recompute the axis.
	float cvpw = c.viewport()->width();
	float cvph = c.viewport()->height();
	if((vpw_ != cvpw) || (vph_ != cvph))
	{	vpw_ = cvpw;
		vph_ = cvph;
		touch();		// Force recomputation of axis.
	}

	// (Re) generate point list if there is no current list.
	if(!haveAxis())
		generate(&c);

	AvPolyline::draw(c);
}

void AvAxis::draw(AvCanvas *c)
{
	if(c != NULL)
		draw(*c);
}

void AvAxis::xLabel(const char *s)
{	options_->xLabel_.text_.initialString(s, TRUE);
	touch();
}

void AvAxis::yLabel(const char *s)
{
	options_->yLabel_.text_.initialString(s, TRUE);
	touch();
}

void AvAxis::title(const char *s)
{	options_->title_.text_.initialString(s, TRUE);
	touch();
}

// Copy a character string. Returns NULL if the argument is NULL.
char	*AvAxis::strDup(const char *str)const
{
int	len;
char	*s;

	if(str == NULL)
#if 1
		s = NULL;
#else
	{	s = new char[1];
		*s = '\0';
	}
#endif
	else
	{	len = (int)strlen(str);
		s = new char[len+1];
		strcpy(s, str);
	}

	return s;
}

// Size of window in world coordinates.
// AXISTYPE gives the input format for the axis.
// If the axistype is HOURS or DEGREES, the values will be converted to
// seconds.
void AvAxis::setWorldWindow(const float x0, const float x1,
			    const float y0, const float y1)
{
float xmin, xmax, ymin, ymax;

	switch (xAxisInput_) {
	case HOURS:
		xmin = hours2sec(x0);
		xmax = hours2sec(x1);
		break;
	case DEGREES:
		xmin = degrees2sec(x0);
		xmax = degrees2sec(x1);
		break;
	case RADIANS:
		xmin = radians2sec(x0);
		xmax = radians2sec(x1);
		break;
	case FLOAT:
	default:
		xmin = x0;
		xmax = x1;
		break;
	}

	switch(yAxisInput_) {
	case HOURS:
		ymin = hours2sec(y0);
		ymax = hours2sec(y1);
		break;
	case DEGREES:
		ymin = degrees2sec(y0);
		ymax = degrees2sec(y1);
		break;
	case RADIANS:
		ymin = radians2sec(y0);
		ymax = radians2sec(y1);
		break;
	case FLOAT:
	default:
		ymin = y0;
		ymax = y1;
		break;
	}

	if((xwin0_ != xmin) || (xwin1_ != xmax)
	   || (ywin0_ != ymin) || (ywin1_ != ymax))
	{	xwin0_ = xmin; ywin0_ = ymin;
		xwin1_ = xmax; ywin1_ = ymax;
		touch();
	}
}

void AvAxis::setMargins(const int lm, const int rm,
			const int tm, const int bm)
{

	lm_ = lm;
	rm_ = rm;
	tm_ = tm;
	bm_ = bm;
	touch();
}

// Generate a PGTBOX option string from an OPTIONS variable.
// Build an option string for tickmarks, frame and grid only.
char *AvAxis::makeLineOptionString(const int o, char *buf)const
{
char	*p = buf;

	*p = '\0';

	if(o & ZEROLINEWITHTICKS)
		*p++ = 'A';

	if(o & MAJORTICKS)
		*p++ = 'T';

	if(o & MINORTICKS)
		*p++ = 'S';

	if(o & FRAME)
	{	*p++ = 'B';	// Draw bottom of frame.
		*p++ = 'C';	// Draw top of frame.
	}

	if(o & GRID)
		*p++ = 'G';	// Draw grid lines.

	*p = '\0';
	return buf;
}

// Build up a PGPLOT option string with or w/o line parameters. If axisOptions
// is NULL, only none line parameters will be set.
char *AvAxis::makeOptionString(const int o, char *buf)const
{
char	*p = buf;

	*p = '\0';

	if(o & HMS)
	{	*p++ = 'Z';	// Time labels.
		*p++ = 'H';	// Use hms superscripts.
		*p++ = 'Y';	// Don't draw days fields.
	}
	else
	if(o & DMS)
	{	*p++ = 'Z';	// Time labels.
		*p++ = 'D';	// Use dms superscripts.
	}

	if(o & STDLABELPOSITION)
		*p++ = 'N';
	else
	if(o & NONSTDLABELPOSITION)
		*p++ = 'M';

	if(o & VERTYLABELS)
		*p++ = 'V';

	if(o & DECIMAL)
		*p++ = '1';
	else
	if(o & EXPONENTIAL)
		*p++ = '2';

	*p = '\0';
	return buf;
}

void AvAxis::maxCharSize(const float size)
{
	if(size != charsize_)
	{	charsize_ = size;
		touch();
	}
}

void AvAxis::showAxis(const Boolean show )
{
	ignore(show ? FALSE : TRUE);
}

Boolean AvAxis::showAxis()const
{
	return ignore() ? FALSE :TRUE ;
}

// Generate a PGTBOX option string from an OPTIONS variable.
// Build an option string for tickmarks, frame and grid only.
int AvAxis::getGlobalOptionMask(AvAxisOptions *options)const
{int o = 0;

	if(options->gDrawFrame_.getValue())
		o |= FRAME;

	if(options->gDrawGrid_.getValue())
		o |= GRID;

	// X,Y label or Title.
	if(options->gDrawLabels_.getValue())
		o |= SHOWLABEL;

	return o;
}

int AvAxis::getAxisOptionMask(const AvAxisOptions::axis *axis,
			      const AXISFORMAT af, const Boolean crvaxis)const
{int o = 0;

	if( af == HOURS)
		o |= HMS;
	else
	if( af == DEGREES)
		o |= DMS;

	switch(axis->drawZeroLine_.getValue()) {
	case AvAxisOptions::WITHTICKS:
		o |= ZEROLINEWITHTICKS;
		break;
	case AvAxisOptions::WITHOUTTICKS:
		o |= ZEROLINENOTICKS;
		break;
	default:;
	}

	// Tick marks OFF, use default intervals, use user supplied intervals.
	int intervalChoice = (crvaxis) ? axis->crvMajor_.getValue()
				       : axis->linMajor_.getValue();
	switch(intervalChoice) {
	case AvAxisOptions::OFF:
		break;
	case AvAxisOptions::ON:
		o |= USERMAJORINTERVALS;	// Fall through.
	case AvAxisOptions::DEFAULT:
		o |= MAJORTICKS;
		break;
	default:;
	}

	switch(axis->minor_.getValue()) {
	case AvAxisOptions::OFF:
		break;
	case AvAxisOptions::ON:
		o |= USERMINORINTERVALS;	// Fall through.
	case AvAxisOptions::DEFAULT:
		o |= MINORTICKS;
		break;
	default:;
	}

	switch(axis->position_.getValue()) {
	case AvAxisOptions::OFF:
		break;
	case AvAxisOptions::ALTERNATE:
		o |= NONSTDLABELPOSITION;
		break;
	default:
	case AvAxisOptions::NORMALPOSITION:
		o |= STDLABELPOSITION;
		break;
	}	

	if(axis->vertical_.getValue())
		o |= VERTYLABELS;

	switch(axis->decimal_.getValue()) {
	case AvAxisOptions::DECIMAL:
		o |= DECIMAL;
		break;
	case AvAxisOptions::EXPONENTIAL:
		o |= EXPONENTIAL;
		break;
	case AvAxisOptions::DEFAULT:
	default:
		break;
	}

	return o;
}

void AvAxis::globalOptions(AxisGlobalOptions *opt)
{
	if(opt == goptions_)
		return;
	if(goptions_ != NULL)
		goptions_->unref();
	goptions_ = opt;
	if(goptions_ != NULL)
		goptions_->ref();
	useRasterInfo(raster_);		// Fix up labels.
}

AxisGlobalOptions *AvAxis::makeGlobalOptions(AvRaster *r, const int sz)
{
	return new AxisGlobalOptions(r, sz);
}



// Scale factors to convert to seconds.
//static const double DEG2SEC = 240.0;
//static const double HRS2SEC = 3600;


////////////////////////////////////////////////////////////////
///			AxisGlobal Options
////////////////////////////////////////////////////////////////
// Used to hold caching information for displaying curvilinear axes.
// Isn't used otherwise.
AxisGlobalOptions::AxisGlobalOptions(AvRaster *ras, const int cachesize)
{
	accessor_ = NULL;
	haveExtrema_ = FALSE;
	mayUseExtrema_ = TRUE;
	mayUseCurvilinear_ = FALSE;
	cacheEnabled_ = TRUE;
	cacheSize_ = cachesize;

	if(cacheSize_ <= 0)
		cacheSize_ = 8192;
	XSteps_ = YSteps_ = -1;
	wXStep_ = wYStep_ = 1.0;
	xAxis_ = 0;
	yAxis_ = 1;
	zAxis_ = 2;

	raster_ = ras;
	if(raster_ != NULL)
	{	raster_->ref();
		if(raster_->isDerivedFrom(AvCLUIRaster::getClassTypeID()))
		{ AvCLUIRaster *cr = (AvCLUIRaster *)raster_;
			accessor_ = cr->accessor();
			accessor_->ref();
			int rank = accessor_->rank();
			world_.resize(rank);
			world_ = accessor_->maxRegion();
			wXMax_ = world_(0);
			wYMax_ = world_(1);
			world_ = accessor_->minRegion();
			wXMin_ = world_(0);
			wYMin_ = world_(1);
			ijk_.resize(rank);
			start_.resize(rank);
			// Unmapped starting position
			AvIPosition s = accessor_->start(FALSE);
			// Starting position as doubles.
			for(int i=0; i< rank; i++)
				start_(i) = (double)s(i);
			int xaxis = accessor_->xAxis();
			int yaxis = accessor_->yAxis();
			xAxis_ = accessor_->getAxisMapping(xaxis);
			yAxis_ = accessor_->getAxisMapping(yaxis);
			if(rank <= 2)
				zAxis_ = -1;
			else
				zAxis_ =
				 accessor_->getAxisMapping(accessor_->zAxis());
			xScale_.init(accessor_, xaxis);
			yScale_.init(accessor_, yaxis);

			// Need access to world to/from pixels to use.
			mayUseCurvilinear_ = TRUE;

#if defined(AIPSPLUSPLUS) && defined(GLISH)
			AvDataSet *ds = accessor_->dataset();
			// If we're dealing with an AvScrollingBufferDataSet,
			// we can't cache the extrema.
			if(strcmp(ds->dataSetType(), "Scrolling Buffer") == 0)
				mayUseExtrema_ = FALSE;
#endif
		}
		else
		{	wXMin_ = 0;
			wYMin_ = 0;
			wXMax_ = raster_->width()-1;
			wYMax_ = raster_->height()-1;
		}
	}
	else
	{	wXMin_ = 0;
		wYMin_ = 0;
		wXMax_ = 1.0;
		wYMax_ = 1.0;
	}

	px_.resize(cacheSize_);
	py_.resize(cacheSize_);
	wx_.resize(cacheSize_);
	wy_.resize(cacheSize_);
	inUse_.resize(cacheSize_);
	resetCache();
}

AxisGlobalOptions::~AxisGlobalOptions()
{
	if(raster_ != NULL)
		raster_->unref();
	if(accessor_ != NULL)
		accessor_->unref();
}

// Flush any cache entries.
void AxisGlobalOptions::resetCache()
{
	inUse_ = 0;	// Flag entry as not being used.
	cacheSeeks_ = 0;
	cacheHits_ = 0;
}

// Save world and pixel values in cache.
int AxisGlobalOptions::storeValues(const int index,
				   const double wx, const double wy,
				   const double px, const double py)
{
	if(validIndex(index))
	{	wx_(index) = wx;
		wy_(index) = wy;
		px_(index) = px;
		py_(index) = py;
		inUse_(index) = 1;
		return 1;
	}
	return 0;
}

// Convert args to an index and find their slot which may or may not
// have anything in it.
int AxisGlobalOptions::findEntry( const double wx, const double wy)
{int i;

	int index = getCacheIndex(wx, wy);

	for(i=index; i< cacheSize_; i++)
	{	if(inUse_(i) <= 0)
			return i;
		if((wx_(i) == wx) && (wy_(i)== wy))
			return i;
		cacheSeeks_++;
	}

	// Wrap around.
	if(index > 0)
		for(i=0; i< index; i++)
		{	if(inUse_(i) <= 0)
				return i;
			if((wx_(i) == wx) && (wy_(i)==wy))
				return i;
			cacheSeeks_++;
		}
	return -1;
}

void AxisGlobalOptions::getPValues(const int index, double &px, double &py)
{
	if(validIndex(index))
	{	px = px_(index);
		py = py_(index);
		cacheHits_ += 1;
	}
}

static inline unsigned long getI(const double z)
{
  union {
	double	d;
	unsigned long	m;
	} U;

	U.d = z;
	unsigned long m = U.m;
	while( m && ((m & 1) == 0))
		m = m >> 1;
	m &= 0xFFFF;	// Just keep 16 bits.
	return m;
}

int AxisGlobalOptions::getCacheIndex(const double wx, const double wy)
{
  // Step number. 0..w?Steps.
  double xd = (wx-wXMin_)/wXStep_;
  double yd = (wy-wYMin_)/wYStep_;

  unsigned long xm = getI(xd);
  unsigned long ym = getI(yd);
  unsigned long index = xm*ym;

	// Right shift away any zeros.
	while( index && ((index & 1) == 0))
		index = index >> 1;
	index = index % cacheSize_;
//	if(index < 0)
		index = cacheSize_-index;

  return (int)index;
}

// Set the number of steps (grid lines) to be printed. These are used
// in forming the cache index.
void AxisGlobalOptions::setSteps(const int Xsteps, const int Ysteps)
{double tmpX, tmpY;
 int xsteps=Xsteps, ysteps = Ysteps;

	if(xsteps <= 0)
		xsteps = 8;	// pgcrvl default is 8 steps.
	if(ysteps <= 0)
		ysteps = 8;

	if((xsteps == XSteps_) && (ysteps == YSteps_))
		return;

	XSteps_ = xsteps;
	YSteps_ = ysteps;

	tmpX = wXMax_-wXMin_;
	tmpX = ABS(tmpX);
	tmpX = tmpX/XSteps_;

	tmpY = wYMax_-wYMin_;
	tmpY = ABS(tmpY);
	tmpY = tmpY/YSteps_;
	wXStep_ = tmpX;
	wYStep_ = tmpY;
	resetCache();
}

// Reset statitstics counters.
void AxisGlobalOptions::resetCounts()
{
	cacheHits_ = 0;
	cacheSeeks_ = 0;
}

// Return statistices.
// hits		- # of requests for info that were in cache.
// seeks	- # of slots that needed to be searched to find entries.
//		  ( 0 would mean that all hits were at index points). 
void AxisGlobalOptions::getCounts( int &cacheHits, int &cacheSeeks)
{
	cacheHits = cacheHits_;
	cacheSeeks = cacheSeeks_;
}

// Set extrema that were generated by pgcrvl. Used to avoid having to
// recalculate them. (Very time consuming).
void AxisGlobalOptions::setExtrema(const double *vals)
{
	if(mayUseExtrema())
	{	memcpy(extrema_, vals, 4*sizeof(*vals));
		haveExtrema_ = TRUE;
	}
}

void AxisGlobalOptions::getExtrema( double *vals)const
{
	if(mayUseExtrema())
		memcpy(vals, extrema_, 4*sizeof(*vals));
}

// Convert world values to pixel. If caching is enabled and an entry
// is in the cache, use it. Otherwise, compute it.
// Due to the desire to display m/s as km/s, we scale world values after
// conversion from pixel and unscale before being converted back. The
// scale factor will be 1.0 when nothing needs to change.
void AxisGlobalOptions::WorldToPixel(const double world[], double pixel[])
{
	if(accessor_ == NULL)
		return;
	int rank = accessor()->rank();

	if(rank < 2)
		return;

	// Look for a an entry for x/y.
	int index;
	if(cacheEnabled_)
	{	index = findEntry(world[0], world[1]);
		if(index < 0)	// Not there, & full.
		{	resetCache();
			index = findEntry(world[0], world[1]);
		}
	}
	else
		index = -1;

	double x, y;

	// Use cache if available.
	if((index >= 0) && isHit(index))
		getPValues(index, x, y);
	else	// Compute and store.
	{
		world_(xAxis_) = xScale_.unscale(world[0]);
		world_(yAxis_) = yScale_.unscale(world[1]);
		accessor_->dataset()->WorldToIJK_(world_, ijk_);
		x = ijk_(xAxis_) - start_(xAxis_);
		y = ijk_(yAxis_) - start_(yAxis_);
		if((index >= 0) && cacheEnabled_)
			storeValues(index, world[0], world[1], x, y);
	}
	// Convert to PGPLOT indexing.
	pixel[0] = x + 1.0;
	pixel[1] = y + 1.0;
}

// Convert from pixel to world coordinates.
// pgcrvl assumes pixels range from 1..N and request conversions from
// .5 to N + .5. We subtract 1. Here and add 1 in WorldToPixel.
// We handle the mapping ourselves and go directly to a lower level DataSet
// function to speed things up. A side effect is that the pixel values
// are already doubles.
// Note: Calling this routine initializes world_.
void AxisGlobalOptions::PixelToWorld(const double pixel[], double world[])
{
	ijk_ = start_;
	ijk_(xAxis_) += pixel[0] - 1.0;
	ijk_(yAxis_) += pixel[1] - 1.0;
	// Get absolute plane index.
	// We could probably get along with leaving value 0.
	if(zAxis_ >= 0)
		ijk_(zAxis_) = accessor_->getPlane(FALSE);
	accessor_->dataset()->IJKToWorld_(ijk_, world_);
	world[0] = xScale_.scale(world_(xAxis_));
	world[1] = yScale_.scale(world_(yAxis_));
}

AxisGlobalOptions::ScaleInfo::ScaleInfo()
{
	needscale_ = FALSE;
	scaleFactor_ = 1.0;
	units_ = "";
	measurementName_ = "";
	scaledUnits_ = "";
	abbrev_ = "";
}

AxisGlobalOptions::ScaleInfo::~ScaleInfo() {}

// Compute scale factor needed to convert to 'scaled' version. (eg. m/s->km/s)
// RA & DEC aren't affected so their scale is 1.0. The primary purpose is
// to display velocity in KM/S. Technically, we should use valueInUnits()
// whenever we convert to/from world & pixel values, but we'll assume a
// linear scaling whose constant can be saved.
void AxisGlobalOptions::ScaleInfo::init(AvAccessor *accessor, const int axis)
{ AvDataSet::AXISPRINTFORMAT apfmt = accessor->axisPrintFormat(axis);

	units_ = accessor->axisUnitsName(axis);
	measurementName_ = accessor->axisMeasurementName(axis);
	scaledUnits_ = "";
	abbrev_ = "";

	if (apfmt == AvDataSet::HMS)
		needscale_ = FALSE;
	else if (apfmt == AvDataSet::DMS)
		needscale_ = FALSE;
	else
	{	// Otherwise, pick representative value on axis, get the units
		// that form the best fit, and use those to decide on units.

		AvWPosition minWorld = accessor->minWorld();
		AvWPosition maxWorld = accessor->maxWorld();
		double minW = minWorld(axis);
		double maxW = maxWorld(axis);
		double min = ABS(minW);
		double max = ABS(maxW);
		double representativeValue = MAX(min, max);

		AvString SclUnitsAbbrev;
		AvString SclUnits = accessor->axisUnits( axis,
						representativeValue,
						SclUnitsAbbrev);
		double tmp = accessor->valueInUnits(axis,
						representativeValue,
						SclUnits.chars());
		scaledUnits_ = SclUnits;
		abbrev_ = SclUnitsAbbrev;
		if(tmp != representativeValue)
			needscale_ = TRUE;
		scaleFactor_ = tmp/representativeValue;
	}
}

// Convert world position to/from its scaled value. If QUICKSCALE is
// TRUE, scaling is via a multiply/divide. Otherwise, call a routine that
// might do a better job (although currently it too just multiplies/divides).
double AxisGlobalOptions::ScaleInfo::scale_(const double v)const
{double vout;

	// Yes, this isn't needed since the header does it inline.
	if(QUICKSCALE)
		vout = v*scaleFactor_;
	else
		AvUnits::convertValue(v, units_, scaledUnits_, vout,
				      measurementName_);
	return vout;
}


double AxisGlobalOptions::ScaleInfo::unscale_(const double v)const
{double vout;

	if(QUICKSCALE)
		vout = v/scaleFactor_;
	else
		AvUnits::convertValue(v, scaledUnits_, units_, vout,
				      measurementName_);
	return vout;
}
