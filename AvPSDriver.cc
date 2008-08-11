//# AvPSDriver.cc: Low level PostScript interface.
//# Copyright (C) 1996,1997,1999,2000,2002
//# Associated Universities, Inc. Washington DC, USA.
//#
//# This library is free software; you can redistribute it and/or modify it
//# under the terms of the GNU Library General Public License as published by
//# the Free Software Foundation; either version 2 of the License, or (at your
//# option) any later version.
//#
//# This library is distributed in the hope that it will be useful, but WITHOUT
//# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
//# License for more details.
//#
//# You should have received a copy of the GNU Library General Public License
//# along with this library; if not, write to the Free Software Foundation,
//# Inc., 675 Massachusetts Ave, Cambridge, MA 02139, USA.
//#
//# Correspondence concerning AIPS++ should be addressed as follows:
//#        Internet email: aips2-request@nrao.edu.
//#        Postal address: AIPS++ Project Office
//#                        National Radio Astronomy Observatory
//#                        520 Edgemont Road
//#                        Charlottesville, VA 22903-2475 USA
//#
//# $Id: AvPSDriver.cc,v 19.0 2003/07/16 05:48:13 aips2adm Exp $
/* AvPSDriver.cc
Class to generate PostScript.

ToDo
  Much, including:
	clip window

Notes:
	There is a hack to reset the colorspace everytime the color tables
are changed. This is necessary because ghostscript appears to only read
them when the "setcolorspace" command is issued.

 Images are currently only supported in INDEXED colorspace.

 Points draw filled circles of radius 1 point.

 drawText supports justification, but vertical positioning isn't quite
 what might be expected.

 Beginnings of EPS support. If EPS is requested, a bounding box will be
calculated. However, text size must be guessed at. Also, the Display
Library tends to call clear() which implicitly sets the bounding box to
the entire writable area.

 It's OK to switch between colorspaces at any time. However, single
image draws are always done in INDEXED mode, temporarily changing
colorspace if necessary.

*/
#include <AvPSDriver.h>
#include <AvVersion.h>
#include <fstream.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <math.h>
#define SHOW_DATE
#if defined(SHOW_DATE)
#include <sys/types.h>
#include <time.h>
#endif

#if !defined(M_PI)
#define M_PI	3.14159265358979323846
#endif

////////////////////////////////////////////////////////////////
///		Internal constants.
// What media type to use if none is specified or is unknown.
static const AvPSDriver::MediaSize DEFAULTMEDIA = AvPSDriver::LETTER;
static const float DEFAULTCHARSCALE=12;
static const char *BGNAME="useBGColor";
////////////////////////////////////////////////////////////////

AvPSDriver::AvPSDriver()
{ AvString name = "";

	initname( name, INDEXED, NULL, PORTRAIT, NULL);
}

AvPSDriver::AvPSDriver(ostream &Output)
{
	init(Output, INDEXED, NULL, PORTRAIT, NULL);
}

AvPSDriver::AvPSDriver(const char *name)
{AvString Output(name);

	initname(Output, INDEXED, NULL, PORTRAIT, NULL);
}

AvPSDriver::AvPSDriver(ostream &Output, const MediaSize ms, const Layout lo,
		   PSInfo *info)
{ const PageInfo *pi = pageInfo(ms);

	init(Output, INDEXED, pi, lo, info);
}

AvPSDriver::AvPSDriver(const AvString &Output, const MediaSize ms, const Layout lo,
		   PSInfo *info)
{ const PageInfo *pi = pageInfo(ms);

	initname(Output, INDEXED, pi, lo, info);
}

AvPSDriver::AvPSDriver(const char *name, const MediaSize ms, const Layout lo,
		   PSInfo *info)
{ AvString Output(name);
 const PageInfo *pi = pageInfo(ms);

	initname(Output, INDEXED, pi, lo, info);
}

AvPSDriver::AvPSDriver(ostream &Output, const Dimension dim,
		   const float x0, const float y0,
		   const float x1, const float y1,
		   const Layout lo, PSInfo *info)
{ PageInfo userinfo = { USERPAGE, x0, y0, x1, y1, dim, NULL, NULL, NULL};

	init(Output, INDEXED, &userinfo, lo, info);
}

AvPSDriver::AvPSDriver(const AvString &Output, const Dimension dim,
		   const float x0, const float y0,
		   const float x1, const float y1,
		   const Layout lo, PSInfo *info)
{ PageInfo userinfo = { USERPAGE, x0, y0, x1, y1, dim, NULL, NULL, NULL};
	initname(Output, INDEXED, &userinfo, lo, info);
}

AvPSDriver::AvPSDriver(const AvString &Output, const MediaSize ms,
		   const Dimension dim,
		   const float x0, const float y0,
		   const float x1, const float y1,
		   const Layout lo, PSInfo *info)
{ PageInfo userinfo = { ms, x0, y0, x1, y1, dim, NULL, NULL, NULL};
	initname(Output, INDEXED, &userinfo, lo, info);
}

AvPSDriver::AvPSDriver(const char *name, const Dimension dim,
		   const float x0, const float y0,
		   const float x1, const float y1,
		   const Layout lo, PSInfo *info)
{AvString Output(name);
 PageInfo userinfo = { USERPAGE, x0, y0, x1, y1, dim, NULL, NULL, NULL};

	initname(Output, INDEXED, &userinfo, lo, info);
}

float AvPSDriver::toPoints(const float in, const Dimension indem)
{ float val;

	switch(indem) {
	case INCHES:
		val = inchesToPoints(in);
		break;
	case MM:
		val = mmToPoints(in);
		break;
	case POINTS:
	default:
		val = in;
		break;
	}
	return val;
}

const char *AvPSDriver::dimensionToString(const AvPSDriver::Dimension indem)
{ const char *rtn;

	switch(indem) {
	case AvPSDriver::INCHES:
		rtn = "inches";
		break;
	case AvPSDriver::MM:
		rtn = "mm";
		break;
	case AvPSDriver::POINTS:
		rtn = "points";
		break;
	default:
		rtn =  "Unknown";
		break;
	}
	return rtn;
}

float AvPSDriver::fromPoints(const float in, const Dimension indem)
{ float rtn;
	switch(indem) {
	case INCHES:
		rtn =  pointsToInches(in);
		break;
	case MM:
		rtn = pointsToMM(in);
		break;
	case POINTS:
	default:
		rtn = in;
		break;
	}
	return rtn;
}

void AvPSDriver::initname(const AvString &name, const ColorSpace cs,
		    const PageInfo *pinfo, const Layout lo, PSInfo *psinfo)
{fstream *os;
static int sequence=0;

	if(name.empty())
	{ char buf[64];
		sprintf(buf, "output_%d_%d.ps", sequence++, (int)getpid());
//		os = new fstream(buf, ios::out, 0666);
		os = new fstream(buf, ios::out);
	}
	else
//	{	os = new fstream(name.chars(), ios::out, 0666);
	{	os = new fstream(name.chars(), ios::out);
	}

	init( *os, cs, pinfo, lo, psinfo);
	mystream_ = os;
}

// 1 point = .013837 in. (not quite 72pts/in).

void AvPSDriver::init(ostream &Output, const ColorSpace cs,
		    const PageInfo *pginfo, const Layout lo, PSInfo *info)
{ const PageInfo *pinfo = (pginfo) == NULL ? pageInfo(DEFAULTMEDIA) : pginfo;

	Output.precision(4);
	out = &Output;
	mystream_ = NULL;
	statestackindex_ = 0;

	colorSpace_ = cs;	// Will be xfred to PS in emitSetup().
	portrait_ = ((lo & ~EPS) == PORTRAIT) ? True : False;
	eps_ = (lo & EPS) ? True : False;
	// Copy info from page description. 
	dimension_ = pinfo->dimension;
	if(pinfo->media == USERPAGE)	// User specified page size.
	{	// The fields are used differently for user described page.
		xll_ = toPoints(pinfo->width, dimension_);
		yll_ = toPoints(pinfo->height, dimension_);
		xur_ = toPoints(pinfo->lrmargin, dimension_);
		yur_ = toPoints(pinfo->tbmargin, dimension_);
		eps_ = True;
		bbx0_ = xll_;
		bby0_ = yll_;
		bbx1_ = xur_;
		bby1_ = yur_;
		haveBoundingBox_ = True;
		boxCheck0_ = True;
		checkBoundingBox_ = False;
	}
	else
	{float lm, rm, tm, bm;
	 Bool haveMargins = False;
	 Dimension dim = pinfo->dimension;

		// If user has supplied margins, use them, else defaults.
		if(info != NULL)	// Try for user supplied margins.
			haveMargins = info->getMargins(lm, rm, tm, bm);

		if(!haveMargins)	// If not, use defaults.
		{	lm = rm = toPoints(pinfo->lrmargin, dim);
			tm = bm = toPoints(pinfo->tbmargin, dim);
			// (Use this toPoints to convert from a known
			//  dimension. Besides state isn't initialized yet.
		}
		// Page size - margins.
		xll_ = lm;
		yll_ = bm;
		xur_ = toPoints(pinfo->width,dim) -1 -rm;
		yur_ = toPoints(pinfo->height,dim) -1 -tm;
		clipXll_ = xll_;
		clipXur_ = xur_;
		clipYll_ = yll_;
		clipYur_ = yur_;

		// Initial values.
		bbx0_ = xur_;
		bby0_ = yur_;
		bbx1_ = xll_;
		bby1_ = yll_;
		haveBoundingBox_ = False;
		checkBoundingBox_ = True;
		boxCheck0_ = False;
	}

	info_ = info;
	lineStyle_ = SOLID;	// PS defaults to solid.
	pageNum_ = 1;		// First page.

	{ float scale;
		switch (dimension_) {
		case INCHES:
			scale = inchesToPoints(1.0);
			break;
		case MM:
			scale = mmToPoints(1.0);
			break;
		case POINTS:
		default:
			scale = 1.0;
			break;
		}
		xscale_ = yscale_ = scale;
	}

	defaultFontSize_ = DEFAULTCHARSCALE;
	// defaultFontSize is meant to be in points so we need to
	// 'predistort' it so the transform will 'undistort' it.
	{ float a, b;
		fromPoints(defaultFontSize_, defaultFontSize_, a, b, False);
		defaultFontSize_ = (a+b)/2.0;
	}

	finished_ = 0;
	startDocument();

#define TEST_
#ifdef TEST
	// Run a consistency check on the pageinfo table.
	// This should be done if the table or enum ever get changed.
	// Errors will be reported by pageInfo();
	{ int npt = AvPSDriver::numPageTypes();
		for(int k=0; k<npt; k++)
		const AvPSDriver::PageInfo *po =
			 pageInfo((AvPSDriver::MediaSize)k);
	}
#endif
}

