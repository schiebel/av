//# Copyright (C) 1995-97 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvEvent.h,v 19.0 2003/07/16 05:47:03 aips2adm Exp $
//
// $Log: AvEvent.h,v $
// Revision 19.0  2003/07/16 05:47:03  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:47  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:15  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:23  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:41  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:06  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:53  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:24  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:26  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:13  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:26:40  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:14  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:14:35  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.5  1996/06/18  18:42:49  pixton
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
//  Revision 1.2  1995/05/11  21:55:14  hr
//  Added WindowResize and WindowExpose to list of events.
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvEvent.h
*/
#ifndef AvEvent_h
#define AvEvent_h

class AvCanvas;
#include <AvGeometry.h>

enum AvKeyMask { AvNoKeyMask=0, AvShiftMask=1, AvLockMask=2,
		AvControlMask=4, AvMod1Mask=8,
		AvMod2Mask=0x10, AvMod3Mask=0x20,
		AvButton1Mask=0x100, AvButton2Mask=0x200,
		AvButton3Mask=0x400
	};

class AvEvent {
  friend class AvCanvas;
  public:

	enum EventMask { AvNOMask=0,
		AvButtonPressMask=1, AvButtonReleaseMask=2,
		AvPointerMotionMask=4,
		AvButton1MotionMask=8, AvButton2MotionMask=0x10,
		AvButton3MotionMask=0x20,
		AvKeyPressMask=0x40, AvKeyReleaseMask=0x80,
		AvEnterWindowMask=0x100, AvLeaveWindowMask=0x200,
		AvWindowResizeMask=0x400, AvWindowExposeMask=0x800,
		// Pseudo events.
		AvPseudoEventMasks=(1<<16),
		// If set gets events anywhere in window rather than only
		// when cursor is in outermost viewport.
		AvAnyWhereMask=AvPseudoEventMasks,
		AvEnterViewportMask=AvPseudoEventMasks << 1,
		AvLeaveViewportMask=AvPseudoEventMasks << 2,
		AvPseudoEventMask= (~0 & 0xffff)
		};
	enum EventType { NOTYPE=0,
		AvKeyPress, AvKeyRelease,
		AvButtonPress, AvButtonRelease,
		AvPointerMotion, 
		AvEnterWindowNotify, AvLeaveWindowNotify,
		AvEnterViewportNotify, AvLeaveportNotify,
		AvWindowResize, AvWindowExpose
		};

	// Returns true if event has been handled.
	inline Boolean	isHandled(){return handled_;}
	void	setHandled();	// Sets isHandled flag.
	AvEvent(const EventType t, const float x, const float y,
		const int keymask, AvCanvas *canvas=NULL);
	AvEvent();
	AvEvent(const AvEvent &);
	inline AvAllotment position()const { return xy_;}
	inline float x()const { return xy_.x();}
	inline float y()const { return xy_.y();}
	inline EventType eventType()const {return type_;}
	inline AvCanvas	*canvas(){return canvas_;}	// Where event occurred
	inline int keymask()const { return keymask_;}
	// Utility functions to test state of keymask.
	inline Boolean keysPressed()const{ return (keymask_ != AvNoKeyMask);}
	inline Boolean shiftPressed()const{return (keymask_ & AvShiftMask)!=0;}
	inline Boolean lockPressed()const{return (keymask_ & AvLockMask)!=0;}
	inline Boolean controlPressed()const{return
				(keymask_ & AvControlMask)!=0;}
	inline Boolean mod1Pressed()const{return (keymask_ & AvMod1Mask)!=0;}
	inline Boolean mod2Pressed()const{return (keymask_ & AvMod2Mask)!=0;}
	inline Boolean mod3Pressed()const{return (keymask_ & AvMod3Mask)!=0;}
	inline Boolean button1Pressed()const{return
				(keymask_ & AvButton1Mask)!=0!=0;}
	inline Boolean button2Pressed()const{return
				(keymask_ & AvButton2Mask)!=0;}
	inline Boolean button3Pressed()const{return
				(keymask_ & AvButton3Mask)!=0;}
  protected:
	inline void position(const AvAllotment &xy){xy_ = xy;}
	void position(const float x, const float y);
	inline void keymask(const int k){ keymask_=k;}
	inline void eventType(const EventType &t){type_ = t;}
	inline void canvas(AvCanvas *c){ canvas_=c;}	// Where event occurred

	EventType	type_;
	AvCanvas	*canvas_;	// Where event occurred
	int		keymask_;	// Is CTL, SHIFT, etc key pressed too?
					// (Can't use AvKeyMask since undefined
					//  combinations will occur.
	// Position within window.
	AvAllotment	xy_;
	void	init(const EventType t,	const float x, const float y,
		     const int keymask, AvCanvas *canvas);
  private:
	Boolean		handled_;
};

enum AvButton { AvNoButton=0, AvButton1=1, AvButton2, AvButton3 };

class AvButtonEvent : public AvEvent {
  public:
	AvButtonEvent(const float x, const float y,
			const int keymask, const Boolean pressed,
			const AvButton b, AvCanvas *c=NULL);
	inline AvButton button()const { return button_;}
	AvButtonEvent(const AvButtonEvent &);
  protected:
	inline void button(const AvButton b){ button_=b;}
	AvButton	button_;	// Which button is it?
};

class AvMotionEvent : public AvEvent {
  public:
	AvMotionEvent(const float x, const float y,
			const int keymask, AvCanvas *c=NULL);
	AvMotionEvent(const AvMotionEvent &);
};

/* The keyevent isnot usable yet (X Canvas doesn't handle characters).
The keys array will be NULL terminated, however it may not be a 'valid'
UNIX char string since it may contain embedded NULLs. numkeys() returns
the number of keys given to the constructor, not the NULL char added by
KeyEvent.
*/
class AvKeyEvent : public AvEvent {
  public:
	AvKeyEvent(const float x, const float y,
		   const int keymask, const Boolean pressed,
		   const char *keys, const int len, AvCanvas *c=NULL);
	AvKeyEvent(const AvKeyEvent &);
	~AvKeyEvent();
	inline char *keys(){return keys_;}
	inline int numkeys()const {return count_;}
  protected:
	char	*keys_;			// key string.
	unsigned int count_;		// Length of string.
};

// X/Y is the position of the window, not the pointer.
class AvResizeEvent : public AvEvent {
  public:
	AvResizeEvent(const float width, const float height);
	float width()const {return width_;}
	float height()const{return height_;}
  private:
	float	width_, height_;
};

#if 0
class AvRubberBandEvent : public AvEvent {
  public:
	AvAllotment	xy1;
	Boolean		canceled;
};
#endif

/* This class is passed to the Canvas::addCallback function. Its execute
function is called whenever a requested event is seen. The eventmask
determines which events are responded to.

Two constructors are provided. the first is for the case where AvCallback
is subclassed. The second is where the user wishes to use a more nearly
'X' style interface.
*/

typedef void* AvPointer;
typedef void(*AvCallbackProc)(AvEvent &, AvPointer);


class AvCallback {
  public:
	AvCallback(int eventmask=0);
	AvCallback(int eventmask, AvCallbackProc, AvPointer=NULL);
	virtual ~AvCallback();
	virtual void execute(AvEvent &);
	virtual int mask();
	virtual void mask(int eventmask);
	virtual AvPointer data();
	virtual void data(AvPointer);
  private:
	void	init(int eventmask, AvCallbackProc, AvPointer);
	int	eventmask_;
	AvCallbackProc	proc_;
	AvPointer	data_;
};
#endif
