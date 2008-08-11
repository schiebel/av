//# Copyright (C) 1995-97 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvStringHashTable.h,v 19.0 2003/07/16 05:47:20 aips2adm Exp $
//
// $Log: AvStringHashTable.h,v $
// Revision 19.0  2003/07/16 05:47:20  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:01  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:30  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:37  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:43  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:24  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:12  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:01  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:16  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:11  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:28:13  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:17:14  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 08:57:35  droberts
// Final update from monet archive.
//
// Revision 1.4  1996/12/11 20:40:12  hr
// Added RCS header and copyright.
//
//
//---------------------------------------------------------------------------
//
//  AvStringHashTable.h
//

#ifndef __AvStringHashTable_h__
#define __AvStringHashTable_h__

#include <string.h>

//  AvStringHashTable.h
//
//  <summary>
//  AvStringHashTable is a specialization of the more general
//  HashTable<K,V>.  It uses double-hashing on C arrays to 
//  generate a fast associative array for string to value
//  of type V.
//  </summary>
//
//  <synopsis>
//  Key,Value pairs are added using a hash table of power-of-two
//  size.  The table is doubled automatically when the load 
//  factor exceeds 95%.
//
//  Dictionary operations on the hash table 
//  can be expected to run in 1/(1-loadfactor) time if the strings are
//  different in the first twelve characters. 
//
//  The constructor requires a size estimate and a nil value,
//  i.e. one that would never be stored in the table.  
//  The size estimate will be rounded up to a power of 2. The 
//  hash table assumes that the zero value will never be stored 
//  in the table and can therefore be used to test if the table
//  entry is valid.
//  </synopsis>
//

template <class V>
class AvStringHashTable
{
public:

  // The zero value should be a value that will never be used.
  // name is optional and will be used in debugging
  AvStringHashTable(int n, V zero, const char * name = "<unnamed>");

  ~AvStringHashTable();

  // Returns a power of two number equal to or larger than val
  int gPow2(int val)
    {
      int p = 1;
      int v = val;
      while (v >>= 1) p <<=1;
      return (p == val) ? p : p << 1;
    }
  
  // Returns -1 if hash table overflows. [ O(1/(1-loadfactor)) ]
  int add(const char * key, V value);

  // look for the key 'key' and return its corresponding
  // value or return the zero value. [ O(1/(1-loadfactor)) ]
  V search(const char * key);

  // remove entry matching the key 'key' [ O(nElements) if key is in 
  // table else O(1/(1-loadfactor)) ]
  void remove(const char * key);

  // double hash function
  int hash(const char * key, int counter);

  // ratio of number of elements to size of array
  float loadFactor() const { return nElements_ / (float) size_; }

  // number of elements in the array
  int nElements() const { return nElements_; }

  // number of available entries the array currently supports
  int size() const { return size_; }

  // double the table size.  Called if necessary by add.  Values
  // are re-hashed into a new table, and the old one is deleted.
  // [ O(n) ]
  int grow();
  
  // erase the table [ O(nElements_) ]
  void clear();

private:

  // search and return index or -1
  int search_(const char * key);

  char * name_;

  int size_;
  int nElements_;
  char ** Karray_;
  V * Varray_;
  V Vzero_;
};

#endif
