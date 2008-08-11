//# AvRegex.h: Regular expression class
//# Copyright (C) 1993,1994,1995,1996,1999,2002
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
//# $Id: AvRegex.h,v 19.0 2003/07/16 05:47:43 aips2adm Exp $
//# --------------------------------------------------
//
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvRegex.h,v 19.0 2003/07/16 05:47:43 aips2adm Exp $
//
// $Log: AvRegex.h,v $
// Revision 19.0  2003/07/16 05:47:43  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:20  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/14 17:42:19  hravlin
// Replaced "class ostream;" with inclusion of iostream.h.
//
// Revision 17.0  2001/11/12 19:42:49  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:55  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:58  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:48  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:37  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:44  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.1  1999/05/27 08:27:29  wbrouw
// Removed ifdef _AIX dependency
//
// Revision 11.0  1998/10/03 07:01:19  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:24  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:30:07  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:18:28  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1996/12/12 09:25:25  droberts
// Changed RX* to AvRX*
//
// Revision 1.2  1996/11/18 20:40:12  hr
// Added RCS header.
//
//
//---------------------------------------------------------------------------

#if !defined(_AvRegex_h_)
#define _AvRegex_h_


#if defined(__GNUG__)
#pragma interface
#endif

#if defined(SHORT_NAMES) || defined(VMS)
#define re_compile_pattern	recmppat
#define re_pattern_buffer	repatbuf
#define re_registers		reregs
#endif

#include <iostream.h>
// Forward declarations.
class  AvString;

struct re_pattern_buffer;       // defined elsewhere
struct re_registers;

// <summary> Regular expression class </summary>
// <use visibility=export>
// <reviewed reviewer="Friso Olnon" date="1995/03/20" tests="tRegex" demos="">
// </reviewed>

// <synopsis> 
// This class provides regular expression functionality, such as
// matching and searching in strings, comparison of expressions, and
// input/output. It is built on the GNU function library (see files 
// cregex.h and cregex.cc).
// </synopsis> 

// <todo asof="1995/03/17">
//   <li> Decide on documentation of GNU stuff (cregex.h, cregex.cc)
// </todo>

class AvRegex
{
public:
    // Default constructor uses a zero-length regular expression.
    // <thrown>
    //  <li> RegexExpressnError
    // </thrown>
    AvRegex();
    
    // Construct a regular expression.
    // Optionally a fast map can be created, a buffer size can be given
    // and a translation table (of 256 chars) can be applied.
    // The translation table can, for instance, be used to map
    // lowercase characters to uppercase.
    // See cregex.cc (the extended regular expression matching and search
    // library) for detailed information.
    // <thrown>
    //  <li> RegexExpressnError
    // </thrown>
    AvRegex(const AvString&, 
	  int fast = 0, 
	  int bufsize = 40, 
	  const char* transtable = 0);
    
    // Copy constructor (copy semantics).
    // <thrown>
    //  <li> RegexExpressnError
    // </thrown>
    AvRegex(const AvRegex&);
    
    ~AvRegex();
    
    // Assignment (copy semantics).
    // <thrown>
    //  <li> RegexExpressnError
    // </thrown>
    // <group>
    AvRegex& operator = (const AvRegex&);
    AvRegex& operator = (const AvString&);
    // </group>
    
    // Get the regular expression string.
    const AvString& regexp() const;
    
    // Get the translation table (can be a zero pointer).
    const char* transtable() const;
    
    // Test if the regular expression matches (part of) string <src>s</src>.
    // The return value gives the length of the matching string part,
    // -1 if there is no match, or -2 in case of an internal error.
    // The string has <src>len</src> characters and the test starts at
    // position <src>pos</src>. The string may contain null characters.
    //
    // <note role=tip>
    // Use the appropriate <linkto class=AvString>String</linkto> functions
    // to test if a string matches a regular expression. 
    // <src>AvRegex::match</src> is pretty low-level.
    // </note>
    int match(const char* s, int len, int pos = 0) const;
    
    // Test if the regular expression occurs in string <src>s</src>.
    // The return value gives the position of the first substring
    // matching the regular expression. The length of that substring
    // is returned in <src>matchlen</src>.
    // The string has <src>len</src> characters and the test starts at
    // position <src>pos</src>. The string may contain null characters.
    //
    // <note role=tip>
    // Use the appropriate <linkto class=AvString>String</linkto> functions
    // to test if a regular expression occurs in a string.
    // <src>AvRegex::search</src> is pretty low-level.
    // </note>
    int search(const char* s, int len, int& matchlen, int startpos = 0) const;
    
    // Return some internal <src>cregex</src> info.
    int match_info(int& start, int& length, int nth = 0) const;
    
    // Representation invariant.
    // <thrown>
    //  <li> RegexMemAllocError
    // </thrown>
    int OK() const;
    
    // Write as ASCII.
   
    friend ostream& operator<< (ostream&, const AvRegex&);
    
protected:

    AvString*            str;                 // the reg. exp.
    int                fastval;             // fast flag
    int                bufsz;               // buffer size given
    char*              trans;               // possible translation table
    re_pattern_buffer* buf;                 // compiled reg.exp.
    re_registers*      reg;                 //# internal reg.exp. stuff
    
    // Compile the regular expression
    // <thrown>
    //  <li> RegexExpressnError
    // </thrown>
    void create (const AvString&, int, int, const char*);
    
    // Deallocate the stuff allocated by <src>create</src>.
    void dealloc ();
};


// some built in regular expressions

extern const AvRegex AvRXwhite;          // = "[ \n\t\r\v\f]+"
extern const AvRegex AvRXint;            // = "-?[0-9]+"
extern const AvRegex AvRXdouble;         // = "-?\\(\\([0-9]+\\.[0-9]*\\)\\|
                                     //    \\([0-9]+\\)\\|\\(\\.[0-9]+\\)\\)
                                     //    \\([eE][---+]?[0-9]+\\)?"
extern const AvRegex AvRXalpha;          // = "[A-Za-z]+"
extern const AvRegex AvRXlowercase;      // = "[a-z]+"
extern const AvRegex AvRXuppercase;      // = "[A-Z]+"
extern const AvRegex AvRXalphanum;       // = "[0-9A-Za-z]+"
extern const AvRegex AvRXidentifier;     // = "[A-Za-z_][A-Za-z0-9_]*"

#endif

















