//# Copyright (C) 1995-2003 Board of Trustees of the University of Illinois
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
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvFitsDataSet.cc,v 19.2 2003/09/26 16:35:56 hravlin Exp $
//
// $Log: AvFitsDataSet.cc,v $
// Revision 19.2  2003/09/26 16:35:56  hravlin
// For Darwin, added extern "C" to find isnan().
// made error_out() a dummy function.
//
// Revision 19.1  2003/09/02 21:11:21  hravlin
// Added dataSetTypeName(), isDataSet and other support for factory info.
//
// Revision 19.0  2003/07/16 05:47:49  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:25  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.6  2002/01/23 19:42:48  hravlin
// Changed several off_t's to size_t's in MapSegment to make picky compilers
// happy.
//
// Revision 17.5  2002/01/22 21:55:01  hravlin
// The IsNaNf macro wasn't working with 64 bit compilers.
// header_len_ and the variable index are size_t.
//
// Revision 17.3  2002/01/09 18:12:01  hravlin
// Changed the way existence of ieeefp.h is checked for.
// Changed references to MAXVALUE to FLOATMAX which is defined in Av.h.
// Changed maximum # of pages to be mapped to 16384. (64MB on many systems).
// Apple's OS X uses getpagesize().
//
// Revision 17.2  2002/01/07 22:52:20  hravlin
// Removed check for __P.
// Included <float.h> to get FLT_MAX
//
// Revision 17.1  2002/01/03 21:07:17  hravlin
// Define M_PI if it isn't alread defined.
//
// Revision 17.0  2001/11/12 19:42:54  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:00  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:16  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:55:24  hravlin
// Assume result of strrchr is const char *.
//
// Revision 14.0  2000/03/23 16:08:55  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.2  1999/09/29 16:49:48  hravlin
// Fixed compiler warnings.
//
// Revision 13.1  1999/08/25 19:53:56  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:40:44  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:56  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.1  1999/04/30 09:45:40  gvandiep
// Moved part of the code in the attach_ function to an initattach_ function.
// egcs 1.1.1/2 gave an internal compiler error and separating this
// part of the code cured the problem.
//
// Revision 11.0  1998/10/03 07:01:36  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.1  1998/08/14 17:59:55  hr
// Minor format change to error message in get(string)keyword.
//
// Revision 10.0  1998/07/20 17:55:45  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.5  1998/06/11 17:32:09  hr
// Changes in MapSegment to handle cases where off_t is long long (SGI).
//
// Revision 9.4  1998/02/18 21:59:56  hr
// Changed couts to cerr.
//
// Revision 9.3  1997/11/21 18:26:06  hr
// In attach_(), if the value of CDELT in the file is 0.0, reset it to 1.0
// so we don't get a singular matrix when it's multiplied by the PC matrix.
//
// Revision 9.2  1997/10/29 19:55:40  hr
// attach_() Added support for CD matrices. Fixed a problem with reading
// PC matrix keywords.
//
// Revision 9.1  1997/10/22 19:33:21  hr
// Changed attach_() to use defaults and print a warning message if any
// of CRPIX, CDELT or CRVAL keywords are missing. Turned off an initialization
// done for Sun's memory checker.
//
// Revision 9.0  1997/08/25 21:30:38  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.1  1997/05/07 15:53:16  hr
// Changes for DEC alpha including start of FITSTRACE.
//
// Revision 8.0  1997/02/20 03:16:51  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.19  1997/02/18 21:27:16  jpixton
// corrected pcmatrix bug.
//
// Revision 7.18  1997/02/17 11:27:15  gvandiep
// added __hpux__ switch to prevent inclusion of ieeefp.h
//
// Revision 7.17  1997/02/11 16:32:22  bglenden
// Minor linux fixup.
//
// Revision 7.16  1997/02/11 16:11:47  jpixton
// CTYPE keyword no longer mandatory - aipsview needs a label, so
// AxisN was chosen as a default ctype, where N is in 1,2,...NAXIS.
//
// Revision 7.15  1997/02/10 16:01:24  hr
// Under g++ M_PI wasn't being defined and g++ complained about a
// signed/unsigned comparision.
//
// Revision 7.14  1997/02/06 19:29:47  hr
// setMinMax() now sets min = max = 0.0 if it thinks the file is completely
// blanked.
//
// Revision 7.13  1997/01/31 21:32:31  jpixton
// Added support for CROTA FITS keyword for long/lat axis pair.
//
// Revision 7.12  1997/01/13 09:36:40  gvandiep
// _SC_PAGESIZE is called _SC_PAGE_SIZE by HP; extended #if switch
//
// Revision 7.11  1997/01/09 20:47:05  hr
// The SGI compiler under 5.3 couldn't find isnan() in math.h.
//
// Revision 7.10  1997/01/09 03:34:30  bglenden
// Add an ifdef so all AIPS++ little endian machines are automagicall
// discovered
//
// Revision 7.9  1997/01/07 23:00:15  hr
// Removed close() function and replaced calls to close with calls to detach_()
// which now resets pointers to NULL.
//
// Revision 7.8  1996/12/28 00:27:39  bglenden
// Set a deleted pointer to zero so it won't be deleted twice.
//
// Revision 7.7  1996/12/18 16:49:16  droberts
// Explicitly cast MAP_FAILED to char*.
//
// Revision 7.6  1996/12/17 17:08:59  droberts
// Replaced call to isnand() with isnan().
//
// Revision 7.5  1996/12/12 09:20:21  droberts
// Added Polarization axis name recognition.  Removed message: 'cant map header'
//
// Revision 1.13  1996/11/06 22:44:25  hr
// Had previously misspelled __SVR4 as __SVR.
//
// Revision 1.12  1996/11/06 17:12:18  hr
// 		For SunOS 4.1.3
// isnand() is not defined. ulong was undefined. _SC_PAGE_SIZE is not defined.
//
// Revision 1.11  1996/10/10  20:04:12  pixton
// missing initializer.
//
// Revision 1.10  1996/09/25  21:23:19  hr
// Added close() function.
//
// Revision 1.9  1996/09/20  20:19:35  pixton
// Prefixed all classes with Av
//
// Revision 1.8  1996/09/17  16:33:49  hr
// Clear "kword" to 0 if running on a Sun to make memory checker happy.
//
// Revision 1.7  1996/09/12  17:00:18  hr
// getMinMax() wasn't checking for blanked data.
//
// Revision 1.6  1996/09/12  15:19:20  hr
// Turned on hack to get around apparent bug in Sun's memory checker.
// getBuffer() was computing "toMap" incorrectly.
//
// Revision 1.5  1996/08/23  17:00:09  pixton
// fixed dataUnits()
//
// Revision 1.4  1996/08/13  21:53:22  pixton
// added delete for pcmatrix in attach
//
// Revision 1.3  1996/08/12 22:48:04  pixton
// bad delete call fixed.
//
// Revision 1.2  1996/08/12 21:32:25  pixton
// modified IJKToWorld...
//
// Revision 1.1  1996/08/12  14:49:07  pixton
// Initial revision
//
// Revision 1.1  1996/08/02  16:53:19  hr
// Initial revision
//
//
//---------------------------------------------------------------------------
/* AvFitsDataSet.cc
   Handles reading of FITS files for aipsview. Files are memory mapped using
   a moving "window" so very large files may be accessed. The window is usually
   set large enough so that "reasonable" sized files will fit in one window.

*/
#include <stdio.h>
#include <math.h>

