//# Copyright (C) 1995-99 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvPalette.h,v 19.0 2003/07/16 05:47:09 aips2adm Exp $
//
// $Log: AvPalette.h,v $
// Revision 19.0  2003/07/16 05:47:09  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:52  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:20  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:28  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:10:03  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:12  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.2  1999/09/16 20:09:46  hravlin
// Removed egcs and IRIX -64 compiler warnings.
//
// Revision 13.1  1999/09/15 21:20:01  hravlin
// Major changes to support TrueColor visuals.
//
// Revision 13.0  1999/08/10 18:39:59  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:37  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:43  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:33  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.3  1998/06/11 17:36:21  hr
// Made AvColormapSegment value a signed long.
//
// Revision 9.2  1998/05/04 20:50:30  hr
// In AvColormapSegment::setValue(), made value unsigned long.
//
// Revision 9.1  1998/05/04 20:36:03  hr
// Changed AvColormapSegment::getValue() to return unsigned long rather
// than int.
//
// Revision 9.0  1997/08/25 21:27:12  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.1  1997/04/30 20:13:27  hr
// Added enable/disableCheckAllocated() and made checkAllocated non virtual
// function that calls virtual checkAllocated_().
//
// Revision 8.0  1997/02/20 03:17:17  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:50:11  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.5  1996/06/18  18:43:10  pixton
//  Copyright Update
//
//  Revision 1.4  1996/03/07  21:13:37  hr
//  Added AvPalette::namedColorTable().
//
//  Revision 1.3  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.2  1995/06/22  18:56:08  hr
//  Worked on physicalToVirtual and allocColorName routines.
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvPalette.h

Handle I/O to a colormap.

AvPalette implements a virtual colormap. All pixel values used by aipsview
are virtual color indexes. To obtain the physical pixel value, a call to
virtualToPhysical is done. This is done:
	a) For PseudoColor colormaps, the read/write color cells can be
	   interspersed with read only values. Image scaling routines
	   see the colormap as a contiguous set of RW entries.
	b) So TrueColor colormaps can be emulated.

Functions exist to return the current effective size of the colormap, to
read/write into the colormap and other functions.

Colors are stored/retrieved as floats in the range of (0..1).

A virtual color index consists ofan index into a map array. The array is
needed since X11 colormaps may not have contiguous RW cells. The output of
the mapping array is an index into the internal color table which holds
the pixel value (index or 'TrueColor' pixel), the color values as floats
and some bookkeeping information.

Each color table entry is in one of three states: unallocated, read only
or read/write.
	unallocated	No use has been assigned yet.
	read only	The entry is considered constant.
	read/write	The value is changeable. These are the only values
			used to scale images.
NOTE:
 For TrueColor colormaps, the aipsview colormap will still list read/write
cells. The derived class will have to deal with emulating the read/write
cabability.

A number of the routines handle internal bookkeeping then call the
subclassed varient to do the actual work. eg. allocNamedColor.
*/

#ifndef AvPalette_h
#define AvPalette_h

#include <X11/Intrinsic.h>	// Defines Boolean
#include <AvResource.h>
#include <AvString.h>

// A color table entry holds a color value.
class AvColortableEntry {
  public:
	enum STATE { UA /*unallocated */, RW /* read write */, RO };
	enum RGBVALS { RGBNONE = 0,
		       RED=1, GREEN=2, BLUE=4, RGBALL = (RED | GREEN | BLUE)};

	AvColortableEntry();

	void operator=(const AvColortableEntry &);

	Boolean doRed(const int rgbvals)const{return rgbvals & RED;}
	Boolean doGreen(const int rgbvals)const{return rgbvals & GREEN;}
	Boolean doBlue(const int rgbvals)const{return rgbvals & BLUE;}

	// Store/get the values.
	void store(const float red, const float green, const float blue,
		   const int dorgb=RGBALL)
	{	if(doRed(dorgb))
			red_ = red;
		if(doGreen(dorgb))
			green_ = green;
		if(doBlue(dorgb))
			blue_ = blue;
		flags_ |= dorgb;	// Mark changed values.
	}

	void get(float &red, float &green, float &blue,
		 const int dorgb=RGBALL)const
	{	if(doRed(dorgb))
			red = red_;
		if(doGreen(dorgb))
			green = green_;
		if(doBlue(dorgb))
			blue = blue_;
	}

	int	index()const{return index_;}
	void	setIndex(const int i){index_ = i;}
	unsigned long pixel()const{return pixel_;}
	void	setPixel(const unsigned long p){pixel_ = p;}

	const	char *name()const{return name_.chars();}
	void	setName(AvString &name) {name_ = name;}
	void	setName(const char *name) {name_ = name;}

	unsigned short state()const{return state_;}
	void	setState(const STATE s){ state_ = s;}
	unsigned short flags()const{return flags_;}
	void	setFlags(const unsigned short f){flags_ = f;}

	Boolean	isReadOnly()const {return (state_ == RO);}
	Boolean	isReadWrite()const {return (state_ == RW);}
	Boolean	isAllocated()const {return (state_ != UA);}
	Boolean	isUnallocated()const {return (state_ == UA);}
	friend ostream &operator<<(ostream &s, const AvColortableEntry &x);

