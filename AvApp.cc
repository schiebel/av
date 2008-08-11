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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvApp.cc,v 19.3 2003/10/09 20:46:24 hravlin Exp $
//
// $Log: AvApp.cc,v $
// Revision 19.3  2003/10/09 20:46:24  hravlin
// In getFileNameFromUser() added check for valid directory.
// made fileSort an internal "C" routine to keep Solaris' compiler happy.
//
// Revision 19.2  2003/10/07 21:09:48  hravlin
// Cleanup of fileSearch().
//
// Revision 19.1  2003/09/02 21:06:55  hravlin
// Added our own file search procedure for file dialog.
//
// Revision 19.0  2003/07/16 05:46:22  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:11  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.4  2002/01/30 22:29:15  hravlin
// Initialized two widgets to 0 that theoretically could have been used
// uninitialized.
//
// Revision 17.3  2002/01/22 21:26:51  hravlin
// Copying fallback_resources to a non const string doesn't work because it
// would have to stick around. Just cast away the const instead.
// Removed unused debugging code.
//
// Revision 17.2  2002/01/14 16:59:48  hravlin
// Changed fallback arg to be const char *[]. And modified AvOpenApplication()
// to convert it to non const strings.
// Rearranged processmenu() so break didn't follow exit to get rid of a
// compiler warning.
//
// Revision 17.1  2002/01/03 19:46:28  hravlin
// Removed pthread macros/constants that are now handled on command line.
//
// Revision 17.0  2001/11/12 19:41:39  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:41:50  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.1  2000/12/12 21:55:30  hravlin
// Removed compile restriction for pthreads.
//
// Revision 15.0  2000/10/26 17:07:23  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 18:19:53  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:07:24  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.7  2000/02/16 17:31:06  hravlin
// Fixed problem with CPP macro.
//
// Revision 13.6  1999/10/25 18:45:03  hravlin
// Added reserveX()/releaseX() routines for thread locking.
//
// Revision 13.5  1999/09/28 21:51:29  hravlin
// Fixed up black/white Pixel(). Menus should use Widgets rather than
// gadgets so "Include" works correctly.
//
// Revision 13.4  1999/09/17 16:06:37  hravlin
// Commented out an unused function.
//
// Revision 13.3  1999/09/16 14:25:18  hravlin
// Removed some g++ warnings.
//
// Revision 13.2  1999/09/15 21:42:54  hravlin
// Rewrote initial X connection routines to support visuals other than
// the default. A number of variables that had been retrieved from the
// root window or stored elsewhere are now here.
//
// Revision 13.1  1999/08/25 20:02:20  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:39:01  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:01  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.2  1999/04/19 20:53:42  hr
// Slight update to PseudoColor visual check.
//
// Revision 11.1  1999/04/19 19:48:36  hr
// Aipsview now exits with an error message if the default visual is
// not PseudoColor.
//
// Revision 11.0  1998/10/03 06:57:24  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:50:48  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:22:53  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:14:03  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/01/09 18:19:31  hr
// Changed constructor arg list to include parameters to allow retrieving
// of command line X args.
//
// Revision 7.2  1996/12/12 05:33:26  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.19  1996/06/18  18:45:46  pixton
//  Copyright Update
//
//  Revision 1.18  1996/03/14  18:11:43  baker
//  Split the X color functionality out of AvApp into AvXColorApp.
//
//  Revision 1.17  1996/03/05  17:32:04  hr
//  Reenabled the dummy colormap editor create/delete hack.
//
//  Revision 1.16  1996/03/04  23:46:17  baker
//  Fixed a typo that showed up if PGPLOT is undefined...
//
//  Revision 1.15  1996/02/29  22:55:40  hr
//  The colormap editor doesn't clean up after itself when deleted. Can't
//  use the dummy editor until it does.
//
//  Revision 1.14  1996/02/29  17:09:23  hr
//  The XPalette is now only ref'ed once rather than for each window.
//  Added code to initialize to preallocate the colormap editor's colors.
//
//  Revision 1.13  1996/02/28  21:22:44  hr
//  initialize was calling initializePGColormappings more than once.
//
//  Revision 1.12  1996/02/01  21:44:45  hr
//  Added support for button specific creation parameter to allow creating
//  toggle buttons in the on state.
//
//  Revision 1.11  1996/01/24  17:53:19  hr
//  Added call to initializePGColormappings.
//
//  Revision 1.10  1995/10/26  14:02:40  baker
//  Changed hide and show to refer to the parent class.
//
//  Revision 1.9  1995/10/23  17:20:39  hr
//  In function initializeTop, strip any leading pathname component before
//  copying to appName_.
//
//  Revision 1.8  1995/10/10  23:18:46  baker
//  General cleanup.
//
//  Revision 1.7  1995/10/06  23:56:03  baker
//  Added "QUIT" and "CLOSE" to 'processMenu'.
//
//  Revision 1.6  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.6  1995/09/20  01:52:41  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/04/06  20:52:54  hr
//  ~AvApp was unrefing the palette, but it was only being ref'd during
//  palette creation.
//
//  Revision 1.4  1995/03/30  22:20:01  hr
//  Removed tear off menu support since it caused crashes on
//  Solaris (at least) systems.
//
//  Revision 1.3  1995/03/29  18:22:05  pixton
//  Inserted mini-event loop to allow the file selection box to
//  disappear as soon as OK is pressed.
//
//  Added filter to prevent continueProc_ from being called when
//  the filename is degenerate (NULL or zero length).
//
//  Revision 1.2  1995/03/24  15:46:43  baker
//  *** empty log message ***
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <regex.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <Xm/Xm.h>
#include <Xm/Command.h>
#include <Xm/FileSB.h>
#include <Xm/Form.h>
#include <Xm/MainW.h>
#include <Xm/RowColumn.h>
#include <Xm/TextF.h>
// Use widgets rather than gadgets since gadgets don't have their own
// colors.  ("Include" could have problems).
#include <Xm/CascadeB.h>
#include <Xm/Separator.h>
#include <Xm/ToggleB.h>
#include <Xm/PushB.h>

