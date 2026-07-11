/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Erich Berger and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_CUBOID_H_
#define _INCLUDE__GEM_GEOS_CUBOID_H_

#include "Base/GemShape.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  cuboid

  Creates a cuboid

  KEYWORDS
  geo

  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN cuboid : public GemShape
{
  CPPEXTERN_HEADER(cuboid, GemShape);

public:

  //////////
  // Constructor
  cuboid(t_floatarg sizex, t_floatarg sizey, t_floatarg sizez);
protected:

  //////////
  // Destructor
  virtual ~cuboid(void);


  //////////
  // The height of the object
  void          heightMess(float sizey);

  //////////
  // The width of the object
  void          widthMess(float sizez);

  //////////
  // Do the rendering
  virtual void  renderShape(GemState *state);

  //////////
  // The height of the object
  GLfloat               m_sizey;

  //////////
  // The height inlet
  t_inlet         *m_inletY;

  //////////
  // The height of the object
  GLfloat               m_sizez;

  //////////
  // The height inlet
  t_inlet         *m_inletZ;

};

#endif  // for header file
