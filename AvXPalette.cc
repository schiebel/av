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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvXPalette.cc,v 19.0 2003/07/16 05:48:04 aips2adm Exp $
//
// $Log: AvXPalette.cc,v $
// Revision 19.0  2003/07/16 05:48:04  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:39  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.3  2002/01/22 22:26:44  hravlin
// Modified shiftcount() to avoid 64 bit compiler warnings.
//
// Revision 17.2  2002/01/07 22:54:39  hravlin
// Replaced a variable named "or" with "OR" since "or" is a keyword.
//
// Revision 17.1  2002/01/03 20:07:46  hravlin
// Added scaling functions used by non X canvases (eg. OpenGL canvas).
//
// Revision 17.0  2001/11/12 19:43:07  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:13  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:43  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:09:10  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.8  1999/10/25 18:50:44  hravlin
// Added scaleFToX() to convert floating point data directly to XImage
// struct so a separate copy is not necessary.
//
// Revision 13.7  1999/09/27 16:15:01  hravlin
// Fixed another warning.
//
// Revision 13.6  1999/09/27 16:11:55  hravlin
// Removed a couple of compiler warnings.
//
// Revision 13.5  1999/09/27 15:06:31  hravlin
// For read only colormaps with decomposed indexes, the length of the emulated
// colormap is set to the minimum of what the window can handle & 4096.
//
// Revision 13.4  1999/09/27 14:02:44  hravlin
// Fixed a memory leak in getcolors().
//
// Revision 13.3  1999/09/16 20:11:22  hravlin
// Removed egcs and IRIX -64 compiler warnings.
//
// Revision 13.2  1999/09/15 20:56:38  hravlin
// Redid colormap handling to support TrueColor visuals.
//
// Revision 13.1  1999/08/25 19:33:09  hravlin
// Added a cast to int to reduce compiler warnings.
//
// Revision 13.0  1999/08/10 18:41:01  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:26  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:02:20  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:56:36  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.6  1998/06/11 17:38:03  hr
// Removed some signed/unsigned comparision warnings.
//
// Revision 9.5  1998/06/09 21:46:43  hr
// Fixed several type conversion warnings.
//
// Revision 9.4  1998/05/20 20:33:59  hr
// allocNamedColor() was setting current_.flags with unitialized values.
//
// Revision 9.3  1998/05/04 20:39:21  hr
// Removed some unused variables, adjusted an unsigned comparision and
// in a call to XQueryColors, added a cast to int.
//
// Revision 9.2  1998/05/01 21:48:09  hr
// Numerous changes, mostly to reallocRO, checkAllocate_, allocNamedColor
// and update. Now supports unallocated (free) colors so widgets can allocate
// colors from aipsview's colormap for their needs.
//
// Revision 9.1  1997/12/17 21:53:07  hr
// Changes to remove warnings from MIPSpro.7x compiler.
//
// Revision 9.0  1997/08/25 21:31:56  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.4  1997/05/21 20:08:59  hr
// Added reallocRW() and reallocRO() and redid color allocation again.
// Reallocating an entry RO doesn't always get back the same index.
//
// Revision 8.3  1997/05/13 19:44:00  hr
// Fixed some g++ warnings.
// In checkAllocated(), when converting current_ cells to RO, ignore those
// already marked as readonly. (Makes a difference when client and server
// are far apart).
//
// Revision 8.2  1997/05/05 20:03:41  hr
// current_ had stopped being initialized.
//
// Revision 8.1  1997/04/30 20:17:14  hr
// Primarily changes to allow aipsview to get widget colors when compiled
// with Motif 2.x. Colormaps used to be allocated completely RW with entries
// corresponding to in use entries in the colormap marked as RO. The new
// method actually allocates these entries as RO so widget generation routines
// can use them.
//
// Revision 8.0  1997/02/20 03:16:42  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.5  1997/01/30 22:43:08  hr
// Got rid of 1 of 2 g++ warnings.
//
// Revision 7.4  1997/01/30 22:11:06  hr
// If user requested more colors than were available, checkAllocated()
// did not setup the mapping segment correctly.
//
// Revision 7.3  1997/01/09 18:26:06  hr
// Changes to support checking of the -numColors command line switch.
//
// Revision 7.2  1996/12/12 07:51:05  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.11  1996/06/18  18:46:48  pixton
//  Copyright Update
//
//  Revision 1.10  1996/02/28  15:59:53  hr
//  queryColors had two parameters reversed. Added a readonly check for
//  an XStoreColors.
//
//  Revision 1.9  1995/10/25  15:52:56  hr
//  allocNamedColor needs to check for success after call to XAllocNamedColor.
//
//  Revision 1.8  1995/10/25  15:18:46  hr
//  Initialize XColor.pad to keep memory checker happy.
//
//  Revision 1.7  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.7  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.6  1995/09/01  21:20:33  hr
//  Renamed a variable named len to avoid compiler warnings about hiding.
//
//  Revision 1.5  1995/07/18  17:27:17  hr
//  'Fixed' g++ warnings. Mostly don't do "for(int i...)".
//
//  Revision 1.4  1995/06/22  19:05:01  hr
//  Changed the way allocNamedColor interacts with addColor.
//
//  Revision 1.3  1995/05/17  14:29:33  hr
//  Added call to queryColors in init so current_ gets initialized.
//
//  Revision 1.2  1995/03/29  21:33:25  hr
//  allocNamedColor now calls checkAllocated
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvXPalette.cc

Handles interactions with X colormaps for aipsview.

