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
//---------------------------------------------------------------------------
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvContourList.h,v 19.0 2003/07/16 05:46:56 aips2adm Exp $
//
// $Log: AvContourList.h,v $
// Revision 19.0  2003/07/16 05:46:56  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:40  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:09  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:17  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:17  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:57:46  hravlin
// Changes to remove char * <- const char * compiler warnings.
//
// Revision 14.0  2000/03/23 16:07:59  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.0  1999/08/10 18:39:43  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:09  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:04  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:47  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.2  1997/09/17 14:46:21  hr
// Added filename_ and emphasisEnabledW_.
//
// Revision 9.1  1997/09/15 16:31:28  hr
// Added support for changing line width.
//
// Revision 9.0  1997/08/25 21:25:59  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:50  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/02/06 20:03:37  hr
// Changed name_ to an AvString.
//
// Revision 7.2  1996/12/12 09:00:32  droberts
// Final update from monet archive.
//
// Revision 1.2  1996/11/05 21:14:02  hr
// Several changes to deal with new AvConfigInfo used elsewhere.
// getListValues() changed to return an AvWPosition.
//
// Revision 1.1  1996/07/11  21:31:21  pixton
// Automated Checkin
//
//  Revision 1.6  1996/06/18  18:42:47  pixton
//  Copyright Update
//
//  Revision 1.5  1996/04/17  20:31:46  hr
//  Added apply(), applyCB().
//
//  Revision 1.4  1996/02/01  17:50:23  hr
//  Added SORTING and addXMValueToList.
//  Modified getListItem to take a position (1..n) rather than index (0..len-1).
//
//  Revision 1.3  1996/01/31  17:36:53  hr
//  Added valueW since it doesn't seem possible to explictly turn off a
//  radio button.
//
//  Revision 1.2  1996/01/24  22:17:49  hr
//  Redid how color toggles are handled.
//
//  Revision 1.1  1996/01/24  18:07:44  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvContourList.h
Implements a list dialog where the user can enter a list of strings.
It is possible to restrict new entries to:
	Unique items.
	Non empty strings.
	A range of floating point numbers.

The easiest way to make a selection list:
	AvContourList *list =
		new AvContourList(char *name, Widget ref);

name is the name for the popup shell.
ref is a widget used to obtain a parent shell.

	list->addString( <char string>, int position);	// 0 to append
Do this for each item to appear in the list.

	list->addCallback( callback, callback_data);

	list->popup();

Return to X loop.

The callback will be called:

 Reason		Immediate	Explanation
		Mode only
--------	----------	---------------------
ACCEPT				When the "Accept" button is pressed.
CANCEL				When the "Cancel" button is pressed.
ADD		Y		When an entry has been added.
INVALID		Y		When an entry has failed to be added because
				it was an invalid string.
DELETE		Y		When an entry has been deleted.
MATCH		Y		When an entry failed to be added because it
				matched an existing entry.
HELP				When the help button is pushed.
				This and changing the visibility of the
				button are the only support for "Help".
DESTROYED			Called when the object is being destroyed.
				The object still exists, but the widgets may
				have been destroyed.

"Y" under Immediate are only called if immediateMode has been set to TRUE.

When the callback is called with either "ACCEPT" or "CANCEL":
	list->getListLength();
	and some combination of:
		getListItem
		getListValue(s)	If the list is restricted to numbers.

The virtual function "Boolean validString(char *)" is called whenever
the user tries to enter a new value. This allows subclasses to define
different valid strings.

The list is saved when the list is popped up and will be restored if
the "Cancel" button is pressed.

To enter an item into the the list, type it in the selection field
and either press <RETURN> or toggle the "Add" button. Pressing "Delete"
will delete the first occurrence of the entry in the selection field
or the last item on the list.

*/
#ifndef AvContourList_h
#define AvContourList_h
#include <X11/Intrinsic.h>
#include <AvContourInfo.h>
#include <AvWPosition.h>
#include <AvString.h>

