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
//---------------------------------------------------------------------------
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvContourList.cc,v 19.0 2003/07/16 05:47:02 aips2adm Exp $
//
// $Log: AvContourList.cc,v $
// Revision 19.0  2003/07/16 05:47:02  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:46  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.4  2002/01/22 22:42:27  hravlin
// Changed an index to uInt.
//
// Revision 17.3  2002/01/22 21:44:37  hravlin
// Made NCOLORS & NREASONS uInt. (For 64 bit compilers).
//
// Revision 17.2  2002/01/09 18:01:09  hravlin
// Changed MAXVALUE to FLOATMAX which is defined in Av.h.
//
// Revision 17.1  2002/01/07 22:27:43  hravlin
// Removed check for __P.
//
// Revision 17.0  2001/11/12 19:42:15  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:23  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:39  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:57:41  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:08:06  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/10/01 21:56:57  hravlin
// *** empty log message ***
//
// Revision 13.0  1999/08/10 18:39:52  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:23  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:24  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:11  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.5  1998/06/09 21:26:02  hr
// Removed an unused widget variable.
//
// Revision 9.4  1998/06/09 17:22:09  hr
// Removed an unused widget variable from buildList().
//
// Revision 9.3  1998/05/04 16:48:05  hr
// buildList(): Create shell using aipsview's colormap.
//
// Revision 9.2  1997/09/17 14:43:35  hr
// In contourInfo(), new worldInfo_ objects are created using the filename.
// name_ had been changed to be the name of the widget.
// Added "Emphasized" button.
//
// Revision 9.1  1997/09/15 16:31:11  hr
// Added support for changing line width.
//
// Revision 9.0  1997/08/25 21:26:37  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:12  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.4  1997/02/06 19:53:27  hr
// Changed the window label.
//
// Revision 7.3  1997/02/05 17:35:02  hr
// The __P, g++ and math.h problem.
// Removed an unused variable.
//
// Revision 7.2  1996/12/12 09:31:09  droberts
// Final update from monet archive.
//
// Revision 1.3  1996/11/05 21:14:02  hr
// Several changes to deal with new AvConfigInfo used elsewhere.
// getListValues() changed to return an AvWPosition.
//
// Revision 1.2  1996/08/19 20:12:29  pixton
// Now runs through g++ -Wall without warnings.
//
// Revision 1.1  1996/07/11  21:31:21  pixton
// Automated Checkin
//
//  Revision 1.9  1996/06/18  18:45:57  pixton
//  Copyright Update
//
//  Revision 1.8  1996/04/17  20:31:11  hr
//  Added Apply button.
//
//  Revision 1.7  1996/04/09  19:44:20  hr
//  Removed a debugging statement
//
//  Revision 1.6  1996/04/08  20:40:32  pixton
//  Patch added to destructor to check if selbox exists before
//  calling XtRemoveCallback.  May be hiding bigger problem.
//
//  Revision 1.5  1996/03/28  22:32:41  hr
//  The destructor now removes the destroyCB before calling destroy().
//
//  Revision 1.4  1996/02/01  17:58:45  hr
//  Changed getListItem to take a position rather than an index.
//  Added addXmValueToList to insert an Xm string representing a number
//  in sorted order into the list.
//  restoreInfo (via appendValueList) causes info_'s contour list to be
//  cleared so popup() now initializes info_ after the call to restoreInfo().
//
//  Revision 1.3  1996/01/31  17:39:31  hr
//  Added valueW since setting fractionW to False didn't work.
//
//  Revision 1.2  1996/01/24  22:15:06  hr
//  Redid how color toggles are handled and added more color choices.
//
//  Revision 1.1  1996/01/24  18:04:31  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvContourList.cc

"Dialog" to allow user to configure contour information.



The buttons of the selection box are renamed to match their use here.
	OK becomes Add	Add an item from the text field to the list.
	Accept becomes Delete
			Delete the item in the text field from the list.

A Done button is added to signify that user is finished.

One would assume that the OK button would be used to signal a
return. However, it appears the widget is wired to also use this to
close the widget. Since the user is expected to be able to enter
multiple lines, another button had to be added. The "Add" button is
extraneous, since <RETURN> does this. However, it appears the button
is necessary for proper functioning, so we leave it.

Basically the same as AvListItem with some additions to handle contour
specific needs.
This version uses its own top level window.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>		// strtod, malloc

#include <Xm/Xm.h>
#include <X11/Shell.h>
#include <Xm/SelectioB.h>
#include <Xm/PushB.h>
/////////
#include <Xm/TextF.h>
#include <Xm/Frame.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>
#include <Xm/Separator.h>
#include <Xm/ToggleB.h>		// For XmToggleButtonSetState()
//////////
#include <Xm/DialogS.h>
#include <Xm/List.h>
#include <AvContourList.h>
#include <AvGUIParts.h>

#include <Av.h>
#ifndef STANDALONE
#include <AvXColorApp.h>
#endif

// Labels for the "mode" buttons.
static const char *valueNames[] = { "Contour Levels", "Fractions of Range"};

// Color information for all the color toggles.
static struct {
		const char *name;
		AvContourInfo::COLORSTYLE color;
	} colorinfo[] = {
	// The number and ordering here will result in reasonable layout.
	// More colors could be added by extending COLORSTYLE in contourInfo
	// and adding to this table.
	{"Red", AvContourInfo::RED},
	{"Green", AvContourInfo::GREEN},
	{"Blue", AvContourInfo::BLUE},
	{"Cyan", AvContourInfo::CYAN},
	{"Magenta", AvContourInfo::MAGENTA},
	{"Yellow", AvContourInfo::YELLOW},
	{"Black", AvContourInfo::BLACK},
	{"White", AvContourInfo::WHITE},
	{"Mixed", AvContourInfo::MIXED},
	};
static const uInt NCOLORS = (uInt)(sizeof(colorinfo)/sizeof(*colorinfo));