// It appears that only IRIX and Solaris have ieeefp.h.
#if defined(__sgi) || defined(__sun)
#include <ieeefp.h>
#define isnan(x) isnanf((x))
#endif

#include <iostream.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#ifdef __GNUC__
extern "C" {
#endif
#include <sys/mman.h>
#ifdef __GNUC__
}
#endif
#include <sys/stat.h>

#if defined(__APPLE_CC__) && defined(__POWERPC__)
// This needs to be after some above include file.
extern "C" { int isnan(double x); }
#endif
     
#include "AvString.h"
#include "AvFitsDataSet.h"
#include "Av.h"

#if !defined(M_PI)
#define M_PI            3.14159265358979323846
#endif

//	Constants are different on different systems.
#ifndef MAP_FAILED
#define MAP_FAILED (void *)-1
#endif

// Types of data files AvFitsDataSet understands.
enum { TYPE_FLOAT, TYPE_16INT, TYPE_32INT, TYPE_UCHAR, TYPE_DOUBLE};

// The maximum number of pages that will be mapped at one time.
// This is currently a compile time constant, but it may become a resource
// someday.
// Note: The actual mapping may be up to two pages longer than this due to
// rounding to page boundaries.
//static const int MAXPAGES=4096;		// ~ 16MB.
static const int MAXPAGES=16384;		// ~ 64MB. (with 4k pages).
//static const int MAXPAGES=1;

// Simple class to handle memory mapping read only files.
class MapSegment {
public:
  // Create a mapping object for the file descriptor.
  // fd is an open file descriptor.
  // The 'zero' will be where index 0 accesses data into the file and
  // the maximum allowable mapped length will be the minimum of
  // maxlength bytes and an internally generated maximum.(See MAXPAGES).
  MapSegment(const int fd, size_t zero=0, size_t maxlength=0);
  // Does not close the file descriptor.
  virtual ~MapSegment();
  Boolean mapped()const{return mapped_;}
  // Create a mapping length elements long starting at offset
  // elements from the zero point of the file. (Note: offset is >= 0).
  // The actual map will be made by rounding offset down to the nearest
  //page boundary and rounding length up to the next page boundary.
  // Returns TRUE if the mapping succeeded. map will fail if
  // the mmap fails or the length is greater than the maximum allowed
  // length.
  // Request is ignored if currently mapped and the requested mapping
  // is valid. Otherwise, it will call unmap first. (Returns TRUE).
  // If length is 0, the maximum length will be used.
  Boolean map(const size_t offset, const size_t length=0,
	      const size_t elementSize=1);
  void	unmap();	// Gets rid of current mapping.
  // Return a pointer to a value index elements into the file.
  // The mapped segment will be (at least) nelements long. Returns
  // NULL if the mapping can't be done.
  // If nelements is 0, the maximum length will be used.
  // NOTE: Since each call to pointer may result in a different mapping
  //	old pointers may not be valid and pointer comparisons may
  //	not work!
  void *pointer(const size_t index=0,
		const size_t nelements=0,
		const size_t elementSize=1);
  // Pointer to start of mapped segment. (Offset from file start).
  inline char *segmentStart()const{return base_;}
  // Length of actual segment.
  inline size_t segmentLength()const
  {return length_;}
  // Is ptr a valid pointer for current mapping?
  // (It may not point to what it pointed to initially).
  inline Boolean validPointer(const char *ptr)const
  { return (mapped() &&
	    (ptr >= segmentStart()) &&
	    (ptr < segmentStart()+segmentLength()))
      ? TRUE : FALSE;}
  // Is index valid for current mapping?
  // Index is indexed from 'zero', not the start of the file.
  // size is the element size.
  inline Boolean validIndex(const size_t index, const size_t size=1)const
  { size_t idx = index*size + zero_;	// To character offset.
    return ((idx >= baseOffset_) && (idx < baseOffset_+length_))
		? TRUE : FALSE;
  }
  // Returns TRUE if the range is valid for current mapping.
  inline Boolean validRange(const size_t start, const size_t length,
			    const size_t size=1)const
  {	if(length == 0)		// No zero length ranges.
		return FALSE;
	 return validIndex(start,size) && validIndex((start+length)-1,size);
  }

  // Round up to the next page boundary.
  // (This isn't static to make sure pagesize_ is set).
  inline size_t roundToPage(const size_t n)const
  { return (n/pagesize_ + ((n % pagesize_) ? 1 : 0))*pagesize_;}

  // The maximum allowed # of elements to map.
  inline size_t maxElements(const size_t elementSize=1)
       const{return (maxlength_)/elementSize;}

  // Offset parameter from last map().
  inline size_t offset()const{return offset_;}

  // Actual offset (index from start) of mapped data.
  inline size_t baseOffset()const{return baseOffset_;}

private:
  void	init(const int fd, const size_t zero, const size_t maxlength);
  int	fd_;
  Boolean mapped_;	// True when there is a mapping.
  size_t offset_;	// Requested offset.
  size_t baseOffset_;	// What actually got passed to mmap.
  char	*pointer_;	// Requested start of mapping.
  char	*base_;		// Actual start of mapping.
  size_t length_;	// Actual length of mapped segment.
  size_t zero_;		// Index (in bytes) from start of file to
			// beginning of data. (pointer(0)).
  size_t filesize_;	// No mapping past this.
  size_t maxlength_;	// Max # of bytes to map at a time.
			// (Default is min of filesize_ - zero_
			//  and pagesize_*MAXPAGES.)
  static int pagesize_;	// System page size. Memory mapping has to
			// be in chunks of this.
};

int MapSegment::pagesize_ = 0;

void MapSegment::init(const int fd, const size_t zero, const size_t maxlength)
{	
  fd_ = fd;
  filesize_ = AvFitsDataSet::fileSize(fd);
  length_ = offset_ = baseOffset_ = 0;
  base_ = pointer_ = NULL;
  if(zero < filesize_)
	zero_ = zero;
  else				// Should make a stink here!
	zero_ = filesize_ -1;

  mapped_ = FALSE;
  if(pagesize_ == 0)
#if defined(sparc) && ! defined(__SVR4)
	pagesize_ = 4096;	// For SunOS 4.1.3 compiler.
#elif defined(__hpux__)
	pagesize_ = sysconf(_SC_PAGE_SIZE);
#elif defined(__APPLE__)
	pagesize_ = getpagesize();	// Mac OS X. (BSD).
#else
	pagesize_ = (int)sysconf(_SC_PAGESIZE);
#endif

  // The maximum allowable map length request is the minimum of the
  // data size, the maxlength parameter and the compiled maximum.
  // (There is an implicit assumption that these values will hold
  //  an integral number of elements).
  size_t max1 = filesize_ - zero_;
  size_t max2 = pagesize_*MAXPAGES;
  maxlength_ = (max1 <= max2) ? max1 : max2;
  if((maxlength > 0) && (maxlength < maxlength_))
    maxlength_ = maxlength;
}

