//# Copyright (C) 1995-2003 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvImageDataItem.cc,v 19.1 2003/09/02 21:12:11 hravlin Exp $
//
// $Log: AvImageDataItem.cc,v $
// Revision 19.1  2003/09/02 21:12:11  hravlin
// Changed makeDataSet() to AvMakeDataSet::makeDataSet().
//
// Revision 19.0  2003/07/16 05:47:34  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:13  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:42  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:49  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:30  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:39  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.2  1999/09/28 16:23:33  hravlin
// Needed to include strings.h.
//
// Revision 13.1  1999/09/27 15:48:54  hravlin
// Moved setBusy() to include makeDataSet(). Removed the now uneeded checks
// to see if aipsview was already busy.
//
// Revision 13.0  1999/08/10 18:40:27  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:28  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:55  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.1  1998/07/30 20:45:44  hr
// In AvImageDataItem::View::View(), made 0 the default starting slice for
// the Z axis when axis name is "STOKES".
//
// Revision 10.0  1998/07/20 17:54:56  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.3  1998/07/15 20:23:41  hr
// Revisited View::View() to make consistent the remapping of the Z axis.
//
// Revision 9.2  1998/07/13 20:47:24  hr
// In AvImageDataItem::View::View() aipsview tries to make the 3rd button
// have an axis whose length is > 1.
//
// Revision 9.1  1998/06/19 20:26:01  hr
// In destructor, force data set and accessor to go away and print warning
// message if something else is still referencing them.
//
// Revision 9.0  1997/08/25 21:29:22  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.4  1997/06/12 21:32:08  hr
// View::View() was only initializing the starting plane if it was being
// set from the config file.
//
// Revision 8.3  1997/06/09 20:07:29  hr
// In lastRegion(), was checking for nelements >= 2.
//
// Revision 8.2  1997/03/24 19:49:55  hr
// id_ stopped being initialized during the conversion to handle higher
// order data sets.
//
// Revision 8.1  1997/02/25 16:43:18  hr
// Many changes to support multidimensional data sets.
// Reworked constructors to us common init().
// Added save/restore of clipMin/Max.
// Replaced views_[3] array with array of new object Views used to hold
// mapping & setup information to construct a view.
//
// Revision 8.0  1997/02/20 03:17:59  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.6  1997/02/10 21:13:35  hr
// Turned off "Closing ..." messages.
// Initial changes for multidimensional datasets (views_).
//
// Revision 7.5  1996/12/12 09:33:50  droberts
// Changed search by accessor to match when datasets match.  Upgrades to support dynamic datasets.
//
// Revision 1.7  1996/11/06 16:59:38  hr
// Sun's compiler for 4.1.3 was complaining about "end" being redefined in
// lastRegion().
//
// Revision 1.6  1996/09/27  16:41:50  hr
// Moved creation of the data set to the file AvMakeDataSet.
//
// Revision 1.5  1996/09/25 21:47:57  hr
// Checks added to keep from crashing if a dataset could not be attached to.
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
//  Revision 1.6  1996/06/18  18:46:10  pixton
//  Copyright Update
//
//  Revision 1.5  1996/04/06  18:12:05  hr
//  Added calls to set/resetBusy().
//
//  Revision 1.4  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/08/04  19:52:10  hr
//  Initialize more variables to NULL, check for data OK in case of a bad
//  data file and it is necessary to back out.
//
//  Revision 1.2  1995/08/03  21:03:30  hr
//  Added support to handle new HDF code.
//
//  Revision 1.1  1995/06/12  15:20:33  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

#include <Xm/ToggleB.h>		// For XmToggleButtonSetState()
#include <AvImageDataItem.h>
#include <AvMakeDataSet.h>
#include <AvConfigInfo.h>

/*
	There is one base accessor (which points to the data set) for a
file. The main purpose of the base accessor is to hold the data clip values
and to propagate change information to interested parties. Each "view
window" will have its own accessor describing its mapping, region, etc.
 */

#include <strings.h>		// strncasecmp
#include <AvCLUImage.h>
#include <AvImageDataPanel.h>
#include <AvImageViewItem.h>
#include <AvXColorApp.h>	// busy()
#include <AvList.h>
#include <AvNDConfig.h>

static const char *SLICEINFO = "sliceControl";
//
//  Counter to get next ID's.
//
int AvImageDataItem::nextID_ = 1;

