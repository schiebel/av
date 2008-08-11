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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvGeometry.h,v 19.0 2003/07/16 05:47:06 aips2adm Exp $
//
// $Log: AvGeometry.h,v $
// Revision 19.0  2003/07/16 05:47:06  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:50  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:18  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:26  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:53  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:10  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:57  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:32  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:36  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:25  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:26:59  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:26  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:25:24  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.3  1996/06/18  18:42:53  pixton
//  Copyright Update
//
//  Revision 1.2  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvGeometry.h
Defines geometry information for the Display routines.

*/
#ifndef AvGeometry_H
#define AvGeometry_H
#include <X11/Intrinsic.h>	// Defines Boolean, TRUE, FALSE

typedef unsigned int AvDimension;

enum {	AvDimensionX, AvDimensionY, Av2D, AvDimensionZ=Av2D, Av3D,
					AvUndefinedDimension=Av3D};

enum {AvDIMENSIONS=Av2D};

#ifndef NULL
#define NULL 0
#endif

class AvAllotment{
  public:
	AvAllotment();
//	AvAllotment(AvAllotment &);
	AvAllotment(const AvAllotment &);
	AvAllotment( const float x, const float y);
	AvAllotment( const float *valarray);
	AvAllotment& operator+=(const AvAllotment &);
	AvAllotment& operator-=(const AvAllotment &);
	AvAllotment& operator*=(const AvAllotment &);
	AvAllotment& operator/=(const AvAllotment &);
	friend AvAllotment operator+(const AvAllotment &, const AvAllotment &);
	friend AvAllotment operator-(const AvAllotment &, const AvAllotment &);
	friend AvAllotment operator*(const AvAllotment &, const AvAllotment &);
	friend AvAllotment operator/(const AvAllotment &, const AvAllotment &);
	float value(const AvDimension d)const ;
	void value(const AvDimension d, const float v);
	Boolean equals(const AvAllotment &a, const float epsilon)const;
	// True if this is within a & b. (a <= x <= b);
	Boolean within(const AvAllotment &a, const AvAllotment &b)const;
	// Set this to maximum of this and a.
	void maximum(const AvAllotment &a);
	void minimum(const AvAllotment &a);
	// A couple of useful constants.
	static const AvAllotment& one();
	static const AvAllotment& zero();
	inline float x()const {return values_[AvDimensionX];}
	inline float y()const {return values_[AvDimensionY];}
  private:
	float values_[AvDIMENSIONS];
};

/* Holds two Allotments, a starting position and a width & height.
   The first allotment gives the upper left point, the 2nd
   the width & height.
*/
class AvAllocation {
  public:
	AvAllocation();
	AvAllocation(const AvAllocation &);
	AvAllotment &allotment(const AvDimension);
	inline AvAllotment &xy() { return xy_;}
	inline const AvAllotment &xy() const { return xy_;}
	AvAllotment &wh() { return wh_;}
	AvAllotment const &wh()const { return wh_;}
	void allot(const AvDimension, const AvAllotment &);
	// start point.
	void getStart(AvAllotment &)const;
	void setStart(const AvAllotment &);
	// end point
	void getEnd(AvAllotment &)const;
	void setEnd(const AvAllotment &);
	// width/height.
	void getSpan(AvAllotment &)const;
	void setSpan(const AvAllotment &);
	Boolean equals(const AvAllocation &, const float epsilon)const;
	// True if this contains the Allocation.
	Boolean contains(const AvAllocation &)const;
	// True if the point is withing this Allocation's boundaries.
	Boolean contains(const AvAllotment &)const ;
	// Various ways to think of the values.
	float x0()const, y0()const, x1()const, y1()const;
	float width()const, height()const;
	void x0(const float), y0(const float);
	void width(const float), height(const float);
	void x1(const float), y1(const float);
  private:
	AvAllotment	xy_;
	AvAllotment	wh_;
	Boolean validDimension(const AvDimension)const;
	enum {NVALUES=2};
};

inline Boolean AvAllocation::validDimension(const AvDimension d)const
{	return (d < NVALUES);
}
#endif
