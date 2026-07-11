/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_TEXT2D_H_
#define _INCLUDE__GEM_GEOS_TEXT2D_H_

#include "Base/TextBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    text2d

    Creates a text2d string

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN text2d : public TextBase
{
  CPPEXTERN_HEADER(text2d, TextBase);

public:

  /////////
  // Constructor
  text2d(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~text2d();

  //////
  // anti aliasing (aka: pixmap instead of bitmap)
  bool m_antialias;
  void aliasMess(int io);

#ifdef FTGL
  /////////
  // Do the rendering
  virtual void renderLine(const char*line,float dist);
  virtual void renderLine(const wchar_t*line,float dist);

  virtual void            setFontSize(void);

  virtual FTFont* makeFont(const char*fontname);
  virtual FTFont* selectFont(void);
  FTGLPixmapFont *m_aafont;
  FTGLBitmapFont *m_bmfont;
#endif
};

#endif  // for header file
