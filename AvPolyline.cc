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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvPolyline.cc,v 19.0 2003/07/16 05:47:50 aips2adm Exp $
//
// $Log: AvPolyline.cc,v $
// Revision 19.0  2003/07/16 05:47:50  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:26  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/22 22:13:07  hravlin
// Casts to int from XtPointer are now handled with Av::PtrToInt().
//
// Revision 17.0  2001/11/12 19:42:54  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:43:01  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:12:18  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:08:56  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  1999/08/25 19:44:34  hravlin
// Edits (mostly casts) to remove compiler warnings.
//
// Revision 13.0  1999/08/10 18:40:45  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:23:57  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 07:01:38  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:55:46  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.4  1998/06/19 14:16:22  hr
// Added GNUC to an if defined() list.
//
// Revision 9.3  1998/06/09 21:24:33  hr
// Removed unneeded semicolon after DEFINE_MEMBER_INFO macro.
//
// Revision 9.2  1998/03/10 22:06:46  hr
// SGI 7.2.0 & 7.2.1 compilers were giving errors on a cast.
//
// Revision 9.1  1997/09/15 16:22:47  hr
// Conversion of line width from an enum to an int.
//
// Revision 9.0  1997/08/25 21:30:41  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:18:48  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1996/12/12 09:36:16  droberts
// Changed all references to IVList to AvIVList.
//
// Revision 1.2  1996/11/18 21:35:08  hr
// Renamed IVlist to AvIVlist.
//
// Revision 1.1  1996/07/11  21:31:21  pixton
// Automated Checkin
//
//  Revision 1.9  1996/06/18  18:46:27  pixton
//  Copyright Update
//
//  Revision 1.8  1996/01/16  19:02:43  hr
//  Some of the AvPolylineCmd constructors weren't setting the aux/aux2
//  variables to NULL.
//
//  Revision 1.7  1995/10/19  16:00:59  hr
//  Added length function.
//
//  Revision 1.6  1995/10/07  15:52:25  hr
//  Reworked drawVector. Added segments, circles and arcs. Rectangles work
//  now.
//
//  Revision 1.5  1995/09/20  20:03:06  baker
//  Release Beta 1.0
//
//  Revision 1.5  1995/09/20  01:56:34  baker
//  Release Beta 1.0
//
//  Revision 1.4  1995/08/30  17:49:23  hr
//  Added destructor for AvPolyStateCmd_.
//
//  Revision 1.3  1995/07/17  21:10:07  hr
//  Added destructor for AvPolyStateCmd.
//
//  Revision 1.2  1995/06/22  19:02:29  hr
//  Added routines that allow changing of graphic context such as line style
//  and color.
//
//  Revision 1.1  1995/03/16  19:45:22  pixton
//  Initial revision
//
//
//---------------------------------------------------------------------------

/* AvPolyline.C
*/

#include "AvPolyline.h"
#include "AvPolylineP.h"
#include "AvCanvas.h"
#include "AvIVList.h"
#include "Av.h"


DEFINE_MEMBER_INFO(AvPolyline)

AvPolyCmd_::AvPolyCmd_()
{
}

AvPolyCmd_::~AvPolyCmd_() {}

AvPolyStateCmd::AvPolyStateCmd(const AvPolyStateCmd::COMMANDS cmd,
							const void *arg)
{
	init(cmd, arg);
}

AvPolyStateCmd::AvPolyStateCmd(const AvPolyStateCmd::COMMANDS cmd,
						const unsigned long arg)
{
	init(cmd, (void *)arg);
}

AvPolyStateCmd::~AvPolyStateCmd()
{
}

void AvPolyStateCmd::init(const AvPolyStateCmd::COMMANDS cmd, const void *arg)
{// long argl = (long) arg;

	command_ = cmd;
	switch (cmd) {
	case DRAWMODE:
		drawMode = (AvGraphicContext::DrawMode) Av::PtrToInt(arg);
		break;
	case FOREGROUND:
	case BACKGROUND:
		color = (unsigned long) arg;
		break;
	case FGNAME:
	case BGNAME:
		name = (char *) arg;
		break;
	case LINEWIDTH:
		lineWidth = Av::PtrToInt(arg);
		break;
	case LINESTYLE:
		lineStyle = (AvGraphicContext::LineStyle)Av::PtrToInt(arg);
		break;
	case NONE:
	default:
		command_ = NONE;
		break;
	}
}

