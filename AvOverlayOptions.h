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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvOverlayOptions.h,v 19.0 2003/07/16 05:48:09 aips2adm Exp $
//
// $Log: AvOverlayOptions.h,v $
// Revision 19.0  2003/07/16 05:48:09  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:43  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:43:11  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:17  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:51  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:09:15  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:41:07  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:36  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:02:33  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:56:52  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/02/20 22:02:42  hr
// Overlay option variables.
//
//
//---------------------------------------------------------------------------
/* AvOverlayOptions.h
Holds information describing how to draw overlays.
Overlay commands are read in from a file and drawn on top of other graphics.
*/

#ifndef AvOverlayOptions_h
#define AvOverlayOptions_h

#include <AvResource.h>
#include <AvConductor.h>
#include <AvNotifier.h>
#include <AvConfigInfo.h>

class AvAccessor;

class AvOverlayOptions : public AvResource, public AvConductor {
  public:
	AvOverlayOptions(const char *filename=NULL,
			 const char *configName=NULL,
			 AvAccessor *accessor=NULL);

	enum {Destroyed = AvConductor::Destroyed,
			ValueChanged=AvConductor::ValueChanged};
	enum COLORS{ BLACK, WHITE, RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW};
	enum COLORCHOICE {DEFAULTCOLOR, OVERRIDESET, OVERRIDEALL};

	unsigned long timestamp()const {return timestamp_;}
	Boolean modifiedSince(const unsigned long ts)const
		{ return (timestamp_ > ts) ? TRUE : FALSE;}
	void	touch();		// Mark as modified & notify.
	AvConfigInfo	*config()const{return cnfg_;}
	AvAccessor *accessor()const{return accessor_;}
  protected:
	virtual ~AvOverlayOptions();	// Use ref/unref.
  private:
	void init(const char *fileName, const char *configName, AvAccessor *);
  private:
	AvConfigInfo	*cnfg_;
  public:
	////////////////////////////////////////////////
	// AvOverlayConfig and AvOverlay directly access these.
  public:
	AvStringNotifier	filename_; // Where to read data from.
	AvBooleanNotifier	show_;		// Show/noshow overlay.
	AvIntMapNotifier	colorChoice_;
	AvIntMapNotifier	color_;
	AvDoubleNotifier	scale_; // Scale used for vectors.
  private:
	
	unsigned long timestamp_;
	AvAccessor		*accessor_;
};
#endif
