//# Copyright (C) 1995-99 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvPGCanvas.cc,v 19.0 2003/07/16 05:47:11 aips2adm Exp $
//
// $Log: AvPGCanvas.cc,v $
// Revision 19.0  2003/07/16 05:47:11  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:54  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:22  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:30  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:11  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:15  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.2  1999/09/15 21:27:16  hravlin
// Redid colormap handling for the new TrueColor visual support.
//
// Revision 13.1  1999/08/25 19:45:34  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:40:02  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:42  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:50  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:42  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.2  1998/06/11 17:33:50  hr
// Removed several signed/unsigned comparision warnings.
//
// Revision 9.1  1997/09/15 16:25:04  hr
// Conversion of line width from an enum to an int.
//
// Revision 9.0  1997/08/25 21:27:25  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.1  1997/04/30 20:08:51  hr
// Renamed checkAllocated to protected checkAllocated_().
//
// Revision 8.0  1997/02/20 03:18:24  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.6  1997/02/10 16:03:06  hr
// Added an "#include <string.h>".
//
// Revision 7.5  1997/02/09 01:06:35  hr
// Fixed signed/unsigned comparisions.
// A case statement wasn't dealing with all of an enum.
//
// Revision 7.4  1997/01/24 19:59:06  hr
// cpgend() now only calls cpgiden() if the value of showID_ is TRUE;
//
// Revision 7.3  1997/01/09 18:27:17  hr
// Changed getPGdevicename() to check for the "-pgDevice" switch.
//
// Revision 7.2  1996/12/12 09:42:10  droberts
// Final update from monet archive.
//
// Revision 1.4  1996/11/12 20:16:37  hr
// Changed drawRaster() to use getRegion() instead of getRow().
//
// Revision 1.3  1996/11/04 20:11:39  hr
// getPGfilename() & getPGdevicename() changed to return AvStrings.
// getPGfilename() & makePGname() replace any spaces in file names with '_'.
// Added cpgbeg() to handle const char * filenames.
//
// Revision 1.2  1996/08/14 19:35:51  hr
// Changes to remove g++ warnings (from bglenden).
//
// Revision 1.1  1996/07/11  21:31:21  pixton
// Automated Checkin
//
//  Revision 1.13  1996/06/18  18:46:21  pixton
//  Copyright Update
//
//  Revision 1.12  1996/04/16  20:27:24  hr
//  Comment change.
//
//  Revision 1.11  1996/03/20  16:54:25  hr
//  Added getPGfilename() and getPGdevicename().
//
//  Revision 1.10  1996/03/07  21:33:55  hr
//  Removed flipBW().
//
//  Revision 1.9  1996/03/07  21:29:51  hr
//  Needed to turn off queryColorIndex again.
//
//  Revision 1.8  1996/03/07  21:23:22  hr
//  Changed the way checkAllocated initializes its colormap. As many of the
//  reference's colors are copied as will fit, the reference's named color
//  table is not literally copied. Over and under range colors bracket
//  the changeable colors. Blanked data values are set to the overrange color.
//  Over and under range colors are set to the reference's min/max colors.
//
//  Revision 1.7  1996/03/02  21:54:21  hr
//  More work on image position and size. The image size is dependent on the
//  size of the image on the screen and the image is roughly centered.
//
//  Revision 1.6  1996/03/01  17:20:30  hr
//  The size of the image's viewport affects the size of the output image.
//
//  Revision 1.5  1996/02/29  22:23:22  hr
//  Added better control of aspect ratio to ::open.
//
//  Revision 1.4  1996/02/29  21:50:29  hr
//  Added "Done" printout when printing is done.
//
//  Revision 1.3  1996/02/29  21:37:46  hr
//  Added flipBW so pgiden would write with black text. Added call to cpgpap
//  in a first crack at controlling image size and aspect ratio.
//
//  Revision 1.2  1996/02/29  17:39:26  hr
//  Removed unused test routine.
//
//  Revision 1.1  1996/02/29  17:30:36  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------
/* AvPGCanvas.cc
Canvas -> PGPLOT interface.
(AvPGDriver is the PGPLOT->Canvas interface).

****************************************************************
****************************************************************
****************************************************************
======> This used to be what was used to create PostScript files. It is
now only used to print profiles and may be removed in the future.
****************************************************************
****************************************************************
****************************************************************

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

#include <AvPGCanvas.h>
#include <AvRaster.h>
#include <AvPolylineP.h>
#include <AvViewportSetter.h>
#include <AvPalette.h>
#include <AvString.h>
#include <AvFallbacks.h>		// for getXOptions().
#include <string.h>

extern "C" {
#include <cpgplot.h>
}

class AvPGPalette : public AvPalette {
  public:
	AvPGPalette(AvPGCanvas &pgc, AvPalette *ref=NULL);
	AvPalette *referencePalette()const{return ref_;}
  protected:
	virtual ~AvPGPalette();
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

	Boolean canvasOpened()const {return pgc_->opened();}
	virtual void checkAllocated_();
  private:
	void initialize();
  private:
	int cilo_, cihi_;	// Valid color index range for images
	int cimin_, cimax_;	// Min/max valid color range.
	AvPGCanvas *pgc_;
	AvPalette *ref_;
};

// The reference palette is used to get the initial color table. Also,
// virtual to physical translations are done using it.
AvPGPalette::AvPGPalette(AvPGCanvas &pgc, AvPalette *ref) : AvPalette(0)
{
	pgc_ = &pgc;
	ref_ = ref;
	if(ref_ != NULL)
	{	ref_->ref();
		expandTable(tableLength(ref_));
	}
	cilo_ = cihi_ = 0;	// Valid color index range
	cimin_ = cimax_ = 0;
}

AvPGPalette::~AvPGPalette()
{
	if(ref_ != NULL)
		ref_->unref();
}

void AvPGPalette::initialize()
{
	if(!canvasOpened())
		return;
	// Initialize
	cpgqcir(&cilo_, &cihi_);
	cpgqcol(&cimin_, &cimax_);
	if(cimax_ == 1)			// 1 means no color capability.
		readonly_ = TRUE;
	maxcolors_ = cimax_ - cimin_ + 1;
#if 0
printf("Color index range: %d-%d, Image CI Range %d-%d Maxcolors %d\n",
		cimin_, cimax_, cilo_, cihi_, maxcolors_);
#endif
}

/*
Write colormap.

red, green, blue	Color values
start			starting index
len			length of red, green and blue arrays.
			If len is 0 or > <color table length> - start,
			<color table length> - start entries are stored.
*/

