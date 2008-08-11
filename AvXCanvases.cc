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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvXCanvases.cc,v 19.0 2003/07/16 05:46:58 aips2adm Exp $
//
// $Log: AvXCanvases.cc,v $
// Revision 19.0  2003/07/16 05:46:58  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:43  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/03 21:48:54  hravlin
// Added check for synch() and busy check to show().
//
// Revision 17.0  2001/11/12 19:42:12  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:20  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:27  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:21:37  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:08:02  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/09/15 21:00:33  hravlin
// Use AvApp's reference window instead of root's.
//
// Revision 13.0  1999/08/10 18:39:46  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:15  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:13  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:57  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:26:15  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:59  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.4  1997/02/09 00:58:56  hr
// Fixed signed/unsigned comparisions.
//
// Revision 7.3  1997/02/05 18:05:20  hr
// Added getPixmap().
//
// Revision 7.2  1996/12/12 08:19:36  droberts
// Final update from monet archive.
//
// Revision 1.2  1996/11/11 16:08:52  hr
// In show(), added a call to clear in case width is reduced since independent
// scaling of x axis might leave garbage.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.15  1996/06/18  18:46:46  pixton
//  Copyright Update
//
//  Revision 1.14  1996/04/09  18:44:55  hr
//  ~AvXWindow() was deleting a widget it shouldn't.
//
//  Revision 1.13  1996/02/23  17:18:44  hr
//  The backing pixmap has to be discarded whenever a different size is
//  needed since the vpY to XY conversion uses the pixmap's height.
//
//  Revision 1.12  1996/01/18  21:56:09  hr
//  Changed setDisplayObject to ignore request if old and new objects are
//  the same. Returns TRUE if there was a replacement done.
//
//  Revision 1.11  1995/12/07  22:38:49  hr
//  Removed a couple of unused variables from setDisplayObject.
//
//  Revision 1.10  1995/11/13  22:05:14  hr
//  Added setDisplayObject(). Moved handling of display list (vps_)
//  to AvCanvas.
//
//  Revision 1.9  1995/10/27  19:10:01  hr
//  Added a bunch of resources to widgets using our private colormap to try
//  and avoid warning about not being able to allocate a color.
//
//  Revision 1.8  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.8  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.7  1995/05/24  15:02:23  hr
//  Turned off reason_name debugging routine.
//
//  Revision 1.6  1995/05/11  21:50:37  hr
//  Added WindowResize and WindowExpose to list of events that call the
//  callback list.
//
//  Revision 1.5  1995/04/24  18:32:22  hr
//  Changed the way pixmap caching was handled.
//
//  Revision 1.4  1995/04/14  19:22:35  hr
//  Calling show with the same object results in a redisplay rather
//  than replacing the old object.
//
//  Revision 1.3  1995/04/05  20:03:46  hr
//  Added pixmap caching
//
//  Revision 1.2  1995/03/29  21:30:23  hr
//  Remove Graphic context, add drawcolor()
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvCanvases.C

The two subclasses of AvCanvas.
*/
#include <stdio.h>
#include <memory.h>
#include <Xm/DrawingA.h>
#include "AvWidgetMisc.h"
#include "AvXCanvases.h"
#include "AvViewportSetter.h"
#include "AvPathList.h"
#include "AvXPalette.h"
#include "AvPixmapCache.h"
#include "AvApp.h"

AvXPixmap::AvXPixmap(Widget ref, AvXPalette *pal,
		unsigned int width, unsigned int height,
		unsigned int depth) : AvXCanvas(ref, pal)
{
Window	w;

	// Since it is expected that AvXPixmap always succeed, force valid
	// size.
	width_ = (width > 0) ? width : 1;
	height_ = (height > 0) ? height : 1;
	depth_ = depth;
	resetViewport(0, 0, width_, height_);
	if(depth_ == 0)
		depth_ = widgetDepth(ref);	// Use default.

	w = XtWindow(ref);
	/* If the window is 0 the widget hasn't been realized yet.
	*/
	if(w == 0)
		w = AvApp::referenceWindow();

	pm_ = XCreatePixmap(XtDisplay(ref), w, width, height, depth_);
	pixmapReleased_ = FALSE;
	XGCValues	vals;
	vals.foreground = 2;
	vals.background = 2;
//	XChangeGC(XtDisplay(ref), xgc_, GCBackground, &vals);
	XChangeGC(XtDisplay(ref), xgc_, GCForeground, &vals);
	clear();
}

