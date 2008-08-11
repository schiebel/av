//# Copyright (C) 1995-97 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvViewport.cc,v 19.0 2003/07/16 05:46:48 aips2adm Exp $
//
// $Log: AvViewport.cc,v $
// Revision 19.0  2003/07/16 05:46:48  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:34  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:02  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:11  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:50  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:07:50  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:34  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:52  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:39  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:19  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:25:14  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:24  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 07:38:24  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.6  1996/06/18  18:46:41  pixton
//  Copyright Update
//
//  Revision 1.5  1996/02/14  22:27:09  hr
//  Removed offsets, and added {get,set}Position.
//
//  Revision 1.4  1996/01/26  21:09:39  hr
//  Made width/height const.
//
//  Revision 1.3  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/05/19  19:49:40  hr
//  Changed min/max scale factors.
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvViewport.C
Define a place to draw. x/y, width/height.
*/

#include <AvViewport.h>

// Arbitrary min/max scale values since scale of <= 0 is bad.
static float	minRasterScale_ = 1.0/1024.0, maxRasterScale_ = 1024.0;

AvViewport::AvViewport()
{
	port_.x0(0);
	port_.y0(0);
	port_.width(1);
	port_.height(1);
	rasterscale_.value(AvDimensionX, 1.0);
	rasterscale_.value(AvDimensionY, 1.0);
	vector_position_.value(AvDimensionX, 0.0);
	vector_position_.value(AvDimensionY, 0.0);
}

AvViewport::AvViewport( int x0, int y0,
			unsigned int width, unsigned int height)
{
	port_.x0(x0);
	port_.y0(y0);
	port_.width(width);
	port_.height(height);
	rasterscale_.value(AvDimensionX, 1.0);
	rasterscale_.value(AvDimensionY, 1.0);
	vector_position_.value(AvDimensionX, 0.0);
	vector_position_.value(AvDimensionY, 0.0);
}

void AvViewport::xy(const AvAllotment &r)
{
	port_.allot(AvDimensionX, r);
}

void AvViewport::wh(const AvAllotment &r)
{
	port_.allot(AvDimensionY, r);
}

int AvViewport::x0()
{
	return (int) (port_.x0());
}

void AvViewport::x0(int x)
{
	port_.x0(x);
}

int AvViewport::y0()
{
	return (int) (port_.y0());
}

void AvViewport::y0(int y)
{
	port_.y0(y);
}

unsigned int AvViewport::width()const
{
	return (int) (port_.width());
}

void AvViewport::width(unsigned int x)
{
	port_.width(x);
}

unsigned int AvViewport::height()const
{
	return (int) (port_.height());
}

void AvViewport::height(unsigned int y)
{
	port_.height(y);
}

/* Merge two viewports. The current viewport essentially becomes the child
viewport that results from the subset of the argument and the current vp.

	new x/y are this + current.
	new width/height are min of this, current and available
(Resulting viewport is constrained to be within current).
	Rasterscale is product of the two.
	vector position is that of the child.

	The resulting viewport will be at least 1x1 in size.
NOTE:
	This should only be called once/object-level since the x/y positions
	are cumulative.
*/
void AvViewport::merge(AvViewport &vp)
{
static AvAllotment newxy, newwh, end; // Don't reinit all the time.

	// New offset.
//	newxy = port_.xy() + vp.xy() + vp.offset();
	newxy = port_.xy() + vp.xy();
	// Make sure it's within range.
	port_.getEnd(end);
	// The resulting viewport can't start on the right edge (it must be
	//  at least 1x1 in size).
//	newxy.minimum(end - AvAllotment::one());
	newxy.minimum(end);

	// New size is minimum of requested and what's available.
	newwh = end - newxy + AvAllotment::one();	// What's available.
	newwh.minimum(vp.wh());

	// Set new values.
	xy(newxy);
	wh(newwh);
	rasterscale_ *= vp.rasterscale_;
	vector_position_ = vp.vector_position_;
}

Boolean AvViewport::contains(const AvAllocation &a)const
{
	return port_.contains(a);
}

Boolean AvViewport::contains(const AvAllotment &a)const
{
	return port_.contains(a);
}

void AvViewport::setRasterScale( const float xscl, const float yscl)
{
	if((xscl >= minRasterScale()) && (xscl <= maxRasterScale()))
		rasterscale_.value(AvDimensionX, xscl);
	if((yscl >= minRasterScale()) && (yscl <= maxRasterScale()))
		rasterscale_.value(AvDimensionY, yscl);
}

void AvViewport::setRasterScale( const float scl)
{
	setRasterScale(scl, scl);
}

void AvViewport::getRasterScale( float &xscl, float &yscl)
{
	xscl = rasterscale_.value(AvDimensionX);
	yscl = rasterscale_.value(AvDimensionY);
}

void AvViewport::setPosition( const int x, const int y)
{
	port_.x0(x);
	port_.y0(y);
}

void AvViewport::getPosition( int &x, int &y)const
{
	x = (int) (port_.y0());
	y = (int) (port_.y0());
}

void AvViewport::getScale( float &xscl, float &yscl)const
{
	xscl = rasterscale_.value(AvDimensionX);
	yscl = rasterscale_.value(AvDimensionY);
}

void AvViewport::vectorPosition(AvAllotment &newposition)
{
	vector_position_ = newposition;
}

float AvViewport::minRasterScale()
{
	return minRasterScale_;
}

float AvViewport::maxRasterScale()
{
	return maxRasterScale_;
}

Boolean AvViewport::toVP(const AvAllotment &input, AvAllotment &output,
						const Boolean scale)const
{
	output = input - xy();		// Remove offset.
	if(scale)
		output *= rasterscale_;
	return contains(output);
}

Boolean AvViewport::fromVP(const AvAllotment &input, AvAllotment &output,
						const Boolean scale)const
{
	if(scale)
		output = input / rasterscale_;
	else
		output = input;
	output += xy();

	return contains(input);
}
