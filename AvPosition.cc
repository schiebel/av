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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvPosition.cc,v 19.0 2003/07/16 05:47:04 aips2adm Exp $
//
// $Log: AvPosition.cc,v $
// Revision 19.0  2003/07/16 05:47:04  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:48  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/07 22:38:01  hravlin
// *** empty log message ***
//
// Revision 17.0  2001/11/12 19:42:17  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:24  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:47  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:08  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:54  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:28  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:30  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:18  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:26:48  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.2  1997/08/01 21:40:40  hr
// DCC found some unused code.
//
// Revision 8.1  1997/08/01 21:16:30  hr
// Rewrite to use accessor rather than AvImageViewItem. Compound number is gone.
//
// Revision 8.0  1997/02/20 03:18:51  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/02/05 17:56:29  hr
// The __P problem.
//
// Revision 7.2  1996/12/12 09:11:50  droberts
// Final update from monet archive.
//
// Revision 1.6  1996/09/25 20:50:16  hr
// When a new AvPosition is created from an imageViewItem, the current
// cursor position is used to initialize the position values.
//
// Revision 1.5  1996/09/20 20:19:35  pixton
// Prefixed all classes with Av
//
// Revision 1.4  1996/09/20  19:34:44  hr
// Removed unaccessible "break"s.
//
// Revision 1.3  1996/09/18  19:31:56  hr
// Made some arguments const &/or pass by reference.
//
// Revision 1.2  1996/09/10 16:55:48  hr
// Initial changes for integration with AvAccessor & AvDataSet.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.10  1996/06/18  18:46:28  pixton
//  Copyright Update
//
//  Revision 1.9  1996/04/08  15:51:09  hr
//  getlabel() was incorrectly checking the rank.
//
//  Revision 1.8  1996/03/28  22:32:00  hr
//  getLabel() now checks for a valid index.
//
//  Revision 1.7  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.7  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.6  1995/09/01  14:21:51  hr
//  Replaced quotes around math.h with '<>'.
//
//  Revision 1.5  1995/07/27  14:18:57  hr
//  Removed 'empty' if statements in assign to make g++ happy.
//
//  Revision 1.4  1995/07/20  17:19:13  hr
//  Replaced use of HUGE_VAL with MAXVALUE and BLANKEDVALUE.
//
//  Revision 1.3  1995/05/05  14:51:29  hr
//  Added AvPosition(const AvPosition &).
//
//  Revision 1.2  1995/05/02  19:16:27  hr
//  Forgot an include.
//
//  Revision 1.1  1995/05/02  19:06:39  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include <AvPosition.h>
#include <AvIPosition.h>

#if 0
//#if defined(__GNUC__) && defined(__P) && !defined(__linux__)
//#undef __P
//#endif
#include <math.h>

static Quark invalidUnits_ = 0;
Quark AvCompoundNumber::invalidUnits()
{
	if(invalidUnits_ == 0)
		invalidUnits_ = XrmStringToQuark("Invalid_Units");
	return invalidUnits_;
}

AvCompoundNumber::AvCompoundNumber()
{
	initialize(0, NULL);
}

AvCompoundNumber::AvCompoundNumber(const double v, const char *units)
{
	initialize(v, units);
}

void AvCompoundNumber::initialize(const double val, const char *units)
{
	value_ = val;
	if(units != NULL)
		units_ = XrmStringToQuark(units);
	else
		units_ = invalidUnits();
}

AvCompoundNumber::AvCompoundNumber(const AvCompoundNumber &n)
{
	value_ = n.value_;
	units_ = n.units_;
}

void AvCompoundNumber::setInvalid()
{
	units_ = invalidUnits();
}

AvCompoundNumber::~AvCompoundNumber()
{
}

char *AvCompoundNumber::getUnitString()const
{
	return quarkToString(units_);
}

char *AvCompoundNumber::quarkToString(const Quark q)
{
	return XrmQuarkToString(q);
}

Boolean AvCompoundNumber::compatible(const AvCompoundNumber &n)const
{
	return (isValid() && (units_ == n.units_));
}

Boolean AvCompoundNumber::compatible(const AvCompoundNumber *n)const
{
	if(n != NULL)
		return compatible(*n);
	else
		return FALSE;
}

void AvCompoundNumber::limit(const double Min, const double Max)
{double min, max;

	if(Min > Max)
	{	min = Max;
		max = Min;
	}
	else
	{	min = Min;
		max = Max;
	}
	if(value_ < min)
		value_ = min;
	else
	if(value_ > max)
		value_ = max;
}

Boolean AvCompoundNumber::assign(const AvCompoundNumber &n)
{
	if(! compatible(n))
		return FALSE;
	else
	{	setValue(n.getValue());
		return TRUE;
	}
}

#if 0
// Used for only debugging now.
char *AvCompoundNumber::format(char *bufr)
{
static char buffer[128], *out, *in;
float hours, minutes, seconds, val;
int iHours, iMinutes;
char sign;

  out = (bufr == NULL) ? buffer : bufr;
  in = getUnitString();
  val = getValue();
  if(!strncmp ("RA", in, 2)) {
    // RA is always positive, so don't worry about the sign...
    hours = val / 15.0;

    minutes = 60 * (hours - floor (hours));
    seconds = 60 * (minutes - floor (minutes));
    sprintf (out, "%dh %dm %.2fs", (int) hours,  (int) minutes, seconds);
  }
  else if (!strncmp ("DEC", in, 3)) {
    if (val < 0) {
      sign = '-';
      val = - val;
    }
    else
      sign = '+';
    hours = val;
    iHours = (int) val;
    minutes = (hours - iHours) * 60;
    iMinutes = (int) minutes;
    seconds = (minutes - iMinutes) * 60;
    if (sign == '-')
      sprintf (out, "%c%dd %dm %.2fs", sign, iHours, iMinutes, seconds);
    else
      sprintf (out, "%dd %dm %.2fs", iHours, iMinutes, seconds);
  }
  else if (!strncmp ("VEL", in, 3))
    sprintf (out, "%.3f km/s", val / 1000.0);
  else if (!strncmp ("FRE", in, 3))
    sprintf (out, "%.3f GHz", val / 1000000000);
  // we don't know how to format this, so just return string
  else {
    sprintf (out, "%f", val);
  }
//  return (strlen (out));
  return out;
}
#endif

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/
#endif

