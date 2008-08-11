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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvConfigInfo.cc,v 19.0 2003/07/16 05:47:22 aips2adm Exp $
//
// $Log: AvConfigInfo.cc,v $
// Revision 19.0  2003/07/16 05:47:22  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:03  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:32  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:39  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:50  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 17:01:17  hravlin
// Update copyright.
//
// Revision 14.0  2000/03/23 16:08:26  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.2  1999/09/24 21:13:01  hravlin
// Removed leading and trailing spaces when outputting a list.
//
// Revision 13.1  1999/08/25 19:55:30  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:40:14  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:04  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:21  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:18  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.2  1998/05/04 20:33:11  hr
// Removed a couple of unneeded variables.
//
// Revision 9.1  1997/09/17 14:37:13  hr
// getRCName() now leaves on the file name extension. The suffix is preceded by
// a '.' and neither are written if the suffix is blank.
//
// Revision 9.0  1997/08/25 21:28:24  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.1  1997/02/25 16:41:18  hr
// When the get<xxx> functions write a default, they now use the data set's
// name rather than "aipsview".
// Added setIntList() for AvIPositions.
//
// Revision 8.0  1997/02/20 03:17:21  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.4  1997/01/24 21:44:50  hr
// Fixes to allow compiling w/o TCL.
//
// Revision 7.3  1997/01/24 20:01:27  hr
// The get<xxx>() routines now write the default to def_ if the requested
// variable does not exist. The set<xxx>() routines accept an argument to
// write the value to def_ rather than set_.
// Don't write old file entries as comments.
// Change which variables get dumped when writeOptions() is called.
//
// Revision 7.2  1996/12/16 17:22:22  droberts
// some of the get routines were not returning the default value if TCL was not turned on.
//
// Revision 7.1  1996/12/12 08:11:02  droberts
// *** empty log message ***
//
// Revision 1.5  1996/11/06 22:44:25  hr
// Had previously misspelled __SVR4 as __SVR.
//
// Revision 1.4  1996/11/06 17:04:50  hr
// Sun's 4.1.3 compiler complains about variables named "end".
// It also complains about arguments to free() that aren't "char *".
//
// Revision 1.3  1996/11/05  22:21:03  hr
// Non TCL version of loadDefaults() was typed as void.
//
// Revision 1.2  1996/11/05  22:09:59  hr
// IRIX compiler can't handle two functions of the same name one with an
// int arg and one with char. (getBoolean()).
//
// Revision 1.1  1996/11/05  21:10:58  hr
// Initial revision
//
//
//---------------------------------------------------------------------------

/* AvConfigInfo.cc
*/
#include <string.h>
#include <stdlib.h>			// free().
#include <sys/types.h>			// stat
#include <sys/stat.h>			// ..
#include <AvConfigInfo.h>

#ifdef AIPSPLUSPLUS
#include <aips/Utilities/cregex.h>
#else
#include <cregex.h>
#endif

#include <pwd.h>			// getpwnam
extern int obscure_syntax;

#ifndef FALSE
#define FALSE 0
#define TRUE 1
typedef int Boolean;
#endif

// Environment variable that hold name of rc file to use.
static const char *AIPSVIEWRC= "AIPSVIEWRC";
// If variable doesn't exist, use this.
static const char *DEFAULTRC= "~/.aipsviewrc";
static const char *DEFAULTVAR = "aipsview";

#ifdef TCL

/* Create a ConfigInfo object.
 filename	Name of the data file for which we need the configuration
		information. Any trailing extension will be removed and
		"rc" will be appended. If the rc file exists in the current
		directory, it will be used. If not, the path will be used.

*/

AvConfigInfo::AvConfigInfo(const char *filename, const char *moduleName,
			   const char *defaultScript)
{
	initialize();

	// Data file name w/o path.
	if(filename == NULL)
		fileVar_ = defaultVar();
	else
		fileVar_ = getRCName(filename, FALSE, "");
	module_ = moduleName;

	// Load module specific defaults.
	if(defaultScript != NULL)
		def_.eval(defaultScript);

	// Load data set specific defaults.
	AvString fileRC = getRCName(filename);
	if(fileExists(fileRC))			// Does it exist locally?
		rc_.evalFile(fileRC);
	else					// If not, try with full path.
	{	fileRC = getRCName(filename, TRUE);
		if(fileExists(fileRC))
			rc_.evalFile(fileRC);
	}
}

#else
AvConfigInfo::AvConfigInfo(const char *, const char *,
	   const char *)
{
	fileVar_ = "";
	module_ = "";
}
#endif