AvContourList::AvContourList(const char *fileName, Widget ref)
{
	filename_ = fileName;
	// Generic ItemList
	name_ = "Setup for ";
	name_ += fileName;
	name_ += ":";
	name_ += "contours";

	ref_ = ref;
	dialog = selbox = list = del = NULL;
	valueW = fractionW = NULL;
	multW = NULL;
	showContourW = NULL;
	showLabelW = NULL;
	dashNegativeW = NULL;
	useSolidLinesW = NULL;
	useDashedLinesW = NULL;
	drawTitleW = NULL;
	enableBlankingW = NULL;
	immediateW = NULL;

	listLabel_ = textFieldLabel_ = NULL;
	listLabel("Values");
	textFieldLabel("Enter:");
	up_ = FALSE;
	callback_ = NULL;
	cbdata_ = NULL;
	match_ = CANT_MATCH;	// Only unique items to list.
	emptyStrings_ = FALSE;	// No empty strings.
	numbersOnly_ = FALSE;
	sorting_ = APPEND;
	showHelp_ = FALSE;	// Don't show help button.
	immediateNotify_ = FALSE;
	toggling_ = FALSE;
	setNumberRange();
	colorinfo_ = new ColorInfo[NCOLORS];
	for(uInt i=0; i< NCOLORS; i++)
	{	colorinfo_[i].name = colorinfo[i].name;
		colorinfo_[i].color= colorinfo[i].color;
		colorinfo_[i].widget= NULL;
	}
	buildList();
	// Contour Levels specific
	numbersOnly(TRUE);
	sorting(INCREASING);
	textFieldLabel("Enter a Contour Value:");
	listLabel("Contour Values:");
	backupInfo_ = new AvContourInfo();
	info_ = new AvContourInfo(fileName);
	info_->addCallback(&AvContourList::infoCB, AvContourInfo::ValueChanged,
			   this, NULL);
	backupInfo_->ref();
	worldInfo_ = NULL;
	contourInfo(NULL);
}

AvContourList::~AvContourList()
{
  if (selbox)
    XtRemoveCallback(selbox, XmNdestroyCallback, &AvContourList::destroyCB,
		     (XtPointer) this);
  
  // Call the callback now rather than after things have started
  // disappearing.
  callCallback(DESTROYED);
  callback_ = NULL;
  
  if(isUp())
    popdown();
//  delete [] name_;
  delete [] listLabel_;
  delete [] textFieldLabel_;
  //	deleteStringList(saveList_, saveLength_);
  delete [] colorinfo_;
  
  info_->removeCallback(	&AvContourList::infoCB,
				AvContourInfo::ValueChanged,
				this, NULL);
  info_->unref();
  backupInfo_->unref();
  worldInfo_->unref();
  
  destroy(TRUE);
}

// Use a different contour info.
void AvContourList::contourInfo(AvContourInfo *inf)
{
	// ref this before unrefing worldInfo_ in case they are the same.
	if(inf != NULL)
		inf->ref();
	if(worldInfo_!= NULL)
		worldInfo_->unref();
	worldInfo_ = inf;
	// Have to have one.
	if(worldInfo_ == NULL)
	{	worldInfo_ = new AvContourInfo(filename_.chars());
		worldInfo_->ref();
	}
	restoreInfo(worldInfo_);	// Use these values.
	if(backupInfo_ != NULL)
		AvContourInfo::copy(*worldInfo_, *backupInfo_);
}

void	AvContourList::saveValues(AvContourInfo *info)
{
	// Save the value list.
	info->setContourLevels(getListValues());
}

// Copy current settings to info.
// Disable notification of the AvContourInfo's connections.
void	AvContourList::saveInfo(AvContourInfo *info)
{
	if((dialog == NULL) || (info == NULL))	// Nothing to save from.
		return;
	info->disableNotify();

		info->drawContours(getToggle(showContourW));
		info->labelContours(getToggle(showLabelW));
		info->levelsAreFractional(getToggle(fractionW));
		// The three widgets in a linestyle radio box.
		if(getToggle(dashNegativeW))
			info->lineStyle(AvContourInfo::DASHEDifNEGATIVE);
		else
		if(getToggle(useSolidLinesW))
			info->lineStyle(AvContourInfo::SOLID);
		else
			info->lineStyle(AvContourInfo::DASHED);

		info->lineWidth(getSlider());

		// Color style.
		for(uInt i=0; i< NCOLORS; i++)
			if(getToggle(colorinfo_[i].widget))
			{	info->colorStyle(colorinfo_[i].color);
				break;
			}

		info->drawTitle(getToggle(drawTitleW));
		info->enableBlanking(getToggle(enableBlankingW));
		saveValues(info);
		info->clearCallbackMask();
		{double value;
			checkMultiplier();
			if(getMultiplier(value))
				info->scale(value);
		}
		info->emphasisEnabled(getToggle(emphasisEnabledW_));

	info->enableNotify(FALSE);

	if(getToggle(immediateW))
		info->immediateUpdate(TRUE);
	else
		info->immediateUpdate(FALSE);
}

// Copy current settings from info.
void	AvContourList::restoreInfo(AvContourInfo *info)
{
	if((dialog == NULL) || (info == NULL))	// Nothing to restore to.
		return;

	setToggle(immediateW, info->immediateUpdate());
	setToggle(showContourW, info->drawContours());
	setToggle(showLabelW, info->labelContours());
#if 0
	setToggle(fractionW, info->levelsAreFractional(), True);
#else
	if(info->levelsAreFractional())
		setToggle(fractionW, True, True);
	else
		setToggle(valueW, True, True);
#endif
	setToggle(emphasisEnabledW_, info->emphasisEnabled());

	switch(info->lineStyle()) {
	case AvContourInfo::SOLID:
		setToggle(useSolidLinesW, True, True);
		break;
	case AvContourInfo::DASHED:
		setToggle(useDashedLinesW, True, True);
		break;
	case AvContourInfo::DASHEDifNEGATIVE:
		setToggle(dashNegativeW, True, True);
		break;
	default:;
	}

	setSlider(linewidthW_, info->lineWidth());

	// Color style
	{ int cs = info->colorStyle();
		for(uInt i=0; i< NCOLORS; i++)
			if(cs == colorinfo_[i].color)
			{	setToggle(colorinfo_[i].widget, True, True);
				break;
			}
	}

	setToggle(drawTitleW, info->drawTitle());
	setToggle(enableBlankingW, info->blankingEnabled());
	setMultiplier(info->scale());
	// Save the value list.
	int len = info->numContourLevels();
	double *lst = info->getContourLevels();
	appendValueList(lst, len, TRUE);
	delete [] lst;
}

// Reset widget pointers to NULL. If destroyWidgets is TRUE,
// kill them off first.
void	AvContourList::destroy(Boolean destroyWidgets)
{
	if(destroyWidgets && (dialog != NULL))
		XtDestroyWidget(dialog);
	dialog = selbox = NULL;
	// The other widgets belong to item.
}

