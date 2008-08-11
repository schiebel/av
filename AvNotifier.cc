//# Copyright (C) 1995-2002 Board of Trustees of the University of Illinois
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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvNotifier.cc,v 19.0 2003/07/16 05:48:02 aips2adm Exp $
//
// $Log: AvNotifier.cc,v $
// Revision 19.0  2003/07/16 05:48:02  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:36  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/30 22:37:25  hravlin
// Copy constructors initialize their base class.
//
// Revision 17.0  2001/11/12 19:43:04  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:11  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:39  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:09:07  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/09/24 21:17:38  hravlin
// Reworked enable/disable Update().
//
// Revision 13.0  1999/08/10 18:40:58  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:24:20  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:02:12  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:56:26  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.2  1998/02/20 20:48:21  hr
// Added StringNotifier.
//
// Revision 9.1  1997/11/06 16:57:40  hr
// Added Integer & Double Array Notifiers.
//
// Revision 9.0  1997/08/25 21:31:40  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:19:27  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.1  1997/02/05 17:47:54  hr
// Notifiers are 'smart' variables that call a conductor when they are
// changed and update the configuration database. They are mostly used
// to communicate between a user interface and what it's controlling.
//
//
//---------------------------------------------------------------------------

/* AvNotifier.cc
'Smart' variables that notify a conductor &/or update a database when
they are changed.
*/

#include <stdio.h>
#include <X11/Intrinsic.h>
#include "AvString.h"
#include "AvNotifier.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
///			Notifiers
////////////////////////////////////////////////////////////////
AvNotifier::AvNotifier()
{
	name_ = "";
	info_ = NULL;
	conductor_ = NULL;
	conductorMask_ = AvConductor::ValueChanged;
	// If noUpdate_ is > 0, update() becomes a nop. Most notifiers don't
	// need it, but some, like IntMapNotifier have to to call their
	// setVar() routine during initialization. They will need to
	// set noUpdate_ to > 0, do the initialization then reset it.
	noUpdate_ = 0;
}

AvNotifier::AvNotifier(const AvNotifier &n)
{
	noUpdate_ = n.noUpdate_;
	name_ = n.name_;
	conductor_ = n.conductor_;
	conductorMask_ = n.conductorMask_;
	info_	= info_;
	if(info_ != NULL)
		info_->ref();
}

AvNotifier::~AvNotifier()
{
	if(info_ != NULL)
		info_->unref();
}

void AvNotifier::enableUpdate(const Boolean callUpdate)
{
	if(noUpdate_ > 0)
	{	noUpdate_ -= 1;
		if((noUpdate_ == 0) && callUpdate)
			update();
	}
}

// Call the callbacks.
void AvNotifier::notify()
{
	if(conductor_ != NULL)
		conductor_->invokeCallbacks(conductorMask_);
}

// Calls notify and updates the database.
void AvNotifier::update()
{
	if(noUpdate_ != 0)	// Object is being initialized.
		return;
	if(info_ != NULL)
		writeValue();
	notify();
}

void AvNotifier::setInfo(AvConfigInfo *info)
{	if(info_ != NULL)
		info_->unref();
	info_ = info;
	if(info_ != NULL)
		info_->ref();
}

void AvNotifier::init_(const char *name, AvConfigInfo *info,
				   AvConductor *conductor)
{
	optionName(name);
	setInfo(info);
	setConductor(conductor);
}

//////////////////////////////////////////////

AvIntegerNotifier::AvIntegerNotifier(const Boolean isBool)
{
	value_ = 0;
	isBool_ = isBool;
}

AvIntegerNotifier::AvIntegerNotifier(const AvIntegerNotifier &n) :
		   AvNotifier(n)
{
	value_ = n.value_;
}

void AvIntegerNotifier::writeValue()
{	if(info_ != NULL)
		if(isBool_)
			info_->setBoolean(optionName(), value_);
		else
			info_->setInt(optionName(), value_);
#ifdef DEBUG
	cout << optionName() << " = " << value_ << endl;
#endif
}

