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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvPosition.h,v 19.0 2003/07/16 05:47:51 aips2adm Exp $
//
// $Log: AvPosition.h,v $
// Revision 19.0  2003/07/16 05:47:51  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:27  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:55  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:01  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:20  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:57  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:45  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:59  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:41  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:50  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:30:46  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.1  1997/08/01 21:17:10  hr
// Rewrite to use accessor rather than AvImageViewItem. Compound number is gone.
//
// Revision 8.0  1997/02/20 03:18:14  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:57:15  droberts
// Final update from monet archive.
//
// Revision 1.3  1996/09/18 19:31:56  hr
// Made some arguments const &/or pass by reference.
//
// Revision 1.2  1996/09/10 16:55:48  hr
// Initial changes for integration with AvAccessor & AvDataSet.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.4  1996/06/18  18:43:14  pixton
//  Copyright Update
//
//  Revision 1.3  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/05/05  14:50:01  hr
//  Added AvPosition(const AvPosition &).
//
//  Revision 1.1  1995/05/02  19:06:58  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

#ifndef AvPosition_h
#define AvPosition_h

//#include <X11/Intrinsic.h>
//#include <X11/Xresource.h>        /* Need Quark definition. */
//typedef XrmQuark  Quark;

#include <iostream.h>
#include <AvWPosition.h>
#include <AvAccessor.h>

/* Give a position including units.
AvPosition is used by AvImageWindow to convert points in one window to
points in other windows. (And other places).

No conversion is done on units (eg. no conversion between m/s & km/s).
*/

class AvPosition {
  public:
//	AvPosition();
	AvPosition(const AvPosition &);
	AvPosition( AvAccessor *ac);
	~AvPosition();
	// Return a pointer to a new position with units from ac, and values
	// from pos. Returns NULL if the x & y can't be set from pos.
	// (No units match X & Y units).
	static AvPosition *newPosition( AvAccessor *ac, const AvPosition &pos);
	void getXYZValues(double &x, double &y, double &z)const;
//	void setXYZValues(const double x, const double y, const double z);
//	void setXYValues(const double x, const double y);
	void setValue(const double val, const int index)
		      { values_(index) = val;}
	double getValue(const int i)const{return values_(i);}
	const AvWPosition &getValues()const {return values_;}
	// Set the first maxn values to val. (0 means all available).
	// If different lengths, only as many as will fit (or are available)
	// will be copied.
	void setValues(const AvWPosition &val, const int maxn=0);
	// Values in pos are copied to entries with corresponding unit fields.
	// (ie. RA_SIN to RA_SIN fields irrespective of which axis).
	// Returns the number of consecutive entries copied starting from 0.
	// (eg. If 2, then X & Y got copied).
	int assign(const AvPosition &pos);
	// Convert position to XYZ values.
	int WorldToXYZ(AvIPosition &xyz)const
			   {return accessor_->WorldToXYZ(values_, xyz);}
	// Convert x, y, and z from indexes in region to world values
	// and store.
	void XYZToWorld(const int x, const int y, const int z);
	// Returns TRUE if the XY (& Z) values are within the data set.
	// Returns 0 if no, else non zero.
	int validXY()const;
	int validXYZ()const;
	int length()const{return values_.nelements();}
	const char *units(const int i)const {return accessor_->axisName(i);}
  private:
	// For now, units are retrieved from the accessor.
	AvAccessor	*accessor_;
	AvWPosition	values_;
};


#endif
