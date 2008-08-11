//# Copyright (C) 1995-2002 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvUnits.cc,v 19.0 2003/07/16 05:46:38 aips2adm Exp $
//
// $Log: AvUnits.cc,v $
// Revision 19.0  2003/07/16 05:46:38  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:24  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.4  2002/01/23 22:12:22  hravlin
// Modified roundToNDigits(,,string) to use 'G' format when creating a string.
//
// Revision 17.3  2002/01/14 20:43:03  hravlin
// Replaced the return for IRIX compilers only since ours complains if
// the function abbreviate doesn't return a value.
//
// Revision 17.2  2002/01/14 17:51:14  hravlin
// Removed unaccessible return from abbreviate().
//
// Revision 17.1  2002/01/07 22:54:02  hravlin
// Added __APPLE_ to NOIEEEFP check.
//
// Revision 17.0  2001/11/12 19:41:53  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:03  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.1  2001/03/21 17:34:16  hravlin
// Fixed some ambiguous arguments in calls to fmod() & pow().
//
// Revision 15.0  2000/10/26 17:08:16  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:27:35  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:07:38  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:22  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:33  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:10  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.1  1998/08/20 15:13:16  hr
// ieeefp.h was being included under SunOS 4.1.3
//
// Revision 10.0  1998/07/20 17:51:44  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.3  1998/06/08 21:40:58  hr
// If roundToNDigits() is passed NaN, it returns "NaN".
//
// Revision 9.2  1997/12/17 21:52:00  hr
// Changes to remove warnings from MIPSpro.7x compiler.
//
// Revision 9.1  1997/12/11 22:21:53  hr
// Limit the maximum number of digits in the string created by roundToNDigits().
//
// Revision 9.0  1997/08/25 21:24:19  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.3  1997/07/15 21:28:55  droberts
// Changed ctime_r call to ctime to get around ambiguity in number of
// arguments (2 or 3) to ctime_r.  Removed definition of ctime_r for Sun4.
//
// Revision 8.2  1997/06/11 18:55:06  hr
// convertValue_() was checking updo, but referencing updi.
//
// Revision 8.1  1997/05/07 15:54:19  hr
// Changes for DEC alpha.
//
// Revision 8.0  1997/02/20 03:15:03  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.5  1996/12/18 16:38:10  droberts
// Fixed ctime_r and ftunc definitions so they work on systems other than SGI and Sun.
//
// Revision 7.4  1996/12/12 09:48:54  droberts
// Final update from monet archive.
//
// Revision 1.4  1996/12/11 20:40:12  hr
// Added RCS header and copyright.
//
//
//---------------------------------------------------------------------------
//
//
//

#include <math.h>
#if defined(__linux__) || defined(__hpux__) || defined(__alpha__)
#define NOIEEEFP
#endif

#if defined(__APPLE__)
#define NOIEEEFP
#endif
// (About the only places it seems to be defined are IRIX & Solaris).
#if (defined(__sgi) || (__GNUC__)) && !defined(NOIEEEFP)
#include <ieeefp.h>
// isnan doesn't seem to get defined under 5.3 (it appears to under 5.2)
#define isnan(x) isnand((x))
#endif

#include <time.h>

// ftrunc doesn't exist under POSIX
static inline float Avftrunc(const float x)
{ double xd = x;
	return (float)(xd - fmod(xd, 1.0));
}

#include "AvString.h"
#include "AvUnits.h"


// bitfield entries ...
const unsigned int AvUnits::PICO      = 0x0001;
const unsigned int AvUnits::NANO      = 0x0002;  
const unsigned int AvUnits::MICRO     = 0x0004;
const unsigned int AvUnits::MILLI     = 0x0008;
const unsigned int AvUnits::CENTI     = 0x0010;
const unsigned int AvUnits::KILO      = 0x0020;
const unsigned int AvUnits::MEGA      = 0x0040;
const unsigned int AvUnits::GIGA      = 0x0080; 
const unsigned int AvUnits::TERA      = 0x0100;
const unsigned int AvUnits::PETA      = 0x0200;
const unsigned int AvUnits::THOUSAND  = 0x0400;
const unsigned int AvUnits::MILLION   = 0x0800;
const unsigned int AvUnits::BILLION   = 0x1000;
const unsigned int AvUnits::TRILLION  = 0x2000;
const unsigned int AvUnits::N_PREFIXES = 14;

int AvUnits::initialized_ = 0;

AvStringHashTable<AvUnitDef *> * AvUnits::measurementName2canonicalUnitDef_;
AvList<AvUnitDef *> AvUnits::canonicalUnitDefList_;
AvStringHashTable<AvStringHashTable<AvUnitDef *> *> * AvUnits::unitsTable_;
AvList<AvStringHashTable<AvUnitDef *> *> AvUnits::utList_;
AvList<AvUnitDef *> AvUnits::unitDefList_;
AvStringHashTable<AvString> * AvUnits::baseName2measurementName_;
AvStringHashTable<int> * AvUnits::baseName2measurementNameCount_;
AvStringHashTable<int> * AvUnits::measurementName2id_;
int AvUnits::axisIDCounter_;
AvStringHashTable<AvUnitPrefixDef * > * AvUnits::name2unitPrefixDef_;
AvList<AvUnitPrefixDef * > AvUnits::unitPrefixDefList_;
AvUnitPrefixDef * AvUnits::unitPrefixDefArray_[40];

