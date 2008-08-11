//# AvWPosition.cc: A vector of doubles, used for world coordinates in aipsview.
//# Copyright (C) 1994,1995,1996,1997,1998
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
//# $Id: AvWPosition.cc,v 19.0 2003/07/16 05:46:57 aips2adm Exp $
//# --------------------------------------------------
//
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvWPosition.cc,v 19.0 2003/07/16 05:46:57 aips2adm Exp $
//
// $Log: AvWPosition.cc,v $
// Revision 19.0  2003/07/16 05:46:57  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:41  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:10  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:18  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:20  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:00  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:44  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:11  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:07  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:51  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/09 20:53:15  hr
// Removed an unneeded semicolon.
//
// Revision 9.0  1997/08/25 21:26:04  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:01  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 08:22:43  droberts
// Added RCS header.
//
// Revision 1.3  1996/11/18 20:40:12  hr
// Added RCS header.
//
//
//---------------------------------------------------------------------------

#include <iostream.h>
#include <assert.h>

#include <AvIPosition.h>
#include <AvWPosition.h>

#ifndef MIN
#define MIN(A,B) (((A) < (B)) ? (A) : (B))
#endif

#define DebugAssert(A,B)

const double AvWPosition::MIN_DOUBLE = 1e-100;

AvWPosition::AvWPosition() : size(0), data(&buffer_p[0])
{
    // Nothing
    DebugAssert(ok(), AipsError);
}

AvWPosition::AvWPosition(unsigned int length) : size(length)
{
    if (size <= BufferLength) {
        data = buffer_p;
    } else {
	data = new double[size];
	if (data == 0) 
	  {
	    cerr << "Cannot allocate storage" << endl;
	}
    }
    DebugAssert(ok(), AipsError);
}

AvWPosition::AvWPosition(const AvIPosition & kp)
{
  size = kp.nelements();

    if (nelements() <= BufferLength) {
        data = buffer_p;
    } else {
	data = new double[size];
	if (data == 0) {
	  cerr << "Cannot allocate storage" << endl;
	}
    }
    for (unsigned int i=0; i<size; i++) {
	data[i] = (double) kp(i);
    }
    DebugAssert(ok(), AipsError);
}

AvWPosition::AvWPosition(unsigned int length, double val) : size(length)
{
    if (size <= BufferLength) {
        data = buffer_p;
    } else {
	data = new double[size];
	if (data == 0) {
	  cerr << "Cannot allocate storage" << endl;
	}
    }
    for (unsigned int i=0; i<size; i++) {
	data[i] = val;
    }
    DebugAssert(ok(), AipsError);
}

AvWPosition::AvWPosition (unsigned int length, double val0, double val1, double val2, double val3, 
		      double val4, double val5, double val6, double val7, double val8, 
		      double val9)
: size(length), data(0)
{
    if (size <= BufferLength) {
        data = buffer_p;
    } else {
	data = new double[size];
	if (data == 0) {
	  cerr << "Cannot allocate storate" << endl;
	}
    }
    if (size > 10 || length < 1) {
      cerr << "AvWPosition::AvWPosition(unsigned int length, val0, ...) - "
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
	if (data[i] == MIN_DOUBLE) {
	cerr << "AvWPosition::AvWPosition(unsigned int length, val0, ...) - "
		"One or more valn == INT_MIN. Probably haven't defined "
		"enough values. Otherwise specify after construction.";
	}
    }

    DebugAssert(ok(), AipsError);
}

AvWPosition::AvWPosition(const AvWPosition &other) : size(other.size)
{
    if (size <= BufferLength) {
        data = buffer_p;
    } else {
	data = new double[size];
	if (data == 0) {
	  cerr << "Cannot allocate storage" << endl;
	}
    }
    for (unsigned int i=0; i < size; i++) {
      data[i] = other.data[i];
    }
    DebugAssert(ok(), AipsError);
}

AvWPosition::~AvWPosition()
{
    if (data != &buffer_p[0]) {
	// We could also unconditionally delete data; however in the past
	// some compilers have complained about deleting a zero-pointer even
	// though it is supposed to be OK.
	delete [] data;
    }
}

