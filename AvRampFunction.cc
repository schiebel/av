//# Copyright (C) 1995-2002 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvRampFunction.cc,v 19.0 2003/07/16 05:47:19 aips2adm Exp $
//
// $Log: AvRampFunction.cc,v $
// Revision 19.0  2003/07/16 05:47:19  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:00  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/07 22:53:09  hravlin
// Included <float.h> to get FLT_MIN/FLT_MAX.
//
// Revision 17.0  2001/11/12 19:42:30  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:37  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:39  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:23  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/08/25 19:43:11  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:40:11  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:59  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:13  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:08  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/09 21:34:40  hr
// Fixed some type conversion warnings.
//
// Revision 9.0  1997/08/25 21:28:08  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:17:10  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:59:14  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.10  1996/06/18  18:46:29  pixton
//  Copyright Update
//
//  Revision 1.9  1995/11/16  23:04:26  pixton
//  fixed endpoint bug.
//
//  Revision 1.8  1995/11/16  17:34:16  pixton
//  modified XDraw to include border area.
//
//  Revision 1.7  1995/11/14  17:19:51  pixton
//  Added clamp HACK to fix array overrun.  Will fix later.
//
//  Revision 1.6  1995/11/13  22:16:59  pixton
//  resize added.
//
//  Revision 1.5  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/09/12  16:10:51  hr
//  Added check to prevent a divide by zero in linearLocal.
//
//  Revision 1.3  1995/08/30  18:00:02  hr
//  Changed the way expFunction in expFit gets initialized to g++ 2.6
//  can compile it.
//
//  Revision 1.2  1995/07/18  17:27:17  hr
//  'Fixed' g++ warnings. Mostly don't do "for(int i...)".
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>		// FLT_MIN, FLT_MAX
#include <AvRampFunction.h>


//-----Rotational-shift the values in the ramp by delta positions
//     If delta is negative, then the shift is to the left. 
//     System V memmove() is intentionally avoided.
void AvRampFunction::shift(int delta)
{
  if (delta < 0)
    {
      int mvlen = abs(delta);
      float * t1 = new float[mvlen];
      float * t2 = new float[size_ - mvlen];

      memcpy(t1, array_, mvlen*sizeof(float));
      memcpy(t2, array_+mvlen, (size_ - mvlen)*sizeof(float));
      memcpy(array_ + (size_ - mvlen), t1, mvlen*sizeof(float));
      memcpy(array_, t2, (size_ - mvlen)*sizeof(float));

      delete [] t1;
      delete [] t2;
    }
  else if (delta > 0)
    {
      int mvlen = delta;
      float * t1 = new float[mvlen];
      float * t2 = new float[size_ - mvlen];
      
      memcpy(t1, array_, (size_ - mvlen)*sizeof(float));
      memcpy(t2, array_ + (size_ - mvlen), mvlen*sizeof(float));
      memcpy(array_ + mvlen, t1, (size_ - mvlen)*sizeof(float));
      memcpy(array_, t2, mvlen*sizeof(float));

      delete [] t1;
      delete [] t2;
    }
}

void AvRampFunction::setValues(const float * data, int len)
{
  if (len > size_) resize(len);
  memcpy(array_, data, len*sizeof(float));
}

//-----Reset changes the ramp to full linear ramp
void AvRampFunction::reset(float scale)
{ linearLocal(0, 0, (int)size_-1, scale); }

//-----Cached value should be implemented
float AvRampFunction::maxY() const
{
  float max = FLT_MIN;
  for (long i = 0; i < size_; i++)
    { if (array_[i] > max) max = array_[i]; }
  return max;
}
float AvRampFunction::minY() const
{
  float min = FLT_MAX;
  for (long i = 0; i < size_; i++)
    { if (array_[i] < min) min = array_[i]; }
  return min;
}

//-----resize and stretch data to fit new size
void AvRampFunction::resize(long newSize)
{ 
  if (newSize == size_) return;  // Don't need to do anything

  if (newSize < 2)
    {
      printf("AvRampFunction::resize() - newSize (< 2) request ignored\n");
      return;
    }

  float * newArray = new float[newSize];
  if (!newArray) 
    { 
      printf("AvRampFunction::resize() failed - Out of Memory\n"); 
      return; 
    }
  
  float step = ((float) (size_)) / ((float)newSize);
  float floc = 0.0;
  
  // Actually do the stretching
  for (long ndx = 0; ndx < newSize; ndx++, floc += step)
    {
      long floorval = (long) floc;
      float w2 = floc - (float) floorval;
      float w1 = 1 - w2;
      long ceilval = floorval + 1;
      long w1loc = (long) floc;

      if (w1loc >= size_) w1loc = size_-1;
      if (ceilval >= size_) ceilval = size_-1;

      newArray[ndx] = (w1*array_[w1loc] + w2*array_[ceilval]);
    }
  
  delete [] array_;

  // Scale the control point X values from [0,size_] to [0,newSize]

  float factor = ((float) newSize) / (float)size_;
  pts()->scaleX(factor);

  array_ = newArray;
  size_ = newSize;
}

