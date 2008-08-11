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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvPGDriver.cc,v 19.0 2003/07/16 05:47:21 aips2adm Exp $
//
// $Log: AvPGDriver.cc,v $
// Revision 19.0  2003/07/16 05:47:21  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:02  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.3  2002/03/11 16:31:19  hravlin
// Added NEED_FORTRAN_UNDERSCORES defines for psdriv_. Needed by HP systems.
//
// Revision 17.2  2002/01/30 22:39:45  hravlin
// Added a variable initialization that may have been missing from an if/else
// block.
//
// Revision 17.1  2002/01/22 22:12:15  hravlin
// Added some (int) casts to sizeof()s for 64 bit compilers.
//
// Revision 17.0  2001/11/12 19:42:31  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:38  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.1  2000/12/13 18:20:06  hravlin
// Renamed MIN() to iMIN().
//
// Revision 15.0  2000/10/26 17:10:47  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:38:40  hravlin
// Changed some char * to const char *.
//
// Revision 14.0  2000/03/23 16:08:25  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.3  1999/09/29 16:50:02  hravlin
// Fixed compiler warnings.
//
// Revision 13.2  1999/09/15 21:25:30  hravlin
// A few changes for the rewritten colormap handler.
//
// Revision 13.1  1999/08/25 19:45:05  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:40:13  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:02  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:18  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:14  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.5  1998/06/11 17:34:33  hr
// Removed a couple signed/unsigned comparision warnings.
//
// Revision 9.4  1997/11/14 22:51:12  hr
// Added flag to getPGColormappings() to limit warning messages.
//
// Revision 9.3  1997/11/07 18:58:44  bglenden
// Quiet Purify
//
// Revision 9.2  1997/09/17 14:49:08  hr
// In exec(), removed conversion of linewidth to inches. (Just copy).
//
// Revision 9.1  1997/09/15 16:25:35  hr
// For line width calculation, changed from assuming 72PPI to 90PPI.
//
// Revision 9.0  1997/08/25 21:28:18  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.1  1997/04/30 20:10:39  hr
// Added enable/disable CheckAllocated() calls around calls to allocNamedColor
// when initializing PGPLOT colors.
//
// Revision 8.0  1997/02/20 03:18:32  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/02/05 17:53:28  hr
// Changed some signed/unsigned comparisions.
//
// Revision 7.2  1996/12/12 06:43:02  droberts
// Final update from monet archive.
//
// Revision 1.2  1996/08/14 19:03:39  hr
// Changes to remove g++ warnings (from bglenden).
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.4  1996/06/18  18:46:22  pixton
//  Copyright Update
//
//  Revision 1.3  1996/03/05  17:35:12  hr
//  Made black the default background color (index 0) to match the XWindow
//  driver.
//  A call to querycolors had the wrong arguments.
//  Changed how get/set color interact with the palette. (More changes to come).
//
//  Revision 1.2  1996/02/29  17:29:53  hr
//  Added PostScript driver calls to grexec.
//
//  Revision 1.1  1996/01/24  18:02:13  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <AvPGDriver.h>
#include <AvPolyline.h>
#include <AvViewportSetter.h>
#include <AvPointList2D.h>
#include <AvCanvas.h>
#include <AvPalette.h>

AvPGDriver *AvPGDriver::current = NULL;
static const char *DEVICENAME_ = "CANVAS (AvCanvas driver)";

// Return a pointer to the current device driver, creating one if necessary.
AvPGDriver *AvPGDriver::currentDriver()
{
	if(current == NULL)
		AvPGDriver::currentDriver(new AvPGDriver());

	return current;
}

// Change the current driver.
void AvPGDriver::currentDriver( AvPGDriver *d)
{
	current = d;
}

AvPGDriver::AvPGDriver()
{
	poly_ = NULL;
	polyappended_ = FALSE;
	canvas_ = NULL;
	plotID = unitID = -1;
	devName_ = NULL;
	append_ = FALSE;
	width_ = height_ = 0;
	vps_ = NULL;
	p1_ = p2_ = NULL;
	polycnt_ = 0;
	popc_ = NONE;
	tracemask_ = FALSE;
	pgcmap_ = NULL;
}

AvPGDriver::~AvPGDriver()
{
	close();
//	endPicture();

	if(canvas_ != NULL)
		canvas_->unref();
//	delete devName_;
	delete [] pgcmap_;
	pgcmap_ = 0; // force crash if dereferenced
}

