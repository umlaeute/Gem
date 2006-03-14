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
//    Copyright (c) 2005 Georg Holzmann <grh@mur.at>
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

CPPEXTERN_NEW_WITH_GIMME(text3d)

/////////////////////////////////////////////////////////
//
// text3d
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
#ifdef FTGL
text3d :: text3d(int argc, t_atom *argv)
  : TextBase(argc, argv) {
  fontNameMess(DEFAULT_FONT);
} 
text3d :: ~text3d() {
  if(m_font)delete m_font;m_font=NULL;
}
FTFont *text3d :: makeFont(const char*fontfile){
  if(m_font)delete m_font; m_font=NULL;
  m_font =  new FTGLPolygonFont(fontfile);
  if (m_font->Error()){
    delete m_font;
    m_font = NULL;
  }
  return m_font;
}

#elif defined GLTT

text3d :: text3d(int argc, t_atom *argv)
  : TextBase(argc, argv)
    , m_font(NULL)
{
   fontNameMess(DEFAULT_FONT);
 }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
text3d :: ~text3d()
{
  if(m_font)delete m_font;m_font=NULL;
  if(m_face)delete m_face;m_face=NULL;
}

/////////////////////////////////////////////////////////
// makeFontFromFace
//
/////////////////////////////////////////////////////////
void text3d :: destroyFont() {
  if(m_font)delete m_font; m_font=NULL;
}
int text3d :: makeFontFromFace()
{
  if (!m_face)    {
    error("GEM: text3d: True type font doesn't exist");
    return(0);
  }
  if(m_font)delete m_font;m_font=NULL;
  m_font = new GLTTFont(m_face);
  m_font->setPrecision((double)m_precision);
  if( ! m_font->create((int)m_fontSize) ) {
    error("GEM: text3d: unable to create polygonal font");
    delete m_font; m_font = NULL;
    return(0);
  }
  return(1);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void text3d :: render(GemState *)
{
  if (m_valid && !m_theText.empty()) 
  {
    // compute the offset due to the justification
    float x=0, y=0;
    x=m_font->getWidth (m_theText[0]);
    y=m_font->getHeight();
    
    glPushMatrix();
    justifyFont(0, 0, 0, x, y, 0, y);
    m_font->output(m_theText[0]);
    glPopMatrix();
  }
}


#else /* !FTGL && !GLTT */

text3d :: text3d(int argc, t_atom *argv)
  : TextBase(argc, argv)
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
text3d :: ~text3d()
{}

#endif /* !GLTT && !FTGL */

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void text3d :: obj_setupCallback(t_class *)
{ }
