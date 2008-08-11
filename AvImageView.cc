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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvImageView.cc,v 19.3 2003/10/14 21:41:03 hravlin Exp $
//
// $Log: AvImageView.cc,v $
// Revision 19.3  2003/10/14 21:41:03  hravlin
// Indexing from 1 is now the default if MIRIAD support is built.
//
// Revision 19.2  2003/09/02 21:13:36  hravlin
// Changed from old to new makeDataSet() call. Added request to use new
// file requester when opening data files.
//
// Revision 19.1  2003/08/19 16:58:28  hravlin
// In displayData(filename) remove trailing '/' and delete temporary char
// buffer when done.
//
// Revision 19.0  2003/07/16 05:47:36  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:14  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.3  2002/01/14 17:32:08  hravlin
// Changed fallbacks arg to be const char *[].
//
// Revision 17.2  2002/01/09 22:56:32  hravlin
// Moved an abort() after the following delete.
//
// Revision 17.1  2002/01/03 22:09:06  hravlin
// Changed from calling flushPMCache() to flushRasterCache().
//
// Revision 17.0  2001/11/12 19:42:44  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:50  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:37  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:49:03  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:08:41  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.3  1999/10/04 15:02:21  hravlin
// Reworked how the ColormapEditor was show/hidden.
//
// Revision 13.2  1999/10/01 21:56:59  hravlin
// *** empty log message ***
//
// Revision 13.1  1999/09/15 21:33:10  hravlin
// Support for redrawing images when colormap changes and we're using a
// readonly window.
//
// Revision 13.0  1999/08/10 18:40:30  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:32  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:01  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:03  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.2  1998/03/16 16:44:14  hr
// AvImageView::displayData(filename) was requiring a file to exist.
//
// Revision 9.1  1997/10/14 20:16:41  hr
// Changed calls to "new AvConfigInfo" to use MODULENAME so reads and
// writes are consistent.
//
// Revision 9.0  1997/08/25 21:29:33  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.2  1997/04/24 14:47:02  hr
// Was including AvConfigInfo.h twice.
//
// Revision 8.1  1997/02/25 17:07:15  hr
// In getImageViewList() changed to use AvImageDataItem::View.
//
// Revision 8.0  1997/02/20 03:18:04  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.9  1997/02/06 18:08:24  hr
// Changes to support 1 relative readouts.
//
// Revision 7.8  1997/02/05 17:37:49  hr
// Removed an assignment to an unused widget.
//
// Revision 7.7  1997/01/29 15:11:40  hr
// Added a line to initialize the PGCangas::showID variable to the same
// value as the menu toggle.
//
// Revision 7.6  1997/01/24 20:17:34  hr
// Added menu options to write aipsviewrc files.
//
// Revision 7.5  1997/01/09 18:21:11  hr
// Changed constructor arg list to include parameters to allow retrieving
// of command line X args.
//
// Revision 7.4  1996/12/12 07:59:27  droberts
// Search by accessor changed to match when datasets match.  Rewrote displayFile/replaceData routines.
//
// Revision 1.3  1996/09/10 16:56:46  hr
// Initial changes for integration with AvAccessor & AvDataSet.
//
// Revision 1.2  1996/08/14 18:52:17  hr
// Changes to remove g++ warnings (from bglenden).
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.35  1996/06/18  18:46:12  pixton
//  Copyright Update
//
//  Revision 1.34  1996/03/20  17:35:15  hr
//  Changed to be derived from AvXColorApp.
//
//  Revision 1.33  1996/03/13  16:00:01  hr
//  Moved "Options" submenu to last.
//
//  Revision 1.32  1996/02/09  18:13:28  hr
//  Added Region & Profile to defaults menu.
//
//  Revision 1.31  1996/02/08  18:12:39  hr
//  Reworked menus to support more default options for the Show menu.
//
//  Revision 1.30  1996/01/08  21:00:46  pixton
//  added min/max to replaceData()
//
//  Revision 1.29  1995/11/17  21:02:16  pixton
//  added replaceData(filename, id) for SC '95
//
//  Revision 1.28  1995/11/01  22:52:24  hr
//  Increased size of text widget so overlay panels are more likely to fit,
//  especially with non standard font choices.
//
//  Revision 1.27  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.27  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.26  1995/09/19  16:20:35  hr
//  Added XmNrows to text widget.
//
//  Revision 1.25  1995/09/15  21:47:55  hr
//  Added support for AvVersion
//
//  Revision 1.24  1995/09/12  17:36:48  hr
//  Upped Alpha 4 release date to 9/95
//
//  Revision 1.23  1995/07/13  21:52:04  hr
//  Fixed getProcessSize for SGI.
//
//  Revision 1.22  1995/07/13  21:25:05  hr
//  Added (optional) code to allow printout of heap/process size on SUNS & SGI.
//
//  Revision 1.21  1995/06/28  21:20:41  hr
//  Changed release number to alpha 4.
//
//  Revision 1.20  1995/06/16  16:31:00  hr
//  Set title of colormap editor to "Colormap Editor".
//
//  Revision 1.19  1995/06/12  15:20:33  hr
//  Broke AvImageView.cc 1.18 into AvImageView, AvImageViewItem and
//  AvImageDataItem.
//
//  Revision 1.18  1995/06/09  19:41:33  hr
//  Moved creation of the AvImageViewPanel to inside AvImageWindow.
//  Added menu entries to allow user to decide whether panel should be
//  built inside or outside the image window.
//
//  Revision 1.17  1995/05/18  20:55:01  hr
//  Bumped release number to 3.
//
//  Revision 1.16  1995/05/17  14:55:49  hr
//  Reinserted call to dataModified in updateView so GLISH updates should
//  work again. Copied relevant parts of updateView to showSlice so it
//  doesn't have to call updateView (so animations continue to work).
//  Moved call to setCurrentPanel to before the delete in
//  AvImageView::closeSession.
//
//  Revision 1.15  1995/05/12  21:20:09  hr
//  Changes to data clip min/max are propagated to profile display.
//
//  Revision 1.14  1995/05/12  16:02:07  hr
//  Removed call to dataModified in update since it was disabling pm caching.
//
//  Revision 1.13  1995/05/12  15:49:18  hr
//  Added "release alpha 2.0" text to startup message.
//
//  Revision 1.12  1995/05/12  15:12:45  pixton
//  no changes - needed to track down mem leaks.
//
//  Revision 1.11  1995/05/05  17:07:15  pixton
//  Bug Fix - Close data with 0 or 1 data items up caused seg fault
//  Bug Fix - Bad file names now rejected by displayData(fName)
//
//  Revision 1.10  1995/05/05  14:51:12  hr
//  Edits to WorldToIJK routines.
//
//  Revision 1.9  1995/05/04  18:04:47  pixton
//  comment mods only.
//
//  Revision 1.8  1995/05/03  15:18:45  pixton
//  Animator panel/data selection bug (I introduced in recent merge) fixed.
//
//  Revision 1.7  1995/05/02  20:21:31  pixton
//  Introduction of templated lists to deal with data management.
//  Added flags for removing GLISH constructs from compilation if
//  desired (See the Makefile for how-to).
//  Each new data tagged with unique ID.
//  Rewrote checkViewItem and getImageViewList to use AvList iterators.
//  No more limits on number of data items.
//  Added lastRegion, lastLocation, lastValue to the data item so aipsView
//  could respond in an intelligent way to "return a region from this data set"
//  without assuming anything about how the data was being viewed.
//
//  Added AvImageDataItem constructor which accepts an AvGriddedData instead of
//  a file name.
//
//  Modified accessory functions that dealt with the previous list rep.
//  Removed unused functions pertaining to old rep.
//
//  Revision 1.6  1995/05/02  19:08:59  hr
//  Added functions to convert between world and IJK coordinates.
//
//  Revision 1.5  1995/04/25  16:49:58  hr
//  Added redraw flags to fitRasterToWindow and fitWindowToRaster.
//
//  Revision 1.4  1995/04/19  14:28:22  hr
//  Changes to move animation controls to inside imageView window.
//  Added AvAnimPanel. Removed AvAnimCtlr.
//
//  Revision 1.3  1995/04/14  19:27:32  hr
//  changed AvCLUImage::setSlice to include sliceIndex.
//
//  Revision 1.2  1995/04/10  20:55:33  hr
//  added a number of functions to return pointers to data items.
//  added AvImageView::getImageViewList and AvImageView::instance
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#include <unistd.h>
#include <Xm/Form.h>
#include <Xm/Text.h>
#include <Xm/ToggleB.h>

