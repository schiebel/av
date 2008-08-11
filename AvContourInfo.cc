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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvContourInfo.cc,v 19.0 2003/07/16 05:47:04 aips2adm Exp $
//
// $Log: AvContourInfo.cc,v $
// Revision 19.0  2003/07/16 05:47:04  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:47  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/30 22:34:41  hravlin
// Ensured switch stmnts always initialize variables.
//
// Revision 17.0  2001/11/12 19:42:16  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:24  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:09:45  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.1  2000/07/18 16:58:57  hravlin
// Update copyright.
//
// Revision 14.0  2000/03/23 16:08:07  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/08/25 19:54:58  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:39:54  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:22:27  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:59:29  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:53:16  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.2  1997/09/17 14:42:23  hr
// Added support for emphasized contours.
//
// Revision 9.1  1997/09/15 16:29:58  hr
// Added support for changing line width.
//
// Revision 9.0  1997/08/25 21:26:45  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:16:17  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1997/01/24 20:20:52  hr
// Changes to allow writing into the defaults database.
//
// Revision 7.2  1996/12/12 09:15:31  droberts
// Added inclusion of stdio.h.
//
// Revision 1.4  1996/11/11 15:51:57  hr
// Added an initial value of zero to AvWPosition levels definition.
//
// Revision 1.3  1996/11/05 22:08:45  hr
// Changed to use int arg to getBoolean().
//
// Revision 1.2  1996/11/05  21:12:07  hr
// Several changes to begin using AvConfigInfo.
//
// Revision 1.1  1996/07/11 21:31:21  pixton
// Automated Checkin
//
//  Revision 1.7  1996/06/18  18:45:56  pixton
//  Copyright Update
//
//  Revision 1.6  1996/03/28  22:35:07  hr
//  Added operator= and copy() to LevelInfo to get rid of memory checker
//  complaints.
//
//  Revision 1.5  1996/03/20  17:07:43  hr
//  Removed an unused variable.
//
//  Revision 1.4  1996/03/05  17:39:32  hr
//  Needed to exchange B/W indices after PGDriver changed default background.
//  checkLevelArray now initializes the color index when extending the table.
//  Colors went from 1 to 16 rather than 1 to 15.
//
//  Revision 1.3  1996/02/01  18:03:16  hr
//  setContourLevels wasn't setting ncontours_ to 0 if the new length was 0.
//
//  Revision 1.2  1996/01/24  22:15:06  hr
//  Extended COLORSTYLE.
//
//  Revision 1.1  1996/01/24  18:05:08  hr
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvContourInfo.cc
Holds information describing how to draw contours such as the
contour levels, colors, line type, etc.
*/

#include "AvContourInfo.h"
#include "AvConfigInfo.h"

#include <stdio.h>

// TCL Variable name for contour info.
static const char *CONTOUR = "contour";

// Linestyles
static const int SOLIDLINE=1;	// PGPLOT definitions.
static const int DASHEDLINE=2;


// Default string used to format contour labels.
static const char *DEFAULTFORMATSTRING = "%.4g";

// Holds information for one contour level.
class LevelInfo {
  public:
	LevelInfo();
	LevelInfo(const LevelInfo &);
	~LevelInfo();
	LevelInfo& operator=(const LevelInfo &);
	void copy(const LevelInfo &from, LevelInfo &to);
	int operator==(const LevelInfo &)const;
	int operator!=(const LevelInfo &)const;
	double value()const {return value_;}
	void value(const double v) {value_ = v;isSet.value=1;}
	int colorIndex()const{return colorIndex_;}
	void colorIndex(const int ci){ colorIndex_ = ci; isSet.ci=1;}
	int lineStyle()const{return lineStyle_; }
	void lineStyle(const int ls){lineStyle_ = ls; isSet.ls=1;}
	int lineWidth()const{return lineWidth_; }
	void lineWidth(const int ls){lineWidth_ = ls; isSet.lw=1;}
	void getSpacing(int &minint, int &intval)const
			{minint = minint_;intval=intval_;}
	void setSpacing(const int minint, const int intval)
			{minint_ = minint;intval_=intval;isSet.spc=1;}
	// Have the values been set?
	Boolean haveValue()const {return (isSet.value==1) ? TRUE : FALSE;}
	Boolean haveColorIndex()const {return (isSet.ci==1) ? TRUE : FALSE;}
	Boolean haveLinestyle()const {return (isSet.ls==1) ? TRUE : FALSE;}
	Boolean haveLinewidth()const {return (isSet.lw==1) ? TRUE : FALSE;}
	Boolean haveSpacing()const {return (isSet.spc==1) ? TRUE : FALSE;}
	Boolean equals(const LevelInfo &i)const;
  private:
	AvConfigInfo	*info()const{return info_;}
	AvConfigInfo	*info_;
	double	value_;		// Contour level value.
	int	colorIndex_;
	int	lineStyle_;
	int	lineWidth_;
	int	minint_;	// Label spacing info.
	int	intval_;
	struct	 {
	 unsigned	value:1;
	 unsigned	ci:1;
	 unsigned	ls:1;
	 unsigned	lw:1;
	 unsigned	spc:1;
	 unsigned	jnk:27;
	} isSet;
};