/* Build the selection composite.
*/
void AvContourList::buildList()
{
const char *name;
Widget parentW;

	name = name_.chars();

	// If dialog is a DialogShell, Pressing return in the text field
	// unmaps the shell.
	dialog = XtVaCreatePopupShell(name, 
		topLevelShellWidgetClass, ref_,
#ifndef STANDALONE
		XmNcolormap,	AvXColorApp::colormap(),
#endif
		NULL);


	// Container for everything.
	parentW = XtVaCreateWidget("contourRC",
			xmRowColumnWidgetClass,
			dialog,
			NULL);


	////////////////////////////////////////////////////////////////////
	////////////	Value/Fraction toggles.			////////////
	////////////////////////////////////////////////////////////////////
	/* Widget valueLabel = */
	XtVaCreateManagedWidget("Values are:",
				xmLabelWidgetClass,
				parentW,
				NULL);
	
#if 0
	Widget radioFrame = XtVaCreateManagedWidget("Values are:",
			xmFrameWidgetClass,
			parentW,
			NULL);
#endif
	// Radio box to hold "Value" & "Fraction" toggles.
	Widget valueBox;
	{XmString vstr = AvXmStringCreateLocalized(valueNames[0]);
	 XmString fstr = AvXmStringCreateLocalized(valueNames[1]);
	 char *vb = Strdup("valueBox");

		valueBox = XmVaCreateSimpleRadioBox( parentW,
			vb, 0, NULL,
			XmNorientation, XmHORIZONTAL,
			XmVaRADIOBUTTON, vstr, 0, 0, 0,
			XmVaRADIOBUTTON, fstr, 0, 0, 0,
			NULL);
		delete [] vb;
		XmStringFree(vstr);
		XmStringFree(fstr);
	}

	// We need both widgets since setting one of them to False doesn't
	// seem to work.
	valueW = XtNameToWidget(valueBox, "button_0");
	XtAddCallback(valueW, XmNvalueChangedCallback,
			&AvContourList::handleToggleCB,	(XtPointer) this);
	fractionW = XtNameToWidget(valueBox, "button_1");
	XtAddCallback(fractionW, XmNvalueChangedCallback,
			&AvContourList::handleToggleCB,	(XtPointer) this);

	XtVaCreateManagedWidget("",
				xmSeparatorWidgetClass,
				parentW,
				NULL);
	////////////////////////////////////////////////////////////
	///		Line styles
	/* Widget lineStyleLabel = */
	XtVaCreateManagedWidget("Contour Lines are:",
				xmLabelWidgetClass,
				parentW,
				NULL);

	Widget lineStyleBox;
	{XmString str1 = AvXmStringCreateLocalized("Solid");
	 XmString str2 = AvXmStringCreateLocalized("Dashed");
	 XmString str3 = AvXmStringCreateLocalized("Dashed if < 0");
	 char *lsb = Strdup("lineStyleBox");

		lineStyleBox = XmVaCreateSimpleRadioBox( parentW,
			lsb, 2, NULL,
			XmNorientation, XmHORIZONTAL,
			XmVaRADIOBUTTON, str1, 0, 0, 0,
			XmVaRADIOBUTTON, str2, 0, 0, 0,
			XmVaRADIOBUTTON, str3, 0, 0, 0,
			NULL);
		delete [] lsb;
		XmStringFree(str1);
		XmStringFree(str2);
		XmStringFree(str3);
	}

	useSolidLinesW = XtNameToWidget(lineStyleBox, "button_0");
	XtAddCallback(useSolidLinesW, XmNvalueChangedCallback,
			&AvContourList::handleToggleCB,	(XtPointer) this);
	useDashedLinesW = XtNameToWidget(lineStyleBox, "button_1");
	XtAddCallback(useDashedLinesW, XmNvalueChangedCallback,
			&AvContourList::handleToggleCB,	(XtPointer) this);
	dashNegativeW = XtNameToWidget(lineStyleBox, "button_2");
	XtAddCallback(dashNegativeW, XmNvalueChangedCallback,
			&AvContourList::handleToggleCB,	(XtPointer) this);


	////////////////////////////////////////////////////////////
	///		Color styles
	// Build a radio box then add a bunch of toggles.
	Widget colorStyleBox;
	{uInt i;
	 char *csb = Strdup("colorStyleBox");

		colorStyleBox = XmVaCreateSimpleRadioBox( parentW,
			csb, AvContourInfo::MIXED, NULL,
			XmNnumColumns,	4,
			XmNpacking, XmPACK_COLUMN,
			XmNorientation, XmHORIZONTAL,
			NULL);
		delete [] csb;

		for(i=0; i< NCOLORS; i++)
		{  Widget w = XtVaCreateManagedWidget(
			colorinfo_[i].name,
			xmToggleButtonWidgetClass,
			colorStyleBox,
			NULL);
			XtAddCallback(w, XmNvalueChangedCallback,
			    &AvContourList::handleColorCB, (XtPointer) this);
		   colorinfo_[i].widget = w;
		}
	}

	linewidthW_ = buildSliderBox(parentW, "Line Width", 1, 21,
				2, 0, TRUE, this,
				&AvContourList::widthSliderCB);

	// Returned widget is RCbox around it.
	linewidthW_ = childNameToWidget(linewidthW_, "slider");

	/* Widget sepW3 = */
	XtVaCreateManagedWidget("",
				xmSeparatorWidgetClass,
				parentW,
				NULL);
	////////////////////////////////////////////////////////////

	// Radio box to hold "drawContours" and "drawLabels" toggles.
	Widget labelBox;
	{
		labelBox = XtVaCreateManagedWidget("labelRC",
			xmRowColumnWidgetClass,
			parentW,
//			XmNorientation, XmHORIZONTAL,
			XmNnumColumns, 2,
			XmNpacking, XmPACK_COLUMN,
			NULL);
		showContourW = XtVaCreateManagedWidget("Show Contours",
			xmToggleButtonWidgetClass,
			labelBox,
			XmNset, True,
			NULL);
		XtAddCallback(showContourW, XmNvalueChangedCallback,
			&AvContourList::handleToggleCB,	(XtPointer) this);

		showLabelW = XtVaCreateManagedWidget("Show Labels",
			xmToggleButtonWidgetClass,
			labelBox,
			NULL);
		XtAddCallback(showLabelW, XmNvalueChangedCallback,
			&AvContourList::handleToggleCB,	(XtPointer) this);

		drawTitleW = XtVaCreateManagedWidget("Show Title",
			xmToggleButtonWidgetClass,
			labelBox,
			NULL);
		XtAddCallback(drawTitleW, XmNvalueChangedCallback,
			&AvContourList::handleToggleCB,	(XtPointer) this);

		enableBlankingW = XtVaCreateManagedWidget("Enable Blanking",
			xmToggleButtonWidgetClass,
			labelBox,
			NULL);
		XtAddCallback(enableBlankingW, XmNvalueChangedCallback,
			&AvContourList::handleToggleCB,	(XtPointer) this);

		emphasisEnabledW_ = XtVaCreateManagedWidget("Emphasize",
			xmToggleButtonWidgetClass,
			labelBox,
			XmNset, False,
			NULL);
		XtAddCallback(emphasisEnabledW_, XmNvalueChangedCallback,
			&AvContourList::handleToggleCB, (XtPointer) this);


		immediateW = XtVaCreateManagedWidget("Immediate Update",
			xmToggleButtonWidgetClass,
			labelBox,
			NULL);

		XtAddCallback(immediateW, XmNvalueChangedCallback,
			&AvContourList::handleToggleCB,	(XtPointer) this);
	}

	XtVaCreateManagedWidget("",
				xmSeparatorWidgetClass,
				parentW,
				NULL);

	////////////////////////////////////////////////////////////////////
	///		Multiplier text field.			////////////
	////////////////////////////////////////////////////////////////////
	Widget rcW = XtVaCreateWidget("contourvalueRC",
			xmRowColumnWidgetClass,
			parentW,
			XmNorientation, XmHORIZONTAL,
			NULL);
	/* Widget label = */
	XtVaCreateManagedWidget("Values will be multiplied by:",
				xmLabelWidgetClass,
				rcW,
				NULL);
	multW = XtVaCreateManagedWidget("MultiplierTF",
			xmTextFieldWidgetClass,
			rcW,
			NULL);
	XtAddCallback(multW, XmNactivateCallback, &AvContourList::multCB,
						(XtPointer) this);
	setMultiplier(1.0);		// Default value.

	/* Widget sepW1 = */ 
	XtVaCreateManagedWidget("",
				xmSeparatorWidgetClass,
				parentW,
				NULL);
	

	////////////////////////////////////////////////////////////////////
	///		Selection Box.					////
	////////////////////////////////////////////////////////////////////

	// The apply button is used as a delete button.
	{XmString delstr = AvXmStringCreateLocalized("Delete");
	 XmString addstr = AvXmStringCreateLocalized("Add");
		selbox = XtVaCreateWidget(name, xmSelectionBoxWidgetClass,
			parentW,
			XmNokLabelString, addstr,
			// The apply button is used as a delete button.
			XmNapplyLabelString, delstr,
			XmNmustMatch, (match_ == MAY_MATCH) ? False : True,
			XmNchildPlacement, XmPLACE_BELOW_SELECTION,
			NULL);
		XmStringFree(addstr);
		XmStringFree(delstr);
	}
	textFieldLabel(textFieldLabel_);
	listLabel(listLabel_);
	if(!showHelp())
	{	showHelp_ = TRUE;	// Need to lie.
		showHelp(FALSE);
	}

	XtAddCallback(selbox, XmNdestroyCallback, &AvContourList::destroyCB,
							(XtPointer) this);
	XtAddCallback(selbox, XmNapplyCallback, &AvContourList::deleteCB,
							(XtPointer) this);
	XtAddCallback(selbox, XmNokCallback, &AvContourList::mightMatchCB,
							(XtPointer) this);
	XtAddCallback(selbox, XmNcancelCallback, &AvContourList::cancelCB,
							(XtPointer) this);
	XtAddCallback(selbox, XmNnoMatchCallback, &AvContourList::noMatchCB,
							(XtPointer) this);


	Widget done = XtVaCreateManagedWidget("Done",
			xmPushButtonWidgetClass, selbox, NULL);
	XtAddCallback(done, XmNactivateCallback, &AvContourList::doneCB, this);

	Widget apply = XtVaCreateManagedWidget("Apply",
			xmPushButtonWidgetClass, selbox, NULL);
	XtAddCallback(apply, XmNactivateCallback,
			&AvContourList::applyCB,this);

	Widget clear = XtVaCreateManagedWidget("Clear",
			xmPushButtonWidgetClass, selbox, NULL);
	XtAddCallback(clear, XmNactivateCallback, &AvContourList::clearCB,
							(XtPointer)this);

	list = XmSelectionBoxGetChild(selbox, XmDIALOG_LIST);
	del = XmSelectionBoxGetChild(selbox, XmDIALOG_APPLY_BUTTON);

	Widget help = XmSelectionBoxGetChild(selbox, XmDIALOG_HELP_BUTTON);
	XtAddCallback(help, XmNactivateCallback, &AvContourList::helpCB, this);

	XtManageChild(del);	// "Apply" isn't managed by default.

	XtManageChild(valueBox);
	XtManageChild(lineStyleBox);
	XtManageChild(colorStyleBox);
	XtManageChild(selbox);
	XtManageChild(rcW);
	XtManageChild(parentW);
}

