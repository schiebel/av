//# AvIPosition.cc: A vector of integers, used to index into arrays.
//# Copyright (C) 1994,1995,1996,1997,1998,1999
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
//# $Id: AvIPosition.cc,v 19.0 2003/07/16 05:47:27 aips2adm Exp $
//# --------------------------------------------------
//
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvIPosition.cc,v 19.0 2003/07/16 05:47:27 aips2adm Exp $
//
// $Log: AvIPosition.cc,v $
// Revision 19.0  2003/07/16 05:47:27  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:07  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:36  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:43  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:07  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:32  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/12/13 09:57:25  gvandiep
// Move IPosition,Slice,Slicer from Lattices to Arrays
//
// Revision 13.0  1999/08/10 18:40:19  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:15  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:35  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:34  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/09 21:37:42  hr
// Removed an unneeded semicolon.
//
// Revision 9.0  1997/08/25 21:28:48  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:17:49  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1996/12/12 09:26:59  droberts
// Final update from monet archive.
//
// Revision 1.5  1996/11/18 20:40:12  hr
// Added RCS header.
//
//
//---------------------------------------------------------------------------

// #include <aips/Arrays/AvIPosition.h>
// #include <aips/Exceptions/Error.h>
// #include <aips/Arrays/ArrayError.h>
// #include <aips/IO/AipsIO.h>
// #include <aips/Utilities/Assert.h>

#include <iostream.h>
#if defined(sparc) && ! defined(__SVR4)
#include <stdio.h>		// printf (for assert)
#endif
#include <assert.h>
#include "AvWPosition.h"
#include "AvIPosition.h"

#ifndef MIN
#define MIN(A,B) (((A) < (B)) ? (A) : (B))
#endif

#define DebugAssert(A,B)

AvIPosition::AvIPosition() : size(0), data(&buffer_p[0])
{
    // Nothing
    DebugAssert(ok(), AipsError);
}

AvIPosition::AvIPosition(unsigned int length) : size(length)
{
    if (size <= BufferLength) {
        data = buffer_p;
    } else {
	data = new int[size];
	if (data == 0) 
	  {
	    cerr << "Cannot allocate storage" << endl;
	}
    }
    DebugAssert(ok(), AipsError);
}

AvIPosition::AvIPosition(unsigned int length, int val) : size(length)
{
    if (size <= BufferLength) {
        data = buffer_p;
    } else {
	data = new int[size];
	if (data == 0) {
	  cerr << "Cannot allocate storage" << endl;
	}
    }
    for (unsigned int i=0; i<size; i++) {
	data[i] = val;
    }
    DebugAssert(ok(), AipsError);
}

#if 1
AvIPosition::AvIPosition(const AvWPosition & wp)
{
  size = wp.nelements();

  if (nelements() <= BufferLength) {
    data = buffer_p;
  } else {
    data = new int[size];
    if (data == 0) {
      cerr << "Cannot allocate storage" << endl;
    }
  }
  for (unsigned int i=0; i<size; i++) {
    data[i] = (int) (wp(i) + 0.5);
  }
  DebugAssert(ok(), AipsError);
}
#endif

AvIPosition::AvIPosition (unsigned int length, int val0, int val1, int val2, int val3, 
		      int val4, int val5, int val6, int val7, int val8, 
		      int val9)
: size(length), data(0)
{
    if (size <= BufferLength) {
        data = buffer_p;
    } else {
	data = new int[size];
	if (data == 0) {
	  cerr << "Cannot allocate storate" << endl;
	}
    }
    if (size > 10 || length < 1) {
	cerr << "AvIPosition::AvIPosition(unsigned int length, val0, ...) - "
			 "Can only initialize from 1 to 10 elements";
    }

    switch (length) {
         case 10: data[9] = val9;    // Fall through
	 case 9: data[8] = val8;    // Fall through
	 case 8: data[7] = val7;    // Fall through
	 case 7: data[6] = val6;    // Fall through
	 case 6: data[5] = val5;    // Fall through
	 case 5: data[4] = val4;    // Fall through
	 case 4: data[3] = val3;    // Fall through
	 case 3: data[2] = val2;    // Fall through
	 case 2: data[1] = val1;    // Fall through
	 case 1: data[0] = val0; break;
	 default: DebugAssert(0, AipsError); // NOTREACHED
    }
    for (unsigned int i=0; i < size; i++) {
	if (data[i] == MIN_INT) {
	  cerr << "AvIPosition::AvIPosition(unsigned int length, val0, ...) - "
		"One or more valn == INT_MIN. Probably haven't defined "
		"enough values. Otherwise specify after construction.";
	}
    }

    DebugAssert(ok(), AipsError);
}

