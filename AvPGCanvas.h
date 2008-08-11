//# Copyright (C) 1995-99 Board of Trustees of the University of Illinois
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
//---------------------------------------------------------------------------
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvPGCanvas.h,v 19.0 2003/07/16 05:47:19 aips2adm Exp $
//
// $Log: AvPGCanvas.h,v $
// Revision 19.0  2003/07/16 05:47:19  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:01  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:30  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:37  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:41  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:23  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/09/15 21:28:52  hravlin
// Removed unneeded include.
//
// Revision 13.0  1999/08/10 18:40:11  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:00  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:14  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:10  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1997/09/15 16:25:21  hr
// Conversion of line width from an enum to an int.
//
// Revision 9.0  1997/08/25 21:28:10  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:17:12  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/01/24 20:07:58  hr
// Added showID_ and added cpgend() calls.
//
// Revision 7.2  1996/12/12 08:42:05  droberts
// Final update from monet archive.
//
// Revision 1.2  1996/11/04 20:11:39  hr
// getPGfilename() & getPGdevicename() changed to return AvStrings.
// getPGfilename() & makePGname() replace any spaces in file names with '_'.
// Added cpgbeg() to handle const char * filenames.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.5  1996/06/18  18:43:08  pixton
//  Copyright Update
//
//  Revision 1.4  1996/03/20  16:25:13  hr
//  Added getPGfilename(), getPGdevicename() and makePGname().
//
//  Revision 1.3  1996/03/02  21:55:54  hr
//  Added scale_ and {x,y}off_.
//
//  Revision 1.2  1996/02/29  21:36:18  hr
//  Removed some commented out lines.
//
//  Revision 1.1  1996/02/29  17:33:46  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvPGCanvas.h
Defines 'Canvas' for X.
*/

#ifndef AvPGCanvas_h
#define AvPGCanvas_h


#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <AvGraphics2D.h>
#include <AvCanvas.h>
#include <AvGraphicContext.h>

class AvPGCanvas;
class AvPGPalette;
class AvString;

class AvPGGraphicContext : public AvGraphicContext {
  public:
	AvPGGraphicContext(AvPGCanvas *can);
	AvPGGraphicContext(const AvGraphicContext&);
	virtual AvGraphicContext& operator=(const AvGraphicContext &);
	virtual AvPGGraphicContext& operator=(const AvPGGraphicContext &);
	~AvPGGraphicContext();

//	virtual void setLineWidth(const AvPGGraphicContext::LineWidth lw);
	virtual void setLineWidth(const int lw);
	virtual void setLineStyle(const LineStyle ls);
	virtual void setForeground(const unsigned long);
//	virtual void setBackground(const unsigned long);
  private:
	virtual void copy(const AvGraphicContext &);
	virtual void copy(const AvPGGraphicContext &);
	virtual void update( const int msk=0);
	Boolean canvasOpened()const;
  private:
	AvPGCanvas	*canvas_;
};

class AvPGCanvas : public AvCanvas {
  public:
	AvPGCanvas(AvPalette *pal=NULL,
		   const char *deviceName=NULL, const char *fileName=NULL);
	// Pass in parameters for PGBEG.
	void beginArgs(const int unit=0,
		   const char *device=NULL, const char *file=NULL,
		   const int nxsub=1, const int nysub=1);
	Boolean opened()const{return opened_;}
	virtual void drawRaster(AvRaster &img,
			int srcx=0, int srcy=0, int dstx=0, int dsty=0,
			unsigned int width=0, unsigned int height=0);
	virtual void drawRaster(AvRaster *img);
	virtual void drawVector(AvPolylineCmd *cmd);

	virtual void popState();
	virtual void setViewport(AvViewport &vp);
	virtual AvPalette *palette()const;
	AvPGPalette *getPGPalette()const{return palette_;}
	virtual void show(AvGraphics2D *dob);
	virtual void show(AvGraphics2D &dob);
	// Return a filename/device to use for PGPLOT output.
	// evar is a pointer to an environment variable to use to look for
	// a user supplied filename. def is returned if evar doesn't exist.
	static AvString getPGfilename(const char *evar=NULL,
				      const char *def=NULL);
	static AvString getPGdevicename(const char *evar=NULL,
					const char *def=NULL);
	// Form a PGPLOT file/device name. If fn is NULL and empty filename
	// will be used. (PGPLOT will supply a name). If dev is NULL, the
	// default device will be used.
	static AvString makePGname(const char *fn=NULL, const char *dev=NULL);
	////////////////////////////////////////////////////////////////
	//		(More or less) Direct interfaces to pgplot rtns.
	// Calls cpgbeg, but takes a const char *filename
	static int cpgbeg(const int unit, const char *file,
			  const int nxsub, const int nysub);
	static int cpgbeg(const int unit, const AvString &file,
			  const int nxsub, const int nysub);
	// Calls PGPLOT cpgend(), possible calling cpgiden() first.
	static void cpgend();
	static void cpgend(const Boolean showID);
	////////////////////////////////////////////////////////////////
	static void showID(const Boolean show){showID_ = show;}
	static Boolean showID(){return showID_;}
  protected:
	~AvPGCanvas();
	AvPGPalette *palette_;
	int	open();
	void	close();
	static char *strDup(const char *str);
	// Set window/clip plane.
	virtual void setClipPlane(AvViewport *vp=NULL);
	virtual void setClipPlane(AvViewport &vp);
  private:
	// Make sure scale_ isn't too big.
	void initScale(const float max, unsigned int size);
	// Scale an X, Y, W or H value.
	inline float scale(const float val)const{return val*scale_;}
	static const char *getPGEVar_(const char *evar, const char *def,
							const char *defev);
  private:
	int	unit_;
	char	*file_;
	char	*device_;
	int	nxsub_, nysub_;
	Boolean	opened_;
	float	scale_;			// Scale factor.
	float	xoff_, yoff_;		// Image offset
	static	Boolean	showID_;	// If true, cpgend() calls cpgiden().
};
#endif