AvConfigInfo::~AvConfigInfo()
{
}

#ifdef TCL

static int initialized_=0;
// Interpreters to hold all the defaults.
AvTcl	AvConfigInfo::def_;		// Defaults set internally by aipsview.
AvTcl	AvConfigInfo::rc_;		// Defaults from rc files.
AvTcl	AvConfigInfo::set_;		// Defaults set by user
AvTclHashTable	AvConfigInfo::forget_;// Table of variable names to forget.
AvTclHashTable	AvConfigInfo::remember_;// Table of variable names to dump.

// One time only global initializations.
void AvConfigInfo::initialize()
{
	if( initialized_)
		return;

	// Keys will be strings.
	forget_.init(TCL_STRING_KEYS);
	remember_.init(TCL_STRING_KEYS);
	// Keep track of variable names that the interpreter creates.
	forgetAll(def_);


	// Try to run the user's .aipsviewrc script. The name may be
	// overridden by an environment variable.
	const char *avrc = defaultName();
	// Use tcl's fileExist routine since it handles "~".
	if(rc_.fileExists(avrc))
		rc_.evalFile(avrc);

	initialized_ = 1;
}

#else
void AvConfigInfo::initialize(){}
#endif

// Build a TCL array spec of the form:
//	"<fileName>(<moduleName>,<variableName>)"
// fileName and moduleName are the internal variables.
// If def is TRUE, then defaultVar() is substituted for fileName_.
// No check is made to make sure var is valid.
AvString AvConfigInfo::makeIndex( const char *var, const int def)const
{AvString s = (def) ? AvString(defaultVar()) : fileVar_;

	s += "(" + module_ + "," + var + ")";
	return s;
}


/////////////////////////////////////////////////////////////////////////
////////////////	Set/get values.				/////////
/////////////////////////////////////////////////////////////////////////

#ifdef TCL
// Routines to retrieve variables. Returns 1 if the variable was found,
// otherwise, 0.
// The routines try the databases in this order:
//	set_ and rc_ with the filename.
//	set_, rc_, def_ with defaultVar().
// If the variable isn't found, the default is returned AND it is written
// to def_.
int AvConfigInfo::getInt(const char *var, int &value, const int def)const
{ AvString index = makeIndex(var);

	if(set_.getIntVal(index.chars(), value) == TCL_OK)
		return 1;
	if(rc_.getIntVal(index.chars(), value) == TCL_OK)
		return 1;
	AvString index1 = makeIndex(var, TRUE);
	if(set_.getIntVal(index1.chars(), value) == TCL_OK)
		return 1;
	if(rc_.getIntVal(index1.chars(), value) == TCL_OK)
		return 1;
	if(def_.getIntVal(index1.chars(), value) == TCL_OK)
		return 1;
	value = def;
	def_.setIntVal(index.chars(), value);
	return 0;
}
#else
int AvConfigInfo::getInt( const char *, int &value, const int def)const
{
        value = def;
	return 0;
}
#endif


// Read in a variable and convert to an int.
int AvConfigInfo::getMappedInt(const char *variable, int &value,
			       const IntMap *map)const
{AvString str;

	if(! getString(variable, str))
		return 0;
	return stringToInt(str.chars(), value, map);
}

// Read in a variable and convert to an int.
int AvConfigInfo::getMappedInt(const char *variable, int &value,
			       const IntMap *map, const int def)const
{
	if(getMappedInt(variable, value, map))
		return 1;
	value = def;
#ifdef TCL
	{const char *str;
		if((str = intToString(value, map)) != NULL)
			def_.setVar(makeIndex(variable), str);
	}
#endif
	return 0;
}

#ifdef TCL
int AvConfigInfo::getDouble(const char *var, double &value,
			    const double def)const
{ AvString index = makeIndex(var);

	if(set_.getDoubleVal(index.chars(), value) == TCL_OK)
		return 1;
	if(rc_.getDoubleVal(index.chars(), value) == TCL_OK)
		return 1;
	AvString index1 = makeIndex(var, TRUE);
	if(set_.getDoubleVal(index1.chars(), value) == TCL_OK)
		return 1;
	if(rc_.getDoubleVal(index1.chars(), value) == TCL_OK)
		return 1;
	if(def_.getDoubleVal(index1.chars(), value) == TCL_OK)
		return 1;
	value = def;
	def_.setDoubleVal(index.chars(), value);
	return 0;
}
#else
int AvConfigInfo::getDouble(const char *, double &value, const double def)const
{
        value = def;
	return 0;
}
#endif