#include "AvApp.h"
#include "AvMenu.h"
#include "AvXPalette.h"
#include "AvFallbacks.h"
#include "AvMakeDataSet.h"

#ifdef PGPLOT
#include "AvPGDriver.h"
#endif

#if defined(PTHREADS)
#include <semaphore.h>

// Used to limit access to X calls.
static Boolean access_inited = FALSE;
static sem_t access_X;
#endif

XtAppContext AvApp::appContext_ = 0;
Screen * AvApp::screen_ = NULL;
Display * AvApp::display_ = NULL;
Visual *AvApp::visual_ = NULL;
Colormap AvApp::colormap_=0;
AvXPalette *AvApp::palette_;
Window AvApp::referenceWindow_ = 0;
Pixel AvApp::blackPixel_;
Pixel AvApp::whitePixel_;

// Create a window that matches our needs for visual, depth, etc.
// It is small, offscreen and never mapped. Its purpose is to be
// available when a Window or Drawable is needed as an argument to
// some X function.
static Window makeDummyWindow(Widget ref, Visual *visual,
			      int depth, Colormap cmap)
{XSetWindowAttributes attr;

	attr.background_pixmap = None;
	attr.background_pixel = 0;
	attr.border_pixel = 0;
	attr.backing_pixel = 0;
	attr.cursor = None;
	attr.colormap = cmap;

	long mask = 
		CWBackPixmap |CWBackPixel | CWBorderPixel |
		CWBackingPixel | CWColormap | CWCursor;
	Window rootWindow = RootWindowOfScreen(XtScreen(ref));
	Window w = XCreateWindow(
			XtDisplay(ref),
			rootWindow,
			-100, -100,
			1, 1, 0,
			depth,
			// It's declared 0L, but passed uInt.
			(unsigned int)CopyFromParent,	// window class
			visual,
			mask, &attr);
	return w;			
}

const char * AvApp::className () const
{
  return ("AvApp");
}


AvApp::AvApp (const char * appClass, int & argc, String * argv,
	      const char * fallbacks[])
{
  initializeTop (appClass, NULL, 0, argc, argv, fallbacks, NULL, 0);
}

AvApp::AvApp (const char * appClass, XrmOptionDescRec * opt, Cardinal noptions,
	      int &argc, String * argv,
	      const char * fallbacks[],
	      ArgList args, Cardinal nargs,
	      XtPointer varStruct,	// User defined struct to hold vars.
	      XtResource *resources, const int nResources)

{
  initializeTop (appClass, opt, noptions, argc, argv, fallbacks,
		 args, nargs);
  // Fill in the optionVar struct.
  if(varStruct != NULL)
	XtGetApplicationResources(parentWidget_, varStruct,
				  resources, nResources, NULL, 0);
}

