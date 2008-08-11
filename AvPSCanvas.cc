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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvPSCanvas.cc,v 19.0 2003/07/16 05:48:14 aips2adm Exp $
//
// $Log: AvPSCanvas.cc,v $
// Revision 19.0  2003/07/16 05:48:14  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:47  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/03 20:21:34  hravlin
// Included strings.h
//
// Revision 17.0  2001/11/12 19:43:15  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:22  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:57  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:09:19  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.2  1999/09/16 20:06:55  hravlin
// Removed egcs and IRIX -64 compiler warnings.
//
// Revision 13.1  1999/09/15 21:23:09  hravlin
// Initial checkin.
//
//---------------------------------------------------------------------------
/* AvPSCanvas.cc
Canvas -> PostScript interface.
(AvPSDriver is the PostScript->Canvas interface).

This still needs more work particularly in drawVector where not all
polyline objects have been implemented.

This isn't likely to work well as a stand alone canvas. It pretty much
assumes the display list was generated for something else and is being
printed out.

The available colors from the reference palette are copied. The palette
is setup thusly:
	|named colors|<modifyable colors>
The modifyable colors are layed out as:
	<underflow color><colors from reference><overflow color>

However, the current setup is configured to map underrange values to
the reference's minimum value and overrange values to the reference's
maximum value. Blanked data will use the overflow color which is set to
white.
*/

#include <stdio.h>
#include <stdlib.h>			// getenv
#include <ctype.h>			// isalnum

#include "AvPSCanvas.h"
#include "AvRaster.h"
#include "AvPolylineP.h"
#include "AvViewportSetter.h"
#include "AvPalette.h"
#include "AvString.h"
#include "AvFallbacks.h"		// for getXOptions().
#include <string.h>
#include <strings.h>

class AvPSPalette : public AvPalette {
  public:
	AvPSPalette(AvPSCanvas &psc, AvPalette *ref=NULL);
	AvPalette *referencePalette()const{return ref_;}
	unsigned long blackIndex()const{return blackIndex_;}
	unsigned long whiteIndex()const{return whiteIndex_;}
  protected:
	virtual ~AvPSPalette();
	// AvPalette calls these to do I/O to the physical color table.
	// Copy dirty entries to physical cm.
	// For decomposed index colormaps, the subclass will use the call as
	// an opportunity to convert colors_' RGB values to pixels.
	virtual void putColors();
	// Copy physical cm to internal. The force flag forces the write
	// if possible. Otherwise, it's up to the subclass.
	virtual void getColors(const Boolean force=False);
	// Allocate named color.
	virtual Boolean allocNamedColor_(const char *colorname,
			unsigned long &pixel, float &r, float &g, float &b);

	Boolean canvasOpened()const {return psc_->opened();}
	virtual void checkAllocated_();
  private:
	void initialize();
	AvPSDriver *psd()const{return psd_;}
  private:
	AvPSCanvas *psc_;
	AvPalette *ref_;
	AvPSDriver *psd_;
	unsigned long blackIndex_;	// Index of black.
	unsigned long whiteIndex_;
};

// Maximum # of colors in PS level 2 colormap.
static const int MAX_PS_COLORS = 4096;

// The reference palette is used to get the initial color table. Also,
// virtual to physical translations are done using it.
AvPSPalette::AvPSPalette(AvPSCanvas &psc, AvPalette *ref) : AvPalette(0)
{
	psc_ = &psc;
	ref_ = ref;
	if(ref_ != NULL)
	{	ref_->ref();
	}
	psd_ = psc_->PSDriver();
	psd_->ref();
	initialize();
}

AvPSPalette::~AvPSPalette()
{
	if(ref_ != NULL)
		ref_->unref();
	psd_->unref();
}

void AvPSPalette::checkAllocated_()
{
}

#if 0
static void pcte(AvColortableEntry *e)
{
	cout << *e << endl;
}
#endif

/*
Write colormap.

red, green, blue	Color values
start			starting index
len			length of red, green and blue arrays.
			If len is 0 or > <color table length> - start,
			<color table length> - start entries are stored.

Writes the RW color cells.
*/

