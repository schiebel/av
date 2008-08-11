//# AvString.h: AvString classes
//# Copyright (C) 1992-1999,2000,2001,2002
//# Associated Universities, Inc. Washington DC, USA.
//#
//# This library is free software; you can redistribute it and/or modify it
//# under the terms of the GNU Library General Public License as published by
//# the Free Software Foundation; either version 2 of the License, or (at your
//# option) any later version.
//#
//# This library is distributed in the hope that it will be useful, but WITHOUT
//# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
//# License for more details.
//#
//# You should have received a copy of the GNU Library General Public License
//# along with this library; if not, write to the Free Software Foundation,
//# Inc., 675 Massachusetts Ave, Cambridge, MA 02139, USA.
//#
//# Correspondence concerning AIPS++ should be addressed as follows:
//#        Internet email: aips2-request@nrao.edu.
//#        Postal address: AIPS++ Project Office
//#                        National Radio Astronomy Observatory
//#                        520 Edgemont Road
//#                        Charlottesville, VA 22903-2475 USA
//#
//# $Id: AvString.h,v 19.1 2004/02/04 15:26:14 gvandiep Exp $

#if !defined (_AvSTRING_H)
#define _AvSTRING_H

//# AvString classes, adapted from gnu string classes.

//# This is the gnu string implementation,
//# modified by AIPS++ to use aips++ style exceptions, move some things out
//# of line, etc.
// Converted for standalone use by aipsview.

#include <iostream>
#include <AvRegex.h>

extern void Av_stringThrowError(const char *);

// <summary> 
// AvStrRep: The internal AvString representation which handles most operations.
// </summary>

// <use visibility=local>

// <reviewed reviewer="Timothy P. P. Roberts, troberts@NRAO.edu" date="Mon 1995/03/27 15:57:01 GMT" tests="tAvString.cc" demos="">
// </reviewed>

// <prerequisite>
//   <li> none
// </prerequisite>
//
// <etymology>
// The AvStrRep struct name is a concatination of AvString Representation.  This
// is to indicate that AvStrReps handle most of the internal functionality of
// the AvString class.
// </etymology>
//
// <synopsis> 
// The user never uses AvStrRep directly - all processing is done at the AvString
// or SubAvString class level.
// </synopsis> 
//
// <motivation>
// By isolating the AvString functionality into a set of simple processes on a 
// small set of data elements the AvString class is broken into levels that may
// be easily coded, optimized and debugged.  Thus, AvStrReps may be changed 
// without changing the users interface.
// </motivation>
//
// <todo asof="Mon 1995/03/27 15:57:01 GMT">
//   <li> none noted.
// </todo>

struct AvStrRep
{

  // string length 
  unsigned int len;

  // allocated space
  unsigned int sz;

  // the string starts here 
  // (at least 1 char for trailing null)
  // allocated & expanded via non-public fcts
  char s[1];
};

//# primitive ops on AvStrReps -- nearly all AvString fns go through these.
AvStrRep *AvSalloc(AvStrRep*, const char*, int, int);
AvStrRep *Scopy(AvStrRep*, AvStrRep*);
AvStrRep *Sresize(AvStrRep*, int);
AvStrRep *Scat(AvStrRep*, const char*, int, const char*, int);
AvStrRep *Scat(AvStrRep*, const char*, int,const char*,int, const char*,int);
AvStrRep *Sprepend(AvStrRep*, const char*, int);
AvStrRep *Sreverse(AvStrRep*, AvStrRep*);
AvStrRep *Supcase(AvStrRep*, AvStrRep*);
AvStrRep *Sdowncase(AvStrRep*, AvStrRep*);
AvStrRep *Scapitalize(AvStrRep*, AvStrRep*);


//# These classes need to be defined in the order given

class AvString;
class SubAvString;

// <summary> 
// SubAvString: A reference to all or part of an existing AvString.
// </summary>

// <use visibility=export>

// <reviewed reviewer="Timothy P. P. Roberts, troberts@NRAO.edu" date="Mon 1995/03/27 15:57:01 GMT" tests="tAvString.cc" demos="">
// </reviewed>

