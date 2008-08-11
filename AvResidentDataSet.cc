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
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvResidentDataSet.cc,v 19.0 2003/07/16 05:46:26 aips2adm Exp $
//
// $Log: AvResidentDataSet.cc,v $
// Revision 19.0  2003/07/16 05:46:26  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:14  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/09 18:19:38  hravlin
// Changed references to MAXVALUE to FLOATMAX which is defined in Av.h.
//
// Revision 17.0  2001/11/12 19:41:42  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:41:53  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:07:28  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:07:26  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:05  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:06  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:57:33  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:50:59  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:23:09  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:18:46  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.5  1996/12/17 17:20:02  droberts
// Removed unnecessary include of <ieeefp.h>
//
// Revision 7.4  1996/12/12 07:03:27  droberts
// Final update from monet archive.
//
// Revision 7.3  1996/10/25 22:41:17  jpixton
// added putData(AvDataSet)
//
// Revision 7.2  1996/10/22 20:04:24  jpixton
// improved axis name recognition.
//
// Revision 7.1  1996/09/26 20:45:46  jpixton
//  initial checkin before native aips images supported.
//
// Revision 1.4  1996/09/20 20:19:35  pixton
// Prefixed all classes with Av
//
// Revision 1.3  1996/08/30  20:25:29  pixton
// fixed min/max bug.
//
// Revision 1.2  1996/08/28  22:16:31  pixton
// bug fix - failed to copy passed data array if takeover is false.
//
// Revision 1.1  1996/08/23  19:32:19  pixton
// Initial revision
//
//---------------------------------------------------------------------------

#include <stdio.h>
#include <math.h>
#include <iostream.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#ifdef __GNUC__
extern "C" {
#endif
#include <sys/mman.h>
#ifdef __GNUC__
}
#endif
#include <sys/stat.h>
     
#include "AvString.h"
#include "AvResidentDataSet.h"
#include "Av.h"			// FLOATMAX

AvDataSet * AvResidentDataSet::makeExample(int sizex,
					   int sizey,
					   int sizez)
{
  AvIPosition shape(3, sizex, sizey, sizez);
  int nelts = shape.product();
  float * array = new float[nelts];
  for (int z = 0; z < sizez; z++)
    for (int y = 0; y < sizey; y++)
      for (int x = 0; x < sizex; x++)
	{
	  float val = (float)(x + 100*y + 10000*z);
	  array[z*sizey*sizex + y*sizex + x] = val ? val : -1;
	}

  const char * bunit = "mjy/beam";

  AvString ctypes[3];
  ctypes[0] = "RA---SIN";
  ctypes[1] = "DEC--SIN";
  ctypes[2] = "FELO-LSR";
  

  double crvals[3] = { 1, 1, 1 };
  double crpixs[3] = { 10,10,10 };
  double cdelts[3] = { 1, 1, 1 };
  
  double pcmatrix[9];
  memset(pcmatrix, 0, 9*sizeof(double));
  for (int i = 0; i < 3; i++) pcmatrix[i*3+i] = 1;

  double projps[10] = { 0,0,0,0,0,0,0,0,0,0 };
  double longpole = 999.0;
  
  AvCoordinateSystem cs(3, 
			ctypes,
			crvals,
			crpixs,
			cdelts,
			pcmatrix,
			projps,
			longpole);

  return new AvResidentDataSet(array,
			       shape,
			       bunit,
			       cs,
			       "example",
			       -1,
			       1);
}

AvResidentDataSet::AvResidentDataSet(const float * array,
				     const AvIPosition& shape,
				     const char * bunit,
				     const AvCoordinateSystem& cs,
				     const char * name,
				     const float blankVal,
				     Boolean takeOverArray,
				     const float datamin,
				     const float datamax)
  : attached_(0),
    cs_(new AvCoordinateSystem(cs)),
    dataUnits_(bunit),
    dataUnitsName_(""),
    dataMeasurementName_(""),
    name_(name),
    array_((float *)array),
    arrayOwner_(takeOverArray),
    shape_(shape),
    blankVal_(blankVal),
    dataMin_(datamin),
    dataMax_(datamax)
{
  init();
}

AvResidentDataSet::AvResidentDataSet(const float * array,
				     const AvIPosition& shape,
				     const char * bunit,
				     const AvCoordinateSystem& cs,
				     const char * name,
				     const float blankVal,
				     Boolean takeOverArray)
  : attached_(0),
    cs_(new AvCoordinateSystem(cs)),
    dataUnits_(bunit),
    dataUnitsName_(""),
    dataMeasurementName_(""),
    name_(name),
    array_((float *)array),
    arrayOwner_(takeOverArray),
    shape_(shape),
    blankVal_(blankVal),
    dataMin_(FLOATMAX),
    dataMax_(-FLOATMAX)
{
  init();
  for (int i = 0; i < shape_.product(); i++)
    {
      if (array_[i] < dataMin_) dataMin_ = array_[i];
      if (array_[i] > dataMax_) dataMax_ = array_[i];
    }  
}