class AvConductor;
class AvImageDataItem;
class AvContourList {
  public:
	AvContourList(const char *name, Widget ref);
	virtual ~AvContourList();
	AvContourInfo *contourInfo()const { return worldInfo_;}
	void contourInfo(AvContourInfo *);
	//////////////////////////////////////////////////////////////////
	//		Configuration functions.
	// Determines whether identical items can be added to the list.
	enum MATCHES { CANT_MATCH, MUST_MATCH, MAY_MATCH};
	void setMatch(const MATCHES m);
	MATCHES getMatch()const {return match_;} // Default is CANT_MATCH.
	// Whether empty strings can be added.
	// Default = FALSE
	void emptyStrings(const Boolean e) {emptyStrings_ = e;}
	Boolean emptyStrings()const {return emptyStrings_;}
	// If set to TRUE, only floating point #s will be accepted.
	// Default is FALSE;
	void numbersOnly(const Boolean n) {numbersOnly_ = n;}
	Boolean numbersOnly()const {return numbersOnly_;}
	// How numbers should be added to list.
	enum SORTING { APPEND, INCREASING, DECREASING};
	SORTING sorting()const {return sorting_;}
	void sorting(const SORTING s){ sorting_ = s;}
	// Set the label above the textfield. (Default is "Enter:").
	void textFieldLabel(const char *);
	// Set the label above the selection box. (Default is "Values").
	void listLabel(const char *);
	// Will help button be displayed?
	Boolean showHelp()const {return showHelp_;}
	void showHelp(const Boolean show);	// Default is FALSE.
	// (Re)Set, Get, the range of valid numbers.
	void setNumberRange();		// Reset
	void setNumberRange(const double min, const double max);
	void getNumberRange(double &min, double &max)const {
			min = min_; max = max_;}
	// Is (min <= value <= max)?
	virtual Boolean rangeCheck(const double value)const;
	//////////////////////////////////////////////////////////////////

	/////////		Access to the selection box list.
	// Return the number of items on the list.
	int getListLength()const;
	// Return the item at position (1..listlength) or NULL.
	// Use delete [] to free. 0 means last item on the list.
	char *getListItem(const int position)const;
	// Return a value as an unscaled number. Returns TRUE if string at
	// the position represents a valid number. Position 1 is the first.
	// 0 means the last value.
	Boolean getListValue(const int position, double &value)const;
	// Return a pointer to a list of scaled numbers of length
	// getListLength(). If values is not NULL, it will be used.
	// Otherwise, an array will be allocated (use delete []).
//	double *getListValues(double *values=NULL)const;
	// Return a pointer to a list of scaled numbers.
	// getListLength(). (Each value will be multiplied by getMultiplier()).
	AvWPosition getListValues()const;

	// Return a pointer to an array of string pointers.
	// Use delete [] to free both the buffer and the pointers. (Or
	// call 'deleteStringList()').
	// If buf is not NULL, it will be used. If it is NULL, a buffer will
	// be allocated.
	char **getList(char **buf=NULL)const;

	//////////////////////////////////////////////////////////////////
	////// Various operations on the list.
	/////	(Except for addString or appendList, not normally needed).
	// If position is 0, the end of the list is used.
	void addXmStringToList(XmString str, const int position=0);
	// Add value to list in sorted order. Defaults to addXmStringToList
	// if numbersOnly() is FALSE. This is called internally to enter
	// new values.
	void addXmValueToList(XmString value);
	void addStringToList(char *str, const int position=0);
	void addValueToList(double val, const int position=0);
	// Delete the first occurrence of the string.
	void deleteXmStringFromList(XmString);
	void deleteStringFromList(char *str);
	// Delete entry(ies) at the specified positions.
	void deletePositionFromList(const int position=0);
	void deletePositionsFromList(int *list, const int count);
	// Return pointer to the XmString list. DON'T free.
	XmStringTable getXmList()const;

