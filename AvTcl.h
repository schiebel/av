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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvTcl.h,v 19.1 2004/02/04 15:27:24 gvandiep Exp $
//
// $Log: AvTcl.h,v $
// Revision 19.1  2004/02/04 15:27:24  gvandiep
// Fixed for tcl-8.4
//
// Revision 19.0  2003/07/16 05:47:56  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:32  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:43:00  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:06  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:29  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:31:24  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:09:02  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:52  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:11  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:57  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:56:09  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:31:16  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:19:10  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1996/12/19 17:00:40  droberts
// Undefined "CONST" macro defined in tcl.h to prevent problems on some systems
// when Glish is enabled.
//
// Revision 7.2  1996/12/12 22:01:45  droberts
// Added ifdef TCL around entire file.
//
// Revision 7.1  1996/12/12 00:21:56  droberts
// Initial checkin from monet.
//
// Revision 1.1  1996/11/05 21:16:27  hr
// Initial revision
//
//
//---------------------------------------------------------------------------
/* Tcl.h
C++ wrapper for tcl.
Only a relatively few library commands are available.
*/

#ifdef TCL

#ifndef TCL_H
#define TCL_H

#include <unistd.h>
#include <AvString.h>
#include <AvWPosition.h>
#include <AvIPosition.h>
//# Use tcl8.4 in non-const mode (to be able to use tcl 8.3 as well)
#define USE_NON_CONST
#include <tcl.h>

// tcl.h defines "CONST" which may cause problems when Glish is turned on.
#ifdef CONST
#undef CONST
#endif

/*
NOTES:
	Most functions that return "const char *" are returning a pointer
to the result string. This value is volitile and may change on the next
Tcl call. (ie. Copy the string if it is desired to keep it around).
*/

class AvTcl {
  public:
	AvTcl();
	virtual ~AvTcl();
	////////////////////////////////////////////////////////////////
	//		Eval (pg 288)
	int eval(char *script);
	int eval(const char *script);
	int eval(const AvString &script);
	int evalFile(char * fileName);
	int evalFile(const char * fileName);
	int evalFile(const AvString &fileName);
	int globalEval(char *script);
	int globalEval(const char *script);
	int globalEval(const AvString &script);
	// Low budget version of varEval.
	int varEval(const char *s1, const char *s2=NULL, const char *s3=NULL,
		    const char *s4=NULL, const char *s5=NULL,
		    const char *s6=NULL, const char *s7=NULL,
		    const char *s8=NULL, const char *s9=NULL)const;
	////////////////////////////////////////////////////////////////
	//		get (Value)	(pg 314)
	// (Convert a string to a number).
	// Return is either TCL_OK or TCL_ERROR.
	int getInt(char *string, int &value)const;
	int getInt(const char *string, int &value)const;
	int getInt(const AvString &string, int &value)const;
	int getDouble(char *string, double &value)const;
	int getDouble(const char *string, double &value)const;
	int getDouble(const AvString &string, double &value)const;
	int getBoolean(char *string, int &value)const;
	int getBoolean(const char *string, int &value)const;
	int getBoolean(const AvString &string, int &value)const;

	int exprString(char *string)const;
	int exprString(const char *string)const;
	int exprString(const AvString &string, AvString &value)const;
	int exprLong(char *string, long &value)const;
	int exprDouble(char *string, double &value)const;
	////////////////////////////////////////////////////////////////
	//		get/setVar	(pg 326)
#if 0
	const char *setVar(char *varName, char *newValue,
			   const int flags=0)const;
#endif
	static char *Tcl_SetVar(Tcl_Interp *, const char *varName,
				      const char *newValue,
				      const int flags=0);
	const char *setVar(const char *varName, const char *newValue,
			   const int flags=0)const;
	static char *Tcl_SetVar2(Tcl_Interp *interp, const char *name1,
				 const char *name2, const char *newvalue,
				 const int flags);
	const char *setVar2(char *name1, char *name2, char *newValue,
			    const int flags=0)const;
	const char *setVar2(const char *name1, const char *name2,
			    const char *newValue, const int flags=0)const;
	const char *getVar(char *varName, const int flags=0)const;
	const char *getVar(const char *varName,	const int flags=0)const;
	const char *getVar2(char *name1, char *name2, const int flags=0)const;
	const char *getVar2(const char *name1, const char *name2,
			    const int flags=0)const;
	// Interface to Tcl_TraceVar that takes const char *.
	static int Tcl_TraceVar(Tcl_Interp *, const char *varname,
				const int flags,
				Tcl_VarTraceProc *proc, ClientData clientdata);
	static Tcl_Command Tcl_CreateCommand(Tcl_Interp *interp,
					     const char *cmdName,
					     Tcl_CmdProc *proc,
					     ClientData clientData,
					     Tcl_CmdDeleteProc *deleteProc);
	static void Tcl_CreateMathFunc(Tcl_Interp *interp, const char *name,
				       const int numArgs,
				       Tcl_ValueType *argTypes,
				       Tcl_MathProc *proc,
				       ClientData clientData);