void AvResidentDataSet::init()
{
  int i;
  arrayStep_.resize(shape_.nelements());
  arrayStep_(0) = 1;
  for (i = 1; i < rank(); i++) 
    arrayStep_(i) = shape_(i-1)*arrayStep_(i-1);

  if (!arrayOwner_)
    {
      float * array = array_;

      // don't manage means make a copy
      array_ = new float[nelements()];
      if (!array_)
	{
	  cerr << "AvResidentDataSet ctor - not enough memory\n";
	  
	  // [ ] Intelligent cleanup here, maybe copy what we can
	}
      memcpy(array_, array, shape_.product()*sizeof(float));
    }
  
  if (dataUnits_ == "mjy/beam")
    setDataUnits("millijanskys/beam", "specific intensity");
  else if (dataUnits_ == "jy/beam")
    setDataUnits("janskys/beam", "specific intensity");
  else if (dataUnits_ == "gauss")
    setDataUnits("gauss", "magnetic field strength");
  else
    {
      AvString prefix, base, type;
      double multiplier;
      if (AvUnits::lookup(dataUnits_, prefix, base, type, multiplier))
	setDataUnits(prefix+base, type);
      else
	setDataUnits(dataUnits_, "");
    } 

  
  // Establish axis units based on FITS/AIPS conventions.  Add as
  // needed, consulting AvUnits for units and measurement type.
  for (i = 0; i < rank(); i++)
    {
      AvString s = cs_->axisName(i);
      s.downcase( );
      if (s == "ra" || s.contains("right ascension"))
	cs_->setAxisUnits(i, "degrees", "right ascension");
      else if (s == "elon" || s == "glon" || s.contains("longitude"))
	cs_->setAxisUnits(i, "degrees", "longitude");
      else if (s == "dec" || s.contains("declination"))
	cs_->setAxisUnits(i, "degrees", "declination");
      else if (s == "elat" || s == "GLAT" || s.contains("latitude"))
	cs_->setAxisUnits(i, "degrees", "latitude");
      else if (s == "freq" || s.contains("frequency"))
	cs_->setAxisUnits(i, "hertz", "frequency");
      else if (s == "felo" || (s.contains("optical") && s.contains("velocity")))
	cs_->setAxisUnits(i, "meters/second", "optical velocity");
      else if (s == "velo" || s.contains("velocity"))
	cs_->setAxisUnits(i, "meters/second", "velocity");
      else if (s == "stokes" || s.contains("polarization"))
	cs_->setAxisUnits(i, "stoke index", "polarization");
      else if (s == "seq num")
	cs_->setAxisUnits(i, "sequence number", "sequence");
      else if (s == "channel")
	cs_->setAxisUnits(i, "channel number", "channel");
      else if (s == "beam")
	cs_->setAxisUnits(i, "beam index", "beam");
      else if (s == "time")
	cs_->setAxisUnits(i, "seconds", "time");
      else
	cs_->setAxisUnits(i, "", "");
    }
}

#ifdef GLISH_POST_AVINTEGRATION
AvResidentDataSet::AvResidentDataSet(GlishEvent ...)
{
  // [ ] Build ResidentDataSet from glish array
  
  // (Optional) pc matrix
  double * pcmatrix = new double[rank*rank];
  for (j = 0; j < rank; j++)
    for (i = 0; i < rank; i++)
      {
	sprintf(kword, "PC%03d%03d", i, j);
	getKeyword(kword, pcmatrix[i*rank+j], (i == j) ? 1.0 : 0.0);
      }
  
  cs_ = new AvCoordinateSystem(rank,
			       ctypes,
			       crvals,
			       crpixs,
			       cdelts,
			       pcmatrix,
			       projps,
			       longPole);
  
  // Establish axis units based on FITS conventions.  Add as
  // needed, consulting AvUnits for units and measurement type.
  for (i = 0; i < rank; i++)
    {
      AvString s = cs_->axisName(i);
      if (s == "RA")
	cs_->setAxisUnits(i, "degrees", "right ascension");
      else if (s == "ELON" || s == "GLON")
	cs_->setAxisUnits(i, "degrees", "longitude");
      else if (s == "DEC")
	cs_->setAxisUnits(i, "degrees", "declination");
      else if (s == "ELAT" || s == "GLAT")
	cs_->setAxisUnits(i, "degrees", "latitude");
      else if (s == "FREQ")
	cs_->setAxisUnits(i, "hertz", "frequency");
      else if (s == "VELO")
	cs_->setAxisUnits(i, "meters/second", "velocity");
      else if (s == "FELO")
	cs_->setAxisUnits(i, "meters/second", "optical velocity");
      else if (s == "STOKES")
	cs_->setAxisUnits(i, "stoke index", "polarization");
      else if (s == "SEQ NUM")
	cs_->setAxisUnits(i, "sequence number", "sequence");
      else
	cs_->setAxisUnits(i, "", "");
    }
  delete pcmatrix;
  
  bunit_ = getKeyword("BUNIT");

  if (bunit_ == "mjy/beam")
    setDataUnits("millijanskys/beam", "specific intensity");
  else if (bunit_ == "jy/beam")
    setDataUnits("janskys/beam", "specific intensity");
  else if (bunit_ == "gauss")
    setDataUnits("gauss", "magnetic field strength");
  else
    {
      AvString prefix, base, type;
      double multiplier;
      if (AvUnits::lookup(bunit_, prefix, base, type, multiplier))
	setDataUnits(prefix+base, type);
      else
	setDataUnits(bunit_, "");
    }
}
#endif

