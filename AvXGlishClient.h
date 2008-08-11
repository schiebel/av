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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvXGlishClient.h,v 19.0 2003/07/16 05:47:08 aips2adm Exp $
//
// $Log: AvXGlishClient.h,v $
// Revision 19.0  2003/07/16 05:47:08  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:51  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:20  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:27  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:59  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:11  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:58  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:35  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:40  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:30  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:27:06  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:30  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1996/12/28 00:28:37  bglenden
// include "time.h" so linux can find a missing #define (FD_SETSIZE).
//
// Revision 7.2  1996/12/12 07:48:17  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.3  1996/06/18  18:43:37  pixton
//  Copyright Update
//
//  Revision 1.2  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

//
//
//  The goal here is to design a CLEAN interface to X from glish.
//
//  Specification:
//
//  0) Dependencies
//
//  Requires three libraries, which together form the glish library to
//  compile: -lglish -lsds -ledit. 
//
//--------------------------------------------------------------------------
//
//  1) Creating
//
//     The user creates the AvXGlishClient, specifying the procedure to
//  be called when input is available on any glish-controlled file
//  descriptor. 
//
//  AvXGlishClient * xgclient = new AvXGlishClient(argc, // command args
//                                             argv,  // 
//                                             app,   // X AppContext  
//                                             glishEventProc, // See below
//                                             userData);
//
//  ---int& argc, char *argv[]---
//  argc is passed by reference, so the REAL argc from main should be used
//  after X has removed its arguments (usually from XtAppInitialize()).  
//
//  ---XtAppContext app---
//  app is the initialized application context, required for the calls to 
//  XtAppAddInput.  As directed by the Xt manuals, the application context
//  is passed as a parameter instead of assuming the default app context.
//
//  ---void (*glishEventProc)(_,_,_,_)---
// The function passed here should be defined as below:
//
//  void Xgeh(AvXGlishClient * xgclient,  // gives the pointer to the glish
//                                      // client.  This allows the program
//                                      // to create the client once and 
//                                      // forget about keeping track of
//                                      // a pointer to it.
//            GlishEvent * ge,          // Pointer to the glish event.  The
//                                      // function can switch on ge->name to
//                                      // determine which event name was
//                                      // received (type char *).
//            int fd,                   // The file descriptor with the
//                                      // available input.  UNDER NO 
//                                      // CIRCUMSTANCES SHOULD THIS FILE
//                                      // DISCRIPTOR BE CLOSED OR MODIFIED
//                                      // VIA IOCTL OR FNCTL!
//            XtPointer userData)       // Pointer to client data.  Not to
//                                      // be confused with e->value, the 
//                                      // event value
//  {
//    if (!strcmp(ge->name, "event1")) doSomething();
//    else if (!strcmp(ge->name, "event2")) doSomethingElse();
//    ...
//    else xgclient->Unrecognized()  // It is important to put this in to
//                                   // help track event name typos and
//                                   // missing events.  A message is sent
//                                   // to the interpreter when an event
//                                   // is not handled in the if switch 
//                                   // above.
//  }
//
//  ---userData---
//  A pointer for user use.  It will be passed along to the callback function
//  given in the constructor.  
//
//---------------------------------------------------------------------------
//
//  2) Using
//
//  Whenever glish events appear on any glish-controlled file descriptor, the
//  callback function is executed.  AvXGlishClient automatically tracks
//  connection changes introduced by external processes and reports them to
//  the X server via calls to XtAppAddInput() and XtRemoveInput().
//
//  Due to inheritance from Client, all of the ordinary glish "Client" 
//  class functions are available.  These include:
//
//  * Unrecognized(), which must be called by the callback function if
//    the event is not recognized by the parser.
//
//  * PostEvent has many versions and is used to send an event to the
//    interpreter.  Often used to respond to events or report when an
//    activity is finished.   [Glish manual, page 107]
//
//    PostEvent(GlishEvent * event)
//    PostEvent(const char * name)
//    PostEvent(const char * name, const Value * value)
//    PostEvent(const char * name, const char * format, const char * arg)
//    (same as above, only two character arguments)
//
//  * 

#ifndef __AvXGlishClient_h__
#define __AvXGlishClient_h__

#include <X11/Intrinsic.h>

#include <string.h>
#include <time.h>
#include <Glish/Client.h>
#include <unistd.h>
#include <stdio.h>

class AvXGlishClient : public Client
{
 public:

  AvXGlishClient(int& argc, 
	       char *argv[],
	       XtAppContext app,
	       void (* proc)(AvXGlishClient *, GlishEvent *, int fd, XtPointer),
	       XtPointer clientData) 
    : Client(argc, argv), app_(app), proc_(proc), clientData_(clientData) 
    {
      memset(&fdSet_, 0, sizeof(fd_set));
      memset(xiid_, 0, FD_SETSIZE * sizeof(XtInputId));

      programName_ = new char[strlen(*argv) + 1];
      strcpy(programName_, *argv);

      // Determine which file descriptors are in use by glish and call
      // FD_Change() for each.

      AddInputMask(&fdSet_);               // Client::
      initFdSet();
    }

  // Is a given file descriptor being monitored for Glish activity?
  int fdActive(int fd) const { return FD_ISSET(fd, &fdSet_); }

  const char * programName() const { return (const char *) programName_; }

 protected:

  // Called whenever there is a change in the glish file descriptors
  void FD_Change(int fd, int add_flag)
    {
      if (add_flag)
	{
	  // Store the XtInputId in the file descriptor table, below, and
          // use an fd_set to keep track of fd_set changes.
	  FD_SET(fd, &fdSet_);
	  xiid_[fd] = XtAppAddInput(app_, fd, (XtPointer) XtInputReadMask, 
				    inputCB, this); 
	}
      else
	{
          // Retrieve the XtInputID and use it to remove the XtAppInputCallback
	  FD_CLR(fd, &fdSet_);
	  XtRemoveInput(xiid_[fd]);
	}
    } 

 private:

  // Scans glish fd_set for active fd's
  void initFdSet();

  // This is the function that is sent to all the XtAppAddInputs, it
  // calls proc_ if *fd is recognized.
  static void inputCB(XtPointer client_data, int * fd, XtInputId *);

  //----------------------------------------------------------------------
  //
  //   The following are set upon application initialization.
  //   The application context needs to be initialized by X
  //   before calling the AvXGlishClient constructor..

  XtAppContext app_;
  void (* proc_)(AvXGlishClient *, GlishEvent *, int fd, XtPointer);
  XtPointer clientData_;
  char * programName_;

  fd_set    fdSet_;               // fd_set usage by glish on this client
  XtInputId xiid_[FD_SETSIZE];    // table of glish transport fd values

};

#endif