#ifdef TCL
int AvConfigInfo::getBoolean(const char *var, int &value, const int def)const
{ AvString index = makeIndex(var);

	if(set_.getBooleanVal(index.chars(), value) == TCL_OK)
		return 1;
	if(rc_.getBooleanVal(index.chars(), value) == TCL_OK)
		return 1;
	AvString index1 = makeIndex(var, TRUE);
	if(set_.getBooleanVal(index1.chars(), value) == TCL_OK)
		return 1;
	if(rc_.getBooleanVal(index1.chars(), value) == TCL_OK)
		return 1;
	if(def_.getBooleanVal(index1.chars(), value) == TCL_OK)
		return 1;
	value = def;
	def_.setBooleanVal(index.chars(), value);
	return 0;
}
#else
int AvConfigInfo::getBoolean( const char *, int &value, const int def)const
{
        value = def;
	return 0;
}
#endif

#ifdef TCL

int AvConfigInfo::getString(const char *var, AvString &value)const
{ AvString index = makeIndex(var);
  const char *rtn;

	if((rtn = set_.getVar(index.chars())) != NULL)
	{	value = rtn;
		return 1;
	}
	if((rtn = rc_.getVar(index.chars())) != NULL)
	{	value = rtn;
		return 1;
	}
	index = makeIndex(var, TRUE);
	if((rtn = set_.getVar(index.chars())) != NULL)
	{	value = rtn;
		return 1;
	}
	if((rtn = rc_.getVar(index.chars())) != NULL)
	{	value = rtn;
		return 1;
	}
	if((rtn = def_.getVar(index.chars())) != NULL)
	{	value = rtn;
		return 1;
	}
	return 0;
}
#else
int AvConfigInfo::getString( const char *, AvString &)const
{
	return 0;
}
#endif

int AvConfigInfo::getString(const char *var, AvString &value,
			    const AvString &def)const
{
	if(getString(var, value))
		return 1;
	value = def;
#ifdef TCL
	def_.setVar(makeIndex(var), value);
#endif
	return 0;
}

#ifdef TCL
int AvConfigInfo::getDoubleList(const char *var, AvWPosition &value)const
{ AvString index = makeIndex(var);

	if(set_.getDoubleList(index.chars(), value) == TCL_OK)
		return 1;
	if(rc_.getDoubleList(index.chars(), value) == TCL_OK)
		return 1;
	index = makeIndex(var, TRUE);
	if(set_.getDoubleList(index.chars(), value) == TCL_OK)
		return 1;
	if(rc_.getDoubleList(index.chars(), value) == TCL_OK)
		return 1;
	if(def_.getDoubleList(index.chars(), value) == TCL_OK)
		return 1;
	return 0;
}
#else
int AvConfigInfo::getDoubleList(const char *, AvWPosition &)const
{
	return 0;
}
#endif

int AvConfigInfo::getDoubleList(const char *var, AvWPosition &value,
				const AvWPosition &def)const
{
	if(getDoubleList(var, value))
		return 1;
	value = def;
#ifdef TCL
	def_.setDoubleList(makeIndex(var), value);
#endif
	return 0;
}

#ifdef TCL
int AvConfigInfo::getIntList(const char *var, AvIPosition &value)const
{ AvString index = makeIndex(var);

	if(set_.getIntList(index.chars(), value) == TCL_OK)
		return 1;
	if(rc_.getIntList(index.chars(), value) == TCL_OK)
		return 1;
	index = makeIndex(var, TRUE);
	if(set_.getIntList(index.chars(), value) == TCL_OK)
		return 1;
	if(rc_.getIntList(index.chars(), value) == TCL_OK)
		return 1;
	if(def_.getIntList(index.chars(), value) == TCL_OK)
		return 1;
	return 0;
}
#else
int AvConfigInfo::getIntList(const char *, AvIPosition &)const
{
	return 0;
}
#endif

int AvConfigInfo::getIntList(const char *var, AvIPosition &value,
			     const AvIPosition &def)const
{
	if(getIntList(var, value))
		return 1;
	value = def;
#ifdef TCL
	def_.setIntList(makeIndex(var), value);
#endif
	return 0;
}

