//# Copyright (C) 1995-98 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvRampFunction.h,v 19.0 2003/07/16 05:47:32 aips2adm Exp $
//
// $Log: AvRampFunction.h,v $
// Revision 19.0  2003/07/16 05:47:32  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:11  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:41  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:47  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:25  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:38  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:26  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:26  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:51  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:51  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1998/06/09 21:32:00  hr
// Fixed a type conversion warning.
//
// Revision 9.0  1997/08/25 21:29:14  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:17:54  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 07:02:16  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.5  1996/06/18  18:43:15  pixton
//  Copyright Update
//
//  Revision 1.4  1995/11/13  22:25:47  pixton
//  added resize.
//
//  Revision 1.3  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/08/03  16:42:14  hr
//  Initialize array_ to 0.0.
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

//
//
//  AvRampManager - Manages a single ramp
//
//
//  Model : manages a array of size_ floats dynamically located at ramp_
//
//  The float arrays are             
//
#ifndef _SVRAMPFUNCTION_H_
#define _SVRAMPFUNCTION_H_

#ifndef CLAMP
#define CLAMP(X,L,H) ((X < L) ? L : (X > H) ? H : X)
#endif

#ifndef MIN
#define MIN(A,B) ((A < B) ? A : B)
#endif

#ifndef MAX
#define MAX(A,B) ((A > B) ? A : B)
#endif

#include <AvPointList2D.h>

class AvRampFunction
{
public:

  enum Order_t { CONSTANT, LINEAR, SPLINE, EXPONENTIAL };
  enum Wave_t { TRUNCATE, EXTEND, REPEAT };
  
  AvRampFunction(long len = 256) : size_(len), order_(LINEAR), wave_(EXTEND)
    { 
      array_ = new float[size_]; 
      for(int i=0; i< size_; i++)
		array_[i] = 0.0;
      pts_ = new AvPointList2D(20);
    }
      
  ~AvRampFunction() { delete [] array_; }

  AvRampFunction& operator = (const AvRampFunction& rf);

  void reset(float scale);

  //-----Read Access
  float * array() const { return array_; }
  long length()   const { return size_;  }

  //-----Write Access
  void setValues(const float * array, int len);
  void length(long len) { resize(len); }

  //-----lin stretch to fit new array
  void resize(long len); 

  //-----Set the whole array from 0 to size_-1;
  void linear(const float y1, const float y2);

  //-----Set a portion of the array, from x1 to x2
  void linearLocal(int x1, float y1, 
		   int x2, float y2);

  //-----Sawtooth Fit
  void orderMode(Order_t mode) 
    { order_ = CLAMP(mode, CONSTANT, EXPONENTIAL); }
  void waveMode(Wave_t mode) 
    { wave_ = CLAMP(mode, TRUNCATE, REPEAT); }

  Wave_t waveMode() const { return wave_; }
  Order_t orderMode() const { return order_; }

  void waveRepeat();
  void waveExtend();
  void waveTruncate();

  void recalc();

  //-----functions that fill array from control points
  void constantFit();
  void linearFit();
  void splineFit();

  void expFit();
  void expFwdFill(float k, float xoff, float yoff, float xlen, float ylen);
  void expRevFill(float k, float xoff, float yoff, float xlen, float ylen);
  float expFwdFunction(float k, float x, float xlen, float ylen);
  float expRevFunction(float k, float x, float xlen, float ylen);

  float splineEvalX(float t);
  float splineEvalY(float t);
  void getSplineCoef(int segment, float * coef, float * pts);

  AvPointList2D * pts() const { return pts_; }

  void setValue(int x, float y)
    { if (0 <= x && x < size_) array_[x] = y; }
  
  float minX() const { return 0; }
  float minY() const;
  float maxX() const { return (float)(size_ - 1); }
  float maxY() const;

  void shift(int delta);

private:

  AvPointList2D * pts_;  // control points
  long size_;        // number of float values in ramp
  float * array_;  // space for array.
  Order_t order_;  // function order for curve
  Wave_t wave_;    // Defines how control points are extended to fill array
};

#endif
