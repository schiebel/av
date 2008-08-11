#if defined(AVOGL)

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
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvGLCanvas.cc,v 19.0 2003/07/16 05:48:16 aips2adm Exp $
//
// $Log: AvGLCanvas.cc,v $
// Revision 19.0  2003/07/16 05:48:16  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:29:48  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.1  2002/01/03 21:04:52  hravlin
// Initial checkin.
//
//---------------------------------------------------------------------------

/* AvGLCanvas.C
Handles Canvas functions for OpenGL.

Issues:

 When resized, the image is drawn twice. Once from the exposecb callback
 (from GLX?) and once from above. The exposecb is needed to repaint the
 window when it becomes unobscured.

 Choosing 'Show Contours' causes two redraws. It appears the first one is
 from expose event when the menu goes away.

 Need to handle both TrueColor and PseudoColor visuals

 Multiple exposure events?


 Allow switching between textures and drawpixels.

 Thread textures.

 Have image scaled to wiregl's window size initially. Don't want to allow
 user to scale when using wiregl, but aipsview needs to be able to reduce
 size.

 X draws are used to draw regions. (& Displaying regions has problems).

 Add display lists.

Deal with mouse/cursor for wiregl.
Pipeserver? occasionally gets very slow. Also, it's a cpu pig.

Upper level routines set viewport to window size so images & axes can be
too big. (Wedge and text get truncated).


			wiregl problems
glXIsDirect returns 1 with wiregl.
glFinish doesn't block with wiregl. (V1.1)
glTexSubImage2D doesn't handle subtextures correctly. (V1.1)

*/

#include <stdio.h>
#include <stdlib.h>			// getenv
#include <sys/types.h>
#include <time.h>
#include <sys/times.h>
#include <limits.h>
#include <strings.h>

#include <Xm/Xm.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <X11/GLw/GLwDrawA.h>

#ifdef  HANDLEBLANKING
#include <math.h>
#ifndef STANDALONE
#include "Av.h"			// For BLANKEDVALUE.
#else
#define	BLANKEDVALUE	MAXFLOAT
#endif
#endif

#if defined(PTHREADS)
// Currently, multithreading is only available while scaling an image.

#include <pthread.h>
#include <semaphore.h>

/*SGI
 cc -D_POSIX_C_SOURCE=199506L app.c -llib0 -llib1 ... -lpthread
 SUN
 cc <file.c> -mt lpthread -lrt
*/

#if !defined(ENABLE_TIMING)
#define ENABLE_TIMING
#endif

// Used to limit access to OGL library.
static Boolean access_inited = False;
static sem_t access_GL;
#endif	// PTHREADS

#include "AvViewportSetter.h"
#include "AvGLCanvas.h"
#include "AvRaster.h"
#include "AvRasterCache.h"
#include "AvWidgetMisc.h"
#include "AvPolylineP.h"
#include "AvApp.h"
#include "AvFallbacks.h"
#include "AvXColorApp.h"

int AvGLCanvas::externaldrawingarea_ = -1;

static const char *CLIPTO_COLOR = "AV_CLIPTO_COLOR";

/* Get environment integer value for environment variable str.
*/
static int getEnvInt(const char *str, const int def)
{
char	*env;
int	val;

	env = getenv(str);
	if((env == NULL) || (strlen(env) == 0))
		return def;

	val = (int) strtol(env, NULL, 0);
	return val;
}

static const char *isOn(const GLboolean ison)
{
	return ison ? "on" : "off";
}

static void pstate()
{
	GLboolean depth = glIsEnabled(GL_DEPTH_TEST);
// Not in wireGL
//	GLboolean fog = glIsEnabled(GL_FOG);
	GLboolean blend = glIsEnabled(GL_BLEND);
	GLboolean lighting = glIsEnabled(GL_LIGHTING);
	GLboolean alpha = glIsEnabled(GL_ALPHA_TEST);
// Not in wireGL
//	GLboolean scissor = glIsEnabled(GL_SCISSOR_TEST);

#if 0
	printf("GL state:\n\tDepth %s, Fog %s, Blend %s, Lighting %s, Alpha %s\n",
		isOn(depth), isOn(fog), isOn(blend), isOn(lighting),
		isOn(alpha));
#else
	printf("GL state:\n\tDepth %s, Blend %s, Lighting %s, Alpha %s\n",
		isOn(depth), isOn(blend), isOn(lighting),
		isOn(alpha));
#endif
//	printf("\tScissor %s\n", isOn(scissor));
}

int AvGLCanvas::externalDrawingArea()
{	if(externaldrawingarea_ == 0)
		return 0;
	else if(externaldrawingarea_ == 1)
		return 1;
	else
	{	printf("externalDrawingArea called before initialization.\n");
		return 0;
	}
}

// Print any OGL errors.
int AvGLCanvas::glcheck(const char *msg) {
int numerrs = 0;
GLenum err;

	const char *str = (msg == NULL) ? "AvGLCanvas:: OGL errors:"
			: msg;

	while( (err = glGetError()) != GL_NO_ERROR)
	{	const unsigned char *errstr = gluErrorString(err);
		if(numerrs == 0)
			printf("\t\t%s\n", str);
		numerrs += 1;
		if(errstr != NULL)
			printf("%d: %s\n", err, errstr);
		else
			printf("GLError: %d:\n", err);

	}
	if(numerrs > 0)
		printf("\n");
	return numerrs;
}

#if 1
static void pstring(const char *name, const char *value)
{ const char *str = (value == NULL) ? "NULL" : value;
	printf("%s\t- %s\n", name, str);
}

static Boolean haveGLInfo=False;

static void printGLInfo()
{ char *val;

	if(haveGLInfo)
		return;
	val = (char*)glGetString(GL_VENDOR);
	pstring("VENDOR", val);
	val = (char*)glGetString(GL_RENDERER);
	pstring("RENDERER", val);
	if(val != NULL)
		haveGLInfo = True;
	val = (char*)glGetString(GL_VERSION);
	pstring("VERSION", val);
#if 0
	val = (char*)glGetString(GL_EXTENSIONS);
	pstring("EXTENSIONS", val);
#endif

	GLint vpmax[2];
	glGetIntegerv(GL_MAX_VIEWPORT_DIMS, vpmax);
	printf("VPmax %d,%d\n", vpmax[0], vpmax[1]);

	GLint maxtexturesize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxtexturesize);
	printf( "Maxtexturesize %d\n", maxtexturesize);
}
#endif

GLint AvGLCanvas::maxTextureSize_ = 0;

////////////////////////////////////////////////////////////////
////			AvGLContext
//// Wrapper around GLXContext
////////////////////////////////////////////////////////////////
class AvGLContext : public AvResource {
  public:
	AvGLContext(Display *dpy, XVisualInfo *vi); 
	~AvGLContext();
	void makeCurrent(Display *dpy, Window window);
	GLXContext glxcontext()const { return context_;}
	Boolean modifiedSince(const AvGLContext &c)const;
	void waitX();
	void waitGL();
  private:
	GLXContext	context_;
	AvTimeStamp	timestamp_;
};

AvGLContext::AvGLContext(Display *dpy, XVisualInfo *vi)
{
	context_ = glXCreateContext(dpy, vi, 
				    /* No display list sharing. */ None,
			      	    /* Favor direct rendering. */ True);
}

AvGLContext::~AvGLContext() {}

void AvGLContext::makeCurrent(Display *dpy, Window window)
{
	glXMakeCurrent(dpy, window, context_);
	timestamp_.touch();
}

Boolean AvGLContext::modifiedSince(const AvGLContext &ctx)const
{	return timestamp_.modifiedSince(ctx.timestamp_);
}

void AvGLContext::waitX()
{	if(!AvGLCanvas::externalDrawingArea())
		glXWaitX();
}

void AvGLContext::waitGL()
{	if(!AvGLCanvas::externalDrawingArea())
		glXWaitGL();
}

// Create a new GLcontext. If wiregl, reuse the same one.
static unsigned int MAXWIREGLCONTEXTS=1;
// Someday this may grow as needed.
static const int MAXGLCONTEXTS=64;	// Max OGL contexts. (OGL Windows)
static AvGLContext **glcontexts=NULL;
static unsigned int maxglcontexts=0;
static unsigned int numglcontexts=0;

// Return 
static AvGLContext *makeGLContext(Display *dpy, XVisualInfo *vi)
{ AvGLContext *glc;

	if(maxglcontexts == 0)
	{	// Init.
		maxglcontexts = AvGLCanvas::externalDrawingArea() 
				? MAXWIREGLCONTEXTS : MAXGLCONTEXTS;
		glcontexts = new (AvGLContext *[maxglcontexts]);
		numglcontexts = 0;
	}

	if(numglcontexts < maxglcontexts)	// Make a new one.
	{	glc = new AvGLContext(dpy, vi);
		glcontexts[numglcontexts++] = glc;
	} else if(numglcontexts == 1)	// The only one.
		glc = glcontexts[0];
	else	// Use context with oldest timestamp.
	{ int c = 0;
		for(unsigned int i=1; i< numglcontexts; i++)
		  if(glcontexts[0]->modifiedSince(*glcontexts[i]))
			c = i;
		glc = glcontexts[c];
	}

	glc->ref();
//	printf("makeGLContext: %d contexts. 0x%X\n", numglcontexts, glc);
//	glcheck();
	return glc;
}

////////////////////////////////////////////////////////////////
// Contains one GL texture, + info on how to draw it.

class AvTextureObject {
  public:
	// Create a texture object of the given size and format.
	AvTextureObject(const GLsizei width, const GLsizei height,
			const GLenum format=GL_RGB,
			const GLenum type=GL_UNSIGNED_SHORT,
			const GLint wraps=GL_REPEAT,
			const GLint wrapt=GL_REPEAT,
			const GLint smag=GL_NEAREST,
			const GLint tmag=GL_NEAREST);
	~AvTextureObject();

	void createTexture(const GLsizei width, const GLsizei height,
			   const GLenum format=GL_RGB,
			   const GLenum type=GL_UNSIGNED_SHORT,
			   const GLint wraps=GL_REPEAT,
			   const GLint wrapt=GL_REPEAT,
			   const GLint mags=GL_NEAREST,
			   const GLint magt=GL_NEAREST);

	GLuint bindTexture();

	// Copy data to existing texture.
	void storeTexture(const GLint xoffset, const GLint yoffset,
			  const GLsizei dwidth, const GLsizei dheight,
			  const GLvoid *data);

	// Use x & y as starting points for drawing current texture.
	void locateTexture(const float x, const float y,
			   const float xscl=1.0, const float yscl=1.0)
		{	x_ = x;
			y_ = y;
			xscl_ = xscl;
			yscl_ = yscl;
		}

	GLuint textureID()const { return texid_;}
	// Draw texture
	void drawTexture();
	static unsigned int pow2(const unsigned int num);
  private:
	static void reserveGL() {AvGLCanvas::reserveGL();}
	static void releaseGL() {AvGLCanvas::releaseGL();}
  private:
	GLuint	texid_;
	GLfloat x_, y_;		// Where to draw texture.
	GLfloat xscl_, yscl_;
	GLsizei	twidth_;	// Texture width/height
	GLsizei	theight_;
	GLsizei dwidth_;	// Data width/height.
	GLsizei dheight_;
	GLenum	format_;
	GLenum	type_;
};

