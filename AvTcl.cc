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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvTcl.cc,v 19.0 2003/07/16 05:47:56 aips2adm Exp $
//
// $Log: AvTcl.cc,v $
// Revision 19.0  2003/07/16 05:47:56  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:31  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:43:00  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:06  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:28  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:31:02  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:09:02  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/09/24 21:28:20  hravlin
// Write an empty list if trying to set a variable to nothing.
//
// Revision 13.0  1999/08/10 18:40:52  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:10  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.1  1999/04/05 17:00:27  hr
// Had been using a NULL to initialize an int in a structure.
//
// Revision 11.0  1998/10/03 07:01:56  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:56:07  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.2  1998/06/11 17:36:52  hr
// Small change to get rid of a typedef warning.
//
// Revision 9.1  1998/05/04 20:38:13  hr
// Updated copyright date.
//
// Revision 9.0  1997/08/25 21:31:13  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.1  1997/02/25 17:14:17  hr
// setIntList() doesn't no longer surrounds the list with braces.
// (Should probably do doubleLists too).
//
// Revision 8.0  1997/02/20 03:19:08  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 21:57:30  droberts
// Added ifdef TCL around entire file.
//
// Revision 7.1  1996/12/12 00:16:21  droberts
// Initial checkin from monet.
//
// Revision 1.3  1996/11/06 22:44:25  hr
// Had previously misspelled __SVR4 as __SVR.
//
// Revision 1.2  1996/11/06 17:03:15  hr
// Sun's 4.1.3 compiler complained about using free to free anything but
// a "char *".
//
// Revision 1.1  1996/11/05  21:16:27  hr
// Initial revision
//
//
//---------------------------------------------------------------------------

/* Tcl.cc
C++ wrapper for tcl
*/

#ifdef TCL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AvTcl.h"

// Default format strings in case one isn't supplied.
static const char *DEFAULTIFORMAT = "%d";
static const char *DEFAULTDFORMAT = "%g";
static const char *DEFAULTTFORMAT = "True";
static const char *DEFAULTFFORMAT = "False";


AvTcl::AvTcl()
{
	interp_ = Tcl_CreateInterp();
	tfmt_ = ffmt_ = dfmt_ = ifmt_ = NULL;
	setIformat();
	setDformat();
	setTformat();
	setFformat();
}

AvTcl::~AvTcl()
{
	Tcl_DeleteInterp(interp_);
	interp_ = NULL;
}

const char *AvTcl::result()const
{
	return interp_->result;
}

// AvTcl::
int AvTcl::eval(char *script)
{
	if(script != NULL)
		return Tcl_Eval(interp_, script);
	else
		return TCL_ERROR;
}

int AvTcl::eval(const AvString &script)
{
	return eval(script.chars());
}

int AvTcl::eval(const char *script)
{
	if(script != NULL)
	{char *s = new char[strlen(script)+1];
		strcpy(s, script);
		int rtn = eval(s);
		delete [] s;
		return rtn;
	}
	else
		return TCL_ERROR;
}

int AvTcl::evalFile(char *fileName)
{
	return Tcl_EvalFile(interp_, fileName);
}

int AvTcl::evalFile(const char *fileName)
{
	if(fileName != NULL)
	{char *s = new char[strlen(fileName)+1];
		strcpy(s, fileName);
		int rtn = Tcl_EvalFile(interp_, s);
		delete [] s;
		return rtn;
	}
	else
		return TCL_ERROR;
}

int AvTcl::evalFile(const AvString &fileName)
{
	return evalFile(fileName.chars());
}

int AvTcl::globalEval(char *script)
{
	return Tcl_GlobalEval(interp_, script);
}

int AvTcl::globalEval(const char *script)
{
	if(script != NULL)
	{char *s = new char[strlen(script)+1];
		strcpy(s, script);
		int rtn = globalEval(s);
		delete [] s;
		return rtn;
	}
	else
		return TCL_ERROR;
}

