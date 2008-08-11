//# Copyright (C) 1995-98 Board of Trustees of the University of Illinois
//#
//# This software, both binary and source, is copyrighted by The
//# Board of Trustees of the University of Illinois.  Ownership
//# remains with the University.  You should have received a copy 
//# of a licensing agreement with this software.  See the file 
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvUnits.h,v 19.0 2003/07/16 05:46:48 aips2adm Exp $
//
// $Log: AvUnits.h,v $
// Revision 19.0  2003/07/16 05:46:48  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:34  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:02  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:11  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:52  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:07:50  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:34  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:53  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:41  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:20  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/08 21:50:24  hr
// Removed addUnits() since it wasn't declared as returning a value and it wasn't implemented.
//
// Revision 9.0  1997/08/25 21:25:16  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:25  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 08:40:35  droberts
// Final update from monet archive.
//
// Revision 1.4  1996/12/11 20:40:12  hr
// Added RCS header and copyright.
//
//
//---------------------------------------------------------------------------

#ifndef __AvUnits_h__
#define __AvUnits_h__

#include "AvList.h"
#include "AvString.h"
#include "AvStringHashTable.h"

class AvUnits;

//
// <summary>
// Stores the representation of a prefix database entry
// </summary>
//
// <synopsis>
// <ul>
// <li>name - stores the name of the prefix (e.g., "kilo")
// <li>prefix  - stores the abbreviated form of the prefix (e.g., "k")
// <li>multiplier - stores the multiplier for the prefix (e.g., 1e3)
// </ul>
// </synopsis>
//

class AvUnitPrefixDef
{
public:
  AvUnitPrefixDef(const AvString & p_name,
		  const AvString & p_prefix,
		  const double p_mult)
    : name(p_name), prefix(p_prefix), multiplier(p_mult) {}

  AvString name;
  AvString prefix;
  double multiplier;  // to go from units to prefixunits
};

//
// <summary>
// Stores the representation of a unit definition entry 
// </summary>
//
// <synopsis>
// <ul>
// <li>measurementName - stores name of measurement (e.g., "time")
// <li>unitName     - stores name of units (e.g., "hours")
// <li>unitAbbrev   - stores string for abbreviation (e.g., "h")
// <li>allowedPrefixes - uint bitfield with '1' entries for each allowed prefix
// <li>multFactor   - multiplication factor based on the first (canonical) entry
//                added to the time table (e.g., 3600 canonical seconds to an hour)
// <li>canon        - pointer to canonical type entry
// </ul>
// </synopsis>
//

class AvUnitDef
{
public:
  AvUnitDef(const AvString & p_measurementName,
	    const AvString & p_unitName,
	    const AvString & p_unitAbbrev,
	    unsigned int p_allowedPrefixes,
	    double p_multFactor)
    : measurementName(p_measurementName), 
      unitName(p_unitName), 
      unitAbbrev(p_unitAbbrev),
      allowedPrefixes(p_allowedPrefixes),
      multFactor(p_multFactor) {}

  AvString measurementName;
  AvString unitName;
  AvString unitAbbrev;
  unsigned int allowedPrefixes;
  double multFactor;

  class AvUnitDef * canon;
};

