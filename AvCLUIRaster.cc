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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvCLUIRaster.cc,v 19.0 2003/07/16 05:46:39 aips2adm Exp $
//
// $Log: AvCLUIRaster.cc,v $
// Revision 19.0  2003/07/16 05:46:39  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:25  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:41:54  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:04  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:21  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:07:41  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:23  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:36  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:14  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:51:49  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.2  1998/06/16 16:20:19  hr
// Fixed a couple of enum comparision warnings.
//
// Revision 9.1  1998/06/09 17:17:42  hr
// Removed extra semicolons from the "DECLARE_xxx" macros.
//
// Revision 9.0  1997/08/25 21:24:27  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.2  1997/06/16 21:36:14  hr
// Changes to support displaying m/s as km/s, etc.
//
// Revision 8.1  1997/05/20 19:32:49  hr
// In getRegion(), need to only set Z axis if rank > 2.
//
// Revision 8.0  1997/02/20 03:14:51  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.4  1996/12/12 09:59:44  droberts
// Final update from monet archive.
//
// Revision 1.7  1996/11/12 20:12:24  hr
// Added new getRegion() call and removed unneeded {begin,end}Access() calls.
//
// Revision 1.6  1996/11/07 17:13:54  hr
// Changed getRow() to call accessor's getRow routine.
//
// Revision 1.5  1996/10/01 21:55:53  hr
// Calls to accessor()->setPlane() use DS slice values.
//
// Revision 1.4  1996/09/20 20:59:48  pixton
// name changes from IPosition/WPosition to AvIPosition/AvWPosition
//
// Revision 1.3  1996/09/20  18:36:22  hr
// Changes to work with accessor including rewrite of axisinfo.
//
// Revision 1.2  1996/09/10 16:49:19  hr
// Initial changes for integration with AvAccessor & AvDataSet.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.11  1996/06/18  18:45:48  pixton
//  Copyright Update
//
//  Revision 1.10  1996/04/11  16:17:43  hr
//  Modified beginAccess() to ensure that the slice data will be coming from
//  is the correct slice.
//
//  Revision 1.9  1996/02/08  18:27:05  hr
//  Changed the way titles are generated. initAxisInfo now knows about "FREQ".
//
//  Revision 1.8  1996/02/02  20:53:53  hr
//  Added minimum field width for a sprintf.
//  If axis label is "VELO" rather than having the formatter scale by 0.001,
//  the min & max are now scaled.
//
//  Revision 1.7  1996/02/01  18:08:49  hr
//  Removed an unused function.
//
//  Revision 1.6  1996/01/30  23:01:52  hr
//  Renamed image_ to imageSetter_.
//  Removed some turned off code.
//  Changed how axis info is initialized.
//
//  Revision 1.5  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/04/25  16:45:15  hr
//  Added array to hold pointers to the start of each row.
//
//  Revision 1.3  1995/04/24  21:54:04  hr
//  Added getRow function.
//
//  Revision 1.2  1995/04/24  18:39:12  hr
//  Moved index generation to CLUIRaster from CLUImageSetter to support
//  raster's beginAccess and endAccess.
//
//  Revision 1.1  1995/04/14  16:29:08  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvCLUIRaster.C
A 2D Raster that gets its data from the AvCLUImageSetter that created it.

*/
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <AvCLUIRaster.h>
#include <AvCLUImageSetter.h>
#include <AvImageViewItem.h>

DEFINE_MEMBER_INFO(AvCLUIRaster)

AvCLUIRaster::AvCLUIRaster(AvCLUImageSetter *img, const int sliceIndex)
{
	INIT_CLASS(AvRaster);
	imageSetter_ = img;
	sliceIndex_ = sliceIndex;
	accessor_ = new AvAccessor( *img->accessor());
	accessor_->ref();
	accessor_->lockResource();
	accessor_->setPlane(sliceIndex, FALSE);

	AvCLUIRaster::axisInfo *inf = new AvCLUIRaster::axisInfo(accessor_);
	setAxisInfo(inf);

	AvImageViewItem *ivi = img->imageViewItem();


	// The name of the raster contains slice information.
	{char namebuf[512];

		 if((accessor_->rank() > 2) && (inf->title() != NULL))
		 { AvIPosition ijk = accessor_->dataset()->start();
		   AvWPosition wpos;

			ijk(2) = sliceIndex;
			accessor_->XYZToWorld(ijk, wpos);
			sprintf(namebuf, "%s:%s:%s", ivi->label(),
				inf->title(),
				accessor_->axisValueUnits(2,wpos(2)).chars());
		 }
		else
			sprintf(namebuf, "%s", ivi->label());

		name(namebuf);
	}
}

