//# Copyright (C) 1995-2002 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvOverlay.h,v 19.0 2003/07/16 05:47:31 aips2adm Exp $
//
// $Log: AvOverlay.h,v $
// Revision 19.0  2003/07/16 05:47:31  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:11  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/07 22:32:44  hravlin
// Removed check for __P.
//
// Revision 17.0  2001/11/12 19:42:40  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:47  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:22  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:41:43  hravlin
// Changed some char * to const char *.
//
// Revision 14.0  2000/03/23 16:08:37  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:25  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:24  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:48  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:49  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.3  1998/06/09 21:16:49  hr
// Removed unneeded semicolon from DECLARE_HEADER.
//
// Revision 9.2  1998/05/04 20:35:06  hr
// Removed the unused function window(WINDOWTYPE).
//
// Revision 9.1  1998/02/20 22:00:48  hr
// Draws vector plots on top of the image from commands read in from an
// external file.
//
//
//---------------------------------------------------------------------------
/* AvOverlay.h

Subclass of AvPolyline that reads commands read in from a file and displays
them using PGPLOT calls.

*/

#ifndef AvOverlay_h
#define AvOverlay_h
//#if defined(__GNUC__) && defined(__P) && !defined(__linux__)
//#undef __P
//#endif
#include <math.h>
#include <AvPolyline.h>
#include "AvOverlayOptions.h"
#include <AvAccessor.h>

class AvRaster;
class OverlayGlobalOptions;

class AvOverlay : public AvPolyline {
  public:
	// Neither argument should be NULL, although AvOverlay will create
	// an options object if it is NULL.
	AvOverlay(AvAccessor *,AvOverlayOptions *);
	virtual void draw(AvCanvas &);
	virtual void draw(AvCanvas *);
	// Returns TRUE if the vector list exists.
	inline Boolean haveOverlay()const{return haveOverlay_;}
	// Get rid of current Overlay. (clears haveOverlay_).
	virtual void touch();
	void showOverlay(const Boolean);
	Boolean showOverlay()const;
	void setOptions(AvOverlayOptions *o);
	AvAccessor *accessor()const{return accessor_;}
	// Offsets within viewport to start drawing. (Pixels).
	void setMargins(const int lm, const int rm,
			const int tm, const int bm);
  protected:
	virtual ~AvOverlay();
  private:
	void initToZero();
	void generate(AvCanvas *c);	// Generate the Overlay vector list.
	void	drawOverlay();
	// Called by vector & line commands to pick up arguments.
	int vectorSetup(Tcl_Interp *interp, int argc, char *argv[],
			double &v1, double &v2, double &v3, double &v4,
			int &color);
	// Performs the drawing for the vector and line commands.
	void drawLine(const double x0, const double y0,
		      const double x1, const double y1,
		      const int color, const Boolean drawArrow);
	// Load an error message, returns TCL_ERROR.
	static int tclerror(Tcl_Interp *interp,
			    const char *msg, const int argc=0,
			    char *argv[]=NULL);

	// Decode a color value. Value can be either an integer or a color
	// name. Returns -1 if clrstr wasn't valid.
	int getColor(Tcl_Interp *, char *clrstr)const;
	// Get a list of doubles from argv. Returns the number of
	// successful decodes. Decodes v.nelements() values.
	static int getDoubles(Tcl_Interp *, char *argv[], AvWPosition &v);
	// Set variable 'name' with 'value'.
	// Returns TCL_OK or TCL_ERROR.
	const char *setVariable(Tcl_Interp *, const char *name, char *value);
  private:
	// Routines that are only available when TCL script is executing.
  public:
	// TCL Callbacks. These are only public so their addresses can
	// be in the static tables.
	static int vectorCmd(ClientData clientData, Tcl_Interp *interp,
			     int argc, char *argv[]);
	static int markerCmdCB( ClientData clientData, Tcl_Interp *interp,
				int argc, char *argv[]);
	int markerCmd(Tcl_Interp *interp, int argc, char *argv[]);