struct AvUnitPrefixDef_s
{
  const char * name;
  const char * prefix;
  double multiplier;
};

struct AvUnitPrefixDef_s builtinPrefixes[] = 
{
  { "pico","p",1e-12 },
  { "nano","n",1e-9 },
  { "micro","u",1e-6 },
  { "milli","m",1e-3 },
  { "centi","c",1e-2 },
  { "kilo", "k",1e3 },
  { "mega", "M",1e6 },
  { "giga", "G",1e9 },
  { "tera", "T",1e12 },
  { "peta", "P",1e15 },
  { "thousand ", "K ", 1e3 },
  { "million ", "M ", 1e6 },
  { "billion ", "G ", 1e9 },
  { "trillion ","T ", 1e12 },
  { NULL, NULL, 0.0 }
};
  

struct AvUnitDef_s
{
  const char * measurementName;
  const char * unitName; 
  const char * abbrev;
  unsigned int allowedModifiers;
  double multFactor;
};

// [ ] Verify accuracy of constants
//
// first entry in each table defines canonical units
//
// measurementName   long name   abbreviation    allowed prefixes    value relative to canonical
//
struct AvUnitDef_s builtinUnits[] =
{
  { "distance", "meters", "m", AvUnits::PICO | AvUnits::NANO | AvUnits::MICRO | AvUnits::MILLI | AvUnits::CENTI | AvUnits::KILO, 1.0 },
  { "distance", "angstroms", "A", 0, 1e-10 },
  { "distance", "microns", "um", 0, 1e-6 },
  { "distance", "AUs", "AU", AvUnits::THOUSAND | AvUnits::MILLION | AvUnits::BILLION | AvUnits::TRILLION, 1.496e11 },
  { "distance", "parsecs", "pc", AvUnits::MICRO | AvUnits::MILLI | AvUnits::KILO | AvUnits::MEGA | AvUnits::GIGA | AvUnits::TERA | AvUnits::PETA, 3.1192e14 },
  { "distance", "lightyears", "ly", AvUnits::THOUSAND | AvUnits::MILLION | AvUnits::BILLION | AvUnits::TRILLION, 9.461451e15 },
  
  { "time", "seconds", "sec", AvUnits::PICO | AvUnits::NANO | AvUnits::MICRO | AvUnits::MILLI, 1.0 },
  { "time", "minutes", "min", 0, 60.0 },
  { "time", "hours", "hr", 0, 3600.0 },
  { "time", "days", "days", 0, 8.64e4 },
  { "time", "months", "mos", 0, 2.629e6 },
  { "time", "years", "yrs", AvUnits::THOUSAND | AvUnits::MILLION | AvUnits::BILLION, 3.156e7 },
  { "time", "decades", "decades", 0, 3.156e8 },
  { "time", "centuries", "centuries", 0, 3.156e9 },
  { "time", "millenia", "millienia", 0, 3.156e10 },

  // Right ascension can be measured as a time value
  { "right ascension", "degrees", "d", 0, 1.0 },
  { "right ascension", "radians", "rad", 0, 57.29578 },
  { "right ascension", "arcseconds", "s", AvUnits::MILLI | AvUnits::MICRO, 1.0/3600.0 },
  { "right ascension", "arcminutes", "m", 0, 1.0/60.0 },

  { "right ascension", "hours", "h", 0, 15.0 },
  { "right ascension", "minutes", "m", 0, 1.0/4.0 },
  { "right ascension", "seconds", "s", 0, 1.0/240.0 },

  { "declination", "arcseconds", "s", AvUnits::MILLI | AvUnits::MICRO, 1.0 },
  { "declination", "arcminutes", "m", 0, 60.0 },
  { "declination", "degrees", "d", 0, 3.6e3 },
  { "declination", "radians", "rad", 0, 2.0626481e5 },

  { "longitude", "degrees", "d", 0, 1.0 },
  { "longitude", "radians", "rad", 0, 57.29578 },  

  { "latitude", "degrees", "d", 0, 1.0 },
  { "latitude", "radians", "rad", 0, 57.29578 }, 

  { "velocity", "meters/second", "m/s", AvUnits::MILLI | AvUnits::MICRO | AvUnits::KILO, 1.0 },

  { "frequency", "hertz", "Hz", AvUnits::KILO | AvUnits::MEGA | AvUnits::GIGA | AvUnits::TERA | AvUnits::PETA, 1.0 },

  { "specific intensity", "janskys/beam", "jy/beam", AvUnits::MICRO | AvUnits::MILLI | AvUnits::KILO, 1.0 },
  
  { "magnetic field strength", "gauss", "gauss", AvUnits::NANO | AvUnits::MICRO | AvUnits::MILLI | AvUnits::KILO, 1.0 },

  { "mass","gram", "g", AvUnits::PICO | AvUnits::NANO | AvUnits::MICRO | AvUnits::MILLI | AvUnits::KILO | AvUnits::MEGA | AvUnits::GIGA | AvUnits::TERA | AvUnits::PETA, 1.0 },
  
  { "charge", "Coulomb", "C", AvUnits::KILO, 0.0 },

  { "area","square meters", "m sq", 0, 1.0 },

  { "volume","liters", "l", AvUnits::MICRO | AvUnits::MILLI | AvUnits::KILO, 1.0 },
  { "volume","cubic centimeters", "cc", 0, 1e-3 },
  { "volume","cubic meters", "m^3", 0, 1e3 },

