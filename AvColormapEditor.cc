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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvColormapEditor.cc,v 19.0 2003/07/16 05:46:52 aips2adm Exp $
//
// $Log: AvColormapEditor.cc,v $
// Revision 19.0  2003/07/16 05:46:52  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:37  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.5  2002/01/30 22:33:56  hravlin
// Initialized some variables g++ thought might get used unset.
//
// Revision 17.4  2002/01/17 15:53:34  hravlin
// Make sure all of black_ is initialized.
//
// Revision 17.3  2002/01/14 16:52:12  hravlin
// Changed fallbacks arg to be const char *[].
//
// Revision 17.2  2002/01/09 20:23:06  hravlin
// Changed an assignment to hsv_ to 1 from -1.
// Add '&' to conditional checks around line 2130.
//
// Revision 17.1  2002/01/07 22:27:12  hravlin
// Removed check for __P.
//
// Revision 17.0  2001/11/12 19:42:06  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:14  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:05  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 18:12:15  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:07:55  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.5  1999/10/01 21:56:56  hravlin
// *** empty log message ***
//
// Revision 13.4  1999/09/28 19:09:04  hravlin
// Added check to prevent indirect recursion during TrueColor map updates.
//
// Revision 13.3  1999/09/15 22:09:30  hravlin
// Fixed a typo from previous checkin.
//
// Revision 13.2  1999/09/15 21:39:14  hravlin
// Added support for callback notification when colormap is edited and
// using global X values rather than root's.
// Also changes for new colormap handler.
//
// Revision 13.1  1999/08/25 19:57:06  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:39:38  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:01  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:53  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:34  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.3  1998/06/09 21:29:24  hr
// Fixed several type conversion warnings.
//
// Revision 9.2  1998/05/04 20:31:10  hr
// Removed an unused variable (dmy).
//
// Revision 9.1  1998/05/01 21:30:42  hr
// First attempt to deal with large colormaps - limit max width to 256.
// Use aipsview's colormap rather than root's.
//
// Revision 9.0  1997/08/25 21:25:37  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:38  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/02/05 17:32:58  hr
// The __P, g++ and math.h problem.
//
// Revision 7.2  1996/12/12 05:54:56  droberts
// Final update from monet archive.
//
// Revision 1.3  1996/08/19 20:04:04  pixton
// removed unused variables - now g++ -Wall warning free
//
// Revision 1.2  1996/08/14  17:29:00  hr
// Changes to remove g++ warnings (from bglenden).
//
// Revision 1.1  1996/07/11  21:31:21  pixton
// Automated Checkin
//
//  Revision 1.16  1996/06/18  18:45:53  pixton
//  Copyright Update
//
//  Revision 1.15  1996/04/16  16:24:28  hr
//  Some compilers can't handle initializations like:
//  	char * buffer[] = { "A","B","C" };
//  Changing to "static char *..." works.
//
//  Revision 1.14  1996/03/29  17:28:33  hr
//  palette_ needed to be ref'ed and unref'ed.
//
//  Revision 1.13  1996/03/14  18:13:12  baker
//  Updated to derive from AvXColorApp rather than AvApp.
//
//  Revision 1.12  1996/03/13  16:00:01  hr
//  Moved "Options" submenu to last.
//
//  Revision 1.11  1996/03/05  17:43:03  hr
//  The label gadget for "Palette:" was using the wrong XmString creation
//  routine and throwing away the resultant XmString.
//
//  Revision 1.10  1995/10/27  19:10:01  hr
//  Added a bunch of resources to widgets using our private colormap to try
//  and avoid warning about not being able to allocate a color.
//
//  Revision 1.9  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.9  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.8  1995/08/11  20:46:44  baker
//  Added constructor to take fallback resources.
//
//  Revision 1.7  1995/08/03  16:43:51  hr
//  Changes to allow multiple user named palettes and to notify callbacks
//  when changes are made.
//
//  Revision 1.6  1995/07/18  17:27:17  hr
//  'Fixed' g++ warnings. Mostly don't do "for(int i...)".
//
//  Revision 1.5  1995/07/17  21:32:53  hr
//  Added destructor.
//
//  Revision 1.4  1995/05/03  21:59:00  pixton
//  fixed some problems with the colormap editor initialization.
//  fixed bug preventing Pseudo-Contour from generating its RGB
//  version after maps are tied together (as in greyscale-RGB mode)
//
//  Revision 1.3  1995/04/17  17:38:10  pixton
//  colormap now initialized to the "Rainbow" palette.
//
//  Revision 1.2  1995/03/21  14:35:15  pixton
//  Corrected map ruler line color.  Previously used one of the colors
//  in the modifiable map.
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------


//
//                       ---------------------------
//                      |                           |
//                      |           "Map"           |
//                      |---------------------------|
//                      |                           |    
//                      |        "funcEditor 0"     |    
//                      |                           |    
//                      |---------------------------|
//                      |                           |    
//                      |        "funcEditor 1"     |    
//                      |                           |    
//                      |---------------------------|
//                      |                           |    
//                      |        "funcEditor 2"     |    
//                      |                           |    
//                      |---------------------------|
//                      |                           |    
//                      |        "funcEditor 3"     |    
//                      |                           |    
//                      |---------------------------|
//

#include <iostream.h>
#include <stdio.h>
//#include <malloc.h>
//#if defined(__GNUC__) && defined(__P) && !defined(__linux__)
//#undef __P
//#endif
#include <math.h>
#include <stdlib.h>
#include <Xm/Xm.h>
#include <Xm/BulletinB.h>
#include <Xm/DrawingA.h>
#include <Xm/FileSB.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Scale.h>
#include <Xm/SelectioB.h>
#include <Xm/Separator.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>
#include <X11/cursorfont.h>
#include <X11/Xlib.h>
#include <X11/IntrinsicP.h>

#include <AvColormap.h>
#include <AvColormapEditor.h>
#include <AvFunctionEditor.h>
#include <AvPointList2D.h>
#include <AvXPalette.h>
#include <AvApp.h>

#include <string.h>
#include <X11/keysym.h>		// After AvColormapEditor.h

/*
String AvColormapEditor::fallback_resources[] =
{
  "*workArea.panel.mapMgr.map.height:  60",
  "*workArea.panel.graph1Mgr.height:  60",
  "*workArea.panel.graph2Mgr.height:  60",
  "*workArea.panel.graph3Mgr.height:  60",
  "*workArea.panel.graph4Mgr.height:  60",
  NULL
};
*/


#define MAP_HEIGHT 60

const char * AvColormapEditor::rgbGraphNames[] = { "R", "G", "B" };
const char * AvColormapEditor::hsvGraphNames[] = { "H", "S", "V" };


AvColormapEditor::AvColormapEditor
(char * appClass, int & argc, String * argv)
: AvXColorApp (appClass, argc, argv, NULL)
{
  init (TRUE);
  initMap();
  show ();
  XtAppMainLoop (appContext_);
}


AvColormapEditor::AvColormapEditor
(char * appClass, int & argc, String * argv, const char * fallbacks[])
: AvXColorApp (appClass, argc, argv, fallbacks)
{
  init (TRUE);
  initMap();
  show ();
  XtAppMainLoop (appContext_);
}

AvColormapEditor::AvColormapEditor (Widget parent, int buildInsideParent)
: AvXColorApp (parent, buildInsideParent)
{
  init (FALSE);
  initMap();
}


#define CBTEST_
#ifdef CBTEST
// Routines to help test multiple colormaps.
char *cbreason(const int n)
{static char buf[132];

	buf[0] = '\0';
	if(n & AvConductor::Destroyed)
		strcat(buf, "Destroyed | ");
	if(n & AvColormap::COMPONENT_CHANGED)
		strcat(buf, "COMPONENT_CHANGED | ");
	if(n & AvColormap::NAME_CHANGED)
		strcat(buf, "NAME_CHANGED | ");
	if(n & AvColormap::LENGTH_CHANGED)
		strcat(buf, "LENGTH_CHANGED | ");
	if(n & AvColormap::MODEL_CHANGED)
		strcat(buf, "MODEL_CHANGED");
	return buf;
}

char *cereason(const int n)
{static char buf[132];

	buf[0] = '\0';
	if(n & AvConductor::Destroyed)
		strcat(buf, "Destroyed | ");
	if(n & AvColormapEditor::NEW_COLORMAP)
		strcat(buf, "NEW_COLORMAP");
	return buf;
}

static void tst2(XtPointer , AvConductor *c, XtPointer )
{
AvColormap *map = (AvColormap *) c;
int	mask = map->callbackReason();

	printf("%s:: Reason: %s (0x%X)\n", map->name(),cbreason(mask), mask);
}

static void tst(XtPointer , AvConductor *c, XtPointer )
{
AvColormapEditor *ce = (AvColormapEditor *) c;
AvCECmap *map = ce->getLastColormap();
int	mask = ce->callbackReason();
static	Boolean first=TRUE;

	if((mask & AvColormapEditor::COLORMAP_DESTROYED) != 0)
		printf("%s Destroyed\n", map->name());
	else
	if((mask & AvColormapEditor::MAP_CHANGED) != 0)
		printf("Using %s\n", ce->currentCmap()->name());
	else
	{	printf("%s Created\n", map->name());
		int cbmask = AvColormap::COMPONENT_CHANGED |
			   AvColormap::NAME_CHANGED |
			   AvColormap::LENGTH_CHANGED |
			   AvColormap::MODEL_CHANGED |
			   AvConductor::Destroyed;
		map->addCallback( tst2, cbmask, NULL, NULL);
		if(first)
		{	map = ce->getColormap(0);
			map->addCallback( tst2, cbmask, NULL, NULL);
			first = FALSE;
		}
	}
}
#endif

