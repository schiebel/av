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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvPointList2D.h,v 19.0 2003/07/16 05:47:23 aips2adm Exp $
//
// $Log: AvPointList2D.h,v $
// Revision 19.0  2003/07/16 05:47:23  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:04  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:33  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:40  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:54  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:27  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:16  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:07  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:26  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:23  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:28:32  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:17:26  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:54:47  droberts
// Final update from monet archive.
//
// Revision 1.3  1996/08/19 16:42:29  pixton
//  Cleaned up g++ -Wall warnings.
//
// Revision 1.2  1996/08/14  19:08:27  hr
// Changes to remove g++ warnings (from bglenden).
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.5  1996/06/18  18:43:13  pixton
//  Copyright Update
//
//  Revision 1.4  1995/11/13  22:25:31  pixton
//  added grow.
//
//  Revision 1.3  1995/10/06  15:10:45  pixton
//  Added grow() function and modified setValue() and add() to
//  grow the array to satisfy the request.
//
//  Revision 1.2  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

//
//
//  AvPointList2D
//
//  Maintains a set of float x,y values
//
//  access is by 0-based indexing and adjusting the active value
//

#ifndef _AvPointList2D_h_
#define _AvPointList2D_h_

#include <stdio.h>

#define SVPL2D_GROWVAL 32

// Consider Rep Change to single array

class AvPointList2D
{
public:
  
  AvPointList2D(int s = 20) 
    : active_(0), maxSize_(s), size_(0), lock_(0)
    { 
      xArray_ = new float[maxSize_];
      yArray_ = new float[maxSize_];
    }

  ~AvPointList2D()
    {
      delete [] xArray_;
      delete [] yArray_;
    }

  AvPointList2D& operator = (const AvPointList2D& pl);

  void dump() const;

  int firstX() const { return int(xArray_[0]); }
  int lastX() const { return int(xArray_[size_-1]); }

  int empty() const { return (size_ == 0); }
  int full() const { return (size_ == maxSize_); }
  void clear() { size_ = 0; }

  void grow(int growVal = SVPL2D_GROWVAL);

  //-----add point to list
  void add (float x, float y);
  void remove (int ndx);
  void deleteBetweenXRange(float x1, float x2, int * saveNdx);

  //-----set/retrieve values
  void getValue(int ndx, float * x, float * y);
  void setValue(int ndx, float x, float y);

  //-----Returns index of point nearest the x,y position
  int nearestNdx(float x, float y) const;
  
  //-----Returns index of point nearest in X to the x passed in
  int nearestXNdx(float x) const;

  int find (float x, float y) const;

  //-----2nd Order Function Application
  // mapcar:     Apply f to each point in the list
  // mapcarPair: Apply f to each pair of points in the list
  void mapcar(void (*f)(float x, float y));
  void mapcarPair(void (*f)(float x1, float y1, float x2, float y2));

  void xSort();

  //-----Read Access
  int active()     const { return active_; }
  int size()       const { return size_;   }
  int length()     const { return size_;   }
  float * xArray() const { return xArray_; }
  float * yArray() const { return yArray_; }

  float minX() const;
  float maxX() const;
  float minY() const;
  float maxY() const;

  //-----Write Access
  void active(int ndx) { if (ndx < size_) active_ = size_; }
  
  //-----add/delete prevention
  int lock() const { return lock_; }
  void lock(int l) { lock_ = l;    }

  //-----Size control
  int maxSize() const { return maxSize_; }

  //-----Scale x values
  void scaleX(float factor);

private:
  
  int active_;        // currently active point
  int maxSize_;       // max number of pairs array can hold
  int size_;          // current number of pairs
  float * xArray_;    // storage for x
  float * yArray_;    // storage for y
  int lock_;          // 1 ==> can't add or delete points
};

int Av2DCPLxSortFunc(const void * av, const void * bv);

#endif
