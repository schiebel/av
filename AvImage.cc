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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvImage.cc,v 19.0 2003/07/16 05:47:33 aips2adm Exp $
//
// $Log: AvImage.cc,v $
// Revision 19.0  2003/07/16 05:47:33  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:12  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:41  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:48  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:27  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:38  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:26  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:27  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:52  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:53  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/09 21:20:35  hr
// Removed unneeded semicolon after DEFINE_MEMBER_INFO macro.
//
// Revision 9.0  1997/08/25 21:29:17  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:17:56  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/02/09 00:47:11  hr
// A number of of signed/unsigned comparisions.
//
// Revision 7.2  1996/12/12 08:56:49  droberts
// Final update from monet archive.
//
// Revision 1.3  1996/11/12 20:10:47  hr
// Removed getRegion().
//
// Revision 1.2  1996/08/14 18:48:49  hr
// Changes to remove g++ warnings (from bglenden).
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.5  1996/06/18  18:46:09  pixton
//  Copyright Update
//
//  Revision 1.4  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/06/09  19:55:08  hr
//  getElement wasn't always returning a value.
//
//  Revision 1.2  1995/06/05  19:17:48  hr
//  Changed to use getElement rather than getRegion.
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvImage.C
*/
#include "AvImage.h"

//static char *Name = "AvImage";
DEFINE_MEMBER_INFO(AvImage)

/***********************************************************************/
/***********************************************************************/

void AvImage::init(float *data, unsigned int width, unsigned int height)
{
	INIT_CLASS(AvRaster);
	if(width == 0)
		width = 1;
	if(height == 0)
		height = 1;
	width_ = width;
	height_ = height;
	data_ = data;
	min_ = 0;
	max_ = -1;
}

AvImage::AvImage(float *data, unsigned int width, unsigned int height)
{
	init(data, width, height);
	mydata_ = FALSE;
}

AvImage::AvImage(unsigned int width, unsigned int height)
{
float	*data;
int	n;

	n = width*height;
	data = (n == 0) ? (float *)NULL : new float[width*height];
	init(data, width, height);
	mydata_ = TRUE;
}

AvImage::~AvImage()
{
	if(mydata_ && (data_ != NULL))
		delete [] data_;
}

unsigned int AvImage::width()const { return width_;}
unsigned int AvImage::height()const { return height_;}

float AvImage::min()const
{
	if(min_ >= max_)
	{ AvImage *that = (AvImage *)this;
		that->findMinMax();	//Lie
	}
	return min_;
}

void AvImage::min(const float m)
{
	min_ = m;
}

float AvImage::max()const
{
	if(min_ >= max_)
	{ AvImage *that = (AvImage *)this;
		that->findMinMax();	//Lie
	}
	return max_;
}

void AvImage::max(const float m)
{
	max_ = m;
}

void AvImage::findMinMax(int xs, int ys, unsigned int w, unsigned int h)
{
unsigned int	r, c;
float	*d, v, low, hi;
unsigned int wdth;

	rangeCheck(xs, ys, w, h);
	wdth = width();
	low = hi = *(data_+ys*wdth + xs);	// Initial values.

	for(r=ys; r< ys+h; r++)
	{	d = data_+(r*wdth);	// Point to row.
		for(c=xs; c< xs+w; c++)
		{	v = *d++;
			if(v < low)
				low = v;
			else
			if( v > hi)
				hi = v;
		}
	}
	min(low);
	max(hi);
}

void AvImage::putRegion(float *data, int xs, int ys,
		unsigned int w, unsigned int h)
{
unsigned int r, c;
float	*d;
unsigned int wdth;

	rangeCheck(xs, ys, w, h);
	wdth = width();

	for(r=ys; r< ys+h; r++)
	{	d = data_+(r*wdth);	// Point to row.
		for(c=xs; c< xs+w; c++)
			*d++ = *data++;
	}
}

float AvImage::getElement(const int x, const int y)
{
	if((x >= 0) && (x < (int)width_) && (y >= 0) && (y < (int)height_))
		return data_[y*width_ + x];
	return 0.0;
}


void AvImage::getRow(float *row, const int X, const int Y,
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
		{	*row++ = getElement_(x, y);
			x += incr;
		}
	}
	else	// Right to left.
	{	if( x1 < 0)	// End point is < start point.
			x1 = 0;
		while(x >= x1)
		{	*row++ = getElement_(x, y);
			x += incr;
		}
	}
}

void AvImage::size(AvAllotment &r, AvAllotment *)
{
	r.value(AvDimensionX, width());
	r.value(AvDimensionY, height());
}
