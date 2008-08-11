//# Copyright (C) 1995-2002 Board of Trustees of the University of Illinois
//#
//# This software, both binary and source, is copyrighted by The
//# Board of Trustees of the University of Illinois.  Ownership
//# remains with the University.  You should have received a copy 
//# of a licensing agreement with this software.  See the file 
//# "AIPSVIEW_COPYRIGHT", or contact the University at this address:
//#
//#     The NCSA AipsView Visualization System
//#     National Center for Supercomputing Applications
//#     University of Illinois
//#     405 North Mathews Ave.
//#     Urbana, IL 61801
//#     aipsview@ncsa.uiuc.edu
//# --------------------------------------------------
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/aipsview.cc,v 19.0 2003/07/16 05:47:37 aips2adm Exp $
//
// Description:
//
// $Log: aipsview.cc,v $
// Revision 19.0  2003/07/16 05:47:37  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:15  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/14 16:46:14  hravlin
// Modified many literal string variables to be non const so compilers wouldn't
// complain. Changed fallbacks to be const char **.
//
// Revision 17.0  2001/11/12 19:42:45  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:51  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:11:40  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:42  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.6  2000/03/09 19:50:30  hravlin
// Removed some compiler warnings.
//
// Revision 13.5  2000/02/28 17:46:40  hravlin
// Arg count wasn't being incremented for "numColors" in parseargs().
// Added '(void *)' to some X structs to keep a compiler happy.
//
// Revision 13.4  1999/10/25 18:52:30  hravlin
// Added switches/resources for experimental thread support.
//
// Revision 13.3  1999/09/28 16:27:28  hravlin
// Needed to include strings.h.
//
// Revision 13.2  1999/09/27 15:46:25  hravlin
// main() is declared to return an int.
//
// Revision 13.1  1999/09/15 20:52:21  hravlin
// Changes to support TrueColor visuals including handling some command
// line switches locally.
//
// Revision 13.0  1999/08/10 18:40:31  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:33  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:04  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:07  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.3  1998/05/01 22:13:35  hr
// Temporarily increased default for numFreeColors to 25.
//
// Revision 9.2  1998/05/01 21:38:06  hr
// Added new XPalette resources/switches.
//
// Revision 9.1  1998/03/16 17:41:34  bglenden
// Get rid of "file exists" check in displayFile glish event, since, in fact,
// the "file" might be an image expression.
//
// Revision 9.0  1997/08/25 21:29:39  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.4  1997/08/18 19:27:38  hr
// Added an "XmToggleButton.selectColor" entry to the fallbacks.
//
// Revision 8.3  1997/08/14 17:03:01  hr
// The height of the PGPLOT profile is now taken from the fallback resources.
//
// Revision 8.2  1997/08/14 16:31:20  hr
// Set fallback color of X profile's background to black to match PGPLOT window.
//
// Revision 8.1  1997/08/13 21:57:20  hr
// Added xProfile.height to fallback resources.
//
// Revision 8.0  1997/02/20 03:19:03  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.8  1997/02/11 21:03:10  jpixton
// removed extra cout's, fixed a typo.
//
// Revision 7.7  1997/02/06 20:39:17  hr
// Command line switches now have all lowercase versions.
//
// Revision 7.6  1997/01/27 20:40:03  hr
// Changed main() to treat all remaining arguments as files to be displayed.
//
// Revision 7.5  1997/01/09 18:30:41  hr
// Moved fallbacks back into aipsview. Added a number of command line
// switches.
//
// Revision 7.4  1996/12/12 20:59:28  droberts
// Added copyright notice
//
// Revision 7.3  1996/10/25 22:44:31  jpixton
// enabled replaceFile() which should work.
//
// Revision 7.2  1996/10/22 20:06:48  jpixton
// added scrolling buffer stuff.
//
// Revision 7.1  1996/10/11 20:36:52  jpixton
// first pass, glish events enabled.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.8  1996/04/10  17:46:46  pixton
//  made the buggy displayImage() event unavailable.
//
//  Revision 1.7  1996/04/09  18:45:52  hr
//  Added a Ref(v) and and "else" to glishEventHandler.
//
//  Revision 1.6  1995/11/17  21:03:12  pixton
//  added "replaceFitsFile(id, filename)" event for SC '95 support.
//
//  Revision 1.5  1995/11/13  22:17:24  pixton
//  glish displayArray, replaceArray added.
//  still need to finish replaceFitsFile.
//
//  Revision 1.4  1995/05/16  20:43:38  pixton
//  removed leftover debugging info.
//
//  Revision 1.3  1995/05/12  18:27:09  pixton
//  missed Unref fixed, minor cfront2DCC transition bugs fixed.
//
//  Revision 1.2  1995/05/02  20:34:46  pixton
//  The key word is .... "Glish"
//
//  Major Glish events added.  Some events are queued for the user
//  to respond to if desired.  Others return immediately with an answer.
//
//  This construct gives the glish script the power to batch requests or
//  send one at a time.
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#include <string.h>
#include <strings.h>			// strncasecmp
#include <stdlib.h>
#include "AvAccessor.h"
#include "AvImageView.h"
#include "AvFallbacks.h"		// Defines fallbacks.
#include "AvCoordinateSystem.h"
#include "AvString.h"