void AvPolyStateCmd::draw(AvCanvas &c)
{
AvGraphicContext *gc = c.getGraphicContext();

	switch (command_) {
	case DRAWMODE:
		gc->setDrawMode(drawMode);
		break;
	case FOREGROUND:
		gc->setForeground(color);
		break;
	case BACKGROUND:
		gc->setBackground(color);
		break;
	case FGNAME:
		c.setDrawColor(name);
		break;
	case BGNAME:
		// Oops.
		break;
	case LINEWIDTH:
		gc->setLineWidth(lineWidth);
		break;
	case LINESTYLE:
		gc->setLineStyle(lineStyle);
		break;
	case NONE:
	default:
		command_ = NONE;
		break;
	}
}

/*****************************************************************/
/*****************************************************************/

/*****************************************************************/

AvPolyDrawCmd_::AvPolyDrawCmd_(COMMAND cmd, AvPolyline::ADDRESSING mode,
		AvPolyline::COORDINATES cmode) : AvPolyCmd_()
{
	cmd_ = cmd;
	mode_ = mode;
	cmode_ = cmode;
	filled_ = FALSE;
}

AvPolyDrawCmd_::~AvPolyDrawCmd_() {}

AvPolylineCmd::AvPolylineCmd(COMMAND cmd, AvPolyline::ADDRESSING mode,
		AvPolyline::COORDINATES cmode,
		float x, float y): AvPolyDrawCmd_(cmd, mode, cmode)
{
	points_ = NULL;
	aux_ = NULL;
	aux2_ = NULL;
	append( &x, &y, 1);
}

AvPolylineCmd::AvPolylineCmd(COMMAND cmd, AvPolyline::ADDRESSING mode,
		AvPolyline::COORDINATES cmode,
		float *x, float *y, int len) : AvPolyDrawCmd_(cmd, mode, cmode)
{
	points_ = NULL;
	aux_ = NULL;
	aux2_ = NULL;
	append(x, y, len);
}

AvPolylineCmd::AvPolylineCmd(COMMAND cmd, AvPolyline::ADDRESSING mode,
		AvPolyline::COORDINATES cmode,
		float x, float y, float a, float b):
	AvPolyDrawCmd_(cmd, mode, cmode)
{
	points_ = NULL;
	aux_ = NULL;
	aux2_ = NULL;
	append( &x, &y, &a, &b, 1);
}

AvPolylineCmd::AvPolylineCmd(COMMAND cmd, AvPolyline::ADDRESSING mode,
		AvPolyline::COORDINATES cmode,
		float *x, float *y, float *a, float *b,int len) :
	AvPolyDrawCmd_(cmd, mode, cmode)
{
	points_ = NULL;
	aux_ = NULL;
	aux2_ = NULL;
	append(x, y, a, b, len);
}

AvPolylineCmd::AvPolylineCmd(COMMAND cmd, AvPolyline::ADDRESSING mode,
		AvPolyline::COORDINATES cmode,
		float *x, float *y, float *a, float *b, float *c, float *d,
		int len) : AvPolyDrawCmd_(cmd, mode, cmode)
{
	points_ = NULL;
	aux_ = NULL;
	aux2_ = NULL;
	append(x, y, a, b, c, d, len);
}

AvPolylineCmd::~AvPolylineCmd()
{
	if(points_ == NULL)
		return;
	delete points_;
	delete aux_;
}

PointList *AvPolylineCmd::positionList()
{
	return points_;
}

PointList *AvPolylineCmd::auxList()
{
	return aux_;
}

PointList *AvPolylineCmd::aux2List()
{
	return aux2_;
}

int AvPolylineCmd::size()const
{
	if(points_ == NULL)
		return 0;
	else
		return points_->size();
}

// Not really append for now. Only get 1 crack at it.
void AvPolylineCmd::append(float *x, float *y, int len)
{
	if((x==NULL) || (y==NULL) || (len <= 0))
		return;
	points_ = new PointList(len);
	while(--len >= 0)
		points_->add(*x++, *y++);
}

void AvPolylineCmd::append(float *x, float *y, float *a, float *b, int len)
{
	if((x==NULL) || (y==NULL) || (len <= 0))
		return;
	if((a==NULL) || (b == NULL))
		return;
	points_ = new PointList(len);
	aux_ = new PointList(len);
	while(--len >= 0)
	{	points_->add(*x++, *y++);
		aux_->add(*a++, *b++);
	}
}

