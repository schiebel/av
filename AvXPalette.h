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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvXPalette.h,v 19.0 2003/07/16 05:47:48 aips2adm Exp $
//
// $Log: AvXPalette.h,v $
// Revision 19.0  2003/07/16 05:47:48  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:24  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/03 20:08:08  hravlin
// Added scaling functions used by non X canvases (eg. OpenGL canvas).
//
// Revision 17.0  2001/11/12 19:42:53  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:59  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:11  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:53  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.2  1999/10/25 18:51:36  hravlin
// Added scaleFToX().
//
// Revision 13.1  1999/09/15 20:57:10  hravlin
// *** empty log message ***
//
// Revision 13.0  1999/08/10 18:40:42  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:53  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:32  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:39  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/05/01 21:36:57  hr
// Removed reallocRW, changed reallocRO and renamed iobuf_ to obuf_.
//
// Revision 9.0  1997/08/25 21:30:30  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.2  1997/05/21 20:08:03  hr
// Added reallocRW() and reallocRO().
//
// Revision 8.1  1997/04/30 20:15:15  hr
// Renamed checkAllocated() to checkAllocated_().
// Added iobuf_.
//
// Revision 8.0  1997/02/20 03:16:33  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 00:13:11  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.3  1996/06/18  18:43:36  pixton
//  Copyright Update
//
//  Revision 1.2  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/*
Handle Colormap I/O to X colormaps.
*/

#ifndef AvXPalette_h
#define AvXPalette_h

#include "AvPalette.h"

class AvXPalette : public AvPalette {
  public:
	/*
	  If cmap is 0, create a colormap. If it is non 0,
	  it is used. (The colormap must match the visual).
	*/
	AvXPalette(Display *, Visual *, Colormap colormap=0, const int len=0);
	// AvPalette calls these to do I/O to the physical color table.
	// Copy dirty entries to physical cm.
	virtual void putColors();
	// Copy physical cm to internal. The force flag forces the write
	// if possible. Otherwise, it's up to the subclass.
	virtual void getColors(const Boolean force=False);

	inline Display *display() const { return dpy_;}
	inline Visual *visual() const { return visual_;}
	inline Colormap colormap()const { return cmap_;}
	// Merge system colormap and current colormap and write.
	// Usually only called after useSegment().
//	virtual void update();
	static inline float colorToFloat(const unsigned long v)
		{ return ((float )v)/65535.0;}
	static inline unsigned long floatToColor(const float v)
		{ return (unsigned long )(v*65535.0 + 0.5);}
	void scaleFToX(const float *in, 
		   const int iWidth, const int iHeight,
		   XImage *out, const int y0, const int yscl,
		   const float Min, const float Max,
		   const unsigned long under, const unsigned long over,
		   const unsigned long blank, const float blankval);
	void scaleFToRGB(const float *databin,
			 const int inWidth,
			 const int ncols, const int nrows,
			 unsigned short *rgbout,
			 const float Min, const float Max,
			 const unsigned long undercolor,
			 const unsigned long overcolor,
		 	 const unsigned long blankcolor,
			 const float blankval);
  protected:
	~AvXPalette();		// Use unref() instead.
	// Called from AvPalette to allocate a named color and return its
	// values.
	virtual Boolean allocNamedColor_(const char *colorname,
			unsigned long &pixel, float &r, float &g, float &b);
	// Query server for what colors are allocated.
	virtual void checkAllocated_();
	Display	*dpy_;
	Visual	*visual_;
	Colormap cmap_;			// X colormap
  private:
	// Allocates all unallocated cells in a colormap RW and returns
	// a pointer to an array holding the indexes of the allocated
	// cells. (Use delete [] to free). If free is set, the cells will
	// be freed before return ( the array then lists the allocatable
	// cells).
	static unsigned long *allocRWCells(Display *dpy, Colormap cmap,
				   const unsigned int maplength,
				   unsigned int &numallocated,
				   const Bool free);
	static unsigned long *CheckAllocated(  Display *dpy,
				Visual *visual,
				Colormap map,
				unsigned int &numcells,
				unsigned int &cellsfree);
	void freeColor(XColor &color);	// Free an allocated color.

	// Change a color entry from RW to RO (or LOCKED if a different color
	// was allocated) and sets the new state in .pad.
	// Returns TRUE if successful.
	Boolean reallocRO(XColor &xc);

	void initialize(Display *dpy, Visual *visual, Colormap cmap);
	XColor	*current_;	// Current copy of our colormap.
	Boolean	checked_;
	// Stuff for decomposed (eg TrueColor) colormaps.
	unsigned short red_shift_, green_shift_, blue_shift_;
	unsigned short red_max_, green_max_, blue_max_;
	unsigned long red_mask_, green_mask_, blue_mask_;
};

#endif
