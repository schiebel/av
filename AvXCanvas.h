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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvXCanvas.h,v 19.0 2003/07/16 05:47:01 aips2adm Exp $
//
// $Log: AvXCanvas.h,v $
// Revision 19.0  2003/07/16 05:47:01  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:45  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/03 21:43:06  hravlin
// Changes due to new AvRasterCache class and additions of flush() and wait().
//
// Revision 17.0  2001/11/12 19:42:14  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:22  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:36  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:05  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.3  1999/10/25 18:49:30  hravlin
// Removed scale().
//
// Revision 13.2  1999/09/28 19:15:25  hravlin
// Added variables used in determining how long an image takes to display.
//
// Revision 13.1  1999/09/15 21:01:44  hravlin
// Added flushPixmapCache() & white/blackPixel().
//
// Revision 13.0  1999/08/10 18:39:50  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:21  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:21  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:08  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.2  1998/06/09 21:50:36  hr
// Changed arguments to scale to be mostly const and unsigned long for colors.
//
// Revision 9.1  1997/09/15 16:20:50  hr
// Conversion of line width from an enum to an int.
//
// Revision 9.0  1997/08/25 21:26:31  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:09  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/02/05 18:04:15  hr
// Added setClearColor().
//
// Revision 7.2  1996/12/12 07:45:57  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.13  1996/06/18  18:43:33  pixton
//  Copyright Update
//
//  Revision 1.12  1996/02/23  17:34:55  hr
//  Added clipRect_;
//
//  Revision 1.11  1995/10/07  15:14:33  hr
//  Removed drawRectangle function.
//
//  Revision 1.10  1995/10/04  21:13:25  hr
//  XCanvas now gets blank value from the raster.
//
//  Revision 1.9  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.9  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.9  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.8  1995/06/22  18:57:33  hr
//  Added AvXGraphicContext class.
//
//  Revision 1.7  1995/06/05  19:26:39  hr
//  Added dstx/y to imageRaster.
//
//  Revision 1.6  1995/05/24  14:03:59  hr
//  Added convertRasterScale.
//
//  Revision 1.5  1995/04/24  18:41:54  hr
//  Began using raster:{begin,end}Access and added scaleSize.
//
//  Revision 1.4  1995/04/07  20:57:18  hr
//  Subclass AvCanvas from AvResource rather than AvXCanvas
//
//  Revision 1.3  1995/04/05  20:05:17  hr
//  Added pixmap caching
//  AvXCanvases are now subclassed from AvResource
//
//  Revision 1.2  1995/03/29  21:34:43  hr
//  Remove Graphic context, add drawcolor()
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvXCanvas.h
Defines 'Canvas' for X.
*/

#ifndef AvXCanvas_h
#define AvXCanvas_h


#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <AvGraphics2D.h>
#include <AvCanvas.h>
#include <AvXPalette.h>
#include <AvGraphicContext.h>

class	AvPixmapCache;
class	AvXCanvas;

class AvXGraphicContext : public AvGraphicContext {
  public:
	AvXGraphicContext(AvXCanvas *can, GC gc);
	AvXGraphicContext(const AvGraphicContext&);
	virtual AvGraphicContext& operator=(const AvGraphicContext &);
	virtual AvXGraphicContext& operator=(const AvXGraphicContext &);
	~AvXGraphicContext();

	virtual void setDrawMode(const AvGraphicContext::DrawMode d);
	virtual void setLineWidth(const int lw);
	virtual void setLineStyle(const LineStyle ls);
	virtual void setFont(const Font f);
	virtual void setForeground(const unsigned long);
	virtual void setBackground(const unsigned long);

	GC	gc();
	void	setGC(GC);
  private:
	virtual void copy(const AvGraphicContext &);
	virtual void copy(const AvXGraphicContext &);
	virtual void update(const int mask=0); // force xgc_ to match settings.
  private:
	GC	gc_;
	AvXCanvas *canvas_;
};

