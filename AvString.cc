//# AvString.cc: AvString classes
//# Copyright (C) 1992-1999,2000,2002
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
//# $Id: AvString.cc,v 19.1 2004/02/04 15:26:14 gvandiep Exp $

//# AIPS++ history:
//#	10-mar-92  pjt  added the hash() member function from ivstring

#include <AvString.h>

#include <string.h>

#include <ctype.h>

#ifdef AIPSPLUSPLUS
#include <aips/Utilities/cregex.h>
#else
#include <cregex.h>
#endif

// change the following if your machine is weird
#if defined (_AIX)
# undef CHAR_BIT
#endif
#define CHAR_BIT 8

// minimum & maximum representable rep size

// #define MAXAvStrRep_SIZE   ((1 << (sizeof(short)* CHAR_BIT - 1)) - 1)
#define MINAvStrRep_SIZE   16

#if !defined(MALLOC_MIN_OVERHEAD)
#define MALLOC_MIN_OVERHEAD  4
#endif

// The basic allocation primitive:
// Always round request to something close to a power of two.
// This ensures a bit of padding, which often means that
// concatenations don't have to realloc. Plus it tends to
// be faster when lots of AvStrings are created and discarded,
// since just about any version of malloc (op new()) will
// be faster when it can reuse identically-sized chunks


inline static AvStrRep* Snew(int newsiz)
{
  unsigned int siz =
    (unsigned int)sizeof(AvStrRep) + newsiz + MALLOC_MIN_OVERHEAD;
  unsigned int allocsiz = MINAvStrRep_SIZE;
  while (allocsiz < siz) allocsiz <<= 1;
  allocsiz -= MALLOC_MIN_OVERHEAD;
    
  AvStrRep* rep = (AvStrRep *) new char[allocsiz];
  rep->sz = allocsiz - (unsigned int)sizeof(AvStrRep);
  return rep;
}


AvString::AvString() : rep(&_nilAvStrRep)
{
    // Nothing
}

#if 0
AvString::AvString(std::ostringstream &os)
  : rep(Snew(os.pcount()+1))
{   os << ends;
    memcpy(rep->s, os.str(), os.pcount());
    rep->s[os.pcount()] = 0;
    rep->len = strlen(os.str());
    // rep->len = os.pcount();
    delete [] os.str();
}
#endif

AvString::operator const char*() const
{ 
  return (const char*)chars();
}

AvString::~AvString()
{
    if (rep != &_nilAvStrRep)
	delete [] (char*)rep;
}


//  globals

AvStrRep  _nilAvStrRep = { 0, 1, { 0 } }; // nil strings point here
AvString _nilAvString;               // nil SubAvStrings point here




/*
 the following inline fcts are specially designed to work
 in support of AvString classes, and are not meant as generic replacements
 for libc "str" functions.

 inline copy fcts -  I like left-to-right from->to arguments.
 all versions assume that `to' argument is non-null

 These are worth doing inline, rather than through calls because,
 via procedural integration, adjacent copy calls can be smushed
 together by the optimizer.
*/

// copy n bytes
inline static void ncopy(const char* from, char* to, int n)
{
  if (from != to) while (--n >= 0) *to++ = *from++;
}

// copy n bytes, null-terminate
inline static void ncopy0(const char* from, char* to, int n)
{
  if (from != to) 
  {
    while (--n >= 0) *to++ = *from++;
    *to = 0;
  }
  else
    to[n] = 0;
}

// copy until null
inline static void scopy(const char* from, char* to)
{
  if (from != 0) while((*to++ = *from++) != 0);
}

// copy right-to-left
inline static void revcopy(const char* from, char* to, int n)
{
  if (from != 0) while (--n >= 0) *to-- = *from--;
}


inline static int slen(const char* t) // strlen
{
  if (t == 0)
    return 0;
  else
  {
    const char* a = t;
    while (*a++ != 0);
    return (int)(a - 1 - t);
  }
}


// Do-something-while-allocating routines.

// We live with two ways to signify empty Sreps: either the
// null pointer (0) or a pointer to the nilAvStrRep.

// We always signify unknown source lengths (usually when fed a char*)
// via len == -1, in which case it is computed.

// allocate, copying src if nonull

AvStrRep* AvSalloc(AvStrRep* old, const char* src, int srclen, int newlen)
{
  if (old == &_nilAvStrRep) old = 0;
  if (srclen < 0) srclen = slen(src);
  if (newlen < srclen) newlen = srclen;
  AvStrRep* rep;
  if (old == 0 || newlen > (int)(old->sz))
    rep = Snew(newlen);
  else
    rep = old;

  rep->len = newlen;
  ncopy0(src, rep->s, srclen);

  if (old != rep && old != 0) delete [] (char *)old;

  return rep;
}

