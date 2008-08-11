//# Copyright (C) 1995-99 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvImageDataItem.h,v 19.0 2003/07/16 05:47:30 aips2adm Exp $
//
// $Log: AvImageDataItem.h,v $
// Revision 19.0  2003/07/16 05:47:30  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:10  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:39  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:46  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:18  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:36  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/08/25 19:49:41  hravlin
// Changed nSubviews() to return a long instead of an int.
//
// Revision 13.0  1999/08/10 18:40:24  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:22  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:45  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:45  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:29:06  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.1  1997/02/25 16:46:21  hr
// Changes to support multidimensional data sets. Particularly the addition
// of the View object.
// Added setIndexing() to deal with 1/0 indexing in the AvNDConfig window.
//
// Revision 8.0  1997/02/20 03:17:48  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.5  1997/02/10 21:17:13  hr
// Initial changes for multidimensional datasets (views_).
//
// Revision 7.4  1996/12/12 10:04:34  droberts
// Final update from monet archive.
//
// Revision 1.5  1996/09/25 21:47:06  hr
// dataset() now checks for NULL accessor.
//
// Revision 1.4  1996/09/20 20:19:35  pixton
// Prefixed all classes with Av
//
// Revision 1.3  1996/09/18  18:58:09  hr
// More integration with AvAccessor.
//
// Revision 1.2  1996/09/10 16:52:17  hr
// Initial changes for integration with AvAccessor & AvDataSet.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.3  1996/06/18  18:42:59  pixton
//  Copyright Update
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
//  Revision 1.1  1995/06/12  15:22:02  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

#ifndef AvImageDataItem_h
#define AvImageDataItem_h

/*

AvImageDataItem holds those items that relate to a particular data set,
including pointers to instances the initial AvAcessor, the data set (via
the accessor), the data panel (AvImageDataPanel) describing this data set,
and any views of this data set.  The views are of two types.  The array
'views_' points to instances of AvImageViewItem representing views along
the I, J, and K axes.  These views are always of the whole (possibly
subsampled) slice.  The user can interact with these overall views to
define and display subregions.  A list of subviews (also instances of
AvImageViewItem) so generated is pointed at by 'subviewHead_'.

AvImageDataItem defines how the an active data set is being viewed.

*/

#include <X11/Intrinsic.h>
#include "AvList.h"

#include "AvAccessor.h"
#include "AvDataSet.h"

class AvImageDataPanel;
class AvImageViewItem;
class AvNDConfig;
class AvConfigInfo;

class AvImageDataItem {
  
  friend class AvImageView;

public:

//  enum { I, J, K };

  // constructor needs a Widget to serve as the parent for the
  // ImageDataPanel and a file name for the data set
  AvImageDataItem (char * fName, Widget parent);

  // Builds the AvImageDataItem from its component parts.
  AvImageDataItem (AvDataSet* dataset, Widget parent);

#if 0
  // This doesn't seem to be used anywhere.
  // Build accessor slaved to passed accessor
  AvImageDataItem (AvAccessor* acc, Widget parent);
#endif

  ~AvImageDataItem ();

 // Adjust 1/0 index in any readouts. (The view_->config panels).
 void setIndexing(const int index);

  // returns data panel for this data set
  AvImageDataPanel * dataPanel () { return (dataPanel_); }

  // total number of subviews
  long nSubviews () { return subViews_.length(); }

  // total number of subviews along an axis
  int nSubviews (int axis);

  AvList<AvImageViewItem *> * subViews() const
    { return (AvList<AvImageViewItem *> *) &subViews_; }

  // routines to make and destroy whole-slice view along 'axis'
  // In the old days (3 dimensions), makeView took an axis (I, J, K) specifier
  // which was used to index into the views array. It still is used as an
  // index, but it can no longer be assumed to be an axis.
  void makeView (const int viewIndex);

  void deleteView (int axis);

  // routines to add and destroy subviews
  void addSubview (AvImageViewItem * item) { subViews_.add(item); }

  void deleteSubview (AvImageViewItem * item);

