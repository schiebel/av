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
//# --------------------------------------------------
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvXCanvas.cc,v 19.0 2003/07/16 05:47:05 aips2adm Exp $
//
// $Log: AvXCanvas.cc,v $
// Revision 19.0  2003/07/16 05:47:05  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.1  2002/08/12 16:44:06  hravlin
// When ENABLE_TIMING is defined, use sysconf() if CLK_TCK is not available.
//
// Revision 18.0  2002/06/07 21:28:48  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.3  2002/01/30 22:45:03  hravlin
// g++ thought a couple '_t' variables could get used uninitialized.
//
// Revision 17.2  2002/01/09 18:20:53  hravlin
// Get BLANKEDVALUE from Av.h. It is back to being HUGE_VAL where available.
// Reordered includes.
//
// Revision 17.1  2002/01/03 21:42:35  hravlin
// Changes due to new AvRasterCache class and additions of flush() and wait().
//
// Revision 17.0  2001/11/12 19:42:17  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:25  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.1  2000/12/12 21:55:57  hravlin
// Removed compile restriction for pthreads.
//
// Revision 15.0  2000/10/26 17:09:48  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:08  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.8  2000/02/16 17:31:18  hravlin
// Fixed problem with CPP macro.
//
// Revision 13.7  1999/10/27 20:20:25  hravlin
// Found a for block scoping oops.
//
// Revision 13.6  1999/10/25 18:46:32  hravlin
// Modified imageRaster() to add experimental thread support.
// Removed scale() since its functionality was moved to AvXPalette.
//
// Revision 13.4  1999/09/28 21:55:33  hravlin
// setBusy() should not be called when displaying cached images.
//
// Revision 13.3  1999/09/28 19:14:26  hravlin
// Added display of "Busy" indicators if image takes a long time to display.
//
// Revision 13.2  1999/09/15 21:03:36  hravlin
// Numerous changes in support of TrueColor visuals.
//
// Revision 13.1  1999/08/25 19:34:46  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:39:55  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:29  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:32  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:19  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.5  1998/06/11 17:37:30  hr
// Removed a signed/unsigned comparision warning.
//
// Revision 9.4  1998/06/09 21:51:32  hr
// Changed arguments to scale to be mostly const and unsigned long for colors.
//
// Revision 9.3  1997/12/17 21:52:37  hr
// Changes to remove warnings from MIPSpro.7x compiler.
//
// Revision 9.2  1997/09/17 14:51:51  hr
// In AvXGraphicContext::update(), added conversion of line width to inches to
// track the definition in PGPLOT.
//
// Revision 9.1  1997/09/15 16:20:30  hr
// Conversion of line width from an enum to an int.
//
// Revision 9.0  1997/08/25 21:26:51  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:21  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.5  1997/02/14 20:43:31  hr
// In imageRaster(), removed attempt to limit the data & pixel buffer sizes.
// It now always reads all the pixels at once.
//
// Revision 7.4  1997/02/09 01:03:13  hr
// Fixed signed/unsigned comparisions.
// A case statement wasn't dealing with all of an enum.
//
// Revision 7.3  1997/02/05 18:01:45  hr
// Renamed SV_CLIPTO_COLOR to AV_CLIPTO_COLOR.
// Added setClearColor().
// update() was erroneously calling getForeground() when setting background.
//
// Revision 7.2  1996/12/12 09:54:18  droberts
// Explicit cast of getMaxBufLen() to an int.
//
// Revision 1.4  1996/11/12 20:22:08  hr
// Within imageRaster(), changed from calling getRow() to getRegion().
//
// Revision 1.3  1996/11/07 17:16:28  hr
// In imageRaster() the call to the raster's getRow function was passing
// the row length rather than the number of elements to be returned.
//
// Revision 1.2  1996/08/14 19:32:09  hr
// Changes to remove g++ warnings (from bglenden).
//
// Revision 1.1  1996/07/11  21:31:21  pixton
// Automated Checkin
//
//  Revision 1.23  1996/06/18  18:46:45  pixton
//  Copyright Update
//
//  Revision 1.22  1996/02/29  17:32:13  hr
//  Removed a commented out line of code.
//
//  Revision 1.21  1996/02/23  17:11:09  hr
//  The clipping rectangle is now changed only if the viewport size/position
//  changes.
//  Conversion from viewport Y to X11 Y was being done incorrectly. There
//  is now a routine to do it that is being used in several places.
//
//  Revision 1.20  1995/12/18  22:26:49  hr
//  Filled Polygons had an extra point at the beginning.
//
//  Revision 1.19  1995/10/09  21:48:25  hr
//  Rectangles were being drawn upside down. Negative widths/heights are
//  now accepted.
//
//  Revision 1.18  1995/10/07  15:52:25  hr
//  Reworked drawVector. Added segments, circles and arcs. Rectangles work
//  now.
//
//  Revision 1.17  1995/10/04  21:12:00  hr
//  XCanvas now gets blank value from the raster.
//
//  Revision 1.16  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.16  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.15  1995/07/20  17:19:13  hr
//  Replaced use of HUGE_VAL with MAXVALUE and BLANKEDVALUE.
//
//  Revision 1.14  1995/06/22  21:23:33  hr
//  Removed a bunch of "#if 0"s and changed "blank" to "blankcolor".
//
//  Revision 1.13  1995/06/22  18:41:38  hr
//  Added AvXGraphicContext code and modified canvas to use it.
//
//  Revision 1.12  1995/06/05  19:25:26  hr
//  Added dstx/y to imageRaster. Enabled blanking check with HUGE_VAL rather
//  than finite().
//
//  Revision 1.11  1995/05/24  14:03:07  hr
//  Added convertRasterScale.
//
//  Revision 1.10  1995/04/26  20:07:18  hr
//  Pass the scaled size of the raster to AvPixmap rather than scaling.
//
//  Revision 1.9  1995/04/24  21:55:12  hr
//  replaced calls to getRegion with calls to getRow.
//
//  Revision 1.8  1995/04/24  18:30:51  hr
//  Changes to the way pixmap caching was handled and started use of
//  raster.{begin,end}Access
//
//  Revision 1.7  1995/04/11  14:11:46  hr
//  Edits to remove compiler messages.
//
//  Revision 1.6  1995/04/07  20:41:01  hr
//  Draw directly to canvas when not caching.
//
//  Revision 1.5  1995/04/05  21:28:12  hr
//  Removed attempt at reducing XImage size
//
//  Revision 1.4  1995/04/05  20:38:46  hr
//  Removed some unused variables.
//
//  Revision 1.3  1995/04/05  20:02:35  hr
//  Added pixmap caching
//
//  Revision 1.2  1995/03/29  21:30:23  hr
//  Remove Graphic context, add drawcolor()
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvXCanvas.C
Handles Canvas functions for X. This is only a roughed in version and
a lot of changes will be made.

X Context isn't finished yet. (It may even get phased out).