AvTextureObject::AvTextureObject(const GLsizei width, const GLsizei height,
				 const GLenum format, const GLenum type,
				 const GLint wraps, const GLint wrapt,
				 const GLint mags, const GLint magt)
{	texid_ = 0xFFFF;
	x_ = y_ = 0;
	xscl_ = yscl_ = 1.0;

	reserveGL();
	 glGenTextures(1, &texid_);
//	 glGenTexturesEXT(1, &texid_);
	releaseGL();
	createTexture(width, height, format, type, wraps, wrapt, mags, magt);
}

void AvTextureObject::createTexture(const GLsizei width, const GLsizei height,
				    const GLenum format, const GLenum type,
				    const GLint wraps, const GLint wrapt,
				    const GLint mags, const GLint magt)
{	twidth_ = pow2(width);
	theight_ = pow2(height);
	dwidth_ = dheight_ = 0;	// No data yet.
	format_ = format;
	type_ = type;

	reserveGL();
	glBindTexture(GL_TEXTURE_2D, texid_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wraps);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapt);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mags);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, magt);

	// Should use GL_PROXY_TEXTURE_2D to check first!
	// Creates (allocates) a texture, but w/o data.
	// Requires V1.1 or greater.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth_, theight_, 0,
		     format_, type_, NULL);
	releaseGL();
}

AvTextureObject::~AvTextureObject()
{
	reserveGL();
	 glDeleteTextures(1, &texid_);
	releaseGL();
	printf("AvTextureOBject: deleted textureid %d\n", texid_);
}

// Return the byte alignment for the value.
static inline int getalignment(const long val)
{
	int alignment, algncheck;
	algncheck = (int)(val & 0x7);
	switch (algncheck) {
	case 0:
		alignment = 8;
		break;
	case 2:
	case 6:
		alignment = 2;
		break;
	case 4:
		alignment = 4;
		break;
	default:		// 1,3,5,7
		alignment = 1;
		break;
	};
	return alignment;
}

void AvTextureObject::storeTexture(
			const GLint xoffset, const GLint yoffset,
			const GLsizei Width, const GLsizei Height,
			const GLvoid *pixels)
{ GLsizei width=Width, height=Height;

	if(width + xoffset > twidth_)
	{ printf(
	    "AvTextureObject::storeTexture width (%d) too large for (%d)\n",
		width, twidth_);	
		width = twidth_ - xoffset;
	}

	if(height + yoffset > theight_)
	{ printf(
	    "AvTextureObject::storeTexture height (%d) too large for (%d)\n",
		height, theight_);	
		height = theight_ - yoffset;
	}

	// Odd widths and odd heights (from the 1 big container buffer)
	// can cause alignment problems. If odd coloring occurs, make
	// alignment always one.
	// Choose the minimum alignment.
	int alignment = getalignment((long)pixels);
	if(alignment > 1)
	{ int widthalignment = getalignment(width);
		if(widthalignment < alignment)
			alignment = widthalignment;
	}

#if 0
	printf(
"storing texture: id: %u, xoff/yoff %d/%d, w/h %d/%d tw/h %d/%d algn: %d 0x%x\n",
		texid_, xoffset, yoffset, width, height, twidth_, theight_,
		alignment, pixels);
#endif

	reserveGL();
	bindTexture();
// wireGL has a problem with glTexSubImage2D.
#define WIREGLTexSubImage2DHack 1

#ifdef WIREGLTexSubImage2DHack

	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

	// This assumes unsigned shorts and 3 components/pixel!!!!
//	if(externaldrawingarea_)
	if(0)
	{ const GLushort *ptr = (GLushort *)pixels;
		for(int r=0; r< height; r++, ptr+=width*3)
		{	glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset+r,
					width, 1, format_, type_, ptr);
		}
	}
#endif
	else
	{	glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset,
				width, height, format_, type_,
				(GLushort *)pixels);
	}
	releaseGL();
	dwidth_ = width;
	dheight_ = height;
}

