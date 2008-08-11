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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvUserComp.cc,v 19.1 2003/08/18 19:15:36 hravlin Exp $
//
// $Log: AvUserComp.cc,v $
// Revision 19.1  2003/08/18 19:15:36  hravlin
// In getStringResource() made default '""' rather than '"NULL"' so file
// requester wouldn't have a "NULL" in the directory path.
//
// Revision 19.0  2003/07/16 05:46:32  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:19  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/22 22:22:57  hravlin
// Added casts to avoid warnings from 64 bit compilers.
//
// Revision 17.0  2001/11/12 19:41:47  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:41:58  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:07:54  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:26:50  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:07:33  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.4  2000/02/29 22:26:34  hravlin
// Added a couple of casts to keep a compiler happy about assigning to an
// X struct.
//
// Revision 13.3  1999/10/04 15:04:11  hravlin
// Added the isHidden_ variable.
//
// Revision 13.2  1999/10/01 22:54:00  hravlin
// Changed Gadgets to Widgets.
//
// Revision 13.1  1999/09/15 21:07:27  hravlin
// Use global values rather than those from the root window.
//
// Revision 13.0  1999/08/10 18:39:12  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:20  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:57:53  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:51:24  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:23:47  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:14:33  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 07:35:49  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.7  1996/06/18  18:46:39  pixton
//  Copyright Update
//
//  Revision 1.6  1996/03/29  15:10:08  baker
//  Added 'isReturnKey' and 'getTextField' methods.
//
//  Revision 1.5  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/08/24  21:27:27  baker
//  *** empty log message ***
//
//  Revision 1.3  1995/08/11  15:23:05  baker
//  Added convenience functions for making push and toggle buttons
//  using pixmaps.
//
//  Revision 1.2  1995/08/08  14:41:52  baker
//  Overloaded 'setLabelString' to accept integers.
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <Xm/Xm.h>
#include <X11/keysym.h>
#include <X11/Shell.h>
#include <Xm/MwmUtil.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>

#include "AvUserComp.h"
#include "AvApp.h"
#include "AvWidgetMisc.h"

char AvUserComp::appClass_[32] = "Av";
char AvUserComp::appName_[32] = "Av";


const char * AvUserComp::className () const
{
  return ("AvUserComp");
}


AvUserComp::AvUserComp (Widget parent, int buildInsideParent)
{
  if (buildInsideParent) {
    topLevelP_ = FALSE;
    parentWidget_ = parent;
  }
  else {
    topLevelP_ = TRUE;
    // Make a topLevelShell, and make it resizable.
    Visual *visual = AvApp::visual();
    Colormap cmap = AvApp::colormap();
    if((visual != NULL) && (cmap != 0))
     parentWidget_ = XtVaAppCreateShell
      (appName_,
       appClass_,
       topLevelShellWidgetClass,
       XtDisplay (parent),
	XmNvisual, visual,
	XmNcolormap, cmap,
	NULL);
    else
     parentWidget_ = XtAppCreateShell
      (appName_,
       appClass_,
       topLevelShellWidgetClass,
       XtDisplay (parent),
       NULL, 0);

    XtVaSetValues (parentWidget_, XmNallowShellResize, TRUE, NULL);
  }

  baseWidget_ = NULL;
  isHidden_ = True;	// Initially it is not showing.
			// (This is a late edition and is currently (10/99)
			//  only used for controlling the ColormapEditor).
}


int AvUserComp::getColorResource (const char * name)
{
  componentResources res;

  XtResource resourceList[1];
  char *strtmp = new char[strlen(name)+1];
  strcpy(strtmp, name);
  resourceList[0].resource_name = strtmp;
  resourceList[0].resource_class = strtmp;
  resourceList[0].resource_type = XtRPixel;
  resourceList[0].resource_size = (Cardinal)sizeof(Pixel);
  resourceList[0].resource_offset = XtOffset (componentResources *, pixel);
  resourceList[0].default_type = XtRString;
  resourceList[0].default_addr = (void *)"White";

  XtGetSubresources
    (parentWidget_, (XtPointer) &res, "", "",
     resourceList, XtNumber (resourceList), NULL, 0);

  delete [] strtmp;
  return ((int) res.pixel);
}


