//# Copyright (C) 1995-98 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvTimer.cc,v 19.0 2003/07/16 05:46:38 aips2adm Exp $
//
// $Log: AvTimer.cc,v $
// Revision 19.0  2003/07/16 05:46:38  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:25  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:41:53  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:03  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:17  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:07:40  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:22  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:34  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:11  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:51:46  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.2  1998/07/06 21:18:14  hr
// Really fixed the unsigned long problem (I hope).
//
// Revision 9.1  1998/06/09 20:35:00  hr
// Fixed a long vs unsigned long comparision in execute().
//
// Revision 9.0  1997/08/25 21:24:21  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:14:52  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/02/05 18:00:16  hr
// Changed an int/unsigned comparision.
//
// Revision 7.2  1996/12/12 07:33:19  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.6  1996/06/18  18:46:37  pixton
//  Copyright Update
//
//  Revision 1.5  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/04/11  15:01:40  hr
//  Set min execute interval to 10ms since 1ms was causing response delays.
//
//  Revision 1.3  1995/04/10  20:39:46  hr
//  Delay interval can be specified as ticks/sec in addition to ms.
//
//  Revision 1.2  1995/04/10  15:04:29  hr
//  execute now makes sure timer is running before restarting.
//
//  Revision 1.1  1995/04/04  20:21:44  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

/*AvTimer.cc
*/

#include "AvTimer.h"
#include <sys/types.h>
#include <sys/times.h>		// clock_t
#include <unistd.h>		// sysconf (needed to get ticks/sec).

// Minimum user settable delay in ms.
static const unsigned long MINDELAY = 10;
// Minimum delay in ms between callbacks. Must be at least 1.
static const unsigned long MININTERVAL = 10;

AvTimer::AvTimer(XtAppContext app, const float interval,
				const Boolean tps, const Boolean once)

{
	init(app, interval, tps, once);
}

// Creates a timer object using the widget to get the app context.
AvTimer::AvTimer(Widget ref, const float interval,
				const Boolean tps, const Boolean once)
{
	init(XtWidgetToApplicationContext(ref), interval, tps, once);
}

void	AvTimer::init(XtAppContext app, const float interval,
				const Boolean tps, const Boolean once)
{
	app_ = app;
	onlyOnce_ = once;
	id_ = 0;
	callback_ = NULL;
	cbdata_ = NULL;
	interval_ = MINDELAY;	// In case interval is bad.
	setInterval(interval, tps);

	// The number of clock ticks/second.
	// If sysconf isn't defined on some system, set ticks_sec_ to 0.
	ticks_sec_ = sysconf(_SC_CLK_TCK);
}

AvTimer::~AvTimer()
{
	stop();
}

// Use cb as the callback procedure.
void AvTimer::addCallback(AvTimerCallback cb, XtPointer cbdata)
{
	callback_ = cb;
	cbdata_ = cbdata;
}

// Remove the callback. Currently only one callback is supported. The
// args are in case a callback list is added later.
void AvTimer::removeCallback(AvTimerCallback , XtPointer )
{
	callback_ = NULL;
}

// (Re)starts the timer using preset interval.
void AvTimer::start()
{
	start(interval_);
}

/* (Re)starts the timer using this interval. Future restarts will use
internal interval_. The delay is forced to be > 0 since 0 seems to
effectively turn off response to events. The interval is allowed to be
less than MINDELAY since execute() may be trying to keep a constant rate.

If this is called while running, the existing timeout will be removed first.
The effective delay will then be up to (old interval + this interval).
*/
void AvTimer::start(const unsigned long interval)
{
unsigned long intrvl = interval;

	if(intrvl == 0) intrvl = MININTERVAL;
	if(isRunning())
		stop();
	if(app_ != NULL)
	  id_ = XtAppAddTimeOut(app_, intrvl, timerCB, (XtPointer)this);
}

// Stops the timer
void AvTimer::stop()
{
	if(id_ != 0)
	{	XtRemoveTimeOut(id_);
		id_ = 0;
	}
}

/* Sets the interval, restarting if necessary.
interval is in MS unless tps is TRUE in which case interval is in ticks/sec.
*/

void AvTimer::setInterval(const float interval, const Boolean tps)
{
float	ms;

	if(interval <= 0)
		return;

	ms = (tps) ? 1000.0/interval : interval;

	interval_ = (unsigned long) ms;
	if(interval_ < MINDELAY)
		interval_ = MINDELAY;

	if(isRunning())		// start() will do the stop.
		start();
}

float AvTimer::getInterval(const Boolean tps)const
{
float	val = (float) interval_;

	if( (val > 0.0) && (tps))
		val = 1000.0/val;

	return val;
}

// Call any callback(s).
void AvTimer::callCallback()
{
	if(callback_ != NULL)
		(callback_)(this, cbdata_, 0);
}

/* Called when the interval times out. Calls callbacks, and restarts unless
this is a one shot timer or the callbacks have stopped the timer.

The time taken to execute the callbacks is subtracted from the delay time
so that the total delay will be about interval_ ms. If the callback time
is too long, use MININTERVAL.
*/
void AvTimer::execute()
{
clock_t startt, stopt, elapsed;
struct tms dmy;
unsigned long newInterval;

	startt = times(&dmy);	// Time how long it takes to execute callbacks.
		callCallback();
	stopt = times(&dmy);
	if(!isOneShot() && isRunning())
	{	// Compute the elapsed time in ms if we have the constant.
		if(ticks_sec_ > 0)
		{	elapsed = (stopt - startt)*1000;
			elapsed /= ticks_sec_;
		}
		else	// If not, use the entire interval.
			elapsed = 0;
		/* Subtract the elapsed time from the delay. While we
		   won't allow the user to choose a delay less than
		   MINDELAY, we go down to MININTERVAL to attempt to keep
		   a constant callback rate. 1ms will still allow events
		   to take place, although it may be too small on slow
		   systems.
		*/
		if((unsigned long)elapsed >= interval_)
			newInterval = MININTERVAL; // If would be negative.
		else
		{	newInterval = interval_ - elapsed;
			if(newInterval < MININTERVAL)
				newInterval = MININTERVAL;
		}
		start(newInterval);
	}
}

void AvTimer::timerCB(XtPointer cbdata, XtIntervalId *)
{
AvTimer *me = (AvTimer *) cbdata;

	if(me != NULL)
		me->execute();
}
