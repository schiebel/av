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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvImageViewItem.cc,v 19.0 2003/07/16 05:47:55 aips2adm Exp $
//
// $Log: AvImageViewItem.cc,v $
// Revision 19.0  2003/07/16 05:47:55  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:31  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/03 22:17:05  hravlin
// Mostly changed to support wiregl.
//
// Revision 17.0  2001/11/12 19:42:59  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:05  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.1  2000/12/21 20:21:54  hravlin
// Disabled showSlice() since it is no longer needed and results in an extra redraw.
// Similarly, updateView() should no longer cause a redraw.
//
// Revision 15.0  2000/10/26 17:12:27  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:09:01  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:51  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:09  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:54  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:56:06  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1997/09/15 16:27:09  hr
// Leave extension on file name.
//
// Revision 9.0  1997/08/25 21:31:10  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.4  1997/08/01 21:11:27  hr
// Changed pos.setXYZValues() to pos.setValues().
//
// Revision 8.3  1997/06/09 20:11:05  hr
// In destructor was sending disconnect to the accessor, but shouldn't since
// subwindows may remain after user closes main window.
// In validWorld(), was checking for rank >= 2.
//
// Revision 8.2  1997/05/20 19:41:27  hr
// Needed a check for rank in XYZToWorld.
//
// Revision 8.1  1997/02/25 17:08:49  hr
// Added new constructor. Renamed myAxis_ to idKey_. Changes associated
// with support of data sets with rank > 3.
//
// Revision 8.0  1997/02/20 03:18:07  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.4  1997/02/05 17:39:09  hr
// Removed some unused variables.
//
// Revision 7.3  1996/12/12 09:01:29  droberts
// Final update from monet archive.
//
// Revision 1.12  1996/11/11 17:58:45  hr
// Moved equalScales() to AvImageWindow.cc
//
// Revision 1.11  1996/11/11 15:54:45  hr
// Added equalScales(). Wherever scalefactor was used, changed to use
// x & y scale factors.
//
// Revision 1.10  1996/10/02 16:53:49  hr
// Added length initialization to an AvWposition.
//
// Revision 1.9  1996/10/01 21:59:42  hr
// Removed validSlice().
// showSlice ignores its argument.
//
// Revision 1.8  1996/09/27 16:45:09  hr
// Added myAxis_ variable to keep track of the view axis/index.
//
// Revision 1.7  1996/09/25 21:51:34  hr
// Changes to deal with no longer being a friend of ImageViewItem.
// Check to make sure we don't set Z axis if there isn't one.
// Fixes to the usual DS vs SR problems.
//
// Revision 1.6  1996/09/20 20:19:35  pixton
// Prefixed all classes with Av
//
// Revision 1.5  1996/09/18  19:08:52  hr
// More integration with AvAccessor.
//
// Revision 1.4  1996/09/10 16:53:00  hr
// Initial changes for integration with AvAccessor & AvDataSet.
//
// Revision 1.3  1996/08/19 20:32:41  pixton
// added casts to deal with Polly's String32
//
// Revision 1.2  1996/08/14  18:54:09  hr
// Changes to remove g++ warnings (from bglenden).
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.14  1996/06/18  18:46:13  pixton
//  Copyright Update
//
//  Revision 1.13  1996/03/27  22:16:46  hr
//  If the AvImageViewItem is being created from an uninitialized region,
//  the region is assumed to be the full surface.
//
//  Revision 1.12  1996/03/22  20:48:01  hr
//  Sometimes numSlices returns 0. validSlice was modified to return TRUE
//  if the slice number is 0.
//
//  Revision 1.11  1996/03/20  17:54:59  hr
//  Changed validIJK to return TRUE if k is valid for the cube rather than
//  within minIJK/maxIJK values.
//
//  Revision 1.10  1996/02/23  17:03:39  hr
//  Moved from using x/y offsets to t,b,l,r margins.
//  Started using AvCLUImage's AvCLUImageSetter access routines.
//
//  Revision 1.9  1996/02/08  18:08:10  hr
//  Changes to allow axis to be displayed with initial window.
//
//  Revision 1.8  1996/01/30  22:52:46  hr
//  Changed fitWindowToRaster and fitRasterToWindow to handle possible offset
//  of the raster image.
//
//  Revision 1.7  1996/01/24  18:12:43  hr
//  Moved creation of raster_ to after label_ is created.
//
//  Revision 1.6  1996/01/08  22:06:10  hr
//  Pass pointer to AvImageViewItem to AvCLUImageSetter.
//
//  Revision 1.5  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/07/26  21:23:55  hr
//  Changed way getMinMax gets zaxis values so WorldToIJK wouldn't generate
//  divide by 0s.
//
//  Revision 1.3  1995/07/21  17:12:48  hr
//  fitWindowToRaster redraw logic needed reworking again.
//
//  Revision 1.2  1995/07/11  21:21:36  hr
//  Edited fitWindowToRaster to take into account that it can indirectly
//  call itself.
//
//  Revision 1.1  1995/06/12  15:20:33  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

