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

CPPEXTERN_NEW_WITH_GIMME(camera)

/////////////////////////////////////////////////////////
//
// camera
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
camera :: camera(int argc, t_atom *argv)
		    : left(0), right(0), up(0), down(0), forward(0), reverse(0), m_mode(0),
		    lookX(0.0), lookY(0.0), lookZ(0.0), m_speed( 0.03f ),
		    hAngle(90.0f), vAngle(0.0f), distance(4.0f), slideLeft(0), slideRight(0)
{
    m_vPosition    = CVector3(0.0, 0.0, 0.0);	// Init the position to zero
    m_vView        = CVector3(0.0, 0.0, 0.0);	// Init the view to a std starting view
    m_vUpVector    = CVector3(0.0, 1.0, 0.0);	// Init the UpVector
			//  Position     View         Up Vector
    PositionCamera(0, 0.0, 4,   0, 0.0, 0,   0, 1, 0 ); 
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
camera :: ~camera()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void camera :: render(GemState *)
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
    if(left)
    {
	if (m_mode)
	    incHRot(m_speed);
	else
	    decHRot(m_speed);
    }
    if(right)
    {
	if (m_mode)
	    decHRot(m_speed);
	else
	    incHRot(m_speed);
    }
    if(up)
    {
	if (m_mode)
	    incVRot(m_speed);
	else
	    decVRot(m_speed);
    }
    if(down)
    {
	if (m_mode)
	    decVRot(m_speed);
	else
	    incVRot(m_speed);
    }
/*
    if(slideLeft) {            

        // Strafe the camera left
        SlideCamera(-m_speed);
    }

    if(slideRight) {            

        // Strafe the camera right
        SlideCamera(m_speed);
    }
*/    
    calcCameraVals();
/*
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(m_vPosition.x, m_vPosition.y, m_vPosition.z, 
	      m_vView.x, m_vView.y, m_vView.z, 
	      m_vUpVector.x, m_vUpVector.y, m_vUpVector.z);
*/
	GemMan::m_lookat[0] = m_vPosition.x;
	GemMan::m_lookat[1] = m_vPosition.y;
	GemMan::m_lookat[2] = m_vPosition.z;
	GemMan::m_lookat[3] = m_vView.x;
	GemMan::m_lookat[4] = m_vView.y;
	GemMan::m_lookat[5] = m_vView.z;
	GemMan::m_lookat[6] = m_vUpVector.x;
	GemMan::m_lookat[7] = m_vUpVector.y;
	GemMan::m_lookat[8] = m_vUpVector.z;
#if 0
    post("m_vPosition = %f %f %f",m_vPosition.x, m_vPosition.y ,m_vPosition.z);
    post("m_vView     = %f %f %f",m_vView.x,m_vView.y,m_vView.z);
    post("m_vUpVector = %f %f %f",m_vUpVector.x,m_vUpVector.y,m_vUpVector.z);
    post("g_Camera.hAngle = %f", hAngle);
    post("g_Camera.vAngle = %f", vAngle);
    post("g_Camera.distance = %f", distance);
#endif
}

void camera :: resetState()
{
    m_speed = 0.03f;
    			//  Position     View         Up Vector
    PositionCamera(0, 0.0, 4,   0, 0.0, 0,   0, 1, 0 );
}

void camera :: leftMess(bool val )
{
    if (m_mode)
	incHRot(m_speed);
    else
	decHRot(m_speed);
}
void camera :: rightMess(bool val )
{
    if (m_mode)
	decHRot(m_speed);
    else
	incHRot(m_speed);
}
void camera :: upMess(bool val )
{
    if (m_mode)
	incVRot(m_speed);
    else
	decVRot(m_speed);
}
void camera :: downMess(bool val )
{
    if (m_mode)
	decVRot(m_speed);
    else
	incVRot(m_speed);
}

void camera :: vAngleMess(float val)
{
    vAngle = val;
}
void camera :: hAngleMess(float val)
{
    hAngle = val;
}
void camera :: distanceMess(float val)
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

    if (hAngle > 360)
	hAngle -= 360;
}

/// Decrease Horizontal Rotation
void camera :: decHRot(float decVal)
{
    hAngle -= decVal;

    if (hAngle < 0)
	hAngle += 360;
}

/// Increase Vertical Rotation
void camera :: incVRot(float incVal)
{
    vAngle += incVal;

    if (vAngle > 360)
	vAngle -= 360;
}

/// Decrease Vertical Rotation
void camera :: decVRot(float decVal)
{
    vAngle -= decVal;

    if (vAngle < 0)
	vAngle += 360;
}
/////////////////////////////////////// CROSS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This returns a perpendicular vector from 2 given vectors by taking the cross product.
/////
/////////////////////////////////////// CROSS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
                                                
CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
    CVector3 vNormal;	// The vector to hold the cross product

    // If we are given 2 vectors (the view and up vector) then we have a plane define.  
    // The cross product finds a vector that is perpendicular to that plane, 
    // which means it's point straight out of the plane at a 90 degree angle.
    // The equation for the cross product is simple, but difficult at first to memorize:
    
    // The X value for the vector is:  (V1.y * V2.z) - (V1.z * V2.y)                           
    // Get the X value
    vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
                                                        
    // The Y value for the vector is:  (V1.z * V2.x) - (V1.x * V2.z)
    vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
                                                        
    // The Z value for the vector is:  (V1.x * V2.y) - (V1.y * V2.x)
    vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

    // *IMPORTANT* This is not communitive. You can not change the order or this or
    // else it won't work correctly.  It has to be exactly like that.  Just remember,
    // If you are trying to find the X, you don't use the X value of the 2 vectors, and
    // it's the same for the Y and Z.  You notice you use the other 2, but never that axis.
    // If you look at the camera rotation tutorial, you will notice it's the same for rotations.

    // So why do I need the cross product to do a first person view?  Well, we need
    // to find the axis that our view has to rotate around.  Rotating the camera left
    // and right is simple, the axis is always (0, 1, 0).  Rotating the camera
    // up and down is different because we are constantly going in and out of axies.
    // We need to find the axis that our camera is on, and that is why we use the cross
    // product.  By taking the cross product between our view vector and up vector,
    // we get a perpendicular vector to those 2 vectors, which is our desired axis.
    // Pick up a linear algebra book if you don't already have one, you'll need it.

    // Return the cross product
    return vNormal;                                         
}