static struct CLASSES {
	int	id;
	const char	*name;} classes[] = {
		{StaticGray, "StaticGray"},
		{GrayScale, "GrayScale"},
		{StaticColor, "StaticColor"},
		{PseudoColor, "PseudoColor"},
		{TrueColor, "TrueColor"},
		{DirectColor, "DirectColor"}
		};
static const size_t NCLASSES = (sizeof classes)/(sizeof *classes);

static const char *classToString(const int id)
{size_t i;

	for( i=0; i< NCLASSES; i++)
		if(classes[i].id == id)
			return classes[i].name;
	return "BADCLASS";
}

static void printVisualInfo( XVisualInfo *v, int len=1)
{
	while(len-- > 0)
	{	printf("Visual 0x%p ID: 0x%lx, Screen: %d\n",
			 v->visual, v->visualid, v->screen);
		printf("Depth %d (#map entries %d) %s\n",
			v->depth, v->visual->map_entries,
			classToString(v->c_class));
		printf("\n");
	}
}

#if defined (sgi) && defined(__NEVER__)

static inline int ABSI(const int a)
{ return (((a) >= 0) ? (a) : -(a));}

// Go through a list of visuals and find the visual with at least the
// number of color table entries requested or the one that comes closest.
static XVisualInfo *findMatchingCMap(const int len, XVisualInfo *list,
				     const int mincolors)
{ XVisualInfo *bestMatch = NULL;

	if((len <= 0) || (list == NULL))
		return NULL;

	for(int i=0; i < len; i++)
	{ XVisualInfo *ptr = &list[i];
	  int ncolors = ptr->visual->map_entries;
		if(ncolors == mincolors)
			return ptr;
		else
		if( bestMatch == NULL)
			bestMatch = ptr;
		else
		{ int d1, d2;
			d1 = bestMatch->visual->map_entries - mincolors;
			d2 = ptr->visual->map_entries - mincolors;
			// Prefer cmaps with at least mincolors entries.
			if( (d1 > 0) && (d2 < 0)) continue;
			if( (d1 < 0) && (d2 > 0))
			{	bestMatch = ptr;
				continue;
			}
			// Choose the closest.
			if( (ABSI(d1) > ABSI(d2)))
				bestMatch = ptr;
		}
	}
	return bestMatch;
}
#endif