#include <AvImageViewItem.h>
#include <AvImageWindow.h>
#include <AvCLUImage.h>
#include <AvImageDataPanel.h>
#include <AvImageDataItem.h>
#include <AvImageViewPanel.h>
#include <AvWPosition.h>
#include <AvXColorApp.h>	// For externalDrawingArea().

// Create a "sub view" from an existing view.
AvImageViewItem::AvImageViewItem (const AvImageViewItem & item) :
dItem_ (item.dItem_)
{
  vItem_ = &item;

  // Reset size to that of the region, but only if the region exists.
  int minX, maxX, minY, maxY;

  // Create accessor.
  accessor_ = new AvAccessor( *vItem_->accessor());
  accessor_->ref();
// Can't lock since other constructor doesn't create.
//  accessor_->lockResource();

  // Set the subregion.
	// Set (sub)region.
	item.getImageWindow()->viewPanel()->getRegion(minX, minY, maxX, maxY);
	accessor_->setAxisStart(0, minX);
	accessor_->setAxisLength(0, maxX-minX + 1);
	accessor_->setAxisStart(1, minY);
	accessor_->setAxisLength(1, maxY-minY + 1);
	if(accessor_->rank() > 2)
	{ int z0 = item.accessor()->getAxisStart(zAxis());
	  int len = item.accessor()->getAxisLength(zAxis());
		accessor_->setAxisStart(zAxis(), z0);
		accessor_->setAxisLength(zAxis(), len);
	}

  // add this into the list of views of this data
  dItem_.addSubview (this);

  init ();
}

AvImageViewItem::AvImageViewItem(AvImageDataItem & d,
				 const int idKey,
				 AvAccessor *a) : dItem_ (d)
{
  vItem_ = NULL;

  accessor_ = a;
  accessor_->ref();
//  accessor_->lockResource();

  // initialize the rest of the instance.
  init (idKey);
}


