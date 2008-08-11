//# Copyright (C) 1995-2002 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvCLUImageSetter.cc,v 19.0 2003/07/16 05:48:00 aips2adm Exp $
//
// $Log: AvCLUImageSetter.cc,v $
// Revision 19.0  2003/07/16 05:48:00  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:35  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.2  2002/01/22 21:40:53  hravlin
// Cast menuLen to int to make sure 64 bit compilers don't complain.
//
// Revision 17.1  2002/01/09 18:00:03  hravlin
// Removed unneeded include of math.h.
//
// Revision 17.0  2001/11/12 19:43:03  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:10  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:37  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 18:14:32  hravlin
// Changed some char * to const char *.
//
// Revision 14.0  2000/03/23 16:09:06  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.2  1999/10/01 21:56:55  hravlin
// *** empty log message ***
//
// Revision 13.1  1999/08/25 19:59:59  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:40:57  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:19  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:02:09  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:56:22  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.6  1998/06/19 20:21:59  hr
// Wasn't unref'ing overlay_ in destructor.
//
// Revision 9.5  1998/05/04 20:26:57  hr
// Removed an unneeded semicolon from DEFINE_MEMBER_INFO.
// Removed an unused variable.
//
// Revision 9.4  1998/05/04 16:44:43  hr
// Wasn't deleting overlaySetup_ in the destructor.
//
// Revision 9.3  1998/02/20 21:57:33  hr
// Added support for vector overlays.
//
// Revision 9.2  1997/11/17 16:50:54  hr
// Added initialization of axisOptions_.gAxisOffsets_ variable.
//
// Revision 9.1  1997/09/19 21:31:58  hr
// In setSlice(). If the requested plane was > than the current # of CLURasters,
// but < the current starting plane of the Accessor, aipsview was generating
// illegal plane errors while creating new rasters.
//
// Revision 9.0  1997/08/25 21:31:35  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.2  1997/04/23 17:15:02  hr
// In buildRasterObject(), added code to use common globalOptions among axis
// objects.
//
// Revision 8.1  1997/04/15 20:48:11  hr
// Added "Show Wedge" menu option.
//
// Revision 8.0  1997/02/20 03:14:56  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.8  1997/02/09 00:07:28  hr
// Yet another change to setSlice() to check for valid slice index.
//
// Revision 7.7  1997/02/06 17:56:49  hr
// setSlice was checking the new index against the current depth rather
// than the DS axis length.
//
// Revision 7.6  1997/02/05 17:30:04  hr
// Removed some unused variables.
//
// Revision 7.5  1997/01/24 20:34:00  hr
// Added support for axis and wedge configuration.
//
// Revision 7.4  1996/12/12 08:07:21  droberts
// Final update from monet archive.
//
// Revision 1.10  1996/11/11 15:51:02  hr
// Added getRasterScale() to retrieve the x/y scale factors.
//
// Revision 1.9  1996/11/05 21:08:04  hr
// Removed some contour info initializations (now done in contourInfo).
// getPrintName() now passed underscores.
//
// Revision 1.8  1996/11/04 15:39:10  hr
// Went back to checking sliceIndex against depth() in setSlice since
// depth() returns 1 for a 2D data set.
//
// Revision 1.7  1996/10/01  21:57:00  hr
// setSlice checks slice as DS value.
//
// Revision 1.6  1996/09/27  16:38:59  hr
// Added a pointer to the imported accessor in order to keep track of
// whether AvCLUImageSetter had already disconnected from it.
//
// Revision 1.5  1996/09/25  21:46:00  hr
// Worked out some DS vs SR bugs and turned off some debugging statements.
//
// Revision 1.4  1996/09/18  18:50:17  hr
// More integration with AvAccessor.
//
// Revision 1.3  1996/09/10 16:51:26  hr
// Initial changes for integration with AvAccessor & AvDataSet.
//
// Revision 1.2  1996/08/14 17:23:32  hr
// Changes to remove g++ warnings (from bglenden).
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.25  1996/06/18  18:45:50  pixton
//  Copyright Update
//
//  Revision 1.24  1996/04/01  22:42:25  hr
//  Added getOverlays().
//
//  Revision 1.23  1996/03/28  22:42:50  hr
//  Locked the contourinfo_ resource to check for unintentional delete attempts.
//
//  Revision 1.22  1996/03/07  21:21:38  hr
//  Added getPrintName().
//
//  Revision 1.21  1996/02/29  17:21:57  hr
//  Changes to get wedge to work with PGPLOT canvas. Mostly PGPLOT doesn't
//  appear to scale 1 pixel wide images well.
//
//  Revision 1.20  1996/02/23  18:09:25  hr
//  Removed an unused variable.
//
//  Revision 1.19  1996/02/23  17:22:31  hr
//  Many changes. Mostly cleanup, moving from "offsets" to "margins" and
//  dealing with the color wedge. Moved a lot of axis setup code to AvAxis.
//
//  Revision 1.18  1996/02/08  18:47:05  hr
//  Removed some unused constants.
//
//  Revision 1.17  1996/02/08  18:31:40  hr
//  Lots of changes to try and get axis labels to display well and to deal
//  with showing the axis on creation.
//  Reworked menus again to show menus can also be used in defaults menu.
//  Changed touch() to touchRasters() and removed modifyEnabled().
//
//  Revision 1.16  1996/02/02  20:56:56  hr
//  buildRasterObject was setting the raster's vps to the wrong size.
//  Moved axis labeling to AvAxis.cc
//
//  Revision 1.15  1996/02/02  15:40:23  hr
//  Added support for turning off menus. Redid menus.
//
//  Revision 1.14  1996/02/01  17:54:40  hr
//  Changed setScaledSize to conditionally include offsets.
//  Added use of vpi_ for drawing overlayed contours.
//
//  Revision 1.13  1996/01/31  21:25:20  hr
//  Changed default contour levels again.
//
//  Revision 1.12  1996/01/31  17:50:49  hr
//  Added support for axis drawing.
//
//  Revision 1.11  1996/01/24  22:57:59  hr
//  The min & max passed to ContourInfo are the cube's min and max, not
//  the value of the data slider.
//
//  Revision 1.10  1996/01/24  18:01:44  hr
//  Introduce PGPLOT/Contour support.
//
//  Revision 1.9  1996/01/08  22:21:22  hr
//  Turn off more contour related code until it's ready.
//
//  Revision 1.8  1996/01/08  22:19:14  hr
//  Beginnings of support for contours.
//
//  Revision 1.7  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.7  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.6  1995/07/28  21:03:05  hr
//  Changed reference from MAXFLOAT to MAXVALUE (defined in Av.h).
//
//  Revision 1.5  1995/07/26  19:57:33  hr
//  Initialize clipMin/clipMax.
//
//  Revision 1.4  1995/04/26  19:45:47  hr
//  Calling setRasterScale doesn't 'touch' the rasters.
//
//  Revision 1.3  1995/04/24  18:39:12  hr
//  Moved index generation to CLUIRaster from CLUImageSetter to support
//  raster's beginAccess and endAccess.
//
//  Revision 1.2  1995/04/17  14:57:05  hr
//  Doing append in setSlice no longer causes PM cache flush.
//
//  Revision 1.1  1995/04/14  19:43:58  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

