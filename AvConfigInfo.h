//# Copyright (C) 1995-2000 Board of Trustees of the University of Illinois
//#
//# This software, both binary and source, is copyrighted by The
//# Board of Trustees of the University of Illinois. Ownership
//# remains with the University. You should have received a copy 
//# of a licensing agreement with this software. See the file 
//# "AIPSVIEW_COPYRIGHT", or contact the University at this address:
//#
//#     The NCSA AipsView Visualization System
//#     National Center for Supercomputing Applications
//#     University of Illinois
//#     405 North Mathews Ave.
//#     Urbana, IL 61801
//#     aipsview@ncsa.uiuc.edu
//# --------------------------------------------------
//
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvConfigInfo.h,v 19.0 2003/07/16 05:47:57 aips2adm Exp $
//
// $Log: AvConfigInfo.h,v $
// Revision 19.0  2003/07/16 05:47:57  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:32  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:43:01  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:07  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:31  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 17:01:42  hravlin
// Changed some char * to const char *.
//
// Revision 14.0  2000/03/23 16:09:03  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:53  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:13  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:02:00  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:56:12  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1997/12/17 21:50:24  hr
// Changes to remove warnings from MIPSpro.7x compiler.
//
// Revision 9.0  1997/08/25 21:31:19  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.1  1997/02/25 16:40:52  hr
// Added setIntList() for AvIPositions.
//
// Revision 8.0  1997/02/20 03:19:14  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1997/01/24 20:19:02  hr
// Changed set<xxx>() routines to accept an optional argument to allow writing
// value into the defaults database.
//
// Revision 7.1  1996/12/12 10:02:32  droberts
// *** empty log message ***
//
// Revision 1.2  1996/11/05 22:09:59  hr
// IRIX compiler can't handle two functions of the same name one with an
// int arg and one with char. (getBoolean()).
//
// Revision 1.1  1996/11/05  21:11:19  hr
// Initial revision
//
//---------------------------------------------------------------------------

/* AvConfigInfo.h


Provides a mechanism for retrieving, setting and printing
configuration/default variables.

Routines are provided to get/set integers, doubles, booleans, strings,
lists of integers and lists of doubles. There are also routines to
translate between names (character strings) and integers.  eg. A color
name, say "RED", could be read in and converted to a number, say, 4.

Externally, options are stored in a global defaults file for options that
apply throughout aipsview and/or file specific configuration files.

If a file specific option does not exist, the global value is used. If the
global value isn't available, a value provided by the programmer is
returned.

data are stored in pseudo two dimensional TCL arrays:

		file(module,variable) value

file		Data file name w/o path or extension. eg. "smallcube".
module		aipsview module name (eg. contour)
variable	variable within the module (eg. lineColor)

For defaults, the file name is defaultVar() (aipsview).

Note that there are no spaces between the parentheses.

Each module should supply its own script for initializing its default
values. This is needed so aipsview can write out an initial defaults
file. It should look something like:

static char *defaultScript = \
	set aipsview(contour,lineColor) PINK\n\
	# Comments will be stripped out by the interpreter.\n\
	set aipsview(contour,lineColor) mixed\n\
	set aipsview(contour,showContours) TRUE\n\
	set aipsview(contour,showLabels) TRUE\n\
	set aipsview(contour,showTitle) TRUE\n\
	set aipsview(contour,enableBlanking) TRUE\n\
	set aipsview(contour,immediateUpdate) FALSE\n\
	set aipsview(contour,multiplier) 1.0\n\
	set aipsview(contour,colorStyles) \\\n\
			{ WHITE RED GREEN BLUE CYAN MAGENTA YELLOW }\n\
	set aipsview(contour,nlevels) 4\n\
	set aipsview(contour,values) { 1.0 2.0 3.0\\\n\
					    4.0 5.0 6.0 }\n\
	";

	obj->loadDefaults(defaultScript);

This needs to be done only once since the script is loaded into a global
database. However, each time an AvConfigInfo object is created, any file
specific options file will be reread. Note that default values set by
scripts provided within aipsview are kept separate from scripts loaded from
external files. Both are separate from values set by the user (indirectly
via the set value functions). It is therefore possible to reread scripts
without overwriting previous changes.
[Note: Default scripts are not strictly necessary since calling a get<xxx>
routine will write the deault to the database if the variable doesn't exist.]

The global configuration file, ~/.aipsviewrc", might have entries like:

set aipsview(contour,colorStyles) { WHITE RED GREEN BLUE CYAN YELLOW }
set aipsview(contour,levels) { .1 .3 .5 .7 .9 }
set aipsview(contour,immediateUpdate) FALSE

A user supplied file for a specific data set would replace "aipsview" with
the name of the data set filename (w/o extension) with "rc" appended. eg.

	For $HOME/data/smallcube.fits, the configuration filename would be
"smallcuberc". Aipsview first looks for the file in the current directory.
If that doesn't exist, it prepends the data file's path and tries again.

set smallcube(contour,immediateUpdate) TRUE

Note: the name of the array, not the file, differentiates values from each
other. For instance, "~/.aipsviewrc" could have the line:

	set smallcube(contour,immediateUpdate) TRUE

There is one database, which consists of 3 TCL interpreters, for all of
aipsview. AvConfigInfo objects serve primarily to hold the file and module
strings to help build the tcl variable names. Since no variable information
is stored in a ConfigInfo object, it would be alright to create multiple
objects with the same file and modules. However, the RC file for the data
set would be executed each time an object was created.

When the user changes an option value (say, via a toggle button), the
new value should be set in the Tcl interpreter. eg.:

	configInfo->setDouble("multiplier", 4.0);

Note: It is almost never correct to change the "default" values since these
are global for all data files. Most options should be changed on a per
datafile basis. (Unless they truly are global).


The default "default" file is "~/.aipsviewrc", but this may be overridden
by the environment variable: "AIPSVIEWRC".

			Example of using AvConfigInfo
			 (From AvContourInfo.cc)

// Table to convert between color names and their integer values.
AvConfigInfo::IntMap colorStyles[] = {
	{ AvContourInfo::WHITE, "WHITE"},
	{ AvContourInfo::RED, "RED"},
	{ AvContourInfo::MIXED, "MIXED"},
	{0, NULL}
	};

	AvConfigInfo *c = new AvConfigInfo(filename, "contour", defaultScript);
	c->ref();

	// Get a boolean value.
	c->getBoolean("drawContours", drawContours_, TRUE);
	// get a double.
	c->getDouble("levelScale", levelScale_, 1.0);// Level multiplier.
	// get a "Mapped Int". Here turn a color name into an int.
	c->getMappedInt("colorStyle", color, colorStyles, MIXED);
	AvWPosition levels(0);
	c->getDoubleList("levels", levels);

Note that most of the above calls provide a default to use in case there
was none set. While this may seem redundant since the value should have
been set int the default script, it isn't. If aipsview is built without the
TCL library, ie. "TCL" is not defined, there is no configuration
mechanism. All the variable retrieval routines return 0 if there was no
variable found. Programmers should either supply a default or check the
return value. If a call to a get<xxx> routine would return the default
value, the default value is written into the default interpreter. Therefore,
it is not strictly necessary to use a default script.


When the user changes a configuration value, for instance by pressing a toggle,
the corresponding variable should be set via the appropriate call.

eg:

void AvContourInfo::colorStyle(const COLORSTYLE style)
{
	if(style != colorStyle_)
	{	colorStyle_ = style;
		cnfg_->setMappedInt("colorStyle", style, colorStyles);
		touch();
	}
}

*/