static const int MAXCMAPS=32;	// Maximum # of cmaps we'll allow.
static int colormapCount=0;	// # of colormaps 'new'd. Used for new names.

AvCECmap::AvCECmap(const int maxlen, const int len,
		   const char *name, Widget menuitem) :
	AvColormap(maxlen, len)
{
	mi_ = menuitem;
	pan_ = 0.0;
	zoom_ = 1.0;

	setName(name);		// Make sure widget label & name match.
}

AvCECmap::~AvCECmap()
{
//printf("Deleting widget %s (0x%X)\n", XtName(mi_), mi_);
	XtUnmanageChild(mi_);
	XtDestroyWidget(mi_);
}

void AvCECmap::setName(const char *newName)
{
	if(newName == NULL)
		return;
	if(strcmp(newName, name()) != 0)
	{	char *n = new char[strlen(newName)+1];
		strcpy(n, newName);
		AvUserComp::setLabelString(mi_, n);
		delete [] n;
		AvColormap::setName(newName);
	}
}

void AvColormapEditor::show()
{
  AvUserComp::show();
  for (int i = 0; i < 4; i++) functionEditors_[i]->graphWidth(256);
//  setSize(267, 434);
  setSize(267, 464);
  centerMap();
  updateMap(FALSE);
  drawMapRuler();
}

#ifdef CHANGENCOLORS
static char *USE_AVAILABLE = "Use available colors";
static char *USE_NOT_BW	= "Use all but B/W";
static char *USE_ALL	= "Use all colors";
#endif

//
// INITIALIZATION ONLY FUNCTION!!!!!!!!
//
void AvColormapEditor::buildMenus (int standAloneApp)
{
   // make the editor's menu bar
  AvMenu * fileMenu = new AvMenu (10);
  fileMenu->add ("New",        PUSH_BUTTON, FILE_MENU);
  fileMenu->add ("Open...",    PUSH_BUTTON, FILE_MENU);
  fileMenu->add ("Save",       PUSH_BUTTON, FILE_MENU);
  fileMenu->add ("Save As...", PUSH_BUTTON, FILE_MENU);
  fileMenu->add ("Delete",     PUSH_BUTTON, FILE_MENU);

  fileMenu->add ("", SEPARATOR);
  if (standAloneApp)
    fileMenu->add ("Quit",       PUSH_BUTTON, FILE_MENU);
  else
    fileMenu->add ("Close",      PUSH_BUTTON, FILE_MENU);
  
  AvMenu * optionMenu = new AvMenu (15);
  optionMenu->add ("Gray",       PUSH_BUTTON, OPTIONS_MENU);
  optionMenu->add ("RGB",        PUSH_BUTTON, OPTIONS_MENU);
  optionMenu->add ("HSV",        PUSH_BUTTON, OPTIONS_MENU);
  optionMenu->add ("", SEPARATOR);
  optionMenu->add ("Show Alpha", PUSH_BUTTON, OPTIONS_MENU);
  optionMenu->add ("", SEPARATOR);
  optionMenu->add ("Reset Map", PUSH_BUTTON, OPTIONS_MENU);
  if(readonlyColormap())
	optionMenu->add ("Auto Update", TOGGLE_BUTTON, OPTIONS_MENU, NULL, autoUpdate_);

#ifdef CHANGENCOLORS
  optionMenu->add ("", SEPARATOR);
  optionMenu->add (USE_AVAILABLE, PUSH_BUTTON, OPTIONS_MENU);
  optionMenu->add (USE_NOT_BW, PUSH_BUTTON, OPTIONS_MENU);
  optionMenu->add (USE_ALL, PUSH_BUTTON, OPTIONS_MENU);
#endif

  paletteMenu = new AvMenu (11 + MAXCMAPS);
  paletteMenu->add ("Grayscale", PUSH_BUTTON, PALETTES_MENU);
  paletteMenu->add ("Rainbow",   PUSH_BUTTON, PALETTES_MENU);
  paletteMenu->add ("Logarithmic", PUSH_BUTTON, PALETTES_MENU);
  paletteMenu->add ("Pseudo-Contour", PUSH_BUTTON, PALETTES_MENU);
  paletteMenu->add ("Multiple Ps-Contour", PUSH_BUTTON, PALETTES_MENU);
  paletteMenu->add ("", SEPARATOR);
  paletteMenu->add ("Copy to Clipboard",  PUSH_BUTTON, PALETTES_MENU);
  paletteMenu->add ("Copy from Clipboard",  PUSH_BUTTON, PALETTES_MENU);
  paletteMenu->add ("", SEPARATOR);

  AvMenu * mainMenu = new AvMenu (3);
  mainMenu->add ("File", SUBMENU_BUTTON, 0, fileMenu);
  mainMenu->add ("Palettes", SUBMENU_BUTTON, 0, paletteMenu);
  mainMenu->add ("Options", SUBMENU_BUTTON, 0, optionMenu);

  makeMainMenu (mainMenu);  // Inherited from AvApp
}

//
//  Function values changed callbacks
//

void AvColormapEditor::graph0ChangedCB(AvColormapEditor * ce)
{ 
  if (!ce->hsv() && ce->greyScale()) ce->relayFunctionData(0);
  ce->updateMap();
  ce->drawMapRuler();
}
void AvColormapEditor::graph1ChangedCB(AvColormapEditor * ce)
{ 
  if (!ce->hsv() && ce->greyScale()) ce->relayFunctionData(1);
  ce->updateMap();
  ce->drawMapRuler();
}
void AvColormapEditor::graph2ChangedCB(AvColormapEditor * ce)
{ 
  if (!ce->hsv() && ce->greyScale()) ce->relayFunctionData(1);
  ce->updateMap();
  ce->drawMapRuler();
}
void AvColormapEditor::graph3ChangedCB(AvColormapEditor * ce)
{ 
  ce->updateMap();
  ce->drawMapRuler();
}

void AvColormapEditor::buttonReleasedCB(AvColormapEditor * ce)
{
	ce->changeDone();
}

// Called when some operation that changed the colormap has finished. If we're
// not in auto update mode and the colormap is readonly, this invokes any
// callbacks that are attached.
void AvColormapEditor::changeDone()
{
	if(!autoUpdate() && readonlyColormap())
	{	invokeCallbacks(MAP_EDITED);
	}
}


//
//  Function mode changed callbacks
//
void AvColormapEditor::graph0ModeChangedCB(AvColormapEditor * ce)
{ if (!ce->hsv() && ce->greyScale()) ce->relayModeChange(0); }
void AvColormapEditor::graph1ModeChangedCB(AvColormapEditor * ce)
{ if (!ce->hsv() && ce->greyScale()) ce->relayModeChange(1); }
void AvColormapEditor::graph2ModeChangedCB(AvColormapEditor * ce)
{ if (!ce->hsv() && ce->greyScale()) ce->relayModeChange(2); }

void AvColormapEditor::graph0RepeatChangedCB(AvColormapEditor * ce)
{ if (!ce->hsv() && ce->greyScale()) ce->relayRepeatChange(0); }
void AvColormapEditor::graph1RepeatChangedCB(AvColormapEditor * ce)
{ if (!ce->hsv() && ce->greyScale()) ce->relayRepeatChange(1); }
void AvColormapEditor::graph2RepeatChangedCB(AvColormapEditor * ce)
{ if (!ce->hsv() && ce->greyScale()) ce->relayRepeatChange(2); }