XColor current_ is used to hold a current copy of the colormap. The pad
field is used to hold what state the entry is in:
 State		Function
UNALLOC		Initial state, entry is allocated.
RDONLY		Entry has been allocated RO on server.
RDWR		Entry has been allocated RW on server.
LOCKED		An attempt to allocate this entry RDONLY failed (presumably
		because another entry matching the reqested color already
		existed). An attempt is made to allocate the entry RW, so
		it won't be allocatable. The entry is marked LOCKED so it
		won't be used. Used to track root cmap entries.

RDONLY entries are used for widget and PGPLOT colors. RDWR entries are used
as the changeable colormap.

The number of entries available for RW is:
	<# of colors in map> - <# RO colors> - <# unallocated colors>.
Configurations with many RO colors (Piggy window managers), may wish to
reduce the number of unallocated colors.

*/

#include "AvXPalette.h"
#include "AvWidgetMisc.h"
#include "AvFallbacks.h"	// getXOptions().
#include <stdlib.h>		// getenv, strtol
#include <iostream.h>

#include <strings.h>
#include "AvApp.h"


static const char NCOLORS[] = "AV_PAL_NUMCOLORS";

// States a colormap entry can be in.
#if 0
enum CMState {UNALLOC=0, RDONLY=1, RDWR=2, LOCKED};
#else
enum CMState {UNALLOC=AvColortableEntry::UA, RDWR=AvColortableEntry::RW,
	      RDONLY = AvColortableEntry::RO, LOCKED};
#endif

#if 0
/////// Used during testing.
static char *CMStates[] = {"UnAlloc", "RDOnly", "RdWr", "Locked"};

static const char *cmapState(const CMState s)
{int i = (int) s;

	if((i >= 0) && (i < sizeof(CMStates)/sizeof(*CMStates)))
		return CMStates[(int)i];
	else
		return "UNK";
}

static char *pxcolor(ostream &s, const XColor &x)
{	s << x.pixel << ":" << x.red << "/" << x.green << "/" << x.blue
	  << "/" << cmapState((CMState)x.pad);
	return "";
}
#endif

enum {ALLCOLORS=-2, NOTBW=-1, UNALLOCATED=0};

/* Check the environment variable to see how many colors to use.
Value		effect				return
undefined	use unallocated			  0
 or 0
BW		use all but B/W			 -1
ALL		use entire table		 -2
 n < max	use 'n' colors as long as n	  n
		is <= maxColors, otherwise max.

if n < -2, then it is treated as the number of colors to protect, rather
than the number to use.
*/

// Convert val to encoded # of colors. Returns 1 if a match was found
// otherwise 0.
static int convertName(const char *val, int &ival)
{
long	n;

	if( val == NULL)
		return 0;
	else
	if(strlen(val) == 0)
		return 0;
	else
//	if(strcasecmp(val, "AVAILABLE") == 0)
	if(strncasecmp(val, "AVAIL", sizeof("AVAIL")) == 0)
	{	ival = UNALLOCATED;
		return 1;
	}
	else
	if(strcasecmp(val, "BW") == 0)
	{	ival = NOTBW;
		return 1;
	}
	else
	if(strcasecmp(val, "ALL") == 0)
	{	ival = ALLCOLORS;
		return 1;
	}
	else
	{	n = strtol(val, NULL, 0);
		if( n == 0)
			return 0;
		ival = (int) n;
		return 1;
	}
}


// Return the encoded number of colors the user wants to use.
// If an X resource exists use it. Otherwise try for an evironment variable.
static int getncolors()
{
char	*val = getXOptions()->numColors;
int	n;

	if(convertName(val, n))
		return n;
	val = getenv(NCOLORS);
	if(convertName(val, n))
		return n;
	return 0;
}

#if 0

/* f = 1 RO colors
   f = 2 RW colors
   f = 3 All
*/
static void printXColor(ostream &s, XColor &ptr, const int num, const int f)
{
	s << endl;
	s << "\t" << num << " colors " << endl;
	for(int i=0; i < num; i++, ptr++)
	{ int pad = clr.pad;
	   if( ((f&1) && (pad == RO)) || ((f&2) && ( pad == RW)) || (f==3))
		{	s << i << "\t" << pxcolor(s, clr) << endl;
		}
	}
}

static void printXColors(ostream &s, XColor *system, XColor *current,
			 int syslen)
{int num=syslen;
 XColor *sys=system, *cur=current;

	s << endl;
	s << "\t" << num << " colors " << endl;
	for(int i=0; i < num; i++, sys++, cur++)
	{	s << i << "\t" << pxcolor(s, *sys) << " " << pxcolor(s,*cur)
		<< endl;
	}
	s << endl;
}

#endif


#if 0
/* f = 1 RO colors
   f = 2 RW colors
   f = 3 All
*/
static void compareMaps(ostream &s, XColor *ptr1, const int num0,
			XColor *ptr2, const int num1)
{ int num;

	num = (num0 <= num1) ? num0 : num1;

	s << endl;
	s << "\t" << num << " colors " << endl;
	for(int i=0; i < num; i++, ptr1++, ptr2++)
	{
#if 0
		if(((ptr1->pixel != ptr2->pixel) || (ptr1->red != ptr2->red)
		   || (ptr1->green != ptr2->green) ||
		      (ptr1->blue != ptr2->blue)) && (ptr1->flags != UNALLOC))
#endif
		{ cout  << i << " "
			<< pxcolor(cout, *ptr1)
			<< " : " << pxcolor(cout, *ptr2)
			<< endl;
		}
	}
}
#endif

