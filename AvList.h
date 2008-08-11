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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvList.h,v 19.0 2003/07/16 05:47:12 aips2adm Exp $
//
// $Log: AvList.h,v $
// Revision 19.0  2003/07/16 05:47:12  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:54  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:24  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:30  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:13  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:15  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:03  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:43  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:52  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:43  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:27:29  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:43  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:40:17  droberts
// Final update from monet archive.
//
// Revision 1.3  1996/08/19 19:31:02  pixton
// Really fixed g++ warnings.
//
// Revision 1.2  1996/08/19  17:20:24  pixton
// removed g++ -Wall warnings
//
// Revision 1.1  1996/07/11  21:31:21  pixton
// Automated Checkin
//
//  Revision 1.5  1996/06/18  18:43:06  pixton
//  Copyright Update
//
//  Revision 1.4  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/09/12  16:42:51  pixton
//  Fixed a dormant bug in AvList in the functions removeHead and
//  removeTail - application to 1-item list left head or tail
//  pointer dangling.
//
//  Also added first-pass at cxx2html-compatible documentation.
//
//  Revision 1.2  1995/07/17  20:17:07  hr
//  Repositioned AvListItem::val_ to make g++ happy.
//
//  Revision 1.1  1995/03/30  22:54:25  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#ifndef __AvList_H__
#define __AvList_H__

/*
   <summary> Implementation of Templated Doubly-Linked list </summary>

   Assign/Copy make copy of the list, and destructor first clears the list.
   (The no leak, no surprises approach)

   The T used to define the list is what is stored in the list as well as
   what is returned (in most cases).

   Adds/deletes done in constant time.
   Insert and other functions requiring search for item done in O(N/2) time.
   List length done in O(N) time.

   List items can be tagged, and operations can be carried out on tagged items.

   Quick-Sort available as 2nd-order function qsort(), which sorts according
   to a user-specified sort function.
   mapcar() applies a function to each item.
   walk() searches the list according to a matching function
   filter() removes items from the list according to function

   The defined interface consists of the public functions of AvList and
   the public members of the iterators.  AvListItem functions are not
   defined as part of the external interface and are subject to change.

   <note role=warning>

   Iterators are not sync'd to list removes.  The code should insure that all
   active iterators point to something valid.  Registration function needs to
   be added for this, and the private remove(AvListItem *) needs to step each
   iterator that is pointing to the node being removed.

   */

#include <stdlib.h>
#include <stdio.h>

template <class T> class AvListIter;
template <class T> class AvListIndexIter;

template <class T> class AvListItem
{
public:

  // These functions operate on the list nodes and should
  // never be accessed directly by application code.
  //

  // Create a list item
  AvListItem(const T & x) : val_(x), next_(0), prev_(0), tag_(0) {}

  // Return the current value
  T value() const { return val_; }

  // Set the value
  void value(T v) { val_ = v; }

  // Get a pointer to the value
  T * valuePtr() const { return (T *) &val_; }

  // Get the next node
  AvListItem<T> *next() { return next_; }

  // Get the previous node
  AvListItem<T> *prev() { return prev_; }

  // Set the next node
  void next(AvListItem<T> *n) { next_ = n; }

  // Set the prev node
  void prev(AvListItem<T> *p) { prev_ = p; }

  // Attach node p to this on the previous end
  void prelink(AvListItem<T> *p) { next_ = p; p->prev_ = this; }

  // Attach node p to this on the next end
  void postlink(AvListItem<T> *p) { prev_ = p; p->next_ = this; }

  // Detach all links for a node
  void separate()
  { if (next_) next_->prev_ = prev_;
    if (prev_) prev_->next_ = next_;
    next_ = 0; prev_ = 0; }

  // Tag a node with a value
  void tag(int n = 1) { tag_ = n; }

  // return the tag value
  int tagged() const { return tag_; }
  
private:
  T         val_;
  AvListItem<T> *next_;
  AvListItem<T> *prev_;
  int       tag_;
};