void AvIntegerNotifier::init(const char *name, AvConfigInfo *info,
					  AvConductor *conductor,
					  const int def)
{
	AvNotifier::init_(name, info, conductor);
	if(info != NULL)
	{	if(isBool_)
			info->getBoolean(name, value_, def);
		else
			info->getInt(name, value_, def);
	}
	else
		value_ = def;
}

AvIntegerNotifier::~AvIntegerNotifier(){}

void AvIntegerNotifier::setValue(const int newval, const Boolean Update)
{
	if(value_ != newval)
	{	value_ = newval;
		if(Update)
			update();
	}
}

int AvIntegerNotifier::getValue()const
{	return value_;
}


//////////////////////////////////////////////

AvDoubleNotifier::AvDoubleNotifier()
{
	value_ = 0.0;
}

AvDoubleNotifier::AvDoubleNotifier(const AvDoubleNotifier &n) :
		  AvNotifier(n)
{
	value_ = n.value_;
}

AvDoubleNotifier::~AvDoubleNotifier(){}


void AvDoubleNotifier::writeValue()
{	if(info_ != NULL)
		info_->setDouble(optionName(), value_);

#ifdef DEBUG
	cout << optionName() << " = " << value_ << endl;
#endif
}

void AvDoubleNotifier::init(const char *name, AvConfigInfo *info,
			    AvConductor *conductor, const double def)
{
	AvNotifier::init_(name, info, conductor);
	if(info != NULL)
		info->getDouble(name, value_, def);
	else
		value_ = def;
}

void AvDoubleNotifier::setValue(const double newval, const Boolean Update)
{
	if(value_ != newval)
	{	value_ = newval;
		if(Update)
			update();
	}
}

double AvDoubleNotifier::getValue()const
{	return value_;
}

//////////////////////////////////////////////

AvStringNotifier::AvStringNotifier()
{
	value_ = "";
}

AvStringNotifier::AvStringNotifier(const AvStringNotifier &n) :
		  AvNotifier(n)
{
	value_ = n.value_;
}

AvStringNotifier::~AvStringNotifier(){}


void AvStringNotifier::writeValue()
{	if(info_ != NULL)
		info_->setString(optionName(), value_);

#ifdef DEBUG
	cout << optionName() << " = " << value_ << endl;
#endif
}

void AvStringNotifier::init(const char *name, AvConfigInfo *info,
			    AvConductor *conductor, const AvString &def)
{
	AvNotifier::init_(name, info, conductor);
	if(info != NULL)
		info->getString(name, value_, def);
	else
		value_ = def;
}

void AvStringNotifier::init(const char *name, AvConfigInfo *info,
			    AvConductor *conductor, const char *defstr)
{AvString def;

	if(defstr != NULL)
		def = defstr;
	else
		def = "";
	init(name, info, conductor, def);
}

void AvStringNotifier::setValue(const AvString &newval, const Boolean Update)
{
	if(value_ != newval)
	{	value_ = newval;
		if(Update)
			update();
	}
}

AvString AvStringNotifier::getValue()const
{	return value_;
}

const char* AvStringNotifier::getString()const
{	return value_.chars();
}

//////////////////////////////////////////////

AvIntegerArrayNotifier::AvIntegerArrayNotifier(const int initialLength)
{
	value_.resize(initialLength);
	value_ = 0;
}

AvIntegerArrayNotifier::AvIntegerArrayNotifier(const AvIntegerArrayNotifier &n) :
			AvNotifier(n)
{
	if(value_.nelements() != n.value_.nelements())
		value_.resize(n.value_.nelements());
	value_ = n.value_;
}

void AvIntegerArrayNotifier::writeValue()
{	if(info_ != NULL)
		info_->setIntList(optionName(), value_);
#ifdef DEBUG
	cout << optionName() << " = " << value_ << endl;
#endif
}

void AvIntegerArrayNotifier::init(const char *name, AvConfigInfo *info,
				  AvConductor *conductor,
				  const AvIPosition &def)
{
	AvNotifier::init_(name, info, conductor);
	if(info != NULL)
		info->getIntList(name, value_, def);
	else
		value_ = def;
}