// External interface to the driver. This just passes the commands to
// the exec routine of the current driver. (PGPLOT can only have 1 active
// device at a time.
void avdriv(int *opc, float *rbuf, int *nbuf,
		     char *chr, int *lchr, int *mode, int len)
{
AvPGDriver *c;

	c = AvPGDriver::currentDriver();
	c->exec(*opc, rbuf, nbuf, chr, lchr, mode, len);
}

void avdriv_(int *opc, float *rbuf, int *nbuf,
		     char *chr, int *lchr, int *mode, int len)
{
AvPGDriver *c;

	c = AvPGDriver::currentDriver();
	c->exec(*opc, rbuf, nbuf, chr, lchr, mode, len);
}

/*
	From the PGPLOT docs.
CHR(1:1) = `H' if the device is a hardcopy device, `I' if it is an
interactive
device. On an interactive device, the image is visible as it is being drawn,
while on a hardcopy device it cannot be viewed until the workstation is
closed. 

	CHR(2:2) = `C' if a cursor is available, `X' if a cursor is
available and opcode 27 is accepted by the handler, `N' if there is no
cursor. PGPLOT cannot emulate a cursor if none is available.

	CHR(3:3) = `D' if the hardware can draw dashed lines, `N' if
it cannot.  PGPLOT emulates dashed lines by drawing line
segments. Software emulation is usually superior to hardware dashed
lines, and not much slower, so CHR(3:3) = `N' is recommended.

	CHR(4:4) = `A' if the hardware can fill arbitrary polygons
with solid color, `N' if it cannot. PGPLOT emulates polygon fill by
drawing horizontal or vertical lines spaced by the pen diameter (see
OPCODE = 3).

	CHR(5:5) = `T' if the hardware can draw lines of variable
width, `N' if it cannot. PGPLOT emulates thick lines by drawing
multiple strokes. Note that thick lines are supposed to have rounded
ends, as if they had been drawn by a circular nib of the specified
diameter.

	CHR(6:6) = `R' if the hardware can fill rectangles with solid
color, `N' if it cannot. If this feature is not available, PGPLOT will
treat the rectangle as an arbitrary polygon. In this context, a
`rectangle' is assumed to have its edges parallel to the
device-coordinate axes.

	CHR(7:7) = `P' if the handler understands the pixel
primitives, 'Q' if it understands the image primitives (opcode 26), or
`N' otherwise (see the description of opcode 26).

	CHR(8:8) = `V' if PGPLOT should issue an extra prompt to the
user before closing the device (in PGEND), `N' otherwise. Use `V' for
devices where the PGPLOT window is deleted from the screen when the
device is closed.

	CHR(9:9) = `Y' if the handler accepts color representation
queries (opcode 29), `N' if it does not.

CHR(10:10) = `M' if the device handler accepts opcode 28 to draw graph
markers; `N' otherwise. 

*/
/*
1) Hardcopy device. May be changed in future.
2) No cursor is available. May be changed in future.
3) Can't draw dashed lines (PGPLOT recommendation).
4) Can fill arbitrary polygons with solid color.
5) Can draw lines of variable width
6) Can fill rectangles with solid color
7) Handler doesn't understand the pixel primitives.
8) PGPLOT should not issue an extra prompt to the user before closing device.
9) Handler accepts color representation queries (opcode 29).
10)No markers.
*/
//static const char *CAPABILITIES_ = "HNNATRPNYN";
static const char *CAPABILITIES_ = "HNNATRNNYN";
static const char *DEFAULTNAME_  = "pgplot";


//#define MINMAX
#if defined(MINMAX)
static float xmin, ymin,xmax,ymax;

float x0,y0, x1,y1;
#endif