AvXPixmap::~AvXPixmap()
{
	if((pm_ != 0) && !pixmapReleased_)
		XFreePixmap(display(), pm_);
}

Drawable AvXPixmap::drawable() { return pm_;}
// Same as above, but casts drawable to the pixmap it is.
Pixmap AvXPixmap::pixmap()const { return (Pixmap) pm_;}

// Like pixmap() but sets the pixmapReleased flag.
Pixmap AvXPixmap::getPixmap()
{	pixmapReleased_ = TRUE;
	return (Pixmap) pm_;
}

#if 0
static int pparents(Widget wid)
{ Widget parent = XtParent(wid);
  Dimension	w,h;
  int cnt;

	if(wid == 0)
		return 0;

	XtVaGetValues(wid,
		XtNwidth, &w,
		XtNheight, &h,
		NULL);

	if(parent == 0)
		cnt = 0;
	else
		cnt = pparents(parent) + 1;

	for(int i=0; i< cnt; i++)
		printf(" ");
	printf("%s: W/H: %d/%d\n", XtName(wid), w, h);
	return cnt;
}
#endif

//**********************************************************
void AvXWindow::init(const char *name, Widget parent,
			Arg *arglst, int nargs)
{
Widget wid;
Arg *args;
int	i;
Colormap cm;

	i = nargs;
	args = new Arg[i+16];	// Plenty of room for my additions.
	if(nargs > 0)
		memcpy(args, arglst, sizeof(*arglst)*nargs);

	cm = getXPalette()->colormap();
	XtSetArg(args[i], XtNcolormap, cm); i++;

	/**** (internal note: ) **/
	// Doesn't cause the error one would expect.
	//	XtSetArg(args[i], XmNcolormap, 2); i++;
	/**** *******/

#if 0
	XtSetArg(args[i], XmNbackground,
			BlackPixelOfScreen(XtScreen(parent))); i++;
	/* These are just to help keep away messages like:
		Warning: Cannot allocate colormap entry for "gray"
	*/
	Pixel	dmy = WhitePixelOfScreen(XtScreen(parent));
	XtSetArg(args[i], XmNforeground, dmy); i++;
	XtSetArg(args[i], XmNborderWidth, 0);i++;
	XtSetArg(args[i], XmNborderColor, dmy); i++;
	XtSetArg(args[i], XmNbottomShadowColor, dmy); i++;
	XtSetArg(args[i], XmNhighlightColor, dmy); i++;
	XtSetArg(args[i], XmNtopShadowColor, dmy); i++;
#endif

	wid = XtCreateManagedWidget(name, xmDrawingAreaWidgetClass, parent,
		args, i);
	wid_ = wid;
	setcmap_ = FALSE;

	XtAddCallback( wid, XmNinputCallback,
		&AvXWindow::XinputCallback, (XtPointer) this);
	XtAddCallback( wid, XmNresizeCallback,
		&AvXWindow::XresizeCallback, (XtPointer) this);
	XtAddCallback( wid, XmNexposeCallback,
		&AvXWindow::XexposeCallback, (XtPointer) this);
	XtAddEventHandler(wid,
		Button1MotionMask | Button2MotionMask | Button3MotionMask,
		FALSE,
		&AvXWindow::XmotionHandler, (XtPointer) this);
	XtAddEventHandler(wid, EnterWindowMask | LeaveWindowMask, FALSE,
		&AvXWindow::XenterHandler, (XtPointer) this);
	delete [] args;
	pm_ = NULL;
}

AvXWindow::AvXWindow(const char *name, Widget parent, AvXPalette *pal,
	Arg *args, int nargs) :AvXCanvas(parent, pal)
{
	init(name, parent, args, nargs);
}

AvXWindow::~AvXWindow()
{
	if(pm_ != NULL)
		pm_->unref();
#if 0
	// Don't want to destroy this since it is now owned by parent.
	// Also, wid_ is used as the reference widget by the AvXPixmaps
	// created by caching. They don't go away until the AvXCanvas
	// part of the destructor gets executed.
	if(wid_ != NULL)
		XtDestroyWidget(wid_);
#endif
	wid_ = NULL;
}