//-----Copy data from another rf with identical size
AvRampFunction& AvRampFunction::operator = (const AvRampFunction& rf)
{
  (*pts_) = *(rf.pts_);
  memcpy(array_, rf.array_, size_*sizeof(float));
  order_ = rf.order_;
  wave_ = rf.wave_;
  return *this;
}

//-----partial linear from "low" value of y1 at x1 to
//     "high" value of y2 at x2
void AvRampFunction::linearLocal(int x1, float y1,
		       int x2, float y2)
{
  int xLow = CLAMP(x1, 0, (int)size_-1);
  int xHi  = CLAMP(x2, 0, (int)size_-1);

  if (xLow == xHi) { array_[xLow] = y2; return; }

  // Swap if needed to sort x's and y's
  if (xLow > xHi)
    { 
      int x = xLow; xLow = xHi; xHi = x;
      float y = y1; y1 = y2; y2 = y; 
    }
    
  float m = (y2 - y1) / (xHi - xLow);
  int i = xLow;

  for (float v = y1; i < xHi; i++, v += m) array_[i] = v;

  // Can't rely on endpoint being exact in loop.  Instead set
  // the last value explicitly.
  array_[xHi] = y2;
}

//-------------------------------------------------------------------
//  Takes the control points, treats it as a wave, and propagates it
//  throughout the array.
//
void AvRampFunction::waveRepeat()
{
  if (pts_->empty()) return;

  if (pts_->length() == 1)
    {
      float x,y;
      pts_->getValue(0,&x,&y);
      for (int k = 0; k < size_; k++) array_[k] = y;
      return;
    }

  int x1 = (int) pts_->firstX();
  int x2 = (int) pts_->lastX();
  int period = (x2 - x1 + 1);
 
  // Propagate the wave forward in the array
  int k;
  for (k = x2+1; k+period <= size_; k += period)
    {
      memcpy(array_+k, array_+x1, period*sizeof(float));
    }
  if (k < size_) memcpy(array_+k, array_+x1, (size_-k)*sizeof(float));

  // Propagate the wave backward in the array
  for (k = x1 - period; k >= 0; k -= period)
    {
      memcpy(array_+k, array_+x1, period*sizeof(float));
    }
  if (k > -period) memcpy(array_, array_+x1-k, (period+k)*sizeof(float));
} 

//-----------------------------------------------------------------
//  Takes the control points, treats it as a unit and extends 
//  x1's value from 0 to x1-1, and x2's value from x2 to the
//  end of the array.
//
void AvRampFunction::waveExtend()
{
  if (pts_->empty()) return;
 
  if (pts_->length() == 1)
    {
      float x,y;
      pts_->getValue(0,&x,&y);

      for (int k = 0; k < size_; k++) array_[k] = y;
      return;
    }

  int x1 = (int) pts_->firstX();
  int x2 = (int) pts_->lastX();
  int k;
  for (k = 0; k < x1; k++) array_[k] = array_[x1];   // opt
  for (k = x2+1; k < size_; k++) array_[k] = array_[x2]; // opt
}

//-----------------------------------------------------------------
//  Same as above, only zero's the values instead producing a
//  band-pass type function
//
void AvRampFunction::waveTruncate()
{
  if (pts_->empty()) return;

  if (pts_->length() == 1)
    {
      float x,y;
      pts_->getValue(0,&x,&y);

      for (int k = 0; k < size_; k++) array_[k] = y;
      return;
    }

  int x1 = (int) pts_->firstX();
  int x2 = (int) pts_->lastX();
  memset(array_, 0, x1*sizeof(float));
  memset(array_+x2+1, 0, (size_ - x2 - 1)*sizeof(float));
}