// Main driver interface.
void AvPGDriver::exec(int opc, float *rbuf, int *nbuf,
			     char *chr, int *lchr, int * /*mode*/, int len)
{
//	TRACE((OPCODES)opc);
	switch(opc) {
	case DEVICENAME:
		*lchr = returnString(chr, DEVICENAME_, len);
		break;
	case DIMENSIONS:
		{float w, h, nc;
		// Tell PGPLOT that the coordinate range is 0..1. This
		// effectively results in 'fractional' vectors.
#if 0
			if( vps_ != NULL)
			{	w = (float) (vps_->viewport()->width()-1);
				h = (float) (vps_->viewport()->height()-1);
			}
			else
			{	w = 1.0;
				h = 1.0;
			}
#else
			{	w = 1.0;
				h = 1.0;
			}
#endif
			if(canvas_ != NULL)
				nc = (float)
				  (canvas_->palette()->availableColors()-1);
			else
				nc = 1;

			rbuf[0] = 0.0;
			rbuf[1] = w;
			rbuf[2] = 0.0;
			rbuf[3] = h;
			rbuf[4] = 0.0;
			rbuf[5] = nc;
			*nbuf = 6;
		}
		break;
	case SCALE:		// OC = 3
		// This should be retrievable from the canvas.
		rbuf[0] = 72;
		rbuf[1] = 72;
		rbuf[2] = 1;
		*nbuf = 3;
		break;
	case CAPABILITIES:
		*lchr = returnString(chr, CAPABILITIES_, len);
		break;
	case DEVICE_NAME:
		*lchr = returnString(chr, DEFAULTNAME_, len);
		break;
	case DEFAULT_SIZE:
		{float w, h;
#if 0
			if( vps_ != NULL)
			{	w = (float) (vps_->viewport()->width()-1);
				h = (float) (vps_->viewport()->height()-1);
			}
			else
			{	w = 255.;
				h = 255.;
			}
#else
			w = h = 1.0;
#endif
			rbuf[0] = 0.0;
			rbuf[1] = w;
			rbuf[2] = 0.0;
			rbuf[3] = h;
			*nbuf = 4;
		}
		break;
	case CHARSCALE:
		rbuf[0] = 1.0;
		*nbuf = 1;
		break;
	case SELECT:		// OP = 8
		plotID = (int) rbuf[0];
		unitID = (int) rbuf[1];
		break;
	case OPEN:
		{	Boolean status = open( (int) rbuf[2], chr, *lchr);
			rbuf[0] = 0.0;	// Unit id = 0 for now;
			if(status)
				rbuf[1] = 1.0;	// Opened OK.
			else
				rbuf[1] = 0.0;	// Open failed.
			break;
		}
	case CLOSE:
		close();
		break;
	case BEGIN:
#if defined(MINMAX)
xmin=100; ymin=100; xmax=-100; ymax = -100;
#endif
		beginPicture(rbuf[0] + 1.0, rbuf[1] + 1.0);
		break;
	case LINE:
		checkPcache(LINE);
		p1_->add( rbuf[0], rbuf[1]);
		p2_->add( rbuf[2], rbuf[3]);
#if defined(MINMAX)
x0 = rbuf[0]; y0 = rbuf[1]; x1 = rbuf[2]; y1 = rbuf[3];
	if(x0 < xmin) xmin = x0;
//	else
	if(x0 > xmax) xmax = x0;
	if(x1 < xmin) xmin = x1;
//	else
	if(x1 > xmax) xmax = x1;
	if(y0 < ymin) ymin = y0;
//	else
	if(y0 > ymax) ymax = y0;
	if(y1 < ymin) ymin = y1;
//	else
	if(y1 > ymax) ymax = y1;

//printf("%.2f/%.2f -> %.2f/%.2f\n", rbuf[0], rbuf[1], rbuf[2], rbuf[3]);
#endif
		break;
	case POINT:
		checkPcache(POINT);
		p1_->add( rbuf[0], rbuf[1]);
		break;
	case ENDPICTURE:
#if defined(MINMAX)
printf("MinMax: %.2f/%.2f -> %.2f/%.2f\n", xmin,ymin, xmax,ymax);
#endif
		endPicture();
		break;
	case COLORINDEX:
		if(rbuf[0] < 0)
			return;
		setColorIndex((unsigned long) rbuf[0]);
		break;
	case FLUSH:
		flush();
		break;
	case CURSOR:		// None yet.
		break;
	case ERASEALPHA:	// Ignore
		break;
	case LINESTYLE:		// OP = 19, Ignore.
		break;
	case FILLPOLYGON:
		checkPcache(FILLPOLYGON);
		// If polycnt_ is 0, we're starting a polygon, otherwise,
		// we're appending a point.
		if( polycnt_ <= 0)
		{	polycnt_ = (int) rbuf[0];
			// Make sure there is enough space.
			if(polycnt_ < p1_->maxSize())
				p1_->grow(polycnt_ - p1_->maxSize());
		}
		else
		{	p1_->add(rbuf[0], rbuf[1]);
			// Force a flush if done.
			if(p1_->size() == polycnt_)
			{	checkPcache(NONE);
				polycnt_ = 0;		// Signal done.
			}
		}
		break;
	case COLORVALUE:
		if(canvas_ != NULL)
		{	// PGPLOT expects to be able to access the entire
			// palette. If the color index is one of the predefined
			// colors, translate it into a physical index and
			// read from full colormap. Otherwise, use it as
			// an index into the current table.
			// This has the side effect of making the first 15
			// changable colors unaccessible
//			int ci = int(rbuf[0] + 0.5), colorIndex;
			int ci = int(rbuf[0] + 0.5);
			AvPalette *pal = canvas_->palette();
#if 0
			if(ci < pgcolorentries())
			{	colorIndex = (int)pgcmap_[ci].pindex;
				int cs = pal->currentSegment();
				pal->useSegment(2);
					pal->storeColors(&rbuf[1], &rbuf[2],
						&rbuf[3], colorIndex, 1);
				pal->useSegment(cs);
			}
			else
				pal->storeColors(&rbuf[1], &rbuf[2],
						 &rbuf[3], ci, 1);
#else
			if(ci < pal->availableColors())
				pal->storeColors(&rbuf[1], &rbuf[2],
						 &rbuf[3], ci, 1);
#endif
		}
		break;
	case LINEWIDTH:
		int lw;
		checkPcache(LINEWIDTH);
		// Approximation. (# inches)*(pixels/inch).
//		lw = (int) (rbuf[0]*.005*90);
		lw = (int) rbuf[0];
		poly_->lineWidth(lw);
		break;
	case ESCAPE:		// Ignore
		break;
	case FILLRECTANGLE:	// OP = 24.
		checkPcache(FILLRECTANGLE);	// In case of overlap.
		float w, h;
		w = rbuf[2] - rbuf[0] + 1;
		h = rbuf[3] - rbuf[1] + 1;
		poly_->rectangle(rbuf[0], rbuf[1], w, h, TRUE);
		break;
	case FILLPATTERN:		// OP=25, set fill pattern
		break;
	case PIXELS:		// OP=26, Ignored.
		break;
	case SCALEINFO:		// OP=27, CURSOR scale info. Ignored.
		break;
	case MARKER:		// Ignored.
		break;
	case QCOLOR:		// OP=29 Query color.
		if( canvas_ == NULL)
		{	rbuf[1] = rbuf[2] = rbuf[3] = 0.0;
		}
		else
		{
#if 0
			// PGPLOT expects to be able to access the entire
			// palette. If the color index is one of the predefined
			// colors, translate it into a physical index and
			// read from full colormap. Otherwise, use it as
			// an index into the current table.
			// This has the side effect of making the first 15
			// changable colors unaccessible.
			int ci = int(rbuf[0] + 0.5), colorIndex;
			AvPalette *pal = canvas_->palette();
			if(ci < pgcolorentries())
			{	colorIndex = (int)pgcmap_[ci].pindex;
				int cs = pal->currentSegment();
				pal->useSegment(2);
					pal->queryColors(&rbuf[1], &rbuf[2],
						&rbuf[3], colorIndex, 1);
				pal->useSegment(cs);
			}
			else
				pal->queryColors(&rbuf[1], &rbuf[2],
						 &rbuf[3], ci, 1);
#else
			AvPalette *pal = canvas_->palette();
//			int ci = int(rbuf[0] + 0.5), colorIndex;
			int ci = int(rbuf[0] + 0.5);
			pal->queryColors(&rbuf[1], &rbuf[2], &rbuf[3], ci, 1);
#endif
		}
		*nbuf = 4;
		break;
	default:
		fprintf(stderr, "AvPGDriver::exec: bad op code (%d)\n", opc);
		break;
	}
	return;
}


	// Returns device name. (OP=1).