///// Local implementation of XtOpenApplication modified to make sure
// the initial shell has the correct colormap and visual.
Widget AvApp::AvOpenApplication(XtAppContext *app_context_return,
			 const char *application_class,
			 XrmOptionDescRec* options, Cardinal num_options,
			 int *argc, String *argv,
			 const char *fallback_resources[],
			 WidgetClass widget_class,
			 ArgList args, Cardinal num_args)
{int depth, screenid;
 XVisualInfo *list=NULL, *vinfo=NULL;
 Visual *defaultVisual;

	// Initialize then open a connection to the server.
	XtToolkitInitialize();
	XtAppContext app = XtCreateApplicationContext();
	// If fallback_resources were copied to a non const array, that
	// array would have to remain. So, we just cast away the const.
	if(fallback_resources != NULL)
	  XtAppSetFallbackResources(app, (char **)fallback_resources);

	Display *dpy = XtOpenDisplay(app, NULL, NULL, application_class,
			options, num_options, argc, argv);
	if( dpy == NULL)
	{	cerr << "Could not open X display." << endl;
		exit(1);
	}

	display_ = dpy;
	// Default visual.
	defaultVisual = DefaultVisualOfScreen(DefaultScreenOfDisplay(dpy));

	//////////////////////////////////////////////////////
	////		Choose visual to use.		//////
	//////////////////////////////////////////////////////

	// We want to use the value
	// If a specific visual was requested, use it. Otherwise,
	// use default visual.

	int visualID = getXOptions()->visualID;
	if(visualID >= 0)		// Did user request a specific visual?
	{XVisualInfo tmpl;
	 int mask = VisualIDMask;
	 int len;

		tmpl.visualid = visualID;
		list = XGetVisualInfo(display_, mask, &tmpl, &len);
		if(len <= 0)	// Should be exactly 1.
		{	cerr << "Could not find visual: " << visualID << endl;
			exit(1);
		}
		vinfo = list;
	}
#if defined(sgi) && defined(__NEVER_USE_)
	// This isn't turned on for SGIs since we've decided to let
	// aipsview use the same visual as the root.

	// Sun's Widgets seem to use root window for some things even if
	// they're told not to.
	else	// Is default visual PseudoColor?
	if(defaultVisual->c_class != PseudoColor)
	{XVisualInfo tmpl;
	 tmpl.c_class = PseudoColor;
	 int mask = VisualClassMask;
	 int len;
		// List of PseudoColor visuals.
		list = XGetVisualInfo(display_, mask, &tmpl, &len);
		int cmapLength=getXOptions()->nCMapColors;
		vinfo = findMatchingCMap(len, list, cmapLength);
	}
#endif

	// If the vinfo variable hasn't been set, use info from the default.
	if(vinfo == NULL)
	{XVisualInfo tmpl;
	 tmpl.visualid = defaultVisual->visualid;
	 int mask = VisualIDMask;
	 int len;

		list = XGetVisualInfo(display_, mask, &tmpl, &len);
		if(len <= 0)
		{	cerr << "Could not find visual info." << endl;
			exit(1);
		}
		vinfo = list;
	}	

	visual_ = vinfo->visual;
	depth = vinfo->depth;
	screenid = vinfo->screen;
	if(getXOptions()->colorVerbosity > 0)
		printVisualInfo(vinfo);

	if(list != NULL)
		XFree(list);

	///////////////////////////////////////////////////////

	Window rootwin = RootWindow(display_, screenid); //RootWindow OK here.
	colormap_ = XCreateColormap(display_, rootwin, visual_, AllocNone);

   
	// The X palette is created here so colors currently allocated in
	// the root get copied thereby keeping the same widget colors.
	palette_ = new AvXPalette(display_, visual_, colormap_);
	palette_->ref ();
	palette_->lockResource();	// Make sure it hangs around forever.
#ifdef PGPLOT
	// Need to do a one time initialization of the PGPLOT driver colors.
	AvPGDriver::initializePGColormappings(palette_);
#endif
	Pixel dummy;
	palette_->allocNamedColor("White", dummy, whitePixel_);
	palette_->allocNamedColor("Black", dummy, blackPixel_);

	if(getXOptions()->colorVerbosity > 0)
	{	cout << "Colors available for image: "
		     << palette_->availableColors()
		      << " Colortable length: "
		     << palette_->maxColors() << endl;
	}

	// Create the app shell with needed parameters.
	Widget shell;
	if(num_args == 0)
		shell = XtVaAppCreateShell(/*appname*/NULL, application_class,
				       widget_class, display_,
				       XmNvisual, visual_,
				       XmNdepth, depth,
				       XmNcolormap, colormap_,
					NULL);
	else
	{ int i = num_args;
	  Arg *nargs = new Arg[num_args+3];
		memcpy(nargs, args, sizeof(*args)*num_args);
		XtSetArg(nargs[i], XmNvisual, visual_); i++;
		XtSetArg(nargs[i], XmNdepth, depth); i++;
		XtSetArg(nargs[i], XmNcolormap, colormap_); i++;
		shell = XtAppCreateShell(/*appname*/NULL, application_class,
				       widget_class, display_,
				       nargs, i);
		delete [] nargs;
	}

	referenceWindow_ = makeDummyWindow(shell, visual_, depth, colormap_);
	screen_ = XtScreen(shell);

	if(app_context_return != NULL)
		*app_context_return = app;
	return shell;
}


void AvApp::initializeTop (const char * appClass, XrmOptionDescRec * opt,
			   Cardinal noptions, int & argc, String * argv,
			   const char * fallbacks[],
			   ArgList args, Cardinal nargs)
{
static Boolean initialized = FALSE;

  if(initialized)
  {	cerr << "AvApp::initializeTop: attempt to reinitialize.\n";
	return;
  }
  else
	initialized = TRUE;

#if defined(PTHREADS)
  if(!access_inited)
  {	if(sem_init(&access_X, 0, 1) == 0)
		access_inited = TRUE;
	else
		perror("AvApp::initializeTop:");
  }
#endif

  // If appClass_ and appName_ are still set to the defaults, reset now
  if (!strcmp (appClass_, "Av"))
    strcpy (appClass_, appClass);

  // Strip leading pathname from the application name.
  if (!strcmp (appName_, "Av")) {
    char * name = strrchr (*argv, '/');	
    name = (name == NULL) ? *argv : ++name;
    strcpy (appName_, name);
  }

  parentWidget_ = AvOpenApplication(
		&appContext_, appClass_,
		opt, noptions,
		&argc, argv,
		fallbacks, applicationShellWidgetClass,
		args, nargs);

  initialize ();

  topLevelP_ = TRUE;
}

