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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvImageView.h,v 19.0 2003/07/16 05:47:34 aips2adm Exp $
//
// $Log: AvImageView.h,v $
// Revision 19.0  2003/07/16 05:47:34  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:13  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/14 17:32:23  hravlin
// Changed fallbacks arg to be const char *[].
//
// Revision 17.0  2001/11/12 19:42:42  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:49  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:32  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:49:18  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:08:40  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.2  1999/09/15 21:33:54  hravlin
// Added redisplayImages() & colorMapChangedCB().
//
// Revision 13.1  1999/08/25 19:48:49  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:40:28  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:29  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:57  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:58  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1997/12/17 21:50:58  hr
// Changes to remove warnings from MIPSpro.7x compiler.
//
// Revision 9.0  1997/08/25 21:29:25  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:18:01  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.7  1997/02/06 18:15:05  hr
// Changes to support 1 relative readouts.
//
// Revision 7.6  1997/01/24 20:17:59  hr
// Added menu options to write aipsviewrc files.
//
// Revision 7.5  1997/01/09 18:21:28  hr
// Changed constructor arg list to include parameters to allow retrieving
// of command line X args.
//
// Revision 7.4  1996/12/12 10:00:20  droberts
// Search by accessor changed to match when datasets match.  Rewrote displayFile/replaceData routines.
//
// Revision 1.2  1996/09/10 16:56:46  hr
// Initial changes for integration with AvAccessor & AvDataSet.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.13  1996/06/18  18:43:01  pixton
//  Copyright Update
//
//  Revision 1.12  1996/03/20  17:37:06  hr
//  Changed to being derived from AvXColorApp rather than AvApp.
//
//  Revision 1.11  1995/11/17  21:03:47  pixton
//  added "replaceFitsFile(id, filename)" event for SC '95 support.
//
//  Revision 1.10  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.10  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.10  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.9  1995/06/12  15:22:02  hr
//  Broke AvImageView.h 1.8 into AvImageView, AvImageViewItem and
//  AvImageDataItem.
//
//  Revision 1.8  1995/06/09  19:44:36  hr
//  Added buildInside().
//
//  Revision 1.7  1995/05/05  14:49:25  hr
//  Edits to WorldToIJK routines.
//
//  Revision 1.6  1995/05/02  20:39:17  pixton
//  Rewritten to use AvList, a templated list class.  This represents
//  the first use of templates in our code.
//  Certain unused functions from the previous representation were
//  removed.  Used functions were rewritten.
//  The list of AvImageDataItems now accessible globally as
//  AvImageView::instance()->sessionList().  See AvImageView.cc for
//  examples of iterating the list.
//  added nthSession(index) which returns nth item in the session list
//  or null.
//  added flags for compiling in GLISH stuff.
//
//
//  Revision 1.5  1995/05/02  19:09:43  hr
//  Added functions to AvImageViewItem to convert between world and 
//  IJK coordinates.
//
//  Revision 1.4  1995/04/25  16:47:27  hr
//  Added redraw flag to fitRasterToWindow and fitWindowToRaster.
//
//  Revision 1.3  1995/04/19  14:25:57  hr
//  Changes to move animation controls to inside imageView window.
//  Added AvAnimPanel. Removed AvAnimCtlr.
//
//  Revision 1.2  1995/04/10  20:54:02  hr
//  add a number of functions to return pointers to data items.
//  added AvImageView::getImageViewList and AvImageView::instance
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#ifndef _IMAGE_VIEW_H
#define _IMAGE_VIEW_H

#include <Xm/Xm.h>

#include "AvList.h"
//#include <AvGriddedData.h>

#ifdef GLISH
#include "AvGlishRequestList.h"
#endif

class AvCLUImage;
class AvColormapEditor;
class AvFitsFile;
class AvGrid;
class AvImageDataPanel;
class AvImageViewItem;
class AvImageViewPanel;
class AvImageWindow;
class AvAnimPanel;
class AvPosition;

#if 0
#define SV_IMAGE_FILE 2000
#define SV_HELP       2001
#define SET_CURRENT_DATA 2002
#define SV_IV_OPTION  3000
#endif

#if 1
#include "AvImageViewItem.h"
#include "AvImageDataItem.h"
#else
//AvImageView will compile with just these, but other files won't unless
//modified to include the above includes.
class AvImageViewItem;
class AvImageDataItem;
#endif


#include "AvXColorApp.h"

/*
  AvImageView is the primary class for the 'imageView' application.
*/

class AvImageView : public AvXColorApp {

  friend class AvImageDataPanel;
  friend class AvImageWindow;

public:

  AvImageView
    (const char * appClass, int & argc, String * argv,
     const char * fallbacks[] = NULL);

  AvImageView (const char * appClass,
	       XrmOptionDescRec * opt, Cardinal noptions,
	       int & argc, String * argv,
	       const char * fallbacks[],
	       ArgList args, Cardinal nargs,
		// Arguments to XtGetApplicationResources
	       XtPointer varStruct=NULL,// User defined struct to hold vars.
	       XtResource *resources=NULL, const int nResources=0);

  static AvImageView * instance() { return instance_; }


  // use this as the data file at startup time
  void setImage (char * fName) { displayData (fName); }

  // show this data panel
  static void setCurrentDataPanel (AvImageDataPanel * p);
  // Above but any UserComp
  static void setCurrentPanel (AvUserComp *);