Boolean AvContourList::getToggle(Widget w)const
{
	if(w != NULL)
		return XmToggleButtonGetState(w) ? TRUE : FALSE ;
	else
		return FALSE;
}

// Change a toggle and optionally notify its callbacks. Usually Notify
// is FALSE, but radio toggles seem to need the notification in order to
// turn off the others in the list. However, this will cause our callbacks
// to be called too. "toggling_" is used to make sure we don't try to
// change info_ unnecessarily.
void AvContourList::setToggle(Widget w, const Boolean set, const Boolean Notify)
{
	if(w != NULL)
	{	toggling_ = TRUE;
			XmToggleButtonSetState(w, set, Notify);
		if(XmToggleButtonGetState(w) != set)
		{	XmToggleButtonSetState(w, set, Notify);
			printf("%d %d\n", set, XmToggleButtonGetState(w));
		}
		toggling_ = FALSE;
	}
}

// Set the value in the info object corresponding to the widget. Radio
// buttons are handled a bit differently. Since both the button being set
// and the one being unset will trigger a change, we only call the member
// function for the one being set.
// Colorstyle is handled in handleColor
void AvContourList::handleToggle(Widget w)
{
Boolean isSet = getToggle(w);

	if(toggling_)	// Ignore if the result of a restoreInfo.
		return;

	if(w == immediateW)
		info_->immediateUpdate(isSet);
	else
	if(w == showContourW)
		info_->drawContours(isSet);
	else
	if(w == showLabelW)
		info_->labelContours(isSet);
	else
#if 0
	if(w == fractionW)
		info_->levelsAreFractional(isSet);
	else
#else
	if(w == fractionW)
	{	if(isSet)
			info_->levelsAreFractional(True);
	}
	else
	if(w == valueW)
	{	if(isSet)
			info_->levelsAreFractional(False);
	}
	else
#endif
	if(w == dashNegativeW)
	{	if(isSet)
			info_->lineStyle(AvContourInfo::DASHEDifNEGATIVE);
	}
	else
	if(w == useSolidLinesW)
	{	if(isSet)
			info_->lineStyle(AvContourInfo::SOLID);
	}
	else
	if(w == useDashedLinesW)
	{	if(isSet)
			info_->lineStyle(AvContourInfo::DASHED);
	}
	else
	if(w == drawTitleW)
		info_->drawTitle(isSet);
	else
	if(w == enableBlankingW)
		info_->enableBlanking(isSet);
	else
	if(w == emphasisEnabledW_)
		info_->emphasisEnabled(isSet);
}