void AvPSPalette::putColors()
{
int	num;

	// Maximum # of colors to deal with.
	num = maxColors();

	int *indexes = new int[num];
	float *a = new float[num];
	float *b = new float[num];
	float *c = new float[num];


	// Scan the array looking for entries that have been marked
	// changed and build a list of those entries.
	int numcolors = 0;
	for(int i=0; i< num; i++)
	{ AvColortableEntry *ct = &colors_[i];
		if(ct->flags() != 0)
		{	indexes[numcolors] = (int)ct->pixel();
			ct->get( a[numcolors], b[numcolors], c[numcolors]);
			numcolors += 1;
			ct->setFlags(0);
		}
	}
	psd_->storeColorValues(numcolors, indexes, a, b, c);

	delete [] indexes;
	delete [] a;
	delete [] b;
	delete [] c;

}

// Read colormap from device.
// If force is TRUE, do the read. Otherwise, assume the table is current.
void AvPSPalette::getColors(const Boolean /*force*/)
{
}

void AvPSPalette::initialize()
{
//float	*r, *g, *b;
//int maxOurColors, maxRefColors, maxcolors, availRefColors;
int maxOurColors, maxRefColors, maxcolors;

	if(ref_ == NULL)
		return;
	expandTable( MAX_PS_COLORS);

	maxcolors_ = maxOurColors = MAX_PS_COLORS;
	maxRefColors = ref_->maxColors();
	// Minimum of the two.
	maxcolors = (maxOurColors <= maxRefColors) ?
						maxOurColors : maxRefColors;

	{int rlen = 0;
	 int blacki= -1, whitei = -1;
		 /* Copy the colors from the reference colormap to ours.
		 */
		const AvColortableEntry *refcolors = getColortable(ref_);
		const unsigned short *refmap = getMap(ref_);

		for(int i=0; i< maxcolors; i++)
		{// float r, g, b;

			colors_[i] = refcolors[i];
			map_[i] = refmap[i];
			// Pixel values are indexes even if X is TrueColor.
			colors_[i].setPixel(i);
			// Flag as dirty.
			if(isAllocated( &colors_[i]))
			     colors_[i].setFlags(AvColortableEntry::RGBALL);
			if(colors_[i].isReadWrite())
				rlen += 1;
			if(strcasecmp(colors_[i].name(), "black") == 0)
				blacki = colors_[i].index();
			else
			if(strcasecmp(colors_[i].name(), "white") == 0)
				whitei = colors_[i].index();
		}
		// This exchanges the black and white colors since the
		// X canvas draws on a black background and we're probably
		// drawing on a white background.
		if( (blacki >= 0) && (whitei >= 0))
		{ int bci = map_[blacki];
		  int wci = map_[whitei];

#if 0
cout << "WCI is " << wci << " BCI is " << bci << endl;
cout << " blacki = " << blacki << " whitei = " << whitei << endl;
cout << colors_[wci] << endl;
cout << colors_[bci] << endl;
#endif
			map_[blacki] = wci;
			map_[whitei] = bci;
			colors_[wci].setIndex(blacki);
			colors_[bci].setIndex(whitei);

			// These are supposed to point to what's REALLY
			// black and white.
			blackIndex_ = whitei;
			whiteIndex_ = blacki;
#if 0
cout << "White is " << whiteIndex_ << " black is " << blackIndex_ << endl;
cout << colors_[map_[whiteIndex_]] << endl;
cout << colors_[map_[blackIndex_]] << endl;
#endif
		}
		else
		{	blackIndex_ = whiteIndex_ = 0;	// Probably an error.
		}
		rwcells_ = rlen;
	}
	putColors();
}

// Dummy, but needed since AvPalette is virtual.
	// Allocate named color.
#if 0
Boolean AvPSPalette::allocNamedColor_(const char *colorname,
			unsigned long &pixel, float &r, float &g, float &b)
#else
Boolean AvPSPalette::allocNamedColor_(const char *colorname,
			unsigned long &, float &, float &, float &)
#endif
{
	printf("AvPSPalette::allocNamedColor %s\n", colorname);
	return FALSE;
}


////////////////////////////////////////////////////////////////


