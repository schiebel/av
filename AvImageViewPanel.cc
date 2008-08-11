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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvImageViewPanel.cc,v 19.0 2003/07/16 05:47:38 aips2adm Exp $
//
// $Log: AvImageViewPanel.cc,v $
// Revision 19.0  2003/07/16 05:47:38  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:17  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.3  2002/01/22 21:58:23  hravlin
// Made infoMenuLen a uInt. Gave some widgets non-empty names.
//
// Revision 17.2  2002/01/09 18:18:44  hravlin
// Changed references to MAXVALUE to FLOATMAX which is defined in Av.h.
//
// Revision 17.1  2002/01/07 22:31:50  hravlin
// Removed check for __P.
//
// Revision 17.0  2001/11/12 19:42:46  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:52  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.1  2000/12/21 20:20:36  hravlin
// Removed a call to vItem_.showSlice() that now causes an extra redraw.
//
// Revision 15.0  2000/10/26 17:11:47  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:48:01  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:08:44  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.2  1999/10/01 21:57:00  hravlin
// *** empty log message ***
//
// Revision 13.1  1999/08/25 19:48:06  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:40:33  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:37  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:09  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.3  1998/08/26 19:45:14  hr
// setSlice() gets called recursively when animating. Removed the warning
// message.
//
// Revision 10.2  1998/08/19 21:20:54  hr
// In buildProfile(), made sure that profile setup window would have
// class name of AipsView.
//
// Revision 10.1  1998/08/04 19:36:52  hr
// AvImageViewPanel::displaySliceValue() wasn't telling XYZToWorld() that
// ijk_ was in SR values.
//
// Revision 10.0  1998/07/20 17:55:12  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.4  1998/06/18 14:40:47  hr
// Added a comment saying the egcs issues an ignorable warning in showRegion(...).
//
// Revision 9.3  1997/12/08 22:18:42  hr
// In updateReadouts(), the call to XYZToWorld wasn't saying values were SR.
//
// Revision 9.2  1997/10/29 19:57:17  hr
// Added support for profile locking.
//
// Revision 9.1  1997/09/19 21:35:38  hr
// In accessorCB(), the call to sliceStep_->setValue was passing SR, not DS values.
// If the starting plane wasn't 0, this could cause setSlice to be recursively
// called and, sometimes, apparently creating an oscillation in the display.
//
// Revision 9.0  1997/08/25 21:29:47  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.8  1997/08/12 21:39:41  hr
// In the constructor, changed check of Item_.nSlice to accessor()->depth() for deciding whether
// to create profile support.
//
// Revision 8.7  1997/08/01 21:40:24  hr
// DCC found some unused code.
//
// Revision 8.6  1997/08/01 21:12:52  hr
// Moved most of the profile code to AvProfile.cc.
//
// Revision 8.5  1997/06/23 19:58:13  hr
// In openProfileDevice(), allow user to flip axes.
//
// Revision 8.4  1997/06/16 21:46:06  hr
// Changes to print axes in scaled format (m/s -> km/s).
//
// Revision 8.3  1997/05/20 19:43:43  hr
// In constructor, needed a check for rank. In setRegion, test for rank
// was against >= 2.
//
// Revision 8.2  1997/04/23 20:52:13  hr
// Changed variables named 'end' to 'End' to make SunOs compiler happy.
//
// Revision 8.1  1997/04/15 20:00:25  hr
// Added support for displaying cursor world position in "raw" mode rather
// than formated. Increased the number of digits shown when displaying in
// formatted mode.
//
// Revision 8.0  1997/02/20 03:18:13  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.15  1997/02/17 20:05:37  hr
// setSlice() wasn't checking to make sure sliceStep_ wasn't NULL (
// that there was more than one slice).
//
// Revision 7.14  1997/02/11 21:31:01  hr
// In displayInfo() added a check to make sure profile gets created if
// user requests the profile be displayed from the current cursor position.
//
// Revision 7.13  1997/02/09 22:32:08  bglenden
// undef INVALID if defined. It can be defined mby MwmUtil.h, which can
// cause an aipsview enum to get trashed.
//
// Revision 7.12  1997/02/09 00:37:24  hr
// Removed an unused variable.
//
// Revision 7.11  1997/02/09 00:11:21  hr
// displayInfo() wasn't checking to see if sliceStep_ was NULL when
// handling oneIndexing.
// Added ability to change profile colors.
//
// Revision 7.10  1997/02/06 18:02:42  hr
// Changes to support 1 relative readouts.
//
// Revision 7.9  1997/02/05 18:08:36  hr
// __P problem.
// Removed most environment variables.
// Changes to use AvProfileOptions, overlayed profile prints and
// dumping profiles to a file.
//
// Revision 7.8  1997/02/03 21:15:54  hr
// Init was using a blank value label instead of "Value" if dataUnits()
// returned an empty string.
//
// Revision 7.7  1997/01/24 20:28:27  hr
// Added worldCursor().
// printProfile() calls PGCanvas::cpgend() rather than directly calling PGPLOT.
//
// Revision 7.6  1997/01/09 21:38:30  hr
// setSlice() had an effeciency check so it wouldn't try to reset the current
// slice. Unfortunately, this conflicts with the way AnimPanel works.
// (The image would change but no other information for included windows).
//
// Revision 7.5  1996/12/17 17:12:07  droberts
// Removed call to isnand().
//
// Revision 7.4  1996/12/12 08:32:17  droberts
// Final update from monet archive.
//
// Revision 1.16  1996/11/11 15:57:50  hr
// Changed setRegionTo() and setRegionFrom() to each make a single call
// to XYZToWorld() rather than one for each axis.
//
// Revision 1.15  1996/11/06 22:44:25  hr
// Had previously misspelled __SVR4 as __SVR.
//
// Revision 1.14  1996/11/06 17:00:47  hr
// Sun's 4.1.3 doesn't know about isnand().
// The compiler was complaining about "end" being redefined in a couple
// of places.
//
// Revision 1.13  1996/11/04  20:10:36  hr
// In printProfile(), changed to use AvString rather than char * in
// some calls to PGCanvas.
//
// Revision 1.12  1996/10/02 16:55:58  hr
// profileHandler calls imageWindow::setPositions to update any included
// windows.
//
// Revision 1.11  1996/10/02  14:55:16  hr
// Added work around for problem of displayInfo sometimes trying to display
// NaN for world position.
//
// Revision 1.10  1996/10/02  14:20:54  hr
// Initial pass at letting slice be set from profile window.
// Added initial length to AvWPosition declarations.
// Changed setSlice() to take an optional SR argument.
// Calls to accessor()->{set,get}Plane() now state slice is in DS coords.
//
// Revision 1.9  1996/10/01  16:55:45  hr
// In displayInfo(), needed to give wp an initial length.
//
// Revision 1.8  1996/09/27  16:50:47  hr
// ijk_ needed to be fully initialized.
// In printProfile(), one of the zero line y coordinates wasn't being set.
//
// Revision 1.7  1996/09/25  21:39:06  hr
// Stepper made editable again. String formatting changed in a few places.
// More work to clean up DS vs SR problems.
// displayInfo modified to not need event mask. Code to set region was moved
// elsewhere.
// getProfile was modified to work better with AvAccessor.
// Profile printing was turned on.
// The function setRegion() was added to handle region changes from ImageWindow.
//
// Revision 1.6  1996/09/20  20:19:35  pixton
// Prefixed all classes with Av
//
// Revision 1.5  1996/09/18  19:20:55  hr
// More integration with AvAccessor.
//
// Revision 1.4  1996/09/10  16:54:26  hr
// Initial changes for integration with AvAccessor & AvDataSet.
//
// Revision 1.3  1996/08/19  20:41:56  pixton
// remove g++ warnings, provided code snippets for
// future integration (search AVINTEGRATION).
//
// Revision 1.2  1996/08/14  18:57:06  hr
// Changes to remove g++ warnings (from bglenden).
//
// Revision 1.1  1996/07/11  21:31:21  pixton
// Automated Checkin
//
//  Revision 1.36  1996/06/18  18:46:14  pixton
//  Copyright Update
//
//  Revision 1.35  1996/04/17  16:38:35  hr
//  printProfile() now draws the zero line rather than let pgtbox do it.
//
//  Revision 1.34  1996/04/16  20:47:20  hr
//  Had a problem in the dataReadoutWindow creation.
//
//  Revision 1.33  1996/04/16  20:28:43  hr
//  Reordered some code, removed some unused code, added setAtWindow().
//
//  Revision 1.32  1996/04/16  16:24:28  hr
//  Some compilers can't handle initializations like:
//  	char * buffer[] = { "A","B","C" };
//  Changing to "static char *..." works.
//
//  Revision 1.31  1996/03/28  22:26:55  hr
//  Changes to help prevent accessing non existent array entries when
//  the rank is 2.
//  Moved the "Printing to..." message so it is displayed after a successful
//  open (printProfile()).
//
//  Revision 1.30  1996/03/27  22:24:36  hr
//  Wide spread changes needed to support user editing of start and end
//  slice indexes.
//
//  Revision 1.29  1996/03/25  18:44:30  hr
//  Added ability to change line width and style and to draw as a histogram
//  to drawProfile.
//
//  Revision 1.28  1996/03/20  18:13:54  hr
//  Replaced a strcat with strcpy in printProfile.
//
//  Revision 1.27  1996/03/20  16:49:30  hr
//  Moved call to displayInfo to later in constructor so profile wouldn't be
//  generated until a mouse click.
//  Added support to print the profile (getProfile(), printProfile()).
//
//  Revision 1.26  1996/03/13  17:17:45  hr
//  Added drawing of zero line to drawProfile().
//
//  Revision 1.25  1996/02/09  18:22:47  hr
//  Removed an erroneous static from an enum definition.
//
//  Revision 1.24  1996/02/09  18:15:27  hr
//  Moved menu support for Region & Profile to AvImageViewPanel and
//  added check to see if Region &/or Profile should be turned on by default.
//
//  Revision 1.23  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.23  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.22  1995/08/14  21:11:21  hr
//  Profiles 'skip over' blanked data rather than drawing them.
//  If the first point was blanked, value readout remained blank until a non
//  blanked value was chosen.
//
//  Revision 1.21  1995/07/28  21:03:05  hr
//  Changed reference from MAXFLOAT to MAXVALUE (defined in Av.h).
//
//  Revision 1.20  1995/07/26  19:41:29  hr
//  Initialize dataValue_ to MAXFLOAT.
//
//  Revision 1.19  1995/07/20  18:57:00  hr
//  During last checkin, the file got truncated.
//
//  Revision 1.18  1995/07/20  18:48:17  hr
//  showRegion had accidently started deleting the AvImageViewItem it created.
//
//  Revision 1.17  1995/07/20  17:19:13  hr
//  Replaced use of HUGE_VAL with MAXVALUE and BLANKEDVALUE.
//
//  Revision 1.16  1995/07/18  17:14:41  hr
//  g++ doesn't like constructs like for(int i;;); for(i). Or buf[int &];
//
//  Revision 1.15  1995/07/11  21:23:09  hr
//  Call imageWindow::updateBox when "Value" or "Region" toggles change.
//
//  Revision 1.14  1995/07/10  19:43:35  hr
//  Added displayValue(). Changing Slice index now updates value readout.
//
//  Revision 1.13  1995/06/27  16:29:15  hr
//  Profile now displays blanked data using the previous value.
//
//  Revision 1.12  1995/06/23  15:28:38  hr
//  Changed display of blanked data to say "Blanked".
//
//  Revision 1.11  1995/06/16  18:04:43  hr
//  Hide region/profile if built inside parent.
//  When "Show Region" or "Show Profile" is selected the first time,
//  enable region/profile. Turn off when hiding, but remember for the
//  next time its shown.
//  displayInfo was always redrawing profile even when profiling was off.
//
//  Revision 1.10  1995/06/09  19:33:01  hr
//  Added show/hidePart so Region and Profile could be hidden.
//
//  Revision 1.9  1995/05/18  20:58:35  hr
//  The "Value" toggle in the info box is on by default
//
//  Revision 1.8  1995/05/17  17:10:49  hr
//  Profile: added a vertical line indicating current slice.
//  	    min/max labels are constant width and created with g format.
//  	    updateDisplayInfo was calling updateProfileInfo even when
//  	    there was no profile.
//
//  Revision 1.7  1995/05/12  21:20:09  hr
//  Changes to data clip min/max are propagated to profile display.
//
//  Revision 1.6  1995/05/02  20:31:25  pixton
//  Added registration of region and location selection with the
//  imageDataItem to separate the method of selection from the
//  glish interpreter.
//
//  Revision 1.5  1995/04/19  20:39:29  hr
//  Added haveProfile_ flag so profiles aren't drawn until one exists.
//
//  Revision 1.4  1995/04/19  17:18:49  hr
//  setSlice checks for valid slice index.
//
//  Revision 1.3  1995/04/19  14:28:22  hr
//  Changes to move animation controls to inside imageView window.
//  Added AvAnimPanel. Removed AvAnimCtlr.
//
//  Revision 1.2  1995/04/10  21:02:55  hr
//  Added support for syncronized animation.
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>			// getenv
#include <iostream.h>
//#include <math.h>
#include <Xm/ArrowB.h>
#include <Xm/DrawingA.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/Frame.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Scale.h>
#include <Xm/Separator.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>
#include <Xm/MwmUtil.h>	// The MWM_DECOR constants.