int AvTcl::globalEval(const AvString &script)
{
	return globalEval(script.chars());
}

int AvTcl::varEval(const char *S1, const char *S2,
		   const char *S3, const char *S4,
		   const char *S5, const char *S6,
		   const char *S7, const char *S8, const char *S9)const
{char *s1, *s2, *s3, *s4, *s5, *s6, *s7, *s8, *s9;

	s1 = Strdup(S1);
	s2 = Strdup(S2);
	s3 = Strdup(S3);
	s4 = Strdup(S4);
	s5 = Strdup(S5);
	s6 = Strdup(S6);
	s7 = Strdup(S7);
	s8 = Strdup(S8);
	s9 = Strdup(S9);
	int rtn = Tcl_VarEval(interp_, s1, s2, s3, s4, s5, s6, s7, s8, s9,
			      NULL);
	if(s1)
		delete [] s1;
	if(s2)
		delete [] s2;
	if(s3)
		delete [] s3;
	if(s4)
		delete [] s4;
	if(s5)
		delete [] s5;
	if(s6)
		delete [] s6;
	if(s7)
		delete [] s7;
	if(s8)
		delete [] s8;
	if(s9)
		delete [] s9;
	return rtn;
}

	////////////////////////////////////////////////////////////////
	//		get (Value)
int AvTcl::getInt(char *string, int &value)const
{
	return Tcl_GetInt(interp_, string, &value);
}

int AvTcl::getInt(const char *string, int &value)const
{
	char *s = Strdup(string);
	if(s != NULL)
	{int rtn = getInt(s, value);
		delete [] s;
		return rtn;
	}
	else
		return TCL_ERROR;
}

int AvTcl::getInt(const AvString &string, int &value)const
{
	return getInt(string.chars(), value);
}

int AvTcl::getDouble(char *string, double &value)const
{
	return Tcl_GetDouble(interp_, string, &value);
}

int AvTcl::getDouble(const char *string, double &value)const
{
	char *s = Strdup(string);
	if(s != NULL)
	{int rtn = getDouble(s, value);
		delete [] s;
		return rtn;
	}
	else
		return TCL_ERROR;
}

int AvTcl::getDouble(const AvString &string, double &value)const
{
	return getDouble(string.chars(), value);
}

int AvTcl::getBoolean(char *string, int &value)const
{
	return Tcl_GetBoolean(interp_, string, &value);
}

int AvTcl::getBoolean(const char *string, int &value)const
{
	char *s = Strdup(string);
	if(s != NULL)
	{int rtn = getBoolean(s, value);
		delete [] s;
		return rtn;
	}
	else
		return TCL_ERROR;
}

int AvTcl::getBoolean(const AvString &string, int &value)const
{
	return getBoolean(string.chars(), value);
}

int AvTcl::exprString(char *string)const
{
	return Tcl_ExprString(interp_, string);
}

int AvTcl::exprString(const char *string)const
{
	char *s = Strdup(string);
	int rtn = exprString(s);
	delete [] s;
	return rtn;
}

int AvTcl::exprString(const AvString &string, AvString &value)const
{int rtn = exprString(string.chars());

	value = result();
	return rtn;	
}

int AvTcl::exprLong(char *string, long &value)const
{
	return Tcl_ExprLong(interp_, string, &value);
}

int AvTcl::exprDouble(char *string, double &value)const
{
	return Tcl_ExprDouble(interp_, string, &value);
}
////////////////////////////////////////////////////////////////
//		get/setVar

char *AvTcl::Tcl_SetVar(Tcl_Interp *interp,
			      const char *varName, const char *newValue,
			      const int flags)
{
	char *vs = AvTcl::Strdup(varName);
	char *nv = AvTcl::Strdup(newValue);
	char *rtn = ::Tcl_SetVar(interp, vs, nv, flags);
	delete [] vs;
	delete [] nv;
	return rtn;
}

