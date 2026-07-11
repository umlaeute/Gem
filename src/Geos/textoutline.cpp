////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
/////////////////////////////////////////////////////////

#include "textoutline.h"

#if defined FTGL && !defined HAVE_FTGL_FTGL_H
# include <FTGLOutlineFont.h>
#endif

CPPEXTERN_NEW_WITH_GIMME(textoutline);

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
  : TextBase(argc, argv)
{
  fontNameMess(DEFAULT_FONT);
}
textoutline :: ~textoutline()
{
  if(m_font) {
    delete m_font;
  }
  m_font=NULL;
}
FTFont *textoutline :: makeFont(const char*fontfile)
{
  if(m_font) {
    delete m_font;
  }
  m_font=NULL;
  m_font =  new FTGLOutlineFont(fontfile);
  if (m_font->Error()) {
    delete m_font;
    m_font = NULL;
  }
  return m_font;
}


#else /* !FTGL */

textoutline :: textoutline(int argc, t_atom *argv)
  : TextBase(argc, argv)
{}
textoutline :: ~textoutline()
{}

#endif /* FTGL */

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void textoutline :: obj_setupCallback(t_class *)
{ }