#if defined(INVALID)
// This define in MwmUtil.h can make AvImageControlPanel.h unhappy
#undef INVALID
#endif

#include "AvClipper.h"
#include "AvImageView.h"
#include "AvImageViewPanel.h"
#include "AvStepper.h"
#include "AvTable.h"
#include "AvUserComp.h"
#include "AvImageWindow.h"		// for getImageWindow call.
#include "AvPosition.h"
#include "AvProfile.h"
#include "Av.h"

#ifdef PGPLOT
#include "AvProfileConfig.h"

// Menu string for opening profile setup window.
static const char *PROFILEMENU=	"Profile";
#endif

#include "AvProfileOptions.h"

/* Get environment string for environment variable str.
*/
static char *getEnvStr(const char *str, char *def=NULL)
{
char	*env;

	env = getenv(str);
	if((env == NULL) || (strlen(env) == 0))
		return def;
	else
		return env;
}

static Boolean getEnvBoolean(const char *str, Boolean def=FALSE)
{
char	*env;

	env = getEnvStr(str);
	if(env == NULL)
		return def;
	if((*env == 'T') || (*env == 't') || (*env == '1'))
		return TRUE;
	else
	if((*env == 'F') || (*env == 'f') || (*env == '0'))
		return FALSE;
	return def;
}

const char * AvImageViewPanel::className () const
{
  return ("AvImageViewPanel");
}

static AvMenuButtonInfo infoMenu[] = {
	{ "Region", TOGGLE_BUTTON, 0},
	{ "Profile", TOGGLE_BUTTON, 0},
	{ "Included Profiles", TOGGLE_BUTTON, 0},
	};
enum {REGIONINDEX, PROFILEINDEX, INCLUDEDPROFILEINDEX};

static const uInt infoMenuLen = (uInt)(sizeof(infoMenu)/sizeof(*infoMenu));
static const int ACCMASK = AvAccessor::DEFAULTMASK | AvDataSet::DEFAULTMASK
			 | AvAccessor::PLANE | AvAccessor::REGION;