  { "polarization","stoke index","stokes",0,1.0 },
  
  { "sequence","sequence number","",0,1.0 },

  { "beam","beam index", "",0,1.0 },

  { "channel", "channel number","",0,1.0 },

  { NULL, "", "", 0, 0.0 }
};

// Stokes names for astronomical data (+8 from FITS UV data specs)
static const char * stokesNames[] = {
  "Linear YX",
  "Linear XY",
  "Linear YY",
  "Linear XX",
  "Circular LR",
  "Circular RL",
  "Circular LL",
  "Circular RR",
  "",
  "Stokes I", 
  "Stokes Q", 
  "Stokes U", 
  "Stokes V",
};
static const int STOKES_OFFSET = 8;
static const int NSTOKES = 12;

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

void AvUnits::initClass()
{
  int i;

  if (initialized_) return;

  axisIDCounter_ = 0;

  // returns a pointer to the canonical unitdef for the base type
  measurementName2canonicalUnitDef_ = new AvStringHashTable<AvUnitDef *>(32, NULL, "measurementName2canonicalUD");

  // maps "kilo" to prefix def pointer, for example
  name2unitPrefixDef_ = new AvStringHashTable<AvUnitPrefixDef *>(32, NULL, "name2upd");

  // maps "hertz" to "frequency" for example.  Ambiguous entries are 
  // removed and warning is issued if search is required.
  baseName2measurementName_ = new AvStringHashTable<AvString>(64, "", "bn2tn");
  baseName2measurementNameCount_ = new AvStringHashTable<int>(64, 0, "bn2tnc");

  for (i = 0; builtinPrefixes[i].name != NULL; i++)
    {
      AvUnitPrefixDef * upd = new AvUnitPrefixDef(builtinPrefixes[i].name,
						  builtinPrefixes[i].prefix,
						  builtinPrefixes[i].multiplier);
      unitPrefixDefList_.add(upd);
      name2unitPrefixDef_->add(upd->name, upd);
      unitPrefixDefArray_[i] = upd;
    }

  // maps <"latitude","seconds"> for example, to an AvUnitDef pointer.  Note this is only
  // a base unit mapping.
  unitsTable_ = new AvStringHashTable<AvStringHashTable<AvUnitDef *> *>(64, NULL,"unitsTable");
  measurementName2id_ = new AvStringHashTable<int>(32, -1,"measurementName2id");

  for (i = 0; builtinUnits[i].measurementName != NULL; i++)
    {
      TNadd(builtinUnits[i].measurementName,
	    builtinUnits[i].unitName,
	    builtinUnits[i].abbrev,
	    builtinUnits[i].allowedModifiers,
	    builtinUnits[i].multFactor);
    }

  initialized_ = 1;
}

void AvUnits::destroyClass()
{
  if (!initialized_) return;

  while (!unitDefList_.empty()) { delete unitDefList_.removeHead(); }
  while (!unitPrefixDefList_.empty()) { delete unitPrefixDefList_.removeHead(); }
  while (!utList_.empty()) { delete utList_.removeHead(); }

  delete measurementName2canonicalUnitDef_;
  delete name2unitPrefixDef_;
  delete baseName2measurementName_;
  delete baseName2measurementNameCount_;
  delete unitsTable_;
  delete measurementName2id_;
}

int AvUnits::lookup(const AvString & unknownUnits_p,
		     AvString& prefixOut,
		     AvString& baseOut,
		     AvString& typeOut,
		     double& prefixMultiplierOut)
{
  AvString unknownUnits = downcase(unknownUnits_p);

  // First, try doing pbsplit and type lookup for units
  pbsplit(unknownUnits, prefixOut, baseOut);
  typeOut = baseName2measurementName_->search(baseOut);
  if (typeOut != "")
    {
      // success
      AvUnitPrefixDef * upd = name2unitPrefixDef_->search(prefixOut);
      prefixMultiplierOut = (upd) ? upd->multiplier : 1.0;
      return 1;
    }
#if 0
  // Second, try splitting off the abbreviated prefix and
  // doing a lookup on the prefix
  pbsplitAbbrev(unknownUnits, prefixOut, baseOut);
  typeOut = baseName2measurementName_->search(baseOut);
  if (typeOut != "")
    {
      // success
      AvUnitPrefixDef * upd = name2unitPrefixDef_->search(baseOut);
      prefixMultiplierOut = (upd) ? upd->multiplier : 1.0;
      return 1;
    }
#endif

  return 0;
}

// Try to abbreviate the string in units
AvString AvUnits::abbreviate(const AvString & units)
{
  AvString units_p = downcase(units);
  AvString prefix, base;
  pbsplit(units_p, prefix, base);
  AvString measurementName = baseName2measurementName_->search(base);

  AvUnitPrefixDef * upd = name2unitPrefixDef_->search(prefix);
  if (measurementName == "")
    {
      // couldn't recognize units, but might deal with prefix
      return (upd ? (upd->prefix + base) : base);
    }
  else
    {
      // found name, so use it
      AvStringHashTable<AvUnitDef *> * t = unitsTable_->search(measurementName);
      if (!t) 
	{
	  cerr << "Shouldn't get here" << endl;
	  abort();
	}
      else
	{
	  AvUnitDef * ud = t->search(base);
	  return (ud ? (upd ? (upd->prefix + ud->unitAbbrev) : (ud->unitAbbrev))
		  : (upd ? (upd->prefix + base) : (units)));
	}
    }
#if defined(sgi)
  // This statement is unreachable, but our SGI compiler complains if
  // the function doesn't return something.
  return AvString(units);
#endif
}

