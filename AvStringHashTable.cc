//# Copyright (C) 1995-99 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvStringHashTable.cc,v 19.0 2003/07/16 05:47:29 aips2adm Exp $
//
// $Log: AvStringHashTable.cc,v $
// Revision 19.0  2003/07/16 05:47:29  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:08  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:38  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:45  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:13  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:34  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/08/25 19:39:48  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:40:22  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:19  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:41  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:41  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/11 21:06:50  hr
// Removed call to abs with an unsigned number.
//
// Revision 9.0  1997/08/25 21:29:01  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:17:43  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 10:01:37  droberts
// Final update from monet archive.
//
// Revision 1.4  1996/12/11 20:40:12  hr
// Added RCS header and copyright.
//
//
//---------------------------------------------------------------------------
//
//
//  open-addressed hash table, power of two size, multiplication method,
//  double hashing.
//
//

#include <iostream.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "AvStringHashTable.h"

//#define HASHTABLEVERBOSE

template <class V>
AvStringHashTable<V>::AvStringHashTable(int n, V zero, const char * name) 
  : nElements_(0), Vzero_(zero)
{
  size_ = gPow2(n);
  if ((nElements_ / (float) size_) > 0.95) size_ *= 2;
  
  Karray_ = new char *[size_];
  memset(Karray_, 0, size_*sizeof(char *));
  Varray_ = new V[size_];
  for (int i = 0; i < size_; i++) Varray_[i] = Vzero_;

  name_ = new char[strlen(name)+1];
  strcpy(name_, name);
}

template <class V>
AvStringHashTable<V>::~AvStringHashTable()
{
  // delete any non-zero entries
  for (int i = 0; i < size_; i++)
    {
      if (Karray_[i] != 0) 
	{ delete [] Karray_[i]; Karray_[i] = 0; nElements_--; }
    }
  assert(nElements_ == 0);

  delete [] Karray_;
  delete [] Varray_;

  if (name_) delete [] name_;
}

template<class V>
int AvStringHashTable<V>::add(const char * key, V value)
{
  int counter = 0;
  
  if (loadFactor() > .90) grow();

  while (counter < size_)
    {
      int ndx = hash(key, counter);

      if (Karray_[ndx] == 0)
	{
	  Karray_[ndx] = new char[strlen(key)+1];
	  strcpy(Karray_[ndx], key);
	  Varray_[ndx] = value;
#ifdef HASHTABLEVERBOSE
	  cout << "Hashtable " << name_ << ": Stored '" << key << "' in " << ndx << " after " << counter << " conflicts\n";
#endif
	  nElements_++;
	  return 0;
	}
      else
	{
	  counter++;
	}
    }
  cerr << "Hashtable " << name_ << "::add() - Table Overflow - load = " << nElements() / (float) size_ << endl;
  return -1;
}

//
// search for key.  If found then make new table and rehash all
// entries into new table.
//
// 1 entry no longer in table
// 0 failure to remove
//
// 
//

template <class V>
void AvStringHashTable<V>::remove(const char * key)
{
  int i;
  int ndx = search_(key);
  if (ndx == -1)
    {
      // key doesn't exist in table
      return;
    }

  char ** newKarray = new char *[size_];
  if (!newKarray) return;
  V * newVarray = new V[size_];
  if (!newVarray) { delete [] newKarray; return; }

  memset(newKarray, 0, size_*sizeof(char *));
  for (i = 0; i < size_; i++) newVarray[i] = Vzero_;

  char ** oldKarray = Karray_;
  V * oldVarray = Varray_;

  Karray_ = newKarray;
  Varray_ = newVarray;

  nElements_ = 0;

  for (i = 0; i < size_; i++)
    {
      if (i == ndx) continue;
      if (oldVarray[i] != Vzero_)
	add(oldKarray[i], oldVarray[i]);
    }

  delete [] oldKarray;
  delete [] oldVarray;

#ifdef HASHTABLEVERBOSE
  cout << "HashTable: remove successful\n";
#endif

}