#include "AvAnimPanel.h"
#include "AvColormapEditor.h"
#include "AvImageDataItem.h"
#include "AvImageDataPanel.h"
#include "AvImageView.h"
#include "AvImageViewPanel.h"
#include "AvImageViewItem.h"
#include "AvImageWindow.h"
#include "AvVersion.h"
#include "AvCLUImageSetter.h"
#include "AvPGCanvas.h"			// To access showID().
#include "AvConfigInfo.h"
#include "AvXPalette.h"
#include "AvXCanvases.h"
#include "AvMakeDataSet.h"

//AvDataSet *makeDataSet(const char *filename, AvString &tries);

AvImageView * AvImageView::instance_ = NULL;

AvImageDataPanel * AvImageView::currentDataPanel_ = NULL;
AvUserComp	*AvImageView::currentPanel_ = NULL;

// Whether imageWindows should build data panel inside or outside.
Boolean AvImageView::buildInside_;
static const char *BUILDINSIDE = "Info Inside";
static const char *BUILDOUTSIDE = "Info Outside";
static const char *SHOWID = "PGPLOT ID String";
static const char *ONERELATIVEMENU = "One Relative Indexing";
static const char *ONERELATIVENAME = "oneRelativeIndexing";
static const char *MODULENAME = "global";	// Be consistent throughout.
int AvImageView::oneIndexing_ = 0;