AvIPosition::AvIPosition(const AvIPosition &other) : size(other.size)
{
    if (size <= BufferLength) {
        data = buffer_p;
    } else {
	data = new int[size];
	if (data == 0) {
	  cerr << "Cannot allocate storage" << endl;
	}
    }
    for (unsigned int i=0; i < size; i++) {
      data[i] = other.data[i];
    }
    DebugAssert(ok(), AipsError);
}

AvIPosition::~AvIPosition()
{
    if (data != &buffer_p[0]) {
	// We could also unconditionally delete data; however in the past
	// some compilers have complained about deleting a zero-pointer even
	// though it is supposed to be OK.
	delete [] data;
    }
}

AvIPosition AvIPosition::nonDegenerate(unsigned int startingAxis) const
{
  DebugAssert(ok(), AipsError);
  if (startingAxis >= size) 
    cerr << "AvIPosition::nonDegenerate: startingAxis is greater than"
		     " size of this AvIPosition.";
  unsigned int ctr = startingAxis;
  if(size <= BufferLength) {
    for (unsigned int i=startingAxis;i<size;i++)
      if (buffer_p[i]!=1) ctr++;
  } else {
    for (unsigned int i=startingAxis;i<size;i++)
      if (data[i]!=1) ctr++;
  }
  if (ctr == size) return *this;

  AvIPosition nondegenerateIP(ctr,1);
  ctr=0;
  if(size <= BufferLength) {
    for (unsigned int i=0; i<size; i++)
      if(buffer_p[i] == 1 && i >= startingAxis) ctr++;
      else nondegenerateIP(i-ctr) = buffer_p[i]; 
  } else {
    for (unsigned int i=0; i<size; i++)
      if(data[i] == 1 && i >= startingAxis) ctr++;
      else nondegenerateIP(i-ctr) = data[i];    
  }
  return nondegenerateIP;
}

void AvIPosition::resize(unsigned int newSize, int copy)
{
    DebugAssert(ok(), AipsError);

    // If the size is unchanged, just return (more efficient)
    if (newSize == size) {
	return;
    }

    int *newData = 0;
    if (newSize <= BufferLength) {
        newData = buffer_p;
    } else {
        newData = new int[newSize];
	if (data == 0) {
	  cerr << "Cannot allocate storage" << endl;
	}
    }
    
    if (newData != data && copy) {
	for (unsigned int i=0; i < MIN(size, newSize); i++) {
	    newData[i] = data[i];
	}
    }

    // Delete the old data, if any
    if (size > 0 && data != &buffer_p[0]) {
	delete [] data;
        data = 0;
    }
    // Point to new data
    size = newSize;
    data = newData;
}

AvIPosition &AvIPosition::operator=(const AvIPosition &other)
{
  //cout << "KPos:op=() - size=" << size << ", other.size=" << other.size << endl;

    DebugAssert(ok(), AipsError);
    if (&other == this) {
	return *this;
    }
    if (size == 0) {
	this->resize(other.nelements());
    }
    if (conform(other) == 0) 
      {
	cerr << "AvIPosition::operator=(const AvIPosition & - "
				    "this and other differ in length";
	assert(0);
    }
    for (unsigned int i=0; i < size; i++) {
	data[i] = other.data[i];
    }
    return *this;
}

AvIPosition &AvIPosition::operator=(int value)
{
    DebugAssert(ok(), AipsError);
    for (unsigned int i=0; i < size; i++) {
	data[i] = value;
    }
    return *this;
}

void AvIPosition::append (const AvIPosition& other)
{
    unsigned int j = size;
    resize (size + other.size);
    for (unsigned int i=0; i<other.size; i++) {
	data[j++] = other.data[i];
    }
}

void AvIPosition::prepend (const AvIPosition& other)
{
    unsigned int j = size, i;
    resize (size + other.size);
    for (i=size; j>0;) {
	data[--i] = data[--j];
    }
    for (i=0; i<other.size; i++) {
	data[i] = other.data[i];
    }
}