// <prerequisite>
//   <li> AvString
//   <li> AvRegex - the regular expressions class
// </prerequisite>
//
// <etymology>
// The SubAvString class name is an indication of its role as a reference into
// a small part of a whole AvString.  
// </etymology>
//
// <synopsis> 
// There isn't a public SubAvString constructor - SubAvStrings are always 
// created by a AvString member function.  Since SubAvStrings are a reference to 
// part of an already existing AvString, changing the value of the SubAvString 
// will change the value of the parent AvString.  This was the intent behind 
// their creation.  SubAvStrings allow you to change the rapidly varying part
// of a AvString without needing to concatinate the AvString back together.
// <srcblock> 
//     AvString myOutput = "Error: class foo has thrown an exception.";
//     myOutput.at("foo") = "Oinker";
//     cout << myOutput << endl;
//     myOutput.at("Oinker") = "Swine";
//     cout << myOutput << endl;
// </srcblock> Would print the following:
//
// Error: class Oinker has thrown an exception.
// Error: class Swine has thrown an exception.
//
// The SubAvString class itself has many member functions which check if a set
// of characters is "contain"ed within a SubAvString or allow "match"ing of a 
// regular expression.  Length of the SubAvString, a check to see if the 
// SubAvString is empty and conversion to "const char *" are provided, as well.
// </synopsis> 
//
// <example>
// <srcblock>
// AvString MailingLabel("Dr. Richard Cranium, 1003 Lopezville Road, Socorro");
// MailingLabel += ", NM  87801";
// // we will extract some SubAvString "fields"
// // the part which holds "Richard Cranium" is attached
// SubAvString name = MailingLabel.at("Richard Cranium");
// // the part before ", Soc" and after skipping 20 characters of name
// SubAvString street = MailingLabel.before(", Soc", 20);
// // the through to ", NM" and after skipping 43 previous characters.
// SubAvString city = MailingLabel.through(", NM, 43);
// // the part from "NM" to the end
// SubAvString zip = MailingLabel.from("NM");
// // the possible extra part after the zip code (e.g. zip+4)
// SubAvString plusFour = MailingLabel.after("801");
// // Now we replace as needed - assume an Array of AvStrings is our data source
// for(int i = 0; i<5, i++){
//    name = creditCardData(i,0);
//    street = creditCardData(i,1);
//    city = creditCardData(i,2);
//    zip = creditCardData(i,3);
//    plusFour = creditCardData(i,4);
//    fprintf(Harass, "%s/n", MailingLabel.chars());
// }
// </srcblock>
// </example>
//
// <motivation>
// SubAvStrings were created to allow the user to access parts of a AvString 
// without changing the AvString's coded appearence.  This is part of the GNU
// C++ library and has been converted to use AIPS++ exception handling.
// </motivation>
//
// <todo asof="Mon 1995/03/27 15:57:01 GMT">
//   <li> none noted
// </todo>

class SubAvString 
{
public:

  friend class AvString;

  // destructor
  ~SubAvString();

  // assignment operator
  // <group>
  void operator = (const AvString &y);
  void operator = (const SubAvString &y);
  void operator = (const char *t);
  void operator = (char c);
  // </group>

  // ostream operator for output.
  friend std::ostream &operator<<(std::ostream &s, const SubAvString &x);

  // Return 1 if the target appears anywhere in SubAvString; else 0.
  //<group>
  int contains(char c) const;
  int contains(const AvString &y) const;
  int contains(const SubAvString &y) const;
  int contains(const char *t) const;
  int contains(const AvRegex &r) const;
  //</group>

  // Return 1 if the target matches entire SubAvString.
  int matches(const AvRegex &r) const;

  // length of the SubAvString
  unsigned int length() const;

  // return 1 if empty, 0 if filled.
  int empty() const;

  // convert to char* for use in "C" type character arguments
  const char *chars() const;

  // indexing ok
  int OK() const; 

protected:

  // The AvString I'm a substring of
  AvString &S;

  // starting position in S's rep
  unsigned int pos;

  // length of substring
  unsigned int len;

  void assign(AvStrRep *, const char *, int = -1);

  // <note> There are no public constructors for SubAvString.
  //        SubAvStrings are always created via AvString operations.</note>
  //<group> 
  SubAvString(AvString &x, unsigned int p, unsigned int l);
  SubAvString(const SubAvString &x);
  //</group>

};


// <summary> 
// AvString: the storage and methods of handling collections of characters.
// </summary>

// <use visibility=export>

// <reviewed reviewer="Timothy P. P. Roberts, troberts@NRAO.edu" date="Mon 1995/03/27 15:57:01 GMT" tests="tAvString.cc" demos="">
// </reviewed>

