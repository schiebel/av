//# AvWPosition.cc: A vector of doubles, used for world coordinates in aipsview.
//# Copyright (C) 1994,1995,1996,1999,2000
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
//# $Id: AvWPosition.h,v 19.0 2003/07/16 05:47:31 aips2adm Exp $
//# --------------------------------------------------
//
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvWPosition.h,v 19.0 2003/07/16 05:47:31 aips2adm Exp $
//
// $Log: AvWPosition.h,v $
// Revision 19.0  2003/07/16 05:47:31  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:10  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:40  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:46  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.1  2000/12/17 22:10:16  wbrouw
// Removed an 'imported class' comment
//
// Revision 15.0  2000/10/26 17:11:20  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:36  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:24  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:23  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.1  1999/05/27 08:28:26  wbrouw
// Removed ifdef _AIX dependency
//
// Revision 11.0  1998/10/03 07:00:47  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:47  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1997/12/17 21:52:22  hr
// Changes to remove warnings from MIPSpro.7x compiler.
//
// Revision 9.0  1997/08/25 21:29:09  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.1  1997/05/20 20:03:41  hr
// Added endl to lines with cerr.
//
// Revision 8.0  1997/02/20 03:15:53  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 09:03:28  droberts
// Added RCS header.
//
// Revision 1.3  1996/11/18 20:40:12  hr
// Added RCS header.
//
//
//---------------------------------------------------------------------------

#if !defined (AvWPOSITION_H)
#define AvWPOSITION_H


//# Forward declaration for functions which convert an AvWPosition to and from
//# 1-D arrays.
//template<class T> class Array;
//template<class T> class Vector;

// <category lib=aips sect="Array">
// <summary> A Vector of integers, for indexing into Array<T> objects </summary>

// AvWPosition is "logically" a Vector<int> constrained so that it's origin
// is zero-based, and in fact that used to be the way it was implemented.
// It was split out into a separate class to make the inheritance from
// Arrays simpler (since Arrays use IPositions). The
// template instantiation mechanism is complicated enough that this
// simplification was felt to be a good idea.
//
// AvWPosition objects are normally used to index into, and define the shapes
// of, multi-dimensional arrays. For example, if you have a 5 dimensional
// array, you need an AvWPosition of length 5 to index into the array (or
// to define its shape, etc.).
//
// Unlike Vectors, IPositions always use copy semantics.
// <code>
// AvWPosition ip1(5);                         // An AvWPosition of length 5
// ip1(0) = 11; ip1(1) = 5; ... ip1(4) = 6;  // Indices 0-based
// AvWPosition ip2(ip1);                       // Copy constructor; a COPY
// </code>
//
// Binary operations must take place either with a conformnat (same size)
// AvWPosition or with an integer, which behaves as if it was an AvWPosition
// of the same size (i.e., length). All the usual binary arithmetic
// operations are available, as well as logical operations, which return
// Booleans. These all operate "element-by-element".
//
// All non-inlined member functions of AvWPosition check invariants if the
// preprocessor symbol AIPS_DEBUG is defined.
// That is, the member functions check that ok() is true (constructors
// check after construction, other functions on entry to the function).
// If these tests fail, an AipsError exception is thrown; its message
// contains the line number and source file of the failure (it is thrown
// by the lAssert macro defined in aips/Assert.h).
//
// Example:
// <code>
// AvWPosition blc(5), trc(5,1,2,3,4,5);
// blc = 0;            // OR AvWPosition blc(5,0);
// //...
// if (blc > trc) {
//    AvWPosition tmp;
//    tmp = trc;       // Swap
//    trc = blc;
//    blc = tmp;
// }
// //...
// trc += 5;           // make the box 5 larger in all dimensions
// </code>

class AvIPosition;
class AvWPosition
{
public:

  static const double MIN_DOUBLE;
  
  // A zero-length AvWPosition.
  AvWPosition();
  
  // An AvWPosition of size "length." The values in the object are undefined.
  AvWPosition(unsigned int length);

  // An AvWPosition of size "length." The values in the object get
  // initialized to val.
  AvWPosition(unsigned int length, double val);

  // Make a AvWPosition from a AvIPosition
  AvWPosition(const AvIPosition & kp);