// Environment variables to use to get PostScript device and filename.
static const char *PS_FILE = "AV_PS_FILE";
static const char *PS_DEVICE = "AV_PS_DEVICE";
static const char *DEFAULT_DEVICE = "PORTRAIT";

// Initial scale factor 256 pixels should give 4".
static const float PIXEL2INCH = 4.0/256;

/* Get environment string for environment variable str.
*/
static const char *getEnvStr(const char *str, const char *def=NULL)
{
const char	*env;

	env = getenv(str);
	if((env == NULL) || (strlen(env) == 0))
		return def;
	else
		return env;
}

/*
ref	Reference palette. Its contents are copied to ours.
psd	The PostScript driver to use.
x/yscale Used to convert from 'pixel' to Points. Typically, the values
	 would be:
		<Page width>/<Viewport width>
		<Page height>/<View port height>
*/

AvPSCanvas::AvPSCanvas( AvPalette *ref, AvPSDriver *psd,
			const float xscale, const float yscale)
{
	psd_ = psd;
	psd_->ref();

	xscale_ = xscale;
	yscale_ = yscale;
	// Image options.
	imageOpts_ = 0;


	palette_ = new AvPSPalette(*this, ref);
	if(palette_ != NULL)
		palette_->ref();

	// scale and offsets are set in open();
	scale_ = PIXEL2INCH;
	xoff_ = yoff_ = 0.0;
	opened_ = FALSE;
	AvPSGraphicContext *pggc = new AvPSGraphicContext(this);
	state_.setGraphicContext( pggc);
	open();
}

AvPSCanvas::~AvPSCanvas()
{
	close();
	if(palette_ != NULL)
		palette_->unref();
	psd_->unref();
}

/*

These are ignored.
srcx, srcy	Where in raster image the region should come from.
		(Normally 0 since entire image is drawn).
dstx,dsty	Offset within current viewport (Usually 0).
swidth,		How much of the image to draw. Usually 0 to indicate draw
sheight		as much as possible.

The raster is scaled to fit the current viewport (by PostScript).

The values are scaled so that under/over range maps to the ends of the
colormap. Blanked data are mapped to white. It is assumed that the value
used to indicate blanked data is greater than the cube's data max.
*/
void AvPSCanvas::drawRaster(	AvRaster &img,
		int /*srcx*/, int /*srcy*/, int /*dstx*/, int /*dsty*/,
			unsigned int /*swidth*/, unsigned int /*sheight*/)
{
AvViewport *vp = viewport();
float	xscl, yscl;
float	*data;
float	blankValue;	// Data value used to represent blanked pixels.
float	min, max;	// Data min/max
int	iWidth, iHeight;// Size of input image in pixels.
int	iXoff, iYoff;	// Position of input image within viewport.
float	oXoff, oYoff;	// Position of output image within viewport.
float	oWidth, oHeight;// Size of output image in current PS dimensions.

	vp->getRasterScale(xscl, yscl);

	min = img.min();
	max = img.max();

	// Where to place image.
	iXoff = vp->x0();
	iYoff = vp->y0();
	oXoff = iXoff*xscale_;
	oYoff = iYoff*yscale_;

	iWidth = img.width();
	iHeight = img.height();
	oWidth = iWidth*xscl*xscale_;
	oHeight = iHeight*yscl*yscale_;

	// In/out data arrays.
	int nelements = iWidth*iHeight;
	data = new float[nelements];
	unsigned long *scaledData = new unsigned long[nelements];
	blankValue = img.getBlankValue();

	// Get data slice.
	img.beginAccess();
		img.getRegion(data, 0, 0, iWidth, iHeight);
	img.endAccess();
	// Scale it.
	unsigned long underColor = palette_->whiteIndex();
	unsigned long overColor = palette_->whiteIndex();
	unsigned long blankColor = palette_->whiteIndex();

	palette_->scale(data, scaledData, nelements, min, max,
		 underColor, overColor, blankColor,
		blankValue);

	psd_->drawImage(oXoff, oYoff, iWidth, iHeight,
			oWidth, oHeight, scaledData, imageOpts_);
	delete [] data;
	delete [] scaledData;
}

