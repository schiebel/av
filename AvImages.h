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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvImages.h,v 19.0 2003/07/16 05:47:35 aips2adm Exp $
//
// $Log: AvImages.h,v $
// Revision 19.0  2003/07/16 05:47:35  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:13  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:43  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:49  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:34  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:40  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:29  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:30  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:58  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:00  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/09 21:16:44  hr
// Removed unneeded semicolon from DECLARE_HEADER.
//
// Revision 9.0  1997/08/25 21:29:27  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:18:02  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 09:57:05  droberts
// Final update from monet archive.
//
// Revision 1.2  1996/11/12 20:15:42  hr
// Removed unneeded getRegion() call.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.7  1996/06/18  18:43:05  pixton
//  Copyright Update
//
//  Revision 1.6  1996/02/23  17:33:21  hr
//  Added the colorwedge.
//
//  Revision 1.5  1995/10/06  15:49:24  hr
//  Changed AvImageWedge2 to support getRow and changed its constructor
//  interface.
//
//  Revision 1.4  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/06/09  19:54:07  hr
//  added an 'int' to lineWidth().
//
//  Revision 1.2  1995/06/05  19:20:00  hr
//  Added getElement and lineWidth functions.
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvImages.h

Examples images
*/


#ifndef AvImages_H
#define AvImages_H

#include "AvImage.h"

class  AvImageCross : public AvRaster {
  public:
	AvImageCross(unsigned int width, unsigned int height,
		float on, float off);
	~AvImageCross();
	virtual float getElement(const int x, const int y);
	unsigned int width()const;
	unsigned int height()const;
	float min()const;
	void min(const float);
	float max()const;
	void max(const float);
	inline int lineWidth()const{return lineWidth_;}
	void lineWidth(const int l);
  protected:
	float on_, off_;
	unsigned int width_, height_;
	int	lineWidth_;
	DECLARE_HEADER
};

class  AvImageWedge : public AvImage {
  public:
	AvImageWedge(unsigned int width, unsigned int height,
		unsigned long max);
	~AvImageWedge();
  protected:
	DECLARE_HEADER
};

// Creates a wedge that is the size of its viewport.
// 

class  AvImageWedge2 : public AvRaster {
  public:
	AvImageWedge2( const float maxValue);
	~AvImageWedge2();
	virtual void getRow(float *row, const int x, const int y,
			const int num=0, const int incr=1);
	unsigned int width() const;
	unsigned int height()const;
	float min()const;
	float max()const;
	void min(const float);
	void max(const float);
	void getBoundingBox(AvCanvas &c, AvAllotment &);
	void draw(AvCanvas &c);
	void draw(AvCanvas*);
  private:
	float max_;
	// Computed from viewport when draw is called.
	unsigned int width_;
	unsigned int height_;
	float scl_;
	DECLARE_HEADER
};

//
class  AvColorWedge : public AvRaster {
  public:
	AvColorWedge(const float min, const float max,
		     const int width, const int height,
		     const Boolean vertical=TRUE);
	~AvColorWedge();
	virtual float getElement(const int x, const int y);
	virtual float min()const;
	void min(const float);
	virtual float max()const;
	virtual void max(const float);
	virtual unsigned int width()const;
	virtual unsigned int height()const;

	inline Boolean vertical()const {return vertical_;}
	void vertical(const Boolean);

	virtual void draw(AvCanvas&);
	virtual void draw(AvCanvas*);
	void setSize(const unsigned int width, const unsigned int height);
	void setMinMax(const float min, const float max);
	inline unsigned int size()const
				{return vertical() ? height() : width();}
  private:
	void computeDelta();
	inline float getElement_(const int n)const {return min_ + delta_*n;}
  protected:
	float		min_, max_, delta_;
	unsigned int	width_, height_;
	Boolean		vertical_;
	DECLARE_HEADER
};
#endif