// reallocate: Given the initial allocation scheme, it will
// generally be faster in the long run to get new space & copy
// than to call realloc

AvStrRep* Sresize(AvStrRep* old, int newlen)
{
  if (old == &_nilAvStrRep) old = 0;
  AvStrRep* rep;
  if (old == 0)
    rep = Snew(newlen);
  else if (newlen > (int)(old->sz))
  {
    rep = Snew(newlen);
    ncopy0(old->s, rep->s, old->len);
    delete [] (char *)old;
  }
  else
    rep = old;

  rep->len = newlen;

  return rep;
}

// like allocate, but we know that src is a AvStrRep

AvStrRep* Scopy(AvStrRep* old, AvStrRep* s)
{
  if (old == &_nilAvStrRep) old = 0;
  if (s == &_nilAvStrRep) s = 0;
  if (old == s) 
    return (old == 0)? &_nilAvStrRep : old;
  else if (s == 0)
  {
    old->s[0] = 0;
    old->len = 0;
    return old;
  }
  else 
  {
    AvStrRep* rep;
    int newlen = s->len;
    if (old == 0 || newlen > (int)(old->sz))
    {
      if (old != 0) delete [] (char *)old;
      rep = Snew(newlen);
    }
    else
      rep = old;
    rep->len = newlen;
    ncopy0(s->s, rep->s, newlen);
    return rep;
  }
}

// allocate & concatenate

AvStrRep* Scat(AvStrRep* old, const char* s, int srclen, const char* t, int tlen)
{
  if (old == &_nilAvStrRep) old = 0;
  if (srclen < 0) srclen = slen(s);
  if (tlen < 0) tlen = slen(t);
  int newlen = srclen + tlen;
  AvStrRep* rep;

  if (old == 0 || newlen > (int)(old->sz) || 
      (t >= old->s && t < &(old->s[old->len]))) // beware of aliasing
    rep = Snew(newlen);
  else
    rep = old;

  rep->len = newlen;

  ncopy(s, rep->s, srclen);
  ncopy0(t, &(rep->s[srclen]), tlen);

  if (old != rep && old != 0) delete [] (char *)old;

  return rep;
}

// double-concatenate

AvStrRep* Scat(AvStrRep* old, const char* s, int srclen, const char* t, int tlen,
             const char* u, int ulen)
{
  if (old == &_nilAvStrRep) old = 0;
  if (srclen < 0) srclen = slen(s);
  if (tlen < 0) tlen = slen(t);
  if (ulen < 0) ulen = slen(u);
  int newlen = srclen + tlen + ulen;
  AvStrRep* rep;
  if (old == 0 || newlen > (int)(old->sz) || 
      (t >= old->s && t < &(old->s[old->len])) ||
      (u >= old->s && u < &(old->s[old->len])))
    rep = Snew(newlen);
  else
    rep = old;

  rep->len = newlen;

  ncopy(s, rep->s, srclen);
  ncopy(t, &(rep->s[srclen]), tlen);
  ncopy0(u, &(rep->s[srclen+tlen]), ulen);

  if (old != rep && old != 0) delete [] (char *)old;

  return rep;
}

// like cat, but we know that new stuff goes in the front of existing rep

AvStrRep* Sprepend(AvStrRep* old, const char* t, int tlen)
{
  char* s;
  int srclen;
  if (old == &_nilAvStrRep || old == 0)
  {
    s = 0; old = 0; srclen = 0;
  }
  else
  {
    s = old->s; srclen = old->len;
  }
  if (tlen < 0) tlen = slen(t);
  int newlen = srclen + tlen;
  AvStrRep* rep;
  if (old == 0 || newlen > (int)(old->sz) || 
      (t >= old->s && t < &(old->s[old->len])))
    rep = Snew(newlen);
  else
    rep = old;

  rep->len = newlen;

  revcopy(&(s[srclen]), &(rep->s[newlen]), srclen+1);
  ncopy(t, rep->s, tlen);

  if (old != rep && old != 0) delete [] (char *)old;

  return rep;
}


// string compare: first argument is known to be non-null

inline static int scmp(const char* a, const char* b)
{
  if (b == 0)
    return *a != 0;
  else
  {
    int diff = 0;
    while ((diff = *a - *b++) == 0 && *a++ != 0);
    return diff;
  }
}


inline static int ncmp(const char* a, int al, const char* b, int bl)
{
  int n = (al <= bl)? al : bl;
  int diff;
  while (n-- > 0) if ((diff = *a++ - *b++) != 0) return diff;
  return al - bl;
}

