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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvGeometry.cc,v 19.0 2003/07/16 05:47:22 aips2adm Exp $
//
// $Log: AvGeometry.cc,v $
// Revision 19.0  2003/07/16 05:47:22  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:03  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:32  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:40  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:51  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:26  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:15  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:05  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:23  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:20  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:28:26  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:17:22  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:24:41  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.3  1996/06/18  18:46:02  pixton
//  Copyright Update
//
//  Revision 1.2  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvGeometry.C
*/

#include "AvGeometry.h"

static const AvAllotment one_(1,1);
static const AvAllotment zero_(0,0);

const AvAllotment& AvAllotment::one()
{

	return one_;
}

const AvAllotment& AvAllotment::zero()
{

	return zero_;
}

AvAllotment::AvAllotment()
{
	values_[0] = 0;
	values_[1] = 0;
}

AvAllotment::AvAllotment(const float x, const float y)
{
	values_[0] = x;
	values_[1] = y;
}

AvAllotment::AvAllotment(const float *vals)
{
	for(AvDimension d=AvDimensionX; d < AvDIMENSIONS; d++)
		values_[d] = *vals++;
}

#if 0
AvAllotment::AvAllotment(AvAllotment &r)
{
	for(AvDimension d=AvDimensionX; d < AvDIMENSIONS; d++)
		values_[d] = r.values_[d];
}
#endif

AvAllotment::AvAllotment(const AvAllotment &r)
{
	for(AvDimension d=AvDimensionX; d < AvDIMENSIONS; d++)
		values_[d] = r.values_[d];
}

float AvAllotment::value(const AvDimension d)const
{
	if(d < AvDIMENSIONS)
		return values_[d];
	else
		return 0.0;
}

void AvAllotment::value(const AvDimension d, const float val)
{
	if(d < AvDIMENSIONS)
		values_[d] = val;
}

#if 0
AvAllotment::abs()
{
	for(AvDimension d=AvDimensionX; d < AvDIMENSIONS; d++)
		if(values_[d] < 0.0)
			values_[d] = -values_[d];
}
#endif

// Are two requisitions (almost) equal?
Boolean AvAllotment::equals( const AvAllotment &req, const float epsilon)const
{
float	t1, t2;

	for(AvDimension d=AvDimensionX; d < AvDIMENSIONS; d++)
	{	t1 = values_[d];
		if(t1 < 0.0)
			t1 = -t1;
		t2 = req.values_[d];
		if(t2 < 0.0)
			t2 = -t2;
		if((t1 - t2) > epsilon)
			return FALSE;
	}
	return TRUE;
}

void AvAllotment::maximum(const AvAllotment &req)
{
	for(AvDimension d=AvDimensionX; d < AvDIMENSIONS; d++)
		if(values_[d] < req.values_[d])
			values_[d] = req.values_[d];
}

void AvAllotment::minimum(const AvAllotment &req)
{
	for(AvDimension d=AvDimensionX; d < AvDIMENSIONS; d++)
		if(values_[d] > req.values_[d])
			values_[d] = req.values_[d];
}

AvAllotment& AvAllotment::operator+=(const AvAllotment &r)
{
	for(AvDimension d=AvDimensionX; d < AvDIMENSIONS; d++)
		values_[d] += r.values_[d];
	return *this;
}

AvAllotment& AvAllotment::operator-=(const AvAllotment &r)
{
	for(AvDimension d=AvDimensionX; d < AvDIMENSIONS; d++)
		values_[d] -= r.values_[d];
	return *this;
}

AvAllotment& AvAllotment::operator*=(const AvAllotment &r)
{
	for(AvDimension d=AvDimensionX; d < AvDIMENSIONS; d++)
		values_[d] *= r.values_[d];
	return *this;
}

AvAllotment& AvAllotment::operator/=(const AvAllotment &r)
{
	for(AvDimension d=AvDimensionX; d < AvDIMENSIONS; d++)
		values_[d] /= r.values_[d];
	return *this;
}

