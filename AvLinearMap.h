//# AvLinearMap.h: Convert a location via a linear transformation
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
//# $Id: AvLinearMap.h,v 19.1 2003/09/10 05:03:27 mcalabre Exp $

#ifndef __AvLinearMap_h__
#define __AvLinearMap_h__

#include "AvString.h"
#include <wcslib/lin.h>

// <summary>
// Apply an offset (CRPIX), arbitrary matrix (PC), and scaling (CDELT).
// </summary>
//
// <etymology>
// This class performs a linear parts of the transformation required to turn
// a world coordinate into a pixel one, and vice versa.
// </etymology>
//
// <synopsis>
// This class handles the linear part of the transformation of a pixel 
// coordinate to a world coordinate. Starting with the pixel coordinate
// <src>(i,j,...k)</src> it:
// <ol>
//    <li> Subtracts the value of a reference pixel (CRPIX in FITS 
//         terminology).
//    <li> Multiples by an arbitrary matrix (the PC matrix in FITS). While
//         this matrix will normally be diagonal, this allows arbitrary
//         rotations/skews/... to be entered.
//    <li> Multiples each output value by an arbitrary scaling factor (CDELT
//         in FITS). This scaling factor is intended to convert pixels into
//         physical units.
// </ol>
// After these transformations, the pixel values have been turned into relative
// physical coordinates, but still in the map plane. A (usually) non-linear
// projection is still needed, typically a 
// <linkto class="AvCelestialProjection">AvCelestialProjection</linkto>.
// <note role="hint">
// At present, it is required that the number of world and pixel axes be the
// same.
// </note>
// The computations are carried out by the WCSLIB library, written by Mark
// Calabretta.
// </synopsis>
//
// <motivation>
// The linear part of coordinate transformations required for image 
// coordinates.
// </motivation>
//

class AvLinearMap
{
public:
  // Create with "naxis" axes, CRPIX of 0, CDELT of 1, and a diagonal PC
  // matrix.
  AvLinearMap(int naxis=1);
  
  // Create the linear transformation from the supplied crpix and cdelt. The
  // PC matrix is the unit matrix. crpix and cdelt must have the same number
  // of elements.
  AvLinearMap(const double * crpix, const double * cdelt, int naxis);
  
  // Create a linear transformation, supplying all of crpix, cdelt, and pc.
  // The vectors must be of the same length "n", and the number of rows and
  // columns in the matrix must also be n.
  AvLinearMap(const double * crpix, const double * cdelt, const double * pc, int naxis);
  
  // Make a copy of other. Copy sematics, NOT reference semanticsl
  // <group>
  AvLinearMap(const AvLinearMap &other);
  AvLinearMap &operator=(const AvLinearMap &other);
  // </group>

  ~AvLinearMap();

  // Returns the number or world axes, which for this class is also the
  // number of pixel axes.
  int nWorldAxes() const;
  int naxis() const;

  // Convert world coordinates to pixel coordinates (forward), or pixel 
  // coordinates to world (reverse). If the conversion works True is returned,
  // otherwise False is returned and errorMsg is set.
  // <group>
  int pixel2world(const double * pixel, double * world); 
  int world2pixel(const double * world, double * pixel);

  // </group>

  // Retrieve the value of crpix, cdelt, and pc.
  // <group>
  double crpix(int axis) const;
  double cdelt(int axis) const;
  double pc(int i, int j) const;
  // </group>
  
private:
  linprm linprm_;

  void set_linprm();
};

#endif