	// sets interp->result to the char string.
	static void Tcl_SetResult(Tcl_Interp *interp, const char *result);

	int traceVar(const char *varname, const int flags,
		     Tcl_VarTraceProc *proc, ClientData clientdata);
	////////////////////////////////////////////////////////////////
	int splitList(char *list, int &argc, char ***argPtr)const;
	int splitList(const char *list, int &argc, char ***argPtr)const;
	int splitList(const AvString &list, int &argc, AvString **argPtr)const;

	char *merge(const int argc, char **argv)const;
	//AvString mergeList(const int argc, const AvString *argv);
	////////////////////////////////////////////////////////////////
	//	Helper routines that call the lower level ones.
	// Set/Get the value of a variable.
	// varName is the name of the variable to be set.
	// name1 is the name of the array and name2 is the index.
	// flags is the tcl flags variable for set/get.
	// format is the format string to be used for the sprintf. A default
	// will be used if it is NULL.
	void setIntVal(const char *varName, const int newValue,
		       const int flags=0, const char *format=NULL);
	void setIntVal2(const char *name1, const char *name2,
			const int newValue,
			const int flags=0, const char *format=NULL);
	// Return the value of a variable. Returns TCL_OK for success.
	int getIntVal(const char *varName, int &value, const int flags=0);
	int getIntVal2(const char *name1, const char *name2, int &value,
		       const int flags=0);

	void setDoubleVal(const char *varName, const double newValue,
			  const int flags=0, const char *format=NULL);
	void setDoubleVal2(const char *name1, const char *name2,
			   const double newValue,
			   const int flags=0, const char *format=NULL);
	// Return the value of a variable. Returns TCL_OK for success.
	int getDoubleVal(const char *varName, double &value,
			 const int flags=0);
	int getDoubleVal2(const char *name1, const char *name2, double &value,
			  const int flags=0);

	void setDoubleList(const char *varName,
			   const AvWPosition &value,
			   const int flags=0, const char *format=NULL)const;
	void setDoubleList2(const char *name1, const char *name2,
			    const AvWPosition &value,
			    const int flags=0, const char *format=NULL)const;
	void setDoubleList(const char *varName,
			  const int length, const double *values,
			  const int flags=0, const char *format=NULL)const;
	void setDoubleList2(const char *name1, const char *name2,
			    const int length, const double *values,
			    const int flags=0, const char *format=NULL)const;

	void setIntList(const char *varName,
			const AvIPosition &value,
			const int flags=0, const char *format=NULL)const;
	void setIntList2(const char *name1, const char *name2,
			 const AvIPosition &value,
			 const int flags=0, const char *format=NULL)const;
	void setIntList(const char *varName,
			const int length, const int *values,
			const int flags=0, const char *format=NULL)const;
	void setIntList2(const char *name1, const char *name2,
			 const int length, const int *values,
			 const int flags=0, const char *format=NULL)const;

	// Return the value of a variable. Returns TCL_OK for success.
	int getDoubleList(const char *varName, AvWPosition &value,
			  const int flags=0)const;
	int getDoubleList2(const char *name1, const char *name2,
			   AvWPosition &value, const int flags=0)const;

	int getIntList( const char *varName, AvIPosition &value,
			const int flags=0)const;
	int getIntList2(const char *name1, const char *name2,
			AvIPosition &value, const int flags=0)const;

	void setBooleanVal(const char *varName, const int newValue,
				  const int flags=0)const;
	void setBooleanVal2(const char *name1, const char *name2,
				   const int newValue, const int flags=0)const;
	// Return the value of a variable. Returns 1 if OK, else 0.
	int getBooleanVal(const char *varName, int &value,
			 const int flags=0)const;
	int getBooleanVal2(const char *name1, const char *name2, int &value,
			  const int flags=0)const;