// <prerequisite>
//   <li> AvRegex - the regular expressions class
// </prerequisite>
//
// <etymology>
// The AvString class name is a continuation of the "C" language custom of
// refering to collections of characters as "strings of characters".
// </etymology>
//
// <synopsis> 
// The AvString class may be instantiated in many ways:
// <ol><li> A single character - <src>AvString myChar("C");</src>
// <li> A char* argument - <src>AvString myWord("Yowza");</src>
// <li> The first n chararcters of a pre-existing string - 
// <src>AvString myFoo("fooey", 3);</src>
// </ol> As well as the copy, default and SubAvString constructors.
//
// A AvString may be concatinated with another object (AvString, SubAvString, or 
// char*) with either prepending or postpending.  A search for the position
// of a character within a AvString may return its position, a Bool that it
// is contained within or a Bool confirming your guess at the charater's 
// position is correct.  A check of the frequency of occurance of a character
// within a AvString will return the number of occurances.  
// 
// SubAvStrings may be extracted from AvStrings at, before, through, from and 
// after a starting position within the AvString.  Deletion of characters is
// possible after a given position within the AvString. Global substitution
// of characters within a AvString is provided, as well.  Splitting of AvStrings 
// into a carray of AvStrings is possible, based upon a given separator 
// character, with a return value of the number of elements split.  The joining
// together of the elements of a carray of AvStrings into one AvString is possible.
// 
// Finally, transformations of case and conversions of type are provided. 
// </synopsis> 
//
// <example>
// <srcblock>
// // Let's start with a simple string.
// AvString myAvString("the time");
// // add some more on the end...
// myAvString += " for all good men";
// // prepend some on the front...
// myAvString.prepend("Now is ");
// // do some concatination...
// AvString evenMore;
// evenMore += myAvString + " to come to";
// // do some three way concatination
// AvString allKeys, finishIt(" their country.");
// allKeys = evenMore + "the aid of" + finishIt;
// // find the spot where we put something earlier
// int position = allKeys.index(finishIt);
// // find if the word is in the AvString...
// int query = myAvString.contains("good men");
// // ask if the position we think is true is correct...
// int answer = allKeys.matches(finishIt, position);
// // How many spaces are in our phrase?
// int spacesCount = allKeys.freq(" ");
// </scrblock>
// SubAvStrings are references to all or part of a AvString.  An obvious advantage
// of this would be attaching the small, rapidly changing part (i.e. a name) 
// within a lengthy AvString to a SubAvString.  Thus, the SubAvString may be changed 
// without affecting the larger AvString's handling.  
// <srcblock>
// AvString MailingLabel("Dr. Richard Cranium, 1003 Lopezville Road, Socorro");
// MailingLabel += ", NM  87801";
// // we will extract some SubAvString "fields"
// // the part which holds "Richard Cranium" is attached
// SubAvString name = MailingLabel.at("Richard Cranium");
// // the part before ", Soc" and after skipping 20 characters of name
// SubAvString street = MailingLabel.before(", Soc", 20);
// // the through to ", NM" and after skipping 43 previous characters.
// SubAvString city = MailingLabel.through(", NM, 43);
// // the part from "NM" to the end
// SubAvString zip = MailingLabel.from("NM");
// // the possible extra part after the zip code (e.g. zip+4)
// SubAvString plusFour = MailingLabel.after("801");
// // Now we replace as needed - assume an Array of AvStrings is our data source
// for(int i = 0; i<5, i++){
//    name = creditCardData(i,0);
//    street = creditCardData(i,1);
//    city = creditCardData(i,2);
//    zip = creditCardData(i,3);
//    plusFour = creditCardData(i,4);
//    fprintf(Harass, "%s/n", MailingLabel.chars());
// }
// </srcblock>
// </example>
//
// <motivation>
// The AvString class eases the handling of characters within the AIPS++ 
// environment.  The class was written as part of the GNU C++ library and 
// adapted to use the AIPS++ exception handling.
// </motivation>
//
// <todo asof="Mon 1995/03/27 15:57:01 GMT">
//   <li> none noted
// </todo>

class AvString
{
public:

  friend class SubAvString;

  // default constructors
  AvString();

  // copy constructor
  AvString(const AvString &x);

  // SubAvString constructor
  AvString(const SubAvString &x);

  // const char * constructor
  AvString(const char *t);

  // construct AvString of len number of characters from beginning of char *t
  AvString(const char *t, int len);

  // single char constructor
  AvString(char c);

  // Create a string from the characters in an ostrstream. The ostrstream must be
  // dynamic, that is created with its default constructor.
  //  AvString(std::ostringstream &os);

  // destructor
  ~AvString();

  // Assignment operator
  // <group name=assign>
  void operator=(const AvString &y);
  void operator=(const char *y);
  void operator=(char  c);
  void operator=(const SubAvString &y);
  // </group>