int fcompare(const AvString& x, const AvString& y)
{
  const char* a = x.chars();
  const char* b = y.chars();
  int al = x.length();
  int bl = y.length();
  int n = (al <= bl)? al : bl;
  int diff = 0;
  while (n-- > 0)
  {
    char ac = *a++;
    char bc = *b++;
    if ((diff = ac - bc) != 0)
    {
      if (ac >= 'a' && ac <= 'z')
        ac = ac - 'a' + 'A';
      if (bc >= 'a' && bc <= 'z')
        bc = bc - 'a' + 'A';
      if ((diff = ac - bc) != 0)
        return diff;
    }
  }
  return al - bl;
}

// these are not inline, but pull in the above inlines, so are 
// pretty fast

int compare(const AvString& x, const char* b)
{
  return scmp(x.chars(), b);
}

int compare(const AvString& x, const AvString& y)
{
  return ncmp(x.chars(), x.length(), y.chars(), y.length());
}

int compare(const AvString& x, const SubAvString& y)
{
  return ncmp(x.chars(), x.length(), y.chars(), y.length());
}

int compare(const SubAvString& x, const AvString& y)
{
  return ncmp(x.chars(), x.length(), y.chars(), y.length());
}

int compare(const SubAvString& x, const SubAvString& y)
{
  return ncmp(x.chars(), x.length(), y.chars(), y.length());
}

int compare(const SubAvString& x, const char* b)
{
  if (b == 0)
    return x.length();
  else
  {
    const char* a = x.chars();
    int n = x.length();
    int diff;
    while (n-- > 0) if ((diff = *a++ - *b++) != 0) return diff;
    return (*b == 0) ? 0 : -1;
  }
}

/*
 index fcts
*/

int AvString::search(int start, int sl, char c) const
{
  const char* s = chars();
  if (sl > 0)
  {
    if (start >= 0)
    {
      const char* a = &(s[start]);
      const char* lasta = &(s[sl]);
      while (a < lasta) if (*a++ == c) return (int)(--a - s);
    }
    else
    {
      const char* a = &(s[sl + start + 1]);
      while (--a >= s) if (*a == c) return (int)(a - s);
    }
  }
  return -1;
}

int AvString::search(int start, int sl, const char* t, int tl) const
{
  const char* s = chars();
  if (tl < 0) tl = slen(t);
  if (sl > 0 && tl > 0)
  {
    if (start >= 0)
    {
      const char* lasts = &(s[sl - tl]);
      const char* lastt = &(t[tl]);
      const char* p = &(s[start]);

      while (p <= lasts)
      {
        const char* x = p++;
        const char* y = t;
        while (*x++ == *y++) if (y >= lastt) return (int)(--p - s);
      }
    }
    else
    {
      const char* firsts = &(s[tl - 1]);
      const char* lastt =  &(t[tl - 1]);
      const char* p = &(s[sl + start + 1]); 

      while (--p >= firsts)
      {
        const char* x = p;
        const char* y = lastt;
        while (*x-- == *y--) if (y < t) return (int)(++x - s);
      }
    }
  }
  return -1;
}

int AvString::match(int start, int sl, int exact, const char* t, int tl) const
{
  if (tl < 0) tl = slen(t);

  if (start < 0)
  {
    start = sl + start - tl + 1;
    if (start < 0 || (exact && start != 0))
      return -1;
  }
  else if (exact && sl - start != tl)
    return -1;

  if (sl == 0 || tl == 0 || sl - start < tl || start >= sl)
    return -1;

  int n = tl;
  const char* s = &(rep->s[start]);
  while (--n >= 0) if (*s++ != *t++) return -1;
  return tl;
}

void SubAvString::assign(AvStrRep* ysrc, const char* ys, int ylen)
{
  if (&S == &_nilAvString) return;

  if (ylen < 0) ylen = slen(ys);
  AvStrRep* targ = S.rep;
  int sl = targ->len - len + ylen;

  if (ysrc == targ || sl >= (int)(targ->sz))
  {
    AvStrRep* oldtarg = targ;
    targ = Sresize(0, sl);
    ncopy(oldtarg->s, targ->s, pos);
    ncopy(ys, &(targ->s[pos]), ylen);
    scopy(&(oldtarg->s[pos + len]), &(targ->s[pos + ylen]));
    delete [] (char*)oldtarg;
  }
  else if (len == (unsigned int)ylen)
    ncopy(ys, &(targ->s[pos]), len);
  else if (ylen < (int)len)
  {
    ncopy(ys, &(targ->s[pos]), ylen);
    scopy(&(targ->s[pos + len]), &(targ->s[pos + ylen]));
  }
  else
  {
    revcopy(&(targ->s[targ->len]), &(targ->s[sl]), targ->len-pos-len +1);
    ncopy(ys, &(targ->s[pos]), ylen);
  }
  targ->len = sl;
  S.rep = targ;
}



/*
 * substitution
 */