void AvRampFunction::constantFit()
{
  if (pts_->empty()) return;

  float *xa = pts_->xArray();
  float *ya = pts_->yArray();

  if (pts_->length() > 1)
    {int ndx;

      for (ndx = 1; ndx < pts_->size(); ndx++)
	{
	  float val = ya[ndx-1];
	  for (int i = (int) xa[ndx-1]; i < (int) xa[ndx]; i++)
	    array_[i] = val;
	}
      array_[(int)xa[ndx-1]] = ya[ndx-1];
    }
  else
    {
      array_[(int)xa[0]] = ya[0];
    }
}

void AvRampFunction::linearFit()
{ 
  if (pts_->length() < 2) return;

  float *xa = pts_->xArray();
  float *ya = pts_->yArray();
  for (int ndx = 1; ndx < pts_->size(); ndx++)
    {
      linearLocal((int) xa[ndx-1], ya[ndx-1], 
		  (int) xa[ndx], ya[ndx]);
    }
} 

//
// [ ] OPT : Cache the coefficients!
//
// Fill the array in the defined region with a spline SPLINE between
// the control points.
//
void AvRampFunction::splineFit()
{
  if (pts_->empty()) return;
  if (pts_->length() < 3) { linearFit(); return; }

  float n = (float) pts_->length();
  float tStep = 0.1;  // [ ] HARD

  float prevX, prevY;
  pts_->getValue(1, &prevX, &prevY);  // plot 1-->0
  float t;
  for (t = 1; t >= -tStep/2; t -= tStep)
    {
      float x = splineEvalX(t);
      float y = splineEvalY(t);
      y = CLAMP(y, 0, 1);
      linearLocal((int) (prevX + 0.5), prevY, (int)(x + 0.5), y);
      prevX = x; prevY = y;
    }

  pts_->getValue(1, &prevX, &prevY);  // plot 1-->n
  float endPt = (n-1)+tStep/2;
  for (t = 1; t <= endPt; t += tStep)
    {
      float x = splineEvalX(t);
      float y = splineEvalY(t);
      y = CLAMP(y, 0, 1);
      linearLocal((int) (prevX + 0.5), prevY, (int)(x + 0.5), y);
      prevX = x; prevY = y;
    }
}

void AvRampFunction::getSplineCoef(int s, float * coef, float * pts)
{
  // BASIS MATRICES BEING USED IN COMPUTATION BELOW
  //
  // float crLo[16]  = {0,0,-.5,.5,0,0,0,0,0,-1,1.5,-.5,0,1,0,0};
  // float crMid[16] = {-.5,1.5,-1.5,.5,1,-2.5,2,-.5,-.5,0,.5,0,0,1,0,0};
  // float crHi[16]  = {-.5,.5,0,0,1.5,-1,5,0,0,-1,0,1,0,0,1,0,0};
  
  if (s == 0)  // First Segment - set 2nd derivative of first point == 0
    {
      coef[0] =           -.5*pts[1] + .5*pts[2];
      coef[1] = 0;
      coef[2] = -pts[0] + 1.5*pts[1] - .5*pts[2];
      coef[3] = pts[0];
    }
  else if (s == pts_->length() - 2)  // Last Seg - 2nd Derivative last point = 0
    {
      coef[0] = -.5*pts[s-1] + .5*pts[s];
      coef[1] = 1.5*pts[s-1] - 1.5*pts[s];
      coef[2] = -pts[s-1]                  + pts[s+1];
      coef[3] =                 pts[s];
    }
  else  // middle section - full spline continuity
    {
      coef[0] = -.5*pts[s-1] + 1.5*pts[s] - 1.5*pts[s+1] + .5*pts[s+2];
      coef[1] =   1*pts[s-1] - 2.5*pts[s] +  2*pts[s+1] - .5*pts[s+2];
      coef[2] = -.5*pts[s-1]              + .5*pts[s+1];
      coef[3] =                   pts[s];
    }
}

float AvRampFunction::splineEvalX(float t)
{
  int segment = MIN(pts_->length()-2, (int) t);
  t = t - (float) segment;
  float *xp = pts_->xArray();  // pointer to pts data to load;
  float coefX[4];
  getSplineCoef(segment, coefX, xp);
  return (((coefX[0])*t + coefX[1])*t + coefX[2])*t + coefX[3];
}

float AvRampFunction::splineEvalY(float t)
{
  int segment = MIN(pts_->length()-2, (int) t);
  t = t - (float) segment;
  float *yp = pts_->yArray();
  float coefY[4];
  getSplineCoef(segment, coefY, yp);
  return (((coefY[0])*t + coefY[1])*t + coefY[2])*t + coefY[3];
}

