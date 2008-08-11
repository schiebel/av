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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvBlinkWindow.cc,v 19.0 2003/07/16 05:46:36 aips2adm Exp $
//
// $Log: AvBlinkWindow.cc,v $
// Revision 19.0  2003/07/16 05:46:36  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:23  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.3  2002/01/16 22:44:13  hravlin
// Added cast to int for arg to AvMenu (Solaris 64bit).
//
// Revision 17.2  2002/01/07 22:27:09  hravlin
// Removed check for __P.
//
// Revision 17.1  2002/01/03 21:56:22  hravlin
// Changed call to setPMCacheParameters() to setRasterCacheParameters().
//
// Revision 17.0  2001/11/12 19:41:52  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:02  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:10  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 18:16:24  hravlin
// Changed some char * to const char *.
//
// Revision 14.0  2000/03/23 16:07:37  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/08/25 20:00:36  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:39:20  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:29  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:06  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:51:40  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:24:11  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:14:46  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.5  1997/02/09 00:37:02  hr
// Had an int/unsigned comparision.
//
// Revision 7.4  1997/02/05 17:29:04  hr
// The __P, g++ and math.h problem.
//
// Revision 7.3  1996/12/12 05:40:59  droberts
// Final update from monet archive.
//
// Revision 1.4  1996/09/30 21:20:45  hr
// Removed several resize menu options since they'd produce unusable sizes.
//
// Revision 1.3  1996/09/30  21:11:08  hr
// Scaling is labelled differently to minimize confusion with how ImageWindow
// does it.
//
// Revision 1.2  1996/08/14 17:20:53  hr
// Changes to remove g++ warnings (from bglenden).
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.8  1996/06/18  18:45:47  pixton
//  Copyright Update
//
//  Revision 1.7  1996/04/02  15:25:11  hr
//  Changed the way scaling works. Now, the contents of the AvAxisVPS's are
//  copied to a local VPS which has its rasterscale set so the raster scales
//  to the window size.
//  Also, added support for displaying overlayed contours.
//
//  Revision 1.6  1996/03/27  22:35:16  hr
//  Fixed a string copy problem in the scale menu handler.
//
//  Revision 1.5  1996/03/20  17:02:22  hr
//  Moved to reference AvXColorApp instead of AvApp.
//
//  Revision 1.4  1996/03/13  16:00:01  hr
//  Moved "Options" submenu to last.
//
//  Revision 1.3  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/09/20  01:55:16  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/04/24  19:02:32  hr
//  Clean up unused variables.
//
//  Revision 1.1  1995/04/24  18:51:18  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* BlinkWindow.c

BlinkWindow maintains its own displaylist of items to be shown. When
setSlice(index) is called, slice # index is displayed.

The display list is handled here rather than having it done in AnimPanel
since scaling requires setting scale factors for each object on the list.

			   ViewPortSetter
			        |
		---------------------------------
		|				|
	   ViewportSetter	...	ViewportSetter
		|				|
		Object				Object

The ViewportSetters are there to allow scaling individual objects.

Scaling is such that whatever scaling is required to fill the initial
window size is defined to be "1". Scaling x2 will double the window size.
It is not possible to act completely like AvImageWindow since different
sized objects may be displayed. Each is scaled to fit. The scale display
was made different from ImageWindow to help minimize confusion.

The list of objects to display is generated on each pass since there is
no current way to know when a window has been (in)excluded.
*/
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
//#if defined(__GNUC__) && defined(__P) && !defined(__linux__)
//#undef __P
//#endif
#include <math.h>

#include <X11/cursorfont.h>
#include <Xm/Xm.h>
#include <Xm/ArrowBG.h>
#include <Xm/LabelG.h>
#include <Xm/Label.h>
#include <Xm/DrawingA.h>
#include <Xm/Form.h>
#include <Xm/SeparatoG.h>
#include <Xm/PushBG.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Text.h>
#include <Xm/ToggleBG.h>
#include <Xm/Command.h>
#include <Xm/MainW.h>

#include <AvImageView.h>
#include <AvDisplayList.h>
#include <AvViewportSetter.h>
#include <AvXCanvases.h>
#include <AvBlinkWindow.h>
#include <AvCLUImage.h>
#include <AvCLUImageSetter.h>
#include <AvAxisVPS.h>

#include <AvXColorApp.h>

const char * AvBlinkWindow::className () const
{
  return ("AvBlinkWindow");
}

AvBlinkWindow::AvBlinkWindow (Widget parent, int buildInsideParent)
: AvDisplayWindow (parent, buildInsideParent)
{
  init ();
}