void AvPolylineCmd::append(float *x, float *y, float *a, float *b,
				float *c, float *d, int len)
{
	if((x==NULL) || (y==NULL) || (len <= 0))
		return;
	if((a==NULL) || (b == NULL))
		return;
	if((c==NULL) || (d == NULL))
		return;
	points_ = new PointList(len);
	aux_ = new PointList(len);
	aux2_ = new PointList(len);
	while(--len >= 0)
	{	points_->add(*x++, *y++);
		aux_->add(*a++, *b++);
		aux2_->add(*c++, *d++);
	}
}

void AvPolylineCmd::draw(AvCanvas &c)
{
	c.drawVector( this);
}

/*****************************************************************/

declarePtrList(CmdList, AvPolyCmd_)
implementPtrList(CmdList, AvPolyCmd_)

class AvPolylineImpl {
  public:
	AvPolylineImpl(int len=0);
	~AvPolylineImpl();
	enum { DEFAULTLEN = 4};
	void append(AvPolyCmd_ *cmd);
	int	length()const { return (int)list_->count();}

	CmdList *list_;
};

AvPolylineImpl::AvPolylineImpl(int len)
{
	if(len <= 0)
		len = DEFAULTLEN;

	list_ = new CmdList(len);
}

AvPolylineImpl::~AvPolylineImpl()
{
//	for(ListUpdater(CmdList) i(*list_); i.more(); i.next())
	for(ListItr(CmdList) i(*list_); i.more(); i.next())
	{ AvPolyCmd_ *c = i.cur();
//		i.remove_cur();
		delete c;
	}
	delete list_;
}

void AvPolylineImpl::append(AvPolyCmd_ *cmd)
{
	if((cmd != NULL) && (list_ != NULL))
		list_->append(cmd);
}

/*****************************************************************/
/*		External interface.				**/
/*****************************************************************/

void AvPolyline::init(int len)
{
	INIT_CLASS(AvGraphics2D);
	mode_ = ABSOLUTE;
	cmode_ = FRACTIONAL;
	shape_ = COMPLEX;
	impl_ = new AvPolylineImpl(len);
}

AvPolyline::AvPolyline(int len)
{
	init(len);
}

AvPolyline::AvPolyline(float x, float y, int len)
{
	init(len);
	movea(x,y);
}

AvPolyline::~AvPolyline()
{
	delete impl_;
}

// Remove current command list and start from scratch.
// If newlen < 0 use current list size
// 	     = 0 use default
//	    else newlen.
void AvPolyline::reset(const int newlen)
{
int	len;

	if(impl_ != NULL)
	{	len = impl_->length();
		delete impl_;
	}
	else
		len = 0;

	if(newlen > 0)
		len = newlen;

	impl_ = new AvPolylineImpl(len);
}

// Returns the number of items in the list. Since the list is not accessible,
// this is mostly used to indicate whether the list is empty or not.
int AvPolyline::length()const
{
int	len;

	if(impl_ != NULL)
		len = impl_->length();
	else
		len = 0;
	return len;
}

#if 0
// Create a new segment.
void AvPolyline::open()
{
	impl_->open();
}

void AvPolyline::close()
{
	impl_->close();
}
#endif

void AvPolyline::addressMode(ADDRESSING mode)
{
	mode_ = mode;
}

AvPolyline::ADDRESSING AvPolyline::addressMode() { return mode_; }

void AvPolyline::coordinateMode(COORDINATES cmode) { cmode_ = cmode; }
AvPolyline::COORDINATES AvPolyline::coordinateMode(){ return cmode_; }

void AvPolyline::move(float x, float y)
{
	impl_->append(new AvPolylineCmd(MOVE, mode_, cmode_, x, y));
}

void AvPolyline::movea(float x, float y)
{
	impl_->append(new AvPolylineCmd(MOVE, ABSOLUTE, FRACTIONAL, x, y));
}

void AvPolyline::mover(float x, float y)
{
	impl_->append(new AvPolylineCmd(MOVE, RELATIVE, FRACTIONAL, x, y));
}

void AvPolyline::line(float x, float y)
{
	impl_->append(new AvPolylineCmd(LINE, mode_, cmode_, x, y));
}