/*
AvCLUImageSetter is the part of AvCLUImage that handles displaying of
'slices' on a 2D canvas.

The upper level aipsView code assumes the CLUImage object can display
one slice at a time. Each time the slice changes, CLUImage is told to
redisplay.

The XCanvas wants a different object for each slice so it can keep
track of its Pixmap cache.

(AvCLUImage and AvCLUImageSetter aren't the same class due to a
conflict with their respective 'className()' function.

If PGPLOT support (contours & axis) is desired, define PGPLOT.
If DOOVERLAY is defined (in AvCLUImageSetter.h), vector overlays are enabled.

For each plane (slice), CLUImageSetter creates a viewportSetter
that contains a child viewportSetter and the axis object. The
child viewportSetter contains the raster and the contour objects.

The surrounding viewportSetter is large enough to display the axis and
shows the raster/contour offset.


		[ AvCLUImageSetter ]
			|
		-----------------
		|	|	|
	     IMAGEVPS	VPI	OVR
		|
		|................
		|		|
		VPS	...	VPS
		| |
		| ----- Axis, wedgeVPS
		VPR
		|
		|- Raster
		.- Contour

		VPI-> w1, w2,...

VPS	Per slice viewportSetter
VPR	Per raster viewportSetter
VPI	viewportSetter for included windows.
wedgeVPS The color wedge. (One wedge is shared).
OVR	Vector Overlays.

Contours from other windows aren't really in this object's display list.
They are explictly drawn from within AvCLUImageSetter's draw function.

-> Draw the current image, its axis and the wedge.
   Draw any included contours.
   Finally, draw the vector overlays.

NOTE:
	Included contours are frequently referred to as overlays in this
file. They are not the same as the vector overlay mechanism.
*/

#include <ctype.h>			// isalnum
#include "AvCLUIRaster.h"
#include "AvCLUImage.h"
#include "AvCLUImageSetter.h"

#include "AvMenu.h"
#include "AvImageViewItem.h"
#include "AvXCanvas.h"
#ifdef PGPLOT
#include <Xm/ToggleB.h>
#include "AvContour.h"
#include "AvAxis.h"
#include "AvContourList.h"
#include "AvContourInfo.h"
#include "AvImageDataPanel.h"
#include "AvImageWindow.h"
#include "AvAxisConfig.h"
#endif
// DO_OVERLAY is defined (or not) in AvCLUImageSetter.h.
#ifdef DO_OVERLAY
#include "AvOverlay.h"
#include "AvOverlayConfig.h"
#endif

#include "AvAxisOptions.h"
#include "AvOverlayOptions.h"
#include "AvImages.h"
#include "AvAxisVPS.h"

DEFINE_MEMBER_INFO(AvCLUImageSetter)

/* The horizontal offset of the raster image when the axis is shown is
dependent on the number of characters in a Y axis label. However, the
size of the characters is dependent on the size of the viewport (including
the offsets). The X offset will be computed by finding the size needed to
display a certain number of characters of a particular size.

CHRSIZE is the value returned by cpgqcs(3,,) for the current driver which
assumes a size of 1x1 so coordinates are effectively relative.

CHRFF	Empirical "adjustment" to convert character height to width.

NUMCHRS is a number roughly equal to the maximum expected number of
characters in a horizontal string. (eg. -12h 32m 24s).

In order for this to work, the raster's viewport size has to be set before
any call to getMargins().
*/
#if 0
static const float CHARHEIGHT = 0.025;	// Character height w/o scaling.
					// (in range (0..1).
static const float CHRFF = 0.5625;
static const float CHRWIDTH = CHARHEIGHT * CHRFF;
// Change PWIDTH to vary the target character size (in pixels).
static const int PWIDTH = 10;		// Desired width of a char in pixels.
static const int NUMCHRS = 12;		// Max # of chars in a horizonal label.
static const int NUMROWS = 5;		// max # of horizontal rows. (est.).
					// (for title, H labels, etc).
static const float NUMRASTERCHRS=22.;	// Max # of chars along horizontal
					// axis of the raster (est).
#endif

// Type ids of the various objects to be retrieved.
static  int RASTER_ID = -1;		// -1 -> not initialized yet.
static  int VPS_ID;
static  int AXISVPS_ID;

#ifdef PGPLOT
static  int AXIS_ID;
static  int CONTOUR_ID;

// Menu strings.
static const char *RASTER =	"Raster";
static const char *CONTOURS = "Contours";
static const char *SHOWINCLUDED = "Included Contours";
static const char *AXIS =	"Axis";
static const char *WEDGE = 	"Wedge";
#if defined DO_OVERLAY
static const char *OVERLAY =	"Overlay";
#endif

static AvMenuButtonInfo menus[] = {
	{RASTER, TOGGLE_BUTTON, 1},
	{CONTOURS, TOGGLE_BUTTON, 0},
	{SHOWINCLUDED, TOGGLE_BUTTON, 0},
	{AXIS, TOGGLE_BUTTON, 0},
	{WEDGE, TOGGLE_BUTTON, 1},
#if defined(DO_OVERLAY)
	{OVERLAY, TOGGLE_BUTTON, 0}
#endif
	};
// Indexes of above.
enum {RASTERINDEX, CONTOURSINDEX, SHOWINCLUDEDINDEX, AXISINDEX, WEDGEINDEX
#if defined(DO_OVERLAY)
      ,OVERLAYINDEX
#endif
};

static const int menuLen = (int)(sizeof(menus)/sizeof(*menus));

#endif

