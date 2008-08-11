//# Copyright (C) 1995-2003 Board of Trustees of the University of Illinois
//#
//# This software, both binary and source, is copyrighted by The
//# Board of Trustees of the University of Illinois. Ownership
//# remains with the University. You should have received a copy 
//# of a licensing agreement with this software. See the file 
//# "AIPSVIEW_COPYRIGHT", or contact the University at this address:
//#
//#     The NCSA AipsView Visualization System
//#     National Center for Supercomputing Applications
//#     University of Illinois
//#     405 North Mathews Ave.
//#     Urbana, IL 61801
//#     aipsview@ncsa.uiuc.edu
//# --------------------------------------------------
//
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvMakeDataSet.h,v 19.1 2003/09/02 21:16:27 hravlin Exp $
//
// $Log: AvMakeDataSet.h,v $
// Revision 19.1  2003/09/02 21:16:27  hravlin
// Changed to being a class to support AvDataSet factory methods.
//
// Revision 19.0  2003/07/16 05:47:53  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:29  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:57  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:03  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.1  2000/12/17 21:48:48  wbrouw
// Removed '#include <aips_exit.h>' and/or aips_enter.h
//
// Revision 15.0  2000/10/26 17:12:24  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:59  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:48  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:04  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:47  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:58  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:30:56  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:18:58  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.4  1996/12/12 09:33:22  droberts
// Modified to manage AIPS++ and X namespaces.  Keine anhung
//
// Revision 1.2  1996/09/27 17:00:02  hr
// No change.
//
// Revision 1.1  1996/09/27 16:34:54  hr
// Initial revision
//
//
//---------------------------------------------------------------------------
#if !defined(_AvMakeDataSet_h_)
#define _AvMakeDataSet_h_
/*
	This is the interface file between the various data set readers and
the rest of aipsview. All member functions are static. It's really only a
class to enclose the function names.

 makeDataSet() returns a pointer to an attached data set or NULL if none
 could be attached to.

filename	- Name of file to attach to.
tries		- List of file types that makeDataSet could not attach to.

 isDataSet returns true if file is a regular file or a special (ie Miriad or
 AIPS++ data set) directory. Otherwise false.
 NOTE:
   true does not mean that the file name refers to a valid data set,
but that it COULD be a data set. (false means no way, true means maybe). It's
purpose is to help the file requester decide whether a directory should be
included in the files list.

*/

#include "AvDataSet.h"

class AvMakeDataSet {
 public:
  /* Return a pointer to an appropriate data set reader for the file or
     none if none was found.
  */
  static AvDataSet *makeDataSet(const char *filename);

  /* Returns true if the filename should be considered as a potential data set
     for some reader. false otherwise.
  */
  static bool isDataSet(const char *filename);

  // Returns a string containing an error message from the last call to
  // makeDataSet.
  static const AvString &errorString();

  // There should never be a reason to create one.
  AvMakeDataSet();
  ~AvMakeDataSet();
};

#endif