template <class T> class AvList
{
  friend class AvListIter<T>;
  friend class AvListIndexIter<T>;

 // Friend functions: 
    //   AvListItem<T> * head(), 
    //   AvListItem<T> * tail()

public:
  AvList() : head_(0), tail_(0), trace_(0) {}

  ~AvList();
  AvList(const AvList<T>& l);
  AvList<T>& operator = (const AvList<T>& l);
  
private:
  // used to implement above
  void copy(const AvList<T>& l);

public:

  // Apply func to each element in the list
  void mapcar(void (*func)(T)) const;

  // This process walks the list from the beginning until user-specified function returns 
  // non-zero.  At that point walk() returns a pointer to the value that it stopped on.
  //
  // In the 2nd version, the void * pointer is passed on to the user function.
  // This can be useful for searching for items with a specific property.
  //
  // walk() returns NULL if there the user function never returned a non-zero value.
  //
  // The user function should return non-zero if walk should stop on the item presented to it.
  // <group>
  T * walk(int (*func)(T v)) const;
  T * walk(int (*func)(T v, void * data), void * data) const;
  // </group>
  // Remove all items that pass (return non-zero) the test function func.  The data pointer
  // in the call to filter will be passed along to the user function.
  // <group>
  int filter(int (*func)(T v));
  int filter(int (*func)(T v, void * data), void * data);
  // </group>

  // Quick-Sort the list using the user-specified compare function.  The compare
  // function should return positive if a should appear before b, zero if they are
  // equal, and negative if b should appear before a. 
  //
  void qsort(int (*compar)(const T * a, const T * b));

  // add to front of list
  void prepend(const T & x)
  {
    if (trace_)  fprintf(stderr, "AvList<T>::prepend()\n");
    AvListItem<T> *n = new AvListItem<T>(x);
    if (head_) n->prelink(head_);
    else tail_ = n;
    head_ = n;
  }

  // add to end of list
  void append(const T & x)
  {
    if (trace_) fprintf(stderr, "AvList<T>::append()\n");
    AvListItem<T> *n = new AvListItem<T>(x);
    if (tail_) n->postlink(tail_);
    else head_ = n;
    tail_ = n;
  }

  // Return first, second, third values.  User responsible for
  // ensuring that the list has at least that many values
  // <group>
  T first() const { return head_->value(); }
  T second() const { return head_->next()->value(); }
  T third() const { return head_->next()->next()->value(); }
  // </group>

  // Add the value or list of values to the list.
  // <group>
  void add(const AvList<T>& l); 
  void add(const T & x) { append(x); }
  // </group>

  // Add the value or list of values to the list without
  // unless they are already present.
  // <group>
  void setAdd(const T & x) { if (!member(x)) append(x); }
  void setAdd(const AvList<T>& l);
  // </group>

  // Remove the first item in the list and return its value
  T removeHead()
  {
    if (trace_) fprintf(stderr, "AvList<T>::removeHead()\n");
    return remove(head_);
  }

  // Remove the last item in the list and return the value.
  T removeTail()
  {
    fprintf(stderr, "AvList<T>::removeTail()\n");
    return remove(tail_);
  }

  // Remove the value from the list if it exists and return
  // it.
  T remove(const T & x)
  {
    if (trace_) fprintf(stderr, "AvList<T>::remove()\n");
    T retval = (T)0;
    AvListItem<T> *n = findNodePtr(x);
    if (n) 
      {
	retval = remove(n);
      }
    return retval;
  }

  // Insert the value before the mark
  int insert(const T & x, const T & mark)
  {
    if (trace_) fprintf(stderr, "AvList<T>::insert()\n");
    AvListItem<T> *m = findNodePtr(mark);
    if (m)
      {
	AvListItem<T> *n = new AvListItem<T>(x);
	if (head_ == m) head_ = n;
	n->prelink(m);
	return 0;
      }
    else
      {
	return -1;
      }
  }

  // tag the first item in the list whose value is x
  void tag(const T & x, int t = 1)
  {
    if (trace_) fprintf(stderr, "AvList<T>::tag()\n");
    AvListItem<T> *n = findNodePtr(x);
    if (n) n->tag(t);
  }
  
  // Return true if the item is tagged
  int tagged(const T & x) const 
  {
    if (trace_) fprintf(stderr, "AvList<T>::tagged()\n");
    AvListItem<T> *n = findNodePtr(x);
    return n ? n->tagged() : 0;
  }

  // Return the index for the first x in the list (Base zero index)
  int index(const T & x) const;
  
  // Return true if there is at least one x in the list
  int member(const T & x) const
  { if (trace_) fprintf(stderr, "AvList<T>::member\n"); return (findNodePtr(x) != 0); }

  // Return true if all entries are tagged
  int allTagged() const;

  // remove all instances of x
  void removeAll(const T & x);
  
  // remove all tagged values
  void removeTagged();

  // Assign tag values to all members of the list
  void tagAll(int v);
  
  // returns true if empty
  int empty() const { return head_ == 0; }

  // returns number of entries in list
  long length() const;

  // Compatibility with Polly's AvList
  long getNum() const { return length(); }

  // Promote x to front(end) of list
  // <group>
  void toHead(const T & x);
  void toEnd(const T & x);
  void promote(const T & x) { toHead(x); }
  void demote(const T & x) { toEnd(x); }
  // </group>

  // clear the list (remove all elements from the list)
  void clear()
  {
    if (trace_) fprintf(stderr, "AvList<T>::clear()\n");
    while (!empty()) removeHead();
  }

  // send 1 to enable list tracing, or 0 to disable tracing
  void trace(int t)
  { fprintf(stderr, "AvList<T>::trace(%d)\n", t); trace_ = t; }

  // return true if tracing is enabled
  int  trace() const { return trace_; }

private:

  // Return the head/tail node in the list
  AvListItem<T> * head() const { return head_; }
  AvListItem<T> * tail() const { return tail_; }
  
  // Find node given value
  AvListItem<T> *findNodePtr(const T & x) const;
  
  // Remove and return value when node pointer is available
  T remove(AvListItem<T> *n)
  {
    if (trace_) fprintf(stderr, "AvList<T>::remove(AvListItem<T>*)\n");
    T retval;
    if (head_ == n) head_ = head_->next();
    if (tail_ == n) tail_ = tail_->prev();
    n->separate();
    retval = n->value();
    delete n;
    return retval;
  }

  AvListItem<T> *head_;
  AvListItem<T> *tail_;
  int       trace_;
};