  // x Return the session list
  AvList<AvImageDataItem *> * sessionList() const
    { return (AvList<AvImageDataItem *> *) &sessionList_; }

  // x Return pointer to the nth session or NULL on bad index
  AvImageDataItem * nthSession(int n) const;

  // x How many sessions?
  long nSessions() const { return sessionList_.length(); }
  long getNumDataItems() const { return nSessions(); }

  // x synchronize the data item and hook in windows, etc.
  void registerDataItem (AvImageDataItem * idi);

  // x Convenience func.
  int addSession(AvImageDataItem * item);
  int addSession(AvImageDataItem& item);

  // display this data set.  If data set is already open, then re-read
  // xx and update views.  If new, read and make initial view.  Return id.
  int displayData (const char * fName);
//  int displayData (AvGriddedData * data);
  int displayData (AvDataSet* dataset);

  // x replace the data - return id or -1 if id passed not found.
  // Because the file could be memory-mapped, the gridded data
  // must be deleted and replaced.
  int replaceData (const char * fName, int id);
//  int replaceData (AvGriddedData * data, int id);
  int replaceData (AvAccessor *acc, int id);
  
  // xxx close the session
  int closeSession (AvImageDataItem * item);
  int closeSession (int id);
  int closeSession (char * name);

  // search for idi pointer by fileName, id, or data pointer. 
  AvImageDataItem * search (char * fName);
  AvImageDataItem * search (int id);
//  AvImageDataItem * search (AvGriddedData * dptr);
  AvImageDataItem * search (AvDataSet *avds);

  // returns non-zero if glish is active
  int glishActive() const { return glishActive_; }
  
  // This really belongs in animator code, no?
  enum ItemSelection { NONE=0, INCLUDED=1, NOTINCLUDED=2, ALL=3};

  // Returns an array of pointers to all ImageViewItems.
  // Used by animation and region to determine which windows need updating.
  // The return array of pointers is dynamically allocated with each call
  // and should be deleted by the caller.
  //
  int checkViewItem(AvImageViewItem * vi, const ItemSelection which);
  AvImageViewItem **getImageViewList(int &nitems, const ItemSelection which);

  // Should the data panel be built inside or out?
  void buildInside(const Boolean b) {buildInside_ = b;}
  static Boolean buildInside();

  // Do index readouts start at 0 or 1?
  // If 0, the value is zero, else 1.
  static int oneIndexing(){return oneIndexing_;}

protected:

  virtual void processMenu (char *, int, Widget);

private:

  // animation controls
  AvAnimPanel * animPanel_; 

  static AvImageView * instance_;   // last instance

  // Open (loaded) list of sessions.  
  AvList<AvImageDataItem *> sessionList_;

  // Matching functions for searches.
  static int matchIdFunc(AvImageDataItem * test, void * id);
  static int matchNameFunc(AvImageDataItem * test, void * name);
  static int matchDPtrFunc(AvImageDataItem * test, void * dptr);
  
  // removes this 'item' from AvImageView's list of data sets
  void remove (AvImageDataItem * item) { sessionList_.remove(item); }

  // the current data set.  This set's data panel is shown in the 
  // main window.  A "Close" menu selection applies to this data set
  AvImageDataItem * currentDataItem_;

  // the data panel that is currently showing in the main window
  static AvImageDataPanel * currentDataPanel_;
  static AvUserComp * currentPanel_;

  // path name for location of data
  char dataDirectory_[512];

  // the "Session" choice on the main menu bar holds a list of
  // the names of the open data sets, so that the user can select
  // a data set as the 'current' data set.   This is the widget
  // that holds the menu list of the session's data sets
  Widget dataMenuWidget_;

  // the colormap editor
  AvColormapEditor * cmapEditor_;

  // Do index readouts start at 0 or 1?
  // If 0, the value is zero, else 1.
  static int oneIndexing_;

  // make the widgets for the main window, etc.
  void init ();

  // put together the menus
  void buildMenus ();

  // callback when user hits "okay" on file selection dialog
  // for bringing in new data if file is meaningful
  static void newFileNameCB (AvImageView * app, XtPointer d1);

  // Set to true if glish is active 
  int glishActive_;

  // Global value for indicating whether viewPanels should be built inside
  // or outside of imageWindows.
  static Boolean buildInside_;

//  void registerLocationSelection(AvGriddedData * data,
  void registerLocationSelection(AvAccessor *acc,
				 int x, int y, int z,
				 float value);

//  void registerRegionSelection(AvGriddedData * data,
  void registerRegionSelection(AvAccessor *acc,
			       int x1, int y1, int z1, 
			       int x2, int y2, int z2);

 
  void redisplayImages();
  static void colorMapChangedCB(XtPointer , AvConductor *c, XtPointer );

#ifdef GLISH

public:

  // Called only by aipsView.cc
  void makeGlishRequestList(Widget parent);

  void glishRequestList(AvGlishRequestList * rl)  
    { glishRequestList_ = rl; glishActive_ = 1; }
  AvGlishRequestList * glishRequestList() const   { return glishRequestList_; }

private:

  AvGlishRequestList * glishRequestList_;			      

#endif
	enum {	SV_IMAGE_FILE=2000, SV_HELP, SET_CURRENT_DATA, SV_IV_OPTION,
		SV_SETUP};
};

#endif