#if 0
static void printXColors(ostream &s, XColor *system, XColor *current,
			 int syslen)
{int num=syslen;
 XColor *sys=system, *cur=current;

	s << endl;
	s << "\t" << num << " colors " << endl;
	for(int i=0; i < num; i++, sys++, cur++)
	{	s << i << "\t" << pxcolor(s, *sys) << " " << pxcolor(s,*cur)
		<< endl;
	}
	s << endl;
}

#endif

#if 0
void printSegment(ostream &s, AvColormapSegment &seg)
{int length;

	s << (seg.locked()) ? "Locked " : "Unlocked ";
	length = seg.getLength();
	s << "Start = " << seg.getStart() << " Length = " << length << endl;
	for(int i=0; i< length; i++)
	{	s << seg.getValue(i);
		if( ((i+1) % 32) == 0)
			s << endl;
		else
			s << " ";
	}
	s << endl;
}
#endif

#if 0
// If cmap is non zero, and is rw, it must not have any cells allocated.
AvXPalette::AvXPalette(Widget wid, Colormap cmap, const int cmaplen):
	AvPalette(cmaplen)
{
	initialize(XtDisplay(wid), widgetVisual(wid), cmap);
}
#endif

AvXPalette::AvXPalette( Display *dpy, Visual *visual,
			Colormap cmap, const int cmaplen):
	AvPalette(cmaplen)
{
	initialize( dpy, visual, cmap);
}

AvXPalette::~AvXPalette()
{
	if(current_ != NULL)
		delete [] current_;
}

/* If we create the colormap, this is called to make sure is has something
useful in it.
*/
static void loaddefault(AvPalette *pal)
{
int	nc;
float	*r, *g, *b;

	nc = pal->availableColors();
	r = new float[nc];
	g = new float[nc];
	b = new float[nc];
	for(int i=0; i< nc; i++)
		r[i] = g[i] = b[i] = (float)(nc-i-1)/(float)(nc-1);
	pal->storeColors(r, g, b);
	delete [] r;
	delete [] g;
	delete [] b;
}

// Count the # of trailing 0 bits in a #. (Amount needed to shift a value).
static inline unsigned short shiftcount(const unsigned long v)
{ unsigned long mask = 1;
  const unsigned short nbits = (unsigned short)(sizeof(v)*8);
  unsigned short i;
	for(i=0; i<nbits; i++,mask += mask)
		if(mask&v)
			break;
	return i;
}

void AvXPalette::initialize(Display *dpy, Visual *visual, Colormap cmap)
{
int	i;

	checked_ = False;
	if((dpy == NULL) || (visual == NULL))
	{	dpy_ = NULL;
		visual_ = NULL;
		current_ = NULL;
		return;		// ERROR!
	}
	dpy_ = dpy;
	visual_ = visual;

	// PseudoColor & TrueColor are the only ones aipsview supports.
	switch(visual->c_class) {
	case StaticGray:
		readonly_ = True;
		decomposedIndex_ = False;
		break;
	case GrayScale:
		readonly_ = False;
		decomposedIndex_ = False;
		break;
	case StaticColor:
		readonly_ = True;
		decomposedIndex_ = False;
		break;
	case PseudoColor:
		readonly_ = False;
		decomposedIndex_ = False;
		break;
	case DirectColor:
		readonly_ = False;
		decomposedIndex_ = True;
		break;
	case TrueColor:
		readonly_ = True;
		decomposedIndex_ = True;
		break;
	default:
		readonly_ = False;
		break;
	}

	if(decomposedIndex_)
	{	red_mask_ = visual->red_mask;
		red_shift_ = shiftcount(red_mask_);
		if(red_shift_ > 0)
			red_max_ = (int)(red_mask_ >> red_shift_);
		else
			red_max_ = (int)red_mask_;

		green_mask_ = visual->green_mask;
		green_shift_ = shiftcount(green_mask_);
		if(green_shift_ > 0)
			green_max_ = (int)(green_mask_ >> green_shift_);
		else
			green_max_ = (int)green_mask_;

		blue_mask_ = visual->blue_mask;
		blue_shift_ = shiftcount(blue_mask_);
		if(blue_shift_ > 0)
			blue_max_ = (int)(blue_mask_ >> blue_shift_);
		else
			blue_max_ = (int)blue_mask_;
//		maxIndex_ = red_mask_ | green_mask_ | blue_mask_;
//printf("Maxindex is %d\n", maxIndex_);
	}
	else
	{	red_shift_ = green_shift_ = blue_shift_ = 0;
		red_max_ = green_max_ = blue_max_ = 0;
		red_mask_ = green_mask_ = blue_mask_ = 0;
	}

	if(cmap == 0)
	{Window w = AvApp::referenceWindow();
		// Don't allocate any colors. Cells will be allocated later.
		cmap_ = XCreateColormap( dpy, w, visual, AllocNone);
	}
	else
		cmap_ = cmap;

	if(readonly_ && decomposedIndex_)
	{ unsigned long mc = (red_mask_ | green_mask_ | blue_mask_)+1;
		// For read only windows, we're emulating the LUT in
		// software so the size is arbitrary. Limit the
		// size to the minimum of what the screen can hold and
		// what PostScript can handle.
		if(mc < 4096)
			maxcolors_ = (int)mc;
		else
			maxcolors_ = 4096;
	}
	else
	{	maxcolors_ = visual->map_entries; // Total # of slots in CM.
	}

	// Make sure the color mapping tables get created.
	expandTable(maxcolors_);
	current_ = new XColor[maxcolors_];	// Copy of our color table.
	for(i=0; i< maxcolors_; i++)
	{	current_[i].pixel = i;
		current_[i].flags = DoRed | DoGreen | DoBlue;
		current_[i].pad = UNALLOC;	// Flag all as unused.
		// Just in case.
		current_[i].red = current_[i].green = current_[i].blue = 0;
	}

	if(!readonly())
	{
		// Initially, mark all colors as unallocated since
		// anything that can't be allocated RW is already allocated RO
		for(i=0; i< maxcolors_; i++)
			current_[i].pad = RDONLY;		// RO

		// Allocate all colors in our color map RW.
		// This isn't the same as creating with AllocAll since
		// FreeColors fails for that. All colors are allocated RW now
		// so when we need to allocated one RO, it is possible to
		// control which one gets reallocated.
		unsigned long *pbuf = new unsigned long[maxcolors_];
		unsigned int numRW;
		pbuf = allocRWCells(dpy_, cmap_, maxcolors_, numRW, False);
		for(unsigned int I=0; I < numRW; I++)
			current_[pbuf[I]].pad = RDWR;
		delete [] pbuf;
	}

	// Figure out which colors are in use + copy to cmap_.
	checkAllocated();

	if(cmap == 0)	// Load a default palette if we created colormap.
		loaddefault(this);
}