AvImageDataItem::AvImageDataItem (char * fName, Widget parent)
{
  AvXColorApp::setBusy();	// Let user know something is happening.

  // Try to attach a data set to the file.
  //  AvString err;
  AvDataSet *dataset = AvMakeDataSet::makeDataSet(fName);
  if (!dataset) 
    {
      // err.prepend(" as any of: ");
      // err.prepend(fName);
      // err.prepend("Could not open ");
      // cerr << err.chars() << endl;
      init0();	// Set everything to 0.
      return;
    }

  AvAccessor *accessor = new AvAccessor( *dataset);
  init(accessor, parent);

  AvXColorApp::resetBusy();
}

AvImageDataItem::AvImageDataItem (AvDataSet * dataset, Widget parent)
{
  AvXColorApp::setBusy();	// Let user know something is happening.
  AvAccessor *accessor = new AvAccessor( *dataset);
  init(accessor, parent);

  AvXColorApp::resetBusy();
}

#if 0
// Not used??
AvImageDataItem::AvImageDataItem (AvAccessor *acc, Widget parent)
{
  AvAccessor *accessor = new AvAccessor( *acc);
  // Connect to accessor.
  accessor->addCallback(&AvImageDataItem::accessorCB_,
			 AvAccessor::DEFAULTMASK | AvDataSet::DEFAULTMASK,
			 this,
			 NULL);
  init(accessor, parent);
}
#endif

void AvImageDataItem::init(AvAccessor *a, Widget parent)
{
  init0();
  accessor_ = a;
  accessor_->ref();
  lastLocation_.resize(accessor()->rank());
  lastLocation_ = accessor()->start();

  // The initial AvAccessor is locked as an aid in debugging. (To
  // keep it and the dataset from mysteriously going away).
  // The only way it should ever get deleted is when it's time to close the
  // file.
  accessor_->lockResource();

  // Make a new data object.

  // Generate information needed to open in image 'view'.
  initViewsArray();  // This has to be setup before idp.

  // This is as good a place as any to see if the user wants to change
  // the clip min/max.
  {double min, max;
   AvConfigInfo *info = views_[0]->info();
   Boolean needSet=FALSE;
	if(info->getDouble("clipMin", min, a->clipMin()))
		needSet = TRUE;
	if(info->getDouble("clipMax", max, a->clipMax()))
		needSet = TRUE;
	if(needSet)
		a->setClipMinMax(min, max);
  }

  // Construct the panel that shows info about Image, provides
  // for user control of data range and slice.
  dataPanel_ = new AvImageDataPanel (parent, TRUE, *this);

  // Display any views that are flagged to be shown automatically.
  for(int index = 0; index < nViews_; index++)
  {  AvImageDataItem::View *view = getView(index);
	if((view != NULL) && (view->autoDisplay()))
		makeView(index);
  }
}

// This would be part of init, except that we want to make sure some things
// get initialized even if the constructor fails for some reason.
void AvImageDataItem::init0()
{
  memset(lastRegion_, 0, 6*sizeof(int));
  lastRegionValid_ = 0;
  lastLocationValid_ = 0;
  lastValue_ = 0.0;
  id_ = makeID();

  //
  dataListEntry_  = NULL;
  views_ = NULL;
  nViews_ = 0;
  dataPanel_ = NULL;
  accessor_ = NULL;
  views_ = NULL;
}

// Replace the data with an update
// 
// returns -1 if imageID doesn't match any open id
//
// IDEALLY, we would like to swap in a new ds pointer for the old
// ds and call appropriate updates.  Unfortunately, a host of accessors
// exist that could never be reached.  We would need to keep a list of
// active accessors in AvImageView and swap the ds pointer on all accessors
// that reference the dataset.  This means accessor creation and deletion
// would be done by AvImageView functions, NOT new and unref.
//
// Until then, we're gonna just copy the data and coordinates if the
// dataset allows the putSlice() operation.
//
int AvImageDataItem::replaceData (AvDataSet * newData)
{
  AvDataSet * currentDS = dataset();

  if (currentDS->putData(*newData))
    {
      dataPanel_->update();
      return id_;
    }
  else
    return -1;
}

// Create a 'view' of an image. Index is an index into the views_ array.
void AvImageDataItem::makeView (const int index)
{
  AvXColorApp::setBusy();

  AvImageDataItem::View *view = getView(index);
  if(view != NULL)
	view->ivi = new AvImageViewItem(*this, index, view->accessor());
  AvXColorApp::resetBusy();
}