int AvString::_gsub(const char* pat, int pl, const char* r, int rl)
{
  int nmatches = 0;
  if (pl < 0) pl = slen(pat);
  if (rl < 0) rl = slen(r);
  int sl = length();
  if (sl <= 0 || pl <= 0 || sl < pl)
    return nmatches;
  
  const char* s = chars();

  // prepare to make new rep
  AvStrRep* nrep = 0;
  int nsz = 0;
  char* x = 0;

  int si = 0;
  int xi = 0;
  int remaining = sl;

  while (remaining >= pl)
  {
    int pos = search(si, sl, pat, pl);
    if (pos < 0)
      break;
    else
    {
      ++nmatches;
      int mustfit = xi + remaining + rl - pl;
      if (mustfit >= nsz)
      {
        if (nrep != 0) nrep->len = xi;
        nrep = Sresize(nrep, mustfit);
        nsz = nrep->sz;
        x = nrep->s;
      }
      pos -= si;
      ncopy(&(s[si]), &(x[xi]), pos);
      ncopy(r, &(x[xi + pos]), rl);
      si += pos + pl;
      remaining -= pos + pl;
      xi += pos + rl;
    }
  }

  if (nrep == 0)
  {
    if (nmatches == 0)
      return nmatches;
    else
      nrep = Sresize(nrep, xi+remaining);
  }

  ncopy0(&(s[si]), &(x[xi]), remaining);
  nrep->len = xi + remaining;

  if (nrep->len <= rep->sz)   // fit back in if possible
  {
    rep->len = nrep->len;
    ncopy0(nrep->s, rep->s, rep->len);
    delete [] (char*)nrep;
  }
  else
  {
    delete [] (char*)rep;
    rep = nrep;
  }
  return nmatches;
}

int AvString::_gsub(const AvRegex& pat, const char* r, int rl)
{
  int nmatches = 0;
  int sl = length();
  if (sl <= 0)
    return nmatches;

  if (rl < 0) rl = slen(r);

  const char* s = chars();

  AvStrRep* nrep = 0;
  int nsz = 0;

  char* x = 0;

  int si = 0;
  int xi = 0;
  int remaining = sl;
  int  pos, pl = 0;				  // how long is a regular expression?

  while (remaining > 0)
  {
    pos = pat.search(s, sl, pl, si); // unlike string search, the pos returned here is absolute
    if (pos < 0 || pl <= 0)
      break;
    else
    {
      ++nmatches;
      int mustfit = xi + remaining + rl - pl;
      if (mustfit >= nsz)
      {
        if (nrep != 0) nrep->len = xi;
        nrep = Sresize(nrep, mustfit);
        x = nrep->s;
        nsz = nrep->sz;
      }
      pos -= si;
      ncopy(&(s[si]), &(x[xi]), pos);
      ncopy(r, &(x[xi + pos]), rl);
      si += pos + pl;
      remaining -= pos + pl;
      xi += pos + rl;
    }
  }

  if (nrep == 0)
  {
    if (nmatches == 0)
      return nmatches;
    else
      nrep = Sresize(nrep, xi+remaining);
  }

  ncopy0(&(s[si]), &(x[xi]), remaining);
  nrep->len = xi + remaining;

  if (nrep->len <= rep->sz)   // fit back in if possible
  {
    rep->len = nrep->len;
    ncopy0(nrep->s, rep->s, rep->len);
    delete [] (char *)nrep;
  }
  else
  {
    delete [] (char*)rep;
    rep = nrep;
  }
  return nmatches;
}


/*
 * deletion
 */

void AvString::del(int pos, int len)
{
  if (pos < 0 || len <= 0 || (unsigned)(pos + len) > length()) return;
  int nlen = length() - len;
  int first = pos + len;
  ncopy0(&(rep->s[first]), &(rep->s[pos]), length() - first);
  rep->len = nlen;
}

void AvString::del(const AvRegex& r, int startpos)
{
  int mlen;
  int first = r.search(chars(), length(), mlen, startpos);
  del(first, mlen);
}

void AvString::del(const char* t, int startpos)
{
  int tlen = slen(t);
  int p = search(startpos, length(), t, tlen);
  del(p, tlen);
}

void AvString::del(const AvString& y, int startpos)
{
  del(search(startpos, length(), y.chars(), y.length()), y.length());
}

void AvString::del(const SubAvString& y, int startpos)
{
  del(search(startpos, length(), y.chars(), y.length()), y.length());
}

void AvString::del(char c, int startpos)
{
  del(search(startpos, length(), c), 1);
}

/*
 * substring extraction
 */


SubAvString AvString::at(int first, int len)
{
  return _substr(first, len);
}

SubAvString AvString::operator() (int first, int len)
{
  return _substr(first, len);
}

SubAvString AvString::before(int pos)
{
  return _substr(0, pos);
}

SubAvString AvString::through(int pos)
{
  return _substr(0, pos+1);
}

SubAvString AvString::after(int pos)
{
  return _substr(pos + 1, length() - (pos + 1));
}