#ifdef TCL
static const char *SAVEINFO = "Update ~/.aipsviewrc";
//static const char *SAVEALLINFO = "Create .aipsviewrc";
static const char *SAVEALLINFO = "Create aipsviewrc.list";
static const char *SAVEALLFILE = "aipsviewrc.list";
#endif

AvImageView::AvImageView
(const char * appClass, int & argc, String * argv, const char * fallbacks[])
: AvXColorApp (appClass, argc, argv, fallbacks)
{
  init ();
}


AvImageView::AvImageView(const char * appClass,
			 XrmOptionDescRec *opt, Cardinal noptions,
			 int &argc, String *argv,
			 const char * fallbacks[],
			 ArgList args, Cardinal nargs,
			 // User defined struct to hold vars.
			 XtPointer varStruct,
			 XtResource *resources, const int nResources)
  : AvXColorApp (appClass, opt, noptions, argc, argv, fallbacks, args, nargs,
		 varStruct, resources, nResources)
{
  init ();
}


void AvImageView::buildMenus ()
{
  AvMenu * fileMenu = new AvMenu (8);
  fileMenu->add ("Open Data...", PUSH_BUTTON, SV_IMAGE_FILE);
  fileMenu->add ("Close Data", PUSH_BUTTON, SV_IMAGE_FILE);
#ifdef TCL
  fileMenu->add ("", SEPARATOR);
  fileMenu->add (SAVEINFO, PUSH_BUTTON, SV_IMAGE_FILE);
  fileMenu->add (SAVEALLINFO, PUSH_BUTTON, SV_IMAGE_FILE);
#endif
  fileMenu->add ("", SEPARATOR);
  fileMenu->add ("Quit", PUSH_BUTTON, SV_QUIT);

  // (Use same module name a
  AvConfigInfo *info = new AvConfigInfo(NULL, MODULENAME);
  AvMenu *editMenu = new AvMenu(2);
  // Whether PGPLOT hardcopy should have the PGPLOT ID string in the lower
  // right corner.
  int showID;
  info->getBoolean("showID", showID, TRUE);
  editMenu->add(SHOWID, TOGGLE_BUTTON, SV_SETUP, NULL, showID);
  AvPGCanvas::showID(showID);

#if defined(AIPSPLUSPLUS) || defined(MIRIAD)
#define DEFINDEXING	1
#else
#define DEFINDEXING	0
#endif

  info->getBoolean(ONERELATIVENAME, oneIndexing_, DEFINDEXING);
  editMenu->add(ONERELATIVEMENU, TOGGLE_BUTTON, SV_SETUP, NULL, oneIndexing_);

  info->unref();
  info = NULL;
  AvMenu * optionMenu = new AvMenu (4);
  optionMenu->add ("Colormap Editor", PUSH_BUTTON, SV_IV_OPTION);
  optionMenu->add ("Animate", PUSH_BUTTON, SV_IV_OPTION);

  // The defaults menu is used to set global defaults that will be used
  // when building later windows.
  int defMenuLen = AvCLUImageSetter::getShowMenuLength() +
		   AvImageViewPanel::getShowMenuLength() + 4;
  AvMenu *defaultsMenu = new AvMenu(defMenuLen);
  // There are two cases since I don't know how to set the toggle from here.
  if(buildInside_)
//    defaultsMenu->add("Build Outside", TOGGLE_BUTTON, SV_IV_OPTION);
    defaultsMenu->add(BUILDOUTSIDE, TOGGLE_BUTTON, SV_IV_OPTION);
  else
    defaultsMenu->add(BUILDINSIDE, TOGGLE_BUTTON, SV_IV_OPTION);
//    defaultsMenu->add("Build Inside", TOGGLE_BUTTON, SV_IV_OPTION);
  defaultsMenu->add("", SEPARATOR);
  AvImageViewPanel::buildShowMenu(defaultsMenu, SV_IV_OPTION);
  defaultsMenu->add("", SEPARATOR);
  AvCLUImageSetter::buildShowMenu(defaultsMenu, SV_IV_OPTION);
  optionMenu->add("Defaults:(Show)", SUBMENU_BUTTON, 0, defaultsMenu);

#ifdef SHOWHEAP
	if(showHeap())
		optionMenu->add("Heap Size", PUSH_BUTTON, SV_IV_OPTION);
#endif

  AvMenu * dataMenu = new AvMenu (0);

  AvMenu * mainMenu = new AvMenu (4);
  mainMenu->add ("File", SUBMENU_BUTTON, 0, fileMenu);
  mainMenu->add ("Edit", SUBMENU_BUTTON, 0, editMenu);
  mainMenu->add ("Session", SUBMENU_BUTTON, 0, dataMenu);
  mainMenu->add ("Options", SUBMENU_BUTTON, 0, optionMenu);

  makeMainMenu (mainMenu);

  dataMenuWidget_ = dataMenu->parentWidget();
}