//////////////////////////////////////////////////////////////////////
//	Support for command line arguments.

// Globally available structure containing the value of the command line
// arguments.
static xOptions optionVars;
const xOptions *getXOptions()
{
	return &optionVars;
}

// Resource names in options, optionVarResources and fallbacks must match!

// Passing literal strings to functions wanting char * is considered
// incorrect.
static char displayWidthsw[] = "-displayWidth";
static char DisplayWidth[] = "*DisplayWidth";
static char displayHeightsw[] = "-displayHeight";
static char DisplayHeight[] = "*DisplayHeight";
static char displaySizesw[] = "-displaySize";
static char DisplaySize[] = "*DisplaySize";
static char pgdevicesw[] = "-pgdevice";
static char pgDevice[] = "*pgDevice";
static char colorVerbositysw[] = "-colorVerbosity";
static char colorVerbosity[] = "*colorVerbosity";
static char numfreecolorssw[] = "-numfreecolors";
static char numFreeColors[] = "*numFreeColors";
static char displaywidthsw[] = "-displaywidth";
static char displayheightsw[] = "-displayheight";
static char displaysizesw[] = "-displaysize";
static char nthreadssw[] = "-nthreads";
static char nThreads[] = "*nThreads";
static char kbPerThreadsw[] = "-kbPerThread";
static char kbPerThread[] = "*kbPerThread";
static char showtimingssw[] = "-showtimings";
static char showTimings[] = "*showTimings";
static char One[] = "1";

// Parameters that get set in the xOptions table.
static XrmOptionDescRec options[] = {
#if 0
	// These can no longer be handled by the X routines since they
	// are used before resources get loaded.
   // # of RW colors to use.
   { "-numColors", "*numColors", XrmoptionSepArg, NULL},
   // Minimum # of RW colors if numColors isn't specified.
   { "-minColors", "*minColors", XrmoptionSepArg, NULL},
   { "-numFreeColors", "*numFreeColors", XrmoptionSepArg, NULL},
   { "-visualID", "*visualID", XrmoptionSepArg, NULL},
   { "-visualid", "*visualID", XrmoptionSepArg, NULL},
   // Want a colormap at least this long.
   { "-nCMapColors", "*nColors", XrmoptionSepArg, NULL},
   ////////////////// Lower case versions.
   { "-numcolors", "*numColors", XrmoptionSepArg, NULL},
   { "-mincolors", "*minColors", XrmoptionSepArg, NULL},
#endif
   { displayWidthsw, DisplayWidth, XrmoptionSepArg, NULL},
   { displayHeightsw, DisplayHeight, XrmoptionSepArg, NULL},
   // If > 0, this sets width = height = size.
   { displaySizesw, DisplaySize, XrmoptionSepArg, NULL},
   // If non zero, print color information.
   { colorVerbositysw, colorVerbosity, XrmoptionSepArg, NULL},
   { numfreecolorssw, numFreeColors, XrmoptionSepArg, NULL},
   { displaywidthsw, DisplayWidth, XrmoptionSepArg, NULL},
   { displayheightsw, DisplayHeight, XrmoptionSepArg, NULL},
   // If > 0, this sets width = height = size.
   { displaysizesw, DisplaySize, XrmoptionSepArg, NULL},
   { pgdevicesw, pgDevice, XrmoptionSepArg, NULL},
   { nthreadssw, nThreads, XrmoptionSepArg, NULL},
   { kbPerThreadsw, kbPerThread, XrmoptionSepArg, NULL},
   { showtimingssw, showTimings, XrmoptionNoArg, One}
   //////////////////
};

static char Width[] = "Width";
static char Height[] = "Height";
static char Size[] = "Size";
static char Verbosity[] = "Verbosity";
static char Device[] = "Device";
static char Threads[] = "Threads";
static char Timings[] = "Timings";

// Describes how X should fill in the optionVars struct.
static XtResource optionVarResources[] = {
	// Resource name   Class   Type	    size
	{ &DisplayWidth[1], Width, XtRInt, sizeof(int),
	//		Offset within xOptions		Initial value.
		XtOffset(xOptionsPtr,displayWidth), XtRString, (void *)"256"},
	{ &DisplayHeight[1], Height, XtRInt, sizeof(int),
	       XtOffset(xOptionsPtr,displayHeight), XtRString, (void *)"256"},
	{ &DisplaySize[1], Size, XtRInt, sizeof(int),
		XtOffset(xOptionsPtr,displaySize), XtRString, (void *)"-1"},
	{ &colorVerbosity[1], Verbosity, XtRInt, sizeof(int),
		XtOffset(xOptionsPtr,colorVerbosity), XtRString, (void *)"0"},
		// (Default is to use default defined in AvPGCanvas.cc)
	{ &pgDevice[1], Device, XtRString, sizeof(String),
		XtOffset(xOptionsPtr,pgDevice), XtRString, NULL},
	// # of threads. Default is to use one, but allow overridding by
	// OMP environment variable.
	{ &nThreads[1], Threads, XtRInt, sizeof(int),
		XtOffset(xOptionsPtr,nThreads), XtRString, (void *)"0"},
	// Minimum # of kb/thread.
	{ &kbPerThread[1], Threads, XtRInt, sizeof(int),
		XtOffset(xOptionsPtr,kbPerThread), XtRString, (void *)"32"},

	{ &showTimings[1], Timings, XtRInt, sizeof(int),
		XtOffset(xOptionsPtr,showTimings), XtRString, (void *)"0"}
	};