AvIPosition AvIPosition::concatenate (const AvIPosition& other) const
{
    AvIPosition tmp (*this);
    tmp.append (other);
    return tmp;
}

void AvIPosition::setFirst (const AvIPosition& other)
{
    if (size < other.size) {
	cerr << "AvIPosition::setFirst(other); other is too long";
    }
    for (unsigned int i=0; i<other.size; i++) {
	data[i] = other.data[i];
    }
}

void AvIPosition::setLast (const AvIPosition& other)
{
    if (size < other.size) {
	cerr << "AvIPosition::setLast(other); other is too long";
    }
    unsigned int j = size - other.size;
    for (unsigned int i=0; i<other.size; i++) {
	data[j++] = other.data[i];
    }
}

AvIPosition AvIPosition::getFirst (unsigned int n) const
{
    if (size < n) {
	cerr << "AvIPosition::getFirst(n); n is too high";
    }
    AvIPosition tmp(n);
    for (unsigned int i=0; i<n; i++) {
	tmp.data[i] = data[i];
    }
    return tmp;
}

AvIPosition AvIPosition::getLast (unsigned int n) const
{
    if (size < n) {
        cerr << "AvIPosition::getLast(n); n is too high";
    }
    AvIPosition tmp(n);
    unsigned int j = size - n;
    for (unsigned int i=0; i<n; i++) {
	tmp.data[i] = data[j++];
    }
    return tmp;
}


void AvIPosition::operator += (const AvIPosition  &other)
{
    DebugAssert(ok(), AipsError);
    if (conform(other) == 0) {
      cerr << "AvIPosition::operator += "
	"(const AvIPosition &) - "
	"this and other differ in length";
    }
    for (unsigned int i=0; i < size; i++) {
	data[i] += other.data[i];
    }
}

void AvIPosition::operator -= (const AvIPosition  &other)
{
    DebugAssert(ok(), AipsError);
    if (conform(other) == 0) {
      cerr << "AvIPosition::operator -= "
	"(const AvIPosition &) - "
	"this and other differ in length";
    }
    for (unsigned int i=0; i < size; i++) {
	data[i] -= other.data[i];
    }
}

void AvIPosition::operator *= (const AvIPosition  &other)
{
    DebugAssert(ok(), AipsError);
    if (conform(other) == 0) {
      cerr << "AvIPosition::operator *= "
	"(const AvIPosition &) - "
	"this and other differ in length";
    }
    for (unsigned int i=0; i < size; i++) {
	data[i] *= other.data[i];
    }
}

void AvIPosition::operator /= (const AvIPosition  &other)
{
    DebugAssert(ok(), AipsError);
    if (conform(other) == 0) {
      cerr << "AvIPosition::operator /= "
	"(const AvIPosition &) - "
	"this and other differ in length";
    }
    for (unsigned int i=0; i < size; i++) {
	data[i] /= other.data[i];
    }
}

void AvIPosition::operator += (int val)
{
    DebugAssert(ok(), AipsError);
    for (unsigned int i=0; i < size; i++) {
	data[i] += val;
    }
}

void AvIPosition::operator -= (int val)
{
    DebugAssert(ok(), AipsError);
    for (unsigned int i=0; i < size; i++) {
	data[i] -= val;
    }
}

void AvIPosition::operator *= (int val)
{
    DebugAssert(ok(), AipsError);
    for (unsigned int i=0; i < size; i++) {
	data[i] *= val;
    }
}

void AvIPosition::operator /= (int val)
{
    DebugAssert(ok(), AipsError);
    for (unsigned int i=0; i < size; i++) {
	data[i] /= val;
    }
}

int AvIPosition::isEqual (const AvIPosition &other) const
{
    return isEqual (other, nelements());
}

int AvIPosition::isEqual (const AvIPosition &other, unsigned int nrCompare) const
{
    if (! conform (other)) {
	return 0;
    }
    if (nrCompare > nelements()) {
	nrCompare = nelements();
    }
    for (unsigned int i=0; i<nrCompare; i++) {
	if (data[i] != other(i)) {
	    return 0;
	}
    }
    return 1;
}


AvIPosition operator + (const AvIPosition &left, const AvIPosition &right)
{
    if (left.conform(right) == 0) {
      cerr << "::operator + "
	"(const AvIPosition &, const AvIPosition &) - "
	"left and right operand do not conform ";
    }
    AvIPosition result(left);
    result += right;
    return result;
}