void AvRampFunction::recalc()
{
  // Fill the area within the control points according to
  // the point fitting function

  switch(order_)
    {
    case CONSTANT: 
      constantFit(); break;
    case LINEAR:   
      linearFit(); break;
    case SPLINE:    
      splineFit(); break;
    case EXPONENTIAL:
      expFit(); break;
    }

  // Repeat the pattern according to the wave type
  switch(wave_)
    {
    case TRUNCATE: 
      waveTruncate(); break;
    case EXTEND:
      waveExtend(); break;
    case REPEAT:
      waveRepeat(); break;
    }
}

//
//  Use the computed parameter k to fill the array with discretized
//  exponential curve between the control points 
//
void AvRampFunction::expFwdFill(float k, float xoff, float yoff, float xlen, float ylen)
{
  k = CLAMP(k, -100, 100);
  if (k == 0) k = 0.0001;
  
  float a = ylen / (exp(k) - 1);
  for (int x = 0; x < (int) (xlen+0.999); x++)  // ceil xlen
    {
      array_[(int)(x + xoff)] = yoff + a*(exp(x * k / xlen) - 1);
    }
}

void AvRampFunction::expRevFill(float k, float xoff, float yoff, float xlen, float ylen)
{
  k = CLAMP(k, -100, 100);
  if (k == 0) k = 0.0001;
  
  float a = ylen / (exp(k) - 1);
  for (int x = 0; x < (int) (xlen+0.999); x++)  // ceil xlen
    {
      array_[(int)(x + xoff)] = yoff + ylen - (a*(exp(x * k / xlen) - 1));
    }
}     

// Function used in bisection algorithm below to fit the exponential through
// <0,0>, <x,y>, and <xlen, ylen>.  
float AvRampFunction::expFwdFunction(float k, float x, float xlen, float ylen)
{
  if (k == 0.0) k = 0.00000001;
  return (ylen / (exp(k) - 1)) * (exp(k * x / xlen) - 1);
}

float AvRampFunction::expRevFunction(float k, float x, float xlen, float ylen)
{
  if (k == 0.0) k = 0.00000001;
  return ylen - (ylen / (exp(k) - 1)) * (exp(k * x / xlen) - 1);
}

//
//  Fits an exponential curve through the first three control points and fills
//  the corresponding array.
//
void AvRampFunction::expFit()
{
  float x1,y1; pts_->getValue(0, &x1, &y1);
  float x3,y3; pts_->getValue(2, &x3, &y3);

  float xoff = MIN(x1,x3);
  float yoff = MIN(y1,y3);

  float xlen = fabs(x1 - x3);
  float ylen = fabs(y1 - y3);


#if 0
  // GNU g++ 2.6 can't handle this.
  float (AvRampFunction::*expFunction)(float k, float x, float xlen, float ylen) = 
    (y3 > y1) ? &AvRampFunction::expFwdFunction 
      : &AvRampFunction::expRevFunction;
#else
  float (AvRampFunction::*expFunction)(float k, float x, float xlen, float ylen);
  if( y3 > y1)
        expFunction = &AvRampFunction::expFwdFunction;
  else
        expFunction = &AvRampFunction::expRevFunction;
#endif

  // Curve is computed to pass through the middle point
  float x2,y2; pts_->getValue(1, &x2, &y2);
  x2 -= xoff;
  y2 -= yoff;

  // Find the value of k such that the point passes through
  // the point x2,y2 or the closest reasonable approximation to it
  float k;
  if (y2 <= 0 || x2 >= xlen)
    {
      k = (y3 > y1) ? 100 : -100;
    }
  else if (y2 >= ylen)
    {
      k = (y3 > y1) ? -100 : 100;
    }
  else  // run bisection to find k 
    {
      float ka = (y3 > y1) ? -100 : 100;  // Corresponds to high curve
      float kb = (y3 > y1) ? 100 : -100;   // Corresponds to low curve
      
      float fka = (this->*expFunction)(ka, x2, xlen, ylen);
      float fkb = (this->*expFunction)(kb, x2, xlen, ylen);

      while (fabs(fka - fkb) > 0.001)
	{
	  k = (ka + kb) / 2;
	  float y = (this->*expFunction)(k, x2, xlen, ylen);
	  if (y > y2)  // too high
	    {
	      ka = k;
	      fka = y;
	    }
	  else
	    {
	      kb = k;
	      fkb = y;
	    }
	}
      k = ka;
    }
  if (y1 > y3)
    expRevFill(k, xoff, yoff, xlen, ylen);
  else
    expFwdFill(k, xoff, yoff, xlen, ylen);
}





