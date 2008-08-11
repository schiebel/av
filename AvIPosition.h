//# AvIPosition.h: A vector of integers, used to index into arrays.
//# Copyright (C) 1994,1995,1996,1999,2000,2001
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
//# $Id: AvIPosition.h,v 19.0 2003/07/16 05:47:28 aips2adm Exp $
//# --------------------------------------------------
//
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvIPosition.h,v 19.0 2003/07/16 05:47:28 aips2adm Exp $
//
// $Log: AvIPosition.h,v $
// Revision 19.0  2003/07/16 05:47:28  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:08  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:37  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:44  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.2  2001/03/15 21:39:14  wyoung
// Updated copyright
//
// Revision 15.1  2000/12/17 22:09:13  wbrouw
// Removed a comment with 'imported class'
//
// Revision 15.0  2000/10/26 17:11:10  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:33  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:21  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:17  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.1  1999/05/27 08:26:41  wbrouw
// Removed ifdef _AIX dependency
//
// Revision 11.0  1998/10/03 07:00:38  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:38  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:28:56  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.1  1997/05/20 20:03:11  hr
// Added endl to lines with cerr.
//
// Revision 8.0  1997/02/20 03:17:35  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1996/12/12 08:55:18  droberts
// Final update from monet archive.
//
// Revision 1.3  1996/11/18 20:40:12  hr
// Added RCS header.
//
//
//---------------------------------------------------------------------------

#if !defined (AvIPosition_h)
#define AvIPosition_h
#include <iostream.h>


//# Forward declaration for functions which convert an AvIPosition to and from
//# 1-D arrays.
//template<class T> class Array;
//template<class T> class Vector;

// <category lib=aips sect="Array">
// <summary> A Vector of integers, for indexing into Array<T> objects </summary>

// AvIPosition is "logically" a Vector<int> constrained so that it's origin
// is zero-based, and in fact that used to be the way it was implemented.
// It was split out into a separate class to make the inheritance from
// Arrays simpler (since Arrays use IPositions). The
// template instantiation mechanism is complicated enough that this
// simplification was felt to be a good idea.
//
// AvIPosition objects are normally used to index into, and define the shapes
// of, multi-dimensional arrays. For example, if you have a 5 dimensional
// array, you need an AvIPosition of length 5 to index into the array (or
// to define its shape, etc.).
//
// Unlike Vectors, IPositions always use copy semantics.
// <code>
// AvIPosition ip1(5);                         // An AvIPosition of length 5
// ip1(0) = 11; ip1(1) = 5; ... ip1(4) = 6;  // Indices 0-based
// AvIPosition ip2(ip1);                       // Copy constructor; a COPY
// </code>
//
// Binary operations must take place either with a conformnat (same size)
// AvIPosition or with an integer, which behaves as if it was an AvIPosition
// of the same size (i.e., length). All the usual binary arithmetic
// operations are available, as well as logical operations, which return
// Booleans. These all operate "element-by-element".
//
// All non-inlined member functions of AvIPosition check invariants if the
// preprocessor symbol AIPS_DEBUG is defined.
// That is, the member functions check that ok() is true (constructors
// check after construction, other functions on entry to the function).
// If these tests fail, an AipsError exception is thrown; its message
// contains the line number and source file of the failure (it is thrown
// by the lAssert macro defined in aips/Assert.h).
//
// Example:
// <code>
// AvIPosition blc(5), trc(5,1,2,3,4,5);
// blc = 0;            // OR AvIPosition blc(5,0);
// //...
// if (blc > trc) {
//    AvIPosition tmp;
//    tmp = trc;       // Swap
//    trc = blc;
//    blc = tmp;
// }
// //...
// trc += 5;           // make the box 5 larger in all dimensions
// </code>

class AvWPosition;
class AvIPosition
{
public:
  enum {MIN_INT = -2147483647};
  // A zero-length AvIPosition.
  AvIPosition();

    // An AvIPosition of size "length." The values in the object are undefined.
  AvIPosition(unsigned int length);

  // An AvIPosition of size "length." The values in the object get
  // initialized to val.
  AvIPosition(unsigned int length, int val);

#if 1
  // Build from AvWPosition
  AvIPosition(const AvWPosition & wp);
#endif
    // An AvIPosition of size "length" with defined values. You need to supply
    // a value for each element of the AvIPosition (up to 10). [Unfortunately
    // varargs might not be sufficiently portable.]
  AvIPosition (unsigned int length, int val0, int val1, int val2=MIN_INT, 
	     int val3=MIN_INT, int val4=MIN_INT, int val5=MIN_INT,
	     int val6=MIN_INT, int val7=MIN_INT, int val8=MIN_INT, 
	     int val9=MIN_INT);