AvIPosition operator - (const AvIPosition &left, const AvIPosition &right)
{
    if (left.conform(right) == 0) {
	cerr << "::operator - "
	  "(const AvIPosition &, const AvIPosition &) - "
	  "left and right operand do not conform ";
    }
    AvIPosition result(left);
    result -= right;
    return result;
}

AvIPosition operator * (const AvIPosition &left, const AvIPosition &right)
{
  if (left.conform(right) == 0) {
    cerr << "::operator * "
      "(const AvIPosition &, const AvIPosition &) - "
      "left and right operand do not conform ";
    }
    AvIPosition result(left);
    result *= right;
    return result;
}

AvIPosition operator / (const AvIPosition &left, const AvIPosition &right)
{
    if (left.conform(right) == 0) {
	cerr << "::operator / "
	  "(const AvIPosition &, const AvIPosition &) - "
	  "left and right operand do not conform ";
    }
    AvIPosition result(left);
    result /= right;
    return result;
}

AvIPosition operator + (const AvIPosition &left, int val)
{
    AvIPosition result(left);
    result += val;
    return result;
}

AvIPosition operator - (const AvIPosition &left, int val)
{
    AvIPosition result(left);
    result -= val;
    return result;
}

AvIPosition operator * (const AvIPosition &left, int val)
{
    AvIPosition result(left);
    result *= val;
    return result;
}

AvIPosition operator / (const AvIPosition &left, int val)
{
    AvIPosition result(left);
    result /= val;
    return result;
}

AvIPosition operator + (int val, const AvIPosition &right)
{
    AvIPosition result(right.nelements());
    result = val;
    result += right;
    return result;
}

AvIPosition operator - (int val, const AvIPosition &right)
{
    AvIPosition result(right.nelements());
    result = val;
    result -= right;
    return result;
}

AvIPosition operator * (int val, const AvIPosition &right)
{
    AvIPosition result(right.nelements());
    result = val;
    result *= right;
    return result;
}

AvIPosition operator / (int val, const AvIPosition &right)
{
    AvIPosition result(right.nelements());
    result = val;
    result /= right;
    return result;
}

int AvIPosition::product() const
{
    if (nelements() ==  0) {
	return 0;
    }
    int total = 1;
    for (unsigned int i=0; i<nelements(); i++) {
	total *= (*this)(i);
    }
    return total;
}

int AvIPosition::sum() const
{
    if (nelements() ==  0) {
	return 0;
    }
    int total = 0;
    for (unsigned int i=0; i<nelements(); i++) {
	total += (*this)(i);
    }
    return total;
}

int operator == (const AvIPosition &left, const AvIPosition &right)
{
  if (left.conform(right) == 0) {
    cerr << "::operator== "
      "(const AvIPosition &, const AvIPosition &) - "
      "left and right operand do not conform ";
    }
    unsigned int n=left.nelements();
    int result = 1;
    for (unsigned int i=0; i < n; i++) {
	if (left(i) == right(i)) {
	    // Nothing - written to make cut and paste easier
	} else {
	    result = 0;
	    break;
	}
    }
    return result;
}

int operator != (const AvIPosition &left, const AvIPosition &right)
{
    if (left.conform(right) == 0) {
	cerr << "::operator!= "
	  "(const AvIPosition &, const AvIPosition &) - "
	  "left and right operand do not conform ";
    }
    unsigned int n=left.nelements();
    int result = 0;
    for (unsigned int i=0; i < n; i++) {
	if (left(i) != right(i)) {
	    result = 1;
	    break;
	} else {
	    // Nothing - written to make cut and paste easier
	}
    }
    return result;
}

int operator < (const AvIPosition &left, const AvIPosition &right)
{
    if (left.conform(right) == 0) 
      {
	cerr << "::operator< "
	  "(const AvIPosition &, const AvIPosition &) - "
	  "left and right operand do not conform ";
      }
    unsigned int n=left.nelements();
    int result = 1;
    for (unsigned int i=0; i < n; i++) {
	if (left(i) < right(i)) {
	    // Nothing - written to make cut and paste easier
	} else {
	    result = 0;
	    break;
	}
    }
    return result;
}

