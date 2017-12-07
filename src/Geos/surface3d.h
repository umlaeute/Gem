/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A surface3d

    Copyright (c) 1997-1998 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

    -----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_GEOS_SURFACE_D_H_
#define _INCLUDE__GEM_GEOS_SURFACE_D_H_

#include "Base/GemShape.h"


/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  surface3d

  Creates a surface3d

  KEYWORDS
  geo

  DESCRIPTION

  -----------------------------------------------------------------*/

class GEM_EXTERN surface3d : public GemShape
{
  CPPEXTERN_HEADER(surface3d, GemShape);

public:

  //////////
  // Constructor
  surface3d(t_floatarg size_X, t_floatarg size_Y);

protected:

  //////////
  // Destructor
  virtual ~surface3d();

  //////////
  // Do the renderShapeing
  virtual void  renderShape(GemState *state);


//  typedef struct {
//    GLfloat x,y,z;
//  } t_float3;


  int nb_pts_control_X;
  int nb_pts_control_Y;
  int nb_pts_affich_X;
  int nb_pts_affich_Y;
  bool compute_normal;
  void resolutionMess(int resX, int resY);
  void gridMess(int gridX, int gridY);
  void setMess(int X,int Y,float posX, float posY,float posZ);
  void interpolate(float X,float Y);
  t_float3 bicubic3(t_float X, t_float Y);
  void normalMess(bool normal);

  enum C3dDrawType {LINE, FILL, POINT,
                    LINE1, LINE2, LINE3, LINE4,
                    CONTROL_FILL, CONTROL_POINT, CONTROL_LINE, CONTROL_LINE1, CONTROL_LINE2
                   };

  t_float3              *m_posXYZ;

private:
  static void           interpolate(void *data, t_float X, t_float Y);
  static t_float        cubic (t_float  X0, t_float  X1, t_float  X2,
                               t_float  X3, t_float fract);
  static t_float3       cubic3(t_float3 X0, t_float3 X1, t_float3 X2,
                               t_float3 X3, t_float fract);
};

#endif  // for header file