  // This is a concatenation operator.
  //<group name=concate_ops>
  void operator+=(const AvString &y); 
  void operator+=(const SubAvString &y);
  void operator+=(const char *t);
  void operator+=(char c);
  //</group> 

  // This is a synonym for at (int pos, int len).
  SubAvString operator()(int pos, int len);

  // Stream operator
  // <group name=streams>
  friend std::ostream &operator<<(std::ostream &s, const AvString &x);
  friend std::ostream &operator<<(std::ostream &s, const SubAvString &x);
  //  friend std::istream &operator>>(std::istream &s, AvString &x);
  // </group>

  // Concatenate by prepending the argument onto AvString
  //<group name=concatenation_method>
  void prepend(const AvString &y); 
  void prepend(const SubAvString &y);
  void prepend(const char *t);
  void prepend(char c);
  //</group> 


  // Return the position of the target in the string or -1 for failure.
  //<group name=index>
  int index(char c, int startpos = 0) const; 
  int index(const AvString &y, int startpos = 0) const; 
  int index(const SubAvString &y, int startpos = 0) const; 
  int index(const char *t, int startpos = 0) const; 
  int index(const AvRegex &r, int startpos = 0) const; 
  //</group>

  // Return 1 if the target appears anyhere in the AvString; else 0.
  //<group name=contains>
  int contains(char c) const;
  int contains(const AvString &y) const;
  int contains(const SubAvString &y) const;
  int contains(const char *t) const;
  int contains(const AvRegex &r) const;
  //</group> 

  // Return 1 if the target appears anywhere after position pos 
  // (or before, if pos is negative) in AvString; else 0.
  //<group name=contains_pos>
  int contains(char c, int pos) const;
  int contains(const AvString &y, int pos) const;
  int contains(const SubAvString &y, int pos) const;
  int contains(const char *t, int pos) const;
  int contains(const AvRegex &r, int pos) const;
  //</group>

  // Return 1 if the target appears at position pos in AvString; else 0.
  //<group name=matches>
  int matches(char c, int pos = 0) const;
  int matches(const AvString &y, int pos = 0) const;
  int matches(const SubAvString &y, int pos = 0) const;
  int matches(const char *t, int pos = 0) const;
  int matches(const AvRegex &r, int pos = 0) const;
  //</group>

  //  Return the number of occurences of target in AvString.
  //<group name=freq>
  int freq(char c) const; 
  int freq(const AvString &y) const;
  int freq(const SubAvString &y) const;
  int freq(const char *t) const;
  //</group>

  // <note> You can't take a substring of a const AvString, since this leaves 
  // open the possiblility of indirectly modifying the AvString through the 
  // SubAvString </note>
  //<group name=SubAvString>
  // extract the SubAvString "at" the argument's position.
  // <group name=at>
  SubAvString at(int pos, int len);
  SubAvString at(const AvString &x, int startpos = 0); 
  SubAvString at(const SubAvString &x, int startpos = 0); 
  SubAvString at(const char *t, int startpos = 0);
  SubAvString at(char c, int startpos = 0);
  SubAvString at(const AvRegex &r, int startpos = 0); 
  //</group>

  // Start at startpos and extract the SubAvString "before" the argument's 
  // position, exclusive.
  // <group name=before>
  SubAvString before(int pos);
  SubAvString before(const AvString &x, int startpos = 0);
  SubAvString before(const SubAvString &x, int startpos = 0);
  SubAvString before(const char *t, int startpos = 0);
  SubAvString before(char c, int startpos = 0);
  SubAvString before(const AvRegex &r, int startpos = 0);
  //</group>

  // Start at startpos and extract the SubAvString "through" to the argument's 
  // position, inclusive.
  // <group name=through>
  SubAvString through(int pos);
  SubAvString through(const AvString &x, int startpos = 0);
  SubAvString through(const SubAvString &x, int startpos = 0);
  SubAvString through(const char *t, int startpos = 0);
  SubAvString through(char c, int startpos = 0);
  SubAvString through(const AvRegex &r, int startpos = 0);
  //</group>

  // Start at startpos and extract the SubAvString "from" the argument's 
  // position, inclusive, to the AvString's end.
  // <group name=from>
  SubAvString from(int pos);
  SubAvString from(const AvString &x, int startpos = 0);
  SubAvString from(const SubAvString &x, int startpos = 0);
  SubAvString from(const char *t, int startpos = 0);
  SubAvString from(char c, int startpos = 0);
  SubAvString from(const AvRegex &r, int startpos = 0);
  //</group>