#ifdef TCL
int AvConfigInfo::getMappedIntList(const char *var, AvIPosition &value,
				   const IntMap *map)const
{AvString lst;

	if(!getString(var, lst))
		return 0;
	int size, i;
	char **list, **ptr;
	if( def_.splitList(lst.chars(), size, &list) != TCL_OK)
		return 0;

	value.resize(size);

	int rtn = 0;
	for(ptr=list, i=0; i< size; i++, ptr++)
	{int v;
		if(stringToInt(*ptr, v, map))
		{	value(i) = v;
			rtn += 1;
		}
	}
#if defined(sparc) && ! defined(__SVR4)
	free((char *)list);
#else
	free(list);
#endif
	return rtn;
}
#else
int AvConfigInfo::getMappedIntList(const char *, AvIPosition &,
				   const IntMap *)const
{
	return 0;
}
#endif

#ifdef TCL
void AvConfigInfo::setInt(const char *var, const int value, const int def)
{
	if(def)
		def_.setIntVal(makeIndex(var), value);
	else
		set_.setIntVal(makeIndex(var), value);
}
#else
void AvConfigInfo::setInt(const char *, const int, const int )
{
}
#endif

#ifdef TCL
void AvConfigInfo::setHex(const char *var, const int value, const int def)
{
	if(def)
		def_.setIntVal(makeIndex(var), value, 0, "0X%x");
	else
		set_.setIntVal(makeIndex(var), value, 0, "0X%x");
}
#else
void AvConfigInfo::setHex(const char *, const int, const int )
{
}
#endif

#ifdef TCL
void AvConfigInfo::setDouble(const char *var, const double value,
			     const int def)
{
	if(def)
		def_.setDoubleVal(makeIndex(var), value);
	else
		set_.setDoubleVal(makeIndex(var), value);
}
#else
void AvConfigInfo::setDouble(const char *, const double, const int )
{
}
#endif

#ifdef TCL
void AvConfigInfo::setDoubleList(const char *var, const AvWPosition &value,
				 const int def)
{
	if(def)
		def_.setDoubleList(makeIndex(var), value);
	else
		set_.setDoubleList(makeIndex(var), value);
}
#else
void AvConfigInfo::setDoubleList(const char *, const AvWPosition &,
				 const int)
{
}
#endif

#ifdef TCL
void AvConfigInfo::setDoubleList(const char *var,
				 const int length, const double *values,
				 const int def)
{
	if(def)
		def_.setDoubleList(makeIndex(var), length, values);
	else
		set_.setDoubleList(makeIndex(var), length, values);
}
#else
void AvConfigInfo::setDoubleList(const char *, const int , const double *,
				 const int)
{
}
#endif

#ifdef TCL
void AvConfigInfo::setIntList(const char *var,
			      const int length, const int *values,
			      const int def)
{
	if(def)
		def_.setIntList(makeIndex(var), length, values);
	else
		set_.setIntList(makeIndex(var), length, values);
}
#else
void AvConfigInfo::setIntList(const char *, const int , const int *,
			      const int)
{
}
#endif

#ifdef TCL
void AvConfigInfo::setIntList(const char *var, const AvIPosition &values,
			      const int def)
{
	if(def)
		def_.setIntList(makeIndex(var), values);
	else
		set_.setIntList(makeIndex(var), values);
}
#else
void AvConfigInfo::setIntList(const char *, const AvIPosition &,
			      const int)
{
}
#endif

#ifdef TCL
void AvConfigInfo::setMappedIntList(const char *var, const AvIPosition &lst,
				    const IntMap *map, const int def)
{//AvString v = makeIndex(var);
 AvString list = "{";

 int n = lst.nelements();

	for(int i = 0; i < n; i++)
	{const char *str = intToString(lst(i), map);
		if(str != NULL)
		{	list += " ";
			list += str;
		}
	}
	list += "}";
	if(def)
		def_.setVar(makeIndex(var), list.chars());
	else
		set_.setVar(makeIndex(var), list.chars());

}
#else
void AvConfigInfo::setMappedIntList(const char *, const AvIPosition &,
				    const IntMap *, const int )
{
}
#endif

#ifdef TCL
void AvConfigInfo::setBoolean(const char *var, const int value,
			      const int def)
{
	if(def)
		def_.setBooleanVal(makeIndex(var), value);
	else
		set_.setBooleanVal(makeIndex(var), value);
}
#else
void AvConfigInfo::setBoolean(const char *, const int, const int )
{
}
#endif

#ifdef TCL
void AvConfigInfo::setString(const char *var, AvString &value,
			     const int def)
{
	if(def)
		def_.setVar(makeIndex(var), value);
	else
		set_.setVar(makeIndex(var), value);
}