void AvXWindow::getWidthHeight()
{
Arg	arg[4];
Dimension	w,h;
Position	x,y;
int		i;

	i = 0;
	XtSetArg(arg[i], XtNwidth, &w);	i++;
	XtSetArg(arg[i], XtNheight, &h); i++;
	XtSetArg(arg[i], XtNx, &x);	i++;
	XtSetArg(arg[i], XtNy, &y);	i++;
	XtGetValues(wid_, arg, i);
	width_ = w;
	height_ = h;
//printf("Wid: w=%d, h=%d, x/y= %d/%d\n", width_, height_, x, y);
	AvViewport *vp = state_.viewport();
	vp->x0(x);
	vp->y0(y);
	vp->width(width_);
	vp->height(height_);
}

void AvXWindow::getWidthHeight(XEvent *)
{
#if 0
int	x,y;
char	*name;

	if(ex == NULL)
		return;
#if 1
	if(ex->type == Expose)
	{XExposeEvent *e = (XExposeEvent *) ex;
		width_ = e->width;
		height_ = e->height;
		x = e->x;
		y = e->y;
		name = "Expose";
	}
	else
#endif
	if(ex->type == ResizeRequest)
	{XResizeRequestEvent *e = (XResizeRequestEvent *) ex;
		width_ = e->width;
		height_ = e->height;
		x = y = 0;
		name = "Resize";
	}
	else
	if(ex->type == ConfigureRequest)
	{XConfigureRequestEvent *e = (XConfigureRequestEvent *) ex;
		width_ = e->width;
		height_ = e->height;
		x = e->x;
		y = e->y;
		name = "Configure";
	}
//printf("XEV: w=%d, h=%d, x/y= %d/%d : %s\n", width_, height_, x, y, name);
#endif
getWidthHeight();
//printf("XC:getwidthheight: w=%d, h=%d\n", width_, height_);
}

void AvXWindow::XinputCallback(Widget w, XtPointer clientData, XtPointer data)
{
AvXWindow *obj = (AvXWindow *) clientData;

	obj->inputCallback(w, NULL, data);
}

void AvXWindow::XexposeCallback(Widget w, XtPointer clientData, XtPointer data)
{
AvXWindow *obj = (AvXWindow *) clientData;

	obj->exposeCallback(w, NULL, data);
}

void AvXWindow::XresizeCallback(Widget w, XtPointer clientData, XtPointer data)
//void AvXWindow::XresizeCallback(Widget , XtPointer , XtPointer )
{
AvXWindow *obj = (AvXWindow *) clientData;

	obj->resizeCallback(w, NULL, data);
}

void AvXWindow::XenterHandler(Widget w, XtPointer data, XEvent *e, Boolean *)
{
AvXWindow *obj = (AvXWindow *) data;

	obj->enterHandler(w, e);
}

void AvXWindow::XmotionHandler(Widget w, XtPointer data, XEvent *e, Boolean *)
{
AvXWindow *obj = (AvXWindow *) data;

	obj->motionCallback(w, (XMotionEvent *) e);
}