template <class V>
int AvStringHashTable<V>::search_(const char * key)
{
  // printf("search('%s')\n", key);
  int counter = 0;
  while(counter < size_)
    {
      int ndx = hash(key, counter);

      if (Karray_[ndx] == NULL) return -1;

      if (!strcmp(Karray_[ndx], key))
	{
	  counter = counter + 1;	  
#ifdef HASHTABLEVERBOSE
	  cout << "HashTable " << name_ << "::search_() - found '" << key << "' in " << counter << " tries\n";
#endif
	  return ndx;
	}
      else
	{
	  counter = counter + 1;
	}
    }
  cerr << "HashTable " << name_ << "::search() - Overflow!\n";
  return -1;
} 

template <class V>
V AvStringHashTable<V>::search(const char * key)
{
  // printf("search('%s')\n", key);
  int counter = 0;
  while(counter < size_)
    {
      int ndx = hash(key, counter);

      if (Karray_[ndx] == NULL) return Vzero_;

      if (!strcmp(Karray_[ndx], key))
	{
	  counter = counter + 1;	  
#ifdef HASHTABLEVERBOSE
	  cout << "HashTable " << name_ << "::search() - found '" << key << "' in " << counter << " tries\n";
#endif
	  return Varray_[ndx];
	}
      else
	{
	  counter = counter + 1;
	}
    }
  cerr << "HashTable " << name_ << "::search() - Overflow!\n";
  return Vzero_;
}

template <class V>
int AvStringHashTable<V>::hash(const char * key, int counter)
{
  unsigned long keyval = 0;
  unsigned long k2 = 0;
  int len = (int)strlen(key);
  if (len < 4) strcpy((char *) &keyval, key);
  if (len >= 4) { memcpy(&keyval, key, 4); }
  if (len >= 8) { memcpy(&k2, key+4, 4); keyval ^= k2; }
  if (len >= 12) { memcpy(&k2, key+8, 4); keyval ^= k2; }
  
// (keyval is unsigned, so abs is unnecessary).
//  keyval = abs(keyval);

  double v1 = (double)keyval * 0.6180339887;  // (sqrt5-1)/2, Knuth
  double hv1 = (v1 - (int) v1);
  int h1 = (int) (size_ * hv1);
  
  double v2 = (double)keyval * 0.87380339887;  // double hash, me 
  double hv2 = (v2 - (int) v2);      
  int h2 = ((int) (size_ * hv2)) * 2 + 1; 
  
  int retval = (h1 + counter*h2) % size_;

  return retval;
}

//
//  Grow must extract all values present in the current array and,
//  using the hash function based on the new table size, put them
//  into the new table area.
//
//  Returns -1 on fail
//
//  [ ] DYNAMIC STRINGS!!! FIX!!!
//
template <class V>
int AvStringHashTable<V>::grow()
{
  int i;
#ifdef HASHTABLEVERBOSE
  cout << "HashTable " << name_ << ": growing table from " << size_ << " to " << size_*2 << endl;
#endif
  
  // allocate space or return fail
  char ** newKarray = new char *[size_*2];
  if (!newKarray) return -1;
  V * newVarray = new V[size_*2];
  if (!newVarray) { delete [] newKarray; return -1; }

  // rehash into new table
  memset(newKarray, 0, size_*2*sizeof(char *));
  for (i = 0; i < size_*2; i++) newVarray[i] = Vzero_;

  int oldSize = size_;
  size_ *= 2;
  nElements_ = 0;

  char ** oldKarray = Karray_;
  V * oldVarray = Varray_;

  Karray_ = newKarray;
  Varray_ = newVarray;
  
  for (i = 0; i < oldSize; i++)
    {
      if (oldVarray[i] != Vzero_)
	{
	  add(oldKarray[i], oldVarray[i]);
	}
    }

  // delete the old string entries and arrays
  for (i = 0; i < oldSize; i++)
    {
      if (oldKarray[i] != 0)
	{ delete [] oldKarray[i]; }
    }
  delete [] oldKarray;
  delete [] oldVarray;

#ifdef HASHTABLEVERBOSE
  cout << "HashTable " << name_ << ": grow succeded!\n";
#endif

  return 0;
}

template <class V>
void AvStringHashTable<V>::clear()
{
  for (int i = 0; i < size_; i++) 
    {
      if (Varray_[i] != Vzero_)
	{
	  delete [] Karray_[i];
	  Varray_[i] = Vzero_;
	}
    }
  nElements_ = 0;
}
