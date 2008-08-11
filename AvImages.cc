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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvImages.cc,v 19.0 2003/07/16 05:47:40 aips2adm Exp $
//
// $Log: AvImages.cc,v $
// Revision 19.0  2003/07/16 05:47:40  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:18  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:47  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:53  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:50  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:46  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:34  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:39  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:12  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:16  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/09 21:20:44  hr
// Removed unneeded semicolon after DEFINE_MEMBER_INFO macro.
// Also added as cast to float for a call to max(dmax).
//
// Revision 9.0  1997/08/25 21:29:53  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:18:16  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/02/09 00:52:32  hr
// Fixed signed/unsigned comparisions, a long in printf and removed an
// unused variable.
//
// Revision 7.2  1996/12/12 07:53:20  droberts
// Final update from monet archive.
//
// Revision 1.4  1996/11/12 20:15:42  hr
// Removed unneeded getRegion() call.
//
// Revision 1.3  1996/09/20 19:47:15  hr
// Temporarily removed some code.
//
// Revision 1.2  1996/08/14  19:01:27  hr
// Changes to remove g++ warnings (from bglenden).
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.7  1996/06/18  18:46:17  pixton
//  Copyright Update
//
//  Revision 1.6  1996/02/23  17:09:16  hr
//  Added AvColorWedge.
//
//  Revision 1.5  1995/10/06  15:48:23  hr
//  Changed AvImageWedge2 to support getRow and changed its constructor
//  interface.
//
//  Revision 1.4  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/06/09  19:57:22  hr
//  added an 'int' to lineWidth().
//
//  Revision 1.2  1995/06/05  19:18:17  hr
//  Changed to use getElement rather than getRegion.
//  Added line width to cross.
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvImages.C
Example images.
*/
#include "AvImages.h"
#include "AvViewport.h"
#include "AvCanvas.h"

#include <stdio.h>

DEFINE_MEMBER_INFO(AvImageCross)

AvImageCross::AvImageCross(unsigned int width, unsigned int height,
	float on, float off) : AvRaster()
{
	INIT_CLASS(AvRaster);
	on_ = on;
	off_ = off;
	width_ = width;
	height_ = height;
	lineWidth_ = 1;
	name("ImageCross");
}

AvImageCross::~AvImageCross(){}

unsigned int AvImageCross::width()const {return width_;}
unsigned int AvImageCross::height()const{return height_;}
void AvImageCross::lineWidth(const int l)
{
	if( l > 0)
		lineWidth_ = l;
}

float AvImageCross::getElement(const int x, const int y)
{
	// Cross is defined to be width x height.
	int w2 = width()/2;
	int h2 = height()/2;
	int l2a = (lineWidth()-1)/2;
	int l2b = lineWidth() - l2a;
	if(	((x >= w2-l2a)  && ( x <= w2+l2b))
				||
		((y >= h2-l2a)  && ( y <= h2+l2b)))
		return on_;
	else
		return off_;
}

float AvImageCross::min()const
{
	return (off_ < on_) ? off_ : on_;
}

void AvImageCross::min(const float m)
{
	off_ = m;
}

float AvImageCross::max()const
{
	return (off_ > on_) ? off_ : on_;
}

void AvImageCross::max(const float m)
{
	on_ = m;
}


/*****************************************************************/
/*****************************************************************/
/*****************************************************************/
DEFINE_MEMBER_INFO(AvImageWedge)

AvImageWedge::AvImageWedge(unsigned int dwidth, unsigned int dheight,
	unsigned long dmax) : AvImage(dwidth, dheight)
{
unsigned int r,c;
unsigned int w,h, wh;
float	*d, scl;
char buf[80];

	INIT_CLASS(AvImage);
	min(0.0);
	max((float)dmax);
	w = width();
	h = height();
	sprintf(buf, "ImageWedge:%ld", dmax);
	name(buf);
	wh = w*h - 1;
	if(wh <= 0)
		wh = 1;
	d = new float[w];

	scl = (float) dmax/(float)(wh);

	for(r=0; r< h; r++)
	{float tmp;
	  for(c=0; c< w; c++)
	  {	tmp = (r*w + c) * scl;
		d[c] = tmp;
	  }
	   putRegion(d, 0, r);
	}

	delete [] d;
}

AvImageWedge::~AvImageWedge() {}

DEFINE_MEMBER_INFO(AvImageWedge2)

// Creates a wedge that is the size of its viewport.
AvImageWedge2::AvImageWedge2( const float dmax)
{
	INIT_CLASS(AvRaster);
	max_ = dmax;
	// These don't get set until draw is called.
	width_ = 0;
	height_ = 0;
	scl_ = 0.0;
}

