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
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "text2d.h"

#include "FTFace.h"
#ifndef FTGL
#include "GLTTBitmapFont.h"
#ifdef __linux__ || __APPLE__
#include "GLTTPixmapFont.h"
#endif
#else
#include "FTGLBitmapFont.h"
#ifdef __linux__ || __APPLE__
#include "FTGLPixmapFont.h"
#endif
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
#ifdef __linux__ || __APPLE__
  : TextBase(argc, argv), m_pfont(NULL), m_bfont(NULL), m_face(NULL), m_antialias(0)
#else
    : TextBase(argc, argv), m_pfont(NULL), m_face(NULL)
#endif
{
#ifdef MACOSX
  if (!HaveValidContext ()) {
    post("GEM: geo: text3d - need window to load font");
    return;
  }
#endif
  fontNameMess(DEFAULT_FONT);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
text2d :: ~text2d()
{
#ifdef __linux__ || __APPLE__
  delete m_pfont;
#endif
  delete m_bfont;
  delete m_face;
}

/////////////////////////////////////////////////////////
// setFontSize
//
/////////////////////////////////////////////////////////
void text2d :: setFontSize(int size)
{
  m_fontSize = size;

  m_valid = makeFontFromFace();
  setModified();
}

/////////////////////////////////////////////////////////
// fontNameMess
//
/////////////////////////////////////////////////////////
void text2d :: fontNameMess(const char *filename)
{
  m_valid = 0;
#ifdef __linux__ || __APPLE__
  delete m_pfont;
  m_pfont = NULL;
#endif
  delete m_bfont;
  m_bfont = NULL;
  delete m_face;

  m_face = new FTFace;

  char buf[MAXPDSTRING];
  canvas_makefilename(getCanvas(), (char *)filename, buf, MAXPDSTRING);

#ifndef FTGL
  if( ! m_face->open(buf) )
#else
  if( ! m_face->Open(buf) )
#endif
    {
      error("GEM: text2d: unable to open font: %s", buf);
      return;
    }
  m_valid = makeFontFromFace();
  setModified();
}

/////////////////////////////////////////////////////////
// makeFontFromFace
//
/////////////////////////////////////////////////////////
int text2d :: makeFontFromFace()
{
  if (!m_face)
    {
      error("GEM: text2d: True type font doesn't exist");
      return(0);
    }

  //delete m_pfont;
  //delete m_bfont;
#ifndef FTGL
  m_bfont = new GLTTBitmapFont(m_face);
#ifdef __linux__ || __APPLE__
  m_pfont = new GLTTPixmapFont(m_face);
#endif
#else
  m_bfont = new FTGLBitmapFont();
#ifdef __linux__ || __APPLE__
  m_pfont = new FTGLPixmapFont(m_face);
#endif
#endif
  if( ! m_bfont->create(m_fontSize) )
    {
      post("GEM: text2d: unable to create bitmap'ed font");
      return(0);
    }
#ifdef __linux__ || __APPLE__
  if( ! m_pfont->create(m_fontSize) )
    {
      post("GEM: text2d: unable to create pixmap'ed font");
      return(0);
    }
#endif
  return(1);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void text2d :: render(GemState *)
{
  if (m_valid)
    if (m_antialias)
      {
#ifdef __linux__  || __APPLE__ /* used to be "#ifndef NT", maybe this is better ? */
	// compute the offset due to the justification
	int width = 0;
	if (m_widthJus == LEFT)
	  width = 0;
	else if (m_widthJus == RIGHT)
	  width = m_pfont->getWidth(m_theString);
	else if (m_widthJus == CENTER)
	  width = m_pfont->getWidth(m_theString) / 2;
	      
	int height = 0;
	if (m_heightJus == BOTTOM)
	  height = 0;
	else if (m_heightJus == TOP)
	  height = m_pfont->getHeight();
	else if (m_heightJus == MIDDLE)
	  height = m_pfont->getHeight() / 2;
	m_pfont->output(-width, -height,m_theString);
#else
	post("unaliased fonts not (yet) supported under NT");
#endif
      }
    else
      {
	// compute the offset due to the justification
	int width = 0;
	if (m_widthJus == LEFT)
	  width = 0;
	else if (m_widthJus == RIGHT)
	  width = m_bfont->getWidth(m_theString);
	else if (m_widthJus == CENTER)
	  width = m_bfont->getWidth(m_theString) / 2;
	      
	int height = 0;
	if (m_heightJus == BOTTOM)
	  height = 0;
	else if (m_heightJus == TOP)
	  height = m_bfont->getHeight();
	else if (m_heightJus == MIDDLE)
	  height = m_bfont->getHeight() / 2;
	m_bfont->output(-width, -height,m_theString);
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