MapSegment::MapSegment(const int fd, size_t zero, size_t maxlength)
{
  init(fd, zero, maxlength);
}

MapSegment::~MapSegment()
{
  unmap();
}

// Create a mapping starting at Offset elements from the zero point that
// is Length elements long. Returns TRUE if the mapping succeeded or the
// request was already mapped.
// Note: The actual mapping may be up to two pages longer than the request
// due to rounding to page boundaries.
Boolean MapSegment::map(const size_t Offset, const size_t Length,
			const size_t elementSize)
{
  char *ptr;
  int prot, flags;
  size_t offset;
  size_t length, len;

  length = (Length > 0) ? Length : maxElements(elementSize);
  if(validRange(Offset, length, elementSize))
    return TRUE;
  offset = Offset*elementSize + zero_;	// Offset in bytes.
  length *= elementSize;		// Length in bytes.
  
  // Is length too big?
  if(length > maxlength_)
    {	
      cerr << "MapSeg::map: request too long.\n";
      return FALSE;
    }
  // Make sure new mapping would stay within file.
  if(offset+length > filesize_)
    {	
    cerr << "MapSeg::map: request too long for file\n";
    return FALSE;
  }
  if(mapped())
    unmap();
  
  prot = PROT_READ;
  flags = MAP_SHARED;
  // Round offset down to a page boundary.
  off_t off = (off_t)(offset/pagesize_)*pagesize_;
  // Round length up to next page boundary.
  // (length + the extra from rounding down).
  len = roundToPage(length + (offset-off));
  ptr =  (char *) mmap(0,len, prot, flags, fd_, off);
  if(ptr != (char*)MAP_FAILED)
    {	
      length_ = len;		// Length of mapping.
      offset_ = offset;		// Caller's offset from start of file.
      baseOffset_ = off;	// Actual offset.
      base_ = ptr;		// Start of mapping.
      // What caller expects.
      pointer_ = base_ + (offset - baseOffset_);
      mapped_ = TRUE;
#if 0
      if(zero_ > 0)
	cerr	<< "Mapped " << len << " bytes starting at " << off
		<< " Offset " << Offset << " offset " << offset_ << " baseOffset "
		<< baseOffset_ << endl;
#endif
#if 0
      printf("Start = 0x%X End = 0x%X Pointer = 0x%X (+%d)\n",
	     base_, base_ + len-1, pointer_, pointer_ - base_);
      //printf("|%8.8s| |%8.8s|\n", base_, pointer_);
      fflush(stdout);
#endif
    }
  
  return mapped();
}

void MapSegment::unmap()
{
  if(mapped())
  { munmap(base_, length_);
    pointer_ = base_ = NULL;
    baseOffset_ = 0;
    length_ = offset_ = 0;
    mapped_ = FALSE;
    // (Don't reset zero_).
  }
}

// Return a pointer index elements from the zero point or NULL.
void *MapSegment::pointer(const size_t index,
			  const size_t nelements,
			  const size_t elementSize)
{
  if(!map((long)index, nelements, elementSize))
    return NULL;
  else
  { void *ptr = pointer_ + index*elementSize - offset_ + zero_;
    return ptr;
  }
}

// If this is defined, it will enable turning on tracing. Currently, only
// numerical kewords are traced.
//#define FITSTRACE
#ifdef FITSTRACE
static Boolean getBooleanEnv(const char *name, const Boolean def=FALSE)
{

	if(name == NULL)
		return def;
	char *val = getenv(name);
	if(val == NULL)
		return def;
	switch(*val) {
	case 'T':
	case 'Y':
	case 't':
	case 'y':
		return TRUE;
	default:
		return FALSE;
	}
}

static Boolean traceFits_ = FALSE;

static inline Boolean traceFits(){return traceFits_;}
static void checkFitsTrace()
{
	traceFits_ = getBooleanEnv("AV_TRACEFITS", FALSE);
}

#endif

//================================================================

// Do we need to swap bytes? The assumption is made that if we do, 4 byte
// variables (ints and floats) need to have the high and low bytes exchanged
// and the middle two exchanged.

#if defined(__alpha) || defined(AIPS_LITTLE_ENDIAN)
// '__alpha' is defined by both DEC's cc and g++. Hopefully, it is also
// defined by DEC's c++ compiler.
#ifndef NEEDSWAP
#define NEEDSWAP
#endif
#endif

#ifdef NEEDSWAP
#define swap2(x) ( (((x)<<8) & 0xff00) | (((x)>>8) & 0xff))
#define swap4(x) ( ((((unsigned)(x))<<24) & 0xff000000)\
		   |((((unsigned)(x)) << 8) & 0x00ff0000) \
		   |((((unsigned)(x))>>8) & 0xff00) \
		   |((((unsigned)(x)) >> 24) & 0xff) )

     // Reverses the order of bytes of a floating point number. Assumes
     // that 'unsigned' and 'float' are the same length.
     static inline float swapf(const float f)
{
  union {
    float	f;
    unsigned u;
  } y;
  y.f = f;
  y.u = swap4(y.u);
  return (y.f);
}

// Reverses the order of bytes of a double. Assumes that 2 'unsigned long's
// and 'double' are the same length.
static inline double swapd(const double d)
{ register unsigned long tmp;
union {
  double	d;
  unsigned long a[2];
} y;
y.d = d;
tmp = y.a[0];
y.a[0] = swap4(y.a[1]);
y.a[1] = swap4(tmp);
return (y.d);
}

// Macros to get swapped values. They turn into NOPs if swapping is not
// needed.
#define GET2(x) (swap2(x))
#define GET4(x) (swap4(x))
#define GETF(x) swapf(x)
#define GETD(x) swapd(x)
#else
#define GET2(x) (x)
#define GET4(x) (x)
#define GETF(x) (x)
#define GETD(x) (x)
#endif


// Normally we use a macro to check for NaN rather than calling a function
// for every data point. If 'USEISNANF' is defined, the function will be used.
// NOTE: If the date are displayed as all WHITE, USEISNANF may need to
//	be defined.
//#define USEISNANF
#ifndef USEISNANF
// Macro from ieeefp.h to test floats for NaN.
//  (replaced 'long' with uInt and removed 'L' suffix on constants since
//   longs are 64 bits in some circumstances).
#define isNaNf(x)       (((*(uInt *)&(x) & 0x7f800000) == 0x7f800000) && \
			 ((*(uInt *)&(x) & 0x007fffff) != 0x00000000))
#else
#define	isNaNf(x)	isnanf(x)
#endif

     //	FITS header constants.
     static const int CARD_LEN  = 80;	// Length of a card.
     static const int KEY_LEN   = 8;		// Length of a keyword.
     static const int LOGIC_POSIT = 29;	// T or F in column 30


     AvFitsDataSet::AvFitsDataSet()
{
  fits_init();
}

Boolean AvFitsDataSet::attached()const
{	return (fd_ >= 0) ? TRUE : FALSE;
}