AvPSDriver::~AvPSDriver()
{
	finish();
	if(mystream_ != NULL)
	{	mystream_->close();
		mystream_ = NULL;
	}
	if(info_ != NULL)
	{	delete info_;
		info_ = NULL;
	}
}

void AvPSDriver::finish()
{	if(!finished_)		// Only do it once.
	{
		emitTrailer();
		emitShowPage();
		finished_ = 1;
	}
}

#if 0
// Returns PS string for the given color space.
const char *AvPSDriver::colorSpace(const ColorSpace cs)
{ const char *str;

	switch(cs) {
	case INDEXED:
		str = "/Index";
		break;
	case GRAY:
		str = "/DeviceGray";
		break;
	case RGB:
	case HSV:
		str = "/DeviceRGB";
		break;
	default:		// Error.
		str =  "/DeviceGray";
		break;
	}
	return str;
}
#endif

void AvPSDriver::setColorSpace(const ColorSpace cs)
{
	emitColorSpace(cs);
	colorSpace_ = cs;
}

void AvPSDriver::storeColors(const int start, const int len,
			   const float *r, const float *g, const float *b)
{
	emitStoreColors(start, len, r, g, b);
}

void AvPSDriver::storeColorValues(const int len, const int *indexes,
			       const float *a, const float *b, const float *c)
{
	emitStoreColorValues(len, indexes, a, b, c);
}


void AvPSDriver::storeColor(const int index,
			  const float r, const float g, const float b)
{
	emitStoreColor(index, r, g, b);
}

// Draw line from current point to x,y.
// If strk is non zero, the current path is drawn.
void AvPSDriver::lineTo(const float x, const float y, const int strk)
{ float x0, y0;

	getCurrXY(x0, y0, True);	// BB check the current point.
	bbCheck(x0, y0, True);

	bbCheck(x, y);
	setCurrXY(x, y);
	emitLineTo(x, y, strk);
}

// Set current color for text and vectors. Only the first color argument
// is used for INDEXED & GRAY.
// For INDEXED mode, color should be in the range (0..ncolors-1)
// where ncolors is the number of colors used in the table. ( ncolors < 4096).
// (The floating point value will be truncated to an integer).
// For the others, color should be in the range (0..1).
// For RGB mode, the args represent red, green & blue components.
// For HSB/HSV mode, the args represent hue, saturation & brightness(value).
void AvPSDriver::setColor(const float rh, const float gs, const float bv)
{
	emitSetColor(colorSpace_, rh, gs, bv);
}

// Set color value for indexed and gray scale modes.
// If called when not in indexed or gray modes, the same value
// is used for all three components.
void AvPSDriver::setColor(const float color)
{
	emitSetColor(colorSpace_, color, color, color);
}

void AvPSDriver::setBackgroundColor(const float r, const float g,
				  const float b, const ColorSpace bgs)
{
	emitBackgroundColor(r, g, b, bgs);
}

// Set color value for indexed and gray scale modes.
// If called when not in indexed or gray modes, the same value
// is used for all three components.
void AvPSDriver::setBackgroundColor(const float color, const ColorSpace bgs)
{
	emitBackgroundColor(color, color, color, bgs);
}

void AvPSDriver::setIndexColor(const float indx)
{
	setColorSpace(INDEXED);
	setColor(indx);
}

void AvPSDriver::setRGBColor(const float r, const float g, const float b)
{
	setColorSpace(RGB);
	setColor(r, g, b);
}

void AvPSDriver::setHSBColor(const float h, const float s, const float b)
{
	setColorSpace(HSB);
	setColor(h, s, b);
}

void AvPSDriver::setLinearRamps(const int ncolors)
{
	emitSetLinearRamps(ncolors);
}

// Dashes are drawn using current user coordinates (eg scale).
void AvPSDriver::setLineStyle(const LineStyle style)
{
	lineStyle_ = style;
	emitLineStyle(style);
}

void AvPSDriver::setDashLength(const float length)
{
	emitDashLength(length);
}

//	Display an image at a given location. Output is a unit square times
// the given scale factors in the current user coordinates.
// x0,y0	Image is drawn with lower left corner at x0, y0.
// width,height	Size of source image in elements.
// xsize,ysize	Size of output image. (Eg. If current user
//		coordinates were in inches, a size of 2 would create
//		a two inch output square regardless of the size of the
//		input image).
// data		Prescaled data array of length (width*height) whose elements
//		are in the range (0..4095). Arrays whose elements are
//		all <= 255 can be encoded more efficiently.
// smooth	If smooth is non zero, request the printer to interpolate
//		the pixels. Old versions of ghostscript may core dump if != 0.
//		Not supported by all interpreters.
// bpc		Used to determing how data is encoded. If (0 < bpc <= 8)
//		8 bit encoding is used. If bpc > 8, 12 bit encoding is used.
//		If bpc <= 0, the array is scanned to determine the encoding
//		method.
void AvPSDriver::drawImage(const float x0, const float y0,
			 const int width, const int height,
			 const float xsize, const float ysize,
			 const unsigned long *data, const int opts,
			 const int bpc)
{float m[] = { 1.0, 0.0, 0.0, 1.0, 0.0, 0.0};
 int smooth = ((opts&SMOOTH) == 0) ? 0 : 1;

	bbCheck(x0, y0);
	bbCheck(x0+xsize, y0 + ysize);

	if(opts & (FLIPX|FLIPY))
	{	if(opts&FLIPX)
		{	m[0] = -width;
			m[4] = width;
		}
		else
			m[0] = width;

		if(opts&FLIPY)
		{	m[3] = -height;
			m[5] = height;
		}
		else
			m[3] = height;

		gsave();
			translate(x0, y0);
			scale(xsize, ysize);
			drawImage(m, width, height, data, bpc, smooth);
		grestore();
	}
	else
	{ float sclx = (float)width/xsize;
	  float scly = (float)height/ysize;

		m[0] = sclx;
		m[3] = scly;
		m[4] = -x0*sclx;
		m[5] = -y0*scly;

		drawImage(m, width, height, data, bpc, smooth);
	}
}

// imagematrix		6 element array that defines transformation from
//			user space to image space:
//			m = [ a b c d tx ty ]
//			 x' = ax + cy + tx
//			 y' = bx + dy + ty
//	The transformation is such that [ width 0 0 height 0 0] will map
//	the image into a unit square which could then be scaled by the
//	current scale() values.
//	To invert the image, use: [ width 0 0 -height 0 height].
//	If imagematrix is NULL, the image will be mapped into the unit square.
// width/height	Size of image in elements.
// data	Scaled input data.
void AvPSDriver::drawImage(const float imagematrix[6],
			 const int width, const int height,
			 const unsigned long *data,
			 const int BPC, const int smooth)
{ int blen, bpc = BPC;

	char *buf = encodeUnsignedLongs(width, height, bpc, data, blen);
	emitImage(imagematrix, width, height, bpc, smooth, buf, blen);

	delete [] buf;
}

// Save/restore graphics state.
void AvPSDriver::gsave()
{
	pushState();
	emitGSave();
}

void AvPSDriver::grestore()
{
	popState();
	emitGRestore();
}

// The value of w = 1.0 is 0.005". (For compatibility with PGPLOT).
static const float LINEWIDTHSCALE=0.005;
void AvPSDriver::lineWidth(const float w)
{ float widthInPoints = inchesToPoints(w*LINEWIDTHSCALE);
  float a, b;

	// Since PostScript will scale the line width by the CTM,
	// do the inverse.
	fromPoints(widthInPoints, widthInPoints, a, b, False);
	if(a < 0.0)
		a = -a;
	if(b < 0.0)
		b = -b;
	float lw = (a + b)/2.0;

	emitLineWidth(lw);
}

void AvPSDriver::moveTo(const float x, const float y)
{
	setCurrXY(x, y);
	emitMoveTo(x, y);
}

// Draw lines from x0/y0 to x1/y1, etc. If close is non zero, draws a
// closed polygon. If fill is nonzero, the polygon is closed, then filled.
void AvPSDriver::drawPolyline(const int len, const float *x, const float *y,
			    const int close, const int fill)
{
	for(int i=0; i< len; i++)
		bbCheck(x[i], y[i]);
	setCurrXY(x[len-1], y[len-1]);

	emitPolyline(len, x, y, close, fill);
}

void AvPSDriver::drawPolygon(const int len, const float *x, const float *y,
			   const int fill)
{
	for(int i=0; i< len; i++)
		bbCheck(x[i], y[i]);
	setCurrXY(x[len-1], y[len-1]);

	emitPolygon(len, x, y, fill);
}

void AvPSDriver::drawLine(const float x0, const float y0,
			const float x1, const float y1)
{
	bbCheck(x0, y0);
	bbCheck(x1, y1);
	setCurrXY(x1, y1);
	emitLine(x0, y0, x1, y1);
}

// Draw unconnected lines from x1/y1 to x2/y2.
void AvPSDriver::drawLines(const int len, const float *x1, const float *y1,
					const float *x2, const float *y2)
{
	for(int i=0; i< len; i++)
	{	bbCheck(x2[i], y1[i]);
		bbCheck(x2[i], y2[i]);
	}
	setCurrXY(x2[len-1], y2[len-1]);

	emitLines(len, x1, y1, x2, y2);
}

void AvPSDriver::drawRectangle(const float x0, const float y0,
			     const float x1, const float y1,
			     const int fill)
{
	bbCheck(x0, y0);
	bbCheck(x1, y1);
	if(lineStyle_ == DASHDASH)
	{	comment("### Start DashDash rectangle");
		emitGSave();
		 emitGSave();
		  emitLineStyle(SOLID);
		  *out << "useBGColor" << endl;
		  emitRectangle(x0, y0, x1-x0, y1-y0, fill);
		 emitGRestore();
		 emitLineStyle(DASHED);
		 emitRectangle(x0, y0, x1-x0, y1-y0, fill);
		emitGRestore();
		comment("### End DashDash rectangle");
	}
	else
		emitRectangle(x0, y0, x1-x0, y1-y0, fill);
}

