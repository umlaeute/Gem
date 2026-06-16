////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "cylinder.h"
#include "Gem/State.h"

CPPEXTERN_NEW_WITH_TWO_ARGS(cylinder, t_floatarg, A_DEFFLOAT, t_floatarg,
                            A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// cylinder
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
cylinder :: cylinder(t_floatarg size,t_floatarg slize)
  : GemGluObj(size,slize),
    baseRadius(size),
    topRadius(size),
    height(size*2),
    slices(slize),
    stacks(slize)
{ }

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
cylinder :: ~cylinder()
{ }

////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void cylinder :: setupParameters(void)
{
  baseRadius=m_size;
  topRadius=m_size;
  height=m_size*2;
  slices=m_numSlices;
  stacks=m_numSlices;
}
void cylinder :: renderShape(GemState *state)
{
  setupParameters();
  if(m_drawType==GL_DEFAULT_GEM) {
    m_drawType=GL_FILL;
  }

  GLenum type = m_drawType;
  switch(m_drawType) {
  case GL_LINE_LOOP:
    type=GL_LINE;
    break;
  case GL_POINTS   :
    type=GL_POINT;
    break;
  case GL_DEFAULT_GEM: // default
  case GL_POLYGON  :
    type=GL_FILL;
    break;
  }
#ifdef GLU_TRUE
  switch(m_drawType) {
  case GLU_LINE :
    type=GL_LINE;
    break;
  case GLU_POINT:
    type=GL_POINT;
    break;
  case GLU_FILL :
    type=GL_FILL;
    break;
  }
#endif
  switch(type) {
  case GL_FILL:
  case GL_LINE:
  case GL_POINT:
    break;
  default:
    error("invalid draw type %d (%d), switching to default %d", m_drawType, type, GL_FILL);
    m_drawType = type = GL_FILL;
  }

  GLdouble da, r, dr, dz;
  GLfloat x, y, z, nz;
  GLint i, j;
  /* coverity[dead_error_condition] we might want to play with orientation (FIXME) */
  GLboolean orientation = false; /* true=INSIDE */
  GLfloat nsign = (orientation)?-1.0:1.0;

  TexCoord*texCoords=NULL;
  int texType=0;
  int texNum=0;
  bool lighting=false;
  state->get(GemState::_GL_TEX_COORDS, texCoords);
  state->get(GemState::_GL_TEX_TYPE, texType);
  state->get(GemState::_GL_TEX_NUMCOORDS, texNum);
  state->get(GemState::_GL_LIGHTING, lighting);

  GLfloat s0 = 0., s01 = -1., s03 =  0., s0123 = 0.;
  GLfloat t1 = 1., t01 =  0., t12 =  1., t0123 = 0.;

  if(texType && texNum>=3) {
    s0 = texCoords[0].s;
    s01 = s0 - texCoords[1].s;
    s03 = s0 - texCoords[3].s;
    s0123 = s01 + texCoords[2].s - texCoords[3].s;

    t1 = texCoords[1].t;
    t12 = t1 - texCoords[2].t;
    t01 = t1 - texCoords[0].t;
    t0123 = t01 - texCoords[2].t + texCoords[3].t;
  }

#define S(s, t) ((s0123*s - s03)*t - s01*s + s0)
#define T(s, t) ((t0123*t - t01)*s - t12*t + t1)



  glPushMatrix();
  glTranslatef(0.f, 0.f, -m_size);

  glPushAttrib(GL_POLYGON_BIT);
  glPolygonMode(GL_FRONT_AND_BACK, type);

  // gluCylinder(m_thing, m_size, m_size, m_size * 2, m_numSlices, m_numSlices);
  da = 2.0 * M_PI / slices;
  dr = (topRadius - baseRadius) / stacks;
  dz = height / stacks;
  nz = (baseRadius - topRadius) /
    height;       /* Z component of normal vectors */

  GLfloat ds = 1.0 / slices;
  GLfloat dt = 1.0 / stacks;
  GLfloat t = 0.0;
  z = 0.0;
  r = baseRadius;
  for (j = 0; j < stacks; j++) {
    GLfloat s = 0.0;
    glBegin(GL_QUAD_STRIP);
    for (i = 0; i <= slices; i++) {
      GLfloat x, y;
      if (i == slices) {
        x = sin(0.0);
        y = cos(0.0);
      } else {
        x = sin(i * da);
        y = cos(i * da);
      }
      glNormal3f(x * nsign, y * nsign, nz * nsign);
      if(texType) {
        glTexCoord2f(S(s, t), T(s, t));
      }
      glVertex3f(x * r, y * r, z);
      glNormal3f(x * nsign, y * nsign, nz * nsign);
      if(texType) {
        GLfloat t_ = t+dt;
        glTexCoord2f(S(s, t_), T(s, t_));
      }
      glVertex3f(x * (r + dr), y * (r + dr), z + dz);

      s += ds;
    }                 /* for slices */
    glEnd();
    r += dr;
    t += dt;
    z += dz;
  }                           /* for stacks */
  glPopAttrib();
  glPopMatrix();
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void cylinder :: obj_setupCallback(t_class *)
{ }