// Fallbacks values are overridden by entries in the optionVarResources
// table in aipsview.cc.
static char const *fallbacks[] = {
  "*background:                         gray",
  "*ArrowButton.foreground:             gray",
  "*XmToggleButtonGadget.selectColor:	yellow",
  "*XmToggleButton.selectColor:		yellow",
  "*DisplayWidth:                       256",
  "*DisplayHeight:                      256",
  "*highlightThickness:			0",
  "*XmTextField*marginHeight: 		0",
  "*fontList:                -*-helvetica-medium-r-normal-*-14-*-*-*-*-*-*-*",
  "*menuBar*fontList:        -*-helvetica-bold-o-normal-*-14-*-*-*-*-*-*-*",
  "*FontPlain*fontList:      -*-helvetica-medium-r-normal-*-14-*-*-*-*-*-*-*",
  "*FontBold*fontList:       -*-helvetica-bold-r-normal-*-14-*-*-*-*-*-*-*",
  "*FontItalic*fontList:     -*-helvetica-medium-o-normal-*-14-*-*-*-*-*-*-*",
  "*FontBoldItalic*fontList: -*-helvetica-bold-o-normal-*-14-*-*-*-*-*-*-*",
  // Height of window used to display X profiles.
  "*xProfile.height:		128",
  "*xProfile.background:	black",
  // Height of window used to display PGPLOT profiles.
  "*pgProfile.height:		384",
  NULL
  };

//////////////////////////////////////////////////////////////////////


//
//  This Application will expect to be linked to appropriate glish libraries
//  if GLISH is defined
//
#ifdef GLISH

#include "AvXGlishClient.h"
#include "AvGlishRequestList.h"
#include "AvResidentDataSet.h"
#include "AvScrollingBufferDataSet.h"

// Return true if the item's dimension matches the array's dimension
int checkArrayMatch(AvImageDataItem * item, Value * arrayVal)
{
  if (!item) return 0;
  AvDataSet * data = item->dataset();

  // Compare ranks
  const Value * shapeVal = arrayVal->ExistingAttribute("shape");
  int arrayRank = shapeVal->Length();
  if (data->rank() != arrayRank) return 0;

  // Compare each dimension
  int * shape = shapeVal->IntPtr();
  for (int i = 0; i < arrayRank; i++)
    {
      if (data->shape()(i) != shape[i]) return 0;
    }
  
  // Passed
  return 1;
}

//  Translate glish header info into AvCoordinateSystem
//
//  header record definition:
//
//  header
//    imageShape
//    axisNames
//    referenceValues
//    referencePixels
//    deltas
//    pcmatrix
//    projectionParameters
//    longpole
//
//  Note: Adjust names as needed below in the Value assignments.
//
//  Maybe move to AvCoordinateSystem?
//
AvCoordinateSystem * makeCoordinateSystem(Value * v_header, AvString& errmsg)
{
  int i;

  Value * v_imageShape =           v_header->Field("imageShape");
  Value * v_axisNames =            v_header->Field("axisNames");
  Value * v_referenceValues =      v_header->Field("referenceValues");
  Value * v_referencePixels =      v_header->Field("referencePixels");
  Value * v_deltas =               v_header->Field("deltas");
  Value * v_pcmatrix =             v_header->Field("pcmatrix");
  Value * v_projectionParameters = v_header->Field("projectionParameters");
  Value * v_longpole =             v_header->Field("longpole");

  if (!(v_imageShape && v_axisNames && v_referenceValues && v_referencePixels))
    return NULL;

  // Establish number of dimensions and shape
  v_imageShape->Polymorph(TYPE_INT);
  int nDim = v_imageShape->Length();
//  int *nAxis = v_imageShape->IntPtr();

  // Prep axis names
  AvString ctypes[16];
  if (v_axisNames)
    {
      char ** ctypes_arr = (char **) v_axisNames->StringPtr();
      for (i = 0; i < nDim; i++)
	ctypes[i] = AvString(ctypes_arr[i]);
    }
  else
    {
      errmsg = "Axis names not found in glish header record";
      return NULL;
    }

  // CRVAL 
  double * crvals;
  if (v_referenceValues) 
    {
      v_referenceValues->Polymorph(TYPE_DOUBLE);
      crvals = v_referenceValues->DoublePtr();
    }
  else
    {
      errmsg = "Reference Values not found in glish header record";
      return NULL;
    }

  // CRPIX
  double * crpixs;
  if (v_referencePixels)
    {
      v_referencePixels->Polymorph(TYPE_DOUBLE);
      crpixs = v_referencePixels->DoublePtr();
    }
  else
    {
      errmsg = "Reference Pixels not found in glish header record";
      return NULL;
    }

  // CDELT
  double * cdelts;
  if (v_deltas)
    {
      v_deltas->Polymorph(TYPE_DOUBLE);
      cdelts = v_deltas->DoublePtr();
    }
  else
    {
      errmsg = "Delta values not found in glish header record";
      return NULL;
    }

  // PCMatrix
  double * pcmatrix = new double[nDim * nDim];
  if (v_pcmatrix)
    {
      // copy from v_pcmatrix
      v_pcmatrix->Polymorph(TYPE_DOUBLE);
      memcpy(pcmatrix, v_pcmatrix->DoublePtr(), nDim*nDim*sizeof(double));
    }
  else 
    {
      // make identity matrix
      memset(pcmatrix, 0, nDim*nDim*sizeof(double));
      for (i = 0; i < nDim; i++) pcmatrix[i*nDim + i] = 1.0;
    }

  // Projection Parameters
  double projParms[10];
  memset(projParms, 0, 10*sizeof(double));
  if (v_projectionParameters)
    {
      // copy up to the first 10 items from v_projParms
      v_projectionParameters->Polymorph(TYPE_DOUBLE);
      unsigned int n = v_projectionParameters->Length();
      if (n > 10) n = 10;
      memcpy(projParms, v_projectionParameters->DoublePtr(), n*sizeof(double));
    }

  // Longpole
  double longpole = v_longpole ? v_longpole->DoubleVal() : 999.0;

  // Build and return coordinate system from this info
  AvCoordinateSystem * retval =  
    new AvCoordinateSystem(nDim, ctypes, crvals, crpixs, cdelts,
			   pcmatrix, projParms, longpole);

  delete [] pcmatrix;

  return retval;
}