#if 0
static struct {
	int reason;
	char *name;
	} reasons[] = {
	{XmCR_NONE, "XmCR_NONE"},
	{XmCR_HELP, "XmCR_HELP"},
	{XmCR_VALUE_CHANGED, "XmCR_VALUE_CHANGED"},
	{XmCR_INCREMENT, "XmCR_INCREMENT"},
	{XmCR_DECREMENT, "XmCR_DECREMENT"},
	{XmCR_PAGE_INCREMENT, "XmCR_PAGE_INCREMENT"},
	{XmCR_PAGE_DECREMENT, "XmCR_PAGE_DECREMENT"},
	{XmCR_TO_TOP, "XmCR_TO_TOP"},
	{XmCR_TO_BOTTOM, "XmCR_TO_BOTTOM"},
	{XmCR_DRAG, "XmCR_DRAG"},
	{XmCR_ACTIVATE, "XmCR_ACTIVATE"},
	{XmCR_ARM, "XmCR_ARM"},
	{XmCR_DISARM, "XmCR_DISARM"},
	{XmCR_MAP, "XmCR_MAP"},
	{XmCR_UNMAP, "XmCR_UNMAP"},
	{XmCR_FOCUS, "XmCR_FOCUS"},
	{XmCR_LOSING_FOCUS, "XmCR_LOSING_FOCUS"},
	{XmCR_MODIFYING_TEXT_VALUE, "XmCR_MODIFYING_TEXT_VALUE"},
	{XmCR_MOVING_INSERT_CURSOR, "XmCR_MOVING_INSERT_CURSOR"},
	{XmCR_EXECUTE, "XmCR_EXECUTE"},
	{XmCR_SINGLE_SELECT, "XmCR_SINGLE_SELECT"},
	{XmCR_MULTIPLE_SELECT, "XmCR_MULTIPLE_SELECT"},
	{XmCR_EXTENDED_SELECT, "XmCR_EXTENDED_SELECT"},
	{XmCR_BROWSE_SELECT, "XmCR_BROWSE_SELECT"},
	{XmCR_DEFAULT_ACTION, "XmCR_DEFAULT_ACTION"},
	{XmCR_CLIPBOARD_DATA_REQUEST, "XmCR_CLIPBOARD_DATA_REQUEST"},
	{XmCR_CLIPBOARD_DATA_DELETE, "XmCR_CLIPBOARD_DATA_DELETE"},
	{XmCR_CASCADING, "XmCR_CASCADING"},
	{XmCR_OK, "XmCR_OK"},
	{XmCR_CANCEL, "XmCR_CANCEL"},
	{XmCR_APPLY, "XmCR_APPLY"},
	{XmCR_NO_MATCH, "XmCR_NO_MATCH"},
	{XmCR_COMMAND_ENTERED, "XmCR_COMMAND_ENTERED"},
	{XmCR_COMMAND_CHANGED, "XmCR_COMMAND_CHANGED"},
	{XmCR_EXPOSE, "XmCR_EXPOSE"},
	{XmCR_RESIZE, "XmCR_RESIZE"},
	{XmCR_INPUT, "XmCR_INPUT"},
	{XmCR_GAIN_PRIMARY, "XmCR_GAIN_PRIMARY"},
	{XmCR_LOSE_PRIMARY, "XmCR_LOSE_PRIMARY"},
	{XmCR_CREATE, "XmCR_CREATE"},
	{XmCR_TEAR_OFF_ACTIVATE, "XmCR_TEAR_OFF_ACTIVATE"},
	{XmCR_TEAR_OFF_DEACTIVATE, "XmCR_TEAR_OFF_DEACTIVATE"},
	{XmCR_OBSCURED_TRAVERSAL, "XmCR_OBSCURED_TRAVERSAL"}
	};

static char *reason_name(int reason)
{
int i;

	for(i = 0; i< sizeof(reasons)/sizeof(*reasons); i++)
		if(reason == reasons[i].reason)
			return reasons[i].name;

	return "Unknown";
}
#endif

static struct {
	int		xmask;
	AvKeyMask	svmask;
	} keyconvert[] = {
		{ ShiftMask, AvShiftMask},
		{ LockMask, AvLockMask},
		{ ControlMask, AvControlMask},
		{ Mod1Mask, AvMod1Mask},
		{ Mod2Mask, AvMod2Mask},
		{ Mod3Mask, AvMod3Mask},
		{ Button1Mask, AvButton1Mask},
		{ Button2Mask, AvButton2Mask},
		{ Button3Mask, AvButton3Mask}
	};

// convert X key state to AvKeyMask.
static int state2keymask( unsigned int state)
{
int mask = AvNoKeyMask;
unsigned int i;

	for(i=0; i < sizeof(keyconvert)/sizeof(*keyconvert); i++)
		if( (state & keyconvert[i].xmask) != 0)
		{	mask |= keyconvert[i].svmask;
		}

	return mask;
}

static struct {
	int		xbut;
	AvButton	svbut;
	} butconvert[] = {
		{ Button1, AvButton1},
		{ Button2, AvButton2},
		{ Button3, AvButton3}
	};

// convert X key state to AvKeyMask.
static AvButton button2svbutton( unsigned int key)
{
unsigned int i;

	for(i=0; i < sizeof(butconvert)/sizeof(*butconvert); i++)
		if( key == (unsigned int)butconvert[i].xbut)
			return butconvert[i].svbut;

	return AvNoButton;
}