*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>			// getenv
#include <sys/types.h>
#include <time.h>
#include <sys/times.h>
#include <limits.h>

#if defined(PTHREADS)
// Currently, multithreading is only available while scaling an image.
#include <pthread.h>
#include <semaphore.h>

/*SGI
  System dependent defines. May or may not be necessary.

 cc -D_POSIX_C_SOURCE=199506L app.c -llib0 -llib1 ... -lpthread
 SUN
 cc <file.c> lpthread
*/

#if !defined(ENABLE_TIMING)
#define ENABLE_TIMING
#endif

#endif		// PTHREADS

#if defined(ENABLE_TIMING)
// If we don't know how to get the value of CLK_TCK, disable timing.
#if !defined(CLK_TCK) && !defined(_SC_CLK_TCK)
#undef ENABLE_TIMING
#endif
#endif

#include <Xm/Xm.h>
#include <AvXCanvas.h>
#include <AvRaster.h>
#include <AvWidgetMisc.h>
#include <AvPolylineP.h>
#include <AvXCanvases.h>
#include <AvPixmapCache.h>
#include <AvApp.h>
#include <AvFallbacks.h>
#include <AvXColorApp.h>

const char *CLIPTO_COLOR = "AV_CLIPTO_COLOR";

#if defined(ENABLE_TIMING)
// To Convert from clock ticks to seconds. Set in constructor.
static float CLOCK_TICKF = 0.0;
#endif

/* Get environment integer value for environment variable str. This sort
of thing will be replaced by resources soon.
*/
static int getEnvInt(const char *str, const int def)
{
char	*env;
int	val;

	env = getenv(str);
	if((env == NULL) || (strlen(env) == 0))
		return def;

	val = (int) strtol(env, NULL, 0);
	return val;
}

AvXCanvas::AvXCanvas(Widget ref, AvXPalette *pal)
{
XGCValues vals;

	ref_ = ref;
	width_ = height_ = 0;
	// The viewport clipping rectangle.
	clipRect_.x = clipRect_.y = 0;
	clipRect_.width = clipRect_.height = 0;

	visual_ = widgetVisual(ref);
	// This may be reset by child class.
	depth_ = widgetDepth(ref);
	if(pal == NULL)
		pal = new AvXPalette(XtDisplay(ref), visual_);
	palette_ = pal;
	palette_->ref();

	unsigned long whitePixelP, blackPixelP;
	// Allocate & get virtual and physical values.
	palette_->allocNamedColor("White", whitePixel_, whitePixelP);
	palette_->allocNamedColor("Black", blackPixel_, blackPixelP);

	Window w = XtWindow(ref);
	if(w == 0)
		w = AvApp::referenceWindow();

	// Initialize.
	vals.foreground = whitePixelP;
	vals.background = blackPixelP;
	xgc_ = XCreateGC(XtDisplay(ref), w, GCForeground | GCBackground, &vals);

	AvXGraphicContext *xgc = new AvXGraphicContext(this, xgc_);
	state_.setGraphicContext( xgc);

	// This should reset xgc_ to any user set value.
	setDrawColor(getDrawColor());

	// This needs to be here until the GC code is working.
//	XChangeGC(XtDisplay(ref), xgc_, GCForeground | GCBackground, &vals);

	// Make a GC for clear. (To set areas to background color.
	vals.foreground = blackPixelP;
	vals.background = whitePixelP;
	xbgGC_ = XCreateGC(XtDisplay(ref), w,
				     GCForeground | GCBackground, &vals);
	pmcache_ = NULL;	// No caching.
	rasterTime_ = 0;
	rasterBytes_ = 0;

#if defined(ENABLE_TIMING)
#if !defined(CLK_TCK)
	long CLK_TCK = sysconf(_SC_CLK_TCK);
#endif
	CLOCK_TICKF = (float)CLK_TCK;
#endif

}

AvXCanvas::~AvXCanvas()
{
Display *dpy = display();

	if(palette_ != NULL)
		palette_->unref();

	if(pmcache_ != NULL)
		pmcache_->unref();

	XFreeGC(dpy, xgc_);
	XFreeGC(dpy, xbgGC_);
}

float AvXCanvas::convertScale(const float scale)
{
int	iscl;
float	scl;

	if(scale >= 1.0)
		scl = (float)((int) scale);
	else
	if(scale == 0.0)
		scl = 1.0;		// Error !!
	else
	{	iscl = (int)(1.0/scale + 0.5);
		scl = 1./(float)iscl;
	}

	return scl;
}

float AvXCanvas::convertRasterScale(const float scale)const
{
	return convertScale(scale);
}

// Scale something. (Also used in drawRaster).
static inline unsigned int scalesize(unsigned int a, int scl)
{
	if(scl == 0)		// error
		return	a;
	else
	if(scl < 0)
		return a/(-scl);
	else
		return a*scl;
}

/* Convert floating point scale (>0..1..) to int (+/- num).
If:
	val is out of range, return closest valid value.
//val is < 0, error	return 1;
//( 0 <= val < minval)	return 0;
//( minval <= val < 1)	return -1/val rounded up.
//( 1 <= val < maxval)	return (int) val.
*/

static int getscl(const float value)
{
int	scl;
float	min, max, val = value;

	// Range check.
	min = AvViewport::minRasterScale();

	if( val < min)
		val = min;
	else
	{	max = AvViewport::maxRasterScale();
		if(val > max)
			val = max;
	}

	val = AvXCanvas::convertScale(val); // Convert to 'integral' #.
	// Convert
	if(val < 1.0)
	{	val = -1.0/val;
		scl = (int)val;
	}
	else
		scl = (int) val;

	return scl;
}

unsigned int AvXCanvas::scaleSize(const AvDimension d,
		const unsigned int val)
{
float	xscl, yscl, scl;
int	newval;

	viewport()->getRasterScale(xscl, yscl);
	scl = (d == AvDimensionX) ? xscl : yscl;

	newval = (int)(val*convertRasterScale(scl));

	return (unsigned int) newval;
}

#if defined(ENABLE_TIMING)
// Clock time in secs.
static inline float hdeltaClock(const clock_t t0, const clock_t t1)
{
	return ((float)(t1-t0))/CLOCK_TICKF;
}

// CPU times in seconds.
static inline float deltaTimes( const struct tms &tms0,
				const struct tms &tms1)
{ clock_t ut = tms1.tms_utime - tms0.tms_utime;
  clock_t st = tms1.tms_stime - tms0.tms_stime;

	return ((float)(st+ut))/CLOCK_TICKF;
}
#endif

#if defined(PTHREADS)

// Used to pass scaling information for thread use.
struct scaleStruct {
	unsigned	num;
	pthread_t	id;
	AvXCanvas	*canvas;
	AvXPalette	*pal;
	float *in;
	int iWidth, iHeight;
	XImage *out;
	int	y0, dScale;
	float	Min, Max;
	unsigned long under, over, blank;
	float blankval;
	int	dstX, dstY;
	GC	xgc;
};

