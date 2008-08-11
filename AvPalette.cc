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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvPalette.cc,v 19.0 2003/07/16 05:47:42 aips2adm Exp $
//
// $Log: AvPalette.cc,v $
// Revision 19.0  2003/07/16 05:47:42  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:19  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/03 20:16:11  hravlin
// Include strings.h
//
// Revision 17.0  2001/11/12 19:42:49  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:55  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:56  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:48  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.4  1999/09/29 16:50:09  hravlin
// Fixed compiler warnings.
//
// Revision 13.3  1999/09/17 15:19:16  hravlin
// Another egcs warning.
//
// Revision 13.2  1999/09/16 20:09:39  hravlin
// Removed egcs and IRIX -64 compiler warnings.
//
// Revision 13.1  1999/09/15 21:19:36  hravlin
// Major changes to support TrueColor visuals.
//
// Revision 13.0  1999/08/10 18:40:36  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:43  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:18  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:23  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.3  1998/06/19 14:15:54  hr
// Added string.h to list of included files.
//
// Revision 9.2  1998/06/11 17:35:57  hr
// Made AvColormapSegment value a signed long.
//
// Revision 9.1  1998/05/04 20:49:52  hr
// In AvColormapSegment::setValue(), made value unsigned long.
//
// Revision 9.0  1997/08/25 21:30:04  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.1  1997/04/30 20:12:09  hr
// Added enable/disableCheckAllocated() and made checkAllocated non virtual
// function that calls virtual checkAllocated_().
//
// Revision 8.0  1997/02/20 03:18:41  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/02/09 00:56:25  hr
// Fixed signed/unsigned comparision.
//
// Revision 7.2  1996/12/12 06:48:46  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.6  1996/06/18  18:46:23  pixton
//  Copyright Update
//
//  Revision 1.5  1996/03/02  22:16:50  hr
//  Now use case insensitive string compares on color name comparisions.
//
//  Revision 1.4  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/06/22  19:01:42  hr
//  Worked on physicalToVirtual and allocColorName.
//
//  Revision 1.2  1995/04/11  14:10:54  hr
//  Removed an unreachable break in a case statement.
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvPalette.cc
*/
#include <string.h>
#include <strings.h>			// SGI when POSIX_SRC is defined
#include <X11/Intrinsic.h>		// Boolean
#include "AvPalette.h"

AvColortableEntry::AvColortableEntry()
{	red_ = green_ = blue_ = 0.0;
	name_ = "";
	index_ = 0;
	pixel_ = 0;
	flags_ = 0;
	state_ = UA;
}

void AvColortableEntry::operator=(const AvColortableEntry &e)
{
	index_ = e.index_;
	pixel_ = e.pixel_;
	red_ = e.red_;
	green_ = e.green_;
	blue_ = e.blue_;
	name_ = e.name_;
	state_ = e.state_;
	flags_ = e.flags_;	// Copy rather set as dirty since this
				// is used by expandTable.
}
////////////////////////////////////////////////////////////////

AvPalette::AvPalette( const int maxcolors)
{
	maxcolors_ = 0;
	readonly_ = FALSE;
	decomposedIndex_ = FALSE;
	checkAllocatedDisabled_ = 0;
	needCheck_ = FALSE;

	rwcells_ = tablelength_ = 0;	// Initially, all cells are UA.
	colors_ = NULL;
	map_ = NULL;
	expandTable(maxcolors);
}

AvPalette::~AvPalette()
{
	delete [] colors_;
}

// Make new color/map tables and copy any existing before deleting.
// Does nothing if size is <= existing size.
void AvPalette::expandTable(const int size)
{ int start = 0, i;

	if(size <= tablelength_)
		return;

	AvColortableEntry *tbl = new AvColortableEntry[size];
	unsigned short *nmap = new unsigned short[size];

	if(colors_ != NULL)	// Copy existing.
	{
		for(i=0; i< tablelength_; i++)
		{	tbl[i] = colors_[i];
			nmap[i] = map_[i];
		}
		delete [] colors_;
		delete [] map_;
		start = tablelength_;
	}

	// Initial mappings are 0->0, 1->1, etc. Subclasses may change
	// mappings.
	for(i=start; i< size; i++)
	{	tbl[i].setIndex(i);
		nmap[i] = i;
	}
	colors_ = tbl;
	map_ = nmap;
	tablelength_ = size;
}