int operator <= (const AvIPosition &left, const AvIPosition &right)
{
    if (left.conform(right) == 0) 
      {
	cerr << "::operator<= "
	  "(const AvIPosition &, const AvIPosition &) - "
	  "left and right operand do not conform ";
      }

    unsigned int n=left.nelements();
    int result = 1;
    for (unsigned int i=0; i < n; i++) {
	if (left(i) <= right(i)) {
	    // Nothing - written to make cut and paste easier
	} else {
	    result = 0;
	    break;
	}
    }
    return result;
}

int operator > (const AvIPosition &left, const AvIPosition &right)
{
    if (left.conform(right) == 0) {
	cerr << "::operator> "
	  "(const AvIPosition &, const AvIPosition &) - "
	  "left and right operand do not conform ";
    }
    unsigned int n=left.nelements();
    int result = 1;
    for (unsigned int i=0; i < n; i++) {
	if (left(i) > right(i)) {
	    // Nothing - written to make cut and paste easier
	} else {
	    result = 0;
	    break;
	}
    }
    return result;
}

int operator >= (const AvIPosition &left, const AvIPosition &right)
{
    if (left.conform(right) == 0) {
      cerr << "::operator>= "
	"(const AvIPosition &, const AvIPosition &) - "
	"left and right operand do not conform ";
    }
    unsigned int n=left.nelements();
    int result = 1;
    for (unsigned int i=0; i < n; i++) {
	if (left(i) >= right(i)) {
	    // Nothing - written to make cut and paste easier
	} else {
	    result = 0;
	    break;
	}
    }
    return result;
}

int operator == (const AvIPosition &left, int val)
{
    int result = 1;
    unsigned int n = left.nelements();
    for (unsigned int i=0; i < n; i++) {
	if (left(i) == val) {
	    // Nothing
	} else {
	    result = 0;
	    break;
	}
    }
    return result;
}

int operator != (const AvIPosition &left, int val)
{
    int result = 0;
    unsigned int n = left.nelements();
    for (unsigned int i=0; i < n; i++) {
	if (left(i) != val) {
	    result = 1;
	    break;
	} else {
	    // Nothing
	}
    }
    return result;
}

int operator < (const AvIPosition &left, int val)
{
    int result = 1;
    unsigned int n = left.nelements();
    for (unsigned int i=0; i < n; i++) {
	if (left(i) < val) {
	    // Nothing
	} else {
	    result = 0;
	    break;
	}
    }
    return result;
}

int operator <= (const AvIPosition &left, int val)
{
    int result = 1;
    unsigned int n = left.nelements();
    for (unsigned int i=0; i < n; i++) {
	if (left(i) <= val) {
	    // Nothing
	} else {
	    result = 0;
	    break;
	}
    }
    return result;
}

int operator > (const AvIPosition &left, int val)
{
    int result = 1;
    unsigned int n = left.nelements();
    for (unsigned int i=0; i < n; i++) {
	if (left(i) > val) {
	    // Nothing
	} else {
	    result = 0;
	    break;
	}
    }
    return result;
}

int operator >= (const AvIPosition &left, int val)
{
    int result = 1;
    unsigned int n = left.nelements();
    for (unsigned int i=0; i < n; i++) {
	if (left(i) >= val) {
	    // Nothing
	} else {
	    result = 0;
	    break;
	}
    }
    return result;
}

int operator == (int val, const AvIPosition &right)
{
    int result = 1;
    unsigned int n = right.nelements();
    for (unsigned int i=0; i < n; i++) {
	if (val == right(i)) {
	    // Nothing
	} else {
	    result = 0;
	    break;
	}
    }
    return result;
}

int operator != (int val, const AvIPosition &right)
{
    int result = 0;
    unsigned int n = right.nelements();
    for (unsigned int i=0; i < n; i++) {
	if (val != right(i)) {
	    result = 1;
	    break;
	} else {
	    // Nothing
	}
    }
    return result;
}

int operator < (int val, const AvIPosition &right)
{
    int result = 1;
    unsigned int n = right.nelements();
    for (unsigned int i=0; i < n; i++) {
	if (val < right(i)) {
	    // Nothing
	} else {
	    result = 0;
	    break;
	}
    }
    return result;
}

int operator <= (int val, const AvIPosition &right)
{
    int result = 1;
    unsigned int n = right.nelements();
    for (unsigned int i=0; i < n; i++) {
	if (val <= right(i)) {
	    // Nothing
	} else {
	    result = 0;
	    break;
	}
    }
    return result;
}

