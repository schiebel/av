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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvCanvas.h,v 19.0 2003/07/16 05:46:39 aips2adm Exp $
//
// $Log: AvCanvas.h,v $
// Revision 19.0  2003/07/16 05:46:39  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:25  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/03 20:26:08  hravlin
// Added flush() and wait().
//
// Revision 17.0  2001/11/12 19:41:54  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:04  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:19  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:07:40  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:23  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:35  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:13  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:51:48  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1997/12/17 21:50:16  hr
// Changes to remove warnings from MIPSpro.7x compiler.
//
// Revision 9.0  1997/08/25 21:24:24  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:14:54  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 05:44:00  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.10  1996/06/18  18:42:40  pixton
//  Copyright Update
//
//  Revision 1.9  1996/01/18  21:57:26  hr
//  Changed setDisplayObject to return TRUE if new object actually replaced
//  the old one.
//
//  Revision 1.8  1995/11/13  22:06:26  hr
//  Added setDisplayObject(). Moved vps_ from AvXWindow to AvCanvas.
//
//  Revision 1.7  1995/10/07  15:10:57  hr
//  Removed drawRectangle function.
//
//  Revision 1.6  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.6  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.6  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/06/22  18:54:33  hr
//  Added support for AvGraphicContext.
//
//  Revision 1.4  1995/05/24  14:03:59  hr
//  Added convertRasterScale.
//
//  Revision 1.3  1995/04/07  20:57:18  hr
//  Subclass AvCanvas from AvResource rather than AvXCanvas
//
//  Revision 1.2  1995/03/29  21:34:43  hr
//  Remove Graphic context, add drawcolor()
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvCanvas.h
Defines 'drawable'.
*/

#ifndef AvCanvas_h
#define AvCanvas_h

#include <AvResource.h>
#include <AvRaster.h>
#include <AvEvent.h>
#include <AvViewport.h>
//#include <AvGraphicContext.h>

class AvCanvasImpl;
class AvPolylineCmd;
class AvDisplayList;
class AvPalette;
class AvGraphicContext;
class AvViewportSetter;

class AvCanvasState {
  friend class AvCanvas;
  public:
	AvCanvasState(AvGraphicContext *gc=NULL);
	~AvCanvasState();
	AvCanvasState(const AvCanvasState &);
	AvCanvasState& operator=(const AvCanvasState &);
	AvViewport *viewport(){ return &vp_;}
	void setGraphicContext(AvGraphicContext *);
	AvGraphicContext *graphicContext(){return gc_;}
	AvGraphicContext *graphicContext()const{return gc_;}
  private:
	void copy(const AvCanvasState &);
	AvViewport	vp_;
	AvGraphicContext *gc_;	// Graphic context.
};

class AvCanvas : public AvResource {
  public:
	virtual void show(AvGraphics2D&);
	virtual void show(AvGraphics2D*);
	// Replace current object, but don't display.
	// Returns TRUE if the new object is different from the old.
	// Returns FALSE and ignores the request if the objects are the same.
	virtual Boolean setDisplayObject(AvGraphics2D * = NULL);
	virtual void drawRaster(AvRaster &img,
			int srcx=0, int srcy=0, int dstx=0, int dsty=0,
			unsigned int width=0, unsigned int height=0);

	virtual void drawRaster(AvRaster *img);
	virtual void drawVector(AvPolylineCmd *cmd);
	virtual void setViewport(AvViewport &vp);
	virtual void getViewport(AvViewport &vp);
	AvViewport *viewport();
	virtual void pushState();
	virtual void popState();
	/* Sets flag indicating whether it is necessary to change complete
	   state when pushing/popping, etc. Some Canvas, eg. XCanvas, do
	   extra operations such as changing clipping planes which don't need
	   to be done when bounding box is being computed.
	*/
	virtual void changeFullState(Boolean);
	virtual Boolean changeFullState();	// Returns current value.
	virtual void clear(int x=0, int y=0,
			unsigned int width=0, unsigned int height=0);
	// return scaled value for a dimension.
	virtual unsigned int scaleSize(const AvDimension,
						const unsigned int val);
	// Compute the bounding box for a display list.
	virtual void getBoundingBox(AvGraphics2D *list, AvAllotment &r);
	// Return the pick path lists for the last pick event. NULL if none.
	virtual AvPathList *getPickPaths();
	// return the current display list.
	virtual AvDisplayList *displayList();
	//	Event related functions.
	virtual void addCallback(AvCallback *);
	virtual void removeCallback(AvCallback *);
	void setGrabber(AvGraphics2D *gbr); // Send all events to object.
	AvGraphics2D *grabber();	// Return object grabbing events.
	void releaseGrabber();		// Stop grabbing events.
	void grabEvents(AvCallback *cb);// Send all events to cb.
	AvCallback *eventGrabber(); // Callback grabbing events.
	void releaseEvents();		// Stop grabbing events.
	// Return pointer to the graphic context.
	AvGraphicContext *getGraphicContext()const;
	// Return pointer to palette.
	virtual AvPalette *palette()const;
	// There is no 'setpalette' since each subclass needs a specific
	// kind. So, there will need to be 'AvXCanvas::setXPalette', etc.
	// color is a virtual color value. (See AvPalette).
	virtual void setDrawColor(const unsigned long color);
	virtual void setDrawColor(const char *colorname);
	unsigned long getDrawColor()const;
	// Convert generic rasterscale (float) to what the canvas actually
	// uses. (eg. XCanvas rounds to nearest int).
	virtual float convertRasterScale(const float scl)const;

	// Flush any internal buffers.
	virtual void flush();
	// Flush buffers, then wait for all operations to finish.
	virtual void wait();
	// When set, show() does a wait.
	Boolean sync()const{return sync_;}
	void sync(const Boolean sync){sync_ = sync;}
  protected:
	virtual void handleEvent(AvCanvas &c, AvGraphics2D *dl,
				AvEvent &, int mask);
	virtual void callCallbacks(AvEvent &, int mask);
	// Change Initial setting of viewport.
	virtual void resetViewport(int x, int y,
					unsigned int w, unsigned int h);
	// This is a base class only.
	AvCanvas();
	~AvCanvas();
  protected:
	AvCanvasState	state_;
	Boolean		changeFullState_;
	AvCanvasImpl	*impl_;
	AvPathList	*pickpaths_;
	AvEvent		*pickevent_;
	AvGraphics2D	*grabber_;	// Object grabbing events.
	AvViewportSetter *vps_;		// setDisplayObject() sticks the
					// object here.
	Boolean		sync_;		// Should show do a wait()?
};

#endif