void AvImageViewItem::init (const int idKey)
{
  Widget parent = dItem_.dataPanel()->baseWidget();

  // For whole views, this is effectively the index into the
  // ImageDataItem::views_ array. We can't use the mapped axis since axis
  // numbers could be >> 2.
  idKey_ = idKey;

#if 0
	// (Leave the extension since AIPS++ uses it.
  //		Generate title for windows.
  // strip the suffix from the file name 
  // *** this doesn't belong here -- move this to AvDataSet?
  char shortFile[64];
  char * suffix;
  const char *fName = dItem_.name();

  suffix = strstr (fName, ".fits");
  if (!suffix)
    suffix = strstr (fName, ".FITS");
  if (!suffix)
    suffix = strstr (fName, ".Fits");

  int shortLength;
  if (suffix)
    shortLength = strlen(fName) - strlen (suffix);
  else
    shortLength = strlen(fName);

  strncpy (shortFile, fName, shortLength);
  shortFile[shortLength] = '\0';

  if (subview ()) 
    sprintf (label_, "%s.%s-%s.%d",
	     shortFile,
	     (const char *) accessor_->axisName(xAxis()),
	     (const char *) accessor_->axisName(yAxis()),
	     dItem_.nSubviews(viewAxis()));
    else
      sprintf (label_, "%s.%s-%s",
	       shortFile,
	     (const char *) accessor_->axisName(xAxis()),
	     (const char *) accessor_->axisName(yAxis()));

#else
  const char *fName = dItem_.name();
  if (subview ()) 
    sprintf (label_, "%s.%s-%s.%d",
	     fName,
	     (const char *) accessor_->axisName(xAxis()),
	     (const char *) accessor_->axisName(yAxis()),
	     dItem_.nSubviews(viewAxis()));
    else
      sprintf (label_, "%s.%s-%s",
	       fName,
	     (const char *) accessor_->axisName(xAxis()),
	     (const char *) accessor_->axisName(yAxis()));

#endif

  // Make the raster that shows this slice of the Image
  // (AvCLUImage needs label_ to be set).
  raster_ = new AvCLUImage (this);

  // We have to play games if the user has requested that the axis be shown
  // as default. The space need by the axis is dependent on the scaled size
  // of the image. However, that isn't figured out until the fitRasterToWindow
  // call. If showAxis is TRUE before then, sizes get messed up. So, the
  // axis is disabled until everything is initialized and then turned back on.
  Boolean showAxis = raster_->showAxis();
  if(showAxis)
  {	raster_->showAxis(FALSE, FALSE);
  }

  // AvImageWindow needs raster_ & setSlice called.;
  window_ = new AvImageWindow (parent, FALSE, this);

  window_->showImage (raster_, FALSE);

  // set the raster slice size to fit inside the window
  fitRasterToWindow ();
  if(showAxis)
  	raster_->showAxis(TRUE, FALSE);

  // Resize the window, maybe, if the raster image didn't fill window.
  fitWindowToRaster (FALSE);	// Will get displayed during resize request.

  window_->show ();

  // Connect to accessor.
  accessor_->addCallback(&AvImageViewItem::accessorCB_,
			 AvAccessor::DEFAULTMASK | AvDataSet::DEFAULTMASK,
			 this,
			 NULL);
}

void AvImageViewItem::deleteView ()
{
  // if this is a whole-slice axis view, ask the data item to 
  // delete so that it can also update its panel 
#if 0
  if (!subview ()) 
      if(idKey_ < 0)	// Does axis exist?
      dItem_.deleteView (2);	// If not, this is the default location.
    else
      dItem_.deleteView (idKey_);
  else
    dItem_.deleteSubview (this);
#else
  if (!subview ()) 
      dItem_.deleteView (idKey_);
  else
    dItem_.deleteSubview (this);
#endif
}

void AvImageDataItem::deleteSubview(AvImageViewItem * item)
{ 
  AvImageViewItem * ivi = subViews_.remove(item);
  if (ivi) delete ivi; 
}


// Is this a subview?
int AvImageViewItem::subview ()
{
  if (vItem_)
    return TRUE;
  else
    return FALSE;
}

void AvImageViewItem::showSlice (int /*which */)
{
#if 0
	// This routine is now redundant.
	// Some of this probably winds up being redundant now.
//	accessor_->setPlane(which);	// (Be careful of SR vs DS).
	if(raster_)
	{//	raster_->setSlice(accessor_);
		window_->updateImageInfo(raster_, TRUE);
	}
#endif
}

void AvImageViewItem::updateView ()
{
  // Don't want to redraw from here.
  window_->updateImageInfo(raster_, FALSE);
}