#if 0
// prefix/base split of a units name by removing an abbreviated prefix
void AvUnits::pbsplitAbbrev(const AvString & units,
			    AvString & prefix,
			    AvString & base)
{
  AvString units_p = downcase(units);

  // take out hyphens and extra spaces
  units_p.gsub(AvRegex("-+"), "");
  units_p.gsub(AvRegex(" +"), " ");
  units_p.gsub(AvRegex("^ +"), "");
  units_p.gsub(AvRegex(" +$"), "");

  AvSubString s_prefix = units_p.at(AvRegex("^[pnumkMGTP]"));
  prefix = s_prefix;
  if (prefix != "")
    {
      for (int i = 0; i < 10; i++)
	{
	  if (unitPrefixDefArray_[i]->prefix == prefix)
	    {
	      prefix = unitPrefixDefArray+[i]->name;
	      break;
	    }
	}
    }

  units_p.del(prefix);
  base = units_p;
  
  cout << "pbsplitAbbrev prefix = '" << prefix << "', base = '" << base << "'.\n";
}
#endif

// prefix/base split of a units name
void AvUnits::pbsplit(const AvString & units,
		      AvString & prefix,
		      AvString & base)
{
  AvString units_p = downcase(units);

  // take out hyphens and extra spaces
  units_p.gsub(AvRegex("-+"), "");
  units_p.gsub(AvRegex(" +"), " ");
  units_p.gsub(AvRegex("^ +"), "");
  units_p.gsub(AvRegex(" +$"), "");

  AvString s_prefix = units_p.at(AvRegex("^\\(\\(pico\\)\\|\\(nano\\)\\|\\(micro\\)\\|\\(milli\\)\\|\\(centi\\)\\|\\(kilo\\)\\|\\(mega\\)\\|\\(giga\\)\\|\\(tera\\)\\|\\(peta\\)\\|\\(thousand \\)\\|\\(million \\)\\|\\(billion \\)\\|\\(trillion \\)\\)"));

  prefix = s_prefix;
  units_p.del(prefix);
  base = units_p;
  
  //cout << "pbsplit prefix = '" << prefix << "', base = '" << base << "'.\n";
}

void AvUnits::dumpUnitDef(AvUnitDef * ud)
{
  cout << "Unit Name: " << ud->unitName << "(" << ud->unitAbbrev << ")\n";
  for (int i = 0; i < (int)N_PREFIXES; i++)
    if (ud->allowedPrefixes & (1<<i))
      {
	AvUnitPrefixDef * upd = unitPrefixDefArray_[i];
	cout << "Unit Name: " << upd->name << ud->unitName << "(" << upd->prefix << ud->unitAbbrev << ")\n";
	cout << "There are " << 1.0*upd->multiplier << " " << ud->unitName << " in a " << upd->name << ud->unitName << ".\n";
      }
}

void AvUnits::dump()
{
  cout << "______  Dumping the Units Database  ______\n\n";
  AvListIter<AvUnitDef *> it(&unitDefList_);
  while (!it.done())
    {
      dumpUnitDef(it.get());
    }
  cout << "__________________________________________\n\n";
}

// !!works
// [ ] invert use of prefix values

int AvUnits::convertValue_(const double valueIn,
			   AvUnitPrefixDef * updi,
			   AvUnitDef * udi,
			   AvUnitPrefixDef * updo,
			   AvUnitDef * udo,
			   double & valueOut)
{
#ifdef AVUNITSTRACE
  cout << "[convertValue_ d PD D PD D d]\n"; 
#endif
  
  double mult = 1.0;
  if (updi) mult *= updi->multiplier;
  mult *= udi->multFactor;
  if (updo) mult /= updo->multiplier;
  mult /= udo->multFactor;

  valueOut = valueIn*mult;
#ifdef AVUNITSTRACE
  cout << "[convertValue_ d PD D PD D d] done\n"; 
#endif
  return 1;
}
  