char *AvTcl::Tcl_SetVar2(Tcl_Interp *interp, const char *name1,
			 const char *name2, const char *newvalue,
			 const int flags)
{
  char *n1 = AvTcl::Strdup(name1);
  char *n2 = AvTcl::Strdup(name2);
  char *nv = AvTcl::Strdup(newvalue);
  char *rtn = ::Tcl_SetVar2(interp, n1, n2, nv, flags);
  delete [] n1;
  delete [] n2;
  delete [] nv;
  return rtn;
}

const char *AvTcl::setVar(const char *varName, const char *newValue,
			  const int flags)const
{
  return AvTcl::Tcl_SetVar(interp_, varName, newValue, flags);
}

const char *AvTcl::setVar2(char *name1, char *name2, char *newValue,
			   const int flags)const
{
	return Tcl_SetVar2(interp_, name1, name2, newValue, flags);
}

const char *AvTcl::setVar2(const char *name1, const char *name2,
			   const char *newValue, const int flags)const
{
	char *n1 = Strdup(name1);
	char *n2 = Strdup(name2);
	char *nv = Strdup(newValue);
	const char *rtn = setVar2(n1, n2, nv, flags);
	delete [] nv;
	delete [] n1;
	delete [] n2;
	return rtn;
}

const char *AvTcl::getVar(char *varName, const int flags)const
{
	return Tcl_GetVar(interp_, varName, flags);
}

const char *AvTcl::getVar(const char *varName,	const int flags)const
{
	char *vn = Strdup(varName);
	const char *rtn = getVar(vn, flags);
	delete [] vn;
	return rtn;
}

const char *AvTcl::getVar2(char *name1, char *name2, const int flags)const
{
	return Tcl_GetVar2(interp_, name1, name2, flags);
}

const char *AvTcl::getVar2(const char *name1, const char *name2,
			    const int flags)const
{
	char *n1 = Strdup(name1);
	char *n2 = Strdup(name2);
	const char *rtn = getVar2(n1, n2, flags);
	delete [] n1;
	delete [] n2;
	return rtn;
}


////////////////////////////////////////////////////////////////
int AvTcl::splitList(char *list, int &argc, char ***argPtr)const
{
	return Tcl_SplitList(interp_, list, &argc, argPtr);
}

int AvTcl::splitList(const char *list, int &argc, char ***argPtr)const
{
	char *s = Strdup(list);
	int rtn = splitList(s, argc, argPtr);
	delete [] s;
	return rtn;
}

int AvTcl::splitList(const AvString &list, int &argc, AvString **argPtr)const
{char **ptr;

	int rtn = splitList(list.chars(), argc, &ptr);
	AvString *sptr = new AvString[argc];
	for(int i=0; i< argc; i++)
		sptr[i] = ptr[i];
#if defined(__sparc) && !defined(__SVR4)
	free((char *)ptr);
#else
	free(ptr);
#endif
	*argPtr = sptr;
	return rtn;
}

char *AvTcl::merge(const int argc, char **argv)const
{
	return Tcl_Merge(argc, argv);
}

////////////////////////////////////////////////////////////////
//		Access to internal functions.
////////////////////////////////////////////////////////////////

const char *AvTcl::arrayNames(const char *arrayName)const
{
	varEval("array names ", arrayName);
	return result();
}

#if 0
const char *AvTcl::arrayNames(char *arrayName)const
{
	varEval("array names ", arrayName);
	return result();
}
#endif

int AvTcl::arraySize(const char *arrayName)const
{
	if(arrayName == NULL)
		return 0;
	int rtn = varEval("array size ", arrayName);
	int value = -1;
	if(rtn != TCL_ERROR)
		getInt(result(), value);
	return value;
}

AvString AvTcl::arrayStartSearch(const char *arrayName)const
{AvString id;
	
	int rtn = varEval("array startsearch ", arrayName);
	if(rtn == TCL_OK)
		id = result();
	else
		id = "";
	return id;
}

