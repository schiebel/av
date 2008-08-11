//# Copyright (C) 1995-2000 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvCLUImage.cc,v 19.0 2003/07/16 05:46:33 aips2adm Exp $
//
// $Log: AvCLUImage.cc,v $
// Revision 19.0  2003/07/16 05:46:33  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:20  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:41:48  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:41:59  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:07:58  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 18:15:26  hravlin
// Changed className() to return const char *.
//
// Revision 14.0  2000/03/23 16:07:34  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:14  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:22  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:57:56  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:51:27  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:23:52  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:14:36  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 10:03:02  droberts
// Final update from monet archive.
//
// Revision 1.4  1996/11/11 15:49:35  hr
// Replaced scale() by setScale() and added getScale() during conversion
// to two scale factors.
//
// Revision 1.3  1996/09/18 18:49:13  hr
// Removed some old commented out code and more integration with AvAccessor.
//
// Revision 1.2  1996/09/10 16:50:51  hr
// Initial changes for integration with AvAccessor & AvDataSet.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.14  1996/06/18  18:45:49  pixton
//  Copyright Update
//
//  Revision 1.13  1996/02/23  18:09:43  hr
//  Had forgotten a return value on showAxis().
//
//  Revision 1.12  1996/02/23  17:07:25  hr
//  Added some functions to interface to AvCLUImageSetter.
//
//  Revision 1.11  1996/02/08  18:09:56  hr
//  The constructor now does more initialization. setSlice now initialzes
//  width_ and height_.
//  Changed dataModified to call AvImageSetter::touchRasters rather than
//  touch().
//
//  Revision 1.10  1996/01/24  17:59:06  hr
//  Use the imageViewItem to set the data. Replaced some calls to is_->touch()
//  with dataModifed(), make sure is_ is initialized.
//
//  Revision 1.9  1996/01/08  22:06:10  hr
//  Pass pointer to AvImageViewItem to AvCLUImageSetter.
//
//  Revision 1.8  1996/01/08  20:08:49  hr
//  Added imageSetter() function.
//
//  Revision 1.7  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.7  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.6  1995/05/24  14:07:09  hr
//  Added call to AvXCanvas::convertScale in ::scale so window to IJK
//  conversions would track.
//
//  Revision 1.5  1995/05/08  17:19:06  hr
//  Was including a file twice.
//
//  Revision 1.4  1995/05/05  20:46:34  hr
//  Added dataModified.
//  Removed ceil function from scale.
//
//  Revision 1.3  1995/04/14  20:01:58  hr
//  Include AvCLUImageSetter.h rather than AvGDRaster.h
//
//  Revision 1.2  1995/04/14  19:33:08  hr
//  Changes to handle pixmap caching.
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#include <math.h>
#include <AvXCanvas.h>
#include <AvColormap.h>
#include <AvCLUImageSetter.h>
#include <AvImageViewItem.h>

#include <AvCLUImage.h>

int AvCLUImage::nObjects = 0;


const char * AvCLUImage::className () const
{
  return ("AvCLUImage");
}


AvCLUImage::AvCLUImage (AvImageViewItem *ivi)
{
  dispWin = NULL;
  colormap = NULL;
  accessor_ = ivi->accessor();
  accessor_->ref();

  nObjects++;
  // The CLUImageSetter needs some things from ivi. (Primarily a widget).
  is_ = new AvCLUImageSetter(this, ivi);
  is_->ref();
//  setSlice(ivi->accessor());
}

#if 0
void AvCLUImage::setSlice (int x, int y, int view, int sliceIndex, 
			AvAccessor *  /*acc */)
{
int	w,h;
Boolean changed;

  if(region != NULL)
  {	w = region->getDimSize(xAxis_);
	h = region->getDimSize(yAxis_);
	  changed =	(x != xAxis_) || (y != yAxis_) || (view != viewAxis_)
						||
			(r != region) || (w != width_) || (h != height_);
  }
  else
	changed = TRUE;

  viewAxis_ = view;
  xAxis_ = x;
  yAxis_ = y;
  region = r;
  width_ = width ();
  height_ = height ();

  if(is_ != NULL)
	is_->setSlice(sliceIndex, changed);
}
#else
// Change the currently viewed slice.
void AvCLUImage::setSlice (AvAccessor */*acc*/)
{
	cerr << "AvCLUImage::setSlice called.\n";
//  if(is_ != NULL)
//	is_->setSlice(acc->getPlane());
}
#endif