AvResidentDataSet::~AvResidentDataSet()
{
  if (arrayOwner_) delete [] array_;
}

float AvResidentDataSet::getDatum(const AvIPosition& pos)
{
  if (!validIJK(pos))
    { cerr << "AvResidentDataSet::getDatum: Invalid position : " 
	   << pos << endl; return blankVal_; }

  int ndx = (arrayStep_*pos).sum();

  return array_[ndx];
}

int AvResidentDataSet::getRow(float * buf,
			      const AvIPosition& start,
			      int countp,
			      int stride,
			      int axis)
{
  int i;

  int count = countp;

  if (!validIJK(start))
    { cerr << "AvResidentDataSet::getRow: Invalid start position : " 
	   << start << endl; return 0; }
  if (count <= 0)
    { cerr << "AvResidentDataSet::getRow: Invalid count : " 
	   << count << endl; return 0; }
  if (!validAxis(axis))
    { cerr << "AvResidentDataSet::getRow: Invalid axis : " 
	   << axis << endl; return 0; }
  if (stride <= 0)
    { cerr << "AvResidentDataSet::getRow: Invalid stride of " 
	   << stride << " ignored.\n";  stride = 1;}

  // reduce count if it is too big with respect to start and end
  int nAvail = 1 + (shape_(axis) - start(axis)) / stride;
  if (count > nAvail)
    { cerr << "AvResidentDataSet::getRow: only " << nAvail <<
	" elements available (requested " << count << ").\n";  count = nAvail; }

  float * p = array_ + (arrayStep_*start).sum();
  int arrayStride = arrayStep_(axis)*stride;
  for (i = 0; i < count; i++, p += arrayStride) buf[i] = *p;

#if 0
  // uncomment if you want the remaining part of the
  // buffer filled with blank values
  for (i < countp; i++) buf[i] = blankVal_;
#endif  

  return i;
}

const char * AvResidentDataSet::axisName(int axis) const
{ return cs_->axisName(axis); }
const char * AvResidentDataSet::axisUnitsName(int axis) const
{ return cs_->axisUnits(axis); }
const char * AvResidentDataSet::axisMeasurementName(int axis) const
{ return cs_->axisMeasurementName(axis); }

AvString AvResidentDataSet::axisValueUnits(int axis,
				      double value,
				      int nsigfigs) const
{
  return cs_->valueUnitsStr(axis, value, nsigfigs); 
}

double AvResidentDataSet::axisValueInUnits(int axis,
				      double value,
				      const AvString& units)
{
  return cs_->valueInUnits(axis, value, units); 
}

Boolean AvResidentDataSet::IJKToWorld_(const AvWPosition &ijk,
				  AvWPosition &world) const
{
  return cs_->ijk2world(ijk, world);
}

Boolean AvResidentDataSet::WorldToIJK_(const AvWPosition &world,
				  AvWPosition &ijk) const
{
  return cs_->world2ijk(world, ijk);
}

Boolean AvResidentDataSet::putData(const AvDataSet& newData)
{
  // Ok - need to ensure that datasets conform

  // drop axes from the end if degenerate
  if (newData.shape() != shape())
    {
      cerr << "AvAipsDataSet::putData() datasets don't conform\n";
      return False;
    }

  // Ok, now we must copy the new data onto the old data.  Do this by
  // slicing along the last axis and using a buffer.

  AvIPosition start(shape().nelements(), 0);
  AvIPosition stride(shape().nelements(), 1);

  AvDataSet * hack1 = &((AvDataSet &) newData);
  hack1->getSlice(array_, start, newData.shape(), stride);

  return True;

  // data copied, so now get coordinate system
}