// Returns 1 if there are more elements, otherwise 0.
int AvTcl::arrayAnymore(const char *arrayName, const AvString &searchID)const
{int val, rtn;

	if( varEval("array anymore ", arrayName, " ",
		    searchID.chars())== TCL_OK)
		rtn = getInt(result(), val);
	else
		rtn = TCL_ERROR;

	return (rtn == TCL_OK) ? val : 0;
}

// Terminate search and reclaims (internal) space used for searchID.
void AvTcl::arrayDoneSearch(const char *arrayName,
		     const AvString &searchID)const
{
	varEval("array donesearch ", arrayName, " ", searchID);
}

const char *AvTcl::nextElement(const char *arrayName,
			const AvString &searchID)const
{
	if(varEval("array nextelement ", arrayName, " ",
		   searchID.chars())== TCL_OK)
		return result();
	else
		return "";
}


	// Does the file exist?
int AvTcl::fileExists(char *fileName)
{
	int rtn = varEval("file ", "exists ", fileName);
	if(rtn == TCL_OK)
	{ int val;
		getInt(result(), val);
		return val;
	}
	else
		return 0;
}

int AvTcl::fileExists(const char *fileName)
{
	char *f = Strdup(fileName);
	int rtn = fileExists(f);
	delete [] f;
	return rtn;
}

////////////////////////////////////////////////////////////////

char *AvTcl::Strdup(const char *string)
{	if(string != NULL)
	{	char *s = new char[strlen(string)+1];
		strcpy(s, string);
		return s;
	}
	else
		return NULL;
}

typedef struct codeNames {
	int	code;
	const char *name;
	}codeNames_;

//	Conversion Tables
static codeNames Errors[] = {
	{ TCL_OK, "TCL_OK"},
	{ TCL_ERROR, "TCL_ERROR"},
	{ TCL_RETURN, "TCL_RETURN"},
	{ TCL_BREAK, "TCL_BREAK"},
	{ TCL_CONTINUE, "TCL_CONTINUE"},
	{ 0, NULL}
	};

// Master table listing all tables.
static const codeNames *table[] = {
	Errors};

const char *AvTcl::codeToName(const RETURNCODES id, const int code)
{
	if((id >= 0) && (id < FENCE))
	{const codeNames *tbl = table[id];

		while(tbl->name != NULL)
			if(tbl->code == code)
				return tbl->name;
			else
				tbl++;
	}

	return "Unknown ID";
}

int AvTcl::nameToCode(const RETURNCODES id, const char *name)
{
	if(name == NULL)
		return TCL_ERROR;

	if((id >= 0) && (id < FENCE))
	{const codeNames *tbl = table[id];

		while(tbl->name != NULL)
			if(strcmp(tbl->name, name) == 0)
				return tbl->code;
			else
				tbl++;
	}

	return TCL_ERROR;
}

void AvTcl::setIformat(const char *fmt)
{
	if(ifmt_ != NULL)
		delete [] ifmt_;
	if(fmt == NULL)
		ifmt_ = Strdup(DEFAULTIFORMAT);
	else
		ifmt_ = Strdup(fmt);
}

void AvTcl::setDformat(const char *fmt)
{
	if(dfmt_ != NULL)
		delete [] dfmt_;
	if(fmt == NULL)
		dfmt_ = Strdup(DEFAULTDFORMAT);
	else
		dfmt_ = Strdup(fmt);
}

void AvTcl::setTformat(const char *fmt)
{
	if(tfmt_ != NULL)
		delete [] tfmt_;
	if(fmt == NULL)
		tfmt_ = Strdup(DEFAULTTFORMAT);
	else
		tfmt_ = Strdup(fmt);
}

void AvTcl::setFformat(const char *fmt)
{
	if(ffmt_ != NULL)
		delete [] ffmt_;
	if(fmt == NULL)
		ffmt_ = Strdup(DEFAULTFFORMAT);
	else
		ffmt_ = Strdup(fmt);
}

////////////////////////////////////////////////////////////////
//	Helper routines that call the lower level ones.
// Set a variable to a new value.

