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

#include "text3d.h"

#ifdef USE_FONTS
#include "FTFace.h"
#ifndef FTGL
#include "GLTTFont.h"
#else
#include "FTGLFont.h"
#endif
#endif // USE_FONTS

#ifdef MACOSX
#include <AGL/agl.h>
extern bool HaveValidContext (void);
#endif

CPPEXTERN_NEW_WITH_GIMME(text3d)

  /////////////////////////////////////////////////////////
//
// text3d
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
text3d :: text3d(int argc, t_atom *argv)
  : TextBase(argc, argv)
#ifdef USE_FONTS
  , m_font(NULL), m_face(NULL)
#endif // USE_FONTS
{
  m_fontSize = 20;
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
text3d :: ~text3d()
{
#ifdef USE_FONTS
  delete m_font;
  delete m_face;
#endif // USE_FONTS
}

/////////////////////////////////////////////////////////
// setPrecision
//
/////////////////////////////////////////////////////////
void text3d :: setPrecision(float prec)
{
  m_precision = prec;
  m_valid = makeFontFromFace();
  setModified();
}

/////////////////////////////////////////////////////////
// setFontSize
//
/////////////////////////////////////////////////////////
void text3d :: setFontSize(int size)
{
  m_fontSize = size;
  m_valid = makeFontFromFace();
  setModified();
}

/////////////////////////////////////////////////////////
// fontNameMess
//
/////////////////////////////////////////////////////////
void text3d :: fontNameMess(const char *filename)
{
#ifdef USE_FONTS
  m_valid = 0;
  delete m_font;
  m_font = NULL;
  delete m_face;
  m_face = new FTFace;

  char buf[MAXPDSTRING];
  canvas_makefilename(getCanvas(), (char *)filename, buf, MAXPDSTRING);

  if( ! m_face->open(buf) ) {
    error("GEM: text3d: unable to open font: %s", buf);
    return;
  }
  m_valid = makeFontFromFace();
  setModified();
#endif // USE_FONTS
}

/////////////////////////////////////////////////////////
// makeFontFromFace
//
/////////////////////////////////////////////////////////
int text3d :: makeFontFromFace()
{
#ifdef USE_FONTS
  if (!m_face)
    {
      error("GEM: text3d: True type font doesn't exist");
      return(0);
    }

  delete m_font;
#ifndef FTGL
  m_font = new GLTTFont(m_face);
#else
  m_font = new FTGLFont(m_face);
#endif
  //m_font->setPrecision((double)m_precision);
  if( ! m_font->create(m_fontSize) )
    {
      error("GEM: text3d: unable to create polygonal font");
      return(0);
    }
  return(1);
#endif // USE_FONTS
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void text3d :: render(GemState *)
{
#ifdef USE_FONTS
  if (m_valid && m_theString)
    {
      glPushMatrix();

      // compute the offset due to the justification
      float width = 0.f;
      if (m_widthJus == LEFT)
	width = 0.f;
      else if (m_widthJus == RIGHT)
	width = (float)(m_font->getWidth(m_theString));
      else if (m_widthJus == CENTER)
	width = (float)(m_font->getWidth(m_theString) / 2.f);

      float height = 0.f;
      if (m_heightJus == BOTTOM)
	height = 0.f;
      else if (m_heightJus == TOP)
	height = (float)(m_font->getHeight());
      else if (m_heightJus == MIDDLE)
	height = (float)(m_font->getHeight() / 2.f);

      glScalef(.05f, .05f, .05f);
      glTranslatef(-width, -height, 0.f);
      m_font->output(m_theString);

      glPopMatrix();
    }
#endif // USE_FONTS
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void text3d :: obj_setupCallback(t_class *)
{ }