//  image.data
//  image.header
//  image.header.imageShape
//  image.header.axisNames
//  image.header.referenceValues
//  image.header.referencePixels
//  image.header.deltas
//  image.header.pcmatrix
//  image.header.projectionParameters
//  image.header.longpole
//  image.header.dataUnits
//  image.header.name

AvDataSet * makeDataSet(Value * v, AvString & errmsg)
{
  AvDataSet * ds = NULL;

  // Make sure the incoming type is a record
  if (v->Type() != TYPE_RECORD)
    {
      errmsg = "requires a record";
      return NULL;
    }
  
  // Have image record, now create the coordinate system from
  // the header subrecord and build the dataset
  Value * v_header = v->Field("header");
  if (!v_header)
    {
      errmsg = "requires an image header";
      return NULL;
    }
  
  AvCoordinateSystem * cs = makeCoordinateSystem(v_header, errmsg);
  if (!cs)
    {
      return NULL;
    }

  AvIPosition shape(cs->nAxes());

  // Get the data array pointer
  Value * v_dataArray = v->Field("data");
  if (!v_dataArray)
    { 
      errmsg = "unable to locate data array";
      return NULL;
    }
  
  // Check data array attribute for consistency
  const Value * v_arrshape = v_dataArray->ExistingAttribute("shape");
  if ((unsigned int)v_arrshape->Length() != shape.nelements())
    {
      errmsg = "inconsistent header/data shape (ndim)";
      return NULL;
    }
  
  for (unsigned int i = 0; i < shape.nelements(); i++)
    {
      shape(i) = (v_arrshape->IntPtr())[i];
    }

  v_dataArray->Polymorph(TYPE_FLOAT);

  // Look for data units, use "" if not found
  Value * v_dataUnits = v_header->Field("dataUnits");
  Value * v_name = v_header->Field("name");

  Value * v_blankVal = v_header->Field("blankVal");

  float blankVal = v_blankVal ? v_blankVal->FloatVal() : AvDataSet::defaultBlankingValue();

  Value * v_dataMin = v_header->Field("dataMin");
  Value * v_dataMax = v_header->Field("dataMax");
  
  if (v_dataMin && v_dataMax)
    {
      ds = new AvResidentDataSet(v_dataArray->FloatPtr(),
				 shape,
				 v_dataUnits ? (const char *) *v_dataUnits->StringPtr() : "",
				 *cs,
				 v_name ? (const char *) *v_name->StringPtr() : "",
				 blankVal,
				 0,
				 v_dataMin->FloatVal(),
				 v_dataMax->FloatVal());
    }
  else
    {
      ds = new AvResidentDataSet(v_dataArray->FloatPtr(),
				 shape,
				 v_dataUnits ? (const char *) *v_dataUnits->StringPtr() : "",
				 *cs,
				 v_name ? (const char *) *v_name->StringPtr() : "",
				 blankVal,
				 0);
    }

  ds->ref();
  ds->attach("");

  return ds;
}