// Update color to use.
void AvContourList::handleColor(Widget w)
{
Boolean isSet = getToggle(w);

	if(toggling_)	// Ignore if the result of a restoreInfo.
		return;
	if(!isSet)
		return;	// Only of interest if it's being set.

	for(uInt i=0; i < NCOLORS; i++)
		if(w == colorinfo_[i].widget)
		{	info_->colorStyle(colorinfo_[i].color);
			break;
		}
}

// TRUE means items can be entered multiple times into the list. FALSE
// means items are unique.
void AvContourList::setMatch(const MATCHES m)
{
	if((selbox != NULL) && (m != match_))
		XtVaSetValues(selbox,
			XmNmustMatch, (m == MAY_MATCH) ? False : True,
			NULL);

		match_ = m;
}

// Number of items in the list.
int AvContourList::getListLength() const
{
int	len;

	XtVaGetValues(selbox, XmNlistItemCount, &len, NULL);
	return len;
}

// Return a pointer to the list.
XmStringTable AvContourList::getXmList()const
{
XmStringTable tbl;

	XtVaGetValues(selbox, XmNlistItems, &tbl, NULL);
	return tbl;
}

/* Return a pointer to an array of string pointers.  Use delete [] to
free both the buffer and the pointers.
If buf is not NULL, it will be used. If it is NULL, a buffer
will be allocated.
 */
char **AvContourList::getList(char **buf)const
{
char **b;
int	len = getListLength();
XmStringTable tbl;

	if(len == 0)
		return NULL;

	b = (buf == NULL) ? new char *[len] : buf;
	tbl = getXmList();
	for(int i=0; i< len; i++)
		b[i] = XmStringToString(tbl[i]);
	return b;
}

// Return a pointer to the String representation of list item at position.
// (1..listLength). If position = 0, returns the last item.
char *AvContourList::getListItem(const int position)const
{
int	len = getListLength();

	if((position < 0) || (position > len))
		return NULL;
	else
	{XmStringTable tbl = getXmList();
	 int i = (position == 0) ? len -1 : position-1;
		return XmStringToString(tbl[i]);
	}
}

// Popup the dialog rebuilding it first if necessary.
void AvContourList::popup()
{
	if(dialog == NULL)
		buildList();

	// Get fresh copy of global data.
	info_->disableNotify();
		restoreInfo(worldInfo_);	// Clears info_'s value list.
		info_->copy(*worldInfo_, *info_);
	info_->enableNotify(FALSE);
	info_->copy(*info_, *backupInfo_);
	XtPopup(dialog, XtGrabNone);
	up_ = TRUE;
}

// Pop down the dialog.
void AvContourList::popdown()
{
	if((dialog != NULL) && isUp())
	{	XtPopdown(dialog);
		up_ = FALSE;
	}
}

// converts an XmString to a string.
char *AvContourList::XmStringToString_(XmString string)
{
char *text;

	if(XmStringGetLtoR(string, XmSTRING_DEFAULT_CHARSET, &text))
		return text;
	else
		return NULL;
}

// converts an XmString to a string. Use delete [] to free.
char *AvContourList::XmStringToString(XmString string)
{
char *text, *tptr;

	if( (tptr = AvContourList::XmStringToString_(string)) == NULL)
		text = NULL;
	else
		text = Strdup(tptr);

	return text;
}

// Returns TRUE if the string is valid for entry into the list
// (ignoring Match).
Boolean AvContourList::validString(const char *str)
{
	if(!emptyStrings() && ( (str == NULL) || (strlen(str) == 0)))
		return FALSE;

	if(numbersOnly() && ! validNumber(str))
		return FALSE;
	else
		return TRUE;
}

Boolean AvContourList::validXmString(XmString s)
{
char *str = XmStringToString_(s);

	return validString(str);
}

// Converts a string into a double. Returns TRUE if the string represents
// a valid number.
Boolean AvContourList::stringToNumber(const char * s, double &val)
{
char *n;

	if(s == NULL)
		return FALSE;
	val = strtod(s, &n);
	if(strlen(n) == 0)
		return TRUE;
	else
		return FALSE;
}

Boolean AvContourList::XmStringToNumber(XmString s, double &val)
{
  return AvContourList::stringToNumber(XmStringToString_(s), val);
}

// Return a list entry as an unscaled number.
Boolean AvContourList::getListValue(const int position, double &value)const
{
char	*str;
Boolean	ok = FALSE;

	if( (str = getListItem(position)) != NULL)
	{	ok = stringToNumber(str, value);
		delete [] str;
	}
	return ok;
}

#if 0
/*
Return a pointer to a list of numbers of length
getListLength(). If values is not NULL, it will be used. Otherwise, an
array will be allocated (use delete []).
 */
double *AvContourList::getListValues(double *values)const
{
double	*b;
int	len = getListLength();

	if(len == 0)
		return NULL;

	b = (values == NULL) ? new double [len] : values;
	for(int i=0; i< len; i++)
	{	getListValue(i+1, b[i]);
	}

	return b;
}
#else
/*
Return a pointer to a list of numbers of length
getListLength(). If values is not NULL, it will be used. Otherwise, an
array will be allocated (use delete []).
 */
AvWPosition AvContourList::getListValues()const
{int	len = getListLength();
 AvWPosition values(len);

	for(int i=0; i< len; i++)
	{double v;
		getListValue(i+1, v);
		values(i) = v;
	}
	return values;
}
#endif

// Set the label above the selection box.
void AvContourList::listLabel(const char *s)
{
	if((listLabel_ != NULL) && (s == listLabel_))
		return;

	delete [] listLabel_;

	if(s == NULL)
		listLabel_ = NULL;
	else
		listLabel_ = Strdup(s);

	if(selbox != NULL)		// Already created.
	{	XmString str = AvXmStringCreateLocalized(listLabel_);
		XtVaSetValues(selbox,
			XmNlistLabelString, str,
			NULL);
		XmStringFree(str);
	}
}

// Set the label above the textfield.
void AvContourList::textFieldLabel(const char *s)
{
	if((textFieldLabel_ != NULL) && (s == textFieldLabel_))
		return;
	delete [] textFieldLabel_;

	if(s == NULL)
		textFieldLabel_ = NULL;
	else
		textFieldLabel_ = Strdup(s);

	if(selbox != NULL)		// Already created.
	{	XmString str = AvXmStringCreateLocalized(textFieldLabel_);
		XtVaSetValues(selbox,
			XmNselectionLabelString, str,
			NULL);
		XmStringFree(str);
	}
}

// Change whether ADD, etc should call ContourList callbacks.
void AvContourList::immediateNotify(const Boolean immediate)
{
	immediateNotify_ = immediate;
}

