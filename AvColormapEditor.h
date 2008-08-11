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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvColormapEditor.h,v 19.0 2003/07/16 05:46:49 aips2adm Exp $
//
// $Log: AvColormapEditor.h,v $
// Revision 19.0  2003/07/16 05:46:49  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:35  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/14 16:52:25  hravlin
// Changed fallbacks arg to be const char *[].
//
// Revision 17.0  2001/11/12 19:42:03  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:12  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:56  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 18:12:28  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:07:51  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.3  1999/09/28 19:11:46  hravlin
// Added busy_ variable.
//
// Revision 13.2  1999/09/15 21:38:27  hravlin
// Added routines for support of colormap edit notification.
//
// Revision 13.1  1999/08/25 19:56:31  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:39:35  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:55  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:44  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:24  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:25:22  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:28  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1996/12/12 06:01:23  droberts
// Final update from monet archive.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.9  1996/06/18  18:42:43  pixton
//  Copyright Update
//
//  Revision 1.8  1996/03/14  18:14:51  baker
//  Split the X color functionality out of AvApp into AvXColorApp.
//  Updated editor to derive from AvXColorApp rather then AvApp.
//
//  Revision 1.7  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.7  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.7  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.6  1995/08/11  20:47:35  baker
//  Added constructor to take fallback resources.
//
//  Revision 1.5  1995/08/03  16:40:02  hr
//  Changes to allow multiple user named palettes and to notify callbacks
//  when changes are made.
//
//  Revision 1.4  1995/07/17  19:56:12  hr
//  Added 'virtual' to destructor of virtual classes.
//
//  Revision 1.3  1995/05/03  22:03:54  pixton
//  fixed some problems with the colormap editor initialization.
//
//  Revision 1.2  1995/04/17  17:39:25  pixton
//  colormap now initialized to the "Rainbow" palette.
//
//  Revision 1.1  1995/03/16  21:42:12  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

#ifndef _SV_COLORMAP_EDITOR
#define _SV_COLORMAP_EDITOR

#ifndef MAX
#define MAX(x,y) (x>y?x:y)
#endif
#ifndef MIN
#define MIN(x,y) (x<y?x:y)
#endif

#include <math.h>
#include <limits.h>
#include <Av.h>

#include <AvColormap.h>

class AvCECmap : public AvColormap {
public:
  AvCECmap(const int maxlen, const int len,
	   const char *name, Widget menuitem);
  ~AvCECmap();
  virtual void setName(const char *newName);
  Widget menuItemWidget()const {return mi_;}
  float pan()const {return pan_;}
  void pan(const float p) {pan_ = p;}
  float zoom()const {return zoom_;}
  void zoom(const float z) {zoom_ = z;}
  void grayScale(const int gray) {grayScale_ = gray;}
  int grayScale()const {return grayScale_;}
private:
  Widget	mi_;
  float	pan_;
  float	zoom_;
  int	grayScale_;
};


#include <AvFunctionEditor.h>
#include <AvPalette.h>

#include <AvConductor.h>
#include <AvXColorApp.h>

class AvColormapEditor : public AvXColorApp, public AvConductor {

public:

  enum {
    FILE_MENU = 2000,
    OPTIONS_MENU,
    PALETTES_MENU };

  // Used to notify of a new colormap.
  // NEW_COLORMAP -> a new map was created.
  // MAP_CHANGED  -> a new map has become 'current'.
  // MAP_EDITED	  -> Colormap has been editied.
  enum { COLORMAP_DESTROYED = USER0, NEW_COLORMAP = USER1,
	 MAP_CHANGED=USER2, MAP_MODIFIED, MAP_EDITED};

  // Constructor if standalone.
  AvColormapEditor (char * appClass, int & argc, String * argv);

  AvColormapEditor
    (char * appClass, int & argc, String * argv, const char * fallbacks[]);

  // Constructor if part of larger program.
  AvColormapEditor (Widget parent, int buildInsideParent);

  virtual ~AvColormapEditor ();

  // intercept show and do post-show initialization
  void show();

  virtual const char * nameOfClass () { return ("AvColormapEditor"); }
  virtual int updateObj ();

