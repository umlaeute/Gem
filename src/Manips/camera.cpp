////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// tigital@mac.com
//
// Implementation file
//
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "camera.h"
#include "Gem/Manager.h"

CPPEXTERN_NEW_WITH_GIMME(camera);

#define PI      3.1415926535897932384626433832795

/////////////////////////////////////////////////////////
//
// camera
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
camera :: camera(int argc, t_atom *argv)
  : left(false), right(false), up(false), down(false), forward(false), reverse(false), m_mode(false)
  , m_speed(0.03)
  , hAngle(90.0), vAngle(0.0), distance(4.0)
  , m_infoOut(gem::RTE::Outlet(this))
{
  m_vPosition    = CVector3(0.0, 0.0, 0.0);  // Init the position to zero
  m_vView        = CVector3(0.0, 0.0, 0.0);  // Init the view to a std starting view
  m_vUpVector    = CVector3(0.0, 1.0, 0.0);  // Init the UpVector
  //  Position     View         Up Vector
  PositionCamera(0, 0, 4,   0, 0, 0,   0, 1, 0 );
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
camera :: ~camera()
{ }

////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void camera :: bang()
{
  // Initialize a variable for the cross product result
  //   CVector3 vCross = Cross(m_vView - m_vPosition, m_vUpVector);

  // Normalize the strafe vector
  // m_vSlide = Normalize(vCross);

  if(forward) {
    //MoveCamera(m_speed);
    distance = distance - (m_speed/100);
  }

  if(reverse) {
    // MoveCamera(-m_speed);
    distance = distance + (m_speed/100);
  }
  if(left) {
    if (m_mode) {
      incHRot(m_speed);
    } else {
      decHRot(m_speed);
    }
  }
  if(right) {
    if (m_mode) {
      decHRot(m_speed);
    } else {
      incHRot(m_speed);
    }
  }
  if(up) {
    if (m_mode) {
      incVRot(m_speed);
    } else {
      decVRot(m_speed);
    }
  }
  if(down) {
    if (m_mode) {
      decVRot(m_speed);
    } else {
      incVRot(m_speed);
    }
  }

  calcCameraVals();

  std::vector<gem::any>data;
  gem::any value;
  data.clear();
  data.push_back(value=m_vPosition.x);
  data.push_back(value=m_vPosition.y);
  data.push_back(value=m_vPosition.z);
  data.push_back(value=m_vView.x);
  data.push_back(value=m_vView.y);
  data.push_back(value=m_vView.z);
  data.push_back(value=m_vUpVector.x);
  data.push_back(value=m_vUpVector.y);
  data.push_back(value=m_vUpVector.z);
  m_infoOut.send("list", data);
}

void camera :: render(GemState *)
{
  bang();
  gem::utils::gl::gluLookAt(m_vPosition.x, m_vPosition.y, m_vPosition.z,
                            m_vView.x, m_vView.y, m_vView.z,
                            m_vUpVector.x, m_vUpVector.y, m_vUpVector.z);
}

void camera :: resetState()
{
  m_speed = 0.03f;
  //  Position     View         Up Vector
  PositionCamera(0, 0.0, 4,   0, 0.0, 0,   0, 1, 0 );
}

void camera :: vAngleMess(t_float val)
{
  vAngle = val;
}
void camera :: hAngleMess(t_float val)
{
  hAngle = val;
}
void camera :: distanceMess(t_float val)
{
  distance = val;
}
void camera :: calcCameraVals()
{
  float temp;

  // Calculating vertical position
  m_vPosition.y = distance * (float)sin(vAngle *PI/180);

  // Calculating XZ plane dist and then using the value to calc X angle
  temp = distance * (float)cos( vAngle * PI/180);

  m_vPosition.x = temp * (float)cos( hAngle * PI/180);
  m_vPosition.z = temp * (float)sin( hAngle * PI/180);

  calcUpVector();
}
void camera :: calcUpVector()
{
  float temp;

  m_vUpVector.y = distance * (float)sin((vAngle +90) *PI/180);
  temp = distance * (float)cos((vAngle +90)*PI/180);

  m_vUpVector.x = temp*(float)cos(hAngle * PI/180);
  m_vUpVector.z = temp*(float)sin(hAngle * PI/180);
}

/// Increase Horizontal rotation
void camera :: incHRot(float incVal)
{
  hAngle += incVal;

  if (hAngle > 360) {
    hAngle -= 360;
  }
}

/// Decrease Horizontal Rotation
void camera :: decHRot(float decVal)
{
  hAngle -= decVal;

  if (hAngle < 0) {
    hAngle += 360;
  }
}

/// Increase Vertical Rotation
void camera :: incVRot(float incVal)
{
  vAngle += incVal;

  if (vAngle > 360) {
    vAngle -= 360;
  }
}

/// Decrease Vertical Rotation
void camera :: decVRot(float decVal)
{
  vAngle -= decVal;

  if (vAngle < 0) {
    vAngle += 360;
  }
}

///////////////////////////////// POSITION CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function sets the camera's position and view and up vVector.
/////
///////////////////////////////// POSITION CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void camera :: PositionCamera(  float positionX, float positionY,
                                float positionZ,
                                float viewX,     float viewY,     float viewZ,
                                float upVectorX, float upVectorY, float upVectorZ)
{
  CVector3 vPosition    = CVector3(positionX, positionY, positionZ);
  CVector3 vView        = CVector3(viewX, viewY, viewZ);
  CVector3 vUpVector    = CVector3(upVectorX, upVectorY, upVectorZ);

  // The code above just makes it cleaner to set the variables.
  // Otherwise we would have to set each variable x y and z.

  m_vPosition = vPosition;                    // Assign the position
  m_vView     = vView;                        // Assign the view
  m_vUpVector = vUpVector;                    // Assign the up vector
}
///////////////////////////////// ROTATE VIEW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This rotates the view around the position using an axis-angle rotation
/////
///////////////////////////////// ROTATE VIEW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
void camera :: RotateView(float angle, float x, float y, float z)
{
  CVector3 vNewView;

  // Get the view vector (The direction we are facing)
  CVector3 vView = m_vView - m_vPosition;

  // Calculate the sine and cosine of the angle once
  float cosTheta = (float)cos(angle);
  float sinTheta = (float)sin(angle);

  // Find the new x position for the new rotated point
  vNewView.x  = (cosTheta + (1 - cosTheta) * x * x)        * vView.x;
  vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta)    * vView.y;
  vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta)    * vView.z;

  // Find the new y position for the new rotated point
  vNewView.y  = ((1 - cosTheta) * x * y + z * sinTheta)    * vView.x;
  vNewView.y += (cosTheta + (1 - cosTheta) * y * y)        * vView.y;
  vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta)    * vView.z;

  // Find the new z position for the new rotated point
  vNewView.z  = ((1 - cosTheta) * x * z - y * sinTheta)    * vView.x;
  vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta)    * vView.y;
  vNewView.z += (cosTheta + (1 - cosTheta) * z * z)        * vView.z;

  // Now we just add the newly rotated vector to our position to set
  // our new rotated view of our camera.
  m_vView = m_vPosition + vNewView;
}