void AvPSCanvas::drawRaster(AvRaster *img)
{
	if(img == NULL)
		return;
	drawRaster( *img);
}

// Set the viewport, then set clip plane.
void AvPSCanvas::setViewport(AvViewport &vp)
{
	AvCanvas::setViewport(vp);
	setClipPlane(*viewport());	// Set current effective vp.
}

// Reset state, then set clip plane.
void AvPSCanvas::popState()
{
	AvCanvas::popState();
	setClipPlane( *viewport());
}

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

/* Draw polylines (points, lines, polygons, etc). Moves are handled by
updating the current position.
*/
#if 0
struct {
	int	cmd;
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

static char *cmdstr(int cmd)
{
	for(int i=0; i< NCMDS; i++)
		if(cmd == cmds[i].cmd)
			return cmds[i].name;
	return "??";
}
#endif

void AvPSCanvas::drawVector(AvPolylineCmd *cmd)

{
float		maxPos[2], *scale;
float		xoff, yoff;	// Offset of viewport.
long		numpnts;
Boolean		relative, fractional;
AvViewport	*vp;
PointList	*points;	// Pointer to list of points.

	if(!opened())
		return;

	if((cmd == NULL) || ((points = cmd->positionList()) == NULL))
		return;

	if((numpnts = points->size()) <= 0)
		return;

	vp = viewport();

	// Compute scale factors to convert from Pixels in X window to
	// PS coordinates.
	float vpWidth = vp->width();
	float vpHeight = vp->height();
	maxPos[0] = xscale_*vpWidth;
	maxPos[1] = yscale_*vpHeight;

	// Offset of '0,0' in viewport.
	xoff = vp->x0();
	yoff = vp->y0();

	xoff *= xscale_;
	yoff *= yscale_;

	relative = (cmd->mode_ == AvPolyline::ABSOLUTE) ? FALSE : TRUE;
	fractional = (cmd->cmode_ == AvPolyline::FRACTIONAL);
	scale = fractional ? maxPos : (float *)NULL;

	/* For each point we need to:
		Convert to viewport coords if its fractional.
		Convert to absolute coords if its relative.
		Update the current position.
		Convert from absolute vp coords.
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

	float *xpts = new float[numpnts+2];
	float *ypts = new float[numpnts+2];

	switch(cmd->cmd_) {
	case MOVE:	// Compute new current point.
		points->getValue(0, &currX, &currY);
		frac2full(currX, currY, scale);
		if(relative)
			rel2abs(currX, currY, prevX, prevY);
		currX += xoff;
		currY += yoff;
		psd_->moveTo(currX, currY);
		break;
	case POINT:
		{	for( int i=0; i< numpnts; i++)
			{	// Get x/y from list.
				points->getValue(i, &currX, &currY);
				// Convert to vp from fractional.
				frac2full(currX, currY, scale);
				if(relative)
					rel2abs(currX, currY, prevX, prevY);

				currX += xoff;
				currY += yoff;
				xpts[i] = currX;
				ypts[i] = currY;
			}
			psd_->drawPoints((int)numpnts, xpts, ypts);
		}
		break;
	case LINE:
		{	for( int i=0; i< numpnts; i++)
			{	// Get x/y from list.
				points->getValue(i, &currX, &currY);
				// Convert to vp from fractional.
				frac2full(currX, currY, scale);
				if(relative)
					rel2abs(currX, currY, prevX, prevY);
				else
				{	currX += xoff;
					currY += yoff;
				}
				psd_->lineTo(currX, currY);
			}
//		printf("LINEs (%ld) %f %f\n", numpnts, currX, currY);
		}
			break;
	case POLYGON:

/* NOTES: Need to check. We may be drawing an extra point (at the start).
	(lines, polygons and points probably don't need exactly the same
lists).
Rectangles, circles, polygons, etc, probably shouldn't update the current
point.
*/
		{// numpnts += 1;		// Count the initial 'move'.

		  /* The initial draw is from the current position to
		     the first point.
		  */
//			xpts[0] = currX;
//			ypts[0] = currY;
			for( int i=0; i< numpnts; i++)
			{	// Get x/y from list.
				points->getValue(i, &currX, &currY);
				// Convert to vp from fractional.
				frac2full(currX, currY, scale);
				if(relative)
					rel2abs(currX, currY, prevX, prevY);
				else
				{	currX += xoff;
					currY += yoff;
				}
				xpts[i] = currX;
				ypts[i] = currY;
			}

			switch( cmd->cmd_) {
			case LINE:
				break;
			case POLYGON:
				// Close polygon.
				if((xpts[0] != xpts[numpnts-1]) ||
				   (ypts[0] != ypts[numpnts-1]))
				{	xpts[numpnts] = xpts[0];
					ypts[numpnts++] = ypts[0];
				}

				if(cmd->filled_)
					psd_->drawPolygon((int)numpnts,
							  xpts, ypts, 1);
				else
					psd_->drawPolygon((int)numpnts,
							  xpts, ypts, 0);
#if 0
{for(int i=0; i< numpnts; i++)
	printf("%s (%d) %f %f\n", (cmd->filled_) ? "PFILED" : "PUNF",
							i, xpts[i], ypts[i]);
}
#endif
				break;
			case POINT:
				break;
			default:;
			}

//			delete [] xpts;
//			delete [] ypts;
		}
		break;
	case RECTANGLE:
printf("RECT (%ld)\n", numpnts);
#if 0
		{ 
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
								rlst, numpnts);
		  else
			  XDrawRectangles(display(), drawable(), xgc_,
								rlst, numpnts);
		  delete [] rlst;
//		  delete [] xpts;
//		  delete [] ypts;
		}
#endif
		break;
	case SEGMENT:
		{ 
		  // Need to do segments one segment at a time.
		  PointList *aux = cmd->auxList();
		  float x2, y2;
			for( int i=0; i< numpnts; i++)
			{	// Get x/y from list.
				points->getValue(i, &currX, &currY);
				// Convert to vp from fractional.
				frac2full(currX, currY, scale);
				if(relative)
					rel2abs(currX, currY, prevX, prevY);
				else
				{	currX += xoff;
					currY += yoff;
				}
				aux->getValue(i, &x2, &y2);
				frac2full(x2, y2, scale);
				if(relative)
					rel2abs(x2, y2, prevX, prevY);
				else
				{	x2 += xoff;
					y2 += yoff;
				}
				xpts[0] = currX;
				ypts[0] = currY;
				xpts[1] = x2;
				ypts[1] = y2;
				psd_->drawLine(currX, currY, x2, y2);
			}
		}
//		printf("SEGs (%d) %f %f\n", numpnts, currX, currY);
		break;
	case CIRCLE:
	case ARC:
		{ 
printf("CARC  (%ld)\n", numpnts);
#if 0
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
		  	XFillArcs(display(), drawable(), xgc_, alst, numpnts);
		  else
			XDrawArcs(display(), drawable(), xgc_, alst, numpnts);

		  delete [] alst;
#endif
		}
		break;
	case NONE:
	default:
		break;
	}
	delete [] xpts;
	delete [] ypts;

	AvAllotment tmp(currX, currY);
	viewport()->vectorPosition(tmp);
}