AvWPosition AvWPosition::nonDegenerate(unsigned int startingAxis) const
{
  DebugAssert(ok(), AipsError);
  if (startingAxis >= size) 
    cerr << "AvWPosition::nonDegenerate: startingAxis is greater than"
		     " size of this AvWPosition.";
  unsigned int ctr = startingAxis;
  if(size <= BufferLength) {
    for (unsigned int i=startingAxis;i<size;i++)
      if (buffer_p[i]!=1) ctr++;
  } else {
    for (unsigned int i=startingAxis;i<size;i++)
      if (data[i]!=1) ctr++;
  }
  if (ctr == size) return *this;

  AvWPosition nondegenerateIP(ctr,1);
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

void AvWPosition::resize(unsigned int newSize, int copy)
{
    DebugAssert(ok(), AipsError);

    // If the size is unchanged, just return (more efficient)
    if (newSize == size) {
	return;
    }

    double *newData = 0;
    if (newSize <= BufferLength) {
        newData = buffer_p;
    } else {
        newData = new double[newSize];
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

AvWPosition &AvWPosition::operator=(const AvWPosition &other)
{
    DebugAssert(ok(), AipsError);
    if (&other == this) {
	return *this;
    }
    if (size == 0) {
	this->resize(other.nelements());
    }
    if (conform(other) == 0) {
	cerr << "AvWPosition::operator=(const AvWPosition & - "
	  "this and other differ in length";
    }
    for (unsigned int i=0; i < size; i++) {
	data[i] = other.data[i];
    }
    return *this;
}

AvWPosition &AvWPosition::operator=(double value)
{
    DebugAssert(ok(), AipsError);
    for (unsigned int i=0; i < size; i++) {
	data[i] = value;
    }
    return *this;
}

void AvWPosition::append (const AvWPosition& other)
{
    unsigned int j = size;
    resize (size + other.size);
    for (unsigned int i=0; i<other.size; i++) {
	data[j++] = other.data[i];
    }
}

void AvWPosition::prepend (const AvWPosition& other)
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

AvWPosition AvWPosition::concatenate (const AvWPosition& other) const
{
    AvWPosition tmp (*this);
    tmp.append (other);
    return tmp;
}

void AvWPosition::setFirst (const AvWPosition& other)
{
    if (size < other.size) {
      cerr << "AvWPosition::setFirst(other); other is too long";
    }
    for (unsigned int i=0; i<other.size; i++) {
	data[i] = other.data[i];
    }
}

void AvWPosition::setLast (const AvWPosition& other)
{
    if (size < other.size) {
	cerr << "AvWPosition::setLast(other); other is too long";
    }
    unsigned int j = size - other.size;
    for (unsigned int i=0; i<other.size; i++) {
	data[j++] = other.data[i];
    }
}

AvWPosition AvWPosition::getFirst (unsigned int n) const
{
    if (size < n) {
	cerr << "AvWPosition::getFirst(n); n is too high";
    }
    AvWPosition tmp(n);
    for (unsigned int i=0; i<n; i++) {
	tmp.data[i] = data[i];
    }
    return tmp;
}

AvWPosition AvWPosition::getLast (unsigned int n) const
{
    if (size < n) {
	cerr << "AvWPosition::getLast(n); n is too high";
    }
    AvWPosition tmp(n);
    unsigned int j = size - n;
    for (unsigned int i=0; i<n; i++) {
	tmp.data[i] = data[j++];
    }
    return tmp;
}


void AvWPosition::operator += (const AvWPosition  &other)
{
    DebugAssert(ok(), AipsError);
    if (conform(other) == 0) {
	cerr << "AvWPosition::operator += "
	  "(const AvWPosition &) - "
	  "this and other differ in length";
    }
    for (unsigned int i=0; i < size; i++) {
	data[i] += other.data[i];
    }
}

void AvWPosition::operator -= (const AvWPosition  &other)
{
    DebugAssert(ok(), AipsError);
    if (conform(other) == 0) {
	cerr << "AvWPosition::operator -= "
	  "(const AvWPosition &) - "
	  "this and other differ in length";
    }
    for (unsigned int i=0; i < size; i++) {
	data[i] -= other.data[i];
    }
}

void AvWPosition::operator *= (const AvWPosition  &other)
{
    DebugAssert(ok(), AipsError);
    if (conform(other) == 0) {
	cerr << "AvWPosition::operator *= "
	  "(const AvWPosition &) - "
	  "this and other differ in length";
    }
    for (unsigned int i=0; i < size; i++) {
	data[i] *= other.data[i];
    }
}

void AvWPosition::operator /= (const AvWPosition  &other)
{
    DebugAssert(ok(), AipsError);
    if (conform(other) == 0) {
	cerr << "AvWPosition::operator /= "
	  "(const AvWPosition &) - "
	  "this and other differ in length";
    }
    for (unsigned int i=0; i < size; i++) {
	data[i] /= other.data[i];
    }
}

void AvWPosition::operator += (double val)
{
    DebugAssert(ok(), AipsError);
    for (unsigned int i=0; i < size; i++) {
	data[i] += val;
    }
}

void AvWPosition::operator -= (double val)
{
    DebugAssert(ok(), AipsError);
    for (unsigned int i=0; i < size; i++) {
	data[i] -= val;
    }
}

void AvWPosition::operator *= (double val)
{
    DebugAssert(ok(), AipsError);
    for (unsigned int i=0; i < size; i++) {
	data[i] *= val;
    }
}

void AvWPosition::operator /= (double val)
{
    DebugAssert(ok(), AipsError);
    for (unsigned int i=0; i < size; i++) {
	data[i] /= val;
    }
}

int AvWPosition::isEqual (const AvWPosition &other) const
{
    return isEqual (other, nelements());
}

int AvWPosition::isEqual (const AvWPosition &other, unsigned int nrCompare) const
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


AvWPosition operator + (const AvWPosition &left, const AvWPosition &right)
{
    if (left.conform(right) == 0) {
      cerr << "::operator + "
	"(const AvWPosition &, const AvWPosition &) - "
	"left and right operand do not conform ";
    }
    AvWPosition result(left);
    result += right;
    return result;
}

AvWPosition operator - (const AvWPosition &left, const AvWPosition &right)
{
    if (left.conform(right) == 0) {
      cerr << "::operator - "
	"(const AvWPosition &, const AvWPosition &) - "
	"left and right operand do not conform ";
    }
    AvWPosition result(left);
    result -= right;
    return result;
}

AvWPosition operator * (const AvWPosition &left, const AvWPosition &right)
{
    if (left.conform(right) == 0) {
	cerr << "::operator * "
	  "(const AvWPosition &, const AvWPosition &) - "
	  "left and right operand do not conform ";
    }
    AvWPosition result(left);
    result *= right;
    return result;
}

AvWPosition operator / (const AvWPosition &left, const AvWPosition &right)
{
    if (left.conform(right) == 0) {
	cerr <<"::operator / "
	  "(const AvWPosition &, const AvWPosition &) - "
	  "left and right operand do not conform ";
    }
    AvWPosition result(left);
    result /= right;
    return result;
}

AvWPosition operator + (const AvWPosition &left, double val)
{
    AvWPosition result(left);
    result += val;
    return result;
}

AvWPosition operator - (const AvWPosition &left, double val)
{
    AvWPosition result(left);
    result -= val;
    return result;
}

AvWPosition operator * (const AvWPosition &left, double val)
{
    AvWPosition result(left);
    result *= val;
    return result;
}

AvWPosition operator / (const AvWPosition &left, double val)
{
    AvWPosition result(left);
    result /= val;
    return result;
}

AvWPosition operator + (double val, const AvWPosition &right)
{
    AvWPosition result(right.nelements());
    result = val;
    result += right;
    return result;
}

AvWPosition operator - (double val, const AvWPosition &right)
{
    AvWPosition result(right.nelements());
    result = val;
    result -= right;
    return result;
}

AvWPosition operator * (double val, const AvWPosition &right)
{
    AvWPosition result(right.nelements());
    result = val;
    result *= right;
    return result;
}

AvWPosition operator / (double val, const AvWPosition &right)
{
    AvWPosition result(right.nelements());
    result = val;
    result /= right;
    return result;
}

double AvWPosition::product() const
{
    if (nelements() ==  0) {
	return 0;
    }
    double total = 1;
    for (unsigned int i=0; i<nelements(); i++) {
	total *= (*this)(i);
    }
    return total;
}

int operator == (const AvWPosition &left, const AvWPosition &right)
{
    if (left.conform(right) == 0) {
      cerr << "::operator== "
	"(const AvWPosition &, const AvWPosition &) - "
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

int operator != (const AvWPosition &left, const AvWPosition &right)
{
    if (left.conform(right) == 0) {
      cerr << "::operator!= "
	"(const AvWPosition &, const AvWPosition &) - "
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

int operator < (const AvWPosition &left, const AvWPosition &right)
{
    if (left.conform(right) == 0) {
      cerr << "::operator< "
	"(const AvWPosition &, const AvWPosition &) - "
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

int operator <= (const AvWPosition &left, const AvWPosition &right)
{
    if (left.conform(right) == 0) {
      cerr << "::operator<= "
	"(const AvWPosition &, const AvWPosition &) - "
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

int operator > (const AvWPosition &left, const AvWPosition &right)
{
    if (left.conform(right) == 0) {
      cerr << "::operator> "
	"(const AvWPosition &, const AvWPosition &) - "
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

int operator >= (const AvWPosition &left, const AvWPosition &right)
{
    if (left.conform(right) == 0) {
	cerr << "::operator>= "
	  "(const AvWPosition &, const AvWPosition &) - "
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

int operator == (const AvWPosition &left, double val)
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

int operator != (const AvWPosition &left, double val)
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

int operator < (const AvWPosition &left, double val)
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

int operator <= (const AvWPosition &left, double val)
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

int operator > (const AvWPosition &left, double val)
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

int operator >= (const AvWPosition &left, double val)
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

int operator == (double val, const AvWPosition &right)
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

int operator != (double val, const AvWPosition &right)
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

int operator < (double val, const AvWPosition &right)
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

int operator <= (double val, const AvWPosition &right)
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

int operator > (double val, const AvWPosition &right)
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

int operator >= (double val, const AvWPosition &right)
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

ostream &operator<<(ostream &os, const AvWPosition &ip)
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

int AvWPosition::ok() const
{
    int retval = 1;
    if (size <= BufferLength && data != &buffer_p[0]) { retval = 0; }
    if (data == 0) { retval = 0; }
    return retval;
}