SubAvString AvString::from(int pos)
{
  return _substr(pos, length() - pos);
}

SubAvString AvString::at(const AvString& y, int startpos)
{
  int first = search(startpos, length(), y.chars(), y.length());
  return _substr(first,  y.length());
}

SubAvString AvString::at(const SubAvString& y, int startpos)
{
  int first = search(startpos, length(), y.chars(), y.length());
  return _substr(first, y.length());
}

SubAvString AvString::at(const AvRegex& r, int startpos)
{
  int mlen;
  int first = r.search(chars(), length(), mlen, startpos);
  return _substr(first, mlen);
}

SubAvString AvString::at(const char* t, int startpos)
{
  int tlen = slen(t);
  int first = search(startpos, length(), t, tlen);
  return _substr(first, tlen);
}

SubAvString AvString::at(char c, int startpos)
{
  int first = search(startpos, length(), c);
  return _substr(first, 1);
}

SubAvString AvString::before(const AvString& y, int startpos)
{
  int last = search(startpos, length(), y.chars(), y.length());
  return _substr(0, last);
}

SubAvString AvString::before(const SubAvString& y, int startpos)
{
  int last = search(startpos, length(), y.chars(), y.length());
  return _substr(0, last);
}

SubAvString AvString::before(const AvRegex& r, int startpos)
{
  int mlen;
  int first = r.search(chars(), length(), mlen, startpos);
  return _substr(0, first);
}

SubAvString AvString::before(char c, int startpos)
{
  int last = search(startpos, length(), c);
  return _substr(0, last);
}

SubAvString AvString::before(const char* t, int startpos)
{
  int tlen = slen(t);
  int last = search(startpos, length(), t, tlen);
  return _substr(0, last);
}

SubAvString AvString::through(const AvString& y, int startpos)
{
  int last = search(startpos, length(), y.chars(), y.length());
  if (last >= 0) last += y.length();
  return _substr(0, last);
}

SubAvString AvString::through(const SubAvString& y, int startpos)
{
  int last = search(startpos, length(), y.chars(), y.length());
  if (last >= 0) last += y.length();
  return _substr(0, last);
}

SubAvString AvString::through(const AvRegex& r, int startpos)
{
  int mlen;
  int first = r.search(chars(), length(), mlen, startpos);
  if (first >= 0) first += mlen;
  return _substr(0, first);
}

SubAvString AvString::through(char c, int startpos)
{
  int last = search(startpos, length(), c);
  if (last >= 0) last += 1;
  return _substr(0, last);
}

SubAvString AvString::through(const char* t, int startpos)
{
  int tlen = slen(t);
  int last = search(startpos, length(), t, tlen);
  if (last >= 0) last += tlen;
  return _substr(0, last);
}

SubAvString AvString::after(const AvString& y, int startpos)
{
  int first = search(startpos, length(), y.chars(), y.length());
  if (first >= 0) first += y.length();
  return _substr(first, length() - first);
}

SubAvString AvString::after(const SubAvString& y, int startpos)
{
  int first = search(startpos, length(), y.chars(), y.length());
  if (first >= 0) first += y.length();
  return _substr(first, length() - first);
}

SubAvString AvString::after(char c, int startpos)
{
  int first = search(startpos, length(), c);
  if (first >= 0) first += 1;
  return _substr(first, length() - first);
}

SubAvString AvString::after(const AvRegex& r, int startpos)
{
  int mlen;
  int first = r.search(chars(), length(), mlen, startpos);
  if (first >= 0) first += mlen;
  return _substr(first, length() - first);
}

SubAvString AvString::after(const char* t, int startpos)
{
  int tlen = slen(t);
  int first = search(startpos, length(), t, tlen);
  if (first >= 0) first += tlen;
  return _substr(first, length() - first);
}

SubAvString AvString::from(const AvString& y, int startpos)
{
  int first = search(startpos, length(), y.chars(), y.length());
  return _substr(first, length() - first);
}

SubAvString AvString::from(const SubAvString& y, int startpos)
{
  int first = search(startpos, length(), y.chars(), y.length());
  return _substr(first, length() - first);
}

SubAvString AvString::from(const AvRegex& r, int startpos)
{
  int mlen;
  int first = r.search(chars(), length(), mlen, startpos);
  return _substr(first, length() - first);
}

SubAvString AvString::from(char c, int startpos)
{
  int first = search(startpos, length(), c);
  return _substr(first, length() - first);
}

SubAvString AvString::from(const char* t, int startpos)
{
  int tlen = slen(t);
  int first = search(startpos, length(), t, tlen);
  return _substr(first, length() - first);
}



/*
 * split/join
 */