// Implemented as draw rectangle for now.
void AvPSDriver::drawRoundedRectangle(const float x0, const float y0,
				    const float x1, const float y1,
				    const unsigned int , const unsigned int ,
				    const int fill)
{
	drawRectangle(x0, y0, x1, y1, fill);
}

void AvPSDriver::drawFilledRectangle(const float x0, const float y0,
				   const float x1, const float y1)
{
	drawRectangle(x0, y0, x1, y1, 1);
}

// Points are drawn as circles, since just drawing a 0 length line is
// barely visible.
void AvPSDriver::drawPoint(const float x, const float y, const float radius)
{
	bbCheck(x-radius, y-radius);
	bbCheck(x+radius, y+radius);
	setCurrXY(x, y);
	emitPoint(x, y, radius);
}

void AvPSDriver::drawPoints(const int len, const float *x, const float *y,
			  const float radius)
{
	emitPoints(len, x, y, radius);
}

void AvPSDriver::drawColoredPoints(const int len,
				 const float *x, const float *y,
				 const float *colors, const float radius)
{
	gsave();	// Retain current color.
	for(int i=0; i < len; i++)
	{	setColor(*colors++);
		drawPoint( *x++, *y++, radius);
	}
	grestore();
}

// Rotate by degrees.
void AvPSDriver::rotate(const float degrees)
{
	state_.rotate(degrees);
	emitRotate(degrees);
}

// Scale. (new scale = <old scale>*<new scale>.
void AvPSDriver::scale(const float x, const float y)
{
	state_.scale(x, y);
	emitScale(x, y);
}

// Translate: set origin. X & Y are relative current origin.
void AvPSDriver::translate(const float x, const float y)
{
	state_.translate(x, y);
	emitTranslate(x, y);
}

////////////////////////////
//	Stroke and closePath are called internally.
// Display current path.
void AvPSDriver::stroke()
{
	*out << " stroke" << endl;
}

// Close path.
void AvPSDriver::closePath()
{
	*out << "closepath" << endl;
}

// Start a new graphics path.
void AvPSDriver::newPath()
{
	*out << "newpath" << endl;
}

void AvPSDriver::newPage()
{
	emitShowPage();
	emitPageNum();
}

void AvPSDriver::flush()
{
	(*out).flush();
}

// Set clipping rectangle.
// Subsequent calls can only make the clipping rectangle smaller.
void AvPSDriver::clipRect(const float x0, const float y0,
			const float width, const float height)
{
	if(checkBoundingBox_)
	{  float x1p, y1p, x0p, y0p;
		toPoints(x0+width, y0+height, x1p, y1p);
		toPoints(x0, y0, x0p, y0p);

		if(x0p <= x1p)
		{	clipXll_ = x0p;
			clipXur_ = x1p;
		}
		else
		{	clipXll_ = x1p;
			clipXur_ = x0p;
		}

		if(y0p <= y1p)
		{	clipYll_ = y0p;
			clipYur_ = y1p;
		}
		else
		{	clipYll_ = y1p;
			clipYur_ = y0p;
		}
	}
	emitClipRect(x0, y0, width, height);
}


////////////////////////////////////////////////////////////////
//			Text

// Use fn and default scaling as current text font.
void AvPSDriver::setFont(const char *fn)
{
	findFont(fn, defaultFontSize_);
}

void AvPSDriver::findFont(const char *fn, const float scale)
{	emitFindFont(fn, scale);
}

// Draw text starting at point x, y.
// Newlines, tabs, etc. are not handled.
void AvPSDriver::drawText(const float x, const float y, const char *str,
			const TextAlign algn)
{ int len;

	if((str == NULL) || ((len = (int)strlen(str)) == 0))
		return;

	if(checkBoundingBox_)
	{float xoff, yoff;
	 float x0 = x, y0 = y;
	 float width, height;
		fromPoints(defaultFontSize_, defaultFontSize_,
			   width, height);
		width *= len;

		switch(algn) {
		case AlignCenter:
			xoff = yoff = 0.5;
			break;
		case AlignLeft:
			xoff = 0.0;
			yoff = 0.5;
			break;
		case AlignTop:
			xoff = 0.5;
			yoff = 1.0;
			break;
		case AlignRight:
			xoff = 1.0;
			yoff = 0.5;
			break;
		case AlignBottom:
			xoff = 0.5;
			yoff = 0.0;
			break;
		case AlignTopLeft:
			xoff = 0.0;
			yoff = 1.0;
			break;
		case AlignTopRight:
			xoff = 1.0;
			yoff = 1.0;
			break;
		case AlignBottomLeft:
			xoff = 0.0;
			yoff = 0.0;
			break;
		case AlignBottomRight:
			xoff = 1.0;
			yoff = 0.0;
			break;
		default:
			xoff = 0.0;
			yoff = 0.0;
		}

		bbCheck(x0 - xoff*width, y0 - yoff*height);
		bbCheck(x0 + (1.-xoff)*width, y0 + (1.0-yoff)*height);
		// This is only approximate.
		setCurrXY(x0 + (1.-xoff)*width, y0 + (1.0-yoff)*height);
	}
	emitText(x, y, str, algn);
}

// Page size (drawable area - margins).
// If inPoints is False, values are in current user coordinates.
// Otherwise, they are in points.
void AvPSDriver::pageSize(float &width, float &height,
			  const Boolean inPoints)
{ float xur, yur, xll, yll;

	if(! inPoints)
	{
		fromPoints(xll_, yll_, xll, yll);
		fromPoints(xur_, yur_, xur, yur);
	}
	else
	{	xll = xll; xur = xur_; yll = yll_; yur = yur_;
	}

	width = xur - xll;
	height = yur - yll;
	// If in landscape, numbers may be negative.
	if(width < 0.0)
		width = -width;
	if(height < 0.0)
		height = -height;
#if 0
printf("xur/yur/xll/yll %f/%f/%f/%f width/height %f/%f\n",
	xur, yur, xll, yll, width, height);
printf("xur_/yur_/xll_/yll_ %f/%f/%f/%f width/height %f/%f\n",
	xur_, yur_, xll_, yll_, xur_-xll_, yur_-yll_);
#endif

}

// Returns current value of bounding box.
Bool AvPSDriver::getBoundingBox( float &x0, float &y0, float &x1, float &y1,
				 const Boolean inPoints)const
{	x0 = bbx0_;
	y0 = bby0_;
	x1 = bby1_;
	y1 = bby1_;
	if(! inPoints)
	{	fromPoints( x0, y0, x0, y0);
		fromPoints( x1, y1, x1, y1);
	}
	return haveBoundingBox();
}

// Check x & y to see if bounding box needs changing. X & Y are in user
// coords unless inPoints is True in which case they are in points.
void AvPSDriver::bbCheck(const float X, const float Y, const Boolean inPoints)
{float x, y;

	if(!checkBoundingBox_)
		return;

	if(inPoints)
	{	x = X;
		y = Y;
	}
	else
		toPoints(X, Y, x, y);

	if(boxCheck0_)			// First time.
	{	bbx0_ = bbx1_ = x;
		bby0_ = bby1_ = y;
		boxCheck0_ = False;
	}
	else
	{	if( x < bbx0_)
			bbx0_ = x;
		else
		if( x > bbx1_)
			bbx1_ = x;

		if( y < bby0_)
			bby0_ = y;
		else
		if( y > bby1_)
			bby1_ = y;
	}
}

// Put a, possibly multiline, comment in the output file.
void AvPSDriver::comment(const char *str)
{ const char *p = str;
  char c;

	if(str == NULL)
		return;

	*out << "% ";
	while( (c = *p++) != '\0')
	{	if(c == '\n')
			*out << endl << "% ";
		else
			*out << c;
	}
	*out << endl;
}

void AvPSDriver::comment(const AvString &str)
{
	if(str.empty())
		return;
	comment(str.chars());
}

#if 0
// Write raw PostScript code.
void AvPSDriver::pscode(const char *str)
{	*out << str << endl;
}
#endif

////////////////////////////////////////////////////////////////

/*****************************************************************/
/* Support for converting to ASCII85:  4 bytes -> 5 characters.  */
/*****************************************************************/

const short BASE=33;	// ASCII "!". 0 gets mapped to this.

// Convert a single unsigned int to 5 ASCII characters.
// Special case: returns 'z' if in is zero and special_case is true.
// Returns # of characters stored in out (either 1 or 5).
// out has to be at least 5 characters long.
// Assumes unsigned ints are at least 32 bits.
inline
int unsignedToAscii(const unsigned int in, char out[5], int special_case=0)
{ register unsigned int v=in;
  register int i;

	if((in == 0) && special_case)
	{	out[0] = 'z';
		return 1;
	}

	for(i=0; i< 5; i++)
	{	out[4-i] = (v % 85) + BASE;
		v /= 85;
	}
	return 5;
}

// Convert an array of unsigned bytes to ASCII85 encoding.
// The output array must be at least 5/4 as long as the input.
// Returns the number of characters placed in out.
int AvPSDriver::bytesToAscii(const char *in, const int inlength, char *out)
{int num = inlength;
 char *out0 = out;

	while(num >= 4)
	{ unsigned int v;
		v = *in++ & 0xff;	// Pack 4 bytes into an unsigned int.
		v <<= 8;
		v |= *in++ & 0xff;
		v <<= 8;
		v |= *in++ & 0xff;
		v <<= 8;
		v |= *in++ & 0xff;
		num -= 4;

		int n = unsignedToAscii(v, out, 1);
		out += n;
	}

	// Deal with the last 1-3 characters.
	if(num > 0)
	{ unsigned int v = 0;
	  char c, buf[5];
	  int j;
		for(j=0; j<4; j++)
		{	if(j < num)
				c = *in++;
			else
				c = 0;
			v = (v << 8) | c;
		}
		unsignedToAscii(v, buf, 0);	// Don't special case last 0.
		for(j=0; j < num+1; j++)	// copy N+1 output chars.
			*out++ = buf[j];
	}
	return  (int)(out-out0);
}