// Force variables to 0 value.
void AvFitsDataSet::fits_init()
{
#ifdef FITSTRACE
	checkFitsTrace();
#endif

  fd_ = -1;
  fileName_ = NULL;
  dataSetName_ = NULL;
  dataUnits_ = "";
  header_len_ = 0;
  filelength_ = 0;
  bitpix_ = 0;
  bytesPerElement_ = 0;
  type_ = -1;
  blanking_ = FALSE;	// TRUE if integral data type and have blankval
  blankval_ = 0;
  dataMin_ = FLOATMAX;
  dataMax_ = -FLOATMAX;
  headerseg_ = dataseg_ = NULL;
  cs_ = NULL;
}

AvFitsDataSet::~AvFitsDataSet()
{
  detach_();
}

// Detach from file if attached. Cleanup and reset pointers.
// May be called internally after a failed attach to clean up.
void AvFitsDataSet::detach_()
{
	if (fileName_) delete [] fileName_;
	if (dataSetName_) delete [] dataSetName_;
	if (headerseg_) delete headerseg_;
	if (dataseg_) delete dataseg_;
	if (cs_) delete cs_;

	if(fd_ >= 0)
		::close(fd_);

	fits_init();	// Reset all the pointers.
}

// Error logging routines.
#if 0
void AvFitsDataSet::error_out(const char *funcname, const char *msg,
			     const char *param)const
{
  cerr << "AvFitsDataSet::" << funcname << ": ";
  if(fileName()!= NULL)
    cerr << "(" << fileName() << ") ";
  cerr << msg;
  if(param != NULL)
    cerr << param << endl;
  else
    cerr << endl;
} 
#else
void AvFitsDataSet::error_out(const char *, const char *,
			     const char *)const
{
}

#endif

void AvFitsDataSet::error_out(const char *funcname, const char *msg,
			     int param)const
{
  cerr << "AvFitsDataSet::" << funcname << ": ";
  if(fileName()!= NULL)
    cerr << "(" << fileName() << ") ";
  cerr << msg << param << endl;
}

void AvFitsDataSet::error_out(const char *funcname, const char *msg,
			     const AvIPosition &param)const
{
  cerr << "AvFitsDataSet::" << funcname << ": ";
  if(fileName()!= NULL)
    cerr << "(" << fileName() << ") ";
  cerr << msg << param << endl;
}

////////////////////////////////////////////////////////////////
//		FITS keyword routines.
////////////////////////////////////////////////////////////////

// Search for a keyword and return a pointer to the card containing it.
// Pointer may not be valid after another call to search.
const char * AvFitsDataSet::search(const char *keyword)const
{
  char *ptr;
  int keylen, index;

  if((keyword == NULL) || (headerseg_ == NULL))
    return NULL;

  if((keylen = (int)strlen(keyword)) <= 0)
    return NULL;
  index = 0;
  
  while((ptr = (char *)headerseg_->pointer(index, CARD_LEN,1)) != NULL)
    {	if(strncmp(ptr, keyword, keylen)== 0)
      return ptr;			// Found card.
    else
      if(strncmp(ptr, "END", 3)== 0)
	return NULL;			// Found "END" card.
    
    index += CARD_LEN;
    }
  return NULL;			// Ran off end;
}

// Return the value of a keyword. If the keyword doesn't exist,
// returns the default and FALSE.
Boolean  AvFitsDataSet::getKeyword(const char *keyword, int &value,
				  int def)const
{
  const char *card = search(keyword);
  Boolean	result;

  if(card != NULL)
    {char *ptr;
    const char *iptr = card+KEY_LEN+2;		// (2 for "= ").
    long	lval = strtol(iptr, &ptr, 10);

    if(ptr == iptr)
      result = FALSE;
    else
      {	value = (int) lval;
      result = TRUE;
      }
    }
  else
    result = FALSE;

  if(!result)
    value = def;
#ifdef FITSTRACE
	if(traceFits())
	{	cerr	<< "AvFitsDataSet::getKeyword(I): "
			<< (result ? "T" : "F")
			<< " " << keyword
			<< " " << value << endl;
	}
#endif

  return result;
}

Boolean  AvFitsDataSet::getKeyword(const char *keyword, float &fvalue,
				  float def)const
{
  const	char *card = search(keyword);
  Boolean result;
  
  if(card != NULL)
    { 
      double value;
      char	*eptr;
      const char *sptr = card + KEY_LEN + 2;
      
      value = strtod(sptr, &eptr);
      if(eptr == sptr)
	result = FALSE;
      else
	{	
	  fvalue = (float) value;
	  result = TRUE;
	}
    }
  else
    result = FALSE;
  
  if(!result)
    fvalue = def;
  
#ifdef FITSTRACE
	if(traceFits())
	{	cerr	<< "AvFitsDataSet::getKeyword(F): "
			<< (result ? "T" : "F")
			<< " " << keyword
			<< " " << fvalue << endl;
	}
#endif

  return result;
}

// get double, float and int assume that strto{l,d} will stop even if the
// card is really messed up. This should always be the case since there
// will be an "END" card somewhere, if nothing else. 
Boolean  AvFitsDataSet::getKeyword(const char *keyword, double &value,
				  double def)const
{
  const char *card = search(keyword);
  Boolean	result;

  if(card != NULL)
    {char *eptr;
    const char *sptr = card + KEY_LEN + 2;
    value = strtod(sptr, &eptr);
    if(eptr == sptr)
      result = FALSE;
    else
      result = TRUE;
    }
  else
    result = FALSE;

  if(!result)
    value = def;

#ifdef FITSTRACE
	if(traceFits())
	{	cerr	<< "AvFitsDataSet::getKeyword(D): "
			<< (result ? "T" : "F")
			<< " " << keyword
			<< " " << value << endl;
	}
#endif

  return result;
}

static void Strcpy(char *out, const char *in)
{	if((out!= NULL) && (in != NULL))
  strcpy(out, in);
}

static const char QUOTE_CHAR = '\'';
static const char *QUOTE_STR = "\'";

// Value should be at least 73 characters long.
// Also returns FALSE if value is NULL.
Boolean  AvFitsDataSet::getKeyword(const char *keyword, char *value,
				  const char *def)const
{
  if(value == NULL)		// No place to store value.
    return FALSE;
  const char *card = search(keyword);

  if(card == NULL)		// Didn't find keyword.
    {	Strcpy(value, def);
    return FALSE;
    }

  int len;
  // Have to copy to temporary buffer to make sure strchr will stop.
  char *start, buf[80];
  memcpy(buf, card+KEY_LEN, CARD_LEN-KEY_LEN);
  buf[CARD_LEN-KEY_LEN] = '\0';

  // find pointer to first quote then point past it.
  start = strchr(buf, QUOTE_CHAR);
  if(start == NULL)
    {	error_out("getKeyword (string)",
		  "Leading quote char not found for string keyword: ",
		  keyword);
    Strcpy(value, def);
    return FALSE;
    }
  start += 1;
  // find distance till next quote (or end of card).
  len = (int)strcspn(start, QUOTE_STR);

  // Trim trailing blanks.
  while( (start[--len] == ' ') && (len >= 0));

  len++;

  // copy into value
  strncpy(value, start, len);
  // null terminate
  value[len] = '\0';
  return TRUE;
}

