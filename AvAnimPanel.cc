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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvAnimPanel.cc,v 19.1 2003/09/16 21:38:59 hravlin Exp $
//
// $Log: AvAnimPanel.cc,v $
// Revision 19.1  2003/09/16 21:38:59  hravlin
// Replaced a RowColumn widget with a Form in the constructor because it seems
// to help the blank panel problem.
//
// Revision 19.0  2003/07/16 05:46:25  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:13  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.2  2002/01/16 22:49:13  hravlin
// The mode->animateMode conversion always goes through a cast to long first
// since XtPointer is 64 bits with some compilers.
//
// Revision 17.1  2002/01/03 21:52:44  hravlin
// animate_normal() now calls flush() and wait() so animations are synched.
//
// Revision 17.0  2001/11/12 19:41:41  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:41:52  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:07:27  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 18:20:37  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:07:25  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.5  1999/10/04 20:27:04  hravlin
// Some widgets were being attached to the wrong widgets.
//
// Revision 13.4  1999/10/01 21:56:14  hravlin
// Replaced XmToggleButtonGadget{Get,Set}State() with XmToggleButton{Get,Set}State.
//
// Revision 13.3  1999/09/15 21:45:49  hravlin
// Use global X variables rather than root window's and use XmGetPixmapByDepth
// to pass in colormap depth rather than assume root window's.
//
// Revision 13.2  1999/08/30 17:11:39  hravlin
// Fixed previous fix for SGI 32 bit compilers.
//
// Revision 13.1  1999/08/25 20:02:50  hravlin
// Fixed problem with casting from XtPointer to enum.
//
// Revision 13.0  1999/08/10 18:39:04  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:04  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:57:30  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:50:55  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.3  1998/06/19 14:13:05  hr
// Removed an enum cast warning from egcs.
//
// Revision 9.2  1998/03/10 21:57:17  hr
// In animateButton, SGI 7.2.0 & 7.2.1 compilers were giving errors on a cast.
//
// Revision 9.1  1997/12/17 21:49:34  hr
// Changes to remove warnings from MIPSpro.7x compiler.
//
// Revision 9.0  1997/08/25 21:23:04  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.1  1997/04/15 20:12:00  hr
// Added 'relative indexing'.
//
// Revision 8.0  1997/02/20 03:14:20  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.4  1997/02/06 17:55:17  hr
// Changes to support 1 relative readouts.
//
// Revision 7.3  1996/12/12 00:07:44  droberts
// Final update from monet archive.
//
// Revision 1.3  1996/10/01 21:55:00  hr
// Changes in animate_normal() to work with sliceNums as DS values.
//
// Revision 1.2  1996/09/18 18:45:04  hr
// animate_normal() changed to call accessor:setPlane().
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.10  1996/06/18  18:45:46  pixton
//  Copyright Update
//
//  Revision 1.9  1996/03/27  18:16:31  hr
//  Changes to support allowing user to change start and end slices.
//
//  Revision 1.8  1996/02/04  21:14:29  hr
//  Fixed a problem where animating with included contours would display
//  the incorrect contour if the included contour's window hadn't been
//  redisplayed yet.
//
//  Revision 1.7  1995/11/03  17:40:05  hr
//  Removed an XmNbottomAttachment from the parts widget. This seems to help
//  keep the animation panel from being invisible for 'small' main windows
//  under SunOS 4.1.3.
//
//  Revision 1.6  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.6  1995/09/20  01:52:41  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/06/12  21:27:58  hr
//  Use vi->getImageWindow()->viewPanel()->setSlice(sliceNum) rather
//  than vi->viewPanel()...
//
//  Revision 1.4  1995/04/24  18:34:26  hr
//  Added support for blinking.
//
//  Revision 1.3  1995/04/19  17:17:51  hr
//  Cleanup.
//
//  Revision 1.2  1995/04/19  15:43:28  hr
//  Adjusted Slider minimum value.
//
//  Revision 1.1  1995/04/19  14:28:22  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------
/*
AvAnimPanel.cc
	Builds and handles the animation controls for aipsView.
	Contains pieces from the old AvImageViewPanel and AvAnimCtlr
	classes.
*/

#include <stdio.h>
#include <stdlib.h>		//getenv
#include <iostream.h>

#include <Xm/Form.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/ToggleB.h>
#include <Xm/Scale.h>