static inline unsigned long floatToComponent(const float v,
				const unsigned short max, const short shift,
				const unsigned long mask)
{ unsigned long p = (((unsigned long) (v*max))<< shift) & mask;
	return p;
}

/*
 Copy internal colormap to X server if RW.
For decomposed index colormaps, the RGB values are also converted to
pixel values.
*/
void AvXPalette::putColors()
{
int		DO;
const float	scl =  65535.;

	// Maximum # of colors to deal with. Only need to deal with
	// RW cells since RO cells should already be set.
	int maxC = maxColors();

	DO = DoRed | DoGreen | DoBlue;

	XColor *buf = current_;

	// Copy from internal buffer to XColor buffer.
	int index = 0;
	for(int i=0; i<maxC; i++)
	{ float r, g, b;
	  AvColortableEntry *e = findEntryByIndex(i);

		if(e == NULL)		// Oops. (error)
			continue;
		e->get(r, g, b);	// Get RGB float values.

		if(decomposedIndex())
		{ unsigned long pixel, ri, gi, bi;
			ri = floatToComponent(r,
				red_max_, red_shift_, red_mask_);
			gi = floatToComponent(g,
				green_max_, green_shift_, green_mask_);
			bi = floatToComponent(b,
				blue_max_, blue_shift_, blue_mask_);
			pixel = ri | bi | gi;
			e->setPixel(pixel);
		}

		if(e->state() == RDWR)
		{	buf[index].pixel = e->pixel();
			buf[index].red = (unsigned short) (r * scl + 0.5);
			buf[index].green = (unsigned short) (g * scl + 0.5);
			buf[index].blue = (unsigned short) (b * scl + 0.5);
			buf[index].flags = DO;
			index += 1;
			e->setFlags(0);
		}
	}

	if(!readonly())
	{	XStoreColors(dpy_, cmap_, buf, index);
	}
}

// Read entire colormap int current_, return requested values.
void AvXPalette::getColors(const Boolean /*force*/)
{
int	num, i;
const float	scl = 1.0/65535.;
XColor *buf;

	if( decomposedIndex())
		return;		// Not supported.

	// Get current copy of the colormap.
	num = maxColors();
	buf = current_;
	int cnt=0;
	for(i = 0; i < num; i++)
	{	if(colors_[i].flags() == AvColortableEntry::UA)
			continue;

		buf[cnt].pixel = colors_[i].pixel();
		buf[cnt].flags = DoRed | DoGreen | DoBlue;
		cnt++;
	}
	XQueryColors(dpy_, cmap_, buf, cnt);

	// Assumes pixels are indexes, not real color spec.
	for(i = 0; i < num; i++)
	{ unsigned long index;

		index = buf[i].pixel;
		colors_[index].store(buf[i].red*scl, buf[i].green*scl,
				     buf[i].blue*scl);
		colors_[index].setFlags(0);	// reset to not dirty.
	}
}

void AvXPalette::freeColor(XColor &color)
{
	color.pad = UNALLOC;
	if(readonly())
		return;

	XFreeColors(dpy_, cmap_, &color.pixel, 1, 0);
}

