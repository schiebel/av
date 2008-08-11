//# Copyright (C) 1995-2000 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvPGDriver.h,v 19.0 2003/07/16 05:47:21 aips2adm Exp $
//
// $Log: AvPGDriver.h,v $
// Revision 19.0  2003/07/16 05:47:21  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:02  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:32  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:39  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:48  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:51:21  hravlin
// Changed some char * to const char *.
//
// Revision 14.0  2000/03/23 16:08:25  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/09/15 21:26:00  hravlin
// Updated copyright date.
//
// Revision 13.0  1999/08/10 18:40:14  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:03  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:20  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:16  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:28:21  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:17:19  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/02/05 17:55:33  hr
// ~AvPGDriver() should have been virtual.
//
// Revision 7.2  1996/12/12 06:46:40  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.2  1996/06/18  18:43:09  pixton
//  Copyright Update
//
//  Revision 1.1  1996/01/24  18:08:16  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvPGDriver.h
AvCanvas driver for PGPLOT.

	Implemented as 'hardcopy', no user feedback, no cursor support
and no pixel support, although these could be added later.

	In order to be functional, avdriv has to be called from
GREXEC as one of the PGPLOT device drivers.

	AvGraphics2D objects aren't supposed to keep track of canvases
since it is possible for them to be in the display lists of multiple
canvases. AvPGDriver uses the canvas to get scaling and palette
information so any routine accessing the AvPGDriver, should only do so
while in its 'draw' function. If the driver gets called without a canvas
being set, unexpected results may happen.

The static member function:
"AvPGDriver::initializePGColormappings(AvPalette *p)" should be called
soon after the palette is created to allocate colors for the PGPLOT
standard colors. It needs to be called before any raster images are
created. The default PGPLOT color bindings are not changeable.

AvPGDriver creates a display list of vector commands. Currently, only
one AvPolyline object is used, however, if/when raster objects are
supported, multiple AvPolyline objects may be created. Either supply
an AvViewportSetter object to receive the commands or use the one that
gets created. If it can be ensured that only one AvPolyline object
will be used, it is possible to ignore the AvViewportSetter and supply
an AvPolyline to receive the commands.

This file is meant to be included from C++ programs (to define AvPGDriver)
and from C programs (to define avdriv).
*/

#ifndef AvPGDriver_h
#define AvPGDriver_h


#if defined(__cplusplus)

#include <X11/Intrinsic.h>

class AvPolyline;
class AvCanvas;
class AvPalette;
class AvViewportSetter;
class AvPointList2D;

// Struct to map PGPLOT color indices to virtual color indices.
struct PGColormap {
	const char *name;
	unsigned long	vindex;
	unsigned long	pindex;
};

class AvPGDriver {
  public:
	AvPGDriver();
	virtual ~AvPGDriver();
	// Returns a pointer to the current driver.
	static AvPGDriver *currentDriver();
	static void currentDriver(AvPGDriver *);	// Sets current driver.

	// Graphics2D objects are appended to a viewport setter object.
	// OPEN will create one if one hasn't been set yet.
	void setVPS( AvViewportSetter *vps);
	// Return a pointer to the viewport setter obect.
	// NOTE: CLOSE will call unref. If the viewport setter hasn't
	// been ref'ed by something else, it will delete itself.
	AvViewportSetter *getVPS()const { return vps_;}
	// Returns device name. (OP=1).
	virtual const char *deviceName();
	enum OPCODES  { NONE=0, DEVICENAME=1, DIMENSIONS, SCALE, CAPABILITIES,
		DEVICE_NAME=5, DEFAULT_SIZE, CHARSCALE, SELECT,
		OPEN=9, CLOSE, BEGIN, LINE, POINT, ENDPICTURE,
		COLORINDEX=15, FLUSH, CURSOR, ERASEALPHA, LINESTYLE,
		FILLPOLYGON=20, COLORVALUE, LINEWIDTH, ESCAPE,
		FILLRECTANGLE=24, FILLPATTERN, PIXELS=26, SCALEINFO,
		MARKER, QCOLOR};
	void dump();
	void exec(int opc, float *rbuf, int *nbuf,
			     char *chr, int *lchr, int *mode, int len);
	inline Boolean tracing()const{return (tracemask_ == 0)? FALSE : TRUE;}
	void trace(const int mask) {tracemask_ = mask;}
	int  trace()const{return tracemask_;}
	void flush();
	void newPolyline(AvPolyline * =NULL);	// Setup new poly_ object.
	void setSize(const float width, const float height);
	void setCanvas(AvCanvas *c);
	// Setup PGPLOT colors for this driver & palette.
	void setPGColormappings(AvPalette *p);
	// Setup PGPLOT's color's. This needs to be called for each palette.
	// This should be called before any raster images are displayed.
	static  PGColormap *getPGColormappings(AvPalette *p, int &maplen);
	// Initializes colors, but doesn't return anything.
	static  void initializePGColormappings(AvPalette *p);
	// Returns the length of the PGPLOT color mapping table (always 16).
	static int pgcolorentries();
	// Choose color for drawing vectors. If ci is in (0..15), it is
	// treated as an index into the predefined color table. If > 15,
	// it is used as a virtual color index.
	void setColorIndex(const unsigned long ci);
  private:
	Boolean open(const int append, char *name, const int len);
	void close();
	// Begin picture (OP=11).
	void beginPicture(const float w, const float h);
	void endPicture();	// End picture (OP=14).
	// Return string, padding with blanks. Returns length of in.
	int	returnString(char *out, const char *in, int outlen);
	// Returns a copy of the string, with blanks trimmed and
	//NULL terminated.
	// Use delete [] to free.
	char	*dupString(const char *str, int inlen);
	// Flush the point cache.
	void flushPoints();
	// See if it is time to flush the point cache and do so if necessary.
	void checkPcache(const OPCODES o)
		{ if(popc_ != o){flushPoints(); popc_ = o;}}

	inline void TRACE(const OPCODES oc) const {if(tracing())trace_(oc);}
	void trace_(const OPCODES oc)const;
  private:
	static		AvPGDriver *current;
	AvPolyline	*poly_;		// Pointer to current polyline cmd.
	Boolean		polyappended_;	// TRUE if poly as been added to vps_.
	AvCanvas	*canvas_;	// Current canvas.
	int		plotID;		// Unused except by SELECT.
	int		unitID;		// Unused except by SELECT.
	char		*devName_;	// Device name passed by OPEN.
	Boolean		append_;
	float		width_;		// Size from BEGIN.
	float		height_;
	AvViewportSetter *vps_;
	AvPointList2D	*p1_, *p2_;	// Used to cache points.
	int		polycnt_;	// Holds # of points in a polygon.
	OPCODES		popc_;		// Last line/point opcode. Used for
					// caching.
	Boolean		tracemask_;
	PGColormap	*pgcmap_;
};
#endif

/* Interface to PGPLOT's GREXEC routine.
Notes: The parameter len is supposedly supplied by the FORTRAN compiler
to indicate the declared length of chr. It isn't clear whether all compilers
do this.
*/
#ifdef __cplusplus
extern "C" {
#endif

extern void avdriv(int *opc, float *rbuf, int *nbuf, char *chr, int *lchr,
		int *mode, int len);
extern void avdriv_(int *opc, float *rbuf, int *nbuf, char *chr, int *lchr,
		int *mode, int len);
extern int grexec_(int *idev, int *ifunc, float *rbuf, int *nbuf,
		char *chr, int *lchr, int chr_len);
#ifdef __cplusplus
}
#endif

#endif
