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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvCanvas.cc,v 19.0 2003/07/16 05:46:43 aips2adm Exp $
//
// $Log: AvCanvas.cc,v $
// Revision 19.0  2003/07/16 05:46:43  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:29  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.2  2002/01/30 22:32:22  hravlin
// Added initializer for base class of AvGraphicContext copy constructor.
//
// Revision 17.1  2002/01/03 20:25:57  hravlin
// Added flush() and wait().
//
// Revision 17.0  2001/11/12 19:41:58  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:07  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:35  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:07:45  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:28  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:42  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:25  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:03  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1997/09/15 16:19:55  hr
// Conversion of line width from an enum to an int.
//
// Revision 9.0  1997/08/25 21:24:48  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:08  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.4  1997/02/09 00:39:35  hr
// Removed an unused variable.
//
// Revision 7.3  1996/12/12 08:53:58  droberts
// Changed all references to IVList to AvIVList.
//
// Revision 1.2  1996/11/18 21:35:08  hr
// Renamed IVlist to AvIVlist.
//
// Revision 1.1  1996/07/11  21:31:21  pixton
// Automated Checkin
//
//  Revision 1.9  1996/06/18  18:45:51  pixton
//  Copyright Update
//
//  Revision 1.8  1996/01/18  21:56:09  hr
//  Changed setDisplayObject to ignore request if old and new objects are
//  the same. Returns TRUE if there was a replacement done.
//
//  Revision 1.7  1995/11/13  22:05:14  hr
//  Added setDisplayObject(). Moved handling of display list (vps_)
//  to AvCanvas.
//
//  Revision 1.6  1995/10/07  15:19:13  hr
//  Removed drawRectangle function.
//
//  Revision 1.5  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/06/22  19:00:46  hr
//  Added support for AvGraphicContext.
//
//  Revision 1.3  1995/05/24  14:04:28  hr
//  Added convertRasterScale.
//
//  Revision 1.2  1995/03/29  21:30:23  hr
//  Remove Graphic context, add drawcolor()
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvCanvas.C
*/

#include <stdio.h>
#include <stdlib.h>			// getenv
#include <AvCanvas.h>
#include <AvRaster.h>
#include <AvIVList.h>
#include <AvPolylineP.h>
#include <AvPathList.h>
#include <AvViewportSetter.h>

// Temporary way to let user select vector color.
const char *DRAW_COLOR = "SV_DRAW_COLOR";

static const char *getEnvStr(const char *str, const char *def)
{
char	*env;

	env = getenv(str);
	if((env == NULL) || (strlen(env) == 0))
		return def;
	return env;
}

AvCanvasState::AvCanvasState(AvGraphicContext *g)
{
	gc_ = NULL;
	setGraphicContext(g);
}

AvCanvasState::AvCanvasState(const AvCanvasState &s)
{
	gc_ = NULL;
	copy(s);
}

AvCanvasState& AvCanvasState::operator=(const AvCanvasState &s)
{
	// Presumably, gc_ has already been set.
	copy(s);
	return *this;
}

void AvCanvasState::copy(const AvCanvasState &s)
{
	vp_ = s.vp_;
	setGraphicContext(s.gc_);
}

AvCanvasState::~AvCanvasState()
{
	if(gc_ != NULL)
		gc_->unref();
}

void AvCanvasState::setGraphicContext(AvGraphicContext *g)
{
	if(gc_ != NULL)
		gc_->unref();
	gc_ = g;
	if(gc_ != NULL)
		gc_->ref();
}

#if 0
void printVP(char *str, AvCanvasState &s)
{
AvViewport *vp=s.viewport();
AvAllotment xy, wh;

	xy = vp->xy();
	wh = vp->wh();
	if(str != NULL)
		printf("%s	", str);
	printf("x/y %d,%d	w/h %d/%d\n",
		vp->x0(), vp->y0(),
		vp->width(), vp->height());
}
#endif

declareList(StateStack, AvCanvasState)
implementList(StateStack, AvCanvasState)