// Routine passed to pthread_create. Calls AvXPalette's scale routine then
// sends its chunk of data to the server.
static void *threadScale( void *arg)
{ scaleStruct *s = (scaleStruct *) arg;
  AvXCanvas *c = s->canvas;
  AvXPalette *pal = s->pal;
  XImage *out = s->out;

	pal->scaleFToX( s->in, s->iWidth, s->iHeight,
			out, s->y0, s->dScale,
			s->Min, s->Max,
			s->under, s->over, s->blank, s->blankval);

	AvApp::reserveX();
		XPutImage(c->display(), c->drawable(),
		  s->xgc, s->out, 0, 0, s->dstX, s->dstY,
		  out->width, out->height);
	AvApp::releaseX();

	return (0);
}
#endif

/*
Draw a raster into the drawable. Since this is used exclusively to
draw a complete scaled raster into a pixmap, there are no arguments
for subranging.

Experimental thread support:
	If the number of requested threads is > 1, the sequence
		read data.
		scale data and store in XImage structure.
		send pixels to server.
	is broken into
		read data.
			foreach thread
				scale/store 1/n of the data.
				send pixels to server.
 When nthreads is > 1, there will be upto nthreads-1 pthreads created. The
main thread does some of the work. If the max(#input data points, #output
pixels) is < kbPerThread (a settable value), the number of threads is
reduced.

Trying to break up the data read currently isn't done because:
	It tends to be the fastest part (at least with FITS files).
	AIPS++ slows way down on small reads.
	Haven't had time.

*/
void AvXCanvas::imageRaster(AvRaster &img, const int dstx, const int dsty,
			    const float Xscale, const float Yscale)
{
int	srcx=0, srcy=0;
unsigned int iwidth, iheight;	// src image width/height
char *data;
int	dHeight, dWidth;
XImage	*image;
float	mn, mx;
int	sxinc, syinc;
int	xscl, yscl;
unsigned long undercolor, overcolor, blankcolor;
#if defined(PTHREADS)
Bool	showTimings = getXOptions()->showTimings; // Print scaling times?
int	nthreads = getXOptions()->nThreads;	  // # of threads to use.
int	kbPerThread = getXOptions()->kbPerThread; // min kb/thread.
#endif

	// If the drawable is missing, punt.
	if(drawable() == 0)
		return;
	//		Range checks
	iwidth = img.width();		// Size of input.
	iheight= img.height();

	// Convert to signed ints.
	xscl = getscl(Xscale);
	yscl = getscl(Yscale);
	sxinc = (xscl >= 0) ? 1 : -xscl;
	syinc = (yscl >= 0) ? 1 : -yscl;

	// How much space is needed to store image?
	dWidth = scalesize(iwidth, xscl);
	dHeight = scalesize(iheight, yscl);

	/* Someplace to put data. This is overkill for 8 bit transfers,
	   but should always work.
	*/
	data = new char [dWidth*dHeight*sizeof(unsigned long)];

	// What colors to use for out of range pixels.
	// Use b/w normally, but if that won't work, use 0, max-1.
	// (Or if the environment variable is set).
	if( (palette()->availableColors() <= palette()->maxColors()-2)
				&&
		(getEnvInt(CLIPTO_COLOR, 0) == 0) )

	{	undercolor = blackPixel();
		overcolor = whitePixel();
		blankcolor = undercolor;
	}

	else
	{	undercolor = 0;
		overcolor = palette()->availableColors() -1; // Highest index.
		blankcolor = overcolor;
	}

	mn = img.min();		// Data min/max
	mx = img.max();

	/* Get data, scale and store in XImage struct. (0,0) is at lower left.
	*/

	float blankvalue = img.getBlankValue();

	/* Create an XImage struct for the information. */
	Display *dpy = display();

	/////////////////////////////////////////////////
	// Figure out the largest buffer size we can use.

	int w = (xscl >= 0) ? iwidth : dWidth; // # pixels/row we deal with.
	int h = (yscl >= 0) ? iheight: dHeight;// # rows we deal with.
	int npixels = w*h;	// # of elements to be retrieved from raster.

	unsigned long *pixels = new unsigned long[npixels]; // Pixel buffer.
	float	*dbuffer = new float[npixels];		 // Data buffer.

	img.beginAccess();
	// Get the image, scale it and then put it
	// into the XImage. Expansion is done in storeRow (now in AvXPalette).
	// Reduction is done in the raster.
#if defined(ENABLE_TIMING)
struct tms htime0;
clock_t hclock0 = times(&htime0);
#endif
	img.getRegion(dbuffer, srcx, srcy, w, h, sxinc, syinc);
	img.endAccess();
#if defined(ENABLE_TIMING)
struct tms htime1;
clock_t hclock1 = times(&htime1);
#endif
	// Scale pixels.
	int dScale = dHeight/h;
		if(dScale <= 0)
			dScale = 1;

#if defined(PTHREADS)
//	int nthreads = getEnvInt("NTHREADS", 0);	// # of threads;

	// Use OMP's environment variable if it is not overridden by ours.
	if( nthreads == 0)
		nthreads = getEnvInt("OMP_NUM_THREADS", 1);// # of threads, OMP.

	{ // If the # of pixels processed per thread is too small, reduce the
	  // # of threads.
	  int npD = dHeight*dWidth;
	  int npS = w*h;
	  int np = (npD >= npS) ? npD : npS;	// Max of Destination/Src.
	  int npT = np/(nthreads*1024);
		if(npT < kbPerThread)
			nthreads = npT/kbPerThread;
	}

	if( nthreads > 1)
	{ int h_per_thread = h/nthreads;
	  int h_for_this = h_per_thread + (h- h_per_thread*nthreads);
	  int oH_per_thread = dHeight/nthreads;
	  int oH_for_this = h_for_this*dScale;
	  // The main program is counted as 1 thread.
	  int npthreads = nthreads-1;
	  scaleStruct *scales = new scaleStruct[npthreads];
	  int nt;		// for loop index;

		// XImage struct for main thread.
		image = XCreateImage(dpy, visual_, depth_, ZPixmap,
			0 /*offset*/,
			data+npthreads*dWidth*oH_per_thread*sizeof(long),
			dWidth, oH_for_this,
			BitmapPad(dpy), 0);

#if defined(__sgi)
		// It looks like IRIX may default to 1 processor otherwise.
		pthread_setconcurrency(nthreads);
#endif
		for( nt=0; nt < npthreads; nt++)
		{ scaleStruct *s = &scales[nt];
			s->num = nt;
			s->canvas = this;
			s->pal = palette_;
			s->in = dbuffer + nt*w*h_per_thread;
			s->iWidth = w;
			s->iHeight = h_per_thread;
			AvApp::reserveX();
			 XImage *im = XCreateImage(dpy, visual_, depth_, ZPixmap,
				0 /*offset*/,
				data+nt*dWidth*oH_per_thread*sizeof(long),
				dWidth, oH_per_thread,
				BitmapPad(dpy), 0);
			AvApp::releaseX();
			s->out = im;
			s->y0 = oH_per_thread-1;
			s->dScale = dScale;
			s->Min = mn;
			s->Max = mx;
			s->under = undercolor;
			s->over = overcolor;
			s->blank = blankcolor;
			s->blankval = blankvalue;
			s->dstX = dstx;
			s->dstY = dHeight- oH_per_thread*(1+nt) -1;
			s->xgc = xgc_;

			int pterr = pthread_create(&s->id, NULL,
					threadScale, (void *) s);
			if(pterr)
				perror("AvXCanvas:");
		}

		// Main thread handles what's left over.
		palette_->scaleFToX(dbuffer+npthreads*w*h_per_thread,
				w, h_for_this, image, oH_for_this-1, dScale,
				mn, mx, undercolor, overcolor,
				blankcolor, blankvalue);
		AvApp::reserveX();
			XPutImage(dpy, drawable(), xgc_, image, 0, 0,
			  dstx, 0,
			  dWidth, oH_for_this);
		AvApp::releaseX();

		for( nt = 0; nt < npthreads; nt++)
		{	pthread_join(scales[nt].id, NULL);
			scales[nt].out->data = NULL;
			AvApp::reserveX();
				XFree(scales[nt].out);
			AvApp::releaseX();
		}

		delete [] scales;
	}
	else
#endif
	{	image = XCreateImage(dpy, visual_, depth_, ZPixmap,
			0 /*offset*/, data, dWidth, dHeight,
			BitmapPad(dpy), 0);

		palette_->scaleFToX(dbuffer, w, h, image,
				    dHeight-1, dScale,
				    mn, mx, undercolor, overcolor,
				    blankcolor, blankvalue);
		XPutImage(dpy, drawable(), xgc_, image, 0, 0, dstx, dsty,
		  dWidth, dHeight);

	}

#if defined(ENABLE_TIMING)

	if(showTimings)
	{ struct tms htime2;
		XSync(dpy, False);	// Only when timing.
		clock_t hclock2 = times(&htime2);

		// Display timings.
		float oBytes=dWidth*dHeight;

		printf("Pixel scale/store and write for %d threads.\n", nthreads);
		float deltaReadWall=hdeltaClock(hclock0, hclock1);
		float deltaReadCPU=deltaTimes(htime0, htime1);
		printf("\t\t\tWall\t\tCPU\n");
		printf("Read %5d bytes\t%.3fs\t\t%.3fs\n",
			npixels, deltaReadWall, deltaReadCPU);

		float deltaScaleWall= hdeltaClock(hclock1, hclock2);
		float deltaScaleCPU = deltaTimes(htime1, htime2);
		printf("Scale/store/Put\t\t%.3fs\t\t%.3fs\n",
			deltaScaleWall, deltaScaleCPU);
		printf(" %5d pixels\n", (int)oBytes);

		float deltaWall=deltaReadWall+deltaScaleWall;
		float deltaCPU=deltaReadCPU+deltaScaleCPU;
		printf("Total\t\t\t%.3fs\t\t%.3fs\n",
			deltaWall, deltaCPU);
		printf("Output\t\t\t%.2fkb/s\t%.2fkb/s\n",
			oBytes/(1000.0*deltaWall), oBytes/(1000.0*deltaCPU));
	}
#endif

	delete [] pixels;
	delete [] dbuffer;
	// Free data separately from image.
	delete [] data;
	image->data = NULL;
	XFree(image);
}

