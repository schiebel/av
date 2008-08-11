//# Copyright (C) 1995-98 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvCLUImageSetter.h,v 19.0 2003/07/16 05:48:01 aips2adm Exp $
//
// $Log: AvCLUImageSetter.h,v $
// Revision 19.0  2003/07/16 05:48:01  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:36  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:43:04  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:10  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:38  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:09:07  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:40:58  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:19  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:02:10  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:56:24  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.3  1998/06/09 21:13:08  hr
// Removed unneeded semicolon from DECLARE_HEADER.
//
// Revision 9.2  1998/02/20 21:58:10  hr
// Added support for vector overlays.
//
// Revision 9.1  1997/11/17 16:52:05  hr
// Added axisOptions() and made axisSetup() & wedgeSetup() const.
//
// Revision 9.0  1997/08/25 21:31:38  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.1  1997/04/15 20:48:46  hr
// Added "Show Wedge" menu option.
//
// Revision 8.0  1997/02/20 03:14:49  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.5  1997/01/28 19:42:55  hr
// *** empty log message ***
//
// Revision 7.4  1997/01/24 20:12:48  hr
// Added variables for axis/wedge setup.
//
// Revision 7.3  1996/12/12 10:00:02  droberts
// Final update from monet archive.
//
// Revision 1.5  1996/11/11 15:51:02  hr
// Added getRasterScale() to retrieve the x/y scale factors.
//
// Revision 1.4  1996/09/27 16:38:59  hr
// Added a pointer to the imported accessor in order to keep track of
// whether AvCLUImageSetter had already disconnected from it.
//
// Revision 1.3  1996/09/18 18:50:17  hr
// More integration with AvAccessor.
//
// Revision 1.2  1996/09/10 16:51:26  hr
// Initial changes for integration with AvAccessor & AvDataSet.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.15  1996/06/18  18:42:39  pixton
//  Copyright Update
//
//  Revision 1.14  1996/04/01  22:45:12  hr
//  Added getOverlays().
//
//  Revision 1.13  1996/03/07  21:14:32  hr
//  Added getPrintName().
//
//  Revision 1.12  1996/02/23  17:35:51  hr
//  Removed some unused functions and changed from "offsets" to "margins".
//
//  Revision 1.11  1996/02/08  18:37:43  hr
//  replaced touch by touchRasters().
//  Removed enableModify() and modifyEnabled().
//  Changed showAxis() to take a second argument.
//  Added getShowMenuInfo() and getShowMenuLength().
//  Removed setRasterOffsets().
//  Added charScale() and computeOffsetsAndCharSize().
//
//  Revision 1.10  1996/02/02  15:32:18  hr
//  Comments. Added showRaster(), {build,process}{Show,Setup}Menu().
//  Removed a couple of unused variables.
//  Removed {build,process}{Contour,Axis}Menu().
//
//  Revision 1.9  1996/02/01  17:47:32  hr
//  Changed setScaledSize to conditionally add in offsets to size.
//  Added vpi_.
//
//  Revision 1.8  1996/01/31  18:00:20  hr
//  Renamed several functions as part of Axis installation.
//
//  Revision 1.7  1996/01/24  18:19:18  hr
//  Added support for PGPLOT/Contouring.
//
//  Revision 1.6  1996/01/08  22:14:40  hr
//  Beginning of changes to support contours.
//
//  Revision 1.5  1995/09/20  19:49:04  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/09/20  02:00:27  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/04/26  19:44:49  hr
//  Calling setRasterScale doesn't 'touch' the rasters.
//
//  Revision 1.3  1995/04/24  18:44:26  hr
//  Moved much of the drawing work to CLUIRaster from CLUImageSetter
//  to support beginAccess and endAccess.
//
//  Revision 1.2  1995/04/17  14:57:33  hr
//  Added enableModify().
//
//  Revision 1.1  1995/04/14  19:42:41  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* Builds and shows the main display list shown by aipsview including
raster image, contours and axis. Also supplies and handles menus and
creates the contour user interface.
*/

