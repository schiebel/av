//# Copyright (C) 1995-98 Board of Trustees of the University of Illinois
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
//---------------------------------------------------------------------------
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvAxisVPS.cc,v 19.0 2003/07/16 05:46:34 aips2adm Exp $
//
// $Log: AvAxisVPS.cc,v $
// Revision 19.0  2003/07/16 05:46:34  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:21  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:41:50  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:00  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:04  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:07:35  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:18  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:25  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:00  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:51:32  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.2  1998/06/09 21:20:06  hr
// Removed unneeded semicolon after DEFINE_MEMBER_INFO macro.
//
// Revision 9.1  1998/05/20 21:46:48  hr
// Minor changes to deal with changes to AvViewportSetter constructors.
//
// Revision 9.0  1997/08/25 21:24:00  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:14:40  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.4  1997/02/05 17:28:01  hr
// Removed an unused variable and changed an unsigned/int comparision.
//
// Revision 7.3  1997/01/24 19:56:47  hr
// Changed constructor to take an AvAxisOptions argument.
//
// Revision 7.2  1996/12/12 05:38:07  droberts
// Final update from monet archive.
//
// Revision 1.2  1996/08/14 17:18:52  hr
// Changes to remove g++ warnings (from bglenden).
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.2  1996/06/18  18:45:47  pixton
//  Copyright Update
//
//  Revision 1.1  1996/02/23  17:45:38  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvAxisVPS is a subclass of AvViewportSetter that expects to manage
two objects. The first is a viewport setter typically containing a
raster object. The second is an AvAxis that is meant to surround the
first.

When the axis is enabled, the size of the AVPS is increased to support
the space needed for the labels. The raster's vps is moved inward by
the margins.

Example:
		AVPS
		| |
		| ----- Axis, ...
		VPR
		|
		|- Raster
		.- <possibly more>

AVPS	Axis viewportSetter
VPR	viewportSetter containing a raster and, perhaps, a contour. 

It should be possible to replace the raster with something else, but
the size may have to be explicitly set and it hasn't been tried yet.


	------------
	|    tm	    |
	|  -------  |
	|  |	 |  |
	|lm| ras |rm|
	|  |	 |  |
	|  ------   |
	|    bm	    |
	------------

Labels are drawn in the margins.

*/

#include <stdio.h>
#include <AvAxisVPS.h>
#include <AvXCanvas.h>
#include <AvCLUIRaster.h>

#ifdef PGPLOT
#include <AvAxis.h>
#endif

DEFINE_MEMBER_INFO(AvAxisVPS)

AvAxisVPS::AvAxisVPS(	AvViewportSetter *vps, AvAxis *axis,
			const int lm, const int rm, const int tm, const int bm)
			: AvViewportSetter()
{
	append(vps);
	append(axis);
	init(lm, rm, tm, bm);
}

AvAxisVPS::AvAxisVPS(	AvRaster *ras, AvAxisOptions *options,
			const int lm, const int rm,
			const int tm, const int bm)
{
	init(lm, rm, tm, bm);
	if(ras != NULL)
	{AvViewportSetter *vpr;
	 AvAxis		 *axis;
	 char buf[256];

		vpr = new AvViewportSetter(ras);
		append(vpr);
		axis = new AvAxis(ras, options);
		append(axis);
		setAxisMargins();
		setRasterVPSOffset();
		// The names may be helpful during debugging.
		sprintf(buf, "VPR::%s:", ras->getName());
		vpr->name(buf);
		sprintf(buf, "AxisVPS::%s", ras->getName());
		name(buf);
	}
}

// Copy current margins to the axis.
// Use "real" instead of effective margins in case we're currently
// turned off.
void AvAxisVPS::setAxisMargins()
{
AvAxis *axis = getAxis();

	if(axis != NULL)
		axis->setMargins(lm_, rm_, tm_, bm_);
}

// Copy current offset to the raster's vps.
void AvAxisVPS::setRasterVPSOffset()
{
AvViewportSetter *vps = getRasterVPS();

	if(vps != NULL)
	{ int lm, rm, tm, bm;
		getMargins(lm, rm, tm, bm);
		vps->setPosition(lm, bm);
	}
}