void AvPGPalette::putColors()
{
int		num;

	if(readonly()) return;

	// Maximum # of colors to deal with.
	num = availableColors();

	for(int i=0; i< num; i++)
	{ float r, g, b;

		indexToRGB(i, r, g, b);
		cpgscr(i, r, g, b);
	}
}

// Read colormap from device.
// If force is TRUE, do the read. Otherwise, assume the table is current.
void AvPGPalette::getColors(const Boolean force)
{
int	num, i;

	if(!canvasOpened())
		return;

	// Get current copy of the colormap.
	num = maxColors();

	if(force)
		for(i=0; i< num; i++)
		{AvColortableEntry *e = findEntryByIndex(i);
		 float r, g, b;
			cpgqcr(i, &r, &g, &b);
			e->store(r, g, b);
		}
}

/* Setup the first three segment tables.
Check the system color table for allocated entries and setup the first
three segments:
	0	only unallocated entries.
	1	all entries but B/W.
	2	all entries.

May be called more than once. The first segment will be rebuilt.
NOTE:
	The system colormap may be a different length than the one we are
using. This assumes that an index in the system colormap corresponds to the
same index in the allocated colormap.

It the user requests a number of colors that doesn't match the above, a
segment is created with that many colors. The startpoint is moved up to
minimize flashing on most servers.

*/
//static const int BLACK_INDEX = 0;
//static const int WHITE_INDEX = 1;

