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
//---------------------------------------------------------------------------
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvContour.cc,v 19.0 2003/07/16 05:46:59 aips2adm Exp $
//
// $Log: AvContour.cc,v $
// Revision 19.0  2003/07/16 05:46:59  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:44  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:12  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:20  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:30  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:02  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:47  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:17  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:16  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:01  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.4  1998/06/09 21:20:23  hr
// Removed unneeded semicolon after DEFINE_MEMBER_INFO macro.
//
// Revision 9.3  1997/12/17 21:50:37  hr
// Changes to remove warnings from MIPSpro.7x compiler.
//
// Revision 9.2  1997/09/17 14:40:21  hr
// generate(): Added support for emphasized contours.
//
// Revision 9.1  1997/09/15 16:28:11  hr
// In generate() added support for changing line width.
//
// Revision 9.0  1997/08/25 21:26:21  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:03  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 08:17:52  droberts
// Final update from monet archive.
//
// Revision 1.3  1996/11/12 20:27:27  hr
// Removed unused variables from generate().
//
// Revision 1.2  1996/11/12  20:13:08  hr
// Replaced call to getRow() with getRegion().
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.7  1996/06/18  18:45:55  pixton
//  Copyright Update
//
//  Revision 1.6  1996/04/06  18:10:46  hr
//  Added calls to (re)setBusy().
//
//  Revision 1.5  1996/03/05  17:37:35  hr
//  Contour labels are printed in white. (Comment change).
//
//  Revision 1.4  1996/02/02  16:23:37  hr
//  draw:: If the contour had to be regenerated due to a raster change, the
//  timestamp wasn't being updated.
//
//  Revision 1.3  1996/01/25  17:42:43  hr
//  Needed to add Graphics2D initializations.
//
//  Revision 1.2  1996/01/24  22:58:27  hr
//  The contour will only display itself if ignore() returns FALSE.
//
//  Revision 1.1  1996/01/24  18:03:39  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvContour.cc

subclass of AvPolyline that generates contours of a raster by calling
PGPLOT/AvPGDriver routines.

*/

#include <stdio.h>
//#include <iostream.h>
#ifndef STANDALONE
#include <AvImageView.h>
#endif
#include <AvContour.h>
#include <AvViewportSetter.h>
#include <AvRaster.h>
#include <AvPGDriver.h>
#include <AvCanvas.h>
#include <AvContourInfo.h>

extern "C" {
#include <cpgplot.h>
}

DEFINE_MEMBER_INFO(AvContour)

AvContour::AvContour(AvRaster *r, AvContourInfo *cinfo ) : AvPolyline(1024)
				     // Assume a bunch of points (arbitrary).
{
	INIT_CLASS(AvPolyline);

	info_ = NULL;
	contourInfo(cinfo);
	raster_ = NULL;
	setRaster(r);
	if(r != NULL)
	{ char buf[1024];
		sprintf(buf, "AvContour:%s", raster_->getName());
		name(buf);
	}
}

AvContour::~AvContour()
{
	if(raster_ != NULL)
		raster_->unref();
	if(info_ != NULL)
		info_->unref();
}

// Use a different raster. Get rid of the old and purge the pointlist.
// The contour level and color arrays are not changed.
void AvContour::setRaster( AvRaster *r)
{
	if(r == raster_)
		return;

	if(raster_ != NULL)
		raster_->unref();
	raster_ = r;
	if(raster_ != NULL)
		raster_->ref();
	touch();
}

// Remove current contour points.
void AvContour::purgePoints()
{
	haveContour_ = FALSE;
	reset(-1);	// tell AvPolyline to get rid of its point list.
}