void AvAxisVPS::init(const int lm, const int rm, const int tm, const int bm)
{
	INIT_CLASS(AvViewportSetter);

	lm_ = lm;
	rm_ = rm;
	tm_ = tm;
	bm_ = bm;
	computeSize_ = TRUE;		// Need to figure out size.
	wasShowingAxis_ = showAxis();
}

AvAxisVPS::~AvAxisVPS()
{
}


// Change whether the axis is displayed. If a change occurs, the size of the
// viewports holding the axes has to be changed, the raster viewport
// offsets need changing and the window size needs to be adjusted.
// (Window changing needs to be done elsewhere).
void AvAxisVPS::showAxis(const Boolean show)
{
	if(showAxis() == show)		// Ignore if no change.
		return;

#ifdef PGPLOT
	// Tell axis objects whether to ignore draw requests.
	AvAxis *a;
	if((a = getAxis()) != NULL)
		a->showAxis(show);
#endif

//	AvViewportSetter *vps = getRasterVPS();
	setRasterVPSOffset();
}

Boolean AvAxisVPS::showAxis()const
{
#ifdef PGPLOT
AvAxis *axis = getAxis();

	if(axis != NULL)
		return axis->showAxis();
	else
#endif
		return FALSE;
}

// Set margins to be used and copy to the axis.
void AvAxisVPS::setMargins(const int lm, const int rm,
			   const int tm, const int bm)
{
	if((lm != lm_) || (rm != rm_) || (tm != tm_) || (bm != bm_))
	{	lm_ = lm;
		rm_ = rm;
		tm_ = tm;
		bm_ = bm;
		setAxisMargins();
		setRasterVPSOffset();
		touch();
	}
}

// Return the desired margins
void AvAxisVPS::getMargins(int &lm, int &rm, int &tm, int &bm)const
{
	if(showAxis())
	{	lm = lm_;
		rm = rm_;
		tm = tm_;
		bm = bm_;
	}
	else
	{	lm = rm = tm = bm = 0;
	}
}

// Return total margin size.
void AvAxisVPS::getMargins(int &hm, int &vm)const
{
	if(showAxis())
	{	hm = lm_ + rm_;
		vm = tm_ + bm_;
	}
	else
	{	hm = vm = 0;
	}
}

#ifdef PGPLOT
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
	// Functions to retrieve display objects.
// These are obviously very sensitive to how the display list is generated.

// Return the first child that is derived from the ID. (An object is
// considered to be derived from itself). If vps is NULL, "this" is used.
AvGraphics2D *AvAxisVPS::getChild(
			const int childID, const AvViewportSetter *VPS)const
{
AvGraphics2D *child;
int	len;
const AvViewportSetter *vps = (VPS == NULL) ? this : VPS;

	// Look for first child with the correct ID.
	len = vps->getLength();
	for(int i=0; i< len; i++)
	{	child = vps->getObject(i);
		if(child == NULL)		// INTERNAL ERROR!
			return NULL;
		if(child->isDerivedFrom(childID))
			return child;
	}
	return NULL;
}

// If obj is derived from (or is) a viewportSetter, return it as a
// viewportSetter.
static inline AvViewportSetter *isVPS(AvGraphics2D *obj)
{
	if((obj != NULL) &&
	   ( obj->isDerivedFrom(AvViewportSetter::getClassTypeID())))
		return (AvViewportSetter *)obj;
	else
		return NULL;
}

// Return a pointer to the raster's viewportSetter.
AvViewportSetter *AvAxisVPS::getRasterVPS()const
{
AvGraphics2D *obj;

	// The raster's viewportSetter
	obj = getChild(AvViewportSetter::getClassTypeID(), this);
	return isVPS(obj);
}

