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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvViewportSetter.cc,v 19.0 2003/07/16 05:46:54 aips2adm Exp $
//
// $Log: AvViewportSetter.cc,v $
// Revision 19.0  2003/07/16 05:46:54  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:39  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:08  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:16  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:11  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:07:58  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:41  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:06  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:00  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:42  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.2  1998/06/09 21:24:41  hr
// Removed unneeded semicolon after DEFINE_MEMBER_INFO macro.
//
// Revision 9.1  1998/05/20 21:40:47  hr
// Removed constructors with more than one pointer to AvGraphics2D * to
// prevent possible ambiguity problems with some compilers.
//
// Revision 9.0  1997/08/25 21:25:51  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:45  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 07:40:32  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.7  1996/06/18  18:46:41  pixton
//  Copyright Update
//
//  Revision 1.6  1996/02/14  22:27:09  hr
//  Removed offsets, and added {get,set}Position.
//
//  Revision 1.5  1996/01/29  20:36:33  hr
//  Added get/setOffsets
//
//  Revision 1.4  1996/01/26  21:11:43  hr
//  Added getSize/setSize.
//
//  Revision 1.3  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/04/13  14:05:34  hr
//  Only change scale if new is different from old.
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvViewportSetter.C
Graphics2D object to change viewport.
Implemented as a DisplayList object.

*/

#include <AvViewportSetter.h>
#include <AvCanvas.h>
#include <AvEvent.h>
#include <AvPathList.h>

//static char *Name = "AvViewportSetter";
DEFINE_MEMBER_INFO(AvViewportSetter)

#if 0
AvViewportSetter::AvViewportSetter(
	AvGraphics2D* a, AvGraphics2D* b, AvGraphics2D* c,
	AvGraphics2D* d, AvGraphics2D* e, AvGraphics2D* f,
	AvGraphics2D* g, AvGraphics2D* h, AvGraphics2D* i,
	AvGraphics2D* j) : AvDisplayList(a, b, c, d, e, f, g, h, i, j)
{
	INIT_CLASS(AvDisplayList);
	vp_.x0(0);
	vp_.y0(0);
	localsize_ = FALSE;
}
#else
AvViewportSetter::AvViewportSetter(AvGraphics2D *obj) : AvDisplayList(obj)
{
	INIT_CLASS(AvDisplayList);
	vp_.x0(0);
	vp_.y0(0);
	localsize_ = FALSE;
}
#endif

#if 0
AvViewportSetter::AvViewportSetter(const AvViewport &vp,
	AvGraphics2D* a, AvGraphics2D* b, AvGraphics2D* c,
	AvGraphics2D* d, AvGraphics2D* e, AvGraphics2D* f,
	AvGraphics2D* g, AvGraphics2D* h, AvGraphics2D* i,
	AvGraphics2D* j) : AvDisplayList(a, b, c, d, e, f, g, h, i, j)
{
	INIT_CLASS(AvDisplayList);
	vp_ = vp;
	localsize_ = TRUE;
}
#else
AvViewportSetter::AvViewportSetter(const AvViewport &vp) : AvDisplayList()
{
	INIT_CLASS(AvDisplayList);
	vp_ = vp;
	localsize_ = TRUE;
}
#endif

#if 0
AvViewportSetter::AvViewportSetter(float x, float y,
	AvGraphics2D* a, AvGraphics2D* b, AvGraphics2D* c,
	AvGraphics2D* d, AvGraphics2D* e, AvGraphics2D* f,
	AvGraphics2D* g, AvGraphics2D* h, AvGraphics2D* i,
	AvGraphics2D* j) : AvDisplayList(a, b, c, d, e, f, g, h, i, j)
{
	INIT_CLASS(AvDisplayList);
	localsize_ = FALSE;
	vp_.x0((int)x);
	vp_.y0((int)y);
}
#else
AvViewportSetter::AvViewportSetter(const float x, const float y)
				   : AvDisplayList()
{
	INIT_CLASS(AvDisplayList);
	localsize_ = FALSE;
	vp_.x0((int)x);
	vp_.y0((int)y);
}
#endif

#if 0
AvViewportSetter::AvViewportSetter( float x, float y,
	float width, float height,
	AvGraphics2D* a, AvGraphics2D* b, AvGraphics2D* c,
	AvGraphics2D* d, AvGraphics2D* e, AvGraphics2D* f,
	AvGraphics2D* g, AvGraphics2D* h, AvGraphics2D* i,
	AvGraphics2D* j) : AvDisplayList(a, b, c, d, e, f, g, h, i, j)
{
	INIT_CLASS(AvDisplayList);
	localsize_ = TRUE;
	vp_.x0((int)x);
	vp_.y0((int)y);
	if(width <= 0)
		width = 1.0;
	vp_.width((int)width);
	if(height <= 0)
		height = 1.0;
	vp_.height((int)height);
}
#else
AvViewportSetter::AvViewportSetter( const float x, const float y,
				    const float Width, const float Height)
				    : AvDisplayList()
{
	INIT_CLASS(AvDisplayList);
	localsize_ = TRUE;
	vp_.x0((int)x);
	vp_.y0((int)y);
	float width = Width;
	float height = Height;
	if(width <= 0)
		width = 1.0;
	vp_.width((int)width);
	if(height <= 0)
		height = 1.0;
	vp_.height((int)height);
}
#endif