void AvImageView::init ()
{
  glishActive_ = 0;

  buildInside_ = !(strcmp(getStringResource("InfoPanel"), "OUTSIDE") == 0);

#ifdef GLISH
  glishRequestList_ = NULL;
  glishActive_ = 1;
#endif

  instance_ = this;

  currentDataItem_ = NULL;

  XtVaSetValues (parentWidget_,
		 XmNallowShellResize, TRUE,
		 XmNtitle, "av",
		 XmNcolormap, AvXColorApp::colormap(),	// Ignored??
		 NULL);

  strcpy (dataDirectory_, getStringResource ("DataDirectory"));

  buildMenus ();

  // since we're derived from AvApp, baseWidget_ is an XmMainWindow...
  workArea_ = XtVaCreateManagedWidget
    ("mainWindow", xmFormWidgetClass, baseWidget_,
     NULL);

  XtVaCreateManagedWidget
    ("instructions", xmTextWidgetClass, workArea_,
     XmNeditable, FALSE,
     XmNcursorPositionVisible, FALSE,
     XmNeditMode, XmMULTI_LINE_EDIT,
     XmNtopOffset, 20,
     XmNbottomOffset, 20,
     XmNleftOffset, 20,
     XmNrightOffset, 20,
     XmNleftAttachment, XmATTACH_FORM,
     XmNrightAttachment, XmATTACH_FORM,
     XmNtopAttachment, XmATTACH_FORM,
     XmNbottomAttachment, XmATTACH_FORM,
//     XmNheight, 280,
#if 0
     XmNwidth, 382,
     XmNrows, 14,
#else
     XmNwidth, 400,	// Make big enough so the panel that overlays
//     XmNrows, 16,	// this is more likely to fit in window.
     XmNrows, 20,	// this is more likely to fit in window.
#endif
     XmNscrollVertical, TRUE,
     XmNwordWrap, TRUE,
     XmNvalue, AvVersion::memo(),
     NULL);

  XtVaSetValues (baseWidget_, XmNworkWindow, workArea_, NULL);

  XtManageChild (baseWidget_);
  XtRealizeWidget (parentWidget_);

  // make the colormap editor now.  In particular, this will
  // insure that the X system colormap is defined 
  cmapEditor_ = new AvColormapEditor (parentWidget_, FALSE);
  cmapEditor_->setTitle("Colormap Editor");
  AvPalette *pal = AvXColorApp::palette();
  if( (pal != NULL) && pal->readonly())
	  cmapEditor_->addCallback( &AvImageView::colorMapChangedCB,
		    AvColormapEditor::MAP_MODIFIED | AvColormapEditor::MAP_EDITED,
		    NULL, (XtPointer)this);
  animPanel_ = NULL;
  currentPanel_ = NULL;
}

