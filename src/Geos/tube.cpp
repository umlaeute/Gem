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
//		Diametter of the 1st circle (1st base of the object)
//		Diametter of the 2nd circle
//		X, Y, Z displacement between the 2 circle 
//		X, Y rotation of the 1st circle
//		X, Y rotation of the 2nd circle
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "tube.h"

#include "Base/Matrix.h"
#include <math.h>
#include <string.h>

#include "Base/GemState.h"

const int tube::NUM_PNTS = 80;
const float tube::TWO_PI = 8.f * (float)atan(1.f);

GLfloat *tube::m_cos = NULL;
GLfloat *tube::m_sin = NULL;

CPPEXTERN_NEW_WITH_TWO_ARGS(tube, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)


/////////////////////////////////////////////////////////
//
// circle
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
tube :: tube(t_floatarg size, t_floatarg size2)
  : GemShape(size), m_size2(size2)
{

  if (m_size2 == 0.f) m_size2 = 1.f;
  if (m_size == 0.f)  m_size = 1.f;

  // the 8 inlets
  m_inlet2 = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("size2"));
  m_inlethigh = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("high"));
  m_inletTX = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("TX"));
  m_inletTY = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("TY"));
  m_inletrotX1 = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("rotX1"));
  m_inletrotY1 = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("rotY1"));
  m_inletrotX2 = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("rotX2"));
  m_inletrotY2 = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("rotY2"));
 
  m_drawType = GL_TRIANGLE_STRIP;

  this->m_high = 1.0;
  this->m_TX = 0.0;
  this->m_TY = 0.0;
  this->sin_rotX1 = 0.0; 
  this->sin_rotX2 = 0.0; 
  this->sin_rotY1 = 0.0; 
  this->sin_rotY2 = 0.0; 
  this->cos_rotX1 = 1.0; 
  this->cos_rotX2 = 1.0; 
  this->cos_rotY1 = 1.0; 
  this->cos_rotY2 = 1.0; 
}