AvViewportSetter::~AvViewportSetter( ){}

void AvViewportSetter::setViewport(AvViewport &vp)
{
	vp_ = vp;
}

void AvViewportSetter::getViewport(AvViewport &vp)
{
	vp = vp_;
}

void AvViewportSetter::getSize(unsigned int &w, unsigned &h)const
{
	w = vp_.width();
	h = vp_.height();
}

void AvViewportSetter::setSize(const unsigned int w, const unsigned h)
{
	vp_.width(w);
	vp_.height(h);
	if((w == 0) && (h == 0))
		localsize_ = FALSE;
	else
		localsize_ = TRUE;
	touch();
}

// Get/Set position of the viewport.
void AvViewportSetter::setPosition(const int x, const int y)
{
	vp_.setPosition(x, y);
	touch();
}

void AvViewportSetter::getPosition(int &x, int &y)const
{
	vp_.getPosition(x, y);
}

AvViewport *AvViewportSetter::viewport(){ return &vp_;}

/* Merges the viewport into the canvas' viewport.
If there size isn't set locally, pick up children's bounding box.
Does not save/restore canvas state.
*/
void AvViewportSetter::setViewport(AvCanvas &c)
{
	if(! localsize_)
	{ AvAllotment bb;
		AvDisplayList::getBoundingBox( c, bb);
		vp_.wh(bb);
	}
	c.setViewport(vp_);

}

void AvViewportSetter::toVPCoords(const AvAllotment &from, AvAllotment &to)
{
	to = from - vp_.xy();
}

void AvViewportSetter::fromVPCoords(const AvAllotment &from, AvAllotment &to)
{
	to = from + vp_.xy();
}

void AvViewportSetter::draw(AvCanvas &c)
{
AvViewport vp = vp_;

	c.pushState();
		// If needed, find size of children.
		if(! localsize_)
		{AvAllotment bb;
			getBoundingBox( c, bb);
			vp.wh(bb - vp_.xy()); // BB includes offsets.
		}

		c.setViewport(vp);
		AvDisplayList::draw(c);
	c.popState();
}

void AvViewportSetter::draw(AvCanvas *c)
{
	if(c != NULL)
		draw(*c);
}
// Compute bounding box, turning off uneeded canvas overhead.
void AvViewportSetter::getBoundingBox(AvCanvas &c, AvAllotment &r)
{
	if( localsize_)
		r = vp_.wh();
	else
	{Boolean	cfs = c.changeFullState();
		c.changeFullState(FALSE);
		c.pushState();
			c.setViewport(vp_);		// Really needed??? 
			AvDisplayList::getBoundingBox(c, r);
		c.popState();
		c.changeFullState(cfs);
	}
	r = r + vp_.xy();	// Desired size includes offsets.
}

void AvViewportSetter::setRasterScale( const float xscl, const float yscl)
{
float	oxscl, oyscl;

	// Only change if something is different.
	vp_.getRasterScale(oxscl, oyscl);
	if( (oxscl != xscl) || ( oyscl != yscl))
	{	vp_.setRasterScale(xscl, yscl);
		touch();
	}
}

void AvViewportSetter::setRasterScale( const float scl)
{
	vp_.setRasterScale(scl);
}

#include <stdio.h>
void AvViewportSetter::handleEvent(AvCanvas &c, AvEvent *e)
{
AvViewport vp = vp_;

#if 0
	if(strcmp(getName(), "LR") == 0)
		printf("%s\n", getName());
#endif

	if(e == NULL) return;
	// If needed, find size of children.
#if 0
	if(! localsize_)
		AvDisplayList::getBoundingBox( c, vp.wh());
#else
	setViewport(c);
#endif
	c.pushState();
		c.setViewport(vp);

		if(isHit(c, e->position()))
				AvDisplayList::handleEvent(c, e);
#if 0
		else
			printf("Missed %s\n", getName());
#endif
	c.popState();
	return;
}

/*
Returns TRUE if the point is within these boundaries. The point is assumed
to be in terms of the surrounding viewport.
NOTE:
	Don't do set state here since it may have been done elsewhere and
doing it twice could screw things up.
*/

Boolean AvViewportSetter::isHit(AvCanvas &, const AvAllotment &a)const
{
	return vp_.contains(a);
}

void AvViewportSetter::getPickPaths(AvCanvas &c, const AvAllotment &a,
								AvPathList &pl)
{
//AvViewport vp = vp_;

	c.pushState();

		setViewport(c);

#if 0
{ float x, y, vx, vy;
	if(strcmp(getName(), "LR") == 0)
	{	vx = vp.x0();
		vy = vp.y0();
		x = a.x();
		y = a.y();
		printf("%s VP:%.0f/%.0f A: %.0f/%.0f\n",
			getName(), vx, vy, x, y);
	}
}
#endif

		if(isHit(c, a))
		{
//			printf("Hit %s\n", getName());
			pl.append(this);
			AvDisplayList::getPickPaths(c, a-vp_.xy(), pl);
		}
#if 0
		else
			printf("Missed %s\n", getName());
#endif
	c.popState();
	return;
}
