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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvRasterCache.cc,v 19.0 2003/07/16 05:48:17 aips2adm Exp $
//
// $Log: AvRasterCache.cc,v $
// Revision 19.0  2003/07/16 05:48:17  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:49  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/03 21:29:05  hravlin
// Used internally by Canvases to cache imaged data. AvPixmapCache
// is now subclassed from this.
//
//---------------------------------------------------------------------------

/*
Pixmap cache mechanism.
This is the table class from Interviews.
*/
#include "AvRasterCache.h"
#include "AvRaster.h"

AvRasterCacheKey::AvRasterCacheKey()
{
	raster = NULL;
	width = -1;
	height = -1;
	dmin = 0;
	dmax = -1;
	time = 0;
}

AvRasterCacheKey::AvRasterCacheKey(AvRaster *ras, const int w, const int h)
{
	raster = ras;
	raster->ref();
	width = w;
	height = h;
	// Changing min or max should change modifiedTime, but that doesn't
	// always happen.
	dmin	= raster->min();
	dmax	= raster->max();
	time = raster->modifiedTime();
}

void AvRasterCacheKey::copy(const AvRasterCacheKey &from, AvRasterCacheKey &to)
{
	to.raster = from.raster;
	to.raster->ref();
	to.width = from.width;
	to.height = from.height;
	to.dmin	= from.dmin;
	to.dmax	= from.dmax;
	to.time = from.time;
}

AvRasterCacheKey& AvRasterCacheKey::operator=(const AvRasterCacheKey &from)
{
	if(this != &from)
		copy(from, *this);
	return *this;
}

AvRasterCacheKey::AvRasterCacheKey(const AvRasterCacheKey &a)
{
	copy( a, *this);
}

AvRasterCacheKey::~AvRasterCacheKey()
{
	raster->unref();
}

// Compares Key insertion info: raster address. Does not
// compare internal raster data. (use changed()).
int AvRasterCacheKey::operator==(const AvRasterCacheKey &a)const
{
	return  (a.raster == raster);
}

int AvRasterCacheKey::operator!=(const AvRasterCacheKey &a)const
{
	return !( a == *this);
}

//Returns TRUE if raster's internal info has changed.
Boolean AvRasterCacheKey::changed(const AvRasterCacheKey *k)const
{
float rmin, rmax;
Boolean changed;

	rmin = raster->min();
	rmax = raster->max();
	changed = ( (rmin != dmin) || (rmax != dmax) ||
			raster->modifiedSince(time) );
	if(changed)
		return TRUE;
	if(k != NULL)
		changed = ((width != k->width) || (height != k->height));

	return changed;
}

/*
 * AvRasterCache implementation
*/

AvRasterCache::AvRasterCache(const int n)
{
	init(n);
}

void AvRasterCache::init(const int n)
{
	for (size_ = 32; size_ < n; size_ <<= 1);

	first_ = new AvRasterCacheEntry*[size_];
	// Note the clever way that size_ is a power of two. Subtracting one
	// makes it useful as a mask.
	--size_;
	last_ = &first_[size_];
	for (register AvRasterCacheEntry** e = first_; e <= last_; e++)
	{
		*e = NULL;
	}
	enable(TRUE);
}

AvRasterCache::~AvRasterCache() {
	flush();
}

void AvRasterCache::flush(){
	for (register AvRasterCacheEntry** e = first_; e <= last_; e++)
	{	AvRasterCacheEntry* t = *e;
		delete t;
	}
	delete [] first_;
	first_ = 0; // paranoia - force crash if dereferenced
	init(size_);
}

// The raster serial number is used as the hash value.
inline unsigned long key_to_hash(const AvRasterCacheKey &k)
{	return (unsigned long) k.getRaster()->getSerial();
}

#if 0
#include <stdio.h>
/*inline*/ AvRasterCacheEntry*& AvRasterCache::probe(const Key &i)
{
int hash = key_to_hash(i) & size_;
printf("0X%x	0X%x(%d)\n", &i, hash, hash);
	return first_[key_to_hash(i) & size_];
}

static void dump(AvRasterCacheEntry **list, int len)
{
	for(int i=0; i< len; i++)
	{AvRasterCacheEntry *e;
		if(*list == NULL) continue;
		printf("\n %d ", i);
		e = *list;
		for(e = *list; e != NULL; e = e->chain_)
			printf("%x ", e->key_.raster);
		printf("\n");
	}
}
#else
inline AvRasterCacheEntry*& AvRasterCache::probe(const Key &i)
{
	return first_[key_to_hash(i) & size_];
}
#endif

void AvRasterCache::insertValue(const Key &k, AvRasterCacheValue *v)
{
	if(v == NULL)
		return;
	if(enabled())
	{	register AvRasterCacheEntry* e = new AvRasterCacheEntry(k, v);
		register AvRasterCacheEntry** a = &probe(k);
		e->chain_ = *a;
		*a = e;
	}
}

// Look for an entry. If the raster has changed since it was stored,
// remove the entry if requested.
AvRasterCacheValue *AvRasterCache::findValue(const Key &k,
					     const Boolean removeIfChanged)
{AvRasterCacheValue *v=NULL;

	if(!enabled())
		return NULL;

	for(register AvRasterCacheEntry* e = probe(k);e != NULL; e = e->chain_)
	{	if (e->key_ == k)	// If match
		{	// If match, but it's changed, remove if requested.
			if(removeIfChanged && e->key_.changed(&k))
			{	remove(k);
				return NULL;
			}
			else	// Otherwise, hit.
			{	v = e->value_;
				return v;
			}
		}
	}
	return NULL;
}

void AvRasterCache::remove(const Key &k)
{
	AvRasterCacheEntry** a = &probe(k);
	register AvRasterCacheEntry* e = *a;
	if((e != NULL) && enabled())
	{	if (e->key_ == k)
		{	*a = e->chain_;
			delete e;
		}
		else
		{register AvRasterCacheEntry* prev;
			do
			{	prev = e;
				e = e->chain_;
			} while (e != NULL && e->key_ != k);
			if (e != NULL)
			{	prev->chain_ = e->chain_;
				delete e;
			}
		}
	}
}

AvRasterCacheEntry::AvRasterCacheEntry(const AvRasterCacheKey &key,
				       AvRasterCacheValue *value) 
{
	key_ = key;
	value_ = value;
//	if(value_ != NULL)
//		value_->ref();
	chain_ = NULL;
}

AvRasterCacheEntry::AvRasterCacheEntry()
{
	value_ = NULL;
	chain_ = NULL;
}

AvRasterCacheEntry::~AvRasterCacheEntry()
{
//	if(value_ != NULL)
//		value_->unref();
	delete value_;
}

AvRasterCacheValue::AvRasterCacheValue()
{
}

AvRasterCacheValue::~AvRasterCacheValue()
{
}