AvImageViewPanel::AvImageViewPanel
(Widget parent, int buildInsideParent, AvImageViewItem & v)
 : AvFormComp (parent, buildInsideParent),
   vItem_ (v)
{
  mode_ = 0;
  gc_ = 0;
  showRawPosition_ = FALSE;
  sliceStep_ = NULL;
  dataValue_ = -FLOATMAX;	// Don't want it to be FLOATMAX.
  int rank = v.accessor()->rank();
  int i;

 accessor()->addCallback(&AvImageViewPanel::accessorCB_,
			 ACCMASK, this, NULL);

  // Fill in the table
  // These values are mapped dataset (not subregion).
  regionLeft_ = accessor()->getAxisStart(xAxis());
  regionRight_ = regionLeft_ + accessor()->getAxisLength(xAxis()) -1;
  regionBottom_ = accessor()->getAxisStart(yAxis());
  regionTop_ = regionLeft_ + accessor()->getAxisLength(yAxis())-1;

  // Current cursor position in subrange (SR) coordinates.
  ijk_.resize(rank);
  ijk_ = 0;
  if(rank > 2)
	ijk_(viewAxis()) = accessor()->getPlane();

  // FEEDBACK ITEMS
  int nFeedbackToggles = 3;

  WidgetList feedbackToggles = 
    (WidgetList) XtMalloc (nFeedbackToggles * (int)sizeof (Widget));

  int toggles = 0;

  if(! buildInsideParent)
    XtVaSetValues (parentWidget_,
		 XmNallowShellResize, TRUE,
		 XmNmwmDecorations,
		 MWM_DECOR_ALL |
		 MWM_DECOR_MENU |
		 MWM_DECOR_MINIMIZE |
		 MWM_DECOR_MAXIMIZE,
		 NULL);

  XtVaSetValues (baseWidget_,
		 XmNtopAttachment, XmATTACH_FORM,
		 XmNleftAttachment, XmATTACH_FORM,
		 XmNrightAttachment, XmATTACH_FORM,
		 NULL);

  Widget parts = XtVaCreateManagedWidget
    ("partsRC", xmRowColumnWidgetClass, baseWidget_,
     XmNorientation, XmVERTICAL,
     XmNspacing, 3,
     XmNmarginTop, 10,
     XmNmarginWidth, 8,
     XmNleftAttachment, XmATTACH_FORM,
     XmNrightAttachment, XmATTACH_FORM,
     XmNtopAttachment, XmATTACH_FORM,
//     XmNbottomAttachment, XmATTACH_FORM,
     NULL);

  if (vItem_.nSlice (viewAxis()) > 1) {
    Widget sliceInfo = XtVaCreateManagedWidget
      ("sliceInfo", xmFormWidgetClass, parts,
       NULL);

    buildSliceControls (sliceInfo);

    XtVaCreateManagedWidget ("", xmSeparatorWidgetClass, parts, NULL);
  }

  Widget dataReadoutForm = XtVaCreateManagedWidget
    ("dataReadoutForm", xmFormWidgetClass, parts,
     NULL);

  feedbackToggles[toggles++] = buildDataReadout (dataReadoutForm);
  XtVaCreateManagedWidget ("", xmSeparatorWidgetClass, parts, NULL);

  Widget regionDefForm = XtVaCreateManagedWidget
    ("regionDefForm", xmFormWidgetClass, parts,
     NULL);
  regionForm_ = regionDefForm;

  feedbackToggles[toggles++] = buildRegionDef (regionDefForm);

  if(accessor()->depth() > 1) {
    XtVaCreateManagedWidget ("", xmSeparatorWidgetClass, parts, NULL);

    Widget profileForm = XtVaCreateManagedWidget
      ("", xmFormWidgetClass, parts,
       NULL);
    profileForm_ = profileForm;

    feedbackToggles[toggles++] = buildProfile (profileForm);

  }
  else
  {
	profileForm_ = NULL;
	profileToggle_ = NULL;
	xProfile_ = NULL;
	pgProfile_ = NULL;
#ifdef PGPLOT
	pgProfile_ = NULL;
#endif
	profileOptions_ = NULL;
	profileSetup_ = NULL;
  }

  // Hide region/profile unless defaults have been set to show them.
  if(builtInsideParent())
  { int hparts = HIDDENPARTS;
	if(infoMenu[REGIONINDEX].buttonData != 0)
		hparts &= ~REGION;
	if(infoMenu[PROFILEINDEX].buttonData != 0)
		hparts &= ~PROFILE;
	hidePart(hparts);
  }

//  displayInfo(0, 0, 0);
  ijk_(xAxis()) = -1;	// X & Y will get set via displayInfo call below.
			// This will force the change.
  displayInfo(0, 0);

  // add the callbacks to the toggles controlling feedback mode
  for (i=0; i<toggles; i++)
    XtAddCallback (feedbackToggles[i],
		   XmNvalueChangedCallback,
		   (XtCallbackProc) &AvImageViewPanel::switchModeCB,
		   (XtPointer) this);
}

// Turn on/off a toggle: VALUE, REGION or PROFILE. Except VALUE is never
// changed here.
void AvImageViewPanel::setPartToggle(const int mask, const Boolean on)
{
	if(mask & VALUE)
	{	XmToggleButtonSetState(valueToggle_, on, FALSE);
		if(on)
			mode_ |= UPDATE_VALUE;
		else
			mode_ &= ~UPDATE_VALUE;

	}
	if(mask & REGION)
	{	XmToggleButtonSetState(regionToggle_, on, FALSE);
		if(on)
			mode_ |= UPDATE_REGION;
		else
			mode_ &= ~UPDATE_REGION;

	}
	if((mask & PROFILE) && (profileToggle_ != NULL))
	{	XmToggleButtonSetState(profileToggle_, on, FALSE);
		if(on)
			mode_ |= UPDATE_PROFILE;
		else
			mode_ &= ~UPDATE_PROFILE;
	}
}

void AvImageViewPanel::showPart_ (Widget w)
{
	if( (w != NULL) && (!XtIsManaged(w)))
		XtManageChild(w);
}

AvAccessor *AvImageViewPanel::accessor()const{return vItem_.accessor();}

void AvImageViewPanel::hidePart_ (Widget w)
{
	if( (w != NULL) && (XtIsManaged(w)))
		XtUnmanageChild(w);
}

void AvImageViewPanel::showPart (const int mask)
{
	if(mask & REGION)
	{	// Set the toggle to the state it had when last hidden.
		setPartToggle(REGION, oRegionState_);
		showPart_(regionForm_);
	}
	if(mask & PROFILE)
	{	// Set the toggle to the state it had when last hidden.
		setPartToggle(PROFILE, oProfileState_);
		showPart_(profileForm_);
	}
}

// Hide a part and turn it off remembering how it was set.
void AvImageViewPanel::hidePart (const int mask)
{
	if(mask & REGION)
	{	hidePart_(regionForm_);
		oRegionState_ = doingRegion();
	}
	if(mask & PROFILE)
	{	hidePart_(profileForm_);
		oProfileState_ = doingProfile();
	}
	// Disable what we hide.
	setPartToggle(mask, FALSE);
}

void AvImageViewPanel::showRegion (Widget, AvImageViewPanel * p, XtPointer)
{
  // Create view of subregion. Since it is in its own window, it will delete
  // itself.
  if(p == NULL)
	return;
    // This causes a warning from the egcs compiler that should be ignored.
    // (See above comment).
    new AvImageViewItem (p->vItem_);
}

// Get around XmTextFieldSetString not taking a const char *.
static void SetXmTextFieldSetString (Widget w, const char *cs)
{ char *s = new char[strlen(cs) +1];

	strcpy(s, cs);
	XmTextFieldSetString(w, s);
	delete [] s;
}

