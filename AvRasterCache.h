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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvRasterCache.h,v 19.0 2003/07/16 05:48:17 aips2adm Exp $
//
// $Log: AvRasterCache.h,v $
// Revision 19.0  2003/07/16 05:48:17  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:49  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/03 21:29:29  hravlin
// Initial checkin.
//
//---------------------------------------------------------------------------

/* AvRasterCache.h
Used internally by Canvases to cache imaged data. eg. Pixmaps for
X Canvas, textures for OGL. (Not the raster itself).
The key is formed from a pointer to a raster and the current scaling.

A copy of the raster's modified time is kept along with the data min/max
and compared by the changed() function. changing min or max should change
the modified time, but that doesn't always happen.

The value stored is a RasterCacheValue. Each Canvas needs to subclass
AvRasterCacheValue and provide storage for what is being cached.

*/
#ifndef AvRasterCache_h
#define AvRasterCache_h
#include <AvResource.h>
#include <AvTimeStamp.h>

class AvRaster;

class AvRasterCacheEntry;

class AvRasterCacheValue {
  public:

  protected:
	friend class AvRasterCacheEntry;
	AvRasterCacheValue();
	virtual ~AvRasterCacheValue();
};

class AvRasterCacheKey {
  public:
	// Width/Height are the dimensions after scaling.
	// (Dimensions of pixmap).
	AvRasterCacheKey(AvRaster *, const int width, const int height);
	AvRasterCacheKey();
	AvRasterCacheKey(const AvRasterCacheKey &);
	~AvRasterCacheKey();
	AvRasterCacheKey& operator=(const AvRasterCacheKey &);
	// Checks raster address and scale factors only.
	int operator==(const AvRasterCacheKey &)const;
	int operator!=(const AvRasterCacheKey &)const;
	// Returns TRUE if Raster's internal info has changed.
	// If key is not NULL, the width/height will also be compared.
	Boolean changed(const AvRasterCacheKey *key)const;
	TimeStamp getTime()const{return time;}
	AvRaster *getRaster()const{return raster;}
//  private:
	friend class AvRasterCache;
	void copy(const AvRasterCacheKey &from, AvRasterCacheKey &to);
	AvRaster	*raster;
	int		width, height;
	float		dmin, dmax;
	TimeStamp	time;
};

typedef AvRasterCacheKey Key;

class AvRasterCache : public AvResource {
public:
	AvRasterCache(const int size);

	// Return a pointer to the cached value. if removeIfChanged is TRUE,
	// the entry is removed if the value has changed. Returns TRUE if
	// the pixmap was found and not deleted.
	void remove(const Key &);
	int size()const {return size_;}
	void enable(const Boolean e) { enabled_ = e;}
	Boolean enabled()const{return enabled_;}
	void flush();
protected:
	~AvRasterCache();
	virtual void insertValue(const AvRasterCacheKey &,
				 AvRasterCacheValue *);
	AvRasterCacheValue *findValue(const Key &,
				      const Boolean removeIfChanged=TRUE);
private:
	void init(const int size);
	int size_;
	AvRasterCacheEntry** first_;
	AvRasterCacheEntry** last_;
	AvRasterCacheEntry*& probe(const Key &i);
	Boolean enabled_;
};

class AvRasterCacheEntry {
	AvRasterCacheEntry(const AvRasterCacheKey &key, AvRasterCacheValue *);
	AvRasterCacheEntry();
	~AvRasterCacheEntry();
public:
	friend class AvRasterCache;

	Key key_;
	AvRasterCacheValue *value_;
	AvRasterCacheEntry *chain_;
};

#endif
