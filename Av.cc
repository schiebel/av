//# Copyright (C) 1995-2003 Board of Trustees of the University of Illinois
//#
//# This software, both binary and source, is copyrighted by The
//# Board of Trustees of the University of Illinois. Ownership
//# remains with the University. You should have received a copy 
//# of a licensing agreement with this software. See the file 
//# "AIPSVIEW_COPYRIGHT", or contact the University at this address:
//#
//# The NCSA Aipsview Visualization System
//# National Center for Supercomputing Applications
//# University of Illinois
//# 405 North Mathews Ave.
//# Urbana, IL 61801
//# aipsview@ncsa.uiuc.edu
//# --------------------------------------------------
//
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/Av.cc,v 19.1 2003/10/01 21:32:46 hravlin Exp $
//
// $Log: Av.cc,v $
// Revision 19.1  2003/10/01 21:32:46  hravlin
// Moved DEFINITION of BLANKEDVALUE into Av.cc. Added check for INFINITY.
//
// Revision 19.0  2003/07/16 05:47:47  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:23  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.3  2002/01/16 22:52:38  hravlin
// Changed unsigned int to size_t for 64 bit compilers.
//
// Revision 17.2  2002/01/15 18:15:03  hravlin
// Was indexing list_ with wrong len.
//
// Revision 17.1  2002/01/14 16:44:56  hravlin
// Added strdup & litToStr.
//
//
//---------------------------------------------------------------------------
// Av.cc
// Implements a number of constants and generic routines for aipsview.

#include "Av.h"
#include <strings.h>
#include <math.h>

/* The internal number used to represent blanked data.
 Use INFINITY if it exists.
 Else use HUGE_VAL if it exists (maybe INFINITY).
 Else FLOATMAX.
*/
#ifndef BLANKEDVALUE
#if defined(INFINITY)
#define BLANKEDVALUE INFINITY
#elif defined(HUGE_VAL)
#define BLANKEDVALUE HUGE_VAL
#else
#define BLANKEDVALUE	FLOATMAX
#endif
#endif

const float Av::FloatMax = FLOATMAX;
const float Av::BlankedValue = BLANKEDVALUE;


// Return a pointer to a non const copy of a string.
char *Av::strdup(const char *str)
{ size_t n;
	if(str == 0)
		return 0;
	n = strlen(str) + 1;
	char *buf = new char[n];
	strcpy(buf, str);
	return buf;
}

// Make a non const copy of a list of strings.
Av::litToStr::litToStr(const char **list, size_t len): list_(0), len_(0)
{
	len_ = len;
	// If no length was passed in, count the # of entries.
	if(len_ == 0)
		len_ = findLength(list);
	// Check for empty list.
	if(len_ == 0)
	{	list_ = new char *[1];
	}
	else	// Copy const strings to non const versions.
	{	list_ = new char *[len_+1];
		
		if(list != 0)
		{	for(size_t i=0; i< len_; i++)
			{ const char *p = list[i];
				list_[i] = Av::strdup(p);
			}
		} else
			len_ = 0;
	}
	// NULL terminate list;
	list_[len_] = 0;
}

Av::litToStr::~litToStr()
{
	for(size_t i=0; i < len_; i++)
		delete [] list_[i];
	delete [] list_;
	list_ = 0;
	len_ = 0;
}

// Find the length of a NULL terminated list of strings.
size_t Av::litToStr::findLength(const char **list)
{ const char **l = list;

	if(list == 0)
		return 0;
	while(*l != 0) l++;
	return l-list;
}