void AvImageViewPanel::buildSliceControls (Widget parent)
{

  Widget labelWidget = XtVaCreateManagedWidget
    ("FontBoldItalic", xmLabelWidgetClass, parent,
     XmNalignment, XmALIGNMENT_BEGINNING,
     NULL);
  setLabelString (labelWidget, accessor()->axisName(viewAxis()));
  
  Widget sliceForm = XtVaCreateManagedWidget
    ("", xmFormWidgetClass, parent,
     XmNtopAttachment, XmATTACH_FORM,
     XmNleftAttachment, XmATTACH_WIDGET,
     XmNleftWidget, labelWidget,
     XmNleftOffset, 10,
     XmNmarginHeight, 0,
     XmNshadowThickness, 1,
     XmNshadowType, XmSHADOW_IN,
     NULL);

  sliceWorld_ = XtVaCreateManagedWidget
    ("FontPlain", xmTextFieldWidgetClass, parent,
     XmNtopAttachment, XmATTACH_FORM,
     XmNleftAttachment, XmATTACH_WIDGET,
     XmNleftWidget, sliceForm,
     XmNleftOffset, 10,
     XmNrightAttachment, XmATTACH_FORM,
     XmNcolumns, 8,
     XmNshadowThickness, 1,
     XmNeditable, False,
     NULL);

  // Create the slice stepper.
  int sliceIndex = accessor()->getPlane();	// Slice in SR coords.
// Index of current plane in world coordinates.
  double zVal;
  // It is possible for the value to depend on the X or Y value, but we
  // have to choose something, so we choose 0,0.
  accessor()->XYZToWorld(0, 0, sliceIndex, zAxis(), zVal);

  SetXmTextFieldSetString (sliceWorld_,
	accessor()->axisValueUnits(zAxis(), zVal).chars());

  // Stepper showing start and end planes.
  int zStart = accessor()->getAxisStart(zAxis());
  int zEnd   = zStart + accessor()->getAxisLength(zAxis()) - 1;
  sliceStep_ = new AvStepper(sliceForm, TRUE, zStart, zEnd, TRUE);
  sliceStep_->setDisplayOffset(AvImageView::oneIndexing());
  sliceStep_->setValue (accessor()->getPlane(FALSE));	// In DS coords.

  sliceStep_->addCallback
    ((AvConductorCB) &AvImageViewPanel::updateSliceCB,
     AvConductor::ValueChanged,
     (XtPointer) this, (XtPointer) NULL);

  // In case user changes slice range.
  sliceStep_->addCallback
    ((AvConductorCB) &AvImageViewPanel::rangeChangedCB,
     AvStepper::RangeChanged,
     (XtPointer) this, (XtPointer) NULL);

  sliceStep_->show();

  sliceIJK_ = sliceStep_->baseWidget();

  XtVaSetValues (sliceIJK_,
		 XmNleftAttachment, XmATTACH_FORM,
		 XmNrightAttachment, XmATTACH_FORM,
		 XmNshadowThickness, 1,
		 XmNshadowType, XmSHADOW_IN,
		 XmNmarginHeight, 0,
		 NULL);
}

Widget AvImageViewPanel::buildDataReadout (Widget parent)
{
  // Create the "Value" toggle and start off in UPDATE_VALUE mode.
  Widget dataReadoutLabel = XtVaCreateManagedWidget
    ("FontBoldItalic", xmToggleButtonWidgetClass, parent,
     XmNuserData, UPDATE_VALUE,
     XmNset,	TRUE,
     NULL);
  // The label for the data "Value" indicator was "Value" now it's
  // the data unit string. (eg. "jy/beam").
  const char *value = accessor()->dataUnits();
  if((value == NULL) || (strlen(value) == 0))
	value = "Value";
  setLabelString (dataReadoutLabel, value);

  valueToggle_ = dataReadoutLabel;

    mode_ |= UPDATE_VALUE;

  dataReadoutValue = XtVaCreateManagedWidget
    ("FontPlain", xmTextFieldWidgetClass, parent,
     XmNtopAttachment, XmATTACH_FORM,
     XmNleftAttachment, XmATTACH_WIDGET,
     XmNleftWidget, dataReadoutLabel,
     XmNcolumns, 16,
     XmNshadowThickness, 1,
     XmNeditable, FALSE,
     XmNcursorPositionVisible, FALSE,
     NULL);

  dataReadoutLocation = XtVaCreateManagedWidget
    ("FontPlain", xmTextFieldWidgetClass, parent,
     XmNleftOffset, 8,
     XmNleftAttachment, XmATTACH_WIDGET,
     XmNleftWidget, dataReadoutValue,
     XmNrightAttachment, XmATTACH_FORM,
     XmNrightOffset, 8,
     XmNcolumns, 10,
     XmNshadowThickness, 1,
     XmNeditable, FALSE,
     XmNcursorPositionVisible, FALSE,
     NULL);

  Widget dataReadoutWorldLabelI = XtVaCreateManagedWidget
    ("FontPlain", xmLabelWidgetClass, parent,
     XmNtopAttachment, XmATTACH_WIDGET,
     XmNtopWidget, dataReadoutLocation,
     XmNtopOffset, 8,
     NULL);
  setLabelString (dataReadoutWorldLabelI, accessor()->axisName(xAxis()));

  dataReadoutWorldI = XtVaCreateManagedWidget
    ("FontPlain", xmTextFieldWidgetClass, parent,
     XmNtopAttachment, XmATTACH_WIDGET,
     XmNtopWidget, dataReadoutLocation,
     XmNtopOffset, 4,
     XmNleftAttachment, XmATTACH_WIDGET,
     XmNleftWidget, dataReadoutWorldLabelI,
     XmNrightAttachment, XmATTACH_POSITION,
     XmNrightPosition, 49,
     XmNcolumns, 10,
     XmNshadowThickness, 1,
     XmNeditable, FALSE,
     XmNcursorPositionVisible, FALSE,
     NULL);     

  Widget dataReadoutWorldLabelJ = XtVaCreateManagedWidget
    ("FontPlain", xmLabelWidgetClass, parent,
     XmNtopAttachment, XmATTACH_WIDGET,
     XmNtopWidget, dataReadoutLocation,
     XmNtopOffset, 8,
     XmNleftAttachment, XmATTACH_POSITION,
     XmNleftPosition, 50,
     NULL);
  setLabelString (dataReadoutWorldLabelJ, accessor()->axisName(yAxis()));

  dataReadoutWorldJ = XtVaCreateManagedWidget
    ("FontPlain", xmTextFieldWidgetClass, parent,
     XmNtopAttachment, XmATTACH_WIDGET,
     XmNtopWidget, dataReadoutLocation,
     XmNtopOffset, 4,
     XmNleftAttachment, XmATTACH_WIDGET,
     XmNleftWidget, dataReadoutWorldLabelJ,
     XmNrightAttachment, XmATTACH_FORM,
     XmNcolumns, 15,
     XmNshadowThickness, 1,
     XmNeditable, FALSE,
     XmNcursorPositionVisible, FALSE,
     NULL);     

	dataReadoutWindowLocation = NULL;

    // This will allow displaying the cursor X/Y location. Since it's only
    // useful for debugging, you need to know the magic environment variable
    // to turn it on.
#define SHOWWINDOWLOCATION
#ifdef SHOWWINDOWLOCATION
  if(getEnvBoolean("AV_SHOW_XLOC"))
  { 
	Widget dataReadoutWindowLabel = XtVaCreateManagedWidget
		("FontPlain", xmLabelWidgetClass, parent,
		XmNtopAttachment, XmATTACH_WIDGET,
		XmNtopWidget, dataReadoutWorldLabelI,
		XmNtopOffset, 4,
		NULL);
	setLabelString (dataReadoutWindowLabel, "Cursor x/y");

	dataReadoutWindowLocation = XtVaCreateManagedWidget
	    ("FontPlain", xmTextFieldWidgetClass, parent,
		XmNtopAttachment, XmATTACH_WIDGET,
		XmNtopWidget, dataReadoutWorldI,
		XmNtopOffset, 6,
		XmNleftAttachment, XmATTACH_WIDGET,
		XmNleftWidget, dataReadoutWindowLabel,
		XmNrightAttachment, XmATTACH_POSITION,
		XmNrightPosition, 49,
		XmNcolumns, 10,
		XmNshadowThickness, 1,
		XmNeditable, FALSE,
		XmNcursorPositionVisible, FALSE,
		NULL);
  }
#endif

  return (dataReadoutLabel);
}


