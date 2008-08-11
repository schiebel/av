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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvColormap.cc,v 19.0 2003/07/16 05:46:50 aips2adm Exp $
//
// $Log: AvColormap.cc,v $
// Revision 19.0  2003/07/16 05:46:50  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:36  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:04  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:13  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:59  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:07:52  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/08/25 19:57:34  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:39:36  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:57  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:47  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:27  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:25:27  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:32  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 05:47:23  droberts
// Final update from monet archive.
//
// Revision 1.3  1996/09/11 19:08:26  hr
// Work around problem with Sun's memory checker.
//
// Revision 1.2  1996/08/14 17:26:29  hr
// Changes to remove g++ warnings (from bglenden).
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.5  1996/06/18  18:45:53  pixton
//  Copyright Update
//
//  Revision 1.4  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.3  1995/08/03  16:42:44  hr
//  Rewrite to be subclass of AvConductor.
//
//  Revision 1.2  1995/07/14  20:14:56  hr
//  Removed unneeded reference to AvCanvas.h
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#include <stdio.h>
#include "AvColormap.h"

#include <math.h>

#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

static int mapNum=0;		// Used to give distinct default names.

// Store RGBA or HSVA. Changing this value won't work without
// modifying some code.
static const int NUM_COMPONENTS = 4;

AvColormap::AvColormap (const int maxcolors, const int num,
					const COLOR_MODEL model)
{
  maxColors_ = maxcolors;
  numColors_ = 0;		// To keep memory checkers happy.
  setNumColors(num);
  model_ = model;
  
  cmapValues_ = new float*[NUM_COMPONENTS];
  // Allocate the maximum space that we might need.
  for (int i=0; i<NUM_COMPONENTS; i++)
    cmapValues_[i] = new float[maxColors_];

//  initializeCMapValues();
  namelen_ = 0;
  name_ = NULL;
  {char buf[32];
#ifdef __sun
	// Work around problem with Sun's memory checker.
	memset(buf, 0, sizeof(buf));
#endif
	sprintf(buf, "colormap%0d", mapNum++);
	setName(buf);
  }
}

AvColormap::~AvColormap ()
{
  if(cmapValues_ != NULL)
  {	for(int i=0; i< NUM_COMPONENTS; i++)
		delete [] cmapValues_[i];
	delete [] cmapValues_;
  }
  delete [] name_;
}

void AvColormap::initializeCMapValues()
{
  float scale;
  int num = numColors();
  scale = 1.0/(float)num;
  // Assumes NUM_COMPONENTS = 4!!!!
  for (int i=0; i<num; i++) {
    cmapValues_[0][i] = i * scale;
    cmapValues_[1][i] = i * scale;
    cmapValues_[2][i] = i * scale;
    cmapValues_[3][i] = 1.0;		// Alpha.
  }

/*  for (i=0; i<numColors; i++) {
    hsvTorgb (cmapValues[0][i], cmapValues[1][i], cmapValues[2][i],
	      &r1, &g1, &b1);
    cmapValues[0][i] = r1;
    cmapValues[1][i] = g1;
    cmapValues[2][i] = b1;
  } */
    
}


void AvColormap::setModelType (const COLOR_MODEL newModel)
{
  if(model_ != newModel)
  {	model_ = newModel;
	invokeCallbacks(MODEL_CHANGED);
  }
}

int AvColormap::numComponents()const
{
	return NUM_COMPONENTS;
}

// Set the number of colors to use, up to the max available.
void AvColormap::setNumColors (const int num)
{
int n;

  n = ((num <= 0) || (num >= maxColors())) ?
			maxColors() : num;
  if(n != numColors_)
  {	numColors_ = n;
	invokeCallbacks(LENGTH_CHANGED);
  }
}

// Check for valid start and num values.
// start is constrained to be between (0..numColors-1). Num is constrained
// to be between (1..numColors-start).
// If Num is 0, transfer as many as will fit.
void AvColormap::checkStartNum(	const int Start, const int Num,
				int &start, int &num)
{
int	max = numColors();

	if(Start < 0)
		start = 0;
	else
	if(Start >= max)
		start = max - 1;
	else
		start = Start;

	max -= start;
	if((Num <= 0) || (Num > max))
		num = max;
	else
		num = Num;
}