// Generate the contour(s).
void AvContour::generate(AvCanvas *c)
{
//int	width, height, nc, i;
int	width, height, i;
float	vpwidth, vpheight;
float	*data;
AvViewport *vp;

	if(raster_ == NULL)
		return;
	purgePoints();		// Get rid of any existing contour.

	// Copy raster data to an array that the contour routines can handle.
	width = raster_->width();
	height = raster_->height();

	data = new float[width*height];

	raster_->beginAccess();
#if 0
		for(ptr=data, row=0; row< height; row++, ptr += width)
			raster_->getRow(ptr, 0, row);
#else
		// The entire plane is retrieved regardless of any scaling.
		raster_->getRegion(data, 0, 0, width, height);
#endif
	raster_->endAccess();

	// Draw the negative contours with dashes?
//	nc = (dashNegativeContours()) ? 1 : -1;
	// Disable dash if negative since we handle it manually.
//	nc = -1;
//****************************************************************
//****************************************************************
	//			Make contours.

	// Create a PGPLOT driver object and tell it to draw into this.
	AvPGDriver *pgdriver = new AvPGDriver();
	pgdriver->setCanvas(c);
	// Use our own VPS to set the correct size.
	vp = c->viewport();
	vpwidth = vp->width();
	vpheight = vp->height();
	pgdriver->setVPS(new AvViewportSetter(0.0, 0.0, vpwidth, vpheight));
	pgdriver->newPolyline(this);
	AvPGDriver::currentDriver(pgdriver);

//****************************************************************
//			PGPLOT code
	// Open device.
	if(cpgbeg(0, "/canvas", 1, 1) != 1)
		return;
	cpgask(0);		// Don't ask.

	cpgsvp(0.0, 1.0, 0., 1.0);		// Set viewport to entire area.

	// Set world coordinate window.
float wxmin, wymin, wxmax, wymax;
	{
#if 0
		// This 'works', but since the numbers won't match those
		// in AvAxis (need to translate degrees to seconds), any
		// axis won't align, so why waste the time? To really make
		// it 'right' we'd have to do the same conversions that
		// AvAxis does.
		wxmin = raster_->xAxis()->minValue();
		wxmax = raster_->xAxis()->maxValue();
		wymin = raster_->yAxis()->minValue();
		wymax = raster_->yAxis()->maxValue();
#else
		wxmin = 0; wymin = 0;
		wxmax = 1.0; wymax = 1.0;
#endif
		cpgswin(wxmin, wxmax, wymin, wymax);
//printf("Window min/max: %.2f,%.2f  %.2f,%.2f\n", wxmin, wxmax, wymin, wymax);
//		cpgswin(0.0, 1.0, 0.0, 1.0);
	}

	int lw = getLinewidth(0);	// Only 1 line width.
	cpgslw(lw);

	int emphColor = info_->emphasisColor();
	// Unused now.
//	int emphOffset = info_->emphasisOffset();
	Boolean emphEnabled = info_->emphasisEnabled();

	if(drawTitle())
	{char buf[128];
		sprintf(buf, "Contour of %s", raster_->getName());
		cpgmtxt("t", -1.0, 0.0, 0.0, buf);
	}

	////////////////////////////////////////////////////////////////
	// TR is used to translate from I,J to world coordinates.
	// The -1s are to account for the FORTRAN - C indexing conventions.
	// Wx = tr[0] + tr[1]*i;	( 1<= i,j <= width,height).
	// Wy = tr[3] + tr[4]*j;
	// tr[1] = (wxmax-wxmin)/(width-1)
	// tr[0] = wxmin - tr[0];
	// We could just use the -1.0, 1.0... form, but this still works
	// if wx/ymin/max get changed to the raster's.
	float a = (wxmax-wxmin)/(width-1);
	float b = (wymax-wymin)/(height-1);
	float tr[6] = {wxmin-a, a, 0.0, wymin-b, 0.0, b};
//	static float tr[6] = {-1.0, 1.0, 0.0, -1.0, 0.0, 1.0};


	int ci, ls;
	float contour;
	float blankvalue = raster_->getBlankValue();
	// If emphasizing, draw all the background contours before any
	// foreground so background n+1 doesn't overwrite background n.
	if(drawContours() && emphEnabled)
	{	cpgsci(emphColor);
		cpgslw(lw*3);
		cpgsls(1);	// Solid;

		for (i=0; i < numContourLevels(); i++)
		{	contour = (float) getContourValue(i);
			if(blankingEnabled())
			 cpgconb(data, width, height, 1, width, 1, height,
			   &contour, 1, tr, blankvalue);
			else
			 cpgcont(data, width, height, 1, width, 1, height,
				&contour, -1, tr);
		}
	}

	// Draw contours &/or labels. One contour per iteration to allow
	// for different colors and linestyles.
	for (i=0; i < numContourLevels(); i++)
	{	contour = (float) getContourValue(i);
		// One possible future enhancement would be to allow the
		// display of contours &/or labels to depend on the level.
		if(drawContours())
		{	ci = getContourColor(i);
			cpgsci(ci);
			ls = getLinestyle(i);
			cpgsls(ls);
			cpgslw(lw);
			if(blankingEnabled())
			 cpgconb(data, width, height, 1, width, 1, height,
			   &contour, 1, tr, blankvalue);
			else
			 cpgcont(data, width, height, 1, width, 1, height,
				&contour, -1, tr);
		}

		if(labelContours())
		{ char *label = getLabel(i);
			if(label != NULL)
			{int minint, intval;
				getLabelSpacing(i, intval, minint);
				cpgsci(1);	// White.
				cpgconl(data, width, height,
					1, width, 1, height,
					contour, tr, label, intval, minint);
			}
		}
	}

	cpgend();

//****************************************************************
//****************************************************************
//****************************************************************

	delete [] data;
	delete pgdriver;
	haveContour_ = TRUE;
	infostamp_ = info_->timestamp();
}

// Mark as modified and remove any pointlist since it needs to be rebuilt.
void AvContour::touch()
{
	haveContour_ = FALSE;
	purgePoints();
	infostamp_ = info_->timestamp();
	AvGraphics2D::touch();
}

void AvContour::draw(AvCanvas &c)
{
	if(raster_ == NULL)
		return;
	if(ignore())		// Don't draw if told not to.
		return;

	// Purge list if something has changed.
	if((info_->modifiedSince(infostamp_)) ||
	   (raster_->modifiedSince(*this))
	  )
		touch();

	// (Re) generate if there is no current list.
#ifndef STANDALONE
	if(!haveContour())
	{	AvXColorApp::setBusy();
		generate(&c);
		AvXColorApp::resetBusy();
	}
#else
	if(!haveContour())
		generate(&c);
#endif
	// Display.
	AvPolyline::draw(c);
}

void AvContour::draw(AvCanvas *c)
{
	if(c != NULL)
		draw(*c);
}
// Return the string representation for the contour level. The value returned
// is a pointer to a static buffer. This isn't a problem within AvContour
// since only one string is needed at a time, however if called from without,
// the string should be copied.
char *AvContour::getLabel(const int level)const
{
static char buf[64];
float	value;

	value = getContourValue(level);
	sprintf(buf, getFormatString(), value);
	return buf;
}


// Use a different contour info.
void AvContour::contourInfo(AvContourInfo *inf)
{
	// Ignore if same.
	if((info_ == inf) && (info_ != NULL))
		return;
	if(info_ != NULL)
		info_->unref();

	info_ = inf;
	// Have to have one.
	if(info_ == NULL)
		info_ = new AvContourInfo();

	info_->ref();
	infostamp_ = 0;		// Need an update.
}
