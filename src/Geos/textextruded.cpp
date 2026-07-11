////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 2003, IOhannes m zmölnig and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
////////////////////////////////////////////////////////

#include "textextruded.h"

#if defined FTGL && !defined HAVE_FTGL_FTGL_H
# include <FTGLExtrdFont.h>
#endif

CPPEXTERN_NEW_WITH_GIMME(textextruded);

/////////////////////////////////////////////////////////
//
// textextruded
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
#ifdef FTGL
textextruded :: textextruded(int argc, t_atom *argv)
  : TextBase(argc, argv)
{
  fontNameMess(DEFAULT_FONT);
}
textextruded :: ~textextruded()
{
  if(m_font) {
    delete m_font;
  }
  m_font=NULL;
}
FTFont *textextruded :: makeFont(const char*fontfile)
{
  if(m_font) {
    delete m_font;
  }
  m_font=NULL;
  m_font =  new FTGLExtrdFont(fontfile);
  if (m_font->Error()) {
    delete m_font;
    m_font = NULL;
  }
  return m_font;
}

/////////////////////////////////////////////////////////
// setPrecision
//
/////////////////////////////////////////////////////////
void textextruded :: setDepth(float prec)
{
  m_fontDepth = prec;
  if(!m_font) {
    return;
  }
  m_font->Depth(m_fontDepth);
  setFontSize();
  //setModified();
}
#else

textextruded :: textextruded(int argc, t_atom *argv)
  : TextBase(argc, argv)
{}
textextruded :: ~textextruded()
{}
void textextruded :: setDepth(float prec)
{}
#endif /* FTGL */

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void textextruded :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "depth", setDepth, float);
}