void AvColormap::getComponent(const int which, const int Start,
						const int Num, float *v)
{
int	start, num;
register float *in, *out;
register int count;

  checkStartNum(Start, Num, start, num);
  if((which < 0) || (which >= NUM_COMPONENTS))
	return;

  if((out = v) != NULL)
  {	count = num;
	in = cmapValues_[which];
	while( --count >= 0)
		*out++ = *in++;
  }
}

void AvColormap::putComponent(const int which, const int Start,
						const int Num, float *v)
{
int	start, num;
register float *in, *out;
register int count;

  checkStartNum(Start, Num, start, num);

  if(num <= 0)
	return;

  if((which < 0) || (which >= NUM_COMPONENTS))
	return;

  if((in = v) != NULL)
  {	count = num;
	out = cmapValues_[which];
	while( --count >= 0)
		*out++ = *in++;
	invokeCallbacks(COMPONENT_CHANGED);
  }
}

// Get values from storage.
void AvColormap::getValues(float * red, float * green, float * blue,
			   float *alpha, const int Start, const int Num)
{
int	start, num;
register float *in, *out;
register int count;

  checkStartNum(Start, Num, start, num);

  if((out = red) != NULL)
  {	count = num;
	in = cmapValues_[0];
	while( --count >= 0)
		*out++ = *in++;
  }

  if((out = blue) != NULL)
  {	count = num;
	in = cmapValues_[1];
	while( --count >= 0)
		*out++ = *in++;
  }

  if((out = green) != NULL)
  {	count = num;
	in = cmapValues_[2];
	while( --count >= 0)
		*out++ = *in++;
  }

  if((out = alpha) != NULL)
  {	count = num;
	in = cmapValues_[3];
	while( --count >= 0)
		*out++ = *in++;
  }

}

// Get values from storage taking the model type into consideration.
void AvColormap::getColors(const COLOR_MODEL model,
			   float *w, float *x, float *y, float *z,
			   const int Start, const int Num)
{
int	start, num;

  checkStartNum(Start, Num, start, num);

  getValues( w, x, y, z, start, num);

  if((model_ == RGB) && (model == HSV))
	rgbTohsv(num, w, x, y, w, x, y);
  else
  if((model_ == HSV) && (model == RGB))
	hsvTorgb(num, w, x, y, w, x, y);
}

// Load values into storage.
void AvColormap::putValues(float * red, float * green, float * blue,
			   float *alpha, const int Start, const int Num)
{
int	start, num;
register float *in, *out;
register int count;
Boolean modified = FALSE;

  checkStartNum(Start, Num, start, num);

  if(num <= 0)
	return;

  if((in = red) != NULL)
  {	count = num;
	out = cmapValues_[0];
	while( --count >= 0)
		*out++ = *in++;
	modified = TRUE;
  }

  if((in = blue) != NULL)
  {	count = num;
	out = cmapValues_[1];
	while( --count >= 0)
		*out++ = *in++;
	modified = TRUE;
  }

  if((in = green) != NULL)
  {	count = num;
	out = cmapValues_[2];
	while( --count >= 0)
		*out++ = *in++;
	modified = TRUE;
  }

  if((in = alpha) != NULL)
  {	count = num;
	out = cmapValues_[3];
	while( --count >= 0)
		*out++ = *in++;
	modified = TRUE;
  }

  if(modified)
	  invokeCallbacks(COMPONENT_CHANGED);
}

// Load values into storage.
void AvColormap::putColors(const COLOR_MODEL model,
			float *w, float *x, float *y, float *z,
			const int Start, const int Num)
{
int	start, num;
float	*a, *b, *c;

  checkStartNum(Start, Num, start, num);

  putValues(w, x, y, z, start, num);

  a = cmapValues_[0];
  b = cmapValues_[1];
  c = cmapValues_[2];

  if((model_ == RGB) && (model == HSV))
	rgbTohsv(num, a, b, c, a, b, c);
  else
  if((model_ == HSV) && (model == RGB))
	hsvTorgb(num, a, b, c, a, b, c);
}