  private:
	int	index_;		// Index into map for this entry.
	unsigned long	pixel_;	// Real index or color value.
	float	red_, green_, blue_; // Color component values. (0..1).
	AvString name_;		// Empty except for allocByName colors.
	unsigned short	state_;		// State of entry.
	unsigned short	flags_;
};

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class AvPalette : public AvResource {
  public:
	// Returns the number of currently available colors.
	int availableColors()const {return rwcells_;}
	int maxColors()const {return maxcolors_;}// Maximum size of table.

	// Convert to/from physical and virtual values.
	Boolean physicalToVirtual(const unsigned long vpixel,
				  unsigned long &ppixel);

	Boolean virtualToPhysical(const unsigned long vindex,
			  unsigned long &pindex)const;
	// Allocate a named color and return virtual & physical values.
	// Returns True for success..
	Boolean allocNamedColor(const char *colorname,
			unsigned long &vindex, unsigned long &pindex);
	Boolean allocNamedColor(const char *colorname);
	//////////
	Boolean readonly()const { return readonly_;}
	Boolean decomposedIndex() const { return decomposedIndex_;}

	// Scale an image.
	void scale(float *in, unsigned long *out, const int Len,
		   const float Min, const float Max,
		   const unsigned long under, const unsigned long over,
		   const unsigned long blank, const float blankval);

	// Write/Read virtual colormap. Start is a virtual index.
	void storeColors(const float *red, const float *green,
		const float *blue, const int start=0, const int num=0);
	// Read entries from internal table after, perhaps, reading from
	// physical device.
	void queryColors(float *red, float *green,
		float *blue, const int start=0, const int num=0);

	// Return RGB values for the given virtual index. Data are returned
	// from the internal color table. No I/O is done.
	void indexToRGB(unsigned long index,
			float &r, float &g, float &b)const;


	// Recheck/update colors allocated by system colormap.
	// Checks enable flag then calls palette's checkAllocated_().
	void checkAllocated();
	// Increment/decrement a counter. When 0, checkAllocated will be
	// called. Use around allocNamedColor when making multiple calls.
	// If > 0, checkAllocated() sets a flag saying it should be called
	// when the counter returns to 0.
	void disableCheckAllocated();
	// Decrements counter. If 0, calls checkAllocated if needCheck()
	// has is TRUE.
	void enableCheckAllocated();
	Boolean checkAllocatedEnabled()const;

  protected:
	// AvPalette calls these to do I/O to the physical color table.
	// Copy dirty entries to physical cm.
	// For decomposed index colormaps, the subclass will use the call as
	// an opportunity to convert colors_' RGB values to pixels.
	virtual void putColors();
	// Copy physical cm to internal. The force flag forces the write
	// if possible. Otherwise, it's up to the subclass.
	virtual void getColors(const Boolean force=False);

	// Allocate named color.
	virtual Boolean allocNamedColor_(const char *colorname,
			unsigned long &pixel, float &r, float &g, float &b);

	AvPalette(const int maxlength);
	virtual ~AvPalette();			// Use unref() instead.
	virtual void checkAllocated_();
	
	// Returns colors_ entry that was previously allocated
	// as this color name. Returns the index or -1 if not found.
	AvColortableEntry *findEntryByName(const char *color)const;
	AvColortableEntry *findEntryByIndex(const unsigned long indx)const;
	AvColortableEntry *findEmpty()const;
	void expandTable(const int newlength);
	// Inter object communication.
	static const AvColortableEntry *getColortable(AvPalette *p)
			{	return p->colors_;}
	static const unsigned short *getMap(AvPalette *p)
			{	return p->map_;}
	static int tableLength(AvPalette *p)
			{	return p->tablelength_;}

	static Boolean isReadOnly(const AvColortableEntry *e)
		{	if(e == NULL)
				return False;
			else
				return e->isReadOnly();
		}
	static Boolean isReadWrite(const AvColortableEntry *e)
		{	if(e == NULL)
				return False;
			else
				return e->isReadWrite();
		}
	static Boolean isUnallocated(const AvColortableEntry *e)
		{	if(e == NULL)
				return False;
			else
				return e->isUnallocated();
		}
	static Boolean isAllocated(const AvColortableEntry *e)
		{	if(e == NULL)
				return False;
			else
				return e->isAllocated();
		}
  protected:
	Boolean	readonly_;		// True if Static grey or TrueColor.
	Boolean decomposedIndex_;	// TRUE if Direct or TrueColor.
	int	checkAllocatedDisabled_;// non zero
	Boolean	needCheck_;

	int	maxcolors_;		// Maximum number of cells in
					// physical color table.
	int	rwcells_;		// Number of rw/cells.
	int	tablelength_;		// Length of color/map arrays.
	AvColortableEntry	*colors_;	// Internal color table.
	unsigned short	*map_;		// Holds indexes into color table.
	// The first rwcells_ -1 of map_ hold indexes into colors_ of
	// RW color cells. Larger indexes refer to cells that are either
	// RO or UA.
};

#endif