// public interface
int AvUnits::convertValue(const double valueIn,
			  const AvString& unitsIn,
			  const AvString& unitsOut,
			  double & valueOut,
			  const AvString& measurementName)
{
#ifdef AVUNITSTRACE
  cout << "[convertValue dSSdS]\n"; 
#endif
  AvString prefixIn, baseIn, prefixOut, baseOut;
  pbsplit(unitsIn, prefixIn, baseIn);
  pbsplit(unitsOut, prefixOut, baseOut);

  // First get the measurementName table
  AvStringHashTable<AvUnitDef *> * t = unitsTable_->search(measurementName);
  if (!t)
    {
      // don't know what to do with those units, so don't convert
      valueOut = valueIn;

#ifdef AVUNITSVERBOSE
      cout << "AvUnits::convertValue() - conversion failed: unknown axis type '" << measurementName << "'\n";
#endif
      return 0;
    }
  
  // Get updi
  AvUnitPrefixDef * updi = NULL;
  if (prefixIn != "") 
    {
      updi = name2unitPrefixDef_->search(prefixIn);
      if (!updi)
	{
	  valueOut = valueIn;

#ifdef AVUNITSVERBOSE
	  cout << "conversion failed: prefixIn = '" << prefixIn << "'.\n";
#endif
	  return 0;
	}
    }
  
  // Get udi
  AvUnitDef * udi = t->search(baseIn);
  if (!udi)
    {
      valueOut = valueIn;
#ifdef AVUNITSVERBOSE
      cout << "conversion failed: baseIn = '" << baseIn << "'.\n";
#endif
      return 0;
    }

  // get updo
  AvUnitPrefixDef * updo = NULL;
  if (prefixOut != "") 
    {
      updo = name2unitPrefixDef_->search(prefixOut);
      if (!updo)
	{
	  valueOut = valueIn;
#ifdef AVUNITSVERBOSE
	  cout << "conversion failed: prefixOut = '" << prefixOut << "'.\n";
#endif
	  return 0;
	}
    }

  // get udo
  AvUnitDef * udo = t->search(baseOut);
  if (!udo)
    {
      valueOut = valueIn;
#ifdef AVUNITSVERBOSE
      cout << "conversion failed: baseOut = '" << baseOut << "'.\n";
#endif
      return 0;
    }

  // convert canonical to valueOut
  int retval = convertValue_(valueIn, updi, udi, updo, udo, valueOut);

#ifdef AVUNITSTRACE
  cout << "[convertValue done]\n";
#endif

  return retval;
}

int AvUnits::queryUnits(const double valueIn,
			const AvString& unitsIn,
			const AvString& measurementName,
			AvString& unitsOut)
{
  int retval = 0;
  AvString prefix, base;
  pbsplit(unitsIn, prefix, base);

  int decPlaces = (int) log10(fabs(valueIn)) + 1;
  float score = fabs(decPlaces - 1.55);
  AvUnitPrefixDef * winner = NULL;

  AvUnitDef * ud = TNsearch(measurementName, base);
  if (!ud)
    {
      return 0;
    }

  if (ud->allowedPrefixes > 0)
    {
      for (int i = 0; i < (int)N_PREFIXES; i++)
	{
	  if (ud->allowedPrefixes & (1<<i))
	    {
	      AvUnitPrefixDef * upd = unitPrefixDefArray_[i];
	      int nPlaces = decPlaces - (int) log10(upd->multiplier);
	      float newScore = fabs(nPlaces - 1.55);
	      if (newScore < score)
		{
		  score = newScore;
		  winner = upd;
		}
	    }
	}
    }

  if (!winner)
    {
      unitsOut = base;
      retval = 1;
    }
  else
    {
      unitsOut = winner->name + base;
      retval = 1;
    }
  return retval;
}

int AvUnits::convertToBaseValue(const double valueIn,
				const AvString& unitsIn,
				const AvString& baseUnitsOut,
				double & valueOut,
				AvString& abbrevUnitsOut,
				AvString& unitsOut,
				const AvString& measurementName)
{
#ifdef AVUNITSTRACE
  cout << "[convertToBaseValue dSSdSSS]\n"; 
#endif
  // convert using base units, then correct with available prefixes
  // return the units used.
  convertValue(valueIn, unitsIn, baseUnitsOut, valueOut, measurementName);

  int decPlaces = (int) log10(fabs(valueOut)) + 1;

#ifdef AVUNITSDEBUG
  cout << "---------------------------------\n";
  cout << "valueOut = " << valueOut << endl;
  cout << "decPlaces = " << decPlaces << endl;
#endif

  // what multiplier of those available does the best
  // job of converting?  (Including no units)
  //
  float score = fabs(decPlaces - 1.55);
  AvUnitPrefixDef * winner = NULL;

#ifdef AVUNITSDEBUG
  cout << "trying no prefix" << endl;
  cout << "this would result in " << decPlaces << "decimal places" << endl;
  cout << "and a score of " << score << endl;
#endif
  
  AvUnitDef * ud = TNsearch(measurementName, baseUnitsOut);
  if (!ud)
    {
      valueOut = valueIn;
      abbrevUnitsOut = "";
      unitsOut = "";
#ifdef AVUNITSVERBOSE
      cout << "conversion failed: baseUnitsOut = '" << baseUnitsOut << "'.\n";
#endif
      return 0;
    }

  abbrevUnitsOut = ud->unitAbbrev;

  if (ud->allowedPrefixes > 0)
    {
      for (int i = 0; i < (int) N_PREFIXES; i++)
	{
	  if (ud->allowedPrefixes & (1<<i))
	    {
	      AvUnitPrefixDef * upd = unitPrefixDefArray_[i];
	      int nPlaces = decPlaces - (int) log10(upd->multiplier);
	      float newScore = fabs(nPlaces - 1.55);
#ifdef AVUNITSDEBUG
	      cout << "trying " << upd->name << endl;
	      cout << "this would result in " << nPlaces << "decimal places" << endl;
	      cout << "and a score of " << newScore << endl;
#endif
	      if (newScore < score)
		{
		  score = newScore;
		  winner = upd;
		}
	    }
	}
    }

  int retval;
  if (!winner)
    {
      unitsOut = baseUnitsOut;
      abbrevUnitsOut = abbrevUnitsOut;
      retval = 1;
    }
  else
    {
      valueOut /= winner->multiplier;
      unitsOut = winner->name + baseUnitsOut;
      abbrevUnitsOut = winner->prefix + abbrevUnitsOut;
      retval = 1;
    }
#ifdef AVUNITSTRACE
  cout << "[convertToBaseValue done]\n"; 
#endif
  return retval;
}