AvApp::AvApp (Widget parent, int buildInsideParent) :
AvUserComp (parent, buildInsideParent)
{ XtAppContext appContext = 0;

  if(appContext_ == 0)
	  appContext_ = XtWidgetToApplicationContext (parent);
  else
  {	appContext = XtWidgetToApplicationContext (parent);
	if(appContext != appContext_)
		cerr << "Contexts don't match." << endl;
  }

  initialize ();
}

void AvApp::initialize ()
{
#ifdef DEBUG0
  printf ("AvApp::initialize\n");
#endif

  baseWidget_ =
    XmCreateMainWindow (parentWidget_, appClass_, NULL, 0);

  mainWindow_ = baseWidget_;
  fileSearchFlags_ = 0;
}


void AvApp::makeMainMenu (AvMenu * topMenu)
{
  char mb[] = "menuBar";
  mainMenuBar_ = XmCreateMenuBar (baseWidget_, mb, NULL, 0);
  makeMenu (mainMenuBar_, topMenu);
  XtManageChild (mainMenuBar_);
}


void AvApp::go ()
{
  XtAppMainLoop (appContext_);
}

void AvApp::getFileNameFromUser (AvProc p, const char * title,
				 const char * directory, int flags)
{
  // We'll execute this if user clicks 'OK'
  continueProc_ = p;

  // Check to see if user wants to turn off new dialog.
  // Will go away shortly.
  char *frenv = getenv("AV_NEWFILEDIALOG");
  if(frenv)
  {	if(strcmp(frenv, "0") == 0)
	      flags = 0;
	else
	if(strncasecmp(frenv, "r", 1) == 0)
		flags |= FULLREGEX;
  }

  fileSearchFlags_ = flags;

  char fndb[] = "FileNameDialogBox";
  Widget fileNameDialogBox = XmCreateFileSelectionDialog
    (baseWidget_, fndb, NULL, 0);

  // On some systems (Solaris), if directory is invalid, the widget will
  // display the current directory, however it appears that internally it
  // still uses the supplied directory until the 'Filter' button is pressed.
  // (On Linux/LessTif there was an error message and empty lists).
  // We'll just silently ignore the request.
  if(access(directory , R_OK | X_OK) == 0)
  { char *dir = Strdup(directory);
    XmString xStr = XmStringCreateLocalized (dir);
	XtVaSetValues (fileNameDialogBox,
		 XmNdirectory, xStr,
		 NULL);
	delete [] dir;
	XmStringFree (xStr);
  }

  // If we're allowing the user to select directories, use our own file
  // search procedure.
  if(flags & USEDATADIRS)
    XtVaSetValues(fileNameDialogBox,
		XmNuserData, (XtPointer) this,
		XmNfileSearchProc, fileSearchCB,
		NULL);

  XtVaSetValues (XtParent (fileNameDialogBox), XmNtitle, title, NULL);

  XtAddCallback (fileNameDialogBox, XmNokCallback,
		 (XtCallbackProc) &AvApp::fileNameDialogBoxOK,
		 (XtPointer) this);

  XtAddCallback (fileNameDialogBox, XmNcancelCallback,
		 (XtCallbackProc) &AvApp::fileNameDialogBoxCancel,
		 (XtPointer) this);

  XtManageChild (fileNameDialogBox);
}


void AvApp::fileNameDialogBoxOK (Widget w, AvApp * app, XtPointer cData)
{

  // Get the filename.
  char * fName;
  XmFileSelectionBoxCallbackStruct * cbStruct =
    (XmFileSelectionBoxCallbackStruct *) cData;
  XmStringGetLtoR (cbStruct->value, XmSTRING_DEFAULT_CHARSET, &fName);

  if(access(fName, R_OK) != 0)
  {	XtFree(fName);
	return;
  }

  XtRemoveCallback (w, XmNokCallback,
		    (XtCallbackProc) &AvApp::fileNameDialogBoxOK,
		    (XtPointer) app);

  XtRemoveCallback (w, XmNcancelCallback,
		    (XtCallbackProc) &AvApp::fileNameDialogBoxCancel,
		    (XtPointer) app);

  // Get rid of the file selection box.
  XtUnmanageChild (w);
  XtDestroyWidget (w);

  // Insert mini-event handler so fileSelectionDialogBox can go away
  // right now.
  while(XtAppPending(appContext_)) {
    XEvent event;
    XtAppNextEvent(appContext_, &event);
    XtDispatchEvent(&event);
  }
  
  // If filename is okay, continue with the client-supplied procedure.
  if (fName)
    if (strlen (fName) > 0)
    app->continueProc_ (app, fName);
}