// Encode bytes stored 1 per int as ASCII85.
// Out must have 5/4 as many elements as in.
int AvPSDriver::ints8ToAscii(const int *in, const int inlength, char *out)
{int num = inlength;
 char *out0 = out;

	while(num >= 4)
	{ unsigned int v;
		v = *in++ & 0xff;	// Pack 4 bytes into an unsigned int.
		v <<= 8;
		v |= *in++ & 0xff;
		v <<= 8;
		v |= *in++ & 0xff;
		v <<= 8;
		v |= *in++ & 0xff;
		num -= 4;

		int n = unsignedToAscii(v, out, 1);
		out += n;
	}

	// Deal with the last 1-3 characters.
	if(num > 0)
	{ unsigned int v = 0;
	  char c, buf[5];
	  int j;
		for(j=0; j<4; j++)
		{	if(j < num)
				c = *in++ && 0xff;
			else
				c = 0;
			v = (v << 8) | c;
		}
		unsignedToAscii(v, buf, 0);	// Don't special case 0.
		for(j=0; j < num+1; j++)	// copy N+1 output chars.
			*out++ = buf[j];
	}
	return  (int)(out-out0);
}

int AvPSDriver::unsignedLongs8ToAscii(const unsigned long *in,
				     const int inlength, char *out)
{int num = inlength;
 char *out0 = out;

	while(num >= 4)
	{ unsigned int v;
		// Pack 4 bytes into an unsigned int.
		v = (unsigned int)(*in++ & 0xff);
		v <<= 8;
		v |= (unsigned int)(*in++ & 0xff);
		v <<= 8;
		v |= (unsigned int)(*in++ & 0xff);
		v <<= 8;
		v |= (unsigned int)(*in++ & 0xff);
		num -= 4;

		int n = unsignedToAscii(v, out, 1);
		out += n;
	}

	// Deal with the last 1-3 characters.
	if(num > 0)
	{ unsigned int v = 0;
	  char c, buf[5];
	  int j;
		for(j=0; j<4; j++)
		{	if(j < num)
				c = *in++ && 0xff;
			else
				c = 0;
			v = (v << 8) | c;
		}
		unsignedToAscii(v, buf, 0);	// Don't special case 0.
		for(j=0; j < num+1; j++)	// copy N+1 output chars.
			*out++ = buf[j];
	}
	return  (int)(out-out0);
}

int AvPSDriver::intsToAscii(const int *in, const int inlength, char *out)
{int num = inlength;
 char *out0 = out;

	while(num-- > 0)
	{ unsigned int v = (unsigned int) *in++;

		int n = unsignedToAscii(v, out, 1);
		out += n;
	}

	// Deal with the last value.
	if(num > 0)
	{ unsigned int v = *in;
	  char buf[5];
		unsignedToAscii(v, buf, 0);	// Don't special case 0.
		for(int j=0; j < num+1; j++)	// copy N+1 output chars.
			*out++ = buf[j];
	}
	return  (int)(out-out0);
}

int AvPSDriver::unsignedLongsToAscii(const unsigned long *in,
			    const int inlength, char *out)
{int num = inlength;
 char *out0 = out;

	while(num-- > 0)
	{ unsigned int v = (unsigned int) *in++;

		int n = unsignedToAscii(v, out, 1);
		out += n;
	}

	// Deal with the last value.
	if(num > 0)
	{ unsigned int v = (unsigned int) *in;
	  char buf[5];
		unsignedToAscii(v, buf, 0);	// Don't special case 0.
		for(int j=0; j < num+1; j++)	// copy N+1 output chars.
			*out++ = buf[j];
	}
	return  (int)(out-out0);
}

int AvPSDriver::shortsToAscii(const short *in, const int inlength, char *out)
{int num = inlength;
 char *out0 = out;

	while(num-- > 0)
	{ unsigned int v = (unsigned int) *in++;

		int n = unsignedToAscii(v, out, 1);
		out += n;
	}

	// Deal with the last value.
	if(num > 0)
	{ unsigned int v = *in;
	  char buf[5];
		unsignedToAscii(v, buf, 0);	// Don't special case 0.
		for(int j=0; j < num+1; j++)	// copy N+1 output chars.
			*out++ = buf[j];
	}
	return  (int)(out-out0);
}

////////////////////////////////////////////////////////////////
//			Encode 12 bit numbers.

// Pack lower 12 bits of two unsigned shorts into three bytes.
static inline void pack12us(const unsigned short in1,
			    const unsigned short in2,
			    char out[3])
{
	out[0] = ((in1 >> 4) & 0xFF);		// First byte.
	out[2] =  (in2 & 0XFF);			// Last byte.
	unsigned short s = in1 & 0xF;		// Low 4 bits of in1
	s <<= 4;				//  to upper 4 bits of byte
	s |= ((in2 >> 8) & 0xF);		// + upper 4 bits of in2.
	out[1] = s & 0xFF;
}

// Pack lower 12 bits of two unsigned ints into three bytes.
inline
static void pack12ui(const unsigned int in1, const unsigned int in2,
		    char out[3])
{	pack12us((unsigned short)(in1 & 0xFFF),
		 (unsigned short) (in2 & 0xFFF),
		 out);
}

// Converts in to ASCII85 bytes using only lower 12 bits of each integer.
// Packs pairs of 12 bit shorts into 3 bytes and then converts to ASCII85.
// Out must be at least 1.75*inlength long.
// Returns the number of characters stored in out.
// Pack 12 bit ints into ASCII85 format. Each row starts on a byte boundary.
int AvPSDriver::ints12ToAscii(const int *in,
			    const int w, const int h, char *out)
{ int num = w*h;

	char *inb = new char[num*2];
	char *inbp = inb;

	for(int r=0; r < h; r++)
	{int nc = w;
		while(nc >= 2)		// Do one row.
		{ unsigned int v1, v2;
			v1 = *in++; v2 = *in++;
			pack12ui(v1, v2, inbp);
			inbp += 3;
			nc -= 2;
		}
		if(nc > 0)		// Need to pad to even # of pixels.
		{	pack12ui((unsigned int) *in++, (unsigned int) 0,
				 inbp);
			inbp += 2;
		}
	}

	int nbytes = (int)(inbp-inb);
	int nascii = bytesToAscii(inb, nbytes, out);
	delete [] inb;

	return nascii;
}

// Converts in to ASCII85 bytes using only lower 12 bits of each integer.
// Packs pairs of 12 bit shorts into 3 bytes and then converts to ASCII85.
// Out must be at least 1.75*inlength long.
// Returns the number of characters stored in out.
// Pack 12 bit ints into ASCII85 format. Each row starts on a byte boundary.
int AvPSDriver::unsignedLongs12ToAscii(const unsigned long *in,
			    const int w, const int h, char *out)
{ int num = w*h;

	char *inb = new char[num*2];
	char *inbp = inb;

	for(int r=0; r < h; r++)
	{int nc = w;
		while(nc >= 2)		// Do one row.
		{ unsigned int v1, v2;
			v1 = (unsigned int)*in++; v2 = (unsigned int)*in++;
			pack12ui(v1, v2, inbp);
			inbp += 3;
			nc -= 2;
		}
		if(nc > 0)		// Need to pad to even # of pixels.
		{	pack12ui((unsigned int) *in++, (unsigned int) 0,
				 inbp);
			inbp += 2;
		}
	}

	int nbytes = (int)(inbp-inb);
	int nascii = bytesToAscii(inb, nbytes, out);
	delete [] inb;

	return nascii;
}

// Encode data as ASCII85 chars. Function return is an array of
// chars. (use delete [] to free). outlen is set to the number of
// bytes in the array. Width and height are specified since it may be
// necessary to start each row on a byte boundary.
char *AvPSDriver::encodeInts(const int width, const int height, int &BPC,
			   const int *data, int &outlen)
{int bpc;	// Bits per component.
 int numdata = width*height;

	////////////////////////////////////////////////////////////////
	// Choose # of bits (8 or 12) to use for encoding.
	if(BPC <= 0)	// Determine from data.
	{	bpc = 8;
		for(int i=0; i< numdata; i++)
		 if(data[i] > 255)	// 1 byte can hold up to 255.
		 {	bpc = 12;
			break;
		 }
	}
	else
	if( BPC <= 8)
		bpc = 8;		// Only support 8 & 12 bits.
	else
		bpc = 12;

	////////////////////////////////////////////////////////////////
	// Compute length of output char buffer.
	int bsize;
	{ int ncols = (width+1)& ~0x1;	// Round up to even # of bytes.
	  int ndata = ncols*height;
	  float x = ndata;
		if((bpc > 8) && (ndata & 0x1))
			x +=1;			// 12 bit packing uses 2 ints.
		if(bpc > 8) x *= 1.5;		// 1.5 bytes/<12 bits>.
		x *= 1.25;			// Byte -> ASCII85 expansion.
		bsize = (int) (x + 0.5);
		bsize += 5;			// Ensures >= multiple of 5.
	}

	char *buf = new char[bsize];
	int nascii;

	if(bpc == 8)
		nascii = ints8ToAscii(data, numdata, buf);
	else
		nascii = ints12ToAscii(data, width, height, buf);

#if 0
  cerr << " BPC = " << bpc << endl;
  cerr	<< "encodeInts: # data items " << numdata
	<< " bufsize = " << bsize << " # output bytes = " << nascii
	<< " (delta = " << bsize-nascii << ")" << endl;
#endif

	if(nascii > bsize)	// Overflow check, just in case.
		cerr << "encodeInts: !!!!Buffer overflow!!!!" << endl;

	BPC = bpc;
	outlen = nascii;
	return buf;
}

char *AvPSDriver::encodeUnsignedLongs(const int width, const int height,
			   int &BPC,
			   const unsigned long *data, int &outlen)
{int bpc;	// Bits per component.
 int numdata = width*height;

	////////////////////////////////////////////////////////////////
	// Choose # of bits (8 or 12) to use for encoding.
	if(BPC <= 0)	// Determine from data.
	{	bpc = 8;
		for(int i=0; i< numdata; i++)
		 if(data[i] > 255)	// 1 byte can hold up to 255.
		 {	bpc = 12;
			break;
		 }
	}
	else
	if( BPC <= 8)
		bpc = 8;		// Only support 8 & 12 bits.
	else
		bpc = 12;

	////////////////////////////////////////////////////////////////
	// Compute length of output char buffer.
	int bsize;
	{ int ncols = (width+1)& ~0x1;	// Round up to even # of bytes.
	  int ndata = ncols*height;
	  float x = ndata;
		if((bpc > 8) && (ndata & 0x1))
			x +=1;			// 12 bit packing uses 2 ints.
		if(bpc > 8) x *= 1.5;		// 1.5 bytes/<12 bits>.
		x *= 1.25;			// Byte -> ASCII85 expansion.
		bsize = (int) (x + 0.5);
		bsize += 5;			// Ensures >= multiple of 5.
	}

	char *buf = new char[bsize];
	int nascii;

	if(bpc == 8)
		nascii = unsignedLongs8ToAscii(data, numdata, buf);
	else
		nascii = unsignedLongs12ToAscii(data, width, height, buf);

#if 0
  cerr << " BPC = " << bpc << endl;
  cerr	<< "encodeInts: # data items " << numdata
	<< " bufsize = " << bsize << " # output bytes = " << nascii
	<< " (delta = " << bsize-nascii << ")" << endl;
#endif

	if(nascii > bsize)	// Overflow check, just in case.
		cerr << "encodeInts: !!!!Buffer overflow!!!!" << endl;

	BPC = bpc;
	outlen = nascii;
	return buf;
}

