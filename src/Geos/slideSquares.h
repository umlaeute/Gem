/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2003, James Tittle II and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_SLIDESQUARES_H_
#define _INCLUDE__GEM_GEOS_SLIDESQUARES_H_

#include "Base/GemShape.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    slideSquares

    Creates a bunch of sliding squares

KEYWORDS
    geo

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN slideSquares : public GemShape
{
  CPPEXTERN_HEADER(slideSquares, GemShape);

public:

  //////////
  // Constructor
  slideSquares(t_floatarg width, t_floatarg height);

protected:

  //////////
  // Destructor
  virtual ~slideSquares(void);

  //////////
  // The height of the object
  void            heightMess(float size);

  //////////
  // Do the rendering
  virtual void    renderShape(GemState *state);

  //////////
  // The height of the object
  GLfloat         m_height;

  //////////
  // The height inlet
  t_inlet         *m_inletH;


  GLvoid  Slide( int i );
  void    slide_init(void);
};

#endif  // for header file