unsigned int AvCLUIRaster::width()const
{
	return accessor_->width();
}

unsigned int AvCLUIRaster::height()const
{
	return accessor_->height();
}

float AvCLUIRaster::min()const
{
	return accessor_->clipMin();
}

float AvCLUIRaster::max()const
{	return accessor_->clipMax();
}

float AvCLUIRaster::getElement(const int x, const int y)
{
	return accessor_->getDatum(x, y);
}

#if 0
void AvCLUIRaster::getRow(float *row, const int X, const int Y,
					const int Num, const int Incr)
{
int x=X, y=Y, num=Num, incr=Incr;
int x1, w = width();

	if( (x < 0) || ( x >= w))
		return;
	if( (y < 0) || ( y >= height()))
		return;

	if(incr == 0)
		incr = 1;

	if(num <= 0)
		num = w;

	x1 = x + incr*(num-1);	// endpoint.

	if(incr > 0) // Left to right.
	{	if(x1 >= w)	// End point is > start point.
			x1 = w -1;
		while(x <= x1)
		{	*row++ = getElement(x, y);
			x += incr;
		}
	}
	else	// Right to left.
	{	if( x1 < 0)	// End point is < start point.
			x1 = 0;
		while(x >= x1)
		{	*row++ = getElement(x, y);
			x += incr;
		}
	}
}
#else
/* Return a row of data from the given position. If Length is 0, return
as much of the row as will fit.
*/
void AvCLUIRaster::getRow(float *row, const int x, const int y,
			  const int Length, const int stride)
{	int length = (Length <= 0) ? width()-x : Length;
	accessor_->getRow(row, x, y, length, stride);
}
#endif

//
void AvCLUIRaster::getRegion(float *data, const int x, const int y,
			     const unsigned int width,
			     const unsigned int nrows,
			     const int xstride, const int ystride)
{int rank = accessor_->rank();

	AvIPosition start(rank, 0);
	start(AvAccessor::XAXIS) = x;
	start(AvAccessor::YAXIS) = y;
	if(rank > 2)
		start(AvAccessor::ZAXIS) = accessor_->getPlane();
	AvIPosition shape(rank, 1);
	shape(AvAccessor::XAXIS) = width;
	shape(AvAccessor::YAXIS) = nrows;
	AvIPosition stride(rank, 1);
	stride(AvAccessor::XAXIS) = xstride;
	stride(AvAccessor::YAXIS) = ystride;

	accessor_->getSlice(data, start, shape, stride);
}


// Change the slice index for the region.
// newSliceIndex is in DS
void AvCLUIRaster::setSlice(const int newSliceIndex)
{
	accessor_->setPlane(newSliceIndex, FALSE);
}

AvCLUIRaster::~AvCLUIRaster()
{
	accessor_->unlockResource();
	accessor_->unref();
}

void AvCLUIRaster::getBoundingBox(AvCanvas &c, AvAllotment &r)
{
	r.value(AvDimensionX, width());
	r.value(AvDimensionY, height());
	AvRaster::getBoundingBox(c, r);
}

///////////////////////////////////////////////////////////////////////////
AvCLUIRaster::axisInfo::axisInfo(AvAccessor *acc)
{
	accessor_ = acc;
	AvResource::Ref(accessor_);
	AvString abbrev;
	for(int i=0; i< 2 ; i++)
		setUnits(i, units_[i], abbrev);
}

AvCLUIRaster::axisInfo::~axisInfo()
{
	AvResource::UnRef(accessor_);
	accessor_ = NULL;
}

#define ABS(x) (((x)>=0)?(x):-(x))

