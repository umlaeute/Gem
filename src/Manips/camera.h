/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    camera control gem object
    tigital@mac.com

    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_MANIPS_CAMERA_H_
#define _INCLUDE__GEM_MANIPS_CAMERA_H_

#include "Base/GemBase.h"
#include "RTE/Outlet.h"
#include <stdlib.h>
#include <math.h>

#include "Utils/Vector.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    camera

    camera gem object

DESCRIPTION


-----------------------------------------------------------------*/
class GEM_EXTERN camera : public GemBase
//class GEM_EXTERN camera : public CPPExtern
{
  CPPEXTERN_HEADER(camera, GemBase);
  //CPPEXTERN_HEADER(camera, CPPExtern);

public:

  //////////
  // Constructor
  camera(int argc, t_atom *argv);
  //CCamera       g_Camera;

protected:

  //////////
  // Destructor
  virtual ~camera();

  //////////
  // When rendering occurs
  virtual void    render(GemState *state);

  void     bang(void);
  void     resetState();

  //////////
  // Angles changed
  void     hAngleMess(t_float val);
  void     vAngleMess(t_float val);
  void     distanceMess(t_float val);

  void     speedMess(t_float val);
  void     lookXMess(t_float val);
  void     lookYMess(t_float val);
  void     lookZMess(t_float val);
  void     forwardMess(t_float val);
  void     reverseMess(t_float val);
  void     leftMess(t_float val);
  void     rightMess(t_float val);
  void     upMess(t_float val);
  void     downMess(t_float val);
  void     modeMess(bool val);

  t_float m_left, m_right, m_up, m_down, m_forward, m_reverse;
  bool m_mode;
  t_float m_speed;
  t_float hAngle, vAngle, distance;

  // This changes the position, view, and up vector of the camera.
  // (Used for initialization)
  void PositionCamera(float positionX, float positionY, float positionZ,
                      float viewX,     float viewY,     float viewZ,
                      float upVectorX, float upVectorY, float upVectorZ);

  // This rotates the camera's view around the position using axis-angle rotation
  void RotateView(float angle, float x, float y, float z);

  // This will move the camera forward or backward depending on the speed
  void MoveCamera(t_float speed );
  void SlideCamera(t_float speed );
  void calcCameraVals(void);
  void calcUpVector(void);

private:

  CVector3 m_vPosition;                   // The camera's position
  CVector3 m_vView;                       // The camera's View
  CVector3 m_vUpVector;                   // The camera's UpVector
  CVector3 m_vSlide;                      // The camera's slide
  gem::RTE::Outlet       m_infoOut;
};

#endif  // for header file
