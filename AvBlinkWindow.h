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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvBlinkWindow.h,v 19.0 2003/07/16 05:46:33 aips2adm Exp $
//
// $Log: AvBlinkWindow.h,v $
// Revision 19.0  2003/07/16 05:46:33  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:20  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:41:48  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:41:58  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:07:56  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 18:16:35  hravlin
// Changed className() to return const char *.
//
// Revision 14.0  2000/03/23 16:07:33  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:13  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:21  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:57:55  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:51:25  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:23:49  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:14:35  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 05:43:21  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.7  1996/06/18  18:42:37  pixton
//  Copyright Update
//
//  Revision 1.6  1996/04/02  15:15:27  hr
//  Added initialWindowSize_, removed unused code.
//
//  Revision 1.5  1996/04/01  22:45:51  hr
//  AvBlinkWindow.h
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
//  Revision 1.3  1995/07/17  19:56:12  hr
//  Added 'virtual' to destructor of virtual classes.
//
//  Revision 1.2  1995/04/24  19:02:49  hr
//  Added 'int' to a parameter declaration.
//
//  Revision 1.1  1995/04/24  18:51:38  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------


#ifndef _SV_BLINK_WINDOW_H
#define _SV_BLINK_WINDOW_H

/*

AvBlinkWindow
 Stripped down clone of AvImageWindow. (It has things that could cause
trouble here).

*/

class AvXWindow;
class AvViewportSetter;
class AvGraphics2D;
class AvImageViewItem;

#include <AvDisplayWindow.h>


class AvBlinkWindow : public AvDisplayWindow {

  public:

  AvBlinkWindow(Widget parent, int buildInsideParent);
  
  virtual const char * className() const;

  virtual const char * getClassLabel() const { return "Blink Window"; }
  virtual const char * classLabel() const { return "Blink Window"; }
  virtual void show();
  virtual void hide();

  void showImage();
  void updateList();	// Builds its own list.
  // Use this list.
  void updateList(AvImageViewItem **, const int count);
  int getNumObjects()const;
  void showItem(const int index);

  Widget workArea() { return (workArea_); }
  Widget drawArea() { return (drawArea_); }

	AvXWindow * canvas() { return canvas_; }
	enum CBREASON { DESTROYED=1, CLOSED};
	typedef void(*AvBlinkWindowCallback)
			(AvBlinkWindow*, XtPointer cbdata, CBREASON reason);
	// Add/remove callback.
	void addCallback(AvBlinkWindowCallback, XtPointer cbdata);
	void removeCallback(AvBlinkWindowCallback, XtPointer cbdata);

	virtual ~AvBlinkWindow();
	void fitImageToWindow(const Boolean redraw=FALSE);
	void fitWindowToImage(const Boolean redraw=FALSE);
	void fitObjectToWindow();
	void getImageSize(float &width, float &height, AvGraphics2D * =NULL);
  protected:

	virtual void processMenu(char * buttonName, int choice, Widget);
	void scaleObject(AvViewportSetter *vp);

  // When making an instance of this class, should we make the menus or not?
  // It is anticipated that classes derived from this will want to make 
  // their own menus.
  void init(int makeMenus = TRUE);

  // scale the contents of the window
  void scale(float sf);

  protected:
	void	callCallback(const CBREASON);	// Calls the callback(s).
  private:

  void displayTitle();
  AvMenu * scaleMenu;

  AvXWindow * canvas_;
  AvViewportSetter *list_;

  // the work area widget created by the canvas.
  Widget drawArea_;

  // the factor by which to scale the contents of this window
  float scaleFactor_;
  float initialWindowSize_;	// Initial window size (usually 256).

  static void resizeCB(Widget w, AvBlinkWindow * obj, XEvent * event);
  static void destroyCB(Widget w, AvBlinkWindow * obj, XtPointer);
  
  AvBlinkWindowCallback	callback_;
  XtPointer		cbdata_;

enum {
  SV_OPTION = 200,
  SV_SCALE
};

};

#endif