void AvImageView::processMenu (char * buttonName, int choice, Widget mb)
{
AvMenuButtonInfo *info;

  switch (choice) {
  case SET_CURRENT_DATA:
    AvImageDataPanel * p;
    XtVaGetValues (mb, XmNuserData, &p, NULL);
    setCurrentDataPanel (p);
    break;
  case SV_IV_OPTION:
    if(!strcmp(buttonName, "Animate"))
    {  if(animPanel_ == NULL)	// Display animation controls.
	  animPanel_ = new AvAnimPanel(workArea_, TRUE);
	setCurrentPanel(animPanel_);
    }
    else		// Choose where to put image window info panel.
//    if(strcmp(buttonName, "Build Outside") == 0)
    if(strcmp(buttonName, BUILDOUTSIDE) == 0)
    	buildInside(!XmToggleButtonGetState (mb));
    else		// Only one of Inside or Outside will exist.
    if(strcmp(buttonName, BUILDINSIDE ) == 0)
    	buildInside(XmToggleButtonGetState (mb));
    else	// Decide defaults for what to show in image window.
    if( (info = AvMenu::findMenuInfo(buttonName,
				AvCLUImageSetter::getShowMenuInfo(),
				AvCLUImageSetter::getShowMenuLength()))
	!= NULL)
    {	info->buttonData = XmToggleButtonGetState(mb) ? 1 : 0;
    }
    else
    if( (info = AvMenu::findMenuInfo(buttonName,
				AvImageViewPanel::getShowMenuInfo(),
			     	AvImageViewPanel::getShowMenuLength()))
	!= NULL)
    {	info->buttonData = XmToggleButtonGetState(mb) ? 1 : 0;
    }
#ifdef SHOWHEAP
    else
    if(strcmp(buttonName, "Heap Size") == 0)
	printHeap();
#endif
    else
    {  // (un)display colormap editor.
	if(cmapEditor_)
	{	if(cmapEditor_->isHidden())
			cmapEditor_->show();
		else
			cmapEditor_->hide();
	}
	else
	{	cmapEditor_ = new AvColormapEditor (parentWidget_, FALSE);
		cmapEditor_->show();
	}
    }
    break;
  case SV_IMAGE_FILE:
    if (!strcmp (buttonName, "Open Data...")) 
      {
	getFileNameFromUser ((AvProc) &AvImageView::newFileNameCB,
			     "Select an image", dataDirectory_,
			     AvApp::USEDATADIRS);
      }
    else
      if (!strcmp (buttonName, "Close Data")) 
	{
	  if (currentDataPanel_)
	    closeSession(currentDataPanel_->item());
	}
#ifdef TCL
	else
	if (!strcmp (SAVEINFO, buttonName))
	{ // Update configuration information for .aipsviewrc.
		AvConfigInfo::writeOptions(AvConfigInfo::defaultName(),
					   NULL, FALSE);
	}
	else
	if (!strcmp (SAVEALLINFO, buttonName))
	{  // Save all configuration information into ./aipsviewrc.
//		AvConfigInfo::writeOptions(AvConfigInfo::defaultName(),
		cout << "Writing " << SAVEALLFILE << endl;
		AvConfigInfo::writeOptions(SAVEALLFILE, NULL, TRUE);
	}
#endif
    break;
  case SV_SETUP:
#ifdef PGPLOT
	if(strcmp(SHOWID, buttonName) == 0)
	{  AvConfigInfo *cnfg = new AvConfigInfo(NULL, MODULENAME);
		if(XmToggleButtonGetState(mb))
		{	AvPGCanvas::showID(1);
			cnfg->setBoolean("showID", TRUE);
		}
		else
		{	AvPGCanvas::showID(0);
			cnfg->setBoolean("showID", FALSE);
		}
		cnfg->unref();
	}
	else
#endif
	if(strcmp(ONERELATIVEMENU, buttonName) == 0)
	{  AvConfigInfo *cnfg = new AvConfigInfo(NULL, MODULENAME);
		if(XmToggleButtonGetState(mb))
		{	oneIndexing_ = 1;
			cnfg->setBoolean(ONERELATIVENAME, TRUE);
		}
		else
		{	oneIndexing_ = 0;
			cnfg->setBoolean(ONERELATIVENAME, FALSE);
		}
		cnfg->unref();
		int listlen;
		AvImageViewItem **list= getImageViewList(listlen, ALL);
		for(int i=0; i< listlen; i++)
		{AvImageWindow *win = list[i]->getImageWindow();
		 AvImageViewPanel *ivp = win->viewPanel();
			ivp->updateReadouts(TRUE);
			list[i]->dataItem()->setIndexing(oneIndexing_);
		}
		delete [] list;
		if(animPanel_ != NULL)
			animPanel_->updateFrameSlider();
	}
	break;
  default:
    AvApp::processMenu (buttonName, choice, mb);
  }
}