#ifndef AvConfigInfo_h
#define AvConfigInfo_h
#include <AvResource.h>
#include <fstream.h>
#ifdef TCL
#include <AvTcl.h>
#endif
#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif

#include <AvString.h>
#include <AvIPosition.h>
#include <AvWPosition.h>

class AvConfigInfo : public AvResource {
  public:
	// filename is the name of the data file with path.
	// moduleName is the name of the aipsview module for which information
	// will be retrieved (eg. "contour").
	// defaultScript is a Tcl script providing defaults for the variables
	// to be retrieved.
	AvConfigInfo(const char *filename, const char *moduleName,
		     const char *defaultScript=NULL);
	// Load some more defaults. Returns 1 for OK, else 0.
	int loadDefaults(const char *script);
	// s1..s9 are considered 1 script.
	// eg. loadVarDefaults("set ", var, " ", value);
	// Returns 1 for OK, else 0;
	int loadVarDefaults(const char *s1, const char *s2 = NULL,
			     const char *s3=NULL, const char *s4 = NULL,
			     const char *s5=NULL, const char *s6 = NULL,
			     const char *s7=NULL, const char *s8 = NULL,
			     const char *s9=NULL);
	// Table to convert between ints and strings.
	// map[0] is used if no match was found. The last entry must be
	// {0,NULL}.
	typedef struct { int value; const char * name;} IntMap;
	////////////////////////////////////////////////////////////////
	// Routines to retrieve variables. Returns 1 if the variable was
	// found, else 0.
	// If the variable isn't in the database, it is written into it
	// using the default value.
	int getInt(const char *variable, int &value, const int def=0)const;
	// Read in a variable (as a string) and convert to an int.
	int getMappedInt(const char *variable, int &value,
			 const IntMap *)const;
	// Same as above, but use def if no match.
	int getMappedInt(const char *variable, int &value,
			 const IntMap *, const int def)const;
	int getDouble(const char *variablename, double &value,
		      const double def=0.0)const;
	int getBoolean(const char *variable, int &value,
		       const int def=FALSE)const;
#if 0
	int getBoolean(const char *variable, char &value,
		       const char def = FALSE)const;
#endif
	int getString(const char *variable, AvString &value)const;
	int getString(const char *variable, AvString &value,
		      const AvString &def)const;
	int getDoubleList(const char *variable, AvWPosition &value)const;
	int getDoubleList(const char *variable, AvWPosition &value,
			  const AvWPosition &def)const;
	int getIntList(const char *variable, AvIPosition &value)const;
	int getIntList(const char *variable, AvIPosition &value,
		       const AvIPosition &def)const;
	int getMappedIntList(const char *variable, AvIPosition &value,
			     const IntMap *map)const;
	////////////////////////////////////////////////////////////////
	// Routines to set variables.
	// Below, if 'def' is 1, the value is written as a 'default'.
	// This should only be done during initializations. Usually,
	// defaults are set using filename 'aipsview', however if the
	// set routines are used, the filename will be that of the datafile.
	void setInt(const char *var, const int value, const int def=0);
	// Write out an int as a specified string.
	void setMappedInt(const char *var, const int value, const IntMap *,
			  const int def=0);
	void setHex(const char *var, const int value, const int def=0);
	void setDouble( const char *var, const double value,
			const int def=0);
	void setBoolean(const char *var, const int value,
			const int def=0);
	void setString( const char *var, AvString &value,
			const int def=0);
	void setString( const char *var, const char *value,
			const int def=0);
	// Set var to the list of "value"s.
	void setDoubleList(const char *var, const AvWPosition &value,
			   const int def=0);
	void setDoubleList(const char *var,
		      	   const int length, const double *values,
			   const int def=0);
	void setIntList(const char *var, const int length, const int *values,
			const int def=0);
	void setIntList(const char *var, const AvIPosition &list,
			const int def=0);
	void setMappedIntList(const char *variable, const AvIPosition &value,
			      const IntMap *map, const int def=0);