void AvImageViewItem::fitRasterToWindow (const Boolean redraw)
{
  // get the size of the raster's drawing area...
  Dimension ww, hh;
  int marginWidth, marginHeight;
  if(AvXColorApp::externalDrawingArea())
  { int cw, ch;
	cw = window_->canvas()->width();
	ch = window_->canvas()->height();
#if 0
	printf("AvImageViewItem::fitRasterToWindow: wid w/h %d/%d, cw/h %d/%d\n",
		ww, hh, cw, ch);
#endif
	ww = cw;
	hh = ch;
  }
  else
   XtVaGetValues (window_->drawArea(), XmNwidth, &ww, XmNheight, &hh, NULL);

  // Pickup current offsets.
  getCLUImage()->getMargins(marginWidth, marginHeight);
  // Remove from size.
  if(marginWidth < ww)
    ww -= marginWidth;	// In certain bizarre (test) cases,
  if(marginHeight < hh)	// the window can be smaller than the margins.
    hh -= marginHeight;

  // get the minimum scale factor
  float scaleWidth = (float) ww / raster_->width();
  float scaleHeight = (float) hh / raster_->height();
  if(getImageWindow()->equalScales())
  {  float sf = MIN(scaleWidth, scaleHeight);
     scaleWidth = scaleHeight = sf;
  }
  window_->scale (scaleWidth, scaleHeight, redraw);
#if 0
  cerr	<< "AvImageViewItem::fitRasterToWindow\t" << scaleWidth
	<< " " << scaleHeight << endl;
#endif
//  showSizeInfo ();
}

void AvImageViewItem::fitWindowToRaster (const Boolean redraw)
{
static int inUse=0;	// Only 1 window at a time gets changed so static works

  if(AvXColorApp::externalDrawingArea())
  {//	printf("IVI:fWTR calling fitRasterToWindow redraw=%d\n", redraw);
	fitRasterToWindow(redraw);
	return;
  }

  // This should only happen if we're in the middle of changing the code.
  if(window_ == NULL)
  {	cerr << "AvImageViewItem::fitWindowToRaster: window_ is NULL" << endl;
	return;
  }
  float xsf = window_->xScaleFactor();
  float ysf = window_->yScaleFactor();
  // if scale factor is greater than 1, truncate to integer value...
  if (xsf > 1.0)
    xsf = (int) xsf;
  if (ysf > 1.0)
    ysf = (int) ysf;

  Dimension width, height;
  int w, h, marginWidth, marginHeight;
  
  if(AvXColorApp::externalDrawingArea())
  {	width = window_->canvas()->width();
	height = window_->canvas()->height();
  } else
	  XtVaGetValues (window_->drawArea(),
        	         XmNwidth, &width,
                	 XmNheight, &height,
	                 NULL);

  getCLUImage()->getMargins(marginWidth, marginHeight);
  w = int(raster_->width()*xsf + marginWidth);
  h = int(raster_->height()*ysf + marginHeight);

  // If the size needs changing, change it. This usually results in a
  // recursive call to fitWindowToRaster again. Redraw image when back
  // at the top level.
  // On downscales, inUse can reach two, but the corresponding call to
  // SetValues won't result in another callback. On upscales, inUse will go
  // to one and on the last entry, w == width, etc.

//cout << "fitWindowToRaster:inUse= " << inUse << endl;

  if((w != width) || ( h != height))
  {     width = w; height = h;

	inUse += 1;
	// For some reason, this can take a REALLY long time on upscales.
	XtVaSetValues(  window_->drawArea(),
			XmNwidth, width,
			XmNheight, height,
			NULL);

	inUse -= 1;
	// If we're done size changing, force a redraw.
	if(inUse == 0)
	  	window_->showImage(raster_);
  }
  else    // Redisplay image if ordered and we're at upper level.
  if(redraw && ( inUse == 0))
  	window_->showImage(raster_);
}

