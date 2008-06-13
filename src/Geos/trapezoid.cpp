////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "trapezoid.h"

#include "Base/GemState.h"

CPPEXTERN_NEW_WITH_TWO_ARGS(trapezoid, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// trapezoid
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
trapezoid :: trapezoid(t_floatarg size, t_floatarg top)
  : GemShape(size),
    m_top(top),
    m_scale_texcoord(1.f)
{
  if(m_top==0.f)m_top=m_size;

  m_topinlet=floatinlet_new(this->x_obj, &m_top);
  m_scaleinlet=floatinlet_new(this->x_obj, &m_scale_texcoord);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
trapezoid :: ~trapezoid()
{ 
  inlet_free(m_topinlet);
  inlet_free(m_scaleinlet);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void trapezoid :: render(GemState *state)
{
  float top=m_size*m_top;
  float tx = m_scale_texcoord * m_top;
  int type=1;
  int drawType = m_drawType;
  if(drawType==GL_DEFAULT_GEM)drawType=GL_QUADS;

  if (drawType == GL_LINE_LOOP)
    glLineWidth(m_linewidth);
  if (m_top<0)
    top*=-1;

  glNormal3f(0.0f, 0.0f, 1.0f);

  glBegin(drawType);

  SetVertex(state,-m_size, -m_size, 0.0f,  0., 0.,         0);
  SetVertex(state, m_size, -m_size, 0.0f,  1., 0.,         1);
  SetVertex(state,    top,  m_size, 0.0f,  tx, tx, 0., tx, 2);
  SetVertex(state,   -top,  m_size, 0.0f,  0., tx, 0., tx, 3);

#if 0
  glTexCoord2f( 0,  0);
  glVertex2f  (-1, -1);

  glTexCoord2f( 256,  0);
  glVertex2f  ( 1, -1);

  glTexCoord4f( 256*tx,  256*tx, 0, tx);
  glVertex2f  ( m_size,  1);

  glTexCoord4f(  0,  256*tx, 0, tx);
  glVertex2f  (-m_size,  1);
#endif

  glEnd();

}
 
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void trapezoid :: obj_setupCallback(t_class *classPtr)
{     
  class_addmethod(classPtr, (t_method)&trapezoid::topMessCallback,
                  gensym("top"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&trapezoid::scaleMessCallback,
                  gensym("scale"), A_FLOAT, A_NULL);
}

void trapezoid :: scaleMessCallback(void *data, t_floatarg scale)
{
    GetMyClass(data)->m_scale_texcoord=(scale);
}

void trapezoid :: topMessCallback(void *data, t_floatarg top)
{
    GetMyClass(data)->m_top=top;
}
