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
/* version.h
Returns compilation information.

Release info:
	eg. alpha 4.0[.3]
	The edit level is not reported if AvVersion is compiled with
	RELEASE defined.

Compilation date.
	eg. 9/14/95
Compilation options:
	eg. FITS HDF GLISH
	FITS is always listed, optimization/debugging info is not reported.

The only reason this was made a class was to keep the function names from
clashing with anything. All functions are static so there is no point in
creating an instance of AvVersion.

Much of the information is feed to AvVersion by the Makefile.
*/

#ifndef AvVersion_h
#define AvVersion_h

class AvVersion {
  public:
	// All functions returning char * return static buffers. Don't
	// delete the returned buffer.
	static const char *date();		// Compilation/relase date.
	static const char *version();	// ex. "alpha 4.0", "alpha 4.0.3.
	static int minorNum();	// ex. 0.
	static int majorNum();
	// Returns a pointer to a char string listing the compiled options.
	static const char *options();
	// Returns a pointer to the 'logon' message including the above and
	// the disclaimers, etc.
	static const char *memo();
};

#endif
