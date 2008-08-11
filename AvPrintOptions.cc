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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvPrintOptions.cc,v 19.0 2003/07/16 05:48:12 aips2adm Exp $
//
// $Log: AvPrintOptions.cc,v $
// Revision 19.0  2003/07/16 05:48:12  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:45  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:43:13  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:19  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:54  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:09:17  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.2  1999/09/24 21:25:16  hravlin
// Added support for user supplied filename and output file comments.
//
// Revision 13.1  1999/09/15 21:11:26  hravlin
// Initial checkin.
//
//
//---------------------------------------------------------------------------

/* AvPrintOptions.cc
Holds information describing how to draw profiles.
*/

#include <stdio.h>
#include <X11/Intrinsic.h>
#include <Xm/ToggleBG.h>
#include <AvString.h>
#include <AvPrintOptions.h>

// TCL Variable name for print info.
static const char *PRINTSTR = "print";


////////////////////////////////////////////////////////////////

// TCL script to initialize defaults.
// marker 0 is set to be a filled circle.
static const char *defaultScript = \
	"set aipsview(print,scale) 1.0\n\
	set aipsview(print,portrait) TRUE\n\
	set aipsview(print,eps) TRUE\n\
	set aipsview(print,flipX) FALSE\n\
	set aipsview(print,flipY) FALSE\n\
	set aipsview(print,smoothImage) FALSE\n\
	set aipsview(print,paperSize) { LETTER}\n\
	";

////////////////////////////////////////////////////////////////
////		IntMaps for converting between strings and ints.
////////////////////////////////////////////////////////////////

// This is created during initialization.
static AvPrintOptions::IntMap *PageTypes = NULL;

static AvPrintOptions::IntMap Layout[] = {
	{AvPSDriver::PORTRAIT, "Portrait"},
	{AvPSDriver::LANDSCAPE, "Landscape"},
	{AvPSDriver::EPS_PORTRAIT, "EPS Portrait"},
	{AvPSDriver::EPS_LANDSCAPE, "EPS Landscape"},
	{ 0, NULL}
};

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

// ( Initializers with "TRUE" are integerNotifiers configured as Boolean).
AvPrintOptions::AvPrintOptions(const char *fileName, const char *tclname)
{
	init(fileName, tclname);
}

void AvPrintOptions::init(const char *filename, const char *tclname)
{
	if(tclname == NULL)
		tclname = PRINTSTR;
	cnfg_ = new AvConfigInfo(filename, tclname, defaultScript);
	cnfg_->ref();

	// Value will be filled in when window is popped up.
	filename_.init("filename", cnfg_, NULL, "");
	defaultFilename_.init("defaultFilename", NULL, NULL, "");
	scale_.init("scale", cnfg_, this, 1.0);
	aspectRatio_.init("aspectRatio", cnfg_, this, 1.0);
	layout_.init("layout", cnfg_, this, Layout, 0, AvPSDriver::PORTRAIT);
	flipX_.init("flipX",  cnfg_, this, FALSE);
	flipY_.init("flipY",  cnfg_, this, FALSE);
	smoothImage_.init("smoothImage",  cnfg_, this, FALSE);
	comment_.init("comment", cnfg_, this, "");

	// Build an IntMap for paper sizes. ONCE.
	if(PageTypes == NULL)
	{	int numpagetypes = AvPSDriver::numPageTypes();
		PageTypes = new IntMap[numpagetypes + 1];

		for(int i=0; i< numpagetypes; i++)
		{//char *name, vstring[32];

			const AvPSDriver::PageInfo *pi =
						AvPSDriver::getPageInfo(i);
			PageTypes[i].name = pi->alias;
			PageTypes[i].value = i;
		}
		// Null terminate the table.
		PageTypes[numpagetypes].name = NULL;
		PageTypes[numpagetypes].value = 0;
	}
	paperSize_.init("paperSize", cnfg_, this, PageTypes, 0,
			 AvPSDriver::LETTER);
}

AvPrintOptions::~AvPrintOptions()
{
	AvResource::UnRef(cnfg_);
}

void AvPrintOptions::touch()
{
	if(notifyEnabled())
	{
		invokeCallbacks(ValueChanged);
	}
}
