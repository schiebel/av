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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvImageViewItem.h,v 19.0 2003/07/16 05:47:24 aips2adm Exp $
//
// $Log: AvImageViewItem.h,v $
// Revision 19.0  2003/07/16 05:47:24  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:04  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:34  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:41  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:56  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:29  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:16  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:08  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:27  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:24  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:28:34  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.1  1997/02/25 17:11:05  hr
// Added new constructor. Renamed myAxis_ to idKey_. Added setIndexing().
//
// Revision 8.0  1997/02/20 03:17:27  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1996/12/12 06:30:37  droberts
// Final update from monet archive.
//
// Revision 1.8  1996/11/11 17:58:45  hr
// Moved equalScales() to AvImageWindow.cc
//
// Revision 1.7  1996/11/11 15:54:30  hr
// Added equalScales().
//
// Revision 1.6  1996/10/01 21:59:27  hr
// Removed validSlice().
//
// Revision 1.5  1996/09/27 16:45:09  hr
// Added myAxis_ variable to keep track of the view axis/index.
//
// Revision 1.4  1996/09/25 21:49:04  hr
// Renamed functions converting between indexes and world coordinates
// from IJK to XYZ.
//
// Revision 1.3  1996/09/18 19:08:52  hr
// More integration with AvAccessor.
//
// Revision 1.2  1996/09/10 16:53:00  hr
// Initial changes for integration with AvAccessor & AvDataSet.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.6  1996/06/18  18:43:02  pixton
//  Copyright Update
//
//  Revision 1.5  1996/02/23  17:36:34  hr
//  Removed getRasterOffsets.
//
//  Revision 1.4  1996/01/31  18:01:03  hr
//  Added getRasterOffsets().
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
//  Revision 1.2  1995/06/12  21:29:56  hr
//  Removed viewPanel_.
//
//  Revision 1.1  1995/06/12  15:23:34  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

#ifndef AvImageViewItem_h
#define AvImageViewItem_h

/*

AvImageViewItem holds information about a single view of some data set. An
Accessor to hold mapping and other information about the view is created.
Also keeps pointers to the AvCLUImage that shows this data and the window
in which it is displayed.  Has methods for updating the image, and for
matching up the size of the window with a particular scaling factor applied
to the raster.
*/

#include <X11/Intrinsic.h>

#include "AvImageDataItem.h"
#include "AvPosition.h"
#include "AvAccessor.h"

class AvImageWindow;
class AvCLUImage;

class AvImageViewItem {

  friend class AvImageViewPanel;
  friend class AvImageDataItem;

public:

#if 0
  // make a view of 'dItem', using 'x' and 'y' as the X and Y
  // dimensions of the raster view.   Slices are along
  // dimension 'view'.  For 2D data, 'view' axis defaults to 2.
  AvImageViewItem (AvImageDataItem & dItem, int x, int y, int view = 2);
#endif
  // Make a view with the given parameters. start and length are DS
  // (unmapped) values. All should be of length accessor->rank().
  // idKey is a magic value passed to AvImageViewItem by AvImageDataItem
  // at creation time so can AvImageViewItem ask AvImageViewItem to delete it.
  AvImageViewItem(AvImageDataItem & dItem, const int idKey,
		  const AvIPosition &map,
		  const AvIPosition &start, const AvIPosition &shape);

  AvImageViewItem(AvImageDataItem & dItem, const int idKey,
		  AvAccessor *accessor);

  // Called to make a sub-view.
  AvImageViewItem (const AvImageViewItem & item);

  void init (const int axis=-1);

  AvAccessor *accessor()const { return accessor_; }

  // returns the dataview item.
  AvImageDataItem * dataItem()const{return &dItem_;}

  // returns the number of slices along 'axis'
  int nSlice (const int axis)const
	{ return accessor_->getAxisLength(axis);}

  // Returns the number of slices along the view axis.
  int numSlices()const {return nSlice(viewAxis());}

  int sliceIndex ()const { return accessor()->getPlane();}
  // X axis is always 0, Y always 1, etc.. The accessor handles the mapping.
  int xAxis()const { return AvAccessor::XAXIS;}
  int yAxis()const { return AvAccessor::YAXIS;}
  // zAxis and viewAxis are the same.
  int zAxis()const { return AvAccessor::ZAXIS;}
  int viewAxis()const { return AvAccessor::ZAXIS;}

  // is this a subview?
  int subview ();

  // the number of views along this axis
  int nSubview ();

  char * label () { return (label_); }

  // regenerates the view, including remapping the raster
  void updateView ();

  // along viewAxis_, show a particular slice
  void showSlice (int which);

  // match size of raster to the current size of the window
  void fitRasterToWindow (const Boolean redraw=FALSE);

  // raster's are scaled by integer amounts.  This adjusts the
  // size of the window to fit the specific size of the raster
  void fitWindowToRaster (const Boolean readraw=FALSE);

  void deleteView ();

  // Is the position a valid world position for the current subrange?
  Boolean validWorld(const double x, const double y);
  Boolean validWorld(const double x, const double y, const double z);
  // Convert between Position and XYZ (subrange) coordinates.
  Boolean XYZToWorld(const int i, const int j, const int k, AvPosition &pos);
  Boolean WorldToXYZ(const AvPosition &pos, int &i, int &j, int &k);
  Boolean WorldToXYZ(const double x, const double y, const double z,
                                                int &i, int &j, int &k);

  AvImageWindow *getImageWindow() const { return window_;}
  AvCLUImage *getCLUImage()       const { return raster_;}
  // Adjust 1/0 index in any readouts.
  void setIndexing(const int index);
  ~AvImageViewItem ();

  static void print(AvImageViewItem *);

protected:

private:

  // the data item for this data set
  AvImageDataItem & dItem_;

  AvAccessor	*accessor_;
  // the window in which the view is shown
  AvImageWindow * window_;

  // the raster image itself
  AvCLUImage * raster_;

  // Which "aipsview" axis (0, 1, 2) this is. Used to delete.
  // This is used to index into the AvImageDataItem::views_ array.
  int  idKey_;
  char label_[128];

  // the parent view of this view, NULL if this is a whole-slice view
  const AvImageViewItem * vItem_;

  // convenience routine for debugging
  void showSizeInfo ();
  // Callback to receive accessor notices.
  static void accessorCB_(XtPointer me, AvConductor *c, XtPointer uData);
  // Callback to actually handle accessor notices.
  void accessorCB(AvConductor *c, XtPointer uData);
};
#endif