#if 0
// Test routine to display current viewport settings.
static void showvp()
{
float x0, x1, y0, y1;

	cpgqvp(0, &x0, &x1, &y0, &y1);
printf("VP (NDC):\t %.2f-%.2f, %.2f-%.2f\n", x0, x1, y0, y1);
	cpgqvp(1, &x0, &x1, &y0, &y1);
printf("VP (INCHES):\t%.2f-%.2f, %.2f-%.2f\n", x0, x1, y0, y1);
#if 0
	cpgqvp(3, &x0, &x1, &y0, &y1);
printf("VP (PIXELS):\t%.2f-%.2f, %.2f-%.2f\n", x0, x1, y0, y1);
#endif
}
#endif

// Sets up PostScript's viewport.
void AvPSCanvas::setClipPlane(AvViewport &vp)
{
	if(opened())
	{ unsigned int w, h;
	  float X0, X1, Y0, Y1;
	  int x, y;
		x = vp.x0();
		y = vp.y0();
		w = vp.width();
		h = vp.height();
		X0 = scale(x) + xoff_;
		X1 = X0 + scale(w);
		Y0 = scale(y) + yoff_;
		Y1 = Y0 + scale(h);
		psd_->clipRect( X0, Y0, X1, Y1);
#if 0
printf(
 "AvPSCanvas::setClipPlane: X/Y %d/%d,W/H %d/%d; XY:%.2f-%.2f/%.2f-%.2f\n",
	x,y,w,h, X0,X1, Y0, Y1);
showvp();
#endif
//		cpgswin(0.0, (float)w-1, 0.0, (float)h-1);
//		AvAllotment vpos = vp.vectorPosition();
		// Restore last drawing position.
//		cpgmove(vpos.x(), vpos.y());
		psd_->moveTo(0.0, 0.0);
//		cpgmove(0, 0);
	}
}

