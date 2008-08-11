//# Copyright (C) 1995-97 Board of Trustees of the University of Illinois
//#
//# This software, both binary and source, is copyrighted by The
//# Board of Trustees of the University of Illinois.  Ownership
//# remains with the University.  You should have received a copy 
//# of a licensing agreement with this software.  See the file 
//# "AIPSVIEW_COPYRIGHT", or contact the University at this address:
//#
//# 	The NCSA AipsView Visualization System
//# 	National Center for Supercomputing Applications
//# 	University of Illinois
//# 	405 North Mathews Ave.
//# 	Urbana, IL 61801
//# 	aipsview@ncsa.uiuc.edu
//# --------------------------------------------------
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvList.cc,v 19.0 2003/07/16 05:47:35 aips2adm Exp $
//
// $Log: AvList.cc,v $
// Revision 19.0  2003/07/16 05:47:35  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:14  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:43  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:50  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:35  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:40  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:29  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:31  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:59  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:01  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:29:30  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:18:26  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:35:32  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.4  1996/06/18  18:46:18  pixton
//  Copyright Update
//
//  Revision 1.3  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/07/18  20:54:39  hr
//  Needed to move an 'AvListItem<T> * n' out of a for loop.
//
//  Revision 1.1  1995/03/30  22:52:53  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#include <stdio.h>
#include "AvList.h"

template <class T> AvList<T>::~AvList()
{
  if (trace_)
    {
      fprintf(stderr, "~AvList<T>::AvList<T>() [Destructor]\n");
    }
  clear();
}

template <class T> void AvList<T>::add(const AvList<T>& l)
{
  for (AvListItem<T> *n = l.head_; n; n = n->next())
    add(n->value());
}

template <class T> void AvList<T>::setAdd(const AvList<T>& l)
{
  for (AvListItem<T> *n = l.head_; n; n = n->next())
    setAdd(n->value());
}

template <class T> void AvList<T>::mapcar(void (*func)(T)) const
{
  if (trace_) fprintf(stderr, "AvList<T>::mapcar()\n");
  for (AvListItem<T> *n = head_; n; n = n->next())
    func(n->value());
}

template <class T> T * AvList<T>::walk(int (*func)(T, void *), void * data) const
{
  T * retval = NULL;
  if (trace_) fprintf(stderr, "AvList<T>::walk()\n");
  for (AvListItem<T> * n = head_; n; n = n->next())
    if (func(n->value(), data)) { retval = n->valuePtr(); break; }
  return retval;
}

template <class T> T * AvList<T>::walk(int (*func)(T)) const
{
  T * retval = NULL;
  if (trace_) fprintf(stderr, "AvList<T>::walk()\n");
  for (AvListItem<T> * n = head_; n; n = n->next())
    if (func(n->value())) { retval = n->valuePtr(); break; }
  return retval;
}

template <class T> int AvList<T>::filter(int (*func)(T, void *), void * data)
{
  int count = 0;
  if (trace_) fprintf(stderr, "AvList<T>::filter()\n");
  for (AvListItem<T> * n = head_; n;)
    {
      AvListItem<T> * next = n->next();
      if (func(n->value(), data)) { remove(n); count++; }
      n = next;
    }
  return count;
}

template <class T> int AvList<T>::filter(int (*func)(T))
{
  int count = 0;
  if (trace_) fprintf(stderr, "AvList<T>::filter()\n");
  for (AvListItem<T> * n = head_; n;)
    {
      AvListItem<T> * next = n->next();
      if (func(n->value())) { remove(n); count++; }
      n = next;
    }
  return count;
}
  
template <class T>
void AvList<T>::qsort(int (*compar)(const T *, const T *))
{
  // OPT [ ]  operate directly on the list, swapping values as necessary, instead
  // of using a temporary array.  qsort() is a built-in C func.

  int len = length();

  T * array = new T[len];

  int ndx = 0;
  AvListItem<T> *n;
  for (n = head_; n; n = n->next(), ndx++)
    array[ndx] = n->value();

  ::qsort (array, (size_t) len, sizeof(T), (int (*)(const void *, const void *)) compar);

  for (n = head_, ndx = 0; n; n = n->next(), ndx++)
    n->value(array[ndx]);

  delete [] array;
}

template <class T> void AvList<T>::copy(const AvList<T>& l)
{
  clear();
  head_ = 0; tail_ = 0; trace_ = 0;
  for (AvListItem<T> *n = l.head_; n; n = n->next())
    append(n->value());
}  

template <class T> AvList<T>& AvList<T>::operator = (const AvList<T>& l)
{
  if (trace_) fprintf(stderr, "AvList<T>::operator =() [Assignment Op]\n");
  copy(l);
  return *this;
}