// Look for an entry that has the name. The only possible choices will
// be the RO cells at the end of map_;
AvColortableEntry *AvPalette::findEntryByName(const char *name)const
{
	if( (name == NULL) || (strlen(name) == 0))
		return NULL;

	for(int i = rwcells_; i < tablelength_; i++)
	{ int indx = map_[i];
	  AvColortableEntry *e = &colors_[indx];
		if( e->state() != AvColortableEntry::RO)
			continue;
		if( strcasecmp(e->name(), name) == 0)
			return e;
	}
	return NULL;
}

// Find an unallocated slot. Returns the index into colors_ or -1.
AvColortableEntry *AvPalette::findEmpty()const
{
	// Look for an empty slot.
	for(int i = rwcells_; i < tablelength_; i++)
	{ int indx = map_[i];
	  AvColortableEntry *e = &colors_[indx];
		if( e->state() == AvColortableEntry::UA)
			return e;
	}

	return NULL;	// Table full.
}

Boolean AvPalette::physicalToVirtual(const unsigned long pindex,
			  unsigned long &vindex)
{
	// For PseudoColor colormaps, the 'physical' value is an index
	// into the hardware color map and is the same as the index into
	// colors_.
	if(! decomposedIndex())
	{	if(pindex >= (unsigned long)maxColors()) // Out of range?
		{	cerr 
		   << "AvPalette::physicalToVirtual: color index (" << pindex
		   << ") is out of range (" << maxColors() << ")" << endl;
			return FALSE;
		}
		vindex = colors_[pindex].index();
		return TRUE;
	}
	else // For decomposedIndex, physical value isn't an index, so
	     // it's necessary to search for it.
	{	for(int i=0; i< tablelength_; i++)
			if( colors_[i].isAllocated() &&
				colors_[i].pixel() == pindex)
			{	vindex = colors_[i].index();
				return TRUE;
			}
	}
	return FALSE;
}

Boolean AvPalette::virtualToPhysical(const unsigned long vindex,
			  unsigned long &pindex)const
{ unsigned long	v = vindex;

	if(v >= (unsigned long)tablelength_)
	{	cerr <<
			 "AvPalette::virtualToPhysical: index out of range: "
			<< v << endl;
		pindex = vindex;	//??
		return FALSE;
	}

	int index = map_[v];
	pindex = colors_[index].pixel();
	return TRUE;
}

AvColortableEntry *AvPalette::findEntryByIndex(const unsigned long indx)const
{
unsigned long	v = indx;

	if(v >= (unsigned long)tablelength_)
	{	cerr <<
			 "AvPalette::findEntryByIndex: index out of range: "
			<< v << endl;
		return NULL;
	}

	int index = map_[v];
	return &colors_[index];
}

// Allocate a named color and return virtual index.
// Returns True for success..
Boolean AvPalette::allocNamedColor(const char *colorname,
			unsigned long &vindex, unsigned long &pindex)
{
	// First see if it's already allocated.
	AvColortableEntry *e = findEntryByName(colorname);
	if(e != NULL)
	{	pindex = e->pixel();
		vindex = e->index();
		return TRUE;
	}

	// If not, allocate a slot and ask subclass to help.
	if( (e = findEmpty()) == NULL)
		return FALSE;

	float red, green, blue;

	if(allocNamedColor_(colorname, pindex, red, green, blue))
	{	e->store(red, green, blue);
		e->setPixel(pindex);
		e->setName(colorname);
		e->setState(AvColortableEntry::RO);
		vindex = e->index();
		return TRUE;
	}

	return FALSE;
}

Boolean AvPalette::allocNamedColor(const char *colorname)
{ unsigned long dmy1, dmy2;

	return allocNamedColor(colorname, dmy1, dmy2);
}

// Allocate named color. Dummy in base class.
#if 0
Boolean AvPalette::allocNamedColor_(const char *colorname,
			unsigned long &pixel, float &r, float &g, float &b)
#else
Boolean AvPalette::allocNamedColor_(const char *,
			unsigned long &, float &, float &, float &)
#endif
{
	return FALSE;
}

Boolean AvPalette::checkAllocatedEnabled()const
{
	return (checkAllocatedDisabled_ == 0) ? TRUE : FALSE;
}

void AvPalette::enableCheckAllocated()
{
	if( checkAllocatedDisabled_ > 0)
	{	checkAllocatedDisabled_ -= 1;
		if((checkAllocatedDisabled_ == 0) && needCheck_)
			checkAllocated();
	}
}

void AvPalette::disableCheckAllocated()
{
	checkAllocatedDisabled_ += 1;
}