// List of possible menu scale factors. A separator is inserted where there
// is a NULL. Not all of these will be used since some would result in
// an image of too weird a size.
static const char *scales[] = {
//	"x32", "x20", "x16", "x12", "x10", "x8", "x6", "x5", 
	"x4", "x3", "x2",
	NULL,
	"x1",
	NULL,
	"x.50", "x.33", "x.25"
//, "x.125", "x.10", "x.625", "x.05"
	};
#define NSCALES (sizeof(scales)/sizeof(*scales))

void AvBlinkWindow::init (int makeMenus)
{
  scaleFactor_ = 1.0;

  if (makeMenus) {

#if 0
    scaleMenu = new AvMenu (13);
    scaleMenu->add ("5", PUSH_BUTTON, SV_SCALE);
    scaleMenu->add ("4", PUSH_BUTTON, SV_SCALE);
    scaleMenu->add ("3", PUSH_BUTTON, SV_SCALE);
    scaleMenu->add ("2", PUSH_BUTTON, SV_SCALE);
    scaleMenu->add ("1", PUSH_BUTTON, SV_SCALE);
    scaleMenu->add ("", SEPARATOR);
    scaleMenu->add (".5", PUSH_BUTTON, SV_SCALE);
    scaleMenu->add (".33", PUSH_BUTTON, SV_SCALE);
    scaleMenu->add (".25", PUSH_BUTTON, SV_SCALE);
    scaleMenu->add (".20", PUSH_BUTTON, SV_SCALE);
    scaleMenu->add (".10", PUSH_BUTTON, SV_SCALE);
#else
    scaleMenu = new AvMenu ((int)NSCALES+2);
     for(int i=0; i< (int)NSCALES; i++)
	if(scales[i] == NULL)
		scaleMenu->add ("", SEPARATOR);
	else
		scaleMenu->add (scales[i], PUSH_BUTTON, SV_SCALE);
#endif

    AvMenu * optionMenu0 = new AvMenu (6);
    optionMenu0->add ("Close View", PUSH_BUTTON, SV_OPTION);

    AvMenu * mainMenu = new AvMenu (2);
//    mainMenu->add ("Options", SUBMENU_BUTTON, 0, optionMenu0);
    mainMenu->add ("File", SUBMENU_BUTTON, 0, optionMenu0);
//    mainMenu->add ("Scale", SUBMENU_BUTTON, 0, scaleMenu);
    mainMenu->add ("Resize", SUBMENU_BUTTON, 0, scaleMenu);

    makeMainMenu (mainMenu);
  }

  XtAddCallback(parentWidget_, XmNdestroyCallback,
		 (XtCallbackProc) &AvBlinkWindow::destroyCB,
		 (XtPointer) this);
 

  // I don't think this will work.
  XtVaSetValues (parentWidget_,
		 XmNcolormap, AvXColorApp::colormap(),
		 XmNallowShellResize, TRUE,
		 NULL);

  XtVaSetValues (parentWidget_, XmNtitle, "Blink Window", NULL);

  workArea_ = XtVaCreateManagedWidget
    ("DispWin2WorkArea", xmFormWidgetClass, mainWindow_,
     XmNverticalSpacing, 5,
     XmNhorizontalSpacing, 5,
     NULL);

  XtVaSetValues (mainWindow_, XmNworkWindow, workArea_, NULL);

  // did the user set the width and height in the resources file?
  int ww = getIntegerResource ("DisplayWidth");
  int hh = getIntegerResource ("DisplayHeight");

  // if not, set to 256 by 256
  if (ww == 0) ww = 256;
  if (hh == 0) hh = 256;
  initialWindowSize_ = (float) ww;

  // make the X canvas within drawing frame
  int i = 0;
  Arg args[10];

  XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
  XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
  XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM); i++;
  XtSetArg(args[i], XmNbottomAttachment, XmATTACH_FORM); i++;

  XtSetArg(args[i], XmNwidth, ww); i++;
  XtSetArg(args[i], XmNheight, hh); i++;
  
  AvXWindow * xcanvas = new AvXWindow
    ("DisplayWin2Canvas", workArea_, AvXColorApp::palette(), args, i);
  xcanvas->ref();
  xcanvas->setRasterCacheParameters(TRUE, 128);	// Enable PM caching.
  canvas_ = xcanvas;
  drawArea_ = xcanvas->widget();
  list_ = new AvViewportSetter();
  list_->ref();
  // Set display mode to something other than ALL.
  list_->setDisplayMode(AvDisplayList::UP);

  callback_ = NULL;
  cbdata_   = NULL;
  displayTitle();

#if 0
  XtAddCallback (drawArea_, XmNexposeCallback,
		 (XtCallbackProc) &AvBlinkWindow::redrawCB,
		 (XtPointer) this);
#endif

  XtManageChild (baseWidget_);

  XtVaSetValues (parentWidget_, XmNminWidth, 172, NULL);

  XtAddCallback (drawArea_, XmNresizeCallback, 
		 (XtCallbackProc) &AvBlinkWindow::resizeCB,
		 (XtPointer) this);
}