///////////////////////////////// MOVE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This will move the camera forward or backward depending on the speed
/////
///////////////////////////////// MOVE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void camera :: MoveCamera(t_float speed)
{
  // Get our view vector (The direction we are facing)
  CVector3 vVector = (m_vView - m_vPosition).normalize();

  m_vPosition.x += vVector.x *
                   speed;        // Add our acceleration to our position's X
  //m_vPosition.y += vVector.y * speed;
  m_vPosition.z += vVector.z *
                   speed;        // Add our acceleration to our position's Z

  m_vView.x += vVector.x *
               speed;            // Add our acceleration to our view's X
  //m_vView.y += vVector.y * speed;
  m_vView.z += vVector.z *
               speed;            // Add our acceleration to our view's Z

}

///////////////////////////////// STRAFE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This strafes the camera left and right depending on the speed (-/+)
/////
///////////////////////////////// SLIDE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void camera :: SlideCamera(t_float speed)
{
  // Add the slide vector to our position
  m_vPosition.x += m_vSlide.x * speed;
  m_vPosition.z += m_vSlide.z * speed;

  // Add the strafe vector to our view
  m_vView.x += m_vSlide.x * speed;
  m_vView.z += m_vSlide.z * speed;
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void camera :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG0(classPtr, "bang", bang);
  CPPEXTERN_MSG0(classPtr, "reset", resetState);

  CPPEXTERN_MSG1(classPtr, "hAngle", hAngleMess, float);
  CPPEXTERN_MSG1(classPtr, "vAngle", vAngleMess, float);
  CPPEXTERN_MSG1(classPtr, "distance", distanceMess, float);
  CPPEXTERN_MSG1(classPtr, "speed", speedMess, float);
  CPPEXTERN_MSG1(classPtr, "lookX", lookXMess, float);
  CPPEXTERN_MSG1(classPtr, "lookY", lookYMess, float);
  CPPEXTERN_MSG1(classPtr, "lookZ", lookZMess, float);

  CPPEXTERN_MSG1(classPtr, "forward", forwardMess, bool);
  CPPEXTERN_MSG1(classPtr, "reverse", reverseMess, bool);
  CPPEXTERN_MSG1(classPtr, "left", leftMess, bool);
  CPPEXTERN_MSG1(classPtr, "right", rightMess, bool);
  CPPEXTERN_MSG1(classPtr, "up", upMess, bool);
  CPPEXTERN_MSG1(classPtr, "down", downMess, bool);
  CPPEXTERN_MSG1(classPtr, "mode", modeMess, bool);
}
void camera :: speedMess(t_float val)
{
  m_speed=val;
}
void camera :: forwardMess(bool val)
{
  forward=val;
}
void camera :: reverseMess(bool val)
{
  reverse=val;
}
void camera :: leftMess(bool val)
{
  left=val;
}
void camera :: rightMess(bool val)
{
  right=val;
}
void camera :: upMess(bool val)
{
  up=val;
}
void camera :: downMess(bool val)
{
  down=val;
}
void camera :: modeMess(bool val)
{
  m_mode=val;
}
void camera :: lookXMess(t_float val)
{
  m_vView.x=val;
}
void camera :: lookYMess(t_float val)
{
  m_vView.y=val;
}
void camera :: lookZMess(t_float val)
{
  m_vView.z=val;
}
