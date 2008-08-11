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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvOverlay.cc,v 19.0 2003/07/16 05:47:30 aips2adm Exp $
//
// $Log: AvOverlay.cc,v $
// Revision 19.0  2003/07/16 05:47:30  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:09  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.3  2002/01/22 22:10:59  hravlin
// Added some (int) casts to sizeof()s for 64 bit compilers.
//
// Revision 17.2  2002/01/14 17:38:16  hravlin
// Added extern "C" wrappers for static member callbacks.
//
// Revision 17.1  2002/01/03 19:53:52  hravlin
// Include strings.h
//
// Revision 17.0  2001/11/12 19:42:39  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:45  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:17  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:41:32  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:08:35  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/08/25 19:46:18  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:40:23  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:21  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:44  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:44  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.6  1998/07/07 14:34:21  hr
// oneRealCmd() was returning NULL instead of TCL_OK.
//
// Revision 9.5  1998/06/19 14:15:18  hr
// Added string.h to list of included files.
//
// Revision 9.4  1998/06/09 21:24:01  hr
// Removed unneeded semicolon after DEFINE_MEMBER_INFO macro.
//
// Revision 9.3  1998/05/04 20:34:24  hr
// Removed an unused variable.
//
// Revision 9.2  1998/02/20 22:36:16  hr
// Removed IRIX & g++ compiler warnings.
//
// Revision 9.1  1998/02/20 21:59:54  hr
// Draws vector plots on top of the image from commands read in from an
// external file.
//
//
//---------------------------------------------------------------------------
/* AvOverlay.cc

Subclass of AvPolyline that draws overlays from commands read in from a
file.

*/
#ifdef TCL
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>			// SGI when POSIX_SRC is defined
#include "AvOverlay.h"
#include "AvViewportSetter.h"
#include "AvRaster.h"
#include "AvPGDriver.h"
#include "AvCanvas.h"
#include "AvAccessor.h"
#include "AvTcl.h"

extern "C" {
#include <cpgplot.h>
}


// Number of levels of save.
enum {MAXSTATES=10};

////////////////////////////////////////////////////////////////
// Error strings. (NOT consts!)
static char NUMARGS[] = "Wrong # of arguments.";
static char BADVALUE[] = "Bad value.";
static char BADCOLOR[] = "Bad color value.";
static char BADFONT[] = "Bad font name.";
static char BADLINESTYLE[] = "Bad line style.";
static char BADLINEWIDTH[] = "Bad line width.";
static char BADMARKER[] = "Bad Marker type.";
static char NOOVERLAY[] = "Internal error: No AvOverlay for callback.";
static char BADLABEL[] = "Bad label."; 
static char BADWINDOW[] = "Bad window name.";
static char READONLYVAR[] = "Variable is read only.";
static char STATEOVERFLOW[] = "Too many saves.";
static char STATEUNDERFLOW[] = "Too many restores.";
static char BADNULL[] = "Internal error: Illegal NULL pointer.";

////////////////////////////////////////////////////////////////
// Supported command names.
// (don't use 'const' since TCL doesn't handle).
#if 0
static const char *VECTOR_CMD = "vector";	// Vector/line names are special!!
static const char *VECTORA_CMD = "vectora";
static const char *PAVECTOR_CMD = "pavector";
static const char *PAVECTORA_CMD = "pavectora";
#endif
static const char *MOVETO = "move";
static const char *LINETO = "draw";
static const char *LINETOA = "arrow";
static const char *SAVE = "save";
static const char *RESTORE = "restore";
static const char *WINDOW = "window";
static const char *DSTOWORLD = "datasetToWorld";
static const char *TEXT = "text";
static const char *TEXTAT = "label";
static const char *MARKER = "marker";
static const char *COLOR_CMD = "color";
static const char *FONT_CMD = "font";
// These repeat the variables for an alternate way to set the value.
// Names can only differ from variables in case.
static const char *LINESTYLE_CMD = "lineStyle";
static const char *LINEWIDTH_CMD = "lineWidth";
static const char *TEXTHEIGHT_CMD = "textHeight";
static const char *TEXTBG_CMD = "textBackground";
static const char *TEXTANGLE_CMD = "angle";
static const char *TEXTJUSTIFICATION_CMD = "justification";
static const char *SYMBOL_CMD = "symbol";

////////////////////////////////////////////////////////////////
// Tcl variables. Access references the appropriate PGPLOT function.
static const char *COLOR = "Color";
static const char *FONT = "Font";
static const char *LINESTYLE = "LineStyle";
static const char *LINEWIDTH = "LineWidth";
static const char *TEXTHEIGHT = "TextHeight";
static const char *TEXTBG = "TextBackground";
static const char *TEXTANGLE = "Angle";
static const char *TEXTJUSTIFICATION = "Justification";
static const char *SYMBOL = "Symbol";

////////////////////////////////////////////////////////////////
// Args for the pixelToWorld function.
static Tcl_ValueType pixelToWorldArgTypes[] =
					{TCL_DOUBLE, TCL_DOUBLE, TCL_INT};
static int pixelToWorldNumArgs =
	(int)(sizeof(pixelToWorldArgTypes)/sizeof(*pixelToWorldArgTypes));
static Tcl_ValueType datumArgTypes[] = {TCL_DOUBLE, TCL_DOUBLE};
static int datumNumArgs =
		(int)(sizeof(datumArgTypes)/sizeof(*datumArgTypes));
////////////////////////////////////////////////////////////////
// It is no longer kosher to assume a static C++ function is equivalent
// to an extern "C" function. These are wrappers to act as interfaces.
extern "C" {
static int dsToWorldCB(ClientData clientData, Tcl_Interp *interp,
			int argc, char *argv[])
{	return AvOverlay::dsToWorldCB(clientData, interp, argc, argv);
}

static int lineCmd(ClientData clientData, Tcl_Interp *interp,
			int argc, char *argv[])
{	return AvOverlay::lineCmd(clientData, interp, argc, argv);
}

static int lineToCmd(ClientData clientData, Tcl_Interp *interp,
			int argc, char *argv[])
{	return AvOverlay::lineToCmd(clientData, interp, argc, argv);
}

static int markerCmdCB(ClientData clientData, Tcl_Interp *interp,
			int argc, char *argv[])
{	return AvOverlay::markerCmdCB(clientData, interp, argc, argv);
}

static int noArgsCmd(ClientData clientData, Tcl_Interp *interp,
			int argc, char *argv[])
{	return AvOverlay::noArgsCmd(clientData, interp, argc, argv);
}

static int oneIntCmd(ClientData clientData, Tcl_Interp *interp,
			int argc, char *argv[])
{	return AvOverlay::oneIntCmd(clientData, interp, argc, argv);
}

static int oneRealCmd(ClientData clientData, Tcl_Interp *interp,
			int argc, char *argv[])
{	return AvOverlay::oneRealCmd(clientData, interp, argc, argv);
}

static int textCmd(ClientData clientData, Tcl_Interp *interp,
			int argc, char *argv[])
{	return AvOverlay::textCmd(clientData, interp, argc, argv);
}

static int vectorCmd(ClientData clientData, Tcl_Interp *interp,
			int argc, char *argv[])
{	return AvOverlay::vectorCmd(clientData, interp, argc, argv);
}

static int windowCmdCB(ClientData clientData, Tcl_Interp *interp,
			int argc, char *argv[])
{	return AvOverlay::windowCmdCB(clientData, interp, argc, argv);
}

}
////////////////////////////////////////////////////////////////
// When any command or variable is added. It must also be added to
// the handling procedure.

