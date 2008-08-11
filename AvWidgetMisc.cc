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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvWidgetMisc.cc,v 19.0 2003/07/16 05:47:03 aips2adm Exp $
//
// $Log: AvWidgetMisc.cc,v $
// Revision 19.0  2003/07/16 05:47:03  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:47  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/03 20:19:09  hravlin
// Added local copy of Strdup().
//
// Revision 17.0  2001/11/12 19:42:16  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:23  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:43  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:22:28  hravlin
// Added AvXmStringCreateLocalized().
//
// Revision 14.0  2000/03/23 16:08:07  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/09/15 21:05:23  hravlin
// Use AvApp's visual rather than the root's.
//
// Revision 13.0  1999/08/10 18:39:53  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:26  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:27  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:14  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:26:42  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:16  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/02/09 00:19:23  hr
// Added childNameToWidget().
//
// Revision 7.2  1996/12/12 07:45:18  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.4  1996/06/18  18:46:44  pixton
//  Copyright Update
//
//  Revision 1.3  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/03/29  21:31:32  hr
//  add widgetTopShell
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvWidgetMisc.C
Miscelaneous routines for dealing with widgets.
*/

#include <stdlib.h>
#include <strings.h>
#include "AvApp.h"
#include "AvWidgetMisc.h"
#include <X11/Shell.h>		// Get XtNVisual definition.
#include <Xm/Text.h>

#if 0
void printWindowInfo(Display *dpy, Window w)
{
XWindowAttributes	a;
Status	status;

	if((dpy == NULL) || (w == 0))
	{	printf("Null dpy or w\n");
		return;
	}
	status = XGetWindowAttributes(dpy, w, &a);
	printf("0x%0x's colormap is 0x%0x\n", w, a.colormap);
}
void printWindowInfo(Widget w)
{
printf("Wid: 0x%x ", w);
	printWindowInfo(XtDisplay(w), XtWindow(w));
}
#endif


/* Return the visual of a widget. If none is set, returns aipsview's
default visual if set, else the default visual. This will probably always
return the default visual unless a Visual was explicitly given to the top
level shell widget.
*/

Visual *widgetVisual(Widget w)
{
Visual *viz=NULL;
Widget tst;

	if(w == NULL)
		return NULL;
	tst = w;
	if(XtIsRealized(w))		// Crashes if not realized yet.
	  do
	  {	XtVaGetValues(w, XtNvisual, &viz, NULL);
		if(viz == NULL)
			tst = XtParent(tst);
		else
			break;
	  }
	  while(tst != NULL);

	if(viz == NULL)
	{	viz = AvApp::visual();
		if(viz == NULL)
			viz = DefaultVisualOfScreen(XtScreen(w));
	}
	return viz;
}

// Return visual information for the widget's visual.
// Use XFree to free the result.
XVisualInfo *getWidgetVisualInfo(Widget w)
{
XVisualInfo *vi, templ;
int	rtn;

	templ.visualid = XVisualIDFromVisual(widgetVisual(w));
	vi = XGetVisualInfo(XtDisplay(w), VisualIDMask, &templ, &rtn);
	return vi;
}

// Returns the topmost parent of a widget.
Widget widgetRoot(Widget w)
{
Widget p;

	if(w == NULL)
		return NULL;
	while( (p = XtParent(w)) != NULL) w = p;
	return w;
}

// Returns the topmost parent (shell) of a widget.
Widget widgetTopShell(Widget w)
{
	if(w == NULL)
		return NULL;

	while (w && !XtIsWMShell (w))
        	w = XtParent (w);
    return w;
}

unsigned int widgetDepth(Widget w)
{
Arg args[2];
int	i;
unsigned int depth;

	i = 0;
	XtSetArg(args[i], XtNdepth, &depth); i++;
	XtGetValues(w, args, i);
	return depth;
}

Colormap widgetColormap(Widget w)
{
Arg args[2];
int	i;
Colormap cm;

	i = 0;
	XtSetArg(args[i], XtNcolormap, &cm); i++;
	XtGetValues(w, args, i);
	return cm;
}

//Return the contents of a colormap associated with the widget.
// Use 'delete []' to release result.
// Len will be set to the number of entries in the colormap.
XColor* queryWidgetColormap(Widget w, int &len)
{
Arg args[2];
int	i;
Colormap cm;
//unsigned int depth, d;
XVisualInfo *vi;
int	cmsize;
XColor	*xc;

	i = 0;
	XtSetArg(args[i], XtNcolormap, &cm); i++;
//	XtSetArg(args[i], XtNdepth, &d); i++;
	XtGetValues(w, args, i);

	vi = getWidgetVisualInfo(w);
	cmsize = vi->colormap_size;
	xc = new XColor[cmsize];
	for(i = 0; i< cmsize; i++)
	{	xc[i].pixel = i;
		xc[i].flags = DoRed | DoGreen | DoBlue;
	}
	XQueryColors(XtDisplay(w), cm, xc, cmsize);
	len = cmsize;
	return xc;
}

// Given a child's name and its parent, return the child's widget.
Widget childNameToWidget(Widget parent, const char *childName)
{ char name[1024];

	if((parent == NULL) || (childName == NULL))
		return NULL;
	strcpy(name, "*");
	strcat(name, childName);
	Widget child = XtNameToWidget(parent, name);
	return child;
}

static char *Strdup(const char *str)
{
int	len;
char	*s;

	if(str == NULL)
		return NULL;

	len = (int)strlen(str);
	s = new char[len+1];
	strcpy(s, str);

	return s;
}

XmString AvXmStringCreateLocalized(const char *text)
{ char *str;

	if(text == NULL)
		str = Strdup("");
	else
		str = Strdup(text);

	XmString xstr = XmStringCreateLocalized(str);
	delete [] str;
	return xstr;
}