void AvIntegerArrayNotifier::setLength(const int length)
{
	value_.resize(length);
}

void AvIntegerArrayNotifier::init(const char *name, AvConfigInfo *info,
					  AvConductor *conductor,
					  const int def)
{
	AvNotifier::init_(name, info, conductor);
	if(info != NULL)
	{	if(!info->getIntList(name, value_))
			value_ = def;
	}
	else
		value_ = def;
}

AvIntegerArrayNotifier::~AvIntegerArrayNotifier(){}

void AvIntegerArrayNotifier::setValue(const AvIPosition &newval,
				      const Boolean Update)
{
	if(value_ != newval)
	{	value_ = newval;
		if(Update)
			update();
	}
}

AvIPosition AvIntegerArrayNotifier::getValue()const
{	return value_;
}

//////////////////////////////////////////////

AvDoubleArrayNotifier::AvDoubleArrayNotifier(const int initialLength)
{
	value_.resize(initialLength);
	value_ = 0;
}

AvDoubleArrayNotifier::AvDoubleArrayNotifier(const AvDoubleArrayNotifier &n):
		     AvNotifier(n)
{
	if(value_.nelements() != n.value_.nelements())
		value_.resize(n.value_.nelements());
	value_ = n.value_;
}

void AvDoubleArrayNotifier::writeValue()
{	if(info_ != NULL)
		info_->setIntList(optionName(), value_);
#ifdef DEBUG
	cout << optionName() << " = " << value_ << endl;
#endif
}

void AvDoubleArrayNotifier::setLength(const int length)
{
	value_.resize(length);
}

void AvDoubleArrayNotifier::init(const char *name, AvConfigInfo *info,
				  AvConductor *conductor,
				  const AvWPosition &def)
{
	AvNotifier::init_(name, info, conductor);
	if(info != NULL)
		info->getDoubleList(name, value_, def);
	else
		value_ = def;
}

void AvDoubleArrayNotifier::init(const char *name, AvConfigInfo *info,
					  AvConductor *conductor,
					  const double def)
{
	AvNotifier::init_(name, info, conductor);
	if(info != NULL)
	{	if(!info->getDoubleList(name, value_))
			value_ = def;
	}
	else
		value_ = def;
}

AvDoubleArrayNotifier::~AvDoubleArrayNotifier(){}

void AvDoubleArrayNotifier::setValue(const AvWPosition &newval,
				      const Boolean Update)
{
	if(value_ != newval)
	{	value_ = newval;
		if(Update)
			update();
	}
}

AvWPosition AvDoubleArrayNotifier::getValue()const
{	return value_;
}

//////////////////////////////////////////////

AvIntMapNotifier::AvIntMapNotifier()
{
	index_ = 0;
	map_ = NULL;
	mapLen_ = 0;
}

AvIntMapNotifier::AvIntMapNotifier( const AvIntMapNotifier &r) :
		AvNotifier(r)
{
	index_ = r.index_;
	map_ = r.map_;
	mapLen_ = r.mapLen_;
}

void AvIntMapNotifier::init(const char *name, AvConfigInfo *info,
			AvConductor *conductor,
			const IntMap *map, const int mapLen, const int def)
{
	AvNotifier::init_(name, info, conductor);
	setMap(map, mapLen);
	noUpdate_ += 1;	// Turn off updates.
	if(info != NULL)
	{int value;
		info->getMappedInt(name, value, map, def);
		setValue(value);
	}
	else
		setValue(def);
	noUpdate_ -= 1;	// Reenable.
}

void AvIntMapNotifier::writeValue()
{
	if((info_ != NULL)&& !noUpdate_)
		info_->setMappedInt(optionName(), getValue(), map_);

#ifdef DEBUG
	cout << optionName() << " = " << getName() << endl;
#endif
}

// Return value for index'th entry. (Current value if index<0).
int AvIntMapNotifier::getValue(const int index)const
{
	if(validIndex(index))
		return map_[index].value;
	else
		return map_[0].value;
}

