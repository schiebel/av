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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvViewport.h,v 19.0 2003/07/16 05:46:41 aips2adm Exp $
//
// $Log: AvViewport.h,v $
// Revision 19.0  2003/07/16 05:46:41  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:27  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:41:55  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:05  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:26  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:07:43  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:25  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:37  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:18  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:51:54  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:24:35  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:02  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 07:38:40  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.5  1996/06/18  18:43:28  pixton
//  Copyright Update
//
//  Revision 1.4  1996/02/14  22:27:48  hr
//  Removed offsets, and added {get,set}Position.
//
//  Revision 1.3  1996/01/26  21:10:28  hr
//  Made width/height const.
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

/* AvViewport.h
The viewport is a rectangle into which vectors, text and graphics can
be drawn.

A Viewport may be offset from the 0,0 position of its parent.
Besides a size and position, viewports provide the canvas with scaling
information for rasters and the 'current position' for vectors.

Raster scaling is cummulative between a child and its parent.
Current point is the position relative to the viewport's 0,0 where the
next draw operation starts.

A child viewport must be within its parent. Siblings may overlap.

		Raster Scaling
Used to set scaling for raster objects. Scale is defined as the number
of output pixels to draw for each input pixel.
val	scaling
 1	1
 2	2	(Output is 2x the size of the input).
 .5	1/2	(Output is 1/2 the size of the input).
 .25	1/4
    ...
Note:
	While the scale values are floating point numbers, only integer
scaling is supported... It is not possible to skip a fraction of a pixel
nor is it possible to expand by a fraction of a pixel.
*/
#ifndef AvViewport_H
#define AvViewport_H

#include <AvGeometry.h>

class AvViewport {
  public:
	AvViewport( int x0, int y0, unsigned int width, unsigned int height);
	AvViewport();
	AvAllotment const &xy()const {return port_.xy();}
	AvAllotment const &wh()const {return port_.wh();}
	void xy(const AvAllotment &xy);
	void wh(const AvAllotment &wh);
	void merge(AvViewport &);
	void constrain(AvViewport &);
	Boolean contains(const AvAllocation &a)const;
	Boolean contains(const AvAllotment &a)const;
	int x0();
	void x0(int);
	int y0();
	void y0(int);
	unsigned int width()const;
	void width(unsigned int);
	unsigned int height()const;
	void height(unsigned int);
	void getPosition(int &x, int &y)const;
	void setPosition(const int x, const int y);

	// Set scale factors for rasters. Out of range values are ignored.
	void setRasterScale( const float xscl, const float yscl);
	void setRasterScale( const float scl);	// x/y scales are the same.
	void getRasterScale( float &xscl, float &yscl);
	void getScale( float &xscl, float &yscl)const;
	AvAllotment &rasterscale() {return rasterscale_;}
	AvAllotment &vectorPosition() { return vector_position_;}
	void vectorPosition(AvAllotment &newposition);
	// Min/max valid scale values.
	static float minRasterScale();
	static float maxRasterScale();
	// convert to/from vp coord space possibly dealing with raster scale.
	// Returns TRUE if the vp space point is withing the vp.
	Boolean toVP(const AvAllotment &input, AvAllotment &output,
					const Boolean scale=FALSE)const;
	Boolean fromVP(const AvAllotment &input, AvAllotment &output,
					const Boolean scale=FALSE)const;
  private:
	AvAllocation	port_;	// The viewport.
	AvAllotment	rasterscale_;	// How rasters are scaled.
	AvAllotment	vector_position_;
};

#endif