	// Operations on the text field.
	XmString getXmTextField();
	// Returns TRUE if the string is in the selection box.
	Boolean	itemExists(XmString s)const;
	// Return contents of text field. Use delete [] to free.
	char *getTextField();
	void setTextField(const char *str);
	void setXmTextField(XmString str);
	void clearTextField();
	// Append a list to the list perhaps clearing the old list first.
	void	appendList(char **list, const int len,
					const Boolean clearFirst=TRUE);
	// Append a list of doubles to the list perhaps clearing the
	// old list first.
	void	appendValueList(double *list, const int len,
					const Boolean clearFirst=TRUE);

	// Operations on the multiplier text field.
	Boolean getMultiplier(double &value)const;
	void setMultiplier(const double);
	//////////////////////////////////////////////////////////////////
	// Return the selection box widget.
	Widget selectionBoxWidget()const {return selbox;}
	// Returns TRUE if s is valid for entry into the list.
	// (Ignoring Match()).
	virtual Boolean validXmString(XmString s);
	virtual Boolean validString(const char *s);
	void	popup();
	void	popdown();
	Boolean isUp()const{return up_;}		// Popped up?

	////////////////////////////////////////////////////////////////
	///////		Callback support
	enum CBReason { ACCEPT, CANCEL, ADD, INVALID, MATCH, DELETE,
							HELP, DESTROYED};
	typedef void(*AvContourListCallback)
			(AvContourList *, XtPointer cbdata, const int reason);
	// Only one callback at a time is supported for now.
	void addCallback(AvContourListCallback, XtPointer cbdata);
	// Should ADD, and DELETE buttons call the callback?
	void immediateNotify(const Boolean );
	Boolean immediateNotify()const {return immediateNotify_;}
	// Convert from a CBReason to a character string (don't delete).
	static const char *CBReasonToString(const int reason);
	//////////////////////////////////////////////////////////////////
	//		General purpose XmString utilities.
	// Converts an XmString to a string.
	// Use delete [] to free.
	static char	*XmStringToString(XmString str);
	// Operations on number strings. Returns FALSE if s doesn't represent
	// a valid floating point number within the set range.
	static Boolean stringToNumber(const char * s, double &val);
	static Boolean XmStringToNumber(XmString s, double &val);
	//////////////////////////////////////////////////////////////////
	// Returns TRUE if the string represents a valid number.
	virtual Boolean validNumber(const char *)const;
	virtual Boolean validXmNumber(XmString)const;
	//		Internal utility functions.
	// Deletes a list of string pointers.
	static void deleteStringList(char **list, const int len);
	static char *Strdup(const char *);
	//////////////////////
	// Are values "full values" or fractions of the range?
	Boolean fractional()const;
	void fractional(const Boolean f);
  protected:
	// Called internally
	void	callCallback(const CBReason);
	// Converts an XmString to a string.
	static char	*XmStringToString_(XmString str);
  private:
	// Ensure that the multiplier field has a valid value.
	void checkMultiplier();
	void printcb(XmSelectionBoxCallbackStruct *s);
	void buildList();
	// cancel and done handle closing the window when their respective
	// buttons are pressed.
	void cancel();	// Called by cancelCB.
	void done();	// Called by doneCB.
	void apply();	// Called by applyCB.
	// Called (by deleteCB) when the delete button is pressed.
	// If one or more items in the list are selected, they will be deleted.
	// Otherwise, if an item in the selection box matches an entry in the
	// list, the first occurrence of the item is deleted.
	// If the selection box is empty, the last item in the list is deleted.
	void deleteEntry(XmString);
	// Called by noMatchCB to handle insertions.
	void noMatch(XmString );
	void handleToggle(Widget);
	void handleColor(Widget);

