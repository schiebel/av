//# Copyright (C) 1995-2003 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvRaster.cc,v 19.1 2003/10/01 21:35:39 hravlin Exp $
//
// $Log: AvRaster.cc,v $
// Revision 19.1  2003/10/01 21:35:39  hravlin
// Get the blanking value from Av::BlankedValue rather than a constant.
//
// Revision 19.0  2003/07/16 05:47:44  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:21  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/30 22:42:53  hravlin
// AvRaster's copy constructor initializes its base class.
//
// Revision 17.0  2001/11/12 19:42:50  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:56  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:00  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:50  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:38  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:46  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:22  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:28  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/09 21:35:18  hr
// Removed a number of extra semicolons.
//
// Revision 9.0  1997/08/25 21:30:12  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:18:31  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.4  1997/02/09 00:54:40  hr
// Fixed signed/unsigned comparisions.
//
// Revision 7.3  1996/12/12 09:40:05  droberts
// Final update from monet archive.
//
// Revision 1.4  1996/11/12 20:20:27  hr
// Removed unused code.
// Rewrote getRegion() call.
//
// Revision 1.3  1996/09/26 14:24:46  hr
// Removed some unneeded "break"s to keep DCC from bitching.
//
// Revision 1.2  1996/09/20  18:41:29  hr
// Rewrote axisInfo to be usable with AvAccessor.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.11  1996/06/18  18:46:30  pixton
//  Copyright Update
//
//  Revision 1.10  1996/03/20  16:53:30  hr
//  axisInfo::valueStr wasn't creating a buffer if passed a NULL pointer.
//
//  Revision 1.9  1996/03/18  16:01:23  pixton
//  Workaround in AvRaster::AvValueFormatter::format (grep "Type of conversion to do").
//
//  Revision 1.8  1996/02/08  18:57:32  hr
//  Changed default format strings for time and float.
//
//  Revision 1.7  1996/02/02  20:51:27  hr
//  Added {get,set}UnitString(). Changed default formats to include a
//  minimum field width.
//
//  Revision 1.6  1996/01/08  16:30:12  hr
//  Forgot stdio for sprintf prototype.
//
//  Revision 1.5  1996/01/08  16:27:00  hr
//  Added axisInfo and AvValueFormatter.
//
//  Revision 1.4  1995/10/04  21:12:00  hr
//  Added functions to supply value to use as blanked data.
//
//  Revision 1.3  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/04/24  18:33:11  hr
//  Added beginAccess, endAccess, getRow.
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvRaster.cc

axisInfo could be a standalone class, although it might be a good idea to
make it a subclass of AvResource.
*/

#include <AvRaster.h>
#include <AvCanvas.h>

#include <stdio.h>		// sprintf

#if 0
#ifndef STANDALONE
#include <Av.h>			// For BlankedValue.
#else
#include <math.h>
#define	BLANKEDVALUE	MAXFLOAT
#endif
#else
#include <Av.h>
#endif

//static char *Name = "AvRaster";
DEFINE_MEMBER_INFO(AvRaster)

AvRaster::AvRaster()
{
	INIT_CLASS(AvGraphics2D);
	axisinfo_ = NULL;
}

AvRaster::AvRaster(AvRaster & /*ras*/) : AvGraphics2D()
{
	INIT_CLASS(AvGraphics2D);
	axisinfo_ = NULL;
}

AvRaster::~AvRaster()
{
	AvResource::UnRef(axisinfo_);
	axisinfo_ = NULL;
}

void AvRaster::draw(AvCanvas& d)
{
	if(!ignore())
		d.drawRaster(this);
}

void AvRaster::draw(AvCanvas* d)
{
	if((d != NULL) && !ignore())
		d->drawRaster(this);
}

// Ensure that the start point and size is valid.
void AvRaster::rangeCheck(int &x, int &y,
			  unsigned int &w, unsigned int &h)const
{
unsigned int wdth, hght;

	wdth = width();
	hght = height();

	if(x< 0)
		x = 0;
	else
	if(x >= (int)wdth)
		x = wdth-1;
	if(y< 0)
		y = 0;
	else
	if(y >= (int)hght)
		y = hght-1;

	// h/w of 0 mean use available space.
	if((w == 0) || (w > wdth-x))
		w = wdth-x;
	if((h == 0) || (h > hght-y))
		h = hght-y;
}

float AvRaster::getElement(const int /*x*/, const int /*y*/)
{
	return 0.0;
}

void AvRaster::getRow(float *row, const int X, const int Y,
					const int Num, const int Incr)
{
int x=X, y=Y, num=Num, incr=Incr;
int x1, w = width();

	if( (x < 0) || ( x >= w))
		return;
	if( (y < 0) || ( y >= (int)height()))
		return;

	if(incr == 0)
		incr = 1;

	if(num <= 0)
		num = w;

	x1 = x + incr*(num-1);	// endpoint.

	if(incr > 0) // Left to right.
	{	if(x1 >= w)	// End point is > start point.
			x1 = w -1;
		while(x <= x1)
		{	*row++ = getElement(x, y);
			x += incr;
		}
	}
	else	// Right to left.
	{	if( x1 < 0)	// End point is < start point.
			x1 = 0;
		while(x >= x1)
		{	*row++ = getElement(x, y);
			x += incr;
		}
	}
}