void AvConfigInfo::setString(const char *var, const char *value,
			     const int def)
{
	if(def)
		def_.setVar(makeIndex(var), value);
	else
		set_.setVar(makeIndex(var), value);
}
#else
void AvConfigInfo::setString(const char *, AvString &, const int )
{
}

void AvConfigInfo::setString(const char *, const char *, const int )
{
}

#endif

// Returns a pointer to the string that matches val. Returns map[0].name
// if no value matches. Returns NULL if map is NULL.
// (Note that the search starts at map[0] in case the 'default' value is
// also a valid selection.
const char *AvConfigInfo::intToString(const int val, const IntMap *map)
{
	if(map == NULL)
		return NULL;
	const IntMap *ptr = map;
	while(ptr->name != NULL)
		if(val == ptr->value)
			return ptr->name;
		else
			ptr += 1;
	return map[0].name;
}

// Returns the value corresponding to name or map[0].value if none matches
// or name is NULL. Returns 0 if map is NULL. If the name wasn't found
// sets value to map[0].value and returns 0.
// Otherwise, returns 1.
int AvConfigInfo::stringToInt(const char *name, int &value, const IntMap *map)
{
	if(map == NULL)
		return 0;
	if(name == NULL)
	{	value = map[0].value;
		return 0;
	}
	const IntMap *ptr = map;
	while(ptr->name != NULL)
		if(strcmp(name, ptr->name) == 0)
			{	value = ptr->value;
				return 1;
			}
		else
			ptr += 1;

	value = map[0].value;
	return 0;
}

void AvConfigInfo::setMappedInt(const char *var, const int value,
				const IntMap *map, const int def)
{
	const char *str = intToString(value, map);
	if(str == NULL)
		return;
	setString(var, str, def);
}

// Convert a list of mapped ints into a TCL style list.
// (  { <elem1> ... <elemn> } ).
AvString AvConfigInfo::buildMappedIntList(const AvIPosition &list,
					  const IntMap *map)
{ AvString s = "";

	if(map == NULL)
		return s;
	s = "{";
	int len = list.nelements();
	for(int i=0; i < len; i++)
		{	s += " ";
			s += intToString(list(i), map);
		}
	s += "}";
	return s;
}


/////////////////////////////////////////////////////////////////////////
////////////////	Write current values to output file. ////////////
/////////////////////////////////////////////////////////////////////////

#if 0
#ifdef TCL
// Test routine called from the debugger.
static void pa(const char *array, AvTcl &tcl)
{int length;
 AvString id, var;

	length = tcl.arraySize(array);
	cout << array << " has " << length << " elements\n";
	const char *elem;
	for(id = tcl.arrayStartSearch(array); tcl.arrayAnymore(array, id);)
	{
		var = array;
		var += "(";
		elem = tcl.nextElement(array, id);
		var += elem;
		var += ")";
	cout	<< var << " " << tcl.getVar(var.chars()) << endl;
	}
	tcl.arrayDoneSearch(array, id);
}
#endif
#endif

// Logs varName as a variable that will need to be updated when
// saveTo() is called.
#ifdef TCL
void AvConfigInfo::rememberVar(const char *varName, AvTcl &interp)
{
	if(varName == NULL)
		return;
	remember_.setValue(varName, &interp);
}
#endif

#ifdef TCL
// Remember the elements of an array.
void AvConfigInfo::rememberArray(const char *array, AvTcl &tcl)
{ AvString id, var;

	const char *elem;
	for(id = tcl.arrayStartSearch(array); tcl.arrayAnymore(array, id);)
	{
		var = array;
		var += "(";
		elem = tcl.nextElement(array, id);
		var += elem;
		var += ")";
		rememberVar(var.chars(), tcl);
	}
	tcl.arrayDoneSearch(array, id);
}

// Remember all the variables for an interpreter. (Except the ones to 'forget')
// If all is false, all variables for file fn are remembered.
// If not, all variables for all files are remembered.
void AvConfigInfo::rememberAll(AvTcl &interp, const char *fn, const int all)
{
	// Pick up a list of the interpreter's variables.
	int nvars;
	char **vars;

	interp.eval("info vars");
	interp.splitList(interp.result(), nvars, &vars);

	// For each variable, remember simple ones and arrays.
	for(int i=0; i< nvars; i++)
	{const char *var = vars[i];

		if(forget_.findHashEntry(var) != NULL)
			continue;
		if(!all && (strcmp(fn, var) != 0))
			continue;
		if(interp.arraySize(var) == 0)
			rememberVar(var, interp);
		else
			rememberArray(var, interp);
	}
#if defined(sparc) && ! defined(__SVR4)
	free((char *)vars);
#else
	free(vars);
#endif
}