	static int lineCmd(ClientData clientData, Tcl_Interp *interp,
			   int argc, char *argv[]);
	static int lineToCmd(ClientData clientData, Tcl_Interp *interp,
			     int argc, char *argv[]);
	// Handles a number of simple commands.
	static int noArgsCmd(ClientData clientData, Tcl_Interp *interp,
			     int argc, char *argv[]);
#if 0
	static int oneStringCmd(ClientData clientData, Tcl_Interp *interp,
			        int argc, char *argv[]);
#endif
	static int oneIntCmd(ClientData clientData, Tcl_Interp *interp,
			     int argc, char *argv[]);
	static int oneRealCmd(ClientData clientData, Tcl_Interp *interp,
			      int argc, char *argv[]);
	static int textCmd(ClientData clientData, Tcl_Interp *interp,
			     int argc, char *argv[]);
	static int windowCmdCB(ClientData clientData, Tcl_Interp *interp,
			     int argc, char *argv[]);
	int windowCmd(Tcl_Interp *interp, int argc, char *argv[]);
	static int dsToWorldCB( ClientData clientData, Tcl_Interp *interp,
				int argc, char *argv[]);
	// From data set to user's current world.
	void datasetToWorld(const double dx, const double dy,
			    double &wx, double &wy);
	////////////////////////////////////////////////////////////////
	//		Math Functions
	static int pixelToWorldFuncCB(ClientData, Tcl_Interp *,
				      Tcl_Value *args, Tcl_Value *resultPtr);
	int pixelToWorldFunc(Tcl_Interp *,
			     Tcl_Value *args, Tcl_Value *resultPtr);
	static int datumFuncCB(ClientData, Tcl_Interp *,
				      Tcl_Value *args, Tcl_Value *resultPtr);
	int datumFunc(Tcl_Interp *,
			     Tcl_Value *args, Tcl_Value *resultPtr);
	////////////////////////////////////////////////////////////////
	//	Linked variables
	// Handles TCL/PGPLOT variables that hold ints (eg. Color).
	static char *intVarCB(ClientData , Tcl_Interp *,
				    char *name1, char *name2, int flags);
	static char *doubleVarCB(ClientData , Tcl_Interp *,
				 char *name1, char *name2, int flags);
	// Handles RO variables.
	static char *readOnlyVarCB(ClientData , Tcl_Interp *,
				   char *name1, char *name2, int flags);
	// NDC => 0..1. INDEX=> 0..<len-1>. WORLD=>wmin..wmax, USER=> from user
	// INDEX1 start at 1, rather than 0.
	enum WINDOWTYPE {NDC, INDEX, INDEX1, DATASET, USER};
	class grstate {
	 public:
		grstate();
		~grstate();
		void operator=(const grstate &s);
		double XToNDC(const double v)const
			{ return (v-minx())/(maxx()-minx());}
		double YToNDC(const double v)const
			{ return (v-miny())/(maxy()-miny());}
		inline double minx()const{return minx_;}
		inline double miny()const{return miny_;}
		inline double maxx()const{return maxx_;}
		inline double maxy()const{return maxy_;}
		inline WINDOWTYPE type()const{return type_;}
		inline void type(const WINDOWTYPE t){type_ = t;}
		const char *typeString(const WINDOWTYPE)const;
		const char *typeString()const;
//		window(const WINDOWTYPE);
		void window(const WINDOWTYPE t,
			    const double minx, const double miny,
			    const double maxx, const double maxy);
		friend ostream &operator<<(ostream &s, const grstate &);
	 private:
		WINDOWTYPE	type_;
		double minx_, miny_, maxx_, maxy_;
	};

	grstate *state()const{return &states_[0];}
	double worldXMin()const{return states_[0].minx();}
	double worldYMin()const{return states_[0].miny();}
	double worldXMax()const{return states_[0].maxx();}
	double worldYMax()const{return states_[0].maxy();}
	WINDOWTYPE windowType()const{return states_[0].type();}
	int saveState();
	int restoreState();
	// Convert from user's window coords to what we're using for PGPLOT.
	// Used to convert plotting positions.
	void worldToWindow(const double wx, const double wy,
			   double &winx, double &winy,
			   const Boolean scaleOnly=FALSE);
	void indexToWorld(const double wx, const double wy,
			  double &ix, double &iy);
	// Convert between user coords and index.
	void worldToIndex(const double wx, const double wy,
			  double &ix, double &iy);
#if 0
	windowToWorld(const double xwin, const double ywin, AvWPosition world);
	// Convert between index (0..<len-1>) and data set world coords.
	void indexToDataset(const AvWPosition i, AvWPosition &w)const;
	void datasetToIndex(const AvWPosition w, AvWPosition &i)const;
#endif
	// Size of image in 'index' coords. (maxindex)
	double width()const{return (double)accessor_->width();}
	double height()const{return (double)accessor_->height();}
	double depth()const{return (double)accessor_->depth();}
	// Does initializations that need doing each time an interpreter
	// is run.
	void resetForInterp();
	// Decode text justification [LEFT|CENTER|RIGHT|fp #]
	// returns NULL or pointer to error string.
	const char *strToJustification(Tcl_Interp *, char *str,
				       double &just)const;
	// Convert to string. If always is FALSE, only returns the strings:
	// LEFT, CENTER, RIGHT for 0, 0.5, 1.0. Otherwise, NULL. If always
	// is TRUE, other values are converted to a string.
	const char *justificationToStr(const double just,
				       const Boolean always=FALSE)const;
	// Decode marker symbol.
	// returns NULL or pointer to error string.
	const char *strToSymbol(Tcl_Interp *, char *str, int &symb)const;
	// Returns current position from PGPLOT lib.
	void getPosition(double &x, double &y)const;
  private:
	DECLARE_HEADER
	AvOverlayOptions	*options_;
	AvAccessor		*accessor_;
	Boolean			haveOverlay_;
	////////////////////////////////////////////////////////////////
	// Tcl interpreter used during callbacks. This is set by drawOverlay()
	// and is only valid during execution of the TCl script.
	AvTcl			*tcl_;
	////////////////////////////////////////////////////////////////
	// graphics state
	int			stateIndex_;
	grstate			*states_;
	// Keeps track of last world position used so it doesn't have to
	// be created all the time.
	AvWPosition		world_;
	AvIPosition		ijk_;	// Used with world_.
	int	lm_, bm_, tm_, rm_;	// Margins within viewport to draw
					// axis' box.
	// Viewport start and range in NDC.
	double	vpxmin_, vpymin_;
	double	vpxrange_, vpyrange_;	// rm - lm, tm-bm.
	// textat variables.
	double	textAngle_;			// Rotation in degrees.
	double	textJustification_;
	int	markerSymbol_;
};

#endif