////////////////////////////////////////////////////////////////
//////			Low level code emitters
////////////////////////////////////////////////////////////////

//	Header used to start off the file.
// Besides the initial %!PS, the lookup routine for the color tables and
// the color tables themselves are defined and initialized to linear ramps.
// (Colors range from (0..1). PostScript supports 4096 entry tables.

static const char epsheader[] = {
"%!PS-Adobe-3.0 EPSF-3.0\n\
%%LanguageLevel: 2"
};

static const char noepsheader[] = {
"%!PS\n\
%%LanguageLevel: 2"
};

static const char setupstr[] = {
"%%BeginSetup\n\
% Global color tables used for converting from index to RGB.\n\
/color1 4096 array def\n\
/color2 4096 array def\n\
/color3 4096 array def\n\
\n\
% Load color tables with linear ramps (0..1).\n\
% Only the first N entries are set where N is the value on the stack.\n\
/loadRamps	%Stack: maxcolorindex - \n\
 {\n\
  /maxColorIndex exch def\n\
\n\
  % Place linear ramp in first table, then copy.\n\
  0 1 maxColorIndex { color1 exch dup maxColorIndex div put} for\n\
  color2 0 color1 putinterval\n\
  color3 0 color1 putinterval\n\
 } def\n\
4095 loadRamps		% Initialize color tables\n\
\n\
% Convert color index to RGB triple via table lookup.\n\
% Uses the red, green and blue color tables.\n\
% No error checking.\n\
/index2color {			% Stack: colorindex -> R G B\n\
	dup color1 exch get exch\n\
	dup color2 exch get exch\n\
	color3 exch get\n\
  } def\n\
\n\
 % Return the width & height of a string.\n\
/stringsize {			% Stack: (string) -> height width\n\
	gsave\n\
	 newpath\n\
	 0 0 moveto\n\
	 false charpath flattenpath pathbbox\n\
	grestore\n\
	3 -1 roll exch sub 3 1 roll sub\n\
  } def\n\
/NaN 1E20 def /-NaN 1E20 def	% In case NaNs creep in.\n\
% Draw dashdash.\n\
/enabledashes { dashlength 0 setdash} bind def\n\
/dashdash {		% x y - \n\
 gsave\n\
  2 copy\n\
  [] 0 setdash		% Draw solid line in background color.\n\
  useBGColor\n\
  lineto stroke\n\
 grestore\n\
  enabledashes		% Overlay with dashed line in current color.\n\
  lineto currentpoint stroke moveto	%Want a current point after stroke.\n\
} bind def"

};
////////////////////////////////////////////////////////////////

void AvPSDriver::startDocument()
{
	emitHeader();
	emitProlog();
	emitSetup();
	emitPageNum();
}

void AvPSDriver::emitHeader()
{
	if(isEPS())
		*out << epsheader << endl;
	else
		*out << noepsheader << endl;

#if 1
	*out << "%%Creator: ( aipsview " << AvVersion::version() << ")"
	     << endl;
#else
	if((info_ != NULL) && ! info_->creator().empty())
	{	*out << "%%Creator: (" << info_->creator_.chars() << ")"
		     << endl;
	}
	else
	{  *out << "%%Creator: ( aipsview " << AvVersion::version() << ")"
		<< endl;
	}
#endif

	if(haveBoundingBox())
		emitBoundingBox();
	else
		*out << "%%BoundingBox: (atend)" << endl;

#if defined(SHOW_DATE)
	static const int NCHRS=128;
	char str[NCHRS];
	time_t clock = time(0);
	// This generates an ignoreable warning from egcs.
	strftime(str, NCHRS, "%c", localtime(&clock));
	*out << "%%CreationDate: (" << str << ")" << endl;
#endif

	if(portrait_)
		*out << "%%Orientation: Portrait" << endl;
	else
		*out << "%%Orientation: Landscape" << endl;

	if(info_ != NULL)
	{	if(!info_->For().empty())
			*out << "%%For: " << info_->For() << endl;
		if(!info_->Title().empty())
			*out << "%%Title: " << info_->Title() << endl;
	}

	*out << "%%EndComments" << endl;

	if((info_ != NULL) && !info_->Comment().empty())
		comment(info_->Comment());	// Not part of header cmnts.
}

void AvPSDriver::emitProlog()
{
	*out << "%%BeginProlog" << endl;
	*out << "%%EndProlog" << endl;
}

void AvPSDriver::emitSetup()
{	*out << setupstr << endl;

	 if( isPortrait())
	 {	translate( xll_, yll_);
		scale(xscale_, yscale_);
	 }
	 else
	 {
		translate( xur_, yll_);
		rotate(90.0);
		scale(xscale_, yscale_);
	 }

	emitColorSpace(colorSpace_);
	// Set background color to white.
	emitBackgroundColor(1.0, 1.0, 1.0, RGB);
	emitDashLength(2.0);
	*out << "%%EndSetup" << endl;
}

void AvPSDriver::emitPageNum()
{
	*out << "%%Page: " << pageNum_++ << endl;
}

void AvPSDriver::emitTrailer()
{
	*out << "%%Trailer" << endl;
	if(!haveBoundingBox())
	{ float xll, yll, xur, yur;
		emitBoundingBox();	// Write computed bounding box.
		getBoundingBox(xll, yll, xur, yur);
		*out << "% BB in user coords: " <<
			xll << " " << yll
		     <<	" " << xur << " " << yur << endl;
	}
}

void AvPSDriver::emitShowPage(const int force)
{
	if(isEPS() && !force)	// Ignore showpage for EPS.
		*out << "% ";
	*out << "showpage" << endl;
}

////////////////////////////////////////////////////////////////
// String used to build image command.
static const char imgstr[]= {
"<<\n\
 /ImageType 1\n\
 /Width %d /Height %d\n\
 /BitsPerComponent %d\n\
 /Decode [0 %d]\n\
 /ImageMatrix [%f %f %f %f %f %f]\n\
 /Interpolate %s\n\
 /DataSource currentfile /ASCII85Decode filter\n\
>>\n\
image\n"};

static const int LW=80;
// Draw a width x height PS image.
//  imagematrix		6 element array that defines transformation from
//			user space to image space:
//			m = [ a b c d tx ty ]
//			 x' = ax + cy + tx
//			 y' = bx + dy + ty
//	The transformation is such that [ width 0 0 height 0 0] will map
//	the image into a unit square which would then be scaled by the
//	current scale() values.
//	To invert the image, use: [ width 0 0 -height 0 height].
//	If imagematrix is NULL, the image will be mapped into the unit square.
//  width, height	size of input image in pixels.
//  bpc			bits per color component. ( 8 or 12)
//  smooth		0 - no smoothing, 1 - smooth pixels.
//  ascii85		data encoded as ASCII85 chars.
//  len			length of data array in chars.
void AvPSDriver::emitImage(const float imagematrix[6],
			 const int width, const int height, const int bpc,
			 const int smooth, const char *ascii85, const int len)
{ char *buf = new char[sizeof(imgstr) + 128]; // More than enough room.
  int num=len, i;

	// We do single image draws in INDEXED mode.
	ColorSpace cs = colorSpace();
	if(cs != INDEXED)
		setColorSpace(INDEXED);

	// Build/write PostScript image command.
	if(imagematrix != NULL)
	{ const float *m=imagematrix;
		sprintf(buf, imgstr, width, height, bpc, (1 << bpc) -1,
			m[0], m[1], m[2], m[3], m[4], m[5],
			(smooth) ? "true" : "false" );
	}
	else	// Result will be a unit square at 0,0.
		sprintf(buf, imgstr, width, height, bpc, (1 << bpc) -1,
			(float)width, 0.0, 0.0, (float)height, 0.0, 0.0,
			(smooth) ? "true" : "false" );
	*out << buf;
	delete [] buf;

	// Write data LW chars/line.
	while( num > LW)
	{	for(i=0; i< LW; i++)
			*out << *ascii85++;
		*out << endl;
		num -= LW;
	}

	for(i=0; i< num; i++)
		*out << *ascii85++;
	*out << "~>" << endl;

	if(cs != INDEXED)
		setColorSpace(cs);	// Return to previous color space.
}

// Load color table i (1-3) starting at start with len elments of ary.
// Values are clamped to the range (0..1).
void AvPSDriver::emitStoreColors( const int color,
				const int start, const int len,
				const float *ary)
{ int i;

	*out << "color" << color << " " << start << " [";
	for(i=0; i< len; i++)
	{	if( (i % 12) == 0)	// 12 entries/line is a so so default.
			*out << endl;
		else
			*out << " ";
		{float v = ary[i];
			if(v < 0.0)
				v = 0.0;
			else
			if(v > 1.0)
				v = 1.0;
			*out << v;
		}
	}
	*out << "] putinterval" << endl;
}

// Load color tables with contents of a, b & c. Typically, a holds
// red, b holds green & c holds blue. Values should be in the
// range (0..1).
// Color tables may be set at anytime, but are only used in INDEX mode.
void AvPSDriver::emitStoreColors(const int start, const int len,
			       const float *a, const float *b, const float *c)
{	emitStoreColors(1, start, len, a);
	emitStoreColors(2, start, len, b);
	emitStoreColors(3, start, len, c);
	if(colorSpace_ == INDEXED)
		emitColorSpace(colorSpace_);// Work around for ghostscript.
}