// Convert from VP y to X y coordinate.
static inline int vpY2XY_(const float xh, const float vpy)
{
	return (int)(xh - vpy + 0.5);
}

static inline int vpX2XX_(const float vpx, const float xoffset)
{
	return (int)(vpx + xoffset + 0.5);
}

// Sort of arbitrary number of seconds above which we'll display the
// busy flags.
static const float LONGTIME = .025;	// 25ms.

/*
srcx, srcy	Where in raster image the region should come from.
		(Normally 0 since entire image is drawn).
dstx,dsty	Offset within current viewport (Usually 0).
swidth,		How much of the image to draw. Usually 0 to indicate draw
sheight		as much as possible.


If the time to scale and dislay an image is "long", inform the user. If
the time is fairly quick, don't incur the overhead.  The check is on a per
image basis, and doesn't consider multiple image redisplays.

*/

void AvXCanvas::drawRaster(	AvRaster &img,
				int srcx, int srcy, int /*dstx*/, int /*dsty*/,
			unsigned int /*swidth*/, unsigned int /*sheight*/)
				// Requested width/height.
{
unsigned int iwidth, iheight;	// src image width/height
int	dstx, dsty;
float	xfact, yfact;
AvViewport *vp;
AvXPixmap *pmc;
AvRasterCacheKey *key;
time_t	time0 = 0;
clock_t	clock0 = 0;
Boolean	timeit=False;
Boolean showBusy = False;

	pmc = NULL;
	// Pick up current scaling.
	vp = viewport();
	vp->getRasterScale(xfact, yfact);

	// Get the size that imageRaster would(will) use.
	iwidth = img.width();
	iheight = img.height();
	iwidth = scaleSize(AvDimensionX, iwidth);
	iheight = scaleSize(AvDimensionY, iheight);
	dstx = vp->x0();
	dsty = vp->y0();
	// Need to convert from lower left corner to upper right corner.
	// (0,0 for Display list is lower left corner of viewport. (0,0)
	// for X is upper left corner of scaled image.
	dsty =  vpY2XY_(height(), dsty + iheight);

	int npixels = iwidth*iheight;
	if(npixels == 0)
		return;

	// If the number of pixels has changed, force a retime.
	if(npixels != (int)rasterBytes_)
		timeit = True;

	if(timeit)
	{	// Time both cpu and elapsed time. Since network overhead
		// might be a factor.
		time0 = time(NULL);
		clock0 = clock();
	}

	// If we are caching pixmaps, look for it.
	if(cachingRasters())
	{	// Look for an existing pixmap.
		key = new AvRasterCacheKey(&img, iwidth, iheight);
		pmc = pmcache_->find(*key);
		if(pmc == NULL)
		{// Not found. Create an XPixmap canvas and draw
		 // the scaled raster into it.

			if(rasterTime_ >= LONGTIME)
			{	showBusy = True;
				AvXColorApp::setBusy();
			}

			pmc = new AvXPixmap(ref_, getXPalette(),
						iwidth, iheight, depth());
			if(pmc == NULL)
			{	delete key;
				return;		// Error.
			}
			pmc->imageRaster(img, 0, 0, xfact, yfact);
			// Save in cache.
			pmcache_->insert(*key, pmc);
		}
		delete key;
		// Now, copy the pixmap to the output drawable.
		// (NOTE:: Ignoring offsets, etc. for now!!!
		copyRegion(*pmc, *this, srcx, srcy, 0, 0, dstx, dsty);
	}
	else	// No cache, just draw.
	{	 // Never show busy if from cache.
		this->imageRaster(img, dstx, dsty, xfact, yfact);
	}

	if(timeit)
	{ time_t time1 = time(NULL);
	  clock_t clock1 = clock();

		float walltime = (float)(time1-time0);	// Clock time.
						// Will be 0 in most cases.
		float cputime = (float)(clock1-clock0)/CLOCKS_PER_SEC;
		if(walltime > cputime)
			rasterTime_ = walltime;
		else
			rasterTime_ = cputime;
		rasterBytes_ = npixels;
	}
	if(showBusy)
		AvXColorApp::resetBusy();
}