const char *AvIntMapNotifier::getName(const int index)const
{
	if(map_ == NULL)
		return NULL;

	if(validIndex(index))
		return map_[index].name;
	else
		return NULL;
}

// Set new index to be the index of name. Ignore if not valid.
void AvIntMapNotifier::setName(const char *name)
{
	int index = nameToIndex(name);
	setIndex(index);
}

void AvIntMapNotifier::setValue(const int value,
					     const Boolean Update)
{
	int index = valueToIndex(value);
	setIndex(index, Update);
}

void AvIntMapNotifier::setIndex(const int newIndex,
					     const Boolean Update)
{
	if(validIndex(newIndex) && (index_ != newIndex))
	{	index_ = newIndex;
		if(Update)
			update();
	}
}

int AvIntMapNotifier::nameToIndex(const char *name)const
{
	if((map_ == NULL) || (mapLen_ <= 0) || (name == NULL))
		return -1;
	for(int i=0; i< mapLen_; i++)
	{const char *n = map_[i].name;
		if(n == NULL)
			continue;
		if(strcmp(n, name) == 0)
			return i;
	}
	return -1;
}

int AvIntMapNotifier::valueToIndex(const int value)const
{
	if((map_ == NULL) || (mapLen_ <= 0))
		return -1;
	for(int i=0; i< mapLen_; i++)
	{	if(value == map_[i].value)
			return i;
	}
	return -1;
}

void AvIntMapNotifier::setMap(const IntMap *map, const int n)
{
	map_ = map;
	int l = getMapLength(map);
	if( (n > 0) && (n<=l))
		mapLen_ = n;
	else
		mapLen_ = l;

}

int AvIntMapNotifier::getMapLength(const IntMap *map)
{
	if(map == NULL)
		return 0;
	int i=0;
	while( map[i].name != NULL) i++;
	return i;
}

int AvIntMapNotifier::mapLength()const
{
	return getMapLength(map_);
}


////////////////////////////////////////////////////////////////

AvRadioTextNotifier::AvRadioTextNotifier()
{
	defaultValue_	= 0;
	userValue_ = 0;
	defaultString_	= "";
	userString_	= "";
}

// Set the user string to the new value. Force current value to be
// userValue.
void AvRadioTextNotifier::setString(const char *newval)
{ Boolean needNotify = (userString_ != newval);

	userString_ = newval;
	if(getValue() != userValue_)
		setValue(userValue_);
	else
	if(needNotify)
		update();	// Text changed, but not current value.
}


// Return the user's string unless current value is defaultValue.
const char *AvRadioTextNotifier::getString()const
{
	if( getValue() == defaultValue_)
		return defaultString_.chars();
	else
		return userString_.chars();
}

// Initialize a string value w/o notifying.
void AvRadioTextNotifier::initialString(const char *string,
						     const Boolean def)
{
	if(def)
		defaultString_ = string;
	else
		userString_ = string;
}

void AvRadioTextNotifier::init(const char *name,
			  AvConfigInfo *info, AvConductor *cond,
			  const IntMap *map, const int mapLen,
			  const int defaultValue, const int userValue,
			  const char *defText,
			  const int initialValue)

{
	AvIntMapNotifier::init(name, info, cond, map, mapLen, initialValue);
	userValue_ = userValue;
	defaultValue_ = defaultValue;
	defaultString_ = defText;
	if(info != NULL)
		info->getString(name, userString_, "");
	else
		userString_ = "";
}

// "Dummy" init routine.
void AvRadioTextNotifier::init(const char *name,
			  AvConfigInfo *info, AvConductor *cond,
			  const IntMap *map, const int mapLen,
			  const int initialValue)
{
	init(name, info, cond, map, mapLen, 0, 0, "", initialValue);
}

void AvRadioTextNotifier::writeValue()
{
	if(info_ != NULL)
		info_->setString(optionName(), userString_);
#ifdef DEBUG
	cout << "AvRadioTextNotifier::writeValue: ";
	cout << optionName() << " = " << userString_ << endl;
	flush(cout);
#endif
}