#endif

// Names in here aren't updated when save is called.
// (Used internally).
#ifdef TCL
void AvConfigInfo::forget(const char *name)
{	if(name == NULL)
		return;
	// All we remember is the name.
	forget_.setValue(name, NULL);
}

// Put currently existing variables in the forget table so they
// don't get dumped on output.
void AvConfigInfo::forgetAll(AvTcl & tcl)
{int argc;
 char **argv;

	tcl.eval("info vars");
	tcl.splitList(tcl.result(), argc, &argv);
	for(int i=0; i< argc; i++)
		forget(argv[i]);

#if defined(sparc) && ! defined(__SVR4)
	free((char *)argv);
#else
	free(argv);
#endif
}

#else
void AvConfigInfo::forget(const char *)
{
}
#endif


// Copy remembered variables to fileName. If fileName already exists,
// a backup is made and copied to the new version.
// A simple scan is made to look for lines like "set <remembered variable>...".
// When one is found, it is replaced by an updated set command.
#ifdef TCL
void AvConfigInfo::saveTo(const char *fileName, const char *dataFile)
{
	// Pick up a copy of script if it exists.
	AvString script = "";
	AvString rc;
	if(dataFile == NULL)
		rc = defaultName();
	else
	{	rc = getRCName(dataFile);	// Local version
		if(!fileExists(rc))		// version with data file.
			rc = getRCName(dataFile, TRUE);
	}

	// The rc_ interpreter is used below, but it could have been
	// either of the others.
	if(fileExists(rc))
	{	rc_.varEval("open ", rc.chars());
		AvString fileid = rc_.result();	// Copy fileID.
		// Read in the script.
		rc_.varEval("read ", fileid.chars());
		script = rc_.result();
		rc_.varEval("close ", fileid);
	}
	else
		script = "";

	if(fileExists(fileName))
	{	if(rc_.varEval( "file rename -force ", fileName, " ",
				fileName, ".bak")!= TCL_OK)
			cerr	<< "AvConfigInfo::saveTo: Could not rename "
				<< fileName << endl;
	}

	// Remove all variables in the forget table from the remember
	// table.
	Tcl_HashEntry *ef, *er;
	Tcl_HashSearch searchF;
	for(ef = forget_.firstHashEntry(&searchF); ef != NULL;
	    ef = forget_.nextHashEntry(&searchF))
	{	char *key = forget_.getHashKey(ef);
		er = remember_.findHashEntry(key);
		remember_.deleteHashEntry(er);
	}

	// Write out the file.
	fstream out;
	AvString outName = expandPath(fileName);
	if(outName.length() > 0)
	{	out.open(outName, ios::out);
		if(out.rdstate() == ios::goodbit)
		{	writeScript(out, script);
			writeVars(out);		// Dump any remaining.
			out.close();
		}
	}
}
#else
void AvConfigInfo::saveTo(const char *, const char *)
{
}
#endif

// Copy the tcl script to the output. Wherever a "set var" is detected,
// var is looked up in the remember table. If it is found, the current
// value is written instead. var is removed from the table so it won't
// be dumped later.

void AvConfigInfo::writeScript(fstream &out, AvString &script)
{
	if(script.length() == 0)	// Ignore if empty.
		return;
	// Set cregex bits the way we want them.
	int old_syntax = obscure_syntax;
	obscure_syntax = RE_NO_BK_PARENS | RE_NO_BK_VBAR ;
	// Regular expression to detect (continuation) lines.
	// (The "\\\\$" winds up as "\$" (backslash $, not quoted $).
	// Register 1 will contain a continued line or register 2 will
	// conain a non-continued line.
	AvRegex reg("(^.*\\\\$)|(^.*$)");

	const char *ptr = script.chars(), *endp = ptr+script.length();
	AvString line;
	while(ptr < endp)
	{
		if(reg.match(ptr, (int)(endp-ptr), 0) < 0)
			break;
		int start, length;
		// If reg 1 matches, line is continued
		// If reg 2 matches, line is finished, print and reset.
		if(reg.match_info(start, length, 1))	// Line is continued.
		{int startpos = (int)(ptr-script.chars()) + start;
			length += 1;		// Pick up the trailing char.
			line += script.at(startpos, length);
		}
		else	// Line ends
		if(reg.match_info(start, length, 2))
		{int startpos = (int)(ptr-script.chars()) + start;
			length += 1;	// Pick up the trailing newline.
			line += script.at(startpos, length);
			writeLine(out, line);
			line = "";
		}
		ptr += length;
	}
	obscure_syntax = old_syntax;
}

