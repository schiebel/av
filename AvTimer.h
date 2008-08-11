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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvTimer.h,v 19.0 2003/07/16 05:46:43 aips2adm Exp $
//
// $Log: AvTimer.h,v $
// Revision 19.0  2003/07/16 05:46:43  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:29  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:41:58  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:07  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:34  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:07:44  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:27  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:41  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:24  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:01  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:24:45  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:07  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 07:33:37  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.4  1996/06/18  18:43:24  pixton
//  Copyright Update
//
//  Revision 1.3  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/04/10  20:40:39  hr
//  Delay interval can be specified as ticks/sec in addition to ms.
//
//  Revision 1.1  1995/04/04  20:23:01  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

/*AvTimer.h

Used to periodically call a routine approximately every interval
milliseconds.

Normally, AvTimer acts as a recurring timer, but can be configured to be
a one shot.

Currently, only one callback can be handled at once, but this limitation
may be removed in the future.

The timer can be (re)started at any time. If it is currently running, it
stops itself and restarts.

If the interval is changed while the timer is running, the timer stops and
restarts with the new interval.

The callback is an AvTimerCallback procedure:
	void proc(AvTimer *timer, XtPointer cbdata, XtPointer call_data);

timer will point to the timer making the call.
cbdata is the pointer passed in the addCallback function.
call_data is currently 0.

NOTES:
	Interval is forced to be at least 1ms since 0 severely impacts
response to events.

	Since this is a subclass of resource, use timer->ref() after
creation and timer->unref() to destroy.

*/

#ifndef AvTimer_h
#define AvTimer_h
#include <X11/Intrinsic.h>
#include <AvResource.h>

class AvTimer : public AvResource {
  public:
	// Interval is in MS. If once is TRUE, the timer won't automatically
	// restart itself. (use start()).
	AvTimer(XtAppContext app, const float interval=0.0,
			const Boolean tps=FALSE, const Boolean once=FALSE);
	// As above except the XtAppContext is retrieved from the widget.
	AvTimer(Widget ref, const float interval=0.0,
			const Boolean tps=FALSE, const Boolean once=FALSE);
	// The callback needs to be one of these.
	typedef void(*AvTimerCallback)
			(AvTimer *, XtPointer cbdata, XtPointer );
	// Add/remove callback.
	void addCallback(AvTimerCallback, XtPointer cbdata);
	void removeCallback(AvTimerCallback, XtPointer cbdata);
	// (Re)start timer using existing interval.
	void start();
	// (Re)start timer using interval (does not change preset interval).
	void start(const unsigned long interval);
	void stop();
	/* if tps is TRUE, then interval is the number of ticks/per second
	   to generate. The default is FALSE which means the delay between
	   ticks in MS.
	*/
	void setInterval(const float interval, const Boolean tps=FALSE);
	float getInterval(const Boolean tps=FALSE)const;
	Boolean	isRunning()const {return (id_ != 0);}
	Boolean isOneShot()const {return onlyOnce_;}
	void	callCallback();		// Calls the callback(s).
  protected:
	~AvTimer();
  private:
	void	execute();
	static void timerCB(XtPointer, XtIntervalId *);
	void	init(XtAppContext app, const float interval,
			const Boolean tps=FALSE, const Boolean once=FALSE);
  private:
	XtAppContext	app_;
	XtIntervalId	id_;
	Boolean		onlyOnce_;
	unsigned long	interval_;
	AvTimerCallback callback_;
	XtPointer	cbdata_;
	long		ticks_sec_;
};

#endif
