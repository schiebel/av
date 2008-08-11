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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvMakeDataSet.cc,v 19.1 2003/09/02 21:15:46 hravlin Exp $
//
// $Log: AvMakeDataSet.cc,v $
// Revision 19.1  2003/09/02 21:15:46  hravlin
// Rewrote as class rather than a subroutine to support AvDataSet factory
// methods.
//
// Revision 19.0  2003/07/16 05:47:53  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:28  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:57  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:03  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:23  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:58  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:48  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:03  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:46  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:56  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/03/16 18:29:07  hr
// In makeDataSet(), put AIPS++ reader first so we don't get FITS warning
// messages when reading AIPS++ files.
//
// Revision 9.0  1997/08/25 21:30:54  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:18:57  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.5  1996/12/28 00:28:06  bglenden
// Avoid calling a member function on a deleted pointer.
//
// Revision 7.4  1996/12/12 08:57:24  droberts
// Modified to manage AIPS++ and X namespaces.  Removed extra messages.
//
// Revision 1.1  1996/09/27 16:34:54  hr
// Initial revision
//
//
//---------------------------------------------------------------------------

/* This is the interface file between the various data set readers and
the rest of aipsview. makeDataSet() returns a pointer to an attached
data set or NULL if none could be attached to.
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "AvMakeDataSet.h"
//#include "AvDataSet.h"

/////////////////////////////////////////////////////////////////////////
// For now, FITS is always defined in aipsview. Others should be defined
// in the Makefile.
#define FITS
#ifdef FITS
#include "AvFitsDataSet.h"
#endif

#ifdef MIRIAD
#include "AvMIRDataSet.h"
#endif

#ifdef HDF
#include "AvHDFDataSet.h"
#endif

// Whatever the defined is called will need to appear in the makefile and
// will also appear in the list of Options in the announcement window.
#ifdef AIPSPLUSPLUS
#include "AvAipsDataSet.h"
#endif
/////////////////////////////////////////////////////////////////////////

/* Master data set information table. Each supported data set has to have
   an entry here.
*/
static AvDataSetFactoryInfoPtr *factoryPointers_[] = {
  &AvFitsDataSet::getFactoryInfo,

#if defined(AIPSPLUSPLUS)
  &AvAipsDataSet::getFactoryInfo,
#endif

#if defined(MIRIAD)
  &AvMIRDataSet::getFactoryInfo,
#endif

  0};
// The trailing 0 is here so all entries can be alike (ie end with a ",").

static const int NumFactories_ =
	sizeof(factoryPointers_)/sizeof(*factoryPointers_)-1;

static bool initialized_ = false;
static const AvDataSetFactoryInfo *factories_[NumFactories_];
static AvString errorString_ = "";

  // Returns a string containing an error message from the last call to
  // makeDataSet.
const AvString &AvMakeDataSet::errorString() { return errorString_;}

static AvMakeDataSet dummy_;

AvMakeDataSet::AvMakeDataSet()
{
	if(initialized_)
		return;
	for(int i=0; i< NumFactories_; i++)
	{	if(factoryPointers_[i] != 0)
			factories_[i] = factoryPointers_[i]();
		else
			factories_[i] = 0;
	}
	initialized_ = true;
}

AvMakeDataSet::~AvMakeDataSet()
{
}

// Try to open a data set with filename "fname". Scans known file types
// and returns a pointer to an attached data set if successful. Otherwise,
// returns NULL.
AvDataSet *AvMakeDataSet::makeDataSet(const char *fname)
{ AvDataSet *ds = NULL;
  
	errorString_ = "";	// Reset.

	if(fname == NULL)
	{	errorString_ = "NULL file name";
		return NULL;
	}

	for(int i=0; i < NumFactories_; i++)
	{const AvDataSetFactoryInfo *factory = factories_[i];

		if(factory == 0)
			continue;
#if 0
		// Don't really want this. Let the attach fail so the
		// error string gets updated.
		if(!factory->isDataSet(fname))
			continue;
#endif
		ds = factory->makeDataSet();
		if(ds == 0)
			continue;	// Shouldn't happen.
	
		if(ds->attach(fname))
		{	errorString_ = "Opened ";
			errorString_ += fname;
			errorString_ += " as ";
			errorString_ += factory->dataSetTypeName();
			break;
		}
		else	// Failed to open.
		{	if(errorString_.length() == 0)
			{	errorString_ = "Failed to open ";
				errorString_ += fname;
				errorString_ += " as: ";
			}
			else
				errorString_ += ", ";

			errorString_ += factory->dataSetTypeName();
			ds->unref();
			ds = 0;
		}
	}

	if(ds == 0)
		cerr << errorString_ << endl;

//  cout << "Don't know what to do with file '" << fname << "'.\n";
  
  // If we get here none of the readers could handle the file.
  return ds;
}

bool AvMakeDataSet::isDataSet(const char *filename)
{ bool isdataset = false;

	for(int i=0; i < NumFactories_; i++)
	{const AvDataSetFactoryInfo *factory = factories_[i];

		if(factory == 0)
			continue;
		if((isdataset = factory->isDataSet(filename)))
			break;
	}

	return isdataset;
}