void AvTcl::setIntVal(const char *varName, const int newValue,
		     const int flags, const char *format)
{char buf[128];
 const char *fmt = (format != NULL) ? format : getIformat();
	sprintf(buf, fmt, newValue);
	setVar(varName, buf, flags);
}

void AvTcl::setIntVal2(const char *name1, const char *name2,
		      const int newValue,
		      const int flags, const char *format)
{char buf[128];
 const char *fmt = (format != NULL) ? format : getIformat();
	sprintf(buf, fmt, newValue);
	setVar2(name1, name2, buf, flags);
}

// Return the value of a variable. Returns TCL_OK for success.
int AvTcl::getIntVal(const char *varName, int &value, const int flags)
{const char *var = getVar(varName, flags);

	if(var == NULL)
		return TCL_ERROR;
	return getInt(var, value);
}

int AvTcl::getIntVal2(const char *name1, const char *name2, int &value,
	      const int flags)
{const char *var = getVar2(name1, name2, flags);

	if(var == NULL)
		return TCL_ERROR;
	return getInt(var, value);
}

void AvTcl::setDoubleVal(const char *varName, const double newValue,
		     const int flags, const char *format)
{char buf[128];
 const char *fmt = (format != NULL) ? format : getDformat();
	sprintf(buf, fmt, newValue);
	setVar(varName, buf, flags);
}

void AvTcl::setDoubleVal2(const char *name1, const char *name2,
		      const double newValue,
		      const int flags, const char *format)
{char buf[128];
 const char *fmt = (format != NULL) ? format : getDformat();
	sprintf(buf, fmt, newValue);
	setVar2(name1, name2, buf, flags);
}

// Return the value of a variable. Returns TCL_OK for success.
int AvTcl::getDoubleVal(const char *varName, double &value, const int flags)
{const char *var = getVar(varName, flags);

	if(var == NULL)
		return TCL_ERROR;
	return getDouble(var, value);
}

int AvTcl::getDoubleVal2(const char *name1, const char *name2, double &value,
	      const int flags)
{const char *var = getVar2(name1, name2, flags);

	if(var == NULL)
		return TCL_ERROR;
	return getDouble(var, value);
}

void AvTcl::setDoubleList(const char *varName,
				 const AvWPosition &list,
				 const int flags, const char *format)const
{
	setDoubleList2(varName, NULL, list, flags, format);
}

void AvTcl::setDoubleList2(const char *name1, const char *name2,
		      const AvWPosition &list,
		      const int flags, const char *format)const
{char buf[128];
 const char *fmt = (format != NULL) ? format : getDformat();
 AvString value = "{ ";
 int length = list.nelements();

	for(int i=0; i< length; i++)
	{	sprintf(buf, fmt, list(i));
		value += buf;
		value += " ";
	}

	value += "}";
	if(name2 == NULL)
		setVar(name1, value.chars(), flags);
	else
		setVar2(name1, name2, value.chars(), flags);
}

void AvTcl::setDoubleList(const char *varName,
			  const int length, const double *list,
			  const int flags, const char *format)const
{
	setDoubleList2(varName, NULL, length, list, flags, format);
}

void AvTcl::setDoubleList2(const char *name1, const char *name2,
			   const int length, const double *list,
			   const int flags, const char *format)const
{char buf[128];
 const char *fmt = (format != NULL) ? format : getDformat();
#if 1
 AvString value = "{ ";

	if(list == NULL)		// Error.
		return;
	for(int i=0; i< length; i++)
	{	sprintf(buf, fmt, list[i]);
		value += buf;
		value += " ";
	}
	value += "}";

	if(name2 == NULL)
		setVar(name1, value.chars(), flags);
	else
		setVar2(name1, name2, value.chars(), flags);
#else
 AvString value;

	if(list == NULL)		// Error.
		return;
	for(int i=0; i< length; i++)
	{	sprintf(buf, fmt, list[i]);
cout <<		setVar2(name1, name2, buf,
			flags | TCL_LIST_ELEMENT | TCL_APPEND_VALUE) << endl;
	}

#endif
}

