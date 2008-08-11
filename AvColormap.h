//# Copyright (C) 1995-97 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvColormap.h,v 19.0 2003/07/16 05:46:46 aips2adm Exp $
//
// $Log: AvColormap.h,v $
// Revision 19.0  2003/07/16 05:46:46  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:32  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:00  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:09  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:43  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:07:47  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:31  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:48  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:34  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:12  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:25:03  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:17  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 05:48:57  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.5  1996/06/18  18:42:42  pixton
//  Copyright Update
//
//  Revision 1.4  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/08/03  16:38:44  hr
//  Rewrite. AvColormap is now subclassed from AvConductor and implements
//  a named 4xn array that can notify callbacks when something changes.
//
//  Revision 1.2  1995/07/17  19:56:12  hr
//  Added 'virtual' to destructor of virtual classes.
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#ifndef _SV_COLORMAP_H
#define _SV_COLORMAP_H

#include "AvConductor.h"

/* AvColormap implements a named 4xn array of floating point data. It
is used to store colormap values and does not modify any physical
color table. It can notify another object, via conductor, when values,
label, color model or table length are modified.

While AvColormap does not impose restrictions on the range of stored
values, it is assumed within aipsView that colors range from (0..1).
However, hsvTorgb & rgbTohsv do assume the standard range.

The color model is not used internally except by get/put Colors.

Objects connecting callbacks to AvColormap can be notified when values,
the name, color model or length changes.
*/

class AvColormap : public AvConductor {

public:

  enum COLOR_MODEL { RGB, HSV, GRAYSCALE, DEFAULT };
  enum { NO_HUE = -1};
  // Callback keys, 'subclassed' from AvConductor.
  enum { COMPONENT_CHANGED=USER0, NAME_CHANGED=USER1, LENGTH_CHANGED=USER2,
	 MODEL_CHANGED=USER3};

  // max # of entries, # of entries to use (use all if 0).
  AvColormap (const int maxlen, const int len=0, const COLOR_MODEL model=RGB);

  // Name of colormap.
  virtual void setName(const char *newName);
  virtual char *name()const;
  
  int numColors ()const;
  void setNumColors (const int ncolors);	// Can be up to max colors.
						// Does not change values.
  int maxColors ()const;

  COLOR_MODEL getModelType () const {return model_;}
  // Change model type, does not modify stored values.
  void setModelType (const COLOR_MODEL newModel);

  void initializeCMapValues ();

  // Get/set colors. If NULL, that entry is ignored.
  // Start is the starting location (0..numColors-1).
  // Num is the number of colors to get/put, getNumColors()-start if 0.
  // If model type doesn't match internal setting, values are converted.
  // If model is DEFAULT, no conversion is done.
  void getColors( const COLOR_MODEL model,
		  float *w, float *x, float *y, float *alpha,
		  const int start, const int num=0);
  void putColors( const COLOR_MODEL model,
		  float *w, float *x, float *y, float *alpha, 
		  const int start=0, const int num=0);

  // get/set values of the arrays, ignoring model info.
  void getValues( float *w, float *x, float *y, float *alpha,
		  const int start, const int num=0);
  void putValues( float *w, float *x, float *y, float *alpha, 
		  const int start=0, const int num=0);

  // Same as above, but no alpha.
  void queryValues( float *w, float *x, float *y,
		  const int start, const int num=0);
  void storeValues( float *w, float *x, float *y,
		  const int start=0, const int num=0);

  int numComponents()const;			// # of components (4=RGBA).
  // Get/set individual component (R, G, B or ALPHA).
  void getComponent(const int which, const int start, const int num, float *v);
  void putComponent(const int which, const int start, const int num, float *v);

  // Utility conversion functions.
  static void hsvTorgb ( const float h, const float s, const float v,
			float &red, float &green, float &blue);
  static void hsvTorgb ( const float h, const float s, const float v,
			float *red, float *green, float *blue);
  static void rgbTohsv ( const float r, const float g, const float b,
			 float &h, float &s, float &v);
  static void rgbTohsv
    ( float r, float g, float b, float *h, float *s, float *v);

  // Above for arrays.
  static void hsvTorgb ( const int len,
			float *h, float *s, float *v,
			float *red, float *green, float *blue);
  static void rgbTohsv ( const int len,
			 float *r, float *g, float *b,
			 float *h, float *s, float *v);


  virtual ~AvColormap ();

protected:


private:

  int numColors_, maxColors_;
  COLOR_MODEL model_;
  char *name_;
  int	namelen_;

  float **cmapValues_;
  void checkStartNum(const int Start, const int Num, int &start, int &num);
};

#endif