void AvApp::fileSearchCB(Widget widget, XtPointer data)
{
	if(widget != 0)
	{ AvApp *me;

		XtVaGetValues(widget,
			XmNuserData, &me,
			NULL);
		if(me != 0)
			me->fileSearch(widget, data);
	}
}

/* Make sure there's enough space to hold a string. */
static char **chklen(char **ary, int &maxlen, int newlen)
{
	if(ary == 0)
	{	ary = new char *[newlen];
		maxlen = newlen;
	}
	else
	if(newlen > maxlen)
	{char **ary2;
	 int nlen = maxlen*2;
		ary2 =  new char *[nlen];
		for(int i=0; i<maxlen; i++)
			ary2[i] = ary[i];
		delete [] ary;
		ary = ary2;
		maxlen = nlen;
	}
	return ary;
}

extern "C" {

static int fileSort(const void *sv1, const void *sv2)
{ const char **s1 = (const char **)sv1;
  const char **s2 = (const char **)sv2;
  const char *str1 = *s1;
  const char *str2 = *s2;
  int cmp;

	cmp = strcasecmp(str1, str2);
	return cmp;
}

}

/* File search replacement for FileRequester Widget.
   Called when user changes directories.
 */
void AvApp::fileSearch(Widget widget, XtPointer data)
{ char        *mask=0, *dirname;
  XmString    *xmfilenames=0;
  char	**filenames=0, *maskbuffer=0;
  int	nfiles = 0, filenamesLen=0;
  int	i = 0, err;
  DIR	*dirptr;
  struct dirent *dire;
  regex_t	patternbuffer;
  int		patternflags = REG_ICASE | REG_NOSUB | REG_NEWLINE;


  XmFileSelectionBoxCallbackStruct *cbs =
			 (XmFileSelectionBoxCallbackStruct *) data;

  //	printf("+++++++++++++++++++++++++++* file_search++++++++++++*\n");

	/* Get directory to check. */
	if(!XmStringGetLtoR (cbs->dir, XmFONTLIST_DEFAULT_TAG, &dirname))
		return;	/* Error. */

	/* Get mask. If there's an error, keep going. */
	if(!XmStringGetLtoR(cbs->mask, XmFONTLIST_DEFAULT_TAG, &mask))
        	mask[0] = 0;

	/* The filter mask is not a normal regular expression, so create
	   a buffer to hold the mask that has been converted to one.
	   The conversion consists of changing "*" to ".*" and "." to "\.".
	*/
	if((fileSearchFlags_ & FULLREGEX) == 0)
	{int masklen = strlen(mask);
	 int mbi = 0, mi =0;
		maskbuffer = new char[masklen*2+1];
		for(mi=0; mi < masklen; mi++, mbi++)
		{char c = mask[mi];
			if(c == '*')
				maskbuffer[mbi++] = '.';
			else
			if(c == '.')
				maskbuffer[mbi++] = '\\';

			maskbuffer[mbi] = c;
		}
		maskbuffer[mbi] = 0;
	}
	else	/* Don't emulate Motif style wildcards. */
		maskbuffer = mask;

	/* Compile pattern into regex pattern. */
	err = regcomp(&patternbuffer, maskbuffer, patternflags);
	if(err != 0)
	{	printf("Regexc failed with %d for %s\n", err, mask);
	}

	/* Open the directory and read then check each file in it. */
	dirptr = opendir(dirname);
	if(dirptr == 0)
	{	perror(dirname);
		/* Free allocated stuff. */
		regfree(&patternbuffer);
		if(maskbuffer && (maskbuffer != mask))
			delete [] maskbuffer;
		if(mask)
			XtFree(mask);
		return;
	}

	/* Initialize name buffer. */
	filenames = chklen(filenames, filenamesLen, 1024);

	while((dire = readdir(dirptr)) != 0)
	{ char *fn = dire->d_name;
	  char nambuf[BUFSIZ], *nptr;

	/* Defining REMOVE_DOT will cause the '.' to removed from the
	   pathname when the current directory matches. The problem is
	   that the file list window adjusts things so the pathname is
	   obscured so it looks like the entry is empty unless the window
	   is scrolled. Better to just leave it in.
	 */
/*#define REMOVE_DOT*/
#if defined(REMOVE_DOT)
	  int isdot = 0;
#endif
		/* Don't allow parent dir here. */
		if(strcmp(fn, "..") == 0)
			continue;	
		/* But we'll allow current dir (.).*/
#if defined(REMOVE_DOT)
		if(strcmp(fn, ".") == 0)
			isdot = 1;
#endif

		if(*fn != '/')
		{	strcpy(nambuf, dirname);
			/*strcat(nambuf, "/");*/
			strcat(nambuf, fn);
			nptr = nambuf;
		}
		else	/* Probably never gets called. */
		{	nptr = fn;
		}

		/* If the file is a 'data' file and it matches the filter
		   request, then keep it. If there was a bad regular expr,
		   keep all data files.
		*/
		if(AvMakeDataSet::isDataSet(nptr))
		{int rerr;
		 regmatch_t mbuf;	/* Dummy - not used. */
			rerr = regexec( &patternbuffer, nptr, 1, &mbuf, 0);

			if((err != 0) || (rerr == 0))
			{
#if defined(REMOVE_DOT)
				/* Convert "<path>/." to <path> */
				if(isdot)
				{ int len = strlen(nptr);
					nptr[len-2] = 0;
				}
#endif
				filenames = chklen(filenames,
						    filenamesLen, nfiles+1);
				filenames[nfiles++] = strdup(nptr);
			}
#if 0
			else
			 printf("No match(%d): |%s|\n", rerr, nptr);
#endif
		}
	}
	closedir(dirptr);
	regfree(&patternbuffer);
	if(maskbuffer && (maskbuffer != mask))
		delete [] maskbuffer;
	
	if(mask)
		XtFree(mask);

	// If there is a list of files, put them in the list widget.
	if(nfiles)
	{	xmfilenames = new XmString[nfiles];
		/* sort list, then convert to XmString. */
		qsort(filenames, nfiles, sizeof(*filenames), fileSort);
		for(i=0; i< nfiles; i++)
		{	xmfilenames[i] = XmStringCreateLocalized(filenames[i]);
			free(filenames[i]);
		}
		delete [] filenames;

	        XtVaSetValues (widget,
        	    XmNfileListItems,      xmfilenames,
	            XmNfileListItemCount,  nfiles,
        	    XmNdirSpec,            xmfilenames[0],
	            XmNlistUpdated,        TRUE,
        	    NULL);

	        while (nfiles > 0)
        	    XmStringFree (xmfilenames[--nfiles]);
		delete [] xmfilenames;
	} else
        	XtVaSetValues (widget,
	            XmNfileListItems,      NULL,
        	    XmNfileListItemCount,  0,
	            XmNlistUpdated,        TRUE,
	            NULL);

//	printf("---------------------------* file_search------------*\n");
}