const char *AvPGDriver::deviceName()
{
	return DEVICENAME_;
}

inline static int iMIN(const int a, const int b)
{	return (a<=b) ? a : b;
}

// Copy a character string, padding with blanks if necessary.
int	AvPGDriver::returnString(char *out, const char *in, int outlen)
{
	int inlen = (int)strlen(in);
	int n = iMIN(inlen, outlen);

	strncpy(out, in, n);

	for(int i=n; i< outlen-n; i++)
		out[i] = ' ';
	return inlen;
}

// Copy a character string, trim blanks, NULL terminate.
char	*AvPGDriver::dupString(const char *str, int inlen)
{
int	indx, len;

	// Find the last non blank character.
	for(indx = inlen-1; indx > 0; indx--)
		if(str[indx] != ' ')
			break;
	len = indx + 1;
	char *s = new char[len+1];
	strncpy(s, str, len);
	s[indx] = '\0';

	return s;
}

// Change viewport setters.
void AvPGDriver::setVPS( AvViewportSetter *vps)
{
Boolean needPoly = polyappended_;

	flush();		// Dump buffers to current.
	if(vps_ == vps)
		return;
	if(vps_ != NULL)	// Remove current.
		vps_->unref();
	vps_ = vps;
	if(vps_ != NULL)
	{	vps_->ref();
		width_ = vps_->viewport()->width();
		height_ = vps_->viewport()->height();
	}
	else
	{	width_ = 0;
		height_ = 0;
	}
	// If the old polyline object was used, need a new one.
	if(needPoly)
		newPolyline();
}