// Default "ColorView"
void AvColormapEditor::init (int standAloneApp)
{
  palette_ = AvXColorApp::palette();
  palette_->ref();
  alpha_ = 0;
  greyScale_ = 0;
  active_ = 0;
  hsv_ = 1;
  lastPointerX_ = 0;  // Not necessary, but complete
  lastPointerY_ = 0;
  grabbed_ = -1;      // no control point grabbed
  ignoreUpdateRequest_ = FALSE;
  autoUpdate_ = FALSE;	// Should be FALSE for PC and user's choice for
			// TC colormaps.
  busy_ = False;	// Set to true to prevent recursion.

  map_ = NULL;
  buildMenus (standAloneApp);

  strcpy (paletteDirectory_, getStringResource ("PaletteDirectory"));
  XtVaSetValues (AvApp::parentWidget_, XmNtitle, "ColorView", NULL);

  //
  //  Map Parameters Set 
  //

  cursorShape_ = XC_pencil;
  cursor_ = XCreateFontCursor (display_, cursorShape_);

  // declare and set values needed for graph, map, and buttons draw areas
  XGCValues gcv;
//  XColor unused;
//  gcv.background = BlackPixelOfScreen (screen_);
  gcv.background = AvApp::blackPixel();
//  gc_ = XCreateGC (display_, RootWindowOfScreen (screen_), GCBackground, &gcv);
  gc_ = XCreateGC (display_, AvApp::referenceWindow(), GCBackground, &gcv);
 
  gcv.function = GXor;
//  xorGc_ = XCreateGC (display_, RootWindowOfScreen (screen_), GCFunction, &gcv);
  xorGc_ = XCreateGC (display_, AvApp::referenceWindow(), GCFunction, &gcv);
 
  // colormap to just get XColors needed for panel
  Colormap colormap = AvXColorApp::colormap();

  black_.flags = 0;	// These are to keep memory checkers happy.
  black_.pad = 0;
  black_.red = black_.green = black_.blue = 0;
#if 0
  XAllocNamedColor (display_, colormap, "black", &black_,  &unused);
  XAllocNamedColor (display_, colormap, "red",   &RGBA_[0], &unused);
  XAllocNamedColor (display_, colormap, "green4", &RGBA_[1], &unused);
  XAllocNamedColor (display_, colormap, "blue",  &RGBA_[2], &unused);
  XAllocNamedColor (display_, colormap, "yellow", &yellow_, &unused);
  XAllocNamedColor (display_, colormap, "gray80", &graphBackground_, &unused);
#else
 { unsigned long vindex, pindex;

	palette_->allocNamedColor("black", vindex, pindex);
	black_.pixel = pindex;
	palette_->allocNamedColor("red", vindex, pindex);
	RGBA_[0].pixel = pindex;
	palette_->allocNamedColor("green4", vindex, pindex);
	RGBA_[1].pixel = pindex;
	palette_->allocNamedColor("blue", vindex, pindex);
	RGBA_[2].pixel = pindex;
	palette_->allocNamedColor("yellow", vindex, pindex);
	yellow_.pixel = pindex;
	palette_->allocNamedColor("gray80", vindex, pindex);
	graphBackground_.pixel = pindex;
 }
#endif
  RGBA_[3] = black_;

  //--------------------------------------------------------------------------
  //
  //  Build Motif Panel
  //
  //  Uses palette_->availableColors() to set the starting width of both the
  //  function to use and the graph.  The one2one_ setting controls how the
  //  graph reacts to X axis resizing.
  //
  //  baseWidget_ is an XmForm!! 
  //
  //--------------------------------------------------------------------------

#define NOFIX256
#ifdef FIX256
  int funcWidth = 256;
  palette_->useSegment(2);
#else
  int funcWidth = palette_->availableColors();
#endif

  mapZoom_ = 1.0;
  mapPan_ = funcWidth / 2.0;

  XtVaSetValues(baseWidget_, 
		XmNtopAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNnoResize, False,
		XmNresizePolicy, XmRESIZE_ANY,
		XmNcolormap,  colormap,  // (not supposed to be settable here).
		NULL);

  //-----workArea_ 
  workArea_ = XtVaCreateManagedWidget
    ("workArea", xmFormWidgetClass, baseWidget_, 
     XmNtopAttachment, XmATTACH_FORM,
     XmNbottomAttachment, XmATTACH_FORM,
     XmNleftAttachment, XmATTACH_FORM,
     XmNrightAttachment, XmATTACH_FORM,
     XmNnoResize, False,
     XmNresizePolicy, XmRESIZE_ANY,
     XmNcolormap,  colormap,	// Use our colormap.
     NULL);
  
  XtVaSetValues (mainWindow_, XmNworkWindow, workArea_, NULL);
  
  //-----workArea_ | panel_
  panel_ = XtVaCreateManagedWidget
    ("panel", xmRowColumnWidgetClass, workArea_,
     XmNtopAttachment, XmATTACH_FORM,
     XmNbottomAttachment, XmATTACH_FORM,
     XmNleftAttachment, XmATTACH_FORM,
     XmNrightAttachment, XmATTACH_FORM,
     XmNresizeHeight, True,
     XmNresizeWidth, True, NULL);

  // A place to display the name of the current colormap
  Widget nameMgr_ = XtVaCreateManagedWidget
    ("nameMgr", xmFormWidgetClass, panel_,
     NULL);

  char *palstr = Strdup("Palette:");
  XmString xstr = XmStringCreateLocalized(palstr);
  Widget colormapLabelW = XtVaCreateManagedWidget
    ("CmapLabel", xmLabelWidgetClass, nameMgr_,
     XmNtopAttachment, XmATTACH_FORM,
     XmNleftAttachment, XmATTACH_FORM,
     XmNlabelString, xstr,
     NULL);
  delete [] palstr;
  XmStringFree(xstr);

  colormapNameW_ = XtVaCreateManagedWidget
    ("CmapName", xmTextFieldWidgetClass, nameMgr_,
     XmNtopAttachment, XmATTACH_FORM,
     XmNleftAttachment, XmATTACH_WIDGET,
     XmNleftWidget, colormapLabelW,
//     XmNrightAttachment, XmATTACH_WIDGET,
//     XmNrightWidget, closeButton,
     XmNrightAttachment, XmATTACH_FORM,
     XmNhighlightThickness, 0,
     XmNshadowThickness, 1,
     XmNvalue, "Clipboard",
     NULL);


  XtAddEventHandler(colormapNameW_, KeyReleaseMask, FALSE,
		     (XtEventHandler) &AvColormapEditor::changeNameCB,
		     (XtPointer) this);


  //-----workArea_ | panel_ | mapMgr 
  mapMgr_ = XtVaCreateManagedWidget
    ("mapMgr", xmFormWidgetClass, panel_,
     XmNheight, MAP_HEIGHT,
     NULL);
  
  //-----workArea_ | panel_ | mapMgr | map_
// Pixel	dmy = WhitePixelOfScreen(XtScreen(mapMgr_));
  int mapwidth = funcWidth;
  if(mapwidth > 256) mapwidth = 256;
  map_ = XtVaCreateManagedWidget
    ("map", xmDrawingAreaWidgetClass, mapMgr_,
//     XmNwidth,     funcWidth,
     XmNwidth,     mapwidth,
     XmNheight,    60,                   // Fallbacks not active!?!?!?
     XmNforeground, black_.pixel,            // don't remove
     XmNbackground, graphBackground_.pixel,  // don't remove
     XmNcolormap,  AvXColorApp::colormap(),      
     XmNresizePolicy, XmRESIZE_GROW,        // Will Change!
#if 0
	/* These are just to help keep away messages like:
		Warning: Cannot allocate colormap entry for "gray"
	*/
	XmNforeground, dmy,
	XmNborderWidth, 0,
	XmNborderColor, dmy,
	XmNbottomShadowColor, dmy,
	XmNhighlightColor, dmy,
	XmNtopShadowColor, dmy,
#endif
     NULL);

  static char graphTranslations[] =
    "<BtnDown>:    DrawingAreaInput() \n"
      "<BtnUp>:      DrawingAreaInput() \n" 
	"<BtnMotion>:  DrawingAreaInput()";

  XtAddCallback (map_, XmNexposeCallback, 
		 (XtCallbackProc) &AvColormapEditor::mapExposedCB,
		 (XtPointer) this);

  XtOverrideTranslations (map_,
			  XtParseTranslationTable (graphTranslations));

  XtAddCallback (map_, XmNinputCallback,
		 (XtCallbackProc) &AvColormapEditor::mapInputCB,
		 (XtPointer) this);

  XtAddEventHandler (map_, EnterWindowMask, 1,
		     (XtEventHandler) &AvColormapEditor::mapEnterEH,
		     (XtPointer) this);

  XtAddEventHandler (map_, LeaveWindowMask, 1,
		     (XtEventHandler) &AvColormapEditor::mapLeaveEH,
		     (XtPointer) this);

  //-----workArea_ | panel_ | mapRulMgr 
  Widget mapRulMgr = XtVaCreateManagedWidget
    ("mapMgr", xmRowColumnWidgetClass, panel_,
     NULL);

  //-----workArea_ | panel_ | mapRulMgr | mapRuler_
  mapRuler_ = XtVaCreateManagedWidget
    ("mapRuler", xmDrawingAreaWidgetClass, mapRulMgr,
//     XmNwidth, funcWidth,
     XmNwidth, mapwidth,
     XmNheight, 20,
     XmNforeground, black_.pixel,
     XmNbackground, graphBackground_.pixel,
     XmNresizePolicy, XmRESIZE_GROW,
     NULL);

  XtAddCallback (mapRuler_, XmNexposeCallback,
		 (XtCallbackProc) &AvColormapEditor::mapRulerExposedCB,
		 (XtPointer) this);

  //printf("map size = <%d, %d>\n", mapWidth(), mapHeight());
 
  static const char * feLabels[] = {"H", "S", "V", "A"};

  for (int i = 0; i < 4; i++)
    {
      // Required to keep the graph centered (no can do with xmForm)
      // and to identify the graph below since funcEditor not an X widget
      //
      //-----workArea_ | panel_ | graph<i>Mgr
      char name[20]; sprintf(name, "graph%dMgr", i);
      graphMgr_[i] = XtVaCreateManagedWidget
	(name, xmRowColumnWidgetClass, panel_,
	 XmNuserData, (XtPointer) i,
	 XmNresizeHeight, True,
	 XmNresizeWidth, True,
	 XmNleftAttachment, XmATTACH_FORM,
	 XmNrightAttachment, XmATTACH_FORM,
	 NULL);

      // for graph enter - change cursor color and shape
      XtAddEventHandler (graphMgr_[i], EnterWindowMask, 1,
			 (XtEventHandler) &AvColormapEditor::graphMgrEnterEH,
			 (XtPointer) this);
      
      //-----workArea_ | panel_ | graph<i>Mgr | graph

      functionEditors_[i] = new AvFunctionEditor(graphMgr_[i],
						 black_.pixel,
						 graphBackground_.pixel,
						 yellow_.pixel,
						 black_.pixel,
						 funcWidth); 

      functionEditors_[i]->label(feLabels[i]);         // Set the label text
      functionEditors_[i]->labelColor(RGBA_[i].pixel); // Set the label color
      functionEditors_[i]->userData((XtPointer) this);
    }
  
  functionEditors_[0]->valueChangedCallback((void (*)(XtPointer)) graph0ChangedCB);
  functionEditors_[1]->valueChangedCallback((void (*)(XtPointer)) graph1ChangedCB);
  functionEditors_[2]->valueChangedCallback((void (*)(XtPointer)) graph2ChangedCB);
  functionEditors_[3]->valueChangedCallback((void (*)(XtPointer)) graph3ChangedCB);
  functionEditors_[0]->modeChangedCallback((void (*)(XtPointer)) graph0ModeChangedCB);
  functionEditors_[1]->modeChangedCallback((void (*)(XtPointer)) graph1ModeChangedCB);
  functionEditors_[2]->modeChangedCallback((void (*)(XtPointer)) graph2ModeChangedCB);
  functionEditors_[0]->repeatChangedCallback((void (*)(XtPointer)) graph0RepeatChangedCB);
  functionEditors_[1]->repeatChangedCallback((void (*)(XtPointer)) graph1RepeatChangedCB);
  functionEditors_[2]->repeatChangedCallback((void (*)(XtPointer)) graph2RepeatChangedCB);

  // If colormap is read only, data windows typically only get updated when mouse
  // button is released. Don't bother with calling overhead when colormap is read write.
  if(readonlyColormap())
  { functionEditors_[0]->buttonReleaseCallback((void (*)(XtPointer)) buttonReleasedCB);
    functionEditors_[1]->buttonReleaseCallback((void (*)(XtPointer)) buttonReleasedCB);
    functionEditors_[2]->buttonReleaseCallback((void (*)(XtPointer)) buttonReleasedCB);
  }

  if (!alpha_) 
    {
      XtUnmanageChild(graphMgr_[3]);
    }
}
 