static const int WEDGEWIDTH = 16;	// width of the color wedge.
static const int WEDGEOFFSET = 5;	// Offset from raster.
static const int WEDGETM = 10;
static const int WEDGEBM = 10;

// Accessor/dataset events we're interested in.
static const int ACMASK = (AvAccessor::DEFAULTMASK | AvDataSet::DEFAULTMASK
			| AvAccessor::PLANE) & ~AvAccessor::CLIP;

AvCLUImageSetter::AvCLUImageSetter (AvCLUImage *image, AvImageViewItem *ivi)
{
	INIT_CLASS(AvViewportSetter);

#if defined(DO_OVERLAY)
	overlay_ = NULL;
#endif
	wedgeVPS_ = NULL;
	wedge_ = NULL;

	// Decide what to initially show based on current menu settings.
	showRaster_ = menus[RASTERINDEX].buttonData ? TRUE : FALSE;
	showContours_ = menus[CONTOURSINDEX].buttonData ? TRUE : FALSE;
	showIncludedContours_ = menus[SHOWINCLUDEDINDEX].buttonData
								? TRUE : FALSE;
	showAxis_ = menus[AXISINDEX].buttonData ? TRUE : FALSE;
	showWedge_ = menus[WEDGEINDEX].buttonData ? TRUE : FALSE;
#if defined(DO_OVERLAY)
	showOverlay_ = menus[OVERLAYINDEX].buttonData ? TRUE : FALSE;
#else
	showOverlay_ = FALSE;
#endif
	lm_ = 100;
	rm_ = 100;
//	rm_ = WEDGEOFFSET + WEDGEWIDTH*2 + 100;
	tm_ = 50;
	bm_ = 50;

#if 1
	// Set to display only one item instead of entire list.
	setDisplayMode(WRAP);		// Anything but ALL.
#else
	// Set to display only one item instead of entire list.
	imagevps_ = new AvViewportSetter();
	append(imagevps_);
	imagevps_->setDisplayMode(WRAP);		// Anything but ALL.
#endif

	// We have to be careful about how much of image gets used now
	// since it is still being constructed at this point.
	CLUImage_ = image;
	// Connect to accessor.
	accessor_ = CLUImage_->accessor();
	accessor_->ref();

	accessor_->addCallback(&AvCLUImageSetter::accessorCB_,
			 ACMASK, this, NULL);
	const char *filename= ivi->accessor()->dataset()->fileName();

	// AxisOptions structs used for the wedge and the image.
	axisOptions_ = new AvAxisOptions(filename, "imageAxis");
	axisOptions_->ref();
	axisOptions_->gAxisOffsets_.setLength(accessor_->rank());
	axisOptions_->gAxisOffsets_.disableUpdate();
	axisOptions_->gAxisOffsets_.setValue(accessor_->shape()/2);
	axisOptions_->gAxisOffsets_.enableUpdate();
#if defined(DO_OVERLAY)
	overlayOptions_ = new AvOverlayOptions(filename, "overlay", accessor_);
	overlayOptions_->ref();
#endif
	wedgeOptions_ = new AvAxisOptions(filename, "wedgeAxis");
	wedgeOptions_->ref();

	// The imageSetter needs to be called when options change so it
	// can cause a redraw.
	axisOptions_->addCallback(
		&AvCLUImageSetter::infoChangedCB,
		AvContourInfo::ValueChanged, this, NULL);
	wedgeOptions_->addCallback(
		&AvCLUImageSetter::infoChangedCB,
		AvContourInfo::ValueChanged, this, NULL);
#if defined(DO_OVERLAY)
	overlayOptions_->addCallback(
		&AvCLUImageSetter::infoChangedCB,
		AvContourInfo::ValueChanged, this, NULL);
#endif

	// This is used to get a widget to be
	// used as a 'parent' for the Contour level object. And for redisplay.
	imageViewItem_ = ivi;
	{char buf[128];
		sprintf(buf, "AvCLUImageSetter:%s", ivi->label());
		name(buf);	// Helpful when debugging.
	}
#ifdef PGPLOT
#if defined(DO_OVERLAY)
	overlay_ = new AvOverlay(accessor_, overlayOptions_);
	overlay_->ref();
#endif

	if(ivi != NULL)
	{ char name[1024];
	  Widget ref = ivi->dataItem()->dataPanel()->baseWidget();

		// Create setup panels for wedge and axis.
		axisSetup_ = new AvAxisConfig(filename, ref, axisOptions_,
					      ivi);
		// Wedge doesn't support mouse tracking.
		wedgeSetup_ = new AvAxisConfig(filename, ref, wedgeOptions_);

#if defined(DO_OVERLAY)
		overlaySetup_ = new AvOverlayConfig(filename, ref,
						    overlayOptions_);
#endif
		// Initialize the Contour Info object.
		contourlevels_ = new AvContourList(filename, ref);

		contourinfo_ = contourlevels_->contourInfo();
		contourinfo_->ref();
		// It is an error for contourinfo_ to go away before 'this'.
		contourinfo_->lockResource();
		contourinfo_->scale((double) max(), TRUE);
		AvAccessor *a = accessor();
		// The data min/max is used to convert fractional levels
		// to absolute levels.
		if(a != NULL) // If NULL, there was an error somewhere.
			contourinfo_->setMinMax(a->clipMin(), a->clipMax());
		else
			contourinfo_->setMinMax(0.0, 1.0);

		contourinfo_->addCallback(
		 &AvCLUImageSetter::infoChangedCB,
		  AvContourInfo::ValueChanged, this, NULL);

		contourlevels_->addCallback(&AvCLUImageSetter::contourlistCB,
						this);
		// ViewportSetter for the overlayed contours.
		// Since its size needs to be fixed so the contours will
		// display when the raster is turned off, it is also
		// used to get the size of the scaled raster.
		vpi_ = new AvViewportSetter();

		vpi_->ref();
		ovps_ = new AvViewportSetter();
		ovps_->ref();

		//***********************************************************
		// The Color wedge and its vps are global to all slices
		// and shared. Size will be reset later.
		wedge_ = new AvColorWedge(a->clipMin(), a->clipMax(), 1, 1);
		wedgeVPS_ = new AvAxisVPS(wedge_, wedgeOptions_);
		wedgeVPS_->ref();
		// We'll control the size.
//		wedgeVPS_->computeSize(FALSE);
		// Set scale to something that will work if the initial
		// raster scale winds up being 1,1.
//		wedgeVPS_->setRasterScale(WEDGEWIDTH, 1.0);
		wedgeVPS_->setRasterScale(1.0, 1.0);
		sprintf(name, "wedgeVPS:%s", ivi->label());
		wedgeVPS_->name(name);
		{AvAxis *axis = wedgeVPS_->getAxis();
			// Put labels on right.
//			axis->changeYoptions(AvAxis::NONSTDLABELPOSITION);
			axis->name("ColorWedgeAxis");
			axis->title(NULL);
			axis->xLabel(NULL);
			axis->yLabel(NULL);
			axis->maxCharSize(10.0);
//			axis->xOptions(AvAxis::ZEROLINENOTICKS);
		}
		wedgeVPS_->ignore(!(showAxis() && showRaster()));
		//***********************************************************
	}
	else
	{	contourlevels_ = NULL;	// Error!
#if defined(DO_OVERLAY)
		overlaySetup_ = NULL;
#endif
	}
	setSlice(accessor_->getPlane(FALSE));	// slice DS coords.
#endif
}

