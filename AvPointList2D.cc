//# Copyright (C) 1995-2000 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvPointList2D.cc,v 19.0 2003/07/16 05:47:51 aips2adm Exp $
//
// $Log: AvPointList2D.cc,v $
// Revision 19.0  2003/07/16 05:47:51  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:27  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:55  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:02  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:20  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:37:13  hravlin
// Needed an 'extern "C"' around a function.
//
// Revision 14.0  2000/03/23 16:08:57  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:46  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:00  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:42  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:51  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:30:48  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:18:52  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:54:11  droberts
// Final update from monet archive.
//
// Revision 1.2  1996/08/14 19:07:27  hr
// Changes to remove g++ warnings (from bglenden).
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.8  1996/06/18  18:46:26  pixton
//  Copyright Update
//
//  Revision 1.7  1995/11/13  22:16:31  pixton
//  resize, grow added and tested.
//
//  Revision 1.6  1995/10/06  15:09:48  pixton
//  Added grow() function and modified setValue() and add() to
//  grow the array to satisfy the request.
//
//  Revision 1.5  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/07/19  14:27:20  pixton
//  Oops.  Missed a couple.
//
//  Revision 1.3  1995/07/19  14:13:03  pixton
//  Removed dependence on min and maximum float constants
//
//  Revision 1.2  1995/07/18  17:27:17  hr
//  'Fixed' g++ warnings. Mostly don't do "for(int i...)".
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

//
//
//  Av2DControlPointList.cc
//
//

#ifndef MAX
#define MAX(A,B)  (((A) < (B)) ? (A) : (B))
#endif

#include <stdlib.h>
#include <string.h>
#include <AvPointList2D.h>

AvPointList2D& AvPointList2D::operator =
(const AvPointList2D& pl)
{
  active_ = pl.active_;
  maxSize_ = pl.maxSize_;
  size_ = pl.size_;
  memcpy(xArray_, pl.xArray_, size_*sizeof(float));
  memcpy(yArray_, pl.yArray_, size_*sizeof(float));
  return *this;
}

extern "C" {
static int xSortFunc(const void * av, const void * bv)
{
  register float a = *((float *) av);
  register float b = *((float *) bv);
  return ((a < b) ? -1 : (a > b) ? 1 : 0);
}
}

// Ascending sort on x
void AvPointList2D::xSort()
{
  float * sortArea = new float[2*size_];
  float * p = sortArea;
  int i;
  for (i = 0; i < size_; i++)
    {
      *p++ = xArray_[i];
      *p++ = yArray_[i];
    }
  qsort(sortArea, size_, 2*sizeof(float), &xSortFunc);
  for (p = sortArea, i = 0; i < size_; i++)
    {
      xArray_[i] = *p++;
      yArray_[i] = *p++;
    }
 
  delete [] sortArea;
}

float AvPointList2D::minX() const
{
  float ret = xArray_[0];
  for (int i = 1; i < size_; i++) 
    {
      float p = xArray_[i];
      if (p < ret) ret = p;
    }
  return ret;
}

void AvPointList2D::dump() const
{
  for (int i = 0; i < size_; i++)
    {
      printf("<%f,%f>\n", xArray_[i], yArray_[i]);
    }
}

float AvPointList2D::maxX() const
{
  float ret = xArray_[0];
  for (int i = 1; i < size_; i++) 
    {
      float p = xArray_[i];
      if (p > ret) ret = p;
    }
  return ret;
}

void AvPointList2D::mapcar(void (*f)(float x, float y))
{
  for (int i = 0; i < size_; i++) f(xArray_[i], yArray_[i]);
}

void AvPointList2D::mapcarPair(void (*f)(float x1, float y1, 
						float x2, float y2))
{
  for (int i = 1; i < size_; i++) 
    f(xArray_[i-1], yArray_[i-1], xArray_[i], yArray_[i]);
}

// PRE:  contains at least one point
int AvPointList2D::nearestNdx(float x, float y) const
{
  int retval = -1;
  float minD2 = 1000000;
  for (int i = 0; i < size_; i++)
    {
      float d2 = (xArray_[i] - x)*(xArray_[i] - x) + 
	(yArray_[i] - y)*(yArray_[i] - y);
      if (d2 < minD2) { minD2 = d2; retval = i; }
    }
  return retval;
}

int AvPointList2D::nearestXNdx(float x) const
{
  int retval = -1;
  float minD = 1000000;
  for (int i = 0; i < size_; i++)
    {
      float d = abs(int(x - xArray_[i]));
      if (d < minD) { minD = d; retval = i; }
    }
  return retval;
}

int AvPointList2D::find(float x, float y) const
{
  int retval = -1;
  for (int i = 0; i < size_; i++)
    {
      if (x == xArray_[i] && y == yArray_[i])
	{ retval = i; break; }
    }
  return retval;
}

void AvPointList2D::deleteBetweenXRange(float x1, float x2, int *saveNdx)
{
  if (lock_) 
    {
      float xs, ys;
      getValue(*saveNdx, &xs, &ys);
      xSort();
      *saveNdx = find(xs, ys);
    }
  else
    {
      if (x1 > x2) { float t = x1; x1 = x2; x2 = t; }
      for (int i = 0; i < length(); i++)
	{
	  if (*saveNdx == i) continue;
	  if (x1 <= xArray_[i] && xArray_[i] <= x2)
	    { 
	      if (*saveNdx > i) (*saveNdx)--;
	      remove(i); 
	      i--;
	    }
	}
    }
}

void AvPointList2D::grow(int growVal)
{
  // Get new array info
  int newMaxSize = maxSize_ + growVal;
  float * newXArray = new float[newMaxSize];
  float * newYArray = new float[newMaxSize];

  // Copy old data
  memcpy(newXArray, xArray_, size_*sizeof(float));
  memcpy(newYArray, yArray_, size_*sizeof(float));
  
  // Adjust values
  maxSize_ = newMaxSize;
  
  // delete old arrays
  delete [] xArray_;
  delete [] yArray_;
  xArray_ = newXArray;
  yArray_ = newYArray;
}

void AvPointList2D::add(float x, float y)
{ 
  if (lock_) return;
  
  if (full()) grow();
  
  xArray_[size_] = x; 
  yArray_[size_] = y; 
  size_++; 
}

void AvPointList2D::remove(int ndx)
{
  if (lock_) return;
  if (ndx < size_-1)
    {
#if 0
      memmove(xArray_ + ndx, xArray_ + ndx + 1, (size_ - ndx - 1)*sizeof(float));
      memmove(yArray_ + ndx, yArray_ + ndx + 1, (size_ - ndx - 1)*sizeof(float));
#else
      memcpy(xArray_ + ndx, xArray_ + ndx + 1, (size_ - ndx - 1)*sizeof(float));
      memcpy(yArray_ + ndx, yArray_ + ndx + 1, (size_ - ndx - 1)*sizeof(float));
#endif
    }
  size_--;
}

void AvPointList2D::setValue(int ndx, float x, float y)
{
  if (ndx >= maxSize_)
    grow( MAX(ndx - maxSize_ + 1 , SVPL2D_GROWVAL));
	
  xArray_[ndx] = x;
  yArray_[ndx] = y;
}
      
void AvPointList2D::getValue(int ndx, float * x, float * y)
{
  if (ndx < size_) 
    {
      *x = xArray_[ndx];
      *y = yArray_[ndx];
    }
  else
    {
      *x = 0;
      *y = 0;
    }
}

void AvPointList2D::scaleX(float factor)
{
  for (int i = 0; i < size_; i++)
    xArray_[i] *= factor;
}