LevelInfo::LevelInfo()
{
	value_ = 0.0;
	colorIndex_ = 0;
	lineStyle_ = 0;
	lineWidth_ = 2;
	minint_ = 0;
	intval_ = 0;
	isSet.value = isSet.ci = isSet.ls = isSet.spc = 0;
	isSet.lw = 0;
	isSet.jnk = 0;	// For memory checker.
}

LevelInfo::LevelInfo(const LevelInfo &a)
{
	copy(a, *this);
}

LevelInfo& LevelInfo::operator=(const LevelInfo &a)
{
	copy(a, *this);
	return *this;
}

void LevelInfo::copy(const LevelInfo &from, LevelInfo &to)
{
	to.value_ = from.value_;
	to.colorIndex_ = from.colorIndex_;
	to.lineStyle_ = from.lineStyle_;
	to.lineWidth_ = from.lineWidth_;
	to.minint_ = from.minint_;
	to.intval_ = from.intval_;
	to.isSet = from.isSet;
}

LevelInfo::~LevelInfo()
{
}

int LevelInfo::operator==(const LevelInfo &i)const
{
	return equals(i);
}

int LevelInfo::operator!=(const LevelInfo &i)const
{
	return !equals(i);
}

Boolean LevelInfo::equals(const LevelInfo &i)const
{
	if( (value_ == i.value_) && (colorIndex_ == i.colorIndex_)
	   && (lineStyle_ == i.lineStyle_) && (minint_ == i.minint_)
	   && (intval_ == i.intval_) && (haveValue() == i.haveValue())
	   && (haveColorIndex() == i.haveColorIndex())
	   && (haveLinestyle() == i.haveLinestyle())
	   && (haveLinewidth() == i.haveLinewidth())
	   && (haveSpacing() == i.haveSpacing()))
		return TRUE;
	else
		return FALSE;
}



////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
static unsigned long _timestamp = 1;
static inline unsigned long TimeStamp()
//static unsigned long TimeStamp()
{	return _timestamp++;}

// TCL script to initialize defaults.
static const char *defaultScript = \
	"set aipsview(contour,drawContours) TRUE\n\
	set aipsview(contour,levelsAreFractional) FALSE\n\
	set aipsview(contour,lineStyle) DASHEDifNEGATIVE\n\
	set aipsview(contour,lineWidth) 2\n\
	set aipsview(contour,colorStyle) MIXED\n\
	set aipsview(contour,labelContours) FALSE\n\
	set aipsview(contour,drawTitle) FALSE\n\
	set aipsview(contour,blankingEnabled) TRUE\n\
	set aipsview(contour,immediateUpdate) FALSE\n\
	set aipsview(contour,levelScale) 1.0\n\
	set aipsview(contour,nlevels) 4\n\
	set aipsview(contour,levels) { .1 .3 .5 .7 .9 }\n\
	set aipsview(contour,lineStyles) DASHED \n\
	set aipsview(contour,emphasisEnabled) FALSE \n\
	set aipsview(contour,emphasisColor) BLACK \n\
	";