// Change the currently viewed plane creating new CLUIRasters if necessary.
// (To display slice n, slices 0..n must exist).
// sliceIndex is a valid DS index, not subrange.
void AvCLUImageSetter::setSlice(const int sliceIndex)
{
int	nrasters = getLength(); // This is equal to the highest slice # +1.
int	zaxis = accessor()->zAxis();

	// Check for valid slice. Slice 0 should always be valid.
//	if((sliceIndex<0) || (sliceIndex >= accessor()->depth()))
	int dsDepth = accessor()->getDSAxisLength(zaxis);
	if((sliceIndex<0) || ((sliceIndex>0) && (sliceIndex >= dsDepth)))
	{	cerr <<
		"AvCLUImageSetter::setSlice called with bad sliceIndex("
		<< sliceIndex << ")" << endl;
		return;
	}
	// Make sure the display list has at least as many rasters as slices.
	if( sliceIndex >= nrasters)
	{int need = sliceIndex - nrasters +1;
	 // If the SR starting plane is > than the current highest slice,
	 // it is illegal to try to set the plane < start, so we
	 // temporarily change the start, length of the Z axis.
		int axisstart = accessor()->getAxisStart(zaxis);
		int axislen = -1;
		if(nrasters <= axisstart)
		{	axislen = accessor()->getAxisLength(zaxis);
			accessor()->disableNotify();
			accessor()->setAxisStart(zaxis, 0);
			int len = accessor()->getDSAxisLength(zaxis);
			accessor()->setAxisLength(zaxis, len);
		}
// ??		// Turn off touch so existing caches aren't removed.
		for(int i=0; i< need; i++)
			append( buildRasterObject(getLength()));
		if(axislen >= 0)
		{	accessor()->setAxisStart(zaxis, axisstart);
			accessor()->setAxisLength(zaxis, axislen);
			accessor()->enableNotify(FALSE);
		}
	}

//	if(changed)
//		touchRasters();

	setCurrentIndex(sliceIndex);	// Which slice to draw next.
}

AvCLUImageSetter::~AvCLUImageSetter ()
{
#ifdef PGPLOT
	delete contourlevels_;
	contourinfo_->removeCallback(&AvCLUImageSetter::infoChangedCB,
				AvContourInfo::ValueChanged, this, NULL);
	contourinfo_->unlockResource();
	contourinfo_->unref();
	ovps_->unref();

	vpi_->unref();
	wedgeVPS_->unref();
	delete axisSetup_;
	delete wedgeSetup_;
	axisSetup_ = wedgeSetup_ = NULL;
#endif
	axisOptions_->removeCallback(&AvCLUImageSetter::infoChangedCB,
				AvContourInfo::ValueChanged, this, NULL);
	axisOptions_->unref();
	wedgeOptions_->unref();
	wedgeOptions_->removeCallback(&AvCLUImageSetter::infoChangedCB,
				AvContourInfo::ValueChanged, this, NULL);
#if defined(DO_OVERLAY)
	overlayOptions_->unref();
	overlayOptions_->removeCallback(
				&AvCLUImageSetter::infoChangedCB,
				AvContourInfo::ValueChanged, this, NULL);
	overlay_->unref();
	overlay_ = NULL;
	delete overlaySetup_;
#endif
	// Urefs accessor and sets it to NULL.
	disconnect();
}

// "Clipped" min/max.
float AvCLUImageSetter::min()
{
float val = CLUImage()->min();

	return val;
}

float AvCLUImageSetter::max()
{float val = CLUImage()->max();

	return val;
}

// Mark the rasters as having been changed.
// (AvCLUImage::dataModified calls this).
void AvCLUImageSetter::touchRasters()
{
AvCLUIRaster *ras;
int	len = getLength();

	for(int i=0; i< len; i++)
		if((ras = getRaster(i)) != NULL)
			ras->touch();
}

// Set the size of a vp to the raster's width*scale + offset.
// This is used to set the size of this and the "axis" viewports.
void AvCLUImageSetter::setScaledSize( AvViewportSetter *vp,
				      const Boolean useOffsets)
{
float	xscl, yscl;
int	w, h;

	if(vp == NULL)
		return;

	viewport()->getRasterScale(xscl, yscl);
	// We shouldn't have to count on the X Canvas.
	xscl = AvXCanvas::convertScale(xscl);
	yscl = AvXCanvas::convertScale(yscl);

	w = CLUImage()->width();
	h = CLUImage()->height();

	w = int(w*xscl);	// Scaled size.
	h = int(h*yscl);

	if(useOffsets)
	{int	mw, mh;
		getMargins(mw, mh);
		// The offsets are on all sides.
		w += mw;
		h += mh;
	}
//printf("Setting %s to %d %d\n", vp->getName(), w, h);
	vp->setSize(w, h);
}

// Set the size of most (all?) of the viewports.
//	"This" is set only so getBoundingBox() has less work to do.
//	The axis viewports are set to the size of the raster + offsets.
//	The raster viewports are set to the size of the raster so that
//	turning a raster off won't collapse the viewport.
//	The character scaling and axis offsets get recomputed.