#if 0
/* Function to compare two strings. Called by qsort. */
int AvApp::fileSort(const void *sv1, const void *sv2)
{ const char **s1 = (const char **)sv1;
  const char **s2 = (const char **)sv2;
  const char *str1 = *s1;
  const char *str2 = *s2;
  int cmp;

	cmp = strcasecmp(str1, str2);
	return cmp;
}
#endif

void AvApp::fileNameDialogBoxCancel (Widget w, AvApp *, XtPointer)
{
  XtUnmanageChild (w);
  XtDestroyWidget (w);
}


void AvApp::updatePathName (char * path, char * fName)
{
  // where is separation between path name and file name?
  char * f = strrchr (fName, '/');
  if (f) {
    int l = (int)(strlen (fName) - strlen (f));
    strncpy (path, fName, l);
    path[l] = '\0';
  }
}


void AvApp::processMenu (char * buttonName, int choice, Widget)
{
  switch (choice) {
  case CLOSE:
  case SV_CLOSE:
    //    XtUnmanageChild (parentWidget_);
    hide ();
    break;
  default:
    cerr << "Menu choice \"" << buttonName <<
      "\" (code " << choice << ") not implemented" << endl;
    break;
  case QUIT:
  case SV_QUIT:
    XtDestroyWidget (parentWidget_);
    XtCloseDisplay (display_);
    exit (0);
  }
}


void AvApp::helpCB (Widget, int, XtPointer)
{
}


