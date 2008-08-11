//# Copyright (C) 1995-97 Board of Trustees of the University of Illinois
//#
//# This software, both binary and source, is copyrighted by The
//# Board of Trustees of the University of Illinois. Ownership
//# remains with the University. You should have received a copy 
//# of a licensing agreement with this software. See the file 
//# "AIPSVIEW_COPYRIGHT", or contact the University at this address:
//#
//#     The NCSA AipsView Visualization System
//#     National Center for Supercomputing Applications
//#     University of Illinois
//#     405 North Mathews Ave.
//#     Urbana, IL 61801
//#     aipsview@ncsa.uiuc.edu
//#---------------------------------------------------------------------------
// AvCoordinateSystem

#ifndef __AvCoordinateSystem_h__
#define __AvCoordinateSystem_h__

#include <AvString.h>
#include <AvRegex.h>
#include <AvWPosition.h>

class AvCelestialProjection;
class AvLinearMap;

//  <summary>
//  Coordinate System mapping class that is based on although independent of
//  the AIPS++ Coordinate System Classes.
//  </summary>
//
//  
//
//
// <todo asof="1996/08/01">
//   <li> pcmatrix testing
// </todo>
//

class AvCoordinateSystem
{
public:

  // Build a linear, default coordinate system
  AvCoordinateSystem(const int nAxes);


  // Build the coordinate system from the provided information
  // return NULL if the information is inconsistent.
  AvCoordinateSystem(const int nAxes,
		     const AvString * ctypes,
		     const double * crvals,
		     const double * crpixs,
		     const double * cdelts,
		     const double * pcmatrix,
		     const double * projps,
		     const double longpole);
  
  // copy constructor and assignment both use copy semantics
  // <group>
  AvCoordinateSystem(const AvCoordinateSystem& cs);
  AvCoordinateSystem & operator = (const AvCoordinateSystem& cs);
  // </group>

  ~AvCoordinateSystem();

  // Return the number of axes present in the system
  int nAxes() const { return nAxes_; }

  // map a point from world coordinates to array coordinates
  // <group>
  int world2ijk(const AvWPosition & world, AvWPosition & ijk);
  int world2ijk(const double * world, double * ijk);
  // </group>

  // map a point from array coordinates to world coordinates
  // <group>
  int ijk2world(const AvWPosition & ijk, AvWPosition & world);
  int ijk2world(const double * ijk, double * world);
  // </group>

  // Return the name of the axis by index
  const char * axisName(int axisNdx) const 
  { return axisNames_[axisNdx].chars(); }
  const char * axisUnits(int axisNdx) const
  { return axisUnits_[axisNdx].chars(); }
  const char * axisMeasurementName(int axisNdx) const
  { return axisMeasurementNames_[axisNdx].chars(); }

  // Return the (0-based) index of the longitude axis or
  // -1 if no projection system is being used.
  int longitudeAxis() const { return longAxis_; }

  // Return the (0-based) index of the latitude axis or
  // -1 if no projection system is being used.
  int latitudeAxis() const { return latAxis_; }

  // return the longpole value
  double longpole() const { return longpole_; }

  // set axis units and measurementNames.  This can vary
  // with file type, so AvCS can make no assumptions
  void setAxisUnits(int axis, const AvString& units, const AvString& mname);
  
  // Return a string containing value and units for given axes.  sigfigs
  // is the number of significant digits in the smallest breakdown of
  // the value.
  AvString valueUnitsStr(int axis, double value, int sigfigs = 4) const;

  // Return a string containing the units that would be used
  // to print a given value
  AvString unitsForValueStr(int axis, double value, const AvString& units) const;

  // Return converted valueIn on axis to units
  double valueInUnits(int axis, double valueIn, const AvString& units) const;
  
private:

  int nAxes_;

  AvCelestialProjection * cp_;
  AvLinearMap * lm_;

  double * crvals_;

  double longpole_;

  int longAxis_;
  int latAxis_;

  // e.g., RA, FREQ
  AvString * axisNames_;
  // e.g., SIN, LSR
  AvString * axisCoordSysNames_;

  // e.g., seconds, hertz
  AvString * axisUnits_;
  // e.g., right ascension, frequency
  AvString * axisMeasurementNames_;
};

#endif