/* Change a color entry from RW to RO. If a different entry is allocated,
it is freed and an attempt is made to reallocate the initial entry. (The
allocated entry may not be the same if another RO entry matches the
color). Sets xc.pad to RO on success, LOCKED if it had to be reallocated
RW, and UNALLOC if we couldn't get it back. Returns TRUE unless the cell is
returned unallocated. And an error message is printed.

This can only be called during initialization when all colormap cells are
allocated RW.

If cmap_ is read only, just set the flag and return True.
*/
Boolean AvXPalette::reallocRO(XColor &xc)
{ XColor tmpx = xc;	// Make copy in case values get changed.
  Boolean rtn;
  unsigned long pixel;

	if(readonly())
	{	xc.pad = RDONLY;
		return TRUE;
	}
	freeColor(xc);
	if(XAllocColor(dpy_, cmap_, &tmpx))
	{		pixel = tmpx.pixel;
		// Make sure what was returned is same as
		// what we freed. (It can be different).
		if(pixel != xc.pixel)	// Get it back.
		{ unsigned long pixel1 = ~0;
			XFreeColors(dpy_, cmap_, &tmpx.pixel, 1, 0);
			// Assumes that all cells are allocated so there is
			// only the 1 we freed to get back.
			XAllocColorCells(dpy_, cmap_, FALSE,
					 NULL, 0, &pixel1, 1);
			if(pixel1 == xc.pixel)
			{	xc.pad = LOCKED;
				return TRUE;
			}
			else
			{	cerr <<
			"AvXPalette::reallocRO: Couldn't reallocate index "
				<< xc.pixel << endl;
				return TRUE;
			}
		}
		xc.pad = RDONLY;
		rtn = TRUE;
	}
	else
	{	cerr << "AvXPalette::reallocRO: Couldn't reallocate "
		     << xc.pixel << endl;
		rtn = FALSE;
	}

	return rtn;
}

/* Determine the number of allocated colors (& which ones they are)
   by allocating every color we can from the color table.
   The ones we can't allocate are already in use. Try not to take
   very long to do this since another process might fail if it asks
   for a color at the wrong time.
   If free is True, the cells are unallocated when done.
*/
unsigned long *AvXPalette::allocRWCells(Display *dpy, Colormap map,
				   const unsigned int maplength,
				   unsigned int &numallocated,
				   const Bool free)
{
	unsigned long *pixels = new unsigned long[maplength];
	if(pixels == NULL)
	{	numallocated = 0;
		return NULL;
	}
	unsigned long *pxlptr = pixels;
//	int npixels = (int)(maplength/2);
	int npixels = (int)maplength;

	while( npixels > 0)
	{	if(XAllocColorCells(dpy, map, FALSE, NULL, 0,
				    pxlptr, npixels))
		{	pxlptr += npixels;
			if((unsigned int)(pxlptr-pixels) >= maplength)
				break;
		}
		npixels /= 2;
	}

	// Pick up any strays. (Available cells don't have to be contiguous). 
	while(XAllocColorCells(dpy, map, FALSE, NULL, 0, pxlptr, 1))
			pxlptr += 1;

	npixels = (int)(pxlptr - pixels);	// # of RW entries in map.

	if((npixels > 0) && free)
	{	XFreeColors(dpy, map, pixels, npixels, 0);
		XFlush(dpy);		/* Force the xfree to server.*/
	}
	numallocated = npixels;
	return pixels;
}

// Returns:
//	The number of colors in the system colormap.
//	The number of colors allocatable in the system colormap.
//	If the colormap is writable, returns a pointer to a list containing
//	the allocatable entries. The list will be numcells long, but only
//	the first cellsfree entries will be set. (Use delete [] to free).
//	If the colormap is READONLY, cellsfree is set to 0 and a a NULL
//	pointer is returned.
unsigned long *AvXPalette::CheckAllocated(Display *dpy,
				    Visual *visual,
				    Colormap map,
				    unsigned int &numcells,
				    unsigned int &cellsfree)
{
unsigned long *pixels;

	numcells = visual->map_entries;	// Total # of slots in CM.

	// Is colormap RO?
	switch(visual->c_class) {
	case StaticGray:
	case StaticColor:
	case TrueColor:
		cellsfree = 0;
		return NULL;
//		break;
	default:		// Anything else is RW.
		break;
	}

	pixels = allocRWCells(dpy, map, numcells, cellsfree, True);
	return pixels;
}

/*
Check the system color table for allocated entries, copy them to our
colormap and make read only and setup the first three segments:

	0	only unallocated entries.
	1	all entries but B/W.
	2	all entries.
	[3]	User specified # of colors.

//May be called more than once. The first segment will be rebuilt.
	[ The current version ignores all but the first call.]

Colormap layout:
	[ RO....RO RW...RW  UA...UA]
 RO - RO entries copied from system colormap.
 RW - Changeable entries.
 UA - Unallocated entries available for widgets.

NOTES:

 Since the RO entries from the system colormap are not guaranteed to be
contiguous, the others aren't either. Unallocated entries will be
allocated from the available entries at the end of the map. This should
make reenabling multiple calls easier.

 The system colormap may be a different length than the one we are
using. It is assumed that an index in the system colormap corresponds to
the same index in the allocated colormap.

 If the user requests a number of colors that doesn't match the above, a
segment is created with that many colors. The startpoint is moved up to
minimize flashing on most servers.

Assumes it is safe to use XColor.pad for our own use.

 Assumes system colormap has allocateable cells. If the root map is
completely allocated, a minimum number of cells will be used anyway so
aipsview will be able to function.

*/

