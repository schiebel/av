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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/Av.h,v 19.1 2003/10/01 21:33:13 hravlin Exp $
//
// $Log: Av.h,v $
// Revision 19.1  2003/10/01 21:33:13  hravlin
//  Moved DEFINITION of BLANKEDVALUE into Av.cc.
//
// Revision 19.0  2003/07/16 05:46:24  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:12  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.8  2002/01/25 23:02:28  hravlin
// Removed ifdef around the aips++ish typedef since <aips/aips++.h> wouldn't
// compile.
//
// Revision 17.7  2002/01/22 21:34:39  hravlin
// Added PtrToInt().
//
// Revision 17.6  2002/01/17 15:48:58  hravlin
// *** empty log message ***
//
// Revision 17.5  2002/01/16 22:52:55  hravlin
// Changed unsigned int to size_t for 64 bit compilers.
//
// Revision 17.4  2002/01/14 16:44:00  hravlin
// Added the Av class to hold misc. small functions & constants.
//
// Revision 17.3  2002/01/09 18:16:35  hravlin
// Replaced inclusion of Xm/Xm.h with X11/Intrinsic.h (To get XtPointer).
// Included limits.h & float.h for FLT_MAX.
// Changed MAXVALUE to FLOATMAX.
// Minor change to MAX/MIN macros.
//
// Revision 17.2  2002/01/07 22:28:22  hravlin
// Removed typedef for lDouble.
//
// Revision 17.1  2002/01/03 19:41:09  hravlin
// Added typedefs (eg uChar) for non AIPS++ builds.
//
// Revision 17.0  2001/11/12 19:41:40  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:41:51  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:07:26  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:07:24  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:03  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:02  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:57:27  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:50:52  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:22:58  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:14:06  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 05:27:04  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.4  1996/06/18  18:42:35  pixton
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
//  Revision 1.2  1995/07/20  17:17:15  hr
//  Defined MAXVALUE and BLANKEDVALUE constants.
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* Av.h
Av declares some constants  and some miscellaneous
routines used in various places.
*/

#ifndef _SV_H
#define _SV_H

#include <sys/types.h>
#include <limits.h>
#include <float.h>			// FLT_MAX
#include <X11/Intrinsic.h>		// XtPointer

#ifndef NULL
#define NULL 0
#endif

#ifndef FALSE
#define FALSE 0
#define TRUE  1
#endif

#ifndef SV_ERROR
#define SV_ERROR -1
#define SV_OK     1
#endif

#ifndef MAX
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

/* FLOATMAX is the largest value that can be represented on the system. It
is used when computing min/max values and as the blanked data value if
HUGE_VAL isn't available. Therefore, data should not reach this high. If
FLT_MAX & MAXFLOAT don't exist make a guess.
*/
#if defined(FLT_MAX)
#define	FLOATMAX	FLT_MAX
#elif defined(MAXFLOAT)
#define	FLOATMAX	MAXFLOAT
#else
// Use a typical value.
#define	FLOATMAX	((float)3.40282346638528860e+38)
#endif

typedef void (*AvProc) (XtPointer d1, XtPointer d2);

typedef char String4[4];
typedef char String8[8];
typedef char String16[16];
typedef char String32[32];
typedef char String64[64];
typedef char String128[128];
typedef char String256[256];

typedef char Char;
typedef unsigned char uChar;
typedef short Short;
typedef unsigned short uShort;
typedef int Int;
typedef unsigned int uInt;
typedef long Long;
typedef unsigned long uLong;
typedef float Float;
typedef double Double;

class Av {
 public:
  // Beginning of a move away from global constants to those in the Av
  // namespace.
  static const float FloatMax;
  static const float BlankedValue;

  // Similar to regular strdup but returns NULL if passed a NULL ptr and
  // uses delete [] to delete.
  static char *strdup(const char *str);

  // Convert an XtPointer to an int. The two stage cast is to avoid
  // 64 bit compiler errors.
  static inline int PtrToInt(XtPointer p)
    {	return (int)(long)p;}
  static inline int PtrToInt(const void * p)
    {	return (int)(long)p;}

  // Used to convert a list of string literals to non const strings. Needed
  // since one shouldn't pass const strings as non const args.
  class litToStr {
   public:
	// Create an object with a non const copy of the list of strings.
	// If len is > 0, assume that is the number of strings in the list.
	// Otherwise, assume the list is NULL terminated. The output list
	// is always ended with a NULL pointer. (Even if len is 0).
	litToStr(const char *list[], size_t len=0);
	~litToStr();
	// Returns pointer to NULL terminated list of non const strings.
	char **list()const{return list_;}
	// # of strings in list.
	size_t length()const{return len_;}
	// # of strings in a NULL terminated list.
	static size_t findLength(const char **list);
   private:
	char	**list_;
	size_t	len_;
};

};

#endif