//
//  MENU RESPONSE
//

void AvColormapEditor::processMenu (char * buttonName, int choice, Widget mb)
{
  // Routines that change the colormap (via the function editors) will
  // result in changeNotify() being called several times. This turns off
  // notification so it's done only once at the bottom.
  disableNotify();

  switch (choice) 
    {
    case FILE_MENU:
      if (!strcmp (buttonName, "New"))
	useCmap(newCmap());

      if (!strcmp (buttonName, "Open..."))
	getFileNameFromUser ((AvProc) &AvColormapEditor::openMap,
			     "Select a palette", paletteDirectory_);

      if (!strcmp (buttonName, "Save"))
      {	if( (paletteDirectory_ == NULL) ||
	    (strcmp(paletteDirectory_, "NULL")==0))
		saveColorMap(currentCmap()->name());
	else
	{int len = (int)(strlen(currentCmap()->name())
				+
		  strlen(paletteDirectory_)) + 2;

	 char *name = new char[len];
		strcpy(name, paletteDirectory_);
		strcat(name, "/");
		strcat(name, currentCmap()->name());
		saveColorMap(name);
		delete [] name;
	}
      }
      else
      if (!strcmp (buttonName, "Delete"))
      {	//printf("[ ] New File\n");
	deleteCurrentCmap();
      }
      else
      if (!strcmp (buttonName, "Save As..."))
	getFileNameFromUser ((AvProc) &AvColormapEditor::saveMap,
			     "Specify file name", paletteDirectory_);

      if (!strcmp (buttonName, "Close"))
	hide ();

      if (!strcmp (buttonName, "Quit"))	exit(0);

      break;
    case OPTIONS_MENU:
      if (!strcmp (buttonName, "Gray")) grayScaleMode();
      else
	if (!strcmp (buttonName, "RGB")) rgbMode();
      else
	if (!strcmp (buttonName, "HSV")) hsvMode();
      else
	if (!strcmp (buttonName, "Show Alpha")) alphaMode(mb);
      else
	if (!strcmp (buttonName, "Reset Map"))
	  {
	    mapPan_ = nColors()/2.0;
	    mapZoom_ = 1.0;
	    updateMap();
	    drawMapRuler();
	    changeDone();
	  }
      else
	if (!strcmp (buttonName, "Auto Update"))
	  {	if(XmToggleButtonGetState(mb))
			autoUpdate_ = TRUE;
		else
			autoUpdate_ = FALSE;
	  }
// resizeColors() is a member function which orders the ColormapEditor to
// adjust itself to support the integer parameter # of colors.
//
// For now the width of the function editors reflects the number of colors
// available.
#ifdef CHANGENCOLORS
      else
	if (!strcmp (buttonName, USE_AVAILABLE))
	  { // For some reason, this doesn't work, but useSegment(1) does.
	    palette_->useSegment(0);
	    resetColors();
	  }

      else
	if (!strcmp (buttonName, USE_NOT_BW))
	  {
	    palette_->useSegment(1);
	    resetColors();
	  }
      else
	if (!strcmp (buttonName, USE_ALL))
	  {
	    palette_->useSegment(2);
	    resetColors();
	  }
#endif
      break;
      
    case PALETTES_MENU:
      if (!strcmp (buttonName, "Copy to Clipboard"))
      { // Copy current values to Clipboard.
	saveColormap(colormaps_[0]);
      }
      else
      if (!strcmp (buttonName, "Copy from Clipboard"))
      {	// Copy Clipboard to current.
	restoreColormap(colormaps_[0], FALSE);
	changeDone();
      }
      else
      if(loadPalette(buttonName))	// Predefined palette.
      {
	redrawGraphs();			// Redraw & save.
	changeDone();
      }
      else
      { int cmi;			// User palette.
		XtVaGetValues (mb, XmNuserData, &cmi, NULL);
		useCmap(cmi);
		redrawGraphs(FALSE);	// Redraw/don't save.
		changeDone();
      }

//      redrawGraphs(FALSE);
//      refresh(FALSE);
      break;

    default:
      AvXColorApp::processMenu (buttonName, choice, mb);
  }
enableNotify();
}

#ifdef CHANGENCOLORS
// Set number of colors to use to n. If n<= 0, query the palette.
void AvColormapEditor::resetColors (const int num)
{
int n, i;

	// # of colors to use.
	n = (num <= 0) ? palette_->availableColors() : num;
printf("%d\n", n);
	// Tell each function editor's ramp function to resize itself.
	for(i=0; i< sizeof(functionEditors_)/sizeof(*functionEditors_); i++)
		functionEditors_[i]->func()->resize(n);

	// Now, redisplay.
	redrawGraphs();
	/* Need to get list of windows and:
		1) Tell them data has been modified.
		2) Tell them to resdisplay.
	*/
}
#else
void AvColormapEditor::resetColors (const int ) {}

#endif

// Called only after init() by constructor
//
// initMap needs to initialize the map without triggering callbacks.  The
// solution is to build temporary rampFunctions to compute the rainbow
// palette values, install them in the colormap, and delete the temps.
//
void AvColormapEditor::initMap()
{
  int len = (int)functionEditors_[0]->func()->length();

  AvRampFunction * f0 = new AvRampFunction(len);
  f0->orderMode(AvRampFunction::LINEAR);
  f0->pts()->add(0, 213.0/255);
  f0->pts()->add(nColors()-1, 0);
  f0->recalc();
  
  AvRampFunction * f1 = new AvRampFunction(len);
  f1->orderMode(AvRampFunction::CONSTANT);
  f1->pts()->add(nColors()/2, 1.0);
  f1->recalc();
  
  AvRampFunction * f2 = new AvRampFunction(len);
  f2->orderMode(AvRampFunction::CONSTANT);
  f2->pts()->add(nColors()/2, 1.0);
  f2->recalc();

  AvRampFunction * f3 = new AvRampFunction(len);
  f3->orderMode(AvRampFunction::CONSTANT);
  f3->pts()->add(nColors()/2, 1.0);
  f3->recalc();

  float * r0 = f0->array();
  float * r1 = f1->array();
  float * r2 = f2->array();
  float * r3 = f3->array();

  memcpy(functionEditors_[0]->func()->array(), r0, len*sizeof(float));
  memcpy(functionEditors_[1]->func()->array(), r1, len*sizeof(float));
  memcpy(functionEditors_[2]->func()->array(), r2, len*sizeof(float));
  memcpy(functionEditors_[3]->func()->array(), r3, len*sizeof(float));

  for (int i = 0; i < len; i++)
    AvColormap::hsvTorgb(r0[i],
		      r1[i],
		      r2[i],
		      r0+i,
		      r1+i,
		      r2+i);

  storeColors(r0, r1, r2, 0, len);

  delete f0;
  delete f1;
  delete f2;
  delete f3;

  // Allocate space for the cmaps, create the first.
  colormaps_ = new AvCECmap *[MAXCMAPS];
  for(int icm=0; icm<MAXCMAPS; icm++)
	colormaps_[icm] = NULL;
  currentCmap_ = newCmap(getNameField());	// Default colormap.
  lastCmap_ = currentCmap_;
  paletteMenu->add ("", SEPARATOR);	// Doesn't work.
#ifdef CBTEST
  addCallback(tst, NEW_COLORMAP | COLORMAP_DESTROYED | MAP_CHANGED,
								 NULL, NULL);
#endif
#if 0
// This doesn't work either.
  {	// Manually add a separator since doing and add doesn't seem to work.
	AvMenuItem * mi = new AvMenuItem("", SEPARATOR, 0, NULL);

	// Add the item to the menu.
	Widget b = addMenuButton(paletteMenu->parentWidget(), mi);
  }
#endif
}

