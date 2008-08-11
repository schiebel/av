//---------------------------------------------------------------------------
//
//  [Insert Copyright Notice Here!!]
//
//---------------------------------------------------------------------------
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvIVList.cc,v 19.0 2003/07/16 05:47:55 aips2adm Exp $
//
// Description:
//
//   Does the hokey-pokey
//
// $Log: AvIVList.cc,v $
// Revision 19.0  2003/07/16 05:47:55  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:30  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:59  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:05  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:26  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:09:01  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:50  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:08  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:53  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:56:04  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.2  1998/06/09 20:57:15  hr
// Got rid of some signed/unsigned/long conversion warnings.
//
// Revision 9.1  1997/09/15 17:13:45  hr
// Fixed signed/unsigned comparision warnings (g++) in ListImpl_best_new_count.
// Fixed g++ warning about printing a long with just %d in ListImpl_range_error.
//
// Revision 9.0  1997/08/25 21:31:08  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:19:05  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 08:53:38  droberts
// Changed all references to IVList to AvIVList.
//
// Revision 7.1  1996/10/10 20:23:11  jpixton
// initial checkin, native images working,
// ,
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/*
InterViews list class slightly modified for use here. Since this will
probably be replaced, its use tends to be hidden.

 * Copyright (c) 1991 Stanford University
 * Copyright (c) 1991 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Stanford and Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Stanford and Silicon Graphics.
 * 
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 *
 * IN NO EVENT SHALL STANFORD OR SILICON GRAPHICS BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 */

/*
 * Support routines for lists.
 */

#include <AvIVList.h>
#include <stdio.h>
#include <stdlib.h>

implementList(__AnyPtrList,__AnyPtr)

static long ListImpl_best_new_sizes[] = {
    48, 112, 240, 496, 1008, 2032, 4080, 8176,
    16368, 32752, 65520, 131056, 262128, 524272, 1048560,
    2097136, 4194288, 8388592, 16777200, 33554416, 67108848
};

long ListImpl_best_new_count(long count, unsigned int size) {
 long val = (long)(count*size);
 for (unsigned i = 0; i < sizeof(ListImpl_best_new_sizes)/sizeof(long); i++) {
        if (val < ListImpl_best_new_sizes[i]) {
            return ListImpl_best_new_sizes[i] / (long)size;
        }
    }
    return count;
}

void ListImpl_range_error(long i) {
    fprintf(stderr, "internal error: list index %ld out of range\n", i);
    abort();
}
