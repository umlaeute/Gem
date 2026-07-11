/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_TEAPOT_H_
#define _INCLUDE__GEM_GEOS_TEAPOT_H_

#include "Base/GemGluObj.h"


/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
        teapot

    Creates a teapot

KEYWORD
    geo

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN teapot : public GemGluObj
{
  CPPEXTERN_HEADER(teapot, GemGluObj);

public:

  //////////
  // Constructor
  teapot(t_floatarg size, t_floatarg slice);

protected:

  //////////
  // Destructor
  virtual ~teapot();

  //////////
  // Do the rendering
  virtual void    renderShape(GemState *state);


  GLfloat m_texCoords[4][2];

};

#endif  // for header file
