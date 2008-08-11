//# Copyright (C) 1995-99 Board of Trustees of the University of Illinois
//#
//# This software, both binary and source, is copyrighted by The
//# Board of Trustees of the University of Illinois. Ownership
//# remains with the University. You should have received a copy 
//# of a licensing agreement with this software. See the file 
//# "AIPSVIEW_COPYRIGHT", or contact the University at this address:
//#
//# The NCSA AipsView Visualization System
//# National Center for Supercomputing Applications
//# University of Illinois
//# 405 North Mathews Ave.
//# Urbana, IL 61801
//# aipsview@ncsa.uiuc.edu
//# --------------------------------------------------
//
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvPrintOptions.h,v 19.0 2003/07/16 05:48:11 aips2adm Exp $
//
// $Log: AvPrintOptions.h,v $
// Revision 19.0  2003/07/16 05:48:11  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:44  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:43:12  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:19  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:53  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:09:17  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.2  1999/09/24 21:25:37  hravlin
// Added support for user supplied filename and output file comments.
//
// Revision 13.1  1999/09/15 21:10:21  hravlin
// *** empty log message ***
//
//
//---------------------------------------------------------------------------

/* AvPrintOptions.h
Holds information describing how to print a file.
*/

#ifndef AvPrintOptions_h
#define AvPrintOptions_h

#include <iostream.h>
#include <AvResource.h>
#include <AvConductor.h>
#include <AvConfigInfo.h>
#include <AvNotifier.h>
#include <AvPSDriver.h>

class AvPrintOptions : public AvResource, public AvConductor {
  public:
	typedef AvConfigInfo::IntMap IntMap;

	typedef AvPSDriver::MediaSize MediaSize; // ( LETTER, LEGAL, etc).

	AvPrintOptions(const char *filename=NULL,
			 const char *configName=NULL);

	enum FLIPAXES { NOFLIP=0, FLIPX=1, FLIPY=2, FLIPXY=3};
	enum {Destroyed = AvConductor::Destroyed,
			ValueChanged=AvConductor::ValueChanged};

	void	touch();		// Mark as modified & notify.

	AvConfigInfo	*config()const{return cnfg_;}
	Boolean isEPS()const
		{return (layout_.getValue() == (int)AvPSDriver::EPS_PORTRAIT) ||
			(layout_.getValue() == (int)AvPSDriver::EPS_LANDSCAPE);
		}

	//////////////////////////////////////////////

	AvStringNotifier	filename_;	// Output filename.
	AvStringNotifier	defaultFilename_;// Default output filename.
	AvDoubleNotifier	scale_;
	AvDoubleNotifier	aspectRatio_;
	AvIntMapNotifier	layout_;	// Portrait or landscape.
						// + EPS or PS.
	AvIntMapNotifier	paperSize_;	//  All known paper sizes.
	AvBooleanNotifier	flipX_;
	AvBooleanNotifier	flipY_;
	AvBooleanNotifier	smoothImage_;
	AvStringNotifier	comment_;
  protected:
	virtual ~AvPrintOptions();	// Use ref/unref.
  private:
	void init(const char *fileName, const char *configName);// Initialize.
  private:
	AvConfigInfo	*cnfg_;
};
#endif