#ifndef _SV_CLU_IMAGESETTER_H
#define _SV_CLU_IMAGESETTER_H

#include "AvCLUImage.h"
#include "AvViewportSetter.h"

class	AvMenu;
struct	AvMenuButtonInfo;

// Overlay support depends on both PGPLOT and TCL.
#if defined(PGPLOT) && defined(TCL)
#define DO_OVERLAY
#endif

#ifdef PGPLOT
class	AvContourList;
class	AvCLUIRaster;
class	AvAxis;
class	AvContour;
class	AvContourInfo;
class	AvColorWedge;
class	AvAxisConfig;
#endif
class AvAxisVPS;
class AvAxisOptions;

#if defined(DO_OVERLAY)
class	AvOverlayConfig;
class	AvOverlay;
class AvOverlayOptions;
#endif

class AvCLUImageSetter : public AvViewportSetter {

  public:

	AvCLUImageSetter(AvCLUImage *, AvImageViewItem * = NULL);

	float min();	// Clipped data min/max.
	float max();
	AvAccessor *accessor()const { return accessor_;}
	// Set scale factor for rasters.
	virtual void setRasterScale( const float xscl, const float yscl);
	virtual void setRasterScale( const float scl);
	virtual void getRasterScale( float &xscl, float &yscl);

	///////////////////////////////////////////////////////////////////
	//		Turn on/off display of different things.
	// Show/no show contour level entry window.
	void toggleContourLevelDisplay();
	// Show/no show Axis setup window.
	void toggleAxisSetupDisplay();
	// Show/no show Wedge setup window.
	void toggleWedgeSetupDisplay();
	// Show/no show Overlay setup window.
	void toggleOverlaySetupDisplay();
	// Show/No Show This contour.
	void showContours(const Boolean show);
	Boolean showContours()const {return showContours_;}
	// Show/No Show contour levels from other windows.
	void showOtherContours(const Boolean show);
	Boolean showOtherContours()const {return showIncludedContours_;}
	void showAxis(const Boolean show, const Boolean resize=TRUE);
	Boolean showAxis()const {return showAxis_;}
	void showRaster(const Boolean show);
	Boolean showRaster()const {return showRaster_;}
	// The wedge is shown IFF showRaster & showAxis & showWedge are true.
	void showWedge(const Boolean show);
	Boolean showWedge()const;
	void showOverlay(const Boolean show);
	Boolean showOverlay()const {return showOverlay_;}

	///////////////////////////////////////////////////////////////////
	//		Menu support routines
	// Append menu items presented by AvImageWindow.
	static void buildShowMenu(AvMenu *, const int tag);
	static void buildSetupMenu(AvMenu *, const int tag);
	static AvMenuButtonInfo *getShowMenuInfo();
	static int getShowMenuLength();
	// Process the menu request. Returns TRUE if "item" was handled.
	Boolean processShowMenu(char *item, Widget);
	Boolean processSetupMenu(char *item, Widget);
	///////////////////////////////////////////////////////////////////
	// Get margins around the image.
	void getMargins(int &lm, int &rm, int &tm, int &bm)const;
	// Get the total size of the margins.
	void getMargins(int &width, int &height)const;
	// Set the margins
	void setMargins(const int lm, const int rm,
			const int tm, const int bm);
	// Functions to retrieve display objects.
	AvAxisVPS *getAxisVPS(const int index)const;
	AvViewportSetter *getRasterVPS(const int index=-1)const;
	AvCLUIRaster *getRaster(const int index=-1)const;
	// Appends overlay objects to vps IFF showOtherContours() is TRUE.
	void getOverlays(const int noverlays, AvImageViewItem **views,
						AvViewportSetter *vps);
	// As above, but it generates its own include list.
	void getOverlays( AvViewportSetter *vps);

#ifdef PGPLOT
	AvAxis *getAxis(const int index=-1)const;
	// -1 means return currently shown object.
	AvContour *getContour(const int index=-1)const;
	// Pointer to the ItemList used to enter contour levels.
	AvContourList *contourLevels()const{return contourlevels_;}
	// Pointer to 
	AvAxisConfig *axisSetup()const{return axisSetup_;}
	AvAxisConfig *wedgeSetup()const{return wedgeSetup_;}
	AvAxisOptions *axisOptions()const{return axisOptions_;}
#endif
#if defined(DO_OVERLAY)
	AvOverlayOptions *overlayOptions()const{return overlayOptions_;}
	AvOverlayConfig *overlaySetup()const{return overlaySetup_;}
#endif
	// Return a pointer to the name to be used for printing. Use
	// delete to free.
	char *getPrintName();
	AvImageViewItem	*imageViewItem() {return imageViewItem_;}
	virtual void touchRasters();	// Mark rasters as modified.
	AvCLUImage *CLUImage() {return CLUImage_;}

