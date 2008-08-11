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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvEvent.cc,v 19.0 2003/07/16 05:47:13 aips2adm Exp $
//
// $Log: AvEvent.cc,v $
// Revision 19.0  2003/07/16 05:47:13  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:55  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:25  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:31  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:19  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:17  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:04  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:46  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:56  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:48  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:27:36  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:49  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:13:10  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.4  1996/06/18  18:45:59  pixton
//  Copyright Update
//
//  Revision 1.3  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/05/11  21:49:57  hr
//  Added WindowResize and WindowExpose to list of events.
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvEvent.C

*/
#include <AvEvent.h>

void AvEvent::init(const EventType t, const float x, const float y,
		   const int keymask, AvCanvas *canvas)
{
	type_ = t;
	keymask_ = keymask;
	position(x, y);
	canvas_ = canvas;
	handled_ = FALSE;
}

AvEvent::AvEvent(const EventType t, const float x, const float y,
		 const int keymask, AvCanvas *canvas)
{
	init(t, x, y, keymask, canvas);
}

AvEvent::AvEvent()
{
	init(NOTYPE, 0.0, 0.0, AvNoKeyMask, NULL);
}

AvEvent::AvEvent(const AvEvent &e)
{
	init(e.type_, e.x(), e.y(), e.keymask_, e.canvas_);
}

void	AvEvent::setHandled()
{	handled_ = TRUE;
}

void	AvEvent::position(const float x, const float y)
{	
	xy_.value(AvDimensionX, x);
	xy_.value(AvDimensionY, y);
}

AvButtonEvent::AvButtonEvent(const float x, const float y,
			const int keymask, const Boolean pressed,
			const AvButton b, AvCanvas *canvas):
	AvEvent(pressed ? AvButtonPress : AvButtonRelease, x, y,
		keymask, canvas)
{
	button_ = b;
}

AvButtonEvent::AvButtonEvent(const AvButtonEvent &e) : AvEvent(e)
{
	button_ = e.button_;
}

AvMotionEvent::AvMotionEvent(const float x, const float y,
			const int keymask, AvCanvas *canvas):
	AvEvent(AvPointerMotion, x, y, keymask, canvas)
{
}

AvMotionEvent::AvMotionEvent(const AvMotionEvent &e) : AvEvent(e)
{
}

AvResizeEvent::AvResizeEvent(const float width, const float height)
{
	AvEvent::init(AvWindowResize, 0.0, 0.0, 0, NULL);
	width_ = width;
	height_ = height;
}

/* The character string is not assumed to be NULL terminated and may
contain NULLs. A NULL is appended to the end of the supplied string in
case the user tries to treat it as such.
*/
AvKeyEvent::AvKeyEvent(const float x, const float y,
		   const int keymask, const Boolean pressed,
		   const char *keys, const int len, AvCanvas *c):
		AvEvent((pressed) ? AvKeyPress : AvKeyRelease,
			x, y, keymask, c)
{
	if(len <= 0)
	{	count_ = 0;
		keys_ = NULL;
	}
	else
	{	count_ = len;
		keys_ = new char[len+1];
		memcpy(keys_, keys, len);
		keys_[len] = '\0';		// Ensure NULL termination.
	}
}

AvKeyEvent::AvKeyEvent(const AvKeyEvent &e) : AvEvent(e)
{
	count_ = e.count_;
	if(count_> 0)
	{	keys_ = new char[count_+1];
		memcpy(keys_, e.keys_, count_);
		keys_[count_] = '\0';		// Ensure NULL termination.
	}
	else
		keys_ = NULL;
}

AvKeyEvent::~AvKeyEvent()
{
	if(keys_ != NULL)
		delete [] keys_;
}


	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////


void AvCallback::init(int mask, AvCallbackProc proc, AvPointer data)
{
	eventmask_ = mask;
	proc_ = proc;
	data_ = data;
}

AvCallback::AvCallback(int mask)
{
	init(mask, NULL, NULL);
}

AvCallback::AvCallback(int mask, AvCallbackProc proc, AvPointer data)
{
	init(mask, proc, data);
}

AvCallback::~AvCallback(){}

void AvCallback::mask(int mask)
{
	eventmask_ = mask;
}

int AvCallback::mask()
{
	return eventmask_;
}

void AvCallback::data(AvPointer data)
{
	data_ = data;
}

AvPointer AvCallback::data()
{
	return data_;
}

void AvCallback::execute(AvEvent &event)
{
	if(proc_ != NULL)
		proc_(event, data_);
}