// Scatter load the colortable. Much more effecient if indexes are
// consecutive.
void  AvPSDriver::emitStoreColorValues(const int len, const int *indexes,
			       const float *a, const float *b, const float *c)
{int numToGo = len;

	while(numToGo > 0)
	{int start, num;
		// Collect consecutive indexes.
		start = *indexes;
		for(num=1; num<numToGo; num++)
		{	if(indexes[num] != indexes[num-1] + 1)
				break;
		}
		// Store their values.
		if(num == 1)
		{	*out << " color1 " << start << " " << *a << " put";
			*out << " color2 " << start << " " << *b << " put";
			*out << " color3 " << start << " " << *c << " put"
			     << endl;
		}
		else
		{	emitStoreColors(1, start, num, a);
			emitStoreColors(2, start, num, b);
			emitStoreColors(3, start, num, c);
		}
		indexes += num;
		a += num;
		b += num;
		c += num;
		numToGo -= num;
	}

	if(colorSpace_ == INDEXED)
		emitColorSpace(colorSpace_);// Work around for ghostscript.
}

// Load one value into color table.
void  AvPSDriver::emitStoreColor( const int index,
				const float r, const float g, const float b)
{
	*out << " color1 " << index << " " << r << " put";
	*out << " color2 " << index << " " << g << " put";
	*out << " color3 " << index << " " << b << " put" << endl;
	if(colorSpace_ == INDEXED)
		emitColorSpace(colorSpace_);// Work around for ghostscript.
}


static inline float clampColor(const float a)
{
	if(a <= 0.0)
		return 0.0;
	else
	if( a >= 1.0)
		return 1.0;
	else
		return a;
}

// Set color to use for text and vectors.
// If colorspace is INDEXED, color value ranges from 0 to NCOLORS-1.
// For the others, color ranges from 0. to 1.0.
// INDEXED and GRAY only use the first argument.
void AvPSDriver::emitSetColor(const ColorSpace cs,
			    const float rh, const float gs, const float bv)
{
	// (setrgbcolor & sethsbcolor both set colorspace to DeviceRGB).
	switch(cs) {
	case INDEXED:
		*out << (int)rh << " setcolor" << endl;
		break;
	case GRAY:
		*out << clampColor(rh) << " setcolor" << endl;
		break;
	case RGB:
		*out	<< clampColor(rh) << " " << clampColor(gs)
			<< " " << clampColor(bv) << " setrgbcolor" << endl;
		break;
	case HSB:
		*out	<< clampColor(rh) << " " << clampColor(gs)
			<< " " << clampColor(bv) << " sethsbcolor" << endl;
		break;
	default:
		break;
	}
}

// Load color first ncolors entries in the tables with linear
// ramps scaled 0..1.
void AvPSDriver::emitSetLinearRamps(const int num)
{ int n = num;

	if(n > 4096)
		n = 4096;
	else if(n <= 0)
		n = 1;
	*out << n - 1 << " loadRamps"	// loadRamps takes max index.
	     << endl;
	emitColorSpace(colorSpace_);	// Work around for ghostscript.
}

void AvPSDriver::emitGSave()
{
	*out << "gsave" << endl;
}

void AvPSDriver::emitGRestore()
{
	*out << "grestore" << endl;
}

// Change PostScript's ColorSpace. Does not change our setting.
void AvPSDriver::emitColorSpace(const ColorSpace cs)
{
	switch(cs) {
	case INDEXED:
		*out <<
		  "[/Indexed /DeviceRGB 4095 { index2color}] setcolorspace"
		  << endl;
		break;
	case GRAY:
		*out << "/DeviceGray setcolorspace" << endl;
		break;
	case RGB:
	case HSB:	// PostScript treats HSB/HSV as RGB.
		*out << "/DeviceRGB setcolorspace" << endl;
		break;
	case UNKNOWNSPACE:
	default:		// Error;
		return;		// Ignore;
	}
}

// Dashes are drawn using current user coordinates (eg scale).
void AvPSDriver::emitLineStyle(const LineStyle style)
{
	switch (style) {
	case SOLID:
		*out << "[] 0 setdash" << endl;
		break;
	case DASHED:
	case DASHDASH:
		*out << "enabledashes" << endl;
		break;
	default:
		return;
	}
}

#if 0
void AvPSDriver::emitDashLength(const float length)
{
	*out << "/dashlength [" << length/scale() << "] def" << endl;
}
#else
// Length is supposed to be indepented of current scaling.
void AvPSDriver::emitDashLength(const float length)
{ float a, b, l;

	fromPoints(length, length, a, b, False);
	l = (a+b)/2.0;
	*out << "/dashlength [" << l << "] def" << endl;
}
#endif

// See also setstrokeadjust in the ref. manual.
void AvPSDriver::emitLineWidth(const float w)
{
	*out << w << " setlinewidth" << endl;
}

void AvPSDriver::emitMoveTo(const float x, const float y)
{
	*out << x << " " << y << " moveto" << endl;
}

// Draw line from current point to x,y.
void AvPSDriver::emitLineTo(const float x, const float y, const int strk)
{
	if(lineStyle_ == DASHDASH)
		*out << x << " " << y << " dashdash" << endl;
	else
	{	*out << x << " " << y << " lineto" << endl;
		if(strk)
			stroke();
	}
}

// Draw lines from x0/y0 to x1/y1, etc. If close is non zero, draws a
// closed polygon. If fill is nonzero, the polygon is closed, then filled.
void AvPSDriver::emitPolyline(const int len, const float *x, const float *y,
			    const int close, const int fill)
{
	emitMoveTo( *x++, *y++);
	for(int i=1; i< len; i++)
		emitLineTo(*x++, *y++, 0);
	if(fill)
		*out << " fill" << endl;	// Does implicit close.
	else
	{	if(close)
			closePath();
		stroke();
	}
}

void AvPSDriver::emitPolygon(const int len, const float *x, const float *y,
			   const int fill)
{
	emitPolyline(len, x, y, 1, fill);
}

void AvPSDriver::emitLine(const float x0, const float y0,
			const float x1, const float y1)
{
	emitMoveTo(x0, y0);
	emitLineTo(x1, y1);
}

// Draw unconnected lines from x1/y1 to x2/y2.
// If the list is <moveto p1>,<drawto p2>, <moveto p2>, <drawto p3>...,
// then the redundant movetos are not done.
void AvPSDriver::emitLines(const int len, const float *x1, const float *y1,
					const float *x2, const float *y2)
{ float lastx = *x1-1., lasty = *y1-1.;

	for(int i=0; i< len; i++)
	{ float xa = *x1++, ya = *y1++, xb = *x2++, yb = *y2++;

		// If the last end point is same as this move, ignore.
		if((xa != lastx) || (ya != lasty))
			emitMoveTo(xa, ya);
		emitLineTo(xb, yb, 0);
		lastx = xb; lasty = yb;
	}
	stroke();
}

void AvPSDriver::emitRectangle(const float x, const float y,
		   const float width, const float height, const int fill)
{
	*out << x << " " << y << " " << width << " " << height;
	if(fill)
		*out << " rectfill" << endl;
	else
		*out << " rectstroke" << endl;
}

// Points are drawn as circles, since just drawing a 0 length line is
// barely visible.
void AvPSDriver::emitPoint(const float x, const float y, const float radius )
{
#if 0
	moveTo(x, y);
	lineTo(x, y);
	if(strk)
		stroke();
#else
	emitMoveTo(x, y);
	gsave();
	// Draw an arc at current location. The matrix stuff is to prevent
	// current scaling to affect radius.
	*out << "matrix defaultmatrix setmatrix currentpoint "
	     << radius <<" 0 360 arc " << endl;
	*out << " fill" << endl;
	grestore();
#endif
}

void AvPSDriver::emitPoints(const int len, const float *x, const float *y,
			  const float radius)
{
	for(int i=0; i< len; i++)
		drawPoint(*x++, *y++, radius);
	stroke();
}

// Rotate by degrees.
void AvPSDriver::emitRotate(const float degrees)
{
	*out << degrees << " rotate" << endl;
}

// Scale. (new scale = <old scale>*<new scale>.
void AvPSDriver::emitScale(const float x, const float y)
{
	*out << x << " " << y << " scale" << endl;
}

// Translate: set origin. X & Y are relative current origin.
void AvPSDriver::emitTranslate(const float x, const float y)
{
	*out << x << " " << y << " translate" << endl;
}

void AvPSDriver::emitFindFont(const char *fn, const float scale)
{	if(fn == NULL)
		return;
	*out << "/" << fn << " findfont " << scale << " scalefont setfont"
	     << endl;
}

// Draw text starting at point x, y.
// Newlines, tabs, etc. are not handled. (They're treated as blanks).
void AvPSDriver::emitText(const float x, const float y, const char *str,
			const TextAlign algn)
{  float xoff, yoff;

	if(str == NULL)
		return;
	if(strlen(str) == 0)
		return;

	switch(algn) {

	case AlignCenter: // reference point aligned to center of text
		xoff = yoff = 0.5;
		break;
	case AlignLeft:   // reference point aligned to center of left edge
		xoff = 0.0;
		yoff = 0.5;
		break;
	case AlignTop:    // reference point aligned to center ot top edge
		xoff = 0.5;
		yoff = 1.0;
		break;
	case AlignRight:  // reference point aligned to center of right edge
		xoff = 1.0;
		yoff = 0.5;
		break;
	case AlignBottom: // reference point aligned to center of bottom edge
		xoff = 0.5;
		yoff = 0.0;
		break;
	case AlignTopLeft:// reference point aligned to top left corner
		xoff = 0.0;
		yoff = 1.0;
		break;
	case AlignTopRight:// reference point aligned to top right corner
		xoff = 1.0;
		yoff = 1.0;
		break;
	case AlignBottomLeft:// reference point aligned to bottom left corner
		xoff = 0.0;
		yoff = 0.0;
		break;
	case AlignBottomRight://reference point aligned to bottom right corner
		xoff = 1.0;
		yoff = 0.0;
		break;
	default:
		xoff = 0.0;
		yoff = 0.0;
	}

	moveTo(x, y);
	emitString(str);
	if(algn == AlignBottomLeft)	// Simple case
		*out << "show" << endl;
	else
	{	// Compute the string's size then adjust starting point.
		*out << " dup stringsize "
		     << xoff << " mul " << " exch "
		     << yoff << " mul " << " rmoveto show " << endl;
	}
}

// Just throw a string [ '(' <str> ')' ] on the stack and leave it there.
void AvPSDriver::emitString(const char *str)
{int len;

	if(str == NULL)
		return;
	if((len = (int)strlen(str)) == 0)
		return;
	if(strpbrk(str, "()") == NULL)
		*out << "(" << str << ") ";
	else	// Handle strings with parentheses.
	{ char *buf = new char[len*2+1]; // Enough room for all parens & NULL.
	  int j,i;

		// Copy chars to buf, escaping '(' & ')' by backslashes.
		for(j=0, i=0; i< len; i++)
		{ int c = str[i];
			if( (c == '(') || (c == ')'))
				buf[j++] = '\\';
			buf[j++] = c;
		}
		buf[j] = '\0';
		*out << "(" << buf << ") ";
		delete [] buf;
	}
}