double AvUnits::roundToNDigits(const double value_p,
			       int nDigits)
{
#ifdef AVUNITSTRACE
  cout << "[roundToNDigits di]\n"; 
#endif
  double value = value_p;
  if (value == 0) return 0;
  int neg = 0;
  if (value < 0)
    {
      neg = 1;
      value = -value;
    }

  int d1Pos = (int) log10(value);
  double roundValue = pow(10.0, d1Pos - nDigits + 1);  
  double valueOut = roundValue * Avftrunc(value / roundValue + 0.5);
  if (neg) valueOut = -valueOut;

#ifdef AVUNITSDEBUG
  cout << "value in = " << value_p << ", nDigits = " << nDigits << endl;
  cout << "d1Pos = " << d1Pos << endl;
  cout << "roundValue = " << roundValue << endl;
  cout << "value out = " << valueOut << endl;
#endif

  return valueOut;
}

#if 0
double AvUnits::roundToNDigits(const double value_p,
			       int nDigits,
			       AvString & strValOut)
{
#ifdef AVUNITSTRACE
  cout << "[roundToNDigits diS]\n"; 
#endif
  double value = value_p;
  if(isnan(value_p))
  {	strValOut = "NaN";
	return 0;
  }
  if (value == 0) 
    {
      char formatStr[40];
      sprintf(formatStr, "%%%d.%df", nDigits+1, nDigits-1);
      char buffer[40];
      sprintf(buffer, formatStr, 0.0);
      strValOut = buffer;
      return 0;
    }
  int neg = 0;
  if (value < 0)
    {
      neg = 1;
      value = -value;
    }

  int d1Pos = (int) log10(value);
  int dnPos = d1Pos - nDigits + 1;
  double roundValue = pow(10.0, dnPos);
  double valueOut = roundValue * Avftrunc(value / roundValue + 0.5);
  if (neg) valueOut = -valueOut;

#ifdef AVUNITSDEBUG
  cout << "value in = " << value << ", nDigits = " << nDigits << endl;
  cout << "d1Pos = " << d1Pos << endl;
  cout << "roundValue = " << roundValue << endl;
  cout << "value out = " << valueOut << endl;
#endif

  int ndec = (dnPos >= 0) ? 0 : -dnPos;
  // Limit the number of digits printed.
  if(ndec > nDigits) ndec = nDigits;
  int fwidth = d1Pos + ndec + 1 + ((ndec > 0) ? 1 : 0);

#if 0
  if (fwidth > 50) fwidth = 50;

  char formatStr[80];
  sprintf(formatStr, "%%%d.%df", fwidth, ndec);
  char buffer[80];
  sprintf(buffer, formatStr, valueOut);
#else
  char formatStr[80];
  char buffer[80];

  // If the number is, say, +1.779496118082E+248, limit the maximum #
  // of digits printed. (17 digits covers dbl precision).
#define MAXDIGITS 17
  int maxdigits = (nDigits <= MAXDIGITS) ? nDigits : MAXDIGITS;
  if (ndec > maxdigits) ndec = maxdigits.

  if(fwidth > MAXDIGITS) // The number is way big.
  {	fwidth = 0;
	ndec = MAXDIGITS;	// Max # of digits after decimal point.
	sprintf(formatStr, "%%.%dG", ndec);
	sprintf(buffer, formatStr, value); // Use original value. Sprintf
					   // will round.
  }
  else	// Normal
  {	if (ndec > 48) ndec = 48;
	sprintf(formatStr, "%%%d.%dG", fwidth, ndec);
	sprintf(buffer, formatStr, valueOut);
  }
#endif

  strValOut = buffer;

#ifdef AVUNITSDEBUG
  cout << "ndec = " << ndec << endl;
  cout << "fwidth = " << fwidth << endl;
  cout << "format string is '" << formatStr << "'.\n";
  cout << "string value is '" << strValOut << "'.\n";
#endif

  return valueOut;
}
#else
// Rounds the value the way the old version did, but uses 'G' to print.
// G format makes printing N significant digits easier and ensures the
// resulting string has a sane length.
double AvUnits::roundToNDigits(const double value_p,
			       int nDigits,
			       AvString & strValOut)
{
#ifdef AVUNITSTRACE
  cout << "[roundToNDigits diS]\n"; 
#endif
  double value = value_p;
  if(isnan(value_p))
  {	strValOut = "NaN";
	return 0;
  }
  if (value == 0) 
  { // print 0 similarly to how other values are printed.
      char formatStr[40];
      sprintf(formatStr, " %%%d.%df", nDigits+1, nDigits-1);
      char buffer[40];
      sprintf(buffer, formatStr, 0.0);
      strValOut = buffer;
      return 0;
  }

  int neg = 0;
  if (value < 0)
    {
      neg = 1;
      value = -value;
    }

  int d1Pos = (int) log10(value);
  int dnPos = d1Pos - nDigits + 1;
  double roundValue = pow(10.0, dnPos);
  double valueOut = roundValue * Avftrunc(value / roundValue + 0.5);
  if (neg) valueOut = -valueOut;

#ifdef AVUNITSDEBUG
  cout << "value in = " << value << ", nDigits = " << nDigits << endl;
  cout << "d1Pos = " << d1Pos << endl;
  cout << "roundValue = " << roundValue << endl;
  cout << "value out = " << valueOut << endl;
#endif


  // If the number is, say, +1.779496118082E+248, limit the maximum #
  // of digits printed. (17 digits covers dbl precision).
#define MAXDIGITS 17
  int ndigits = (nDigits <= MAXDIGITS) ? nDigits : MAXDIGITS;
  // Left justifying and always printing the sign char help limit the
  // amount of jumping around the number does.
  // Need to use 'G' rather than 'F' since the data values can be anything.
  // Don't want 'E' since many like 'F' format where possible.
  char buffer[80];
  sprintf(buffer, "%-+.*G", ndigits, valueOut);
  strValOut = buffer;

#ifdef AVUNITSDEBUG
  cout << "ndec = " << ndec << endl;
  cout << "fwidth = " << fwidth << endl;
  cout << "format string is '" << formatStr << "'.\n";
  cout << "string value is '" << strValOut << "'.\n";
#endif

  return valueOut;
}
#endif

