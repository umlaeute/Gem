////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
//    A tube.
//
// object by cyrille.henry@la-kitchen.fr
// This primitive create a kind of cilender with paramettre :
//              Diameter of the 1st circle (1st base of the object)
//              Diameter of the 2nd circle
//              X, Y, Z displacement between the 2 circle
//              X, Y rotation of the 1st circle
//              X, Y rotation of the 2nd circle
//
// Implementation file
//
//    Copyright (c) 2003 Cyrille Henry. La Kitchen, Paris
//    Copyright (c) 2003-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "tube.h"

#include "Utils/Matrix.h"
#include <math.h>

#include "Gem/State.h"

const float tube::TWO_PI = 8.f * atan(1.);

/* only gcc allows one to allocate arrays of non-constant length, like:
 *   int i;
 *   float array[i];
 * other compilers will need to know the array-size at runtime!
 */
#ifndef __GNUC__
# define TUBE_NUMPTS 80
#endif

CPPEXTERN_NEW_WITH_FOUR_ARGS(tube, t_floatarg, A_DEFFLOAT, t_floatarg,
                             A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// tube
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
tube :: tube(t_floatarg size, t_floatarg size2, t_floatarg height,
             t_floatarg order_in)
  : GemShape(size),
    m_cos(NULL), m_sin(NULL),
    m_size2(size2), m_inlet2(NULL),
    m_high(height), m_inlethigh(NULL),
    m_TX(0.0), m_inletTX(NULL),
    m_TY(0.0), m_inletTY(NULL),
    cos_rotX1(1.0),   sin_rotX1(0.0), m_inletrotX1(NULL),
    cos_rotY1(1.0),   sin_rotY1(0.0), m_inletrotY1(NULL),
    cos_rotX2(1.0),   sin_rotX2(0.0), m_inletrotX2(NULL),
    cos_rotY2(1.0),   sin_rotY2(0.0), m_inletrotY2(NULL),
    order(80)
{
  if (m_size2 == 0.f) {
    m_size2 = 1.f;
  }
  if (m_size == 0.f) {
    m_size = 1.f;
  }

  if(order_in<1.f) {
    order_in=80.f;
  }
  order=static_cast<int>(order_in)-1; // make sure they are different
  slicesMess(static_cast<int>(order_in));

  // the 8 inlets
  m_inlet2     = inlet_new(x_obj, &x_obj->ob_pd, &s_float, gensym("size2"));
  m_inlethigh  = inlet_new(x_obj, &x_obj->ob_pd, &s_float, gensym("high"));
  m_inletTX    = inlet_new(x_obj, &x_obj->ob_pd, &s_float, gensym("TX"));
  m_inletTY    = inlet_new(x_obj, &x_obj->ob_pd, &s_float, gensym("TY"));
  m_inletrotX1 = inlet_new(x_obj, &x_obj->ob_pd, &s_float, gensym("rotX1"));
  m_inletrotY1 = inlet_new(x_obj, &x_obj->ob_pd, &s_float, gensym("rotY1"));
  m_inletrotX2 = inlet_new(x_obj, &x_obj->ob_pd, &s_float, gensym("rotX2"));
  m_inletrotY2 = inlet_new(x_obj, &x_obj->ob_pd, &s_float, gensym("rotY2"));

  m_drawType = GL_TRIANGLE_STRIP;

  m_drawTypes.clear();
  m_drawTypes["default"]=GL_TRIANGLE_STRIP;
  m_drawTypes["point"]=GL_POINTS;
  m_drawTypes["points"]=GL_POINTS;
  m_drawTypes["line"]=GL_LINE_LOOP;
  m_drawTypes["fill"]=GL_TRIANGLE_STRIP;
}

//////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
tube :: ~tube(void)
{
  inlet_free(m_inlet2);
  inlet_free(m_inlethigh);
  inlet_free(m_inletTX);
  inlet_free(m_inletTY);
  inlet_free(m_inletrotX1);
  inlet_free(m_inletrotY1);
  inlet_free(m_inletrotX2);
  inlet_free(m_inletrotY2);
}

//////////////////////////////////////////////////////////
// renderShape
//
/////////////////////////////////////////////////////////
void tube :: renderShape(GemState *state)
{
#ifdef __GNUC__
  GLfloat vectors1[order+3][3];
  GLfloat vectors2[order+3][3];
#else
  GLfloat vectors1[TUBE_NUMPTS+3][3];
  GLfloat vectors2[TUBE_NUMPTS+3][3];
#endif
  GLfloat vectors_tmp;
  float normal[3];

  int n;

  if(m_drawType==GL_DEFAULT_GEM) {
    m_drawType=GL_TRIANGLE_STRIP;
  }


  for (n = 0; n < order ; n++)    {
    // definition des vecteurs de base des cercles
    vectors1[n][0] = m_cos[n] * m_size;
    vectors1[n][1] = m_sin[n] * m_size;
    vectors1[n][2] = 0.0;

    vectors2[n][0] = m_cos[n] * m_size2;
    vectors2[n][1] = m_sin[n] * m_size2;
    vectors2[n][2] = 0.0;

    // rotation des vecteurs en x
    vectors_tmp    = cos_rotX1 *
                     vectors1[n][1]; // - sin(m_rotX1) * vectors1[n][2];
    vectors1[n][2] = sin_rotX1 *
                     vectors1[n][1]; // + cos(m_rotX1) * vectors1[n][2];
    vectors1[n][1] = vectors_tmp;

    vectors_tmp    = cos_rotX2 *
                     vectors2[n][1]; // - sin(m_rotX2) * vectors2[n][2];
    vectors2[n][2] = sin_rotX2 *
                     vectors2[n][1]; // + cos(m_rotX2) * vectors2[n][2];
    vectors2[n][1] = vectors_tmp;

    // rotation des vecteurs en y
    vectors_tmp    =  cos_rotY1 * vectors1[n][0] + sin_rotY1 * vectors1[n][2];
    vectors1[n][2] = -sin_rotY1 * vectors1[n][0] + cos_rotY1 * vectors1[n][2];
    vectors1[n][0] = vectors_tmp;

    vectors_tmp    =  cos_rotY2 * vectors2[n][0] + sin_rotY2 * vectors2[n][2];
    vectors2[n][2] = -sin_rotY2 * vectors2[n][0] + cos_rotY2 * vectors2[n][2];
    vectors2[n][0] = vectors_tmp;

    // translation du 2eme cercle par rapport au 1er
    vectors2[n][0] += m_TX;
    vectors2[n][1] += m_TY;
    vectors2[n][2] += m_high;
  }
  // copie des premiers vecteurs a la fin du tableau pour reboucler proprement

  vectors2[order][0] = vectors2[0][0];
  vectors2[order][1] = vectors2[0][1];
  vectors2[order][2] = vectors2[0][2];
  vectors2[order+1][0] = vectors2[1][0];
  vectors2[order+1][1] = vectors2[1][1];
  vectors2[order+1][2] = vectors2[1][2];
  vectors2[order+2][0] = vectors2[2][0];
  vectors2[order+2][1] = vectors2[2][1];
  vectors2[order+2][2] = vectors2[2][2];

  vectors1[order][0] = vectors1[0][0];
  vectors1[order][1] = vectors1[0][1];
  vectors1[order][2] = vectors1[0][2];
  vectors1[order+1][0] = vectors1[1][0];
  vectors1[order+1][1] = vectors1[1][1];
  vectors1[order+1][2] = vectors1[1][2];
  vectors1[order+2][0] = vectors1[2][0];
  vectors1[order+2][1] = vectors1[2][1];
  vectors1[order+2][2] = vectors1[2][2];

  glLineWidth(m_linewidth);
  glBegin(m_drawType);

  if (GemShape::m_texType)    {
    GLfloat xsize = 1.0;
    GLfloat ysize0 = 0.0;
    GLfloat ysize1 = 1.0;

    if (GemShape::m_texNum>=3) {
      xsize =  GemShape::m_texCoords[1].s;
      ysize0 = GemShape::m_texCoords[2].t;
      ysize1 = GemShape::m_texCoords[1].t;
    }

    for (n = 1; n < order + 2 ; n++)    {
      Matrix::generateNormal(vectors1[n-1], vectors2[n], vectors1[n+1], normal);
      glNormal3fv(normal);
      glTexCoord2f( 1.*xsize*(n-1)/order, ysize0 );
      glVertex3fv(vectors1[n]);

      Matrix::generateNormal(vectors2[n+1], vectors1[n], vectors2[n-1], normal);
      glNormal3fv(normal);
      glTexCoord2f( 1.*xsize*(n-1)/order, ysize1);
      glVertex3fv(vectors2[n]);
    }
  }  else  {
    for (n = 1; n < order + 2; n++) {
      Matrix::generateNormal(vectors1[n-1], vectors2[n], vectors1[n+1], normal);
      glNormal3fv(normal);
      glVertex3fv(vectors1[n]);

      Matrix::generateNormal(vectors2[n+1], vectors1[n], vectors2[n-1], normal);
      glNormal3fv(normal);
      glVertex3fv(vectors2[n]);
    }
  }

  glEnd();
  glLineWidth(1.0);
}

void tube :: sizeMess2(float size2)
{
  m_size2 = size2;
  setModified();
}

void tube :: highMess(float high)
{
  m_high = high;
  setModified();
}

void tube :: TXMess(float TX)
{
  m_TX = TX;
  setModified();
}
void tube :: TYMess(float TY)
{
  m_TY = TY;
  setModified();
}
void tube :: rotX1Mess(float rotX1)
{
  cos_rotX1 = cos(rotX1/360 * TWO_PI);
  sin_rotX1 = sin(rotX1/360 * TWO_PI);
  setModified();
}
void tube :: rotX2Mess(float rotX2)
{
  cos_rotX2 = cos(rotX2/360 * TWO_PI);
  sin_rotX2 = sin(rotX2/360 * TWO_PI);
  setModified();
}
void tube :: rotY1Mess(float rotY1)
{
  cos_rotY1 = cos(rotY1/360 * TWO_PI );
  sin_rotY1 = sin(rotY1/360 * TWO_PI);
  setModified();
}
void tube :: rotY2Mess(float rotY2)
{
  cos_rotY2 = cos(rotY2/360 * TWO_PI);
  sin_rotY2 = sin(rotY2/360 * TWO_PI);
  setModified();
}
//////////////////////////////////////////////////////////
// slicesMess
//
/////////////////////////////////////////////////////////
void tube :: slicesMess(int slices)
{
#ifndef __GNUC__
  if(slices>TUBE_NUMPTS) {
    error("number of slices (%d) clamped to %d", slices, TUBE_NUMPTS);
    slices=TUBE_NUMPTS;
  }
#endif

  if(slices==order) {
    return;
  }

  order=slices;

  if (order < 1) {
    order = 1;
  }

  if(m_cos) {
    delete[]m_cos;
  }
  if(m_sin) {
    delete[]m_sin;
  }

#ifndef __GNUC__
  m_cos = new GLfloat[TUBE_NUMPTS];
  m_sin = new GLfloat[TUBE_NUMPTS];
#else
  m_cos = new GLfloat[order];
  m_sin = new GLfloat[order];
#endif

  for(int i=0; i<order; i++) {
    m_cos[i] = cos(TWO_PI * 1. * i / order);
    m_sin[i] = sin(TWO_PI * 1. * i / order);
  }

  setModified();
}

//////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void tube :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "size2", sizeMess2, float);
  CPPEXTERN_MSG1(classPtr, "high", highMess, float);

  CPPEXTERN_MSG1(classPtr, "TX", TXMess, float);
  CPPEXTERN_MSG1(classPtr, "TY", TYMess, float);

  CPPEXTERN_MSG1(classPtr, "rotX1", rotX1Mess, float);
  CPPEXTERN_MSG1(classPtr, "rotX2", rotX2Mess, float);
  CPPEXTERN_MSG1(classPtr, "rotY1", rotY1Mess, float);
  CPPEXTERN_MSG1(classPtr, "rotY2", rotY2Mess, float);

  CPPEXTERN_MSG1(classPtr, "numslices", slicesMess, int);
}