// <summary> List Iterator </summary>

template <class T> class AvListIter
{
public:

  AvListIter() :  p_(0), l_(0) {}

  // Build an iterator on the list l
  AvListIter(const AvList<T> * l) : p_(l->head()), l_((AvList<T> *)l) {}
  
  // Return the value at the iterators current position
  T peek()   const     { return (p_) ? p_->value() : (T)0; }

  // Move the iterator forward by 1
  void step()          { p_ = (p_) ? p_->next() : 0;       }

  // Move the iterator backward by 1
  void back()          { p_ = (p_) ? p_->prev() : 0;       }

  // Return the current value and advance the iterator by 1
  T get()              { T r = peek(); step(); return r;   }

  // Return the current value and move the iterator back by 1
  T revGet()           { T r = peek(); back(); return r;   }

  // Return true if the iterator has reached the front or end of the list
  // <group>
  int isdone() const   { return (p_ == 0);                 }
  int done()   const   { return (p_ == 0);                 }
  int atEnd()  const   { return (p_ == 0);                 }
  // </group>

  // Move the iterator to the beginning of the list
  void rewind()        { p_ = l_->head();                  }

  // Move the iterator to the end of the list
  void fwdWind()       { p_ = l_->tail();                  }

  // Return true if this item is tagged
  int tagged() const   { return (p_) ? p_->tagged() : 0;   }

  // Tag the item with the value t
  void tag(int t)      { p_->tag(t);                       }

  // Move the iterator to the first occurrence of x
  int seek(const T & x);
  
protected:

  AvListItem<T> *p_;
  AvList<T> *l_;
};

// <summary> Indexed AvList Iterator </summary>

template <class T> class AvListIndexIter : public AvListIter<T>
{
private:

  int index_;

public:

  AvListIndexIter() : AvListIter<T>(), index_(0) {}

  // Attach an indexed iterator on the list l
  AvListIndexIter(const AvList<T> * l)
  : AvListIter<T>((AvList<T> *)l), index_(0) {}

  // Move to (0-based) nth item in list
  void gotoNth(int ndx);
  
  // Move the iterator forward or backward by 1
  // <group>
  void step()   { if (!isdone()) index_++; AvListIter<T>::step(); }
  void back()   { if (!isdone()) index_--; AvListIter<T>::back(); }
  // </group>

  // Return the current value and move the iterator forward(backward) by 1
  // <group>
  T get()       { if (!isdone()) index_++; return AvListIter<T>::get(); }
  T revGet()    { if (!isdone()) index_--; return AvListIter<T>::revGet(); }
  // </group>

  // Move the iterator to the beginning(end) of the list
  // <group>
  void rewind() { index_ = 0; AvListIter<T>::rewind(); }
  void fwdWind() { int n = l_->length()-1; gotoNth(n); index_ = n; }
  // </group>

  // Return the current (0-based) index into the list.
  int index() const { return index_; }
};
  
#endif