// Write a, possibly continued, line to the output.
// If the line is of the form: "set var ...", then var is checked to
// see if it is in the remember table. If it is, the line is replaced by
// a command to set the variable to its current value and the variable
// is removed from the table.
// This is a very simple check. Multiple statements on a line will not be
// handled correctly.
#ifdef TCL
void AvConfigInfo::writeLine(fstream &out, AvString &line)
{
// White space followed by "set" followed by anything
// that isn't white space.
AvRegex set("[ \t]*set[ \t]+([^ \t]+)");

	// Look for "set <variable> ...".
	set.match(line.chars(), line.length(), 0);
	int start, length;
	// Where variable is.
	int rm2 = set.match_info(start, length, 1);
	if(rm2 <= 0)		// No match found.
	{	out << line;
		return;
	}
	// Pick up the variable.
	char buf[256];
	strncpy(buf, line.chars()+start, length);
	buf[length] = '\0';

	// Does it appear in the remember table?
	Tcl_HashEntry *e = remember_.findHashEntry(buf);
	if(e == NULL)		// Nope.
	{	out << line;
		return;
	}
	// If variable was remembered, write out the old value as a comment.
// (Since there is a backup, we don't need to do this).
//	out << "#" << line;
	// Write out current value.
	writeVar(out, e);
//	// Remove from table.
//	remember_.deleteHashEntry(e);
}
#else
void AvConfigInfo::writeLine(fstream &, AvString &)
{
}
#endif

// Write out the current value of var.
#ifdef TCL
void AvConfigInfo::writeVar(fstream &out, Tcl_HashEntry *e)
{
	if(e == NULL)
		return;
	char *var = remember_.getHashKey(e);
	AvTcl *interp = (AvTcl *) AvTclHashTable::getHashValue(e);
	if(interp == NULL)
		return;
	// Ask the interpreter for the current value.
	// Then check to see if it is a list.
	out << "set " << var << " ";
	int listLen;
	char **list;
	const char *val = interp->getVar(var);
	interp->splitList( val, listLen, &list);
	if(listLen > 1)
	{	out << "{";	// Want {<string>}, not { <string> }
				// Otherwise { multi word value } fails
				// because of the leading and trailing values.
		for(int i=0; i< listLen; i++)
		{	out << list[i];
			if(i != listLen -1)
				out << " ";
			else
				out << "}\n";
		}
	}
	else
	if( strlen(val) == 0)
	{	out << "{ " << val << " }" << endl;
		cout << var << " is empty.";
	}
	else
		out << val << endl;
#if 0
if(strstr(var, "Label)") != NULL)
{ cout << var << " listLen = " << listLen << " vallen = " << strlen(val)
	<< " val = |" << val << "|" << endl;
}
#endif

#if defined(sparc) && ! defined(__SVR4)
	free((char *)list);
#else
	free(list);
#endif
	// Want to remove from hashtable, but can't since we're probably
	// in the middle of a search, so just remove the reference to the
	// interpreter.
	remember_.setHashValue(e, NULL);
}
#endif

#ifdef TCL
// Write current contents of the remember table.
void AvConfigInfo::writeVars(fstream &out)
{ Tcl_HashEntry *er;
  Tcl_HashSearch searchR;

	for(er = remember_.firstHashEntry(&searchR); er != NULL;
	    er = remember_.nextHashEntry(&searchR))
		writeVar(out, er);
}
#else
void AvConfigInfo::writeVars(fstream &)
{
}
#endif

// Dump the current option settings to ofile. If all is TRUE, then all options
// for the dataFile are written. Otherwise, only those that have been changed.
// If dataFile = defaultName() or NULL, the variable name is defaultVar()
// otherwise, it is made from the data file name.