class AvXCanvas : public AvCanvas {
  public:
	// Ref provides info on Display, Visual, etc.
	AvXCanvas(Widget ref, AvXPalette *pal=NULL);
	virtual unsigned int width();
	virtual unsigned int height();
	virtual void drawRaster(AvRaster &img,
			int srcx=0, int srcy=0, int dstx=0, int dsty=0,
			unsigned int width=0, unsigned int height=0);
	virtual void drawRaster(AvRaster *img);
	virtual void drawVector(AvPolylineCmd *cmd);

	// Used to draw a complete scaled raster into a drawable.
	virtual void imageRaster(AvRaster &img, const int dstx, const int dsty,
		const float xscale, const float yscale);
	// Scale a width or height value using current raster scaling.
	virtual unsigned int scaleSize(const AvDimension d,
					const unsigned int val);
	/* Copy from one to another. */
	virtual void copyRegion(AvXCanvas &src, AvXCanvas &dst,
			int srcx=0, int srcy=0,
			// width/height of 0 means what's available.
			unsigned int width=0, unsigned int height=0,
			int dstx=0, int dsty=0);
	virtual Drawable drawable();	// The X drawable used.
	virtual Display *display();
	virtual int depth();
	// Convert generic rasterscale (float) to what the canvas actually
	// uses. (eg. XCanvas rounds to nearest int).
	virtual float convertRasterScale(const float scl)const;
	// Global version of above.
	static float  convertScale(const float scl);
	virtual void clear(int x=0, int y=0,
			unsigned int width=0, unsigned int height=0);
	// Set the color used to clear drawables. If physical is TRUE,
	// ci is a physical color index, otherwise it is a virtual color.
	// (We should be using the AvGraphicContext, but that needs work).
	void setClearColor(const unsigned long ci,
			   const Boolean physical=TRUE);
	virtual void setClipPlane(	const AvAllotment &offset,
					const AvAllotment &size);
	// If NULL, use the current Canvas value.
	virtual void setClipPlane(AvViewport *vp=NULL);
	virtual void setClipPlane(AvViewport &vp);
	virtual void popState();
	virtual void setViewport(AvViewport &vp);
	AvPalette *palette()const;
	AvXPalette *getXPalette()const;
	void setXPalette(AvXPalette *pal);
	void setDrawColor(const unsigned long color);
	void setDrawColor(const char *colorname);
	/* Turn pixmap caching on or off. If size is >=0, any old
	  cache is removed and replaced with a cache of size size.
	  Setting n to 0, disables caching. Setting
	  enable to false also disables caching, but retains the cache.
	*/
	virtual void setRasterCacheParameters(const Boolean enable,
							const int size=-1);
	virtual void getRasterCacheParameters(Boolean &enable, int &size);
	Boolean cachingRasters()const;
	void setRasterCache(AvPixmapCache *);
	AvPixmapCache *getRasterCache()const{ return pmcache_;}
	// Flush any cached pixmaps.
	void flushRasterCache();
	Widget referenceWidget()const {return ref_;}
	// Virtual values for white & black.
	unsigned long whitePixel()const{return whitePixel_;}
	unsigned long blackPixel()const{return blackPixel_;}
	virtual void flush();
	virtual void wait();
  protected:
	~AvXCanvas();
	unsigned int width_;
	unsigned int height_;
	unsigned int depth_;
	unsigned long whitePixel_;	// Virtual indexes.
	unsigned long blackPixel_;
	Widget ref_;
	Visual *visual_;	// Picked up from ref_.
	GC	xgc_;
	GC	xbgGC_;		// For clear.
	AvXPalette *palette_;
	AvPixmapCache	*pmcache_;
  private:
	XRectangle	clipRect_;	// VP clipping rectangle.
	// Variables used to track how long an image takes to display.
	float		rasterTime_;	// Time in seconds.
	unsigned long	rasterBytes_;
};
#endif