void AvTextureObject::drawTexture()
{
#if 0
	float x0=x_, x1=x0+dwidth_*xscl_ -1;
	float y0=y_, y1=y0+dheight_*yscl_ -1;
	float tx0=0.0, tx1=(float)(dwidth_)/(float)(twidth_);
	float ty0=0.0, ty1=(float)(dheight_)/(float)(theight_);
#else
	float x0=x_, x1=x0+dwidth_*xscl_;
	float y0=y_, y1=y0+dheight_*yscl_;
	float tx0=0.0, tx1=(float)(dwidth_)/(float)(twidth_);
	float ty0=0.0, ty1=(float)(dheight_)/(float)(theight_);
#endif

#if 0
//	tx1 = 1.0; ty1 = 1.0;
	printf("dT: %2u: x0/y0 %.2f/%.2f, x1/y1 %.2f/%.2f\n",
	        texid_, x0,y0, x1,y1);

	printf("    tx0/ty0 %.2f/%.2f, tx1/ty1 %.2f/%.2f\n",
		tx0,ty0, tx1, ty1);
	printf("    dw/dh %d/%d, tw/th %d/%d\n",
		dwidth_, dheight_, twidth_, theight_);

	printf("    xscl/yscl %.2f/%.2f\n", xscl_, yscl_);
#endif
	reserveGL();
	bindTexture();
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBegin(GL_QUADS);
	 	glTexCoord2f(tx0, ty0); glVertex2f(x0, y0);
	 	glTexCoord2f(tx0, ty1); glVertex2f(x0, y1);
	 	glTexCoord2f(tx1, ty1); glVertex2f(x1, y1);
	 	glTexCoord2f(tx1, ty0); glVertex2f(x1, y0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	releaseGL();
}

// Return a power of two >= num.
unsigned int AvTextureObject::pow2(const unsigned int num)
{ unsigned int mask = 1, val;

	if(num == 0)
		return 0;

	while(mask < num)
		mask = (mask<<1) + 1;

	val = mask+1;
	if( (val >> 1) == num)	// Num already a power of 2?
		return num;	// Yes.
	else
		return val;
}

// Bind a texture id.
// This doesn't have a reserve/release since they need to be around this
// AND what depends on it.
GLuint AvTextureObject::bindTexture()
{
	glBindTexture(GL_TEXTURE_2D, texid_);
	return texid_;
}

////////////////
///	A list of texture objects. Used for caching.

class AvTextureObjects {
  public:
	AvTextureObjects(const int numtextures);
	~AvTextureObjects();
	AvTextureObject *getTexture(const int index)const;
	int makeTexture(const int width, const int height);
	void draw();
  private:
	int	numtextures_;
	int	nextfree_;
	AvTextureObject **textures_;
};

AvTextureObjects::AvTextureObjects(const int numtextures)
{
	textures_ = new (AvTextureObject *[numtextures]);
	nextfree_ = 0;
	numtextures_ = numtextures;
}

AvTextureObjects::~AvTextureObjects()
{
	for(int i=0; i < nextfree_; i++)
		delete textures_[i];
	delete textures_;
	textures_ = NULL;
	numtextures_ = 0;
}

AvTextureObject *AvTextureObjects::getTexture(const int indx)const
{
	if(indx >= nextfree_)
		return NULL;
	else
		return textures_[indx];
}

int AvTextureObjects::makeTexture(const int width, const int height)
{
	if(nextfree_ >= numtextures_)
		return -1;
	textures_[nextfree_] = new AvTextureObject(width, height);
	return nextfree_++;
}

void AvTextureObjects::draw()
{
	for(int i=0; i< nextfree_; i++)
		textures_[i]->drawTexture();
}

////////////////////////////////////////////////////////////////
//	AvTextureCache
// RasterCache Subclasses to support texture caching.

class AvTextureCacheValue : public AvRasterCacheValue {
public:
	AvTextureCacheValue(AvTextureObjects *objs);
	AvTextureCacheValue();
	virtual ~AvTextureCacheValue();
	AvTextureObjects *value_;
};

class AvTextureCache : public AvRasterCache {
  public:
	AvTextureCache(const int size);
	virtual void insert(const AvRasterCacheKey &, AvTextureObjects *);
	AvTextureObjects *find( const Key &,
				const Boolean removeIfChanged=TRUE);
	virtual ~AvTextureCache();
};

AvTextureCacheValue::AvTextureCacheValue(AvTextureObjects *to)
{
	value_ = to;
}

AvTextureCacheValue::AvTextureCacheValue()
{
	value_ = NULL;
}

AvTextureCacheValue::~AvTextureCacheValue()
{
	delete value_;
	value_ = NULL;
}

AvTextureCache::AvTextureCache(const int size) : AvRasterCache(size)
{
}

AvTextureCache::~AvTextureCache()
{
	printf("AvTextureCache:: deleted\n");
}

void AvTextureCache::insert(const AvRasterCacheKey &key,
			    AvTextureObjects *value)
{
	if(value != NULL)
		insertValue(key, new AvTextureCacheValue(value));
}

AvTextureObjects *AvTextureCache::find( const Key &key,
					const Boolean removeIfChanged)
{ AvTextureCacheValue *v =
			(AvTextureCacheValue *)findValue(key, removeIfChanged);
	if(v == NULL)
		return NULL;
	else
		return v->value_;
}

////////////////////////////////////////////////////////////////
// Return the width and height of the widget.
void getwidthheight(Widget wid, int &width, int &height)
{
Arg	arg[5];
Dimension	w,h, borderWidth;
Position	x,y;
int		i;

	i = 0;
	XtSetArg(arg[i], XtNwidth, &w);	i++;
	XtSetArg(arg[i], XtNheight, &h); i++;
	XtSetArg(arg[i], XtNx, &x);	i++;
	XtSetArg(arg[i], XtNy, &y);	i++;
	XtSetArg(arg[i], XtNborderWidth, &borderWidth); i++;

	XtGetValues(wid, arg, i);
	width = w;
	height = h;
	printf("getwidthheight:: Widget size: w=%d, h=%d bw=%d, x/y= %d/%d\n",
		width, height, borderWidth, x, y);
}

#if 0
static struct Events {	int	value;
		char	*name;
	} events[] = {
	{ KeyPress, "KeyPress"},
	{ KeyRelease, "KeyRelease"},
	{ ButtonPress, "ButtonPress"},
	{ ButtonRelease, "ButtonRelease"},
	{ MotionNotify, "MotionNotify"},
	{ EnterNotify, "EnterNotify"},
	{ LeaveNotify, "LeaveNotify"},
	{ FocusIn, "FocusIn"},
	{ FocusOut, "FocusOut"},
	{ KeymapNotify, "KeymapNotify"},
	{ Expose, "Expose"},
	{ GraphicsExpose, "GraphicsExpose"},
	{ NoExpose, "NoExpose"},
	{ VisibilityNotify, "VisibilityNotify"},
	{ CreateNotify, "CreateNotify"},
	{ DestroyNotify, "DestroyNotify"},
	{ UnmapNotify, "UnmapNotify"},
	{ MapNotify, "MapNotify"},
	{ MapRequest, "MapRequest"},
	{ ReparentNotify, "ReparentNotify"},
	{ ConfigureNotify, "ConfigureNotify"},
	{ ConfigureRequest, "ConfigureRequest"},
	{ GravityNotify, "GravityNotify"},
	{ ResizeRequest, "ResizeRequest"},
	{ CirculateNotify, "CirculateNotify"},
	{ CirculateRequest, "CirculateRequest"},
	{ PropertyNotify, "PropertyNotify"},
	{ SelectionClear, "SelectionClear"},
	{ SelectionRequest, "SelectionRequest"},
	{ SelectionNotify, "SelectionNotify"},
	{ ColormapNotify, "ColormapNotify"},
	{ ClientMessage, "ClientMessage"},
	{ MappingNotify, "MappingNotify"},
	{ LASTEvent, "LASTEvent"}
	};
static const int NEVENTS = sizeof(events)/sizeof(*events);
static const char *XEventToString(const int v)
{	for(int i=0; i< NEVENTS; i++)
		if(v == events[i].value)
			return events[i].name;
	return "Unknown";
}
#endif

#if 0
static struct {
	int reason;
	char *name;
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

static char *motif_reason_to_name(int reason)
{
int i;

	for(i = 0; i< sizeof(reasons)/sizeof(*reasons); i++)
		if(reason == reasons[i].reason)
			return reasons[i].name;

	return "Unknown";
}
#endif

////////////////////////////////////////////////////////////////
///		Callbacks

// Called when window is exposed.
void AvGLCanvas::exposecb(Widget wid, XtPointer clientdata, XtPointer data)
{
	AvGLCanvas *c = (AvGLCanvas *)clientdata;
	XmDrawingAreaCallbackStruct *cbs = (XmDrawingAreaCallbackStruct *)data;
	XExposeEvent *exp = (XExposeEvent *)cbs->event;

#if 0
	XEvent *xevent = cbs->event;
	const char *mname = motif_reason_to_name(cbs->reason);
	const char *ename = XEventToString(xevent->type);

	printf("exposecb 0x%lx Count: %d Reason %s (%d) %s\n",
		exp->window, exp->count, mname, cbs->reason, ename);
	if(xevent->type == Expose)
		printf("X %d Y %d W %d H %d; w/h %d/%d\n",
			exp->x, exp->y, exp->width, exp->height,
			c->width_, c->height_);
#endif

	// Attempt to reduce the number of redraws due to exposeure events.
	// Ignore if to left of above canvas area.
	int cwidth = c->width_, cheight=c->height_;
	if(((exp->x > cwidth) || (exp->y > cheight)) && ! externaldrawingarea_)
		return;

	// Ignore if amount of damage isn't big enough.
	float earea = exp->width * exp->height;
	float carea = cwidth*cheight;
	if( (earea/carea < 0.25) && ! externaldrawingarea_) // Pick any number.
		return;

#if 0
	Display *dpy = XtDisplay(wid);

	// Ignore all but the last expose.
	while(XEventsQueued(dpy, QueuedAlready) > 0)
	{XEvent e;
		XPeekEvent(dpy, &e);
		if(e.type == NoExpose)// Should look into getting rid of these.
			XNextEvent(dpy, &e);	// Pitch NoExpose events.
		else
		if(e.type = Expose)
			XNextEvent(dpy, &e);	// Pitch Expose events.
	}
#endif
	c->expose();

	// Wiregl only needs this called once.
	if(externaldrawingarea_)
	{	XtRemoveCallback(wid, XmNexposeCallback,
			      (XtCallbackProc) exposecb, (XtPointer) c);
	}
}

void AvGLCanvas::expose()
{
//	printf("expose\n");
	if(!exposedOnce_)
	{	resized();		// Initialize viewport.
		glcheck();
		pstate();
		glcheck("After pstate");
	}

	if(!externaldrawingarea_ || !exposedOnce_)
		show();
	exposedOnce_ = True;
}

// Resize callback.
void AvGLCanvas::resizecb(Widget w, XtPointer clientData, XtPointer callData)
{
	if(externaldrawingarea_) // Assume the wiregl window never resizes.
		return;

	AvGLCanvas *c = (AvGLCanvas *)clientData;
	GLwDrawingAreaCallbackStruct *resize =
				(GLwDrawingAreaCallbackStruct *) callData;

	printf("resizecb 0x%lx Size changed from: %d/%d to %d/%d\n",
		XtWindow(w), c->width_, c->height_,
		resize->width, resize->height);
	c->width_ = resize->width;
	c->height_ = resize->height;
	c->resized();
}

// Given a width & height, find the number of textures needed to cover them.
// Uses maxTextureSize_.
void AvGLCanvas::numTextures(const int width, const int height,
			     int &nx, int &ny)
{
	// # of textures along X axis.
	if(width <= maxTextureSize_)
		nx = 1;
	else
	{	nx = width/maxTextureSize_;
		if((width % maxTextureSize_) > 0)
			nx += 1;
	}

	// # of textures along Y axis.
	if(height <= maxTextureSize_)
		ny = 1;
	else
	{	ny = height/maxTextureSize_;
		if((height % maxTextureSize_) > 0)
			ny += 1;
	}
}

/*

On resize
	Reset viewport if not wiregl.
*/
// Called when window resized and after new width_/height_ have been set.
void AvGLCanvas::resized()
{
//	printf("resized\n");
	makeCurrent();

	if(!externaldrawingarea_)
	{	glViewport(0, 0, (GLsizei) width_, (GLsizei) height_);
		printf("glVP set to %d/%d\n", width_, height_);
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)width_, 0.0, (GLdouble)height_);

	glcheck();		// Check for GL errors.
}

// For wiregl, we need to know the size of the wiregl draw surface
// immediately. However, it isn't available until a window ID is created.
// The drawing area widget doesn't have a window until it's mapped. We
// can't map it now since upper level controls it. (Besides, its size
// gets messed up). This routine creates a dummy window, does a glXMakeCurrent
// to it, gets the needed information, then destroys the window.
// The window is small, made off screen and never mapped.
static const char *WIREGL_VENDOR = "Stanford Univeristy Graphics Lab";
void AvGLCanvas::glInfo(Display *dpy, XVisualInfo *vi, GLXContext ctx,
			GLint &width, GLint &height, GLint &maxtexturesize,
			int &externaldrawingarea)
{
	Window win = XCreateWindow(dpy, DefaultRootWindow(dpy),
		/* x    y  w  h  bw */
		 -50, -50, 1, 1, 0,	/* Small and off screen. */
		vi->depth, InputOutput /*CopyFromParent*/, vi->visual,
		0, NULL);

	glXMakeCurrent(dpy, win, ctx);
	printGLInfo();
	GLint vpstat[4];
	glGetIntegerv(GL_VIEWPORT, vpstat);
	printf("glInfo: GL VPstat %d,%d %d/%d\n",
		vpstat[0], vpstat[1], vpstat[2], vpstat[3]);
	width = vpstat[2];
	height = vpstat[3];
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxtexturesize);
	const char *vendor = (const char*)glGetString(GL_VENDOR);
	if(strcmp(vendor, WIREGL_VENDOR) == 0)
		externaldrawingarea = 1;
	else
		externaldrawingarea = 0;

	XDestroyWindow(dpy, win);
}

////////////////////////////////////////////////////////////////
// Set/Release the GL lock. NOPs if no thread support.
//   Make sure only one thread accesses the openGL library at once.
// Assumes all threads use the same context!
// Most calls to the OGL library don't use these. They are only required
// for parts that are threaded. Currently this is only imageRaster and
// imageTexture.

void AvGLCanvas::initGLReservation()
{
#if defined(PTHREADS)
static const char *name = "AvGLCanvas::initGLReservation";

	if(!access_inited)
	{	if(sem_init(&access_GL, 0, 1) == 0)
			access_inited = True;
		else
			perror(name);
	}
#endif
}

void AvGLCanvas::reserveGL()
{
#if defined(PTHREADS)
static const char *name = "AvGLCanvas::reserveGL";

	if(sem_wait(&access_GL) != 0)
		perror(name);
#endif
}

void AvGLCanvas::releaseGL()
{
#if defined(PTHREADS)
	if(sem_post(&access_GL) != 0)
		perror("AvGLCanvas::releaseGL:");
#endif
}
////////////////////////////////////////////////////////////////

AvGLCanvas::AvGLCanvas(const char * /* name*/, Widget ref, AvXPalette *pal,
		       Arg *arglst, int nargs)
{
int	i;
Arg	*args;

	ref_ = ref;
	width_ = height_ = 0;
	doubleBuffered_ = True;
	RGBVisual_ = True;
	exposedOnce_ = False;
	texturecache_ = NULL;		// Pointer to texture cache.
	busy_ = -1;				// See show.

	Display *dpy = XtDisplay(ref);
	visual_ = widgetVisual(ref);
	XVisualInfo *vi = getWidgetVisualInfo(ref);
	visualInfo_ = vi;
	initGLReservation();
	glcontext_ = makeGLContext(dpy, vi);

	GLint width, height, maxtexturesize;

	glInfo(dpy, vi, glcontext_->glxcontext(), width, height,
	       maxtexturesize, externaldrawingarea_);
	maxTextureSize_ = maxtexturesize;
	if(externaldrawingarea_ == 1)
	{	width_ = width;
		height_ = height;
		// WireGL seems to work better with smaller textures.
		// (Don't want texture objects to span pipeservers).
//		maxTextureSize_ = 512;
		maxTextureSize_ = 128;
	}	// Otherwise, set below.

#if 0
	printf("maxtexturesize %d, externaldrawingarea %d\n",
		maxTextureSize_, externaldrawingarea_);
#endif

	// This may be reset by child class.
	depth_ = widgetDepth(ref);
	if(pal == NULL)
		pal = new AvXPalette(XtDisplay(ref), visual_);
	palette_ = pal;
	palette_->ref();

	unsigned long whitePixelP, blackPixelP;
	// Allocate & get virtual and physical values.
	palette_->allocNamedColor("White", whitePixel_, whitePixelP);
	palette_->allocNamedColor("Black", blackPixel_, blackPixelP);

	// Initialize.

	AvGLGraphicContext *xgc = new AvGLGraphicContext(this);
	state_.setGraphicContext(xgc);

	i = nargs;
	args = new Arg[i+16];	// Plenty of room for my additions.
	if(nargs > 0)
		memcpy(args, arglst, sizeof(*arglst)*nargs);

	XtSetArg(args[i], GLwNvisualInfo, vi); i++;
	Widget wid = XtCreateManagedWidget("glxarea",
					   glwDrawingAreaWidgetClass, ref,
//					   glwMDrawingAreaWidgetClass, ref,
					   args, i);
	wid_ = wid;

	// Make a GC for clear. (To set areas to background color).
	if(externaldrawingarea_)
	{ XGCValues vals;
	  Pixel backgroundP;

		Window w = XtWindow(ref);
		if(w == 0)
			w = AvApp::referenceWindow();

		XtVaGetValues(wid_, XmNbackground, &backgroundP, NULL);
		vals.background = backgroundP;
		vals.foreground = backgroundP;
		xbgGC_ = XCreateGC(XtDisplay(ref), w,
					     GCForeground|GCForeground,
					     &vals);
	} else
	{ int wwidth, wheight;
		xbgGC_ = 0;
		// Use widget's declared width & height.
		getwidthheight(wid_, wwidth, wheight);
		width_ = (unsigned int) wwidth;
		height_ = (unsigned int) wheight;
	}

	delete [] args;

	{	XtAddCallback(wid, XmNexposeCallback,
			      (XtCallbackProc) exposecb, (XtPointer) this);
	}

	if(!externaldrawingarea_)
		XtAddCallback(wid, XmNresizeCallback,
			      (XtCallbackProc) resizecb, (XtPointer) this);

  /*  XtAddCallback(wid, XmNinputCallback, input, NULL);*/

	// This should reset gc to any user set value.
	// WireGL doesn't seem to like calling this before context is made.
//	setDrawColor(getDrawColor());

	rasterTime_ = 0;
	rasterBytes_ = 0;
//	cout << "AvGLCanvas made" << endl;
//	printGLInfo();
}