// (Un)Show the help button.
void AvContourList::showHelp(const Boolean show)
{
	if((selbox != NULL) && (show != showHelp_))
	{ Widget help = XmSelectionBoxGetChild(selbox, XmDIALOG_HELP_BUTTON);
		if(show)
			XtManageChild(help);
		else
			XtUnmanageChild(help);
	}
	showHelp_ = show;
}

void AvContourList::setNumberRange(const double min, const double max)
{
	min_ = min;
	max_ = max;
}

void AvContourList::setNumberRange()
{
	setNumberRange( -FLOATMAX, FLOATMAX);
}

// Is (min <= value <= max)?
Boolean AvContourList::rangeCheck(const double value)const
{
	return ((min_ <= value ) && (value <= max_));
}

// Does the string represent a valid floating point number?
Boolean AvContourList::validNumber(const char * s)const
{
double val;

	if(!stringToNumber(s, val))
		return FALSE;
	return rangeCheck(val);
}

Boolean AvContourList::validXmNumber(XmString string)const
{
char *s;

	s = XmStringToString_(string);
	return validNumber(s);
}

char *AvContourList::Strdup(const char *s)
{
char	*str;

	if(s == NULL)
		return NULL;
	str = new char [strlen(s) + 1];
	strcpy(str, s);
	return str;
}

//		Callbacks for the various buttons.

void AvContourList::printcb(XmSelectionBoxCallbackStruct *s)
{
char *str;

	if(!XmStringEmpty(s->value))
	{	str = XmStringToString(s->value);
		printf("%s |%s|(Len=%d)\n", xmReasonToString(s->reason),
				str, s->length);
		delete [] str;
	}
	else
		printf("%s <empty>(Len=%d)\n", xmReasonToString(s->reason),
				s->length);
}

// Called when Add button is pressed and the item in the selection field
// does not match an item in the list and MUST_MATCH or CANT_MATCH is set.
void AvContourList::noMatch(XmString str)
{
	if(getMatch() == CANT_MATCH)
	{	if(validXmString(str))
		{
//			addXmStringToList(str);
			addXmValueToList(str);
			clearTextField();
			callCallback(ADD);
		}
		else
			callCallback(INVALID);
	}
	else
		callCallback(MATCH);
}

void AvContourList::noMatchCB(Widget , XtPointer cbdata, XtPointer d)
{
AvContourList	*me = (AvContourList *)cbdata;
XmSelectionBoxCallbackStruct *s = (XmSelectionBoxCallbackStruct *) d;

	me->noMatch(s->value);
}

// Called when Add button is pressed and the item in the selection field
// matches an item in the list and MUST_MATCH is set or it doesn't match
// and MAY_MATCH is set.
void AvContourList::mightMatchCB(Widget , XtPointer cbdata, XtPointer d)
{
AvContourList	*me = (AvContourList *)cbdata;
XmString	value;
XmSelectionBoxCallbackStruct *s = (XmSelectionBoxCallbackStruct *) d;

	XtVaGetValues(me->selbox, XmNtextString, &value, NULL);

	if(me->getMatch() == AvContourList::MUST_MATCH ||
	   me->getMatch() == AvContourList::MAY_MATCH)
	{	me->addXmValueToList(s->value);
//		me->addXmStringToList(s->value);
		me->clearTextField();
		me->callCallback(ADD);
	}
	else
		me->callCallback(MATCH);
}

// Called by applyCB.
// Updates current info copy and calls any callbacks.
void AvContourList::apply()
{
	// Make sure contents of the multiplier field are valid.
	checkMultiplier();

	// Make sure global values match internal.
	if(!info_->equals( *worldInfo_))
		info_->copy(*info_, *worldInfo_, True);
	callCallback(ACCEPT);
}

// Called by doneCB when done.
// Same as apply except the GUI window is removed.
void AvContourList::done()
{
	popdown();
	apply();
}

void AvContourList::doneCB(Widget , XtPointer cbdata, XtPointer )
{
AvContourList	*me = (AvContourList *)cbdata;

	me->done();
}

void AvContourList::applyCB(Widget , XtPointer cbdata, XtPointer )
{
AvContourList	*me = (AvContourList *)cbdata;

	me->apply();
}

void AvContourList::clearCB(Widget , XtPointer cbdata, XtPointer )
{
AvContourList	*me = (AvContourList *)cbdata;
//XmSelectionBoxCallbackStruct *s = (XmSelectionBoxCallbackStruct *) d;

	me->clearList();
}

// Called (by deleteCB) when the delete (AKA Accept) button is pressed.
// If one or more items in the list are selected, they will be deleted.
// Otherwise, if an item in the selection box matches an entry in the
// list, the first occurrence of the item is deleted.
// If the selection box is empty, the last item in the list is deleted.
void AvContourList::deleteEntry(XmString xstr)
{
int	*plist, pcount;

	if(XmListGetSelectedPos(list, &plist, &pcount))
	{
		deletePositionsFromList(plist, pcount);
		XtFree((char *)plist);
		clearTextField();
		callCallback(DELETE);
	}
	else
	if( XmStringEmpty(xstr))
	{	deletePositionFromList(0);
		callCallback(DELETE);
	}
	else
	{	deleteXmStringFromList(xstr);
		clearTextField();
		callCallback(DELETE);
	}
}

// Called when the delete (AKA Accept) button is pressed.
void AvContourList::deleteCB(Widget, XtPointer cbdata, XtPointer d)
{
AvContourList	*me = (AvContourList *)cbdata;
XmSelectionBoxCallbackStruct *s = (XmSelectionBoxCallbackStruct *) d;

//	printcb(s);
	me->deleteEntry(s->value);
}

void AvContourList::cancel()
{
	popdown();
	restoreInfo(backupInfo_);	// Copy to toggles.
	// If global values have changed, replace them, causing a redisplay.
	if(!backupInfo_->equals( *worldInfo_))
		backupInfo_->copy(*backupInfo_, *worldInfo_);
	callCallback(CANCEL);
//	printf("Default button is: 0X%x\n", def);
}

void AvContourList::cancelCB(Widget , XtPointer cbdata, XtPointer )
{
AvContourList	*me = (AvContourList *)cbdata;
//XmSelectionBoxCallbackStruct *s = (XmSelectionBoxCallbackStruct *) d;

//	printf("Cancel (%s/0X%x)called.\n", XtName(w), w);

//	me->printcb(s);

	if(me != NULL)
		me->cancel();
}

void AvContourList::helpCB(Widget , XtPointer cbdata, XtPointer )
{
AvContourList	*me = (AvContourList *)cbdata;

	me->callCallback(HELP);
}

