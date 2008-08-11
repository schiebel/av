//# Copyright (C) 1995-99 Board of Trustees of the University of Illinois
//#
//# This software, both binary and source, is copyrighted by The
//# Board of Trustees of the University of Illinois. Ownership
//# remains with the University. You should have received a copy 
//# of a licensing agreement with this software. See the file 
//# "AIPSVIEW_COPYRIGHT", or contact the University at this address:
//#
//# The NCSA AipsView Visualization System
//# National Center for Supercomputing Applications
//# University of Illinois
//# 405 North Mathews Ave.
//# Urbana, IL 61801
//# aipsview@ncsa.uiuc.edu
//# --------------------------------------------------
//
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvNotifier.h,v 19.0 2003/07/16 05:47:58 aips2adm Exp $
//
// $Log: AvNotifier.h,v $
// Revision 19.0  2003/07/16 05:47:58  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:33  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:43:01  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:07  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:32  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:09:04  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/09/24 21:17:51  hravlin
// Reworked enable/disable Update().
//
// Revision 13.0  1999/08/10 18:40:54  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:13  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:02:01  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:56:13  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.3  1998/02/20 20:48:37  hr
// Added StringNotifier.
//
// Revision 9.2  1997/11/12 21:24:48  hr
// Added defaultString() & userString() functions.
//
// Revision 9.1  1997/11/06 16:58:21  hr
// Added Integer & Double Array Notifiers.
//
// Revision 9.0  1997/08/25 21:31:21  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.1  1997/04/25 16:07:57  hr
// Added AvBooleanNotifier.
//
// Revision 8.0  1997/02/20 03:19:16  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.2  1997/02/09 00:13:35  hr
// *** empty log message ***
//
// Revision 7.1  1997/02/05 17:46:44  hr
// Notifiers are 'smart' variables that call a conductor when they are
// changed and update the configuration database. They are mostly used
// to communicate between a user interface and what it's controlling.
//
//
//---------------------------------------------------------------------------
/*

 A notifier is something like a smart variable. Whenever it gets set, it
notifies a conductor and an AvConfigInfo object. Note that most of the
initialization has to be done by calling the object's init routine. The
update argument in the subclass' setVar routine can also be used to prevent
updating.

The primary use of notifiers is to hold variables set by a user interface.
Since they call their conductor each time they are changed, the display
can be automatically updated. Since they also update the configuration
database, it is always consistent.

*/

#ifndef AvNotifier_h
#define AvNotifier_h
#include "AvResource.h"
#include "AvConductor.h"
#include "AvConfigInfo.h"
#include "AvIPosition.h"
#include "AvWPosition.h"

// Base class
class AvNotifier {
  public:
	AvNotifier();
	AvNotifier(const AvNotifier &);
	virtual ~AvNotifier();
	AvConductor *conductor()const{return conductor_;}
	void setConductor(AvConductor *c){conductor_ = c;}
	void setConductorMask(const int mask){conductorMask_ = mask;}
	int getConductorMask()const{return conductorMask_;}
	void setInfo(AvConfigInfo *info);
	AvConfigInfo *getInfo()const{return info_;}
	const char *optionName()const{return name_.chars();}
	void optionName(const char *name){name_ = name;}
	virtual void writeValue()=0;
	// Reenable updates. If noUpdate_ goes to 0, update() is called
	// unless update=False;
	void enableUpdate(const Boolean update=False);
	void disableUpdate(){noUpdate_  += 1;}
	// Calls the callbacks. Normally called only internally.
	void	notify();
  protected:
	virtual void init_(const char *name, AvConfigInfo *,
			   AvConductor *);
	// Calls the callbacks and updates the database.
	void	update();
  protected:
	AvString	name_;	// Used for reading/writing info_.
	AvConfigInfo	*info_;
	AvConductor	*conductor_;
	int		conductorMask_;
	int		noUpdate_; // To keep init from notifying.
};

//////////////////////////////////////////////

// Holds an integer.
class AvIntegerNotifier : public AvNotifier {
  public:
	// If isBool is TRUE, when a value is written to info_,
	// it will be "TRUE" or "FALSE" rather than the integer's value
	AvIntegerNotifier(const Boolean isBool=FALSE);
	AvIntegerNotifier(const AvIntegerNotifier &);
	virtual ~AvIntegerNotifier();
	virtual void setValue(const int newval,
			      const Boolean update=TRUE);
	virtual int getValue()const;
	virtual void writeValue();
	virtual void init(const char *name, AvConfigInfo *,
			  AvConductor *,
			  const int def=0);
  private:
	int	value_;
	Boolean	isBool_;
};

class AvBooleanNotifier : public AvIntegerNotifier {
  public:
	AvBooleanNotifier() : AvIntegerNotifier(TRUE) {}
	virtual ~AvBooleanNotifier() {};
};

//////////////////////////////////////////////
// These are frequently used rather than IntegerNotifiers so
// AvConfigInfo can pass all slider events to one callback.
class AvDoubleNotifier : public AvNotifier {
  public:
	AvDoubleNotifier();
	AvDoubleNotifier(const AvDoubleNotifier &);
	virtual ~AvDoubleNotifier();
	virtual void setValue(  const double newval,
				const Boolean update=TRUE);
	virtual double getValue()const;
	virtual void init(const char *name, AvConfigInfo *,
			  AvConductor *,
			  const double def=0);
  private:
	virtual void writeValue();
	double	 value_;
};
//////////////////////////////////////////////
class AvStringNotifier : public AvNotifier {
  public:
	AvStringNotifier();
	AvStringNotifier(const AvStringNotifier &);
	virtual ~AvStringNotifier();
	virtual void setValue(  const AvString &newval,
				const Boolean update=TRUE);
	virtual AvString getValue()const;
	virtual const char *getString()const;
	virtual void init(const char *name, AvConfigInfo *,
			  AvConductor *,
			  const AvString &def);
	virtual void init(const char *name, AvConfigInfo *,
			  AvConductor *,
			  const char *def=NULL);
  private:
	virtual void writeValue();
	AvString value_;
};
//////////////////////////////////////////////