AvGLCanvas::~AvGLCanvas()
{
	if(palette_ != NULL)
		palette_->unref();

	if(texturecache_ != NULL)
		texturecache_->unref();
}

void AvGLCanvas::makeCurrent()
{
	glcontext_->makeCurrent(XtDisplay(wid_), XtWindow(wid_));
}

void AvGLCanvas::update()		// Redraw window.
{
  return;
  //	if(pm_ != NULL)
  //		copyRegion( *pm_, *this);
  //	else
	{ AvGraphics2D	*dob= vps_->getObject(0);
	  if(dob != NULL)
		show();
	}

  cerr << "AvGLCanvas::update() called" << endl;
}

void AvGLCanvas::show(AvGraphics2D *dob)
{
unsigned int w, h;
AvViewport *vp;

	// Needed even with wiregl since it needs a window to do makecurrent.
	if(!XtIsRealized(wid_))
	{//	printf("AvGLCanvas::show: Widget not realized yet.\n");
		return;
	}

	// Use whatever is actually on the list.
	dob = vps_->getObject(0);
	if(dob == NULL) {
	  printf("AvGLCanvas::show: NULL dob\n");
		return;
	}


	// If this is called as a result of a resize, we can get recursion
	// when drawRaster calls setBusy() which calls stall which allows
	// the expose event to get processed. We can't just remove the
	// call from AvImageWindow::showImage since it's followed by
	// a call to its drawBox() routine. Sooo.. We just ignore the
	// expose event if we're already busy.
	if(++busy_ > 0)
	{	busy_--;
		return;
	}

	// Get desired size and set the viewport size to match.
	AvAllotment r;
	getBoundingBox( dob, r);
	w = (unsigned int) r.value(AvDimensionX);
	h = (unsigned int) r.value(AvDimensionY);
	vps_->setSize(w, h);
	// Set the size of the main viewport to be that of the bb.
	vp = viewport();
	vp->width(w);
	vp->height(h);

	if(externaldrawingarea_)
		clearXarea(0, 0, w, h);

//	printf("AvGLCanvas::show showing. w/h %d/%d\n", w, h);

	// Just to make sure.
	makeCurrent();

	glcontext_->waitX();
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	AvCanvas::show( *vps_);

#if 1
	if (doubleBuffered_)
	{	Display *dpy = display();
		glXSwapBuffers(dpy, XtWindow(wid_));
//		printf("Swapped buffer\n\n");
	}
#endif
	glcontext_->waitGL();

	if(sync_)
		wait();		// Flush & wait.
	else
		glFlush();	// else, just flush.

	busy_--;
}

void AvGLCanvas::show(AvGraphics2D &dob)
{
	show(&dob);
}

float AvGLCanvas::convertScale(const float scale)
{
int	iscl;
float	scl;

	if(scale >= 1.0)
		scl = (float)((int) scale);
	else
	if(scale == 0.0)
		scl = 1.0;		// Error !!
	else
	{	iscl = (int)(1.0/scale + 0.5);
		scl = 1./(float)iscl;
	}

	return scl;
}

float AvGLCanvas::convertRasterScale(const float scale)const
{
	return convertScale(scale);
}

// Scale something. (Also used in drawRaster).
static inline unsigned int scalesize(unsigned int a, int scl)
{
	if(scl == 0)		// error
		return	a;
	else
	if(scl < 0)
		return a/(-scl);
	else
		return a*scl;
}

/* Convert floating point scale (>0..1..) to int (+/- num).
If:
	val is out of range, return closest valid value.
//val is < 0, error	return 1;
//( 0 <= val < minval)	return 0;
//( minval <= val < 1)	return -1/val rounded up.
//( 1 <= val < maxval)	return (int) val.
*/

static int getscl(const float value)
{
int	scl;
float	min, max, val = value;

	// Range check.
	min = AvViewport::minRasterScale();

	if( val < min)
		val = min;
	else
	{	max = AvViewport::maxRasterScale();
		if(val > max)
			val = max;
	}

	val = AvGLCanvas::convertScale(val); // Convert to 'integral' #.
	// Convert
	if(val < 1.0)
	{	val = -1.0/val;
		scl = (int)val;
	}
	else
		scl = (int) val;

	return scl;
}

unsigned int AvGLCanvas::scaleSize(const AvDimension d,
				   const unsigned int val)
{
float	xscl, yscl, scl;
int	newval;

	viewport()->getRasterScale(xscl, yscl);
	scl = (d == AvDimensionX) ? xscl : yscl;

	newval = (int)(val*convertRasterScale(scl));

	return (unsigned int) newval;
}

#if defined(ENABLE_TIMING)
// Clock time in secs.
static inline float hdeltaClock(const clock_t t0, const clock_t t1)
{
	return ((float)(t1-t0))/(float)CLK_TCK;
}

// CPU times in seconds.
static inline float deltaTimes( const struct tms &tms0,
				const struct tms &tms1)
{ clock_t ut = tms1.tms_utime - tms0.tms_utime;
  clock_t st = tms1.tms_stime - tms0.tms_stime;

	return ((float)(st+ut))/(float)CLK_TCK;
}
#endif

/*
Draw a raster into the drawable. Since this is used exclusively to
draw a complete scaled raster into a pixmap, there are no arguments
for subranging.

Experimental thread support:
	If the number of requested threads is > 1, the sequence
		read data.
		scale data and store in XImage structure.
		send pixels to server.
	is broken into
		read data.
			foreach thread
				scale/store 1/n of the data.
				send pixels to server.
 When nthreads is > 1, there will be upto nthreads-1 pthreads created. The
main thread does some of the work. If the max(#input data points, #output
pixels) is < kbPerThread (a settable value), the number of threads is
reduced.

Trying to break up the data read currently isn't done because:
	It tends to be the fastest part (at least with FITS files).
	AIPS++ slows way down on small reads.
	Haven't had time.

*/

#if 0
static void printb(const float *b, const int width, const int height,
		   const int xstart=0, const int ystart=0,
		   const int NX=16, const int NY=16, const int size=3)
{ const int Width = width*size;
  int y0 = Width*ystart;
  int x0 = xstart*size;
  int nx = ((NX <= 0) || (NX>(width-xstart))) ? (width-xstart) : NX;
  int ny = ((NX <= 0) || (NY>(height-ystart))) ? (height-ystart) : NY;

	for(int r=0; r < ny; r++)
	{ int yoff = y0 + r*Width;
	  const float *ptr = b+yoff + x0;
		for(int c=0; c< nx; c++)
		{	for(int n=0; n < size; n++)
				printf("%.2f/", *ptr++);
			printf(" ");
		}
		printf("\n");
	}
}
#endif

// Call palette to convert from data to pixels then draw them.
void AvGLCanvas::drawPixels(const float *inbuf,
			    const int iWidth, const int iHeight,
			    const int nrows, const int ncols,
			    GLushort *obuf,
			    const int xscl, const int yscl,
			    const float mn, const float mx,
			    const unsigned long undercolor,
			    const unsigned long overcolor,
			    const unsigned long blankcolor,
			    const float blankvalue,
			    const int dstx, const int dsty)
{

#if 0
printf("dP: x/yscl %d/%d diff %d %d nrows/cols %d/%d dsty %d iW/IH %d/%d\n",
		xscl, yscl, bend-obuf, iend-inbuf,
		nrows, ncols, dsty, iWidth, iHeight);
#endif

	palette_->scaleFToRGB(inbuf, iWidth, nrows, ncols,
			      obuf,
			      mn, mx, undercolor, overcolor,
			      blankcolor, blankvalue);
	reserveGL();
	 glRasterPos2i(dstx, dsty);
	 // Oops. Doesn't work with WireGL!
	 glPixelZoom((GLfloat)xscl, (GLfloat)yscl);
	 glDrawPixels(iWidth, iHeight, GL_RGB, GL_UNSIGNED_SHORT, obuf);
	glcheck();
	releaseGL();
}

#if defined(PTHREADS)

// Used to pass scaling information for thread use.
struct scaleStruct {
	unsigned	num;		// Thread #.
	pthread_t	id;		// Thread id.
	AvGLCanvas	*canvas;
	float		*in;		// Data to be scaled.
	int		iWidth, iHeight;// in's width & height.
#if defined(USEFLOATBUF)
	float		*out;		// Output data
#else
	GLushort	*out;		// Output data
#endif
	int		nrows, ncols;	// # of row/cols to do.
	int		xscl, yscl;	// Scale factors.
	float		min, max;	// Min/max for input data.
	unsigned long undercolor, overcolor, blankcolor;
	float blankval;
	int	dstX, dstY;
};

void *AvGLCanvas::threadScale( void *arg)
{ scaleStruct *s = (scaleStruct *) arg;
  AvGLCanvas *c = s->canvas;

	c->drawPixels(s->in, s->iWidth, s->iHeight,
		      s->nrows, s->ncols,
		      s->out, s->xscl, s->yscl,
		      s->min, s->max, s->undercolor, s->overcolor,
		      s->blankcolor, s->blankval, s->dstX, s->dstY);
	return (0);
}
#endif