AvString AvUnits::makeNiceString(const double value_p,
				 const AvString& units,
				 int nsigfigs)
{
#ifdef AVUNITSTRACE
  cout << "[makeNiceString dSi]\n"; 
#endif
  AvString prefix, baseUnits;
  pbsplit(units, prefix, baseUnits);

  AvString measurementName = baseName2measurementName_->search(baseUnits);
  if (measurementName == "")
    {
      cerr << "Axis type name for '" << baseUnits << "' ambiguous\n";
      // [ ] allow guess
      cerr << "Conversion failed\n";
      return "";
    }
  else
    {
      cout << "'" << units << "' is a measurement of '" << measurementName << "'\n";
      return makeNiceString(value_p, units, nsigfigs, measurementName);
    }
}

// create a nice string for the value
//
// 
AvString AvUnits::makeNiceString(const double value_p,
				 const AvString& units,
				 int nsigfigs,
				 const AvString& measurementName_p)
{
#ifdef AVUNITSTRACE
  cout << "[makeNiceString dSiS]\n"; 
#endif
  AvString prefix, baseUnits;
  pbsplit(units, prefix, baseUnits);

  AvString measurementName = downcase(measurementName_p);

  // convert value to use same base units, but add prefix
  double value2;
  AvString abbrevUnitsOut, unitsOut;
  convertToBaseValue(value_p,
		     units,
		     baseUnits,
		     value2,
		     abbrevUnitsOut,
		     unitsOut,
		     measurementName);

  double value = value_p;

  AvString retval;
  if (baseUnits.contains(AvRegex("^\\(\\(arcseconds\\)\\|\\(arcminutes\\)\\|\\(degrees\\)\\|\\(radians\\)\\|\\(archours\\)\\).*")))
    {
      if (measurementName.contains(AvRegex("^\\(\\(right ascension\\)\\|\\(ra\\)\\)")))
	{
	  // Have a latitude value, so need nice units for those
	  // look at nsigfigs for details
	  // 
	  char buffer[40];
	  char field[40];
	  double t;
	  
	  int neg = 0;
	  if (value < 0)
	    {
	      neg = 1;
	      value = -value;
	    }
	  
	  convertValue(value, baseUnits, "hours", t, measurementName);
	  
	  int hours = (int) t;
	  t -= (double) hours;
	  t *= 60;
	  
	  int minutes = (int) t;
	  t -= (double) minutes;
	  t *= 60;
	  
	  sprintf(field, "%%s%%02dh %%02dm %%0%d.%dfs", nsigfigs+1, (nsigfigs-2 > 0) ? nsigfigs-2 : 0);
	  
	  sprintf(buffer, field, neg ? "-" : "", hours, minutes, t);
	  retval = buffer;
	}
      else if (measurementName.contains(AvRegex("^\\(\\(declination\\)\\|\\(dec\\)\\)")))
	{
	  char buffer[40];
	  char field[40];
	  double t;

	  int neg = 0;
	  if (value < 0)
	    {
	      neg = 1;
	      value = -value;
	    }

	  convertValue(value, baseUnits, "degrees", t, measurementName);
	  
	  int degrees = (int) t;
	  t -= (double) degrees;
	  t *= 60;
	  
	  int minutes = (int) t;
	  t -= (double) minutes;
	  t *= 60;
	  	  
	  sprintf(field, "%%s%%02dd %%02dm %%0%d.%dfs", nsigfigs+1, (nsigfigs-2 > 0) ? nsigfigs-2 : 0);
	  sprintf(buffer, field, neg ? "-" : "", degrees, minutes, t);
	  
	  retval = buffer;
	}
      else
	{
	  AvString strval;
	  roundToNDigits(value2, nsigfigs, strval);
	  retval = strval + " " + abbrevUnitsOut;
	}
    }
  else if (measurementName == "time")
    {	  
#if 1
      // time is given in terms of units since 00:00:00 UTC,
      // January 1, 1970
      
//      int neg=0;
      if (value < 0)
	{
//	  neg = 1;
	  value = -value;
	}
      
      double t;
      
      // only approximate
      convertValue(value, baseUnits, "seconds", t, "time");
      time_t tt = (time_t) t;
      
      retval = ctime(&tt);

      retval.gsub("\n","");
#else
      retval = "<time string>";
#endif
    }
  else if (baseUnits == "stoke index")
    {
      // as per the FITS convention...
      int ndx = STOKES_OFFSET + (int) (value+0.5);
      if (ndx >= 0 && ndx <= NSTOKES && ndx != STOKES_OFFSET)
	retval = stokesNames[ndx];
      else
	retval = "<Unknown Stokes Index>";
    }
  else if (baseUnits == "sequence number" || baseUnits == "beam index")
    {
      char buf[30];
      sprintf(buf, "#%d", (int) value);
      retval = buf;
    }
  else
    {
      // make the print value contain nsigfigs digits by
      // rounding.
      AvString strval;
      roundToNDigits(value2, nsigfigs, strval);      
      retval = strval + " " + abbrevUnitsOut;
    }
  return retval;
}