int operator > (int val, const AvIPosition &right)
{
    int result = 1;
    unsigned int n = right.nelements();
    for (unsigned int i=0; i < n; i++) {
	if (val > right(i)) {
	    // Nothing
	} else {
	    result = 0;
	    break;
	}
    }
    return result;
}

int operator >= (int val, const AvIPosition &right)
{
    int result = 1;
    unsigned int n = right.nelements();
    for (unsigned int i=0; i < n; i++) {
	if (val >= right(i)) {
	    // Nothing
	} else {
	    result = 0;
	    break;
	}
    }
    return result;
}

ostream &operator<<(ostream &os, const AvIPosition &ip)
{
    os << "[";
    for(unsigned int i=0; i < ip.nelements(); i++) {
	os << ip(i);
	if (i < ip.nelements() - 1) {
	    os << ", ";
	}
    }
    os << "]";
    return os;
}

int AvIPosition::ok() const
{
    int retval = 1;
    if (size <= BufferLength && data != &buffer_p[0]) { retval = 0; }
    if (data == 0) { retval = 0; }
    return retval;
}

AvIPosition toAvIPositionInArray (const unsigned int offset,
				  const AvIPosition &shape, 
				  const AvIPosition &origin)
{
    if ( ! origin.conform(shape) ) {
      cerr << "AvIPosition ::toIPositionInArray (const unsigned int offset,"
	" const AvIPosition &origin, const AvIPosition &shape)"
	" - IPositions do not conform";
    }

    if ( ! isInsideArray (offset, shape, origin) ) {
      cerr << "AvIPosition ::toIPositionInArray (const unsigned int offset,"
            " const AvIPosition &origin, const AvIPosition &shape)"
	" - Invalid offset.";
    }
    
    AvIPosition iposition (origin.nelements());
    unsigned int divisor = 1;

    unsigned int ndim = origin.nelements();
    for (unsigned int idim = 0; idim < ndim; idim++) {
        iposition(idim) = origin(idim) + ((offset / divisor) % shape(idim));
        divisor *= shape(idim);
    }

    return iposition;

}

unsigned int toOffsetInArray (const AvIPosition &iposition,
                      const AvIPosition &shape, const AvIPosition &origin)
{
    if ( ! (iposition.conform(origin) && iposition.conform(shape)) ) {
      cerr << "unsigned int ::toOffsetInArray (const AvIPosition &iposition,"
            " const AvIPosition &origin, const AvIPosition &shape)"
             " - IPositions do not conform";
    }

    if ( ! isInsideArray (iposition, shape, origin) ) {
	cerr <<  "unsigned int ::toOffsetInArray (const AvIPosition &iposition,"
            " const AvIPosition &origin, const AvIPosition &shape)"
             " - Invalid iposition.";
    }

    unsigned int offset = 0;
    unsigned int multiplier = 1;

    unsigned int ndim = origin.nelements();
    for (unsigned int idim = 0; idim < ndim; idim++) {
        offset += ((iposition(idim) - origin(idim)) * multiplier);
        multiplier *= shape(idim);
    }

    return offset;
}


int isInsideArray (const unsigned int offset,
                    const AvIPosition &shape, const AvIPosition &origin)
{
    if ( ! origin.conform(shape) ) {
      cerr << "int ::isInsideArray (const unsigned int offset,"
            " const AvIPosition &origin, const AvIPosition &shape)"
	" - IPositions do not conform";
    }

    unsigned int nelements = 1;

    unsigned int ndim = origin.nelements();
    for (unsigned int idim = 0; idim < ndim; idim++) {
        nelements *= shape(idim);
    }

    return (offset < nelements) ? 1 : 0;
}


int isInsideArray (const AvIPosition &iposition,
                    const AvIPosition &shape, const AvIPosition &origin)
{
    if ( ! (iposition.conform(origin) && iposition.conform(shape)) ) {
      cerr << "int ::isInsideArray (const AvIPosition &iposition,"
            " const AvIPosition &origin, const AvIPosition &shape)"
             " - IPositions do not conform";
    }

    int result = 1;
    int ioff;

    unsigned int ndim = origin.nelements();
    for (unsigned int idim = 0; idim < ndim; idim++) {
        ioff = iposition(idim) - origin(idim);
        if ( (ioff < 0) || (ioff >= shape(idim)) ) {
            result = 0;
            break;
        }
    }

    return result;

}