	//		Callbacks
	// 
	static void noMatchCB(Widget, XtPointer, XtPointer);
	// User is ready to leave.
	static void doneCB(Widget, XtPointer, XtPointer);
	static void applyCB(Widget, XtPointer, XtPointer);
	static void clearCB(Widget, XtPointer, XtPointer);
	static void sortCB(Widget, XtPointer, XtPointer);
	static void deleteCB(Widget, XtPointer, XtPointer);
	static void mightMatchCB(Widget, XtPointer, XtPointer);
	static void cancelCB(Widget, XtPointer, XtPointer);
	static void destroyCB(Widget, XtPointer, XtPointer);
	static void helpCB(Widget, XtPointer, XtPointer);
	// Called when a toggle button changes. Then calls handleToggle
	static void handleToggleCB(Widget, XtPointer, XtPointer);
	// Called by one of the color buttons
	static void handleColorCB(Widget, XtPointer, XtPointer);
	// Callback for multiplier text field
	static void multCB(Widget w, XtPointer cbdata, XtPointer );
	// Called when info is modified.
	static void infoCB(XtPointer obj, AvConductor *, XtPointer );
	static void widthSliderCB(Widget, XtPointer, XtPointer);
	////
	void	destroy(Boolean destroyWidgets);
	// Convert a Motif "Reason" to its string representation.
	static const char *xmReasonToString(const int r);
#if 0
	// To support cancel.
	void	saveList();
	void	restoreList();
#endif
	// Clears the selection list.
	void	clearList(const Boolean notify=TRUE);
	void	saveInfo(AvContourInfo *); // Save info.
	void	saveValues(AvContourInfo *); // Save Just level values.
	void	restoreInfo(AvContourInfo *); // Restore from info.
	Boolean	getToggle(Widget)const;
	void	setToggle(Widget, const Boolean, const Boolean Notify=FALSE);
	int	getSlider(Widget)const;
	int	getSlider()const {return getSlider(linewidthW_);}
	void	setSlider(Widget, const int);
  private:
	AvString	name_;
	AvString	filename_;
	char		*listLabel_;	// Label used for list. (Def "Values").
					// label used text field. Def. "Enter:"
	char		*textFieldLabel_;
	Widget		ref_;		// Reference widget.
	Widget		dialog;		// Popup dialog widget.
	Widget		selbox;		// Selection list.
	Widget		list;		// List of items.
	Widget		del;		// Delete button.
	Widget		multW;		// Multiplier widget.
	Widget		fractionW;	// "Fractional" button gadget.
	Widget		valueW;		// "Value" button gadget.
	Widget		showContourW;
	Widget		showLabelW;
	/// Radio buttons
	Widget		useSolidLinesW;
	Widget		useDashedLinesW;
	Widget		dashNegativeW;
	// Color toggle information.
	struct	ColorInfo {
		 const char	*name;
		 AvContourInfo::COLORSTYLE color;
		 Widget	widget;
		}	*colorinfo_;
	///
	Widget		drawTitleW;
	Widget		enableBlankingW;
	Widget		immediateW;	// Immediate Update.
	Widget		linewidthW_;
	Widget		emphasisEnabledW_;

	MATCHES		match_;		// can(can't) a new string match.
					// added to the list.
	Boolean		emptyStrings_;	// If FALSE, empty strings are invalid.
	Boolean		numbersOnly_;	// TRUE -> only numbers can be entered.
	SORTING		sorting_;	// How entries should be sorted.
	Boolean		showHelp_;	// TRUE -> show help button.
	Boolean		up_;
	AvContourListCallback	callback_;
	XtPointer	cbdata_;
	Boolean		immediateNotify_;
	double		min_, max_;	// Numbers have to be in this range.
	AvContourInfo	*worldInfo_;	// Info struct passed to the world.
	AvContourInfo	*info_;		// What really gets changed.
	AvContourInfo	*backupInfo_;	// To restore in case of cancel.
	Boolean		toggling_;	// TRUE if changing a toggle that
					// might cause an unwanted callback.
};

#endif
