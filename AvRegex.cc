//# AvRegex.cc: Regular expression class
//# Copyright (C) 1993,1994,1995,1996,2002
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
//# $Id: AvRegex.cc,v 19.0 2003/07/16 05:47:47 aips2adm Exp $
//# --------------------------------------------------
//
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvRegex.cc,v 19.0 2003/07/16 05:47:47 aips2adm Exp $
//
// $Log: AvRegex.cc,v $
// Revision 19.0  2003/07/16 05:47:47  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:24  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/22 22:19:19  hravlin
// Rearranged some includes and included limits.h to get around compiler
// warnings about redefining a constant on some systems.
//
// Revision 17.0  2001/11/12 19:42:52  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:58  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:10  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:53  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:41  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:52  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:31  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:38  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:30:27  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.3  1997/05/06 15:28:19  hr
// buf->buffer also needs to be allocated by malloc. (cregex calls realloc).
//
// Revision 8.2  1997/05/05 20:02:35  hr
// in dealloc(), buf->buffer needs to be free'd rather than deleted.
//
// Revision 8.1  1997/03/17 21:20:54  hr
// Changes to track entry point names in cregex.
//
// Revision 8.0  1997/02/20 03:18:40  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.4  1996/12/12 07:55:35  droberts
// Changed RX* to AvRX*
//
// Revision 1.2  1996/11/18 20:40:12  hr
// Added RCS header.
//
//
//---------------------------------------------------------------------------

// AvRegex class implementation

#include <stdlib.h>		// For the call to free().
#include <string.h>
#include <iostream.h>
// cregex.h (re)defines RE_DUP_MAX. Some systems define it in limits.
// Include it here to avoid compiler warnings about it redefining ours.
#include <limits.h>

// extern "C" {
#ifdef AIPSPLUSPLUS
#include <aips/Utilities/cregex.h>
#else
#include <cregex.h>
#endif
// }

#include <AvRegex.h>
#include <AvString.h>

AvRegex::AvRegex ()
    { create ("",0,0,0); }

void AvRegex::create (const AvString& exp, int fast, int bufsize, 
		    const char* transtable)
{
  str     = new AvString(exp);
  fastval = fast;
  bufsz   = bufsize;
  trans   = 0;
  if (transtable) {
    trans = new char[256];
    memcpy (trans, transtable, 256);
  }
  int tlen = exp.length();
  buf = new re_pattern_buffer;
  reg = new re_registers;
  if (fast)
    buf->fastmap = new char[256];
  else
    buf->fastmap = 0;
  buf->translate = trans;
  if (tlen > bufsize)
    bufsize = tlen;
  buf->allocated = bufsize;
//  buf->buffer = new char [buf->allocated];
  buf->buffer = (char *) malloc(buf->allocated);
  //Corrected dbx rui found using check -all, but no effect on SGI code
  //memset(buf->buffer, 0, buf->allocated);
  char* msg = a2_re_compile_pattern((char*)(exp.chars()), tlen, buf);
  if (msg != 0) 
    {
      cerr << "AvRegex::AvRegex() - Regular Expression Error: " << msg << endl;
    } 
  else if (fast)
    a2_re_compile_fastmap(buf);
}

void AvRegex::dealloc()
{
  free(buf->buffer);
  delete [] buf->fastmap;
  delete buf;
  delete reg;
  delete str;
  delete [] trans;
}



int AvRegex::match_info(int& start, int& length, int nth) const
{
  if ((unsigned)(nth) >= RE_NREGS)
    return 0;
  else
  {
    start = reg->start[nth];
    length = reg->end[nth] - start;
    return start >= 0 && length >= 0;
  }
}

ostream& operator<< (ostream& ios, const AvRegex& exp)
    { return ios << *exp.str; }


int AvRegex::search(const char* s, int len, int& matchlen, int startpos) const
{
  int matchpos, pos, range;
  if (startpos >= 0)
  {
    pos = startpos;
    range = len - startpos;
  }
  else
  {
    pos = len + startpos;
    range = -pos;
  }
  matchpos = a2_re_search_2(buf, 0, 0, (char*)s, len, pos, range, reg, len);
  if (matchpos >= 0)
    matchlen = reg->end[0] - reg->start[0];
  else
    matchlen = 0;
  return matchpos;
}

int AvRegex::match(const char*s, int len, int p) const
{
  if (p < 0)
  {
    p += len;
    if (p > len)
      return -1;
    return a2_re_match_2(buf, 0, 0, (char*)s, p, 0, reg, p);
  }
  else if (p > len)
    return -1;
  else
    return a2_re_match_2(buf, 0, 0, (char*)s, len, p, reg, len);
}

int AvRegex::OK() const
{
// can't verify much, since we've lost the original string
  int v = buf != 0;             // have a regex buf
  v &= buf->buffer != 0;        // with a pat
  if (!v) 
    {
      cerr << "AvRegex invariant failure" << endl;
      cerr.flush();
    }
  return v;
}


AvRegex::AvRegex (const AvString& exp, int fast, int sz, const char* translation)
{ create (exp,fast,sz,translation); }

AvRegex::~AvRegex ()
{ dealloc(); }

AvRegex::AvRegex (const AvRegex& that)
{ create (*that.str, that.fastval, that.bufsz, that.trans); }

AvRegex& AvRegex::operator= (const AvRegex& that)
{
    dealloc();
    create (*that.str, that.fastval, that.bufsz, that.trans);
    return *this;
}

AvRegex& AvRegex::operator= (const AvString& strng)
{
    dealloc();
    create (strng, 0, 40, 0);
    return *this;
}

const AvString& AvRegex::regexp() const
{ return *str; }

const char* AvRegex::transtable() const
{ return trans; }

/*
 some built-in Regular expressions
*/

const AvRegex AvRXwhite("[ \n\t\r\v\f]+", 1);
const AvRegex AvRXint("-?[0-9]+", 1);
const AvRegex AvRXdouble("-?\\(\\([0-9]+\\.[0-9]*\\)\\|\\([0-9]+\\)\\|\\(\\.[0-9]+\\)\\)\\([eE][---+]?[0-9]+\\)?", 1, 200);
const AvRegex AvRXalpha("[A-Za-z]+", 1);
const AvRegex AvRXlowercase("[a-z]+", 1);
const AvRegex AvRXuppercase("[A-Z]+", 1);
const AvRegex AvRXalphanum("[0-9A-Za-z]+", 1);
const AvRegex AvRXidentifier("[A-Za-z_][A-Za-z0-9_]*", 1);
