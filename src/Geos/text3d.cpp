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

#include "text3d.h"

#if defined FTGL && !defined HAVE_FTGL_FTGL_H
# include "FTGLPolygonFont.h"
#endif

CPPEXTERN_NEW_WITH_GIMME(text3d);

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
  : TextBase(argc, argv), m_antialias(true),
    m_aafont(NULL), m_pyfont(NULL)
{
  fontNameMess(DEFAULT_FONT);
}
text3d :: ~text3d()
{
  if(m_aafont) {
    delete m_aafont;
  }
  m_aafont=NULL;
  if(m_pyfont) {
    delete m_pyfont;
  }
  m_pyfont=NULL;
  m_font=NULL;
}

FTFont *text3d :: selectFont(void)
{
  if(m_antialias && NULL!=m_aafont) {
    return m_aafont;
  }
  if(!m_antialias && NULL!=m_pyfont) {
    return m_pyfont;
  }

  if(m_pyfont) {
    return m_pyfont;
  }
  return m_aafont;
}
FTFont *text3d :: makeFont(const char*fontfile)
{
  if(m_aafont) {
    delete m_aafont;
  }
  m_aafont=NULL;
  if(m_pyfont) {
    delete m_pyfont;
  }
  m_pyfont=NULL;
  m_font=NULL;
  // TextureFont looks nicer, but does not allow for texturing
  m_aafont =  new FTGLTextureFont(fontfile);
  if(m_aafont && m_aafont->Error()) {
    delete m_aafont;
    m_aafont = NULL;
  }
  m_pyfont =  new FTGLPolygonFont(fontfile);
  if (m_pyfont && m_pyfont->Error()) {
    delete m_pyfont;
    m_pyfont = NULL;
  }
  return selectFont();
}
void text3d :: setFontSize()
{
  if (!m_font) {
    return;
  }

  int fs=static_cast<int>(m_fontSize*m_precision);
  if(fs<0) {
    fs=-fs;
  }

  if (m_pyfont) {
    if (! m_pyfont->FaceSize(fs) ) {
      error("unable to set fontsize!");
    }
    if(m_pyfont->Error()) {
      error("error setting fontsize");
    }
  }
  if (m_aafont) {
    if (! m_aafont->FaceSize(fs) ) {
      error("unable to set antialiased-fontfize!");
    }
    if(m_aafont->Error()) {
      error("error setting aa-fontsize");
    }
  }
  m_font=selectFont();

  setModified();
}


#else /* !FTGL */

text3d :: text3d(int argc, t_atom *argv)
  : TextBase(argc, argv)
  , m_antialias(true)
{}
text3d :: ~text3d()
{}

#endif /* FTGL */

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void text3d :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "alias", aliasMess, int);
  CPPEXTERN_MSG1(classPtr, "antialias", aliasMess, int);
}
void text3d :: aliasMess(int io)
{
  m_antialias = io;
#ifdef FTGL
  m_font=selectFont();
#endif
}