/* Load current colormap with predefined palettes.
NOTE:
Turns off update while loading data since this could cause ~ 8 calls.
Whatever function calls this will need to call redrawGraphs().

Returns TRUE if paletteName matches one of the predefined maps.
*/
Boolean AvColormapEditor::loadPalette(char * paletteName)
{
  AvRampFunction * f0 = functionEditors_[0]->func();
  AvRampFunction * f1 = functionEditors_[1]->func();
  AvRampFunction * f2 = functionEditors_[2]->func();

  Boolean success = TRUE;
  ignoreUpdateRequest_ = TRUE;

  if (!strcmp(paletteName, "Grayscale"))
    {
      if (hsv_)
	{
	  f0->pts()->lock(0);
	  f0->pts()->clear();
	  f0->pts()->add(nColors()/2, 0);
	  functionEditors_[0]->gotoMode(AvFunctionEditor::CONSTANT);
	  f0->pts()->lock(1);
	  f0->recalc();

	  f1->pts()->lock(0);
	  f1->pts()->clear();
	  f1->pts()->add(nColors()/2, 0);
	  functionEditors_[1]->gotoMode(AvFunctionEditor::CONSTANT);
	  f1->pts()->lock(1);
	  f1->recalc();
	  
	  f2->pts()->lock(0);
	  f2->pts()->clear();
	  f2->pts()->add(0,0);
	  f2->pts()->add(nColors()-1, 1);
	  functionEditors_[2]->gotoMode(AvFunctionEditor::LINEAR);
	  f2->pts()->lock(1);
	  f2->recalc();
	}
      else
	{
	  f0->pts()->lock(0);
	  f0->pts()->clear();
	  f0->pts()->add(0,0);
	  f0->pts()->add(nColors()-1, 1);
	  functionEditors_[0]->gotoMode(AvFunctionEditor::LINEAR);
	  f0->pts()->lock(1);
	  f0->recalc();

	  f1->pts()->lock(0);
	  f1->pts()->clear();
	  f1->pts()->add(0,0);
	  f1->pts()->add(nColors()-1, 1);
	  functionEditors_[1]->gotoMode(AvFunctionEditor::LINEAR);
	  f1->pts()->lock(1);
	  f1->recalc();

	  f2->pts()->lock(0);
	  f2->pts()->clear();
	  f2->pts()->add(0,0);
	  f2->pts()->add(nColors()-1, 1);
	  functionEditors_[2]->gotoMode(AvFunctionEditor::LINEAR);
	  f2->pts()->lock(1);
	  f2->recalc();
	}
      f0->waveMode(AvRampFunction::EXTEND);
      f1->waveMode(AvRampFunction::EXTEND);
      f2->waveMode(AvRampFunction::EXTEND);
    }
  else if (!strcmp(paletteName, "Rainbow"))
    {
      greyScale_ = 0;
      if (!hsv_)
	{
	  functionEditors_[0]->repeatMode(0, 0);
	  functionEditors_[0]->gotoMode(AvFunctionEditor::PWLINEAR);
	  f0->pts()->lock(0);
	  f0->pts()->clear();
	  f0->pts()->add(0,1);
	  f0->pts()->add((float)f0->length()/5, 0);
	  f0->pts()->add(3*(float)f0->length()/5,0);
	  f0->pts()->add(4*(float)f0->length()/5,1);
	  f0->recalc();

	  functionEditors_[1]->repeatMode(0, 0);
	  functionEditors_[1]->gotoMode(AvFunctionEditor::PWLINEAR);
	  f1->pts()->lock(0);
	  f1->pts()->clear();
	  f1->pts()->add((float)f1->length()/5, 0);
	  f1->pts()->add(2*(float)f1->length()/5,1);
	  f1->pts()->add(4*(float)f1->length()/5,1);
	  f1->pts()->add((float)(f1->length()-1), 0);
	  f1->recalc();

	  functionEditors_[2]->repeatMode(0, 0);
	  functionEditors_[2]->gotoMode(AvFunctionEditor::PWLINEAR);
	  f2->pts()->lock(0);
	  f2->pts()->clear();
	  f2->pts()->add(2*(float)f2->length()/5,1);
	  f2->pts()->add(3*(float)f2->length()/5,0);
	  f2->recalc();
	}
      else
	{
	  functionEditors_[0]->gotoMode(AvFunctionEditor::LINEAR);
	  f0->pts()->lock(0);
	  f0->pts()->clear();
	  f0->pts()->add(0, 213.0/255);
	  f0->pts()->add(nColors()-1, 0);
	  f0->pts()->lock(1);
	  f0->recalc();
	  
	  functionEditors_[1]->gotoMode(AvFunctionEditor::CONSTANT);
	  f1->pts()->lock(0);
	  f1->pts()->clear();
	  f1->pts()->add(nColors()/2, 1.0);
	  f1->pts()->lock(1);
	  f1->recalc();

	  functionEditors_[2]->gotoMode(AvFunctionEditor::CONSTANT);
	  f2->pts()->lock(0);
	  f2->pts()->clear();
	  f2->pts()->add(nColors()/2, 1.0);
	  f2->pts()->lock(1);
	  f2->recalc();
	}
    }
  else if (!strcmp(paletteName, "Logarithmic"))
    {
      greyScale_ = 1;
      if (!hsv_)
	{
	  functionEditors_[0]->gotoMode(AvFunctionEditor::LOG);
	  f0->pts()->lock(0);
	  f0->pts()->clear();
	  f0->pts()->add(0,0);
	  f0->pts()->add(nColors()/2, 0.25);
	  f0->pts()->add(nColors()-1, 1.0);
	  f0->pts()->lock(1);
	  f0->recalc();

	  functionEditors_[1]->gotoMode(AvFunctionEditor::LOG);
	  f1->pts()->lock(0);
	  f1->pts()->clear();
	  f1->pts()->add(0,0);
	  f1->pts()->add(nColors()/2, 0.25);
	  f1->pts()->add(nColors()-1, 1.0);
	  f1->pts()->lock(1);
	  f1->recalc();

	  functionEditors_[2]->gotoMode(AvFunctionEditor::LOG);
	  f2->pts()->lock(0);
	  f2->pts()->clear();
	  f2->pts()->add(0,0);
	  f2->pts()->add(nColors()/2, 0.25);
	  f2->pts()->add(nColors()-1, 1.0);
	  f2->pts()->lock(1);
	  f2->recalc();	  
	}
      else
	{
	  functionEditors_[0]->gotoMode(AvFunctionEditor::CONSTANT);
	  f0->pts()->lock(0);
	  f0->pts()->clear();
	  f0->pts()->add(nColors()/2, 0.5);
	  f0->pts()->lock(1);
	  f0->recalc();

	  functionEditors_[1]->gotoMode(AvFunctionEditor::CONSTANT);
	  f1->pts()->lock(0);
	  f1->pts()->clear();
	  f1->pts()->add(nColors()/2, 0);
	  f1->pts()->lock(1);
	  f1->recalc();

	  functionEditors_[2]->gotoMode(AvFunctionEditor::LOG);
	  f2->pts()->lock(0);
	  f2->pts()->clear();
	  f2->pts()->add(0,0);
	  f2->pts()->add(nColors()/2, 0.25);
	  f2->pts()->add(nColors()-1, 1.0);
	  f2->pts()->lock(1);
	  f2->recalc();
	}
    }
  else if (!strcmp(paletteName, "Pseudo-Contour"))
    {
      if (!hsv_)
	{
	  greyScale_ = 0;
	  functionEditors_[0]->gotoMode(AvFunctionEditor::PWLINEAR);
	  f0->pts()->lock(0);
	  f0->pts()->clear();
	  f0->pts()->add(0,0);
	  f0->pts()->add(nColors()/14, 1.0);
	  f0->pts()->add(nColors()/7, 0.0);
	  f0->recalc();

	  functionEditors_[1]->gotoMode(AvFunctionEditor::CONSTANT);
	  f1->pts()->lock(0);
	  f1->pts()->clear();
	  f1->pts()->add(nColors()/2, 0);
	  f1->pts()->lock(1);
	  f1->recalc();

	  functionEditors_[2]->gotoMode(AvFunctionEditor::CONSTANT);
	  f2->pts()->lock(0);
	  f2->pts()->clear();
	  f2->pts()->add(nColors()/2, 0);
	  f2->pts()->lock(1);
	  f2->recalc();
	}
      else
	{
	  functionEditors_[0]->gotoMode(AvFunctionEditor::CONSTANT);
	  f0->pts()->lock(0);
	  f0->pts()->clear();
	  f0->pts()->add(nColors()/2, 0);
	  f0->pts()->lock(1);
	  f0->recalc();

	  functionEditors_[1]->gotoMode(AvFunctionEditor::CONSTANT);
	  f1->pts()->lock(0);
	  f1->pts()->clear();
	  f1->pts()->add(nColors()/2, 1.0);
	  f1->pts()->lock(1);
	  f1->recalc();

	  functionEditors_[2]->gotoMode(AvFunctionEditor::PWLINEAR);
	  f2->pts()->lock(0);
	  f2->pts()->clear();
	  f2->pts()->add(0,0);
	  f2->pts()->add(nColors()/14, 1.0);
	  f2->pts()->add(nColors()/7, 0.0);
	  f2->recalc();
	}
    }
  else if (!strcmp(paletteName, "Multiple Ps-Contour"))
    {
      greyScale_ = 0;
      hsvMode();

      functionEditors_[0]->gotoMode(AvFunctionEditor::PWCONSTANT);
      f0->pts()->lock(0);
      f0->pts()->clear();
      f0->pts()->add(0, .678);
      f0->pts()->add(0.230*nColors(), .3051);
      f0->pts()->add(0.440*nColors(), 1.0);
      f0->pts()->add(0.690*nColors(), 0.1695);
      f0->recalc();
      
      functionEditors_[1]->gotoMode(AvFunctionEditor::CONSTANT);
      f1->pts()->lock(0);
      f1->pts()->clear();
      f1->pts()->add(nColors()/2, 1.0);
      f1->pts()->lock(1);
      f1->recalc();
      
      functionEditors_[2]->gotoMode(AvFunctionEditor::PWLINEAR);
      f2->pts()->lock(0);
      f2->pts()->clear();
      f2->pts()->add(0, 0);
      f2->pts()->add(.075*nColors(), 1.0);
      f2->pts()->add(.150*nColors(), 0.0);
      f2->pts()->add(.250*nColors(), 0.0);
      functionEditors_[2]->repeatMode(1);
      f2->recalc();
    }
    else
      success = FALSE;

    ignoreUpdateRequest_ = FALSE;
    return success;
}