AvImageWedge2::~AvImageWedge2(){}

unsigned int AvImageWedge2::width() const {return width_;}
unsigned int AvImageWedge2::height()const {return height_;}

static inline float element(const int x, const int y, const int W,
				const float scl)
{
	return (y*W + x)*scl;
}

void AvImageWedge2::getRow(float *row, const int X, const int Y,
					const int Num, const int Incr)
{
int x=X, y=Y, num=Num, incr=Incr;
int x1, w = width();
float scl;
int W, H;

	if( (x < 0) || ( x >= w))
		return;
	if( (y < 0) || ( y >= (int)height()))
		return;

	if(incr == 0)
		incr = 1;

	if(num <= 0)
		num = w;

	x1 = x + incr*(num-1);	// endpoint.

	// Cross is defined to be width x height.
	W = width();
	H = height();
	scl = (float) max()/(float)((W)*(H));

	if(incr > 0) // Left to right.
	{	if(x1 >= w)	// End point is > start point.
			x1 = w -1;
		while(x <= x1)
		{	*row++ = element(x, y, W, scl);
			x += incr;
		}
	}
	else	// Right to left.
	{	if( x1 < 0)	// End point is < start point.
			x1 = 0;
		while(x >= x1)
		{	*row++ = element(x, y, W, scl);
			x += incr;
		}
	}
}

float AvImageWedge2::min()const { return 0.0;}
float AvImageWedge2::max()const { return max_;}

void AvImageWedge2::min(const float ){}
void AvImageWedge2::max(const float m) { max_ = m;}

void AvImageWedge2::getBoundingBox(AvCanvas &, AvAllotment &r)
{
	r.value(AvDimensionX, 1.0);
	r.value(AvDimensionY, 1.0);
}

void AvImageWedge2::draw(AvCanvas &c)
{
AvViewport vp;

	c.getViewport(vp);
	width_ = vp.width();
	height_ = vp.height();
	scl_ = (float) max()/(float)(width()*height());
	AvRaster::draw(c);
}

void AvImageWedge2::draw(AvCanvas *c)
{
	if(c != NULL)
		draw(*c);
}


/*****************************************************************/
/*****************************************************************/
/*****************************************************************/

/* A raster that generates values between min and max.
dependent on the surrounding viewport.
*/
DEFINE_MEMBER_INFO(AvColorWedge)

AvColorWedge::AvColorWedge(const float min, const float max,
		const int width, const int height, const Boolean vertical)
	: AvRaster()
{
	INIT_CLASS(AvRaster);

	min_ = min;
	max_ = max;
	width_ = width;
	height_ = height;
	vertical_ = vertical;
	computeDelta();

	name("ColorWedge");
}

AvColorWedge::~AvColorWedge(){}

unsigned int AvColorWedge::width()const {return width_;}
unsigned int AvColorWedge::height()const{return height_;}

float AvColorWedge::getElement(const int x, const int y)
{
	if(vertical())
		return getElement_(y);
	else
		return getElement_(x);
}

void AvColorWedge::computeDelta()
{
float	s = (float)size();

	if(s == 0.0)
		s = 1;
	delta_ = (max() - min())/s;
#if 0
	// Update the axis info.
	if(vertical())
	{	yAxis()->initialize("", height_,
					(double)min_, (double)max_,NULL);
	}
	else
	{	xAxis()->initialize("", width_,
					(double)min_, (double)max_,NULL);
	}

	touch();
#endif
}

float AvColorWedge::min()const
{
	return min_;
}

void AvColorWedge::min(const float m)
{
	min_ = m;
	computeDelta();
}

float AvColorWedge::max()const
{
	return max_;
}

void AvColorWedge::max(const float m)
{
	max_ = m;
	computeDelta();
}

void AvColorWedge::setMinMax(const float min, const float max)
{
	if((min != min_) || (max != max_))
	{	min_ = min;
		max_ = max;
		computeDelta();
	}
}

void AvColorWedge::vertical(const Boolean vertical)
{
	vertical_ = vertical;
	computeDelta();
}

#if 1
void AvColorWedge::draw(AvCanvas& c)
{
	AvRaster::draw(c);
}

void AvColorWedge::draw(AvCanvas* c)
{
	if(c != NULL)
		draw(*c);
}
#endif

void AvColorWedge::setSize(const unsigned int width,
			   const unsigned int height)
{
	width_ = width;
	height_ = height;
	computeDelta();
}