int split(const AvString& src, AvString results[], int n, const AvString& sep)
{
  AvString x = src;
  const char* s = x.chars();
  int sl = x.length();
  int i = 0;
  int pos = 0;
  while (i < n && pos < sl)
  {
    int p = x.search(pos, sl, sep.chars(), sep.length());
    if (p < 0)
      p = sl;
    results[i].rep = AvSalloc(results[i].rep, &(s[pos]), p - pos, p - pos);
    i++;
    pos = p + sep.length();
  }
  return i;
}

int split(const AvString& src, AvString results[], int n, const AvRegex& r)
{
  AvString x = src;
  const char* s = x.chars();
  int sl = x.length();
  int i = 0;
  int pos = 0;
  int p, matchlen;
  while (i < n && pos < sl)
  {
    p = r.search(s, sl, matchlen, pos);
    if (p < 0)
      p = sl;
    results[i].rep = AvSalloc(results[i].rep, &(s[pos]), p - pos, p - pos);
    i++;
    pos = p + matchlen;
  }
  return i;
}


#if defined(__GNUG__) && !defined(NO_NRV)

AvString join(AvString src[], int n, const AvString& separator) return x;
{
  AvString sep = separator;
  int xlen = 0;
  int i;
  for (i = 0; i < n; ++i)
    xlen += src[i].length();
  xlen += (n - 1) * sep.length();

  x.alloc(xlen);

  int j = 0;
  
  for (i = 0; i < n - 1; ++i)
  {
    ncopy(src[i].chars(), &(x.rep->s[j]), src[i].length());
    j += src[i].length();
    ncopy(sep.chars(), &(x.rep->s[j]), sep.length());
    j += sep.length();
  }
  ncopy0(src[i].chars(), &(x.rep->s[j]), src[i].length());
}

#else

AvString join(AvString src[], int n, const AvString& separator)
{
  AvString x;
  AvString sep = separator;
  int xlen = 0;
  for (int i = 0; i < n; ++i)
    xlen += src[i].length();
  xlen += (n - 1) * sep.length();

  x.alloc(xlen);

  int j = 0;
  
  {
  int i;
  for (i = 0; i < n - 1; ++i)
  {
    ncopy(src[i].chars(), &(x.rep->s[j]), src[i].length());
    j += src[i].length();
    ncopy(sep.chars(), &(x.rep->s[j]), sep.length());
    j += sep.length();
  }
  ncopy0(src[i].chars(), &(x.rep->s[j]), src[i].length());
  }
  return x;
}

#endif
  
/*
 misc
*/

    
AvStrRep* Sreverse(AvStrRep* src, AvStrRep* dest)
{
  int n = src->len;
  if (src != dest)
    dest = AvSalloc(dest, src->s, n, n);
  if (n > 0)
  {
    char* a = dest->s;
    char* b = &(a[n - 1]);
    while (a < b)
    {
      char t = *a;
      *a++ = *b;
      *b-- = t;
    }
  }
  return dest;
}


AvStrRep* Supcase(AvStrRep* src, AvStrRep* dest)
{
  int n = src->len;
  if (src != dest) dest = AvSalloc(dest, src->s, n, n);
  char* p = dest->s;
  char* e = &(p[n]);
  for (; p < e; ++p) if (islower(*p)) *p = toupper(*p);
  return dest;
}

AvStrRep* Sdowncase(AvStrRep* src, AvStrRep* dest)
{
  int n = src->len;
  if (src != dest) dest = AvSalloc(dest, src->s, n, n);
  char* p = dest->s;
  char* e = &(p[n]);
  for (; p < e; ++p) if (isupper(*p)) *p = tolower(*p);
  return dest;
}

AvStrRep* Scapitalize(AvStrRep* src, AvStrRep* dest)
{
  int n = src->len;
  if (src != dest) dest = AvSalloc(dest, src->s, n, n);

  char* p = dest->s;
  char* e = &(p[n]);
  for (; p < e; ++p)
  {
    int at_word;
    if ((at_word = islower(*p)))
      *p = toupper(*p);
    else 
      at_word = isupper(*p) || isdigit(*p);

    if (at_word)
    {
      while (++p < e)
      {
        if (isupper(*p))
          *p = tolower(*p);
        else if (!islower(*p) && !isdigit(*p))
          break;
      }
    }
  }
  return dest;
}

#if defined(__GNUG__) && !defined(NO_NRV)

#if 0
AvString replicate(char c, int n) return w;
{
  w.rep = Sresize(w.rep, n);
  char* p = w.rep->s;
  while (n-- > 0) *p++ = c;
  *p = 0;
}
#endif

AvString replicate(const AvString& y, int n) return w
{
  int len = y.length();
  w.rep = Sresize(w.rep, n * len);
  char* p = w.rep->s;
  while (n-- > 0)
  {
    ncopy(y.chars(), p, len);
    p += len;
  }
  *p = 0;
}

