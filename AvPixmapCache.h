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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvPixmapCache.h,v 19.0 2003/07/16 05:47:28 aips2adm Exp $
//
// $Log: AvPixmapCache.h,v $
// Revision 19.0  2003/07/16 05:47:28  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:08  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/03 21:35:21  hravlin
// AvPixmapCache is now subclassed from AvRasterCache.
//
// Revision 17.0  2001/11/12 19:42:38  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:44  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:12  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:34  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:21  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:18  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:00:40  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:54:39  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.2  1998/01/20 15:09:59  hr
// Replaced references to clock_t by TimeStamp.
//
// Revision 9.1  1997/12/17 21:51:19  hr
// Changes to remove warnings from MIPSpro.7x compiler.
//
// Revision 9.0  1997/08/25 21:28:58  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:17:41  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:53:51  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.9  1996/06/18  18:43:12  pixton
//  Copyright Update
//
//  Revision 1.8  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.8  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.8  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.7  1995/05/05  16:26:04  hr
//  Added constructor for AvPixmapCacheEntry.
//
//  Revision 1.6  1995/05/05  16:00:33  hr
//  Only one cache entry per raster rather than one per raster-size.
//
//  Revision 1.5  1995/04/24  18:47:27  hr
//  Added size, enable, flush functions. Made PixmapCache a Resource.
//
//  Revision 1.4  1995/04/17  15:37:27  hr
//  Added changed() function to PixmapCacheKey. Added removeIfChanged argument
//  to find.
//
//  Revision 1.3  1995/04/13  17:24:28  hr
//  Added data min/max to cached info.
//
//  Revision 1.2  1995/04/07  20:30:47  hr
//  Added assignment operator, misc other changes.
//
//  Revision 1.1  1995/04/05  19:55:01  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvPixmapCache.h
Used internally by the XCanvases to cache pixmaps.
The key is formed from a pointer to a raster and the current scaling.

A copy of the raster's modified time is kept along with the data min/max
and compared by the changed() function. changing min or max should change
the modified time, but that doesn't always happen.

The value returned is a pointer to an AvXPixmap canvas.

*/
#ifndef AvPixmapCache_h
#define AvPixmapCache_h
#include "AvRasterCache.h"

class AvXPixmap;


class AvPixmapCacheValue : public AvRasterCacheValue {
public:
	AvPixmapCacheValue(AvXPixmap *pm);
	AvPixmapCacheValue();
	virtual ~AvPixmapCacheValue();
	AvXPixmap *value_;
};

class AvPixmapCache : public AvRasterCache {
  public:
	AvPixmapCache(const int size);
	virtual void insert(const AvRasterCacheKey &, AvXPixmap *);
	AvXPixmap *find(const Key &, const Boolean removeIfChanged=TRUE);
	virtual ~AvPixmapCache();
};

#endif
