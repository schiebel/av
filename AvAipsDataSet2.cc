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
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvAipsDataSet2.cc,v 19.0 2003/07/16 05:48:07 aips2adm Exp $

// This file is separated from AvAipsDataSet.cc to avoid clashes between
// X and AIPS++ headers, e.g. String.

#include <AvAipsDataSet.h>
#include <AvApp.h>
#include <AvTimer.h>

AvTimer *AvAipsDataSet::timer_ = 0;
int AvAipsDataSet::idle_count_ = 0;

// Conflicting X/AIPS++ definitions
#ifdef True
#undef True
#endif
#ifdef False
#undef False
#endif

void AvAipsDataSet::start_timer_if_necessary() const
{
  // Make one timer for the entire process to release locks
  if (timer_ == 0) {
#if defined(AIPS_DEBUG)
      cerr << "aipsview: timer started" << endl;
#endif
    // Relinquish locks every 5000ms
    timer_ = new AvTimer(AvApp::appContext());
    timer_->ref();
    timer_->addCallback(timerCB, 0);
    timer_->setInterval(1.0/5.0, True); // 1/5 ticks per second, i.e. one per 5s
    timer_->start();
  }
  // Reset since we've just been called from a member function, so interest
  // in this data object must still be high.
  idle_count_ = 0;
}

void AvAipsDataSet::timerCB(AvTimer *, XtPointer , XtPointer)
{
#if defined(AIPS_DEBUG)
  cerr << "aipsview: idle function entered" << endl;
#endif

  static Bool inUse = False;
  if (inUse) {
    cerr << "Entered active callback! (AvAipsDataSet.cc)" << endl;
    return;
  } else {
    inUse = True;
  }
  idle_count_++;
  if (idle_count_ < 10) {
    Table::relinquishAutoLocks();
  } else {
    // Turn off the timer if we can to let aipsview swapout
    idle_count_ = 0;
    Table::relinquishAutoLocks(True);
    if (Table::nAutoLocks() == 0) {
      stop_timer();
#if defined(AIPS_DEBUG)
      cerr << "aipsview: all locks relinquished and timer shutdown" << endl;
#endif
    }
  }
    inUse = False;
}

void AvAipsDataSet::stop_timer()
{
  if (timer_) {
    timer_->stop();
    timer_->unref();
    timer_ = 0;
    idle_count_ = 0;
  }
}