// Delete an image window.
void AvImageDataItem::deleteView (int index)
{
//cerr << "AvImageDataItem::deleteView " << index << endl;

  View *view = getView(index);
  if(view != NULL)
  { Widget tb = view->tb;
	if(tb != NULL)
		XmToggleButtonSetState (tb, FALSE, FALSE);
	delete view->ivi;
	view->ivi = NULL;
  }
}

void AvImageDataItem::dataModified()
{
  AvXColorApp::setBusy();

  for (int i=0; i<nViews_; i++)
    if (views_[i]->ivi) views_[i]->ivi->getCLUImage()->dataModified();
  
  AvListIter<AvImageViewItem *> it(&subViews_);
  while (!it.done()) it.get()->getCLUImage()->dataModified();
  
  // Adjust the clip min in the datapanel
  dataPanel()->dataModified();

  AvXColorApp::resetBusy();  
}

void AvImageDataItem::updateViews ()
{
  AvXColorApp::setBusy();
  for (int i=0; i<nViews_; i++)
    if (views_[i]->ivi) views_[i]->ivi->updateView ();

  AvListIter<AvImageViewItem *> it(&subViews_);
  while (!it.done()) it.get()->updateView();
  AvXColorApp::resetBusy();
}

// static
void AvImageViewItem::print(AvImageViewItem * idi)
{ cerr << idi->label() << ' ' << idi->viewAxis() << ' ' << endl; }

void AvImageDataItem::printViews ()
{ subViews_.mapcar(AvImageViewItem::print); }

AvImageDataItem::~AvImageDataItem ()
{
  // Disconnect from accessor
  if(accessor_ != NULL)
	accessor_->removeCallback(&AvImageDataItem::accessorCB_,
			 AvAccessor::DEFAULTMASK | AvDataSet::DEFAULTMASK,
			 this,
			 NULL);

  // remove this entry from the data list
  if(dataListEntry_ != NULL)
    XtUnmanageChild (dataListEntry_);

  // delete the panel that shows the data set
  delete dataPanel_;

#if 0
  if(dataset() != NULL)
	cerr << "closing all views " << endl;
#endif
  // delete all views...
  for (int i=0; i<nViews_; i++)
    if (views_[i])
      delete views_[i];

  delete [] views_;
  views_ = NULL;

  // ...and any subviews
  AvListIter<AvImageViewItem *> it(&subViews_);
  while (!it.done()) delete it.get();
  
  // delete the data set itself
  // (dataset and accessor will be NULL if the attach failed).
#if 0
  if(dataset() != NULL)
	cerr << "closing data set " << dataset()->fileName() << endl;
#endif
  if(accessor_ != NULL)
  { // We are deleting everything to do with this data set. At this point
    // there should only be 1 connection to the accessor. However, some
    // things may not have been cleaned up correctly. (Eg. bugs).
	if(accessor_->getReferenceCount() > 1)
		accessor_->disconnect();

	while(accessor_->getReferenceCount() > 1) // If really messed up.
		accessor_->unref();
	// Pointer to dataset since accessor is going away.
	AvDataSet *ds = dataset();
	int dscnt = ds->getReferenceCount();	// Should be 1.
	accessor_->unlockResource();
	accessor_->unref();
	accessor_ = NULL;
	// If dscnt is > 1, something is still referencing it. Try to get
	// it to go away. If dscnt == 1, ds is no longer valid after the
	// accessor deleted itself.
	if( dscnt > 1)
	{	cerr	<< "::~AvImageDataItem: DS is over ref'd by "
			<< dscnt -1 << endl;
		ds->ref();	// Make sure it doesn't go away while we're
				// using it.
		ds->disconnect();
		while(ds->getReferenceCount() > 1)
			ds->unref();
		ds->unref();	// Causes ds to go away.
	}
  }
}

void AvImageDataItem::lastRegion(const AvIPosition &start,
                               const AvIPosition &End)
{
      lastRegion_[0] = start(0); lastRegion_[1] = start(1);
      lastRegion_[3] = End(0); lastRegion_[4] = End(1);
      if(start.nelements() > 2)
      {	lastRegion_[2] = start(2);
	lastRegion_[5] = End(2);
      }
      else
      {	lastRegion_[2] = 0;
	lastRegion_[5] = 0;
      }

      lastRegionValid_ = 1;
}

 // Callback to receive accessor notices.
void AvImageDataItem::accessorCB_(XtPointer dis, AvConductor *c, XtPointer uD)
{ AvImageDataItem *me = (AvImageDataItem *) dis;
	me->accessorCB(c, uD);
}

 // Callback to actually handle accessor notices.
