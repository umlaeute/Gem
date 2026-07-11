/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2003, Cyrille Henry and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_CURVE_D_H_
#define _INCLUDE__GEM_GEOS_CURVE_D_H_

#include "Base/GemShape.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  curve3d

  Creates a 3D bezier curve

  KEYWORDS
  geo

  DESCRIPTION

  -----------------------------------------------------------------*/

class GEM_EXTERN curve3d : public GemShape
{
  CPPEXTERN_HEADER(curve3d, GemShape);

public:

  //////////
  // Constructor
  curve3d(t_floatarg size_X, t_floatarg size_Y);

protected:

  //////////
  // Destructor
  virtual ~curve3d();

  //////////
  // Do the renderShapeing
  virtual void  renderShape(GemState *state);


  typedef struct {
    GLfloat x,y,z;
  } t_float3;

  int nb_pts_control_X;
  int nb_pts_control_Y;
  int nb_pts_affich_X;
  int nb_pts_affich_Y;
  void resolutionMess(int resX, int resY);
  void gridMess(int gridX, int gridY);
  void setMess(int X,int Y,float posX, float posY,float posZ);

  enum C3dDrawType {LINE, FILL, POINT,
                    LINE1, LINE2, LINE3, LINE4,
                    CONTROL_FILL, CONTROL_POINT, CONTROL_LINE, CONTROL_LINE1, CONTROL_LINE2
                   };


  t_float3              *m_posXYZ; // attention, valeur critique
};

#endif  // for header file