// Convert data buffer to pixels.
//	Uses drawPixels
void AvGLCanvas::imageRaster(AvRaster &img, const int dstx, const int dsty,
			     const float Xscale, const float Yscale)
{
int	srcx=0, srcy=0;
unsigned int iwidth, iheight;	// src image width/height
float	mn, mx;
int	sxinc, syinc;
int	xscl, yscl;
unsigned long undercolor, overcolor, blankcolor;
#if defined(PTHREADS)
Bool	showTimings = getXOptions()->showTimings; // Print scaling times?
int	nthreads = getXOptions()->nThreads;	  // # of threads to use.
int	kbPerThread = getXOptions()->kbPerThread; // min kb/thread.
#endif

//	cout << "AvGLCanvas::imageRaster" << endl;

	if(dsty < 0)
		printf("dsty is %d\n", dsty);

	// If the drawable is missing, punt.
	if(drawable() == 0)
		return;
	//		Range checks
	iwidth = img.width();		// Size of input.
	iheight= img.height();

	// Convert to signed ints.
	xscl = getscl(Xscale);
	yscl = getscl(Yscale);
	sxinc = (xscl >= 0) ? 1 : -xscl;
	syinc = (yscl >= 0) ? 1 : -yscl;

	// What colors to use for out of range pixels.
	// Use b/w normally, but if that won't work, use 0, max-1.
	// (Or if the environment variable is set).
	if( (palette()->availableColors() <= palette()->maxColors()-2)
				&&
		(getEnvInt(CLIPTO_COLOR, 0) == 0) )

	{	undercolor = blackPixel();
		overcolor = whitePixel();
		blankcolor = undercolor;
	}
	else
	{	undercolor = 0;
		overcolor = palette()->availableColors() -1; // Highest index.
		blankcolor = overcolor;
	}

	mn = img.min();		// Data min/max
	mx = img.max();

	float blankvalue = img.getBlankValue();

	/////////////////////////////////////////////////
	// How many pixels in scaled image?
	const int oWidth = scalesize(iwidth, xscl);
	const int oHeight = scalesize(iheight, yscl);

	/////////////////////////////////////////////////
	// Figure out the size of the input buffer.

	// # pixels/row we deal with.
	const int iw = (xscl >= 0) ? iwidth : oWidth;
	// # input rows we deal with.
	const int ih = (yscl >= 0) ? iheight: oHeight;
	const int XSCL = (xscl > 0) ? xscl : 1;
	const int YSCL = (yscl > 0) ? yscl: 1;

	// # of elements to be retrieved from raster.
	const int NPIXELS = iw*ih;
	float *ibuffer = new float[NPIXELS];		 // Input data buffer.
	GLushort *obuffer = new GLushort [NPIXELS*3+1]; // Output buffer: RGB
	obuffer[NPIXELS*3] = 0x0F0F;			// Overrun check.

	// Get the image & scale it.
	// Expansion is done by OGL, reduction is done in the raster.
#if defined(ENABLE_TIMING)
struct tms htime0;
clock_t hclock0 = times(&htime0);
#endif
	img.beginAccess();
	 img.getRegion(ibuffer, srcx, srcy, iw, ih, sxinc, syinc);
	img.endAccess();
#if defined(ENABLE_TIMING)
struct tms htime1;
clock_t hclock1 = times(&htime1);
#endif

#if defined(PTHREADS)
	// Use OMP's environment variable if it is not overridden by ours.
	if( nthreads == 0)
		nthreads = getEnvInt("OMP_NUM_THREADS", 1);// # of threads, OMP.

	{ // If the # of pixels processed per thread is too small, reduce the
	  // # of threads.
	  int npD = oHeight*oWidth;
	  int npS = iw*ih;
	  int np = (npD >= npS) ? npD : npS;	// Max of Destination/Src.
	  int npT = np/(nthreads*1024);
		if(npT < kbPerThread)
			nthreads = npT/kbPerThread;
	}

	if( nthreads > 1)
	{ int h_per_thread = ih/nthreads;
#if defined(USEFLOATBUF)
	  float *iptr = ibuffer;
	  float *optr = obuffer;
#else
	  float *iptr = ibuffer;
	  GLushort *optr = obuffer;
#endif
	  const int PIXELS_per_thread = iw*h_per_thread;
	  const int RGB_per_thread = PIXELS_per_thread*3;

	  // The main program is counted as 1 thread.
	  int npthreads = nthreads-1;
	  scaleStruct *scales = new scaleStruct[npthreads];
	  int nt;		// for loop index;
	  int y0 = dsty;
	  int nOrows = 0;
	  int totalIRows = 0;

#if defined(__sgi)
		// It looks like IRIX may default to 1 processor otherwise.
		pthread_setconcurrency(nthreads);
#endif
		for( nt=0; nt < npthreads; nt++)
		{ scaleStruct *s = &scales[nt];
			s->num = nt;
			s->canvas = this;
			s->in = iptr;
			iptr += PIXELS_per_thread;
			s->iWidth = iw;
			s->iHeight = h_per_thread;
			totalIRows += h_per_thread;
			s->out = optr;
			optr += RGB_per_thread;
			s->ncols = iw;
			s->nrows = h_per_thread;
			nOrows += h_per_thread;
			s->xscl = XSCL;
			s->yscl = YSCL;
			s->min = mn;
			s->max = mx;
			s->undercolor = undercolor;
			s->overcolor = overcolor;
			s->blankcolor = blankcolor;
			s->blankval = blankvalue;
			s->dstX = dstx;
			s->dstY = y0;
			y0 += h_per_thread*YSCL; //oROWS_per_thread;

			int pterr = pthread_create(&s->id, NULL,
					threadScale, (void *) s);
			if(pterr)
				perror("AvGLCanvas:");
		}

		int nInrows = ih - totalIRows;
		// Main thread handles what's left over.
#if !defined(ALTDRAWP)
		drawPixels(iptr, iw, nInrows, nInrows, iw,  optr,
			   XSCL, YSCL, mn, mx, undercolor, overcolor,
			   blankcolor, blankvalue, dstx, y0);

#endif

		// Wait for all to be done.
		for( nt = 0; nt < npthreads; nt++)
		{	pthread_join(scales[nt].id, NULL);
		}
#if defined(ALTDRAWP)
		drawPixels(iptr, iw, nInrows, iw, nInrows, optr,
			   XSCL, YSCL, mn, mx, undercolor, overcolor,
			   blankcolor, blankvalue, dstx, y0);
#endif

		delete [] scales;	// Delete thread structs
	}
	else
#endif	// ifdef PTHREADS
	{
		drawPixels(ibuffer, iw, ih, iw, ih, obuffer,
			   XSCL, YSCL, mn, mx, undercolor, overcolor,
			   blankcolor, blankvalue, dstx, dsty);

		if(obuffer[NPIXELS*3] != 0x0F0F)
			printf("Buffer overrun\n");
	}

	{int errs = glcheck();
		if(errs != 0)
			printf("%d GL ERRORS\n", errs);
	}
#if defined(ENABLE_TIMING)
	if(showTimings)
	{ struct tms htime2;
	  Display *dpy = display();

		XSync(dpy, False);	// Only when timing.
		clock_t hclock2 = times(&htime2);

		// Display timings.
		float oBytes=oWidth*oHeight;
		int nthr = (nthreads > 1) ? nthreads : 1;

		printf("Pixel scale/store and write for %d thread%s\n",
		       nthr, ((nthr == 1)? "." : "s."));
		float deltaReadWall=hdeltaClock(hclock0, hclock1);
		float deltaReadCPU=deltaTimes(htime0, htime1);
		printf("\t\t\tWall\t\tCPU\n");
		printf("Read %5d bytes\t%.3fs\t\t%.3fs\n",
			NPIXELS, deltaReadWall, deltaReadCPU);

		float deltaScaleWall= hdeltaClock(hclock1, hclock2);
		float deltaScaleCPU = deltaTimes(htime1, htime2);
		printf("Scale/store/Put\t\t%.3fs\t\t%.3fs\n",
			deltaScaleWall, deltaScaleCPU);
		printf(" %5d pixels\n", (int)oBytes);

		float deltaWall=deltaReadWall+deltaScaleWall;
		float deltaCPU=deltaReadCPU+deltaScaleCPU;
		printf("Total\t\t\t%.3fs\t\t%.3fs\n",
			deltaWall, deltaCPU);
		printf("Output\t\t\t%.2fkb/s\t%.2fkb/s\n",
			oBytes/(1000.0*deltaWall), oBytes/(1000.0*deltaCPU));
	}
#endif

	delete [] ibuffer;
	delete [] obuffer;
}