// Reposition the raster and overlayed contour vps.
// The color wedge
void AvCLUImageSetter::setScaledSizes( )
{
int	lm, rm, tm, bm;
int	len;

	// Set the size of vpi_ w/o offsets.
	setScaledSize(vpi_, FALSE);

	setScaledSize(this, TRUE);
	setScaledSize(ovps_, TRUE);

	// Change the offsets of anything that moves when the axes are turned
	// on or off.
	getMargins(lm, rm, tm, bm);

	// Set offset for overlayed contours.
	vpi_->setPosition(lm, bm);
	ovps_->setPosition(lm, bm);
#if defined(DO_OVERLAY)
	overlay_->setMargins(lm, rm, tm, bm);
#endif
	// If the axis is turned off, there is no need to reset the unseen.
	if(!showAxis())
		return;

	// Set size and offset for the wedge. Since the wedge is supposed
	// to fit into the right margin of the raster's axis, it is usually
	// much taller than it is wide. In order for the text to come out
	// about reasonable, the wedge's viewport is made larger. The
	// wedge's margins are adjusted accordingly.
#if 1
	int ww, wh, wx, wy, wlm, wrm;	// Size and position of wedgeVPS.
	wx = lm+vpi_->viewport()->width() + WEDGEOFFSET;
	wy = bm - WEDGEBM;
	ww = rm;
	wh = vpi_->viewport()->height();	// Size of scaled raster.
	// Set the width to one. The effective scaling will be set to
	// WEDGEWIDTH elsewhere. This is more effecient than recomputing
	// the same pixel along each row.
//	wedge_->setSize(1, wh);
	wedge_->setSize(WEDGEWIDTH, wh);
	wh += WEDGETM + WEDGEBM;		// Plus some extra for vp.
	// Adjust so viewport is about square.
	if(ww < wh)
		wlm = wh - ww;
	else
		wlm = 0;

	wx -= wlm;				// Move vp to left
	ww += wlm;
	wrm = rm-WEDGEWIDTH-WEDGEOFFSET;

	wedgeVPS_->setPosition(wx, wy);
	wedgeVPS_->setMargins(wlm, wrm, WEDGETM, WEDGEBM);
	wedgeVPS_->setSize(ww, wh);
	wedgeVPS_->getRasterVPS()->setPosition(wlm, WEDGEBM);
#else
	unsigned int ww, wh;		// Position of wedgeVPS.
	int wx, wy, wlm, wrm;		// Size of wedgeVPS.
	getSize(ww, wh);
	getPosition(wx, wy);
	wlm = lm+vpi_->viewport()->width() + WEDGEOFFSET;
	wrm = rm-WEDGEWIDTH-WEDGEOFFSET;

	wedgeVPS_->setPosition(wx, wy);
	wedgeVPS_->setMargins(wlm, wrm, tm, bm);
	wedgeVPS_->setSize(ww, wh);
//	wedgeVPS_->getRasterVPS()->setPosition(wlm, 0);
#endif

#if 0
printf("Setting wedgevps size to %d/%d, margins: %d/%d, x/y %d/%d \n",
			ww, wh, wlm, wrm, wx, wy);
#endif

	// Force each axis vps to recompute its size.
	len = getLength();
	for(int i=0; i< len; i++)
	{AvAxisVPS *avps;

		if(( avps = getAxisVPS(i)) != NULL)
		{//	avps->touch();
			avps->setMargins(lm_,rm_, tm_, bm_);
		}
	}
}

void AvCLUImageSetter::setRasterScale( const float xscl, const float yscl)
{
float	rxscl, ryscl;

	// If current values are same as new, ignore. (This routine
	// currently gets called more than once during a window resize).
	viewport()->getRasterScale(rxscl, ryscl);
	if( (rxscl == xscl) && (ryscl == yscl))
		return;

	AvViewportSetter::setRasterScale(xscl, yscl);

#if 1
	// Get approximate size of the scaled raster and adjust margins
	// so the viewport is more nearly square.
	{AvRaster *ras = getRaster(0);
		if(ras != NULL)
		{int w = int(ras->width()*xscl), W;
		 int h = int(ras->height()*yscl), H;
		 int delta, adj;

			if(w < 100)
				lm_ = rm_ = 64;
			else
			if( w < 192)
				lm_ = rm_ = w/2;
			else
				lm_ = rm_ = 100;
//			tm_ = bm_ = 50;
			tm_ = bm_ = lm_/2;

			W = w + lm_ + rm_;
			H = h + tm_ + bm_;
			delta = W - H;
			if(delta < 0)
			{	adj = -delta/2;
				lm_ += adj;
				rm_ += adj;
			}
			else
			{	adj = delta/2;
				tm_ += adj;
				bm_ += adj;
			}
			if(lm_ > 100)
				lm_ = rm_ = 100;
			if(tm_ > 100)
				tm_ = bm_ = 100;

//printf("lm %d rm %d tm %d bm %d VPW %d VPH %d\n", lm_,rm_,tm_,bm_, w,h);
		}
	}
#endif
	// Set new size for this and all child vps's.
	// (Also affects x offset).
	setScaledSizes();

	// Set scaling so wedge is WEDGEWIDTH wide by 1*Height.
//	wedgeVPS_->setRasterScale(WEDGEWIDTH/xscl, 1./yscl);
	wedgeVPS_->setRasterScale(1.0/xscl, 1./yscl);
}

void AvCLUImageSetter::setRasterScale( const float scl)
{
	setRasterScale(scl, scl);
}

void AvCLUImageSetter::getRasterScale( float &xscl, float &yscl)
{
	viewport()->getRasterScale(xscl, yscl);
}

// Toggle between showing/hiding the contour setup window.
void AvCLUImageSetter::toggleContourLevelDisplay()
{
#ifdef PGPLOT
	if(contourLevels()->isUp())
		contourLevels()->popdown();
	else
		contourLevels()->popup();
#endif
}

// Toggle between showing/hiding the axis setup window.
void AvCLUImageSetter::toggleAxisSetupDisplay()
{
#ifdef PGPLOT
	if(axisSetup()->isUp())
		axisSetup()->popdown();
	else
		axisSetup()->popup();
#endif
}

// Toggle between showing/hiding the wedge setup window.
void AvCLUImageSetter::toggleWedgeSetupDisplay()
{
#ifdef PGPLOT
	if(wedgeSetup()->isUp())
		wedgeSetup()->popdown();
	else
		wedgeSetup()->popup();
#endif
}

// Toggle between showing/hiding the wedge setup window.
void AvCLUImageSetter::toggleOverlaySetupDisplay()
{
#if defined(DO_OVERLAY)
	if(overlaySetup()->isUp())
		overlaySetup()->popdown();
	else
		overlaySetup()->popup();
#endif
}