    // An AvWPosition of size "length" with defined values. You need to supply
    // a value for each element of the AvWPosition (up to 10). [Unfortunately
    // varargs might not be sufficiently portable.]
  AvWPosition (unsigned int length, double val0, double val1, double val2=MIN_DOUBLE, 
	     double val3=MIN_DOUBLE, double val4=MIN_DOUBLE, double val5=MIN_DOUBLE,
	     double val6=MIN_DOUBLE, double val7=MIN_DOUBLE, double val8=MIN_DOUBLE, 
	     double val9=MIN_DOUBLE);

  // Makes a copy (copy, NOT reference, semantics) of other.
  AvWPosition(const AvWPosition &other);
    
  ~AvWPosition();

    // Makes this a copy of other. "this" and "other" must either be conformant
    // (same size) or this must be 0-length, in which case it will
    // resize itself to be the same length as other.
  AvWPosition &operator=(const AvWPosition &other);

  // Copy "value" double to every position of this AvWPosition.
  AvWPosition &operator=(double value);

  // Convert a AvWPosition to and from an Array<double>. In either case, the
  // array must be one dimensional.
  //+grp
  //AvWPosition(const Array<double> &other);
  //Vector<double> asVector() const;
    //-grp

    // This member function returns a AvWPosition reference which has all 
    // degenerate (length==1) axes removed and the dimensionality reduced 
    // appropriately.
    // Only axes greater than startingAxis are considered (normally one 
    // wants to remove trailing axes.
    AvWPosition nonDegenerate(unsigned int startingAxis=0) const;

    // Old values are copied on resize if copy==1..
    // If the size increases, values beyond the former size are undefined.
    void resize(unsigned int newSize, int copy=1);

    // Index into the AvWPosition. Indices are zero-based. If the preprocessor
    // symbol AIPS_ARRAY_INDEX_CHECK is defined, "index" will be
    // checked to ensure it is not out of bounds. If this check fails, an
    // AipsError will be thrown.
    //+grp
    double &operator()(unsigned int index);
    double operator()(unsigned int index) const;
    //-grp

    // Append this AvWPosition with another one (causing a resize).
    void append (const AvWPosition& other);

    // Prepend this AvWPosition with another one (causing a resize).
    void prepend (const AvWPosition& other);

    // Return an AvWPosition as the concatanation of this and another AvWPosition.
    AvWPosition concatenate (const AvWPosition& other) const;

    // Set the first values of this AvWPosition to another AvWPosition.
    // An exception is thrown if the other AvWPosition is too long.
    void setFirst (const AvWPosition& other);

    // Set the last values of this AvWPosition to another AvWPosition.
    // An exception is thrown if the other AvWPosition is too long.
    void setLast (const AvWPosition& other);

    // Construct an AvWPosition from the first <src>n</src> values of this
    // AvWPosition.
    // An exception is thrown if <src>n</src> is too high.
    AvWPosition getFirst (unsigned int n) const;

    // Construct an AvWPosition from the last <src>n</src> values of this
    // AvWPosition.
    // An exception is thrown if <src>n</src> is too high.
    AvWPosition getLast (unsigned int n) const;

    // The number of elements in this AvWPosition. Since AvWPosition
    // objects use zero-based indexing, the maximum available index is
    // nelements() - 1.
    unsigned int nelements() const;

    // conform returns true if nelements() == other.nelements().
    int conform(const AvWPosition &other) const;

    // Element-by-element arithmetic.
    //+grp
    void operator += (const AvWPosition  &other);
    void operator -= (const AvWPosition  &other);
    void operator *= (const AvWPosition  &other);
    void operator /= (const AvWPosition  &other);
    void operator += (double val);
    void operator -= (double val);
    void operator *= (double val);
    void operator /= (double val);
    //-grp

    // Returns 0 if nelements() == 0, otherwise it returns the product of
    // its elements.
    double product() const;

    // Element-by-element comparison for equality.
    // It returns 1 if the lengths and all elements are equal.
    // The difference between this and operator==() is that if the two
    // IPositions do not conform, this function returns 0, instead of
    // throwing an exception as operator==() does.
    int isEqual (const AvWPosition& other) const;

    // Element-by-element comparison for (partial) equality.
    // It returns true if the lengths and the first <src>nrCompare</src>
    // elements are equal.
    int isEqual (const AvWPosition& other, unsigned int nrCompare) const;

    // Write an AvWPosition to an ostream in a simple text form.
    friend ostream &operator<<(ostream &os, const AvWPosition &ip);

