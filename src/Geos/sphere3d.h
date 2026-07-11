/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2006, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_SPHERE_D_H_
#define _INCLUDE__GEM_GEOS_SPHERE_D_H_

#include "Base/GemGluObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    sphere3d

    Creates a sphere3d

KEYWORDS
    geo

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN sphere3d : public GemGluObj
{
  CPPEXTERN_HEADER(sphere3d, GemGluObj);

public:

  //////////
  // Constructor
  sphere3d(t_floatarg size, t_floatarg slice=10.0, t_floatarg stack=0.0);

protected:

  //////////
  // Destructor
  virtual ~sphere3d(void);

  //////////
  // Do the rendering
  virtual void    renderShape(GemState *state);

  virtual void    createSphere3d(void);

  virtual void    setCartesian(int i, int j, GLfloat x, GLfloat y,
                               GLfloat z);
  virtual void    setSpherical(int i, int j, GLfloat r, GLfloat azimuth,
                               GLfloat elevation);

  virtual void    print(int slice, int stack);
  virtual void    print(void);

  GLfloat         *m_x;
  GLfloat         *m_y;
  GLfloat         *m_z;
  int             oldStacks, oldSlices;
  GLenum          oldDrawType;
  int             oldTexture;

  GLuint          m_displayList;
};
#endif  // for header file