#ifdef PGPLOT
// Show/No Show This contour level.
void AvCLUImageSetter::showContours(const Boolean show)
{
int	nobjs;
AvContour *c;

	if(showContours_ == show)
		return;

	nobjs = getLength();
	for(int i=0; i< nobjs; i++)
		if((c = getContour(i)) != NULL)
			c->ignore(showContours_);
	showContours_ = show;
	redisplay();
}

// Change whether the axis is displayed. If a change occurs, the size of the
// viewports holding the axes have to be changed, the raster viewport
// offsets need changing and the window size needs to be adjusted.
void AvCLUImageSetter::showAxis(const Boolean show, const Boolean resize)
{
int	nobjs;

	if(showAxis_ == show)		// Ignore if no change.
		return;

	// Tell axis objects whether to ignore draw requests.
	nobjs = getLength();
	for(int i=0; i< nobjs; i++)
	{AvAxisVPS *a;
		if((a = getAxisVPS(i)) != NULL)
			a->showAxis(show);
	}
	showAxis_ = show;
//	wedgeVPS_->ignore(! (showAxis() && showRaster()));
	wedgeVPS_->ignore(! showWedge());

	setScaledSizes();	// Resize viewports. AND change xoffset.

	// Need to resize window and redisplay the image.
	if(resize)
		imageViewItem()->fitWindowToRaster(TRUE);
}
#else
void AvCLUImageSetter::showContours(const Boolean )
{
}
void AvCLUImageSetter::showAxis(const Boolean )
{
}
#endif

// Change whether the raster is displayed.
void AvCLUImageSetter::showRaster(const Boolean show)
{
int	nobjs;
AvRaster *c;

	if(showRaster_ == show)		// Ignore if no change.
		return;

	// Tell raster objects whether to ignore draw requests.
	nobjs = getLength();
	for(int i=0; i< nobjs; i++)
		if((c = getRaster(i)) != NULL)
			c->ignore(showRaster_);

	showRaster_ = show;
	setScaledSizes();

	wedgeVPS_->ignore(! showWedge());

	redisplay();
}

// Show/No Show contour levels from other windows.
void AvCLUImageSetter::showOtherContours(const Boolean show)
{
	if(show != showIncludedContours_)
	{	showIncludedContours_ = show;
		redisplay();
	}
}


Boolean AvCLUImageSetter::showWedge()const
{
	return showWedge_ & showRaster() & showAxis();
}

void AvCLUImageSetter::showWedge(const Boolean show)
{
	if(show != showWedge_)
	{	showWedge_ = show;
		wedgeVPS_->ignore(! showWedge());
		redisplay();
	}
}

#if defined(DO_OVERLAY)
void AvCLUImageSetter::showOverlay(const Boolean show)
{
	if(show != showOverlay_)
	{	showOverlay_ = show;
		overlay_->ignore(! showOverlay());
		redisplay();
	}
}
#else
void AvCLUImageSetter::showOverlay(const Boolean )
{
}
	
#endif


/////////////////////////////////////////////////////////////////////////////

/* The name is obsolete. Returns a ViewportSetter for the raster object.
There will be one of these for each plane that gets displayed.

There is a ViewportSetter large enough to hold the labeled axis (if
enabled). Within that there is a smaller one that holds the raster and
its contour object. There is one axis per slice rather than just one
for the cube because the axis title displays the slice number.

		AVPS	...	AVPS
		| |
		| ----- Axis
		VPR
		|
		|- Raster
		.- Contour

AVPS	Per slice axis viewportSetter.
VPR	Per raster viewportSetter
*/

AvViewportSetter *AvCLUImageSetter::buildRasterObject(const int sliceIndex)
{
AvViewportSetter *vpr;
AvAxisVPS	*avps;

	// Something to draw the raster slice.
	AvCLUIRaster *raster = new AvCLUIRaster(this, sliceIndex);
	raster->ignore(!showRaster());		// Don't show unless enabled.

	// This will hold the raster's viewportSetter and the axis object.
	avps = new AvAxisVPS(raster, axisOptions_, lm_, rm_, tm_, bm_);
	avps->showAxis(showAxis());
	// Share global options among all slices.
	AxisGlobalOptions *opt;
	AvAxis *axis = avps->getAxis();
	if(sliceIndex == 0)
		opt = AvAxis::makeGlobalOptions(raster);
	else
	{	AvAxisVPS *avps0 = (AvAxisVPS *) getObject(0);
		AvAxis *axis0 = avps0->getAxis();
		opt = axis0->globalOptions();
	}
	axis->globalOptions(opt);

	//		A viewport for the raster object.
	vpr = avps->getRasterVPS();

#ifdef PGPLOT
	// Contour object.
	AvContour *cntr = new AvContour(raster, contourinfo_);
	cntr->ignore(!showContours());
	vpr->append(cntr);
#endif

	// On the first call, initialize IDs of the types of children we'll
	// need to retrieve later.
	if( RASTER_ID < 0)
	{	RASTER_ID = AvCLUIRaster::getClassTypeID();
		AXISVPS_ID = AvAxisVPS::getClassTypeID();
		VPS_ID = AvViewportSetter::getClassTypeID();
#ifdef PGPLOT
		AXIS_ID = AvAxis::getClassTypeID();
		CONTOUR_ID = AvContour::getClassTypeID();
#endif
	}
	avps->append(wedgeVPS_);

	return avps;
}

// Set the margins.
void AvCLUImageSetter::setMargins(const int lm, const int rm,
						const int tm, const int bm)
{
	if((lm != lm_) || (rm != rm_) || (tm != tm_) || (bm != bm_))
	{	lm_ = lm;
		rm_ = rm;
		tm_ = tm;
		bm_ = bm;
		setScaledSizes();
	}
}


// Return the desired offsets.
void AvCLUImageSetter::getMargins(int &lm, int &rm, int &tm, int &bm)const
{
	if(showAxis())
	{	lm = lm_;
		rm = rm_;
		tm = tm_;
		bm = bm_;
	}
	else
	{	lm = 0;
		rm = 0;
		tm = 0;
		bm = 0;
	}
}

void AvCLUImageSetter::getMargins(int &width, int &height)const
{
int	lm, rm, tm, bm;

	getMargins(lm, rm, tm, bm);
	width = lm + rm;
	height = tm + bm;
}

	// Called by contourList
