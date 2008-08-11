//# Copyright (C) 1995-97 Board of Trustees of the University of Illinois
//#
//# This software, both binary and source, is copyrighted by The
//# Board of Trustees of the University of Illinois. Ownership
//# remains with the University. You should have received a copy 
//# of a licensing agreement with this software. See the file 
//# "AIPSVIEW_COPYRIGHT", or contact the University at this address:
//#
//# The NCSA AipsView Visualization System
//# National Center for Supercomputing Applications
//# University of Illinois
//# 405 North Mathews Ave.
//# Urbana, IL 61801
//# aipsview@ncsa.uiuc.edu
//# --------------------------------------------------
//
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvGUIParts.h,v 19.0 2003/07/16 05:48:05 aips2adm Exp $
//
// $Log: AvGUIParts.h,v $
// Revision 19.0  2003/07/16 05:48:05  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:39  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:43:07  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:14  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:44  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:09:11  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:41:02  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:27  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:02:22  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:56:38  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1997/11/17 16:49:54  hr
// Added ability to have internal callback come before or after caller's.
//
// Revision 9.0  1997/08/25 21:31:58  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:19:38  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.1  1997/02/09 00:09:00  hr
// Miscellaneous routines for building user interface, particularly with
// Notifiers.
//
//
//---------------------------------------------------------------------------

/* AvGUIParts.h
Miscellaneous routines for building user interface, particularly with
Notifiers.

*/
#ifndef AvGUIParts_h
#define AvGUIParts_h
#include <X11/Intrinsic.h>
#include "AvNotifier.h"
#include "AvWidgetMisc.h"	// childNameToWidget.

// Create a frame with a title that is either left, center or right justified.
	enum TITLEJUSTIFY {LEFT, CENTER, RIGHT};

Widget labeledFrame(Widget parent, const char *label,
		    const TITLEJUSTIFY justify=LEFT);

/* Returns a RowColumn widget around a label and slider (XmScale).
Min, Max give the range of the slider.
Value is the slider's initial value.
dp is the number of decimal points to be shown.
If showValue is TRUE, the value of the slider is shown.
cbrtn and cbdata are the callback routine and its user data.
The callback routine will receive both XmNvalueChangedCallback and
XmNdragCallback events.

The width of the slider will be max-min+1. However, the minimum width will
be 128 and the maximum will be 256. If this is not acceptable, use
childNameToWidget(<returned widget>, "slider") to get a pointer to the
slider.

This routine is primarily used by buildIntegerSliderBox() and
buildDoubleSliderBox(). It is seldom called directly.
*/

Widget buildSliderBox(  Widget parent, const char *label,
			const int Min, const int Max,
			const int Value0, const int dp,
			const Boolean showValue,
			XtPointer cbdata, XtCallbackProc cbrtn);

/* As above, but links an AvDoubleNotifier to a slider. The callback
routine is supplied in AvGUIParts. Dp will be used to convert Min, Max and
Value0 into integers and the slider value back to a double.
Min & max are converted to integers via <int> = <value>*10^dp.
*/

Widget buildDoubleSliderBox(Widget parent, const char *label,
			    const double Min, const double Max,
			    const double Value0, const int dp,
			    const Boolean showValue,
			    const AvDoubleNotifier *data);

// As above, but links an AvIntegerNotifier to a slider. The callback routine
// is supplied in AvGUIParts.
Widget buildIntegerSliderBox(Widget parent, const char *label,
			     const int Min, const int Max,
			     const int Value0,
			     const Boolean showValue,
			     const AvIntegerNotifier *data);
////////////////////////////////////////////////////////////////
// If the caller provides a callback, should the default callback be
// placed before or after the caller supplied callback or not used?
enum GUICBPOSITION {GUICB_NEITHER=0, GUICB_BEFORE=1, GUICB_AFTER=2,
		    GUICB_BOTH=3};
// Returns an RC radio box to set the value of an IntMapNotifier. If label
// is non NULL, the return is an RC box containing a label and an RC radio
// box. The buttons' names are the string values of the IntMap.
// If the callback routine is NULL (the usual use), a built int callback
// routine will be used. If it isn't, it can become before or after (or both)
// the internal callback. The default is instead of. If the caller'd callback
// isn't NULL, but cbdata is, the address of the IntMap is used.
// userData is attached to each toggle's XmNuserData resource.
// By default, the box will be setup so there will be about 40 characters
// in a row.
// Use childNameToWidget(<returned widget>, <name>) to get the radio box
// widget.
// Use childNameToWidget(<returned widget>, <toggle name>) to get a toggle
// widget.
//Note: name and label should be different or else childNameToWidget will
//	return the label widget instead of the radiobox.
Widget buildIntMapRadioBox(Widget parent, const char *name,
			   const char *label,
			   const AvIntMapNotifier *map,
			   XtPointer userData=NULL,  // Button's XmNuserData.
			   XtCallbackProc =NULL,
			   XtPointer cbdata=NULL,
			   const GUICBPOSITION=GUICB_NEITHER);

// Calls buildIntMapRadioBox then assigns pixmaps to each toggle. The length
// of the pixmap array should equal the length of the Notifier's IntMap.
// If an entry is NULL, no pixmap is assigned.
// The box is configured so 8 16x16 pixmap toggle buttons will be in a row.

Widget buildIntMapPMRadioBox(Widget parent, const char *name,
			     const char *label,
			     const AvIntMapNotifier *map,
			     Pixmap *pixmaps,
			     XtPointer userData=NULL,
			     XtCallbackProc =NULL);

// Build a generic toggle button connected to a notifier.
Widget buildToggleButton(Widget parent, const char *label,
			 const AvIntegerNotifier *in,
			 XtPointer cbData=NULL, XtCallbackProc=NULL,
			 const GUICBPOSITION=GUICB_NEITHER);

////////////////////////////////////////////////////////////////
//			Special purpose routines.
// imn's IntMap is assumed to be a list of PGPLOT color name/values. A
// toggle button for each color is created. This is the standard means of
// building a color selector.
Widget buildColorSelector(Widget parent, const char *name,
			  const char *label, const AvIntMapNotifier *imn);

// list of PGPLOT colors. The values equal defined PGPLOT color indices.
  enum PGPLOTCOLORS{ BLACK, WHITE, RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW};

// Returns a pointer to an IntMap of the PGPLOT colors.
const IntMap *PGPLOTColors();

// Here the IntMap should consist of PGPLOT marker types.
Widget buildMarkerSelector(Widget parent, const char *name,
			   const char *label, const AvIntMapNotifier *imn);

// Creates a toggle button with the label "Show Escape Codes". When pressed,
// window below the toggle will expand listing the PGPLOT escape codes.
Widget showEscapeCodes(Widget parent);

#endif
