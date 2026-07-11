/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_TEXT3D_H_
#define _INCLUDE__GEM_GEOS_TEXT3D_H_

#include "Base/TextBase.h"


/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    text3d

    Creates a text3d string

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN text3d : public TextBase
{
  CPPEXTERN_HEADER(text3d, TextBase);

public:

  //////////
  // Constructor
  text3d(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~text3d();

#ifdef FTGL
  virtual void setFontSize(void);
  virtual FTFont* selectFont(void);
  virtual FTFont*makeFont(const char*fontname);
  FTGLTextureFont*m_aafont;
  FTGLPolygonFont*m_pyfont;
#endif
  //////
  // anti aliasing (aka: pixmap instead of bitmap)
  bool m_antialias;
  void aliasMess(int io);
};

#endif  // for header file
