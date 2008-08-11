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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvPG2Pixmap.cc,v 19.0 2003/07/16 05:48:02 aips2adm Exp $
//
// $Log: AvPG2Pixmap.cc,v $
// Revision 19.0  2003/07/16 05:48:02  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:37  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:43:05  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:12  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:40  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:09:08  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:59  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:22  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:02:15  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:56:29  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/09 21:24:25  hr
// Removed unneeded semicolon after DEFINE_MEMBER_INFO macro.
//
// Revision 9.0  1997/08/25 21:31:45  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:19:30  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.1  1997/02/05 17:52:17  hr
// Simple object to allow PGPLOT calls to be written into an X pixmap.
//
//
//---------------------------------------------------------------------------

/* AvPG2Pixmap.cc
 Creates an AvPixmap canvas and allows PGPLOT calls to be written into it.
*/
#include <X11/Intrinsic.h>
#include <AvPGDriver.h>
#include <AvXPalette.h>
#include <AvViewportSetter.h>
#include <AvPG2Pixmap.h>


DEFINE_MEMBER_INFO(AvPG2Pixmap)

// Width/height of pixmap to be created.
AvPG2Pixmap::AvPG2Pixmap(Widget reference, AvXPalette *pal,
			 const unsigned int pmwidth,
			 const unsigned int pmheight,
			 const unsigned depth) : AvPolyline(1024)
{
	INIT_CLASS(AvPolyline);

	pmc_ = new AvXPixmap(reference, pal, pmwidth, pmheight, depth);
	pmc_->ref();
	width_ = pmwidth;
	height_ = pmheight;
	pgdriver_ = NULL;
	autoClose_ = TRUE;
	vps_ = NULL;
}

AvPG2Pixmap::~AvPG2Pixmap()
{
	close();	// Just in case.
	pmc_->unref();
}

// Return a pointer to the pixmap.
Pixmap AvPG2Pixmap::getPixmap()const
{
	return pmc_->getPixmap();
}

// By default, open PGPLOT canvas, call execute, call close.
// A pointer to this is passed to execute.
void AvPG2Pixmap::display(XtPointer uData)
{
	uData_ = uData;
	pmc_->show(this);
}

// This gets called to perform the PGPLOT calls.
void AvPG2Pixmap::execute(AvPG2Pixmap *) {}

// Set the color to be used to clear the pixmap.
void AvPG2Pixmap::setBackgroundColor(const int bgcolor)
{
	pmc_->setClearColor(bgcolor);
}

// Clear the pixmap.
void AvPG2Pixmap::clear()
{
	pmc_->clear();
}



// Create a PGPLOT driver object and tell it to draw into this.
// If it already exists, assume it's opened and return.
void AvPG2Pixmap::open(AvCanvas *c)
{
	if(pgdriver_ != NULL)
		return;		// Already opened.

	pgdriver_ = new AvPGDriver();
	pgdriver_->setCanvas(c);
	// Create a viewport the same size as the current one.
	AvViewport *vp = c->viewport();
	int vpw = vp->width();
	int vph = vp->height();
	vps_ = new AvViewportSetter(0.0, 0.0, vpw, vph);
	pgdriver_->setVPS(vps_);
	pgdriver_->newPolyline(this);
	AvPGDriver::currentDriver(pgdriver_);

	// Open device.
	if(cpgbeg(0, "/canvas", 1, 1) != 1)
		return;
	cpgask(0);		// Don't ask.
}

void AvPG2Pixmap::close()
{
	if(pgdriver_ != NULL)
	{	cpgend();
		delete pgdriver_;
		pgdriver_ = NULL;
		vps_ = NULL;	// Goes with driver.
	}
}

////////////////
// Standard canvas interface.
void AvPG2Pixmap::draw(AvCanvas &c)
{
	if(autoClose() || (pgdriver_ == NULL))
		open(&c);
	// Do the PGPLOT calls.
	execute(this);

	if(autoClose())
		close();

	// Draw into the pixmap.
	AvPolyline::draw(c);
}

void AvPG2Pixmap::draw(AvCanvas *c)
{
	if(c != NULL)
		draw(*c);
}