#include <AvAnimPanel.h>
#include <AvImageViewPanel.h>
#include <AvTimer.h>
#include <AvImageView.h>
#include <AvImageWindow.h>
#include <AvViewportSetter.h>
#include <AvCLUImage.h>
#include <AvCLUImageSetter.h>
#include <AvApp.h>

#include <goOnce.icon>
#include <goBackForth.icon>
#include <goCycle.icon>

XImage AvAnimPanel::goOnce;
XImage AvAnimPanel::goBackForth;
XImage AvAnimPanel::goCycle;

const char * AvAnimPanel::className () const
{
	return ("AvAnimPanel");
}


AvAnimPanel::AvAnimPanel(Widget parent, int buildInsideParent)
				: AvFormComp(parent, buildInsideParent)
{

  XtVaSetValues (baseWidget_,
		 XmNtopAttachment, XmATTACH_FORM,
		 XmNbottomAttachment, XmATTACH_FORM,
		 XmNleftAttachment, XmATTACH_FORM,
		 XmNrightAttachment, XmATTACH_FORM,
		 NULL);

  // This was changed from a RowColumn to a Form because with some
  // libraries, the animation panel was blank until the window was resized.
  // Lesstif would do one thing, Motif another. Form seems to work for
  // some reason.
  Widget parts = XtVaCreateManagedWidget
#if 0
    ("AvAnimPanel", xmRowColumnWidgetClass, baseWidget_,
     XmNorientation, XmVERTICAL,
#else
    ("AvAnimPanel", xmFormWidgetClass, baseWidget_,
#endif
	XmNspacing, 3,
	XmNmarginTop, 10,
	XmNmarginWidth, 8,
	XmNtopAttachment, XmATTACH_FORM,
	// Not attaching to bottom may help prevent the entire widget
	// from disappearing if the main window is too small.
//	XmNbottomAttachment, XmATTACH_FORM,
	XmNleftAttachment, XmATTACH_FORM,
	XmNrightAttachment, XmATTACH_FORM,
     NULL);

	buildAnimateControls(parts);
}

void AvAnimPanel::init()
{
XtAppContext app;
AvImageView *iv;

	blinkWindow_ = NULL;
	app = AvApp::appContext();
	app_ = app;

	iv = AvImageView::instance();

	timer_ = new AvTimer(app);
	timer_->ref();
	timer_->addCallback(timerCB, this);
	imageView_ = iv;
	animateChange = 1;
	animateMode = Stopped;
	currentIndex = 0;
	relativeIndexing_ = FALSE;
}

void AvAnimPanel::buildAnimateControls(Widget parent)
{
  animateMode = Stopped;

	init();

  // install the X images for the animation buttons
  AvUserComp::initButton
    (goOnce, goOnce_width, goOnce_height, goOnce_bits, 0, 1, 3);
  
  AvUserComp::initButton
    (goBackForth, goBackForth_width, goBackForth_height,
     goBackForth_bits, 0, 1, 3);
  
  AvUserComp::initButton
    (goCycle, goCycle_width, goCycle_height, goCycle_bits, 0, 1, 3);
  
  char GoOnce[] = "goOnce";
  XmInstallImage (&goOnce, GoOnce);
  char GoBackForth[] = "goBackForth";
  XmInstallImage (&goBackForth, GoBackForth);
  char GoCycle[] = "goCycle";
  XmInstallImage (&goCycle, GoCycle);
  
  Widget animateForm = XtVaCreateManagedWidget
    ("", xmFormWidgetClass, parent,
     NULL);

  Widget animateLabel = XtVaCreateManagedWidget
    ("AutoSlice:", xmLabelWidgetClass, animateForm,
//     XmNbottomAttachment, XmATTACH_FORM,
     XmNtopAttachment, XmATTACH_FORM,
     NULL);
  
  Widget animateButtonsRC = XtVaCreateManagedWidget
    ("AnimateButtonsRC", xmRowColumnWidgetClass, animateForm,
     XmNorientation, XmHORIZONTAL,
//     XmNspacing, 1,
     XmNleftAttachment, XmATTACH_WIDGET,
     XmNleftWidget, animateLabel,
     XmNrightAttachment, XmATTACH_FORM,
     XmNpacking, XmPACK_TIGHT,
     NULL);

  Widget animateRadioButtons = XtVaCreateManagedWidget
    ("AnimateRadioButtons", xmRowColumnWidgetClass, animateButtonsRC,
     XmNorientation, XmHORIZONTAL,
     XmNradioBehavior, TRUE,
     XmNradioAlwaysOne, FALSE,
     XmNspacing, 1,
     NULL);

  Pixel blackPixel = AvApp::blackPixel();
  Screen * screen = XtScreen (baseWidget_);
  Pixel bg;
  int depth;
  XtVaGetValues(baseWidget_,
	XmNbackground, &bg,
	XmNdepth, &depth,
	NULL);

  Pixmap pixmap = XmGetPixmapByDepth(screen, GoOnce, blackPixel, bg, depth);

  animateOnce = XtVaCreateManagedWidget 
    ("Once", xmToggleButtonWidgetClass, animateRadioButtons,
     XmNuserData, Once,
     XmNshadowThickness, 1,
     NULL);

  pixmap = XmGetPixmapByDepth(screen, GoBackForth, blackPixel, bg, depth);

  animateBackAndForth = XtVaCreateManagedWidget 
    ("AnimateButton", xmToggleButtonWidgetClass, animateRadioButtons,
     XmNuserData, BackForth,
     XmNshadowThickness, 1,
     XmNlabelType, XmPIXMAP,
     XmNlabelPixmap, pixmap,
     NULL);
  
  pixmap = XmGetPixmapByDepth(screen, GoCycle, blackPixel, bg, depth);

  animateCycle = XtVaCreateManagedWidget 
    ("AnimateButton", xmToggleButtonWidgetClass, animateRadioButtons,
     XmNuserData, Cycle,
     XmNshadowThickness, 1,
     XmNlabelType, XmPIXMAP,
     XmNlabelPixmap, pixmap,
     NULL);
  
  // add the callbacks for the animation buttons
  XtAddCallback (animateOnce, XmNvalueChangedCallback,
		 (XtCallbackProc) &AvAnimPanel::animateButtonCB,
		 (XtPointer) this);
  
  XtAddCallback (animateBackAndForth, XmNvalueChangedCallback,
		 (XtCallbackProc) &AvAnimPanel::animateButtonCB,
		 (XtPointer) this);
  
  XtAddCallback (animateCycle, XmNvalueChangedCallback,
		 (XtCallbackProc) &AvAnimPanel::animateButtonCB,
		 (XtPointer) this);
// XmNdestroyCallback

  Widget relativeW = XtVaCreateManagedWidget("Relative Indexing",
			xmToggleButtonWidgetClass, animateButtonsRC,
			XmNshadowThickness, 1,
			NULL);
  XtAddCallback(relativeW, XmNvalueChangedCallback,
		(XtCallbackProc) &AvAnimPanel::relativeCB,
		(XtPointer) &relativeIndexing_);


  Widget animateSliders = XtVaCreateManagedWidget
    ("AnimateSliders", xmRowColumnWidgetClass, animateForm,
     XmNorientation, XmVERTICAL,
     XmNleftAttachment, XmATTACH_WIDGET,
     XmNrightAttachment, XmATTACH_FORM,
     XmNleftAttachment, XmATTACH_FORM,
	XmNtopAttachment, XmATTACH_WIDGET,
	XmNtopWidget, animateButtonsRC,
     NULL);

	rateSlider_ = XtVaCreateManagedWidget("Speed", xmScaleWidgetClass,
		animateSliders,
		XtVaTypedArg, XmNtitleString, XmRString, "Speed(%)", 9,
		XmNminimum, 0,
		XmNmaximum, 100,
		XmNvalue, 50,
//		XmNdecimalPoints, 1,
		XmNshowValue, True ,
		XmNorientation, XmHORIZONTAL,
		NULL);

	setRate(50);	// To match above;

	XtAddCallback(rateSlider_, XmNvalueChangedCallback,
						rateSliderCB, this);
	XtAddCallback(rateSlider_, XmNdragCallback, rateSliderCB, this);

	// Note: until we actually try to display a frame, we don't know
	// how many there are. Also, the number can change dynamically.
	minIndex_ = 0;
	maxIndex_ = 100;
	int offset = AvImageView::oneIndexing();
	frameSlider_ = XtVaCreateManagedWidget("Frame #", xmScaleWidgetClass,
		animateSliders,
		XtVaTypedArg, XmNtitleString, XmRString, "Frame #", 8,
		XmNminimum, minIndex_ + offset,
		XmNmaximum, maxIndex_ + offset,
		XmNvalue, offset,
		XmNshowValue, True ,
		XmNorientation, XmHORIZONTAL,
		NULL);
	XtAddCallback(frameSlider_, XmNvalueChangedCallback,
							frameSliderCB, this);
	XtAddCallback(frameSlider_, XmNdragCallback, frameSliderCB, this);

#define BLINKisBUTTON 0
#if BLINKisBUTTON
	blinkToggle_ = XtVaCreateManagedWidget("Blink",
			xmPushButtonWidgetClass, animateSliders,
		NULL);
	XtAddCallback(blinkToggle_, XmNactivateCallback,
		 (XtCallbackProc) &AvAnimPanel::blinkButtonCB,
		 (XtPointer) this);
#else
	blinkToggle_ = XtVaCreateManagedWidget("Blink",
			xmToggleButtonWidgetClass, animateSliders,
		NULL);
	XtAddCallback(blinkToggle_, XmNvalueChangedCallback,
		 (XtCallbackProc) &AvAnimPanel::blinkButtonCB,
		 (XtPointer) this);
#endif
}

void AvAnimPanel::makeBlinkWindow()
{
	if(blinkWindow_ == NULL)
	{	blinkWindow_ = new AvBlinkWindow(baseWidget_, FALSE);
		blinkWindow_->addCallback(blinkWCB, this);
	}
}

// Returns true if blinking is enabled... The blink window is up.
Boolean AvAnimPanel::blinkIsEnabled()const
{
	if( (blinkWindow_ != NULL) && XtIsRealized(blinkWindow_->drawArea()))
		return TRUE;
	else
		return FALSE;
}

void AvAnimPanel::enableBlink(const Boolean enable)
{
	if(enable != blinkIsEnabled())
		stop();
	if(enable)
	{	makeBlinkWindow();
		blinkWindow_->show();
	}
	else
	{	if(blinkWindow_ != NULL)
			blinkWindow_->hide();
		XmToggleButtonSetState(blinkToggle_, FALSE, FALSE);
	}
}


// Called from animateButtonCB control button is pressed...
// Starts or stops the animation.
void AvAnimPanel::blinkButton(Widget w)
{
//printf("blinkButton called.\n");

#if BLINKisBUTTON
	enableBlink(TRUE);
#else
	enableBlink(XmToggleButtonGetState(w));
#endif
}

void AvAnimPanel::animateButton(Widget w)
{
  // has animation been activated?
	if (XmToggleButtonGetState (w))
	{	// record what animation mode we have entered
		XtPointer mode;
		XtVaGetValues (w, XmNuserData, &mode, NULL);
//#if defined(sgi) && (defined(_MIPS_SZPTR) && (_MIPS_SZPTR == 64))
//		// SGI's 7x.x compilers die on casting XtPointer to enum
		// Cast to long first to avoid complaints with 64 bit cmplrs.
		animateMode = (AnimateMode)(long)mode;
//#else
//		animateMode = (AnimateMode)(int)mode;
//#endif
		animateChange = 1;
		// If mode is Once, decide whether to go up or down.
		if((animateMode == Once) && (currentIndex) >
			(maxIndex_ - minIndex_)/2)
			animateChange = -animateChange;
		start();
	}
	else
		stop();
}

static const float MINRATE = .2;	// 1 frame each 5 seconds.
static const float MAXRATE = 60.0;	// 60fps

void AvAnimPanel::setRate(const int percent)
{
	{float rate = (MAXRATE-MINRATE)*percent/100. + MINRATE;
//		printf("%d%%, %.2f\n", percent, rate);
		timer_->setInterval(rate, TRUE);
	}
}

void AvAnimPanel::setFramePosition(const int index)
{
	if(validIndex(index))
		XtVaSetValues(frameSlider_,
			XmNvalue, index + AvImageView::oneIndexing(),
			NULL);
}

int AvAnimPanel::getFramePosition()const
{
int	pos;

	XtVaGetValues(frameSlider_, XmNvalue, &pos, NULL);
	return pos - AvImageView::oneIndexing();
}

void AvAnimPanel::blinkWCB(AvBlinkWindow *, XtPointer cbdata,
		AvBlinkWindow::CBREASON reason)
{
AvAnimPanel *me = (AvAnimPanel *)cbdata;

	switch(reason) {
	case AvBlinkWindow::DESTROYED:
		me->blinkWindow_ = NULL;
//		printf("Blink window removed.\n");
		XmToggleButtonSetState(me->blinkToggle_, FALSE, FALSE);
		break;
	case AvBlinkWindow::CLOSED:
		XmToggleButtonSetState(me->blinkToggle_, FALSE, FALSE);
		break;
	default:
		break;
	}
}

void AvAnimPanel::blinkButtonCB(Widget w, XtPointer cbdata, XtPointer)
{
	AvAnimPanel *p = (AvAnimPanel *) cbdata;
	p->blinkButton(w);
}

void AvAnimPanel::animateButtonCB(Widget w, XtPointer cbdata, XtPointer)
{
	AvAnimPanel *p = (AvAnimPanel *) cbdata;
	p->animateButton(w);
}

AvAnimPanel::~AvAnimPanel ()
{
	enableBlink(FALSE);
	delete blinkWindow_;
	timer_->stop();
	timer_->unref();
}

void AvAnimPanel::frameSliderCB(Widget, XtPointer cbdata, XtPointer calldata)
{
AvAnimPanel *me = (AvAnimPanel *)cbdata;
XmScaleCallbackStruct *cbs = (XmScaleCallbackStruct *)calldata;

	me->showIndex(cbs->value - AvImageView::oneIndexing());
}

void AvAnimPanel::rateSliderCB(Widget, XtPointer cbdata, XtPointer calldata)
{
AvAnimPanel *me = (AvAnimPanel *)cbdata;
XmScaleCallbackStruct *cbs = (XmScaleCallbackStruct *)calldata;

	me->setRate(cbs->value);
}

void AvAnimPanel::relativeCB(Widget w, XtPointer cbdata,
			     XtPointer )
{Boolean *rel = (Boolean *)cbdata;

	*rel = XmToggleButtonGetState(w);
}

// Start animating
void	AvAnimPanel::start()
{
	timer_->start();
}

// Stop animating
void	AvAnimPanel::stop()
{
Widget w;

	// Usually, the buttons get turned off manually. However, sometimes
	// some outside event may cause a stop.
	switch(animateMode) {
	case Once:
		w = animateOnce;
		break;
	case BackForth:
		w = animateBackAndForth;
		break;
	case Cycle:
		w = animateCycle;
		break;
	case Stopped:
	case SingleStep:
	default:
		w = NULL;
		break;
	}

	setToggle(w, FALSE);
	animateMode = Stopped;
	timer_->stop();
}

void AvAnimPanel::setToggle(Widget w, const Boolean ON)
{
	if(w != NULL)
		XmToggleButtonSetState(w, ON, FALSE);
}

// Called on each timer tick. Just calls animate().
void AvAnimPanel::timerCB(AvTimer *, XtPointer cbdata, XtPointer )
{
AvAnimPanel *a = (AvAnimPanel *)cbdata;
static Boolean inUse=FALSE;

	if(inUse)
		printf("Reentered active callback!!\n");
	else
		inUse = TRUE;
	a->animate();
	inUse = FALSE;
}

// Show this frame #. Stops any running animation.
void AvAnimPanel::showIndex(const int index)
{
	if(running())
		stop();
	animateMode = SingleStep;
		animate(index);
	animateMode = Stopped;
}

// Returns whether the AvImageViewItem's ImageWindow has its include flag set.
static inline Boolean included(AvImageViewItem *vi)
{	if(vi == NULL)
		return FALSE;
	else
		return vi->getImageWindow()->include();
}

#if 0
// Is the index within the currently set min/max?
Boolean AvAnimPanel::validIndex(const int index)
{
	return ( (index >= minIndex_) && (index <= maxIndex_));
}
#endif

// This is called by AvImageView when oneIndexing changes. It assumes
// the value toggles between 1 & 0.
void AvAnimPanel::updateFrameSlider()
{int offset = AvImageView::oneIndexing();
 int value = getFramePosition();

	XtVaGetValues(frameSlider_,
		      XmNvalue, &value,
		      NULL);

	// Subtract previous offset.
	value -= (offset == 0) ? 1 : 0;
	XtVaSetValues(frameSlider_,
		      XmNminimum, minIndex_ + offset,
		      XmNmaximum, maxIndex_ + offset,
		      XmNvalue, value + offset,
		      NULL);
}

// Sets min/max values and changes the frame slider limits to match.
void AvAnimPanel::setMinMaxIndex(const int min, const int max)
{ int minval;

	XtVaGetValues(frameSlider_,
		      XmNminimum, &minval,
		      NULL);
	minval -= AvImageView::oneIndexing();

	if((min != minIndex_) || ( max != maxIndex_) || (minIndex_ != minval))
	{	minIndex_ = min;
		maxIndex_ = max;
		if(getFramePosition()> max)
			setFramePosition(max);
		else
		if(getFramePosition()< max)
			setFramePosition(min);

		XtVaSetValues(frameSlider_,
			      XmNminimum, min + AvImageView::oneIndexing(),
			      XmNmaximum, max + AvImageView::oneIndexing(),
			      NULL);

		// Make sure displayed position is legal and in correct place.
		if(currentIndex < min)
			currentIndex = min;
		else
		if(currentIndex > max)
			currentIndex = max;
		setFramePosition(currentIndex);
	}
}

// Choose which of the animate routines to execute.
void AvAnimPanel::animate(const int index)
{
	if(blinkIsEnabled())
		animate_blink(index);
	else
		animate_normal(index);
}

/* Main work procedure for normal animating.
	Scan list of windows in animate list and tell them to show their
next frame.

If no windows have their "Include" flags set, the first window with a
multi-plane image will be animated. If any windows have their Include
flags set, only those will be animated. The first window opened of
those being animated determines the number of frames shown.

A rescan is performed each time period since it possible for windows
to suddenly vanish or to be added.

It is assumed that each viewItemPanel will do the right thing if told to
show an illegal slice (ie nothing).

If index is valid, currentIndex will be set to it before showing the
next frame. Otherwise, currentIndex will be used. (Used when in single step
mode).
*/
void AvAnimPanel::animate_normal(const int index)
{
AvImageViewItem **views, *master;
AvImageViewPanel *ip;
int		viewcount;
AvImageView::ItemSelection which=AvImageView::ALL;
int		i, sliceNum, first;

	/* Get list of all views.
	*/
	views = imageView_->getImageViewList(viewcount, which);

	/* Search the list for a master (The dataset that determines how many
	   frames to show).
	*/
	master = NULL;
	first = -1;		// Index of first window with multiple slices.
	for(i=0; i < viewcount; i++)
	{AvImageViewItem *vi = views[i];
		// Ignore all but multi-slice images.
		if(vi->getImageWindow()->viewPanel()->numSlices() <= 1)
			continue;

		// First window with more than 1 slice.
		if(first < 0)
		{	first = i;
			if(master != NULL)
				break;
		}
		// Find the first included window.
		if(included(vi))
		{	master = vi;
			break;
		}
	}

	// If master is NULL, use the first multi-slice window. It none, punt.
	if(master == NULL)
	{	if(first < 0)		// No usable windows.
		{	stop();
			delete [] views;
			return;
		}
		else
			master = views[first];
	}

	// Update the min/max indices.
	ip = master->getImageWindow()->viewPanel();
	if(relativeIndexing())
		setMinMaxIndex(0, ip->numSlices()-1);
	else
		setMinMaxIndex(ip->getStartSlice(), ip->getEndSlice());

	if(validIndex(index))
		currentIndex = index;
	// Get next slice to show.
	sliceNum = getNextIndex(minIndex_, maxIndex_);

	if(sliceNum < 0) // Stop if no valid slices.
	{	stop();
		delete [] views;
		return;
	}

	// Update slider bar unless changing the slider resulted in this call.
	// (Needs changing if SingleStep is ever done elsewhere too).
	if(animateMode != SingleStep)
		setFramePosition(sliceNum);

	// Go through the list telling each included window to show its slice.
	// The master is always 'included'.
	// This is done in four passes. The first tells each CLUImageSetter
	// to change its current index so that if object[I] includes
	// objects from object[I+n], the overlayed objects will be the
	// correct ones, rather than the previous ones.
	// (imageSetter() does get called twice, though).
	// The last two passes tell all the canvases to flush their buffers
	// and then wait.
	for(i = first; i< viewcount; i++)
	{AvImageViewItem *vi = views[i];

		AvAccessor *a = vi->accessor();
		int start = a->getAxisStart(a->zAxis());
		int sliceIndex = relativeIndexing() ? start + sliceNum
						    : sliceNum;
		if(included(vi) && (sliceIndex >= start)
		   && (sliceIndex < start + a->getAxisLength(a->zAxis())))
			vi->accessor()->setPlane(sliceIndex, FALSE);
	}

	for(i = first; i< viewcount; i++)
	{AvImageViewItem *vi = views[i];

		AvAccessor *a = vi->accessor();
		int start = a->getAxisStart(a->zAxis());
		int sliceIndex = relativeIndexing() ? start + sliceNum
						    : sliceNum;
		if( 	((vi == master) || included(vi)) &&
			(sliceIndex >= start) &&
			(sliceIndex < start + a->getAxisLength(a->zAxis()))
		  )
			vi->getImageWindow()->viewPanel()->setSlice(sliceIndex);
	}

	// Wait until finished. This is really necessary on slow remote
	// displays.
	// Tell each canvas to flush its buffers.
	for(i = first; i< viewcount; i++)
	{AvImageViewItem *vi = views[i];

		AvAccessor *a = vi->accessor();
		int start = a->getAxisStart(a->zAxis());
		int sliceIndex = relativeIndexing() ? start + sliceNum
						    : sliceNum;
		if( 	((vi == master) || included(vi)) &&
			(sliceIndex >= start) &&
			(sliceIndex < start + a->getAxisLength(a->zAxis()))
		  )
		{ AvCanvas *c = vi->getImageWindow()->avcanvas();
			c->flush();
		}
	}

	// After all have started flushing, go back and wait.
	for(i = first; i< viewcount; i++)
	{AvImageViewItem *vi = views[i];

		AvAccessor *a = vi->accessor();
		int start = a->getAxisStart(a->zAxis());
		int sliceIndex = relativeIndexing() ? start + sliceNum
						    : sliceNum;
		if( 	((vi == master) || included(vi)) &&
			(sliceIndex >= start) &&
			(sliceIndex < start + a->getAxisLength(a->zAxis()))
		  )
		{ AvCanvas *c = vi->getImageWindow()->avcanvas();
			c->wait();
		}
	}

	delete [] views;
}

/* Do 'Blink' Animation.
	Foreach window with the include flag set:
		Get its display list and add to our own display list.

	Show next item in our display list.
*/

void AvAnimPanel::animate_blink(const int index)
{
int sliceNum, maxIndex;

	blinkWindow_->updateList();

	// Update the min/max indices.
	maxIndex = blinkWindow_->getNumObjects() - 1;
	setMinMaxIndex(0, maxIndex);
	if(validIndex(index))
		currentIndex = index;
	// Get next slice to show.
	sliceNum = getNextIndex(minIndex_, maxIndex);

	if(sliceNum < 0) // Stop if no valid slices.
	{	stop();
		return;
	}

	// Update slider bar unless changing the slider resulted in this call.
	// (Needs changing if SingleStep is ever done elsewhere too).
	if(animateMode != SingleStep)
		setFramePosition(sliceNum);

	blinkWindow_->showItem(sliceNum);
	blinkWindow_->showImage();
}

// Returns the next slice to show or -1 if none.
// Uses and modifies the internal animation control variables.
int AvAnimPanel::getNextIndex(const int minIndex, const int maxIndex)
{
int newSlice = currentIndex;
int ret;

	switch (animateMode) {
	case Stopped:
		ret = -1;
		break;
	case SingleStep:
		if(validIndex(currentIndex))
			ret = newSlice;
		else
			ret = -1;
		break;
	case Once:
		if((newSlice + animateChange > maxIndex) && (animateChange>0)
					||
		   (newSlice + animateChange < minIndex) && (animateChange< 0))
		{	ret = -1;
		}
		else
		{	newSlice += animateChange;
			ret = newSlice;
		}
		break;

	case BackForth:
	// if we're outside the bounds, turn around and go the other direction
		if(newSlice + animateChange > maxIndex ||
		   newSlice + animateChange < minIndex)
		{
			animateChange = - animateChange;
		}
		newSlice += animateChange;
		ret = newSlice;
		break;
	case Cycle:
		newSlice += animateChange;
		if (newSlice > maxIndex)
		{
			newSlice = minIndex;
		}
		ret = newSlice;
		break;
	default:
		ret = -1;	// Should't get here. For picky compilers.
		break;
	}
	if(ret >= minIndex)
		currentIndex = ret;
	return ret;
}