AvString common_prefix(const AvString& x, const AvString& y, int startpos) return r;
{
  const char* xchars = x.chars();
  const char* ychars = y.chars();
  const char* xs = &(xchars[startpos]);
  const char* ss = xs;
  const char* topx = &(xchars[x.length()]);
  const char* ys = &(ychars[startpos]);
  const char* topy = &(ychars[y.length()]);
  int l;
  for (l = 0; xs < topx && ys < topy && *xs++ == *ys++; ++l);
  r.rep = AvSalloc(r.rep, ss, l, l);
}

AvString common_suffix(const AvString& x, const AvString& y, int startpos) return r;
{
  const char* xchars = x.chars();
  const char* ychars = y.chars();
  const char* xs = &(xchars[x.length() + startpos]);
  const char* botx = xchars;
  const char* ys = &(ychars[y.length() + startpos]);
  const char* boty = ychars;
  int l;
  for (l = 0; xs >= botx && ys >= boty && *xs == *ys ; --xs, --ys, ++l);
  r.rep = AvSalloc(r.rep, ++xs, l, l);
}

#else

#if 0
AvString replicate(char c, int n)
{
  AvString w;
  w.rep = Sresize(w.rep, n);
  char* p = w.rep->s;
  while (n-- > 0) *p++ = c;
  *p = 0;
  return w;
}
#endif

AvString replicate(const AvString& y, int n)
{
  AvString w;
  int len = y.length();
  w.rep = Sresize(w.rep, n * len);
  char* p = w.rep->s;
  while (n-- > 0)
  {
    ncopy(y.chars(), p, len);
    p += len;
  }
  *p = 0;
  return w;
}

AvString common_prefix(const AvString& x, const AvString& y, int startpos)
{
  AvString r;
  const char* xchars = x.chars();
  const char* ychars = y.chars();
  const char* xs = &(xchars[startpos]);
  const char* ss = xs;
  const char* topx = &(xchars[x.length()]);
  const char* ys = &(ychars[startpos]);
  const char* topy = &(ychars[y.length()]);
  int l;  // Make the declaration outside of the for loop to avoid the
          // for loop declaration
  for (l = 0; xs < topx && ys < topy && *xs++ == *ys++; ++l);
  r.rep = AvSalloc(r.rep, ss, l, l);
  return r;
}

AvString common_suffix(const AvString& x, const AvString& y, int startpos) 
{
  AvString r;
  const char* xchars = x.chars();
  const char* ychars = y.chars();
  const char* xs = &(xchars[x.length() + startpos]);
  const char* botx = xchars;
  const char* ys = &(ychars[y.length() + startpos]);
  const char* boty = ychars;
  int l;  // Make the declaration outside of the for loop to avoid the
          // for loop declaration
  for (l = 0; xs >= botx && ys >= boty && *xs == *ys ; --xs, --ys, ++l);
  r.rep = AvSalloc(r.rep, ++xs, l, l);
  return r;
}

#endif

// IO
#if 0
std::istream& operator>>(std::istream& s, AvString& x)
{
#if defined(_OLD_STREAMS) || defined(AIPS_STDLIB)
  if (!s.good())
  {
    return s;
  }
  s >> ws;
  if (!s.good())
  {
    return s;
  }
#else
  if (!s.ipfx(0) || (!(s.flags() & ios::skipws) && !ws(s)))
  {
    s.clear(ios::failbit|s.rdstate()); // Redundant if using GNU iostreams.
    return s;
  }
#endif
  int ch;
  int i = 0;
  x.rep = Sresize(x.rep, 20);
  register streambuf *sb = s.rdbuf();
  while ((ch = sb->sbumpc()) != EOF)
  {
    if (isspace(ch))
      break;
    if (i >= (int)(x.rep->sz) - 1)
      x.rep = Sresize(x.rep, i+1);
    x.rep->s[i++] = ch;
  }
  x.rep->s[i] = 0;
  x.rep->len = i;
  if (i == 0) s.clear(ios::failbit|s.rdstate());
  if (ch == EOF) s.clear(ios::eofbit|s.rdstate());
  return s;
}
#endif

#if 0
int readline(std::istream& s, AvString& x, char terminator, int discard)
{
#if defined(_OLD_STREAMS) || defined(AIPS_STDLIB)
  if (!s.good())
#else
  if (!s.ipfx(0))
#endif
  {
    return 0;
  }
  char ch;
  int i = 0;
  x.rep = Sresize(x.rep, 80);
  while (s.get(ch))
  {
    if (ch != terminator || !discard)
    {
      if (i >= (int)(x.rep->sz) - 1)
        x.rep = Sresize(x.rep, i+1);
      x.rep->s[i++] = ch;
    }
    if (ch == terminator)
      break;
  }
  x.rep->s[i] = 0;
  x.rep->len = i;
  if ((int)ch == EOF) s.clear(ios::eofbit|s.rdstate());
  return i;
}
#endif

