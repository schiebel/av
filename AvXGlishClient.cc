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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvXGlishClient.cc,v 19.0 2003/07/16 05:47:09 aips2adm Exp $
//
// $Log: AvXGlishClient.cc,v $
// Revision 19.0  2003/07/16 05:47:09  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:52  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/25 23:04:41  hravlin
// Added (int) casts to getpid() to avoid compiler warning.
//
// Revision 17.0  2001/11/12 19:42:21  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:28  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:05  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:12  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  2000/03/09 19:43:37  hravlin
// Removed a couple of compiler warnings.
//
// Revision 13.0  1999/08/10 18:40:00  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:38  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:44  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:35  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:27:14  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:35  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 07:47:55  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.6  1996/06/18  18:46:48  pixton
//  Copyright Update
//
//  Revision 1.5  1995/10/03  19:03:29  hr
//  Adjustment needed to the previous ifdef.
//
//  Revision 1.4  1995/10/03  18:35:14  hr
//  Added copyright.
//
//  Revision 1.3  1995/10/03  14:09:26  hr
//  Added ifdef GLISH around code so the file can be compiled even when
//  glish isn't being used.
//
//  Revision 1.2  1995/10/02  14:54:05  pixton
//  stdlib added to provide exit() function prototype
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#ifdef GLISH

//
// Detailed usage info available in AvXGlishClient.h
//

#include <string.h>
#include <Glish/Client.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <AvXGlishClient.h>

void AvXGlishClient::initFdSet()
{
  for (int i = 0; i < FD_SETSIZE; i++)
    {
      if (FD_ISSET(i, &fdSet_)) FD_Change(i, glish_true);
    }
}

// static member function  XtInputCallbackProc
void AvXGlishClient::inputCB(XtPointer xgcptr, int * fd, XtInputId *)
{
  AvXGlishClient * xgclient = (AvXGlishClient *) xgcptr;

  if (!xgclient->fdActive(*fd))
    {
      printf("[pid %ld] AvXGlishClient::inputCB : received input on non-glish fd %d\n",
	     (long)getpid(), *fd);
      return;
    }

  GlishEvent * e = xgclient->NextEvent();

  if (!e)
    {
      printf("program '%s' [pid %ld] terminated.\n", 
	     xgclient->programName(), (long)getpid());
      exit(0);;
    }
  else
    {
      // Call user process
	xgclient->proc_(xgclient, e, *fd, xgclient->clientData_);
    }
}

#endif