void AvXWindow::inputCallback(Widget , XtPointer , XtPointer data)
{
XmDrawingAreaCallbackStruct *cbs = (XmDrawingAreaCallbackStruct *) data;
XEvent *xev = cbs->event;
AvEvent *event;
int	mask;
Boolean pressed;

	switch (xev->type) {
	case KeyPress:
	case KeyRelease:
		{ //AvKeyEvent *kev = new AvKeyEvent;
		  XKeyEvent *e = (XKeyEvent *)xev;

			if(e->type == KeyPress)
			{	pressed = TRUE;
				mask = AvEvent::AvKeyPressMask;
			}
			else
			{	pressed = FALSE;
				mask = AvEvent::AvKeyReleaseMask;
			}
//			kev->position(e->x, e->y);
			event = new AvKeyEvent(pressed,
				  state2keymask(e->state), e->x, e->y,
					// Keys aren't here yet.
				  NULL, 0, this);
		}
		break;
	case ButtonPress:
	case ButtonRelease:
		{ // AvButtonEvent *ev = new AvButtonEvent;
		   XButtonEvent *e = (XButtonEvent *)xev;
		   int	state;

			// Which buttons were pressed when the event happened.
			state = state2keymask(e->state);
			if(e->type == ButtonPress)
			{// The state only indicates the situation before the
			// event so on button press, the pressed button does
			// not appear there. We put it in.
				switch (e->button) {
				case Button1:
					state |= AvButton1Mask;
				case Button2:
					state |= AvButton2Mask;
				case Button3:
					state |= AvButton3Mask;
					break;
				default:
					break;
				}
				pressed = TRUE;
				mask = AvEvent::AvButtonPressMask;
			}
			else
			{	pressed = FALSE;
				mask = AvEvent::AvButtonReleaseMask;
			}
			event = new AvButtonEvent(e->x, e->y,
					state, pressed,
					button2svbutton(e->button), NULL);
		}
			break;
	default:
		event = NULL;
		mask = 0;
	}

	if(event != NULL)
	{	callCallbacks( *event, mask);
		delete event;
	}
}


void AvXWindow::motionCallback(Widget , XMotionEvent *e)
{
int	mask, key;

	key = e->state;
	if( key & Button1Mask)
		mask = AvEvent::AvButton1MotionMask;
	else
	if( key & Button2Mask)
		mask = AvEvent::AvButton2MotionMask;
	else
	if( key & Button3Mask)
		mask = AvEvent::AvButton3MotionMask;
	else
		mask = AvEvent::AvPointerMotionMask;

	AvMotionEvent *event = new AvMotionEvent(e->x, e->y,
				state2keymask(e->state), this);

	callCallbacks( *event, mask);
	delete event;
}

void	AvXWindow::enterHandler(Widget , XEvent *xev)
{
XCrossingEvent *ev = (XCrossingEvent *)xev;
int	mask;
AvEvent::EventType type;

	if(xev->type == EnterNotify)
	{	type = AvEvent::AvEnterWindowNotify;
		mask = AvEvent::AvEnterWindowMask;
	}
	else
	{	type = AvEvent::AvLeaveWindowNotify;
		mask = AvEvent::AvLeaveWindowMask;
	}

	AvEvent *event = new AvEvent(type, ev->x, ev->y, 0, this);
	callCallbacks( *event, mask);
	delete event;
}

/* Make the colormap the colormap of the widget. This needs to be called
after the widget is realized (mapped???). It gets called the first time
the window is exposed. This needs to be called since the colormap of the
window is probably different from the colormap of the parent widget.
We should probably check for this.
*/
static void setcm(Widget w, Colormap )
{
Widget wl[2];

	Widget rwid=widgetRoot(w);
	wl[0] = w;
	wl[1] = rwid;
	XtSetWMColormapWindows(rwid, wl, 2);	// Need the two entries.
}

// Expose should just draw the newly exposed parts!
void AvXWindow::exposeCallback(Widget w, XtPointer , XtPointer data)
{
XmDrawingAreaCallbackStruct *cbs = (XmDrawingAreaCallbackStruct *) data;
XExposeEvent *event = (XExposeEvent *)cbs->event;

	// If there are multiple exposes, only deal with the last one.
	if((event != NULL) &&( event->type == Expose))
	{	if(event->count != 0)
			return;
	}

	getWidthHeight(cbs->event);
//	printf("XC:expose callback called: %s\n", reason_name(cbs->reason));
	update();
	// Make the Colormap loadable
	if(! setcmap_)
	{	setcm(w, getXPalette()->colormap());
		setcmap_ = TRUE;
	}
	AvEvent svevent(AvEvent::AvWindowExpose, 0.0, 0.0, 0, NULL);
	callCallbacks(svevent, AvEvent::AvWindowExposeMask);
}

