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
//# $Id: AvCelestialProjection.cc,v 19.1 2003/09/10 05:02:59 mcalabre Exp $
//---------------------------------------------------------------------------

#include "AvCelestialProjection.h"
#include "AvString.h"
#include <wcslib/cel.h>
#include <wcslib/prj.h>
#include <stdlib.h>

//----------------------------------------------------------------------------

AvCelestialProjection::AvCelestialProjection()
{
  celini(&celprm_);

  strcpy(celprm_.prj.code, "CAR");

  if (int err = celset(&celprm_)) {
    cerr << "celset error: " << cel_errmsg[err] << endl;
  }
}

//----------------------------------------------------------------------------

AvCelestialProjection::AvCelestialProjection(
        const AvString &projName,
	double refLong, double refLat,
        double longPole, double latPole,
        const double *projp)
{
  celini(&celprm_);

  celprm_.ref[0] = refLong;
  celprm_.ref[1] = refLat;
  celprm_.ref[2] = longPole;
  celprm_.ref[3] = latPole;

  strcpy(celprm_.prj.code, projName.chars());

  if (projp) {
    double *dp = celprm_.prj.pv;
    for (int i = 0; i < 10; i++) {
      *(dp++) = *(projp++);
    }
  }

  if (int err = celset(&celprm_)) {
    cerr << "celset error: " << cel_errmsg[err] << endl;
  }
}

//----------------------------------------------------------------------------

AvCelestialProjection::AvCelestialProjection(
        const AvCelestialProjection &other)
{
  celprm_ = other.celprm_;
}

//----------------------------------------------------------------------------

AvCelestialProjection &AvCelestialProjection::operator=(
        const AvCelestialProjection &other)
{
  if (this != &other) {
    celprm_ = other.celprm_;
  }

  return *this;
}

//----------------------------------------------------------------------------

AvCelestialProjection::~AvCelestialProjection()
{
}

//----------------------------------------------------------------------------

int AvCelestialProjection::celestial2world(
        double lng,
        double lat,
        double &x,
        double &y)
{
  int    stat;
  double phi, theta;

  if (cels2x(&celprm_, 1, 1, 1, 1, &lng, &lat, &phi, &theta, &x, &y, &stat)) {
    cerr << "cels2x error: " << cel_errmsg[stat] << endl;
    return 1;
  }

  return 0;
}

//----------------------------------------------------------------------------

int AvCelestialProjection::world2celestial(
        double x,
        double y,
        double &lng,
        double &lat)
{
  int    stat;
  double phi, theta;

  if (celx2s(&celprm_, 1, 1, 1, 1, &x, &y, &phi, &theta, &lng, &lat, &stat)) {
    cerr << "celx2s error: " << cel_errmsg[stat] << endl;
    return 1;
  }

  return 0;
}


//----------------------------------------------------------------------------

int AvCelestialProjection::celprt(void)
{
  return ::celprt(&celprm_);
}