void AvXCanvas::drawRaster(AvRaster *img)
{
	if(img == NULL)
		return;
	drawRaster( *img);
}

// Set the viewport, then set clip plane.
void AvXCanvas::setViewport(AvViewport &vp)
{
	AvCanvas::setViewport(vp);
	if(changeFullState())
		setClipPlane();
}

// Reset state, then set clip plane.
void AvXCanvas::popState()
{
	AvCanvas::popState();
	if(changeFullState())
		setClipPlane( viewport());
}

// Copy an area from one canvas to another.
void AvXCanvas::copyRegion(AvXCanvas &src, AvXCanvas &dst,
			int srcx, int srcy,
			unsigned int rwidth, unsigned int rheight,
			int dstx, int dsty)
{
int	dwidth, dheight;
unsigned int swidth, sheight, width, height;
Display *dpy;

	if( (dst.drawable()==0) || (src.drawable()==0))
		return;
	// Check to make sure some things are sane.
	if( (dpy=dst.display()) != src.display())
		return;
	// Both widgets need to share the same root (window?), but we're
	// not checking here.

	//		Range checks
	/* If the width or height haven't been set, ignore request. */
	dwidth = dst.width();
	dheight = dst.height();
	if((dstx < 0) || (dstx >= dwidth))
		return;
	if((dsty < 0) || (dsty >= dheight))
		return;

	swidth = src.width();
	sheight= src.height();
	if((srcx < 0) || (srcx >= (int)swidth))
		return;
	if((srcy < 0) || (srcy >= (int)sheight))
		return;

	// Width is minimum of src, dst and requested except rwidth=0
	// means use available.
	width = (dwidth <= (int)swidth) ? dwidth : swidth;
	if((rwidth > 0) && (rwidth < width))
		width = rwidth;
	height = (dheight <= (int)sheight) ? dheight : sheight;
	if((rheight > 0) && (rheight < height))
		height = rheight;

	if((srcx + width) > swidth)
		width = swidth - srcx;
	if((int)(dstx + width) > dwidth)
		width = dwidth - dstx;
	if((srcy + height) > sheight)
		height = sheight - srcy;
	if((int)(dsty + height) > dheight)
		height = dheight - dsty;

#if 0
printf("copyRegion: sx/y %d/%d width/height %d/%d, dstx/dsty %d/%d\n",
	srcx, srcy, width, height, dstx, dsty);
#endif
	XCopyArea(dpy, src.drawable(), dst.drawable(), dst.xgc_, srcx, srcy,
		width, height, dstx, dsty);
}

void AvXCanvas::clear(int x, int y, unsigned int wdth, unsigned int hght)
{
unsigned int	W, H;
//Drawable d = drawable();

//	if( (d = drawable()) == (Drawable) NULL)	// Not mapped yet.
	if( drawable() == (Drawable) NULL)	// Not mapped yet.
		return;
	if((x < 0) || (x >= (int)width()))
		return;
	if((y < 0) || (y >= (int)height()))
		return;

	W = width() - x;
	if((wdth > 0) && (wdth < W))
		W = wdth;
	H = height() - y;
	if((hght > 0) && (hght < H))
		H = hght;

	XFillRectangle(display(), drawable(), xbgGC_, x, y, W, H);
}

// Set the color used to clear drawables. If physical is TRUE,
// ci is a physical color index, otherwise it is a virtual color.
// (We should be using the AvGraphicContext, but that needs work).
void AvXCanvas::setClearColor(const unsigned long ci, const Boolean physical)
{unsigned long color;
 XGCValues vals;

	if(physical)
		color = ci;
	else
		palette_->virtualToPhysical(ci, color);
	vals.foreground = color;
	XChangeGC(XtDisplay(ref_), xbgGC_, GCForeground, &vals);
}


/********************************************************************/
/********************************************************************/
/*			Support routines			    */
/********************************************************************/
/********************************************************************/
/********************************************************************/

/************************************************************************/
/*			Polyline drawing				*/
/************************************************************************/

/* Convert from fractional value (0..1) to vp coords (0.. size-1).
	If scale is NULL, nothing is done.
*/
inline static void frac2full( float &x, float &y, float *scale)
{
	if(scale != NULL)
	{	x *= scale[0];
		y *= scale[1];
	}
}

/* Convert from vp to X coords. Y is flipped since vp (0,0) is in the
lower left, while X's (0,0) is in the upper right. Then the X/Y offset
of the viewport in the X window is added.
*/

//	Used for points lines, polygons.
inline static void vp2XPnt(XPoint *lst, const float xvp, const float yvp,
		const float xoff, const float yoff, const float maxY)
{
float	x, y;

	x = vpX2XX_(xvp, xoff);
	y = vpY2XY_(maxY, yvp + yoff);
	lst->x = (short)x;
	lst->y = (short)y;
}

//		Used for rectangles.
// X's sense of +y is different than ours, so besides flipping the y value,
// the height has to be modified too. While we're at it, just
// handle +/- widths/heights.
// Ignores errors like 0 width/height.
//inline static void vp2XRec(XRectangle *lst,
static void vp2XRec(XRectangle *lst,
	const float xvp, const float yvp,
	const float width, const float height,
	const float xoff, const float yoff, const float maxY)
{
float	x, y;
int	xMin, yMin, xMax, yMax;

	x = vpX2XX_(xvp, xoff);
	y = vpY2XY_(maxY, yvp + yoff);

	xMin = (short)(x+0.5);
	yMin = (short)(y+0.5);
	xMax = xMin + (short) (width+0.5);
	yMax = yMin - (short) (height+0.5);	// Flip direction for Y.
	if(xMin > xMax)
	{ short tmp = xMin;
		xMin = xMax;
		xMax = tmp;
	}
	if(yMin > yMax)
	{ short tmp = yMin;
		yMin = yMax;
		yMax = tmp;
	}
	lst->x = xMin;
	lst->y = yMin;
	lst->width = (unsigned short) (xMax-xMin);
	lst->height = (unsigned short) (yMax-yMin);
}

#if 0
static int pc = 0;
#endif