declarePtrList(CallbackList, AvCallback)
implementPtrList(CallbackList, AvCallback)

class AvCanvasImpl {
  public:
	AvCanvasImpl();
	~AvCanvasImpl();
	void pushState(AvCanvasState &);
	void popState(AvCanvasState &);
	StateStack	*statestack_;
	CallbackList	*callbacks_;
	AvCallback	*grabber_;
};

AvCanvasImpl::AvCanvasImpl()
{
	statestack_ = new StateStack;
	callbacks_ = new CallbackList;
	grabber_ = NULL;
}

AvCanvasImpl::~AvCanvasImpl()
{
	delete statestack_;
	delete callbacks_;
}

void AvCanvasImpl::pushState(AvCanvasState &state)
{
	statestack_->append(state);
}

void AvCanvasImpl::popState(AvCanvasState &state)
{
	if(statestack_ == NULL)
	{	fprintf(stderr, "AvCanvas:statestack missing.\n");
		return;
	}

	long i = statestack_->count();
	if( i == 0)
	{	fprintf(stderr, "Canvas statestack underflow.\n");
		return;
	}
	i -= 1;
	state = statestack_->item(i);
	statestack_->remove(i);
}

AvCanvas::AvCanvas()
{
	// The size should be set by the subclassing canvas before drawing.
	impl_ = new AvCanvasImpl();
	pickpaths_ = NULL;
	pickevent_ = NULL;
	grabber_   = NULL;
	vps_ = new AvViewportSetter();
	vps_->ref();
	vps_->name("XCanvas ViewportSetter");
	setDrawColor(getEnvStr(DRAW_COLOR, "white"));
	changeFullState(FALSE);	// Default is to do as little as possible.
	sync_ = False;
}

AvCanvas::~AvCanvas()
{
	if(impl_ != NULL)
		delete impl_;
	if(vps_ != NULL)
		vps_->unref();
}

void AvCanvas::drawRaster(	AvRaster &,
				int , int , int , int ,
				unsigned int , unsigned int )
				// Requested width/height.
				{}
void AvCanvas::drawRaster(AvRaster *){}

void AvCanvas::show(AvGraphics2D &dob)
{
	changeFullState(TRUE);
	pushState();
		dob.draw(this);
	popState();	// Return to original values.
			// (Should probably clear stack and explicitly
			//  reset to original values).
	changeFullState(FALSE);
}

void AvCanvas::show(AvGraphics2D *dob)
{
	if(dob != NULL)
		show(*dob);
}

/* Replace any display object with the supplied object.
If the new object is the same as the old, do nothing and return FALSE.
Returns TRUE if the old object is replaced.
*/
Boolean AvCanvas::setDisplayObject(AvGraphics2D *dob)
{
	// Get the, presumably only, object on the list.
	AvGraphics2D *odob = vps_->getObject(0);
	if(dob == odob)			// Ignore if new is same as old.
		return FALSE;

	vps_->removeList();	// Remove the old list.
	vps_->append(dob);	// add the new.
	return TRUE;
}

#if 0
// Not here yet.
void AvCanvas::popGraphicContext(){}
void AvCanvas::pushGraphicContext(){}

void AvCanvas::popGC(){}
void AvCanvas::pushGC(){}
#endif


AvViewport * AvCanvas::viewport()
{
	return state_.viewport();
}

void AvCanvas::getViewport(AvViewport &vp)
{
	vp = state_.vp_;
}

void AvCanvas::setViewport(AvViewport &vp)
{
	state_.vp_.merge(vp);
}

void AvCanvas::pushState()
{
	impl_->pushState( state_);
}

void AvCanvas::popState()
{
	impl_->popState( state_);
	// Need to update GC if modified.
}

Boolean AvCanvas::changeFullState()
{
	return changeFullState_;
}

void AvCanvas::changeFullState(Boolean change)
{
	changeFullState_ = change;
}

