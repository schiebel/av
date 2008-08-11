//# Copyright (C) 1995-98 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvColorSetter.cc,v 19.0 2003/07/16 05:46:44 aips2adm Exp $
//
// $Log: AvColorSetter.cc,v $
// Revision 19.0  2003/07/16 05:46:44  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:30  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:41:59  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:08  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:39  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:07:45  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:29  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:44  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:28  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:05  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/09 21:20:12  hr
// Removed unneeded semicolon after DEFINE_MEMBER_INFO macro.
//
// Revision 9.0  1997/08/25 21:24:53  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:11  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 05:45:10  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.3  1996/06/18  18:45:52  pixton
//  Copyright Update
//
//  Revision 1.2  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.1  1995/03/29  20:51:00  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvColorSetter.cc
Used to change color used to draw vectors & text.
*/
#include <AvColorSetter.h>
#include <AvCanvas.h>

DEFINE_MEMBER_INFO(AvColorSetter)

AvColorSetter::AvColorSetter(const unsigned long color)
{
	color_ = color;
	haveName_ = FALSE;
	colorname_ = NULL;
	INIT_CLASS(AvGraphics2D);
}

AvColorSetter::AvColorSetter(const char *colorname)
{
	colorname_ = new char[strlen(colorname)+1];
	strcpy(colorname_, colorname);
	haveName_ = TRUE;
	INIT_CLASS(AvGraphics2D);
}

AvColorSetter::~AvColorSetter()
{
	if(haveName_)
		delete [] colorname_;
}

void AvColorSetter::draw(AvCanvas &c)
{
	if(haveName_)
		c.setDrawColor(colorname_);
	else
		c.setDrawColor(color_);
}

void AvColorSetter::draw(AvCanvas *c)
{
	if(c != NULL)
		draw(*c);
}