void AvPGPalette::checkAllocated_()
{ int maxOurColors, maxRefColors, maxcolors, availRefColors;

	if(! canvasOpened())
		return;
	if(ref_ == NULL)
		return;
	initialize();

	maxOurColors = maxColors();
	maxRefColors = ref_->maxColors();
	// Minimum of the two.
	maxcolors = (maxOurColors <= maxRefColors) ?
						maxOurColors : maxRefColors;
	availRefColors = ref_->availableColors();

	// Colors available for images start after preallocated colors.
#if 0
	cilo_ = maxRefColors - availRefColors;
	cpgscir(cilo_, cihi_);
#endif

	{ //unsigned long rv, rp, v;
	  int rlen = 0;
	 /* Copy the colors from the reference colormap to ours.
	  Do the I/O here.(?)
	 */
		for(int i=0; i< maxcolors; i++)
		{ float r, g, b;

			const AvColortableEntry *refcolors =
							getColortable(ref_);
			const unsigned short *refmap = getMap(ref_);
			colors_[i] = refcolors[i];
			map_[i] = refmap[i];
			/// Assume PGPLOT is dealing with a PseudoColor map!
			colors_[i].setPixel(i);
			// Make sure our colormap colors match reference's.
			colors_[i].get(r, g, b);
			cpgscr(i, r, g, b);
			if(colors_[i].state() == AvColortableEntry::RW)
				rlen += 1;
		}

		int	availOurcolors = maxOurColors - rlen;
		int	avc = availOurcolors-2 < availRefColors
					? availOurcolors-2 : availRefColors;

		rwcells_ = rlen;
		// Color for Underflow;
		cpgscr(rlen, 0.0, 0.0, 0.0);
		// Color for Overflow;
		cpgscr(rlen+avc+2-1, 1.0, 1.0, 1.0);

		cihi_ = avc+2+rlen-1;
		// This is set to ignore the underflow color but use the
		// overflow color.
		cpgscir(cilo_+1, cihi_);
	}
}

// Dummy, but needed since AvPalette is virtual.
	// Allocate named color.
#if 0
Boolean AvPGPalette::allocNamedColor_(const char *colorname,
			unsigned long &pixel, float &r, float &g, float &b)
#else
Boolean AvPGPalette::allocNamedColor_(const char *colorname,
			unsigned long &, float &, float &, float &)
#endif
{
	printf("AvPGPalette::allocNamedColor %s\n", colorname);
	return FALSE;
}


////////////////////////////////////////////////////////////////


// Environment variables to use to get PGPLOT device and filename.
static const char *PG_FILE = "AV_PG_FILE";
static const char *PG_DEVICE = "AV_PG_DEVICE";
static const char *DEFAULT_DEVICE = "/VCPS";

// By default, show the id string when canvas is closed.
Boolean AvPGCanvas::showID_ = TRUE;

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

AvPGCanvas::AvPGCanvas(AvPalette *ref, const char *device, const char *file)
{
	palette_ = new AvPGPalette(*this, ref);
	if(palette_ != NULL)
		palette_->ref();

	file_ = device_ = NULL;
	// scale and offsets are set in open();
	scale_ = PIXEL2INCH;
	xoff_ = yoff_ = 0.0;
	beginArgs(0, device, file, 1, 1);
	opened_ = FALSE;
	AvPGGraphicContext *pggc = new AvPGGraphicContext(this);
	state_.setGraphicContext( pggc);
}

AvPGCanvas::~AvPGCanvas()
{
	close();
	if(palette_ != NULL)
		palette_->unref();
	delete [] file_;
	delete [] device_;
}

// Set the arguments to be passed to cpgbeg.
void AvPGCanvas::beginArgs(const int unit,
			   const char *device, const char *file,
			   const int nxsub, const int nysub)
{
	unit_ = unit;
	delete [] file_;
	delete [] device_;
	device = strDup(device);
	file_ = strDup(file);
	nxsub_ = nxsub;
	nysub_ = nysub;
}

