/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A text3d

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    Copyright (c) 2005 Georg Holzmann <grh@mur.at>
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

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
