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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvXCanvases.h,v 19.0 2003/07/16 05:46:52 aips2adm Exp $
//
// $Log: AvXCanvases.h,v $
// Revision 19.0  2003/07/16 05:46:52  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:37  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/03 21:50:43  hravlin
// Copyright.
//
// Revision 17.0  2001/11/12 19:42:05  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:14  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:04  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:21:49  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:07:53  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:38  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:00  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:51  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:32  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:25:35  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:36  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/02/05 18:06:10  hr
// Added getPixmap().
//
// Revision 7.2  1996/12/12 07:46:23  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.8  1996/06/18  18:43:34  pixton
//  Copyright Update
//
//  Revision 1.7  1996/01/18  21:57:26  hr
//  Changed setDisplayObject to return TRUE if new object actually replaced
//  the old one.
//
//  Revision 1.6  1995/11/13  22:06:26  hr
//  Added setDisplayObject(). Moved vps_ from AvXWindow to AvCanvas.
//
//  Revision 1.5  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/04/24  18:44:01  hr
//  Changed the way pixmap caching is handled.
//
//  Revision 1.3  1995/04/05  20:07:10  hr
//  Added pixmap caching
//
//  Revision 1.2  1995/03/29  21:34:43  hr
//  Remove Graphic context, add drawcolor()
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvCanvass.h
The two XCanvas objects.
*/

#ifndef AvCanvass_h
#define AvCanvass_h

#include <X11/Intrinsic.h>
#include "AvXCanvas.h"
class AvXPalette;
#include "AvViewportSetter.h"

class AvXPixmap : public AvXCanvas {
  public:
	 // Palette is needed to scale data.
	AvXPixmap(Widget reference, AvXPalette *pal,
		unsigned int width, unsigned int height,
		unsigned int depth=0);	// depth=0 -> use reference's.
	~AvXPixmap();
	virtual Drawable drawable();
	// Return a pointer to the pixmap.
	Pixmap pixmap()const;
	// Also returns a pointer to the pixmap, but also sets a flag so
	// the pixmap is not freed when AvXPixmap is deleted. Use this if
	// the pixmap will be destroyed by some other object (ie a widget).
	Pixmap getPixmap();
  private:
	Drawable pm_;
	Boolean	pixmapReleased_;	// If set, don't delete pm_.
};

class AvXWindow : public AvXCanvas {
  public:
	AvXWindow(const char *name, Widget parent, AvXPalette *,
		Arg *args=NULL, int nargs=0);
	~AvXWindow();
	virtual Drawable drawable();
	// These are only temporary.
	void	getWidthHeight();
	void	getWidthHeight(XEvent *);
	inline Widget widget() {return wid_;}
	// Replace current object, but don't display.
	// Returns TRUE if the new object is different from the old.
	// Returns FALSE and ignores the request if the objects are the same.
	virtual Boolean setDisplayObject(AvGraphics2D * = NULL);
	virtual void show(AvGraphics2D * = NULL);
	virtual void show(AvGraphics2D &);
	virtual void update();		// Redraw window.
	AvDisplayList *displayList();
  private:
	AvXPixmap *pm_;		// Draw into this then copy to window.
	Widget wid_;
	Boolean	setcmap_;
  private:
	virtual void callCallbacks(AvEvent &, int mask);
	void init(const char *name, Widget parent, Arg *args=NULL,
		  int nargs=0);
	// Called to handle window events.
	void	inputCallback(Widget, XtPointer, XtPointer);
	void	exposeCallback(Widget, XtPointer, XtPointer);
	void	resizeCallback(Widget, XtPointer, XtPointer);
	void	motionCallback(Widget, XMotionEvent *);
	void	enterHandler(Widget, XEvent *);
	// Interface between <>Callback and X<>Callback.
	static void XinputCallback(Widget, XtPointer, XtPointer);
	static void XexposeCallback(Widget, XtPointer, XtPointer);
	static void XresizeCallback(Widget, XtPointer, XtPointer);
	static void XmotionHandler(Widget, XtPointer, XEvent *, Boolean *);
	static void XenterHandler(Widget, XtPointer, XEvent *, Boolean *);
};

#endif
