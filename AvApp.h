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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvApp.h,v 19.2 2003/10/09 20:49:45 hravlin Exp $
//
// $Log: AvApp.h,v $
// Revision 19.2  2003/10/09 20:49:45  hravlin
// Removed fileSort() from class.
//
// Revision 19.1  2003/09/02 21:07:35  hravlin
// Added interfaces for local file search dialog support.
//
// Revision 19.0  2003/07/16 05:46:24  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:13  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/14 17:01:41  hravlin
// Changed fallback arg to be const char *[].
//
// Revision 17.0  2001/11/12 19:41:40  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:41:51  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:07:26  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 18:19:58  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:07:25  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.3  1999/10/25 18:45:22  hravlin
// Added reserveX()/releaseX() routines for thread locking.
//
// Revision 13.2  1999/09/28 21:52:21  hravlin
// Redefined white/blackPixel_.
//
// Revision 13.1  1999/09/15 21:42:05  hravlin
// Added several variables to standin for defaults from the root window.
//
// Revision 13.0  1999/08/10 18:39:03  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:03  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:57:28  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:50:53  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:23:01  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:14:08  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/01/09 18:20:18  hr
// Changed constructor arg list to include parameters to allow retrieving
// of command line X args.
//
// Revision 7.2  1996/12/12 05:35:08  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.8  1996/06/18  18:42:36  pixton
//  Copyright Update
//
//  Revision 1.7  1996/06/18  18:41:20  pixton
//  Copyright Update
//
//  Revision 1.6  1996/03/14  18:10:37  baker
//  Split the X color functionality out of AvApp into AvXColorApp.
//
//  Revision 1.5  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/08/28  14:37:30  baker
//  Added QUIT and CLOSE to enumeration.
//
//  Revision 1.3  1995/07/17  19:56:12  hr
//  Added 'virtual' to destructor of virtual classes.
//
//  Revision 1.2  1995/03/24  15:44:06  baker
//  Added access to mainMenuBar_ widget
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#ifndef _SV_APP_H
#define _SV_APP_H

#include "AvMenu.h"
#include "AvUserComp.h"
class AvXPalette;

class AvApp : public AvUserComp {

  friend class AvPanel;

public:

  enum { CLOSE, QUIT, SV_CLOSE, SV_QUIT };

  virtual const char * className () const;

  // start it up.  Calls XtAppMainLoop. 
  virtual void go ();

  // returns the base widget (the mainWindow) for the component
  virtual Widget baseWidget () { return (baseWidget_); }

  static XtAppContext appContext () { return (appContext_); }

  static Display * display () { return (display_); }

  static Screen * screen () { return (screen_); }

  static Visual *visual() { return visual_; }
  static AvXPalette *palette() {return palette_;}
  ////////////////////////////////////////////////////////////////
  // When the root visual is not PseudoColor, aipsview needs several
  // variables setup.
  static Colormap colormap(){return colormap_;}
  // referenceWindow takes the place of the X RootWindow<xxx> functions.
  // It's needed because the root window may not be of the right class
  // or depth. (eg TrueColor).
  static Window referenceWindow() {return referenceWindow_;}
  static Pixel blackPixel() {return blackPixel_;}
  static Pixel whitePixel() {return whitePixel_;}
  static char *Strdup(const char *s); // For converting const char* to char*.

  ////////////////////////////////////////////////////////////////
  static void reserveX();	// For threads calling X Routines.
  static void releaseX();
protected:

  // Constructor to use when a connection to X is already established,
  // supplying a parent Widget.  If 'buildInsideParent' == TRUE, build
  // this class within 'parent'.  Otherwise, make a topLevel shell and
  // build this class within the new shell.
  AvApp (Widget parent, int buildInsideParent = TRUE);

  // Constructor that makes a connection to X using this application
  // class name, argc, argv, and this set of fallback resources.
  // A topLevel shell is created, and this class is built within
  // that shell.  Resources for this application can be put in a file
  // named 'appClass'.
  AvApp(const char * appClass, int & argc, String * argv,
	const char * fallbacks[] = NULL);