int AvUserComp::getIntegerResource (const char * name)
{
  componentResources res;

  XtResource resourceList[1];
  char *strtmp = new char[strlen(name)+1];
  strcpy(strtmp, name);

  resourceList[0].resource_name = strtmp;
  resourceList[0].resource_class = strtmp;
  resourceList[0].resource_type = XtRInt;
  resourceList[0].resource_size = (Cardinal)sizeof(int);
  resourceList[0].resource_offset = XtOffset (componentResources *, i);
  resourceList[0].default_type = XtRImmediate;
  resourceList[0].default_addr = 0;

  XtGetSubresources
    (parentWidget_, (XtPointer) &res, "", "",
     resourceList, XtNumber (resourceList), NULL, 0);

  delete [] strtmp;
  return ((int) res.i);
}


char * AvUserComp::getStringResource (const char * name)
{
  componentResources res;

  XtResource resourceList[1];
  char *strtmp = new char[strlen(name)+1];
  strcpy(strtmp, name);

  resourceList[0].resource_name = strtmp;
  resourceList[0].resource_class = strtmp;
  resourceList[0].resource_type = XtRString;
  resourceList[0].resource_size = (Cardinal)sizeof(String);
  resourceList[0].resource_offset = XtOffset (componentResources *, str);
  resourceList[0].default_type = XtRString;
  // Can't be a null pointer since some callers expect a valid string.
  // Use '""' since file requester works with it.
  resourceList[0].default_addr = (void *)"";

  XtGetSubresources
    (parentWidget_, (XtPointer) &res, "", "",
     resourceList, XtNumber (resourceList), NULL, 0);
  delete [] strtmp;

  return (res.str);
}


#if 0
void AvUserComp::allocateColor (char * name, XColor & color)
{
  XColor unused;
  Colormap cmap = DefaultColormapOfScreen (XtScreen (parentWidget_));
  XAllocNamedColor (XtDisplay (parentWidget_), cmap, name, &color, &unused);
}
#endif

void AvUserComp::setTitle (const char * newTitle)
{
  if (topLevelP_)
    XtVaSetValues (parentWidget_,
		   XmNtitle, newTitle,
		   XmNiconName, newTitle,
		   NULL);
}


void AvUserComp::setSize (int w, int h)
{
  if (topLevelP_)
    XtVaSetValues (parentWidget_,
		   XmNwidth, w,
		   XmNheight, h,
		   NULL);
  else
    XtVaSetValues (baseWidget_,
		   XmNwidth, w,
		   XmNheight, h,
		   NULL);
}


void AvUserComp::getSize (int & w, int & h)
{
  Dimension width, height;
  XtVaGetValues (baseWidget_,
		 XmNwidth, &width,
		 XmNheight, &height,
		 NULL);
  cerr << width << '\t' << height << endl;
  w = width;
  h = height;
}


void AvUserComp::show ()
{
  if (baseWidget_ != NULL)
    if (!XtIsManaged (baseWidget_))
    {  XtManageChild (baseWidget_);
		isHidden_ = False;
    }

  if (topLevelP_ == TRUE)
    if (!XtIsRealized (parentWidget_))
    {	XtRealizeWidget (parentWidget_);
	isHidden_ = False;
    }
}


void AvUserComp::hide ()
{
  if (XtIsManaged (baseWidget_))
    XtUnmanageChild (baseWidget_);
  if (topLevelP_ == TRUE)
    if (XtIsRealized (parentWidget_))
      XtUnrealizeWidget (parentWidget_);

	isHidden_ = True;
}


void AvUserComp::initButton
(XImage & img, int w, int h, char * data, int pad, int depth, int bytes)
{
  img.width = w;
  img.height = h;
  img.data = data;
  img.xoffset = 0;
  img.format = XYBitmap;
  img.byte_order = MSBFirst;
  img.bitmap_pad = pad;
  img.bitmap_bit_order = LSBFirst;
  img.bitmap_unit = 8;
  img.depth = depth;
  img.bytes_per_line = bytes;
  img.obdata = NULL;
}