// List of Tcl commands to be added to interpreter.
static struct {
		const char		*name;
		Tcl_CmdProc		*cmdproc;
		Tcl_CmdDeleteProc	*delproc;
		}TclCmds[] = {
	////////////////
	// Vector command names can't be changed w/o editing the callback
	{ "vector", &vectorCmd, NULL},	// draw vector
	{ "vectora", &vectorCmd, NULL},	//  with arrow
	{ "pavector", &vectorCmd, NULL},	// draw position vector
	{ "pavectora", &vectorCmd, NULL},	//  with arrow
	{ "line", &lineCmd, NULL},
	{ "linea", &lineCmd, NULL},
	////////////////
	{ MOVETO, &lineToCmd, NULL},
	{ LINETO, &lineToCmd, NULL},
	{ LINETOA, &lineToCmd, NULL},
	{ SAVE, &noArgsCmd, NULL},	// Save state
	{ RESTORE, &noArgsCmd, NULL},	// restore
	{ TEXT, &textCmd, NULL},		// draw text.
	{ TEXTAT, &textCmd, NULL},		// draw text.
	{ WINDOW, &windowCmdCB, NULL},	// User's wrld window.
	{ DSTOWORLD, &dsToWorldCB, NULL},	// Cnvrt ds->wrld.
	{ MARKER, &markerCmdCB, NULL},
	{ COLOR_CMD, &oneIntCmd, NULL},
	{ FONT_CMD, &oneIntCmd, NULL},
	{ LINESTYLE_CMD, &oneIntCmd, NULL},
	{ LINEWIDTH_CMD, &oneIntCmd, NULL},
	{ SYMBOL_CMD, &oneIntCmd, NULL},
	{ TEXTBG_CMD, &oneIntCmd, NULL},
	{ TEXTHEIGHT_CMD, &oneRealCmd, NULL},
	{ TEXTANGLE_CMD, &oneRealCmd, NULL},
	{ TEXTJUSTIFICATION_CMD, &oneRealCmd, NULL},
	};
static int NCMNDS = (int)(sizeof(TclCmds)/sizeof(*TclCmds));

////////////////////////////////////////////////////////////////
// These color values are ordered to match their corresponding PGPLOT values.
enum COLORS{ BLACK, WHITE, RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW,
	     MAXCOLOR=YELLOW};
// This table is ordered so color enums can index it.
IntMap	Colors[] = {
	{BLACK, "BLACK"},
	{WHITE, "WHITE"},
	{RED, "RED"},
	{GREEN, "GREEN"},
	{BLUE, "BLUE"},
	{CYAN, "CYAN"},
	{MAGENTA, "MAGENTA"},
	{YELLOW, "YELLOW"},
	{0, NULL}
	};
//static const int NCOLORS = sizeof(Colors)/sizeof(*Colors) -1;
////////////////////////////////////////////////////////////////
static int strToValue(IntMap *tbl, const char *value, int def=-1)
{ IntMap *ptr=tbl;
	while(ptr->name != NULL)
	{	if(strcasecmp(ptr->name, value) == 0)
			return ptr->value;
		else
			ptr++;
	}
	return def;
}

static const char *valueToStr(IntMap *tbl, const int value)
{ IntMap *ptr=tbl;
	while(ptr->name != NULL)
	{	if(ptr->value == value)
			return ptr->name;
		else
			ptr++;
	}
	return NULL;
}

// Convert a color name to its value. Color has its own rtn since its used
// in several places.
static int strToColor(const char *color)
{	return	strToValue(Colors, color);
}

static const char *colorToStr(const int color)
{	return	valueToStr(Colors, color);
}

// Font information for all the Fonts toggles.
// Strings are in upper case since they will become RO variables.
enum FONTS{ SIMPLEFONT=1, ROMAN, ITALIC, SCRIPT, MAXFONT=SCRIPT};
IntMap	Fonts[] = {
	{SIMPLEFONT, "SIMPLE"},
	{ROMAN, "ROMAN"},
	{ITALIC, "ITALIC"},
	{SCRIPT, "SCRIPT"},
	{0, NULL}
	};
//static const int NFONTS = sizeof(Fonts)/sizeof(*Fonts) -1;

// Line Style information for all the Line Styles toggles.
// Strings are in upper case since they will become RO variables.
enum LINESTYLES{SOLIDLINE=1, DASHED, DOT_DASH_DOT_DASH, DOTTED,
		DASH_DOT_DOT_DOT, MAXLINESTYLE=DASH_DOT_DOT_DOT};
IntMap	LineStyles[] = {
	{SOLIDLINE, "SOLIDLINE"},
	{DASHED, "DASHED"},
	{DOT_DASH_DOT_DASH, "DOT_DASH_DOT_DASH"},
	{DOTTED, "DOTTED"},
	{DASH_DOT_DOT_DOT, "DASH_DOT_DOT_DOT"},
	{0, NULL}
	};
//static const int NLINESTYLES = sizeof(LineStyles)/sizeof(*LineStyles) -1;

enum MARKERS { PLUS=2, ASTERIX, CIRCLE, CROSS, STAR=12, BOX=16, FILLED_STAR=18,
	       FILLED_CIRCLE=-26};
IntMap MarkerTypes[] = {
	{PLUS, "PLUS"},
	{ASTERIX, "ASTERIX"},
	{CIRCLE, "CIRCLE"},
	{CROSS, "CROSS"},
	{STAR, "STAR"},
	{BOX, "BOX"},
	{FILLED_STAR, "FILLED_STAR"},
	{FILLED_CIRCLE, "FILLED_CIRCLE"},
	{0, NULL}
	};


// These are indexes into Justifications.
enum JUSTIFICATIONS {LEFT, CENTER, RIGHT};
static const char *Justifications[] = {"LEFT", "CENTER", "RIGHT"};

////////////////////////////////////////////////////////////////
extern "C" {
static char *readOnlyVarCB(ClientData cd, Tcl_Interp *interp,
				   char *name1, char *name2, int flags)
{  return AvOverlay::readOnlyVarCB(cd , interp, name1, name2, flags);
}
}

// Add a read only variable to the interpreter
static void addROVar(Tcl_Interp *interp, const char *name, const int value)
{ char vbuf[32];

	sprintf(vbuf, "%d", value);
	AvTcl::Tcl_SetVar(interp, name, vbuf, 0);
	AvTcl::Tcl_TraceVar(interp, name, TCL_TRACE_WRITES,
			    &readOnlyVarCB, (ClientData)vbuf);
}