//	Create and return a list of textures containing the image.
// If tobjsin is NULL, a new texture object is created and loaded with
// data. If it isn't NULL, it is updated with current position and scale
// information.
// Returns the texture object used.
AvTextureObjects *AvGLCanvas::textureRaster(AvRaster &img,
					const int dstx, const int dsty,
					const float Xscale, const float Yscale,
					AvTextureObjects *tobjsin)
{
int	srcx=0, srcy=0;
unsigned int iwidth, iheight;	// src image width/height
float	mn, mx;
int	sxinc, syinc;
int	xscl, yscl;
unsigned long undercolor, overcolor, blankcolor;

#if defined(PTHREADS)
Bool	showTimings = getXOptions()->showTimings; // Print scaling times?
int	nthreads = getXOptions()->nThreads;	  // # of threads to use.
//int	kbPerThread = getXOptions()->kbPerThread; // min kb/thread.
#endif

//	cout << "AvGLCanvas::textureRaster" << endl;

	if(dsty < 0)
		printf("dsty is %d\n", dsty);

	// If the drawable is missing, punt.
	if(drawable() == 0)
		return NULL;
	//		Range checks
	iwidth = img.width();		// Size of input.
	iheight= img.height();

	// Convert to signed ints.
	xscl = getscl(Xscale);
	yscl = getscl(Yscale);
	sxinc = (xscl >= 0) ? 1 : -xscl;
	syinc = (yscl >= 0) ? 1 : -yscl;

	// What colors to use for out of range pixels.
	// Use b/w normally, but if that won't work, use 0, max-1.
	// (Or if the environment variable is set).
	if( (palette()->availableColors() <= palette()->maxColors()-2)
				&&
		(getEnvInt(CLIPTO_COLOR, 0) == 0) )

	{	undercolor = blackPixel();
		overcolor = whitePixel();
		blankcolor = undercolor;
	}
	else
	{	undercolor = 0;
		overcolor = palette()->availableColors() -1; // Highest index.
		blankcolor = overcolor;
	}

	mn = img.min();		// Data min/max
	mx = img.max();

	float blankvalue = img.getBlankValue();

////////////////////////////////////////////////////////////////////
#if defined FUDGE
	static const int XSIZE = 64;
	static const int YSIZE = 64;
	iwidth = XSIZE;
	iheight = YSIZE;
	mn = 0;
	mx = 1.0;
#endif
////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////
	// How many pixels in scaled image?
	const int oWidth = scalesize(iwidth, xscl);
	const int oHeight = scalesize(iheight, yscl);

	/////////////////////////////////////////////////
	// Figure out the size of the input buffer.

	// # pixels/row we deal with.
	const int iw = (xscl >= 0) ? iwidth : oWidth;
	// # input rows we deal with.
	const int ih = (yscl >= 0) ? iheight: oHeight;
	const int XSCL = (xscl > 0) ? xscl : 1;
	const int YSCL = (yscl > 0) ? yscl: 1;

	// # of elements to be retrieved from raster.
	const int NPIXELS = iw*ih;
	float *ibuffer = new float[NPIXELS];		// Input data buffer.
	GLushort *obuffer = new GLushort [NPIXELS*3+1]; // Output buffer: RGB
	obuffer[NPIXELS*3] = 0x0F0F;			// Overrun flag.
	GLushort *oend = obuffer+NPIXELS*3;

	// Get the image & scale it.
	// Expansion is done by OGL, reduction is done in the raster.

#if !defined(FUDGE)
#if defined(ENABLE_TIMING)
struct tms htime0;
clock_t hclock0 = times(&htime0);
#endif
	img.beginAccess();
	 img.getRegion(ibuffer, srcx, srcy, iw, ih, sxinc, syinc);
	img.endAccess();
#if defined(ENABLE_TIMING)
struct tms htime1;
clock_t hclock1 = times(&htime1);
#endif
#else	// FUDGE
////////////////////////////////////////////////////////////////
	{ int i = 0;
		for(int c= 0; c < ih; c++)
			for(int r=0; r< iw; r++)
				ibuffer[i++] = c*iw + r;
	}

////////////////////////////////////////////////////////////////
int pixnum=0;
#endif

	int nXtextures, nYtextures;
	// # of textures to use.
	numTextures(iw, ih, nXtextures, nYtextures);
//	printf("Need %d x %d textures\n", nXtextures, nYtextures);
	int numtextures = nXtextures*nYtextures;
	AvTextureObjects *tobjs;
	if(tobjsin == NULL)
		tobjs = new AvTextureObjects(numtextures);
	else
		tobjs = tobjsin;

//	float *iptr=ibuffer, *ibufend=ibuffer+NPIXELS;
	float *iptr=ibuffer;
	GLushort *optr=obuffer;
	// Tile size to use.
	int tWidth = iw/nXtextures;
		if((iw % nXtextures) != 0)	// Make sure we cover it.
			tWidth += 1;
	int tHeight = ih/nYtextures;
		if((ih % nYtextures) != 0)
			tHeight += 1;
	int height = tHeight;
	int tindex = 0;
	int knt = 0;
	int currY = 0;

#if 0
	printf("iw/ih %d/%d, tWidth/tHeight %d/%d\n", iw, ih,
		tWidth, tHeight);
#endif

	while(height > 0)
	{ float *rowend = iptr + iw, *ptr=iptr;
	  int width = tWidth;
	  int currX = 0;
	  AvTextureObject *tobj;

		while( ptr < rowend)
		{	// If we've created the textures, load them.
			// Otherwise, just update position and scale.
			if(tobjsin == NULL)
			{
#if !defined(FUDGE)
				palette_->scaleFToRGB(ptr, iw,
						width, height, optr,
						mn, mx,  undercolor, overcolor,
						blankcolor, blankvalue);
#else
////////////////////////////////////////////////////////////////
	{ printf("w %d h%d p0 %d %d/%d\n",
		 width, height, pixnum, currX, currY);
	  GLushort *ooptr = optr;
		for(int r= 0; r < height; r++)
		 for(int c=0; c< width; c++)
		 {	*ooptr++ = c + currX;
			*ooptr++ = r + currY;
			*ooptr++ = pixnum++;
		 }
	}
////////////////////////////////////////////////////////////////

#endif
				tindex = tobjs->makeTexture(width, height);
				if(tindex < 0)
				{	printf(
		  "AvGLCanvas::textureRaster: No more textures at knt=%d/%d\n",
						knt, numtextures);
					break;
				}
				tobj = tobjs->getTexture(tindex);
				tobj->storeTexture(0, 0, width, height, optr);
				optr += width*height*3;

				if(optr > oend)
					printf(
			      "AvGLCanvas::textureRaster: Obuf overflow: %d\n",
						optr-oend);
				knt += 1;
			} else
				tobj = tobjs->getTexture(tindex++);
	
			if(tobj != NULL)
			{	int xpos = currX*XSCL + dstx;
				int ypos = currY*YSCL + dsty;
				tobj->locateTexture(xpos, ypos, 
						 (GLfloat)XSCL, (GLfloat)YSCL);
			} else
			{	printf(
		"AvGLCanvas::textureRaster: Null texture object at index %d\n",
				tindex-1);
			}
			ptr += width;
			currX += width;
			// Last in row may be narrow.
			if((rowend - ptr) < width)
				width = rowend - ptr;
		}

		iptr += iw*height;
		currY += height;
		if((currY + height) > ih)	// Last row may be short.
		{	height = ih - currY;
		}
	}

	{int errs = glcheck();
		if(errs != 0)
			printf("AvGLCanvas::textureRaster: %d GL ERRORS\n",
				errs);
	}

#if defined(ENABLE_TIMING)
	if(showTimings)
	{ struct tms htime2;
		XSync(display(), False);	// Only when timing.
		clock_t hclock2 = times(&htime2);

		// Display timings.
		float oBytes=oWidth*oHeight;
		int nthr = (nthreads > 1) ? nthreads : 1;

		printf("Pixel scale/store and write for %d thread%s\n",
		       nthr, ((nthr == 1)? "." : "s."));
		float deltaReadWall=hdeltaClock(hclock0, hclock1);
		float deltaReadCPU=deltaTimes(htime0, htime1);
		printf("\t\t\tWall\t\tCPU\n");
		printf("Read %5d bytes\t%.3fs\t\t%.3fs\n",
			NPIXELS, deltaReadWall, deltaReadCPU);

		float deltaScaleWall= hdeltaClock(hclock1, hclock2);
		float deltaScaleCPU = deltaTimes(htime1, htime2);
		printf("Scale/store/Put\t\t%.3fs\t\t%.3fs\n",
			deltaScaleWall, deltaScaleCPU);
		printf(" %5d pixels\n", (int)oBytes);

		float deltaWall=deltaReadWall+deltaScaleWall;
		float deltaCPU=deltaReadCPU+deltaScaleCPU;
		printf("Total\t\t\t%.3fs\t\t%.3fs\n",
			deltaWall, deltaCPU);
		printf("Output\t\t\t%.2fkb/s\t%.2fkb/s\n",
			oBytes/(1000.0*deltaWall), oBytes/(1000.0*deltaCPU));
	}
#endif

	delete [] ibuffer;
	delete [] obuffer;
	return tobjs;
}

// Convert from VP y to X y coordinate.
static inline int vpY2XY_(const float xh, const float vpy)
{
	return (int)(xh - vpy + 0.5);
}

static inline int vpX2XX_(const float vpx, const float xoffset)
{
	return (int)(vpx + xoffset + 0.5);
}

// Sort of arbitrary number of seconds above which we'll display the
// busy flags.
static const float LONGTIME = .025;	// 25ms.

/*
srcx, srcy	Where in raster image the region should come from.
		(Normally 0 since entire image is drawn).
dstx,dsty	Offset within current viewport (Usually 0).
swidth,		How much of the image to draw. Usually 0 to indicate draw
sheight		as much as possible.


If the time to scale and dislay an image is "long", inform the user. If
the time is fairly quick, don't incur the overhead.  The check is on a per
image basis, and doesn't consider multiple image redisplays.

*/

void AvGLCanvas::drawRaster(AvRaster &img,
			    int /*srcx*/, int /*srcy*/,
			    int /*dstx*/, int /*dsty*/,
			    unsigned int /*swidth*/, unsigned int /*sheight*/)
				// Requested width/height.
{
unsigned int iwidth, iheight;	// src image width/height
int	dstx, dsty;
float	xfact, yfact;
AvViewport *vp;
time_t	time0;
clock_t	clock0;
Boolean	timeit=False;
Boolean showBusy = False;

// printf("AvGLCanvas::drawRaster\n");
	// Pick up current scaling.
	vp = viewport();
	vp->getRasterScale(xfact, yfact);

	// Get the size that imageRaster would(will) use.
	iwidth = img.width();
	iheight = img.height();
	iwidth = scaleSize(AvDimensionX, iwidth);
	iheight = scaleSize(AvDimensionY, iheight);
	dstx = vp->x0();
	dsty = vp->y0();

	int npixels = iwidth*iheight;
	if(npixels == 0)
		return;

//printf("dR: VP: width %d, height %d; Scaled Image w %d h %d\n",
//vp->width(), vp->height(), iwidth, iheight);

	// If the number of pixels has changed, force a retime.
	if(npixels != (int)rasterBytes_)
		timeit = True;

	if(timeit)
	{	// Time both cpu and elapsed time. Since network overhead
		// might be a factor.
		time0 = time(NULL);
		clock0 = clock();
	}

	AvTextureObjects *tobjs = NULL;

	if(cachingRasters())
	{	// Look for an existing entry.
		AvRasterCacheKey key(&img, iwidth, iheight);
		tobjs = texturecache_->find(key);
		if(tobjs == NULL)
		{// Not found. Copy raster to textures.

			if(rasterTime_ >= LONGTIME)
			{	showBusy = True;
				AvXColorApp::setBusy();
			}

			tobjs = textureRaster(img, dstx, dsty, xfact, yfact);
			// Save in cache.
			texturecache_->insert(key, tobjs);
		} else
			tobjs = textureRaster(img, dstx, dsty, xfact, yfact,
					      tobjs);
//		delete key;
	} else
		tobjs = textureRaster(img, dstx, dsty, xfact, yfact);

	if(tobjs != NULL)
		tobjs->draw();

	if(timeit)
	{ time_t time1 = time(NULL);
	  clock_t clock1 = clock();

		float walltime = (float)(time1-time0);	// Clock time.
						// Will be 0 in most cases.
		float cputime = (float)(clock1-clock0)/CLOCKS_PER_SEC;
		if(walltime > cputime)
			rasterTime_ = walltime;
		else
			rasterTime_ = cputime;
		rasterBytes_ = npixels;
	}
	if(!cachingRasters())
		delete tobjs;

	if(showBusy)
		AvXColorApp::resetBusy();
}

void AvGLCanvas::drawRaster(AvRaster *img)
{
	if(img == NULL)
		return;
	drawRaster( *img);
}

// Set the viewport, then set clip plane.
void AvGLCanvas::setViewport(AvViewport &vp)
{
	AvCanvas::setViewport(vp);
//	if(changeFullState())
//		setClipPlane();
}

// Reset state, then set clip plane.
void AvGLCanvas::popState()
{
	AvCanvas::popState();
//	if(changeFullState())
//		setClipPlane( viewport());
}

void AvGLCanvas::clear(int x, int y, unsigned int wdth, unsigned int hght)
{
unsigned int	W, H;
//Drawable d = drawable();

	printf("AvGLCanvas::clear x/y, %d/%d, w/h %d/%d\n", x, y, wdth, hght);

	if( drawable() == (Drawable) NULL)	// Not mapped yet.
		return;
	if((x < 0) || (x >= (int)width()))
		return;
	if((y < 0) || (y >= (int)height()))
		return;

	W = width() - x;
	if((wdth > 0) && (wdth < W))
		W = wdth;
	H = height() - y;
	if((hght > 0) && (hght < H))
		H = hght;

//	XFillRectangle(display(), drawable(), xbgGC_, x, y, W, H);
}

void AvGLCanvas::clearXarea(const int x, const int y,
			    const unsigned int wdth, const unsigned int hght)
{
unsigned int	W, H;
//Drawable d = drawable();

//	printf("AvGLCanvas::clearXarea x/y, %d/%d, w/h %d/%d\n",
//		x, y, wdth, hght);

	if( drawable() == (Drawable) NULL)	// Not mapped yet.
		return;
	if((x < 0) || (x >= (int)width()))
		return;
	if((y < 0) || (y >= (int)height()))
		return;

	W = width() - x;
	if((wdth > 0) && (wdth < W))
		W = wdth;
	H = height() - y;
	if((hght > 0) && (hght < H))
		H = hght;

	XFillRectangle(display(), drawable(), xbgGC_, x, y, W, H);
}