//
//  Return the Nth (0-based) session
//
typedef AvListIndexIter<AvImageDataItem *> II_IDI;
AvImageDataItem * AvImageView::nthSession(int n) const
{
  if (n < 0 || n >= sessionList_.length()) 
    return NULL;
  else
    {
      AvListIndexIter<AvImageDataItem *> it(&sessionList_);
      it.gotoNth(n);
      return (it.done()) ? (AvImageDataItem*)NULL : it.peek();
    }
}

//  closeSession
//
//  Shutdown the session.  returns closed id or -1
//
int AvImageView::closeSession(int id)       { return closeSession(search(id));   }
int AvImageView::closeSession(char * name)  { return closeSession(search(name)); }
int AvImageView::closeSession(AvImageDataItem * idi)
{
  if (idi)
    {
      AvImageDataItem * rmItem = sessionList_.remove(idi);
      if (rmItem) 
	{
	  setCurrentDataPanel(0);
	  int closeID = rmItem->id();
	  delete rmItem;
	  return closeID;
	}
    }
  return -1;
}

//  search by id
//
//  Return the IDI ptr to the (1st) data item with this id, else NULL
//
//  walk() is an AvList function that walks a list until the result of applying
//  the matching function to the list item is non-zero.
//
int AvImageView::matchIdFunc(AvImageDataItem * test, void * id)
{ return test->id() == *((int *) id); }

AvImageDataItem * AvImageView::search (int id)
{ 
  AvImageDataItem** retval = sessionList_.walk(matchIdFunc, &id);
  return (retval) ? *retval : (AvImageDataItem*)NULL;
}

//  search by name
//
//  Return the IDI ptr to the (1st) data with this name or NULL
//
int AvImageView::matchNameFunc(AvImageDataItem * test, void * name)
{ return (!strcmp (test->dataset()->fileName(), (char *) name)); }

AvImageDataItem * AvImageView::search (char * fname)
{ 
  AvImageDataItem** retval = sessionList_.walk(matchNameFunc, fname); 
  return (retval) ? *retval : (AvImageDataItem*)NULL;
}

//  search by dataset pointer
//
//  Return the IDI ptr to the (1st) data with this dataset pointer or NULL
//
int AvImageView::matchDPtrFunc(AvImageDataItem * test, void * dptr)
{
  return test->dataset() == (AvDataSet *) dptr;
}

AvImageDataItem * AvImageView::search(AvDataSet * data)
{
  AvImageDataItem** retval = sessionList_.walk(matchDPtrFunc, data);
  return (retval) ? *retval : (AvImageDataItem*)NULL;
}

//  Return the number of views that have a particular axis
//
int AvImageDataItem::nSubviews(int axis)
{
  int count = 0;
  AvListIter<AvImageViewItem *> it(&subViews_);
  while (!it.done()) if (it.get()->viewAxis() == axis) count++;
  return count;
}