int AvContourList::getSlider(Widget w)const
{ int value;

	XtVaGetValues(w, XmNvalue, &value,
		      NULL);
	return value;
}

//#include <Xm/Scale.h>
void AvContourList::setSlider(Widget w, const int value)
{
	XtVaSetValues(w, XmNvalue, value,
		      NULL);
//	XmScaleSetValue(w, value);
}

// Get value of slider. Setting info_ might cause this to be called again.
void AvContourList::widthSliderCB(Widget w, XtPointer cbdata, XtPointer)
{AvContourList	*ci = (AvContourList *)cbdata;
 int   value;

	if(ci->toggling_)
		return;

	ci->toggling_ = TRUE;
		value = ci->getSlider(w);
		ci->info_->lineWidth(value);
	ci->toggling_ = FALSE;
}


// Called if the popup gets destroyed. (Probably by user).
void AvContourList::destroyCB(Widget, XtPointer cbdata, XtPointer)
{
AvContourList	*me = (AvContourList *)cbdata;

//	printf("DestroyCB called.\n");
	if(me != NULL)
	{	me->callCallback(DESTROYED);
		me->destroy(FALSE);
		me->up_ = FALSE;
	}
}

// Called when the multiplier text field receives a <RETURN>.
// Checks for valid number.
void AvContourList::multCB(Widget , XtPointer cbdata, XtPointer )
{
AvContourList *me = (AvContourList *) cbdata;

	me->checkMultiplier();
}

void AvContourList::handleToggleCB(Widget w, XtPointer cbdata, XtPointer )
{
AvContourList	*me = (AvContourList *)cbdata;

	me->handleToggle(w);
}

void AvContourList::handleColorCB(Widget w, XtPointer cbdata, XtPointer )
{
AvContourList	*me = (AvContourList *)cbdata;

	me->handleColor(w);
}

// Called when info_ is modified
void AvContourList::infoCB(XtPointer o, AvConductor *, XtPointer)
{
AvContourList	*me = (AvContourList *)o;

	// If in immediate mode, copy current settings to global struct.
	if(me->info_->immediateUpdate())
		AvContourInfo::copy( *(me->info_), *(me->worldInfo_));
}

////////////////////////////////////////////////////////////////
//		Utility routines to get/set text & list fields.

void AvContourList::addXmStringToList(XmString str, const int position)
{
	XmListAddItem(list, str, position);
}

// Add an XmString representing a number to the list in sorted order.
// If numbersOnly() returns FALSE, the value is appended to the list.
void AvContourList::addXmValueToList(XmString str)
{
double	val;

	if(!numbersOnly() || (sorting()==APPEND))
	{	addXmStringToList(str);
		return;
	}
	if(XmStringToNumber(str, val))
	{int	len = getListLength();

		for(int position= 1; position <= len; position++)
		{double entry, a, b;

			getListValue(position, entry);
			if(sorting() == INCREASING)
			{	a = val;
				b = entry;
			}
			else
			{	a = entry;
				b = val;
			}
			if( a < b)
			{	addXmStringToList(str, position);
				return;
			}
			else
			if(a == b)
			{	if(getMatch()!=CANT_MATCH)
					addXmStringToList(str, position);
				return;
			}
		}
		// Append to end.
		addXmStringToList(str, 0);
	}
}

void AvContourList::addStringToList(char *str, const int /*position*/)
{
	if(str == NULL)
		return;
	XmString xs = AvXmStringCreateLocalized(str);
//		addXmStringToList(xs, position);
		addXmValueToList(xs);
	XmStringFree(xs);
}

void AvContourList::addValueToList(double val, const int position)
{
char	str[80];

	sprintf(str, "%g", val);
	addStringToList(str, position);
}


void	AvContourList::deleteXmStringFromList(XmString str)
{
	// The check for existence is done so there won't be any
	// warning messages.
	if(itemExists(str))
	{	XmListDeleteItem(list, str);
		callCallback(DELETE);
	}
}

void AvContourList::deleteStringFromList(char *str)
{
	if(str == NULL)
		return;

	XmString xs = AvXmStringCreateLocalized(str);
		deleteXmStringFromList(xs);
	XmStringFree(xs);
}

void AvContourList::deletePositionFromList(const int position)
{
	XmListDeletePos(list, position);
}

void AvContourList::deletePositionsFromList(int *plist, const int count)
{
	XmListDeletePositions(list, plist, count);
}

XmString AvContourList::getXmTextField()
{
XmString value;

	XtVaGetValues(selbox, XmNtextString, &value, NULL);
	return value;
}

void AvContourList::setMultiplier(const double value)
{
char	buf[64];

	sprintf(buf, " %g", value);
	XtVaSetValues(multW, XmNvalue, buf, NULL);
}

// Return the value in the multiplier window. Returns FALSE if it isn't
// a valid number, else TRUE.
Boolean AvContourList::getMultiplier(double &value)const
{
char	*str;

	XtVaGetValues(multW, XmNvalue, &str, NULL);
	if(stringToNumber( str, value))
		return TRUE;
	else
		return FALSE;
}

// If the value in the multiplier field isn't valid, reset it.
void AvContourList::checkMultiplier()
{
double	value;

	if(!getMultiplier(value))
	{	value = 1.0;
		setMultiplier(value);
	}
	info_->scale(value);
}

// Is the string already in the list?
Boolean AvContourList::itemExists(XmString str)const
{
	return XmListItemExists(list, str);
}

// Put the string into the text field
void AvContourList::setXmTextField(XmString str)
{
	if(str == NULL)
		return;

	XtVaSetValues(selbox, XmNtextString, str, NULL);
}

void AvContourList::setTextField(const char *Str)
{ const char *str;

	if(Str == NULL)
		str = "";
	else
		str = Str;

	XmString xs = AvXmStringCreateLocalized(str);
	setXmTextField(xs);
	XmStringFree(xs);
}

// Return the contents of the text field.
char *AvContourList::getTextField()
{
	return XmStringToString(getXmTextField());
}

// Clears the text field by calling setTextField with an empty string.
void AvContourList::clearTextField()
{
	setTextField(NULL);
}

// Save the current contents of the list for later restore.
// Strings are used instead of XmStrings since we'd have to allocate space
// for the XmStrings anyway and there is already a routine to get the char
// strings.
#if 0
void AvContourList::saveList()
{

#if 0
	// Delete the current list.
	deleteStringList(saveList_, saveLength_);
	saveList_ = NULL;

	saveLength_ = getListLength();

	if(saveLength_ <= 0)
		return;		// Nothing to save.
	saveList_ = getList();
#endif
}

void AvContourList::restoreList()
{
#if 0
Boolean in = immediateNotify_;

	immediateNotify_ = FALSE;
	appendList(saveList_, saveLength_, TRUE);
	immediateNotify_ = in;
#endif
}
#endif

