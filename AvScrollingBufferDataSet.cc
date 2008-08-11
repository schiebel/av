//# Copyright (C) 1995-2000 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvScrollingBufferDataSet.cc,v 19.0 2003/07/16 05:47:24 aips2adm Exp $
//
// $Log: AvScrollingBufferDataSet.cc,v $
// Revision 19.0  2003/07/16 05:47:24  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:05  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:34  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:41  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:58  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:29  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.2  2000/03/09 19:52:24  hravlin
// Removed some compiler warnings.
//
// Revision 13.1  1999/12/20 05:30:31  wbrouw
// Change (name *) 0 cast to static_cast<name *>(0)
//
// Revision 13.0  1999/08/10 18:40:17  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:09  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:29  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:26  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:28:37  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:17:29  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.4  1996/12/12 10:03:22  droberts
// Final update from monet archive.
//
// Revision 1.12  1996/12/11 20:40:12  hr
// Added RCS header and copyright.
//
//
//---------------------------------------------------------------------------

#include "AvScrollingBufferDataSet.h"

AvScrollingBufferDataSet::AvScrollingBufferDataSet(unsigned int size,
						   AvDataSet * ds)
  : timeAxis_(-1), 
    size_(size), 
    nFilled_(1), 
    origin_(0), 
    buffer_(size, 0),
    shape_(ds->shape()),
    itemShape_(ds->shape()), 
    min_(ds->dataMin()), 
    max_(ds->dataMax()),
    dsWithMin_(ds), 
    dsWithMax_(ds),
    timeVals_(size, 0),
    attached_(0)
{
  unsigned int i;
  for (i = 0; i < (unsigned int) ds->rank(); i++)
    {
      if (!strcmp(ds->axisMeasurementName(i), "time"))
	{
	  cout << "AvSBDS: Found meas. name = '" << ds->axisMeasurementName(i) << "'\n";
	  cout << "AvSBDS:: Setting timeAxis_ = " << i << endl;
	  timeAxis_ = i;
	}
    }

  if (timeAxis_ == -1)
    cerr << "Couldn't figure out time axis!!!\n";
  // [ ] set not ok for class
  
  shape_(timeAxis_) = size_;

  buffer_(0) = ds;
  timeVals_(0) = ds->minWorld()(timeAxis_);
  for (i = 1; i < size_; i++) buffer_(i) = static_cast<AvDataSet *>(0);
}

// private
void AvScrollingBufferDataSet::scroll()
{ 
  origin_++; 
  if (origin_ >= size_) origin_ -= size_; 
}

void AvScrollingBufferDataSet::addDataSet(AvDataSet * ds)
{
  if (scrolling()) 
    {
      replace(ds, origin_);
      scroll();
    }
  else
    {
      if (replace(ds, nFilled_)) nFilled_++;
    }
  AvDataSet::dataChanged();
}

// also private
Boolean AvScrollingBufferDataSet::replace(AvDataSet * ds, int ndx)
{
  if (ds->shape() != itemShape_)
    {
      cerr << "AvScrollingBufferDataSet::replace - new ds doesn't conform:\n";
      cerr << "  existing shape : " << itemShape_ << ", new shape : " 
	   << ds->shape() << endl;
      return False;
    }

  if (!strcmp(ds->dataSetType(), "Scrolling Buffer"))
    {
      cerr << "AvScrollingBufferDataSet::replace - new ds cannot be a scrolling buffer type\n";
      return False;
    }

  // [ ] Ensure only one entry on time axis

  AvDataSet * old = buffer_(ndx);
  buffer_(ndx) = ds;
  timeVals_(ndx) = ds->minWorld()(timeAxis_);

  // expand min/max using new dataset
  float t = ds->dataMin();
  if (t < min_)
    { min_ = t; dsWithMin_ = ds; }
  t  = ds->dataMax();
  if (t > max_)
    { max_ = t; dsWithMax_ = ds; }      

  // maybe have to recalc min/max if removing dataset
  // responsible for min/max
  if (old)
    {
      updateMinMax_(old);
      old->unref();
    }

  ds->ref();

  return True;
}

void AvScrollingBufferDataSet::updateMinMax_(AvDataSet * old)
{
  // update min and max in constant amortized time.  With n
  // datasets, first if will be true in 1/n times incurring
  // n accesses (1/n * n = 1).
  if (old)
    {
      if (dsWithMin_ == old)
	{
	  // recompute min
	  min_ = buffer_(0)->dataMin();
	  dsWithMin_ = 0;
	  for (unsigned int i = 1; i < size_; i++)
	    {
	      if (buffer_(i) == 0) continue;
	      float tmin = buffer_(i)->dataMin();
	      if (tmin < min_)
		{ min_ = tmin; dsWithMin_ = buffer_(i); }
	    }
	}
      
      if (dsWithMax_ == old)
	{
	  // recompute max
	  max_ = buffer_(0)->dataMax();
	  dsWithMax_ = 0;
	  for (unsigned int i = 1; i < size_; i++)
	    {
	      if (buffer_(i) == 0) continue;
	      float tmax = buffer_(i)->dataMax();
	      if (tmax > max_)
		{ max_ = tmax; dsWithMax_ = buffer_(i); }
	    }
	}
    }
}