#ifdef TCL
void AvConfigInfo::writeOptions(const char *ofile, const char *dataFile,
				const int all)
{
	if(ofile == NULL)
		return;
	/* For each variable in the interpreter:
		If it is in the forget table, ignore it.
		If !all and var != filename, ignore it.
		Otherwise:
			if var is an array, remember all its elements
			else remember var.

		Copy old rc file to new inserting new variable values.
	*/
	// Reset the remember table.
	remember_.clear();

	AvString fn;
	if((dataFile == NULL) || (strcmp(dataFile, defaultName())== 0))
		fn = defaultVar();
	else
		fn = getRCName(dataFile, 0, "");

	// If there is no current file, write all known variables.
#if 0
	if(!fileExists(ofile))
	{	rememberAll(def_, fn.chars(), all);
		rememberAll(rc_, fn.chars(), all);
	}
	// Remember variables that have been set by user.
	rememberAll(set_, fn.chars(), all);
#else
	if(all)
	{	rememberAll(def_, fn.chars(), FALSE);
		rememberAll(rc_, fn.chars(), FALSE);
	}
	// Remember variables that have been set by user.
	rememberAll(set_, fn.chars(), FALSE);
#endif
	saveTo(ofile, dataFile);
}
#else
void AvConfigInfo::writeOptions(const char *, const char *,
				const int )
{
}

#endif

// Given a dataset's filename (with path), return the name
// of the rc file with or without the path.
// (Leave the extension on).
AvString AvConfigInfo::getRCName(const char *fileName, const int withPath,
				 const char *suffix)
{AvString str("");
// static AvRegex reg("[.].*$");
 const char *s;

	if(fileName == NULL)
		return str;

	if(!withPath)
	{ const char *pend = strrchr(fileName, '/');
	  if(pend != NULL)
		s = pend +1;
	  else
		s = fileName;
	}
	else
		s = fileName;
	str = s;
//	str.del(reg, -1);
	if((suffix != NULL) && (strlen(suffix) > 0))
	{	str += ".";
		str += suffix;
	}
	return str;
}

extern int errno;

int AvConfigInfo::fileExists(const char *filename)
{struct stat buf;

	if(filename == NULL)
		return FALSE;

	AvString fn = expandPath(filename);
	int err = stat(fn.chars(), &buf);
//	if(err < 0)
//		perror(filename);

	if((err == 0) && ((buf.st_mode & S_IFREG) != 0))
		return TRUE;
	else
		return FALSE;
}

int AvConfigInfo::fileExists(const AvString &filename)
{
	return fileExists(filename.chars());
}

#ifdef TCL
// Load some more defaults.
int AvConfigInfo::loadDefaults(const char *script)
{
	if(script != NULL)
		return (def_.eval(script) == TCL_OK) ? 1 : 0;
	else
		return 0;
}
#else
int AvConfigInfo::loadDefaults(const char *)
{
	return 0;
}
#endif

#ifdef TCL
// Load some more defaults.
int AvConfigInfo::loadVarDefaults(const char *s1, const char *s2,
				   const char *s3, const char *s4,
				   const char *s5, const char *s6,
				   const char *s7, const char *s8,
				   const char *s9)
{
	if(s1 != NULL)
	{	int err = def_.varEval(s1, s2, s3, s4, s5, s6, s7, s8, s9);
		return (err == TCL_OK) ? 1 : 0;
	}
	else
		return 0;
}
#else
int AvConfigInfo::loadVarDefaults(const char *, const char *,
				   const char *, const char *,
				   const char *, const char *,
				   const char *, const char *,
				   const char *)
{
	return 0;
}
#endif

// Name of "default" file.
const char *AvConfigInfo::defaultName()
{
	const char *avrc = getenv(AIPSVIEWRC);
	if(avrc == NULL)
		avrc = DEFAULTRC;
	return avrc;
}

// Name of "default" variable.
const char *AvConfigInfo::defaultVar()
{
	return DEFAULTVAR;
}

// Expand path by converting "~/" or $HOME to the user's home directory.
// "~name/" to "name"'s home directory.
// Returns the empty string if an error was found.
AvString AvConfigInfo::expandPath(const char *path)
{AvString s = path;
 char *home = getenv("HOME");

	if(s.index("~/") == 0)
	{	if(home != NULL)
			s.gsub("~", home);
		else
			s = "";
	}
	else
	if(s.index("$HOME") == 0)
	{	if(home != NULL)
			s.gsub("$HOME", home);
		else
			s = "";
	}
	else
	{ AvRegex reg("\\(~.+/\\)");	// Try "~<anything>/"
	  int start, len;

		reg.match(s, s.length(), 0);
		if(reg.match_info(start, len, 1))
		{ struct passwd *pwd;
		  AvString o = s.at(start, len);
			o.del('/');
		  AvString name = o;
			name.del('~');
			pwd = getpwnam(name.chars());
			if(pwd != NULL)
				s.gsub(o.chars(), pwd->pw_dir);
			else
				s = "";
		}
	}

	return s;
}

AvString AvConfigInfo::expandPath(const AvString &path)
{	return expandPath(path.chars());
}