void AvPSDriver::pushMatrix()
{
	emitPushMatrix();
}

void AvPSDriver::popMatrix()
{
	emitPopMatrix();
}


void AvPSDriver::emitPushMatrix()
{
	*out << " matrix currentmatrix" << endl;
}

// The only reason this works is that code emission routines leave
// the stack unchanged.
void AvPSDriver::emitPopMatrix()
{
	*out << " pop" << endl;
}

// Set clipping rectangle.
// Subsequent calls can only make the clipping rectangle smaller.
#if 0
	// Remove until we can undo clip.
void AvPSDriver::emitClipRect(const float x, const float y,
			const float width, const float height)
{
	*out << x << " " << y << " " << width << " " << height
	     << " rectclip" << endl;
}
#else
void AvPSDriver::emitClipRect(const float , const float ,
			    const float , const float )

{
}
#endif

// Create PS function to set background color when needed.
void AvPSDriver::emitBackgroundColor(const float a, const float b,
				   const float c, const ColorSpace cs)
{
	// Bracket a call to emitSetColor with a procedure definition.
	*out << "/" << BGNAME << " { ";
	 emitSetColor(cs, a, b, c);
	*out << "} bind def" << endl;
}

// Write value of bounding box to output.
void AvPSDriver::emitBoundingBox()
{	*out << "%%BoundingBox: "
	     << (int)bbx0_ << " " << (int)bby0_ << " "
	     << (int)(bbx1_ + 0.5) << " "
	     << (int)(bby1_ + 0.5) << endl;
}

void AvPSDriver::pushState()
{
	if(statestackindex_ < STATESTACKLENGTH)
	{	statestack_[statestackindex_++] = state_;
	}
	else
	{	cerr << "AvPSDriver::pushState: stack overflow." << endl;
	}
	return;
}

void AvPSDriver::popState()
{
	if(statestackindex_ > 0)
	{	state_ = statestack_[ --statestackindex_];
	}
	else
	{	cerr << "AvPSDriver::popState: stack underflow." << endl;
	}
	return;
}

// Entries must match the order in the MEDIATYPE enum.
// This is a list of know paper sizes as taken from rfc1759. Margins
// are arbitrary (ie guessed at).
static AvPSDriver::PageInfo pagesizes[] = {
 {AvPSDriver::NA_LETTER, 8.5, 11, 1.0, 1.0, AvPSDriver::INCHES,
	"NA_LETTER", "Letter", "North American letter"},
 {AvPSDriver::NA_LEGAL, 8.5, 14, 1.0, 1.0, AvPSDriver::INCHES,
	"NA_LEGAL", "Legal", "North American legal"},
 {AvPSDriver::NA_10X13_ENVELOPE, 10, 13, 1.0, 1.0, AvPSDriver::INCHES,
      "NA_10X13_ENVELOPE", "10x13 Envelope", "North American 10x13 envelope"},
 {AvPSDriver::NA_9X12_ENVELOPE, 9, 12, 1.0, 1.0, AvPSDriver::INCHES,
	"NA_9X12_ENVELOPE", "9x12 Envelope", "North American 9x12 envelope"},
 {AvPSDriver::NA_NUMBER_10_ENVELOPE, 4.125, 9.5, 1.0, 1.0, AvPSDriver::INCHES,
	"NA_NUMBER_10_ENVELOPE", "Number 10 Envelope",
	"North American number 10 business envelope"},
 {AvPSDriver::NA_7X9_ENVELOPE, 7, 9, 1.0, 1.0, AvPSDriver::INCHES,
	"NA_7X9_ENVELOPE", "7x9_Envelope", "North American 7x9 envelope"},
 {AvPSDriver::NA_9X11_ENVELOPE, 9, 11, 1.0, 1.0, AvPSDriver::INCHES,
	"NA_9X11_ENVELOPE", "9x11 Envelope", "North American 9x11 envelope"},
 {AvPSDriver::NA_10X14_ENVELOPE, 10, 14, 1.0, 1.0, AvPSDriver::INCHES,
	"NA_10X14_ENVELOPE", "10x14 Envelope", "North American 10x14 envelope"},
 // The NA_6X9_ENVELOPE appears to be the same as the "Number 9" envelope.
 {AvPSDriver::NA_6X9_ENVELOPE, 6, 9, 1.0, 1.0, AvPSDriver::INCHES,
	"NA_6X9_ENVELOPE", "6x9 Envelope", "North American 6x9 envelope"},
 {AvPSDriver::NA_10X15_ENVELOPE, 10, 15, 1.0, 1.0, AvPSDriver::INCHES,
	"NA_10X15_ENVELOPE", "10x15 Envelope", "North American 10x15 envelope"},
 {AvPSDriver::A,  8.5, 11, 1.0, 1.0, AvPSDriver::INCHES,
	"A", "a", "Engineering A"},
 {AvPSDriver::B,  11, 17, 1.0, 1.0, AvPSDriver::INCHES,
	"B", "b", "Engineering B"},
 {AvPSDriver::C,  17, 22, 1.0, 1.0, AvPSDriver::INCHES,
	"C", "c", "Engineering C"},
 {AvPSDriver::D,  22, 34, 1.0, 1.0, AvPSDriver::INCHES,
	"D", "d", "Engineering D"},
 {AvPSDriver::E,  34, 44, 1.0, 1.0, AvPSDriver::INCHES,
	"E", "e", "Engineering E"},
 {AvPSDriver::ISO_A0,  841, 1189, 10.0, 10.0, AvPSDriver::MM, "ISO_A0",
	"ISO A0", "ISO A0"},
 {AvPSDriver::ISO_A1,  594,  841, 10.0, 10.0, AvPSDriver::MM, "ISO_A1",
	"ISO A1", "ISO A1"},
 {AvPSDriver::ISO_A2,  420,  594, 10.0, 10.0, AvPSDriver::MM, "ISO_A2",
	"ISO A2", "ISO A2"},
 {AvPSDriver::ISO_A3,  297,  420, 10.0, 10.0, AvPSDriver::MM, "ISO_A3",
	"ISO A3", "ISO A3"},
	// 1.25in Left/Right, 1.5in Top/Bottom.
 {AvPSDriver::ISO_A4,  210,  297, 31.75, 38.10, AvPSDriver::MM, "ISO_A4",
	"ISO A4", "ISO A4"},
 {AvPSDriver::ISO_A5,  148,  210, 10.0, 10.0, AvPSDriver::MM, "ISO_A5",
	"ISO A5", "ISO A5"},
 {AvPSDriver::ISO_A6,  105,  148, 10.0, 10.0, AvPSDriver::MM, "ISO_A6",
	"ISO A6", "ISO A6"},
 {AvPSDriver::ISO_A7,   74,  105, 10.0, 10.0, AvPSDriver::MM, "ISO_A7",
	"ISO A7", "ISO A7"},
 {AvPSDriver::ISO_A8,   52,   74, 10.0, 10.0, AvPSDriver::MM, "ISO_A8",
	"ISO A8", "ISO A8"},
 {AvPSDriver::ISO_A9,   37,   52, 10.0, 10.0, AvPSDriver::MM, "ISO_A9",
	"ISO A9", "ISO A9"},
 {AvPSDriver::ISO_A10, 26,   37, 10.0, 10.0, AvPSDriver::MM, "ISO_A10",
	"ISO A10", "ISO A10"},
 {AvPSDriver::ISO_B0, 1000, 1414, 10.0, 10.0, AvPSDriver::MM, "ISO_B0",
	"ISO B0", "ISO B0"},
 {AvPSDriver::ISO_B1,  707, 1000, 10.0, 10.0, AvPSDriver::MM, "ISO_B1",
	"ISO B1", "ISO B1"},
 {AvPSDriver::ISO_B2,  500,  707, 10.0, 10.0, AvPSDriver::MM, "ISO_B2",
	"ISO B2", "ISO B2"},
 {AvPSDriver::ISO_B3,  353,  500, 10.0, 10.0, AvPSDriver::MM, "ISO_B3",
	"ISO B3", "ISO B3"},
 {AvPSDriver::ISO_B4,  250,  353, 10.0, 10.0, AvPSDriver::MM, "ISO_B4",
	"ISO B4", "ISO B4"},
 {AvPSDriver::ISO_B5,  176,  250, 10.0, 10.0, AvPSDriver::MM, "ISO_B5",
	"ISO B5", "ISO B5"},
 {AvPSDriver::ISO_B6,  125,  176, 10.0, 10.0, AvPSDriver::MM, "ISO_B6",
	"ISO B6", "ISO B6"},
 {AvPSDriver::ISO_B7,   88,  125, 10.0, 10.0, AvPSDriver::MM, "ISO_B7",
	"ISO B7", "ISO B7"},
 {AvPSDriver::ISO_B8,   62,   88, 10.0, 10.0, AvPSDriver::MM, "ISO_B8",
	"ISO B8", "ISO B8"},
 {AvPSDriver::ISO_B9,   44,   62, 10.0, 10.0, AvPSDriver::MM, "ISO_B9",
	"ISO B9", "ISO B9"},
 {AvPSDriver::ISO_B10,  31,   44, 10.0, 10.0, AvPSDriver::MM, "ISO_B10",
	"ISO B10", "ISO B10"},
 {AvPSDriver::ISO_C0,   917, 1297, 10.0, 10.0, AvPSDriver::MM, "ISO_C0",
	"ISO C0", "ISO C0"},
 {AvPSDriver::ISO_C1,   648,  917, 10.0, 10.0, AvPSDriver::MM, "ISO_C1",
	"ISO C1", "ISO C1"},
 {AvPSDriver::ISO_C2,   458,  648, 10.0, 10.0, AvPSDriver::MM, "ISO_C2",
	"ISO C2", "ISO C2"},
 {AvPSDriver::ISO_C3,   324,  458, 10.0, 10.0, AvPSDriver::MM, "ISO_C3",
	"ISO C3", "ISO C3"},
 {AvPSDriver::ISO_C4,   229,  324, 10.0, 10.0, AvPSDriver::MM, "ISO_C4",
	"ISO C4", "ISO C4"},
 {AvPSDriver::ISO_C5,   162,  229, 10.0, 10.0, AvPSDriver::MM, "ISO_C5",
	"ISO C5", "ISO C5"},
 {AvPSDriver::ISO_C6,   114,  162, 10.0, 10.0, AvPSDriver::MM, "ISO_C6",
	"ISO C6", "ISO C6"},
 {AvPSDriver::ISO_C7,    81,  114, 10.0, 10.0, AvPSDriver::MM, "ISO_C7",
	"ISO C7", "ISO C7"},
 {AvPSDriver::ISO_C8,    57,   81, 10.0, 10.0, AvPSDriver::MM, "ISO_C8",
	"ISO C8", "ISO C8"},
 {AvPSDriver::ISO_DESIGNATED,  110, 220, 10.0, 10.0, AvPSDriver::MM,
	"ISO_DESIGNATED", "ISO Long", "ISO Designated Long"},
 {AvPSDriver::JIS_B0, 1030, 1456, 10.0, 10.0, AvPSDriver::MM, "JIS_B0",
	"JIS B0", "JIS B0"},
 {AvPSDriver::JIS_B1,  728, 1030, 10.0, 10.0, AvPSDriver::MM, "JIS_B1",
	"JIS B1", "JIS B1"},
 {AvPSDriver::JIS_B2,  515,  728, 10.0, 10.0, AvPSDriver::MM, "JIS_B2",
	"JIS B2", "JIS B2"},
 {AvPSDriver::JIS_B3,  364,  515, 10.0, 10.0, AvPSDriver::MM, "JIS_B3",
	"JIS B3", "JIS B3"},
 {AvPSDriver::JIS_B4,  257,  364, 10.0, 10.0, AvPSDriver::MM, "JIS_B4",
	"JIS B4", "JIS B4"},
 {AvPSDriver::JIS_B5,  182,  257, 10.0, 10.0, AvPSDriver::MM, "JIS_B5",
	"JIS B5", "JIS B5"},
 {AvPSDriver::JIS_B6,  128,  182, 10.0, 10.0, AvPSDriver::MM, "JIS_B6",
	"JIS B6", "JIS B6"},
 {AvPSDriver::JIS_B7,   91,  128, 10.0, 10.0, AvPSDriver::MM, "JIS_B7",
	"JIS B7", "JIS B7"},
 {AvPSDriver::JIS_B8,   64,   91, 10.0, 10.0, AvPSDriver::MM, "JIS_B8",
	"JIS B8", "JIS B8"},
 {AvPSDriver::JIS_B9,   45,   64, 10.0, 10.0, AvPSDriver::MM, "JIS_B9",
	"JIS B9", "JIS B9"},
 {AvPSDriver::JIS_B10,  32,   45, 10.0, 10.0, AvPSDriver::MM, "JIS_B10",
	"JIS B10", "JIS B10" }
};
static const int NUMPAGES = (int)(sizeof(pagesizes)/sizeof(*pagesizes));