  // Readable Convenience Functions
  int nGraphs()              const { return (3 + alpha_);       }

  // Returns the number of colors CME is working with.  This is set
  // from Harold's palette manager stuff.
  int nColors()              const 
    { return (int)functionEditors_[0]->func()->length(); }
  
  // Graph #4 (alpha graph) on or off (RW)
  int alpha() const     { return alpha_; }
  void alpha(int onOff) { alpha_ = onOff; }

  // Get/Set the active graph (Normally done by callback from EH's)
  void active(int which) { active_ = CLAMP(which, 0, 2+alpha_); }
  int  active() const { return active_; }
  
  void redraw()  { }
  void redrawGraphs(const Boolean savecm=TRUE)
		      { functionEditors_[0]->redraw();
			functionEditors_[1]->redraw();
			functionEditors_[2]->redraw();
			functionEditors_[3]->redraw(); 
			updateMap(savecm);
			drawMapRuler();
		      }
  void refresh(const Boolean savecm=TRUE)
		 { functionEditors_[active_]->redraw(); 
		   updateMap(savecm); 
		   drawMapRuler();
		 }
  void buildMenus (int standAloneApp);          // Construct the ui menu
  void initMap ();                              // initialize map for app
  Boolean loadPalette(char * paletteName);	// Returns TRUE if valid name.

  void grayScaleMode();
  void rgbMode();
  void hsvMode();
  void alphaMode(Widget menuItem);

  AvCECmap	*currentCmap();	// Current colormap.
  // Return current color model.
  AvColormap::COLOR_MODEL colorModel()const;
  // Return pointer to the last colormap that was created/destroyed.
  AvCECmap	*getLastColormap() { return getColormap(-1);}
  /* Return a pointer to a colormap.
	-1		Last created/destroyed colormap.
	0..MAXCMAPS-1	Pointer to colormap index (may be NULL if none).
	>= MAXCMAPS	NULL.
  */
  AvCECmap	*getColormap(const int index=-1);
  // Return a pointer to a list of colormaps and the number of maps in the
  // list. Use delete [] to free the list.
  AvCECmap	**getColormapList(int &numMaps);

  int mapWidth();
  int mapHeight();

  void relayFunctionData(int source);
  void relayModeChange(int source);
  void relayRepeatChange(int source);

  int hsv() const { return hsv_; }
  int greyScale() const { return greyScale_; }

  void centerMap();

  // Set number of colors to use to n. If n<= 0, query the palette.
  void resetColors (const int n=0);

  // When dealing with readonly colormaps, if autoUpdate_ is True, data
  // windows will be notified whenever the colormap changes. If False,
  // they will only be notified on button release events.
  Boolean autoUpdate()const {return autoUpdate_;}
  Boolean readonlyColormap()const{return palette_->readonly();}
protected:

  virtual void processMenu (char *, int, Widget);

  // read and write map to disk
  static void openMap (AvColormapEditor * app, XtPointer uData);
  static void saveMap (AvColormapEditor * app, XtPointer uData);

  static void mapExposedCB
    (Widget widget, AvColormapEditor *obj, XmDrawingAreaCallbackStruct *cbs);

  static void graphMgrEnterEH  // Event Handler
    (Widget widget, AvColormapEditor *obj, XEvent *, Boolean *);

  static void graph0ChangedCB(AvColormapEditor * ce);
  static void graph1ChangedCB(AvColormapEditor * ce);
  static void graph2ChangedCB(AvColormapEditor * ce);
  static void graph3ChangedCB(AvColormapEditor * ce);

  static void graph0ModeChangedCB(AvColormapEditor * ce);
  static void graph1ModeChangedCB(AvColormapEditor * ce);
  static void graph2ModeChangedCB(AvColormapEditor * ce);

  static void graph0RepeatChangedCB(AvColormapEditor * ce);
  static void graph1RepeatChangedCB(AvColormapEditor * ce);
  static void graph2RepeatChangedCB(AvColormapEditor * ce);

  static void buttonReleasedCB(AvColormapEditor * ce);
  void changeDone();

  char * getColorMapName (XtPointer);

  void openColorMap (char *);

  void saveColorMap (char *);

  void initializeMap ();