/*

These are ignored.
srcx, srcy	Where in raster image the region should come from.
		(Normally 0 since entire image is drawn).
dstx,dsty	Offset within current viewport (Usually 0).
swidth,		How much of the image to draw. Usually 0 to indicate draw
sheight		as much as possible.

The raster is scaled to fit the current viewport (by PGPLOT).

The values are scaled so that under/over range maps to the ends of the
colormap. Blanked data are mapped to white. It is assumed that the value
used to indicate blanked data is greater than the cube's data max.
*/
void AvPGCanvas::drawRaster(	AvRaster &img,
		int /*srcx*/, int /*srcy*/, int /*dstx*/, int /*dsty*/,
			unsigned int /*swidth*/, unsigned int /*sheight*/)
{
AvViewport *vp = viewport();
float	xscl, yscl;
float	*data;
int	width, height;
float	delta, blankValue;
float min, max;

	if(!opened())
		return;

	vp->getRasterScale(xscl, yscl);

	min = img.min();
	max = img.max();

	// The number of colors for the image is 2 less than the
	// available since the lowest and highest are reserved for
	// under/over flow.
	{int ac = palette()->availableColors();
		delta = (max-min)/(ac-2);
	}

	// Copy raster data to an array that the PGPLOT routines can handle.
	// Values greater than the current clip max are set to the max so
	// they will have the same color.
	width = img.width();
	height = img.height();
	int nelements = width*height;
	blankValue = img.getBlankValue();

	data = new float[nelements];
	img.beginAccess();
		img.getRegion(data, 0, 0, width, height);
		// Clip overrange values to max.
		for(int rc=0; rc<nelements; rc++)
		{float v = data[rc];
			if((v>max) && (v < blankValue))
				data[rc] = max;
		}
	img.endAccess();
	////////////////////////////////////////////////////////////////
	// TR is used to translate from I,J to world coordinates.
	// Some of the oddness is to account for the FORTRAN - C indexing
	// convention differences.
	// Wx = tr[0] + tr[1]*i;	( 1<= i,j <= width,height).
	// Wy = tr[3] + tr[4]*j;
	float wxmin, wymin, wxmax, wymax;
	cpgqwin(&wxmin, &wxmax, &wymin, &wymax);
	// ( When i = width, wx = wxmax).
	float a = (wxmax-wxmin)/(width-1);
	float b = (wymax-wymin)/(height-1);
	float tr[6] = {wxmin-a, a, 0.0, wymin-b, 0.0, b};

	// This was used when underrange was set to black.
//	min -= delta;
	max += delta;
	cpgimag(data, width, height, 1, width, 1, height, min, max, tr);
	delete [] data;
}

void AvPGCanvas::drawRaster(AvRaster *img)
{
	if(img == NULL)
		return;
	drawRaster( *img);
}

// Set the viewport, then set clip plane.
void AvPGCanvas::setViewport(AvViewport &vp)
{
	AvCanvas::setViewport(vp);
	setClipPlane(*viewport());	// Set current effective vp.
}

// Reset state, then set clip plane.
void AvPGCanvas::popState()
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

void AvPGCanvas::drawVector(AvPolylineCmd *cmd)

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

	// Size of viewport.
	maxPos[0] = vp->width() -1;
	maxPos[1] = vp->height() - 1;

/*
	// The offsets are not used by the PGCanvas since PGPLOT handles
	// viewport positioning.
	// Offset of the viewport within X window.
	xoff = vp->x0();
	yoff = vp->y0();
*/
	xoff = yoff = 0;	// Viewport positioning is handled by
				// PGPLOT calls in setClipPlane.
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
		cpgmove(currX, currY);
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
			cpgpt((int)numpnts, xpts, ypts, 1);
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
				cpgdraw(currX, currY);
//		printf("LINE (%d) %f %f\n", i, currX, currY);
			}
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
					cpgpoly((int)numpnts, xpts, ypts);
				else
					cpgline((int)numpnts, xpts, ypts);
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
				cpgline(2, xpts, ypts);
			}
		}
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

// Sets up PGPLOT's viewport.
void AvPGCanvas::setClipPlane(AvViewport &vp)
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
		cpgvsiz(X0, X1, Y0, Y1);
#if 0
printf(
 "AvPGCanvas::setClipPlane: X/Y %d/%d,W/H %d/%d; XY:%.2f-%.2f/%.2f-%.2f\n",
	x,y,w,h, X0,X1, Y0, Y1);
showvp();
#endif
		cpgswin(0.0, (float)w-1, 0.0, (float)h-1);
		AvAllotment vpos = vp.vectorPosition();
		// Restore last drawing position.
		cpgmove(vpos.x(), vpos.y());
//		cpgmove(0, 0);
	}
}

void AvPGCanvas::setClipPlane(AvViewport *vp)
{
	if(vp == NULL)
		vp = viewport();
	setClipPlane( *vp);
}

AvPalette *AvPGCanvas::palette()const
{	return  palette_;
}