Widget AvImageViewPanel::buildRegionDef (Widget parent)
{
  // If panel will be built inside parent, enable. It will be
  // turned off by the hidePart() call in constructor.
  Widget regionDefLabel = XtVaCreateManagedWidget
    ("FontBoldItalic", xmToggleButtonWidgetClass, parent,
     XmNuserData, UPDATE_REGION,
     XmNset,	builtInsideParent(),
     NULL);
  setLabelString (regionDefLabel, "Region");
  regionToggle_ = regionDefLabel;

  if(builtInsideParent())
  	mode_ |= UPDATE_REGION;

  regionIJKCoords = XtVaCreateManagedWidget 
    ("FontPlain", xmTextFieldWidgetClass, parent,
     XmNleftAttachment, XmATTACH_WIDGET,
     XmNleftWidget, regionDefLabel,
     XmNleftOffset, 15,
     XmNrightAttachment, XmATTACH_FORM,
     XmNshadowThickness, 1,
     XmNeditable, FALSE,
     XmNcursorPositionVisible, FALSE,
     NULL);

  // make the table
  regionTable = new AvTable (parent, TRUE, 2, 2);

  // position the table's widget
  Widget regionTableWidget = regionTable->baseWidget ();
  XtVaSetValues (regionTableWidget,
		 XmNtopAttachment, XmATTACH_WIDGET,
		 XmNtopWidget, regionDefLabel,
		 XmNleftAttachment, XmATTACH_FORM,
		 XmNrightAttachment, XmATTACH_FORM,
		 NULL);

  // set the labels of the region table
  static const char * defcLabels[] = {"Bottom Left", "Top Right"};
  regionTable->setColumnLabels (defcLabels);

  regionTable->setRowLabel(accessor()->axisName(xAxis()), 0);
  regionTable->setRowLabel(accessor()->axisName(yAxis()), 1);

  regionTable->setCellWidth (8);

  setRegionFrom (regionLeft_, regionBottom_);
  setRegionTo (regionRight_, regionTop_);
  char str[128];
  sprintf (str, "(%d, %d) to (%d, %d)",
	   regionLeft_, regionBottom_, regionRight_, regionTop_);
  XmTextFieldSetString (regionIJKCoords, str);

  regionTable->show();

  // if this is a toplevel view, then we support region display
  // in a separate window.  Otherwise, we don't want this button.
  if (!vItem_.subview()) {
    Widget regionShowButton = XtVaCreateManagedWidget
      ("Display Region", xmPushButtonWidgetClass, parent,
       XmNtopAttachment, XmATTACH_WIDGET,
       XmNtopWidget, regionTable->baseWidget(),
       XmNleftAttachment, XmATTACH_FORM,
       XmNrightAttachment, XmATTACH_FORM,
       XmNshadowThickness, 1,
       NULL);

    XtAddCallback (regionShowButton, XmNactivateCallback,
		   (XtCallbackProc) &AvImageViewPanel::showRegion,
		   (XtPointer) this);
  }

  return (regionDefLabel);
}

Widget AvImageViewPanel::buildProfile (Widget parent)
{
#ifdef PGPLOT
  Widget profileRCW = XtVaCreateManagedWidget
    ("profileRCW", xmRowColumnWidgetClass, parent,
     XmNorientation, XmHORIZONTAL,
//     XmNspacing, 30,
     XmNspacing, 0,
     XmNmarginTop, 1,
     XmNmarginWidth, 1,
     XmNleftAttachment, XmATTACH_FORM,
     XmNrightAttachment, XmATTACH_FORM,
//     XmNtopAttachment, XmATTACH_FORM,
//     XmNbottomAttachment, XmATTACH_FORM,
     NULL);
  // If panel will be built inside parent, enable profiling. It will be
  // turned off by the hidePart() call in constructor.
  Widget profileLabel = XtVaCreateManagedWidget
    ("FontBoldItalic", xmToggleButtonWidgetClass, profileRCW,
     XmNuserData, UPDATE_PROFILE,
     XmNset,	  builtInsideParent(),
     NULL);
  setLabelString (profileLabel, "Profile");
  profileToggle_ = profileLabel;

  Widget lockProfileW = XtVaCreateManagedWidget
    ("FontBoldItalic", xmToggleButtonWidgetClass, profileRCW,
     XmNuserData, LOCK_PROFILE,
//     XmNset,	  builtInsideParent(),
     XmNmarginLeft, 2,
     XmNmarginRight, 2,
     NULL);
  setLabelString (lockProfileW, "Lock");
  XtAddCallback (lockProfileW,
		 XmNvalueChangedCallback,
		 (XtCallbackProc) &AvImageViewPanel::switchModeCB,
		 (XtPointer) this);


  // Put a frame around the print and preview buttons.
  Widget printFrame = XtVaCreateManagedWidget("xprofile",
			xmFrameWidgetClass,
			profileRCW,
			NULL);

  Widget printRCW = XtVaCreateManagedWidget
    ("printRCW", xmRowColumnWidgetClass, printFrame,
     XmNorientation, XmHORIZONTAL,
//     XmNspacing, 30,
     NULL);

  Widget printProfileW = XtVaCreateManagedWidget
//    ("FontBoldItalic", xmPushButtonWidgetClass, profileRCW,
    ("FontBoldItalic", xmPushButtonWidgetClass, printRCW,
     NULL);

  setLabelString (printProfileW, "Print");

    XtAddCallback (printProfileW, XmNactivateCallback,
		   (XtCallbackProc) &AvImageViewPanel::printProfileCB,
		   (XtPointer) this);

  Widget hcPrvw = XtVaCreateManagedWidget
    ("FontBoldItalic", xmToggleButtonWidgetClass, printRCW,
//     XmNset,	  FALSE,	// config/resrc variable?
     NULL);

  setLabelString ( hcPrvw, "Preview");
	XtAddCallback(hcPrvw, 
		   XmNvalueChangedCallback,
		   (XtCallbackProc) &AvImageViewPanel::hcPreviewCB,
		   (XtPointer) this);

  Widget dumpProfileW = XtVaCreateManagedWidget
    ("FontBoldItalic", xmPushButtonWidgetClass, profileRCW,
     NULL);
  setLabelString (dumpProfileW, "Dump");

    XtAddCallback (dumpProfileW, XmNactivateCallback,
		   (XtCallbackProc) &AvImageViewPanel::dumpProfileCB,
		   (XtPointer) this);


#else
  // If panel will be built inside parent, enable profiling. It will be
  // turned off by the hidePart() call in constructor.
  Widget profileLabel = XtVaCreateManagedWidget
    ("FontBoldItalic", xmToggleButtonWidgetClass, parent,
     XmNuserData, UPDATE_PROFILE,
     XmNset,	  builtInsideParent(),
     NULL);
  setLabelString (profileLabel, "Profile");
  profileToggle_ = profileLabel;
#endif

  if(builtInsideParent())
	mode_ |= UPDATE_PROFILE;

  // Create profile setup objects.
  const char *filename = accessor()->dataset()->fileName();
  profileOptions_ = new AvProfileOptions(filename, "profile");
  profileOptions_->ref();

  Widget profileForm = XtVaCreateManagedWidget
    ("", xmFormWidgetClass, parent,
     XmNtopAttachment, XmATTACH_WIDGET,
     XmNtopWidget, profileRCW,
     XmNleftAttachment, XmATTACH_FORM,
     XmNrightAttachment, XmATTACH_FORM,
     XmNbottomAttachment, XmATTACH_FORM,
     NULL);

  xProfile_ = new AvXProfile("xProfile",
			     profileForm, accessor(), profileOptions_,
			     AvXColorApp::palette());
  // This allows notifying included windows when profileOptions_ changes.
  xProfile_->profileData()->setViewItem(&vItem_);

#ifdef PGPLOT
  pgProfile_ = new AvPGProfile( profileForm, xProfile_->profileData(),
				profileOptions_, AvXColorApp::palette());
//#else
//  pgProfile_ = NULL;
#endif

#ifdef PGPLOT
   // Use this rather than one of our widgets so the class name gets set
   // to "AipsView" rather than "TopLevelShell".
   Widget ref = AvImageView::instance()->baseWidget();
   profileSetup_ = new AvProfileConfig(filename, ref,
				      profileOptions_);
//  profileSetup_->addCallback( &AvImageViewPanel::profileConfigCB, this);
#endif
  return (profileLabel);
}