// Return a list of doubles. Returns TCL_OK for success.
// The length of "list" is set the the # of items in the list.
// If an error occurs during conversion, further conversion is stopped and
// the list is resized to hold only the valid values.
int AvTcl::getDoubleList(const char *varName, AvWPosition &list,
			 const int flags)const
{
	return getDoubleList2(varName, NULL, list, flags);
}

int AvTcl::getDoubleList2(const char *name1, const char *name2,
			 AvWPosition &list, const int flags)const
{const char *var = getVar2(name1, name2, flags);

	if(name2 == NULL)
		var = getVar(name1, flags);
	else
		var = getVar2(name1, name2, flags);

	if(var == NULL)
		return TCL_ERROR;
	// Split the list up.
	int length;
	char **lptr;
	splitList(var, length, &lptr);
	list.resize(length);
	for(int i=0; i < length; i++)
	{int rtn = getDouble(lptr[i], list(i));
		if(rtn != TCL_OK)
		{	list.resize(i+1);
			return rtn;
		}
	}
#if defined(__sparc) && !defined(__SVR4)
	free((char *)lptr);
#else
	free(lptr);
#endif
	return TCL_OK;
}

void AvTcl::setIntList( const char *varName,
			const AvIPosition &list,
			const int flags, const char *format)const
{
	setIntList2(varName, NULL, list, flags, format);
}

void AvTcl::setIntList2(const char *name1, const char *name2,
			const AvIPosition &list,
			const int flags, const char *format)const
{char buf[128];
 const char *fmt = (format != NULL) ? format : getIformat();
// AvString value = "{ ";
 AvString value = " ";
 int length = list.nelements();

	for(int i=0; i< length; i++)
	{	sprintf(buf, fmt, list(i));
		value += buf;
		value += " ";
	}

//	value += "}";
	if(name2 == NULL)
		setVar(name1, value.chars(), flags);
	else
		setVar2(name1, name2, value.chars(), flags);
}

void AvTcl::setIntList(const char *varName,
			  const int length, const int *list,
			  const int flags, const char *format)const
{
	setIntList2(varName, NULL, length, list, flags, format);
}

void AvTcl::setIntList2(const char *name1, const char *name2,
			const int length, const int *list,
			const int flags, const char *format)const
{char buf[128];
 const char *fmt = (format != NULL) ? format : getIformat();
// AvString value = "{ ";
 AvString value = " ";

	if(list == NULL)		// Error.
		return;
	for(int i=0; i< length; i++)
	{	sprintf(buf, fmt, list[i]);
		value += buf;
		value += " ";
	}
//	value += "}";

	if(name2 == NULL)
		setVar(name1, value.chars(), flags);
	else
		setVar2(name1, name2, value.chars(), flags);
}

// Return a list of ints. Returns TCL_OK for success.
// The length of "list" is set the the # of items in the list.
// If an error occurs during conversion, further conversion is stopped and
// the list is resized to hold only the valid values.
int AvTcl::getIntList(const char *varName, AvIPosition &list,
			 const int flags)const
{
	return getIntList2(varName, NULL, list, flags);
}

int AvTcl::getIntList2(const char *name1, const char *name2,
			 AvIPosition &list, const int flags)const
{const char *var;

	if(name2 == NULL)
		var = getVar(name1, flags);
	else
		var = getVar2(name1, name2, flags);

	if(var == NULL)
		return TCL_ERROR;
	// Split the list up.
	int length;
	char **lptr;
	splitList(var, length, &lptr);
	list.resize(length);
	for(int i=0; i < length; i++)
	{int rtn = getInt(lptr[i], list(i));
		if(rtn != TCL_OK)
		{	list.resize(i+1);
			return rtn;
		}
	}
#if defined(__sparc) && !defined(__SVR4)
	free((char *)lptr);
#else
	free(lptr);
#endif
	return TCL_OK;
}

