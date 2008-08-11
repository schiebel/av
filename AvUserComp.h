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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvUserComp.h,v 19.0 2003/07/16 05:46:47 aips2adm Exp $
//
// $Log: AvUserComp.h,v $
// Revision 19.0  2003/07/16 05:46:47  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:33  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:02  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:11  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:48  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:27:04  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:07:49  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/10/04 15:04:32  hravlin
// Added the isHidden_ variable.
//
// Revision 13.0  1999/08/10 18:39:33  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:51  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:38  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:17  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:25:11  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:22  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 07:37:22  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.11  1996/06/18  18:43:26  pixton
//  Copyright Update
//
//  Revision 1.10  1996/03/29  15:10:53  baker
//  Added 'isReturnKey' and 'getTextField' methods.
//
//  Revision 1.9  1996/03/14  19:52:05  baker
//  Documentation.
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
//  Revision 1.7  1995/08/28  14:40:00  baker
//  Extended routines for making pixmaps from .icon files.
//
//  Revision 1.6  1995/08/11  15:23:33  baker
//  Added convenience functions for making push and toggle buttons
//  using pixmaps.
//
//  Revision 1.5  1995/08/08  15:10:43  baker
//  Overloaded 'setLabelString' to accept integers.
//
//  Revision 1.4  1995/07/17  19:57:21  hr
//  Added 'virtual' to destructor of virtual classes.
//
//  Revision 1.3  1995/06/16  17:56:57  hr
//  Added builtInsideParent() function.
//
//  Revision 1.2  1995/05/19  21:53:40  baker
//  Changed method 'setLabelString' to 'static'
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#ifndef _SV_USER_COMP_H
#define _SV_USER_COMP_H


/* <g> UserInterface */

/* <s> Abstract class representing a basic Motif widget. */

/* <d> Abstract class representing a basic XMotif widget.  Instances
of derived classes can exist as topLevelShells or as widgets embedded
in other widgets or shells.  */


#include <Xm/Xm.h>
#include <Av.h>

class AvUserComp {

public:

  virtual const char * className () const;

  // For toplevel shells, sets the title.
  void setTitle (const char * newTitle);

  // Sets the size.
  void setSize (int w, int h);

  // Returns the size.
  void size (int & w, int & h);
  void getSize (int & w, int & h);

  // Returns the base widget of this component.
  virtual Widget baseWidget () =0;

  // Returns the parent widget of this component.
  Widget parentWidget () { return (parentWidget_); }

  // Makes the component visible.
  virtual void show ();

  // ... and invisible.
  virtual void hide ();
  Boolean isHidden()const{return isHidden_;}
  
  char * appClass () const { return (appClass_); }
  char * appName () const { return (appName_); }

  // Was this built inside its parent?
  inline Boolean builtInsideParent () const { return (topLevelP_ == FALSE); }

  // Convenience routines for retrieving X resources
  // *** redo this to avoid having multiple functions for this...
  char * getStringResource (const char * cName);
  int getColorResource (const char * cName);
  int getIntegerResource (const char * cName);

  // Convenience routines to set the label of an xmLabelWidget or xmLabelGadget
  static void setLabelString (Widget w, const char * str);
  //  static void setLabelString (Widget w, const char * str);
  static void setLabelString (Widget w, int v);
  static void setLabelString (Widget w, float v);

  // Convenience routine to get the label of an xmLabelWidget or Gadget.
  static void getLabelString (Widget w, char * putItHere);

  // Convenience routine to check whether key press event was <RETURN>.
  static Boolean isReturnKey (XKeyEvent * e);

  // Convenience routine to get contents of a TextField.
  static int getTextField (Widget w);
  static void getTextField (Widget w, int & dest);

protected:

  AvUserComp () { };

  // If buildInsideParent == FALSE, a new toplevel shell 
  // is made for this component. 
  AvUserComp (Widget parent, int buildInsideParent = TRUE);
  
  // The parent widget of this component.  Either the newly
  // created toplevel shell, or the widget passed in as 'parent'
  Widget parentWidget_;

  // The widget that serves as the base for the component
  Widget baseWidget_;
  
  // Is this a toplevel shell or built within a parent?
  int topLevelP_;

  // The application class under which this widget exists
  // i.e., the 'applicationClass' parameter used in XtAppCreateShell
  static char appClass_[32];

  // The name of the application of which this widget is a part (argv[0])
  static char appName_[32];

  typedef struct {
    String str;
    Pixel pixel;
    int i;
  } componentResources;
      
  // Convenience routine for X allocation of a named color
  void allocateColor (char * cName, XColor & color);

  // Convenience routine to initialize an XImage from an '.icon' file.
  static void initButton
    (XImage &, int, int, char *, int = 0, int = 1, int = 3);

  // Convenience routines to return a pixmap, or push and toggle buttons 
  // constructed from pixmaps.
  static Pixmap makePixmap
    (Widget parent, int width, int height, char * bits, char * label);

  static Widget makePixmapPushButton
    (Widget parent, int width, int height, char * bits, char * label);

  static Widget makePixmapToggleButton
    (Widget parent, int width, int height, char * bits, char * label);

  virtual ~AvUserComp ();

  Boolean isHidden_;
private:
};

#endif