// Table to convert between color names and their integer values.
AvConfigInfo::IntMap colorStyles[] = {
	{ AvContourInfo::WHITE, "WHITE"},
	{ AvContourInfo::BLACK, "BLACK"},
	{ AvContourInfo::RED, "RED"},
	{ AvContourInfo::GREEN, "GREEN"},
	{ AvContourInfo::BLUE, "BLUE"},
	{ AvContourInfo::CYAN, "CYAN"},
	{ AvContourInfo::MAGENTA, "MAGENTA"},
	{ AvContourInfo::YELLOW, "YELLOW"},
	{ AvContourInfo::MIXED, "MIXED"},
	{0, NULL}
	};

// Table to convert between line styles and their integer values.
AvConfigInfo::IntMap lineStyles[] = {
	{ AvContourInfo::SOLID, "SOLID"},
	{ AvContourInfo::DASHED, "DASHED"},
	{ AvContourInfo::DASHEDifNEGATIVE, " DASHEDifNEGATIVE"},
	{ 0, NULL}
	};

AvContourInfo::AvContourInfo(const char *fileName, const int nlevels)
{
	init(fileName, nlevels);
}

AvContourInfo::AvContourInfo(const AvContourInfo &a) :
		 AvResource(), AvConductor()
{
	copy(a, *this);
}

void AvContourInfo::init(const char *filename, const int nLevels)
{
	cnfg_ = new AvConfigInfo(filename, CONTOUR, defaultScript);
	cnfg_->ref();
	int val;
	cnfg_->getBoolean("drawContours", val, TRUE);
	drawContours_ = val;
	cnfg_->getBoolean("levelsAreFractional", val, FALSE);
	levelsAreFractional_ = val;
	// Rather than explicitly type in the available color names, we
	// let ConfigInfo generate a table.
	{AvIPosition clrs(7);
		generateColors(clrs);
		int rtn = cnfg_->loadVarDefaults(" set ", cnfg_->defaultVar(),
			"(contour,colorStyles) ",
			AvConfigInfo::buildMappedIntList(clrs, colorStyles));
#if defined(TCL)
		if(!rtn)
		{	cerr <<
			    "AvContourInfo::init: Bad colorStyle script.\n";
			cerr << cnfg_->defaultInterp().result() << endl;
		}
#endif
	}
	cnfg_->getMappedInt("lineStyle", val, lineStyles, DASHEDifNEGATIVE);
	lineStyle_ = (LINESTYLE) val;
	cnfg_->getInt("lineWidth", lineWidth_, 2);
	cnfg_->getMappedInt("colorStyle", val, colorStyles, MIXED);
	colorStyle_ = (COLORSTYLE) val;
	cnfg_->getBoolean("labelContours", val, FALSE);
	labelContours_ = val;
	cnfg_->getBoolean("drawTitle", val, FALSE);
	drawTitle_ = val;
	cnfg_->getBoolean("blankingEnabled", val, TRUE);
	blankingEnabled_ = val;
	fmt_ = NULL;
	setFormatString(DEFAULTFORMATSTRING);
	cnfg_->getMappedInt("emphasisColor", val, colorStyles, BLACK);
	emphasisColor_ = (COLORSTYLE) val;
	cnfg_->getBoolean("emphasisEnabled", val, FALSE);
	emphasisEnabled_ = val;
	// Unused
//	cnfg_->getInt("emphasisOffset", emphasisOffset_, 4);
#if 0
cout << "Offset = " << emphasisOffset_  << " Color = " << emphasisColor_
     << endl;
#endif
	// Used to generate some of the default values.
	min_ = 0.0;
	max_ = 1.0;
	cnfg_->getDouble("levelScale", levelScale_, 1.0);// Level multiplier.
	infolen_ = ncontours_ = 0;
	info_ = NULL;
	cnfg_->getBoolean("immediateUpdate", val, FALSE);
	immediateUpdate_ = val;

	int nlevels;
	// Ask for the number of levels to use.
	cnfg_->getInt("nlevels", nlevels, nLevels);
	AvWPosition levels(nlevels, 0);
	// If a level array exists, it overrides nlevels.
	if(cnfg_->getDoubleList("levels", levels))
		nlevels = levels.nelements();
	else
		generateContourLevels(levels, nlevels);

	// Do these after above.
	setContourLevels(levels, FALSE); // Save, but not back to interpreter.

	// Set line colors and styles.
	AvIPosition lvlcolors(nlevels,0), lvlstyles(nlevels, 0);
	if(!cnfg_->getMappedIntList("colorStyles", lvlcolors, colorStyles))
	{	generateColors(lvlcolors);
		setContourColors(lvlcolors, TRUE);
	}
	else
		setContourColors(lvlcolors, FALSE);

	if(!cnfg_->getMappedIntList("lineStyles", lvlstyles, lineStyles))
	{	generateLineStyles(lvlstyles);
		setLineStyles(lvlstyles, TRUE);
	}
	else
		setLineStyles(lvlstyles, FALSE);

	timestamp_ = TimeStamp();
}

