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

#include "text3d.h"

#ifdef FTGL
#include "FTGLPolygonFont.h"
#elif defined GLTT
#include "GLTTFont.h"
#endif

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
#ifdef GLTT
    , m_font(NULL)
#endif
{
#ifdef MACOSX
  if (!HaveValidContext ()) {post("GEM: geo: text3d - need window to load font");return;}
#endif
#ifdef FTGL
  m_font = new FTGLPolygonFont;
#endif
  fontNameMess(DEFAULT_FONT);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
text3d :: ~text3d()
{
#if defined GLTT || defined FTGL
  if(m_font)delete m_font;m_font=NULL;
  if(m_face)delete m_face;m_face=NULL;
#endif
}

#ifdef GLTT
/////////////////////////////////////////////////////////
// makeFontFromFace
//
/////////////////////////////////////////////////////////
int text3d :: makeFontFromFace()
{
  if(m_font)delete m_font;m_font=NULL;
  if (!m_face)    {
      error("GEM: text3d: True type font doesn't exist");
      return(0);
    }
  m_font = new GLTTFont(m_face);
  m_font->setPrecision((double)m_precision);
  if( ! m_font->create(m_fontSize) ) {
      error("GEM: text3d: unable to create polygonal font");
      delete m_font; m_font = NULL;
      return(0);
    }
  return(1);
}
#endif

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void text3d :: render(GemState *)
{
  if (m_valid && m_theString) {
    // compute the offset due to the justification
    float x1=0, y1=0, z1=0, x2=0, y2=0, z2=0;

#if defined FTGL
    m_font->BBox( m_theString, x1, y1, z1, x2, y2, z2); // FTGL
#elif defined GLTT
    x2=m_font->getWidth (m_theString);
    y2=m_font->getHeight();
#endif
    glPushMatrix();
    justifyFont(x1, y1, z1, x2, y2, z2);
#ifdef FTGL
    m_font->render(m_theString);
#elif defined GLTT
    m_font->output(m_theString);
#endif
    
    glPopMatrix();
  }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void text3d :: obj_setupCallback(t_class *)
{ }