void AvPGDriver::setSize(const float width, const float height)
{
	width_ = width;
	height_ = height;
}

void AvPGDriver::setCanvas(AvCanvas *c)
{
	if(c == canvas_)
		return;
	if(canvas_ != NULL)
		canvas_->unref();
	canvas_ = c;
	if(canvas_ != NULL)
		canvas_->ref();
	setPGColormappings(canvas_->palette());
}

void AvPGDriver::newPolyline(AvPolyline *poly)
{
	if(poly_ != NULL)
		poly_->unref();
	if(poly == NULL)
		poly_ = new AvPolyline();
	else
		poly_ = poly;
	poly_->ref();
	poly_->addressMode(AvPolyline::ABSOLUTE);
//	poly_->coordinateMode(AvPolyline::FULLSIZE);
	polyappended_ = FALSE;
}

	// Open workstation (OP=9).
Boolean AvPGDriver::open(const int Append, char *chr, const int lchr)
{
	append_ = (Append == 0) ? FALSE : TRUE;
	devName_ = dupString(chr, lchr);
	if(vps_ == NULL)
	   setVPS(new AvViewportSetter());
	if(poly_ == NULL)
		newPolyline();
	return TRUE;
}

	// Close workstation (OP=10).
void AvPGDriver::close()
{
	endPicture();
	if(vps_ != NULL)
	{	vps_->unref();
		vps_ = NULL;
	}
	if(poly_ != NULL)
	{	poly_->unref();
		poly_ = NULL;
	}
	delete [] devName_;
	devName_ = NULL;
}

	// Begin picture (OP=11).
void AvPGDriver::beginPicture(const float width, const float height)
{	width_ = width;
	height_= height;
	vps_->viewport()->width((int)width);
	vps_->viewport()->height((int)height);
	p1_ = new AvPointList2D();
	p2_ = new AvPointList2D();
	popc_ = NONE;
	polycnt_ = 0;
}

	// End picture (OP=14).
// This can get called more than once before a begin picture.
void AvPGDriver::endPicture()
{
	flush();		// Flush internal buffers.
	delete p1_;
	delete p2_;
	p1_ = p2_ = NULL;
}

// Flush internal buffers.
void AvPGDriver::flush()
{
	flushPoints();		// Flush point buffer.
	// The the polyline object isn't empty, make sure it is on the
	// display list.
	if(poly_ != NULL)
	{	if( !polyappended_ && (poly_->length() > 0))
		{	vps_->append(poly_);
			polyappended_ = TRUE;
		}
	}
}

// If the point caches aren't empty, generate the polyline call.
void AvPGDriver::flushPoints( )
{
int	size;

	if( (p1_ == NULL) || (popc_ == NONE))	// If p1_ isn't, p2_ won't be.
		return;
	size = p1_->size();
	if(size <= 0)
		return;


	switch(popc_) {
	case LINE:
		poly_->segment(	p1_->xArray(), p1_->yArray(),
				p2_->xArray(), p2_->yArray(), size);
		break;
	case POINT:
		poly_->point(	p1_->xArray(), p1_->yArray(), size);
		break;
	case FILLPOLYGON:
		poly_->polygon( p1_->xArray(), p1_->yArray(), size,
				TRUE, AvPolyline::COMPLEX);
		break;
	default:;
	}
	p1_->clear();
	p2_->clear();
	popc_ = NONE;
}

