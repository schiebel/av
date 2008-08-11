//# Copyright (C) 1995-99 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvFallbacks.h,v 19.0 2003/07/16 05:47:05 aips2adm Exp $
//
// $Log: AvFallbacks.h,v $
// Revision 19.0  2003/07/16 05:47:05  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:49  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:18  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:25  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:50  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:09  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.2  1999/10/25 18:45:51  hravlin
// Added thread related variables.
//
// Revision 13.1  1999/09/15 21:37:13  hravlin
// Added visualID & nCMapColors variables to resource/switch struct.
//
// Revision 13.0  1999/08/10 18:39:56  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:30  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:33  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:22  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/05/01 21:32:08  hr
// Added minColors, numFreeColors and colorVerbosity.
//
// Revision 9.0  1997/08/25 21:26:54  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:22  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/01/09 18:33:16  hr
// Moved fallbacks back to aipsview.cc. Added definition of the xOptions
// structure and getXOptions() function.
//
// Revision 7.2  1996/12/12 06:16:53  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.3  1996/06/18  18:42:50  pixton
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

/* AvFallbacks.h
This file used to define fallback resources for the application.
However, they have been moved back into aipsview.cc. This file now
defines the structure used to hold values that can be set via command
line options. To add more, tables in aipsview.cc will also need to be
changed.
*/
#include <X11/Intrinsic.h>


//////////////////////////////////////////////////////////////////////
//	Support for command line arguments.
// The parameters in the structure correspond to entries in tables defined
// in aipsview.cc.
//////////////////////////////////////////////////////////////////////

typedef struct {
	// Used in AvImageWindow
	int	displayWidth;	// Initial size of the image window.
	int	displayHeight;
	int	displaySize;	// If set, sets width = height.
	// XPalette
	int	minColors;	// Min # of RW colors.
	char	*numColors;	// # of RW colors. (Not always digits).
	int	numFreeColors;	// # colors available for widgets.
	int	colorVerbosity;	// How much to print in XPalette.
	// PGCanvas.
	char	*pgDevice;	// PGPLOT output device.
	// AvImageView.
	int	visualID;	// Which visual to use.
	int	nCMapColors;	// Want a colormap at least this long.
	// For experimental thread support in AvXCanvas.
	int	nThreads;	// # of threads to use.
	int	kbPerThread;	// minimum # of kb to give to a thread.
	int	showTimings;	// Display timings for scaling. (Debug switch).
	} xOptions, *xOptionsPtr;

// Returns a pointer to the the above structure.
// Subclasses of AvUserComp may wish to use get{Integer,String}Resource
// instead.
const xOptions *getXOptions();