void AvPSCanvas::setClipPlane(AvViewport *vp)
{
	if(vp == NULL)
		vp = viewport();
	setClipPlane( *vp);
}

AvPalette *AvPSCanvas::palette()const
{	return  palette_;
}

/* Make the object the current display object and display it. If it
is NULL or the same as current object, redisplay the current list
rather than replacing it.
*/
void AvPSCanvas::show(AvGraphics2D *dob)
{
unsigned int w, h;
AvViewport *vp;

	setDisplayObject(dob);
	// Use whatever is actually on the list.
	dob = vps_->getObject(0);
	if(dob == NULL)
		return;

	// Get desired size and set the viewport size to match.
	AvAllotment r;
	getBoundingBox( dob, r);
	w = (unsigned int) r.value(AvDimensionX);
	h = (unsigned int) r.value(AvDimensionY);
	vps_->setSize(w, h);
	// Set the size of the main viewport to be that of the bb.
	vp = viewport();
	vp->width(w);
	vp->height(h);

#if 0
	AvCanvas::show( *vps_);
	if( open())
	{	cpgswin(0.0, (float)(w-1), 0.0, (float)(h-1));
		AvCanvas::show( *vps_);
		close();
	}
#else
	AvCanvas::show( *vps_);
#endif
}

void AvPSCanvas::show(AvGraphics2D &dob)
{
	show(&dob);
}

// Initializes scale_ to make sure resultant image fits on the page.
void AvPSCanvas::initScale( const float max, unsigned int size)
{
	if(PIXEL2INCH*size > max)
		scale_ = max/size;
	else
		scale_ = PIXEL2INCH;
}

const char *AvPSCanvas::getPSEVar_(const char *evar, const char *def,
				   const char *defevar)
{
	if(evar == NULL)
		return getEnvStr(defevar, def);
	else
		return getEnvStr(evar, def);
}

// Return a filename/device to use for PostScript output.
// evar is a pointer to an environment variable to use to look for
// a user supplied filename. def is returned if evar doesn't exist.
AvString AvPSCanvas::getPSfilename(const char *evar, const char *def)
{
	AvString f = getPSEVar_(evar, def, PS_FILE);
	f.gsub(' ', '_');	// Replace spaces with underscores.
	return f;
}

// Return a PostScript device name.
// Use X resource if it exists.
//  Or an environment variable
//    Or use default.
// The resource is checked first since it may be set on the command line.
AvString AvPSCanvas::getPSdevicename(const char *evar, const char *def)
{
	const char *d = getXOptions()->pgDevice;
	if(d == NULL)
		getPSEVar_(evar, def, PS_DEVICE);
	if(d == NULL)				// We need a device name.
		d = DEFAULT_DEVICE;
	AvString n = d;
	return n;
}

// Form a PostScript file/device name. If fn is NULL an empty filename
// will be used. (PostScript will supply a name). If dev is NULL, the
// default device will be used.
// If the device name doesn't start with a "/", one will be supplied.
AvString AvPSCanvas::makePSname(const char *fn, const char *dev)
{
	// If fn &/or dev is NULL recurse with non NULL arguments.
	if(fn == NULL)
		return makePSname("", dev);

	if(dev == NULL)
		return makePSname(fn, getPSdevicename().chars());

	AvString buf = fn;
	if(*dev != '/')		// PostScript devices should start with this.
		buf += "/";
	buf += dev;
	buf.gsub(' ', '_');	// Replace spaces with underscores.
	return buf;
}

