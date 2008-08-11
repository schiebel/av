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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvImageViewPanel.h,v 19.0 2003/07/16 05:46:42 aips2adm Exp $
//
// $Log: AvImageViewPanel.h,v $
// Revision 19.0  2003/07/16 05:46:42  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:28  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:41:56  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:06  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:30  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:48:14  hravlin
// Changed className() to return const char *.
//
// Revision 14.0  2000/03/23 16:07:43  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:26  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:39  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:21  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:51:57  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1997/10/29 19:58:17  hr
// Added definition for LOCK_PROFILE.
//
// Revision 9.0  1997/08/25 21:24:40  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.3  1997/08/01 21:13:37  hr
// Moved most of the profile code to AvProfile.cc.
//
// Revision 8.2  1997/06/16 21:38:20  hr
// Added getXDataMinMax() and getXDataScale().
//
// Revision 8.1  1997/04/15 20:03:26  hr
// Added support for displaying cursor world position in "raw" mode.
//
// Revision 8.0  1997/02/20 03:18:06  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.6  1997/02/06 18:06:37  hr
// Added updateRegionIJK().
//
// Revision 7.5  1997/02/05 18:12:33  hr
// Changes to use AvProfileOptions, overlayed profile prints and
// dumping profiles to a file.
//
// Revision 7.4  1997/01/24 20:27:01  hr
// Added worldCursor().
//
// Revision 7.3  1996/12/12 06:33:01  droberts
// Final update from monet archive.
//
// Revision 1.7  1996/10/02 16:58:30  hr
// Removed unused class declarations.
//
// Revision 1.6  1996/10/02  14:24:39  hr
// Added SR argument to setSlice.
// Added routines to handle mouse input from profile window.
//
// Revision 1.5  1996/09/25  21:36:11  hr
// Changed interface to displayInfo.
// Added "const" to a number of functions &/or arguments.
// Added the function cursor() to return current cursor position.
//
// Revision 1.4  1996/09/20 20:19:35  pixton
// Prefixed all classes with Av
//
// Revision 1.3  1996/09/18  19:20:55  hr
// More integration with AvAccessor.
//
// Revision 1.2  1996/09/10 16:54:26  hr
// Initial changes for integration with AvAccessor & AvDataSet.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.18  1996/06/18  18:43:03  pixton
//  Copyright Update
//
//  Revision 1.17  1996/04/16  20:32:45  hr
//  Added setAtWindow().
//
//  Revision 1.16  1996/03/27  22:36:48  hr
//  Changes to support user selelected slice start and end values.
//
//  Revision 1.15  1996/03/20  16:24:23  hr
//  Added getProfile() and printProfile().
//
//  Revision 1.14  1996/02/09  18:19:01  hr
//  Added getShowMenuLength(), getShowMenuInfo(), buildShowMenu() &
//  processShowMenu().
//
//  Revision 1.13  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.13  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.13  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.12  1995/07/17  19:56:12  hr
//  Added 'virtual' to destructor of virtual classes.
//
//  Revision 1.11  1995/07/10  19:43:05  hr
//  Added displayValue().
//
//  Revision 1.10  1995/06/16  18:15:46  hr
//  Support for remembering/setting state of region/profile toggles.
//
//  Revision 1.9  1995/06/09  19:33:52  hr
//  Added show/hidePart so Region and Profile could be hidden.
//
//  Revision 1.8  1995/05/24  14:56:56  hr
//  Comment changes.
//
//  Revision 1.7  1995/05/17  17:13:04  hr
//  Removed old 'aix' comments.
//
//  Revision 1.7  1995/05/17  17:13:04  hr
//  Removed old 'aix' comments.
//
//  Revision 1.6  1995/05/12  21:23:38  hr
//  Changes to data clip min/max are propagated to profile display.
//
//  Revision 1.5  1995/04/19  20:40:14  hr
//  Added haveProfile_ flag so profiles aren't drawn until one exists.
//
//  Revision 1.4  1995/04/19  17:19:56  hr
//  Cleanup.
//
//  Revision 1.3  1995/04/19  14:25:57  hr
//  Changes to move animation controls to inside imageView window.
//  Added AvAnimPanel. Removed AvAnimCtlr.
//
//  Revision 1.2  1995/04/10  21:05:34  hr
//  Added support for syncronized animation.
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/*

This panel shows information about a particular view and supports user
interaction with that view (i.e., showing a particular slice,
auto-slicing through the cube, getting data values, defining regions,
getting profiles).

*/

#ifndef _IMAGE_VIEW_PANEL_H
#define _IMAGE_VIEW_PANEL_H

#include <Xm/Xm.h>

#include <AvFormComp.h>
#include <AvAccessor.h>

// mode flags to specify which feedback items have been turned on
#define UPDATE_VALUE   1
#define UPDATE_REGION  2
#define UPDATE_PROFILE 4
#define LOCK_PROFILE 6

class AvImageViewItem;
class AvStepper;
class AvTable;
class AvConductor;
class AvProfileOptions;
class AvPosition;