// Holds an array of integers.
class AvIntegerArrayNotifier : public AvNotifier {
  public:
	AvIntegerArrayNotifier(const int initialLength=1);
	AvIntegerArrayNotifier(const AvIntegerArrayNotifier &);
	virtual ~AvIntegerArrayNotifier();
	virtual void setValue(const AvIPosition &newval,
			      const Boolean update=TRUE);
	virtual AvIPosition getValue()const;
	virtual void writeValue();
	// Reset length of array. Does no notification or write updates.
	void setLength(const int length);
	virtual void init(const char *name, AvConfigInfo *,
			  AvConductor *,
			  const int def=0);
	virtual void init(const char *name, AvConfigInfo *,
			  AvConductor *,
			  const AvIPosition &def);
  private:
	AvIPosition	value_;
};

// Holds an array of doubles
class AvDoubleArrayNotifier : public AvNotifier {
  public:
	AvDoubleArrayNotifier(const int initialLength=1);
	AvDoubleArrayNotifier(const AvDoubleArrayNotifier &);
	virtual ~AvDoubleArrayNotifier();
	virtual void setValue(const AvWPosition &newval,
			      const Boolean update=TRUE);
	virtual AvWPosition getValue()const;
	virtual void writeValue();
	void setLength(const int length);
	virtual void init(const char *name, AvConfigInfo *,
			  AvConductor *,
			  const double def=0.0);
	virtual void init(const char *name, AvConfigInfo *,
			  AvConductor *,
			  const AvWPosition &def);
  private:
	AvWPosition	value_;
};

typedef AvConfigInfo::IntMap IntMap;

/*
 A notifier that can choose between entries in an IntMap list.  Typical use
would be to call setName(<string>) which would set the value to be that of
the entry with name <string>.  (Used with radio pushbuttons. Each button's
label is the name.  When getValue() is called the value associated with the
name is returned).

IntMaps must be NULL terminated. If ( 0<n< the number of non NULL entries),
only n entries will be used.

*/
class AvIntMapNotifier : public AvNotifier {
  public:
	AvIntMapNotifier();
	AvIntMapNotifier(const AvIntMapNotifier &);
	// Return value for index'th entry. (Current value if index<0).
	int getValue(const int index)const;
	int getValue()const {return getValue(index_);}
	// Set index to be entry with value of n.
	virtual void setValue(const int n, const Boolean update=TRUE);
	// Return pointer to name with index. (Current name if index<0)
	const char *getName(const int index)const;
	const char *getName()const { return getName(index_);}
	// Set the index to be the index of name.
	// Ignore if name is not in the list.
	void setName(const char *name);

	// Set Current index. Ignore if not valid.
	void setIndex(const int newIndex, const Boolean update=TRUE);
	int getIndex()const{return index_;}

	// Return index of name or -1.
	int nameToIndex(const char *name)const;
	// Return index of value or -1.
	int valueToIndex(const int value)const;

	inline int validIndex(const int i)const
		{return ((i>=0) && (i < mapLen_)) ? 1 : 0;}
	void setMap(const IntMap *map,const int n=0);
	int mapLength()const;
	const IntMap *getMap()const{return map_;}
	static int getMapLength(const IntMap *);
	virtual void writeValue();
	virtual void init(const char *name, AvConfigInfo *,
			  AvConductor *,
			  const IntMap *map, const int mapLen,
			  const int def=0);
  private:
	const	IntMap *map_;
	int	mapLen_;
	int	index_;
};

////////////////////////////////////////////////////////////////
/* An IntMapNotifier that has an associated text field. get/set
   String return either a user set text field or a supplied default
   text field.
   defaultValue - If current value is this, getString returns
		  default string otherwise, it returns user's string.
   userValue - When setString() is called, current value is set to
		this.
   This is used with the X, Y and Title labels.
*/

class AvRadioTextNotifier : public AvIntMapNotifier {
  public:
	AvRadioTextNotifier();
	virtual void init(const char *name,
		  AvConfigInfo *, AvConductor *,
		  const IntMap *, const int mapLen,
		  const int defaultValue, const int userValue,
		  const char *defText,
		  const int initialValue=0);

	// Set string and set current value to userValue.
	virtual void setString(const char *newval);
	// Get user's text unless current value is defaultValue.
	virtual const char *getString()const;
	// Initialize a string value w/o notifying.
	void initialString(const char *string,
			   const Boolean def=FALSE);
	virtual void writeValue();
	int userValue()const{return userValue_;}
	int defaultValue()const{return defaultValue_;}
	// Return default or user strings.
	const char *defaultString()const{return defaultString_.chars();}
	const char *userString()const{return userString_.chars();}
  private:
	// This only exists to make the compiler happy.
	virtual void init(const char *name,
		  AvConfigInfo *, AvConductor *,
		  const IntMap *, const int mapLen,
		  const int initialValue=0);
		int		defaultValue_;
	AvString	defaultString_;
	int		userValue_;
	AvString	userString_;
};

#endif