// Open the PostScript file and initialize viewport.
int	AvPSCanvas::open()
{
#if 0
	if(opened())
		close();

char	fname[128];
AvString buf;

	AvString f = getPSfilename(PS_FILE, NULL);
	if(f.length() == 0)
	{	if(file_ != NULL)
		{	sprintf(fname, "%s.ps", file_);
			f = fname;
		}
	}
	AvString d = getPSdevicename(PS_DEVICE, device_);
	buf = makePSname(f.chars(), d.chars());

	opened_ = (AvPSCanvas::cpgbeg(unit_, buf, nxsub_, nysub_) == 1)
		  ? TRUE : FALSE;

	if(opened_)
	{AvViewport *vp = viewport();
	 unsigned int	w, h;
	 float width, height;
	 float x1, y1, x2, y2, W, H;

		cpgask(0);		// Don't ask.
		cpgbbuf();
		// Set output to fit the paper and have same aspect
		// ratio as the image. The scale is set so that a viewport
		// of 256 pixels is about 4" wide. If the image is too big,
		// the scaling is reduced. The image is more or less centered.
		w = vp->width();
		h = vp->height();

		// Size of the page.
		cpgqvsz(1, &x1, &x2, &y1, &y2);
		W = x2 - x1;
		H = y2 - y1;

		// Initialize the scale factor.
		if(w/W >= h/H)
			initScale(W, w);
		else
			initScale(H, h);

		width = scale(w);
		// Choose offsets to center the image.
		xoff_ = (W-width)*0.5;	
		if(xoff_ < 0.0)
			xoff_ = 0.0;
		width += xoff_;

		height = scale(h);
		yoff_ = (H-height)/2.0;
		if(yoff_ < 0.0)
			yoff_ = 0.0;
		height += yoff_;

		cpgpap(width, height/width);

		cpgsvp(0.0, 1.0, 0., 1.0);	// Set viewport to entire area.
		palette_->checkAllocated();
#if 1
		printf("Printing to device \"%s\", file: \"%s\"",
				d.chars(), f.chars());	// Finished in close().
#else
		printf("Printing to: \"%s\"...", buf.chars());
#endif
		fflush(stdout);
	}
	return	opened_;
#else
	opened_ = TRUE;
	return	opened_;
#endif
}

// Close device, dumping buffers and adding the id first.
void	AvPSCanvas::close()
{
#if 0
	if(opened())
	{	psd_->finish();
		opened_ = FALSE;
		printf("\tDone\n");
	}
#endif
}

// Copy a character string. Returns NULL if the argument is NULL.
char	*AvPSCanvas::strDup(const char *str)
{
int	len;
char	*s;

	if(str == NULL)
		s = NULL;
	else
	{	len = (int)strlen(str);
		s = new char[len+1];
		strcpy(s, str);
	}

	return s;
}

/****************************************************************/
/****************************************************************/
/****************************************************************/
/*			Graphic context code.			*/
/****************************************************************/
/****************************************************************/
/****************************************************************/

AvPSGraphicContext::AvPSGraphicContext( AvPSCanvas *can) :
						AvGraphicContext()
{
	canvas_ = can;
}

AvPSGraphicContext::AvPSGraphicContext(const AvGraphicContext &gc):
						AvGraphicContext(gc)
{
	canvas_ = NULL;
}

void AvPSGraphicContext::copy(const AvGraphicContext &gcin)
{
	canvas_ = NULL;
	AvGraphicContext::copy(gcin);
}

void AvPSGraphicContext::copy(const AvPSGraphicContext &gcin)
{
	canvas_ = gcin.canvas_;
	AvGraphicContext::copy(gcin);
	update();
}

// Emulate the = operator for plain graphic contexts.
AvGraphicContext& AvPSGraphicContext::operator=(const AvGraphicContext &gcin)
{
	AvGraphicContext::copy(gcin);
	return *this;
}