//	Used for segments.
inline static void vp2XSeg(XSegment *lst,
	const float X1, const float Y1,
	const float X2, const float Y2,
	const float xoff, const float yoff, const float maxY)
{
float	x1, y1, x2, y2;

	x1 = vpX2XX_(X1, xoff);
	y1 = vpY2XY_(maxY, Y1 + yoff);
	lst->x1 = (short)x1;
	lst->y1 = (short)y1;

	x2 = vpX2XX_(X2, xoff);
	y2 = vpY2XY_(maxY, Y2 + yoff);
	lst->x2 = (short)x2;
	lst->y2 = (short)y2;

#if 0
// The pgplot driver does segments.
if(pc++ < 4)
 printf(
  "vp %.2f/%.2f->%.2f/%.2f, %.2f/%.2f->%.2f/%.2f off %.2f/%.2f maxY %.2f\n",
		X1,Y1, x1, y1, X2,Y2, x2, y2, xoff, yoff, maxY);
#endif
}

//	Used for circles and arcs.
inline static void vp2XArc(XArc *lst,
	const float xvp, const float yvp,
	const float width, const float height,
	const float angle1, const float angle2,
	const float xoff, const float yoff, const float maxY)
{
float	x, y;

	x = vpX2XX_(xvp, xoff);
	y = vpY2XY_(maxY, yvp + yoff);
	lst->x = (short)x;
	lst->y = (short)y;
	lst->width = (unsigned short) width;
	lst->height = (unsigned short) height;
	lst->angle1 = (short)(angle1*64.0);	// X arcs take degrees*64.
	lst->angle2 = (short)(angle2*64.0);
}

/* Converts from relative to absolute by adding the current (relative)
position to the previous position. Both positions are set to the
new value.
*/
inline static void rel2abs(float &xcurr, float &ycurr,
				float &xprev, float &yprev)
{
	xcurr += xprev;
	ycurr += yprev;
	xprev =  xcurr;
	yprev =  ycurr;
}

#if 0
struct {
	int	n;
	char	*name;
	} cmds[] = {
		{NONE, "NONE"},
		{MOVE, "MOVE"},
		{POINT, "POINT"},
		{LINE, "LINE"},
		{RECTANGLE, "RECTANGLE"},
		{POLYGON, "POLYGON"},
		{SEGMENT, "SEGMENT"},
		{CIRCLE, "CIRCLE"},
		{ARC, "ARC"}
	};
#define NCMDS (sizeof(cmds)/sizeof(*cmds))

static char *cmd2str(const int mode)
{
	for(int i=0; i< NCMDS; i++)
		if(cmds[i].n == mode)
			return cmds[i].name;

	return "Unknown";
}
#endif

/* Draw polylines (points, lines, polygons, etc). Moves are handled by
updating the current position. X draws are done in absolute mode, but
at some point may be done in relative mode since it will be easier to
cache the values when that support is added later.
*/

void AvXCanvas::drawVector(AvPolylineCmd *cmd)
{
float		maxY, maxPos[2], *scale;
float		xoff, yoff;	// Offset of viewport.
long		numpnts;
Boolean		relative, fractional;
AvViewport	*vp;
PointList	*points;	// Pointer to list of points.

	if((cmd == NULL) || ((points = cmd->positionList()) == NULL))
		return;

	if((numpnts = points->size()) <= 0)
		return;

#if 0
pc = 0;	// For segment tracing.
#endif
	vp = viewport();

	// Size of viewport.
	maxPos[0] = vp->width() -1;
	maxPos[1] = vp->height() - 1;
	maxY = height() - 1;

	// Offset of the viewport within X window.
	xoff = vp->x0();
	yoff = vp->y0();

	relative = (cmd->mode_ == AvPolyline::ABSOLUTE) ? FALSE : TRUE;
	fractional = (cmd->cmode_ == AvPolyline::FRACTIONAL);
	scale = fractional ? maxPos : (float*)NULL;

	/* For each point we need to:
		Convert to viewport coords if its fractional.
		Convert to absolute coords if its relative.
		Update the current position.
		Convert from absolute vp coords to X by
		Adding 'offset' to position within the pixmap and subtracting
		from height to flip the y 0 position.
		Convert to XPoint(etc.) and add to list.
	*/

	points = cmd->positionList();	// Pointer to the points.

	/* For relative to absolute conversion need to keep track of the
	   previous point. The first previous point is the initial position.
	*/
	AvAllotment currentPosition = vp->vectorPosition();
	float currX = currentPosition.x();
	float prevX = currX;
	float currY = currentPosition.y();
	float prevY = currY;

	switch(cmd->cmd_) {
	case MOVE:	// Compute new current point.
		points->getValue(0, &currX, &currY);
		frac2full(currX, currY, scale);
		if(relative)
			rel2abs(currX, currY, prevX, prevY);
		break;
	case LINE:
	case POLYGON:
	case POINT:

/* NOTES: Need to check. We may be drawing an extra point (at the start).
	(lines, polygons and points probably don't need exactly the same
lists).
Rectangles, circles, polygons, etc, probably shouldn't update the current
point.
*/

		{ numpnts += 1;		// Count the initial 'move'.
		  XPoint *plst = new XPoint[numpnts+1], *ptr = plst;
					// (space to close unfilled polygon).

		  /* The initial draw is from the current position to
		     the first point.
		  */
			vp2XPnt(ptr++, prevX, prevY, xoff, yoff, maxY);
			for( int i=0; i< numpnts-1; i++)
			{	// Get x/y from list.
				points->getValue(i, &currX, &currY);
				// Convert to vp from fractional.
				frac2full(currX, currY, scale);
				if(relative)
					rel2abs(currX, currY, prevX, prevY);

				// convert to x coord and store in list.
				vp2XPnt(ptr++, currX, currY, xoff, yoff,
								   maxY);
			}

			switch( cmd->cmd_) {
			case LINE:
				XDrawLines(display(), drawable(), xgc_,
					plst, (int) numpnts, CoordModeOrigin );
				break;
			case POLYGON:
				if(cmd->filled_)
				// Only need the N points of the polygon.
				 XFillPolygon(display(), drawable(), xgc_,
					plst+1, (int) (numpnts-1), cmd->shape_,
					CoordModeOrigin /*CoordModePrevious*/);
				else
				{	// Close polygon.
					plst[numpnts++] = plst[0];
					XDrawLines(display(), drawable(), xgc_,
					plst, (int) numpnts, CoordModeOrigin);
				}
				break;
			case POINT:
				XDrawPoints(display(), drawable(),
					xgc_, plst, (int)numpnts,
					CoordModeOrigin);
				break;
			default:;
			}

			delete [] plst;
		}
		break;
	case RECTANGLE:		// To keep the old compiler happy.
		{ 
		  XRectangle *rlst = new XRectangle[numpnts], *ptr = rlst;
		  PointList *aux = cmd->auxList();
		  float Width, Height;
			for( int i=0; i< numpnts; i++)
			{	// Get x/y from list.
				points->getValue(i, &currX, &currY);
				// Convert to vp from fractional.
				frac2full(currX, currY, scale);
				if(relative)
					rel2abs(currX, currY, prevX, prevY);

				aux->getValue(i, &Width, &Height);
				frac2full(Width, Height, scale);

				// convert to x coord and store in list.
				vp2XRec(ptr++, currX, currY, Width, Height,
					xoff, yoff, maxY);
			}

		  if(cmd->filled_)
			  XFillRectangles(display(), drawable(), xgc_,
					  rlst, (int)numpnts);
		  else
			  XDrawRectangles(display(), drawable(), xgc_,
					  rlst, (int)numpnts);
		  delete [] rlst;
		}
		break;
	case SEGMENT:
		{ 
		  XSegment *slst = new XSegment[numpnts], *ptr = slst;
		  PointList *aux = cmd->auxList();
		  float x2, y2;
			for( int i=0; i< numpnts; i++)
			{	// Get x/y from list.
				points->getValue(i, &currX, &currY);
				// Convert to vp from fractional.
				frac2full(currX, currY, scale);
				if(relative)
					rel2abs(currX, currY, prevX, prevY);

				aux->getValue(i, &x2, &y2);
				frac2full(x2, y2, scale);
				if(relative)
					rel2abs(x2, y2, prevX, prevY);

				// convert to x coord and store in list.
				vp2XSeg(ptr++, currX, currY, x2, y2,
					xoff, yoff, maxY);
			}

		  XDrawSegments(display(), drawable(), xgc_, slst, (int)numpnts);
		  delete [] slst;
		}
		break;
	case CIRCLE:
	case ARC:
		{ 
		  XArc *alst = new XArc[numpnts], *ptr = alst;
		  PointList *aux = cmd->auxList();
		  PointList *aux2 = cmd->aux2List();
		  float ewidth, eheight, angle1, angle2;
			for( int i=0; i< numpnts; i++)
			{	// Get x/y from list.
				points->getValue(i, &currX, &currY);
				// Convert to vp from fractional.
				frac2full(currX, currY, scale);
				if(relative)
					rel2abs(currX, currY, prevX, prevY);

				aux->getValue(i, &ewidth, &eheight);
				frac2full(ewidth, eheight, scale);

				if(aux2 != NULL)
				{	aux2->getValue(i, &angle1, &angle2);
//					frac2full(angle1, angle2, scale);
				}
				else
				{	angle1 = 0.0;	// Circle.
					angle2 = 360;
					eheight = ewidth;	// Radius
				}
				vp2XArc(ptr++, currX, currY, ewidth, eheight,
					angle1, angle2,
					xoff, yoff, maxY);
			}

		  if(cmd->filled_)
		  	XFillArcs(display(), drawable(), xgc_, alst,
				  (int)numpnts);
		  else
			XDrawArcs(display(), drawable(), xgc_, alst,
				  (int)numpnts);

		  delete [] alst;
		}
		break;
	case NONE:
	default:
		break;
	}

	AvAllotment tmp(currX, currY);
	viewport()->vectorPosition(tmp);
}

