//# Copyright (C) 1995-2000 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvAnimPanel.h,v 19.0 2003/07/16 05:46:27 aips2adm Exp $
//
// $Log: AvAnimPanel.h,v $
// Revision 19.0  2003/07/16 05:46:27  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:15  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:41:42  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:41:53  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:07:32  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 18:20:46  hravlin
// Changed className() to return const char *
//
// Revision 14.0  2000/03/23 16:07:27  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:06  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:08  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:57:36  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:51:03  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:23:14  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.1  1997/04/15 20:12:24  hr
// Added 'relative indexing'.
//
// Revision 8.0  1997/02/20 03:14:09  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/02/06 17:53:46  hr
// Added updateFrameSlider().
//
// Revision 7.2  1996/12/12 05:30:51  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.6  1996/06/18  18:42:36  pixton
//  Copyright Update
//
//  Revision 1.5  1996/06/18  18:38:48  pixton
//  Copyright Update
//
//  Revision 1.4  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/07/17  19:56:12  hr
//  Added 'virtual' to destructor of virtual classes.
//
//  Revision 1.2  1995/04/24  18:40:42  hr
//  Added support for blinking.
//
//  Revision 1.1  1995/04/19  14:25:57  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

#ifndef _ANIM_PANEL_H
#define _ANIM_PANEL_H

#include <Xm/Xm.h>

#include "AvFormComp.h"

class AvAnimCtlr;
class AvImageView;
class AvTimer;
class AvImageViewItem;
#include "AvBlinkWindow.h"

class AvAnimPanel : public AvFormComp {

  public:

	AvAnimPanel(Widget parent, int buildInsideParent);
	virtual ~AvAnimPanel();

	virtual const char * className() const;
	void start();
	void stop();
	void setRate(const int percent);
	void showIndex(const int index);	// Show this frame #.
	Boolean validIndex(const int index)const
		 {return ( (index >= minIndex_) && (index <= maxIndex_));}
	void setFramePosition(const int index);
	Boolean running()const{ return animateMode > SingleStep;}
	int getFramePosition()const;
	Boolean blinkIsEnabled()const;
	void enableBlink(const Boolean enable);
	void updateFrameSlider();	// Redraw frame slider's readouts.
	// If TRUE, each frame is incremented by 1 rather than being
	// set. Allows showing of frames at different offsets.
	Boolean		relativeIndexing()const{return relativeIndexing_;}
  protected:
	void setMinMaxIndex(const int min, const int max);
	// Convenience function to change a toggle button's state;
	void setToggle(Widget, const Boolean ON);
  private:
	void	init();
	// Compute the next slice index.
	int getNextIndex( const int minIndex, const int maxIndex);
	void makeBlinkWindow();
  private:
	// Builds the controls.
	void buildAnimateControls(Widget parent);
	void animateButton(Widget w);
	void blinkButton(Widget w);

	// called by timerCB;
	void	animate(const int index=-1);
	void	animate_blink(const int index=-1);
	void	animate_normal(const int index=-1);
	enum AnimateMode { Stopped, SingleStep, Once, BackForth, Cycle };
	AnimateMode animateMode;
	int animateChange;
	int currentIndex;
	int minIndex_, maxIndex_;
	// CALLBACKS
	// Called when a control button is pressed.
	static void animateButtonCB(Widget, XtPointer , XtPointer);
	static void blinkButtonCB(Widget, XtPointer , XtPointer);
	static void frameSliderCB(Widget, XtPointer cbdata, XtPointer call);
	static void rateSliderCB(Widget, XtPointer cbdata, XtPointer calldata);
	// Called on each timer tick.
	static	void timerCB(AvTimer *, XtPointer cbdata, XtPointer );
	// Called by the blinkwindow
	static void blinkWCB(AvBlinkWindow *, XtPointer ,
					AvBlinkWindow::CBREASON);
	static void relativeCB(Widget, XtPointer cbdata, XtPointer calldata);
	static XImage goOnce, goBackForth, goCycle;
	Widget animateOnce, animateBackAndForth, animateCycle;
	Widget	frameSlider_;		// Scale widget to choose frame #
	Widget	rateSlider_;

	XtAppContext	app_;
	AvTimer		*timer_;
	AvImageView	*imageView_;
	Widget		blinkToggle_;
	AvBlinkWindow	*blinkWindow_;	// Non NULL iff we're doing blinking.
	Boolean		relativeIndexing_;
};

#endif