AvContourInfo::~AvContourInfo()
{
	delete [] info_;
	delete [] fmt_;
	AvResource::UnRef(cnfg_);
}

static char *Strdup(const char *str);

// Copy from one to another. The target is assumed to already have been
// initialized.
void AvContourInfo::copy(const AvContourInfo &from, AvContourInfo &to,
			const Boolean update)
{
	if(&to == &from)
	{	fprintf(stderr, "AvContourInfo::copy: to = from\n");
		return;
	}
	to.checkLevelArray(from.ncontours_);
	to.ncontours_ = from.ncontours_;
	for(int i=0; i< from.ncontours_; i++)
		to.info_[i] = from.info_[i];

	to.levelScale_ = from.levelScale_;
	to.drawContours_ = from.drawContours_;
	to.levelsAreFractional_ = from.levelsAreFractional_;
	to.lineStyle_ = from.lineStyle_;
	to.lineWidth_ = from.lineWidth_;
	to.colorStyle_ = from.colorStyle_;
	to.emphasisEnabled_ = from.emphasisEnabled_;
	to.emphasisColor_ = from.emphasisColor_;
//	to.emphasisOffset_ = from.emphasisOffset_;
	to.labelContours_ = from.labelContours_;
	to.drawTitle_ = from.drawTitle_;
	to.blankingEnabled_ = from.blankingEnabled_;
	to.fmt_ = Strdup(from.fmt_);
	to.min_ = from.min_;
	to.max_ = from.max_;
	to.immediateUpdate_ = from.immediateUpdate_;
	if(update)
		to.touch();
	else
		to.timestamp_ = from.timestamp_;
}

// Compares the two ContourInfo objects excluding timestamps.
Boolean AvContourInfo::equals(const AvContourInfo &i)const
{
	if(ncontours_ != i.ncontours_)
		return FALSE;
	for(int j=0; j< ncontours_; j++)
		if(info_[j] != i.info_[j])
			return FALSE;

	if( levelScale_ != i.levelScale_)
		return FALSE;
	if( drawContours_ != i.drawContours_)
		return FALSE;
	if( levelsAreFractional_ != i.levelsAreFractional_)
		return FALSE;
	if( lineStyle_ != i.lineStyle_)
		return FALSE;
	if( lineWidth_ != i.lineWidth_)
		return FALSE;
	if( colorStyle_ != i.colorStyle_)
		return FALSE;
	if( labelContours_ != i.labelContours_)
		return FALSE;
	if( drawTitle_ != i.drawTitle_)
		return FALSE;
	if( blankingEnabled_ != i.blankingEnabled_)
		return FALSE;
	if( emphasisEnabled_ != i.emphasisEnabled_)
		return FALSE;
	if( emphasisColor_ != i.emphasisColor_)
		return FALSE;
#if 0
	if( emphasisOffset_ != i.emphasisOffset_)
		return FALSE;
#endif
	if(strcmp(fmt_, i.fmt_) != 0)
		return FALSE;
	if( min_ != i.min_)
		return FALSE;
	if( max_ != i.max_)
		return FALSE;

	return TRUE;
}

// Make sure the level array has at least numLevels entries.
// If the array length is increased, the old array is copied into it and
// the new color indexes are setup for "mixed".
void AvContourInfo::checkLevelArray(const int numLevels)
{
	if(infolen_ < numLevels)
	{int i;
		LevelInfo *info = new LevelInfo[numLevels];
		for(i=0; i< infolen_; i++)
			info[i] = info_[i];
		// Initialize any new entries.
		for(i=infolen_; i< numLevels; i++)
		{	info[i].colorIndex((i%14)+1);
		}

		infolen_ = numLevels;
		delete [] info_;
		info_ = info;
	}
}