void AvXPalette::checkAllocated_()
{
Display		*dpy;
int		screen;
Visual		*visual;
unsigned long	white, black, i;
unsigned int	maxRootColors, rootCellsUsed, rootCellsFree;
unsigned long	*pixels;
//int		verbose = getXOptions()->colorVerbosity;

	if(checked_)	// Currently can only be called once.
		return;
	else
		checked_ = True;

	/* Use dpy to get 'Default' values. Want to use info from
	   root window, not the visual we are creating a CM for.
	*/
	dpy = display();
	screen = DefaultScreen(dpy);
	visual = DefaultVisual(dpy, screen);
	white = WhitePixel(dpy,screen);	/* special pixels. */
	black = BlackPixel(dpy,screen);
	Colormap rootMap = DefaultColormap(dpy, screen);

	pixels = CheckAllocated(dpy, visual, rootMap,
				maxRootColors, rootCellsFree);

	// Is system cmap read only?
	const Boolean ReadOnlyRoot = (pixels == NULL);
	if(ReadOnlyRoot)
		rootCellsUsed = 0;
	else
	{	// (Note that maxRootColors may not = maxColors_).
		rootCellsUsed = maxRootColors - rootCellsFree;
	}

	// If the system colormap is readonly, don't bother trying to
	// copy the allocated colors.

	XColor *system=NULL;

	if(!ReadOnlyRoot)
	{	system = new XColor[maxRootColors];

		// Pick up a copy of the system colormap.
		for(i=0; i< maxRootColors; i++)
		{	system[i].pixel = i;
			system[i].flags = DoRed | DoGreen | DoBlue;
		}
		XQueryColors(dpy_, rootMap, system, (int)maxRootColors);

		// Flag all as RO.
		for(i=0; i < maxRootColors; i++)
			system[i].pad = RDONLY;

		// Then, flag allocatable cells as UNALLOC.
		for(i=0; i< rootCellsFree; i++)
		{unsigned long indx = pixels[i];
			system[indx].pad = UNALLOC;
		}
	}

	////////////////////////////////////////////////
	// Figure out how many entries to allocated RO, RW and how many to
	// make unallocated.
	////////////////////////////////////////////////
	const int MaxColors = maxColors();
	const Boolean ReadOnly = readonly();

	// Minimum # of RW entries we'll allow.
	int minRW = getXOptions()->minColors;
	if(minRW > MaxColors)
		minRW = MaxColors;
	else
	if(minRW < 8)		// The really absolute minimum we'll allow.
		minRW = 8;

	// Encoded # of unallocated entries to be reserved in our cmap.
	int numColors = getncolors();	// # of colors user asked for.
	if(numColors > MaxColors)
		numColors = MaxColors;
	else
	{ int mc = MaxColors - minRW;
		if(numColors < -mc)	// # of RO colors to protect.
			numColors = -mc;
		// If minRW = MaxColors, it makes numColors -> AVAILABLE (0).
	}

	// # of colors to be reserved for widgets.
	int nUA = getXOptions()->numFreeColors;

	int minRO = 0;	// Minimum # of RO entries to allow.
	// Adjust min number of RO entries based on user's request
	// for RW entries.
	if( numColors == NOTBW)
		minRO = 2;	// All but BW.
	else
	if( numColors == ALLCOLORS)
		minRW = MaxColors - nUA;
	else
	if( numColors < ALLCOLORS)	// # of entries to leave RO.
		minRO = -numColors;

	if(minRO > (int)rootCellsUsed)	// No more than what's already RO.
		minRO = (int)rootCellsUsed;

	int nRO = (int)rootCellsUsed; // Initial # of entries to set RO in our map.

	if( nRO < minRO)
		nRO = minRO;
	//////
	if(ReadOnlyRoot)
		nRO = minRO = 0;	// No RO entries needed.

	// Current # of colors available for RW.
	// (This max go negative, but things get fixed up in the end).
	int avail = MaxColors - (nRO + nUA);
	// The default number of RW entries is the length of our map -
	// the sum of the number of read only and unallocated entries.
	int nRW;
	if(numColors > 0)		// User expicitly set # of RW colors.
		nRW = numColors;
	else
	if(numColors == NOTBW)
		nRW = MaxColors - nUA - 2;
	else
	if( numColors == ALLCOLORS)
		nRW = MaxColors - nUA;
	else
	if(numColors < ALLCOLORS)	// # of colors to protect.
		nRW = MaxColors - nUA - minRO;
	else
		nRW = avail;

	// Fudge values to be within minimum bounds.
	if( nRW < minRW)
	{	nRW = minRW;
		nRO = MaxColors - (nRW + nUA);
	}
	else
	if( nRW > avail)
		nRO = MaxColors - (nRW + nUA);

	if( nRO < minRO)
	{	nRO = minRO;
		nUA = MaxColors - (nRO + nRW);
	}

	////////////////////////////////////////////////////////////////
	// For each entry (up to nRO entries) that is RO in the system cmap,
	// free the corresponding entry in our table then reallocate it RO
	// with the system's colors.
	// This assumes all cells are allocated so if one is freed then
	// it comes back if there is another allocation.

	// The two maps may be different lengths.
	unsigned long mc = (maxRootColors <= (unsigned long)MaxColors)
			? maxRootColors : MaxColors;

	// If both the system and our colormaps are read/write, copy
	// corresponding information from system to ours. If either is
	// RO, don't bother.
	if(!ReadOnlyRoot && ! ReadOnly)
	{ if(numColors == -1)	// Only make Black and White RO.
	 {		current_[white].pixel = system[white].pixel;
			current_[white].red = system[white].red;
			current_[white].green = system[white].green;
			current_[white].blue = system[white].blue;
			current_[white].flags = system[white].flags;
			reallocRO(current_[white]);
			current_[black].pixel = system[black].pixel;
			current_[black].red = system[black].red;
			current_[black].green = system[black].green;
			current_[black].blue = system[black].blue;
			current_[black].flags = system[black].flags;
			reallocRO(current_[black]);
	 }
	 else
	 {int nro = nRO;
	  for(i=0; i< mc; i++)
	  {// Realloc upto nRO entries as read only.
		if(nro <= 0)
			 break;
		if(system[i].pad == RDONLY) // Copy to ours & make RO.
		{
			// This assumes all current_ values are initially RW.
			// If some are already RO, would need to fudge
			// current_'s index.
			current_[i].pixel = system[i].pixel;
			current_[i].red = system[i].red;
			current_[i].green = system[i].green;
			current_[i].blue = system[i].blue;
			current_[i].flags = system[i].flags;

			reallocRO(current_[i]);		// Reallocate as RO.
			nro -= 1;
		}
	  }
	 }
	 // Deallocate nUA entries from those marked RW starting at end of
	 // table.
	 {int nua = nUA;
		for(int j = MaxColors-1; j >= 0; j--)
		{
			if(nua <= 0) break;
			if(current_[j].pad == RDWR)
			{	freeColor(current_[j]);
				nua -= 1;
			}
		}
	 }
	}
	else	// (At least one of system or our colormaps is read only).
	{	for(i=0; i < (unsigned int)nRW; i++)
			current_[i].pad = RDWR;
		int nua = nUA;
		// Mark some as available for allocation. For RO maps, it
		// only gives us some place to store the info in colors_.
		// For RW maps, it frees space for widgets.
		while(nua-- > 0)
		{	current_[i].pad = UNALLOC;
			if(!ReadOnly)
			 XFreeColors(dpy_, cmap_, &current_[i].pixel, 1, 0);
			i+= 1;
		}
#if 0
	if(i > MaxColors)
		cerr << "Set too many UA entries. " << i << " instead of "
			<< nUA << endl;
#endif
	}

	////////////////////////////////////////////////////////////////
	// Use the information in current_ to setup mapping information.

	int rwi=0, uai=MaxColors;
	const float	scl = 1.0/65535.;

	// Copy color information to colors_/map_. Indexes for cells
	// marked RW are placed in the lower part of map_. Others are
	// inserted at the end. They should meet.
	// (Except for colors marked RO, the color information will
	//  be garbage).
	for(i=0; i< (unsigned long)MaxColors; i++)
	{ int indx;
	  AvColortableEntry::STATE state;
		if(current_[i].pad == RDWR)
		{	indx = rwi++;		// Use RW index.
			state = AvColortableEntry::RW;
		}
		else
		{	indx = --uai;		// Use index for RW/UA.
			if(current_[i].pad == RDONLY)
				state = AvColortableEntry::RO;
			else
				state = AvColortableEntry::UA;
		}
		colors_[i].setIndex(indx);
		colors_[i].setPixel(current_[i].pixel);
		colors_[i].store(current_[i].red*scl,
				   current_[i].green*scl,
				   current_[i].blue*scl);
		colors_[i].setFlags(0);	// reset to not dirty.
		colors_[i].setState(state);
		map_[indx] = (unsigned short)i;
	}

#if 0
if(! ReadOnlyRoot)
	compareMaps(cout, system, maxRootColors, current_, MaxColors);


cerr << "rwi = " << rwi << " uai = " << uai << " MaxColors = "
     << MaxColors << " available colors = " << rwi << endl;
cerr << "nRW = " << nRW << " nRO = " << nRO << " nUA = " << nUA << endl;
#endif

	rwcells_ = rwi;
	////////////////////////////////////////////////////////////////

	delete [] pixels;
	delete [] system;

}