#if 0
// If some part of the data changes, such as the clip min/max changes,
// this should be called so the panel readouts can be adjusted.
void AvImageViewPanel::updateDisplayInfo()
{
//	cout << "AvImageViewPanel::updateDisplayInfo: called." << endl;
}
#endif

// Change the value field if current data value has changed.
void AvImageViewPanel::displayValue()
{
  // get the actual data value of this point
  float newValue = accessor()->getDatum(ijk_);
#if 0
cout << "ijk_ = " << ijk_ << " shape = " << accessor()->shape()
     << " start = " << accessor()->start() << " value = " << newValue << endl;
#endif

  if(newValue == dataValue_)
	return;

  dataValue_ = newValue;

  // update value displayed in the feedback panel
  if(doingValue())
	setAtValue(accessor()->dataValue(dataValue_, 10).chars());
}

// Display cursor position in world and IJK coords and the data value at
// the position. X and Y are mapped XYZ coords relative to the start of
// the current region.
// Called from AvImageWindow to update cursor info.
void AvImageViewPanel::displayInfo(const int x, const int y)
{
  int length = ijk_.nelements();

  //		Update cursor position
  // This can save redundant updates.
  if((ijk_(xAxis())==x) && (ijk_(yAxis())==y))
  {	if(doingProfile()) // Profile can change if doing included profiles.
		redrawProfile();
	return;
  }

  ijk_(xAxis()) = x;	// Remember cursor location.
  ijk_(yAxis()) = y;

  if(length > 2)
  {	ijk_(zAxis()) = accessor()->getPlane();
	if(xProfile_ != NULL)
		xProfile_->setXY(x, y);	// Update (both) profile positions.
  }
    updateReadouts();
    AvImageDataItem * idi =
			AvImageView::instance()->search(accessor()->dataset());

    // Remember the last cursor location for this data set (not just view).
    if (idi)
      {
	AvIPosition ipos(ijk_.nelements());
	ipos = accessor()->XYZtoDS(ijk_);
	idi->lastLocation(ipos, dataValue_);
      }

  // Do the profile.
  if(doingProfile())
	redrawProfile();
}

// Redisplay readout values.
void AvImageViewPanel::updateReadouts(const Boolean force)
{
  int length = ijk_.nelements();
  AvWPosition wp(length);
  //		Update cursor position
  accessor()->XYZToWorld(ijk_, wp, TRUE); // Position in world coords.
  imageWorldX_ = accessor()->axisValueUnits(xAxis(), wp(xAxis()), 8);
  imageWorldY_ = accessor()->axisValueUnits(yAxis(), wp(yAxis()), 8);

  if((sliceStep_ != NULL) &&
     (sliceStep_->getDisplayOffset() != AvImageView::oneIndexing()))
	sliceStep_->setDisplayOffset(AvImageView::oneIndexing());

  // Update Value readout.
  if(doingValue() || force) {
    displayValue();
    int xDS =  ijk_(xAxis()) + accessor()->getAxisStart(xAxis());
    int yDS =  ijk_(yAxis()) + accessor()->getAxisStart(yAxis());
    setAtIJK(xDS, yDS);		// Position in DS values.
    if(showRawPosition_)
    {	// Displays the axis values in "raw" mode.
	char xw[64], yw[64];
	sprintf(xw, "%16.12g", wp(xAxis()));
	sprintf(yw, "%16.12g", wp(yAxis()));
	setAtWorld (xw, yw);
    }
    else    // Display the axis values.
	setAtWorld (imageWorldX_.chars(), imageWorldY_.chars());
}
  // Normally, this is done elsewhere.
  if(force)
	updateRegionIJK();
}

// Called when one of the value, region or profile toggles changes.
void AvImageViewPanel::switchMode (Widget w)
{
  int i;
  Boolean isSet = XmToggleButtonGetState (w);
  XtVaGetValues (w, XmNuserData, &i, NULL);
  if(i == LOCK_PROFILE)
  {	xProfile_->freezeProfile(isSet);	// Sets for PGProfile too.
	return;
  }

  if (isSet)
    // add in this feedback type to 'mode'
    mode_ = mode_ | i;
  else
    // remove this feedback type from 'mode'
    mode_ = mode_ ^ i;
//  cerr << "AvImageViewPanel::switchMode\t" << i << '\t' << mode_ << endl;
  if((i == UPDATE_VALUE) || (i == UPDATE_REGION))
	vItem_.getImageWindow()->updateBox();	// Update box.
}

  // Get start and end slice indexes as set in slice control.
// These should probably reference the accessor instead.
int AvImageViewPanel::getStartSlice()const
{
	if(sliceStep_ != NULL)
		return sliceStep_->getMin();
	else
		return 0;		// 2D;
}

int AvImageViewPanel::getEndSlice()const
{
	if(sliceStep_ != NULL)
		return sliceStep_->getMax();
	else
		return 0;		// 2D;
}

int AvImageViewPanel::numSlices()const
{
	return getEndSlice() - getStartSlice() + 1;
}

// Used to change the displayed slice.
// "which" is any valid slice index for the dataset (not current subrange).
void AvImageViewPanel::setSlice(const int which, const Boolean SR)
{int slice = which;

	if(sliceStep_ == NULL)	// Can ignore if only 1 slice.
		return;

//cerr << "AvImageViewPanel::setSlice: which is " << which << endl;

	if(SR)	// Convert to DS.
		slice += accessor()->getAxisStart(viewAxis());
	// Make sure requested slice is within currently set slice range.
	if((slice < getStartSlice()) || (slice > getEndSlice()))
		return;
#if 0
	// This messes up animate.
	int oPlane = accessor()->getPlane(FALSE);
	if(oPlane == slice)
		return;
#endif

#define REENTERCHECK
#if defined(REENTERCHECK)
// During animations, Accessor::setPlane() gets called which causes the
// accessor callbacks to get called which call AvStepper::setValue()
// which again calls callbacks... It may get fixed, someday.
static Boolean setting = FALSE;
	if(setting)
	{//	cerr << "setSlice called while in setSlice." << endl;
		return;
	}
	setting = TRUE;
#endif
	accessor()->setPlane(slice, FALSE);

#if 0
	if(sliceStep_ != NULL)
		sliceStep_->setValue (slice);
	displaySliceValue();
#endif

	// vItem_ takes care of the window and raster display
	// Since accessor, this is redundant.
	//	vItem_.showSlice (slice);
#if defined(REENTERCHECK)
	setting = FALSE;
#endif
}