// Copy info from a Colormap to current info.
void AvColormapEditor::restoreColormap(AvCECmap *map, const Boolean copyname)
{
int i;
const char **names = 0;

  if(map == NULL)
	map = currentCmap();

  float *r0 = functionEditors_[0]->func()->array();
  float *r1 = functionEditors_[1]->func()->array();
  float *r2 = functionEditors_[2]->func()->array();
  float *r3 = functionEditors_[3]->func()->array();

  map->getValues(r0, r1, r2, r3, 0, nColors());
  switch (map->getModelType()) {
  case AvColormap::RGB:
	hsv_ = 0;
	names =  rgbGraphNames;
	break;
  case AvColormap::GRAYSCALE:
	hsv_ = 0;
	names =  rgbGraphNames;
	break;
  default:
  case AvColormap::DEFAULT:
  case AvColormap::HSV:
	hsv_ = 1;
	names = hsvGraphNames;
	break;
  }

  if(copyname)
	setNameField(map->name());

  for (i = 0; i < 3; i++)
  {	  functionEditors_[i]->label(names[i]);
	  functionEditors_[i]->gotoMode(AvFunctionEditor::DRAW);
  }

  mapPan_ = map->pan();
  mapZoom_ = map->zoom();
  greyScale_ = map->grayScale();

  // Refresh command w/o writing back to CM.
//  functionEditors_[active_]->redraw(); 
  updateMap(FALSE);
  drawMapRuler();
}

// Copy info from current info to a Colormap.
void AvColormapEditor::saveColormap(AvCECmap *map)
{
  if(map == NULL)
	map = currentCmap();

  float *r0 = functionEditors_[0]->func()->array();
  float *r1 = functionEditors_[1]->func()->array();
  float *r2 = functionEditors_[2]->func()->array();
  float *r3 = functionEditors_[3]->func()->array();

  map->disableNotify();
	  map->putValues(r0, r1, r2, r3, 0, nColors());
	  map->setModelType(colorModel());
	  map->pan(mapPan_);
	  map->zoom(mapZoom_);
	  map->grayScale(greyScale_);
  map->enableNotify(TRUE);	// Reenable notify, & call callbacks.
}

AvCECmap *AvColormapEditor::currentCmap()
{
	return colormaps_[currentCmap_];
}

/* Return a pointer to a colormap.
	-1		Last created colormap.
	0..MAXCMAPS-1	Pointer to colormap index (may be NULL if none).
	>= MAXCMAPS	NULL.
*/
AvCECmap *AvColormapEditor::getColormap(const int index)
{
	if(index < 0)
		return colormaps_[lastCmap_];
	else
	if(index < MAXCMAPS)
		return colormaps_[index];
	else
		return NULL;
}

AvCECmap **AvColormapEditor::getColormapList(int &count)
{
AvCECmap	**list = new AvCECmap *[MAXCMAPS], **p=list;
int	i;

	for(i=0; i<MAXCMAPS; i++)
		if(colormaps_[i] == NULL)
			*p++ = colormaps_[i];
	count = (int)(p - colormaps_);
	return list;
}

// Build a new colormap, insert it in list & palette menu and return its index.
int AvColormapEditor::newCmap(const char *Name)
{
AvCECmap *cm;
int	index;
char *name;

	// Find an empty slot or use Default if filled up.
	if( colormapCount < MAXCMAPS)
		index = colormapCount++;
	else
	{int j;
		for(index = -1, j = 1; j< MAXCMAPS; j++)
			if(colormaps_[j] == NULL)
			{	index = j;
				break;
			}
		if( index < 0)
		{	cerr <<
	"AvColormapEditor::newCmap: out of colormaps, using Clipboard" << endl;
			return 0;
		}
	}

	if(Name == NULL)
	{	name = new char[32];
		sprintf(name, "__unnamed_colormap%d__", index);
	}
	else
	{	name = new char[strlen(Name)+1];
		strcpy(name, Name);
	}
	//			Add name to palette menu.
	// Create a new menu item.
	AvMenuItem * mi = new AvMenuItem(name,
					  PUSH_BUTTON, PALETTES_MENU, NULL);

	// Add the item to the menu.
	Widget b = addMenuButton(paletteMenu->parentWidget(), mi);
  
	// Save the colormap's index to use for retrieval when selected.
	XtVaSetValues (b, XmNuserData, index, NULL);

	// When selected go to the menu handler
	XtAddCallback (b, XmNactivateCallback,
		 (XtCallbackProc) &AvColormapEditor::processMenuCB,
		 (XtPointer) mi);

	// Create the colormap.
	cm = new AvCECmap(palette_->maxColors(), palette_->availableColors(),
			    name, b);
	cm->setModelType( colorModel());
	colormaps_[index] = cm;

	saveColormap(cm);		// Initialize to current values.
	lastCmap_ = index;

	invokeCallbacks(NEW_COLORMAP);
	delete [] name;
	return index;
}

void AvColormapEditor::deleteCurrentCmap()
{
	if((currentCmap_ >= 1) && (currentCmap_ < MAXCMAPS)
		&& (colormaps_[currentCmap_] != NULL))
	{	//printf("Deleting colormap: %s\n", currentCmap()->name());
		lastCmap_ = currentCmap_;
		invokeCallbacks(COLORMAP_DESTROYED);
		delete colormaps_[currentCmap_];
		colormaps_[currentCmap_] = NULL;
		useCmap(0);		// Use "Default map".
	}
}

void AvColormapEditor::useCmap(const int cm)
{
	if(cm == currentCmap_)		// Avoid overhead.
		return;
	// Update name field.
	if((cm >= 0) && (cm < MAXCMAPS) && (colormaps_[cm] != NULL))
	{	currentCmap_ = cm;
		restoreColormap();
		invokeCallbacks(MAP_CHANGED);
	}
}

// Look for a colormap with the given name. Returns -1 if no match. Or
// The colormap's index.
int AvColormapEditor::findNamedCmap(const char *name)
{
	if(name == NULL)
		return -1;
	for(int i = 0; i< MAXCMAPS; i++)
		if(colormaps_[i] == NULL)
			continue;
		else
		if(strcmp(name, colormaps_[i]->name()) == 0)
			return i;

	return -1;
}

void AvColormapEditor::openMap (AvColormapEditor * ce, XtPointer d1)
{
  //  cerr << "ColormapEditor::openMap " << (char *) d1 << endl;
  ce->openColorMap ((char *) d1);
  char * fName = (char *) d1;

  // set the paletteDirectory to this one...
  ce->updatePathName (ce->paletteDirectory_, fName);
}


void AvColormapEditor::saveMap (AvColormapEditor * ce, XtPointer d1)
{
  //  cerr << "ColormapEditor::saveMap " << (char *) d1 << endl;
  ce->saveColorMap ((char *) d1);
}


int AvColormapEditor::updateObj ()
{
  invokeCallbacks (Freq1);
  return 0;
}

//--------------------------------------------------------------------
//
//  ***  MAP WIDGET FUNCTIONS  ***
//
//--------------------------------------------------------------------
//
//  This function transfers the function data from the editors
//  to the colormap.  If the ramp data represents HSV information,
//  it is first converted to RGB before the update.
//
//  It is assumed that all 3 (4) functions have the same length.
//
//  The functions stored in the function editors have an rotational
//  x shift and a zoom applied to them before they are stored away
//  in the X colormap.
//
//  Zoom is applied first, then pan, to generate the starting and ending
//  points for extracting the map values.
//
//  Initial mapFocus value should be nColors()/2.0
//  Initial zoom value should be 1.0
//
//

//  Apply the pan/zoom to fill the arrays from the values stored in
//  the function editors.
//
void AvColormapEditor::panZoomMaps(float * r, float * g, float * b,
				   float * a)
{
  int len = (int)functionEditors_[0]->func()->length();

  float * f0 = functionEditors_[0]->func()->array();
  float * f1 = functionEditors_[1]->func()->array();
  float * f2 = functionEditors_[2]->func()->array();
  float * f3 = functionEditors_[3]->func()->array();

  if ((mapPan_ == len/2.0) && (mapZoom_ == 1.0))
    {
      memcpy(r, f0, len*sizeof(float));
      memcpy(g, f1, len*sizeof(float));
      memcpy(b, f2, len*sizeof(float));
      memcpy(a, f3, len*sizeof(float));
    }
  else
    {
      float mapStart = mapPan_ - len/(2*mapZoom_);
      float mapStep = 1.0/mapZoom_;
      float mapCurrent = mapStart;
      for (int i = 0; i < len; i++, mapCurrent += mapStep)
	{
	  // Nearest Neighbor
	  // 
	  // (CYLIC) int ndx = ((int)(mapCurrent + 1000*len + 0.5) % len);
	  int ndx = CLAMP((int)(mapCurrent + 0.5), 0, len-1);
	  r[i] = f0[ndx];
	  g[i] = f1[ndx];
	  b[i] = f2[ndx];
	  a[i] = f3[ndx];
	}
    }
}