// Called from AvPalette to allocate a named color and return its values.
Boolean AvXPalette::allocNamedColor_(const char *colorname,
		unsigned long &pixel, float &r, float &g, float &b)
{
Status	err;
XColor	color, dmy;
//unsigned long p;

	if((colorname == NULL) || (strlen(colorname)==0))
		return FALSE;

	err = XAllocNamedColor(dpy_, cmap_, colorname, &color, &dmy);
	// If err is 0, the XAlloc failed, so nothing got set.
	if( err == 0)
		return FALSE;

	pixel = color.pixel;
	r = colorToFloat(color.red);
	g = colorToFloat(color.green);
	b = colorToFloat(color.blue);
	return TRUE;
}

// Store one row of pixels into the XImage doing any necessary magnification.
static inline void storeRowX(XImage *image, const unsigned long *pixels,
			     const int npixels,
			     const int xinc, const int yinc, const int y0)
{int x, i;
 register int yend = y0-yinc;
 register int xend = xinc;

	for( x=0, i=0; i < npixels; i++, x+=xinc, xend+=xinc)
	{ register unsigned long p = *pixels++;

		// Do pixel replication in X & Y directions.
		// (Note that X11 Y(0) is on bottom, ours is on top.
// Theoretically, XPutPixel should be locked. However, on the SGI, doing so
// slows down the program to the point where it seems hung while not locking
// it seems to work just fine.
//		AvApp::reserveX();
			for(register int y=y0; y>yend; y--)
			{   for(register int c=x; c<xend; c++)
				XPutPixel(image, c, y, p);
			}
//		AvApp::releaseX();
	}
}