/* Make the object the current display object and display it. If it
is NULL or the same as current object, redisplay the current list
rather than replacing it.
*/
void AvPGCanvas::show(AvGraphics2D *dob)
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

	// Do a "dry run" to force regeneration of any PGPLOT lists. Then
	// do it again for real. This is necessary because PGPLOT only allows
	// one device to be open at a time.
	AvCanvas::show( *vps_);
	if( open())
	{	cpgswin(0.0, (float)(w-1), 0.0, (float)(h-1));
		AvCanvas::show( *vps_);
		close();
	}
}

void AvPGCanvas::show(AvGraphics2D &dob)
{
	show(&dob);
}

// Initializes scale_ to make sure resultant image fits on the page.
void AvPGCanvas::initScale( const float max, unsigned int size)
{
	if(PIXEL2INCH*size > max)
		scale_ = max/size;
	else
		scale_ = PIXEL2INCH;
}

const char *AvPGCanvas::getPGEVar_(const char *evar, const char *def,
				   const char *defevar)
{
	if(evar == NULL)
		return getEnvStr(defevar, def);
	else
		return getEnvStr(evar, def);
}

// Return a filename/device to use for PGPLOT output.
// evar is a pointer to an environment variable to use to look for
// a user supplied filename. def is returned if evar doesn't exist.
AvString AvPGCanvas::getPGfilename(const char *evar, const char *def)
{
	AvString f = getPGEVar_(evar, def, PG_FILE);
	f.gsub(' ', '_');	// Replace spaces with underscores.
	return f;
}

// Return a PGPLOT device name.
// Use X resource if it exists.
//  Or an environment variable
//    Or use default.
// The resource is checked first since it may be set on the command line.
AvString AvPGCanvas::getPGdevicename(const char *evar, const char *def)
{
	const char *d = getXOptions()->pgDevice;
	if(d == NULL)
		getPGEVar_(evar, def, PG_DEVICE);
	if(d == NULL)				// We need a device name.
		d = DEFAULT_DEVICE;
	AvString n = d;
	return n;
}

// Form a PGPLOT file/device name. If fn is NULL an empty filename
// will be used. (PGPLOT will supply a name). If dev is NULL, the
// default device will be used.
// If the device name doesn't start with a "/", one will be supplied.
AvString AvPGCanvas::makePGname(const char *fn, const char *dev)
{
	// If fn &/or dev is NULL recurse with non NULL arguments.
	if(fn == NULL)
		return makePGname("", dev);

	if(dev == NULL)
		return makePGname(fn, getPGdevicename().chars());

	AvString buf = fn;
	if(*dev != '/')		// PGPLOT devices should start with this.
		buf += "/";
	buf += dev;
	buf.gsub(' ', '_');	// Replace spaces with underscores.
	return buf;
}

// Open the PGPLOT file and initialize viewport.
int	AvPGCanvas::open()
{
	if(opened())
		close();

char	fname[128];
AvString buf;

	AvString f = getPGfilename(PG_FILE, NULL);
	if(f.length() == 0)
	{	if(file_ != NULL)
		{	sprintf(fname, "%s.ps", file_);
			f = fname;
		}
	}
	AvString d = getPGdevicename(PG_DEVICE, device_);
	buf = makePGname(f.chars(), d.chars());

	opened_ = (AvPGCanvas::cpgbeg(unit_, buf, nxsub_, nysub_) == 1)
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
}

// Close device, dumping buffers and adding the id first.
void	AvPGCanvas::close()
{
#if 0
	if(opened())
	{	cpgiden();
		cpgebuf();
		cpgend();
		opened_ = FALSE;
		printf("\tDone\n");
	}
#else
	if(opened())
	{	cpgend();
		opened_ = FALSE;
		printf("\tDone\n");
	}
#endif
}

// Copy a character string. Returns NULL if the argument is NULL.
char	*AvPGCanvas::strDup(const char *str)
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

	// Calls cpgbeg, but takes a const char *filename
int AvPGCanvas::cpgbeg(const int unit, const char *file,
		      const int nxsub, const int nysub)
{char *name = strDup(file);

	int rtn = ::cpgbeg(unit, name, nxsub, nysub);
	delete [] name;
	return rtn;
}

int AvPGCanvas::cpgbeg( const int unit, const AvString &file,
			const int nxsub, const int nysub)
{
	return cpgbeg(unit, file.chars(), nxsub, nysub);
}