// Set the color used to clear drawables. If physical is TRUE,
// ci is a physical color index, otherwise it is a virtual color.
// (We should be using the AvGraphicContext, but that needs work).
void AvGLCanvas::setClearColor(const unsigned long ci, const Boolean physical)
{unsigned long color;

	printf("AvGLCanvas::setClearColor: %ld, %c\n", ci,
		((physical) ? 'T' : 'F'));

	if(physical)
		color = ci;
	else
		palette_->virtualToPhysical(ci, color);
}


/********************************************************************/
/********************************************************************/
/*			Support routines			    */
/********************************************************************/
/********************************************************************/
/********************************************************************/

/************************************************************************/
/*			Polyline drawing				*/
/************************************************************************/

/* Convert from fractional value (0..1) to vp coords (0.. size-1).
	If scale is NULL, nothing is done.
*/
inline static void frac2full( float &x, float &y, float *scale)
{
	if(scale != NULL)
	{	x *= scale[0];
		y *= scale[1];
	}
}

/* Convert from vp to X coords. Y is flipped since vp (0,0) is in the
lower left, while X's (0,0) is in the upper right. Then the X/Y offset
of the viewport in the X window is added.
*/

#if 0
static int pc = 0;
#endif

//	Used for segments.
#if 0
inline static void vp2XSeg(XSegment *lst,
	const float X1, const float Y1,
	const float X2, const float Y2,
	const float xoff, const float yoff, const float maxY)
{
float	x1, y1, x2, y2;

	x1 = vpX2XX_(X1, xoff);
	y1 = vpY2XY_(maxY, Y1 + yoff);
	lst->x1 = (short)x1;
	lst->y1 = (short)y1;

	x2 = vpX2XX_(X2, xoff);
	y2 = vpY2XY_(maxY, Y2 + yoff);
	lst->x2 = (short)x2;
	lst->y2 = (short)y2;

#if 0
// The pgplot driver does segments.
if(pc++ < 4)
 printf(
  "vp %.2f/%.2f->%.2f/%.2f, %.2f/%.2f->%.2f/%.2f off %.2f/%.2f maxY %.2f\n",
		X1,Y1, x1, y1, X2,Y2, x2, y2, xoff, yoff, maxY);
#endif
}
#endif

inline static void vp2GLPnt(const float xin, const float yin,
			   const float xoff, const float yoff,
			   float &xout, float &yout)
{
	xout = xin + xoff;
	yout = yin + yoff;
}

inline static void vp2GLSeg(float lst[4],
	const float X1, const float Y1,
	const float X2, const float Y2,
	const float xoff, const float yoff)
{
float	x1, y1, x2, y2;

	x1 = X1 + xoff;
	y1 = Y1 + yoff;

	lst[0] = x1;
	lst[1] = y1;

	//	x2 = vpX2XX_(X2, xoff);
	//	y2 = vpY2XY_(maxY, Y2 + yoff);
	x2 = X2 + xoff;
	y2 = Y2 + yoff;
	lst[2] = x2;
	lst[3] = y2;
}

//	Used for circles and arcs.
inline static void vp2XArc(XArc *lst,
	const float xvp, const float yvp,
	const float width, const float height,
	const float angle1, const float angle2,
	const float xoff, const float yoff, const float maxY)
{
float	x, y;

	x = vpX2XX_(xvp, xoff);
	y = vpY2XY_(maxY, yvp + yoff);
	lst->x = (short)x;
	lst->y = (short)y;
	lst->width = (unsigned short) width;
	lst->height = (unsigned short) height;
	lst->angle1 = (short)(angle1*64.0);	// X arcs take degrees*64.
	lst->angle2 = (short)(angle2*64.0);
}

/* Converts from relative to absolute by adding the current (relative)
position to the previous position. Both positions are set to the
new value.
*/
inline static void rel2abs(float &xcurr, float &ycurr,
				float &xprev, float &yprev)
{
	xcurr += xprev;
	ycurr += yprev;
	xprev =  xcurr;
	yprev =  ycurr;
}

#if 0
struct {
	int	n;
	char	*name;
	} cmds[] = {
		{NONE, "NONE"},
		{MOVE, "MOVE"},
		{POINT, "POINT"},
		{LINE, "LINE"},
		{RECTANGLE, "RECTANGLE"},
		{POLYGON, "POLYGON"},
		{SEGMENT, "SEGMENT"},
		{CIRCLE, "CIRCLE"},
		{ARC, "ARC"}
	};
#define NCMDS (sizeof(cmds)/sizeof(*cmds))

static char *cmd2str(const int mode)
{
	for(int i=0; i< NCMDS; i++)
		if(cmds[i].n == mode)
			return cmds[i].name;

	return "Unknown";
}
#endif

/* Draw polylines (points, lines, polygons, etc). Moves are handled by
updating the current position. X draws are done in absolute mode, but
at some point may be done in relative mode since it will be easier to
cache the values when that support is added later.
*/
static void MMf( const float a, float &min, float &max) {
	if(a < min)
		min = a;
	else
	if(a > max)
		max = a;
}

#if 0

static void MM( const short a, int &min, int &max) {
	if(a < min)
		min = a;
	else
	if(a > max)
		max = a;
}

static void PMINMAX(XSegment *slst, const int numpnts)
{ XSegment *ptr = slst;

	int xmin = 327680;
	int ymin = xmin;
	int xmax = -xmin;
	int ymax = xmax;

	for(int i=0; i< numpnts; i++, ptr++)
	{	MM(ptr->x1, xmin, xmax);
		MM(ptr->x2, xmin, xmax);
		MM(ptr->y1, ymin, ymax);
		MM(ptr->y2, ymin, ymax);
	}
	printf("%d: %d %d %d %d\n", numpnts, xmin, xmax, ymin, ymax);
}
#endif

void AvGLCanvas::drawVector(AvPolylineCmd *cmd)
{
float		maxY, maxPos[2], *scale;
float		xoff, yoff;	// Offset of viewport.
long		numpnts;
Boolean		relative, fractional;
AvViewport	*vp;
PointList	*points;	// Pointer to list of points.

	if((cmd == NULL) || ((points = cmd->positionList()) == NULL))
		return;

	if((numpnts = points->size()) <= 0)
		return;

//	printf("AvGLCanvas::drawVector %d points\n", numpnts);
#if 0
pc = 0;	// For segment tracing.
#endif
	vp = viewport();

	// Size of viewport.
	maxPos[0] = vp->width() -1;
	maxPos[1] = vp->height() - 1;
	maxY = height() - 1;

	// Offset of the viewport within X window.
	xoff = vp->x0();
	yoff = vp->y0();

	relative = (cmd->mode_ == AvPolyline::ABSOLUTE) ? FALSE : TRUE;
	fractional = (cmd->cmode_ == AvPolyline::FRACTIONAL);
	scale = fractional ? maxPos : (float*)NULL;

	/* For each point we need to:
		Convert to viewport coords if its fractional.
		Convert to absolute coords if its relative.
		Update the current position.
		Convert from absolute vp coords to X by
		Adding 'offset' to position within the pixmap and subtracting
		from height to flip the y 0 position.
		Convert to XPoint(etc.) and add to list.
	*/

	points = cmd->positionList();	// Pointer to the points.

	/* For relative to absolute conversion need to keep track of the
	   previous point. The first previous point is the initial position.
	*/
	AvAllotment currentPosition = vp->vectorPosition();
	float currX = currentPosition.x();
	float prevX = currX;
	float currY = currentPosition.y();
	float prevY = currY;

	switch(cmd->cmd_) {
	case MOVE:	// Compute new current point.
		points->getValue(0, &currX, &currY);
		frac2full(currX, currY, scale);
		if(relative)
			rel2abs(currX, currY, prevX, prevY);
		printf("Move\n");
		break;
	case LINE:
	case POLYGON:
	case POINT:

/* NOTES: Need to check. We may be drawing an extra point (at the start).
	(lines, polygons and points probably don't need exactly the same
lists).
Rectangles, circles, polygons, etc, probably shouldn't update the current
point.
*/

		{ GLenum mode;

			switch( cmd->cmd_) {
			case LINE:
				printf("LINE.\n");
				mode = GL_LINE_STRIP;
				break;
			case POLYGON:
				// GL_FRONT_AND_BACK
				if(cmd->filled_)
				{	mode = GL_POLYGON;
//					printf("POLYGON - filled\n");
					glPolygonMode(GL_FRONT, GL_FILL);
				}
				else
				{	mode = GL_LINE_LOOP;
//					printf("POLYGON - unfilled\n");
//					glPolygonMode(GL_FRONT, GL_FILL);
				}
				break;
			case POINT:
				printf("POINT\n");
				mode = GL_POINTS;
				break;
			default:;
			}

		  float newx, newy;

			glBegin(mode);
			  /* The initial draw is from the current position to
			     the first point.
			  */
#if 0
			// PGPLOT is sending 4 points for contour label
			// background. This is redundant and breaks it.
			vp2GLPnt(prevX, prevY, xoff, yoff, newx, newy);
			printf("POLY: %.2f/%.2f, %.2f/%.2f\n",
				prevX, prevY, newx, newy);
			glVertex2f(newx, newy);
#endif
			for( int i=0; i< numpnts; i++)
			{	// Get x/y from list.
				points->getValue(i, &currX, &currY);
				// Convert to vp from fractional.
				frac2full(currX, currY, scale);
				if(relative)
					rel2abs(currX, currY, prevX, prevY);

				// convert to x coord and store in list.
				vp2GLPnt(currX, currY, xoff, yoff,
					 newx, newy);
#if 0
				printf("POLY: %f/%f, %f/%f\t%d\n",
					prevX, prevY, newx, newy, i);
#endif
				glVertex2f(newx, newy);
			}
			glEnd();
		}

		break;
	case RECTANGLE:		// To keep the old compiler happy.
		{ 
		printf("RECTANGLE ignored\n");
#if 0
		  XRectangle *rlst = new XRectangle[numpnts], *ptr = rlst;
		  PointList *aux = cmd->auxList();
		  float Width, Height;
			for( int i=0; i< numpnts; i++)
			{	// Get x/y from list.
				points->getValue(i, &currX, &currY);
				// Convert to vp from fractional.
				frac2full(currX, currY, scale);
				if(relative)
					rel2abs(currX, currY, prevX, prevY);

				aux->getValue(i, &Width, &Height);
				frac2full(Width, Height, scale);

				// convert to x coord and store in list.
				vp2XRec(ptr++, currX, currY, Width, Height,
					xoff, yoff, maxY);
			}
#endif
#if 0
		  if(cmd->filled_)
			  XFillRectangles(display(), drawable(), xgc_,
					  rlst, (int)numpnts);
		  else
			  XDrawRectangles(display(), drawable(), xgc_,
					  rlst, (int)numpnts);
		  delete [] rlst;
#endif
		}
		break;
	case SEGMENT:
		{ 
		  //			printf("SEGMENT %d\n", numpnts);
		  PointList *aux = cmd->auxList();
		  float x2, y2, lst[4];
		  float xmin=1000000, xmax= -xmin;
		  float ymin=xmin, ymax=xmax;
		  glBegin(GL_LINES);
			for( int i=0; i< numpnts; i++)
			{	// Get x/y from list.
				points->getValue(i, &currX, &currY);
				// Convert to vp from fractional.
				frac2full(currX, currY, scale);
				if(relative)
					rel2abs(currX, currY, prevX, prevY);

				aux->getValue(i, &x2, &y2);
				frac2full(x2, y2, scale);
				if(relative)
					rel2abs(x2, y2, prevX, prevY);

				// convert to OGL coords and draw.
				vp2GLSeg(lst, currX, currY, x2, y2,
					xoff, yoff);
				MMf(lst[0], xmin, xmax);
				MMf(lst[1], ymin, ymax);
				MMf(lst[2], xmin, xmax);
				MMf(lst[3], ymin, ymax);
				glVertex2f(lst[0], lst[1]);
				glVertex2f(lst[2], lst[3]);
			}
			glEnd();
		}

		break;
	case CIRCLE:
	case ARC:
		{ 
		printf("CIRCLE/ARC\n");
		  XArc *alst = new XArc[numpnts], *ptr = alst;
		  PointList *aux = cmd->auxList();
		  PointList *aux2 = cmd->aux2List();
		  float ewidth, eheight, angle1, angle2;
			for( int i=0; i< numpnts; i++)
			{	// Get x/y from list.
				points->getValue(i, &currX, &currY);
				// Convert to vp from fractional.
				frac2full(currX, currY, scale);
				if(relative)
					rel2abs(currX, currY, prevX, prevY);

				aux->getValue(i, &ewidth, &eheight);
				frac2full(ewidth, eheight, scale);

				if(aux2 != NULL)
				{	aux2->getValue(i, &angle1, &angle2);
//					frac2full(angle1, angle2, scale);
				}
				else
				{	angle1 = 0.0;	// Circle.
					angle2 = 360;
					eheight = ewidth;	// Radius
				}
				vp2XArc(ptr++, currX, currY, ewidth, eheight,
					angle1, angle2,
					xoff, yoff, maxY);
			}

#if 0
		  if(cmd->filled_)
		  	XFillArcs(display(), drawable(), xgc_, alst,
				  (int)numpnts);
		  else
			XDrawArcs(display(), drawable(), xgc_, alst,
				  (int)numpnts);

#endif
		  delete [] alst;
		}
		break;
	case NONE:
	default:
		break;
	}

	AvAllotment tmp(currX, currY);
	viewport()->vectorPosition(tmp);
}