//  <summary>
//  Units Translator - Utility class for providing unit conversions
//  between values and for printing values nicely.
//  </summary>
//
//  <synopsis>
//
//  AvUnits is a class that references an extendable database of unit 
//  conversion information to provide unit-translating services.
//
//  When created, AvUnits reads its builtin table which includes perhaps incomplete
//  measurement
//  tables for distance, time, right ascension, declination, longitude, lattitude,
//  velocity, frequency, specific intensity, magnetic field strength, mass, 
//  charge.
//
//  The units parameters are strings which are interpreted by the
//  class.  AvUnits knows how to recognized and apply prefixes such
//  as 'milli-', 'micro-', and can also understand some phrases that describe
//  units, such as "billion lightyears".
//
//  This class provides two fundamental services
//  <ol>
//  <li> Converting values between (e.g. meters to parsecs)
//  <li> Returning nicely formatted strings with units attached
//  </ol>
//
//  The former service is handled by the various <b>convertValue()</b>
//  functions and is used to interconvert values between units.
//
//  The latter is handled by a mechanism which adjusts the value and applies
//  allowed prefixes make the value as presentable as possible.
//
//  This class makes extensive use of <linkto class=AvString>AvString</linkto>s
//  for its input and output parameters.  For const AvString parameters, you 
//  can pass a char * and it will be implicitly converted.  For the return
//  values, the <b>chars()</b> string member function will provide a pointer
//  to the internal array.
//
//  This class also uses <linkto class=AvRegex>Regex</linkto> in the 
//  interpretation of the input strings.
//  
//  This class also makes extensive use of <linkto class=AvStringHashTable>
//  StringHashTable</linkto> specialized hashTable class for character arrays.
//
//  </synopsis>
//
//  <example>
//  <srcBlock>
//  AvUnits::initClass();
//  ...
//  double nLyInKpc;
//  err  = AvUnits::convertValue(1.0, "kiloparsecs", "lightyears", nLyInKpc);
//  ...
//  AvUnits::destroyClass();
//  </srcBlock>
//  In this case AvUnits recognizes the kilo prefix, and knows that parsecs is
//  a measurement of distance.  It finds lightyears in the distance table, generates
//  a conversion factor between parsecs and lightyears, and combines it with
//  the knowledge of kilo to store in nLyInKpc the number of lightyears in a 
//  kiloparsec.
//  <p>
//  Occasionally the name of the units is ambiguous, so AvUnits must be told
//  what type of measurement you are talking about.
//
//  <srcBlock>
//  AvUnits::initClass();
//  ...
//  double arcminutes;
//  int err = avu.convertValue(30.0, "degrees", "arcminutes", arcminutes, "right ascension";
//  ...
//  AvUnits::destroyClass();
//  </srcBlock>
//  </example>
//
//  When created, AvUnits reads its builtin table which includes perhaps incomplete
//  measurement
//  tables for distance, time, right ascension, declination, longitude, lattitude,
//  velocity, frequency, specific intensity, magnetic field strength, mass, 
//  charge.
//
//  <todo asof="96/08/01">
//  <li>none noted
//  </todo>
//
//  </synopsis>
//  
//  

class AvUnits
{
public:

  //
  // Define bit positions for the suite of possible prefixes.  Each
  // UnitDef defines their own set of allowed prefixes.  Don't want to use
  // enum here because of enum --> int conversion probs.
  //
  // PICO-PETA apply "p,n,u,m,c,j,M,G,T,P" to the return string, respectively.
  // THOUSAND-TRILLION prefix "thousand ,million, billion, trillion ", respectively.
  //
  // <group>
  static const unsigned int PICO;
  static const unsigned int NANO;
  static const unsigned int MICRO;
  static const unsigned int MILLI;
  static const unsigned int CENTI;
  static const unsigned int KILO;
  static const unsigned int MEGA;
  static const unsigned int GIGA;
  static const unsigned int TERA;
  static const unsigned int PETA;
  static const unsigned int THOUSAND;
  static const unsigned int MILLION;
  static const unsigned int BILLION;
  static const unsigned int TRILLION;
  static const unsigned int N_PREFIXES;
  // </group>

  // Class initialization - must be called prior to using AvUnits
  static void initClass();

  // Class destruction - call to free memory used by AvUnits
  static void destroyClass();

  // split units into prefix and base type
  static void pbsplit(const AvString & units,
		      AvString & prefix,
		      AvString & base);

  // Try to abbreviate the given units string
  static AvString abbreviate(const AvString & units);

  // return val rounded to ndigits significant figures.
  static double roundToNDigits(const double val,
			int ndigits);

  // return val rounded to ndigits significant figures, and
  // set strVal to the string representation.
  static double roundToNDigits(const double val,
			int ndigits,
			AvString & strVal);
  
private:
  static int convertValue_(const double valueIn,
			   AvUnitPrefixDef * updi,
			   AvUnitDef * udi,
			   AvUnitPrefixDef * updo,
			   AvUnitDef * udo,
			   double & valueOut);
public:

  // Return the name of the units that would be most convenient
  // to print valueIn.
  static int queryUnits(const double valueIn,
			const AvString& unitsIn,
			const AvString& measurementName,
			AvString& unitsOut);

  // search the database for information about a certain unit name
  // not abbreviation
  static int lookup(const AvString& unknownUnitsIn,
		    AvString& prefixOut,
		    AvString& baseOut,
		    AvString& typeOut,
		    double& multiplierOut);

  // convert units using explicit types
  static int convertValue(const double valueIn,
			  const AvString& unitsIn,
			  const AvString& unitsOut,
			  double & valueOut,
			  const AvString& measurementName);
  