#ifdef PGPLOT
void AvCLUImageSetter::contourlistCB(AvContourList *sel, XtPointer xp,
							const int reason)
{
AvCLUImageSetter *is = (AvCLUImageSetter *)xp;

	if(is != NULL)
		is->handleContourList(sel, reason);
}
#endif

#ifdef PGPLOT
// Called by contourlistCB to do the actual work.
// If the done button is pressed (ACCEPT), a redisplay is done.
void AvCLUImageSetter::handleContourList(AvContourList *, const int reason)
{
	switch(reason) {
	case AvContourList::ACCEPT:
		redisplay();
		break;
	default:;
	}
}
#endif

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
	// Functions to retrieve display objects.
// These are obviously very sensitive to how the display list is generated.

// Return the first child that is derived from the ID. (An object is
// considered to be derived from itself). If vps is NULL, "this" is used.
AvGraphics2D *AvCLUImageSetter::getChild(
			const int childID, const AvViewportSetter *VPS)const
{
AvGraphics2D *child;
int	len;
const AvViewportSetter *vps = (VPS == NULL) ? this : VPS;

	// Look for first child with the correct ID.
	len = vps->getLength();
	for(int i=0; i< len; i++)
	{	child = vps->getObject(i);
		if(child == NULL)		// INTERNAL ERROR!
			return NULL;
		if(child->isDerivedFrom(childID))
			return child;
	}
	return NULL;
}

// If obj is derived from (or is) a viewportSetter, return it as a
// viewportSetter.
static inline AvViewportSetter *isVPS(AvGraphics2D *obj)
{
	if((obj != NULL) &&
	   ( obj->isDerivedFrom(AvViewportSetter::getClassTypeID())))
		return (AvViewportSetter *)obj;
	else
		return NULL;
}

// Return a pointer to the axis' viewportSetter for plane "index".
// The only children of the CLUImageSetter are viewportSetters themselves.
// The axis viewportSetter (or its children) contains every displayable
// object for the plane.
// If index < 0, the current object (of the CLUImageSetter) is used.
AvAxisVPS *AvCLUImageSetter::getAxisVPS(const int index)const
{
AvGraphics2D *obj;
AvAxisVPS *avps;

	obj = (index >= 0) ? getObject(index) : currentObject();
	if( (obj != NULL) && (obj->isDerivedFrom(AXISVPS_ID)))
		avps = (AvAxisVPS *)obj;
	else
		avps = NULL;

	return avps;
}

// Return a pointer to the raster's viewportSetter for plane "index".
AvViewportSetter *AvCLUImageSetter::getRasterVPS(const int index)const
{
AvGraphics2D *obj;
AvViewportSetter *vps;

	// Get the parent viewportSetter.
	vps = getAxisVPS(index);
	if(vps == NULL)
		return NULL;
	// The raster's viewportSetter
	obj = getChild(VPS_ID, vps);
	return isVPS(obj);
}

// Return a pointer to the raster object for the slice.
AvCLUIRaster *AvCLUImageSetter::getRaster(const int index)const
{
AvCLUIRaster *obj;
AvViewportSetter *vpr;

	vpr = getRasterVPS(index);
	if(vpr != NULL)
		obj = (AvCLUIRaster *) getChild(RASTER_ID, vpr);
	else
		obj = NULL;
	return obj;
}

#ifdef PGPLOT
// Return a pointer to the axis object for the slice.
AvAxis *AvCLUImageSetter::getAxis(const int index)const
{
AvAxis *obj;
AvViewportSetter *vpa;

	vpa = getAxisVPS(index);
	if(vpa != NULL)
		obj = (AvAxis *) getChild(AXIS_ID, vpa);
	else
		obj = NULL;
	return obj;
}