// Return a pointer to the raster object for the slice.
AvRaster *AvAxisVPS::getRaster()const
{
AvRaster *obj;
AvViewportSetter *vpr;

	vpr = getRasterVPS();
	if(vpr != NULL)
		obj = (AvRaster *) getChild(AvRaster::getClassTypeID(), vpr);
	else
		obj = NULL;
	return obj;
}

// Return a pointer to the axis object.
AvAxis *AvAxisVPS::getAxis()const
{
AvAxis *obj;

	obj = (AvAxis *) getChild(AvAxis::getClassTypeID(), this);
	return obj;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
#endif

//#include <stdio.h>

//static int ow=0, oh=0;

// If something has changed, reset size.
// If the vps contains a raster, the new size will be the raster's size
// times the scale. Otherwise, a bounding box will be requested.
// The bounding box part has not been tested!
// If the size of the viewport changes (not due to turning the axis on/off),
// the axis is forced to recompute itself since the way the axis is generated
// doesn't auto scale well.
void AvAxisVPS::update(AvCanvas &c)
{
AvRaster *ras = getRaster();
#ifdef PGPLOT
AvAxis	*axis = getAxis();
#endif
int lm, rm, tm, bm;
int w, h;	// VP size w/o offsets.
unsigned int	ow, oh;
AvViewportSetter *vpr = getRasterVPS();

	getMargins(lm, rm, tm, bm);
	if(showAxis())
	{	// Pick up current size of window w/o margins.
		getSize(ow, oh);
		if(wasShowingAxis_)
		{	ow -= (lm+rm);
			oh -= (tm+bm);
		}
	}

	// Find current size needed.
	if(computeSize())
	{	if(ras != NULL)
		{ float xscl1, yscl1, xscl, yscl;
			c.viewport()->getRasterScale(xscl, yscl);
			// Local raster scale hasn't been taken
			// into account yet.
			viewport()->getRasterScale(xscl1, yscl1);
			xscl *= xscl1;
			yscl *= yscl1;

			// We shouldn't have to count on the X Canvas.
			xscl = AvXCanvas::convertScale(xscl);
			yscl = AvXCanvas::convertScale(yscl);

			w = ras->width();
			h = ras->height();
			w = int(w*xscl);	// Scaled size.
			h = int(h*yscl);
		}
		else	// Try something else.
		{ AvAllotment r;
			setSize(0,0);	// Turn off local size flag.
				// (Otherwise, bb will return current).
			if(vpr != NULL)
				vpr->setSize(0,0);
			AvViewportSetter::getBoundingBox(c, r);
			// The above adds in the offsets (for the raster),
			// Now add in the right and top margins.
			w = int(r.x());
			h = int(r.y());
		}
		setSize(w + lm + rm, h + tm + bm);
	}
	else	// Use size set externally.
	{unsigned int uw, uh;
		getSize( uw, uh);
		w = uw - (lm + rm);
		h = uh - (tm + bm);
	}

	// The raster's viewport needs to have its size set
	// so turning off the raster won't affect things like
	// contours. (setSize will ignore request if there is no change).
	if(vpr != NULL)
		vpr->setSize(w, h);

	// If the size has changed the axis will need to be recomputed.
	if(showAxis() && (((int)ow != w) || ((int)oh != h)))
	{
#ifdef PGPLOT
		axis->touch();
#endif
	}
	wasShowingAxis_ = showAxis();
}

void AvAxisVPS::getBoundingBox(AvCanvas &c, AvAllotment &r)
{
	if(! ignore())
		update(c);
	r = viewport()->wh();
}

// The Canvas calls draw with a pointer to itself to tell the object
// to display itself.
void AvAxisVPS::draw(AvCanvas &c)
{
	if(ignore())
		return;

	update(c);		// Check to see if anything has changed.
	AvViewportSetter::draw(c);
}

void AvAxisVPS::draw(AvCanvas *c)
{
	if(c != NULL)
		draw(*c);
}

// Normally, AvAxisVPS will determine its size from the raster. If compute
// is TRUE, it will always use the size set by setSize.
void AvAxisVPS::computeSize(const Boolean compute)
{
	computeSize_ = compute;
}