  // convert units using a specific base type.  Allow
  // the return type to vary by prefix
  static int convertToBaseValue(const double valueIn,
			 const AvString& unitsIn,
			 const AvString& baseUnitsOut,
			 double & valueOut,
			 AvString& abbrevUnitsOut,
			 AvString& unitsOut,
			 const AvString& measurementName);

  // create a nice string that identifies the value
  // using the given units.  AvUnits will use the base
  // units given, but may replace the prefix units as 
  // appropriate.  For example, if 10,000 kilohertz is
  // passed in, the prefix will be changed from kilo to mega in
  // the returned string.  nsigfigs is the number of significant
  // digits to put in the return string.  measurement is the
  // name of the axis to help resolve ambiguities. 
  //
  // <group>
  static AvString makeNiceString(const double value,
			  const AvString& units)
  { return makeNiceString(value, units, 4); }
  static AvString makeNiceString(const double value,
			  const AvString& units,
			  int nsigfigs);
  static AvString makeNiceString(const double value,
			  const AvString& units,
			  int nsigfigs,
			  const AvString& measurementName);
  // </group>

  // This string also creates a nice string similar to the
  // one above, but allows the user to specify a different
  // units than the one corresponding to the value.
  // <group>
  static AvString makeNiceString(const double value,
			  const AvString& units,
			  const AvString& preferredUnits)
  { return makeNiceString(value, units, preferredUnits, 4); }
  static AvString makeNiceString(const double value,
			  const AvString& units,
			  const AvString& preferredUnits,
			  int nsigfigs);
  static AvString makeNiceString(const double value,
			  const AvString& units,
			  const AvString& preferredUnits,
			  int nsigfigs,
			  const AvString& measurementName); 
  // </group>

#if 0
  // (Needs a return type if implemented)
  // add a units definition into the database
  static addUnits(const AvString & measurementName,
	   const AvString & abbrev,
	   const AvString & allowedPrefixes, // list of allowed prefixes
	   double canonicalMultFactor,        // canonical -> this unit
	   const AvString & unitName,
	   const AvString & referenceType = "");
#endif

  // for debugging ...
  // <group>
  static void dump();
  static void dumpUnitDef(AvUnitDef *);
  // </group>

#if 0
  // Add a unit definition to the system
  static AvUnitDef * TNadd(const AvString & measurementName,
		    const AvString & unitName,
		    const AvString & abbrev,
		    const AvString & allowedPrefixes, // list of allowed prefixes
		    double canonicalMultFactor);  
#endif

protected:
  AvUnits() {}
private:
  
  // Disallow copy, assign, ctor, dtor
  AvUnits(const AvUnits & ) {}
  void operator = (const AvUnits &) {}
#if 0
  ~AvUnits() {}
#endif

  // 1 if initialized, 0 if not
  static int initialized_;

#if 0
  // future optimization for hash table lookup
  static AvString makeNiceString_(const double value,
			   AvUnitPrefixDef * upd,
			   AvUnitDef * ud,
			   int nsigfigs);
#endif

  // add a unit definition to the system
  static void TNadd(const AvString & measurementName,
	     const AvString & unitName,
	     const AvString & abbrev,
	     unsigned int allowedPrefixes,
	     double canonicalMultFactor);
  
  // Given a measurementName and a unitName
  static AvUnitDef * TNsearch(const AvString & measurementName,
		       const AvString & unitName);

  // (measurementName) --> AvUnitDef * for canonical AvUnitDef *
  static AvStringHashTable<AvUnitDef *> * measurementName2canonicalUnitDef_;
  static AvList<AvUnitDef *> canonicalUnitDefList_;

  // This is the major table of tables used by the class
  // 
  // (measurementName, baseUnitName) --> AvUnitDef *
  static AvStringHashTable<AvStringHashTable<AvUnitDef *> *> * unitsTable_;
  static AvList<AvStringHashTable<AvUnitDef *> *> utList_;
  static AvList<AvUnitDef *> unitDefList_;

  // This goes from base type to axis type name and works for
  // all but ambiguous cases
  static AvStringHashTable<AvString> * baseName2measurementName_;
  static AvStringHashTable<int> * baseName2measurementNameCount_;

  // array of coordinate axis types
  static AvStringHashTable<int> * measurementName2id_;
  static int axisIDCounter_;

  // hash table of known prefixes and list 
  static AvStringHashTable<AvUnitPrefixDef * > * name2unitPrefixDef_;
  static AvList<AvUnitPrefixDef * > unitPrefixDefList_; // need?
  static AvUnitPrefixDef * unitPrefixDefArray_[40];
};

#endif
