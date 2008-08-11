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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvViewportSetter.h,v 19.0 2003/07/16 05:46:56 aips2adm Exp $
//
// $Log: AvViewportSetter.h,v $
// Revision 19.0  2003/07/16 05:46:56  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:41  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:10  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:18  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:18  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:07:59  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:43  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:10  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:06  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:49  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.2  1998/06/09 21:17:11  hr
// Removed unneeded semicolon from DECLARE_HEADER.
//
// Revision 9.1  1998/05/20 21:41:58  hr
// Removed constructors with more than one pointer to AvGraphics2D * to
// prevent possible ambiguity problems with some compilers.
//
// Revision 9.0  1997/08/25 21:26:02  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:52  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 07:42:05  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.7  1996/06/18  18:43:29  pixton
//  Copyright Update
//
//  Revision 1.6  1996/02/14  22:27:48  hr
//  Removed offsets, and added {get,set}Position.
//
//  Revision 1.5  1996/01/29  20:36:53  hr
//  Added get/setOffsets.
//
//  Revision 1.4  1996/01/26  21:11:24  hr
//  Added getSize/setSize.
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
//  Revision 1.2  1995/04/26  19:42:21  hr
//  Made setRasterScale virtual.
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvViewportSetter.h
Graphics2D object to change viewport.
Implemented as a DisplayList object.

*/
#ifndef AvViewportSetter_H
#define AvViewportSetter_H
#include "AvDisplayList.h"
#include "AvViewport.h"

class AvViewportSetter : public AvDisplayList {
  public:
#if 0
	AvViewportSetter(
	 AvGraphics2D* =NULL,AvGraphics2D* =NULL,AvGraphics2D* =NULL,
	 AvGraphics2D* =NULL,AvGraphics2D* =NULL,AvGraphics2D* =NULL,
	 AvGraphics2D* =NULL,AvGraphics2D* =NULL,AvGraphics2D* =NULL,
	 AvGraphics2D * =NULL);

	AvViewportSetter(float x, float y,
	 AvGraphics2D* =NULL,AvGraphics2D* =NULL,AvGraphics2D* =NULL,
	 AvGraphics2D* =NULL,AvGraphics2D* =NULL,AvGraphics2D* =NULL,
	 AvGraphics2D* =NULL,AvGraphics2D* =NULL,AvGraphics2D* =NULL,
	 AvGraphics2D * =NULL);
	AvViewportSetter( float x, float y, float width, float height,
	 AvGraphics2D* =NULL,AvGraphics2D* =NULL,AvGraphics2D* =NULL,
	 AvGraphics2D* =NULL,AvGraphics2D* =NULL,AvGraphics2D* =NULL,
	 AvGraphics2D* =NULL,AvGraphics2D* =NULL,AvGraphics2D* =NULL,
	 AvGraphics2D * =NULL);
	AvViewportSetter(const AvViewport &vp,
	 AvGraphics2D* =NULL,AvGraphics2D* =NULL,AvGraphics2D* =NULL,
	 AvGraphics2D* =NULL,AvGraphics2D* =NULL,AvGraphics2D* =NULL,
	 AvGraphics2D* =NULL,AvGraphics2D* =NULL,AvGraphics2D* =NULL,
	 AvGraphics2D * =NULL);
#else
	AvViewportSetter( AvGraphics2D * =NULL);
	AvViewportSetter(const float x, const float y);
	AvViewportSetter(const float x, const float y,
			 const float width, const float height);
	AvViewportSetter(const AvViewport &vp);
#endif
	virtual void setViewport(AvViewport &vp);
	virtual void getViewport(AvViewport &vp);
	AvViewport *viewport();
	// Sets the current vp into the canvas.
	virtual void setViewport(AvCanvas &);
	virtual void setRasterScale( const float xscl, const float yscl);
	// x/y scales are the same.
	virtual void setRasterScale( const float scl);
	///
	virtual void draw(AvCanvas &);
	virtual void draw(AvCanvas *);
	virtual void getBoundingBox(AvCanvas &c, AvAllotment &r);
	virtual void handleEvent(AvCanvas &, AvEvent *);
	virtual Boolean isHit(AvCanvas &c, const AvAllotment &)const;
	virtual void getPickPaths(AvCanvas &, const AvAllotment &,
							AvPathList &);
	// Convert to vp coords (by subtracting offset). Resultant point
	// is not guaranteed to be within the viewport's boundaries.
	void toVPCoords(const AvAllotment &from, AvAllotment &to);
	// The other direction.
	void fromVPCoords(const AvAllotment &from, AvAllotment &to);
	// Set/Get the viewport's size. localsize_ is set to TRUE if
	// either w/h is non zero.
	void setSize(const unsigned int w, const unsigned h);
	void getSize(unsigned int &w, unsigned &h)const;
	// Get/Set position of the viewport.
	void setPosition(const int xoff, const int yoff);
	void getPosition(int &x, int &y)const;
  protected:
	~AvViewportSetter();
	AvViewport vp_;
	Boolean localsize_;	// If false, size comes from display list.
	DECLARE_HEADER
};

#endif