// Set clipping rectangle. Request is ignored if same as previous.
void AvXCanvas::setClipPlane(const AvAllotment &offset,
					const AvAllotment &size)
{
int x,y;
unsigned short w,h;

	w = (unsigned short)size.value(AvDimensionX);
	h = (int)size.value(AvDimensionY);
	x = (int)offset.value(AvDimensionX);
	y = (int)offset.value(AvDimensionY);
	// Flip from ll to ul.
 	y = vpY2XY_(height()-1, h + y);
	// Ignore request if same as current
	if((w != clipRect_.width) || (h != clipRect_.height) ||
	   (x != clipRect_.x) || (y != clipRect_.y))
	{	clipRect_.x = x;
		clipRect_.y = y;
		clipRect_.width = w;
		clipRect_.height = h;
		XSetClipRectangles(display(), xgc_, 0, 0, &clipRect_, 1,
								YSorted);
#if 0
		printf("setClipPlane:: X %d, Y %d, W %d, H %d\n",
			clipRect_.x, clipRect_.y,
			clipRect_.width, clipRect_.height);
#endif
	}
}

void AvXCanvas::setClipPlane(AvViewport &vp)
{
	setClipPlane(vp.xy(), vp.wh());
}

void AvXCanvas::setClipPlane(AvViewport *vp)
{
	if(vp == NULL)
		vp = viewport();
	setClipPlane( *vp);
}

Drawable AvXCanvas::drawable() {return 0;}
Display *AvXCanvas::display() {return XtDisplay(ref_);}
unsigned int AvXCanvas::width() {return width_;}
unsigned int AvXCanvas::height() {return height_;}
int AvXCanvas::depth() {return depth_;}

AvPalette *AvXCanvas::palette()const
{	return  palette_;
}

AvXPalette *AvXCanvas::getXPalette()const
{	return (AvXPalette *) palette_;
}

void AvXCanvas::setXPalette(AvXPalette *pal)
{
	if(palette_ != NULL)
		palette_->unref();

	palette_ = pal;
	if(palette_ != NULL)
		palette_->ref();
}

/* Set the color used to draw vectors and text. */
void AvXCanvas::setDrawColor(const unsigned long color)
{
#if 0
AvXPalette *pal = getXPalette();
unsigned long pindex;

	if( pal->virtualToPhysical(color, pindex))
	{XGCValues vals;

		vals.foreground = pindex;
		XChangeGC(display(), xgc_, GCForeground, &vals);
		AvCanvas::setDrawColor(color);
	}
#else
	state_.graphicContext()->setForeground(color);
#endif
}

void AvXCanvas::setDrawColor(const char *colorname)
{
AvXPalette *pal = getXPalette();
unsigned long pindex, vindex;

	if( pal->allocNamedColor( colorname, vindex, pindex))
		state_.graphicContext()->setForeground(vindex);
}

void AvXCanvas::setRasterCacheParameters(const Boolean enable, const int n)
{
	if(n >= 0)
	{	if(pmcache_ != NULL)		// Remove old cache.
		{	pmcache_->unref();
			pmcache_ = NULL;
		}
		if(n > 0)			// Insert new.
		{	pmcache_ = new AvPixmapCache(n);
			pmcache_->ref();
		}
	}
	if(pmcache_ != NULL)
		pmcache_->enable(enable);
}

void AvXCanvas::getRasterCacheParameters(Boolean &enable, int &size)
{
	enable = cachingRasters();
	if(pmcache_ != NULL)
		size = pmcache_->size();
	else
		size = 0;
}

Boolean AvXCanvas::cachingRasters()const
{
	if(pmcache_ != NULL)
		return pmcache_->enabled();
	else
		return FALSE;
}

void AvXCanvas::setRasterCache(AvPixmapCache *cache)
{
	if(cache == pmcache_)
		return;
	if(pmcache_ != NULL)
		pmcache_->unref();
	pmcache_ = cache;
	pmcache_->ref();
}

// Flush any cached pixmaps.
void AvXCanvas::flushRasterCache()
{
	pmcache_->flush();
}

void AvXCanvas::flush()
{
	XFlush(display());
}

