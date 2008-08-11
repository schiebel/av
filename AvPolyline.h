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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvPolyline.h,v 19.0 2003/07/16 05:47:29 aips2adm Exp $
//
// $Log: AvPolyline.h,v $
// Revision 19.0  2003/07/16 05:47:29  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:09  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:38  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:45  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:15  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:35  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:22  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:20  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:42  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:43  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/09 21:17:00  hr
// Removed unneeded semicolon from DECLARE_HEADER.
//
// Revision 9.0  1997/08/25 21:29:04  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:17:44  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:55:16  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.6  1996/06/18  18:43:13  pixton
//  Copyright Update
//
//  Revision 1.5  1995/10/19  16:01:12  hr
//  Added length function.
//
//  Revision 1.4  1995/10/07  15:15:14  hr
//  Added functions for segments, circles and arcs.
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
//  Revision 1.2  1995/06/22  18:56:45  hr
//  Added support for ScGraphicContext.
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

 
/* AvPolyline.h
Vector drawing class.

Functions include move, line, point, rectangle and filled polygon.
Addresses can be absolute or relative (within the viewport). By
default, addresses are a fraction of the viewport (in the range 0..1),
but this can be changed to (0.. <viewport_size -1>). The advantage of
using fractional addressing is that the same Polyline object will work
with any viewport.

Either an X and Y (and Width and Height for rectangles) or lists of X/Y
values may be passed. Currently it is more efficient to pass an array
of points rather than to make repeated calls.

Changing one of the modes affects subsequent calls, not previous ones.
Since moves are done frequently, there are versions for absolute and
relative addressing.
*/

#ifndef AvPolyline_h
#define AvPolyline_h
#include "AvGraphics2D.h"

class AvPolylineImpl;

class AvPolyline : public AvGraphics2D
{
  public:
	enum ADDRESSING {ABSOLUTE, RELATIVE};	  // Default is ABSOLUTE.
	enum COORDINATES {FRACTIONAL, FULLSIZE }; // Default is FRACTIONAL.
	// Hints as to what shape a filled polygon is. (From X11).
	enum POLYGONSHAPE {COMPLEX, CONVEX, NONCONVEX};
	enum {DEFAULTLEN=8};
	// The length will be increased if necessary.
	AvPolyline(int length=DEFAULTLEN);
	// Starts with an initial movea to x,y.
	AvPolyline(float x, float y, int len=DEFAULTLEN);
	~AvPolyline();

	// Move ( <current mode>, <absolute>, <relative>).
	// Values are in the range of (0..1).
	virtual void	move(float x, float y);
	virtual void	movea(float x, float y);
	virtual void	mover(float x, float y);

	virtual void	line(float x, float y);
	virtual void	line(float *x, float *y, int num=1);

	virtual void	point(float x, float y);
	virtual void	point(float *x, float *y, int num=1);

	virtual void	linea(float x, float y);
	virtual void	linea(float *x, float *y, int num=1);

	virtual void	liner(float x, float y);
	virtual void	liner(float *x, float *y, int num=1);

	virtual void	pointa(float x, float y);
	virtual void	pointa(float *x, float *y, int num=1);

	virtual void	pointr(float x, float y);
	virtual void	pointr(float *x, float *y, int num=1);
	//	Arguments are in range 0..viewportsize-1.
	virtual void	moveabs(float x, float y);
	virtual void	moverel(float x, float y);

	virtual void	lineabs(float x, float y);
	virtual void	lineabs(float *x, float *y, int num=1);

	virtual void	linerel(float x, float y);
	virtual void	linerel(float *x, float *y, int num=1);

	virtual void	pointabs(float x, float y);
	virtual void	pointabs(float *x, float *y, int num=1);

	virtual void	pointrel(float x, float y);
	virtual void	pointrel(float *x, float *y, int num=1);

	// These functions use the currently set modes.
	virtual void	rectangle(float x, float y, float width, float height,
				Boolean filled=FALSE);
	virtual void	rectangle(float *x, float *y,
				  float *width, float *height, int num=1,
				  Boolean filled=FALSE);

	// Default is non intersecting, possibly non convex.
	virtual void	polygon(float x, float y,
			const Boolean filled=FALSE, POLYGONSHAPE=COMPLEX);
	virtual void	polygon(float *x, float *y, int num,
			const Boolean filled=FALSE, POLYGONSHAPE=COMPLEX);

	// Disconnected segments. Draw from x1/y1 to x2/y2.
	virtual void	segment(float x1, float y1, float x2, float y2);
	virtual void	segment(float *x1, float *y1,
				float *x2, float *y2, const int len);

	virtual void	circle(float x, float y, float radius,
				const Boolean filled=FALSE);
	virtual void	circle(float *x, float *y, float *radius,
				const int num, const Boolean filled=FALSE);

	virtual void	arc(float x, float y, float width, float height,
				float angle0, float angle1,
				const Boolean filled=FALSE);

	virtual void	arc(float *x, float *y, float *width, float *height,
				float *angle0, float *angle1,
				const int narcs,
				const Boolean filled=FALSE);

	void addressMode(ADDRESSING);
	ADDRESSING addressMode();
	void coordinateMode(COORDINATES);
	COORDINATES coordinateMode();

	// Functions to change state.
	void foreground(const unsigned long color);
	void foregroundName(const char *name);
	void background(const unsigned long color);
	void backgroundName(const char *name);
	void lineStyle(const int style);
	void lineWidth(const int width);
	void drawMode(const int mode);

	/////
	virtual void draw(AvCanvas &);
	virtual void draw(AvCanvas *);
	virtual void getBoundingBox(AvCanvas &c, AvAllotment &r);
	// Remove current command list and start from scratch.
	// If newlen < 0 use current list size
	// 	     = 0 use default
	//	    else newlen.
	void	reset(const int newlen);
	// Return the number of items in the list.
	int length()const;
  private:
	ADDRESSING	mode_;
	COORDINATES	cmode_;
	POLYGONSHAPE	shape_;
	void init(int len=0);
	AvPolylineImpl	*impl_;
	DECLARE_HEADER
};
#endif