void AvPalette::checkAllocated()
{
	if(checkAllocatedDisabled_ == 0)
	{	checkAllocated_();
		needCheck_ = FALSE;
	}
	else
		needCheck_ = TRUE;
}

void AvPalette::checkAllocated_()
{
}

// Copy values to internal table, then call subclass to update hardware.
void AvPalette::storeColors(const float *red, const float *green,
		const float *blue, const int begin, const int length)
{ int DO = 0, start, max, num;

	if(red != NULL)
		DO |= AvColortableEntry::RED;
	if(green != NULL)
		DO |= AvColortableEntry::GREEN;
	if(blue != NULL)
		DO |= AvColortableEntry::BLUE;

	max = availableColors();
	// Force start to be valid.
	if(begin >= max)
		start = max-1;
	else
	if(begin < 0)
		start = 0;
	else
		start = begin;

	// Same for num.
	if((length <= 0) || (length > max-start))
		num = max-start;
	else
		num = length;

	for(int i = start; i < num; i++)
	{ unsigned long indx = map_[i];
	  float r=0.0, g=0.0, b=0.0;

		if(red != NULL) r = *red++;
		if(green != NULL) g = *green++;
		if(blue != NULL)  b = *blue++;

		colors_[indx].store( r, g, b, DO);
	}

	putColors();
}

void AvPalette::queryColors(float *red, float *green,
		float *blue, const int start, const int num)
{
	getColors();	// Ask subclass to update table.

	for(int i=start; i<start+num; i++)
	{ float r, g, b;
		indexToRGB(i, r, g, b);

		if(red != NULL)
			*red++ = r;
		if(green != NULL)
			*green++ = g;
		if(blue != NULL)
			*blue++ = b;
	}
}

// Return RGB values for the given virtual index.
void AvPalette::indexToRGB(unsigned long index,
			   float &r, float &g, float &b)const
{ AvColortableEntry *e = findEntryByIndex(index);

	if(e != NULL)
		e->get(r, g, b);
}

void AvPalette::putColors()
{ int max = maxColors();

	for(int i=0; i< max; i++)
		colors_[i].setFlags(0);		// Clear dirty flags.
	return;
}

void AvPalette::getColors(const Boolean /*force */)
{
}

// Convert a floating point image array into unsigned longs.
void AvPalette::scale(  float *in, unsigned long *out, const int Len,
			const float Min, const float Max,
			const unsigned long under, const unsigned long over,
			const unsigned long blank, const float blankval)
{
float scl;
int	ncolors, len = Len;
float min=Min, max=Max;
unsigned long p;
unsigned long undercolor=under, overcolor=over;
unsigned long blankcolor = blank;

	if((in == NULL) || (out==NULL))
		return;
	if(len <= 0)
		return;
	if(min > max)
	{ float tmp = min;
		min = max;
		max = tmp;
	}

	ncolors = availableColors();

	virtualToPhysical(undercolor, undercolor);
	virtualToPhysical(overcolor, overcolor);
	virtualToPhysical(blankcolor, blankcolor);

	// Will scale data to range of (0..ncolors-1);
	if(min == max)
		scl = ncolors-1;
	else
		scl = (ncolors-1)/(max-min);

	// Get a mapping array to minimize virtualToPhysical calls.
	unsigned long *map = new unsigned long[ncolors];
	for(int i=0; i< ncolors; i++)
		map[i] = colors_[map_[i]].pixel();

	// Scale pixel. if out of bounds use the given color.
	// (Be careful not to try to assign a neg # to p).
	while(--len >= 0)
	{float	tmp, tmpp;

		tmpp = *in++;
		// Calling finite() entails a function call.
		if( tmpp == blankval)
			p = blankcolor;
		else
		{	tmp = (tmpp - min)*scl;
			if(tmp < 0.0)
				p = undercolor;
			else
			{	p = (long)(tmp + 0.5);
				if( p >= (unsigned long)ncolors)
					p = overcolor;
				else
					p = map[p];
			}
		}

		*out++ = (unsigned long) p;
	}
	delete [] map;
}

ostream &operator<<(ostream &s, const AvColortableEntry &e)
{
	s << "Index = " << e.index();
	s << " Pixel = " << e.pixel();
	float r, g, b;
	e.get(r, g, b);
	s << " RGB = " << r << "/" << g << "/" << b;
	if(strlen(e.name()) > 0)
		s << " " << e.name();
	if(e.isReadOnly())
		s << " Read Only";
	else
	if(e.isReadWrite())
		s << " Read Write";
	else
		s << " Unallocated";

	return s;
}