  // Start at startpos and extract the SubAvString "after" the argument's 
  // position, exclusive, to the AvString's end.
  // <group name=after>
  SubAvString after(int pos);
  SubAvString after(const AvString &x, int startpos = 0);
  SubAvString after(const SubAvString &x, int startpos = 0);
  SubAvString after(const char *t, int startpos = 0);
  SubAvString after(char c, int startpos = 0);
  SubAvString after(const AvRegex &r, int startpos = 0);
  //</group>
  //</group> 

  // Delete len chars starting at pos.
  void del(int pos, int len);

  // Delete the first occurrence of target after startpos.
  //<group name=del_after>
  void del(const AvString &y, int startpos = 0);
  void del(const SubAvString &y, int startpos = 0);
  void del(const char *t, int startpos = 0);
  void del(char c, int startpos = 0);
  void del(const AvRegex &r, int startpos = 0);
  //</group> 

  // Global substitution: substitute all occurrences of pat with repl.
  //<group name=gsub>
  int gsub(const AvString &pat, const AvString &repl);
  int gsub(const SubAvString &pat, const AvString &repl);
  int gsub(const char *pat, const AvString &repl);
  int gsub(const char *pat, const char *repl);
  int gsub(const AvRegex &pat, const AvString &repl);
  //</group>

  // return a reference to the char element at position "i".
  // <group>
  char &operator[](int i);
  char operator[](int i) const;
  // </group>

  // return the char element at position "i".
  char elem(unsigned int i) const;

  // return the first char of the AvString
  char firstchar() const;

  // return the last char of the AvString
  char lastchar() const;

  // convert to const char * with bogus cast e.g. (const char*)myAvString.
  operator const char*() const;

  // return const char *
  const char *chars() const;

  // return the length of the AvString
  unsigned int length() const;

  // return 0 if the AvString is filled, 1 if empty
  int empty() const;

  // Status
  int OK() const;

  // Preallocate some space for AvString.
  void alloc(int newsize);

  // Report the current allocation (not length!).
  int allocation() const;

  // hash function from IV strings (pjt - march 1992).
  ///  unsigned long	hash() const;

  // internal transformation to reverse order of AvString.
  void reverse();

  // internal transformation to uppercase of AvString.
  void upcase();

  // internal transformation to lowercase of AvString.
  void downcase();

  // internal transformation to capitalization of AvString.
  void capitalize();

  // global function which returns a transformation to reverse order of AvString.
  friend AvString reverse(const AvString &x);

  // global function  which returns a transformation to uppercase of AvString.
  friend AvString upcase(const AvString &x);

  // global function  which returns a transformation to lowercase of AvString.
  friend AvString downcase(const AvString &x);

  // global function  which returns a transformation to capitalization of 
  // AvString.
  friend AvString capitalize(const AvString &x);

  // Global function which splits the AvString into carray res at separators
  // and returns the number of elements.
  //<group name=split>
  friend int split(const AvString &x, AvString res[], int maxn, const AvString &sep);
  friend int split(const AvString &x, AvString res[], int maxn, const AvRegex & sep);
  //</group> 

  friend AvString common_prefix(const AvString &x, const AvString &y, 
			      int startpos = 0);
  friend AvString common_suffix(const AvString &x, const AvString &y, 
			      int startpos = -1);
  //  friend AvString replicate(char c, int n);
  friend AvString replicate(const AvString &y, int n);
  friend AvString join(AvString src[], int n, const AvString &sep);


  // IO
#if 0
  friend int readline(std::istream &s, AvString &x, char terminator = '\n',
		      int discard_terminator = 1);
#endif

  // Convert a integer to a AvString. This is a convenience function. Use the
  // oststream class (or in the future the ostringstream class) for conversion
  // of floating point data types or more sophisticated formatting options.
  // <group>
  // </group>

protected:

  // AvStrings are pointers to their representations
  AvStrRep *rep;

  // This is a helper function.
  //<group name=helperfunc>
  int search(int, int, const char*, int = -1) const;
  int search(int, int, char) const;
  int match(int, int, int, const char*, int = -1) const;
  int _gsub(const char*, int, const char *,int);
  int _gsub(const AvRegex&, const char*, int);
  SubAvString _substr(unsigned int, unsigned int);
  //</group>

};


// This is here for backward compatibility.
typedef AvString AvStrTmp;

//<summary>
// Global Functions which compare AvStrings
//</summary>