void AvTcl::setBooleanVal(const char *varName, const int newValue,
				 const int flags)const
{char buf[128];

	const char *fmt = (newValue) ? getTformat() : getFformat();
	sprintf(buf, fmt, newValue);
	setVar(varName, buf, flags);
}

void AvTcl::setBooleanVal2(const char *name1, const char *name2,
		      const int newValue, const int flags)const
{char buf[128];
 const char *fmt = (newValue) ? getTformat() : getFformat();

	sprintf(buf, fmt, newValue);
	setVar2(name1, name2, buf, flags);
}

// Return the value of a variable. Returns TCL_OK if OK, else TCL_ERROR.
int AvTcl::getBooleanVal(const char *varName, int &value, const int flags)const
{const char *var = getVar(varName, flags);

	if(var == NULL)
		return TCL_ERROR;
	return getBoolean(var, value);
}

int AvTcl::getBooleanVal2(const char *name1, const char *name2, int &value,
			  const int flags)const
{const char *var = getVar2(name1, name2, flags);

	if(var == NULL)
		return TCL_ERROR;
	return getBoolean(var, value);
}

// Interface to Tcl_TraceVar that takes const char *.
int AvTcl::Tcl_TraceVar(Tcl_Interp *interp, const char *varname,
			const int flags,
			Tcl_VarTraceProc *proc, ClientData clientdata)
{
  char *vname = AvTcl::Strdup(varname);
  int rtn = ::Tcl_TraceVar(interp, vname, flags, proc, clientdata);
  delete [] vname;
  return rtn;
}

// Interface to Tcl_TraceVar that takes const char *.
int AvTcl::traceVar(const char *varname, const int flags,
		    Tcl_VarTraceProc *proc, ClientData clientdata)
{
  return AvTcl::Tcl_TraceVar(interp_, varname, flags, proc, clientdata);
}

Tcl_Command AvTcl::Tcl_CreateCommand(Tcl_Interp *interp, const char *cmdName,
			 Tcl_CmdProc *proc, ClientData clientData,
			 Tcl_CmdDeleteProc *deleteProc)
{
  char *cn = AvTcl::Strdup(cmdName);
  Tcl_Command cmd = ::Tcl_CreateCommand(interp, cn, proc, clientData,
					deleteProc);
  delete [] cn;
  return cmd;
}

void AvTcl::Tcl_CreateMathFunc(Tcl_Interp *interp, const char *name,
			  const int numArgs, Tcl_ValueType *argTypes,
			  Tcl_MathProc *proc, ClientData clientData)
{
  char *n = AvTcl::Strdup(name);
  ::Tcl_CreateMathFunc(interp, n, numArgs, argTypes, proc, clientData);
  delete [] n;
}

// sets interp->result to the char string.
void AvTcl::Tcl_SetResult(Tcl_Interp *interp, const char *result)
{
  if(result != NULL) {
    char *str = AvTcl::Strdup(result);
    ::Tcl_SetResult(interp, str, TCL_VOLATILE);
    delete [] str;
  } else
    ::Tcl_SetResult(interp, NULL, TCL_VOLATILE);
}


////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
///			Hash Tables
////////////////////////////////////////////////////////////////

// If the constuctor w/o args is used, what kind of table should be created?
static int defaultKeyType_ = TCL_ONE_WORD_KEYS;

int AvTclHashTable::defaultKeyType()
{
	return defaultKeyType_;
}

void AvTclHashTable::defaultKeyType(const int keyType)
{
	defaultKeyType_ = keyType;
}

// keyType is either: TCL_STRING_KEYS, TCL_ONE_WORD_KEYS
// or an integer > 1.
AvTclHashTable::AvTclHashTable()
{
	initialized_ = 0;
	init(defaultKeyType_);
}

AvTclHashTable::AvTclHashTable(const int keyType)
{
	initialized_ = 0;
	init(keyType);
}

AvTclHashTable::~AvTclHashTable()
{	Tcl_DeleteHashTable(&table_);
}

