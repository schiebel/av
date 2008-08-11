//# Copyright (C) 2002 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvGLCanvas.h,v 19.0 2003/07/16 05:48:16 aips2adm Exp $
//
// $Log: AvGLCanvas.h,v $
// Revision 19.0  2003/07/16 05:48:16  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:48  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/03 21:05:07  hravlin
// Initial checkin.
//
//---------------------------------------------------------------------------

/* AvGLCanvas.h
Defines 'Canvas' for OpenGL and supporting wiregl.

*/

#ifndef AvGLCanvas_h
#define AvGLCanvas_h


#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <AvGraphics2D.h>
#include <AvCanvas.h>
#include <AvXPalette.h>
#include <AvGraphicContext.h>
#include <GL/glx.h>

class	AvTextureCache;
class	AvGLCanvas;
class	AvGLContext;

class AvGLGraphicContext : public AvGraphicContext {
  public:
	AvGLGraphicContext(AvGLCanvas *can);
	AvGLGraphicContext(const AvGraphicContext&);
	virtual AvGraphicContext& operator=(const AvGraphicContext &);
	virtual AvGLGraphicContext& operator=(const AvGLGraphicContext &);
	~AvGLGraphicContext();

	virtual void setDrawMode(const AvGraphicContext::DrawMode d);
	virtual void setLineWidth(const int lw);
	virtual void setLineStyle(const LineStyle ls);
	virtual void setFont(const Font f);
	virtual void setForeground(const unsigned long);
	virtual void setBackground(const unsigned long);
  private:
	virtual void copy(const AvGraphicContext &);
	virtual void copy(const AvGLGraphicContext &);
	virtual void update(const int mask=0); // force xgc_ to match settings.
  private:
	AvGLCanvas *canvas_;
};

class AvTextureObject;
class AvTextureObjects;

class AvGLCanvas : public AvCanvas {
  public:
	// Ref provides info on Display, Visual, etc.
	AvGLCanvas(const char *name, Widget ref, AvXPalette *pal=NULL,
		   Arg *arglst=NULL, int nargs=0);
	virtual unsigned int width();
	virtual unsigned int height();
	virtual void drawRaster(AvRaster &img,
			int srcx=0, int srcy=0, int dstx=0, int dsty=0,
			unsigned int width=0, unsigned int height=0);
	virtual void drawRaster(AvRaster *img);
	virtual void drawVector(AvPolylineCmd *cmd);

	virtual void show(AvGraphics2D * = NULL);
	virtual void show(AvGraphics2D &);
	virtual void update();

	// Used to draw a complete scaled raster into a drawable.
	virtual void imageRaster(AvRaster &img, const int dstx, const int dsty,
		const float xscale, const float yscale);
	// Create textures containing raster image.
	// If tobjs isn't NULL, just update position and scale info.
	AvTextureObjects *textureRaster(AvRaster &img,
			  		const int dstx, const int dsty,
					const float xscale,
					const float yscale,
					AvTextureObjects *tobjs=NULL);
	// Scale a width or height value using current raster scaling.
	virtual unsigned int scaleSize(const AvDimension d,
					const unsigned int val);

	virtual Drawable drawable();	// The X drawable used.
	virtual Display *display();
	virtual int depth();
	inline Widget widget() { return wid_;}
	// Convert generic rasterscale (float) to what the canvas actually
	// uses. (eg. XCanvas rounds to nearest int).
	virtual float convertRasterScale(const float scl)const;
	// Global version of above.
	static float  convertScale(const float scl);
	virtual void clear(int x=0, int y=0,
			unsigned int width=0, unsigned int height=0);
	// Clear the X drawing area.
	virtual void clearXarea(const int x=0, const int y=0,
			const unsigned int width=0,
			const unsigned int height=0);
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
	////////////////////////////////////
	/* Routines for raster caching.
	*/
	virtual void setRasterCacheParameters(const Boolean enable,
					      const int size=-1);
	virtual void getRasterCacheParameters(Boolean &enable, int &size);
	Boolean cachingRasters()const;
	void setRasterCache(AvTextureCache *);
	// Flush cache.
	void flushRasterCache();
	// Flush any internal buffers.
	virtual void flush();
	// Flush buffers, then wait for all operations to finish.
	virtual void wait();
	////////////////////////////////////
	Widget referenceWidget()const {return ref_;}
	// Virtual values for white & black.
	unsigned long whitePixel()const{return whitePixel_;}
	unsigned long blackPixel()const{return blackPixel_;}
	void makeCurrent();
	static void initGLReservation();
	void drawPixels(const float *inbuf,
			const int iWidth, const int iHeight,
			const int nrows, const int ncols,
			GLushort *obuf,
			const int xscl, const int yscl,
			const float mn, const float mx,
			const unsigned long undercolor,
			const unsigned long overcolor,
			const unsigned long blankcolor,
			const float blankvalue,
			const int dstx, const int dsty);
	// Returns # of textures in X & Y needed to cover drawing area.
	void numTextures(const int width, const int height, int &nx, int &ny);
	// Prints any OGL error messages and return # of errors.
	static int glcheck(const char* msg=NULL);
	static int maxTextureSize() {return maxTextureSize_;}
	// Lock/unlock for GL rtns. Empty if no thread support.
	static void reserveGL();
	static void releaseGL();
	static int externalDrawingArea();	// Using WireGL?
  protected:
	~AvGLCanvas();
	unsigned int width_;
	unsigned int height_;
	unsigned int depth_;
	unsigned long whitePixel_;	// Virtual indexes.
	unsigned long blackPixel_;
	Widget	ref_;
	Widget	wid_;		// Drawing widget.
	Visual *visual_;	// Picked up from ref_.
	XVisualInfo *visualInfo_;
	GC	xbgGC_;		// For clear.
	AvXPalette *palette_;
  private:
	// Variables used to track how long an image takes to display.
	float		rasterTime_;	// Time in seconds.
	unsigned long	rasterBytes_;
	// OpenGL stuff.
	Boolean		doubleBuffered_;
	Boolean		RGBVisual_;
	AvGLContext	*glcontext_;
	Boolean		exposedOnce_;
	static GLint	maxTextureSize_;	// Max texture size (from GL).
	static int	externaldrawingarea_;	// Using WireGL?
	AvTextureCache	*texturecache_;
	int		busy_;
#if defined(PTHREADS)
	static void *threadScale(void *arg);
#endif
	// Returns some information needed to initialize canvas.
	static void glInfo(Display *dpy, XVisualInfo *vi, GLXContext ctx,
			   GLint &width, GLint &height, GLint &maxtexturesize,
			   int &externaldrawingarea);
	void resized();
	void expose();
	///// Callbacks.
	static void drawcb(Widget, XtPointer, XtPointer);
	static void exposecb(Widget, XtPointer, XtPointer);
	static void resizecb(Widget, XtPointer, XtPointer);
};
#endif