  // Makes a copy (copy, NOT reference, semantics) of other.
  AvIPosition(const AvIPosition &other);
    
  ~AvIPosition();

    // Makes this a copy of other. "this" and "other" must either be conformant
    // (same size) or this must be 0-length, in which case it will
    // resize itself to be the same length as other.
  AvIPosition &operator=(const AvIPosition &other);

  // Copy "value" into every position of this AvIPosition.
  AvIPosition &operator=(int value);

  // Convert an AvIPosition to and from an Array<int>. In either case, the
  // array must be one dimensional.
  //+grp
  //AvIPosition(const Array<int> &other);
  //Vector<int> asVector() const;
    //-grp

    // This member function returns an AvIPosition reference which has all 
    // degenerate (length==1) axes removed and the dimensionality reduced 
    // appropriately.
    // Only axes greater than startingAxis are considered (normally one 
    // wants to remove trailing axes.
    AvIPosition nonDegenerate(unsigned int startingAxis=0) const;

    // Old values are copied on resize if copy==1..
    // If the size increases, values beyond the former size are undefined.
    void resize(unsigned int newSize, int copy=1);

    // Index into the AvIPosition. Indices are zero-based. If the preprocessor
    // symbol AIPS_ARRAY_INDEX_CHECK is defined, "index" will be
    // checked to ensure it is not out of bounds. If this check fails, an
    // AipsError will be thrown.
    //+grp
    int &operator()(unsigned int index);
    int operator()(unsigned int index) const;
    //-grp

    // Append this AvIPosition with another one (causing a resize).
    void append (const AvIPosition& other);

    // Prepend this AvIPosition with another one (causing a resize).
    void prepend (const AvIPosition& other);

    // Return an AvIPosition as the concetanation of this and another AvIPosition.
    AvIPosition concatenate (const AvIPosition& other) const;

    // Set the first values of this AvIPosition to another AvIPosition.
    // An exception is thrown if the other AvIPosition is too long.
    void setFirst (const AvIPosition& other);

    // Set the last values of this AvIPosition to another AvIPosition.
    // An exception is thrown if the other AvIPosition is too long.
    void setLast (const AvIPosition& other);

    // Construct an AvIPosition from the first <src>n</src> values of this
    // AvIPosition.
    // An exception is thrown if <src>n</src> is too high.
    AvIPosition getFirst (unsigned int n) const;

    // Construct an AvIPosition from the last <src>n</src> values of this
    // AvIPosition.
    // An exception is thrown if <src>n</src> is too high.
    AvIPosition getLast (unsigned int n) const;

    // The number of elements in this AvIPosition. Since AvIPosition
    // objects use zero-based indexing, the maximum available index is
    // nelements() - 1.
    unsigned int nelements() const;

    // conform returns true if nelements() == other.nelements().
    int conform(const AvIPosition &other) const;

    // Element-by-element arithmetic.
    //+grp
    void operator += (const AvIPosition  &other);
    void operator -= (const AvIPosition  &other);
    void operator *= (const AvIPosition  &other);
    void operator /= (const AvIPosition  &other);
    void operator += (int val);
    void operator -= (int val);
    void operator *= (int val);
    void operator /= (int val);
    //-grp

    // Returns 0 if nelements() == 0, otherwise it returns the product of
    // its elements.
    int product() const;
  
  // Returns 0 if nelements() == 0, otherwise it returns the sum of
  // its elements.
  int sum() const;

    // Element-by-element comparison for equality.
    // It returns 1 if the lengths and all elements are equal.
    // The difference between this and operator==() is that if the two
    // IPositions do not conform, this function returns 0, instead of
    // throwing an exception as operator==() does.
    int isEqual (const AvIPosition& other) const;

    // Element-by-element comparison for (partial) equality.
    // It returns true if the lengths and the first <src>nrCompare</src>
    // elements are equal.
    int isEqual (const AvIPosition& other, unsigned int nrCompare) const;

    // Write an AvIPosition to an ostream in a simple text form.
    friend ostream &operator<<(ostream &os, const AvIPosition &ip);

    // Write an AvIPosition to an AipsIO stream in a binary format.
  // friend AipsIO &operator<<(AipsIO &aio, const AvIPosition &ip);

    // Read an AvIPosition from an AipsIO stream in a binary format.
    // Will throw an AipsError if the current AvIPosition Version does not match
    // that of the one on disk.
  // friend AipsIO &operator>>(AipsIO &aio, AvIPosition &ip);