static struct {
	AvPGDriver::OPCODES oc;
	int	mask;
	const char *name;
	} opcodes[] = {
	{ AvPGDriver::NONE, 1<< AvPGDriver::NONE, "NONE" },
	{ AvPGDriver::DEVICENAME, 1<< AvPGDriver::DEVICENAME, "DEVICENAME" },
	{ AvPGDriver::DIMENSIONS, 1<< AvPGDriver::DIMENSIONS, "DIMENSIONS" },
	{ AvPGDriver::SCALE, 1<< AvPGDriver::SCALE, "SCALE" },
	{ AvPGDriver::CAPABILITIES, 1<< AvPGDriver::CAPABILITIES,
							"CAPABILITIES" },
	{ AvPGDriver::DEVICE_NAME, 1<< AvPGDriver::DEVICE_NAME, "DEVICE_NAME"},
	{ AvPGDriver::DEFAULT_SIZE, 1<< AvPGDriver::DEFAULT_SIZE,
							"DEFAULT_SIZE" },
	{ AvPGDriver::CHARSCALE, 1<< AvPGDriver::CHARSCALE, "CHARSCALE" },
	{ AvPGDriver::SELECT, 1<< AvPGDriver::SELECT, "SELECT" },
	{ AvPGDriver::OPEN, 1<< AvPGDriver::OPEN, "OPEN" },
	{ AvPGDriver::CLOSE, 1<< AvPGDriver::CLOSE, "CLOSE" },
	{ AvPGDriver::BEGIN, 1<< AvPGDriver::BEGIN, "BEGIN" },
	{ AvPGDriver::LINE, 1<< AvPGDriver::LINE, "LINE" },
	{ AvPGDriver::POINT, 1<< AvPGDriver::POINT, "POINT" },
	{ AvPGDriver::ENDPICTURE, 1<< AvPGDriver::ENDPICTURE, "ENDPICTURE" },
	{ AvPGDriver::COLORINDEX, 1<< AvPGDriver::COLORINDEX, "COLORINDEX" },
	{ AvPGDriver::FLUSH, 1<< AvPGDriver::FLUSH, "FLUSH" },
	{ AvPGDriver::CURSOR, 1<< AvPGDriver::CURSOR, "CURSOR" },
	{ AvPGDriver::ERASEALPHA, 1<< AvPGDriver::ERASEALPHA, "ERASEALPHA" },
	{ AvPGDriver::LINESTYLE, 1<< AvPGDriver::LINESTYLE, "LINESTYLE" },
	{ AvPGDriver::FILLPOLYGON, 1<< AvPGDriver::FILLPOLYGON, "FILLPOLYGON"},
	{ AvPGDriver::COLORVALUE, 1<< AvPGDriver::COLORVALUE, "COLORVALUE" },
	{ AvPGDriver::LINEWIDTH, 1<< AvPGDriver::LINEWIDTH, "LINEWIDTH" },
	{ AvPGDriver::ESCAPE, 1<< AvPGDriver::ESCAPE, "ESCAPE" },
	{ AvPGDriver::FILLRECTANGLE, 1<< AvPGDriver::FILLRECTANGLE,
							"FILLRECTANGLE" },
	{ AvPGDriver::FILLPATTERN, 1<< AvPGDriver::FILLPATTERN, "FILLPATTERN"},
	{ AvPGDriver::PIXELS, 1<< AvPGDriver::PIXELS, "PIXELS" },
	{ AvPGDriver::SCALEINFO, 1<< AvPGDriver::SCALEINFO, "SCALEINFO" },
	{ AvPGDriver::MARKER, 1<< AvPGDriver::MARKER, "MARKER" },
	{ AvPGDriver::QCOLOR, 1<< AvPGDriver::QCOLOR, "QCOLOR" }
   };
static const int NUMOPCODES = (int)(sizeof(opcodes)/sizeof(*opcodes));

void AvPGDriver::trace_(const OPCODES oc)const
{
static int checked=0;

	if(!checked)
	{	for(int i=0; i< NUMOPCODES; i++)
			if(i != opcodes[i].oc)
			  fprintf(stderr, "OP %d doesn't match entry for %s\n",
				oc, opcodes[i].name);
		checked = 1;
	}
	if((oc < 0) || (oc >= NUMOPCODES))
		fprintf(stderr, "Unknown opcode (%d)\n", oc);
	else
		if(tracemask_ && opcodes[oc].mask)
			printf("%s\n", opcodes[oc].name);
}