// Initialize the units_ var. 
void AvCLUIRaster::axisInfo::setUnits(const int axis, AvString &units,
				      AvString &abbrev)
{ AvRaster::axisInfo::PRINTFORMAT apfmt = printFormat(axis);
  AvWPosition minWorld = accessor()->minWorld();
  AvWPosition maxWorld = accessor()->maxWorld();
  double minW = minWorld(axis);
  double maxW = maxWorld(axis);

//	if (apfmt == AvDataSet::HMS)
	if (apfmt == AvRaster::axisInfo::HMS)
	{	// PGPlot decides how to print HMS
	}
//	else if (apfmt == AvDataSet::DMS)
	else if (apfmt == AvRaster::axisInfo::DMS)
	{	// PGPlot decides how to print DMS
	}
	else
	{	// Otherwise, pick representative value on axis, get the units
		// that form the best fit, and use those for tick boundaries

		double min = ABS(minW);
		double max = ABS(maxW);
		double representativeValue = MAX(min, max);

		units = accessor()->axisUnits(  axis,
						representativeValue,
						abbrev);
	}
}

const char *AvCLUIRaster::axisInfo::label(const int axis)const
{
	return accessor_->axisName(axis);
}

const char *AvCLUIRaster::axisInfo::unitString(const int axis)const
{
	return accessor_->axisUnitsName(axis);
}

// Return the print format for the axis.
AvRaster::axisInfo::PRINTFORMAT
		AvCLUIRaster::axisInfo::printFormat(const int axis)const
{ AvDataSet::AXISPRINTFORMAT fmt = accessor_->axisPrintFormat(axis);

  AvString units = accessor_->axisUnitsName(axis);

  // Convert from AvDataSet::PRINTFORMAT to AvRaster::PRINTFORMAT.
  // They are really the same, but it could be possible they'd diverge.
  switch(fmt) 
    {
    case AvDataSet::DEFAULT:
      return AvRaster::axisInfo::DEFAULT;
    case AvDataSet::HMS:
      if (units == "degrees")
	return AvRaster::axisInfo::HMS;
      else if (units == "radians")
	return AvRaster::axisInfo::HMS_RAD;
      else
	return AvRaster::axisInfo::UNKNOWN;
    case AvDataSet::DMS:
      if (units == "degrees")
	return AvRaster::axisInfo::DMS;
      else if (units == "radians")
	return AvRaster::axisInfo::DMS_RAD;
      else
	return AvRaster::axisInfo::UNKNOWN;
    default:
      return AvRaster::axisInfo::UNKNOWN;
    }
}

int AvCLUIRaster::axisInfo::length(const int axis)const
{
	return accessor_->getAxisLength(axis);
}

Boolean AvCLUIRaster::axisInfo::validIndex(const int axis, const int i)const
{
	return ((i >= 0) && ( i < accessor_->getAxisLength(axis)))
		? TRUE : FALSE;
}

double AvCLUIRaster::axisInfo::XYtoWorld(const int axis,
					 const int x, const int y) const
{double val;

	accessor_->XYZToWorld(x, y, axis, val);
	return val;
}

// Convert to/from scaled value 
double AvCLUIRaster::axisInfo::minValue(const int axis)const
{AvWPosition minW = accessor()->minWorld();
 double min = minW(axis);

	double smin = accessor()->valueInUnits(axis,
					min,
					units_[axis].chars());
#if 0
	cout	<< "Axis " << axis << " Min " << min << " scaled min " << smin
		<< " units " << units_[axis] << endl;
#endif
	return smin;
}

double AvCLUIRaster::axisInfo::maxValue(const int axis)const
{AvWPosition maxW = accessor()->maxWorld();
 double max = maxW(axis);

	double smax = accessor()->valueInUnits(axis,
					max,
					units_[axis].chars());
#if 0
	cout	<< " Axis " << axis << " Max " << max << " scaled max " << smax
		<< " units " << units_[axis] << endl;
#endif
	return smax;
}

// Return pointer to a buffer containing a string representing the
// value of the axis at the given position.
const char *AvCLUIRaster::axisInfo::valueStr(const int axis, const double val)
									const
{
	return accessor_->axisValueUnits(axis, val).chars();
}

const char *AvCLUIRaster::axisInfo::title()const
{
	if(accessor_->validAxis(ZAXIS))
		return accessor_->axisName(ZAXIS);
	else
		return NULL;
}

