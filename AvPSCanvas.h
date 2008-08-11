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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvPSCanvas.h,v 19.0 2003/07/16 05:48:15 aips2adm Exp $
//
// $Log: AvPSCanvas.h,v $
// Revision 19.0  2003/07/16 05:48:15  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:47  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:43:15  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:22  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:59  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:09:20  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.2  1999/09/16 20:07:12  hravlin
// Removed egcs and IRIX -64 compiler warnings.
//
// Revision 13.1  1999/09/15 21:23:23  hravlin
// Initial checkin.
//
//---------------------------------------------------------------------------

/* AvPSCanvas.h
PostScript canvas.
*/

#ifndef AvPSCanvas_h
#define AvPSCanvas_h


#include <X11/Intrinsic.h>
#include "AvGraphics2D.h"
#include "AvCanvas.h"
#include "AvGraphicContext.h"
#include "AvPSDriver.h"

class AvPSCanvas;
class AvPSPalette;
class AvString;

class AvPSGraphicContext : public AvGraphicContext {
  public:
	AvPSGraphicContext(AvPSCanvas *can);
	AvPSGraphicContext(const AvGraphicContext&);
	virtual AvGraphicContext& operator=(const AvGraphicContext &);
	virtual AvPSGraphicContext& operator=(const AvPSGraphicContext &);
	~AvPSGraphicContext();

	virtual void setLineWidth(const int lw);
	virtual void setLineStyle(const LineStyle ls);
	virtual void setForeground(const unsigned long);
//	virtual void setBackground(const unsigned long);
  private:
	virtual void copy(const AvGraphicContext &);
	virtual void copy(const AvPSGraphicContext &);
	virtual void update( const int msk=0);
	Boolean canvasOpened()const;
  private:
	AvPSCanvas	*canvas_;
};

class AvPSCanvas : public AvCanvas {
  public:
	AvPSCanvas(AvPalette *pal, AvPSDriver *psd,
		   const float xscale, const float yscale);
	Boolean opened()const{return opened_;}
	virtual void drawRaster(AvRaster &img,
			int srcx=0, int srcy=0, int dstx=0, int dsty=0,
			unsigned int width=0, unsigned int height=0);
	virtual void drawRaster(AvRaster *img);
	virtual void drawVector(AvPolylineCmd *cmd);

	virtual void popState();
	virtual void setViewport(AvViewport &vp);
	virtual AvPalette *palette()const;
	AvPSPalette *getPSPalette()const{return palette_;}
	AvPSDriver *PSDriver()const{return psd_;}
	virtual void show(AvGraphics2D *dob);
	virtual void show(AvGraphics2D &dob);
	// Return a filename/device to use for PostScript output.
	// evar is a pointer to an environment variable to use to look for
	// a user supplied filename. def is returned if evar doesn't exist.
	static AvString getPSfilename(const char *evar=NULL,
				      const char *def=NULL);
	static AvString getPSdevicename(const char *evar=NULL,
					const char *def=NULL);
	// Form a PostScript file/device name. If fn is NULL and empty filename
	// will be used. (PostScript will supply a name). If dev is NULL, the
	// default device will be used.
	static AvString makePSname(const char *fn=NULL, const char *dev=NULL);
	////////////////////////////////////////////////////////////////
	// Convert from pixel to current PS dimension.
	inline void pixelToPS(const float xi, const float yi,
				    float &xo, float &yo,
			      const Boolean absolute=True)const
		{	psd_->fromPoints(xi, yi, xo, yo, absolute);
			xo *= xscale_;
			yo *= yscale_;
		}

	int imageOptions()const{return imageOpts_;}
	void imageOptions(const int opts){imageOpts_ = opts;}
  protected:
	~AvPSCanvas();
	AvPSPalette *palette_;
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
	static const char *getPSEVar_(const char *evar, const char *def,
							const char *defev);
  private:
	Boolean	opened_;
	float	scale_;			// Scale factor.
	float	xoff_, yoff_;		// Image offset
	AvPSDriver *psd_;
	float	xscale_, yscale_;
	int	imageOpts_;	// Image opts arg to drawImage.
};
#endif