void AvColormapEditor::updateMap (const Boolean savecm)
{
  if(ignoreUpdateRequest_)
	return;

  int len = (int)functionEditors_[0]->func()->length();

  float * f0 = new float[len];
  float * f1 = new float[len];
  float * f2 = new float[len];
  float * f3 = new float[len];

  // fill the arrays with the correct values from the real functions
  // taking shift and zoom into effect.
  panZoomMaps(f0, f1, f2, f3);

  if(savecm)
	saveColormap();

  if (hsv_)   // values represent HSV values ==> require conversion to RGB
    {
      float * redArray   = new float[len];
      float * greenArray = new float[len];
      float * blueArray  = new float[len];
      
      for (int i = 0; i < len; i++)
	{
	  AvColormap::hsvTorgb(f0[i],
			    f1[i],
			    f2[i],
			    redArray+i,
			    greenArray+i,
			    blueArray+i);
	}
      
      delete [] f0;  // delete old HSV arrays
      delete [] f1;
      delete [] f2;

      f0 = redArray;
      f1 = greenArray;
      f2 = blueArray;
    }

  storeColors(f0, f1, f2, 0, len);


  delete [] f0;
  delete [] f1;
  delete [] f2;
  delete [] f3;
}

void AvColormapEditor::centerMap()
{
  mapOffset_ = (functionEditors_[0]->graphWidth() - mapWidth()) / 2;
  XMoveWindow(display_, XtWindow(map_), 3 + mapOffset_, 0);
}

//
//  Map Dimensions
//

int AvColormapEditor::mapWidth()
{ Dimension w; XtVaGetValues(map_, XmNwidth, &w, NULL); return (int) w; }

int AvColormapEditor::mapHeight()
{ Dimension h; XtVaGetValues(map_, XmNheight, &h, NULL); return (int) h; }

//  
//  This is called on each expose.

void AvColormapEditor::drawMap()
{
	if(!XtIsManaged(map_))
		return;
	Window win = XtWindow(map_);
	if(win == 0)
		return;
  // The map widget, now contained by an RC manager, is always completely
  // filled with the colorbars.  It is only necessary to redraw it here.

  float color = 0.0;  // virtual color
  float colorStep = 
    ((float) functionEditors_[0]->func()->length()) / mapWidth();

  for (int x = 0; x < mapWidth(); x++, color += colorStep)
    {   unsigned long physPixel;
	palette_->virtualToPhysical((int)color, physPixel);
      XSetForeground(display_, gc_, physPixel);
      XDrawLine(display_, XtWindow(map_), gc_, x, 0, x, mapHeight()-1);
    }
}

void AvColormapEditor::mapInputCB
(Widget w, AvColormapEditor * ce, XmDrawingAreaCallbackStruct * cb)
{
  switch(cb->event->type)
    {
    case ButtonPress:
      ce->mapPress(w); break;
    case MotionNotify:
      ce->mapMotion(w);
      break;
    case ButtonRelease:
	ce->changeDone();
      break;
    }
}

//-----Save the cursor position when pressed
void AvColormapEditor::mapPress(Widget w)
{ 
  queryPointer(w, &lastPointerX_, &lastPointerY_); 
}

//-----Adjust the focus and zoom of the map
void AvColormapEditor::mapMotion(Widget w)
{ 
  int cx, cy;  queryPointer(w, &cx, &cy);

  // pan first?
  mapPan_ -= (cx - lastPointerX_) / mapZoom_;
  mapZoom_ += mapZoom_ * (cy - lastPointerY_) / 32.0;
  mapZoom_ = CLAMP(mapZoom_, 0.1, 10.0);

  lastPointerX_ = cx;
  lastPointerY_ = cy;

  updateMap();
  drawMapRuler();
}

void AvColormapEditor::mapEnterEH
(Widget, AvColormapEditor *, XEvent *, Boolean *)
{
  //printf("map enter EH\n");
}

void AvColormapEditor::mapLeaveEH
(Widget, AvColormapEditor *, XEvent *, Boolean *)
{
  //printf("map leave EH\n");
}

void AvColormapEditor::mapRulerExposedCB
(Widget , AvColormapEditor * ce, XmDrawingAreaCallbackStruct * )
{
  ce->drawMapRuler();
}

void AvColormapEditor::drawMapRuler()
{
  // Compute map critical points
  float fstart = mapPan_ - nColors()/(2*mapZoom_);
  float fstop = mapPan_ + nColors()/(2*mapZoom_);

  float mstart = nColors()*((0 - fstart)/(fstop - fstart));
  float mstop = mstart + nColors()*mapZoom_;

  fstart = CLAMP(fstart, 0, nColors()-1);
  fstop = CLAMP(fstop, 0, nColors()-1);

  mstart = CLAMP(mstart, 0, nColors()-1);
  mstop = CLAMP(mstop, 0, nColors()-1);

  // Draw it

  XPoint * pts = new XPoint[8];

  XClearWindow(display_, XtWindow(mapRuler_));

  float xScale = 
    (float) functionEditors_[0]->graphWidth()
			/
    (float) functionEditors_[0]->func()->length();
    
  XSetForeground(display_, gc_, black_.pixel);

  pts[0].x = short(mstart + mapOffset_); pts[0].y = 0;
  pts[1].x = short(mstart + mapOffset_); pts[1].y = 4;
  pts[2].x = short(fstart*xScale); pts[2].y = 15;
  pts[3].x = short(fstart*xScale); pts[3].y = 19;

  XDrawLines(display_, XtWindow(mapRuler_), gc_,
	     pts, 4, CoordModeOrigin);

  pts[0].x = short(mstop + mapOffset_);
  pts[1].x = short(mstop + mapOffset_);
  pts[2].x = short(fstop*xScale);
  pts[3].x = short(fstop*xScale);

  XDrawLines(display_, XtWindow(mapRuler_), gc_,
	     pts, 4, CoordModeOrigin);

  delete [] pts;
}

//--------------------------------------------------------------------------
//
// 
//
void AvColormapEditor::setCursor (int ndx)
{
  XFreeCursor (display_, cursor_);
  cursor_ = XCreateFontCursor (display_, cursorShape_);

  // Maybe switch to crosshair when points need to be manipulated

  XDefineCursor (display_, XtWindow (graphMgr_[ndx]), cursor_);
  XRecolorCursor (display_, cursor_, &RGBA_[ndx], &black_);
}

//===================================
//
//  MODE CHANGES ON ACTIVE GRAPH
//
//===================================

int AvColormapEditor::queryMode(int ndx)
{ return functionEditors_[ndx]->mode(); }
	  
void AvColormapEditor::relayModeChange(int source)
{
  int activeMode = functionEditors_[source]->mode();
  for (int i = 0; i < 3; i++)
    {
      if (i == source) continue;
      functionEditors_[i]->gotoMode(activeMode, 0);
    }
}

void AvColormapEditor::relayRepeatChange(int source)
{
  int repeat = functionEditors_[source]->repeatMode();
  for (int i = 0; i < 3; i++)
    {
      if (i == source) continue;
      functionEditors_[i]->repeatMode(repeat, 0);
    }
}


void AvColormapEditor::relayFunctionData(int source)
{
  AvRampFunction * f = functionEditors_[source]->func();

  for (int i = 0; i < 3; i++)
    {
      if (i == source) continue;
      (*functionEditors_[i]->func()) = (*f);
      functionEditors_[i]->redraw();
    }
}

//============================
//
//  OPTION MENU MODE CHANGES
//
//============================

// Invoked from menu switch, not CB's!
void AvColormapEditor::grayScaleMode()
{
  // Turn off callbacks since both rgbMode & refresh would cause callbacks.
  currentCmap()->disableNotify();
   rgbMode();          // Meaningless in hsvMode!
   functionEditors_[0]->gotoMode(AvFunctionEditor::DRAW);
   functionEditors_[1]->gotoMode(AvFunctionEditor::DRAW);
   functionEditors_[2]->gotoMode(AvFunctionEditor::DRAW);
   greyScale_ = 1;     // Means link all changes among RGB ramps
   refresh();
  currentCmap()->enableNotify(TRUE);
}

void AvColormapEditor::rgbMode() 
{
  greyScale_ = 0;
  if (hsv_) // convert hsv ramp data to rgb values
    {int i;
      hsv_ = 0;
      
      for (i = 0; i < 3; i++)
	{
	  functionEditors_[i]->label(rgbGraphNames[i]);
	  functionEditors_[i]->labelColor(RGBA_[i].pixel);  // HACK
	  functionEditors_[i]->gotoMode(AvFunctionEditor::DRAW);
	}
      int n = (int)functionEditors_[0]->func()->length();

      float * r = functionEditors_[0]->func()->array();
      float * g = functionEditors_[1]->func()->array();
      float * b = functionEditors_[2]->func()->array();

      for (i = 0; i < n; i++)
	AvColormap::hsvTorgb(r[i], g[i], b[i], r+i, g+i, b+i);
    }
  redrawGraphs();
//  refresh();
}

