//# Copyright (C) 1995-97 Board of Trustees of the University of Illinois
//#
//# This software, both binary and source, is copyrighted by The
//# Board of Trustees of the University of Illinois.  Ownership
//# remains with the Univerity.  You should have received a copy 
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvPolylineP.h,v 19.0 2003/07/16 05:47:38 aips2adm Exp $
//
// $Log: AvPolylineP.h,v $
// Revision 19.0  2003/07/16 05:47:38  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:16  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:45  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:52  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:45  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:43  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:32  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:36  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:07  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:10  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1997/09/15 16:23:18  hr
// Conversion of line width from an enum to an int.
//
// Revision 9.0  1997/08/25 21:29:44  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:18:11  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:56:18  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.5  1995/10/07  15:52:25  hr
//  Reworked drawVector. Added segments, circles and arcs. Rectangles work
//  now.
//
//  Revision 1.4  1995/09/20  20:09:52  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/09/20  01:49:12  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/07/17  19:57:53  hr
//  Added 'virtual' to destructor of virtual classes.
//
//  Revision 1.2  1995/06/22  19:03:55  hr
//  Added support for routines that use AvGraphicContext.
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvPolylineP.h
Private, internal include file for vectors.
*/
#ifndef AvPolylineP_h
#define AvPolylineP_h
#include "AvPolyline.h"
#include "AvGraphicContext.h"

#include "AvPointList2D.h"
typedef AvPointList2D PointList;

/*****************************************************************/

// Since this file is only included by Polyline implementation files,
// we can get by with making these global.
enum COMMAND {	NONE, MOVE, POINT, LINE, RECTANGLE, POLYGON,
		SEGMENT, CIRCLE, ARC
	     };

// Base class for the Polyline requests.
class AvPolyCmd_ {
  public:
	AvPolyCmd_();
	virtual ~AvPolyCmd_();
	virtual void draw(AvCanvas &c)=0;
};

class AvPolyDrawCmd_ : public AvPolyCmd_ {
  public:
	AvPolyDrawCmd_(COMMAND, AvPolyline::ADDRESSING,
						AvPolyline::COORDINATES);
	virtual ~AvPolyDrawCmd_();
	COMMAND		cmd_;
	AvPolyline::ADDRESSING		mode_;
	AvPolyline::COORDINATES		cmode_;
	Boolean		filled_;	// Ignored for non polygons.
	virtual void draw(AvCanvas &c)=0;
};

class AvPolyStateCmd : public AvPolyCmd_ {
  public:
	enum COMMANDS { NONE, DRAWMODE, FOREGROUND, BACKGROUND,
			FGNAME, BGNAME, LINEWIDTH, LINESTYLE
			};

	AvPolyStateCmd(const AvPolyStateCmd::COMMANDS, const unsigned long);
	AvPolyStateCmd(const AvPolyStateCmd::COMMANDS, const void *arg);
	virtual ~AvPolyStateCmd();
	virtual void draw(AvCanvas &c);
	AvPolyStateCmd::COMMANDS	command_;
	union {
		AvGraphicContext::DrawMode	drawMode;
//		AvGraphicContext::LineWidth	lineWidth;
		int				lineWidth;
		AvGraphicContext::LineStyle	lineStyle;
		unsigned long	color;
		char *		name;
	};
  private:
	void init(const AvPolyStateCmd::COMMANDS, const void *arg);
};

class AvPolylineCmd : public AvPolyDrawCmd_{
  public:
	AvPolylineCmd(COMMAND, AvPolyline::ADDRESSING,
		AvPolyline::COORDINATES, float x, float y);
	AvPolylineCmd(COMMAND, AvPolyline::ADDRESSING,
		AvPolyline::COORDINATES, float *x, float *y, int num=1);
	AvPolylineCmd(COMMAND, AvPolyline::ADDRESSING,
		AvPolyline::COORDINATES, float x, float y, float a, float b);
	AvPolylineCmd(COMMAND, AvPolyline::ADDRESSING, AvPolyline::COORDINATES,
		float *x, float *y, float *a, float *b, int num=1);
	AvPolylineCmd(COMMAND, AvPolyline::ADDRESSING, AvPolyline::COORDINATES,
		float *x, float *y, float *a, float *b, float *c, float *d,
		int num=1);
	virtual ~AvPolylineCmd();
	int	size()const;		// # of entries in list(s).
	PointList *positionList();
	PointList *auxList();
	PointList *aux2List();
	virtual void draw(AvCanvas &c);
	AvPolyline::POLYGONSHAPE	shape_;
  private:
	void append(float *x, float *y, int len);
	// Rectangles and segments need a second set of points.
	void append(float *x, float *y, float *a, float *b, int len);
	// Circles and arcs need a third set of points.
	void append(float *x, float *y, float *a, float *b, float *c, float *d,
								int len);
	PointList	*points_;
	// NULL if not Rectangle or Segment.
	PointList	*aux_;
	PointList	*aux2_;
};
#endif