// FITS' Logical keywords.
Boolean  AvFitsDataSet::getKeyword(const char *keyword, Boolean &value,
				  const Boolean def)const
{const char *ptr = search(keyword);

if(ptr != NULL)
  {	switch (ptr[LOGIC_POSIT]) {
  case 'T':
    value = TRUE;
    break;
  case 'F':
    value = FALSE;
    break;
  default:
    error_out("readCard",
	      "Invalid LOGICAL value for ", keyword);
    value = def;
    return FALSE;
  }
  return TRUE;
  }
else
  {	value = def;
  return FALSE;
  }
}

////////////////////////////////////////////////////////////////
///		AvDataSet related functions
////////////////////////////////////////////////////////////////

const char *AvFitsDataSet::dataSetName()const {return dataSetName_;}
const char *AvFitsDataSet::fileName()const { return fileName_;}

// Size along each axis.
AvIPosition AvFitsDataSet::shape() const
{      
  return shape_;
}

float	AvFitsDataSet::dataMin()const { return dataMin_; }
float	AvFitsDataSet::dataMax()const { return dataMax_; }

// Type an error message saying a keyword wasn't found then detach
// and return FALSE.
// Used only by attach_.
Boolean AvFitsDataSet::noKW(const char *kw)
{
  char buf[128];
  sprintf(buf, "Could not find keyword: %s", kw);
  error_out("attach_", buf);
  detach_();
  return FALSE;
}

Boolean AvFitsDataSet::initattach_(const char* pname)
{
  char *card;
  
  if(attached())
    {	
      error_out("attach_", "Already attached.");
      return FALSE;
    }
  fits_init();
  fd_ = ::open(pname, O_RDONLY);
  if(fd_ < 0) {
    error_out("attach_", "Can't open file: ", pname);
    return FALSE;
  }
  const char *file = strrchr(pname, '/');	// Trim path.
  if(file == NULL)
    fileName_ = Strdup(pname);
  else
    fileName_ = Strdup(file+1);
  
  off_t filesize = fileSize(fd_);
  if( filesize < 0)
    {	error_out("attach_", "Could not stat file.");
	detach_();
	return FALSE;
    }
  filelength_ = (size_t)filesize;
  if(filelength_ == 0)
    {	error_out("attach_", "Empty file.");
	detach_();
	return FALSE;
    }
  
  //		Set up mapping for header.
  
  // Make sure we map in enough to get the first 3 cards.
  //  (minimum mapping will always be 1 page).
  headerseg_ = new MapSegment(fd_, 0, 2880);
  if((card = (char *)headerseg_->pointer()) == NULL)
    {	
      // Probably a directory or AIPS++ file
      // error_out("attach_", "Could not map header.");
	detach_();
	return FALSE;
    }
  
  // Verify that file is probably FITS. These keywords must exist and
  // be in the order listed. Since we're directly using the card pointer,
  // there are no calls to getKeyword since that could change the
  // mapping.
  Boolean isSimple;
  if(    strncmp(card, "SIMPLE  ", 8)
	 ||  strncmp(card+CARD_LEN,	"BITPIX  ", 8)
	 ||  strncmp(card+CARD_LEN*2,	"NAXIS   ", 8))
    {	error_out("attach_", "Bad FITS header.");
	detach_();
	return FALSE;
    }
  // Some more checks.
  if( !getKeyword("SIMPLE", isSimple) || !isSimple)
    {	error_out("attach_", "Bad or missing SIMPLE keyword.");
	detach_();
	return FALSE;
    }
  
  // Compute the length of the header.
  const char *endcard = search("END");
  char	*endbase;
  
  if(endcard == NULL)
    return noKW("END");
  
  endbase = headerseg_->segmentStart();
  // Offset from start of map of end of header.
  header_len_ =  (int)(endcard-endbase)+CARD_LEN;
  header_len_ += headerseg_->baseOffset();// From start of file.
  //		ncards = header_len_;
  //		ncards /= 80;
  //		printf("Header len %d ncards %d\n", header_len_, ncards);
  
  // Round up to the next 2880 byte boundary.
  header_len_ = 2880*((header_len_+2879)/2880);

  return TRUE;
}