	////////////////////////////////////////////////////////////////
	//		Tcl "internal" functions
	// List of index names for the array.
	const char *arrayNames(const char *arrayName)const;
//	const char *arrayNames(char *arrayName)const;
	// # Of elements in the array.
	int arraySize(const char *arrayName)const;
	//	Routines to iterate through the list of array elements.
	// If arrayName is not a valid array, the results are undefined.
	// Initialize a search and return the searchID.
	AvString arrayStartSearch(const char *arrayName)const;
	// Returns 1 if there are more elements, otherwise 0.
	int arrayAnymore(const char *arrayName, const AvString &searchID)const;
	// Terminate search and reclaims (internal) space used for searchID.
	void arrayDoneSearch(const char *arrayName,
			     const AvString &searchID)const;
	const char *nextElement(const char *arrayName,
				const AvString &searchID)const;

	// Does the file exist?
	int fileExists(char *fileName);
	int fileExists(const char *fileName);
	////////////////////////////////////////////////////////////////
	////		Misc
	// Convert between Tcl codes and their strings.
	enum RETURNCODES { ERROR, FENCE};
	static const char *codeToName(const RETURNCODES id, const int code);
	static int nameToCode(const RETURNCODES id, const char *name);
	Tcl_Interp *interp()const{return interp_;}
	const char *result()const;

	const char *getIformat()const{return ifmt_;}
	void setIformat(const char *fmt=NULL);
	const char *getDformat()const{return dfmt_;}
	void setDformat(const char *fmt=NULL);
	const char *getTformat()const{return tfmt_;}
	const char *getFformat()const{return ffmt_;}
	void setTformat(const char *fmt=NULL);
	void setFformat(const char *fmt=NULL);

	static char *Strdup(const char *s);
  protected:
  private:
	Tcl_Interp	*interp_;
	char		*ifmt_;		// Default format string for ints.
	char		*dfmt_;		// Default format string for doubles.
	char		*tfmt_;		// Default format string for True.
	char		*ffmt_;		// Default format string for False
};

// See pg. 338 ff.
// Note: When saving strings, remember that what is saved is a pointer to
//	 the string. So the string needs to stay around until it is removed.

class AvTclHashTable {
  public:
	// keyType is either: TCL_STRING_KEYS, TCL_ONE_WORD_KEYS
	// or an integer > 1.
	AvTclHashTable(const int keyType);
	// Creates a hash table using defaultKeyType().
	AvTclHashTable();
	virtual ~AvTclHashTable();

	////////////////////////////////////////////////
	// Functions that do common operations.
	// These are built on the lower level ones below.

	// Given a key, return/set a value.
	// Returns 1 if entry was found, else 0.
	int getValue(const char *key, ClientData &value);
	int getValue(char *key, ClientData &value);
	// Returns 1 if entry was created, else 0.
	int setValue(char *key, ClientData value);
	int setValue(const char *key, ClientData value);

	// Search through table. nextEntry should not be called before
	// firstEntry.
	ClientData firstValue();
	// Returns NULL when no more entries.
	ClientData nextValue();

	// Deletes the table then reinitializes it.
	void clear();
	// Initializes a table to use the given keyType. Existing entries
	// will be deleted.
	void init(const int keyType);
	// Set/get the keyType to use if the constructor w/o args is used.
	static int defaultKeyType();
	static void defaultKeyType(const int keyType);
	////////////////////////////////////////////////

	// newEntry will be set to 1 if entry was created, 0 if it already
	// existed.
	Tcl_HashEntry *createHashEntry(char *key, int & newEntry);
	// Returns NULL if entry doesn't exist.
	Tcl_HashEntry *findHashEntry(char *key);
	Tcl_HashEntry *findHashEntry(const char *key);
	void deleteHashEntry(Tcl_HashEntry *entry);

	static ClientData getHashValue(Tcl_HashEntry *entry);
	static void setHashValue(Tcl_HashEntry *entry, ClientData value);
	char *getHashKey(Tcl_HashEntry *entry);

	Tcl_HashEntry *firstHashEntry(Tcl_HashSearch *searchPtr);
	Tcl_HashEntry *nextHashEntry(Tcl_HashSearch *searchPtr);

	// Printable string giving usage statistics.
	// Use free to free returned value.
	char *hashStats();
  private:
	int		keyType_;
	Tcl_HashTable	table_;
	Tcl_HashSearch	search;
	int		searchValid()const{return searchValid_;}
	int		searchValid_;
	int		initialized_;
};

#endif

#endif