//
// Show the data and return an id for it.
//
int AvImageView::displayData (AvDataSet * ds)
{
  AvImageDataItem * newDataItem = new AvImageDataItem (ds, workArea_);

  if (newDataItem->ok())
    registerDataItem (newDataItem);
  else
    {
      delete newDataItem;
      abort();
    }
  return newDataItem ? newDataItem->id() : -1;
}

//  register the "data item" with the user interface.  Registration
//  makes this "data item" current.
//
void AvImageView::registerDataItem (AvImageDataItem * idi)
{
  currentDataItem_ = idi;
  setCurrentDataPanel (idi->dataPanel());

  AvMenuItem * mi = new AvMenuItem(idi->dataset()->fileName(),
					PUSH_BUTTON, SET_CURRENT_DATA, NULL);

  Widget b = addMenuButton(dataMenuWidget_, mi);
  idi->dataListEntry_ = b;
  
  XtVaSetValues (b, XmNuserData, idi->dataPanel(), NULL);

  XtAddCallback (b, XmNactivateCallback,
		 (XtCallbackProc) &AvImageView::processMenuCB,
		 (XtPointer) mi);
  
  sessionList_.add(idi);

}

int AvImageView::addSession(AvImageDataItem * item)
{ sessionList_.add(item); return item->id(); }
int AvImageView::addSession(AvImageDataItem& item)   
{ return addSession(&item); }


//
//  Valid id or -1 returned
//
int AvImageView::displayData (const char * fName)
{ int flen;

  if (!fName) return -1;

  flen = strlen(fName);
  if(flen <- 0)
    return -1;

  // if we already have data from this file name, then update
  char * tName = new char[flen+1];
  strcpy(tName, fName);
  // Strip off trailing '/'.
  if (tName[flen-1] == '/') tName[--flen] = 0;
  if(flen <= 0)		// Name was just '/'.
  {  delete [] tName;
     return -1;
  }

  // strip path for match/search
  AvString shortname = tName;
  shortname.gsub(AvRegex(".*/"), "");

  AvImageDataItem * item = search ((char *) shortname.chars());

  if (item) 
    {
      // data with this name already exists, so try to call
      // replace on it
      AvDataSet * ds = AvMakeDataSet::makeDataSet(tName);
      if (ds)
	{
	  ds->ref();
	  int rid = item->replaceData(ds);
	  ds->unref();
	  if (rid == -1)
	    {
	      // can't replace, so close session, the fall through
	      // to loader
	      cerr << "AipsView: replace failed, closing/reopening\n";
	      closeSession(item);
	    }
	  else
	    {
	      // replace successful, return new id
		delete [] tName;
	      return rid;
	    }
	}
      else	// Could not open.
	{ AvString errorString = AvMakeDataSet::errorString();

#if 0
	cerr << "AipsView: Warning - file '" << tName
		     << " could not open." << endl;
		cout << tries << endl;
#else
		cerr << errorString << endl;
#endif
		delete [] tName;
		return -1;
	}
    }

  // if we get here, we're going to open a new dataset for the file
  item = new AvImageDataItem ((char *) tName, workArea_);

  if (item->ok()) 
    registerDataItem (item);
  else
    {
      delete item;  // delete if constructor failed 
      item = NULL;
    }

  delete [] tName;
  return item ? item->id() : -1;
}

//
//  
//

int AvImageView::replaceData(const char * filename, int id)
{
  AvImageDataItem * idi = search(id);
  if (!idi) 
    {
      cerr << "AipsView2: Warning - bad id\n";
      return -1;
    }
  
  if (access(filename, R_OK))
    {
      cerr << "AipsView2: Warning - file '" << filename << "' not found\n";
      return -1;
    }
  
  AvDataSet * ds = AvMakeDataSet::makeDataSet(filename);
  
  if (!ds)
    {
#if 0
      cerr << "AipsView2: Warning - couldn't make dataset from file '" << 
	filename << "'.\n";
#else
      cerr << AvMakeDataSet::errorString() << endl;
#endif
      return -1;
    }

  ds->ref();
  int rid = idi->replaceData(ds);

  ds->unref();

  if (rid > -1)
    {
      idi->dataModified();
      idi->updateViews();
    }

  return rid;
}
  