/////////////////////////////////////// MAGNITUDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This returns the magnitude of a vector
/////
/////////////////////////////////////// MAGNITUDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

float Magnitude(CVector3 vNormal)
{
    // This will give us the magnitude or "Norm" as some say of, our normal.
    // The magnitude has to do with the length of the vector.  We use this
    // information to normalize a vector, which gives it a length of 1.
    // Here is the equation:  magnitude = sqrt(V.x^2 + V.y^2 + V.z^2)   Where V is the vector
    return (float)sqrt( (vNormal.x * vNormal.x) + 
                        (vNormal.y * vNormal.y) + 
                        (vNormal.z * vNormal.z) );
}


/////////////////////////////////////// NORMALIZE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This returns a normalize vector (A vector exactly of length 1)
/////
/////////////////////////////////////// NORMALIZE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

CVector3 Normalize(CVector3 vVector)
{
    // What's this function for you might ask?  Well, since we are using the cross
    // product formula, we need to make sure our view vector is normalized.  
    // For a vector to be normalized, it means that it has a length of 1.
    // For instance, a vector (2, 0, 0) would be (1, 0, 0) once normalized.
    // Most equations work well with normalized vectors.  If in doubt, normalize.

    // Get the magnitude of our normal
    float magnitude = Magnitude(vVector);                

    // Now that we have the magnitude, we can divide our vector by that magnitude.
    // That will make our vector a total length of 1.  
    // This makes it easier to work with too.
    vVector = vVector / magnitude;        
    
    // Finally, return our normalized vector
    return vVector;                                        
}

///////////////////////////////// POSITION CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function sets the camera's position and view and up vVector.
/////
///////////////////////////////// POSITION CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void camera :: PositionCamera(	float positionX, float positionY, float positionZ,
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

void camera :: MoveCamera(float speed)
{
    // Get our view vector (The direction we are facing)
    CVector3 vVector = m_vView - m_vPosition;
    vVector = Normalize(vVector);
    
    m_vPosition.x += vVector.x * speed;        // Add our acceleration to our position's X
    //m_vPosition.y += vVector.y * speed;
    m_vPosition.z += vVector.z * speed;        // Add our acceleration to our position's Z

    m_vView.x += vVector.x * speed;            // Add our acceleration to our view's X
    //m_vView.y += vVector.y * speed;
    m_vView.z += vVector.z * speed;            // Add our acceleration to our view's Z

}

///////////////////////////////// STRAFE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This strafes the camera left and right depending on the speed (-/+)
/////
///////////////////////////////// SLIDE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void camera :: SlideCamera(float speed)
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
    class_addmethod(classPtr, (t_method)&camera::hAngleMessCallback,
    	    gensym("hAngle"), A_FLOAT, A_NULL); 
    class_addmethod(classPtr, (t_method)&camera::vAngleMessCallback,
    	    gensym("vAngle"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&camera::distanceMessCallback,
    	    gensym("distance"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&camera::speedMessCallback,
    	    gensym("speed"), A_FLOAT, A_NULL); 
    class_addmethod(classPtr, (t_method)&camera::forwardMessCallback,
    	    gensym("forward"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&camera::reverseMessCallback,
    	    gensym("reverse"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&camera::leftMessCallback,
    	    gensym("left"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&camera::rightMessCallback,
    	    gensym("right"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&camera::upMessCallback,
    	    gensym("up"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&camera::downMessCallback,
    	    gensym("down"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&camera::resetMessCallback,
	    gensym("reset"), A_NULL);
    class_addmethod(classPtr, (t_method)&camera::modeMessCallback,
    	    gensym("mode"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&camera::lookXMessCallback,
    	    gensym("lookX"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&camera::lookYMessCallback,
    	    gensym("lookY"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&camera::lookZMessCallback,
    	    gensym("lookZ"), A_FLOAT, A_NULL);
}
void camera :: hAngleMessCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->hAngleMess(val);
}
void camera :: vAngleMessCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->vAngleMess(val);
}
void camera :: distanceMessCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->distanceMess((float)val);
}
void camera :: speedMessCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->m_speed=((float)val);
}
void camera :: forwardMessCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->forward=(int)val;
}
void camera :: reverseMessCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->reverse=(int)val;
}
void camera :: leftMessCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->left=(int)val;
}
void camera :: rightMessCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->right=(int)val;
}
void camera :: slideLeftMessCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->slideLeft=(int)val;
}
void camera :: slideRightMessCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->slideRight=(int)val;
}
void camera :: upMessCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->up=(int)val;
}
void camera :: downMessCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->down=(int)val;
}
void camera :: resetMessCallback(void *data)
{
    GetMyClass(data)->resetState();
}
void camera :: modeMessCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->m_mode=(int)val;
}
void camera :: lookXMessCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->m_vView.x=(val);
}
void camera :: lookYMessCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->m_vView.y=(val);
}
void camera :: lookZMessCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->m_vView.z=(val);
}