/*****************************************************************/
/*			AvPosition				 */
/*****************************************************************/

/* Give a position in 3 space including units.
*/

#if 0
AvPosition::AvPosition()
{
}
#endif

AvPosition::AvPosition(const AvPosition &pos)
{
	values_ = pos.values_;
	accessor_ = pos.accessor_;
	if(accessor_ != NULL)
		accessor_->ref();
}

#if 0
static inline const char *getLabel(Accessor *ac, const int w)
{
	if((ac == NULL) || (w >= ac->rank()))
		return NULL;
	return ac->axisName(w);
}
#endif

AvPosition::AvPosition( AvAccessor *ac)
{
	accessor_ = ac;
	if(ac == NULL)		// Error!!
		return;
	accessor_->ref();
	values_.resize(accessor_->rank());
	values_ = accessor_->minRegion();	// Init to valid values.
}

AvPosition::~AvPosition()
{
	if(accessor_ != NULL)
		accessor_->unref();
}

/* Return a pointer to a new position with units from ac, and values
from pos. Returns NULL if the x & y can't be set from pos.
Used by AvImageWindow callbacks to convert coordinates from one window
to another.
*/

AvPosition *AvPosition::newPosition(AvAccessor *ac, const AvPosition &pos)
{
AvPosition *np = new AvPosition(ac);

	if( np->assign(pos) < 2)
	{	delete np;
		return NULL;
	}
	return np;
}

void AvPosition::getXYZValues(double &x, double &y, double &z)const
{
	x = values_(0);
	y = values_(1);
	if(values_.nelements() > 2)
		z = values_(2);
}

void AvPosition::setValues(const AvWPosition &val, const int maxn)
{int l0 = values_.nelements();
 int l1 = val.nelements();

	// Copy the min necessary or possible.
	if((maxn > 0) && ( maxn < l0))
		l0 = maxn;

	if(l0 == l1)
		values_ = val;
	else
	{	if(l0 > l1)
			l0 = l1;
		for(int i=0; i< l0; i++)
			values_(i) = val(i);
	}
}

#if 0
void AvPosition::setXYZValues(const double x, const double y, const double z)
{
	values_(0) = x;
	values_(1) = y;
	if(values_.nelements() > 2)
		values_(2) = z;
}
#endif

/* Assign the values of pos to corresponding values of this.
   (Entries are copied to entries with the same units).

Returns the number of consecutive entries copied to this starting from 0.
(eg. If 2, then X & Y got copied, 3 if X, Y and Z).
*/
int AvPosition::assign(const AvPosition &pos)
{ int start, stop, oldi;
  int len0 = length();
  int len1 = pos.length();
  AvIPosition used(len1, 0);	// Keeps track of entries copied to this.
				// (Could have multiple axes with same units).
	start = 0;
	stop = len1-1;
	oldi = 0;		// Counts # of consecutive entries copied
				// to this.

	// Foreach entry in this, compare units to entries in pos. If
	// they are the same, copy the value and mark the entry used.
	for(int i = 0; i< len0; i++)
	{const char *unts0 = units(i);

		if(unts0 == NULL)		// !! ERROR !!
			continue;
		for(int j=start; j<=stop; j++)
		{const char *unts1 = pos.units(j);
			if(used(j) != 0)
				continue;	// Already copied.
			if(unts1 == NULL)
				continue;	// !! ERROR !!
			if(strcmp(unts0, unts1) == 0)
			{	values_(i) = pos.values_(j);
				// Skip over used entries if at start or end.
				if(j == start)
					start += 1;
				else
				if(j == stop)
					stop -= 1;
				// Count # of consecutive entries from start.
				if(i == oldi)
					oldi += 1;
				used(j) = 1;
				break;		// Done with loop.
			}
		}
	}
	return oldi;
}

#if 0
// If either is NULL, then not same.
Boolean AvPosition::areSame(const int i, const char *s1)const
{ const char *s0 = units(i);

	if((s0 == NULL) || (s1 == NULL))
		return FALSE;
	if(strcmp(s0, s1) == 0)
		return TRUE;
	else
		return FALSE;
}
#endif

// Checks the first 2 entries 
int AvPosition::validXY()const
{ AvIPosition ip(length());

	accessor_->WorldToXYZ(values_, ip);
	return accessor_->validIJK(ip(0), ip(1));
}

// Checks the first 3 entries 
int AvPosition::validXYZ()const
{int l = length();

	if( l <= 2)
		return 0;		// FALSE if only 2 dimensions.

	AvIPosition ip(l);

	accessor_->WorldToXYZ(values_, ip);
	return accessor_->validIJK(ip(0), ip(1), ip(2));
}

// Convert x, y, and z from indexes in region to world values and store.
void AvPosition::XYZToWorld(const int x, const int y, const int z)
{
	AvWPosition wp = accessor_->XYZToWorld(x, y, z);
	int l = length();
	if(l > 3)
		l = 3;
	for(int i=0; i< l; i++)
		values_(i) = wp(i);
}