//
//  Functions that depend on the timeAxis
//

float AvScrollingBufferDataSet::getDatum(const AvIPosition& pos)
{
  if (!validIJK(pos))
    { cerr << "AvScrollingBufferDataSet::getRow - Invalid start position : "
	   << pos << endl; return blankingValue(); }

  AvDataSet * ds = dsByNdx(pos(timeAxis_));

  if (!ds) return blankingValue();

  AvIPosition p = pos;
  p(timeAxis_) = 0;

  return ds->getDatum(p);
}

int AvScrollingBufferDataSet::getRow(float * buf,
		   const AvIPosition& start,
		   int countp,
		   int stride,
		   int axis)
{
  int count = countp;

  // do some checking here
  if (!validAxis(axis)) 
    { cerr << "AvScrollingBufferDataSet::getRow - Invalid Axis : "
	   << axis << endl; return 0; }
  if (!validIJK(start))
    { cerr << "AvScrollingBufferDataSet::getRow - Invalid start position : "
	   << start << endl; return 0; }
  if (count <= 0)
    { cerr << "AvScrollingBufferDataSet::getRow: Invalid count : " 
	   << count << endl; return 0; }
  if (stride <= 0)
    { cerr << "AvScrollingBufferDataSet::getRow: Invalid stride of " 
	   << stride << " ignored.\n";  stride = 1;}

  if (axis == timeAxis_)
    {
      int i;
      int ndx = start(axis);

      // Compare against shape and constrain length if needed
      int nAvail = 1 + (size_ - ndx) / stride;
      if (count > nAvail)
	{ 
	  cerr << "AvScrollingBufferDataSet::getRow: only " << nAvail <<
	    " elements available (requested " << count << ").\n";  
	  count = nAvail; 
	}
      
      AvIPosition p = start;
      p(timeAxis_) = 0;

      for (i = 0; i < count; i++, ndx += stride)
	{
	  AvDataSet * ds = dsByNdx(ndx);
	  buf[i] = (ds) ? ds->getDatum(p) : blankingValue();
	}

      return count;
    }
  else
    {
      // not on time axis, so route to correct ds
      // and set time index to zero
      AvDataSet * ds = dsByNdx(start(timeAxis_));
      AvIPosition p = start;
      p(timeAxis_) = 0;

      return (ds) ? ds->getRow(buf, p, count, stride, axis) : 0;
    }
}

Boolean AvScrollingBufferDataSet::IJKToWorld_(const AvWPosition& ijk,
					      AvWPosition& world) const
{
  // calls IJKToWorld after dealing with time axis
  int timeNdx = (int) (ijk(timeAxis_) + 0.5);

#if 0
  // trap time problems here
  if (timeNdx < 0 || timeNdx >= size_)
    {
      cerr << "AvScrollingBufferDataSet::IJKToWorld_:\n";
      cerr << "    bad ijk : " << ijk << endl;
      return False;
    }
#else
  if (timeNdx < 0) timeNdx = 0;
#endif

  if ((unsigned int) timeNdx >= nFilled_)
    {
#if 0
      cerr << "AvScrollingBufferDataSet::IJKToWorld:\n";
      cerr << "    Time value for " << timeNdx << " not yet available\n";
      return False;
#else
      timeNdx = nFilled_-1;
#endif
    }

  AvWPosition ijk0 = ijk;
  ijk0(timeAxis_) = 0;

  // ok, so forward to ds
  AvDataSet * ds = dsByNdx(timeNdx);
  if (ds)
    return ds->IJKToWorld_(ijk0, world);
  else
    return False;
}

Boolean AvScrollingBufferDataSet::WorldToIJK_(const AvWPosition& world,
					      AvWPosition& ijk) const
{
  // calls WorldToIJK after dealing with time axis
  double timeVal = world(timeAxis_);

  int ndx = timeVal2ndx(timeVal);
  AvDataSet * ds = buffer_(ndx);

  // trap time problems here
  if (!ds)
    {
      cerr << "AvScrollingBufferDataSet::WorldToIJK:\n";
      cerr << "    bad world time value: " << world << endl;
      return False;
    }

  AvWPosition world0 = world;
  world0(timeAxis_) = 0;

  Boolean result = ds->WorldToIJK_(world0, ijk);
  ijk(timeAxis_) = (ndx - origin_) % size_;
  return result;
}

int AvScrollingBufferDataSet::timeVal2ndx(double timeVal) const
{
  int ndx;

  // only one dataset, punt and return its value
  if (nFilled_ == 1) return 0;

  // up to first half-interval?
  double ta = timeVals_(origin_);
  double tb = timeVals_((origin_ + 1) % size_);
  double tm2 = (ta + tb)/2;
  if (timeVal < tm2)
    return origin_;

  // middle intervals?
  double tm1 = tm2;
  for (int i = 1; (unsigned int)i < nFilled_; i++)
    {
      ndx = (i + origin_) % size_;

      ta = tb;
      tb = timeVals_(ndx);
      tm2 = (ta + tb)/2;

      if (tm1 < timeVal && timeVal < tm2)
	return ndx;

      tm1 = tm2;
    }

  // Must be beyond last half-interval.
  return (nFilled_ - 1 + origin_) % size_;
}