void AvCLUImage::setDisplayWindow (AvDisplayWin2D * w)
{
  dispWin = w;
}

void AvCLUImage::setColormap (AvColormap * in)
{
  if(colormap != in)
  {	colormap = in;
	dataModified();
  }
}

void AvCLUImage::setColor (AvAccessor */* in */)
{
#if 0
  if((colorData != in->getData ()) || (grid != in->getGrid()))
  {	colorData = in->getData ();
	grid = in->getGrid ();

	// Perform equivalent of map().
	clipMin = colorData->getClipMin ();
	clipMax = colorData->getClipMax ();

	dataModified();
  }
#else
	cerr << "AvCLUImage::setColor called.\n";
#endif
}

/* This function should be called when the contents of the
AvGriddedData data has changed. It tells the imageSetter to mark all
rasters as having been modified so the canvas will rebuild any pixmap
caches.
*/
void AvCLUImage::dataModified()
{
	if(is_ != NULL)
		is_->touchRasters();
}

int AvCLUImage::width ()
{
//  return (region->getDimSize (xAxis_));
	return accessor_->width();
}

int AvCLUImage::height ()
{
//  return (region->getDimSize (yAxis_));
	return accessor_->height();
}


void AvCLUImage::map ()
{
#if 0
  clipMin = colorData->getClipMin ();
  clipMax = colorData->getClipMax ();

//  cerr << endl << *region;

  width_ = width ();
  height_ = height ();
  
//  cerr << width_ << '\t' << height_ << endl;
#else
	cerr << "AvCLUIMage::map called\n";
#endif
}


void AvCLUImage::setScale (const float xscaleFactor, const float yscaleFactor)
{
  float xsf = xscaleFactor;
  float ysf = yscaleFactor;

  // if we're supsampling, we are currently limited to every 2nd, every
  // 3rd, every 4th pixel, etc.  So create the right scale factor value
  // to give to the viewport scale setting. (And to use for converting
  // to/from window coordinates).
  xsf = AvXCanvas::convertScale(xsf);
  ysf = AvXCanvas::convertScale(ysf);
  is_->setRasterScale (xsf, ysf);
}

void AvCLUImage::getScale (float &xscaleFactor, float &yscaleFactor)const
{
  is_->getRasterScale (xscaleFactor, yscaleFactor);
}

#if 0
float AvCLUImage::scale (float scaleFactor)
{
  float sf = scaleFactor;

  // if we're supsampling, we are currently limited to every 2nd, every
  // 3rd, every 4th pixel, etc.  So create the right scale factor value
  // to give to the viewport scale setting. (And to use for converting
  // to/from window coordinates).
  sf = AvXCanvas::convertScale(sf);
  is_->setRasterScale (sf, sf);

  return (sf);
}
#endif

AvCLUImage::~AvCLUImage ()
{
  is_->unref();
  accessor_->unref();
}

float AvCLUImage::min()
{
//  return colorData->getClipMin();
	return accessor_->clipMin();
}

float AvCLUImage::max()
{
//  return colorData->getClipMax();
	return accessor_->clipMax();
}

AvGraphics2D * AvCLUImage::graphics2d()
{
  return is_;
}

// These are here to keep down the requirement for including AvCLUImageSetter.h

AvCLUImageSetter * AvCLUImage::imageSetter()
{
  return is_;
}

void AvCLUImage::getMargins(int &lm, int &rm, int &tm, int &bm)const
{
	if(is_ != NULL)
		is_->getMargins(lm, rm, tm, bm);
}

// Get the total size of the margins.
void AvCLUImage::getMargins(int &width, int &height)const
{
	if(is_ != NULL)
		is_->getMargins(width, height);
}

Boolean AvCLUImage::showAxis()const
{
	if(is_ != NULL)
		return is_->showAxis();
	else
		return FALSE;
}

void AvCLUImage::showAxis(const Boolean show, const Boolean resize)
{
	if(is_ != NULL)
		is_->showAxis(show, resize);
}