int AvPSDriver::numPageTypes()
{
	return NUMPAGES;
}

// Return entry corresponding to the index'th entry.
const AvPSDriver::PageInfo *AvPSDriver::getPageInfo(const int index)
{
	if((index < 0) || (index >= NUMPAGES))
		return NULL;
	return &pagesizes[index];
}

// Return entry corresponding to the specified MediaSize.
// ASSUMES the MediaSize enum matches the table!!
const AvPSDriver::PageInfo *AvPSDriver::pageInfo(const MediaSize media)
{ int indx = (int)media;

	if((indx < 0) || (indx >= NUMPAGES))
		return NULL;
	const PageInfo *ps = &pagesizes[indx];
	if(ps->media != media)
	{	cerr << "AvPSDriver::PageInfo: inconsistent page size table."
		     << endl;
		cerr << "Media is " << (int)media
		     << ", but table lists " << (int)ps->media 
		     << " (" << ps->name << endl;
		return NULL;
	}
	return ps;
}

// Lookup paper info using a name.
// Checks name, alias and description.
const AvPSDriver::PageInfo *AvPSDriver::lookupPageInfo(const char *name)
{

	if(name == NULL)
		return NULL;
	for(int i=0; i< NUMPAGES; i++)
	{ PageInfo *p = &pagesizes[i];
	   if(	(strcasecmp(name, p->name) == 0)
					||
		((p->alias != NULL) && (strcasecmp(name, p->alias) == 0))
				||
		((p->description != NULL) && (strcasecmp(name, p->description) == 0))
	     )
		return p;
	}
	return NULL;
}

////////////////////////////////////////////////////////////////
//		PSInfo
////////////////////////////////////////////////////////////////

AvPSDriver::PSInfo::PSInfo() :
	title_("aipsview plot"), creator_("aipsview"), comment_()


{ char buf[128];
  struct passwd *pw = getpwuid(getuid());

	sprintf(buf, "%s (%s)", pw->pw_gecos, pw->pw_name);
	for_ = buf;
	lm_ = rm_ = tm_ = bm_ = 0.0;
	haveMargins_ = False;
}

AvPSDriver::PSInfo::~PSInfo()
{
}

void AvPSDriver::PSInfo::For(const AvString &s)
{
	for_ = s;
}

void AvPSDriver::PSInfo::For(const char *s)
{
	for_ = s;
}

void AvPSDriver::PSInfo::Title(const AvString &s)
{
	title_ = s;
}

void AvPSDriver::PSInfo::Title(const char *s)
{
	title_ = s;
}

void AvPSDriver::PSInfo::Comment(const AvString &s)
{
	comment_ = s;
}

void AvPSDriver::PSInfo::Comment(const char *s)
{
	comment_ = s;
}

void AvPSDriver::PSInfo::Creator(const AvString &s)
{
	creator_ = s;
}

void AvPSDriver::PSInfo::Creator(const char *s)
{
	creator_ = s;
}

void AvPSDriver::PSInfo::setMargins(const float lm, const float rm,
				  const float tm, const float bm,
				  const Dimension dim)
{
	lm_ = AvPSDriver::toPoints(lm, dim);
	rm_ = AvPSDriver::toPoints(rm, dim);
	tm_ = AvPSDriver::toPoints(tm, dim);
	bm_ = AvPSDriver::toPoints(bm, dim);
	haveMargins_ = True;
}

Bool AvPSDriver::PSInfo::getMargins(float &lm, float &rm,
				  float &tm, float &bm)
{
	if(haveMargins_)
	{	lm = lm_;
		rm = rm_;
		tm = tm_;
		bm = bm_;
	}
	return haveMargins_;
}

////////////////////////////////////////////////////////////////
// Transform for doing PostScript transforms. Used to enable bounding
// box computations.
//
// PostScript uses the transform matrix:
//	| a  b  0 |
// CTM=	| c  d  0 |
//	| tx ty 1 |

// Values in user coordinates are converted to internal coordinates
// (points) via:
//	[ x' y' 1] = [ x y 1] |CTM|
// Where CTM is the Current Transform Matrix.
//

AvPSDriver::PSState::PSState()
{
	setIdentity();
	currX_ = currY_ = 0.0;
}

AvPSDriver::PSState::~PSState()
{
}

//  | sx  0 0 |
//  |  0 sy 0 | * M
//  |  0  0 1 |
void AvPSDriver::PSState::scale( const float x, const float y)
{
	a_ *= x;
	b_ *= x;
	c_ *= y;
	d_ *= y;
	invert();
}

//  |  0  0 0 |
//  |  0  0 0 | * M
//  | tx ty 1 |
void AvPSDriver::PSState::translate( const float x, const float y)
{
	tx_ += a_*x + c_*y;
	ty_ += b_*x + d_*y;
	invert();
}

//  |  C  S 0 |
//  | -S  C 0 | * M
//  |  0  0 1 |
static const double DEG_TO_RAD = M_PI/180.0;

void AvPSDriver::PSState::rotate(const float degrees)
{ double radians = (double)degrees*DEG_TO_RAD;
  double Cos = cos(radians);
  double Sin = sin(radians);
  double a=a_, b=b_, c=c_, d=d_;

	a_ = (float) (a*Cos + c*Sin);
	b_ = (float) (b*Cos + d*Sin);
	c_ = (float)(-a*Sin + c*Cos);
	d_ = (float)(-b*Sin + d*Cos);

	invert();
}


//	 | 1 0 0 |
//   M = | 0 1 0 |
//	 | 0 0 1 |
void AvPSDriver::PSState::setIdentity()
{
	a_ = d_ = ai_ = di_ = 1.0;
	b_ = c_ = bi_ = bi_ = 0.0;
	tx_ = ty_ = txi_ = tyi_ = 0.0;
}

void AvPSDriver::PSState::operator=(const PSState &m)
{
	a_ = m.a_;
	b_ = m.b_;
	c_ = m.c_;
	d_ = m.d_;
	tx_ = m.tx_;
	ty_ = m.ty_;

	ai_ = m.ai_;
	bi_ = m.bi_;
	ci_ = m.ci_;
	di_ = m.di_;
	txi_ = m.txi_;
	tyi_ = m.tyi_;

	currX_ = m.currX_;
	currY_ = m.currY_;
}

//  -1
// M  <- M
//
// Calculate the inverse of the 3x3 matrix.
//
void AvPSDriver::PSState::invert()
{ // The inverse of a 3x3 matrix is a tad messy, but it cleans up
  // when the 3rd row is [0 0 1](T).

	double det = a_*d_ - b_*c_;
	static const double SMALLNUM = 0.00000000001;
	if( det < SMALLNUM)
	{	cerr << "AvPSDriver::PSState::invert: matrix is singular."
		     << endl;
		det = SMALLNUM;
	}
	float det1 = (float)(1.0/det);
	ai_ =  d_*det1;
	bi_ = -b_*det1;
	ci_ = -c_*det1;
	di_ =  a_*det1;

	txi_ = (c_*ty_ - tx_*d_)*det1;
	tyi_ = (b_*tx_ - a_*ty_)*det1;
}

// Set/Get the current position.
void AvPSDriver::PSState::setXY(const float x, const float y,
				const Boolean inPoints)
{
	if(inPoints)
	{	currX_ = x;
		currY_ = y;
	}
	else
		toPoints(x, y, currX_, currY_);
}

void AvPSDriver::PSState::getXY(float &x, float &y, const Boolean inPoints)
{
	if(inPoints)
	{	x = currX_;
		y = currY_;
	}
	else
		fromPoints(currX_, currY_, x, y);
}