// Set clipping rectangle. Request is ignored if same as previous.
#if 0
void AvGLCanvas::setClipPlane(const AvAllotment &offset,
					const AvAllotment &size)
{
int x,y;
unsigned short w,h;

	w = (unsigned short)size.value(AvDimensionX);
	h = (int)size.value(AvDimensionY);
	x = (int)offset.value(AvDimensionX);
	y = (int)offset.value(AvDimensionY);
	// Flip from ll to ul.
 	y = vpY2XY_(height()-1, h + y);
	// Ignore request if same as current
//	if((w != clipRect_.width) || (h != clipRect_.height) ||
//	   (x != clipRect_.x) || (y != clipRect_.y))
	{
#if 0
		printf("setClipPlane:: X %d, Y %d, W %d, H %d\n",
			x, y, w, h);
#endif
	}
}
#else
void AvGLCanvas::setClipPlane(const AvAllotment &,
					const AvAllotment &)
{
}
#endif

void AvGLCanvas::setClipPlane(AvViewport &vp)
{
	setClipPlane(vp.xy(), vp.wh());
}

void AvGLCanvas::setClipPlane(AvViewport *vp)
{
	if(vp == NULL)
		vp = viewport();
	setClipPlane( *vp);
}

Drawable AvGLCanvas::drawable() {return XtWindow(wid_);}
Display *AvGLCanvas::display() {return XtDisplay(ref_);}
unsigned int AvGLCanvas::width() {return width_;}
unsigned int AvGLCanvas::height() {return height_;}
int AvGLCanvas::depth() {return depth_;}

AvPalette *AvGLCanvas::palette()const
{	return  palette_;
}

AvXPalette *AvGLCanvas::getXPalette()const
{	return (AvXPalette *) palette_;
}

void AvGLCanvas::setXPalette(AvXPalette *pal)
{
	printf("AvGLCanvas::setXPalette\n");

	if(palette_ != NULL)
		palette_->unref();

	palette_ = pal;
	if(palette_ != NULL)
		palette_->ref();
}

/* Set the color used to draw vectors and text. */
void AvGLCanvas::setDrawColor(const unsigned long color)
{
//	printf("AvGLCanvas::setDrawColor %ld\n", color);
	state_.graphicContext()->setForeground(color);
}

void AvGLCanvas::setDrawColor(const char *colorname)
{
AvXPalette *pal = getXPalette();
unsigned long pindex, vindex;

	printf("AvGLCanvas::setDrawColor: %s\n", colorname);
	if( pal->allocNamedColor( colorname, vindex, pindex))
		state_.graphicContext()->setForeground(vindex);
}

void AvGLCanvas::setRasterCacheParameters(const Boolean enable, const int n)
{
	if(n >= 0)
	{	if(texturecache_ != NULL)		// Remove old cache.
		{	texturecache_->unref();
			texturecache_ = NULL;
		}
		if(n > 0)			// Insert new.
		{	texturecache_ = new AvTextureCache(n);
			texturecache_->ref();
		}
	}
	if(texturecache_ != NULL)
		texturecache_->enable(enable);
}

void AvGLCanvas::getRasterCacheParameters(Boolean &enable, int &size)
{
	enable = cachingRasters();
	if(texturecache_ != NULL)
		size = texturecache_->size();
	else
		size = 0;
}

Boolean AvGLCanvas::cachingRasters()const
{
	if(texturecache_ != NULL)
		return texturecache_->enabled();
	else
		return FALSE;
}

void AvGLCanvas::setRasterCache(AvTextureCache * cache)
{
	if(cache == texturecache_)
		return;
	if(texturecache_ != NULL)
		texturecache_->unref();
	texturecache_ = cache;
	texturecache_->ref();
}

// Flush any cached pixmaps.
void AvGLCanvas::flushRasterCache()
{
	if(texturecache_ != NULL)
		texturecache_->flush();
}

void AvGLCanvas::flush()
{
	XFlush(display());
	glFlush();
}

void AvGLCanvas::wait()
{
	XFlush(display());
	glXWaitGL();
}


/****************************************************************/
/****************************************************************/
/****************************************************************/
/*			Graphic context code.			*/
/****************************************************************/
/****************************************************************/
/****************************************************************/


AvGLGraphicContext::AvGLGraphicContext( AvGLCanvas *can) : AvGraphicContext()
{
	canvas_ = can;
	background_ = can->blackPixel();
	foreground_ = can->whitePixel();
	update();
}

AvGLGraphicContext::AvGLGraphicContext(const AvGraphicContext &gc):
						AvGraphicContext(gc)
{
	canvas_ = NULL;
}

void AvGLGraphicContext::copy(const AvGraphicContext &gcin)
{
	canvas_ = NULL;
	AvGraphicContext::copy(gcin);
}

void AvGLGraphicContext::copy(const AvGLGraphicContext &gcin)
{
	canvas_ = gcin.canvas_;
	AvGraphicContext::copy(gcin);
	update();
}

// Emulate the = operator for plain graphic contexts.
AvGraphicContext& AvGLGraphicContext::operator=(const AvGraphicContext &gcin)
{
	AvGraphicContext::copy(gcin);
	return *this;
}

AvGLGraphicContext& AvGLGraphicContext::operator=(const AvGLGraphicContext &gcin)
{
	copy(gcin);
	return *this;
}

#if 0
GC AvGLGraphicContext::gc()
{
	if(modified())		// Make sure it's up to date.
		update();
	return gc_;
}
#endif

AvGLGraphicContext::~AvGLGraphicContext()
{
}

void AvGLGraphicContext::setDrawMode(const DrawMode d)
{ char *dm;

	switch(d) {
	case AvGraphicContext::COPY:
		dm = "Copy";
		break;
	case AvGraphicContext::INVERT:
		dm = "Invert";
		break;
	case AvGraphicContext::XOR:
		dm = "XOR";
		break;
	default:
		dm = "Unknown";
		break;
	};

	printf("AvGLGraphicContext::setDrawMode: %s\n", dm);
	AvGraphicContext::setDrawMode(d);
	update(AvGraphicContext::DRAWMODE);
}

void AvGLGraphicContext::setLineWidth(const int lw)
{
	AvGraphicContext::setLineWidth(lw);
	float w = (float)lw*0.5;
	glLineWidth(w);
}

// Linestyle isn't used. PGPLOT does its own linestyle.
void AvGLGraphicContext::setLineStyle(const LineStyle ls)
{
	AvGraphicContext::setLineStyle(ls);
	GLint factor;
	GLushort pattern;
	switch (ls) {
	case AvGraphicContext::SOLID:
	default:
		glDisable(GL_LINE_STIPPLE);
		break;
	case AvGraphicContext::DASHED:
		glEnable(GL_LINE_STIPPLE);
		factor = 1;
		pattern = 0xff00;
		glLineStipple(factor, pattern);
		break;
	case AvGraphicContext::DOTTED:
		glEnable(GL_LINE_STIPPLE);
		factor = 1;
		pattern = 0xc0c0;
		glLineStipple(factor, pattern);
		break;
	};
}

void AvGLGraphicContext::setFont(const Font f)
{
	AvGraphicContext::setFont(f);
	update(AvGraphicContext::FONT);
}

void AvGLGraphicContext::setForeground(const unsigned long c)
{
	AvGraphicContext::setForeground(c);
	float r, g, b;

	AvXPalette *pal = canvas_->getXPalette();
	pal->indexToRGB(c, r, g, b);
#if 0
	printf("AvGLGraphicContext::setForeground %ld %.2f/%.2f/%.2f\n",
		c, r, g, b);
#endif
	glColor3f(r, g, b);
}

void AvGLGraphicContext::setBackground(const unsigned long c)
{
	printf("AvGLGraphicContext::setBackground %ld\n", c);
	AvGraphicContext::setBackground(c);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	update(AvGraphicContext::BACKGROUND);
}

#if 0
void AvGLGraphicContext::update( const int msk)
{
unsigned long mask= 0;
int	m;

	if((canvas_ == NULL))
		return;

	if(msk == 0)
		m = AvGraphicContext::ALLMASK;
	else
		m = msk;

	if(m & AvGraphicContext::DRAWMODE)
	{	mask |= GCFunction;
		switch (getDrawMode()) {
		case AvGraphicContext::COPY:
				vals.function = GXcopy;
				break;
		case AvGraphicContext::XOR:
				vals.function = GXxor;
				break;
		case AvGraphicContext::INVERT:
				vals.function = GXinvert;
				break;
		default:
			mask &= ~GCFunction;	// oops
		}
	}

	if(m & AvGraphicContext::BACKGROUND)
	{AvXPalette *pal = canvas_->getXPalette();
	 unsigned long color;

		if(pal->virtualToPhysical(getBackground(), color))
		{	vals.background = color;
			mask |= GCBackground;
		}
	}
}
#else
void AvGLGraphicContext::update( const int /* mask */)
{
}
#endif

#endif	// AVOGL