void AvColormapEditor::hsvMode()
{
  greyScale_ = 0;
  if (!hsv_) // convert rgb ramp data to hsv values 
    {int i;
      for (i = 0; i < 3; i++)
	{
	  functionEditors_[i]->label(hsvGraphNames[i]);
	  functionEditors_[i]->labelColor(RGBA_[i].pixel);
	  functionEditors_[i]->gotoMode(AvFunctionEditor::DRAW);
	}
      hsv_ = 1;
      int n = (int)functionEditors_[0]->func()->length();

      float * h = functionEditors_[0]->func()->array();
      float * s = functionEditors_[1]->func()->array();
      float * v = functionEditors_[2]->func()->array();

#if 0
      for (i = 0; i < n; i++)
	AvColormap::rgbTohsv(h[i], s[i], v[i], h+i, s+i, v+i);
#else
	AvColormap::rgbTohsv(n, h, s, v, h, s, v);
#endif
      //
      // Correct rgbTohsv bad hue return values of -1
      //
      for (i = 0; i < n; i++) h[i] = CLAMP(h[i], 0, 1);
    }
  redrawGraphs();
//  refresh();
}

void AvColormapEditor::alphaMode(Widget menuItem)
{
  if (alpha_)  // was on, shut off
    {
      XtUnmanageChild(graphMgr_[3]);
      alpha_ = 0;
      setLabelString (menuItem, "Show Alpha");
      XtRealizeWidget(workArea_);
//      setSize(267, 434);
      setSize(267, 464);
    }
  else
    {
      XtManageChild(graphMgr_[3]);
      alpha_ = 1;
      setLabelString (menuItem, "Hide Alpha");
      XtRealizeWidget(workArea_);
//      setSize(267, 534);
      setSize(267, 564);
    }
}

//=====================
//
//  CONTEXT SWITCHING - Event Handlers responding to widget entry by pointer
//
//=====================

// THIS IS AN EVENT HANDLER
void AvColormapEditor::graphMgrEnterEH
(Widget w, AvColormapEditor *ce, XEvent *, Boolean *)
{
  int whichGraph;
  XtVaGetValues(w, XmNuserData, &whichGraph, NULL);
  ce->active(whichGraph);
}

//=================================================================
//
// UTILITIES
//
// Accepts a window and two x,y pointers.  Returns in x and y the
// local pointer location as well as the returned window from 
// XQueryPointer
//
//=================================================================
Window AvColormapEditor::queryPointer(Widget w, int * x, int * y)
{
  Window root,win; int rootX, rootY; unsigned int mask;
  XQueryPointer(display_, XtWindow(w),  
		&root, &win, &rootX, &rootY, x, y, &mask);
  return win;
}

//======================
//
//  EXPOSURE CALLBACKS
//
//======================

void AvColormapEditor::mapExposedCB
(Widget w, AvColormapEditor *ce, XmDrawingAreaCallbackStruct *)
{
  Widget widgets[2];
  
  // get root of widget
  Widget p, root = w;
  while ((p = XtParent (root)) != NULL) root = p;
  
  widgets[0] = w;
  widgets[1] = root;

  XtSetWMColormapWindows (root, widgets, 2);

  ce->disableNotify();
	ce->drawMap();
	ce->refresh(FALSE);
  ce->enableNotify(False);
}

//
// Read Colormap from file 
//
void AvColormapEditor::openColorMap (char * colorMapName)
{
  enum { RGB, RGBA, HSV, HSVA, FAIL } fileType = FAIL;

  FILE *fp;
  if ((fp = fopen (colorMapName, "r")) == NULL)
    {
      printf ("Can't open file %s\n", colorMapName);
      return;
    }

  char typeStr[256];
  fscanf (fp, "%s", typeStr);

  if (!strcmp(typeStr, "RGB"))
    fileType = RGB;
  else if (!strcmp(typeStr, "RGBA"))
    fileType = RGBA;
  else if (!strcmp(typeStr, "HSV"))
    fileType = HSV;
  else if (!strcmp(typeStr, "HSVA"))
    fileType = HSVA;

  float *r0 = functionEditors_[0]->func()->array();
  float *r1 = functionEditors_[1]->func()->array();
  float *r2 = functionEditors_[2]->func()->array();
  float *r3 = functionEditors_[3]->func()->array();


  // Peal off leading pathname and then use what's left to get a palette.
  {char *p = strrchr(colorMapName, '/');
   int index;

	if(p == NULL)
		p = colorMapName;
	else
		p += 1;

	// Resuse the map if the name already exists.
	if((index = findNamedCmap(p)) >= 0)
		useCmap(index);
	else
		useCmap(newCmap(p));
  }
  currentCmap()->disableNotify();		// Disable callbacks.

  switch(fileType)
    {
    case RGB:
    case HSV:
      {
	(fileType == RGB) ? rgbMode() : hsvMode();
	for (int i = 0; i < nColors(); i++)
	  fscanf(fp, "%f %f %f", r0+i, r1+i, r2+i);
      }
      break;
    case RGBA:
    case HSVA:
      {
	(fileType == RGBA) ? rgbMode() : hsvMode();
	for (int i = 0; i < nColors(); i++)
	  fscanf(fp, "%f %f %f %f", r0+i, r1+i, r2+i, r3+i);
      }
      break;
    case FAIL:
      printf("Bad File Format\n");
      break;
    }
  fclose(fp);
  for (int i = 0; i < 4; i++) 
    functionEditors_[i]->mode(AvFunctionEditor::DRAW);

  currentCmap()->enableNotify(FALSE);	// Reenable callbacks, but let...
  redrawGraphs();			// ...this invoke them.
}

void AvColormapEditor::saveColorMap (char * colorMapName)
{
  enum {RGB = 0, RGBA, HSV, HSVA } fileType = RGB;

  FILE *fp;
  if ((fp = fopen (colorMapName, "w")) == NULL)
    {
      printf ("Can't open file %s\n", colorMapName);
      return;
    }

  if (!hsv_ && !alpha_) fileType = RGB;
  else if (!hsv_ && alpha_) fileType = RGBA;
  else if (hsv_ && !alpha_) fileType = HSV;
  else fileType = HSVA;

  float *r0 = functionEditors_[0]->func()->array();
  float *r1 = functionEditors_[1]->func()->array();
  float *r2 = functionEditors_[2]->func()->array();
  float *r3 = functionEditors_[3]->func()->array();

  switch (fileType)
    {
    case RGB:
    case HSV:
      {
	fprintf(fp, "%s\n", (fileType == RGB) ? "RGB" : "HSV");
	for (int i = 0; i < nColors(); i++)
	  fprintf (fp, "%6.4f %6.4f %6.4f\n", r0[i], r1[i], r2[i]);
      }
    case RGBA:
    case HSVA:
      {
	fprintf(fp, "%s\n", (fileType == RGBA) ? "RGBA" : "HSVA");
	for (int i = 0; i < nColors(); i++)
	  fprintf (fp, "%6.4f %6.4f %6.4f %6.4f\n", r0[i], r1[i], r2[i], r3[i]);
      }
    }
  fclose(fp);
}

void AvColormapEditor::changeNameCB(Widget /*widget*/, AvColormapEditor *obj,
				    XtPointer data)
{

	if(obj != NULL)
	{XEvent *event = (XEvent *)data;
	 KeySym sym;
	 int     index = 0;

		// Ignore unless <RETURN> was typed.
		if(event->type == KeyRelease)
		{	sym = XLookupKeysym((XKeyEvent*)event, index);
			if(sym == XK_Return)
			   obj->setCurrentName(obj->getNameField());
		}
	}
}

// Set name of current cmap.
// Can't change name of 1st (Clipboard) colormap.
// If newname is NULL, just change name of current colormap. Otherwise,
// also set name field.
void AvColormapEditor::setCurrentName(char *newname)
{

	if(currentCmap_ > 0)
	{	if(newname == NULL)
			currentCmap()->setName(getNameField());
		else
		{	currentCmap()->setName(newname);
			setNameField(newname);
		}
	}
	else	// If default, make sure name stays the same.
	if(currentCmap_ == 0)
		setNameField(currentCmap()->name());
}

// Change the name of the current colormap to match that stored in the
// textfield widget.
char *AvColormapEditor::getNameField(Widget widget)
{
static char *str;

	if(widget == NULL)
		widget = colormapNameW_;
	XtVaGetValues(widget, XmNvalue, &str, NULL);
	return str;
}

void AvColormapEditor::setNameField(char *name, Widget widget)
{
	if(name == NULL)
		return;
	if(widget == NULL)
		widget = colormapNameW_;
	XtVaSetValues(widget, XmNvalue, name, NULL);
}

AvColormap::COLOR_MODEL AvColormapEditor::colorModel()const
{
	if( greyScale_ != 0)
		return AvColormap::GRAYSCALE;
	else
		return (hsv_ == 0) ? AvColormap::RGB : AvColormap::HSV;
}

AvColormapEditor::~AvColormapEditor()
{
	palette_->unref();
}

// Update aipsview's colormap. If the hardware colormap is read only,
// we'll also have to redraw our color bars.
// (This probably isn't the best place to call drawMap(). it appears
//  to get called several times directly and indirectly from mapExposedCB().
void AvColormapEditor::storeColors(const float *r, const float *g,
				   const float *b,
				   const int start, const int num)
{

	if(palette_ != NULL)
	{	palette_->storeColors( r, g, b, start, num);
		if(palette_->readonly())
		{	drawMap();
			// callbacks may result in our being sent more
			// events. So prevent annoying recursion problems.
			if(autoUpdate() && !busy_)
			{	busy_ = True;
				  invokeCallbacks(MAP_EDITED);
				busy_ = False;
			}
		}
	}
}