// The global function "compare" returns 0 if the arguments are equal,
// <0 is x is less and >0 is x is greater.
// <group name=compare>
int compare(const AvString &x, const AvString &y);
int compare(const AvString &x, const SubAvString &y);
int compare(const AvString &x, const char *y);
int compare(const SubAvString &x, const AvString &y);
int compare(const SubAvString &x, const SubAvString &y);
int compare(const SubAvString &x, const char *y);

// this version ignores case
int fcompare(const AvString &x, const AvString &y);
//</group>

extern AvStrRep _nilAvStrRep;
extern AvString _nilAvString;

//<summary>
// Global Functions which concatenate AvStrings with the +operator.
//</summary>

// The global "+" operator is a method of concatination
// <group name=operat>
AvString operator+(const AvString &x, const AvString &y);
AvString operator+(const AvString &x, const SubAvString &y);
AvString operator+(const AvString &x, const char *y);
AvString operator+(const AvString &x, char y);
AvString operator+(const SubAvString &x, const AvString &y);
AvString operator+(const SubAvString &x, const SubAvString &y);
AvString operator+(const SubAvString &x, const char *y);
AvString operator+(const SubAvString &x, char y);
AvString operator+(const char *x, const AvString &y);
AvString operator+(const char *x, const SubAvString &y);
// </group>

//<summary>
// Global Functions which compare AvStrings with comparison operators.
//</summary>

// The global "less than, greater than, equal to and not equal to" operators
// return 1 if fulfilled, 0 otherwise.
// <group name=comparitor>

inline int operator==(const AvString &x, const AvString &y) 
{
  return compare(x, y) == 0; 
}

inline int operator!=(const AvString &x, const AvString &y)
{
  return compare(x, y) != 0; 
}

inline int operator>(const AvString &x, const AvString &y)
{
  return compare(x, y) > 0; 
}

inline int operator>=(const AvString &x, const AvString &y)
{
  return compare(x, y) >= 0; 
}

inline int operator<(const AvString &x, const AvString &y)
{
  return compare(x, y) < 0; 
}

inline int operator<=(const AvString &x, const AvString &y)
{
  return compare(x, y) <= 0; 
}

inline int operator==(const AvString &x, const SubAvString &y) 
{
  return compare(x, y) == 0; 
}

inline int operator!=(const AvString &x, const SubAvString &y)
{
  return compare(x, y) != 0; 
}

inline int operator>(const AvString &x, const SubAvString &y) 
{
  return compare(x, y) > 0; 
}

inline int operator>=(const AvString &x, const SubAvString &y)
{
  return compare(x, y) >= 0; 
}

inline int operator<(const AvString &x, const SubAvString &y) 
{
  return compare(x, y) < 0; 
}

inline int operator<=(const AvString &x, const SubAvString &y)
{
  return compare(x, y) <= 0; 
}

inline int operator==(const AvString &x, const char *t) 
{
  return compare(x, t) == 0; 
}

inline int operator!=(const AvString &x, const char *t) 
{
  return compare(x, t) != 0; 
}

inline int operator>(const AvString &x, const char *t) 
{
  return compare(x, t) > 0; 
}

inline int operator>=(const AvString &x, const char *t) 
{
  return compare(x, t) >= 0; 
}

inline int operator<(const AvString &x, const char *t) 
{
  return compare(x, t) < 0; 
}

inline int operator<=(const AvString &x, const char *t) 
{
  return compare(x, t) <= 0; 
}

inline int operator==(const SubAvString &x, const AvString &y) 
{
  return compare(y, x) == 0; 
}

inline int operator!=(const SubAvString &x, const AvString &y)
{
  return compare(y, x) != 0;
}

inline int operator>(const SubAvString &x, const AvString &y) 
{
  return compare(y, x) < 0;
}

inline int operator>=(const SubAvString &x, const AvString &y) 
{
  return compare(y, x) <= 0;
}

inline int operator<(const SubAvString &x, const AvString &y) 
{
  return compare(y, x) > 0;
}

inline int operator<=(const SubAvString &x, const AvString &y) 
{
  return compare(y, x) >= 0;
}

inline int operator==(const SubAvString &x, const SubAvString &y) 
{
  return compare(x, y) == 0; 
}

inline int operator!=(const SubAvString &x, const SubAvString &y)
{
  return compare(x, y) != 0;
}

inline int operator>(const SubAvString &x, const SubAvString &y) 
{
  return compare(x, y) > 0;
}

inline int operator>=(const SubAvString &x, const SubAvString &y)
{
  return compare(x, y) >= 0;
}