    // Write an AvWPosition to an AipsIO stream in a binary format.
  // friend AipsIO &operator<<(AipsIO &aio, const AvWPosition &ip);

    // Read an AvWPosition from an AipsIO stream in a binary format.
    // Will throw an AipsError if the current AvWPosition Version does not match
    // that of the one on disk.
  // friend AipsIO &operator>>(AipsIO &aio, AvWPosition &ip);

    // Is this AvWPosition consistent?
    int ok() const;
    double *operator[](unsigned int index);
private:
    enum { WPositionVersion = 1, BufferLength = 4 };
    unsigned int size;
    double buffer_p[BufferLength];
    // When the iposition is length BufferSize or less data is just buffer_p,
    // avoiding calls to new and delete.
    double *data;
};

// <category sect="Array">
// Element by element arithmetic.
//+grp
AvWPosition operator + (const AvWPosition &left, const AvWPosition &right);
AvWPosition operator - (const AvWPosition &left, const AvWPosition &right);
AvWPosition operator * (const AvWPosition &left, const AvWPosition &right);
AvWPosition operator / (const AvWPosition &left, const AvWPosition &right);

AvWPosition operator + (const AvWPosition &left, double val);
AvWPosition operator - (const AvWPosition &left, double val);
AvWPosition operator * (const AvWPosition &left, double val);
AvWPosition operator / (const AvWPosition &left, double val);

AvWPosition operator + (double val, const AvWPosition &right);
AvWPosition operator - (double val, const AvWPosition &right);
AvWPosition operator * (double val, const AvWPosition &right);
AvWPosition operator / (double val, const AvWPosition &right);
//-grp

// <category sect="Array">
// Element by element boolean operations. The result is true only
// if the operation yields true for every element of the AvWPosition.
//+grp
int operator == (const AvWPosition &left, const AvWPosition &right);
int operator != (const AvWPosition &left, const AvWPosition &right);
int operator <  (const AvWPosition &left, const AvWPosition &right);
int operator <= (const AvWPosition &left, const AvWPosition &right);
int operator >  (const AvWPosition &left, const AvWPosition &right);
int operator >= (const AvWPosition &left, const AvWPosition &right);

int operator == (const AvWPosition &left, double val);
int operator != (const AvWPosition &left, double val);
int operator <  (const AvWPosition &left, double val);
int operator <= (const AvWPosition &left, double val);
int operator >  (const AvWPosition &left, double val);
int operator >= (const AvWPosition &left, double val);

int operator == (double val, const AvWPosition &right);
int operator != (double val, const AvWPosition &right);
int operator <  (double val, const AvWPosition &right);
int operator <= (double val, const AvWPosition &right);
int operator >  (double val, const AvWPosition &right);
int operator >= (double val, const AvWPosition &right);
//-grp

    
//# ------- In-line member functions ------

inline unsigned int AvWPosition::nelements() const
{
    return size;
}

inline double *AvWPosition::operator[](unsigned int index)
{
#if defined(AIPS_ARRAY_INDEX_CHECK)
    if (index > nelements() - 1) {
        // This should be a IndexError<unsigned int> - but that causes multiply
	// defined symbols with the current objectcenter.
        cerr << "AvWPosition::operator() - index error" << endl;
    }
#endif
    return data+index;
}

inline double &AvWPosition::operator()(unsigned int index)
{
#if defined(AIPS_ARRAY_INDEX_CHECK)
    if (index > nelements() - 1) {
        // This should be a IndexError<unsigned int> - but that causes multiply
	// defined symbols with the current objectcenter.
        cerr << "AvWPosition::operator() - index error" << endl;
    }
#endif
    return data[index];
}

inline double AvWPosition::operator()(unsigned int index) const
{
#if defined(AIPS_ARRAY_INDEX_CHECK)
    if (index > nelements() - 1) {
        // This should be a IndexError<unsigned int> - but that causes multiply
	// defined symbols with the current objectcenter.
        cerr << "AvWPosition::operator() - index error" << endl;
    }
#endif
    return data[index];
}

inline int AvWPosition::conform(const AvWPosition &other) const
{
    int result = 0;
    if (this->size == other.size) {
	result = 1;
    }
    return result;
}


unsigned int toOffsetInArray (const AvWPosition &iposition,
                      const AvWPosition &shape, const AvWPosition &origin);


#endif