// Get nrows (sub)rows of width width starting at position x/y.
// Stride > 1 -> skip pixels. stride <= 0 is illegal.
void AvRaster::getRegion(float *data, const int x, const int y,
			 const unsigned int width,
			 const unsigned int nrows,
			 const int xstride, const int ystride)
{
int endRow = y + nrows*ystride, rownum;

	for(rownum = y; rownum < endRow; rownum += ystride)
	{	getRow(data, x, rownum, width, xstride);
		data += width;
	}
}

// Called before first call to getRow/getRegion
void AvRaster::beginAccess(){}
// Called after last call to getRow/getRegion.
void AvRaster::endAccess(){}

void AvRaster::min(const float){}
float AvRaster::min()const {return 0;}
void AvRaster::max(const float){}
float AvRaster::max()const{return 0;}

unsigned int AvRaster::width()const {return axisinfo()->length(XAXIS);}
unsigned int AvRaster::height()const{return axisinfo()->length(YAXIS);}

// Return the desired size scaled by the current raster scale.
void AvRaster::getBoundingBox(AvCanvas &c, AvAllotment &r)
{
	r.value(AvDimensionX, width());
	r.value(AvDimensionY, height());
	scaleBoundingBox(c, r);
}

// This scales the bounding box values by the current raster scale.
// Each subclassed version of Raster's getBounding box should call this after
// setting the unscaled size.
void AvRaster::scaleBoundingBox(AvCanvas &c, AvAllotment &r)
{
unsigned int	w,h;

	// Pick up the values.
	w = (unsigned int) r.value(AvDimensionX);
	h = (unsigned int) r.value(AvDimensionY);
	// Scale them.
	w = c.scaleSize(AvDimensionX, w);
	h = c.scaleSize(AvDimensionY, h);
	// Save them.
	r.value(AvDimensionX, w);
	r.value(AvDimensionY, h);
}

Boolean AvRaster::contains(const AvAllotment &a)const
{
Boolean	hit=FALSE;
static AvAllocation ras;

	ras.width(width());
	ras.height(height());
	hit = ras.contains(a);
	return hit;
}

Boolean AvRaster::isHit(AvCanvas &, const AvAllotment &a)const
{
	return contains(a);
}

void AvRaster::setAxisInfo(axisInfo *ri)
{
	AvResource::UnRef(axisinfo_);
	axisinfo_ = ri;
	AvResource::Ref(axisinfo_);
}

// Get/set value used for indicating blanked data. Must be a valid
// float since on some systems NaN will cause problems.
float AvRaster::getBlankValue()const
{
	return Av::BlankedValue;
}

AvRaster::axisInfo *AvRaster::axisinfo()const
{
	return axisinfo_;
}

AvRaster::axisInfo::axisInfo()
{
}

AvRaster::axisInfo::~axisInfo()
{
}


const char *AvRaster::axisInfo::label(const int axis)const
{
	switch (axis){
	case 0:
		return "X axis";
//		break;
	case 1:
		return "Y axis";
//		break;
	case 2:
		return "Z axis";
//		break;
	default:
		return "Unknown axis";
//		break;
	}
}

const char *AvRaster::axisInfo::unitString(const int axis)const
{
	switch (axis){
	case 0:
		return "X units";
//		break;
	case 1:
		return "Y units";
//		break;
	case 2:
		return "Z units";
//		break;
	default:
		return "Unknown axis units";
//		break;
	}
}

int AvRaster::axisInfo::length(const int /*axis*/)const
{
	return 0;
}

Boolean AvRaster::axisInfo::validIndex(const int , const int )const
{
	return TRUE;
}

double AvRaster::axisInfo::minValue(const int /*axis */)const
{
	return 0.0;
}

double AvRaster::axisInfo::maxValue(const int /*axis */)const
{
	return 1.0;
}

// Return pointer to a buffer containing a string representing the
// value of the axis at the given position.
const char *AvRaster::axisInfo::valueStr(const int axis, const double val)const
{
static char buf[80];
	sprintf(buf, "%d:%g", axis, val);
	return buf;
}

double AvRaster::axisInfo::XYtoWorld(const int axis, const int x, const int y)
									const
{
	return (double)(axis*x*y);
}

AvRaster::axisInfo::PRINTFORMAT
	AvRaster::axisInfo::printFormat(const int /*axis*/)const
{
	return AvRaster::axisInfo::DEFAULT;
}


const char *AvRaster::axisInfo::title()const
{
	return NULL;
}
