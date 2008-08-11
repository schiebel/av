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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvImage.h,v 19.0 2003/07/16 05:47:18 aips2adm Exp $
//
// $Log: AvImage.h,v $
// Revision 19.0  2003/07/16 05:47:18  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:59  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:29  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:36  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:36  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:22  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:10  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:57  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:10  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:05  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/09 21:16:28  hr
// Removed unneeded semicolon from DECLARE_HEADER.
//
// Revision 9.0  1997/08/25 21:28:03  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:17:07  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 10:02:44  droberts
// Final update from monet archive.
//
// Revision 1.2  1996/11/12 20:10:47  hr
// Removed getRegion().
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.4  1996/06/18  18:42:59  pixton
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
//  Revision 1.2  1995/06/05  19:19:23  hr
//  Added getElement/getRow functions.
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvImage.h

AvImage		An AvRaster that stores the image in an internal array.
		If a pointer to the image is passed in as an argument of to
		the constructor, the data is not copied. When the destructor
		is called it will not be freed.
*/


#ifndef AvImage_H
#define AvImage_H

#include "AvRaster.h"

class  AvImage : public AvRaster {
  public:
	AvImage(float *data, unsigned int width, unsigned int height);
	AvImage(unsigned int width, unsigned int height);
	// data is assumed to be w elements wide by h elements high.
	virtual void putRegion(float *data, int x=0, int y=0,
			unsigned int w=0, unsigned int h=1);
	virtual float getElement(const int x, const int y);
	/* Get one row of data.
	  num = 0 implies width()-x, incr =1 is get every element. 2 is
	  every other. -2 is every other element starting at x+num-1 and
	  going right to left. The number of elements returned will be
	  limited to one row.
	*/
	virtual void getRow(float *row, const int x, const int y,
			const int num=0, const int incr=1);
	virtual float min()const;
	virtual void min(const float);
	virtual float max()const;
	virtual void max(const float);
	unsigned int width()const;
	unsigned int height()const;
	// Return desired size.
	virtual void size(AvAllotment &r, AvAllotment *ref);
	// Compute min/max for a subregion.
	virtual void findMinMax(int xs=0, int ys=0,
			unsigned int w=0, unsigned int h=0);
  protected:
	~AvImage();
	void init(float *data, unsigned int width, unsigned int height);
	// Return one element w/o checking.
	inline float getElement_(const int x, const int y)const
					{return *(data_+x+y*width_);}
	unsigned int width_;
	unsigned int height_;
	float	min_, max_;
	float	*data_;
	char	mydata_;
	DECLARE_HEADER
};

#endif