	// The Canvas calls draw with a pointer to itself to tell the object
	// to display itself.
	virtual void draw(AvCanvas &);
	virtual void draw(AvCanvas *);
  protected:
	virtual ~AvCLUImageSetter ();
  private:
	// Setup to show a different slice.
	void setSlice(const int sliceIndex);

	// Build raster compound object.
	AvViewportSetter *buildRasterObject(const int sliceIndex);
	void redisplay();
	// Reset the offsets of the raster viewports.
	void setVPOffsets();
	// Set the size of a vp to the raster's width*scale + offset.
	// This is used to set the size of this and the "axis" viewports.
	// If useoffsets if FALSE, the offsets aren't added in.
	void setScaledSize( AvViewportSetter *vp, const Boolean offsets=True);
	// Update the size of this and the axis viewports.
	void setScaledSizes();
#ifdef PGPLOT
	// Called by info_ when something changes. Does a redisplay.
	static void infoChangedCB(XtPointer o, AvConductor *, XtPointer uData);
	// Called by contourList
	static void contourlistCB(AvContourList *sel, XtPointer,
							const int reason);
	// called by contourlistCB to do the actual work.
	void handleContourList(AvContourList *sel, const int reason);
	// Routines to return pointers to various children that need tweaking.
	AvGraphics2D *getChild(	const int childID,
				const AvViewportSetter *vps=NULL)const;

#endif
	// Draw contours included from other windows.
	void drawIncludedContours(AvCanvas &);
	// Draw the overlay object.
	void drawOverlay(AvCanvas &);
	// Callback to receive accessor notices.
	static void accessorCB_(XtPointer me, AvConductor *c, XtPointer uData);
	// Callback to actually handle accessor notices.
	void accessorCB(AvConductor *c, XtPointer uData);
	// Disconnect from accessor;
	void disconnect();
  private:
	DECLARE_HEADER
	AvCLUImage	*CLUImage_;
	AvAccessor	*accessor_;
#ifdef PGPLOT
	AvContourList	*contourlevels_;	// User interface for contours.
	AvContourInfo	*contourinfo_;		// Holds contour info.
	AvViewportSetter *vpi_;			// Holds "included" contours.
	AvAxisVPS	*wedgeVPS_;		// Holds vps for wedge.
	AvColorWedge	*wedge_;		// Color wedge.
	AvAxisConfig	*axisSetup_;		// Axis setup window.
	AvAxisConfig	*wedgeSetup_;		// Wedge setup window.
#endif
#if defined(DO_OVERLAY)
	AvOverlayConfig	*overlaySetup_;		// Overlay setup window.
	AvOverlay	*overlay_;
	AvOverlayOptions *overlayOptions_;
#endif
	AvViewportSetter *ovps_;		// VPS for overlay object.
	AvAxisOptions	*axisOptions_;
	AvAxisOptions	*wedgeOptions_;

	AvImageViewItem	*imageViewItem_;
	Boolean	showContours_;
	Boolean showIncludedContours_;
	Boolean	showAxis_;
	Boolean	showRaster_;
	Boolean	showOverlay_;
	Boolean showWedge_;	// Also depends on showAxis & showRaster.
	int	lm_, rm_, tm_, bm_;
};

#endif
