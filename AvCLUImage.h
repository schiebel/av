//# Copyright (C) 1995-2000 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvCLUImage.h,v 19.0 2003/07/16 05:46:35 aips2adm Exp $
//
// $Log: AvCLUImage.h,v $
// Revision 19.0  2003/07/16 05:46:35  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:22  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:41:51  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:01  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:07  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 18:15:39  hravlin
// Changed className() to return const char *.
//
// Revision 14.0  2000/03/23 16:07:36  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:18  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:27  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:03  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:51:36  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:24:06  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:14:43  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 09:00:56  droberts
// Final update from monet archive.
//
// Revision 1.4  1996/11/11 15:49:35  hr
// Replaced scale() by setScale() and added getScale() during conversion
// to two scale factors.
//
// Revision 1.3  1996/09/18 18:49:13  hr
// Removed some old commented out code and more integration with AvAccessor.
//
// Revision 1.2  1996/09/10 16:50:51  hr
// Initial changes for integration with AvAccessor & AvDataSet.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.8  1996/06/18  18:42:38  pixton
//  Copyright Update
//
//  Revision 1.7  1996/02/23  17:32:12  hr
//  Added routines to access the imagesetter w/o having to include its '.h' file.
//
//  Revision 1.6  1996/01/08  22:08:12  hr
//  Pass pointer to AvImageViewItem to AvCLUImageSetter.
//
//  Revision 1.5  1996/01/08  20:09:24  hr
//  Added imageSetter() function.
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
//  Revision 1.3  1995/05/05  20:47:28  hr
//  Added dataModified.
//
//  Revision 1.2  1995/04/14  19:40:03  hr
//  Removed some unused 'friends'.
//  Changed call to setSlice.
//  Added functions used by AvCLUImageSetter
//  Removed references to AvGDRaster, AvViewPortSetter and replaced with
//  an AvCLUImageSetter object.
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#ifndef _SV_CLU_IMAGE_H
#define _SV_CLU_IMAGE_H

class AvColormap;
class AvDisplayWin2D;
class AvCLUImageSetter;
class AvImageViewItem;
class AvAccessor;

#include <AvVisRep2D.h>

class AvCLUImage : public AvVisRep2D{

public:

  AvCLUImage (AvImageViewItem * = NULL);
  virtual ~AvCLUImage ();

  virtual const char * className () const;

  virtual const char * getClassLabel () const { return ("Raster Image"); } 
  
  int width ();
  int height ();
  float min();	// Data min/max.
  float max();

  static int nObjects;

  void setColormap (AvColormap * c);

  void setColor (AvAccessor *data);

  int sliceAxis;

  virtual void setDisplayWindow (AvDisplayWin2D * w);

//  void setSlice (int x, int y, int view, int sliceIndex, AvAccessor *acc);
  void setSlice (AvAccessor *acc);

  virtual void map ();

  // This has to exist to override version in AvVisRep2D.
//  virtual float scale (float);
  virtual void setScale (const float, const float);
  virtual void getScale (float &, float &)const;

  AvGraphics2D *graphics2d();
  AvCLUImageSetter * imageSetter();
  AvAccessor *accessor()const{return accessor_;}
  AvColormap *getColormap(){return colormap;}

  // This function should be called when the contents of the AvGriddedData
  // data has changed.
  void dataModified();
  // Some interfaces to AvCLUImageSetter so its include file isn't needed
  // everywhere.
  // Get margins around the image.
  void getMargins(int &lm, int &rm, int &tm, int &bm)const;
  // Get the total size of the margins.
  void getMargins(int &width, int &height)const;
  Boolean showAxis()const;
  void showAxis(const Boolean show, const Boolean resize);
protected:

  // *** maybe make this a list?
  AvDisplayWin2D * dispWin;

  AvColormap * colormap;

private:
  AvAccessor *accessor_;
  AvCLUImageSetter	*is_;
};

#endif