AvPSGraphicContext& AvPSGraphicContext::operator=(const AvPSGraphicContext &gcin)
{
	copy(gcin);
	return *this;
}

AvPSGraphicContext::~AvPSGraphicContext()
{
}

void AvPSGraphicContext::setLineWidth(const int LW)
{
	if(!canvasOpened())
		return;

	AvGraphicContext::setLineWidth(LW);
	canvas_->PSDriver()->lineWidth((float)LW);
}

void AvPSGraphicContext::setLineStyle(const LineStyle LS)
{
	if(!canvasOpened())
		return;

	AvGraphicContext::setLineStyle(LS);
	{AvPSDriver::LineStyle ls;

		switch(getLineStyle()) {
		case AvGraphicContext::SOLID:
			ls = AvPSDriver::SOLID;
			break;
		case AvGraphicContext::DASHED:	// Resource??
			ls = AvPSDriver::DASHED;
			break;
		// There should be a dotted here.
		case AvGraphicContext::DOTTED:	// Resource??
			ls = AvPSDriver::DASHDASH;
		break;
		default:
			ls = AvPSDriver::SOLID;
		}
		canvas_->PSDriver()->setLineStyle(ls);
	}
}

void AvPSGraphicContext::setForeground(const unsigned long c)
{
	if(!canvasOpened())
		return;

	AvGraphicContext::setForeground(c);
	AvPalette *pal = canvas_->getPSPalette();
	unsigned long color;

	if(pal != NULL)
	{	if(pal->virtualToPhysical(c, color))
			canvas_->PSDriver()->setColor((float)color);
	}
}

// Needed by copy
void AvPSGraphicContext::update( const int msk)
{
int	m;

	if(!canvasOpened())
		return;

	if(msk == 0)
		m = AvGraphicContext::ALLMASK;
	else
		m = msk;

	if(m & AvGraphicContext::FOREGROUND)
	{//AvPalette *pal = canvas_->palette();
	   AvPSPalette *pal = canvas_->getPSPalette();
	   unsigned long color;

		if(pal != NULL)
		{	if(pal->virtualToPhysical(getForeground(), color))
//				canvas_->PSDriver()->setColor(color);
				canvas_->PSDriver()->setColor((float)color);
		}

	}

#if 0
	if(m & AvGraphicContext::BACKGROUND)
	{AvPalette *pal = canvas_->palette();
	 unsigned long color;

		if(pal != NULL)
		{	if(pal->virtualToPhysical(getBackground(), color))
			  printf("AvGraphicContext::BACKGROUND %d\n", color);
		}
	}
#endif

	// Should replace constants with resources.
	if(m & AvGraphicContext::LINEWIDTH)
	{int lw;
#if 0
		switch (getLineWidth()) {
		case  AvGraphicContext::DEFAULTLINEWIDTH:
			lw = 1;
			break;
		case AvGraphicContext::THIN:
			lw = 2;
			break;
		case AvGraphicContext::MEDIUM:
			lw = 4;
			break;
		case AvGraphicContext::WIDE:
			lw = 6;
			break;
		default:
			lw = 1;
		}
#else
		lw = getLineWidth();
#endif
		if(lw <= 0)
			lw = 1;
		canvas_->PSDriver()->lineWidth((float)lw*0.5);
//		cpgslw(lw);
	}

	if(m & AvGraphicContext::LINESTYLE)
	{AvPSDriver::LineStyle ls;
		switch(getLineStyle()) {
		case AvGraphicContext::SOLID:
			ls = AvPSDriver::SOLID;
			break;
		case AvGraphicContext::DASHED:	// Resource??
			ls = AvPSDriver::DASHED;
			break;
		// There should be a dotted here.
		case AvGraphicContext::DOTTED:	// Resource??
			ls = AvPSDriver::DASHDASH;
			break;
		default:
			ls = AvPSDriver::SOLID;
		}
		canvas_->PSDriver()->setLineStyle(ls);
	}
}

Boolean AvPSGraphicContext::canvasOpened()const
{return	((canvas_!=NULL)&&(canvas_->opened())) ? TRUE : FALSE;
}