void AvBlinkWindow::resizeCB (Widget w, AvBlinkWindow * win, XEvent *)
{
  XtRemoveCallback (w, XmNresizeCallback, 
		    (XtCallbackProc) &AvBlinkWindow::resizeCB,
		    (XtPointer) win);

	win->fitImageToWindow();
	win->fitWindowToImage();
//	printf("resizeCB called\n");
	win->showImage();

  XtAddCallback (w, XmNresizeCallback, 
		 (XtCallbackProc) &AvBlinkWindow::resizeCB,
		 (XtPointer) win);
}

void AvBlinkWindow::show()
{
	AvDisplayWindow::show();
	updateList();
	showImage();
}

void AvBlinkWindow::hide()
{
	AvDisplayWindow::hide();
}

void AvBlinkWindow::destroyCB(Widget , AvBlinkWindow *win, XtPointer )
{
	delete win;
}

void AvBlinkWindow::processMenu (char * buttonName, int choice, Widget mb)
{
  switch (choice){
  case SV_OPTION:
	if (!strcmp("Close View", buttonName))
	{	hide();
		callCallback(CLOSED);
	}
    break;
  case SV_SCALE:
     {
	char scaleStr[16];
	// strip away the '%' character
	int len = (int)strlen(buttonName)-1;	// Length w/o 'x'
	strncpy (scaleStr, buttonName+1, len);
	scaleStr[len] = '\0';
	float sc;
//	sc = 0.01 * atof (scaleStr);
	sc = atof(scaleStr);

	// 33% needs a little help...
//	if (!strcmp ("33%", buttonName)) sc = 1.0 / 3.0;
	
	// is this really a change?
	if (sc != scaleFactor_) {
	  scale(sc);
	  fitImageToWindow(FALSE);	// Reset DL scaling.
	  fitWindowToImage(TRUE);	// So window will resize.
	}
      }
    break;
  default:
    AvApp::processMenu (buttonName, choice, mb);
  }
}

// Compute the width/height of an object, the entire list by default.
void AvBlinkWindow::getImageSize(float &width, float &height,
							AvGraphics2D *obj)
{
static AvAllotment bb;

	if(obj == NULL)
		obj = list_;
	if(obj == NULL)
	{	width = height = -1;
	}
	else
	{	obj->getBoundingBox( *canvas_, bb);
		width = bb.x();
		height = bb.y();
	}
//printf("getImageSize: %.2f, %.2f\n", width, height);
}

// Scale object so it fits in the window.
// The display list is searched for the raster. The vps' raster scale is set
// so the raster will fit the window. The size of the vps is fixed so that
// vector overlays will still display if the raster is turned off.
void AvBlinkWindow::scaleObject(AvViewportSetter *vp)
{int rasid, j;

	if(vp == NULL)
		return;
	rasid = AvRaster::getClassTypeID();
	int len = vp->getLength();
	for(j=0; j< len; j++)
	{AvGraphics2D *obj;
		obj = vp->getObject(j);
		if(obj == NULL)
			return;
		// It is assumed there is only 1 raster!
		if(obj->isDerivedFrom(rasid))
		{AvRaster *ras = (AvRaster *) obj;
		 int width, height;
		 float scl;
			// Compute what scale factor to use.
			width = ras->width();
			height = ras->height();
			scl = (width<=height) ? width : height;
			scl = (initialWindowSize_/scl)*scaleFactor_;
			vp->setRasterScale(scl);
			// Need a fixed size in case raster is turned off.
			vp->setSize(unsigned(width*scl), unsigned(height*scl));
			return;
		}
	}
}

// Set scaling of current object so it fits in the window.
void AvBlinkWindow::fitObjectToWindow()
{
AvViewportSetter *vp;
AvGraphics2D *obj;

	obj = list_->getObject(list_->getCurrentIndex());
	if(obj != NULL)
		if(obj->isDerivedFrom(AvViewportSetter::getClassTypeID()))
		{	vp = (AvViewportSetter *) obj;
			scaleObject(vp);
		}
}

/* Set the scaling of each object so it scales to the size of the window.
*/
void AvBlinkWindow::fitImageToWindow(const Boolean redraw)
{
int	i, len;

//printf("Fit image to window\n");
	len = list_->getLength();
	for(i=0; i< len; i++)
	{AvGraphics2D *obj;

		obj = list_->getObject(list_->getCurrentIndex());
		if(obj != NULL)
		  if(obj->isDerivedFrom(AvViewportSetter::getClassTypeID()))
		  {	AvViewportSetter *vp = (AvViewportSetter *) obj;
			scaleObject(vp);
		  }
	}
	if(redraw)
		showImage();
}