// Generate contour levels. If numlevels > 0, force levels to be that
// long. Otherwise, use whatever length levels is.
void AvContourInfo::generateContourLevels(AvWPosition &levels,
					  const int numlevels)
					  
{
double	delta, Min, Max;
int	numLevels = numlevels;

	if(numLevels > 0)
		levels.resize(numLevels);
	else
		numLevels = levels.nelements();

        if(levelsAreFractional())
        {       Min = 0.0;
                Max = 1.0;
        }
        else
        {       Min = min();
                Max = max();
        }

	if(numLevels > 1)
		delta = (Max - Min)/(numLevels -1);
	else
	{	delta = 0.0;
		Min = (Max-Min)/2;
	}

	for(int i=0; i< numLevels ; i++, Min += delta)
		levels(i) = Min;
}

// Setup the contour level array by copying the supplied values or
// creating the requested number of levels.
void AvContourInfo::setContourLevels(const AvWPosition &levels,
				     const Boolean save)
{int numLevels = levels.nelements();

	checkLevelArray(numLevels);		// Space check

	for(int i=0; i< numLevels; i++)
		info_[i].value(levels(i));
		ncontours_ = numLevels;

	if(save)
		setDL("levels", levels);

	touch();			// Need to regenerate contours.
}

// Return an array of numContourLevels() unscaled contour levels.
// If levels is not NULL, it will be used, otherwise an array will be
// allocated. (Use delete [] to free).
double *AvContourInfo::getContourLevels(double *levels)
{
int	numLevels = numContourLevels();

	if(numLevels <= 0)
		return NULL;

	if(levels == NULL)			// Copy.
		levels = new double[numLevels];

	for(int i=0; i< numLevels; i++)
		levels[i] = info_[i].value();
	return levels;
}

void AvContourInfo::scale(const double mult, const Boolean def)
{
	if(levelScale_ != mult)
	{	levelScale_ = mult;
		setD("levelScale", mult, def);
		touch();
	}
}

// Return the contour value for the level.
double AvContourInfo::getContourValue(const int level)const
{
float value;

	if(!validIndex(level))
		value = 0.0;	// Need to return something.
	else
		value = info_[level].value()*scale();

	// Convert from fractional to real values if necessary.
	if(levelsAreFractional())
	{ double range = max() - min();
		value = value*range + min();
	}

	return value;
}

/* Generate color entries to fill colors.
There is a maximum of 15 colors (1..15) since PGPLOT defines 15 non-blank
colors.
*/
void AvContourInfo::generateColors(AvIPosition &colors)const
{
int	i;
int	numColors;

	numColors = colors.nelements();
	for(i=0; i< numColors; i++)
		colors(i) = ((i % 14) + 1);	// Wrap around.
}

/* Copy the color array. Does not change the number of contour levels.
*/
void AvContourInfo::setContourColors(const AvIPosition &colors,
				     const Boolean write)
{
int	i;
int	numColors, nlevels = numContourLevels();

	numColors = colors.nelements();
	for(i=0; i< nlevels; i++)
		if(i < numColors)
			info_[i].colorIndex(colors(i));
		else
			info_[i].colorIndex((i % 14) + 1);	// Wrap around.
	if(write && (cnfg_ != NULL))
		cnfg_->setMappedIntList("contourColors", colors,
					colorStyles);
		// (This will probably never happen since there is no way
		// for the user to set individual colors).
	touch();
}

// Return the color index to use for the level.
// The integers are from looking at the PGPLOT color table
// ("Figure 5.1" from the PGPLOT manual).
// (However, B & W are exchanged since it works better and matches
//  the way the X window driver is configured).
int AvContourInfo::getContourColor(const int level)const
{
int ci;

	switch(colorStyle_) {
	case BLACK:
		ci = 0;
		break;
	case WHITE:
		ci = 1;
		break;
	case RED:
		ci = 2;
		break;
	case GREEN:
		ci = 3;
		break;
	case BLUE:
		ci = 4;
		break;
	case CYAN:
		ci = 5;
		break;
	case MAGENTA:
		ci = 6;
		break;
	case YELLOW:
		ci = 7;
		break;
	case MIXED:
		if(!validIndex(level))
			ci = 1;
		else
			ci = info_[level].colorIndex();
		break;
	default:
		ci = 0;
	}
	return ci;
}

