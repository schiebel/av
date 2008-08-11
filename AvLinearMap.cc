//# <ClassFileName.h>: this defines <ClassName>, which ...
//# Copyright (C) 1996-2003
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
//#
//# $Id: AvLinearMap.cc,v 19.1 2003/09/10 05:03:25 mcalabre Exp $
//#---------------------------------------------------------------------------

#include "AvLinearMap.h"
#include <wcslib/lin.h>
#include <stdlib.h>
#include <strings.h>

//----------------------------------------------------------------------------

AvLinearMap::AvLinearMap(int naxis)
{
  linprm_.flag = 0;
  linini(1, naxis, &linprm_);
  set_linprm();
}

//----------------------------------------------------------------------------

AvLinearMap::AvLinearMap(const double * crpix,
			 const double * cdelt,
			 int naxis)
{
  linprm_.flag = 0;
  linini(1, naxis, &linprm_);

  for (int i = 0; i < naxis; i++) {
    linprm_.crpix[i] = crpix[i];
    linprm_.cdelt[i] = cdelt[i];
  }

  set_linprm();
}

//----------------------------------------------------------------------------

AvLinearMap::AvLinearMap(const double * crpix,
			 const double * cdelt,
			 const double * pc,
			 int naxis)
{
  linprm_.flag = 0;
  linini(1, naxis, &linprm_);

  int ij = 0;
  for (int i = 0; i < naxis; i++) {
    for (int j = 0; j < naxis; j++, ij++) {
      linprm_.pc[ij] = pc[ij];
    }

    linprm_.crpix[i] = crpix[i];
    linprm_.cdelt[i] = cdelt[i];
  }

  set_linprm();
}

//----------------------------------------------------------------------------

AvLinearMap::AvLinearMap(const AvLinearMap &other)
{
  lincpy(1, &(other.linprm_), &linprm_);
  set_linprm();
}

//----------------------------------------------------------------------------

AvLinearMap &AvLinearMap::operator=(const AvLinearMap &other)
{
  if (this != &other) {
    lincpy(1, &(other.linprm_), &linprm_);
    set_linprm();
  }
  return *this;
}

//----------------------------------------------------------------------------

AvLinearMap::~AvLinearMap()
{
  linfree(&linprm_);
}

//----------------------------------------------------------------------------

int AvLinearMap::nWorldAxes() const
{
  return linprm_.naxis;
}

//----------------------------------------------------------------------------

int  AvLinearMap::naxis() const
{
  return linprm_.naxis;
}

//----------------------------------------------------------------------------

int AvLinearMap::world2pixel(const double *world,
			     double *pixel)
{
  if (int err = linx2p(&linprm_, 1, 1, world, pixel)) {
    cerr << "linx2p error: " << lin_errmsg[err] << endl;
    return 1;
  }

  return 0;
}

//----------------------------------------------------------------------------

int AvLinearMap::pixel2world(const double *pixel,
			     double *world)
{
  if (int err = linp2x(&linprm_, 1, 1, pixel, world)) {
    cerr << "linp2x error: " << lin_errmsg[err] << endl;
    return 1;
  }

  return 0;
}

//----------------------------------------------------------------------------

double AvLinearMap::crpix(int axis) const
{
  return linprm_.crpix[axis];
}

//----------------------------------------------------------------------------

double AvLinearMap::cdelt(int axis) const
{
  return linprm_.cdelt[axis];
}

//----------------------------------------------------------------------------

double AvLinearMap::pc(int i, int j) const
{
  return linprm_.pc[j*linprm_.naxis + i];
}

//----------------------------------------------------------------------------

void AvLinearMap::set_linprm()
{
  if (int err = linset(&linprm_)) {
    cerr << "linset error: " << lin_errmsg[err] << endl;
  }
}