Pixmap AvUserComp::makePixmap 
(Widget w, int width, int height, char * bits, char * label)
{
  XImage im;
  AvUserComp::initButton (im, width, height, bits);
  XmInstallImage (&im, label);

  Pixel bg;
  XtVaGetValues (w, XmNbackground, &bg, NULL);

  return
    (XmGetPixmap (XtScreen(w), label, AvApp::blackPixel(), bg));
}


Widget AvUserComp::makePixmapPushButton 
(Widget w, int width, int height, char * bits, char * label)
{
  XImage im;
  AvUserComp::initButton (im, width, height, bits);
  XmInstallImage (&im, label);

  Pixel bg;
  XtVaGetValues (w, XmNbackground, &bg, NULL);

  Widget button = XtVaCreateManagedWidget
    ("", xmPushButtonWidgetClass, w,
     XmNlabelType, XmPIXMAP,
     XmNlabelPixmap,
     XmGetPixmap (XtScreen(w), label, AvApp::blackPixel(), bg),
     XmNshadowThickness, 1,
     XmNmarginHeight, 0,
     XmNmarginWidth, 0,
     NULL);

  return button;
}


Widget AvUserComp::makePixmapToggleButton 
(Widget w, int width, int height, char * bits, char * label)
{
  XImage im;
  AvUserComp::initButton (im, width, height, bits);
  XmInstallImage (&im, label);

  Pixel bg;
  XtVaGetValues (w, XmNbackground, &bg, NULL);

  Widget button = XtVaCreateManagedWidget
    ("", xmToggleButtonWidgetClass, w,
     XmNindicatorOn, FALSE,
     XmNlabelType, XmPIXMAP,
     XmNlabelPixmap,
     XmGetPixmap (XtScreen(w), label, BlackPixelOfScreen (XtScreen(w)), bg),
     XmNshadowThickness, 1,
     XmNmarginHeight, 0,
     XmNmarginWidth, 0,
     NULL);

  return button;
}


void AvUserComp::setLabelString (Widget w, const char * str)
{
  XmString xStr = AvXmStringCreateLocalized (str);
  XtVaSetValues (w, XmNlabelString, xStr, NULL);
  XmStringFree (xStr);
}


#if 0
void AvUserComp::setLabelString (Widget w, const char * inStr)
{
  char str[128];
  strcpy (str, inStr);
  XmString xStr = XmStringCreateLocalized (str);
  XtVaSetValues (w, XmNlabelString, xStr, NULL);
  XmStringFree (xStr);
}
#endif

void AvUserComp::setLabelString (Widget w, int v)
{
  char str[32];
  sprintf (str, "%d", v);
  setLabelString (w, str);
}


void AvUserComp::setLabelString (Widget w, float v)
{
  char str[32];
  sprintf (str, "%f", v);
  setLabelString (w, str);
}


void AvUserComp::getLabelString (Widget w, char * dest)
{
  XmString xStr;
  XtVaGetValues (w, XmNlabelString, &xStr, NULL);
  char * str;
  XmStringGetLtoR (xStr, XmSTRING_DEFAULT_CHARSET, &str);
  strcpy (dest, str);
  XmStringFree (xStr);
}


void AvUserComp::getTextField (Widget w, int & dest)
{
  char * val = XmTextFieldGetString (w);
  dest = ((int) strtol (val, NULL, 0));
  XtFree (val);
}


int AvUserComp::getTextField (Widget w)
{
  char * val = XmTextFieldGetString (w);
  int dest = ((int) strtol (val, NULL, 0));
  XtFree (val);
  return (dest);
}


Boolean AvUserComp::isReturnKey (XKeyEvent * e)
{
  KeySym sym;
  int index = 0;

  /* Index is a semi-magic constant. As far as I can determine from
     The XLookupKeysym man page "the first keysym corresponds to the
     keycode without modified keys, and the second corresponds to the
     keycode with Shift pressed". On the other hand, "ShiftMapIndex"
     is 0. */

  sym = XLookupKeysym (e, index);
  return (sym == XK_Return) ? TRUE : FALSE;
}


AvUserComp::~AvUserComp ()
{
  if (baseWidget_)
    XtDestroyWidget (baseWidget_);
  // If the parent widget was made for this component, destroy it
  if (topLevelP_ && parentWidget_)
    XtDestroyWidget (parentWidget_);
}



