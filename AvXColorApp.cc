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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvXColorApp.cc,v 19.0 2003/07/16 05:47:07 aips2adm Exp $
//
// $Log: AvXColorApp.cc,v $
// Revision 19.0  2003/07/16 05:47:07  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:50  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.2  2002/01/14 17:54:05  hravlin
// Changed fallbacks arg to be const char *[].
//
// Revision 17.1  2002/01/03 22:00:04  hravlin
// Added externalDrawingArea() (support for wiregl).
//
// Revision 17.0  2001/11/12 19:42:19  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:27  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:57  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:20:06  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:08:11  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.3  1999/09/28 19:37:25  hravlin
// Redid set/reset/toggle busy() to avoid recursion.
//
// Revision 13.2  1999/09/27 15:47:12  hravlin
// Made busy_ a counter rather than a boolean.
//
// Revision 13.1  1999/09/15 20:59:26  hravlin
// Changes to support TrueColor visuals.
//
// Revision 13.0  1999/08/10 18:39:58  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:34  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:39  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:28  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:27:04  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:29  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.4  1997/02/05 18:07:00  hr
// Changed an int/unsigned comparision.
//
// Revision 7.3  1997/01/09 18:20:35  hr
// Changed constructor arg list to include parameters to allow retrieving
// of command line X args.
//
// Revision 7.2  1996/12/12 07:46:52  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.5  1996/06/18  18:46:47  pixton
//  Copyright Update
//
//  Revision 1.4  1996/04/10  21:06:03  hr
//  Removed toggling background color from busy.
//
//  Revision 1.3  1996/04/06  18:24:50  hr
//  Added set/reset busy.
//
//
//---------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <Xm/Xm.h>
#include <Xm/FileSB.h>

#include "AvXPalette.h"
#include "AvColormapEditor.h"
#include "AvImageWindow.h"
#include "AvImageView.h"
#include "AvWidgetMisc.h"

AvXPalette * AvXColorApp::palette_ = NULL;
int AvXColorApp::busy_ = 0;

const char * AvXColorApp::className () const
{
  return ("AvXColorApp");
}


AvXColorApp::AvXColorApp(const char * appClass,
			 int & argc, String * argv, const char * fallbacks[])
  : AvApp (appClass, argc, argv, fallbacks)
{
  initialize ();
}

AvXColorApp::AvXColorApp(const char * appClass,
			 XrmOptionDescRec * opt, Cardinal noptions,
			 int & argc, String * argv,
			 const char * fallbacks[],
			 ArgList args, Cardinal nargs,
			 // User defined struct to hold vars.
			 XtPointer varStruct,
			 XtResource *resources, const int nResources)
  : AvApp(appClass, opt, noptions, argc, argv, fallbacks, args, nargs,
	  varStruct,resources, nResources)
{
  initialize ();
}


AvXColorApp::AvXColorApp (Widget parent, int buildInsideParent)
: AvApp (parent, buildInsideParent)
{
  initialize ();
}

void AvXColorApp::initialize ()
{
  busyStrings_ = NULL;
  nbusyStrings_ = 0;

  if (!palette_) {

    palette_ = AvApp::palette();

    palette_->checkAllocated();
  }
}

Colormap AvXColorApp::colormap ()
{
  return palette_->colormap ();
}

int AvXColorApp::maxColors () 
{
  return (palette_->maxColors ()); 
}

int AvXColorApp::availableColors () 
{ 
  return (palette_->availableColors ()); 
}

void AvXColorApp::syncColormap () 
{
  palette_->checkAllocated (); 
}

AvXPalette *AvXColorApp::palette ()
{
  return palette_;
}


AvXColorApp::~AvXColorApp ()
{
//  if (palette_ != NULL)
//    palette_->unref();
}


// Wait until all X events have been dealt with.
static void stall()
{
XtAppContext appContext = AvApp::appContext();
  while(XtAppPending(appContext)) {
    XEvent event;
    XtAppNextEvent(appContext, &event);
    XtDispatchEvent(&event);
  }
}

// Reverse the foreground and background colors of a widget.
// Gadgets need not apply.
#ifdef TOGGLECOLORS
static void toggleFGBG(Widget w)
{
Pixel	fg, bg;

	if((w == NULL) || (XtWindow(w)== 0) || !XtIsWidget(w))
		return;
	XtVaGetValues (w,
		XmNbackground, &bg,
		XmNforeground, &fg,
		NULL);
	XtVaSetValues (w,
		XmNbackground, fg,
		XmNforeground, bg,
		NULL);
}
#endif

#ifdef CHECK_for_BUSY_STR
static Boolean isBusyStr(XmString str)
{ char *txt;
	XmStringGetLtoR(str, XmSTRING_DEFAULT_CHARSET, &txt);
	if(strcmp(txt, "Busy") == 0)
		return True;
	else
		return False;
}
#endif