void AvApp::makeMenu (Widget parent, AvMenu * menu)
{
  AvMenuItem * menuInfo = menu->items();
  int nItems = menu->howMany();

  // Record the menu's parent Widget.
  menu->parentWidget_ = parent;

  int i;
  WidgetList buttons = 0;

  XmString xStr;

  int nSeparators = 0;

  if (nItems > 0) 
    buttons = (WidgetList) XtMalloc (nItems * (int)sizeof (Widget));

  for (i=0; i<nItems; i++) {
    switch (menuInfo[i].buttonType) {
    case SEPARATOR:
      if (menuInfo[i].buttonType == SEPARATOR) {
	char sepnam[] = "";
	XtManageChild (XmCreateSeparator (parent, sepnam, NULL, 0));
	nSeparators++;
      }
      break;
    case HELP_BUTTON:
    case SUBMENU_BUTTON:
      {
	buttons [i-nSeparators] =
	  XmCreateCascadeButton
	    (parent, menuInfo[i].buttonName, NULL, 0);
	char pdm[]="";
	Widget subMenuParent = XmCreatePulldownMenu (parent, pdm, NULL, 0);
	XtVaSetValues (buttons[i-nSeparators],
		       XmNsubMenuId, subMenuParent,
		       NULL);
	if (menuInfo[i].buttonType == HELP_BUTTON)
	  XtVaSetValues
	    (parent, XmNmenuHelpWidget, buttons[i-nSeparators], NULL);
	makeMenu
	  (subMenuParent, menuInfo[i].subMenu);
	break;
      }
    case PUSH_BUTTON:
      {
	xStr = XmStringCreateLocalized (menuInfo[i].buttonName);
	buttons[i-nSeparators] =
	  XtVaCreateWidget ("", xmPushButtonWidgetClass, parent,
			    XmNlabelString, xStr,
			    NULL);
	XmStringFree (xStr);
	menuInfo[i].app = this;
	menuInfo[i].button = buttons[i-nSeparators];
	XtAddCallback
	  (buttons[i-nSeparators], XmNactivateCallback,
	   (XtCallbackProc) &AvApp::processMenuCB,
	   (XtPointer) &menuInfo[i]);
	break;
      }
    case TOGGLE_BUTTON:
      {
	xStr = XmStringCreateLocalized (menuInfo[i].buttonName);
	buttons[i-nSeparators] =
	  XtVaCreateWidget ("toggleButton", xmToggleButtonWidgetClass, parent,
			    XmNlabelString, xStr,
			    XmNvisibleWhenOff, TRUE,
			    XmNindicatorSize, 12,
			    XmNset, (menuInfo[i].bdata == 0) ? FALSE : TRUE,
			    NULL);
	XmStringFree (xStr);
	menuInfo[i].button = buttons[i-nSeparators];
	menuInfo[i].app = this;
	XtAddCallback
	  (buttons[i-nSeparators], XmNvalueChangedCallback,
	   (XtCallbackProc) &AvApp::processMenuCB,
	   (XtPointer) &menuInfo[i]);
	break;
      }
    }
  }
  if (nItems > 0) {
    XtManageChildren (buttons, nItems - nSeparators);
    XtFree ((char *) buttons);
  }
}


Widget AvApp::addMenuButton (Widget parent, AvMenuItem * newItem)
{
  XmString xStr = XmStringCreateLocalized (newItem->buttonName);
  Widget w=0;
  switch (newItem->buttonType) {
  case PUSH_BUTTON:
    w = XtVaCreateManagedWidget ("", xmPushButtonWidgetClass, parent,
				 XmNlabelString, xStr,
				 NULL);
    break;
  case TOGGLE_BUTTON:
    w = XtVaCreateManagedWidget ("", xmToggleButtonWidgetClass, parent,
				 XmNlabelString, xStr,
				 XmNvisibleWhenOff, TRUE,
				 XmNindicatorSize, 12,
				 NULL);
    break;
  }
  XmStringFree (xStr);
  newItem->app = this;
  newItem->button = w;
  return (w);
}

void AvApp::processMenuCB (Widget, AvMenuItem * buttonInfo, XtPointer)
{
  AvApp * app = (AvApp *) buttonInfo->app;
  app->processMenu
    (buttonInfo->buttonName, buttonInfo->data, buttonInfo->button);
}

AvApp::~AvApp ()
{
}

// Lock access to X routines or wait until available.
void AvApp::reserveX()
{
#if defined(PTHREADS)
	if(sem_wait(&access_X) != 0)
		perror("AvApp::reserveX:");
#endif
}

// Free the X routines.
void AvApp::releaseX()
{
#if defined(PTHREADS)
	if(sem_post(&access_X) != 0)
		perror("AvApp::reserveX:");
#endif
}

char *AvApp::Strdup(const char *s) // For converting const char* to char*.
{ char *str;

	if(s == NULL)
		str = new char[1];
	else
	{	str = new char[strlen(s)+1];
		strcpy(str, s);
	}
	return str;
}