void AvXPalette::scaleFToX(const float *in,
		   const int iWidth, const int iHeight,
		   XImage *out, const int y0, const int yscl,
		   const float Min, const float Max,
		   const unsigned long under, const unsigned long over,
		   const unsigned long blank, const float blankval)
{
const int oWidth=out->width;
//const int oHeight = out->height;
float scl;
int	ncolors, yout=y0;
float min=Min, max=Max;
unsigned long p;
unsigned long undercolor=under, overcolor=over;
unsigned long blankcolor = blank;
const int xrpt = (oWidth >= iWidth) ? oWidth/iWidth : 1;
const int yrpt =  (yscl >= 0) ? yscl : 1;

	if((in == NULL) || (out==NULL))
		return;
	if(min > max)
	{ float tmp = min;
		min = max;
		max = tmp;
	}

	ncolors = availableColors();

	virtualToPhysical(undercolor, undercolor);
	virtualToPhysical(overcolor, overcolor);
	virtualToPhysical(blankcolor, blankcolor);

	// Will scale data to range of (0..ncolors-1);
	if(min == max)
		scl = ncolors-1;
	else
		scl = (ncolors-1)/(max-min);

	// Get a mapping array to minimize virtualToPhysical calls.
	unsigned long *map = new unsigned long[ncolors];
	for(int i=0; i< ncolors; i++)
		map[i] = colors_[map_[i]].pixel();

	// Scale pixel. if out of bounds use the given color.
	// (Be careful not to try to assign a neg # to p).
	unsigned long *outbuf = new unsigned long[iWidth];
	for( int r=0; r < iHeight; r++, yout -= yrpt)
	{	unsigned long *optr = outbuf;
		// Scale one row.
		for(int c=0; c < iWidth; c++)
		{float	tmp, tmpp;

			tmpp = *in++;
			if( tmpp == blankval)
				p = blankcolor;
			else
			{	tmp = (tmpp - min)*scl;
				if(tmp < 0.0)
					p = undercolor;
				else
				{	p = (long)(tmp + 0.5);
					if( p >= (unsigned long)ncolors)
						p = overcolor;
					else
						p = map[p];
				}
			}

			*optr++ = (unsigned long) p;
		}

		// Place in output buffer.
		storeRowX(out, outbuf, iWidth, xrpt, yrpt, yout);
	 }
	delete [] map;
	delete [] outbuf;
}

// Convert floating point color component in the range (0..1) to
// unsigned short in the range 0.. 65535.
static inline unsigned short f2us(const float v)
{static const int SCL = 65535;
	return (unsigned short)(v*SCL);
}

static inline void f2us3(const float rf, const float gf, const float bf,
			 unsigned short &rus, unsigned short &gus,
			 unsigned short &bus)
{
	rus = f2us(rf);
	gus = f2us(gf);
	bus = f2us(bf);
}

// Convert floating point data to unsigned short color components.
// Data are taken from a rectangular region in in and placed sequentially
// in the output buffer.
// in		- Input buffer. Points to start of subregion to convert.
// inWidth	- Width of entire buffer (no subregion). Eg. the offset
//		  from a data point in one row to the data point below it
//		  in the next row.
// ncols, nrows	- # of rows and columns to scale.
// rgb		- Output buffer. It length must be 3 times the number of
//		  data points converted.
// Min, Max	- Data min & max.
// under, over,	- Virtual pixel values for underrange, overrange and blanking.
// blankcolor
// blankval	- Value to use for blanked data.
void AvXPalette::scaleFToRGB(const float *in,
			     const int inWidth,
			     const int ncols, const int nrows,
			     unsigned short *rgb,
			     const float Min, const float Max,
			     const unsigned long undercolor,
			     const unsigned long overcolor,
		 	     const unsigned long blankcolor,
			     const float blankval)
{
float scl;
int	ncolors;
float min=Min, max=Max;
unsigned short pr, pg, pb;
unsigned short ur, ug, ub, OR, og, ob, br, bg, bb;
unsigned short *red, *green, *blue;
float r, g, b;

	if((in == NULL) || (rgb == NULL))
		return;
	if((nrows <= 0) || (ncols <= 0))
		return;
	if(min > max)
	{ float tmp = min;
		min = max;
		max = tmp;
	}

	indexToRGB(undercolor, r, g, b);
	f2us3(r, g, b, ur, ug, ub);
	indexToRGB(overcolor, r, g, b);
	f2us3(r, g, b, OR, og, ob);
	indexToRGB(blankcolor, r, g, b);
	f2us3(r, g, b, br, bg, bb);

	ncolors = availableColors();
	// Will scale data to range of (0..ncolors-1);
	if(min == max)
		scl = ncolors-1;
	else
		scl = (ncolors-1)/(max-min);

	// Get mapping arrays to minimize virtualToPhysical calls.
	red = new unsigned short[ncolors];
	green = new unsigned short[ncolors];
	blue = new unsigned short[ncolors];
	for(int i=0; i< ncolors; i++)
	{	indexToRGB(i, r, g, b);
		red[i] = f2us(r);
		green[i] = f2us(g);
		blue[i] = f2us(b);
	}

	// Scale pixel. If out of bounds use the corresponding color.
	for( int row=0; row < nrows; row++)
	{	// Scale one row.
		const float *iptr=in;
		for(int c=0; c < ncols; c++)
		{float	tmp, tmpp;

			tmpp = *iptr++;
			if( tmpp == blankval) {
				pr = br;
				pg = bg;
				pb = bb;
			}
			else
			{	tmp = (tmpp - min)*scl;
				if(tmp < 0.0) {
					pr = ur;
					pg = ug;
					pb = ub;
				}
				else
				{	int indx = (int)(tmp + 0.5);
					if( indx >= ncolors) {
						pr = OR;
						pg = og;
						pb = ob;
					}
					else {
						pr = red[indx];
						pg = green[indx];
						pb = blue[indx];
					}
				}
			}

			*rgb++ = pr;
			*rgb++ = pg;
			*rgb++ = pb;
		}

		in += inWidth;
	 }

	delete [] red;
	delete [] green;
	delete [] blue;
}
