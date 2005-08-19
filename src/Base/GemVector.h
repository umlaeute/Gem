/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    vector-classes

    zmoelnig@iem.at, tigital@mac.com

    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMVECTOR_H_
#define INCLUDE_GEMVECTOR_H_

#include "Base/GemExportDef.h"



// This is our 2D point class.  This will be used to store the UV coordinates.
class GEM_EXTERN CVector2 {
public:
    float x, y;
};


// This is our basic 3D point/vector class
class GEM_EXTERN CVector3 {
public:
  // the elements of a vector:
  float x, y, z; 
    
    // A default constructor
  CVector3();

    // This is our constructor that allows us to initialize our data upon creating an instance
    CVector3(float X, float Y, float Z);

    // Here we overload the + operator so we can add vectors together 
    CVector3 operator+(CVector3 vVector);

    // Here we overload the - operator so we can subtract vectors 
    CVector3 operator-(CVector3 vVector);

    // Here we overload the - operator so we can negate the vector
    CVector3 operator-();

    // Here we overload the * operator so we can multiply by scalars
    CVector3 operator*(float num);

    // Here we overload the * operator so we can dot-multiply 
    float    operator*(CVector3 vVector);

     // cross-multiplication
    CVector3 cross(CVector3 vVector);

    // Here we overload the / operator so we can divide by a scalar
    CVector3 operator/(float num);


    // here we calculate the absolute-value of the vector
    float abs();

    // here we calculate the square of the absolute-value of the vector
    float abs2();

    // here we normalize the vector
    CVector3 normalize();

    // here we compare 2 vectors on approx. equality
    bool equals(CVector3 vVector, float epsilon);

    
};

#endif /* INCLUDE_GEMVECTOR_H_ */