void AvPolyline::line(float *x, float *y, int len)
{
	impl_->append(new AvPolylineCmd(LINE, mode_, cmode_, x, y,len));
}

void AvPolyline::point(float x, float y)
{
	impl_->append(new AvPolylineCmd(POINT, mode_, cmode_, x, y));
}

void AvPolyline::point(float *x, float *y, int len)
{
	impl_->append(new AvPolylineCmd(POINT, mode_, cmode_, x, y, len));
}


void AvPolyline::linea(float x, float y)
{
	impl_->append(new AvPolylineCmd(LINE, ABSOLUTE, FRACTIONAL, x, y));
}

void AvPolyline::liner(float x, float y)
{
	impl_->append(new AvPolylineCmd(LINE, RELATIVE, FRACTIONAL, x, y));
}

void AvPolyline::linea(float *x, float *y, int len)
{
	impl_->append(new AvPolylineCmd(LINE, ABSOLUTE, FRACTIONAL, x, y,len));
}

void AvPolyline::liner(float *x, float *y, int len)
{
	impl_->append(new AvPolylineCmd(LINE, RELATIVE, FRACTIONAL, x, y,len));
}

void AvPolyline::pointa(float x, float y)
{
	impl_->append(new AvPolylineCmd(POINT, ABSOLUTE, FRACTIONAL, x, y));
}

void AvPolyline::pointr(float x, float y)
{
	impl_->append(new AvPolylineCmd(POINT, RELATIVE, FRACTIONAL, x, y));
}

void AvPolyline::pointa(float *x, float *y, int len)
{
	impl_->append(new AvPolylineCmd(POINT, ABSOLUTE, FRACTIONAL,
			x, y, len));
}

void AvPolyline::pointr(float *x, float *y, int len)
{
	impl_->append(new AvPolylineCmd(POINT, RELATIVE, FRACTIONAL,
			x, y, len));
}

//		Arguments are 'fullsize'
void AvPolyline::moveabs(float x, float y)
{
	impl_->append(new AvPolylineCmd(MOVE, ABSOLUTE, FULLSIZE, x, y));
}

void AvPolyline::moverel(float x, float y)
{
	impl_->append(new AvPolylineCmd(MOVE, RELATIVE, FULLSIZE, x, y));
}

void AvPolyline::lineabs(float x, float y)
{
	impl_->append(new AvPolylineCmd(LINE, ABSOLUTE, FULLSIZE, x, y));
}

void AvPolyline::linerel(float x, float y)
{
	impl_->append(new AvPolylineCmd(LINE, RELATIVE, FULLSIZE, x, y));
}

void AvPolyline::lineabs(float *x, float *y, int len)
{
	impl_->append(new AvPolylineCmd(LINE, ABSOLUTE, FULLSIZE, x, y, len));
}

void AvPolyline::linerel(float *x, float *y, int len)
{
	impl_->append(new AvPolylineCmd(LINE, RELATIVE, FULLSIZE, x, y, len));
}

void AvPolyline::pointabs(float x, float y)
{
	impl_->append(new AvPolylineCmd(POINT, ABSOLUTE, FULLSIZE, x, y));
}

void AvPolyline::pointrel(float x, float y)
{
	impl_->append(new AvPolylineCmd(POINT, RELATIVE, FULLSIZE, x, y));
}

void AvPolyline::pointabs(float *x, float *y, int len)
{
	impl_->append(new AvPolylineCmd(POINT, ABSOLUTE, FULLSIZE, x, y, len));
}

void AvPolyline::pointrel(float *x, float *y, int len)
{
	impl_->append(new AvPolylineCmd(POINT, RELATIVE, FULLSIZE, x, y, len));
}

void AvPolyline::polygon(float *x, float *y, int len,
		const Boolean filled, AvPolyline::POLYGONSHAPE shape)
{
AvPolylineCmd *cmd;

	cmd = new AvPolylineCmd(POLYGON, mode_, cmode_, x, y, len);
	cmd->filled_ = filled;
	cmd->shape_ = shape;
	impl_->append(cmd);
}

void AvPolyline::polygon(float x, float y,
			const Boolean filled, AvPolyline::POLYGONSHAPE shape)
{
	polygon( &x, &y, 1, filled, shape);
}

void AvPolyline::rectangle(float x, float y, float width, float height,
				Boolean filled)
{
	rectangle( &x, &y, &width, &height, 1, filled);
}