/* Copy the linestyle array if non NULL. If NULL, generate numLines entries.
PGPLOT defines 5 different linestyles;

numLines = 0	Use Solid lines for all contours.
numLines > 0	Use numLines linestyles. (There is a maximum of 5 different
		linestyles).
*/

void AvContourInfo::setLineStyles(const AvIPosition &lines,
				  const Boolean write)
{
int	i, nlevels = numContourLevels();
int	nstyles = lines.nelements();

	for(i=0; i< nlevels; i++)
		if(i < nstyles)
			info_[i].lineStyle(lines(i));
		else	// Generate linestyles.
			info_[i].lineStyle((i % 5) + 1); // (Wrap around).
	if(write && (cnfg_ != NULL))
		cnfg_->setMappedIntList("lineStyles", lines, lineStyles);
	touch();
}

// For "DASHED" line styles, cycle through the various PGPLOT values.
// (Including SOLID).
void AvContourInfo::generateLineStyles(AvIPosition &styles)const
{
int	i, numLines = styles.nelements();

	for(i=0; i< numLines; i++)
		styles(i) = ((i % 5) + 1);	// (Wrap around).
}

/* Return the linestyle to use for the contour level.
	SOLID			All lines are solid.
	DASHED			Use linestyle in info array.
	DASHEDifNEGATIVE	Dash if contour is < 0, else solid.
*/

int AvContourInfo::getLinestyle(const int level)const
{
int	ls;

	if(!validIndex(level))
		return SOLIDLINE;

	switch(lineStyle_) {
	case DASHED:
		if(info_[level].haveLinestyle())
			ls = info_[level].lineStyle();
		else
			ls = SOLIDLINE;		// Default.
		break;
	case DASHEDifNEGATIVE:
		{double	value = getContourValue(level);
			if(value >= 0.0)
				ls = SOLIDLINE;
			else
				ls = DASHEDLINE;
		}
		break;
	default:
	case SOLID:
		ls = SOLIDLINE;
		break;
	}
	return ls;
}

int AvContourInfo::getLinewidth(const int level)const
{ int lw;
	if(!validIndex(level))
		lw = 2;
	else
#if 0
	if(info_[level].haveLinewidth())
		lw = info_[level].lineWidth();
	else
		lw = 2;
#else
		lw = lineWidth_;
#endif
	return lw;
}


// Should contour labels be drawn?
void AvContourInfo::labelContours(const Boolean label)
{	if(label != labelContours_)
	{	labelContours_ = label;
		setB("labelContours", label);
		touch();
	}
}

static char *Strdup(const char *str)
{
int	len;
char	*s;

	if(str == NULL)
		return NULL;

	len = (int)strlen(str);
	s = new char[len+1];
	strcpy(s, str);

	return s;
}

// Change the string used to format contour labels.
void AvContourInfo::setFormatString(const char *fmt)
{
	delete [] fmt_;
	if(fmt == NULL)
		fmt_ = Strdup(DEFAULTFORMATSTRING);
	else
		fmt_ = Strdup(fmt);
	if(labelContours())
		touch();
}


// Change whether to draw contours.
void AvContourInfo::drawContours(const Boolean draw)
{	if(draw != drawContours_)
	{	drawContours_ = draw;
		setB("drawContours", draw);
		touch();
	}
}

// Whether to draw a simple title.
void AvContourInfo::drawTitle(const Boolean draw)
{	if(draw != drawTitle_)
	{	drawTitle_ = draw;
		setB("drawTitle", draw);
		touch();
	}
}

void AvContourInfo::lineStyle(const LINESTYLE style)
{
	if(style != lineStyle_)
	{	lineStyle_ = style;
		setMappedInt("lineStyle", style, lineStyles);
		touch();
	}
}