  public:
	////////////////////////////////////////////////////////////////
	// Less used functions.
	// Name of "default" file. (Nominally "~/.aipsviewrc").
	static const char *defaultName();
	static const char *defaultVar();	// Name used as default var.
	// Given a dataset's filename (with path), return the name
	// of the rc file (with or without path).
	// An RC name is the filename with any extension removed and "rc"
	// appended.
	static AvString getRCName(const char *dataFilename,
				  const int withPath=0,
				  const char *suffix="rc");

	// Convert a list of mapped ints into a TCL style list.
	// (  { <elem1> ... <elemn> } ).
	static AvString buildMappedIntList(const AvIPosition &list,
					   const IntMap *map);

	// Routines to convert between a string and an int.
	static const char *intToString(const int val, const IntMap *);

	static int stringToInt(const char *name, int &value, const IntMap *);

	// Expand a pathname removing $HOME or "~/".
	static AvString expandPath(const AvString &path);
	static AvString expandPath(const char *path);

	// Dump the current option settings to ofile. If all is TRUE, then
	//all options for the dataFile are written. Otherwise, only those
	//that have been changed. If dataFile = defaultName() or NULL, the
	// variable name is "aipsview" otherwise, it is made from the data
	// file name.
	static void writeOptions(const char *ofile, const char *dataFile,
				 const int all=0);
	// Module name
	const char *moduleName()const{return module_.chars();}
#if defined(TCL)
	static AvTcl &defaultInterp(){return def_;}
	static AvTcl &rcInterp(){return rc_;}
	static AvTcl &setInterp(){return set_;}
#endif
  protected:
	~AvConfigInfo();		// Use ref/unref.
	// These are protected only because they are unlikely to be needed
	// externally.

	// Create a tcl index from the variable name and the internal
	// file and module names.
	// If def is TRUE use "aipsview" otherwise use the filename.
	AvString makeIndex(const char *var, const int def=0)const;
	static int fileExists(const char *filename);
	static int fileExists(const AvString &filename);

  private:
	static void initialize();
	// Names in here aren't updated when save is called.
	// (Could be public).
	static void forget(const char *name);
	static void writeScript(fstream &out, AvString &script);
	static void writeLine(fstream &out, AvString &line);
	static void writeVars(fstream &out);
	// Copy variables to fileName. If fileName already exists,
	// a backup is made and copied to the new version.
	static void saveTo(const char *fileName, const char *rcFile);
#ifdef TCL
	static void writeVar(fstream &out, Tcl_HashEntry *);
	// Save a variable's value for later writing along with a pointer to
	// the interpreter where is was defined.
	static void rememberVar(const char *varName, AvTcl &);
	// Save a array elements for later writing.
	static void rememberArray(const char *varName, AvTcl &tcl);
	// Remember all the variable for an interpreter.
	static void rememberAll(AvTcl &interp, const char *fileName,
				const int all);
	// 'Forget' all of interp's vars.
	static void forgetAll( AvTcl &interp);
	static AvTcl def_;		// Defaults by aipsview.
	static AvTcl rc_;		// Defaults from rc file.
	static AvTcl set_;		// Defaults set by user
	static AvTclHashTable	forget_;// Table of variable names to forget.
	static AvTclHashTable	remember_;// Table of variable names to dump.
#endif
	AvString	fileVar_;	// Name of data file without path
					// or extension. This is used as the
					// file variable.
	AvString	module_;	// Module name;
};

#endif