//  Although the file descriptor is available, only glish events may be
//  sent to it.  Do not alter the fd in any way or attempt to read() or
//  write data to it.  Probably this parameter will disappear in the future.
//
void glishEventHandler(AvXGlishClient * xgclient,  // pointer to client class
		       GlishEvent * ge,          // event that occured
		       int,                      // fd, see above
		       XtPointer clientData)     // user data ptr
{
  AvImageView * app = (AvImageView *) clientData;

  //----------------------------------------------------------
  //
  //            "displayFile <filename>"
  //
  if (!strcmp(ge->name, "displayFile"))
    {
      Value * v = ge->value;
      char * filename = v->StringVal();
      
      Ref(v);
      int returnID = app->displayData(filename);
      Value * idv = new Value(returnID);
      xgclient->PostEvent("imageID", idv);
      Unref(idv);

      Unref(v);
      delete [] filename;  // StringVal() dynamically allocs return value!
    }

  //------------------------------------------------------------
  //
  //           "replaceFile <filename>"
  //
  //
  else if (!strcmp(ge->name, "replaceFile"))
    {
      Value * eventVal = ge->value;

      if (eventVal->Length() != 2)
	{
	  printf("replaceFitsFile() usage: replaceFile(Id, Filename)\n");
	}
      else
	{
	  Value * imageIdVal = eventVal->Field("arg1");
	  Value * nameVal = eventVal->Field("arg2");

	  if (!imageIdVal || !nameVal)
	    {
	      printf("Problem reading values\n");
	      exit(1);
	    }
	  
	  int id = imageIdVal->IntVal();
	  char * filename = nameVal->StringVal();
	  
	  if (access(filename, F_OK) == -1)
	    {
	      xgclient->PostEvent("error", "Can't locate file '%s'", filename);
	    }
	  else if (access(filename, R_OK) == -1)
	    {
	      xgclient->PostEvent("error", "Read permission denied on file '%s'.", 
				  filename);
	    }
	  else      // File Access Ok
	    {
	      int returnID = app->replaceData(filename, id);
	      Value * idv = new Value(returnID);
	      xgclient->PostEvent("imageID", idv);
	      Unref(idv);
	    }

	  delete [] filename;  // StringVal() dynamically allocs return value!
	}
    }

  //------------------------------------------------------------
  //
  //           "displayImage([Image])"
  //
  //     value.filename - filename
  //
  //

  else if (!strcmp(ge->name, "displayImage"))
    {
      Value * v = ge->value;
      if (!v) 
	{
	  xgclient->PostEvent("error","Could not get image record");
	  return; 
	}

      AvString errmsg;

      // Build the dataset
      AvDataSet * ds = makeDataSet(v, errmsg);
      if (!ds)
	{
	  xgclient->PostEvent("error", ("displayImage " + errmsg).chars());
	  return;
	}
      
      int returnID = app->displayData(ds);
      Value * rv = new Value (returnID);
      xgclient->PostEvent("imageID", rv);
      Unref(rv);
    }

  //------------------------------------------------------------
  //
  //           "displayArray([array])"
  //
  //  Display the raw array and provide a mock linear coordinate
  //  system.
  //
  //

  else if (!strcmp(ge->name, "displayArray"))
    {
      static int displayArrayCount = 0;
      int i;

      Value * v_dataArray = ge->value;
      v_dataArray->Polymorph(TYPE_FLOAT);

      const Value * v_shape = v_dataArray->ExistingAttribute("shape");

      int ndim = v_shape->Length();
      int * shape_arr = v_shape->IntPtr();

      // Build pixel-based linear coordinate system
      AvCoordinateSystem cs(ndim);
							      
      AvIPosition shape(ndim);
      for (i = 0; i < ndim; i++) shape(i) = shape_arr[i];


      char name[80];
      sprintf(name, "Array#%d", ++displayArrayCount);


      AvDataSet * ds = new AvResidentDataSet(v_dataArray->FloatPtr(),
					     shape,
					     "",
					     cs,
					     name,
					     AvDataSet::defaultBlankingValue(),
					     0);
      ds->ref();
      ds->attach("name"); // just sets attached_

      int returnID = app->displayData(ds);
      Value * v = new Value (returnID);
      xgclient->PostEvent("imageID", v);
      Unref(v);
    }

  //------------------------------------------------------------------
  //
  //        "scrollBufBegin(image, size)"
  //
  else if (!strcmp(ge->name, "scrollBufBegin"))
    {
      Value * eventVal = ge->value;
      if (eventVal->Length() != 2)
	{
	  xgclient->PostEvent("error", "usage: scrollBufBegin(Image, size)");
	  return;
	}

      Value * v_image = eventVal->Field("arg1");
      Value * v_count = eventVal->Field("arg2");

      if (!v_image || !v_count)
	{
	  xgclient->PostEvent("error", "usage: scrollBufBegin(Image, size)");
	  return;
	}

      // Build the first dataset of the scrolling buffer
      AvString errmsg;
      AvDataSet * ds = makeDataSet(v_image, errmsg);
      if (!ds)
	{
	  xgclient->PostEvent("error", errmsg.chars());
	  return;
	}

      // Get the number of datasets the scrolling buffer ds will hold
      int count = v_count->IntVal();

      // Build the scrolling buffer dataset
      AvDataSet * sbds = new AvScrollingBufferDataSet(count, ds);
      sbds->ref();
      sbds->attach("");
	  
      // Wire it into aipsview
      int returnID = app->displayData(sbds);
      Value * v = new Value (returnID);
      xgclient->PostEvent("imageID", v);
      Unref(v);
    }

  //------------------------------------------------------------------
  //
  //        "scrollBufAddImage(id, image)"
  //
  else if (!strcmp(ge->name, "scrollBufAddImage"))
    {
      Value * eventVal = ge->value;
      if (eventVal->Length() != 2)
	{
	  xgclient->PostEvent("error", "usage: scrollBufAddImage(Id, Image)");
	  return;
	}

      Value * v_id = eventVal->Field("arg1");
      Value * v_image = eventVal->Field("arg2");

      if (!v_image || !v_id)
	{
	  xgclient->PostEvent("error", "usage: scrollBufAddImage(Id, Image)");
	  return;
	}
      
      int id = v_id->IntVal();

      // Get handle for id'd datset and make sure its an scrollingbuffer
      AvImageDataItem * idi = app->search(id);
      if (!idi)
	{
	  xgclient->PostEvent("error", "Could not find dataset for id given");
	  return;
	}

      if (strcmp(idi->dataset()->dataSetType(), "Scrolling Buffer"))
	{
	  xgclient->PostEvent("error", "Id given is not a scrolling buffer type");
	  return;
	}
      
      AvScrollingBufferDataSet * sbds = (AvScrollingBufferDataSet *) idi->dataset();

      // Ok, now build the dataset from the image info
      AvString errmsg;
      AvDataSet * ds = makeDataSet(v_image, errmsg);
      if (!ds)
	{
	  xgclient->PostEvent("error", errmsg.chars());
	  return;
	}

      // [ ] Need error checking here
      sbds->addDataSet(ds);
      
      // idi->accessor()->clipChanged();
      idi->dataModified();
      idi->updateViews();
    }

  //------------------------------------------------------------------
  //
  //        "replaceArray(id, array)"
  //
  //
#if 0
  else if (!strcmp(ge->name, "replaceArray"))
    {
      printf("replaceArray...\n");

      Value * eventVal = ge->value;

      if (eventVal->Length() != 2)
	{
	  printf("replaceArray() usage: replaceArray(Id, Array)\n");
	}
      else
	{
	  Value * imageIdVal = eventVal->Field("arg1");
	  Value * arrayVal = eventVal->Field("arg2");
      
	  if (!imageIdVal || !arrayVal)
	    {
	      printf("Problem reading values\n");
	      exit(1);
	    }
	  
	  int imageID = imageIdVal->IntVal();
	  
	  printf("AipsView replacing data for imageID %d\n", imageID);
	  
	  arrayVal->Polymorph(TYPE_FLOAT);
	  
	  const Value * shapeVal = arrayVal->ExistingAttribute("shape");
	  int rank = shapeVal->Length();
	  int * shape = shapeVal->IntPtr();
	  
	  if (!shape) { printf("Couldn't get shape info\n"); exit(1); }
	      
	  AvImageDataItem * item = app->search(imageID);
	  if (checkArrayMatch(item, arrayVal))
	    {
	      // Replace the data
	      float * data = item->data()->data()->getValues(0);
	      memcpy(data, arrayVal->FloatPtr(), arrayVal->Length()*sizeof(float));

	      // Update the displays
	      item->updateViews();
	    }
	  else
	    {
	      // Report Bad Match
	      printf("replaceArray() : bad Match\n");
	    }
	}
    }
#endif

  //------------------------------------------------------------
  //
  //           "replaceImage(id, [Image])"
  //  

#if 0

  else if (!strcmp(ge->name, "replaceImage"))
    {
      int err = 0;
      printf("replaceImage\n");
      Value * v = ge->value;

      int imageID;
      Value * imageRecord = NULL;

      if (v)
	{
	  // Do some checking on the input values
	  Value * idval = v->Field("arg1");
	  int imageID = -1;
	  if (!idval) 
	    {
	      err++;
	    }
	  else
	    {
	      imageID = idval->IntVal();
	    }

	  imageRecord = v->Field("arg2");
	  if (!imageRecord) err++;
	}

      // Setup a temporary Gridded Data
      AvImageDataItem * item = NULL;

      if (!err) item = app->search(imageID);

      if (item && !err)
	{
	  AvGriddedData * data = item->data();

	  // Compare dimensions
	  Value * header = imageRecord->Field("header");
	  Value * gvnaxis = header->Field("imageShape");

	  gvnaxis->Polymorph(TYPE_INT);
	  int nAxes = gvnaxis->Length();
	  int *nAxis = gvnaxis->IntPtr();

	  if (nAxes == data->rank())
	    {
	      for (int i = 0; i < nAxes; i++)
		{
		  if (nAxis[i] != (data->grid()->getMaxIJK(i) + data->grid()->getMinIJK(i) + 1))
		    {
		      Value * v = new Value(imageID);
		      xgclient->PostEvent("Bad shape Match", v);
		      Unref(v);
		    }
		}

	      // Shape match, coerce data array to float, copy, and unref
	      Value * glishData = imageRecord->Field("data");
	      glishData->Polymorph(TYPE_FLOAT);

	      void * dataPtr = data->data()->getValues(0);
	      memcpy(dataPtr, glishData->FloatPtr(), data->data()->getNum()*sizeof(float));
	      item->updateViews();

	      xgclient->PostEvent("imageID", v->Field("arg1"));
	    }
	  else
	    {
	      Value * v = new Value(imageID);
	      xgclient->PostEvent("Images are of unequal dimension", v);
	      Unref(v);
	    }
	}
      else
	{
	  Value * v = new Value(imageID);
	  xgclient->PostEvent("badId", v);
	  Unref(v);	  
	}

      
    }
#endif

  //------------------------------------------------------------
  //
  //           "releaseImage(id)"
  //

  else if (!strcmp(ge->name, "releaseImage"))
    {
      Value * v = ge->value;
      int id = v->IntVal();

      AvImageDataItem * item = app->search(id);

      if (item)
	{
	  // [ ] Ask user to verify remove
	  app->closeSession(item);
	}
      else
	{
	  Value * v = new Value(id);
	  xgclient->PostEvent("badId", v);
	  Unref(v);
	}
    }

  //------------------------------------------------------------
  //
  //           "selectImageRegion(id)"
  //           "getImageRegion(id)"
  //           "selectImageLocation(id)"
  //
  //  These events are queued by aipsView.  Responses are sent
  //  when the user makes the selection.
  //
  //
  else if ((!strcmp(ge->name, "selectImageRegion")) ||
	   (!strcmp(ge->name, "getImageRegion")) ||
	   (!strcmp(ge->name, "selectImageLocation")) ||
	   (!strcmp(ge->name, "releaseImage")))
    {
       Value * v = ge->value;
       int id = v->IntVal();

       AvImageDataItem * idi = app->search(id);
       
       if (!idi) 
	 {
	   xgclient->PostEvent("badId", v);
	   Unref(v);
	 }
       else
	 {
	   app->glishRequestList()->addRequest(ge->name,
					       id,
					       idi->name(),
					       "(Hist region)");
	 }
    }

  //------------------------------------------------------------
  //
  //           "queryImageRegion(id)"
  //
  else if (!strcmp(ge->name, "queryImageRegion"))
    {
       Value * v = ge->value;
       int id = v->IntVal();

       AvImageDataItem * idi = app->search(id);

       if (idi)
	 {
	   // id valid
	   if (idi->lastRegionValid())
	     {
	       // Correct lastRegion to return two endpoints
	       int * v1 = idi->lastRegion();  // lastRegion format: x1 y1 z1 x2 y2 z2
	       int * v2 = v1 + 3;
	       Value * v = create_record();
	       v->SetField("imageID", idi->id());
	       Value * vert1 = new Value(v1, 3, PRESERVE_ARRAY);
	       Value * vert2 = new Value(v2, 3, PRESERVE_ARRAY);
	       Value * verts = create_record();
	       verts->SetField("1", vert1);
	       verts->SetField("2", vert2);
	       v->SetField("vertices", verts);
	       xgclient->PostEvent("imageRegion", v);
	       Unref(vert1);
	       Unref(vert2);
	       Unref(verts);
	       Unref(v);
	     }
	   else
	     {
	       // Add to user queue
	       app->glishRequestList()->addRequest(ge->name,
						   id,
						   idi->name(),
						   ge->name);
	     }
	 }
       else
	 {
	   xgclient->PostEvent("error", "badId");
	 }
    }

  //------------------------------------------------------------
  //
  //           "queryImageLocation(id)"
  //

  else if (!strcmp(ge->name, "queryImageLocation"))
    {
       Value * v = ge->value;
       int id = v->IntVal();

       AvImageDataItem * idi = app->search(id);

       if (idi)
	 {
	   // id valid
	   if (idi->lastLocationValid())
	     {
	       int n = idi->dataset()->rank();

	       AvIPosition ipos(n);
	       ipos = idi->lastLocation();

	       AvWPosition wpos(n);
	       idi->dataset()->IJKToWorld(ipos, wpos);

	       int * ivec = new int[n];
	       double * wvec = new double[n];

	       for (int i = 0; i < n; i++)
		 {
		   ivec[i] = ipos(i);
		   wvec[i] = wpos(i);
		 }

	       Value * v = create_record();
	       v->SetField("imageID", idi->id());
	       v->SetField("pixelLocation", ivec, n, COPY_ARRAY);
	       v->SetField("worldLocation", wvec, n, COPY_ARRAY);
	       v->SetField("value", idi->lastValue());
	       xgclient->PostEvent("imageLocation", v);

	       delete [] ivec;
	       delete [] wvec;

	       Unref(v);
	     }
	   else
	     {
	       // Add to user queue
	       app->glishRequestList()->addRequest(ge->name,
						   id,
						   idi->name(),
						   ge->name);
	     }
	 }
       else
	 {
	   xgclient->PostEvent("error", "badId");
	 }
    }

  //------------------------------------------------------------
  //
  //           "queryImages()"
  //

  else if (!strcmp(ge->name, "queryImages"))
    {
      int n = app->nSessions();
      int * ids = new int[n];
      char **names = new char *[n];
      int count = 0;

      AvListIter<AvImageDataItem *> it(app->sessionList());
      while (!it.done())
	{
	  AvImageDataItem * item = it.get();
	  ids[count] = item->id();
	  names[count] = (char *) item->name();
	  count++;
	}
      
      Value * reply = create_record();

      reply->SetField("count", n);
      reply->SetField("ids", ids, n, PRESERVE_ARRAY);
      reply->SetField("names", (charptr *) names, n, PRESERVE_ARRAY);

      xgclient->PostEvent("imageList", reply);
      
      Unref(reply);
      
      delete ids;
      delete names;
    }

  //------------------------------------------------------------
  //
  //           "indexBase(0 or 1)"
  //
#if 0
  else if (!strcmp(ge->name, "indexBase"))
    {
       Value * v = ge->value;
       int base = v->IntVal();
      
       if (base == 0)
	 {
	   glishIndexBase_ = 0;
	   printf("indexBase set to zero\n");
	 }
       else if (base == 1)
	 {
	   glishIndexBase_ = 1;
	   printf("indexBase set to one\n");
	 }
       else
	 {
	   xgclient->PostEvent("error", "invalidParameter");
	 }
    }
#endif

  //-----------------------------------------------------------
  //
  //  Notify sender that event was not recognized
  //
  else
    xgclient->Unrecognized();
}