void AvImageViewItem::showSizeInfo ()
{
  Dimension ww, hh;
  XtVaGetValues (window_->drawArea(), XmNwidth, &ww, XmNheight, &hh, NULL);

  cerr << "drawArea\t" << ww << '\t' << hh << endl;

  float rw, rh;
  rw = raster_->width ();
  rh = raster_->height ();

  cerr << "raster\t" << rw << '\t' << rh << endl;

  float sw, sh;
  sw = (float) ww / rw;
  sh = (float) hh / rh;
  float sf = MIN(sw, sh);

  cerr << "scale\t" << sw << '\t' << sh << '\t' << sf << endl;
}


AvImageViewItem::~AvImageViewItem ()
{
//  delete viewPanel_;
  accessor_->removeCallback(&AvImageViewItem::accessorCB_,
			 AvAccessor::DEFAULTMASK | AvDataSet::DEFAULTMASK,
			 this,
			 NULL);
// Don't disconnect accessor or all subwindows go away too.
//  accessor_->disconnect();
  delete raster_;
  delete window_;
#if 0
  accessor_->unlockResource();
  if(accessor_->getReferenceCount() != 1)
  {	cerr <<
"AvImageViewItem::~AvImageViewItem: accessor_ ref count should be 1, not "
 << accessor_->getReferenceCount() << endl;
  }
#endif
  accessor_->unref();
  accessor_ = NULL;
}

// Are the World values valid?
Boolean AvImageViewItem::validWorld(const double x, const double y)
{
AvIPosition ijk;
AvWPosition wp = accessor()->minWorld();
//int rank = accessor()->rank();

	wp(0) = x;
	wp(1) = y;
	return accessor()->WorldToXYZ(wp, ijk);
}

Boolean AvImageViewItem::validWorld(const double x, const double y,
								const double z)
{
AvIPosition ijk;
//AvWPosition wp = accessor()->minWorld();
AvWPosition wp(accessor()->minWorld());
int rank = accessor()->rank();

	wp(0) = x;
	wp(1) = y;
	if(rank > 2)
		wp(2) = z;

	return accessor()->WorldToXYZ(wp, ijk);
}

// Convert XYZ coordinates to World.
Boolean AvImageViewItem::XYZToWorld(const int x, const int y, const int z,
                                        AvPosition &pos)
{
int rank = accessor()->rank();
AvWPosition world(rank);

	if(!accessor()->XYZToWorld(x, y, z, world))
		return FALSE;
        else
	{	if(rank > 2)
			pos.setValues(world, 3);
//			pos.setXYZValues(world(0), world(1), world(2));
		else
			pos.setValues(world, 2);
//			pos.setXYValues(world(0), world(1));

		return TRUE;
	}
}

/////////////////

// Convert World coordinates to XYZ (subrange) coordinates.
Boolean AvImageViewItem::WorldToXYZ(const double x, const double y,
			const double z, int &i, int &j, int &k)
{
//	AvWPosition world = accessor()->minWorld();
	AvWPosition world = accessor()->minRegion(); // In case rank > 3.
	world(0) = x;
	world(1) = y;
	if(accessor()->rank() > 2)
		world(2) = z;
	return accessor()->WorldToXYZ(world, i, j, k);
}

// Convert World coordinates to XYZ (subrange) coordinates.
Boolean AvImageViewItem::WorldToXYZ(const AvPosition &pos,
				    int &i, int &j, int &k)
{
  double  x, y, z;
  
  pos.getXYZValues(x, y, z);
  return WorldToXYZ(x, y, z, i, j, k);
}

 // Callback to receive accessor notices.
void AvImageViewItem::accessorCB_(XtPointer dis, AvConductor *c, XtPointer uD)
{ AvImageViewItem *me = (AvImageViewItem *) dis;
	me->accessorCB(c, uD);
}

 // Callback to actually handle accessor notices.
//void AvImageViewItem::accessorCB(AvConductor *c, XtPointer uData)
void AvImageViewItem::accessorCB(AvConductor *, XtPointer )
{//int r = c->callbackReason();
	// Later.
//	cerr << "AvImageViewItem:accessorCB called: ";
//	cerr << AvAccessor::reasonToString(r) << endl;
}
