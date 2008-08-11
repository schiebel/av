//# AvCelestialProjection.h: Map a position on the celestial sphere to a plane.
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
//# $Id: AvCelestialProjection.h,v 19.1 2003/09/10 05:03:16 mcalabre Exp $
//#---------------------------------------------------------------------------

#ifndef AvCelestialProjection_h
#define AvCelestialProjection_h

#include "AvString.h"
#include <wcslib/cel.h>

// <summary>
// Map a position on the celestial sphere to a plane.
// </summary>

//
// <synopsis>
// This class is used to interconvert between longitude and latitude (RA/DEC)
// coordinates on the celestial sphere ("world" coordinates), and those same
// coordinates projected onto a plane ("image" coordinates). The possible
// projections are enumerated in the WCSProjections enum.
//
// The units on both the celestial sphere and after the projections are
// degrees, <b>not</b> radians. This transformation is usually combined with a
// linear transformation (provided by 
// <linkto class="AvLinearMap">AvLinearMap</linkto>) to turn the
// image coordinates into true pixel coordinates. This combination usually
// occurs in the <linkto class="AvCoordinateSystem">AvCoordinateSystem</linkto>
// class.
// 
// In FITS terminology, this class applies CTYPE (the type of the projection),
// and CRVAL (the coordinate value of the reference position).
//
// This computations for this class are carried out by WCSLIB, written by
// Mark Calabretta.
// </synopsis>
//
// <motivation>
// Coordinate systems for images.
// </motivation>
//
// <todo asof="1996/05/23">
//   <li> Support for COBE QSC and QSC cubic-face projections
//   <li> Move <src>WCSProjections</src> out of the global namespace?
// </todo>

class AvCelestialProjection
{
public:

  // Cartesian default.
  AvCelestialProjection();

  // Constructors for the Celestial projection which take a subset of
  // the following:
  //
  // <ul>
  // <li>projShortName - 3 letter name (e.g., "SIN") of projection
  // <li>refLong       - longitude reference point
  // <li>refLat        - latitude reference point
  // <li>longPole      - pole for rotations
  // <li>latPole       - generally not passed, latitude pole
  // <li>projp         - array of up to 10 projection parameters
  // </ul>
  //
  // <group>
  AvCelestialProjection(const AvString & projShortName,
                        double refLong, double refLat,
			double longPole = 999.0, double latPole = 999.0,
			const double *projp = 0);
  AvCelestialProjection(const AvCelestialProjection &other);
  AvCelestialProjection &operator=(const AvCelestialProjection &other);
  // </group>

  // destructor
  ~AvCelestialProjection();

  // Map a pixel position to a world position (degrees)
  int world2celestial(double x, double y, double &lng, double &lat);
  // Map a world position to a pixel position
  int celestial2world(double lng, double lat, double &x, double &y);

  // Print the contents of the celprm struct (for debugging).
  int celprt(void);

private:
  celprm celprm_;
};

#endif