  // Alternative to above but passes all options to XtAppInitialize
  AvApp (const char * appClass, XrmOptionDescRec * opt, Cardinal noptions,
	 int & argc, String * argv, const char * fallbacks[],
	 ArgList args, Cardinal nargs,
	 // Arguments to XtGetApplicationResources
	 XtPointer varStruct=NULL,	// User defined struct to hold vars.
	 XtResource *resources=NULL, const int nResources=0);

  // X application context and related info.
  static XtAppContext appContext_;
  static Display * display_;
  static Screen * screen_;
  static Visual *visual_;
  static Colormap colormap_;	//Aipsview's colormap.
  static AvXPalette *palette_;
  static Window referenceWindow_;
  static Pixel blackPixel_;
  static Pixel whitePixel_;

  Widget mainWindow_, workArea_;

  // Init routine for establishing basic application functionality --
  // creates base widgets, stores X parameters for Display, Screen, etc.
  virtual void initialize ();


  // ---------- MENU MAKING AND HANDLING --------------------
  // The menu bar widget.
  Widget mainMenuBar_;

  // Constructs the menu for the main menu bar, including submenus.
  void makeMainMenu (AvMenu * topMenu);

  // Add a new button to an existing menu and return the widget.
  // Useful for making dynamically growing menus.
  Widget addMenuButton (Widget parent, AvMenuItem * newItem);

  virtual void processMenu (char * buttonName, int choice, Widget menuButton);


  // ---------- FILE SELECTION DIALOGS --------------------
  // Convenience routines to handle getting filenames from user.
  // Label the selection dialog box with 'title', and set its
  // directory string to 'directory'.  When the user has hit 'OK',
  // execute procedure 'p', stored as 'continueProc_'.
  // The flags variable is used to indicate whether to check for directories
  // that should be treated as data files. (And potentially other things).
  // USEDATADIRS	1 means check directories to see if they should be
  //			  treated as data files.
  // FULLREGEX		1 means use normal regular expression handling.
  //			  Normally, "*" and "." are converted to ".*" and "\.".
  enum FileFlags {USEDATADIRS=1, FULLREGEX=2};
  virtual void getFileNameFromUser (AvProc p, const char * title, const
				    char * directory, int fileFlags=0);
  static void fileNameDialogBoxOK (Widget w, AvApp * app, XtPointer cData);
  static void fileNameDialogBoxCancel (Widget w, AvApp * app, XtPointer cData);
  static void fileSearchCB(Widget widget, XtPointer search_data);
  void fileSearch(Widget widget, XtPointer search_data);
  //  static int fileSort(const void *fn1, const void *fn2);
  /* Returns 1 if file is a regular file or a special directory. (eg. A
     Miriad or AIPS++ data set). Otherwise 0.
  */
  int isDataFile(const char *filename);

  AvProc continueProc_;
  int	fileSearchFlags_;

  // Convenience routine to take a fully specified file name
  // and update 'path' to the directory containing the file.
  void updatePathName (char * path, char * fileName);

  static void processMenuCB (Widget, AvMenuItem * buttonInfo, XtPointer);
  static void helpCB (Widget, int, XtPointer);

  virtual ~AvApp ();

private:

  void makeMenu (Widget, AvMenu *);

  // init routine for establishing basic application functionality --
  // creates base widgets, stores X parameters for Display, Screen, etc.
  void initializeTop (const char * appClass, XrmOptionDescRec * opt,
		      Cardinal noptions, int &argc, String * argv,
		      const char * fallbacks[],
		      ArgList args, Cardinal nargs);

	Widget AvOpenApplication(XtAppContext *app_context_return,
			 const char *application_class,
			 XrmOptionDescRec* options, Cardinal num_options,
			 int *argc, String *argv,
			 const char *fallback_resources[],
			 WidgetClass widget_class,
			 ArgList args, Cardinal num_args);
};

#endif
