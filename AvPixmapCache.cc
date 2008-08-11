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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvPixmapCache.cc,v 19.0 2003/07/16 05:47:48 aips2adm Exp $
//
// $Log: AvPixmapCache.cc,v $
// Revision 19.0  2003/07/16 05:47:48  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:24  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/03 21:35:05  hravlin
// AvPixmapCache is now subclassed from AvRasterCache.
//
// Revision 17.0  2001/11/12 19:42:53  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:59  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:13  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:54  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:43  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:54  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:33  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:41  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.1  1997/11/07 18:58:47  bglenden
// Quiet Purify
//
// Revision 9.0  1997/08/25 21:30:33  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:18:43  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:53:30  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.11  1996/06/18  18:46:25  pixton
//  Copyright Update
//
//  Revision 1.10  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.10  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.9  1995/05/05  16:25:41  hr
//  Added constructor for AvPixmapCacheEntry.
//
//  Revision 1.8  1995/05/05  15:59:40  hr
//  Only one cache entry per raster rather than one per raster-size.
//
//  Revision 1.7  1995/04/24  18:49:45  hr
//  Added size, enable, flush functions. Made PixmapCache a Resource.
//  key_to_hash now uses raster's serial # rather than pointer to the raster.
//
//  Revision 1.6  1995/04/20  19:16:42  hr
//  Find wasn't checking the correct key for change.
//
//  Revision 1.5  1995/04/18  15:01:37  hr
//  Fixed a problem that appeared in find.
//
//  Revision 1.4  1995/04/17  15:36:08  hr
//  Comparision operator only checks raster address and scale factors.
//
//  Revision 1.3  1995/04/13  17:23:53  hr
//  Added data min/max to cached info
//
//  Revision 1.2  1995/04/07  20:31:23  hr
//  Added assignment operator, misc other changes.
//
//  Revision 1.1  1995/04/05  19:54:35  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

#include <AvPixmapCache.h>
#include <AvXCanvases.h>

AvPixmapCacheValue::AvPixmapCacheValue(AvXPixmap *pm)
{
	value_ = pm;
	if(value_ != NULL)
		value_->ref();
}

AvPixmapCacheValue::AvPixmapCacheValue()
{
	value_ = NULL;
}

AvPixmapCacheValue::~AvPixmapCacheValue()
{
	if(value_ != NULL)
		value_->unref();
	value_ = NULL;
}

AvPixmapCache::AvPixmapCache(const int size) : AvRasterCache(size)
{
}

AvPixmapCache::~AvPixmapCache()
{
}

void AvPixmapCache::insert(const AvRasterCacheKey &key, AvXPixmap *value)
{
	insertValue(key, new AvPixmapCacheValue(value));
}

AvXPixmap *AvPixmapCache::find(const Key &key, const Boolean removeIfChanged)
{ AvPixmapCacheValue *v =
			(AvPixmapCacheValue *)findValue(key, removeIfChanged);
	if(v == NULL)
		return NULL;
	else
		return v->value_;
}