void AvImageDataItem::accessorCB(AvConductor *, XtPointer )

{
	// Later.
	cerr << "AvImageDataItem:accessorCB called.\n";
}


// Changes the 1/0 relative readouts.
void AvImageDataItem::setIndexing(const int index)
{
	for(int i=0; i< nViews_; i++)
		if(views_[i]->config != NULL)
			views_[i]->config->setOffset(index);
}

// Create and initialize the views_ array.
// Requires accessor_ be valid.
void AvImageDataItem::initViewsArray()
{  int rank = accessor_->rank(); 
AvAccessor *a = accessor();
AvConfigInfo *info = new AvConfigInfo(a->dataset()->fileName(), SLICEINFO);

	info->ref();

	if(rank < 3)		// Only one image.
		nViews_ = 1;
	else
	{int extra=0;
		if(info->getInt("extraToggles", extra, rank-3))
			// Use exactly as many extra as user requests.
			nViews_ = rank + extra;
		else
		if( extra > 0)
		{int e = rank+extra;
			// If user didn't request any, round up to a
			// multiple of 4.
			nViews_ = ((e+3)/4)*4;
			// (Makes the buttons look better).
		}
		else	// Don't round if there are only three buttons.
			nViews_ = rank;
	}

	views_ = new View *[nViews_];
	for(int i=0; i< nViews_; i++)
		views_[i] = new View(i, *a, info);
	info->unref();
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
///			View
////////////////////////////////////////////////////////////////

AvImageDataItem::View::View(const int index, AvAccessor &a, AvConfigInfo *i)
{
	index_ = index;
	info_ = i;
	info_->ref();
	ivi = NULL;
	idp = NULL;
	config = NULL;
	tb = NULL;
	haveMapping_ = FALSE;
	autoDisplay_ = FALSE;
	int startingPlane = 0;
	accessor_ = new AvAccessor(a);
	accessor_->ref();
	int rank = accessor_->rank();
	int axis3 = 2;			// Default third axis is the 3rd axis.
	// If the third axis has length one, look for another that's longer,
	// make it the third axis and map the real third axis to its
	// position.
	if((accessor_->getAxisLength(axis3) <= 1)  && (rank > 3))
		for(int n=3; n<rank; n++)
			if(accessor_->getAxisLength(n)>1)
			{	axis3 = n;	// New third axis.
				accessor_->setAxisMapping(axis3, 2);
				// Initialze default which may get changed.
				accessor_->setAxisMapping(2, axis3);
				break;
			}


	// Default label to use after mapping window closes.
	if(index < rank)
		label_ = accessor_->axisName(index);
	else
		label_ = "Mapped";

	// The first 3 indices are initialized to act the same as 'old'
	// aipsview except that aipsview now tries to make the third axis
	// have a length > 1.
	if((rank > 2)&&(index < 3))
	{
		if(index == 0)
		{	// First button uses 1 as X axis, 3rd axis as Y &
			// 0 as Z axis.
			haveMapping_ = TRUE;
			AvIPosition map = accessor_->map();
			map(0) = 1;		// 'Y' axis used as X.
			map(1) = axis3;		// 3rd axis along y.
			map(2) = 0;		// 'X' axis along Z.
			accessor_->map(map);
		}
		else
		if(index == 1)	// 0 as X, 2 as Y & 1 as Z.
		{	haveMapping_ = TRUE;
			accessor_->setZaxisMapping(1);
			accessor_->setYaxisMapping(axis3);
		}
		else
		if(index == 2)	// Normal: X vs Y vs Z.
		{// The 3rd button (typically VELO) uses default mapping
			haveMapping_ = TRUE;
			autoDisplay_ = TRUE;
			accessor_->setAxisMapping(2, axis3);
			// Need to get new label if third axis was changed.
			if(axis3 != 2)	// After setMapping, index is correct.
				label_ = accessor_->axisName(index);
		}
		// Default starting plane.
		if(strncasecmp("STOKES", accessor_->axisName(2), 6) == 0)
			startingPlane = 0;	// Hack for STOKES axis.
		else	// Default is half way through the slices.
			startingPlane = accessor_->getAxisLength(2)/2;
	}
	else
	if(rank <= 2)		// Only one choice possible.
	{	label_ = "Show Image";
		haveMapping_ = TRUE;
		autoDisplay_ = TRUE;
	}

	////// Now that the defaults are set, check to see if the user has
	// other ideas. Start and shape are in raw DS coords (ie. not mapped).

	// Should the image be displayed immediately after a mapping is
	// available?
	int bval;
	info_->getBoolean(autoDisplayVar(), bval, autoDisplay_);
	autoDisplay_ = bval;

	// If ANY of the configuration variables are set, assume a valid
	// mapping exists. (User could be using default mapping).
	// User specified label for the toggle button.
	if(info_->getString(labelVar(), label_, label_))
		haveMapping_ = TRUE;

	// How to map the axes.
	AvIPosition ip(accessor_->map());
	if(info_->getIntList(mapVar(), ip, ip))
	{	haveMapping_ = TRUE;
		accessor_->map(ip);
	}

	// There is a problem in that aipsview doesn't really know how
	// to handle subranging the Z axis. However, we'll let users
	// find out the hard way if they try. (Actually, it more or less
	// works, but AvCLUImageSetter tries to build raster from DS slice #0).
	// Get unmapped start position.
	ip = accessor_->start(FALSE);
	if(info_->getIntList(startVar(), ip, ip))
		accessor_->setStart(ip, FALSE);

	ip = accessor_->shape(FALSE);
	if(info_->getIntList(shapeVar(), ip, ip))
	{	accessor_->shape(ip, FALSE);
		haveMapping_ = TRUE;
	}

	// What is the first plane to be displayed?
	if(info_->getInt(startingPlaneVar(), startingPlane, startingPlane))
	{	accessor_->setPlane(startingPlane, FALSE);
		haveMapping_ = TRUE;
	}
	else	// Make sure it gets set.
		accessor_->setPlane(startingPlane, FALSE);
}

AvImageDataItem::View::~View()
{
//	cout << "Deleting view for " << label_ << endl;
	if(ivi != NULL)
	{	delete ivi;
		ivi = NULL;
	}
	if(config != NULL)
		delete config;
	if(info_ != NULL)
		info_->unref();
	accessor_->unref();
}

AvString AvImageDataItem::View::label()const
{
	return label_;
}

// Update values from config object.
// Called when the GUI window is popped down. Copies data from the window
// to wherever it should go and updates the config database.
void	AvImageDataItem::View::updateConfig()
{
	if(config == NULL)
		return;
	AvString s = config->getLabel();
	if(s != label_)
	{	info_->setString(labelVar(), s);
		label_ = s;
	}

	// Turn off any callbacks until all done.
	accessor_->disableNotify();

	AvIPosition ip = config->getMap();
	if(ip != accessor_->map())
	{	accessor_->map(ip);
		info_->setIntList(mapVar(), ip);
	}

	ip = config->getStart(FALSE);
	if(ip != accessor_->start(FALSE))
	{	accessor_->start(ip, FALSE);
		info_->setIntList(startVar(), ip);
	}

	ip = config->getShape();
	if(ip != accessor_->shape(FALSE))
	{	accessor_->shape(ip, FALSE);
		info_->setIntList(shapeVar(), ip);
	}

	Boolean ad = config->autoDisplay();
	if(ad != autoDisplay_)
		autoDisplay(ad);

	int sp = config->startingPlane();
	if(sp != accessor_->getPlane(FALSE))
	{	accessor_->setPlane(sp, FALSE);
		info_->setInt(startingPlaneVar(), sp);
	}

	haveMapping_ = TRUE;
	accessor_->enableNotify();
}

// Set the autoDisplay variable.
void AvImageDataItem::View::autoDisplay(const Boolean autoDisplay)
{
	autoDisplay_ = autoDisplay;
	info_->setBoolean(autoDisplayVar(), autoDisplay);
}

// The config variables share this.
static char varbuf[32];

const char *AvImageDataItem::View::mapVar()const
{
	sprintf(varbuf, "map%02d", index_);
	return varbuf;
}

const char *AvImageDataItem::View::startVar()const
{
	sprintf(varbuf, "start%02d", index_);
	return varbuf;
}

const char *AvImageDataItem::View::shapeVar()const
{
	sprintf(varbuf, "shape%02d", index_);
	return varbuf;
}

const char *AvImageDataItem::View::labelVar()const
{
	sprintf(varbuf, "label%02d", index_);
	return varbuf;
}

const char *AvImageDataItem::View::autoDisplayVar()const
{
	sprintf(varbuf, "autoDisplay%02d", index_);
	return varbuf;
}

const char *AvImageDataItem::View::startingPlaneVar()const
{
	sprintf(varbuf, "startingPlane%02d", index_);
	return varbuf;
}