std::ostream& operator<<(std::ostream& s, const SubAvString& x)
{ 
  const char* a = x.chars();
  const char* lasta = &(a[x.length()]);
  while (a < lasta)
    s.put(*a++);
  return(s);
}

// from John.Willis@FAS.RI.CMU.EDU

int AvString::freq(const SubAvString& y) const
{
  int found = 0;
  for (unsigned int i = 0; i < length(); i++) 
    if (match(i,length(),0,y.chars(), y.length())>= 0) found++;
  return(found);
}

int AvString::freq(const AvString& y) const
{
  int found = 0;
  for (unsigned int i = 0; i < length(); i++) 
    if (match(i,length(),0,y.chars(),y.length()) >= 0) found++;
  return(found);
}

int AvString::freq(const char* t) const
{
  int found = 0;
  for (unsigned int i = 0; i < length(); i++) 
    if (match(i,length(),0,t) >= 0) found++;
  return(found);
}

int AvString::freq(char c) const
{
  int found = 0;
  for (unsigned int i = 0; i < length(); i++) 
    if (match(i,length(),0,&c,1) >= 0) found++;
  return(found);
}


int AvString::OK() const
{
  if (rep == 0             // don't have a rep
    || rep->len > rep->sz     // string oustide bounds
    || rep->s[rep->len] != 0)   // not null-terminated
    {
      cerr << "AvString::OK: invariant failure" << endl;
      cerr.flush();
    }
  return 1;
}

int SubAvString::OK() const
{
  int v = S.empty();           // don't have a string
  v |= !(S.OK());                 // that is legal
  v |= (int)pos + (int)len > (int)(S.rep->len);// pos and len within bounds
  if (v)
    {
      cerr << "AvString invariant failure" << endl;
      cerr.flush();
    }
  return 1;
}

#if 0
 unsigned long AvString::hash() const {
    const char* p;
    unsigned long v = 0;
    char *s = rep->s;
    const char* q = &s[rep->len];	// this is actually where the NULL is

    for (p = s; p < q; p++)
        v = (v << 1) ^ (*p);
    unsigned long t = v >> 10;
    t ^= (t >> 10);

    return v ^ t;
}
#endif

// needed to get around the dual dependance of AvStrings and exceptions


void Av_stringThrowError(const char *msg)
{
  cerr << msg << endl;
}

// constructive concatenation

AvString operator + (const AvString& x, const AvString& y) {
  AvString r(x);  r += y;  return r;
}

AvString operator + (const AvString& x, const SubAvString& y)  {
  AvString r(x); r += y; return r;
}

AvString operator + (const AvString& x, const char* y)  {
  AvString r(x); r += y; return r;
}

AvString operator + (const AvString& x, char y)  {
  AvString r(x); r += y; return r;
}

AvString operator + (const SubAvString& x, const AvString& y)  {
  AvString r(x); r += y; return r;
}

AvString operator + (const SubAvString& x, const SubAvString& y)  {
  AvString r(x); r += y; return r;
}

AvString operator + (const SubAvString& x, const char* y) {
  AvString r(x); r += y; return r;
}

AvString operator + (const SubAvString& x, char y) {
  AvString r(x); r += y; return r;
}

AvString operator + (const char* x, const AvString& y) {
  AvString r(x); r += y; return r;
}

AvString operator + (const char* x, const SubAvString& y) {
  AvString r(x); r += y; return r;
}

AvString reverse(const AvString& x) 
{
  AvString r; r.rep = Sreverse(x.rep, r.rep); return r;
}

AvString upcase(const AvString& x) 
{
  AvString r; r.rep = Supcase(x.rep, r.rep); return r;
}

AvString downcase(const AvString& x) 
{
  AvString r; r.rep = Sdowncase(x.rep, r.rep); return r;
}

AvString capitalize(const AvString& x) 
{
  AvString r; r.rep = Scapitalize(x.rep, r.rep); return r;
}

// prepend

void AvString::prepend(const AvString& y)
{
  rep = Sprepend(rep, y.chars(), y.length());
}

void AvString::prepend(const char* y)
{
  rep = Sprepend(rep, y, -1); 
}

void AvString::prepend(char y)
{
  rep = Sprepend(rep, &y, 1); 
}

void AvString::prepend(const SubAvString& y)
{
  rep = Sprepend(rep, y.chars(), y.length());
}

// misc transformations


void AvString::reverse()
{
  rep = Sreverse(rep, rep);
}


void AvString::upcase()
{
  rep = Supcase(rep, rep);
}


void AvString::downcase()
{
  rep = Sdowncase(rep, rep);
}


void AvString::capitalize()
{
  rep = Scapitalize(rep, rep);
}
