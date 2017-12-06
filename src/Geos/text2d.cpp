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
//    Copyright (c) 2001-2014 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    Copyright (c) 2005 Georg Holzmann <grh@mur.at>
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "text2d.h"

#if defined FTGL && !defined HAVE_FTGL_FTGL_H
# include "FTGLPixmapFont.h"
# include "FTGLBitmapFont.h"
#endif

CPPEXTERN_NEW_WITH_GIMME(text2d);

/////////////////////////////////////////////////////////
//
// text2d
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
#ifdef FTGL
text2d :: text2d(int argc, t_atom *argv)
  : TextBase(argc,argv), m_antialias(true),
    m_aafont(NULL), m_bmfont(NULL)
{
  fontNameMess(DEFAULT_FONT);
}

text2d :: ~text2d()
{
  if(m_bmfont) {
    delete m_bmfont;
  }
  m_bmfont=NULL;
  if(m_aafont) {
    delete m_aafont;
  }
  m_aafont=NULL;
  m_font=NULL;
}

FTFont *text2d :: selectFont(void)
{
  if(m_antialias && NULL!=m_aafont) {
    return m_aafont;
  }
  if(!m_antialias && NULL!=m_bmfont) {
    return m_bmfont;
  }

  if(m_bmfont) {
    return m_bmfont;
  }
  return m_aafont;
}
FTFont *text2d :: makeFont(const char*fontfile)
{
  if(m_bmfont) {
    delete m_bmfont;
  }
  m_bmfont=NULL;
  if(m_aafont) {
    delete m_aafont;
  }
  m_aafont=NULL;
  m_font=NULL;

  m_bmfont =  new FTGLBitmapFont(fontfile);
  if (m_bmfont && m_bmfont->Error()) {
    delete m_bmfont;
    m_bmfont = NULL;
  }
  m_aafont =  new FTGLPixmapFont(fontfile);
  if (m_aafont && m_aafont->Error()) {
    delete m_aafont;
    m_aafont = NULL;
  }

  return selectFont();
}
/////////////////////////////////////////////////////////
// setFontSize
//
/////////////////////////////////////////////////////////
void text2d :: setFontSize()
{
  int fs=static_cast<int>(m_fontSize);
  if(fs<0) {
    fs=-fs;
  }

  if (m_bmfont) {
    if (! m_bmfont->FaceSize(fs) ) {
      error("unable to set fontsize!");
    }
  }
  if (m_aafont) {
    if (! m_aafont->FaceSize(fs) ) {
      error("unable to set antialiased-fontfize!");
    }
  }
  m_font=selectFont();
  setModified();
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void text2d :: renderLine(const char*line, float dist)
{
  float x1=0, y1=0, z1=0, x2=0, y2=0, z2=0;
  m_font->BBox(line, x1, y1, z1, x2, y2, z2); // FTGL

  glPushMatrix();
  glNormal3f(0.0, 0.0, 1.0);

  Justification just=justifyFont(x1, y1, z1, x2, y2, z2, dist/m_precision);

  glRasterPos2i(0,0);
  glBitmap(0,0,0.0,0.0,-just.width,-just.height, NULL);
  m_font->Render(line);
  glPopMatrix();
}
void text2d :: renderLine(const wchar_t*line, float dist)
{
  float x1=0, y1=0, z1=0, x2=0, y2=0, z2=0;
  m_font->BBox(line, x1, y1, z1, x2, y2, z2); // FTGL

  glPushMatrix();
  glNormal3f(0.0, 0.0, 1.0);

  Justification just=justifyFont(x1, y1, z1, x2, y2, z2, dist/m_precision);

  glRasterPos2i(0,0);
  glBitmap(0,0,0.0,0.0,-just.width,-just.height, NULL);
  m_font->Render(line);
  glPopMatrix();
}

#else /* !FTGL */
text2d :: text2d(int argc, t_atom *argv)
  : TextBase(argc, argv)
  , m_antialias(true)
{}
text2d :: ~text2d()
{}
#endif /* FTGL */

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void text2d :: obj_setupCallback(t_class *classPtr )
{
  CPPEXTERN_MSG1(classPtr, "alias", aliasMess, int);
  CPPEXTERN_MSG1(classPtr, "antialias", aliasMess, int);
}

void text2d :: aliasMess(int io)
{
  m_antialias = io;
#ifdef FTGL
  m_font=selectFont();
#endif
}