// Return current cusor position.
AvIPosition AvImageViewPanel::cursor()const
{ AvIPosition c = ijk_;
	return c;
}

// Return the cursor position in world coordinates.
AvWPosition AvImageViewPanel::worldCursor()const
{
  int length = ijk_.nelements();
  AvWPosition wp(length);
  accessor()->XYZToWorld(ijk_, wp); // Position in world coords.
  return wp;
}

// Update readouts that change when current slice changes.
// Retrieves SR slice, but displays DS slice.
void AvImageViewPanel::displaySliceValue()
{int which = accessor()->getPlane();

    if(sliceWorld_ != NULL)
    {	AvWPosition wpos(ijk_.nelements());
	ijk_(zAxis()) = which;
	accessor()->XYZToWorld(ijk_, wpos, TRUE);
	SetXmTextFieldSetString(sliceWorld_,
			    accessor()->axisValueUnits(2,wpos(2)).chars());
    }
    displayValue();			// Update value readout.
}

void AvImageViewPanel::setRowLabel (char * str, int axis)
{
  regionTable->setRowLabel (str, axis);
}

// x,y are DS
void AvImageViewPanel::setAtIJK (const int x, const int y)
{
  char str[64];
  int indexing = AvImageView::oneIndexing();
  sprintf (str, "(%d,%d)", x+indexing, y+indexing);
  XmTextFieldSetString (dataReadoutLocation, str);
}

// This updates the window location (cursor position) readout (if it exists).
// It is called from AvImageWindow since the cursor location isn't locally
// known.
// x, y are in window coords.
#ifdef SHOWWINDOWLOCATION
void AvImageViewPanel::setAtWindow(const int x, const int y)
{
  // For Debugging use. Shows X cursor position.
  if( dataReadoutWindowLocation != NULL)
  {char str[64];
	sprintf (str, "(%d,%d)", x, y);
	XmTextFieldSetString (dataReadoutWindowLocation, str);
  }
}
#else
void AvImageViewPanel::setAtWindow(const int , const int ){}
#endif

// Display the world coordinate position of the cursor.
void AvImageViewPanel::setAtWorld (const char * a, const char * b)
{
  SetXmTextFieldSetString (dataReadoutWorldI, a);
  SetXmTextFieldSetString (dataReadoutWorldJ, b);
}

void AvImageViewPanel::setAtValue (const char * str)
{
  SetXmTextFieldSetString (dataReadoutValue, str);
}


////////////////////////////////////////////////////////////////
//	Display the region start and end position.
// x, y should be SR coords.
void AvImageViewPanel::setRegionFrom (const int x, const int y)
{
  AvWPosition wp(accessor()->rank());
  int xaxis = xAxis(), yaxis = yAxis();
  if( accessor()->XYZToWorld(x, y, xaxis, wp))
  {  setRegionFrom(accessor()->axisValueUnits(xaxis, wp(xaxis)).chars(),
		 accessor()->axisValueUnits(yaxis, wp(yaxis)).chars());
  }
  else
	cerr	<< "AvImageViewPanel::setRegionFrom: Bad coordinates: "
		<< x << ", " << y << endl;
}

void AvImageViewPanel::setRegionTo (const int x, const int y)
{
  AvWPosition wp(accessor()->rank());
  int xaxis = xAxis(), yaxis = yAxis();
  if( accessor()->XYZToWorld(x, y, xaxis, wp))
  {  setRegionTo(accessor()->axisValueUnits(xaxis, wp(xaxis)).chars(),
		 accessor()->axisValueUnits(yaxis, wp(yaxis)).chars());
  }
  else
	cerr	<< "AvImageViewPanel::setRegionTo: Bad coordinates: "
		<< x << ", " << y << endl;
}

void AvImageViewPanel::setRegionFrom (const char * i, const char * j)
{
  regionTable->setCell (0, 0, i);
  regionTable->setCell (1, 0, j);
}

void AvImageViewPanel::setRegionTo (const char * i, const char * j)
{
  regionTable->setCell (0, 1, i);
  regionTable->setCell (1, 1, j);
}

// Draw the Regions range in IJK coords.
void AvImageViewPanel::updateRegionIJK()
{
	char str[128];
	int indx = AvImageView::oneIndexing();
	sprintf (str, "(%d, %d) to (%d, %d)",
		 regionLeft_+indx, regionBottom_+indx,
		 regionRight_+indx, regionTop_+indx);
	XmTextFieldSetString (regionIJKCoords, str);
}

// Set the values of the region.
// Set the region (mapped DS coords).
void AvImageViewPanel::setRegion(const int x0, const int y0,
				 const int x1, const int y1)
{
	regionLeft_ = x0;
	regionRight_ = x1;
	if(regionLeft_ > regionRight_)
	{int tmp = regionLeft_;
		regionLeft_ = regionRight_;
		regionRight_ = tmp;
	}
	regionBottom_ = y0;
	regionTop_ = y1;
	if(regionBottom_ > regionTop_)
	{int tmp = regionBottom_;
		regionBottom_ = regionTop_;
		regionTop_ = tmp;
	}
	// Update readouts.
	setRegionFrom (regionLeft_, regionBottom_);
	setRegionTo (regionRight_, regionTop_);
	updateRegionIJK();

	// This will pick up any region settings >= 3 dimensions.
	AvIPosition start = accessor()->start();
	AvIPosition End = accessor()->start() + accessor()->shape() -1;
	start(xAxis()) = regionLeft_;
	start(yAxis()) = regionBottom_;
	End(xAxis()) = regionRight_;
	End(yAxis()) = regionTop_;
	if(start.nelements() > 2)
	{	start(zAxis()) = accessor()->getAxisStart(zAxis());
		End(viewAxis()) = start(zAxis()) +
				   accessor()->getAxisLength(zAxis()) - 1;
	}
	AvImageDataItem * idi =
		AvImageView::instance()->search(accessor()->dataset());

	if (idi)
	  idi->lastRegion(start, End);
}

// Get the region (mapped DS coords).
void AvImageViewPanel::getRegion(int &x0, int &y0, int &x1, int &y1)const
{
	x0 = regionLeft_;
	x1 = regionRight_;
	y0 = regionBottom_;
	y1 = regionTop_;
}

////////////////////////////////////////////////////////////////


void AvImageViewPanel::redrawProfile ()
{
//	cout << "AvImageViewPanel::redrawProfile called." << endl;

	if(xProfile_ == NULL)		// NULL IFF no profile to draw.
		return;
	if(xProfile_->isUp())
		xProfile_->redrawProfile();
#ifdef PGPLOT
	else
		pgProfile_->redrawProfile();
#endif
}

void AvImageViewPanel::switchModeCB
(Widget w, AvImageViewPanel * p, XtPointer)
{
  p->switchMode (w);
}


// Make hard copy output.
void AvImageViewPanel::printProfileCB(Widget, AvImageViewPanel * p, XtPointer)
{
	if(p != NULL)
		p->pgProfile_->printProfile(NULL);
}

// Make ASCII dump.
void AvImageViewPanel::dumpProfileCB(Widget, AvImageViewPanel * p, XtPointer)
{
	if(p != NULL)
		p->xProfile_->dumpProfile();
}

void AvImageViewPanel::updateSliceCB
(AvImageViewPanel * p, AvConductor * c, XtPointer)
{
  AvStepper * step = (AvStepper *) c;
  int slice = step->getValue();
  p->setSlice (slice);
}