AvString AvUnits::makeNiceString(const double value_p,
				 const AvString& units,
				 const AvString& preferredUnits,
				 int nsigfigs)
{
#ifdef AVUNITSTRACE
  cout << "[makeNiceString dSSi]\n"; 
#endif
  AvString prefix, baseUnits;
  pbsplit(units, prefix, baseUnits);

  AvString measurementName = baseName2measurementName_->search(baseUnits);
  if (measurementName == "")
    {
      pbsplit(preferredUnits, prefix, baseUnits);
      measurementName = baseName2measurementName_->search(baseUnits);
    }

  if (measurementName == "")
    {
      cerr << "Axis type name for both '" << units << "' and '" << preferredUnits << "' ambiguous\n";
      // [ ] allow guess
      cerr << "Conversion failed\n";
      return "";
    }
  else
    {
#ifdef AVUNITSDEBUG
      cout << "'" << baseUnits << "' is a measurement of '" << measurementName << "'\n";
#endif
      return makeNiceString(value_p, units, preferredUnits, nsigfigs, measurementName);
    }
}

AvString AvUnits::makeNiceString(const double value,
				 const AvString& unitsIn,
				 const AvString& preferredUnits,
				 int nsigfigs,
				 const AvString& measurementName)
{
#ifdef AVUNITSTRACE
  cout << "[makeNiceString dSSiS]\n"; 
#endif
  // first convert to preferredUnits
  double valueOut;
  if (!convertValue(value, unitsIn, preferredUnits, valueOut, measurementName))
    {
#ifdef AVUNITSVERBOSE
      cerr << "conversion from " << unitsIn << " to " << preferredUnits << " failed.\n";
#endif
      return "";
    }
  return makeNiceString(valueOut, preferredUnits, nsigfigs, measurementName);
}
  

#if 0
// add a units definition into the database
addUnits(const AvString & measurementName,
	 const AvString & abbrev,
	 const AvString & allowedModifiers, // list of allowed prefixes
	 double canonicalMultFactor,        // canonical -> this unit
	 const AvString & unitName,
	 const AvString & referenceType)
{
}
#endif

void AvUnits::TNadd(const AvString & measurementName,
		    const AvString & unitName,
		    const AvString & unitAbbrev,
		    unsigned int allowedPrefixes,
		    double canonicalMultFactor)
{
#ifdef AVUNITSTRACE
  cout << "[TNadd SSSid]\n"; 
#endif
  int isCanon = 0;

  // find the table entry in unitsTable_.  If no table, add one
  // and mark as canonical entry.
  AvStringHashTable<AvUnitDef *> * t = unitsTable_->search(measurementName);
  if (!t)
    {
#ifdef AVUNITSDEBUG
      cout << "Adding table for " << measurementName << endl;
#endif
      t = new AvStringHashTable<AvUnitDef *>(8, NULL, AvString(measurementName+" table").chars());
      unitsTable_->add(measurementName, t);
      utList_.add(t);
      isCanon = 1;
    }

  AvUnitDef * ud = new AvUnitDef(measurementName, 
				 unitName,
				 unitAbbrev,
				 allowedPrefixes,
				 canonicalMultFactor);
  
  if (isCanon)
    {
      measurementName2canonicalUnitDef_->add(ud->measurementName, ud);
      ud->canon = ud;
    }
  else
    {
      AvUnitDef * can = measurementName2canonicalUnitDef_->search(measurementName);
      if (!can)
	{
	  cerr << "Fatal: canon not found!!\n";
	  abort();
	}
      ud->canon = can;
    }
  unitDefList_.add(ud);
  t->add(ud->unitName, ud);
  
  // setup the baseName2measurementName entry if unambiguous
  int count = baseName2measurementNameCount_->search(unitName);
  if (count == 0)
    {
#ifdef AVUNITSDEBUG
      cout << "Adding type '" << unitName << "' (" << measurementName << ") to bn2tn table\n";
#endif
      baseName2measurementNameCount_->add(unitName, 1);
      baseName2measurementName_->add(unitName, measurementName);
    }
  else
    {
#ifdef AVUNITSDEBUG
      cout << "Removing ambiguous type '" << unitName << "' from bn2tn table\n";
#endif
      baseName2measurementName_->remove(unitName);
    }  
}

AvUnitDef * AvUnits::TNsearch(const AvString & measurementName,
			      const AvString & unitName)
{
#ifdef AVUNITSTRACE
  cout << "[TNsearch SS]\n"; 
#endif
  AvStringHashTable<AvUnitDef *> * t = unitsTable_->search(measurementName);
  if (!t) 
    return 0;
  else
    return t->search(unitName);
}