//			Attach to FITS file.
// Open for reading, all required cards (num axes etc) will be read from file
//	Open file
//	Memory map the header
//		Check and read header
//	Initial memory map of data.
//		Do minmax search if necessary.
Boolean AvFitsDataSet::attach_(const char *pname)
{
  if (! initattach_(pname))
    return FALSE;

  ////////////////////////////////////////////////////////////////
  // OK, It's a FITS file.
  ////////////////////////////////////////////////////////////////
  
  // Map data segment so start of data is at index '0'.
  // Request that the entire data segment be mapped, but it's OK, if
  // it isn't.
  dataseg_ = new MapSegment(fd_, header_len_, filelength_-header_len_);
  
  // For "reasonable" sized files, this may be the only mapping.
  if(!dataseg_->map(0, dataseg_->maxElements(), 1))
    {	error_out("attach_", "Could not map data segment.");
	detach_();
	return FALSE;
    }
  
  ////////////////////////////////////////////////////////////////
  // Get required keyword/value pairs
  ////////////////////////////////////////////////////////////////
  
  if(!getKeyword("BITPIX", bitpix_))
    return noKW("BITPIX");
  
  // Determine data type by bits per pixel
  switch (bitpix_) {
  case  8:
    type_ = TYPE_UCHAR;
    bytesPerElement_ = 1;
    break;
  case 16:
    type_ = TYPE_16INT;
    bytesPerElement_ = 2;
    break;
  case 32:
    type_ = TYPE_32INT;
    bytesPerElement_ = 4;
    break;
  case -32:
    type_ = TYPE_FLOAT;
    bytesPerElement_ = 4;
    break;
  case -64:		// Not tested.
    type_ = TYPE_DOUBLE;
    bytesPerElement_ = 8;
    break;
  default:
    {	error_out("attach_",
		  "Unsupported value of BITPIX : ", bitpix_);
	detach_();
	return FALSE;
    }
  }
  
  // Blanking value.
  if((type_ != TYPE_FLOAT) && (type_ != TYPE_DOUBLE))
    blanking_ = getKeyword("BLANK", blankval_);
  
  // Number of axes
  int rank;
  if(!getKeyword("NAXIS",  rank))
    return noKW("NAXIS");
  
  if(rank < 2)
    {	
      error_out("attach_", "Rank must be at least 2 : ", rank);
      detach_();
      return FALSE;
    }

  // won't have more than 16 axes
  AvString ctypes[16];
  double crvals[16];
  double cdelts[16];
  double crpixs[16];
  double * pcmatrix = new double[rank*rank];	// Optional PC or CD matrix.
  Boolean foundPC = False;
  int i,j;
   // Initialize to identity.
  memset(pcmatrix, 0, rank*rank*sizeof(double));
  for (i = 0; i < rank; i++) pcmatrix[i*(rank+1)] = 1;

  shape_.resize(rank);

  // To track that there were missing Cxxxx keywords.
  enum {CRPIX=1, CDELT=2, CRVAL=4};
  int cerror=0;

  for(i = 0; i < rank; i++)
    {
      char kword[16], ctype[80], ctypeDefault[20];
      int nelements;
    
#if 0
#ifdef __sun
      // This is only so the Sun memory checker doesn't give rui
      // messages in later strlen() calls.
      if(i == 0)
	{for(int jjj=0; jjj<(int)sizeof(kword); jjj++)kword[jjj] = '\0';}
#endif
#endif
      sprintf(kword, "NAXIS%d", i+1);
      if(!getKeyword(kword, nelements))
	return noKW(kword);
      if(nelements <= 0)
	{	
	  error_out("attach_", "Bad axis length : ", nelements);
	  detach_();
	  return FALSE;
	}
      shape_(i) = nelements;
    
      // get the label
      sprintf(kword, "CTYPE%d", i+1);
      sprintf(ctypeDefault, "Axis%d", i+1);
      getKeyword(kword, ctype, ctypeDefault);
      ctypes[i] = ctype;
    
      // get the reference pixel, reference value, cdelt value
      sprintf(kword, "CRPIX%d", i+1);
      if(!getKeyword(kword, crpixs[i]))
      {	crpixs[i] = 1.0;
	cerror |= CRPIX;
//	return noKW(kword);
      }
      
      sprintf(kword, "CDELT%d", i+1);
      if(!getKeyword(kword, cdelts[i]))
      {	cdelts[i] = 1.0;
	cerror |= CDELT;
//	return noKW(kword);
      }
      else
      if(cdelts[i] == 0.0)	// CDELT KW was set to 0!
      {	cdelts[i] = 1.0;
		cerr << kword << " was set to 0.0, resetting to 1.0." << endl;
      }

      sprintf(kword, "CRVAL%d", i+1);
      if(!getKeyword(kword, crvals[i]))
      {	crvals[i] = 1.0;
//	return noKW(kword);
	cerror |= CRVAL;
      }
    }

  // If no CDELT keywords, check for CD matrix.
  // If ANY CD matrix keyword is found, assume it's all there.
  if(cerror & CDELT)
  { char kword[16];
    for (j = 0; j < rank; j++)
      for (i = 0; i < rank; i++)
      {	  sprintf(kword, "CD%03d%03d", i+1, j+1);
	  if(getKeyword(kword, pcmatrix[j*rank+i], (i == j) ? 1.0 : 0.0))
	    foundPC = True;
	  else
	  { sprintf(kword, "CD%d_%d", i+1, j+1); // alternate KW form.
	    if(getKeyword(kword, pcmatrix[j*rank+i], (i == j) ? 1.0 : 0.0))
		foundPC = True;
	  }
      }
     if(foundPC) cerror &= ~CDELT;	// Turn off CDELT warning.
  }

  // If cerror isn't zero, give a warning message.
  if(cerror != 0)
  {	cerr << "Missing keyword(s) (and defaults): ";
	if(cerror & CRPIX) cerr << "CRPIX(1.0) ";
	if(cerror & CRVAL) cerr << "CRVAL(1.0) ";
	if(cerror & CDELT) cerr << "CDELT(1.0)";
	cerr << endl;
  }

  // These are optional.  If not there, use defaults.
  getKeyword("BSCALE", scale_, (float) 1.0);
  getKeyword("BZERO",  zero_, (float) 0.0);
  
  // (Optional name of dataset).
  {
    char label[80];
    
    if (getKeyword("OBJECT", label))
      dataSetName_ = Strdup(label);
    else
      dataSetName_ = Strdup(fileName());
  }
  
  // Fill in strides used to convert from an AvIPosition to a vector index.
  stride_.resize(rank);
  stride_(0) = 1;
  for(i=1; i< rank; i++)
    stride_(i) = stride_(i-1)*shape_(i-1);
  
  // Make sure that the amount of data matches what is needed.
  {
//    unsigned long len = filelength_-header_len_;
    size_t len = (off_t)(filelength_-(size_t)header_len_);
//    unsigned long nels = nelements();
    size_t nels = (size_t)nelements();
    char buf[80];
    if(nels > len/bytesPerElement_)
      {	sprintf(buf,
       "File data length (%ld bytes) is too small to hold %ld data elements",
		(long)len, (long)nels);
	// Should use lld above on SGI, but gcc was complaining.
      error_out("attach_", buf);
      detach_();
      return FALSE;
      }
#if 0
    //This will go away when we're tired of it.
    else
      if( nels*bytesPerElement_ < len)
	{	sprintf(buf,
			"File data length (%ld) is %ld bytes longer than needed.",
			len, len - nels*bytesPerElement_);
	error_out("attach_", buf);
	}
#endif
  }
  
  getMinMax();

  // Setup the World Coordinate System

  {
    char kword[16];

    // (Optional) Get the longitude rotation reference point
    double longPole;
    getKeyword("LONGPOLE", longPole, 999.0);
    
#if 0
#ifdef __sun
      // This is only so the Sun memory checker doesn't give rui
      // messages in later strlen() calls.
	memset(kword, 0, sizeof(kword));
#endif
#endif

    // (Optional) projection parameters
    double projps[10];
    for (i = 0; i < 10; i++)
      {
	sprintf(kword, "PROJP%d", i+1);
	getKeyword(kword, projps[i], 0.0);
      }
    
#if 0
    // (Optional) pc matrix
    double * pcmatrix = new double[rank*rank];
    memset(pcmatrix, 0, rank*rank*sizeof(double));
    for (i = 0; i < rank; i++) pcmatrix[i*(rank+1)] = 1;
    Boolean foundPC = False;
#endif

    if(!foundPC)	// If found => have CD matrix.
    { for (j = 0; j < rank; j++)
       for (i = 0; i < rank; i++)
	{
//	  sprintf(kword, "PC%03d%03d", i, j);
	  sprintf(kword, "PC%03d%03d", i+1, j+1);
	  if (getKeyword(kword, pcmatrix[j*rank+i], (i == j) ? 1.0 : 0.0) == True)
	    foundPC = True;
	}
    }

    // Scan for CROTA on latitude axis if no pc found
    if (foundPC == False)
      {
	int longAxis = -1;
	int latAxis = -1;
	for (int r = 0; r < rank; r++)
	  {
	    // look for longitude axis
	    if (!strncmp(ctypes[r].chars(), "RA--", 4) ||
		!strncmp(ctypes[r].chars(), "GLON", 4) ||
		!strncmp(ctypes[r].chars(), "ELON", 4))
	      {
#ifdef AVFDS_CROTA_DEBUG
		cerr << "found longitude axis on " << r+1 << endl;
#endif
		longAxis = r;
	      }
	    else if (!strncmp(ctypes[r].chars(), "DEC-", 4) ||
		     !strncmp(ctypes[r].chars(), "GLAT", 4) ||
		     !strncmp(ctypes[r].chars(), "ELAT", 4))
	      {
#ifdef AVFDS_CROTA_DEBUG
		cerr << "found latitude axis on " << r+1 << endl;
#endif
		latAxis = r;
	      }
	  }

	if (longAxis >= 0 && latAxis >= 0)
	  {
	    sprintf(kword, "CROTA%d", latAxis+1);
	    double angle;
	    if (getKeyword(kword, angle, 0.0) == True)
	      {
#ifdef AVFDS_CROTA_DEBUG
		cerr << "found CROTA of "<< angle << " degrees on latitude axis "
		     << latAxis+1 << ".\n";
#endif
		
		// convert to radians and store
		angle *= M_PI/180.0;  
		pcmatrix[latAxis*rank+latAxis] = cos(angle);
		pcmatrix[latAxis*rank+longAxis] = -sin(angle);
		pcmatrix[longAxis*rank+latAxis] = sin(angle);
		pcmatrix[longAxis*rank+longAxis] = cos(angle);
	      }
	  }
      }

    cs_ = new AvCoordinateSystem(rank,
				 ctypes,
				 crvals,
				 crpixs,
				 cdelts,
				 pcmatrix,
				 projps,
				 longPole);
    
    // Establish axis units based on FITS conventions.  Add as
    // needed, consulting AvUnits for units and measurement type.
    for (i = 0; i < rank; i++)
      {
	AvString s = cs_->axisName(i);
	if (s == "RA")
	  cs_->setAxisUnits(i, "degrees", "right ascension");
	else if (s == "ELON" || s == "GLON")
	  cs_->setAxisUnits(i, "degrees", "longitude");
	else if (s == "DEC")
	  cs_->setAxisUnits(i, "degrees", "declination");
	else if (s == "ELAT" || s == "GLAT")
	  cs_->setAxisUnits(i, "degrees", "latitude");
	else if (s == "FREQ")
	  cs_->setAxisUnits(i, "hertz", "frequency");
	else if (s == "VELO")
	  cs_->setAxisUnits(i, "meters/second", "velocity");
	else if (s == "FELO")
	  cs_->setAxisUnits(i, "meters/second", "optical velocity");
	else if (s == "STOKES" || s == "Polarization")
	  cs_->setAxisUnits(i, "stoke index", "polarization");
	else if (s == "SEQ NUM")
	  cs_->setAxisUnits(i, "sequence number", "sequence");
	else
	  cs_->setAxisUnits(i, "", "");
      }
//    delete pcmatrix;
//    pcmatrix = 0;
  }
  delete [] pcmatrix;
  pcmatrix = 0;

  // Establish data units (sets dataUnits, dataUnitsAbbrev, and
  // dataMeasurementName).
  char label[80];

  if (getKeyword("BUNIT", label)) {
    dataUnits_ =  AvString(label);
    dataUnits_.downcase( );
  } else {
    dataUnits_ = AvString("");
  }

  AvString s = downcase(dataUnits_);

  // [ ] Replace later with AvUnits::lookup(str, ...) that
  // can handle prefixes.
  if (s == "mjy/beam")
    setDataUnits("millijanskys/beam", "specific intensity");
  else if (s == "jy/beam")
    setDataUnits("janskys/beam", "specific intensity");
  else if (s == "gauss")
    setDataUnits("gauss", "magnetic field strength");
  else
    {
      AvString prefix, base, type;
      double multiplier;
      if (AvUnits::lookup(s, prefix, base, type, multiplier))
	setDataUnits(prefix+base, type);
      else
	setDataUnits(s, "");
    }

  return TRUE;
}