  // replace the dataset (via copying for now) with data and 
  // coordinates from newData.
  int replaceData (AvDataSet * newData);

  // regenerate views when, e.g., the transfer function has been modified
  void updateViews ();

  // call to inform views that data has changed
  void dataModified();

  AvDataSet *dataset()const { return accessor_ ? accessor_->dataset() : 0;}
  AvAccessor *accessor()const{return accessor_;}

  // compat.
  int ok () { return accessor() ? dataset()->attached() : 0; }

  // convenience routine for debugging.  Prints subview labels.
  void printViews ();

  const char * name() const { return dataset()->fileName(); }

  int id() const { return id_; }

  // Last Location Selection on this image
  AvIPosition lastLocation()const {return lastLocation_;}
  float lastValue() const { return lastValue_; }
  // Update last cursor position.
  void lastLocation(const AvIPosition &newLoc, const float value)
  {	lastLocation_ = newLoc;
	lastValue_ = value;
	lastLocationValid_ = 1;
  }

  int lastLocationValid() const { return lastLocationValid_; }

  // Last Region Selection on this image
  int * lastRegion() const { return (int *) lastRegion_; }
  void lastRegion(const AvIPosition &start, const AvIPosition &end);
  int lastRegionValid() const { return lastRegionValid_; }

  // Describes information needed to generate an individual view. For other
  // then the first 3, map, start and shape need to be set by the user either
  // from the GUI or a configuration file.
  class View {
    public:
	View(const int index, AvAccessor &a, AvConfigInfo *i);
	~View();
	AvAccessor *accessor()const{return accessor_;}
	AvConfigInfo *info()const{return info_;}
	AvString label()const;
	int index()const{return index_;}
	AvNDConfig	*config;
	void	updateConfig();
	Boolean haveMapping()const{return haveMapping_;}
	Boolean autoDisplay()const{return autoDisplay_;}
	void autoDisplay(const Boolean autoDisplay);
    public:
	AvImageViewItem	*ivi;
	Widget		tb;	// The button that turns a view on or off.
	AvImageDataPanel *idp;

    private:
	// Used to access the configuration database.
	const char *mapVar()const;
	const char *startVar()const;
	const char *shapeVar()const;
	const char *labelVar()const;
	const char *autoDisplayVar()const;
	const char *startingPlaneVar()const;
    private:
	int		index_;			// views_ index.
	// map, start and shape are used to hold size information that will
	// be used to create the view item.
	AvAccessor	*accessor_;
	AvString	label_;		// Label for push button.
	AvConfigInfo	*info_;
	Boolean		haveMapping_; // Has mapping been set?
	Boolean		autoDisplay_; // Show view when created?
  };

  View *getView(const int i)const {if((i>= 0) && (i<nViews_))
				    return views_[i];
			 	    else return NULL;}
  int getNumViews()const{return nViews_;}
protected:

private:

  static int nextID_;

  int makeID() { int id = nextID_; nextID_++; return id; }
  int id_;

  // The base accessor for the data set.
  AvAccessor *accessor_;

  // the panel that describes this data set
  AvImageDataPanel * dataPanel_;

  // the widget within the "Session" list that names this data set
  Widget dataListEntry_;
  // the views along each of the axes.
  // This used to be just an array of 3 AvImageViewItems, but things are
  // more complicated now.
  View	**views_;	// Array of Views.
  int nViews_;

  // the complete list of subviews
  AvList<AvImageViewItem *> subViews_;

  // The last picked region (x1,y1,z1,x2,y2,z2)
  int lastRegion_[6];
  int lastRegionValid_;

  // The last picked location and its value (x1,y1,z2)
  AvIPosition lastLocation_;
  int lastLocationValid_;
  float lastValue_;

private:

  void init(AvAccessor *, Widget parent);
  void init0();
  void initViewsArray();	// Create and initialize the views_ array.

  // Callback to receive accessor notices.
  static void accessorCB_(XtPointer me, AvConductor *c, XtPointer uData);
  // Callback to actually handle accessor notices.
  void accessorCB(AvConductor *c, XtPointer uData);
};

#endif