// ifdef Glish
#endif

// Convert a string to an int or a default.
static int StrToInt(const char *str, const int def = 0)
{ int oval;
  char *eptr;

	long val = strtol(str, &eptr, 0);
	if(eptr != str)
		oval = (int)val;
	else
		oval = def;
	return oval;
}

// Initialize those optionVar variables that aren't handled by the X
// resource mechanism.
static void initializeVars()
{
	optionVars.visualID = -1;
	optionVars.nCMapColors = 256;
	optionVars.numColors = NULL;
	optionVars.numFreeColors = 25;
	optionVars.minColors = 16;
	optionVars.colorVerbosity = 0;
}

static void parseArgs(int &argc, char **argv)
{ int nargc = 1;
  char **nargv = new char *[argc];

	nargv[0] = argv[0];

	// We'd like to use resources/command line args to specify some
	// variables. Unfortunately, they haven't been processed yet.
	// We therefore scan the command line args ourselves.
	for(int n=1; n < argc; n++)
	{
		if(strcasecmp(argv[n], "-visualid") == 0)
		{	optionVars.visualID = StrToInt(argv[++n], -1);
		}
		else
		if(strcasecmp(argv[n], "-nCMapColors") == 0)
		{	optionVars.nCMapColors = StrToInt(argv[++n], 256);
		}
		else
		if(strcasecmp(argv[n], "-numColors") == 0)
		{	optionVars.numColors = argv[++n];
		}
		else
		if(strcasecmp(argv[n], "-numFreeColors") == 0)
		{	optionVars.numFreeColors = StrToInt(argv[++n], 25);
		}
		else
		if(strcasecmp(argv[n], "-minColors") == 0)
		{	optionVars.nCMapColors = StrToInt(argv[++n], 256);
		}
		else
		if(strcasecmp(argv[n], "-colorVerbosity") == 0)
		{	optionVars.colorVerbosity = StrToInt(argv[++n], 0);
		}
		else	// Copy unused args.
			nargv[nargc++] = argv[n];	

	}

	// If some args were processed, replace argv with the list of
	// unprocessed arg strings.
	if(nargc < argc)
	{	for(int i=1; i< nargc; i++)
			argv[i] = nargv[i];
		argv[nargc] = NULL;
		argc = nargc;
	}
	delete [] nargv;
}