void AvXCanvas::wait()
{
	XSync(display(), 0);
}

/****************************************************************/
/****************************************************************/
/****************************************************************/
/*			Graphic context code.			*/
/****************************************************************/
/****************************************************************/
/****************************************************************/


AvXGraphicContext::AvXGraphicContext( AvXCanvas *can, GC gc) :
						AvGraphicContext()
{
	canvas_ = can;
	gc_ = gc;
	background_ = can->blackPixel();
	foreground_ = can->whitePixel();
	update();
}

AvXGraphicContext::AvXGraphicContext(const AvGraphicContext &gc):
						AvGraphicContext(gc)
{
	canvas_ = NULL;
	gc_ = 0;
}

void AvXGraphicContext::copy(const AvGraphicContext &gcin)
{
	canvas_ = NULL;
	gc_ = 0;
	AvGraphicContext::copy(gcin);
}

void AvXGraphicContext::copy(const AvXGraphicContext &gcin)
{
	canvas_ = gcin.canvas_;
	gc_ = gcin.gc_;
	AvGraphicContext::copy(gcin);
	update();
}

// Emulate the = operator for plain graphic contexts.
AvGraphicContext& AvXGraphicContext::operator=(const AvGraphicContext &gcin)
{
	AvGraphicContext::copy(gcin);
	return *this;
}

AvXGraphicContext& AvXGraphicContext::operator=(const AvXGraphicContext &gcin)
{
	copy(gcin);
	return *this;
}

GC AvXGraphicContext::gc()
{
	if(modified())		// Make sure it's up to date.
		update();
	return gc_;
}

AvXGraphicContext::~AvXGraphicContext()
{
}

void AvXGraphicContext::setDrawMode(const DrawMode d)
{
	AvGraphicContext::setDrawMode(d);
	update(AvGraphicContext::DRAWMODE);
}

#if 0
void AvXGraphicContext::setLineWidth(const AvXGraphicContext::LineWidth lw)
{
	AvGraphicContext::setLineWidth(lw);
	update(AvGraphicContext::LINEWIDTH);
}
#else
void AvXGraphicContext::setLineWidth(const int lw)
{
	AvGraphicContext::setLineWidth(lw);
	update(AvGraphicContext::LINEWIDTH);
}
#endif

void AvXGraphicContext::setLineStyle(const LineStyle ls)
{
	AvGraphicContext::setLineStyle(ls);
	update(AvGraphicContext::LINESTYLE);
}

void AvXGraphicContext::setFont(const Font f)
{
	AvGraphicContext::setFont(f);
	update(AvGraphicContext::FONT);
}

void AvXGraphicContext::setForeground(const unsigned long c)
{
	AvGraphicContext::setForeground(c);
	update(AvGraphicContext::FOREGROUND);
}

void AvXGraphicContext::setBackground(const unsigned long c)
{
	AvGraphicContext::setBackground(c);
	update(AvGraphicContext::BACKGROUND);
}

void AvXGraphicContext::update( const int msk)
{
XGCValues vals;
unsigned long mask= 0;
int	m;

	if((canvas_ == NULL) || (gc_ == 0))
		return;

	if(msk == 0)
		m = AvGraphicContext::ALLMASK;
	else
		m = msk;

	if(m & AvGraphicContext::DRAWMODE)
	{	mask |= GCFunction;
		switch (getDrawMode()) {
		case AvGraphicContext::COPY:
				vals.function = GXcopy;
				break;
		case AvGraphicContext::XOR:
				vals.function = GXxor;
				break;
		case AvGraphicContext::INVERT:
				vals.function = GXinvert;
				break;
		default:
			mask &= ~GCFunction;	// oops
		}
	}

	if(m & AvGraphicContext::FOREGROUND)
	{AvXPalette *pal = canvas_->getXPalette();
	 unsigned long color;

		if(pal->virtualToPhysical(getForeground(), color))
		{	vals.foreground = color;
			mask |= GCForeground;
		}
	}

	if(m & AvGraphicContext::BACKGROUND)
	{AvXPalette *pal = canvas_->getXPalette();
	 unsigned long color;

		if(pal->virtualToPhysical(getBackground(), color))
		{	vals.background = color;
			mask |= GCBackground;
		}
	}


	// Should replace constants with resources.
	if(m & AvGraphicContext::LINEWIDTH)
	{
#if 0
		mask |= GCLineWidth;
		switch (getLineWidth()) {
		case  AvGraphicContext::DEFAULTLINEWIDTH:
			vals.line_width = 0;
			break;
		case AvGraphicContext::THIN:
			vals.line_width = 1;
			break;
		case AvGraphicContext::MEDIUM:
			vals.line_width = 2;
			break;
		case AvGraphicContext::WIDE:
			vals.line_width = 4;
			break;
		default:
			mask &= ~GCLineWidth;
		}
#else
		int lw = getLineWidth();
		if(lw >= 0)
		{ // Currently, the Canvas library doesn't apply a meaning
		  // to line width. However, in order to get X output to
		  // track PGPLOT, we're currently applying the same
		  // conversion. ( lw=1 is .005").

			{ Widget ref = canvas_->referenceWidget();
			  Screen *s = XtScreen(ref);
			  int wp = WidthOfScreen(s);
			  int wmm = WidthMMOfScreen(s);
			  float xdpi = (25.4*wp)/(float)wmm;
			  lw = (int)(lw*xdpi*.005);
			}

			mask |= GCLineWidth;
			vals.line_width = lw;
		}
#endif
	}

	if(m & AvGraphicContext::LINESTYLE)
	{	mask |= GCLineStyle;
		switch(getLineStyle()) {
		case AvGraphicContext::SOLID:
			vals.line_style = LineSolid;
			break;
		case AvGraphicContext::DASHED:	// Resource??
			vals.line_style = LineOnOffDash;
			break;
		// There should be a dotted here.
		default:
			mask &= ~GCLineStyle;
		}
	}

#if 0
	if(m & FONT)
	{	font_ = font_;
	if(m & DASHTYPE)
	{	
dashtype_ = dashtype_;
	if(m & DASHLIST)
		setDashList(dashlistlen_, dashlist_);
	if(m & DASHOFFSET)
		dashoffset_ = dashoffset_;
#endif

	if(mask != 0)
		XChangeGC(canvas_->display(), gc_, mask, &vals);
#if 0
{ Widget ref = canvas_->referenceWidget();
  Screen *s = XtScreen(ref);
  int wp = WidthOfScreen(s);
  int hp = HeightOfScreen(s);
  int wmm = WidthMMOfScreen(s);
  int hmm = HeightMMOfScreen(s);
  cout	<< "Width = " << wp << "/" << wmm
	<< " Height = " << hp << "/" << hmm << endl;
  float xdpi = 25.4*wp/(float)wmm;
  float ydpi = 25.4*hp/(float)hmm;
  cout << "XDPI = " << xdpi << " YDPI = " << ydpi << endl;
}
#endif

}
