/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_PRIMTRI_H_
#define _INCLUDE__GEM_GEOS_PRIMTRI_H_

#include "Base/GemBase.h"
#include "Base/GemShape.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    primtTri

    Creates a triangle primitive

KEYWORDS
    geo

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN primTri : public GemShape
{
  CPPEXTERN_HEADER(primTri, GemShape);

public:

  //////////
  // Constructor
  primTri(t_floatarg size);

protected:

  //////////
  // Destructor
  virtual ~primTri();

  //////////
  // Do the renderShapeing
  virtual void    renderShape(GemState *state);

  //////////
  // Vector changed
  void            vectMess(int which, float x, float y, float z)
  {
    mVectors[which][0] = x;
    mVectors[which][1] = y;
    mVectors[which][2] = z;
  }

  //////////
  // Color changed
  void            colMess(int which, float r, float g, float b, float a)
  {
    mColors[which][0] = r;
    mColors[which][1] = g;
    mColors[which][2] = b;
    mColors[which][3] = a;
  }

  //////////
  // Vertex vectors
  float                   mVectors[3][3];

  //////////
  // Vertex colors
  float                   mColors[3][4];

private:

  //////////
  // static member functions
  static void     vect1MessCallback(void *data, t_float x, t_float y,
                                    t_float z);
  static void     vect2MessCallback(void *data, t_float x, t_float y,
                                    t_float z);
  static void     vect3MessCallback(void *data, t_float x, t_float y,
                                    t_float z);
  static void     col1MessCallback(void *data, t_symbol*, int argc,
                                   t_atom *argv);
  static void     col2MessCallback(void *data, t_symbol*, int argc,
                                   t_atom *argv);
  static void     col3MessCallback(void *data, t_symbol*, int argc,
                                   t_atom *argv);
};

#endif  // for header file