// TRUE iff lower <= a <= upper.
Boolean AvAllotment::within(const AvAllotment &lower,
				const AvAllotment &upper)const
{
	for(int i=AvDimensionX; i< AvDIMENSIONS; i++)
	{ float v = value(i);

		if( (lower.value(i) > v) || (upper.value(i) < v))
			return FALSE;
	}
	return TRUE;
}

AvAllotment operator+(const AvAllotment &a, const AvAllotment &b)
{
AvAllotment v = a;

	v += b;
	return v;
}

AvAllotment operator-(const AvAllotment &a, const AvAllotment &b)
{
AvAllotment v = a;

	v -= b;
	return v;
}

AvAllotment operator*(const AvAllotment &a, const AvAllotment &b)
{
AvAllotment v = a;

	v *= b;
	return v;
}

AvAllotment operator/(const AvAllotment &a, const AvAllotment &b)
{
AvAllotment v = a;

	v /= b;
	return v;
}

///////////////////////////////////////////////////////////////////

static AvAllotment empty_allot;

AvAllocation::AvAllocation()
{
	for(int i= AvDimensionX; i< AvDIMENSIONS; i++)
	{	xy_.value(i, 0.0);
		wh_.value(i, 0.0);
	}
}

AvAllocation::AvAllocation(const AvAllocation &a)
{
	xy_ = a.xy_;
	wh_ = a.wh_;
}

AvAllotment &AvAllocation::allotment(const AvDimension d)
{
	if(d == AvDimensionX)
		return xy_;
	else
	if(d == AvDimensionY)
		return wh_;
	else
		return empty_allot;
}

void AvAllocation::allot(const AvDimension d, const AvAllotment &a)
{
	if(d == AvDimensionX)
		xy_ = a;
	else
	if(d == AvDimensionY)
		wh_ = a;
}

void AvAllocation::setStart(const AvAllotment &a)
{
	xy_ = a;
}

void AvAllocation::getStart(AvAllotment &a)const
{
	a = xy_;
}

void AvAllocation::setSpan(const AvAllotment &a)
{
	wh_ = a;
}

void AvAllocation::getSpan(AvAllotment &a)const
{
	a = wh_;
}

void AvAllocation::getEnd(AvAllotment &a)const
{
	a = xy_ + wh_ - one_;
}

void AvAllocation::setEnd(const AvAllotment &a)
{
	wh_ = a - xy_ + one_;	// Doesn't check for negative!?
}

Boolean AvAllocation::equals(const AvAllocation &a, const float epsilon)const
{
	return xy_.equals(a.xy_, epsilon) && wh_.equals(a.wh_, epsilon);
}

/* True if this contains the Allocation. (This box surrounds the
other box. (If both are the same they 'contain' each other).
*/
Boolean AvAllocation::contains(const AvAllocation &a)const
{
static AvAllotment end, aend;

	getEnd(end);
	a.getEnd(aend);
	return  a.xy_.within(xy_, end) && aend.within(xy_, end);
}

Boolean AvAllocation::contains(const AvAllotment &a)const
{
static AvAllotment end;

	getEnd(end);

	return a.within(xy_, end);
}

	// Various ways to think of the values.
float AvAllocation::x0()const
{
	return xy_.value(AvDimensionX);
}

float AvAllocation::y0()const
{
	return xy_.value(AvDimensionY);
}

float AvAllocation::width()const
{
	return wh_.value(AvDimensionX);
}

float AvAllocation::height()const
{
	return wh_.value(AvDimensionY);
}

float AvAllocation::x1()const
{
	return x0() + width();
}

float AvAllocation::y1()const
{
	return y0() + height();
}

void AvAllocation::x0(const float v)
{
	xy_.value(AvDimensionX, v);
}

void AvAllocation::y0(const float v)
{
	xy_.value(AvDimensionY, v);
}

void AvAllocation::width(const float v)
{
	wh_.value(AvDimensionX, v);
}

void AvAllocation::height(const float v)
{
	wh_.value(AvDimensionY, v);
}

void AvAllocation::x1(const float v)
{
	wh_.value(AvDimensionX, v-x0());
}

void AvAllocation::y1(const float v)
{
	wh_.value(AvDimensionY, v-y0());
}