int main (int argc, char ** argv)
{

  // Initialize those optionVar variables that aren't handled by the X
  // resource mechanism.
  initializeVars();
  // Handle arguments that get used before X resources are enabled.
  parseArgs(argc, argv);

  // create an instance of the ImageView application
  AvImageView * app = new AvImageView ("AipsView",
				       options, XtNumber(options),
				       argc, argv,
				       fallbacks, NULL, 0,
				       (XtPointer) &optionVars,
				       optionVarResources,
				       XtNumber(optionVarResources));
#ifdef GLISH

  // attach glish event handler to X.  The glish client
  // removes glish inputs from the cmd line.  GlishClient uses
  // XtAppAddInput() to track connections from other programs.
  
  AvXGlishClient * xgc = new AvXGlishClient(argc, argv,
					    app->appContext(),
					    glishEventHandler,
					    app);
  
  AvGlishRequestList * rl = new AvGlishRequestList(app->baseWidget(), xgc); 
  
  app->glishRequestList(rl);

#endif
  
  argv++;
  argc--;
  
  // if the name of a data file was supplied, display it on start up
//  if (argc) app->displayData (*argv);
 // Treat all remaining arguments as files to be opened.
 while(--argc >= 0)
   app->displayData (*argv++);

  
  app->go ();
  
  return 0;
}