template <class T> AvList<T>::AvList(const AvList<T>& l)
: head_(0), tail_(0), trace_(0)
{
  if (trace_) fprintf(stderr, "AvList<T>::AvList(const AvList<T>) [Copy Ctor]");
  copy(l);
}

    
// -1 on failure
template <class T> int AvListIter<T>::seek(const T & x)
{
  int retval = -1;
  while(!isdone())
    {
      if (x == peek()) { retval = 0; break; }
      step();
    }
  return retval;
}

template <class T> void AvListIndexIter<T>::gotoNth(int ndx)
{
  rewind();
  int i = ndx;
  while (i--) step();
}

template <class T> AvListItem<T> * AvList<T>::findNodePtr(const T & x) const
{
  if (trace_) fprintf(stderr, "AvList<T>::findNodePtr()\n");
  for(AvListItem<T> *n = head_; n; n = n->next())
    if (x == n->value()) return n;
  return 0;
}

template <class T> int AvList<T>::index(const T & x) const
{
  if (trace_) fprintf(stderr, "AvList<T>::index()\n");
  int count = 0;
  for(AvListItem<T> *n = head_; n; n = n->next(), count++)
    if (x == n->value()) break;
  return count;
}

template <class T> void AvList<T>::removeAll(const T & x)
{
  if (trace_) fprintf(stderr, "AvList<T>::removeAll()\n");
  for(AvListItem<T> *n = head_; n; n = n->next())
    if (x == n->value()) remove(n);
}
    
template <class T> void AvList<T>::removeTagged()
{
  if (trace_) fprintf(stderr, "AvList<T>::removeTagged()\n");
  for(AvListItem<T> *n = head_; n; n = n->next())
    if (n->tagged()) remove(n);
}

template <class T> int AvList<T>::allTagged() const
{
  if (trace_) fprintf(stderr, "AvList<T>::allTagged()\n");
  int retval = 1;
  for (AvListItem<T> *n = head_; n; n = n->next())
    if (!n->tagged()) retval = 0;
  return retval;
}

template <class T> void AvList<T>::tagAll(int t)
{
  for (AvListItem<T> *n = head_; n; n = n->next())
    n->tag(t);
}

template <class T> long AvList<T>::length() const
{
  if (trace_) fprintf(stderr, "AvList<T>::length()\n");
  long len = 0;
  for(AvListItem<T> *n = head_; n; n = n->next())
    len++;
  return len;
}

template <class T> void AvList<T>::toHead(const T & x)
{
  AvListItem<T> *n = findNodePtr(x);
  if (n) { remove(n); prepend(x); }
}

template <class T> void AvList<T>::toEnd(const T & x)
{
  AvListItem<T> *n = findNodePtr(x);
  if (n) { remove(n); append(x); }
}

//--- END -----------------------------------------------------------------------------

// Turn on to test the list module separately.

#if 0
typedef AvList<int> IL;
typedef AvListIter<int> IT;
typedef AvListIndexIter<int> NDXIT;

void dump(IL * l)
{
  NDXIT iter = NDXIT(l);
  
  printf("%d Entries: ", l->length());
  while (!iter.isdone())
    {
      printf("#%d:", iter.index());
      printf("%d ", iter.get());
    }
  printf("\n");
}

void print(int t) { printf(" %d", t); }
int sortInts(const int * i1, const int * i2) { return *i1 - *i2; }

int matchFuncV(int x, void * v) { return (x == *((int *) v)); }
int matchFunc(int x) { return (x == 5); }

int filterFunc(int x) { return (x % 2); }
int filterFuncV(int x, void * v) { return (x % *((int *) v)) == 0; }

void main()
{
  AvList<int> s = AvList<int>();
  for (int i = 0; i < 7; i++) s.append(i);
  for (i = 10; i < 17; i++) s.prepend(i);
  printf("first time:");
  s.mapcar(print);
  printf("\n2nd time:");
  s.mapcar(print);
  printf("\nsorted  :");
  s.qsort(sortInts);
  s.mapcar(print);

  int x = 13; int y = 9;
  printf("\n matching value %d?: %d\n", x, *s.walk(matchFuncV, &x));

  int * pv = s.walk(matchFuncV, &x);
  if (pv) 
    printf("Match value found for v=13: %d\n", *pv);
  else
    printf("Match value not found for v=13\n");

  pv = s.walk(matchFuncV, &y);
  if (pv) 
    printf("Match found for v=9: %d\n", *pv);
  else
    printf("Match not found for 9\n");

  pv = s.walk(matchFunc);
  if (pv) 
    printf("Match found for 5: %d\n", *pv);
  else
    printf("Match not found for 5\n");

  int fc = s.filter(filterFunc);
  printf("Odd's filtered (%d)", fc);
  s.mapcar(print);

  int vf = 3;
  fc = s.filter(filterFuncV, &vf);

  printf("\nDivisible-by-3's filtered (%d)", fc);
  s.mapcar(print);

  printf("\n");
}
#endif