static struct PGCOLORSNAMES {
		const char *name;
		float	red;
		float	blue;
		float	green;
	} pgcolornames_[] = {
	{ "black",		0.0, 0.0, 0.0},
	{ "white",		1.0, 1.0, 1.0},
	{ "red",		1.0, 0.0, 0.0},
	{ "green",		0.0, 1.0, 0.0},
	{ "blue",		0.0, 0.0, 1.0},
	{ "cyan",		0.0, 1.0, 1.0},
	{ "magenta",		1.0, 0.0, 1.0},
	{ "yellow",		1.0, 1.0, 0.0},
	// "orange"
	{ "DarkOrange1",	1.0, 0.5, 0.0},
	// green+yellow
	{ "chartreuse",		0.5, 1.0, 0.0},
	// green+cyan
	{ "SpringGreen",	0.0, 1.0, 0.5},
//	{ "blue+cyan",		0.0, 0.5, 1.0},
	{ "DeepSkyBlue",	0.0, 0.5, 1.0},	// only fair approximation.
//	{ "blue+magenta",	0.5, 0.0, 1.0},
	{ "DarkViolet",		0.5, 0.0, 1.0},	// Weak approximation.
//	{ "red+magenta",	1.0, 0.0, .50},
	{ "DeepPink1",		1.0, 0.0, .50},	// Weak approximation.
//	{ "dark gray",		.33, .33, .33},
	{ "gray34",		.33, .33, .33},
	// "light gray"
	{ "gray66",		.66, .66, .66}
  };
static const int NUMPGCOLORS =
			(int)(sizeof(pgcolornames_)/sizeof(*pgcolornames_));

int AvPGDriver::pgcolorentries()
{
	return NUMPGCOLORS;
}

static int pgmapfailures = 0;	// To limit warnings since if it fails once,
				// it will probably fail again.
// Allocate colors more or less matching PGPLOT's default color definitions.
PGColormap *AvPGDriver::getPGColormappings(AvPalette *p, int &maplen)
{
PGColormap *tbl;
int errs = 0;

	maplen = pgcolorentries();
	tbl = new PGColormap[maplen];
	p->disableCheckAllocated();
	for(int i =0; i< NUMPGCOLORS; i++)
	{unsigned long vcolor, pcolor;
		if( p->allocNamedColor(pgcolornames_[i].name, vcolor, pcolor))
		{	tbl[i].name = pgcolornames_[i].name;
			tbl[i].vindex = vcolor;
			tbl[i].pindex = pcolor;
		}
		else
		if(pgmapfailures <= 0 )
		{	fprintf(stderr,
			  "AvPGDriver::getPGColormappings %s not allocated.\n",
			  pgcolornames_[i].name);
			fflush(stderr);
			errs++;
		}
	}
	pgmapfailures += errs;
	p->enableCheckAllocated();
	return tbl;
}

void AvPGDriver::initializePGColormappings(AvPalette *p)
{
PGColormap *tbl;
int	maplen;

	tbl = getPGColormappings(p, maplen);
	delete [] tbl;
}

void AvPGDriver::setPGColormappings(AvPalette *p)
{
int	maplen;

	if(pgcmap_ != NULL)
		delete [] pgcmap_;
	pgcmap_ = getPGColormappings(p, maplen);
}

// Choose color for drawing vectors. If ci is in (0..15), it is
// treated as an index into the predefined color table. If > 15,
// it is used as a virtual color index.
void AvPGDriver::setColorIndex(const unsigned long ci)
{
unsigned long color;

	// Clear any pending draw requests.
	checkPcache(COLORINDEX);

	if(canvas_ == NULL)		// Can't do anything.
		return;

	if(ci < (unsigned long)pgcolorentries())
		color = pgcmap_[ci].vindex;
	else	// If out of bounds, choose default.
	if(ci >= (unsigned long)canvas_->palette()->availableColors())
		color = pgcmap_[1].vindex;
	else
		color = ci;	//(1/29/02) Added this to keep compiler
				// happy. I'm not sure it's right.
	poly_->foreground(color);
}	

/* Do FORTRAN functions have a trailing underscore? (Most systems do). */
#if !defined(NEED_FORTRAN_UNDERSCORES)
#if defined(__hpux__)
#define NEED_FORTRAN_UNDERSCORES 0
#else
#define NEED_FORTRAN_UNDERSCORES 1
#endif
#endif