void AvContourList::clearList(const Boolean notify)
{	if(list != NULL)
	{	XmListDeleteAllItems(list);
		if(notify)
			callCallback(DELETE);
	}
}

// Append the items in list to the selection list. If clear is TRUE,
// the selection list is cleared first.
void AvContourList::appendList(char **lst, const int len, const Boolean clear)
{
	if(clear)
		clearList(FALSE);
	for(int i=0; i< len; i++)
		addStringToList(lst[i], 0);
	callCallback(ADD);
}

void AvContourList::appendValueList(double *lst, const int len,
							const Boolean clear)
{
	if(clear)
		clearList(FALSE);
	for(int i=0; i< len; i++)
		addValueToList(lst[i], 0);
	callCallback(ADD);
}

// Deletes the contents of list, then deletes list.
void AvContourList::deleteStringList(char **lst, const int len)
{
	if(lst == NULL)
		return;
	for(int i=0; i< len; i++)
		delete [] lst[i];
	delete [] lst;
}

////////////////////////////////////////////////////////////////

// Call the callback for the particular reason.
void AvContourList::callCallback(const CBReason why)
{
Boolean immediate;

	switch(why) {
	case ACCEPT:
	case CANCEL:
	case HELP:
	case DESTROYED:
		immediate = TRUE;
		break;
	case ADD:
	case DELETE:
		saveValues(info_);	// Update the contour level list.
	case INVALID:
	case MATCH:
		immediate = immediateNotify();
		break;
	default:
		immediate = FALSE;		// Oops.
		break;
	}

	if((callback_ != NULL) && immediate)
		(callback_)(this, cbdata_, why);
}

// Add a callback.
void AvContourList::addCallback(AvContourListCallback cb, XtPointer cbdata)
{
	callback_ = cb;
	cbdata_ = cbdata;
}

static struct {
	int reason;
	const char *name;
	} reasons[] = {
	{XmCR_NONE, "XmCR_NONE"},
	{XmCR_HELP, "XmCR_HELP"},
	{XmCR_VALUE_CHANGED, "XmCR_VALUE_CHANGED"},
	{XmCR_INCREMENT, "XmCR_INCREMENT"},
	{XmCR_DECREMENT, "XmCR_DECREMENT"},
	{XmCR_PAGE_INCREMENT, "XmCR_PAGE_INCREMENT"},
	{XmCR_PAGE_DECREMENT, "XmCR_PAGE_DECREMENT"},
	{XmCR_TO_TOP, "XmCR_TO_TOP"},
	{XmCR_TO_BOTTOM, "XmCR_TO_BOTTOM"},
	{XmCR_DRAG, "XmCR_DRAG"},
	{XmCR_ACTIVATE, "XmCR_ACTIVATE"},
	{XmCR_ARM, "XmCR_ARM"},
	{XmCR_DISARM, "XmCR_DISARM"},
	{XmCR_MAP, "XmCR_MAP"},
	{XmCR_UNMAP, "XmCR_UNMAP"},
	{XmCR_FOCUS, "XmCR_FOCUS"},
	{XmCR_LOSING_FOCUS, "XmCR_LOSING_FOCUS"},
	{XmCR_MODIFYING_TEXT_VALUE, "XmCR_MODIFYING_TEXT_VALUE"},
	{XmCR_MOVING_INSERT_CURSOR, "XmCR_MOVING_INSERT_CURSOR"},
	{XmCR_EXECUTE, "XmCR_EXECUTE"},
	{XmCR_SINGLE_SELECT, "XmCR_SINGLE_SELECT"},
	{XmCR_MULTIPLE_SELECT, "XmCR_MULTIPLE_SELECT"},
	{XmCR_EXTENDED_SELECT, "XmCR_EXTENDED_SELECT"},
	{XmCR_BROWSE_SELECT, "XmCR_BROWSE_SELECT"},
	{XmCR_DEFAULT_ACTION, "XmCR_DEFAULT_ACTION"},
	{XmCR_CLIPBOARD_DATA_REQUEST, "XmCR_CLIPBOARD_DATA_REQUEST"},
	{XmCR_CLIPBOARD_DATA_DELETE, "XmCR_CLIPBOARD_DATA_DELETE"},
	{XmCR_CASCADING, "XmCR_CASCADING"},
	{XmCR_OK, "XmCR_OK"},
	{XmCR_CANCEL, "XmCR_CANCEL"},
	{XmCR_APPLY, "XmCR_APPLY"},
	{XmCR_NO_MATCH, "XmCR_NO_MATCH"},
	{XmCR_COMMAND_ENTERED, "XmCR_COMMAND_ENTERED"},
	{XmCR_COMMAND_CHANGED, "XmCR_COMMAND_CHANGED"},
	{XmCR_EXPOSE, "XmCR_EXPOSE"},
	{XmCR_RESIZE, "XmCR_RESIZE"},
	{XmCR_INPUT, "XmCR_INPUT"},
	{XmCR_GAIN_PRIMARY, "XmCR_GAIN_PRIMARY"},
	{XmCR_LOSE_PRIMARY, "XmCR_LOSE_PRIMARY"},
	{XmCR_CREATE, "XmCR_CREATE"},
	{XmCR_TEAR_OFF_ACTIVATE, "XmCR_TEAR_OFF_ACTIVATE"},
	{XmCR_TEAR_OFF_DEACTIVATE, "XmCR_TEAR_OFF_DEACTIVATE"},
	{XmCR_OBSCURED_TRAVERSAL, "XmCR_OBSCURED_TRAVERSAL"}
	};

const char *AvContourList::xmReasonToString(const int reason)
{
int i;

	for(i = 0; i< (int) (sizeof(reasons)/sizeof(*reasons)); i++)
		if(reason == reasons[i].reason)
			return reasons[i].name;

	return "Unknown";
}


static struct REASONS {
	AvContourList::CBReason	reason;
	const char *		name;
	} CBReasons[] = {
	{ AvContourList::ACCEPT, "ACCEPT"},
	{ AvContourList::CANCEL, "CANCEL"},
	{ AvContourList::HELP, "HELP"},
	{ AvContourList::ADD, "ADD"},
	{ AvContourList::MATCH, "MATCH"},
	{ AvContourList::INVALID, "INVALID"},
	{ AvContourList::DELETE, "DELETE"},
	{ AvContourList::DESTROYED, "DESTROYED"}
	};
static const uInt NREASONS = (uInt)(sizeof(CBReasons)/sizeof(*CBReasons));

const char *AvContourList::CBReasonToString(const int reason)
{
	for(uInt i=0; i< NREASONS; i++)
		if(reason == CBReasons[i].reason)
			return CBReasons[i].name;

	return "UNKNOWN";
}