void AvImageView::setCurrentDataPanel (AvImageDataPanel * p)
{
  if (currentDataPanel_) 
    {
      currentDataPanel_->hide();
    }
  currentDataPanel_ = p;
  setCurrentPanel(p);
  if (currentDataPanel_) currentDataPanel_->show();
}

void AvImageView::setCurrentPanel (AvUserComp * p)
{
  if (currentPanel_) { currentPanel_->hide(); }
  currentPanel_ = p;
  if (currentPanel_) { currentPanel_->show(); }
}


void AvImageView::newFileNameCB (AvImageView * app, XtPointer d1)
{
  app->displayData ((char *) d1);
  // we'll assume that the user wants to use this directory name as 
  // the data source, so set "dataDirectory" to this path...
  app->updatePathName (app->dataDirectory_, (char *) d1);
}

//  This function was rewritten (r1.7 to check only a single view item.  
//  The success of the test is passed as the return value instead of by 
//  side effect via private variables.
int AvImageView::checkViewItem(AvImageViewItem * vi, const ItemSelection which)
{
  int retval = 0;

  if (vi)
    {
      AvImageWindow *iw = vi->getImageWindow();
      Boolean included = (iw->include());
      
      if ((which == ALL) || ((which & INCLUDED) && included) ||
	  ((which&NOTINCLUDED) && !included))
	retval = 1;
    }
  return retval;
}

//  Produce an array of ImageViewItems according to the matching parameter "which".
//  nitems is set to the number of items returned, up to 128.  The caller cannot
//  assume the array space is only nitems in length.  This was rewritten
//  to use iterators in r1.7.
//
AvImageViewItem **AvImageView::getImageViewList(int &nitems,
						const ItemSelection which)
{
  const int MAXRETURNLEN = 128;      // don't pollute global name space
  AvImageViewItem ** list = new AvImageViewItem *[MAXRETURNLEN];
  int ndx = 0;

  // for each imageDataItem, fetch all imageViewItem pointers and stash
  // them in the above array if they pass the inclusion test "checkViewItem".
  AvListIter<AvImageDataItem *> itd(&sessionList_);
  while (!itd.done())
    {
      AvImageDataItem * idi = itd.get();
#if 0
      if (checkViewItem(idi->view(0), which)) list[ndx++] = idi->view(0);
      if (ndx >= MAXRETURNLEN) break;
      if (checkViewItem(idi->view(1), which)) list[ndx++] = idi->view(1);
      if (ndx >= MAXRETURNLEN) break;
      if (checkViewItem(idi->view(2), which)) list[ndx++] = idi->view(2);
      if (ndx >= MAXRETURNLEN) break;
#else
      AvImageDataItem::View *view;
      int k=0;
      while( (view = idi->getView(k++)) != NULL)
	if (checkViewItem(view->ivi, which)) list[ndx++] = view->ivi;
#endif

      AvListIter<AvImageViewItem *> itv(idi->subViews());
      while (!itv.done())
	{
	  AvImageViewItem * ivi = itv.get();
	  if (checkViewItem(ivi, which)) list[ndx++] = ivi;
	  if (ndx >= MAXRETURNLEN) break;
	}
      if (ndx >= MAXRETURNLEN) break;
    }
  
  nitems = ndx;  // this is a RETURN PARAMETER!
  return list;
}

Boolean AvImageView::buildInside() {return buildInside_;}

// Flush all pixmap caches, then redisplay images.
// Called when the colormap has changed and we're displaying on a
// ReadOnly window.
void AvImageView::redisplayImages()
{ int nitems;
  AvImageViewItem **list = getImageViewList(nitems, ALL);

	for(int i=0; i< nitems; i++)
	{ AvImageViewItem *vi = list[i];
	  AvImageWindow *iw = vi->getImageWindow();

		iw->canvas()->flushRasterCache();
		iw->showImage();
	}
	delete [] list;
}

//void AvImageView::colorMapChangedCB(XtPointer , AvConductor *c, XtPointer ud)
void AvImageView::colorMapChangedCB(XtPointer , AvConductor *, XtPointer ud)
{
//AvColormapEditor *ce = (AvColormapEditor *) c;
//AvCECmap *map = ce->getLastColormap();
AvImageView *iv = (AvImageView *)ud;

//cerr << "colorMapChangedCB" << endl;

		iv->redisplayImages();
}