void AvColormap::queryValues( float *w, float *x, float *y,
					  const int start, const int num)
{
	getValues(w, x, y, NULL, start, num);
}

void AvColormap::storeValues( float *w, float *x, float *y,
				  const int start, const int num)
{
	putValues(w, x, y, NULL, start, num);
}

#ifdef OLDCOLORMAP
void AvColormap::installColormap(float * red, float * green, float * blue)
{
	cerr	<< "AvColormap::installColormap: please use putValues!"
		<< endl;
	putValues(red, green, blue, NULL);
}
#endif

int AvColormap::numColors ()const {return numColors_;}
int AvColormap::maxColors ()const {return maxColors_;}

// Change colormap name. Ignore if new name is NULL.
void AvColormap::setName(const char *n)
{
int	len;

	if(n == NULL)
		return;
	if((name_ != NULL) && (strcmp(n, name_) == 0))
		return;
	len = (int)strlen(n) + 1;
	if(namelen_ < len)
	{	delete [] name_;
		name_ = new char[len];
		namelen_ = len;
	}
	strcpy(name_, n);
	invokeCallbacks(NAME_CHANGED);
}

char *AvColormap::name()const
{
	return name_;
}

//		Standalone utility functions.

void AvColormap::hsvTorgb(const float H, const float S, const float V,
			  float &red, float &green, float &blue) 
// assumes h, s, and v to be in range [0, 1]. Returns r, g, b in range [0, 1].
{
  float f, a, b, c;
  float h, s, v;
  int i;

  h = H; s = S; v = V;

  if (s == 0) {
    /* must be grayscale */
    red = v;
    green = v;
    blue = v;
  }
  else {
    if (h == 1.0)
      h = 0.0;
    
    h = h * 6.0;
    i = int(floor (h));
    f = h - i;

    a = v * (1 - s);
    b = v * (1 - (s * f));
    c = v * (1 - (s * (1 - f)));

    switch (i) {
    case 0:
      red = v;      green = c;      blue = a;      break;
    case 1:
      red = b;      green = v;      blue = a;      break;
    case 2:
      red = a;      green = v;      blue = c;      break;
    case 3:
      red = a;      green = b;      blue = v;      break;
    case 4:
      red = c;      green = a;      blue = v;      break;
    case 5:
      red = v;      green = a;      blue = b;      break;
    }
  }
}

void AvColormap::hsvTorgb(const float H, const float S, const float V,
			  float *red, float *green, float *blue)
{
	if((red != NULL) && (green != NULL) && (blue != NULL))
		hsvTorgb(H, S, V, *red, *green, *blue);
}

void AvColormap::hsvTorgb(const int len, float *h, float *s, float *v,
			  float *red, float *green, float *blue)
{
int n = len;

	while(--n >= 0)
		hsvTorgb(h[n], s[n], v[n], red[n], green[n], blue[n]);
}

void AvColormap::rgbTohsv
(const float r, const float g, const float b, float &h, float &s, float &v)
//Assumes r, g, and b to be in range [0, 1]. Returns h, s, v in range [0, 1].
{
  float min, max, delta;

  max = MAX(r,MAX(g,b));
  min = MIN(r,MIN(g,b));
  delta = max - min;

  v = max;
  if (max != 0.0) s = delta / max;
  else s = 0.0;

  if (s == 0.0) h = NO_HUE;
  else {
    if (r == max) h = (g - b) / delta;
    else
      if (g == max) h = 2 + (b - r) / delta;
    else
      if (b == max) h = 4 + (r - g) / delta;
    h = h * 60.0;
    if (h < 0) h = h + 360.0;
    h = h / 360.0;
  }
}

void AvColormap::rgbTohsv
(float r, float g, float b, float *h, float *s, float *v)
{
	if((h != NULL) && (s != NULL) && (v != NULL))
		rgbTohsv(r, g, b, *h, *s, *v);
}


void AvColormap::rgbTohsv(const int len,
		float *r, float *g, float *b, float *h, float *s, float *v)
{
int	n = len;

	while( --n >= 0)
		rgbTohsv( r[n], g[n], b[n], h[n], s[n], v[n]);
}