void AvCanvas::getBoundingBox(AvGraphics2D *list, AvAllotment &r)
{
Boolean	cfs;

	if(list == NULL)
		return;
	cfs = changeFullState();
	changeFullState(FALSE);
		list->getBoundingBox( *this, r);
	changeFullState(cfs);
}

void AvCanvas::clear(int, int, unsigned int, unsigned int)
{
}

void AvCanvas::resetViewport(int x, int y, unsigned int w, unsigned int h)
{
	state_.vp_.x0(x);
	state_.vp_.y0(y);
	state_.vp_.width(w);
	state_.vp_.height(h);
}

float AvCanvas::convertRasterScale(const float scl)const
{
	return scl;
}

// Scale a given size. (May be overridden by particular canvases!
unsigned int AvCanvas::scaleSize(const AvDimension d,
		const unsigned int val)
{
float	xscl, yscl, scl, newval;

	viewport()->getRasterScale(xscl, yscl);
	scl = (d == AvDimensionX) ? xscl : yscl;

#if 0
	newval = scl * val;
	if(newval < 0.0)
		return 0;
	else
		return (unsigned int) newval;
#else
	newval = val * convertRasterScale(scl);
	if(newval < 0.0)
		return 0;
	else
		return (unsigned int) newval;
#endif
}

void AvCanvas::grabEvents(AvCallback *cb)
{
	impl_->grabber_ = cb;
}

void AvCanvas::releaseEvents()
{
	impl_->grabber_ = NULL;
}

AvCallback *AvCanvas::eventGrabber() { return impl_->grabber_;}

void AvCanvas::setGrabber(AvGraphics2D *gbr)
{
	if(gbr != NULL)
		gbr->ref();
	grabber_ = gbr;
}

void AvCanvas::releaseGrabber()
{
	if(grabber_ != NULL)
		grabber_->unref();
	grabber_ = NULL;
}

AvGraphics2D *AvCanvas::grabber() { return grabber_;}

void AvCanvas::addCallback(AvCallback *CB)
{
CallbackList *cbl = impl_->callbacks_;

	// If callback is already in list, ignore.
	for(ListItr(CallbackList) i(*cbl); i.more(); i.next())
	{ AvCallback *cb = i.cur();
		if(cb == CB)
			return;
	}
	// Not in list, so add.
	cbl->append(CB);
}

// Remove a callback from callback list.
void AvCanvas::removeCallback(AvCallback *CB)
{
CallbackList *cbl = impl_->callbacks_;

	if(CB == NULL)
		return;		// Nothing to do.
	for(ListUpdater(CallbackList) i(*cbl); i.more(); i.next())
	{ AvCallback *cb = i.cur();
		if(cb == CB)
		{	i.remove_cur();
			break;
		}
	}
}

// Call each callback that wants this event.
void AvCanvas::callCallbacks(AvEvent &event, int mask)
{
CallbackList *cbl = impl_->callbacks_;
AvCallback *cbg;
int	cbmask;

	// See if a callback is grabbing all events.
	cbg = impl_->grabber_;
	if(cbg != NULL)
	{	cbg->execute(event);
		if(event.isHandled())
			return;
	}
	// If event still isn't handled, try the callback list except don't
	// bother trying the grabber again.
	for(ListItr(CallbackList) i(*cbl); i.more(); i.next())
	{ AvCallback *cb = i.cur();
		if((cb == NULL) || (cb == cbg))
			 continue;
		cbmask = cb->mask();
		if((cbmask & mask) != 0)
		{	cb->execute(event);
			if(event.isHandled())
				return;
		}
	}
}