// Return a pointer to the contour object for the slice.
AvContour *AvCLUImageSetter::getContour(const int index)const
{
AvContour *obj;
AvViewportSetter *vpr;

	vpr = getRasterVPS(index);
	if(vpr != NULL)
		obj = (AvContour *) getChild(CONTOUR_ID, vpr);
	else
		obj = NULL;

	return obj;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
#endif

// Force a redisplay of the display list.
void AvCLUImageSetter::redisplay()
{
#ifdef PGPLOT
	imageViewItem_->getImageWindow()->showImage();
#endif
}

#ifdef PGPLOT
// Append the contours from the views list to vps if showOtherContours() is
// TRUE, otherwise append nothing.

void AvCLUImageSetter::getOverlays(const int noverlays,
				AvImageViewItem **views, AvViewportSetter *vps)
{
	if((noverlays == 0) || (views == NULL) || (vps == NULL))
		return;
	if(!showOtherContours())
		return;

	// Don't include contour for this object.
	AvContour *thiscontour = getContour();

	for(int i=0; i < noverlays; i++)
	{AvImageViewItem *vi = views[i];
	 AvContour *contour;

		// Get item's ImageSetter object.
		contour = vi->getCLUImage()->imageSetter()->getContour();
		if((contour != NULL) && (contour != thiscontour))
			vps->append(contour);
	}
}
#else
void AvCLUImageSetter::getOverLays(const int ,
				AvImageViewItem **, AvViewportSetter *)
{
}
#endif

#if 0
void AvCLUImageSetter::getOverlays(AvViewportSetter *vps)
{
int	noverlays;	// # of overlays to display.
AvImageViewItem **views;

	if(!showOtherContours())
		return;

	views = AvImageView::instance()->
		getImageViewList(noverlays, AvImageView::INCLUDED);

	getOverlays(noverlays, views, vps);
	delete [] views;
}

#else
void AvCLUImageSetter::getOverlays(AvViewportSetter *vps)
{
int	noverlays;	// # of overlays to display.
AvImageViewItem **views;

	if(showOtherContours())
	{	views = AvImageView::instance()->
			getImageViewList(noverlays, AvImageView::INCLUDED);
		getOverlays(noverlays, views, vps);
		delete [] views;
	}
#if 0
	if(showOverlay())
		vps->append(overlay_);
#endif
}
#endif

// Draw overlays. (Contours from other windows).
// Append all included windows (except this) to vpi_ and display.
// vpi_ has had its size and offsets setup to match our raster's.
void AvCLUImageSetter::drawIncludedContours(AvCanvas &canvas)
{
#ifdef PGPLOT
	if(!showOtherContours())
		return;

	getOverlays(vpi_);
	vpi_->draw(canvas);
	vpi_->removeList();	// Purge when done.
#endif
}

// Draw the overlay object. NOP if OVERLAY not defined.
#if defined(DO_OVERLAY)
void AvCLUImageSetter::drawOverlay(AvCanvas &canvas)
{
	if(!showOverlay())
		return;

	overlay_->draw(canvas);
}
#else
void AvCLUImageSetter::drawOverlay(AvCanvas &)
{
}
#endif

// The Canvas calls draw with a pointer to itself to tell the object
// to display itself. Since some of the objects on our display list need
// special handling, the standard Viewportsetter draw function has been
// overridden.
void AvCLUImageSetter::draw(AvCanvas &c)
{
	if(ignore())		// Don't display if we're not supposed to.
		return;
#ifdef PGPLOT
	wedge_->setMinMax(CLUImage_->min(), CLUImage_->max());
	wedgeVPS_->getAxis()->setWorldWindow(
			0, wedge_->width(),
			CLUImage_->min(), CLUImage_->max());
#endif

	AvViewportSetter::draw(c);

#ifdef PGPLOT
	// Contours included from other windows are not part of the display
	// list.
	drawIncludedContours(c);
	drawOverlay(c);
#endif
}

void AvCLUImageSetter::draw(AvCanvas *c)
{
	if(c != NULL)
		draw(*c);
}

#ifdef PGPLOT
// Called when the info struct has been modified.
void AvCLUImageSetter::infoChangedCB(XtPointer o, AvConductor *, XtPointer )
{
AvCLUImageSetter *me = (AvCLUImageSetter *)o;

	me->redisplay();
}
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef PGPLOT
			// Menus for the imageWindow

int AvCLUImageSetter::getShowMenuLength()
{
	return menuLen;
}

AvMenuButtonInfo *AvCLUImageSetter::getShowMenuInfo()
{
	return menus;
}

// Build the menus for the AvImageWindow to use
void AvCLUImageSetter::buildShowMenu(AvMenu *menu, const int tag)
{
int i;
AvMenuButtonInfo *m;

	for(m = menus, i=0; i< menuLen; i++, m++)
		menu->add(m->name, m->buttonType, tag, NULL, m->buttonData);
}

void AvCLUImageSetter::buildSetupMenu(AvMenu *menu, const int tag)
{
	menu->add(CONTOURS, PUSH_BUTTON, tag);
	menu->add(AXIS, PUSH_BUTTON, tag);
	menu->add(WEDGE, PUSH_BUTTON, tag);
#if defined(DO_OVERLAY)
	menu->add(OVERLAY, PUSH_BUTTON, tag);
#endif
}

#else

// Build the menus for the AvImageWindow to use... Dummies if no PGPLOT
void AvCLUImageSetter::buildShowMenu(AvMenu *, const int )
{
}

void AvCLUImageSetter::buildSetupMenu(AvMenu *, const int )
{
}
#endif

#ifdef PGPLOT
Boolean AvCLUImageSetter::processShowMenu(char *name, Widget mb)
{
Boolean	isSet = XmToggleButtonGetState(mb);

	if (!strcmp (name, RASTER))
		showRaster(isSet);
	else
	if (!strcmp (name, CONTOURS))
		showContours(isSet);
	else
	if (!strcmp (name, SHOWINCLUDED))
		 showOtherContours(isSet);
	else
	if (!strcmp (name, AXIS))
		 showAxis(isSet);
	else
	if (!strcmp (name, WEDGE))
		 showWedge(isSet);
#if defined(DO_OVERLAY)
	else
	if (!strcmp (name, OVERLAY))
		 showOverlay(isSet);
#endif
	else
		return FALSE;

	return TRUE;
}

Boolean AvCLUImageSetter::processSetupMenu(char *name, Widget /*mb*/)
{
	if (!strcmp (name, CONTOURS))
		toggleContourLevelDisplay();
	else
	if (!strcmp (name, AXIS))
		toggleAxisSetupDisplay();
	else
	if (!strcmp (name, WEDGE))
		toggleWedgeSetupDisplay();
#if defined(DO_OVERLAY)
	else
	if (!strcmp (name, OVERLAY))
		toggleOverlaySetupDisplay();
#endif
	else
		return FALSE;

	return TRUE;
}
#else

Boolean AvCLUImageSetter::processShowMenu(char *, Widget )
{
	return FALSE;
}

Boolean AvCLUImageSetter::processSetupMenu(char *, Widget )
{
	return FALSE;
}
#endif

char *AvCLUImageSetter::getPrintName()
{
int len, c;
//char *buf, *name, *bptr;
char *buf, *bptr;
AvRaster *ras = getRaster();

#if 0
	if(ras != NULL)
		const char *name = ras->getName();
	else
		const char *name = getName();		// Punt.
#endif

	const char *name = (ras != NULL) ? ras->getName() : getName();

	len = (int)strlen(name) + 1;
	bptr = buf = new char[len];

	// Copy removing blanks and translating colons into dashes.
	while((c = *name++) != '\0')
		if(c == ':')
			*bptr++ = '-';
		else
		if(c == ' ')
			*bptr++ = '_';
		else
		if( isalnum(c) || (c == '.') || (c == '-') || (c == '_'))
			*bptr++ = c;

	*bptr = '\0';
	return buf;
}

 // Callback to receive accessor notices.
void AvCLUImageSetter::accessorCB_(XtPointer dis, AvConductor *c, XtPointer uD)
{ AvCLUImageSetter *me = (AvCLUImageSetter *) dis;
	me->accessorCB(c, uD);
}

 // Callback to actually handle accessor notices.
//void AvCLUImageSetter::accessorCB(AvConductor *c, XtPointer uData)
void AvCLUImageSetter::accessorCB(AvConductor *c, XtPointer )
{int r = c->callbackReason();

//	cerr << "AvCLUImageSetter:accessorCB called: ";
//	cerr << AvAccessor::reasonToString(r).chars() << endl;
	if(r & AvAccessor::PLANE)
		setSlice(accessor()->getPlane(FALSE));
	if(r & (AvAccessor::DISCONNECT | AvAccessor::Destroyed))
	{  // Disconnect from accessor
		disconnect();
	}
}

void AvCLUImageSetter::disconnect()
{//AvAccessor *ac = accessor();

	if(accessor_ != NULL)
	{	accessor_->removeCallback(&AvCLUImageSetter::accessorCB_,
					   ACMASK, this, NULL);
		accessor_->unref();
		accessor_ = NULL;
	}
}