const char * AvFitsDataSet::dataUnits() const
{ return dataUnits_.chars(); }
const char * AvFitsDataSet::dataUnitsName() const
{ return dataUnitsName_.chars(); }
const char * AvFitsDataSet::dataMeasurementName() const
{ return dataMeasurementName_.chars(); }

const char * AvFitsDataSet::axisName(int axis) const
{ return cs_->axisName(axis); }
const char * AvFitsDataSet::axisUnitsName(int axis) const
{ return cs_->axisUnits(axis); }
const char * AvFitsDataSet::axisMeasurementName(int axis) const
{ return cs_->axisMeasurementName(axis); }

AvString AvFitsDataSet::axisValueUnits(int axis,
				      double value,
				      int nsigfigs) const
{
#ifdef AVDSDEBUG 
  cerr << "AvFR::axisValueUnitsStr(axis=" << axis << ",value=" 
       << value << ",nsigfigs=" << nsigfigs << endl;
#endif
  return cs_->valueUnitsStr(axis, value, nsigfigs); 
}

double AvFitsDataSet::axisValueInUnits(int axis,
				      double value,
				      const AvString& units)
{
#ifdef AVDSDEBUG 
  cerr << "AvFR::axisValueInUnitsStr(axis=" << axis << ",value=" 
       << value << ",units=" << units << endl;
#endif
  return cs_->valueInUnits(axis, value, units); 
}

Boolean AvFitsDataSet::IJKToWorld_(const AvWPosition &ijk,
				  AvWPosition &world) const
{
  Boolean retval = cs_->ijk2world(ijk, world);
  return retval;
}

Boolean AvFitsDataSet::WorldToIJK_(const AvWPosition &world,
				  AvWPosition &ijk) const
{
  return cs_->world2ijk(world, ijk);
}


// Return the size of a file.
//long AvFitsDataSet::fileSize(int fd)
off_t AvFitsDataSet::fileSize(int fd)
{
  struct stat filestuff;

  if (fstat(fd , &filestuff) < 0)
    return -1;
  
//  return (unsigned long) filestuff.st_size;
  return filestuff.st_size;
}

//long AvFitsDataSet::fileSize(const char *fn)
off_t AvFitsDataSet::fileSize(const char *fn)
{
  struct stat filestuff;
  
  if (stat(fn , &filestuff) < 0)
    {//	error_out("fileSize", "Could not stat file.", fn);
      return -1;
    }
//  return (unsigned long) filestuff.st_size;
  return filestuff.st_size;
}

// Get the min/max data values.
// First try the keywords. If that fails, search entire file.
void AvFitsDataSet::getMinMax()
{
	if(getKeyword("DATAMIN", dataMin_) && getKeyword("DATAMAX", dataMax_))
		return;

	dataMin_ = FLOATMAX;
	dataMax_ = -dataMin_;

	size_t nel = nelements();
	// Limit the amount to read. (The actual amount is arbitrary).
	size_t len = (nel > 1024*1024) ? 1024*1024 : nel;
	float *buf = new float[len], blankvalue = blankingValue();
	size_t index=0;
	size_t nread;

	// Treat the file as a vector and read the whole thing a piece at
	// at time.
	while(nel > 0)
	{	if( nel < len)
			len = nel;
		nread = getBuffer(buf, index, len, 1);
		if(nread != len)
			error_out("getMinMax", "Short read\n");
		for(size_t i=0; i< nread; i++)
		{float val = buf[i];
			if(val == blankvalue)
				continue;
			if(val < dataMin_)
				dataMin_ = val;
			if(val > dataMax_)
				dataMax_ = val;
		}
		index += nread;
		nel -= nread;
	}
	{Boolean allBlanked = False;
		if(dataMin_ == FLOATMAX)
		{	dataMin_ = 0.0;
			allBlanked = True;
		}
		if(dataMax_ == -FLOATMAX)
		{	dataMax_ = 0.0;
			allBlanked = True;
		}
		if(allBlanked)
		{	cerr << "Dataset appears to be all blanked.\n";
			cerr	<< "Setting data min to " << dataMin_
				<< " and data max to " << dataMax_ << endl;
		}
		else
			cerr	<< "Computed Data Min " << dataMin_
				<< " Data Max " << dataMax_ << endl;
	}
	delete [] buf;
	buf = 0;
}

