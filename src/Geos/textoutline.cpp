////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2003 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "textoutline.h"

#ifdef FTGL
#include "FTGLOutlineFont.h"
#endif

CPPEXTERN_NEW_WITH_GIMME(textoutline)

/////////////////////////////////////////////////////////
//
// textoutline
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
#ifdef FTGL
textoutline :: textoutline(int argc, t_atom *argv)
  : TextBase(argc, argv) {
  fontNameMess(DEFAULT_FONT);
} 
textoutline :: ~textoutline() {
  if(m_font)delete m_font;m_font=NULL;
}
FTFont *textoutline :: makeFont(const char*fontfile){
  if(m_font)delete m_font; m_font=NULL;
  m_font =  new FTGLOutlineFont(fontfile);
  if (m_font->Error()){
    delete m_font;
    m_font = NULL;
  }
  return m_font;
}

#elif defined GLTT

textoutline :: textoutline(int argc, t_atom *argv)
  : TextBase(argc, argv)
    , m_font(NULL)
{
#ifdef MACOSX
  if (!HaveValidContext ()) {post("GEM: geo: textoutline - need window to load font");return;}
#endif
   fontNameMess(DEFAULT_FONT);
 }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
textoutline :: ~textoutline()
{
  if(m_font)delete m_font;m_font=NULL;
  if(m_face)delete m_face;m_face=NULL;
}

/////////////////////////////////////////////////////////
// makeFontFromFace
//
/////////////////////////////////////////////////////////
void textoutline :: destroyFont() {
  if(m_font)delete m_font; m_font=NULL;
}
int textoutline :: makeFontFromFace()
{
  if(m_font)delete m_font;m_font=NULL;
  if (!m_face)    {
    error("GEM: textoutline: True type font doesn't exist");
    return(0);
  }
  m_font = new GLTTOutlineFont(m_face);
  m_font->setPrecision((double)m_precision);
  if( ! m_font->create((int)m_fontSize) ) {
    error("GEM: textoutline: unable to create outlined font");
    delete m_font; m_font = NULL;
    return(0);
  }
  return(1);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void textoutline :: render(GemState *)
{
  if (m_valid && m_theString) {
    // compute the offset due to the justification
    float x=0, y=0;

    x=m_font->getWidth (m_theString);
    y=m_font->getHeight();
    glPushMatrix();
    justifyFont(0, 0, 0, x, y, 0);
    m_font->output(m_theString);
    
    glPopMatrix();
  }
}


#else /* !FTGL && !GLTT */

textoutline :: textoutline(int argc, t_atom *argv)
  : TextBase(argc, argv)
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
textoutline :: ~textoutline()
{}

#endif /* !GLTT && !FTGL */

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void textoutline :: obj_setupCallback(t_class *)
{ }