// Set the visual indication that this window is busy by flipping the
// menubar's foreground and background colors and changing the menu labels
// to "Busy".

void AvXColorApp::setBusyToggle(const Boolean set)
{
Cardinal	nchildren;
WidgetList	wl;

	// List of menuBar_'s children and how many there are.
	XtVaGetValues(mainMenuBar_,
		XmNchildren, &wl,
		XmNnumChildren, &nchildren,
		NULL);

	if(set)	// Save current labels and set to "Busy".
	{ // Place to store normal strings.
	  busyStrings_ = new XmString[nchildren];
	  nbusyStrings_ = nchildren;
	  XmString busystr = AvXmStringCreateLocalized("Busy");

		for(Cardinal i=0; i< nchildren; i++)
		{ XmString str;
			XtVaGetValues(wl[i],
				XmNlabelString, &str,
				NULL);

#ifdef CHECK_for_BUSY_STR
			// Consistency check to see if somehow the string
			// "Busy" is about to be saved. (& overwriting
			//  the real string).
			if(isBusyStr(str))
			{ char *txt;
			 XmStringGetLtoR(str, XmSTRING_DEFAULT_CHARSET, &txt);
			 fprintf(stderr,
"AvXColorApp::setBusyToggle: Tried to replace %s (index=%d) with \"Busy\"\n",
				 txt, i);
			}
			else
#endif
				busyStrings_[i] = str;
			XtVaSetValues(wl[i], XmNlabelString, busystr, NULL);
		}
		XmStringFree(busystr);

#ifdef TOGGLECOLORS
	toggleFGBG(mainMenuBar_);	// Flip fg, bg colors.
#endif
	}
	else	// Restore old labels.
	if(busyStrings_ != NULL) // NULL => has just been created.
	{	// Check to make sure that something hasn't gone wrong.
		// (Like the menu bar being changed while busy).
		if(nchildren != nbusyStrings_)
		{	fprintf(stderr,
	 "AvXColorApp::setBusyToggle: # of children changed from %d to %d.\n",
				nbusyStrings_, nchildren);
			if(nchildren > nbusyStrings_)
				nchildren = nbusyStrings_;
		}

		for(Cardinal i=0; i< nchildren; i++)
			XtVaSetValues(wl[i],
				XmNlabelString, busyStrings_[i], NULL);

		delete [] busyStrings_;
		busyStrings_ = NULL;
		nbusyStrings_ = 0;

#ifdef TOGGLECOLORS
	toggleFGBG(mainMenuBar_);	// Flip fg, bg colors.
#endif
	}
}


// Increment the global busy flag. If aipsview has just become busy
// (busy_ was 0), set the busy indicators. If called while busy>0,
// increment the counter, but don't change labels since they've
// already been changed.
void AvXColorApp::setBusy()
{
	if(busy_++ == 0)	// Only change if currently not busy.
	{AvImageViewItem **views;
	 int		viewcount, i;
	 AvImageView::ItemSelection which=AvImageView::ALL;

	 	// Toggle the main window.
		AvImageView *iv = AvImageView::instance();
		iv->setBusyToggle(TRUE);

		// Set toggle on all image windows.
		views = iv->getImageViewList(viewcount, which);
		for(i=0; i< viewcount; i++)
		{AvImageViewItem *vi = views[i];
			vi->getImageWindow()->setBusyToggle(True);
		}
		delete [] views;

		// Both of these seem to be necessary.
		XSync(XtDisplay(iv->mainMenuBar_), False);
		stall();
	}
}

// Decrement the busy flag. If the flag goes to 0, turn off the busy
// indicators. Ignore if not currently busy.
void AvXColorApp::resetBusy()
{
	if(busy_ > 0)
	{	busy_ -= 1;

		if( busy_ == 0)
		{ AvImageView *iv = AvImageView::instance();
		  AvImageViewItem **views;
		  int		viewcount, i;
		  AvImageView::ItemSelection which=AvImageView::ALL;

		  	iv->setBusyToggle(FALSE); // Clear main window busy.
	  
			// Reset all image windows.
			views = iv->getImageViewList(viewcount, which);
			for(i=0; i< viewcount; i++)
			{AvImageViewItem *vi = views[i];
				vi->getImageWindow()->setBusyToggle(False);
			}
			delete [] views;
		}
	}
}

// When linked with wiregl, the drawing area is not part of a widget
// we control. A number of routines need to know that.
// The routine is here since most places have access to it.
Boolean AvXColorApp::externalDrawingArea()
{
#if defined AVOGL
	return AvGLCanvas::externalDrawingArea(); 
#else
	return FALSE;
#endif
}