void AvBlinkWindow::fitWindowToImage(const Boolean redraw)
{
float	width, height;

	getImageSize(width, height);

	// Width & height have scaleFactor_ applied.
	XtVaSetValues (drawArea(),
		 XmNwidth, (int) (width),
		 XmNheight, (int) (height),
		 NULL);

//printf("fitWindowToImage: %d, %d\n", (int)width, (int)height);
	if(redraw)
		showImage();
}

// Display currently selected object in the list after first checking
// to make sure scaling is set so it will fit. The check is done at display
// time since the size of the object may have changed. (ie If the user has
// resized the 'View Window' where the cube is displayed.
void AvBlinkWindow::showImage()
{
	fitObjectToWindow();
	canvas_->show(list_);
}

void AvBlinkWindow::scale(float sf)
{
  // scale the image by this factor.  Since we're passing floating
  // point scale factors, but the CLUBlink is only going to do pixel 
  // replication or integer subsampling, the scale factor actually used
  // by the CLUBlink is returned and stored in the window's scaleFactor.
  // if we're supsampling, we are currently limited to every 2nd, every
  // 3rd, every 4th pixel, etc.
	if (sf < 1.0)
		sf = 1.0 / ceil (1.0 / (double) sf);

	scaleFactor_ = sf;
	displayTitle();
}

void AvBlinkWindow::displayTitle()
{ char title[64];

#if 0
	float scale = scaleFactor_*100;

	if((float)(int)scale == scale)
		sprintf(title, "Blink Window  %d%%", (int)scale);
	else
		sprintf(title, "Blink Window  %.1f%%", scale);
#else
	float scale = scaleFactor_;

	if((float)(int)scale == scale)
		sprintf(title, "Blink Window  \"%d\"", (int)scale);
	else
		sprintf(title, "Blink Window  \"%.1f\"", scale);
#endif

	setTitle(title);
}

AvBlinkWindow::~AvBlinkWindow ()
{
	callCallback(DESTROYED);
	delete scaleMenu;
	canvas_->unref();
	list_->unref();
}

// Use cb as the callback procedure.
void AvBlinkWindow::addCallback(AvBlinkWindowCallback cb, XtPointer cbdata)
{
	callback_ = cb;
	cbdata_ = cbdata;
}

// Remove the callback. Currently only one callback is supported. The
// args are in case a callback list is added later.
void AvBlinkWindow::removeCallback(AvBlinkWindowCallback , XtPointer )
{
	callback_ = NULL;
}

// Call any callback(s).
void AvBlinkWindow::callCallback(const CBREASON reason)
{
	if(callback_ != NULL)
		(callback_)(this, cbdata_, reason);
}

////////////////
// These two routines are called from AvAnimPanel.
int AvBlinkWindow::getNumObjects()const
{
	return list_->getLength();
}

// This just tells the list set currentIndex to index. It does not cause
// an actual redisplay.
void AvBlinkWindow::showItem(const int index)
{
	list_->setCurrentIndex(index);
}
////////////////

/* Replace current display list with windows on the supplied list.

Purge current list.
Foreach item on the include list:
	Copy the display list from the rasterVPS to a new VPS.
	Set the scale factor.
*/
void AvBlinkWindow::updateList(AvImageViewItem **views, const int count)
{
int		j, i;

	list_->removeList();

	if((count <= 0) || (views == NULL))
		return;

	// For each window, create a new VPS and copy the rasterVPS children.
	// Can't directly use the rasterVPS since it has positioning info.
	for(i=0; i< count; i++)
	{AvImageViewItem *vi = views[i];
	 AvAxisVPS *avps;
	 AvCLUImageSetter *is;
	 AvViewportSetter *rvps, *cvps;
	 int rlen;
		is = vi->getCLUImage()->imageSetter();
		if(is == NULL)
			return;
		avps = is->getAxisVPS(-1);
		if(avps == NULL)
			continue;
		rvps = avps->getRasterVPS();
		if(rvps == NULL)
			continue;
		rlen = rvps->getLength();
		if(rlen <= 0)
			continue;
		cvps = new AvViewportSetter();
		for(j=0; j< rlen; j++)
		{AvGraphics2D *obj;
			obj = rvps->getObject(j);
			if(obj != NULL)
				cvps->append(obj);
		}
		// An included contours.
		is->getOverlays(count, views, cvps);
		list_->append(cvps);
	}
}

void AvBlinkWindow::updateList()
{
AvImageViewItem **views;
int	viewcount;
AvImageView::ItemSelection which=AvImageView::INCLUDED;

	/* Get list of included views.
	*/
	views = AvImageView::instance()->getImageViewList(viewcount, which);

		updateList(views, viewcount);

	delete [] views;
}