/* Handle events.
  Process until something deals with the event.
	Pass events to callbacks.
	if events are grabbed, call grabing object.
	if event not handled, call DisplayList's handleEvent.
*/
void AvCanvas::handleEvent(AvCanvas &c, AvGraphics2D *dl,
				AvEvent &event, int eventMask)
{
Boolean cfs = changeFullState();

	event.canvas(this);
	changeFullState(FALSE);	// Don't bother

	// If the event is a pick event, reset pick path and event.
	if((event.eventType() == AvEvent::AvButtonPress)
					&& event.button1Pressed())
	{	if(pickevent_ != NULL)
			delete pickevent_;
		pickevent_ = new AvEvent(event);
		if(pickpaths_ != NULL)
		{	pickpaths_->unref();
			pickpaths_ = NULL;
		}

	}
	// The order here sets the priority of who gets to see an event first.
	AvCanvas::callCallbacks(event, eventMask);

	if( !event.isHandled() && (grabber_ != NULL))
		grabber_->handleEvent(c, &event);

	// Finally, if the grabber didn't want it try the DL.
	if(! event.isHandled() && (dl != NULL))
		dl->handleEvent(c, &event);

	changeFullState(cfs);
}

AvPathList *AvCanvas::getPickPaths()
{
	if(pickevent_ == NULL) return NULL;
	if(pickpaths_ == NULL)
	{	pickpaths_ = new AvPathList();
		pickpaths_->ref();
		displayList()->getPickPaths(*this,
				pickevent_->position(),
				*pickpaths_);
	}
	return pickpaths_;
}

void AvCanvas::drawVector(AvPolylineCmd *)
{
}

AvDisplayList *AvCanvas::displayList() { return NULL;}

AvPalette *AvCanvas::palette()const{return NULL;}

AvGraphicContext *AvCanvas::getGraphicContext()const
{
	return state_.graphicContext();
}

/* Set the color used to draw vectors and text.
Since the default canvas has no palette, this can't do much.
*/
void AvCanvas::setDrawColor(const unsigned long color)
{
	state_.graphicContext()->setForeground(color);
}

unsigned long AvCanvas::getDrawColor()const
{
	return state_.graphicContext()->getForeground();
}

void AvCanvas::setDrawColor(const char * /*colorname*/)
{
}

void AvCanvas::flush()
{
	return;
}

void AvCanvas::wait()
{
	return;
}

/****************************************************************/
/****************************************************************/
/****************************************************************/
/*			Graphic context code.			*/
/****************************************************************/
/****************************************************************/
/****************************************************************/

AvGraphicContext::AvGraphicContext()
{
	drawmode_ = COPY;
	linewidth_ = DEFAULTLINEWIDTH;
	linestyle_ = SOLID;
	font_ = DEFAULTFONT;
	foreground_ = 0;
	background_ = 1;
}

AvGraphicContext::AvGraphicContext(const AvGraphicContext &gc) :
		AvResource()
{
	copy(gc);
}

// Make sure this GC is set correctly.
void AvGraphicContext::update(const int /*mask*/)
{
	setModified(FALSE);
}

void AvGraphicContext::copy(const AvGraphicContext &gcin)
{
	drawmode_ = gcin.drawmode_;
	linewidth_ = gcin.linewidth_;
	linestyle_ = gcin.linestyle_;
	foreground_ = gcin.foreground_;
	background_ = gcin.background_;
	font_ = gcin.font_;
	setModified();
}

AvGraphicContext& AvGraphicContext::operator=(const AvGraphicContext &gcin)
{
	copy(gcin);
	return *this;
}

AvGraphicContext::~AvGraphicContext()
{

}

void AvGraphicContext::setDrawMode(const DrawMode d)
{
	drawmode_ = d;
}

#if 0
void AvGraphicContext::setLineWidth(const AvGraphicContext::LineWidth lw)
{
	linewidth_ = lw;
}
#else
void AvGraphicContext::setLineWidth(const int lw)
{
	if(lw >= 0)
		linewidth_ = lw;
	else
		linewidth_ = 0;	// Force valid.
}
#endif

void AvGraphicContext::setLineStyle(const LineStyle ls)
{
	linestyle_ = ls;
}

void AvGraphicContext::setFont(const Font f)
{
	font_ = f;
}

void AvGraphicContext::setForeground(const unsigned long c)
{
	foreground_ = c;
}

void AvGraphicContext::setBackground(const unsigned long c)
{
	background_ = c;
}
