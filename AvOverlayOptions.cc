//# Copyright (C) 1995-98 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvOverlayOptions.cc,v 19.0 2003/07/16 05:48:09 aips2adm Exp $
//
// $Log: AvOverlayOptions.cc,v $
// Revision 19.0  2003/07/16 05:48:09  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:42  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:43:10  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:17  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:50  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:09:15  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:41:06  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:35  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:02:32  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:56:51  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/02/20 22:02:24  hr
// Overlay option variables.
//
//
//---------------------------------------------------------------------------
/* AvOverlayOptions.cc
Holds information describing how to draw overlays.
(Most information comes from the overlay command file).
*/

#include "AvOverlayOptions.h"
#include "AvAccessor.h"
#include "AvString.h"
#include "AvGUIParts.h"		// PGPLOTColors().
#include <math.h>		// sin/cos

// TCL Variable name for overlay info.
static const char *OVERLAYSTR = "overlay";


////////////////////////////////////////////////////////////////
static unsigned long _timestamp = 1;
static inline unsigned long TimeStamp()
{	return _timestamp++;}

// TCL script to initialize defaults.
// (wedge overlay is set to 0 markers because a: no one probably wants then.
//  and b: they can't be positioned via the mouse).
static const char *defaultScript = \
	"set aipsview(imageOverlay,drawOverlay) FALSE\n\
	";

////////////////////////////////////////////////////////////////
////		IntMaps for converting between strings and ints.
////////////////////////////////////////////////////////////////

// Color information for all the color toggles.
static const IntMap *Colors=NULL;
// How should color selector be used?
// As initial default color.
// As default color overriding file settings (except for explicit colors).
// Overridding all color choices from file.
static IntMap ColorChoice[] = {
	{AvOverlayOptions::DEFAULTCOLOR, "Default"},
	{AvOverlayOptions::OVERRIDESET, "Override color"},
	{AvOverlayOptions::OVERRIDEALL, "Only Color"},
	{ 0, NULL}
	};
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

AvOverlayOptions::AvOverlayOptions(const char *fileName, const char *tclname,
				   AvAccessor *acc)
{
	init(fileName, tclname, acc);
}

void AvOverlayOptions::init(const char *filename, const char *tclname,
			    AvAccessor *acc)
{
	accessor_ = acc;
	if(Colors == NULL)
		Colors = PGPLOTColors();
	if(tclname == NULL)
		tclname = OVERLAYSTR;
	cnfg_ = new AvConfigInfo(filename, tclname, defaultScript);
	cnfg_->ref();

	AvString fn;
	if(acc == NULL)
		fn = "overlay.dat";
	else
	{	fn = acc->dataset()->path();
		fn += ".overlay";
	}
	filename_.init("filename", cnfg_, this, fn);
	color_.init("overlayColor", cnfg_, this, Colors, 0, WHITE);
	colorChoice_.init("overlayColorChoice", cnfg_, this, ColorChoice, 0,
			  DEFAULTCOLOR);
	scale_.init("overlayScale", cnfg_, this, 1.0);
	timestamp_ = TimeStamp();
}

AvOverlayOptions::~AvOverlayOptions()
{
	AvResource::UnRef(cnfg_);
}

void AvOverlayOptions::touch()
{
	timestamp_ = TimeStamp();

	if(notifyEnabled())
	{
		invokeCallbacks(ValueChanged);
	}
}