inline int operator<(const SubAvString &x, const SubAvString &y) 
{
  return compare(x, y) < 0;
}

inline int operator<=(const SubAvString &x, const SubAvString &y)
{
  return compare(x, y) <= 0;
}

inline int operator==(const SubAvString &x, const char *t) 
{
  return compare(x, t) == 0; 
}

inline int operator!=(const SubAvString &x, const char *t) 
{
  return compare(x, t) != 0;
}

inline int operator>(const SubAvString &x, const char *t) 
{
  return compare(x, t) > 0; 
}

inline int operator>=(const SubAvString &x, const char *t) 
{
  return compare(x, t) >= 0; 
}

inline int operator<(const SubAvString &x, const char *t) 
{
  return compare(x, t) < 0; 
}

inline int operator<=(const SubAvString &x, const char *t) 
{
  return compare(x, t) <= 0; 
}
//</group>


//#-----------------begin inlining------------------------------------

//#
//# status reports, needed before defining other things
//#

inline unsigned int AvString::length() const { return rep->len; }
inline int AvString::empty() const { return rep->len == 0; }
inline const char *AvString::chars() const { return &(rep->s[0]); }
inline int AvString::allocation() const { return rep->sz; }
inline void AvString::alloc(int newsize) { rep = Sresize(rep, newsize); }

inline unsigned int SubAvString::length() const { return len; }
inline int SubAvString::empty() const { return len == 0; }
inline const char *SubAvString::chars() const { return &(S.rep->s[pos]); }


//#
//# constructors
//#

inline AvString::AvString(const AvString &x) 
  : rep(Scopy(0, x.rep)) {}
inline AvString::AvString(const char *t) 
  : rep(AvSalloc(0, t, -1, -1)) {}
inline AvString::AvString(const char *t, int tlen)
  : rep(AvSalloc(0, t, tlen, tlen)) {}
inline AvString::AvString(const SubAvString &y)
  : rep(AvSalloc(0, y.chars(), y.length(), y.length())) {}
inline AvString::AvString(char c) 
  : rep(AvSalloc(0, &c, 1, 1)) {}

inline SubAvString::SubAvString(const SubAvString &x)
  :S(x.S), pos(x.pos), len(x.len) {}
inline SubAvString::SubAvString(AvString &x, unsigned int first, unsigned int l)
  :S(x), pos(first), len(first+l > x.length()  ?  x.length()-first : l) {}

inline SubAvString::~SubAvString() {}

//#
//# assignment
//#

inline void AvString::operator = (const AvString &y)
{ 
  rep = Scopy(rep, y.rep);
}

inline void AvString::operator=(const char *t)
{
  rep = AvSalloc(rep, t, -1, -1); 
}

inline void AvString::operator=(const SubAvString &y)
{
  rep = AvSalloc(rep, y.chars(), y.length(), y.length());
}

inline void AvString::operator=(char c)
{
  rep = AvSalloc(rep, &c, 1, 1); 
}


inline void SubAvString::operator = (const char *ys)
{
  assign(0, ys);
}

inline void SubAvString::operator = (char ch)
{
  assign(0, &ch, 1);
}

inline void SubAvString::operator = (const AvString &y)
{
  assign(y.rep, y.chars(), y.length());
}

inline void SubAvString::operator = (const SubAvString &y)
{
  assign(y.S.rep, y.chars(), y.length());
}

//#
//# operator versions of cats
//#

inline void AvString::operator +=(const AvString &y) {
  rep = Scat(rep, chars(), length(), y.chars(), y.length());
}

inline void AvString::operator +=(const SubAvString &y) {
  rep = Scat(rep, chars(), length(), y.chars(), y.length());
}

inline void AvString::operator += (const char *y) {
  rep = Scat(rep, chars(), length(), y, -1);
}

inline void AvString:: operator +=(char y) {
  rep = Scat(rep, chars(), length(), &y, 1);
}

//#
//# element extraction
//#

inline char &AvString::operator [] (int i) 
{ 
  if (((unsigned int)i) >= length()) {
    Av_stringThrowError("invalid index");
  }
  return rep->s[i];
}

inline char AvString::operator [] (int i) const
{ 
  if (((unsigned int)i) >= length()) {
    Av_stringThrowError("invalid index");
  }
  return rep->s[i];
}

inline char AvString::elem (unsigned int i) const
{ 
  if (i >= length()) {
    Av_stringThrowError("invalid index");
  }
  return rep->s[i];
}

inline char AvString::firstchar() const
{ 
  return elem(0);
}