// Called by stepper when the range (start/end slice) has changed.
void AvImageViewPanel::rangeChangedCB
(AvImageViewPanel * p, AvConductor * , XtPointer)
{
  // Update the start, length of the Z axis part of the sub range.
  AvAccessor *a = p->accessor();
  a->disableNotify();
    int start = p->sliceStep_->getMin();
    a->setAxisStart(p->zAxis(), start);
    a->setAxisLength(p->zAxis(), p->sliceStep_->getMax() - start + 1);
  a->enableNotify();

#if 0
  p->updateProfileXLabels();
  p->generateProfile();
#endif
}

AvImageViewPanel::~AvImageViewPanel ()
{
#ifdef PGPLOT
	if(profileSetup_ != NULL)
	{	delete profileSetup_;
		profileSetup_ = NULL;
	}
	// Disconnect from accessor.
	if(accessor() != NULL)
	{	accessor()->removeCallback(&AvImageViewPanel::accessorCB_,
					   ACCMASK, this, NULL);
	}
	if(pgProfile_ != NULL)
		delete pgProfile_;
#endif
	if(xProfile_ != NULL)
		delete xProfile_;
	if(profileOptions_ != NULL)
	{	profileOptions_->removeCallback(
				&AvImageViewPanel::optionsInfoChangedCB,
				AvProfileOptions::ValueChanged, this, NULL);
		profileOptions_->unref();
	}
}

// Toggle between showing/hiding profile setup window.
void AvImageViewPanel::toggleProfileSetupDisplay()
{
#ifdef PGPLOT
	if(profileSetup()->isUp())
	{	profileSetup()->popdown();
	}
	else
	{	profileSetup()->popup();
	}
#endif
}

int AvImageViewPanel::getShowMenuLength()
{ return infoMenuLen;
}

AvMenuButtonInfo *AvImageViewPanel::getShowMenuInfo()
{
	return infoMenu;
}

// Build the "Show" Menu. If vi is NULL, it will be assumed the entire
// menu is desired (for default settings). Otherwise, buildInside and vi
// will be used to determine which toggles to generate. 
void AvImageViewPanel::buildShowMenu(AvMenu *showMenu, const int tag,
				     AvImageViewItem *vi)
{
AvMenuButtonInfo *m;

  if(vi == NULL)
  { int num = getShowMenuLength();
    m = getShowMenuInfo();
	for(int i=0; i < num; i++, m++)
		showMenu->add(m->name, m->buttonType, tag,
			      NULL, m->buttonData);
  }
  else
  {  m = &infoMenu[REGIONINDEX];
     showMenu->add(m->name, m->buttonType, tag, NULL, m->buttonData);
     // Profiles are only available with multiplane datasets.
     if(vi->numSlices() > 1)
     {
	m = &infoMenu[PROFILEINDEX];
	showMenu->add(m->name, m->buttonType, tag, NULL, m->buttonData);
	m = &infoMenu[INCLUDEDPROFILEINDEX];
	showMenu->add(m->name, m->buttonType, tag, NULL, m->buttonData);
     }
  }
}

// Handle a menu selection. Returns TRUE if item is one of the available
// choices, otherwise, NULL.
// Shows/Hides the region or profile subwindows.
Boolean AvImageViewPanel::processShowMenu(char *item, Widget mb)
{
    if (!strcmp (infoMenu[REGIONINDEX].name, item))
    {	if (XmToggleButtonGetState (mb)) {
		showPart (AvImageViewPanel::REGION);
	}
	else
		hidePart (AvImageViewPanel::REGION);
    }
    else
    if (!strcmp (infoMenu[PROFILEINDEX].name, item))
    {	if (XmToggleButtonGetState (mb))
	  showPart (AvImageViewPanel::PROFILE);
	else
	  hidePart (AvImageViewPanel::PROFILE);
    }
    else
    if (!strcmp (infoMenu[INCLUDEDPROFILEINDEX].name, item))
    {Boolean show = XmToggleButtonGetState(mb);
		xProfile_->showIncluded(show);
#ifdef PGPLOT
		pgProfile_->showIncluded(show);
#endif
		redrawProfile();
    }
    else
	return FALSE;

    return TRUE;
}

// Build our part of the Edit Menu.
#ifdef PGPLOT
void AvImageViewPanel::buildEditMenu(AvMenu *editMenu, const int tag,
				     AvAccessor *accessor)
{
	if(accessor->depth() > 1)
		editMenu->add(PROFILEMENU, PUSH_BUTTON, tag);
}
#else
void AvImageViewPanel::buildEditMenu(AvMenu *, const int )
{

}
#endif

#ifdef PGPLOT
// Called when the info struct has been modified.
void AvImageViewPanel::optionsInfoChangedCB(XtPointer o,
				AvConductor *, XtPointer )
{
AvImageViewPanel *me = (AvImageViewPanel *)o;

	me->redisplay();
}
#endif

// Handle a menu selection. Returns TRUE if item is one of the available
// choices, otherwise, NULL.
// Shows/Hides the region or profile subwindows.
#ifdef PGPLOT
Boolean AvImageViewPanel::processEditMenu(char *item, Widget )
{
	if(!strcmp(PROFILEMENU, item))
	{	toggleProfileSetupDisplay();
		return TRUE;
	}
	else
		return FALSE;
}
#else
Boolean AvImageViewPanel::processEditMenu(char *, Widget )
{
	return FALSE;
}
#endif

// Force a redisplay of the display list.
void AvImageViewPanel::redisplay()
{
#ifdef PGPLOT
	redrawProfile();
#endif
}

// Change whether cursor position world readout is 'raw' or converted.
void AvImageViewPanel::showRawPosition(const Boolean showRaw)
{
	if(showRaw != showRawPosition_)
	{	showRawPosition_ = showRaw;
		updateReadouts(TRUE);
	}
}

void AvImageViewPanel::profileConfigCB( AvProfileConfig *, XtPointer ,
					const int )
{
}

// Called when 'Preview' button is pressed. Toggles between showing the
// X11 and PGPLOT profiles.
void AvImageViewPanel::hcPreviewCB(Widget w, AvImageViewPanel *ivp, XtPointer)
{
#ifdef PGPLOT
	if(XmToggleButtonGetState(w))
	{	ivp->pgProfile_->show(TRUE);
		ivp->xProfile_->show(FALSE);
	}
	else
	{	ivp->xProfile_->show(TRUE);
		ivp->pgProfile_->show(FALSE);
	}
#endif
}

 // Callback to receive accessor notices.
void AvImageViewPanel::accessorCB_(XtPointer dis, AvConductor *c, XtPointer uD)
{ AvImageViewPanel *me = (AvImageViewPanel *) dis;
	me->accessorCB(c, uD);
}

// When the accessor is changed, this is called to respond to the changes.
void AvImageViewPanel::accessorCB(AvConductor *c, XtPointer /* uData */)
{ int r = c->callbackReason();

#if 0
	cerr << "AvImageViewPanel:accessorCB called: ";
	cerr << AvAccessor::reasonToString(r).chars() << endl;
#endif
	if(r & AvAccessor::DISCONNECT)
	{  // Disconnect from accessor
	  if(accessor() != NULL)
	  {	accessor()->removeCallback(&AvImageViewPanel::accessorCB_,
			ACCMASK, this, NULL);
		return;
	  }
	}

	if(r & AvAccessor::CLIP)
	{
//cout << "AvImageViewPanel::accessorCB: clip changed. "  << endl;
	}

	if(r & AvAccessor::REGION)
	{
//cout << "AvImageViewPanel::accessorCB: Region changed. "  << endl;
	}

	if(r & AvAccessor::PLANE)
	{
//		redrawProfile();
#if 0
cout << "AvImageViewPanel::accessorCB: slice changed to " << accessor()->getPlane()
     << endl;
#endif
	if(sliceStep_ != NULL)
		// The slice is stored in "DS" values, not "SR".
		sliceStep_->setValue(accessor()->getPlane(FALSE));
	displaySliceValue();

	}
}