void AvPolyline::rectangle(float *x, float *y,
			  float *width, float *height, int num,
			  Boolean filled)
{
AvPolylineCmd *cmd;

	cmd = new AvPolylineCmd(RECTANGLE, mode_, cmode_,
					x, y, width, height, num);
	cmd->filled_ = filled;
	impl_->append(cmd);
}

void AvPolyline::segment(float *x1, float *y1,
				float *x2, float *y2, const int len)
{
AvPolylineCmd *cmd;

	cmd = new AvPolylineCmd(SEGMENT, mode_, cmode_,
					x1, y1, x2, y2, len);
	impl_->append(cmd);
}

void AvPolyline::circle(float x, float y, float radius,
				const Boolean filled)
{
	circle( &x, &y, &radius, 1, filled);
}

void AvPolyline::circle(float *x, float *y, float *radius,
				const int len, const Boolean filled)
{
AvPolylineCmd *cmd;
float	zero=0.0;

	cmd = new AvPolylineCmd(CIRCLE, mode_, cmode_,
					x, y, radius, &zero, len);
	cmd->filled_ = filled;
	impl_->append(cmd);
}

void AvPolyline::arc(float x, float y, float width, float height,
			float angle1, float angle2,
			const Boolean filled)
{
	arc( &x, &y, &width, &height, &angle1, &angle2, 1, filled);
}

void AvPolyline::arc(float *x, float *y, float *width, float *height,
			float *angle1, float *angle2,
			const int narcs,
			const Boolean filled)
{
AvPolylineCmd *cmd;

	cmd = new AvPolylineCmd(ARC, mode_, cmode_,
			x, y, width, height, angle1, angle2, narcs);

	cmd->filled_ = filled;
	impl_->append(cmd);

}

void AvPolyline::segment(float x1, float y1, float x2, float y2)
{
	segment(&x1, &y1, &x2, &y2, 1);
}



/*****************************************************************/
/*****************************************************************/
void AvPolyline::foreground(const unsigned long color)
{
AvPolyStateCmd *cmd;

	cmd = new AvPolyStateCmd(AvPolyStateCmd::FOREGROUND, (void *)color);
	impl_->append(cmd);
}

void AvPolyline::foregroundName(const char *name)
{
AvPolyStateCmd *cmd;

	cmd = new AvPolyStateCmd(AvPolyStateCmd::FGNAME, name);
	impl_->append(cmd);
}

void AvPolyline::background(const unsigned long color)
{
AvPolyStateCmd *cmd;

	cmd = new AvPolyStateCmd(AvPolyStateCmd::BACKGROUND, color);
	impl_->append(cmd);
}
void AvPolyline::backgroundName(const char *name)
{
AvPolyStateCmd *cmd;

	cmd = new AvPolyStateCmd(AvPolyStateCmd::BGNAME, name);
	impl_->append(cmd);
}

void AvPolyline::lineStyle(const int style)
{
AvPolyStateCmd *cmd;

	cmd = new AvPolyStateCmd(AvPolyStateCmd::LINESTYLE, style);
	impl_->append(cmd);
}

void AvPolyline::lineWidth(const int width)
{
AvPolyStateCmd *cmd;

	cmd = new AvPolyStateCmd(AvPolyStateCmd::LINEWIDTH, width);
	impl_->append(cmd);
}

void AvPolyline::drawMode(const int mode)
{
AvPolyStateCmd *cmd;

	cmd = new AvPolyStateCmd(AvPolyStateCmd::DRAWMODE, mode);
	impl_->append(cmd);
}


/*****************************************************************/
/*****************************************************************/

void AvPolyline::draw(AvCanvas &c)
{
//cerr << name() << "enter " << endl;
	c.pushState();


	for(ListItr(CmdList) i(*impl_->list_); i.more(); i.next())
	{ AvPolyCmd_ *cmd = i.cur();
		cmd->draw(c);
	}
//cerr << name() << "exit " << endl;

	c.popState();
}

void AvPolyline::draw(AvCanvas *c)
{
	if(c != NULL)
		draw(*c);
}

// Vectors always request a size of 1x1. Their enclosing Viewport sets the
// real size.
void AvPolyline::getBoundingBox(AvCanvas &, AvAllotment &r)
{
	r.value(AvDimensionX, 1.0);
	r.value(AvDimensionY, 1.0);
}