// Add a number of variables to the interpreter.
// Create RO variables from an IntMap.
static void addROVars(Tcl_Interp *interp, IntMap *tbl)
{ IntMap *ptr = tbl;

	while(ptr->name != NULL)
	{	addROVar(interp, ptr->name, ptr->value);
		ptr++;
	}
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

/* AvGraphics2D boiler plate. */
DEFINE_MEMBER_INFO(AvOverlay)

				   // Assume a bunch of points (arbitrary).
AvOverlay::AvOverlay(AvAccessor *acc, AvOverlayOptions *o) : AvPolyline(1024)
{
	INIT_CLASS(AvPolyline);
	name("Overlay");
	accessor_ = acc;
	accessor_->ref();
	initToZero();

	// o Should not be NULL, but if it is, don't die.
	if(o == NULL)
		o = new AvOverlayOptions("Overlay", "config", accessor_);
	setOptions(o);
}

// Initialize things to 0. (Requires accessor to be set).
void AvOverlay::initToZero()
{
	options_ = NULL;
	haveOverlay_ = FALSE;
	tcl_ = NULL;
	states_ = new grstate[MAXSTATES+1];
	setMargins(0, 0, 0, 0);
	resetForInterp();
}

// Does initializations that need doing each time an interpreter is run.
void AvOverlay::resetForInterp()
{	stateIndex_ = 1;
	textAngle_ = 0.0;
	textJustification_ = 0.0;
	markerSymbol_ = PLUS;
	world_ = accessor_->minRegion();
	ijk_.resize(accessor()->rank());
	ijk_ = 0;
	state()->window(NDC, 0.0, 1.0, 0.0, 1.0);
}


// When the options object changes mark as needing to regenerate.
static void optionsCB(XtPointer , AvConductor *, XtPointer data)
{AvOverlay *me = (AvOverlay *)data;

	me->touch();
}

// Change current options pointer.
void AvOverlay::setOptions(AvOverlayOptions *o)
{
	if(o == options_)
		return;
	if(options_ != NULL)
	{	options_->removeCallback(&optionsCB, AvConductor::ValueChanged,
					 this, this);
		options_->unref();
	}
	options_ = o;
	if(options_ != NULL)
	{	options_->ref();
		// Add to front of list so the imageSetter gets called after
		// this touches itself.
		options_->addCallback(&optionsCB, AvConductor::ValueChanged,
				      this, this, TRUE);
	}
}

AvOverlay::~AvOverlay()
{
//cout << "AvOverlay::~AvOverlay()" << endl;

	if(options_ != NULL)
		options_->unref();
	options_ = NULL;
	accessor_->unref();
	accessor_ = NULL;
	delete [] states_;
}

void AvOverlay::touch()
{
	haveOverlay_ = FALSE;
	reset(-1);	// tell AvPolyline to get rid of its point list.
	AvPolyline::touch();
}

static inline double deg2rad(const double angle)
{
	return angle/57.29577951308232;
}

extern "C" {
static int pixelToWorldFuncCB(ClientData cd, Tcl_Interp *interp,
			      Tcl_Value *args, Tcl_Value *resultPtr)
{
  return AvOverlay::pixelToWorldFuncCB(cd, interp, args, resultPtr);
}

static int datumFuncCB(ClientData cd, Tcl_Interp *interp,
			      Tcl_Value *args, Tcl_Value *resultPtr)
{
  return AvOverlay::datumFuncCB(cd, interp, args, resultPtr);
}

static char *intVarCB(ClientData cd , Tcl_Interp *interp,
			    char *name1, char *name2, int flags)
{
 return AvOverlay::intVarCB(cd , interp, name1, name2, flags);
}

static char *doubleVarCB(ClientData cd , Tcl_Interp *interp,
			 char *name1, char *name2, int flags)
{
 return AvOverlay::doubleVarCB(cd , interp, name1, name2, flags);
}
}

// Create an interpreter, load commands, functions and variables. Then
// run the script.
void AvOverlay::drawOverlay()
{AvOverlayOptions *optns = options_;

	if(optns == NULL)
		return;

	// Reset between calls.
	resetForInterp();

	AvTcl avinterp;
	Tcl_Interp *interp = avinterp.interp();
	// Some callbacks need a reference to this.
	tcl_ = &avinterp;

	// Add commands.
	for(int n=0; n < NCMNDS; n++)
	{	AvTcl::Tcl_CreateCommand(interp, TclCmds[n].name,
			TclCmds[n].cmdproc, (ClientData)this,
			TclCmds[n].delproc);
	}

	AvTcl::Tcl_CreateMathFunc(interp, "pixelToWorld",
		pixelToWorldNumArgs, pixelToWorldArgTypes,
		&::pixelToWorldFuncCB, this);

	AvTcl::Tcl_CreateMathFunc(interp, "datum",
		datumNumArgs, datumArgTypes,
		&::datumFuncCB, this);

	////////////////////////////////////////////////////////////////
	// Initialize RW variables.
	avinterp.setIntVal(COLOR, optns->color_.getValue());
	AvTcl::Tcl_TraceVar(interp, COLOR,
		     TCL_TRACE_WRITES | TCL_TRACE_READS,
//		     TCL_TRACE_WRITES | TCL_TRACE_READS | TCL_GLOBAL_ONLY,
		     &::intVarCB, (ClientData)this);

	avinterp.setIntVal(FONT, SIMPLEFONT);
	AvTcl::Tcl_TraceVar(interp, FONT,
		     TCL_TRACE_WRITES | TCL_TRACE_READS | TCL_GLOBAL_ONLY,
		     &::intVarCB, (ClientData)this);

	avinterp.setIntVal(LINESTYLE, SOLIDLINE);
	AvTcl::Tcl_TraceVar(interp, LINESTYLE,
		     TCL_TRACE_WRITES | TCL_TRACE_READS | TCL_GLOBAL_ONLY,
		     &::intVarCB, (ClientData)this);

	avinterp.setIntVal(LINEWIDTH, 1);
	AvTcl::Tcl_TraceVar(interp, LINEWIDTH,
		     TCL_TRACE_WRITES | TCL_TRACE_READS | TCL_GLOBAL_ONLY,
		     &::intVarCB, (ClientData)this);

	avinterp.setIntVal(TEXTBG, -1);
	AvTcl::Tcl_TraceVar(interp, TEXTBG,
		     TCL_TRACE_WRITES | TCL_TRACE_READS | TCL_GLOBAL_ONLY,
		     &::intVarCB, (ClientData)this);

	avinterp.setDoubleVal(TEXTHEIGHT, 1.0);
	AvTcl::Tcl_TraceVar(interp, TEXTHEIGHT,
		     TCL_TRACE_WRITES | TCL_TRACE_READS | TCL_GLOBAL_ONLY,
		     &::doubleVarCB, (ClientData)this);

	avinterp.setDoubleVal(TEXTANGLE, textAngle_);
	AvTcl::Tcl_TraceVar(interp, TEXTANGLE,
		     TCL_TRACE_WRITES | TCL_TRACE_READS | TCL_GLOBAL_ONLY,
		     &::doubleVarCB, (ClientData)this);

	avinterp.setDoubleVal(TEXTJUSTIFICATION, textJustification_);
	AvTcl::Tcl_TraceVar(interp, TEXTJUSTIFICATION,
		     TCL_TRACE_WRITES | TCL_TRACE_READS | TCL_GLOBAL_ONLY,
		     &::doubleVarCB, (ClientData)this);

	avinterp.setDoubleVal(SYMBOL, markerSymbol_);
	AvTcl::Tcl_TraceVar(interp, SYMBOL,
		     TCL_TRACE_WRITES | TCL_TRACE_READS | TCL_GLOBAL_ONLY,
		     &::intVarCB, (ClientData)this);

	addROVars(interp, Colors);
	addROVars(interp, Fonts);
	addROVars(interp, LineStyles);
	addROVars(interp, MarkerTypes);
	addROVar(interp, "TRANSPARENT", -1);	// For text background.
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	////	PGPLOT initializations.
	////////////////////////////////////////////////////////////////
	cpgsci(optns->color_.getValue());
	int ec = avinterp.evalFile(optns->filename_.getValue());
	if(ec != TCL_OK)
	{	cout	<< "Line " << interp->errorLine << ": "
			<< interp->result << endl;
	}
	else	// Don't print if there was another error.
	if(stateIndex_ > 1)
		cout << "Too many \"save\"s w/o \"restore\"s." << endl;

	// Not valid after return.
	tcl_ = NULL;
}

// Generate the display list for the overlay.
// Create a TCL interpreter, add commands then execute.
void AvOverlay::generate(AvCanvas *c)
{
AvViewport *vp;
AvViewportSetter *myvps;
float cvpw, cvph;	// Canvas' viewport size.

//	touch();

//cout << "AvOverlay::generate." << endl;

	// Use the current canvas viewport to get size and raster scaling
	// information.
	vp = c->viewport();
	cvpw = vp->width();
	cvph = vp->height();

//****************************************************************
//****************************************************************
	//			Make Overlay.

	// Create a PGPLOT driver object and tell it to draw into this.
	AvPGDriver *pgdriver = new AvPGDriver();
	pgdriver->setCanvas(c);
	// Create a viewport the same size as the current one.
	myvps = new AvViewportSetter(0.0, 0.0, cvpw, cvph);
	pgdriver->setVPS(myvps);

	pgdriver->newPolyline(this);
	AvPGDriver::currentDriver(pgdriver);
//****************************************************************
//			PGPLOT code
	// Open device.
	if(cpgbeg(0, "/CANVAS", 1, 1) != 1)
		return;
	cpgask(0);		// Don't ask.

	// Set scaling so coords in range (0..1) map onto the real viewport.
	double width=(double)cvpw;
	double height = (double)cvph;
	vpxmin_ = (double)lm_/width;
	vpymin_ = (double)bm_/height;
	vpxrange_ = (width - rm_)/width - vpxmin_;
	vpyrange_ = (height - tm_)/height - vpymin_;
	cpgsvp(0.0, 1.0, 0.0, 1.0);

	////////////////////////////////////////////////////////////////
	///	Draw Overlays
	////////////////////////////////////////////////////////////////
	 drawOverlay();
	cpgend();

//****************************************************************
//****************************************************************
//****************************************************************

	delete pgdriver;
	haveOverlay_ = TRUE;
}

void AvOverlay::draw(AvCanvas &c)
{
	if(ignore())
		return;

	// (Re) generate point list if there is no current list.
	if(!haveOverlay())
		generate(&c);

	AvPolyline::draw(c);
}

void AvOverlay::draw(AvCanvas *c)
{
	if(c != NULL)
		draw(*c);
}

void AvOverlay::showOverlay(const Boolean show )
{
	ignore(show ? FALSE : TRUE);
}

Boolean AvOverlay::showOverlay()const
{
	return ignore() ? FALSE :TRUE ;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
///			TCL callbacks.
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// Draw a line from x0/y0 to x1/y1. If color is >= 0, save current color
// and use supplied one. If drawArrow is TRUE, draw an arrow instead of
// a line. No transformations are done.
void AvOverlay::drawLine(const double x0, const double y0,
			 const double x1, const double y1,
			 const int color, const Boolean drawArrow)
{
	int oldcolor;
	if(color >= 0)
	{	cpgqci(&oldcolor);
		cpgsci(color);
	}
	float x[2], y[2];
	x[0] = (float)x0;
	x[1] = (float)x1;
	y[0] = (float)y0;
	y[1] = (float)y1;

	if(drawArrow)
		cpgarro(x[0], y[0], x[1], y[1]);
	else
		cpgline(2,x, y);

	if(color >= 0)
		cpgsci(oldcolor);
}

// Pick up the arguments for the vector and line commands.
// <dbl> <dbl> <dbl> <dbl> [ COLOR ]
// Color is set to -1 if no valid color was found.
int AvOverlay::vectorSetup(Tcl_Interp *interp, int argc, char *argv[],
			   double &v1, double &v2, double &v3, double &v4,
			   int &color)
{

	if((argc < 5) || (argc > 6))
		return tclerror(interp, NUMARGS, argc, argv);

#if 1
	if(Tcl_GetDouble(interp, argv[1], &v1) != TCL_OK)
		return tclerror(interp, BADVALUE, 2, argv);
	if(Tcl_GetDouble(interp, argv[2], &v2) != TCL_OK)
		return tclerror(interp, BADVALUE, 3, argv);
	if(Tcl_GetDouble(interp, argv[3], &v3) != TCL_OK)
		return tclerror(interp, BADVALUE, 4, argv);
	if(Tcl_GetDouble(interp, argv[4], &v4) != TCL_OK)
		return tclerror(interp, BADVALUE, 4, argv);
#else
	AvWPosition v(4);
	int n = getDoubles(interp, &argv[1], v);
	if(n != 4)
		return tclerror(interp, BADVALUE, n+2, argv);
	v1 = v(0);
	v2 = v(1);
	v3 = v(2);
	v4 = v(3);
#endif

	if(argc > 5)
	{	color = getColor(interp, argv[5]);
		if(color < 0)
			return tclerror(interp, BADCOLOR, 6, argv);
	}
	else
		color = -1;	// Set to no color seen.

	return TCL_OK;
}

// Handle vector commands.
// {pa}vector{a} x y angle_in_degrees [COLOR]
int AvOverlay::vectorCmd( ClientData clientData, Tcl_Interp *interp, int argc,
		char *argv[])
{double x0, y0, x1, y1, degrees, length;
 int color;
 AvOverlay *me = (AvOverlay *)clientData;

#if 0
	if(me == NULL)
	{	interp->result = NOOVERLAY;
		return TCL_ERROR;
	}
#else
	if(me == NULL)
	{	AvTcl::Tcl_SetResult(interp,NOOVERLAY);
		return TCL_ERROR;
	}
#endif
	AvOverlayOptions *optns = (AvOverlayOptions *)me->options_;

	if(me->vectorSetup(interp, argc, argv, x0, y0, degrees, length,
			     color) != TCL_OK)
		return TCL_ERROR;

	char *cmd = argv[0];

	// Length scale.
	double scale = optns->scale_.getValue();
	double angle = deg2rad(degrees);

	// Convert to PGPLOT coords.
	me->worldToWindow(x0, y0, x0, y0);
	length = length/100.0;	// In percent.

	length *= scale;

	////////////////////////////////////////////////////////////////
	// The following two checks depend on the spelling of the commands.
	////////////////////////////////////////////////////////////////
	// See if we're using Position Angle vectors.
	Boolean positionAngle;
	if((*cmd == 'p') || (*cmd == 'P'))
	{	cmd += 2;	// Past the "PA".
		positionAngle = TRUE;
	}
	else
		positionAngle = FALSE;

	// Draw arrow or normal vector?
	Boolean drawArrow;
	if( *(cmd+6) == '\0')
		drawArrow = FALSE;
	else
		drawArrow = TRUE;
	////////////////////////////////////////////////////////////////

	if(positionAngle)	// 0 is vertical.
	{	x1 = x0 + -sin(angle)*length;
		y1 = y0 + cos(angle)*length;
	}
	else			// 0 is horizontal.
	{	x1 = x0 + cos(angle)*length;
		y1 = y0 + sin(angle)*length;
	}

	me->drawLine(x0, y0, x1, y1, color, drawArrow);
	return TCL_OK;
}

// Handle line commands.
// line{a} x0 y0 x1 y1 [COLOR]
int AvOverlay::lineCmd( ClientData clientData, Tcl_Interp *interp, int argc,
			char *argv[])
{double x0, y0, x1, y1;
 int ec, color;
 AvOverlay *me = (AvOverlay *)clientData;

#if 0
	if(me == NULL)
	{	interp->result = NOOVERLAY;
		return TCL_ERROR;
	}
#else
	if(me == NULL)
	{	AvTcl::Tcl_SetResult(interp,NOOVERLAY);
		return TCL_ERROR;
	}
#endif

//	AvOverlayOptions *optns = (AvOverlayOptions *)me->options_;

	ec = me->vectorSetup(interp, argc, argv, x0, y0, x1, y1, color);
	if(ec != TCL_OK)
		return ec;

	char *cmd = argv[0];

	// Convert to PGPLOT coords.
	me->worldToWindow(x0, y0, x0, y0);
	me->worldToWindow(x1, y1, x1, y1);

	float x[2], y[2];
	x[0] = (float)x0;
	y[0] = (float)y0;
	x[1] = x1;
	y[1] = y1;
	////////////////////////////////////////////////////////////////
	// The following check depends on the spelling of the commands.
	////////////////////////////////////////////////////////////////

	// Draw arrow or normal vector?
	Boolean drawArrow;
	if( *(cmd+4) == '\0')
		drawArrow = FALSE;
	else
		drawArrow = TRUE;
	////////////////////////////////////////////////////////////////

	me->drawLine( (double)x[0], (double)y[0], (double)x[1], (double)y[1],
		 color, drawArrow);
	return ec;
}

// Returns current position from PGPLOT lib.
void AvOverlay::getPosition(double &x, double &y)const
{float xr, yr;
	cpgqpos(&xr, &yr);
	x = (double)xr;
	y = (double)yr;
}


// Draw a line from current position to given postion. Or move to a new
// position.
// draw x y [COLOR]
// arrow x y [COLOR]
// moveto x y		Move will ignore any third argument.
int AvOverlay::lineToCmd(ClientData clientData, Tcl_Interp *interp, int argc,
			 char *argv[])
{double x1, y1;
// int ec, color;
 int color;
 AvOverlay *me = (AvOverlay *)clientData;

#if 0
	if(me == NULL)
	{	interp->result = NOOVERLAY;
		return TCL_ERROR;
	}
#else
	if(me == NULL)
	{	AvTcl::Tcl_SetResult(interp, NOOVERLAY);
		return TCL_ERROR;
	}
#endif
//	AvOverlayOptions *optns = (AvOverlayOptions *)me->options_;


	if((argc < 3) || (argc > 4))
		return tclerror(interp, NUMARGS, argc, argv);

	if(Tcl_GetDouble(interp, argv[1], &x1) != TCL_OK)
		return tclerror(interp, BADVALUE, 2, argv);
	if(Tcl_GetDouble(interp, argv[2], &y1) != TCL_OK)
		return tclerror(interp, BADVALUE, 3, argv);

	me->worldToWindow(x1, y1, x1, y1);

	// If just move, do it here and leave.
	if(strcmp(MOVETO, argv[0]) == 0)
	{	cpgmove((float)x1, (float)y1);
		return TCL_OK;
	}

	if(argc == 4)
	{	color = me->getColor(interp, argv[3]);
		if(color < 0)
			return tclerror(interp, BADCOLOR, 4, argv);
	}
	else
		color = -1;	// Set to no color seen.

//	char *cmd = argv[0];

	// Get starting position.
	float xr, yr;
	cpgqpos(&xr, &yr);

	float x[2], y[2];
	x[0] = xr;
	y[0] = yr;
	x[1] = (float)x1;
	y[1] = (float)y1;

	// Draw arrow or normal vector?
	Boolean drawArrow;
	if(strcmp(argv[0], LINETOA)== 0)
		drawArrow = TRUE;
	else
		drawArrow = FALSE;
	////////////////////////////////////////////////////////////////

	me->drawLine( (double)x[0], (double)y[0], (double)x[1], (double)y[1],
		 color, drawArrow);
	return TCL_OK;
}

// Decode marker symbol. Returns NULL or pointer to error string.
const char *AvOverlay::strToSymbol(Tcl_Interp *interp,
			     char *mstr, int &markerType)const
{
	// Get Marker type. Valid values are either an int or one of
	// the RO marker type variables.
	if(isalpha(*mstr))
	{	markerType = strToValue(MarkerTypes, mstr, -128);
		if(markerType == -128)
			return BADMARKER;
//			return tclerror(interp, BADMARKER, 4, argv);
	}
	else
	if(Tcl_GetInt(interp, mstr, &markerType) != TCL_OK)
		return BADMARKER;
// tclerror(interp, BADMARKER, 4, argv);
	return NULL;
}

// marker x y <markertype> [<color>]
// marker			// Uses current position & marker.
int AvOverlay::markerCmd(Tcl_Interp *interp, int argc, char *argv[])
{double x, y;
 int ec, color=-1;
 int markerType;

	if(argc == 1)
	{	getPosition(x, y);
		markerType = markerSymbol_;
		ec = TCL_OK;
	}
	else
	if((argc < 4) || (argc > 5))
		return tclerror(interp, NUMARGS, argc, argv);
	else
	{	// X, Y positon.
		if((ec = Tcl_GetDouble(interp, argv[1], &x) != TCL_OK))
			return tclerror(interp, BADVALUE, 2, argv);
		if((ec = Tcl_GetDouble(interp, argv[2], &y) != TCL_OK))
			return tclerror(interp, BADVALUE, 3, argv);
		// Convert to PGPLOT coords.
		worldToWindow(x, y, x, y);

		// Get Marker type. Valid values are either an int or one of
		// the RO marker type variables.
		const char *msg = strToSymbol(interp, argv[3], markerType);
			if(msg != NULL)
				return tclerror(interp, msg, 4, argv);

		if(argc == 5)
		{	color = getColor(interp, argv[4]);
			if(color < 0)
				return tclerror(interp, BADCOLOR, 5, argv);
		}
		else
			color = -1;	// Set to no color seen.
	}

	////////////////////////////////////////////////////////////////

	int oldcolor;
	if(color >= 0)
	{	cpgqci(&oldcolor);
	 	cpgsci(color);
	}
	float xm = (float)x, ym = (float)y;
	cpgpt(1, &xm, &ym, markerType);
	if(color >= 0)
		cpgsci(oldcolor);
	return ec;
}

int AvOverlay::markerCmdCB(ClientData clientData, Tcl_Interp *interp, int argc,
			   char *argv[])
{ AvOverlay *me = (AvOverlay *)clientData;

#if 0
	if(me == NULL)
	{	interp->result = NOOVERLAY;
		return TCL_ERROR;
	}
#else
	if(me == NULL)
	{	AvTcl::Tcl_SetResult(interp, NOOVERLAY);
		return TCL_ERROR;
	}
#endif
	return me->markerCmd(interp, argc, argv);
}

// Handles a number of commands that don't take arguments.
int AvOverlay::noArgsCmd(ClientData clientData, Tcl_Interp *interp, int argc,
			 char *argv[])
{
 AvOverlay *me = (AvOverlay *)clientData;

#if 0
	if(me == NULL)
	{	interp->result = NOOVERLAY;
		return TCL_ERROR;
	}
#else
	if(me == NULL)
	{	AvTcl::Tcl_SetResult(interp, NOOVERLAY);
		return TCL_ERROR;
	}
#endif

	if(argc != 1)
		return tclerror(interp, NUMARGS, argc, argv);

	if(strcmp(argv[0], SAVE) == 0)
		me->saveState();
	else
	if(strcmp(argv[0], RESTORE) == 0)
		me->restoreState();
	return TCL_OK;
}

// Handles a number commands that take only 1 argument.
#if 0
int AvOverlay::oneStringCmd(ClientData clientData, Tcl_Interp *interp,
			    int argc, char *argv[])
{ AvOverlay *me = (AvOverlay *)clientData;

	if(me == NULL)
	{	interp->result = NOOVERLAY;
		return TCL_ERROR;
	}
	AvOverlayOptions *optns = me->options_;

	if(argc != 2)
	{	interp->result = NUMARGS;
		return TCL_ERROR;
	}
	return TCL_OK;
}
#endif

// Handles a number of commands that take only 1 integer argument.
int AvOverlay::oneIntCmd(ClientData clientData, Tcl_Interp *interp, int argc,
			 char *argv[])
{  AvOverlay *me = (AvOverlay *)clientData;

#if 0
	if(me == NULL)
	{	interp->result = NOOVERLAY;
		return TCL_ERROR;
	}
#else
	if(me == NULL)
	{	AvTcl::Tcl_SetResult(interp, NOOVERLAY);
		return TCL_ERROR;
	}
#endif
//	AvOverlayOptions *optns = me->options_;

	if(argc != 2)
		return tclerror(interp, NUMARGS, argc, argv);

	const char *err = me->setVariable(interp, argv[0], argv[1]);
	if(err != NULL)
		return tclerror(interp, err, 2, argv);
	return TCL_OK;
}

// Handles commands that take only 1 real argument.
int AvOverlay::oneRealCmd(ClientData clientData, Tcl_Interp *interp, int argc,
			  char *argv[])
{ double value;
  AvOverlay *me = (AvOverlay *)clientData;

#if 0
	if(me == NULL)
	{	interp->result = NOOVERLAY;
		return TCL_ERROR;
	}
#else
	if(me == NULL)
	{	AvTcl::Tcl_SetResult(interp, NOOVERLAY);
		return TCL_ERROR;
	}
#endif

//	AvOverlayOptions *optns = me->options_;

	if(argc != 2)
		return tclerror(interp, NUMARGS, argc, argv);

	if(strcmp(argv[0], TEXTJUSTIFICATION_CMD) == 0)
	{ const char *msg = me->strToJustification(interp, argv[1], value);
		if(msg == NULL)
			me->textJustification_ = value;
		else
			return tclerror(interp, BADVALUE, 2, argv);
		return TCL_OK;
//		return NULL;
	}

	if(Tcl_GetDouble(interp, argv[1], &value) != TCL_OK)
		return tclerror(interp, BADVALUE, 2, argv);

	if(strcmp(argv[0], TEXTHEIGHT_CMD) == 0)
		cpgsch(value);
	else
	if(strcmp(argv[0], TEXTANGLE_CMD) == 0)
		me->textAngle_ = value;
	else
		cout << "Unknown command " << argv[0] << endl;

	return TCL_OK;
}

// Decode text justification [LEFT|CENTER|RIGHT|fp #]
// Only the first character of the string is checked.
// Returns error message or NULL if OK.
const char *AvOverlay::strToJustification(Tcl_Interp *interp,
				  char *str, double &just)const
{
	if(str == NULL)
		return BADNULL;
	// Justification can either be a double or the strings
	// LEFT, CENTER or RIGHT. (However, only the first letter is checked).
	int ch = *str;
	if(isalpha(ch))	// It's a letter.
	{	switch(ch) {
		case 'L':
		case 'l':
			just = 0.0;
			break;
		case 'C':
		case 'c':
			just = 0.5;
			break;
		case 'R':
		case 'r':
			just = 1.0;
			break;
		default:
			return BADLABEL;
		}
	}
	else
	if(Tcl_GetDouble(interp, str, &just) != TCL_OK)
		return BADVALUE;
	return NULL;
}

// Convert to string. If always is FALSE, only returns the strings:
// LEFT, CENTER, RIGHT for 0, 0.5, 1.0. Otherwise, NULL. If always
// is TRUE, other values are converted to a string.
const char *AvOverlay::justificationToStr(const double just,
					  const Boolean always)const
{
	if(just == 0.0)
		return Justifications[LEFT];
	else
	if(just == 0.5)
		return Justifications[CENTER];
	else
	if(just == 1.0)
		return Justifications[RIGHT];
	else
	if(always)
	{ static char buf[64];
		sprintf(buf, "%f", just);
		return buf;
	}
	return NULL;
}

// Draws text.
// text x y <angle_degrees> [LEFT | CENTER | RIGHT | fp_num] text [..text]
// textat text [..text]
int AvOverlay::textCmd(ClientData clientData, Tcl_Interp *interp, int argc,
			 char *argv[])
{double x, y, angle=0.0, fjust = 0.5;
 AvOverlay *me = (AvOverlay *)clientData;

#if 0
	if(me == NULL)
	{	interp->result = NOOVERLAY;
		return TCL_ERROR;
	}
#else
	if(me == NULL)
	{	AvTcl::Tcl_SetResult(interp, NOOVERLAY);
		return TCL_ERROR;
	}
#endif

//	AvOverlayOptions *optns = me->options_;

	Boolean textat = FALSE;
	if(strcmp(argv[0], TEXTAT) == 0)
		textat = TRUE;

	if(((argc < 6) && !textat) || (argc <= 1))
		return tclerror(interp, NUMARGS, argc, argv);

	if(!textat)
	{	if(Tcl_GetDouble(interp, argv[1], &x) != TCL_OK)
			return tclerror(interp, BADVALUE, 2, argv);
		if(Tcl_GetDouble(interp, argv[2], &y) != TCL_OK)
			return tclerror(interp, BADVALUE, 3, argv);
		if(Tcl_GetDouble(interp, argv[3], &angle) != TCL_OK)
			return tclerror(interp, BADVALUE, 4, argv);
		const char *msg = me->strToJustification(interp, argv[4],
							 fjust);
		if(msg != NULL)
			return tclerror(interp, msg, 5, argv);
		// Convert to PGPLOT coords.
		me->worldToWindow(x, y, x, y);
		argc -= 5;
		argv += 5;
	}
	else
	{ float xr, yr;
		cpgqpos(&xr, &yr);
		x = (double)xr;
		y = (double)yr;
		angle = me->textAngle_;
		fjust = me->textJustification_;
		argc -= 1;
		argv += 1;
	}

	if(argc == 0)	// If just simple string.
		cpgptxt(x, y, angle, fjust, argv[0]);
	else		// Concatenate the rest of the args.
	{AvString t = *argv;
		while(argc-- > 0)
			t += *(++argv);
		cpgptxt(x, y, angle, fjust, t.chars());
//cout << t << state() << endl;
	}
	return TCL_OK;
}

// Sets user's world window.
// window xmin ymin xmax ymax
//	     or
// window [INDEX WORLD NDC]
int AvOverlay::windowCmd(Tcl_Interp *interp, int argc, char *argv[])
{//AvOverlayOptions *optns = options_;

	if(argc == 2)	// window [ <type_name> ]
	{	if(strcasecmp(argv[1], "NDC") == 0)
			state()->window(NDC, 0.0, 0.0, 1.0, 1.0);
		else
		if(strcasecmp(argv[1], "DATASET") == 0)
			state()->type(DATASET);
		else
		if(strcasecmp(argv[1], "INDEX") == 0)
			state()->window(INDEX, 0.0, 0.0,
					width()-1.0, height()-1.0);
		else
		if(strcasecmp(argv[1], "INDEX1") == 0)
			state()->window(INDEX1, 1.0, 1.0,
					width(), height());
		else
			return tclerror(interp, BADWINDOW, 2, argv);
		return TCL_OK;
	}

	if(argc != 5)
		return tclerror(interp, NUMARGS, argc, argv);

	// window xmin ymin xmax ymax
	double xmin, ymin, xmax, ymax;
	if(Tcl_GetDouble(interp, argv[1], &xmin) != TCL_OK)
		return tclerror(interp, BADVALUE, 2, argv);
	if(Tcl_GetDouble(interp, argv[2], &ymin) != TCL_OK)
		return tclerror(interp, BADVALUE, 3, argv);
	if(Tcl_GetDouble(interp, argv[3], &xmax) != TCL_OK)
		return tclerror(interp, BADVALUE, 4, argv);
	if(Tcl_GetDouble(interp, argv[4], &ymax) != TCL_OK)
		return tclerror(interp, BADVALUE, 5, argv);

	state()->window(USER, xmin, ymin, xmax, ymax);
	return TCL_OK;
}

int AvOverlay::windowCmdCB(ClientData clientData, Tcl_Interp *interp, int argc,
			   char *argv[])
{ AvOverlay *me = (AvOverlay *)clientData;

#if 0
	if(me == NULL)
	{	interp->result = NOOVERLAY;
		return TCL_ERROR;
	}
#else
	if(me == NULL)
	{	AvTcl::Tcl_SetResult(interp, NOOVERLAY);
		return TCL_ERROR;
	}
#endif
	return me->windowCmd(interp, argc, argv);
}

int AvOverlay::dsToWorldCB(ClientData clientData, Tcl_Interp *interp, int argc,
			   char *argv[])
{ AvOverlay *me = (AvOverlay *)clientData;

	if(argc != 5)
		return tclerror(interp, NUMARGS, argc, argv);

#if 0
	if(me == NULL)
	{	interp->result = NOOVERLAY;
		return TCL_ERROR;
	}
#else
	if(me == NULL)
	{	AvTcl::Tcl_SetResult(interp, NOOVERLAY);
		return TCL_ERROR;
	}
#endif
	double dx, dy, wx, wy;
	if(Tcl_GetDouble(interp, argv[1], &dx) != TCL_OK)
		return tclerror(interp, BADVALUE, 2, argv);
	if(Tcl_GetDouble(interp, argv[2], &dy) != TCL_OK)
		return tclerror(interp, BADVALUE, 3, argv);

	me->datasetToWorld(dx, dy, wx, wy);
	AvTcl *tcl = me->tcl_;
	tcl->setDoubleVal(argv[3], wx);
	tcl->setDoubleVal(argv[4], wy);
	return TCL_OK;
}


// From data set to user's current world.
void AvOverlay::datasetToWorld(const double dx, const double dy,
			      double &wx, double &wy)
{
	if(state()->type() == DATASET)
	{	wx = dx;
		wy = dy;
		return;
	}
	// First, convert from dataset to index.
	world_(0) = dx; world_(1) = dy;
	accessor()->WorldToXYZ(world_, ijk_);
	double ix, iy;
	ix = (double)ijk_(0);
	iy = (double)ijk_(1);
	indexToWorld(ix, iy, wx, wy);
}


int AvOverlay::pixelToWorldFuncCB(ClientData cd, Tcl_Interp *interp,
			        Tcl_Value *args, Tcl_Value *resultPtr)
{ AvOverlay *ovr = (AvOverlay *)cd;

	return ovr->pixelToWorldFunc(interp, args, resultPtr);
}

/* Convert an X/Y position index to world value for an axis.
args	0	double	x
	1	double	y
	2	int	axis (0 if should return X, 1 for Y)
result		double	axis value in world coords.
*/

int AvOverlay::pixelToWorldFunc(Tcl_Interp *interp,
			        Tcl_Value *args, Tcl_Value *resultPtr)
{double x = args[0].doubleValue;
 double y = args[1].doubleValue;
 int axis = (int)args[2].intValue;
 double value = 0.0;
 int ec;
 int ix = (int) x, iy = (int)y;

	if( options_->accessor()->XYZToWorld(ix, iy, axis, value))
	{	resultPtr->doubleValue = value;
		resultPtr->type = TCL_DOUBLE;
		ec = TCL_OK;
	}
	else
	{	//interp->result = "XYZToWorld failed";
		AvTcl::Tcl_SetResult(interp, "XYZToWorld failed");
		ec = TCL_ERROR;
	}
	return ec;
}

/* Return the value of the data set located at the given coords.
args	0	double	x
	1	double	y
result		double	datum at that point.
Result is undefined if point is outside of image region.
*/

int AvOverlay::datumFunc(Tcl_Interp *,
			 Tcl_Value *args, Tcl_Value *resultPtr)
{double x = args[0].doubleValue;
 double y = args[1].doubleValue;
 double value;
 double ix, iy;

	worldToIndex(x, y, ix, iy);
	value = (double) options_->accessor()->getDatum((int)ix, (int)iy);
	{	resultPtr->doubleValue = value;
		resultPtr->type = TCL_DOUBLE;
	}
	return TCL_OK;
}

int AvOverlay::datumFuncCB(ClientData cd, Tcl_Interp *interp,
			   Tcl_Value *args, Tcl_Value *resultPtr)
{ AvOverlay *ovr = (AvOverlay *)cd;

	return ovr->datumFunc(interp, args, resultPtr);
}

////////////////////////////////////////////////////////////////
// Set PGPLOT variable 'name' with 'value'.
// Returns NULL for OK, else an error string.
// Called by variable and cmd handlers.
const char *AvOverlay::setVariable(Tcl_Interp *interp, const char *name,
				   char *str)
{Boolean haveValue = FALSE;
 int value;
	
	// See if set to numeric value. + is OK, but no variables take
	// minus values.
	if(isdigit(*str) || (*str == '+') || (*str == '-'))
	{	if(Tcl_GetInt(interp, str, &value)!=TCL_OK)
			return BADVALUE;
		else
			haveValue = TRUE;
	}
	////////////////////////////////////////////////
	// Do lookups rather than evaluating the variable
	// to catch "set Color foo".
	// Ignore case to catch both "Color" and "color". (Variable & cmd).
	if(strcasecmp(name, COLOR)==0)
	{	if(!haveValue)
			value = strToColor(str);
		if((value < 0)||(value > MAXCOLOR))
			return BADCOLOR;
		cpgsci(value);
	}
	else
	if(strcasecmp(name, FONT)==0)
	{	if(!haveValue)
			value = strToValue(Fonts, str);
		if((value <= SIMPLEFONT)||(value > MAXFONT))
			return BADFONT;
		cpgscf(value);
	}
	else
	if(strcasecmp(name, LINESTYLE)==0)
	{	if(!haveValue)
			value = strToValue(LineStyles, str);
		if((value < SOLIDLINE)||(value> MAXLINESTYLE))
			return BADLINESTYLE;
		cpgsls(value);
	}
	else
	if(strcasecmp(name, LINEWIDTH)==0)
	{	if(!haveValue || (value<= 0) || (value >201))
			return BADLINEWIDTH;
		cpgslw(value);
	}
	else
	if(strcasecmp(name, TEXTBG)==0)
	{	if(!haveValue)
		{	if(strcasecmp("TRANSPARENT", str)==0)
				value = -1;
			else
			{	if((value = strToColor(str))<0)
					return BADCOLOR;
			}
		}
		if(value > MAXCOLOR)
			return BADCOLOR;
		cpgstbg(value);
	}
	else
	if(strcasecmp(name, SYMBOL)==0)
	{ int symb;
		const char *msg = strToSymbol(interp, str, symb);
		if(msg != NULL)
			return msg;
		markerSymbol_ = symb;
	}

	return NULL;
}

// Called when user accesses the "Color", "LineStyle", etc. variables.
// Handles mostly PGPLOT but also some internal variables.
// Returns NULL or error string.
char *AvOverlay::intVarCB(ClientData cd , Tcl_Interp *interp,
			  char *name1, char *name2, int flags)
{ AvOverlay *ovr = (AvOverlay *)cd;

	if(flags & TCL_TRACE_READS)	// Read PGPLOT value.
	{ int value;
	  const char *str = NULL;
		if(strcasecmp(name1, COLOR)==0)
		{	cpgqci(&value);
			str = colorToStr(value);
		}
		else
		if(strcasecmp(name1, FONT)==0)
		{	cpgqcf(&value);
			str = valueToStr(Fonts, value);
		}
		else
		if(strcasecmp(name1, LINESTYLE)==0)
		{	cpgqls(&value);
			str = valueToStr(LineStyles, value);
		}
		else
		if(strcasecmp(name1, LINEWIDTH)==0)
			cpgqlw(&value);
		else
		if(strcasecmp(name1, TEXTBG)==0)
		{	cpgqtbg(&value);
			if(value < 0)
				str = "TRANSPARENT";
			else
				str = colorToStr(value);
		}
		else
		if(strcasecmp(name1, SYMBOL)==0)
		{	value = ovr->markerSymbol_;
			str = valueToStr(MarkerTypes, value);
		}
		if(str != NULL)
			AvTcl::Tcl_SetVar2(interp, name1, name2, str,
					   flags & TCL_GLOBAL_ONLY);
		else
			ovr->tcl_->setIntVal2(name1, name2, value,
					      flags & TCL_GLOBAL_ONLY);
		return NULL;
	}
	else	// Set PGPLOT value
	{  char *str = Tcl_GetVar2(interp, name1, name2,
				    flags & TCL_GLOBAL_ONLY);
		static char errorString[128];
		const char *rtn = ovr->setVariable(interp, name1, str);
		if(rtn == NULL)
			return NULL;
		else
		{	strcpy(errorString, rtn);
			return errorString;
		}
	}
}

// Called when user accesses a TCL/PGPLOT variable that contains a float/dbl.
static char UNKNOWNVARIABLE[] = "Internal Error: unknown TCL variable.";

char *AvOverlay::doubleVarCB(ClientData cd , Tcl_Interp *interp,
			     char *name1, char *name2, int flags)
{ AvOverlay *ovr = (AvOverlay *)cd;

	if(flags & TCL_TRACE_READS)	// Read PGPLOT value.
	{float fvalue;
	 double dvalue;
	 const char *str = NULL;

		if(strcasecmp(name1, TEXTHEIGHT)==0)
		{	cpgqch(&fvalue);
			dvalue = (double)fvalue;
		}
		else
		if(strcasecmp(name1, TEXTANGLE)==0)
			dvalue = ovr->textAngle_;
		else
		if(strcasecmp(name1, TEXTJUSTIFICATION)==0)
		{	dvalue = ovr->textJustification_;
			str = ovr->justificationToStr(dvalue);
		}
		else
			return UNKNOWNVARIABLE;

		if(str != NULL)
			AvTcl::Tcl_SetVar2(interp, name1, name2, str,
					   flags & TCL_GLOBAL_ONLY);
		else
			ovr->tcl_->setDoubleVal2(name1, name2, dvalue,
					 flags & TCL_GLOBAL_ONLY);
	}
	else	// Set PGPLOT value
	{double value;
	 char *str = Tcl_GetVar2(interp, name1, name2,
				    flags & TCL_GLOBAL_ONLY);

		if(str != NULL)
		{	// Get numeric value.
			if(Tcl_GetDouble(interp, str, &value)!=TCL_OK)
				return NULL;

			if(strcmp(name1, TEXTHEIGHT)==0)
				cpgsch((float)value);
			else
			if(strcmp(name1, TEXTANGLE)==0)
				ovr->textAngle_ = value;
			else
			if(strcmp(name1, TEXTJUSTIFICATION)==0)
				ovr->textJustification_ = value;
			else
				return UNKNOWNVARIABLE;
		}
	}
	return NULL;
}

// This routine is called if the user attempts to write to a read only
// variable. It resets the variable's value and sets an error message.
// Taken from "Tcl and the Tk Toolkit" pg334.
char *AvOverlay::readOnlyVarCB( ClientData cd , Tcl_Interp *interp,
				char *name1, char *name2, int flags)
{ char *value = (char *)cd;

	Tcl_SetVar2(interp, name1, name2, value, flags & TCL_GLOBAL_ONLY);
	return READONLYVAR;
}
////////////////////////////////////////////////////////////////

// States[0] is always the current one. Save/restores go to upper levels.
// Valid values of stateIndex_ are (1..MAXSTATES).

int AvOverlay::saveState()
{
	if( stateIndex_ <= MAXSTATES)
	{	states_[stateIndex_++] = states_[0];
		cpgsave();
	}
	else
		return tclerror(tcl_->interp(), STATEOVERFLOW);
	return TCL_OK;
}

int AvOverlay::restoreState()
{
	stateIndex_ -= 1;
	if( stateIndex_ > 0)
	{	states_[0] = states_[stateIndex_];
		cpgunsa();
	}
	else
	{	stateIndex_ = 1;
		return tclerror(tcl_->interp(), STATEUNDERFLOW);
	}
	return TCL_OK;
}

// Convert between user coords and index. (0..<len-1>).
void AvOverlay::worldToIndex(const double wx, const double wy,
			     double &ix, double &iy)
{
	switch (windowType()) {
	case INDEX:
		ix = wx;
		iy = wy;
		break;
	case INDEX1:
		ix = wx-1.0;
		iy = wy-1.0;
		break;
	case NDC:
	case USER:
	     {double xmin = worldXMin();
	      double ymin = worldYMin();
		ix = ((wx-xmin)/(worldXMax()-xmin))*width();
		iy = ((wy-ymin)/(worldYMax()-ymin))*height();
	     }
		break;
	case DATASET:
		{ world_(0) = wx; world_(1) = wy;
			accessor()->WorldToXYZ(world_, ijk_);
			ix = (double)ijk_(0);
			iy = (double)ijk_(1);
		}
		break;
	}
}

void AvOverlay::indexToWorld(const double ix, const double iy,
			     double &wx, double &wy)
{
	switch (windowType()) {
	case INDEX:
		wx = ix;
		wy = iy;
		break;
	case INDEX1:
		wx = ix+1.0;
		wy = iy+1.0;
		break;
	case NDC:
	case USER:
	     {//double xmin = worldXMin();
	      //double ymin = worldYMin();
		wx = (ix/(width()-1))*worldXMax();
		wy = (iy/(height()-1))*worldYMax();
	     }
		break;
	case DATASET:
		{ ijk_(0) = (int)ix; ijk_(1) = (int)iy;
			// Convert from index to dataset for our subrange.
			accessor()->XYZToWorld(ijk_, world_, TRUE);
			wx = world_(0);
			wy = world_(1);
		}
		break;
	}
}

// Convert from user's window coords to what we're using for PGPLOT.
// If margins are off (=0), the result is in the range (0..1)(NDC),
// otherwise the result is mapped onto the PGPLOT viewport.
// ScaleOnly is used to prevent adding in the viewport offsets if the
// result is a length rather than a position.
void AvOverlay::worldToWindow(const double wx, const double wy,
			      double &winx, double &winy,
			      const Boolean scaleOnly)
{
	switch (windowType()) {
	case NDC:
		winx = wx;
		winy = wy;
		break;
	case INDEX:
	case INDEX1:
	case USER:
		winx = state()->XToNDC(wx);
		winy = state()->YToNDC(wy);
		break;
	case DATASET:
		// Use accessor to convert to IJK coords, then scale.
		{ world_(0) = wx; world_(1) = wy;
			accessor()->WorldToXYZ(world_, ijk_);
			// X, Y are in range (0..<max_axis_index -1>).
			winx = (double)ijk_(0)/(width()-1.0);
			winy = (double)ijk_(1)/(height()-1.0);
		}
		break;
	}

	// Convert 0..1 to be within the raster image. Mapping is a NOP
	// margins are off.
	winx = winx*vpxrange_;
	winy = winy*vpyrange_;
	if(!scaleOnly)
	{	winx += vpxmin_;
		winy += vpymin_;
	}
}

// Set margins... The offset from the canvas viewport where the image is.
void AvOverlay::setMargins(const int lm, const int rm,
			   const int tm, const int bm)
{

	lm_ = lm;
	rm_ = rm;
	tm_ = tm;
	bm_ = bm;
	touch();
}

// Generate an error message and return TCL_ERROR.
int AvOverlay::tclerror(Tcl_Interp *interp,
			const char *Msg, const int argc, char *argv[])
{
	if(Msg != NULL)
	{	char *msg = AvTcl::Strdup(Msg);
		Tcl_SetResult(interp, msg, TCL_VOLATILE);
		Tcl_AppendResult(interp, "\n", NULL);
		delete [] msg;
	}
	for(int i=0; i< argc; i++)
		Tcl_AppendResult(interp, argv[i], " ", NULL);

	return TCL_ERROR;
}

// Read a color value. Returns TCL_OK or TCL_ERROR.
int AvOverlay::getColor(Tcl_Interp *interp, char *clrstr)const
{
	// Use color specified in marker call if there was one and
	// choice wasn't overridden.
	int colorChoice = options_->colorChoice_.getValue();
	if(colorChoice == AvOverlayOptions::OVERRIDEALL)
		return colorChoice;

	// See if set to numeric value.
	if(isdigit(*clrstr) || (*clrstr == '+'))
	{int value;
		if(Tcl_GetInt(interp, clrstr, &value)!=TCL_OK)
			return -1;
		// Do a range check.
		if((value >= 0) && (value <= MAXCOLOR))
			return value;
		return -1;
	}

	// See if string matches known list.
	return strToColor(clrstr);
}

// Get a list of doubles from argv. Returns the number of
// successful decodes. Decodes v.nelements() values.
int AvOverlay::getDoubles(Tcl_Interp *interp, char *argv[], AvWPosition &v)
{int num = v.nelements(), n;
 double value;

	for(n = 0; n < num; n++)
	{	if(Tcl_GetDouble(interp, argv[n], &value)!=TCL_OK)
			return n;
		v(n) = value;
	}
	return n;
}


////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//		grstate

AvOverlay::grstate::grstate()
{
	window(AvOverlay::NDC, 0.0, 0.0, 1.0, 1.0);
}

AvOverlay::grstate::~grstate() {}

void AvOverlay::grstate::operator=(const grstate &s)
{
	type_ = s.type_;
	minx_ = s.minx_;
	miny_ = s.miny_;
	maxx_ = s.maxx_;
	maxy_ = s.maxy_;
}

void AvOverlay::grstate::window(const WINDOWTYPE t,
			      const double minx, const double miny,
			      const double maxx, const double maxy)
{
	type_ = t;
	minx_ = minx;
	miny_ = miny;
	maxx_ = maxx;
	maxy_ = maxy;
}

const char *AvOverlay::grstate::typeString()const
{
	return typeString(type());
}

const char *AvOverlay::grstate::typeString(const WINDOWTYPE t)const
{
	switch(t) {
	case AvOverlay::NDC:
		return "NDC";
	case AvOverlay::INDEX:
		return "INDEX";
	case AvOverlay::INDEX1:
		return "INDEX1";
	case AvOverlay::DATASET:
		return "DATASET";
	case AvOverlay::USER:
		return "USER";
	default:
		return "Unknown";
	}
}

ostream &operator<<(ostream &os, const AvOverlay::grstate &s)
{
	os << "[ ";
//	os << hex << &s << " " << dec;
	os << s.typeString();

	if(s.type_ != AvOverlay::DATASET)
		os	<< " " << s.minx_ << " " << s.miny_
			<< " " << s.maxx_ << " " << s.maxy_;
	os <<  " ]";
	return os;
}

////////////////////////////////////////////////////////////////
#endif
/* TCL */
