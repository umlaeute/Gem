////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2003 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "text2d.h"

#ifdef FTGL
#include "FTGLPolygonFont.h"
#elif defined GLTT
#include "GLTTFont.h"
#endif

#ifdef MACOSX
#include <AGL/agl.h>
extern bool HaveValidContext (void);
#endif

CPPEXTERN_NEW_WITH_GIMME(text2d)

/////////////////////////////////////////////////////////
//
// text2d
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
text2d :: text2d(int argc, t_atom *argv)
  : TextBase(argc, argv),
#if defined GLTT || defined FTGL
    m_bfont(NULL),
#endif
#if defined __linux__ || defined __APPLE__ || defined FTGL
    m_pfont(NULL), m_antialias(1)
#else
    m_antialias(0)
#endif
{
#ifdef FTGL
  m_bfont = new FTGLBitmapFont;
  m_pfont = new FTGLPixmapFont;
#elif defined MACOSX && defined GLTT
  if (!HaveValidContext ()) {post("GEM: text2d - need window to load font");return;}
#endif
  fontNameMess(DEFAULT_FONT);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
text2d :: ~text2d()
{
#ifdef FTGL
  if(m_font)delete m_font;
#endif
#if defined GLTT || defined FTGL
  if(m_pfont)delete m_pfont;
# if defined __linux__ || defined __APPLE__ || defined FTGL
  if(m_bfont)delete m_bfont;
# endif
  if(m_face)delete m_face;
#endif
}

#ifdef GLTT
/////////////////////////////////////////////////////////
// makeFontFromFace
//
/////////////////////////////////////////////////////////
int text2d :: makeFontFromFace()
{
  if (!m_face)    {
      error("GEM: text2d: True type font doesn't exist");
      return(0);
    }
  if(m_bfont)delete m_bfont;m_bfont=NULL;
# if defined __linux__ || defined __APPLE__
  if(m_pfont)delete m_pfont;m_pfont=NULL;
# endif
  m_bfont = new GLTTBitmapFont(m_face);
  if( ! m_bfont->create(m_fontSize) )    {
    error("GEM: text2d: unable to create bitmap'ed font");
    delete m_bfont; m_bfont=NULL;
    return(0);
  }
# if defined __linux__ || defined __APPLE__
  m_pfont = new GLTTPixmapFont(m_face);
  if( ! m_pfont->create(m_fontSize) )    {
    delete m_pfont; m_pfont=NULL;
    error("GEM: text2d: unable to create pixmap'ed font");
    return(0);
  }
# endif
  return (1);
}
#endif

/////////////////////////////////////////////////////////
// setFontSize
//
/////////////////////////////////////////////////////////
void text2d :: setFontSize(int size)
{
  m_fontSize = size;
  m_valid=0;
#ifdef FTGL
  if (!m_pfont || !m_bfont)return;
  if( m_bfont->FaceSize(m_fontSize) &&  m_pfont->FaceSize(m_fontSize)) m_valid=1;
  else error("GEM: text2d: unable to set font-size!");
#elif defined GLTT
  m_valid = makeFontFromFace();
#endif
  setModified();
}
/////////////////////////////////////////////////////////
// fontNameMess
//
/////////////////////////////////////////////////////////
void text2d :: fontNameMess(const char *filename)
{
  m_valid = 0;
  char buf[MAXPDSTRING];
  canvas_makefilename(getCanvas(), (char *)filename, buf, MAXPDSTRING);
#ifdef FTGL
  if(!m_bfont && !m_pfont)return;
  if(!m_bfont->Open(buf, false) ) {
    error("GEM: text2d: unable to open bitmap'ed font: %s", buf);
    return;
  }
  if( ! m_pfont->Open(buf, false) ) {
    error("GEM: text2d: unable to open pixmap'ed font: %s", buf);
    return;
  }
  m_valid = 1;

  setFontSize(m_fontSize);
#elif defined GLTT
  if(m_bfont)delete m_bfont; m_bfont=NULL;
# if defined __linux__ || defined __APPLE__
  if(m_pfont)delete m_pfont; m_pfont=NULL;
# endif
  if(m_face)delete m_face;m_face = NULL;
  m_face = new FTFace;

  if( ! m_face->open(buf) ) {
    error("text2d: unable to open font: %s", buf);
    return;
  }
  m_valid = makeFontFromFace();
#endif
  setModified();
}
/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void text2d :: render(GemState *)
{
  if (m_valid && m_theString) {
    glPushMatrix();

    // compute the offset due to the justification
    float x1=0, y1=0, z1=0, x2=0, y2=0, z2=0;

#if defined FTGL || defined __linux__ || defined __APPLE__
    // pixmap'ed fonts are not supported under GLTT/NT
    if (m_antialias){
# if defined FTGL
      m_pfont->BBox( m_theString, x1, y1, z1, x2, y2, z2); // FTGL
# elif defined GLTT
      x2=m_pfont->getWidth (m_theString);
      y2=m_pfont->getHeight();
# endif
      float width  = 0.f;
      float height = 0.f;
      float depth  = 0.f;

      if (m_widthJus == LEFT)       width = x1;
      else if (m_widthJus == RIGHT) width = x2-x1;
      else if (m_widthJus == CENTER)width = x2 / 2.f;

      if (m_heightJus == BOTTOM)     height = y1;
      else if (m_heightJus == TOP)   height = y2-y1;
      else if (m_heightJus == MIDDLE)height = y2 / 2.f;
    
      if (m_depthJus == FRONT)       depth = z1;
      else if (m_depthJus == BACK)   depth = z2-z1;
      else if (m_depthJus == HALFWAY)depth = z2 / 2.f;

# ifdef FTGL
      glRasterPos2i(0,0);
      glBitmap(0,0,0.0,0.0,-width,-height, NULL);
      m_pfont->render(m_theString);
# elif defined GLTT
      m_pfont->output((int)-width, (int)-height, m_theString);
# endif
    }else
#endif
#if defined GLTT || defined FTGL
      {
# if defined FTGL
	m_bfont->BBox( m_theString, x1, y1, z1, x2, y2, z2); // FTGL
# elif defined GLTT
	x2=m_bfont->getWidth (m_theString);
	y2=m_bfont->getHeight();
# endif
	float width  = 0.f;
	float height = 0.f;
	float depth  = 0.f;

	if (m_widthJus == LEFT)       width = x1;
	else if (m_widthJus == RIGHT) width = x2-x1;
	else if (m_widthJus == CENTER)width = x2 / 2.f;

	if (m_heightJus == BOTTOM)     height = y1;
	else if (m_heightJus == TOP)   height = y2-y1;
	else if (m_heightJus == MIDDLE)height = y2 / 2.f;
	
	if (m_depthJus == FRONT)       depth = z1;
	else if (m_depthJus == BACK)   depth = z2-z1;
	else if (m_depthJus == HALFWAY)depth = z2 / 2.f;
# ifdef FTGL
	glRasterPos2i(0,0);
	glBitmap(0,0,0.0,0.0,-width,-height, NULL);
	m_bfont->render(m_theString);
# elif defined GLTT
	m_bfont->output((int)-width, (int)-height, m_theString);
# endif
      }
#endif
    glPopMatrix();
  }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void text2d :: obj_setupCallback(t_class *classPtr )
{ 
  class_addmethod(classPtr, (t_method)&text2d::aliasMessCallback,
		  gensym("alias"), A_FLOAT, A_NULL);
}

void text2d :: aliasMess(float size)
{
  m_antialias = (int)size;
}
void text2d :: aliasMessCallback(void *data, t_floatarg tog)
{
  GetMyClass(data)->aliasMess((float)tog);
}