#if NEED_FORTRAN_UNDERSCORES
#define PSDRIV psdriv_
#else
#define PSDRIV psdriv
#endif

// The dispatcher for PGPLOT.
static const int NUMDRIVERS = 5;
extern "C" {
extern int PSDRIV(int *ifunc, float *rbuf, int *nbuf, char *chr,
					int *lchr, int *mode, int chr_len);
}

int grexec_(int *idev, int *ifunc, float *rbuf, int *nbuf,
	    char *chr, int *lchr, int chr_len)
{
int one = 1;
int num;
	switch (*idev) {
	case 0:
		rbuf[0] = NUMDRIVERS;
		*nbuf = 1;
		break;
	case 1:
		avdriv_(ifunc, rbuf, nbuf, chr, lchr, &one, chr_len);
		break;
	case 2:
		num = 1;
		PSDRIV(ifunc, rbuf, nbuf, chr, lchr, &num, chr_len);
		break;
	case 3:
		num = 2;
		PSDRIV(ifunc, rbuf, nbuf, chr, lchr, &num, chr_len);
		break;
	case 4:
		num = 3;
		PSDRIV(ifunc, rbuf, nbuf, chr, lchr, &num, chr_len);
		break;
	case 5:
		num = 4;
		PSDRIV(ifunc, rbuf, nbuf, chr, lchr, &num, chr_len);
		break;
	default:
		fprintf(stderr, "AvPGDriver::grexec: Unknown device code %d\n",
			*idev);
		break;
	}
	return 0;
}


#if 0

static struct {
	char	*yes;
	char	*no;
	} caps[] = {
	{ "Hardcopy",	"Interactive"},
	{ "Cursor",	"No Cursor"},
	{ "HW Dashed Lines", "No HW Dashed Lines"},
	{ "Arbitrary Polygon Fill", "No Arbitrary Polygon Fill"},
	{ "Variable Width Lines", "No Variable Width Lines"},
	{ "Rectangle Fill",		"No Rectangle Fill"},
	{ "Pixel Primitives",		"Image Primitives"},
	{ "Prompt On EndP",		"Don't Prompt On EndP"},
	{ "Can Query Color",		"Can't Query Color"},
	{ "Markers",			"No Markers"}
	};

static void pdump( int i, Boolean j, char *fmt=NULL)
{
	if(fmt == NULL)
		fmt = "\t%s\n";

	if(j)
		printf(fmt, caps[i].yes);
	else
		printf(fmt, caps[i].no);
}

void AvPGDriver::dump()
{
char	*name;
float	xmin, xmax, ymin, ymax;
int	cimin, cimax, mode;
float	xdpi, ydpi, penWidth;
float	xll, yll, xur, yur;
char	*opnd;

	mode = (MODE > 0) ? MODE: mode_;

	name = deviceName(mode);
	opnd = opened() ? "Opened" : "Not Open";
	printf("%s\t Mode: %d\t %s\n", name, mode, opnd);
	dimensions(xmin, xmax, ymin, ymax, cimin, cimax, mode);
printf("Xmin %.2f, Xmax %.2f, Ymin %.2f, Ymax %.2f CIMin %d, CIMax %d\n",
		xmin, xmax, ymin, ymax, cimin, cimax, mode);
	scale(xdpi, ydpi, penWidth);
	printf("Xdpi %.2f, Ydpi %.2f, Pen Width %.2f\n", xdpi, ydpi, penWidth);
	printf("Capabilities (%s):\n", capabilities_);
	pdump(0, isHardcopy(mode));
	pdump(1, hasCursor(mode));
	pdump(2, doesDashedLines(mode));
	pdump(3, fillsPolygons(mode));
	pdump(4, hasVariableWidthLines(mode));
	pdump(5, fillsRectangles(mode));
	if(doesPixels(mode))
		pdump(6, TRUE);
	else
	if(doesImage(mode))
		pdump(6, FALSE);
	else
		printf("\tNo Pixel or Image Primitives.\n");
	pdump(7, promptOnEnd(mode));
	pdump(8, canQueryColor(mode));
	pdump(9, hasMarkers(mode));

	printf("Default filename: %s\n", defaultName(mode));
	surfaceSize( xll, xur, yll, yur);
	printf("Surface: xll = %.2f, yll = %.2f, xur = %.2f, yur = %.2f\n",
		xll, yll, xur, yur);
	printf("Character scale %d\n", charScale(mode));

	printf("\n");
}
#endif