  void updateMap (const Boolean saveCM=TRUE);

  void setCursor (int ndx);

  void drawMap ();

  void init (int standAloneApp);

  Window queryPointer(Widget w, int * x, int * y);

  int queryMode(int ndx);  // return enum type above

  static void mapEnterEH
    (Widget widget, AvColormapEditor *ce, XEvent *, Boolean *);

  static void mapLeaveEH
    (Widget widget, AvColormapEditor *ce, XEvent *, Boolean *);

  static void mapInputCB(Widget w, AvColormapEditor * ce,
			 XmDrawingAreaCallbackStruct * cb);
  void mapPress(Widget w);
  void mapMotion(Widget w);
  void panZoomMaps(float * r, float * g, float * b, float * a);

  static void mapRulerExposedCB
    (Widget widget, AvColormapEditor *obj, XmDrawingAreaCallbackStruct *cbs);

  void drawMapRuler();

  int		newCmap(const char *name=NULL);	// create new colormap.
  void deleteCurrentCmap();		// Remove current cmap.
  void useCmap(const int cm);		// Make this current.
  int findNamedCmap(const char *name);	// Look for map with this name.
  void setCurrentName(char *newname=NULL); // Set name of current cmap.
  static void changeNameCB(Widget widget, AvColormapEditor *obj, XtPointer);
  char *getNameField(Widget widget=0);
  void setNameField(char *name, Widget widget=0);
  // Copy current info to map or current if NULL.
  void saveColormap(AvCECmap *map=NULL);
  // Copy info from map or current colormap if NULL.
  void restoreColormap(AvCECmap *map=NULL, const Boolean copyname=TRUE);
  void storeColors(const float *r, const float *g, const float *b,
		   const int start, const int num);

private:

  AvPalette *palette_;
  char paletteDirectory_[256];

  static const char *rgbGraphNames[3];
  static const char *hsvGraphNames[3];

  void makeUserModeButtons (char * inLabel, Widget parent, int i);

  int mapOffset_;              // Map shift

  int active_;                 // which (0-3) graph is active
  int hsv_;                    // 1 if ramps represent HSV values
  int drawMode_[4];            // 1 if the map (0-3) is in freehand mode
  int greyScale_;              // 1 if RGB ramp updates should be tied
  int alpha_;                  // 1 if alpha ramp is displayed/active
  XColor yellow_, black_, RGBA_[4];  // X colors for graph
  XColor graphBackground_;
  int lastPointerX_;           // last recorded X pointer position (Arm CB)
  int lastPointerY_;           // last recorded Y pointer position ("")
  int grabbed_;                // = point ndx, else -1 if no grab active
                               // set during arm, used during motion CB
  Widget saveSelector;         // Dialog Box for saving colormaps

  Widget panel_;
  Widget mapMgr_;
  Widget map_;
  Widget mapRuler_;

  float  mapPan_;              // point of focus for cycling
  float  mapZoom_;             // magnification applied to original funcs

  Widget graphMgr_[4];         // Graph manager for each of the fe's
  AvFunctionEditor * functionEditors_[4];  // fe for each graph

  Cursor cursor_;              // X FontCursor for this display
  int    cursorShape_;         // XC_????? value for shape type

  XmFontList fontlist_;        // FontList for widgets

  GC gc_;                      // Rendering context for graphs
  GC xorGc_;                   // Rendering context for control points
  
  Pixel bg, fg;

  char *helpMessage, *modeMessage;

  Pixmap mapPixmap, buttonsPixmap, RGBpixmaps[3];

  AvCECmap	**colormaps_;	// List of colormaps.
  AvMenu	*paletteMenu;
  int		currentCmap_;
  int		lastCmap_;	// Index of last colormap created or destroyed.
  Widget	colormapNameW_;	// Widget holding name of current colormap.

  // Some functions can (indirectly) result in many calls to update which
  // will result in extraneous calls to the server and the callbacks.
  // If this is TRUE, update will make an immediate return.
  // It will be necessary to explicitly call redrawGraphs after resetting this
  // to FALSE.
  Boolean	ignoreUpdateRequest_;
  Boolean	autoUpdate_;
  Boolean	busy_;
};

#endif
