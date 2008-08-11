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
//---------------------------------------------------------------------------
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvAxisVPS.h,v 19.0 2003/07/16 05:46:31 aips2adm Exp $
//
// $Log: AvAxisVPS.h,v $
// Revision 19.0  2003/07/16 05:46:31  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:18  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:41:46  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:41:57  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:07:48  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:07:31  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:11  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:16  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:57:49  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:51:19  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/09 21:12:46  hr
// Removed unneeded semicolon from DECLARE_HEADER.
//
// Revision 9.0  1997/08/25 21:23:39  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:14:29  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/01/24 20:13:46  hr
// Added AvAxisOptions argument to constructor.
//
// Revision 7.2  1996/12/12 05:39:32  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.2  1996/06/18  18:42:37  pixton
//  Copyright Update
//
//  Revision 1.1  1996/02/23  17:40:37  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* 
 AvAxisVPS is a subclass of AvViewportSetter that expects to manage
two objects. The first is a viewport setter typically containing a
raster object. The second is an AvAxis that is meant to surround the
first.

When the axis is enabled, the size of the AVPS is increased to support
the space needed for the labels. The raster's vps is moved inward by
the margins.

Example:
		AVPS
		| |
		| ----- Axis, ...
		VPR
		|
		|- Raster
		.- <possibly more>

AVPS	Axis viewportSetter
VPR	viewportSetter containing a raster and, perhaps, a contour. 

It should be possible to replace the raster with something else, but
the size may have to be explicitly set and it hasn't been tried yet.


	------------
	|    tm	    |
	|  -------  |
	|  |	 |  |
	|lm| ras |rm|
	|  |	 |  |
	|  ------   |
	|    bm	    |
	------------

Labels are drawn in the margins.

*/


#ifndef _AV_AXISVPS_H
#define _AV_AXISVPS_H

#include "AvViewportSetter.h"

class	AvAxis;
class	AvRaster;
class	AvAxisOptions;

class AvAxisVPS : public AvViewportSetter {

  public:

	// Width and height give the size of the scaled raster (no margins).
	AvAxisVPS( AvViewportSetter *vps, AvAxis *axis = NULL,
	 const int lm=0, const int rm=0, const int tm=0, const int bm=0);
	AvAxisVPS( AvRaster *ras, AvAxisOptions *o,
	 const int lm=0, const int rm=0, const int tm=0, const int bm=0);

	void getMargins(int &lm, int &rm, int &tm, int &bm)const;
	// horiz is lm+rm. vert = tm+bm.
	void getMargins(int &horiz, int &vert)const;
	void setMargins(const int lm, const int rm,
						const int tm, const int bm);

	void showAxis(const Boolean show);
	Boolean showAxis()const;

	// Usually the axisVPS gets its size from the children. If this
	// is set, size will need to be set externally.
	void computeSize(const Boolean);
	Boolean computeSize()const{return computeSize_;}

	// Functions to retrieve display objects.
	AvViewportSetter *getRasterVPS()const;
	AvRaster *getRaster()const;
	AvAxis *getAxis()const;
	virtual void getBoundingBox(AvCanvas &c, AvAllotment &r);
	// The Canvas calls draw with a pointer to itself to tell the object
	// to display itself.
	virtual void draw(AvCanvas &);
	virtual void draw(AvCanvas *);
  protected:
	virtual ~AvAxisVPS ();
  private:
	void init(const int lm=0, const int rm=0,
		  const int tm=0, const int bm=0);
	void setAxisMargins();
	void setRasterVPSOffset();
	void update(AvCanvas &c);
	// Routines to return pointers to various children that need tweaking.
	AvGraphics2D *getChild(	const int childID,
				const AvViewportSetter *vps=NULL)const;
  private:
	DECLARE_HEADER
	Boolean	computeSize_;			// How to determine size.
	int	lm_, rm_, tm_, bm_;		// Axis margins.
	Boolean	wasShowingAxis_;
};

#endif
