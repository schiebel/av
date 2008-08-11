//# Copyright (C) 1995-97 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvGraphicContext.h,v 19.0 2003/07/16 05:47:15 aips2adm Exp $
//
// $Log: AvGraphicContext.h,v $
// Revision 19.0  2003/07/16 05:47:15  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:56  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:26  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:33  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:24  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:19  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:06  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:49  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:00  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:53  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1997/09/15 16:22:25  hr
// Conversion of line width from an enum to an int.
//
// Revision 9.0  1997/08/25 21:27:44  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:56  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:26:55  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.3  1996/06/18  18:42:54  pixton
//  Copyright Update
//
//  Revision 1.2  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.1  1995/06/22  18:53:45  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------


/* AvGraphicContext.h

Class describing graphic context:
	line width, type;
	foreground, background colors.
	text font.
	etc.

This class is used as an interface between calling routines and the
Canvas's mechanism for changing attributes. Not all drawables will
handle all attributes.

Many of the parameters are limited to predefined types.

A Graphic context should be assumed to be 'bound' to a single canvas.
*/

#ifndef AvGraphicContext_h
#define AvGraphicContext_h

#include "AvResource.h"

class AvGraphicContext :public AvResource {
  public:
	AvGraphicContext();
	AvGraphicContext(const AvGraphicContext&);
	virtual AvGraphicContext& operator=(const AvGraphicContext &);

	enum DrawMode { COPY, XOR, INVERT};
	DrawMode getDrawMode()const {return drawmode_;}
	virtual void setDrawMode(const DrawMode d);

#if 0
	enum LineWidth { DEFAULTLINEWIDTH, THIN, MEDIUM, WIDE};
	LineWidth getLineWidth()const { return linewidth_;}
	virtual void setLineWidth(const AvGraphicContext::LineWidth lw);
#else
	// A width of 0 means to draw a single width line quickly.
	enum {DEFAULTLINEWIDTH = 0};
	int getLineWidth()const { return linewidth_;}
	// Line width = 0 => draw a single width line as quickly as possible.
	virtual void setLineWidth(const int lw);
#endif

	enum LineStyle { SOLID, DASHED, DOTTED};
	LineStyle getLineStyle()const { return linestyle_;}
	virtual void setLineStyle(const LineStyle ls);

	enum Font {DEFAULTFONT, FONT0, FONT1, FONT2, FONT3,
		SMALL=FONT0, MEDIUMFONT=FONT1, LARGE=FONT2, VERYLARGE=FONT3
		};
	Font getFont()const { return font_;}
	virtual void setFont(const Font f);

	// Colors are 'virtual'. They will be converted to physical color
	// values by the canvas' palette.
	virtual void setForeground(const unsigned long);
	unsigned long getForeground()const {return foreground_;}

	virtual void setBackground(const unsigned long);
	unsigned long getBackground()const {return background_;}
	enum Mask {	DRAWMODE=1, FOREGROUND=2, BACKGROUND=4,
			LINEWIDTH=0x8, LINESTYLE=0x10, FONT=0x20,

			ALLMASK = DRAWMODE| FOREGROUND| BACKGROUND|
			LINEWIDTH| LINESTYLE| FONT
		};
	Boolean modified()const {return modified_;}
  protected:
	void setModified(const Boolean modified = TRUE){modified_ = modified;}
	virtual void copy(const AvGraphicContext &);
	~AvGraphicContext();
  private:
	// Ensure settings are really set.
	virtual void update(const int mask=0);
  protected:
	DrawMode	drawmode_;
	AvGraphicContext::Font font_;
	int		linewidth_;
	LineStyle	linestyle_;
	unsigned long	foreground_;
	unsigned long	background_;
	Boolean		modified_;
};
#endif