inline char AvString::lastchar() const
{ 
  return elem(length() - 1);
}


//#
//# searching
//#

inline int AvString::index(char c, int startpos) const
{
  return search(startpos, length(), c);
}

inline int AvString::index(const char *t, int startpos) const
{ 
  return search(startpos, length(), t);
}

inline int AvString::index(const AvString &y, int startpos) const
{ 
  return search(startpos, length(), y.chars(), y.length());
}

inline int AvString::index(const SubAvString &y, int startpos) const
{ 
  return search(startpos, length(), y.chars(), y.length());
}

inline int AvString::index(const AvRegex &r, int startpos) const
{
  int unused;  return r.search(chars(), length(), unused, startpos);
}



inline int AvString::contains(char c) const
{
  return search(0, length(), c) >= 0;
}

inline int AvString::contains(const char *t) const
{ 
  return search(0, length(), t) >= 0;
}

inline int AvString::contains(const AvString &y) const
{ 
  return search(0, length(), y.chars(), y.length()) >= 0;
}

inline int AvString::contains(const SubAvString &y) const
{ 
  return search(0, length(), y.chars(), y.length()) >= 0;
}

inline int AvString::contains(char c, int p) const
{
  return match(p, length(), 0, &c, 1) >= 0;
}

inline int AvString::contains(const char *t, int p) const
{
  return match(p, length(), 0, t) >= 0;
}

inline int AvString::contains(const AvString &y, int p) const
{
  return match(p, length(), 0, y.chars(), y.length()) >= 0;
}

inline int AvString::contains(const SubAvString &y, int p) const
{
  return match(p, length(), 0, y.chars(), y.length()) >= 0;
}

inline int AvString::contains(const AvRegex &r) const
{
  int unused;  return r.search(chars(), length(), unused, 0) >= 0;
}

inline int AvString::contains(const AvRegex &r, int p) const
{
  return r.match(chars(), length(), p) >= 0;
}


inline int AvString::matches(const SubAvString &y, int p) const
{
  return match(p, length(), 1, y.chars(), y.length()) >= 0;
}

inline int AvString::matches(const AvString &y, int p) const
{
  return match(p, length(), 1, y.chars(), y.length()) >= 0;
}

inline int AvString::matches(const char *t, int p) const
{
  return match(p, length(), 1, t) >= 0;
}

inline int AvString::matches(char c, int p) const
{
  return match(p, length(), 1, &c, 1) >= 0;
}

inline int AvString::matches(const AvRegex &r, int p) const
{
  int l = (p < 0)? -p : length() - p;
  return r.match(chars(), length(), p) == l;
}


inline int SubAvString::contains(const char *t) const
{ 
  return S.search(pos, pos+len, t) >= 0;
}

inline int SubAvString::contains(const AvString &y) const
{ 
  return S.search(pos, pos+len, y.chars(), y.length()) >= 0;
}

inline int SubAvString::contains(const SubAvString &y) const
{ 
  return S.search(pos, pos+len, y.chars(), y.length()) >= 0;
}

inline int SubAvString::contains(char c) const
{
  return S.search(pos, pos+len, c) >= 0;
}

inline int SubAvString::contains(const AvRegex &r) const
{
  int unused;  return r.search(chars(), len, unused, 0) >= 0;
}

inline int SubAvString::matches(const AvRegex &r) const
{
  return r.match(chars(), len, 0) == int(len);
}


inline int AvString::gsub(const AvString &pat, const AvString &r)
{
  return _gsub(pat.chars(), pat.length(), r.chars(), r.length());
}

inline int AvString::gsub(const SubAvString &pat, const AvString &r)
{
  return _gsub(pat.chars(), pat.length(), r.chars(), r.length());
}

inline int AvString::gsub(const AvRegex &pat, const AvString &r)
{
  return _gsub(pat, r.chars(), r.length());
}

inline int AvString::gsub(const char *pat, const AvString &r)
{
  return _gsub(pat, -1, r.chars(), r.length());
}

inline int AvString::gsub(const char *pat, const char *r)
{
  return _gsub(pat, -1, r, -1);
}



inline std::ostream &operator<<(std::ostream &s, const AvString &x)
{
   s << x.chars(); return s;
}



//
// This is a helper needed by at, before, etc.
//

inline SubAvString AvString::_substr(unsigned int first, unsigned int l)
{
  if (first >= length() )
    return SubAvString(_nilAvString, 0, 0) ;
  else 
    return SubAvString(*this, first, l);
}

#endif  /* AIPS_STRING_H */