// Retrieve data from file using current mapping.
// Handles byte swapping and conversion from file format to float.
// Assumes the entire input buffer is mapped.
// data		buffer to receive data.
// buf		pointer to file data.
// nelements	# of elements to read.
// stride	stride in elements.
void AvFitsDataSet::getBuffer_(float *data, const void *buf,
			      unsigned long nelements, unsigned long stride)
{
  float blankvalue = blankingValue();
  
  switch (type_){
  case TYPE_FLOAT:
    {
      float *fbuf = (float *)buf, val;
      for(size_t j = 0; j < nelements; j++)
	{	
	  val = GETF(*fbuf);
	  fbuf += stride;
	  if(isNaNf(val))
	    *data++ = blankvalue;
	  else
	    *data++ = val;
	}
    }
    break;
  case TYPE_16INT:
    {
      short * sbuf = (short *) buf, sval;
      
      for(size_t j = 0; j < nelements; j++)
	{ 
	  sval = GET2(*sbuf);	// Get (maybe swapped) datum.
	  sbuf += stride;
	  if(blanking_ &&(sval == blankval_))
	    *data++ = blankvalue;
	  else
	    *data++ = scale_ * sval + zero_;
	}
    }
  break;
case TYPE_32INT:
  {int * ibuf = (int *) buf, ival;

  for(size_t j = 0; j < nelements; j++)
    { ival = GET4(*ibuf);
    ibuf += stride;
    if(blanking_ && (ival == blankval_))
      *data++ = blankvalue;
    else
      *data++ = scale_ * ival + zero_;
    }
  }
  break;
case TYPE_UCHAR:
  {unsigned char * cbuf = (unsigned char *) buf, cval;
  for(size_t j = 0; j < nelements; j++)
    {	cval = *cbuf++;
    if(blanking_ && (cval == blankval_))
      *data++ = blankvalue;
    else
      *data++ = scale_ * cval + zero_;
    }
  break;
  }
case TYPE_DOUBLE:	// Untested, may or may not work.
  {double *dbuf = (double *)buf, dval;
  for(size_t j = 0; j < nelements; j++)
    {	dval = GETD(*dbuf);
    dbuf += stride;
    if(isnan(dval))
      *data++ = blankvalue;
    else
      *data++ = (float)dval;
    }
  }
  break;
default:
  error_out("getBuffer", "Bad type_ ", type_);
  break;
}
}

// Return a buffer of data. Converts the requests into mappable lengths
// and repeatedly calls getBuffer_ to do the actuall data access.
// Treats the file as a single vector.
// Returns the number of data points retrieved. (count unless there was
// a problem).
// start, count and stride are in pixels (not bytes).
unsigned long AvFitsDataSet::getBuffer(float *buf, unsigned long start,
			    unsigned long count, unsigned long stride)
{ float *buf0=buf;

	if(buf == NULL)
	{	error_out("getBuffer", "NULL output buffer");
		return 0;
	}

	// Fill buffer with count elements starting at index each stride
	// elements apart.
	size_t elementsLeft = count;
	unsigned long index = start;
	size_t maxElements = dataseg_->maxElements(bytesPerElement_)/stride;

	// In a really bad case, we'll only be able to get one data point
	// per mapping.
	if(maxElements == 0)
		maxElements = 1;

	// Loop picking up as much as we can.
	while(elementsLeft > 0)
	{unsigned long toMap;
	 void *ptr;
	 size_t nels;

		// Request min of what's left and what we can get.
		nels = (maxElements <= elementsLeft)
			? maxElements : elementsLeft;

		toMap = (nels-1)*stride +1;
		// Request mapping for data
		ptr = dataseg_->pointer(index, toMap, bytesPerElement_);
		if(ptr == NULL)
		{	error_out("getBuffer", "Could not map data");
			return count-elementsLeft;
		}

		// Get the data.
		getBuffer_(buf, ptr, nels, stride);
		elementsLeft -= nels;
		index += nels*stride;
		buf += nels;
	}
	return buf-buf0;
}

// Convert a position to a file index.
long AvFitsDataSet::PositionToIndex(const AvIPosition &kp)const
{ unsigned int len = stride_.nelements();

if(kp.nelements() != len)
  {	error_out("PositionToIndex",
		  "Incorrect AvIPosition length: ", kp.nelements());
  return -1;
  }
long val=0;
for(unsigned int i=0; i< len; i++)
  val += kp(i)*stride_(i);
return val;
}

int AvFitsDataSet::getRow(	float *buf,
				const AvIPosition &start, int count,
				int Stride0, int axis)
{long index;
int stride, Stride=Stride0;
 
if(!validIJK(start))
  {	error_out("getRow", "Invalid start position : ", start);
  return 0;
  }
if(count <= 0)
  {	error_out("getRow", "Invalid count : ", count);
  return 0;
  }

if(!validAxis(axis))
  {	error_out("getRow", "Invalid axis: ", axis);
  return 0;
  }

if(Stride <= 0)
  {	error_out("getRow", "Stride was <= 0, using 1.\n");
  Stride = 1;
  }
stride = this->stride(axis)*Stride;

// Make sure that there are count elements available along the
// axis. If not, clip and give a warning message.
int	slen = axisElements(start(axis), axisLength(axis), Stride);
if(slen < count)
  { char cbuf[128];
  // Use supplied stride for printout rather than computed.
  sprintf(cbuf,
	  "\nAxis %d has %d elements with start=%d, stride=%d. %d were requested.",
	  axis, slen, start(axis), Stride, count);
  error_out("getRow", cbuf);
  }
else
  slen = count;

// Convert to index into data.
if((index = PositionToIndex(start)) < 0)
  return 0;

return (int)getBuffer(buf, index, slen, stride);
}

float AvFitsDataSet::getDatum(const AvIPosition &start)
{ 
  float value;
  // Convert to index into data.
  unsigned long index = PositionToIndex(start);
  getBuffer(&value, index, 1, 1);
  return value;
}

bool AvFitsDataSet::isDataSet(const char *filename)
{
	return AvDataSet::isDataSet(filename);
}

static AvDataSetFactoryInfo factoryInfo_ =
	{ &AvFitsDataSet::makeDataSet,
	  &AvFitsDataSet::isDataSet,
	  &AvFitsDataSet::dataSetTypeName
	};

const AvDataSetFactoryInfo *AvFitsDataSet::getFactoryInfo()
{
	return &factoryInfo_;
}


// Open a dataset and return a pointer to the object.
// Returns NULL on failure.
AvDataSet * AvFitsDataSet::makeDataSet()
{  AvDataSet *ds = new AvFitsDataSet();
	return ds;
}