// Call PGPLOT's cpgend routine, possibly calling cpgiden() first.
void AvPGCanvas::cpgend()
{
	cpgend(showID_);
}

void AvPGCanvas::cpgend(const Boolean showID)
{
	if(showID)
		cpgiden();
	cpgebuf();
	::cpgend();
}

/****************************************************************/
/****************************************************************/
/****************************************************************/
/*			Graphic context code.			*/
/****************************************************************/
/****************************************************************/
/****************************************************************/

AvPGGraphicContext::AvPGGraphicContext( AvPGCanvas *can) :
						AvGraphicContext()
{
	canvas_ = can;
}

AvPGGraphicContext::AvPGGraphicContext(const AvGraphicContext &gc):
						AvGraphicContext(gc)
{
	canvas_ = NULL;
}

void AvPGGraphicContext::copy(const AvGraphicContext &gcin)
{
	canvas_ = NULL;
	AvGraphicContext::copy(gcin);
}

void AvPGGraphicContext::copy(const AvPGGraphicContext &gcin)
{
	canvas_ = gcin.canvas_;
	AvGraphicContext::copy(gcin);
	update();
}

// Emulate the = operator for plain graphic contexts.
AvGraphicContext& AvPGGraphicContext::operator=(const AvGraphicContext &gcin)
{
	AvGraphicContext::copy(gcin);
	return *this;
}

AvPGGraphicContext& AvPGGraphicContext::operator=(const AvPGGraphicContext &gcin)
{
	copy(gcin);
	return *this;
}

AvPGGraphicContext::~AvPGGraphicContext()
{
}

#if 0
void AvPGGraphicContext::setLineWidth(const AvPGGraphicContext::LineWidth LW)
{
	if(!canvasOpened())
		return;

	AvGraphicContext::setLineWidth(LW);
	{int lw;
		switch (LW) {
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
		cpgslw(lw);
	}
}
#else
void AvPGGraphicContext::setLineWidth(const int LW)
{
	if(!canvasOpened())
		return;

	AvGraphicContext::setLineWidth(LW);
	// PGPLOT doesn't support lw=0.
	int lw = LW;
	if(lw <= 0)
		lw = 1;
	cpgslw(lw);
}
#endif

void AvPGGraphicContext::setLineStyle(const LineStyle LS)
{
	if(!canvasOpened())
		return;

	AvGraphicContext::setLineStyle(LS);
	{int ls;
		switch(getLineStyle()) {
		case AvGraphicContext::SOLID:
			ls = 1;
			break;
		case AvGraphicContext::DASHED:	// Resource??
			ls = 2;
			break;
		// There should be a dotted here.
		default:
			ls = 1;
		}
		cpgsls(ls);
	}
}

void AvPGGraphicContext::setForeground(const unsigned long c)
{
	if(!canvasOpened())
		return;

	AvGraphicContext::setForeground(c);
#if 0
	AvPalette *pal = canvas_->getPGPalette()->referencePalette();
	unsigned long color;

	if(pal != NULL)
	{	if(pal->virtualToPhysical(c, color))
			cpgsci(color);
	}
#else
	AvPalette *pal = canvas_->getPGPalette();
	unsigned long color;

	if(pal != NULL)
	{	if(pal->virtualToPhysical(c, color))
			cpgsci((int)color);
	}
#endif
}

// Needed by copy
void AvPGGraphicContext::update( const int msk)
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
	   AvPGPalette *pal = canvas_->getPGPalette();
	   unsigned long color;

		if(pal != NULL)
		{	if(pal->virtualToPhysical(getForeground(), color))
				cpgsci((int)color);
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
		cpgslw(lw);
	}

	if(m & AvGraphicContext::LINESTYLE)
	{int ls;
		switch(getLineStyle()) {
		case AvGraphicContext::SOLID:
			ls = 1;
			break;
		case AvGraphicContext::DASHED:	// Resource??
			ls = 2;
			break;
		// There should be a dotted here.
		default:
			ls = 1;
		}
		cpgsls(ls);
	}
}

Boolean AvPGGraphicContext::canvasOpened()const
{return	((canvas_!=NULL)&&(canvas_->opened())) ? TRUE : FALSE;
}
