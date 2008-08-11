//# Copyright (C) 1995-98 Board of Trustees of the University of Illinois
//#
//# This software, both binary and source, is copyrighted by The
//# Board of Trustees of the University of Illinois. Ownership
//# remains with the University. You should have received a copy 
//# of a licensing agreement with this software. See the file 
//# "AIPSVIEW_COPYRIGHT", or contact the University at this address:
//#
//# The NCSA AipsView Visualization System
//# National Center for Supercomputing Applications
//# University of Illinois
//# 405 North Mathews Ave.
//# Urbana, IL 61801
//# aipsview@ncsa.uiuc.edu
//# --------------------------------------------------
//
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvPG2Pixmap.h,v 19.0 2003/07/16 05:48:02 aips2adm Exp $
//
// $Log: AvPG2Pixmap.h,v $
// Revision 19.0  2003/07/16 05:48:02  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:37  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:43:05  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:11  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:39  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:09:08  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:59  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:21  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:02:13  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:56:28  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/09 21:16:56  hr
// Removed unneeded semicolon from DECLARE_HEADER.
//
// Revision 9.0  1997/08/25 21:31:43  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:19:29  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.1  1997/02/05 17:52:47  hr
// Simple object to allow PGPLOT calls to be written into an X pixmap.
//
//
//---------------------------------------------------------------------------

#ifndef AvPG2Pixmap_h
#define AvPG2Pixmap_h

#include <AvPolyline.h>

#include <AvXCanvases.h>
class AvPGDriver;
class AvXPalette;

extern "C" {
#include <cpgplot.h>
}

/* Creates an AvPixmap canvas and allows PGPLOT calls to be written into it.
The resulting pixmap may be used as a pixmap for a label.

Whe display() is called, AvPG2Pixmap does the following:
	a) If the PGPLOT driver hasn't been created, create and open it. 
	b) The routine execute() is called.
	a) If autoClose is TRUE (default), the AvPGCanvas is closed.


There are two ways to use this:
1)	Subclass the object. Create a subclass of AvPG2Pixmap and override
the execute() function. Execute should make the PGPLOT calls.

2)	Turn off autoClose and call desired PGPlot calls then close();

Currently, only polylines are supported. (No raster images).
*/

class AvPG2Pixmap : public AvPolyline {
  public:

	// Width/height of pixmap to be created.
	AvPG2Pixmap(Widget reference, AvXPalette *pal,
		    const unsigned int width, const unsigned int height,
		    const unsigned depth=0);	// depth=0 -> use reference's.
	virtual ~AvPG2Pixmap();

	// Return a pointer to the pixmap. The caller will be responsible
	// for freeing the pixmap.
	Pixmap getPixmap()const;

	// By default, open PGPLOT canvas, call execute, call close.
	// uData will be available to the execute routine.
	void display(XtPointer uData);

	// This gets called to perform the PGPLOT calls.
	// A pointer to this object is passed to execute.
	virtual void execute(AvPG2Pixmap *me);
	// Return the user supplied data.
	XtPointer uData(){return uData_;}

	void close();
	// Setting autoClose to FALSE, should allow multiple calls to
	// display.
	Boolean autoClose()const {return autoClose_;}
	void autoClose(const Boolean aoc) { autoClose_ = aoc;}

	// Miscellaneous
	unsigned int width()const{return width_;}
	unsigned int height()const{return height_;}

	// Set the color to be used to clear the pixmap.
	void setBackgroundColor(const int bgcolor);
	void clear();	// Clear the pixmap. NOT called by AvPG2Pixmap.
	////////////////
	// Standard canvas interface.
	virtual void draw(AvCanvas &);
	virtual void draw(AvCanvas *);

  protected:
	virtual void open(AvCanvas *);
	AvXPixmap	*pmc_;
	// This only exists while it's open.
	AvPGDriver	*pgdriver_;
	Boolean		autoClose_;
	unsigned int	width_;
	unsigned int	height_;
	XtPointer	uData_;
	AvViewportSetter *vps_;
  private:
	DECLARE_HEADER
};
#endif