// Initialize the hash table to use keyType. If the table has already
// been initialized once, clear it first.
void AvTclHashTable::init(const int keyType)
{
	keyType_ = keyType;
	if(initialized_)
		clear();
	else
	{	Tcl_InitHashTable(&table_, keyType);
		searchValid_ = 0;		// Can't use findNext().
		initialized_ = 1;
	}
}

void AvTclHashTable::clear()
{
	Tcl_DeleteHashTable( &table_);	// Delete all entries.
	initialized_ = 0;
	init(keyType_);			// Reset.
}

Tcl_HashEntry *AvTclHashTable::createHashEntry(char *key, int & newEntry)
{Tcl_HashEntry *entry;

	entry = Tcl_CreateHashEntry(&table_, key, &newEntry);
	if(newEntry == 1)
		searchValid_ = 0;
	return entry;
}

// Returns NULL if entry doesn't exist.
Tcl_HashEntry *AvTclHashTable::findHashEntry(char *key)
{
	return Tcl_FindHashEntry( &table_, key);
}

Tcl_HashEntry *AvTclHashTable::findHashEntry(const char *key)
{ char *k = AvTcl::Strdup(key);
	Tcl_HashEntry *e = findHashEntry(k);
	delete [] k;
	return e;
}

void AvTclHashTable::deleteHashEntry(Tcl_HashEntry *entry)
{
	if(entry != NULL)
	{	Tcl_DeleteHashEntry(entry);
		searchValid_ = 0;
	}
}


ClientData AvTclHashTable::getHashValue(Tcl_HashEntry *entry)
{
	return Tcl_GetHashValue(entry);
}

void AvTclHashTable::setHashValue(Tcl_HashEntry *entry, ClientData value)
{
	Tcl_SetHashValue(entry, value);
}

char *AvTclHashTable::getHashKey(Tcl_HashEntry *entry)
{
	return Tcl_GetHashKey(&table_, entry);
}


Tcl_HashEntry *AvTclHashTable::firstHashEntry(Tcl_HashSearch *searchPtr)
{
	searchValid_ = 1;
	return Tcl_FirstHashEntry( &table_, searchPtr);
}

Tcl_HashEntry *AvTclHashTable::nextHashEntry(Tcl_HashSearch *searchPtr)
{
	if(searchValid_)
		return Tcl_NextHashEntry(searchPtr);
	else
		return NULL;
}


// Printable string giving usage statistics.
// Use free to free returned value.
char *AvTclHashTable::hashStats()
{
	return Tcl_HashStats( &table_);
}

////////////////////////////////
// Helper functions that do common operations.
// Given a key, return a value.
// Returns 1 if entry was found, else 0.
int AvTclHashTable::getValue(char *key, ClientData &value)
{ Tcl_HashEntry *entry = findHashEntry(key);

	if(entry == NULL)
		return 0;
	value = getHashValue(entry);
	return 1;
}

int AvTclHashTable::getValue(const char *key, ClientData &value)
{ char *k = AvTcl::Strdup(key);
	int rtn = getValue(k, value);
	delete [] k;
	return rtn;
}

// Returns 1 if entry was created, else 0.
int AvTclHashTable::setValue(char *key, ClientData value)
{ int rtn;

	Tcl_HashEntry *entry = createHashEntry(key, rtn);
	setHashValue(entry, value);
	return rtn;
}

int AvTclHashTable::setValue(const char *key, ClientData value)
{ int rtn;

	char *s = AvTcl::Strdup(key);
	rtn = setValue(s, value);
	delete [] s;
	return rtn;
}

ClientData AvTclHashTable::firstValue()
{
	Tcl_HashEntry *entry = firstHashEntry(&search);
	if(entry == NULL)
		return NULL;
	return getHashValue(entry);
}

ClientData AvTclHashTable::nextValue()
{
	Tcl_HashEntry *entry = nextHashEntry(&search);
	if(entry == NULL)
		return NULL;
	return getHashValue(entry);
}

#endif