    // Is this AvIPosition consistent?
    int ok() const;
private:
    enum { IPositionVersion = 1, BufferLength = 4 };
    unsigned int size;
    int buffer_p[BufferLength];
    // When the iposition is length BufferSize or less data is just buffer_p,
    // avoiding calls to new and delete.
    int *data;
};

// <category sect="Array">
// Element by element arithmetic.
//+grp
AvIPosition operator + (const AvIPosition &left, const AvIPosition &right);
AvIPosition operator - (const AvIPosition &left, const AvIPosition &right);
AvIPosition operator * (const AvIPosition &left, const AvIPosition &right);
AvIPosition operator / (const AvIPosition &left, const AvIPosition &right);

AvIPosition operator + (const AvIPosition &left, int val);
AvIPosition operator - (const AvIPosition &left, int val);
AvIPosition operator * (const AvIPosition &left, int val);
AvIPosition operator / (const AvIPosition &left, int val);

AvIPosition operator + (int val, const AvIPosition &right);
AvIPosition operator - (int val, const AvIPosition &right);
AvIPosition operator * (int val, const AvIPosition &right);
AvIPosition operator / (int val, const AvIPosition &right);
//-grp

// <category sect="Array">
// Element by element boolean operations. The result is true only
// if the operation yields true for every element of the AvIPosition.
//+grp
int operator == (const AvIPosition &left, const AvIPosition &right);
int operator != (const AvIPosition &left, const AvIPosition &right);
int operator <  (const AvIPosition &left, const AvIPosition &right);
int operator <= (const AvIPosition &left, const AvIPosition &right);
int operator >  (const AvIPosition &left, const AvIPosition &right);
int operator >= (const AvIPosition &left, const AvIPosition &right);

int operator == (const AvIPosition &left, int val);
int operator != (const AvIPosition &left, int val);
int operator <  (const AvIPosition &left, int val);
int operator <= (const AvIPosition &left, int val);
int operator >  (const AvIPosition &left, int val);
int operator >= (const AvIPosition &left, int val);

int operator == (int val, const AvIPosition &right);
int operator != (int val, const AvIPosition &right);
int operator <  (int val, const AvIPosition &right);
int operator <= (int val, const AvIPosition &right);
int operator >  (int val, const AvIPosition &right);
int operator >= (int val, const AvIPosition &right);
//-grp

    
//# ------- In-line member functions ------

inline unsigned int AvIPosition::nelements() const
{
    return size;
}

inline int &AvIPosition::operator()(unsigned int index)
{
#if defined(AIPS_ARRAY_INDEX_CHECK)
    if (index > nelements() - 1) {
        // This should be a IndexError<unsigned int> - but that causes multiply
	// defined symbols with the current objectcenter.
        cerr << "AvIPosition::operator() - index error" << endl;
    }
#endif
    return data[index];
}

inline int AvIPosition::operator()(unsigned int index) const
{
#if defined(AIPS_ARRAY_INDEX_CHECK)
    if (index > nelements() - 1) {
        // This should be a IndexError<unsigned int> - but that causes multiply
	// defined symbols with the current objectcenter.
        cerr << "AvIPosition::operator() - index error" << endl;
    }
#endif
    return data[index];
}

inline int AvIPosition::conform(const AvIPosition &other) const
{
    int result = 0;
    if (this->size == other.size) {
	result = 1;
    }
    return result;
}


AvIPosition toAvIPositionInArray (const unsigned int offset,
				  const AvIPosition &shape, 
				  const AvIPosition &origin);

// This requires that type T have shape and origin members which return
// IPositions.
template <class T>
inline
AvIPosition toAvIPositionInArray (const unsigned int offset, const T &array)
{
    return toAvIPositionInArray (offset, array.shape(), array.origin());
}

//-grp


// Convert from Iposition to offset in an array.
//
//+grp

unsigned int toOffsetInArray (const AvIPosition &iposition,
                      const AvIPosition &shape, const AvIPosition &origin);

// This requires that type T have shape and origin members which return
// IPositions.
template <class T>
inline
unsigned int toOffsetInArray (const AvIPosition &iposition, const T &array)
{
    return toOffsetInArray (iposition, array.shape(), array.origin());
}

int isInsideArray (const unsigned int offset,
                    const AvIPosition &shape, const AvIPosition &origin);


int isInsideArray (const AvIPosition &iposition,
                    const AvIPosition &shape, const AvIPosition &origin);

#endif