#ifdef PGPLOT
class AvProfileConfig;
class AvXWindow;
class AvPGDriver;
class AvViewportSetter;
#endif

class AvXProfile;
class AvPGProfile;


class AvImageViewPanel : public AvFormComp {

public:

  AvImageViewPanel
    (Widget parent, int buildInsideParent, AvImageViewItem & v);

  virtual const char * className () const;

  // Change the currently show slice. If SR is FALSE, Slice is a valid slice
  // index for the entire view axis. Otherwise, it must be within the
  // currently set subrange.
  void setSlice (const int slice, const Boolean SR=FALSE);

  // take an index x and y, and display relevant feedback information.
  void displayInfo (const int x, const int y);
  // Redisplay readouts. If force is true something changed other than a
  // position so redisplay even if it normally wouldn't.
  void updateReadouts(const Boolean force=FALSE);

  int sliceIndex () const { return (accessor()->getPlane()); }
  // Axis indexes in XYZ coordinates. These are constants since the
  // accessor does the mapping.
  int xAxis () const { return AvAccessor::XAXIS;}
  int yAxis () const { return AvAccessor::YAXIS;}
  int zAxis () const { return AvAccessor::ZAXIS;}
  int viewAxis () const { return AvAccessor::ZAXIS;} // Another name for zAxis.

  // convenience routines to set the various fields of this panel
  void setRowLabel (char * str, int axis);
  void setAtIJK (const int xDS, const int yDS);
  void setAtWindow (const int, const int); // Displays window coords.
  void setAtWorld (const char *, const char *);
  void setAtValue (const char *);
  void setRegionFrom (const char *, const char *);
  void setRegionFrom (const int, const int);
  void setRegionTo (const char *, const char *);
  void setRegionTo (const int, const int);
  // Set the region (mapped DS coords).
  void setRegion(const int x0, const int y0, const int x1, const int y1);
  // Get the region (mapped DS coords).
  void getRegion(int &x0, int &y0, int &x1, int &y1)const;
  unsigned short mode ()const { return (mode_); }

  // Are we doing different functions?
  inline Boolean doingValue()const {return (mode() & UPDATE_VALUE) != 0;}
  inline Boolean doingProfile()const {return (mode() & UPDATE_PROFILE) != 0;}
  inline Boolean doingRegion()const {return (mode() & UPDATE_REGION) != 0;}
  // Show/hide parts of panel
  enum { VALUE=1, REGION=2, PROFILE=4, HIDDENPARTS=6, ALLPARTS=7};
  void showPart(const int mask);
  void hidePart(const int mask);
  // Turn on/off a toggle (REGION or PROFILE).
  void setPartToggle(const int mask, const Boolean on);
  // If some part of the data changes, such as the clip min/max changes,
  // this should be called so the panel readouts can be adjusted.
  void updateDisplayInfo();

  virtual ~AvImageViewPanel ();
  // Region/Profile menus.
  static int getShowMenuLength();
  static AvMenuButtonInfo *getShowMenuInfo();
  static void buildShowMenu(AvMenu *showMenu, const int tag,
			    AvImageViewItem *vi=NULL);
  static void buildEditMenu(AvMenu *showMenu, const int tag,
			    AvAccessor *);
  // Process the menu request. Returns TRUE if "item" was handled.
  Boolean processShowMenu(char *item, Widget);
  Boolean processEditMenu(char *item, Widget);

  ////////////////////////////////////////////////////////////////
  //  			Profile Routines
  ////////////////////////////////////////////////////////////////
#if 0
  /* Get the profile information from the datacube at position
    x/y. (Returns values of the data cube along the view axis. If scale is
    FALSE, axis and data will hold the world coordinate values and data
    along the z axis. If scale is TRUE, the axis values will be 0..n-1
    and data will be scaled to be 0..1. (dataMin -> 0, dataMax->1).
  */
  void AvImageViewPanel::getProfile(const int x, const int y,
				const int zStart, const int zLen,
			double *axis, float *data, const Boolean scale);
  // Use current profile parameters. There will be numSlices() elements.
  void AvImageViewPanel::getProfile(
			double *axis, float *data, const Boolean scale);
#endif
  void redrawProfile ();
  AvXProfile *xProfile()const{return xProfile_;}
#ifdef PGPLOT
  AvPGProfile *pgProfile()const{return pgProfile_;}
#endif
  ////////////////////////////////////////////////////////////////

  // Get start and end slice indexes as set in slice control.
  int getStartSlice()const;
  int getEndSlice()const;
  // Returns the number of slices between getStartSlice and getEndSlice, not
  // the number of slices in the cube.
  int numSlices()const;
  // Returns the current cursor position in subrange (SR) coordinates.
  AvIPosition cursor()const;
  // Returns the current cursor position in world coordinates.
  AvWPosition worldCursor()const;
  // Print a profile line &/or markers.
  void printProfileOnly( const AvPosition &start, const AvPosition &end);
  // Print just the current profile. (No box or labels).
  void printProfileOnly();
  void showRawPosition(const Boolean);
  Boolean showRawPosition()const{return showRawPosition_;}
protected:

  AvTable * regionTable;

  Widget dataReadoutValue;	// Widget to display cursor location
				// if enabled.
private:

  // this is the imageViewItem passed at construction
  AvImageViewItem & vItem_;

  AvAccessor *accessor()const;

  // ---------- WHICH SLICE?

  Widget sliceIJK_, sliceWorld_;
  AvStepper * sliceStep_;
  void buildSliceControls (Widget parent);


  // -------------------- FEEDBACK ITEMS

  // keeps track of which feedback items are activated
  unsigned short mode_;

  void switchMode (Widget);

  Display * display_;
  GC gc_;

  AvIPosition ijk_;		// Tracks cursor/slice position.
				// In "subregion" coordinates.

  // ---------- DATA READBACK
  Widget valueToggle_;		// "Value" toggle gadget.
  Widget dataReadoutLocation;
  Widget dataReadoutWorldI, dataReadoutWorldJ;
  Widget dataReadoutWindowLocation; // Optional X/Y location in Window coords.
  // Cursor position in world coords.
  AvString imageWorldX_, imageWorldY_;
  Boolean showRawPosition_;	// Normally position is shows in world coords.

  float dataValue_;
  Widget buildDataReadout (Widget parent);
  void displayValue();		// Update current value readout.
  void displaySliceValue();	// Update current slice readout.

  // ---------- REGION DEFINITION
  Widget regionToggle_;		// "Region" toggle gadget.
				//  (also stored in feedbackToggles).
  Boolean oRegionState_;	// Was region on when it was last hidden?

  // Region in mapped DS (not subregion) coords.
  int regionTop_, regionBottom_, regionLeft_, regionRight_;
  
  Widget regionForm_;
  Widget regionIJKCoords;
  Widget buildRegionDef (Widget parent);
  static void showRegion (Widget, AvImageViewPanel * p, XtPointer);
  // Draw the region's IJK range field.
  void updateRegionIJK();

  // ---------- PROFILES
  // *** some of this should be moved to an "AvLineChart" class
  Widget profileToggle_;	// "Profile" toggle gadget.
				//  (also stored in feedbackToggles).
  Boolean oProfileState_;	// Was profile on when it was last hidden?
  Widget profileForm_;
  AvXProfile	*xProfile_;
#ifdef PGPLOT
  AvPGProfile	*pgProfile_;
#endif
  AvProfileConfig *profileSetup(){return profileSetup_;}
  ////////////////

  float dataMin_, dataRange_;		// Data min/range we're using.
  Widget buildProfile (Widget parent);
//  Boolean haveProfile_;		// True after a profile has been made.
#ifdef PGPLOT
  AvProfileConfig	*profileSetup_;	// Pointer to user interface object.
  // Used when drawing PGPLOT profile to a window.
  Widget		profilePGDrawArea_;
#endif
  AvProfileOptions *profileOptions_;	// Holds profile configuration vars.

//  void drawProfile (double * xPts, float * yPts, int nPts);
  // Recheck data min/max. Returns TRUE if min or max has changed.
  // (Profile needs to be redrawn).
//  Boolean updateProfileInfo();
//  void generateProfile();		// Generate Profile data.

  void showPart_(Widget);
  void hidePart_(Widget);
  // Handles printing of the profile, called by printProfileCB.
#if 0
  void printProfile(const Boolean hardcopy=TRUE); // Print entire profile.
  // dumps profile as ASCII text in X & Y columns.
  void dumpProfile();
  void updateProfileXLabels();
#endif
  // Show/no show Profile setup window.
  void toggleProfileSetupDisplay();
  // Handles mouse movement in profile window.
//  void profileHandler(XmDrawingAreaCallbackStruct *);
  // CALLBACKS
  static void redrawCB (Widget, AvImageViewPanel *, XtPointer);
  // Called when mouse is moved in profile window.
//  static void profileCB(Widget, AvImageViewPanel *, XtPointer);

  static void printProfileCB (Widget, AvImageViewPanel *, XtPointer);
  static void dumpProfileCB (Widget, AvImageViewPanel *, XtPointer);
  static void hcPreviewCB(Widget, AvImageViewPanel *, XtPointer);
  static void switchModeCB (Widget, AvImageViewPanel *, XtPointer);
  static void lockDimClipper (Widget, AvImageViewPanel *, XtPointer);
  static void updateSliceCB (AvImageViewPanel *, AvConductor *, XtPointer);
  // Called when the slice range is changed.
  static void rangeChangedCB (AvImageViewPanel *, AvConductor *, XtPointer);
  // Called by info_ when something changes. Does a redisplay.
  static void optionsInfoChangedCB(XtPointer o,
				   AvConductor *, XtPointer uData);
  static void profileConfigCB(AvProfileConfig *, XtPointer cbdata,
			      const int reason);
  // Called when something in the accessor changes.
  static void accessorCB_(XtPointer me, AvConductor *c, XtPointer uData);
  // Called by accessorCB_ to handle accessor changes.
  void accessorCB(AvConductor *c, XtPointer uData);
  void redisplay();
};

#endif
