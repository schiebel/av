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
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvVersion.cc,v 19.1 2003/10/07 20:46:00 hravlin Exp $
//
// $Log: AvVersion.cc,v $
// Revision 19.1  2003/10/07 20:46:00  hravlin
// If DATE macro isn't defined, use cpp's date. If OPTIONS macro isn't
// defined, try to build one.
//
// Revision 19.0  2003/07/16 05:46:51  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:36  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/03 22:24:35  hravlin
// Updated copyright date.
//
// Revision 17.0  2001/11/12 19:42:05  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:14  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:02  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:25:25  hravlin
// Changed functions to return const char *.
//
// Revision 14.0  2000/03/23 16:07:53  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.2  1999/09/15 21:06:25  hravlin
// Updated for version 2.2
//
// Revision 13.1  1999/08/25 19:36:42  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:39:37  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:59  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.1  1999/05/21 20:27:52  hravlin
// Updated copyright dates. Added RCS Header & Log.
//
//
//---------------------------------------------------------------------------
/* version.cc
Contains release level, date and memo info.

Since this is likely to get recompiled every time, it is meant to be small
and compile quickly.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>		// uname
#include <AvVersion.h>

// This probably shouldn't be set outside since the memo may use the 
// release level itself. Changing this won't change references there.
#ifndef RELEASE_LEVEL
#define RELEASE_LEVEL	""
#endif

//	Major/minor release numbers will probably need to be edited rather
// than passed in from the Makefile.
// Major release number
#ifndef MAJOR
#define MAJOR	"2"
#endif

// Minor release number
#ifndef MINOR
#define	MINOR	"2"
#endif

#ifndef EDIT
#define	EDIT	"6"
#endif

// Compilation/release date. Normally set in Makefile.
#ifndef DATE
#if defined(__DATE__)
#define DATE	__DATE__
#else
#define DATE	"10/2003"
#endif
#endif

static const char *release_ = RELEASE_LEVEL;
static const char *major_ = MAJOR;
static const char *minor_ = MINOR;
static const char *edit_  = EDIT;
static const char *date_ = DATE;

// Compilation options. (-DTCL, etc).
// If OPTIONS isn't defined, try to generate a list.
#ifndef OPTIONS
static const char *options_ = {
	"FITS"

#if defined(AIPSPLUSPLUS)
	" AIPS++"
#endif

#if defined(MIRIAD)
	" MIRIAD"
#endif

#if defined(OGL)
	" OGL"
#endif

// This currently has to be defined for aipsview to compile.
#if defined(PGPLOT)
	" PGPLOT"
#endif

#if defined(PTHREADS)
	" PTHREADS"
#endif

#if defined(GLISH)
	" GLISH"
#endif

#if defined(TCL)
	" TCL"
#endif
	};
#else
// Built when options() is called.
static char *options_ = NULL;
#endif

// Startup memo. memo() also includes version and option information.
static const char memo_[] = 

"Copyright (C) 1996-2003 Board of Trustees of the University of Illinois.\n\
Aipsview is a visualization tool created by the \
Radio Synthesis Imaging group at the National Center for \
Supercomputing Applications (NCSA) as part of the international \
Astronomical Information Processing System++ (AIPS++) project. \
Please send questions \
and comments via email to: aipsview@astro.uiuc.edu.";

// Compile/release date.
const char *AvVersion::date()
{
	return date_;
}

// Version information.
const char *AvVersion::version()
{
static char buf[80];

	sprintf(buf, "%s.%s.%s %s",major_, minor_, edit_, release_);
	return buf;
}

// Major/minor version numbers.
int AvVersion::majorNum()
{
	return atoi(major_);
}

int AvVersion::minorNum()
{
	return atoi(minor_);
}

// Compile options.
const char *AvVersion::options()
{
#if defined OPTIONS
	if(options_ == NULL)
	{// Build list from the externally defined OPTIONS constant.
	// Note: FITS is always available.
	 int fitslen = (int)strlen("FITS ");
	 int len = (int)strlen(OPTIONS) + fitslen;
	 int c;
		options_ = new char[len + 1];
	 const char *pin=OPTIONS;
	 char *pout = options_ + fitslen;

		strcpy(options_, "FITS ");
		// Copy OPTIONS string removing any '-D's.
		// Will also remove any embedded '-D',
		// but there shouldn't be any.
		while( (c = *pin++) != '\0')
			if((c == '-') && (*pin == 'D'))
				pin += 1;
			else
				*pout++ = c;

		*pout = '\0';
	}
#endif

	return options_;
}


// The memo consists of the startup message, version, compile date and
// compile options.
const char *AvVersion::memo()
{  static char *buf = 0;

	if(buf == 0)
	{ const char *optns = options();
	  int optnslen = strlen(optns);
	  int nspace = optnslen + sizeof(memo_) + sizeof(DATE) + 128;
	   // Room for the memo + other information.
		buf = new char[nspace+1];

		sprintf(buf,
		"\t aipsview %s (%s)\n%s\n Options: %s\n",
			version(), date(), memo_, options());
#if !defined(COMPILEINFO) || defined(USE_UNAME)
		struct utsname info;
		int	len = strlen(buf);
		uname(&info);
		sprintf(buf+len, "Compiled on: %s %s %s\n",
		info.sysname, info.release,info.machine);
#else
		int	len = (int)strlen(buf);
		sprintf(buf+len, "Compiled on: %s\n", COMPILEINFO);
#endif

		// There is plenty of room, but if the format changes,
		// this may help find a problem.
		int space = (int)(nspace-strlen(buf));
		if(space <= 0)
			fprintf(stderr,
		   "AvVersion::memo: BUFFER overflow by %d bytes.\n", -space);
	}
	return buf;
}