void AvContourInfo::lineWidth(const int width)
{
	if(width != lineWidth_)
	{	lineWidth_ = width;
		setI("lineWidth", width);
		touch();
	}
}

void AvContourInfo::emphasisEnabled(const Boolean val)
{
	if(val != emphasisEnabled_)
	{	emphasisEnabled_ = val;
		setB("emphasisEnabled", val);
		touch();
	}
}

#if 0
void AvContourInfo::emphasisOffset(const int val)
{
	if(val != emphasisOffset_)
	{	emphasisOffset_ = val;
		setB("emphasisOffset", val);
		touch();
	}
}
#endif

void AvContourInfo::emphasisColor(const COLORSTYLE style)
{
	if(style != emphasisColor_)
	{	emphasisColor_ = style;
		setMappedInt("emphasisColor", style, colorStyles);
		touch();
	}
}

void AvContourInfo::colorStyle(const COLORSTYLE style)
{
	if(style != colorStyle_)
	{	colorStyle_ = style;
		setMappedInt("colorStyle", style, colorStyles);
		touch();
	}
}

// Set label spacing parameters. If minint/intval are NULL, the information
// will be generated. See PGCONL for more information.

static const int MINVAL = 8;
static const int INTVAL = 100;

void AvContourInfo::setLabelSpacing(const int levels, int *minint, int *intval)
{
int i;

	checkLevelArray(levels);

	for(i=0; i< levels; i++)
	{ int mi, iv;
		if(minint != NULL)
			mi = minint[i];
		else
			mi = MINVAL;

		if(intval != NULL)
			iv = intval[i];
		else
			iv = INTVAL;
		info_[i].setSpacing(mi, iv);
//		setI("MININT", minint);
//		setI("INTVAL", intval);
	}
	if(labelContours())
		touch();
}

void AvContourInfo::getLabelSpacing(const int level, int &intval, int &minval)
{
	if(! validIndex(level))
		return;
	if(info_[level].haveSpacing())
		info_[level].getSpacing(intval, minval);
	else	// Generate.
	{double value = getContourValue(level);

		if(value < max()*.75)
		{	minval = MINVAL;
			intval = INTVAL;
		}
		else	// Fewer labels closer to the max.
		{	minval = MINVAL*2;
			intval = INTVAL*3;
		}
	}
}

// Whether to support blanked data. It is possible that dashed contours
// will look better if blanking is disabled.
void AvContourInfo::enableBlanking(const Boolean enable)
{
	if(blankingEnabled_ != enable)
	{	blankingEnabled_ = enable;
		setB("enableBlanking", enable);
		touch();
	}
}

void AvContourInfo::setMinMax(const double min, const double max)
{
	min_ = min;
	max_ = max;
//	setD("min", min);
//	setD("max", max);
	touch();
}

void AvContourInfo::levelsAreFractional(const Boolean frac)
{
	if(levelsAreFractional_ != frac)
	{	levelsAreFractional_ = frac;
		setB("levelsAreFractional", frac);
		touch();
	}
}

// Mark as having been modified and notify unless notification
// has been turned off.
void AvContourInfo::touch()
{
	timestamp_ = TimeStamp();

	if(notifyEnabled())
	{
		invokeCallbacks(ValueChanged);
	}
}

// Routines to set values into the interpreter.

void AvContourInfo::setD(const char *name, const double val, const Boolean def)
{
	if(cnfg_ != NULL)
		cnfg_->setDouble(name, val, def);
}

void AvContourInfo::setDL(const char *name, const AvWPosition &list,
			  const Boolean def)
{
	if(cnfg_ != NULL)
		cnfg_->setDoubleList(name, list, def);
}

void AvContourInfo::setI(const char *name, const int val, const Boolean def)
{
	if(cnfg_ != NULL)
		cnfg_->setInt(name, val, def);
}

void AvContourInfo::setMappedInt(const char *name, const int val,
				 const AvConfigInfo::IntMap *map)
{
	if(cnfg_ != NULL)
		cnfg_->setMappedInt(name, val, map);
}

void AvContourInfo::setB(const char *name, const Boolean val)
{
	if(cnfg_ != NULL)
		cnfg_->setBoolean(name, val);
}