//////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
tube :: ~tube(){ 
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
// render
//
/////////////////////////////////////////////////////////
void tube :: render(GemState *state){
  GLfloat vectors1[NUM_PNTS+3][3];
  GLfloat vectors2[NUM_PNTS+3][3];
  GLfloat vectors_tmp; 
  float normal[3];

  int n;
   
  glLineWidth(m_linewidth);
  glBegin(m_drawType);

  for (n = 0; n < NUM_PNTS ; n++)    {
    // definition des vecteurs de base des cercles
    vectors1[n][0] = m_cos[n] * m_size;
    vectors1[n][1] = m_sin[n] * m_size;
    vectors1[n][2] = 0.0;

    vectors2[n][0] = m_cos[n] * m_size2;
    vectors2[n][1] = m_sin[n] * m_size2;
    vectors2[n][2] = 0.0;
    
    // rotation des vecteurs en x
	
    vectors_tmp    = cos_rotX1 * vectors1[n][1]; // - sin(m_rotX1) * vectors1[n][2];
    vectors1[n][2] = sin_rotX1 * vectors1[n][1]; // + cos(m_rotX1) * vectors1[n][2];
    vectors1[n][1] = vectors_tmp;

    vectors_tmp    = cos_rotX2 * vectors2[n][1]; // - sin(m_rotX2) * vectors2[n][2];
    vectors2[n][2] = sin_rotX2 * vectors2[n][1]; // + cos(m_rotX2) * vectors2[n][2];
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
		
  vectors2[NUM_PNTS][0] = vectors2[0][0];
  vectors2[NUM_PNTS][1] = vectors2[0][1];
  vectors2[NUM_PNTS][2] = vectors2[0][2];
  vectors2[NUM_PNTS+1][0] = vectors2[1][0];
  vectors2[NUM_PNTS+1][1] = vectors2[1][1];
  vectors2[NUM_PNTS+1][2] = vectors2[1][2];
  vectors2[NUM_PNTS+2][0] = vectors2[2][0];
  vectors2[NUM_PNTS+2][1] = vectors2[2][1];
  vectors2[NUM_PNTS+2][2] = vectors2[2][2];

  vectors1[NUM_PNTS][0] = vectors1[0][0];
  vectors1[NUM_PNTS][1] = vectors1[0][1];
  vectors1[NUM_PNTS][2] = vectors1[0][2];
  vectors1[NUM_PNTS+1][0] = vectors1[1][0];
  vectors1[NUM_PNTS+1][1] = vectors1[1][1];
  vectors1[NUM_PNTS+1][2] = vectors1[1][2];
  vectors1[NUM_PNTS+2][0] = vectors1[2][0];
  vectors1[NUM_PNTS+2][1] = vectors1[2][1];
  vectors1[NUM_PNTS+2][2] = vectors1[2][2];


  if (state->texture)    {
    GLfloat xsize = 1.0;
    GLfloat ysize0 = 0.0;
    GLfloat ysize1 = 1.0;

    if (state->image){
      xsize =  state->texCoords[1].s;
      ysize0 = state->texCoords[2].t;
      ysize1 = state->texCoords[1].t;
    }

    for (n = 1; n < NUM_PNTS + 2; n++)	{
      Matrix::generateNormal(vectors1[n-1], vectors2[n], vectors1[n+1], normal);
      glNormal3fv(normal);
      glTexCoord2f( xsize*(float)(n-1)/NUM_PNTS, ysize0 );
      glVertex3fv(vectors1[n]);

      Matrix::generateNormal(vectors2[n+1], vectors1[n], vectors2[n-1], normal);
      glNormal3fv(normal);
      glTexCoord2f( xsize*(float)(n-1)/NUM_PNTS, ysize1);
      glVertex3fv(vectors2[n]);
    }
  }  else  {
    for (n = 1; n < NUM_PNTS + 2 ; n++) {
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

//////////////////////////////////////////////////////////
// size2Mess
//
/////////////////////////////////////////////////////////
void tube :: sizeMess2(float size2){
  m_size2 = size2;
  setModified();
}

//////////////////////////////////////////////////////////
// highMess
//
/////////////////////////////////////////////////////////
void tube :: highMess(float high){
  m_high = high;
  setModified();
}

//////////////////////////////////////////////////////////
// size2Mess
//
/////////////////////////////////////////////////////////
void tube :: TXMess(float TX){
  m_TX = TX;
  setModified();
}

//////////////////////////////////////////////////////////
// size2Mess
//
/////////////////////////////////////////////////////////
void tube :: TYMess(float TY){
  m_TY = TY;
  setModified();
}

//////////////////////////////////////////////////////////
// size2Mess
//
/////////////////////////////////////////////////////////
void tube :: rotX1Mess(float rotX1){
  cos_rotX1 = cos(rotX1/360 * TWO_PI);
  sin_rotX1 = sin(rotX1/360 * TWO_PI);
  setModified();
}

//////////////////////////////////////////////////////////
// size2Mess
//
/////////////////////////////////////////////////////////
void tube :: rotX2Mess(float rotX2){
  cos_rotX2 = cos(rotX2/360 * TWO_PI);
  sin_rotX2 = sin(rotX2/360 * TWO_PI);
  setModified();
}

//////////////////////////////////////////////////////////
// size2Mess
//
/////////////////////////////////////////////////////////
void tube :: rotY1Mess(float rotY1){
  cos_rotY1 = cos(rotY1/360 * TWO_PI );
  sin_rotY1 = sin(rotY1/360 * TWO_PI);
  setModified();
}

//////////////////////////////////////////////////////////
// size2Mess
//
/////////////////////////////////////////////////////////
void tube :: rotY2Mess(float rotY2){
  cos_rotY2 = cos(rotY2/360 * TWO_PI);
  sin_rotY2 = sin(rotY2/360 * TWO_PI);
  setModified();
}

//////////////////////////////////////////////////////////
// typeMess
//
/////////////////////////////////////////////////////////
void tube :: typeMess(t_symbol *type){
  if (!strcmp(type->s_name, "line")) 
    m_drawType = GL_LINE_LOOP;
  else if (!strcmp(type->s_name, "fill")) 
    m_drawType = GL_TRIANGLE_STRIP;
  else if (!strcmp(type->s_name, "point"))
    m_drawType = GL_POINTS;
  else {
      error("GEM: tube draw style");
      return;
    }
  setModified();
}

//////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void tube :: obj_setupCallback(t_class *classPtr){
  m_cos = new GLfloat [NUM_PNTS];
  m_sin = new GLfloat [NUM_PNTS];

  // compute sin/cos lookup table
  for(int i = 0; i < NUM_PNTS; i++)    {
    m_cos[i] = (float)cos(TWO_PI * (double)i / (double)NUM_PNTS);
    m_sin[i] = (float)sin(TWO_PI * (double)i / (double)NUM_PNTS);
  }

  class_addmethod(classPtr, (t_method)&tube::sizeMessCallback2,
		  gensym("size2"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&tube::highMessCallback,
		  gensym("high"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&tube::TXMessCallback,
		  gensym("TX"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&tube::TYMessCallback,
		  gensym("TY"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&tube::rotX1MessCallback,
		  gensym("rotX1"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&tube::rotX2MessCallback,
		  gensym("rotX2"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&tube::rotY1MessCallback,
		  gensym("rotY1"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&tube::rotY2MessCallback,
		  gensym("rotY2"), A_FLOAT, A_NULL);
}

void tube :: sizeMessCallback2(void *data, t_floatarg size2)
{
  GetMyClass(data)->sizeMess2((float)size2);
}

void tube :: highMessCallback(void *data, t_floatarg high)
{
  GetMyClass(data)->highMess((float)high);
}

void tube :: TXMessCallback(void *data, t_floatarg TX)
{
  GetMyClass(data)->TXMess((float)TX);
}

void tube :: TYMessCallback(void *data, t_floatarg TY)
{
  GetMyClass(data)->TYMess((float)TY);
}

void tube :: rotX1MessCallback(void *data, t_floatarg rotX1)
{
  GetMyClass(data)->rotX1Mess((float)rotX1);
}

void tube :: rotX2MessCallback(void *data, t_floatarg rotX2)
{
  GetMyClass(data)->rotX2Mess((float)rotX2);
}

void tube :: rotY1MessCallback(void *data, t_floatarg rotY1)
{
  GetMyClass(data)->rotY1Mess((float)rotY1);
}

void tube :: rotY2MessCallback(void *data, t_floatarg rotY2)
{
  GetMyClass(data)->rotY2Mess((float)rotY2);
}