void AvXWindow::resizeCallback(Widget , XtPointer , XtPointer data)
{
XmDrawingAreaCallbackStruct *cbs = (XmDrawingAreaCallbackStruct *) data;

//	printf("XC:resize callback called: %s\n", reason_name(cbs->reason));
	getWidthHeight(cbs->event);
	clear();	// Really needed only on size reductions.
	AvEvent *event = new AvResizeEvent((float)width_, (float)height_);
	callCallbacks(*event, AvEvent::AvWindowResizeMask);
	delete event;
}

Drawable AvXWindow::drawable() { return XtWindow(wid_);}

/* Replace any display object with the supplied object.
Ignores the request if dob is NULL or the same as the current object.
The object is not displayed until show() or update (perhaps via exposeCallback)
is called.
*/
Boolean AvXWindow::setDisplayObject(AvGraphics2D *dob)
{
	if(dob == NULL)
		return FALSE;

	if(AvCanvas::setDisplayObject(dob))
	{	// Since we've removed the list, flush the cache.
		if(pmcache_ != NULL)
			pmcache_->flush();

		// Remove pixmap to force a 'show' if update gets called first.
		if(pm_ != NULL)
		{	pm_->unref();
			pm_ = NULL;
		}
		return TRUE;
	}
	else
		return FALSE;
}

/* Make the object the current display object and display it. If it
is NULL or the same as current object, redisplay the current list
rather than replacing it.
*/
void AvXWindow::show(AvGraphics2D *dob)
{
unsigned int w, h;
AvViewport *vp;

	setDisplayObject(dob);
	// Use whatever is actually on the list.
	dob = vps_->getObject(0);
	if(dob == NULL)
		return;

static int busy_ = -1;

	if(++busy_ > 0)
	{	// printf("XCanvas is BUSY\n");
		busy_--;
		return;
	}
	// Set the size of the main viewport to be that of the pixmap.
	vp = vps_->viewport();
	// Get desired size and make sure the pixmap is big enough.
	AvAllotment r;
	getBoundingBox( dob, r);
	w = (unsigned int) r.value(AvDimensionX);
	h = (unsigned int) r.value(AvDimensionY);
	vp->width(w);
	vp->height(h);

	// Make sure any pixmap is large enough.
	// For now, never reuse a larger pixmap. It messes up Y calculations.
	if(pm_ != NULL)
	{	// If it looks like a size reduction, clear the window.
		// Height isn't checked since it always just fits.
		if(w < pm_->width())
			clear();
		if((w != pm_->width()) || ( h != pm_->height()))
		{	pm_->unref();
			pm_ = NULL;
		}
	}

	if(pm_ == NULL)
	{	if((w == 0) || (h == 0))
		{	busy_--;
			return;	// !! Error!!
		}
		pm_ = new AvXPixmap(wid_, getXPalette(), w, h);
		pm_->ref();
		pm_->setRasterCache(pmcache_);
	}
	else	// Clear any old crud.
		pm_->clear();

	if(pm_ != NULL)
		pm_->show(vps_);	// Draw into pixmap
	update();			// copy pixmap to window.
	if(sync())
	  wait();
	busy_--;
}

// Copy an existing pixmap to the window. If there isn't one, check for the
// case of there being an display list object that hasn't been displayed yet.
// (aipsview Note: If window size is increased, this will probably be called
// twice. Once from the fitWindowToRaster and once from exposeCallback.
// Oh, well...)
void AvXWindow::update()
{
	if(pm_ != NULL)
		copyRegion( *pm_, *this);
	else
	{ AvGraphics2D	*dob= vps_->getObject(0);
	  if(dob != NULL)
		show();
	}
}

AvDisplayList *AvXWindow::displayList(){return vps_;}

void AvXWindow::show(AvGraphics2D &dob)
{
	show(&dob);
}

void AvXWindow::callCallbacks(AvEvent &event, int mask)
{
	handleEvent( *this, vps_, event, mask);
}
