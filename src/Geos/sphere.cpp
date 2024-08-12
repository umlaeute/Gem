////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "sphere.h"
#include "Gem/State.h"
CPPEXTERN_NEW_WITH_TWO_ARGS(sphere, t_floatarg, A_DEFFLOAT, t_floatarg,
                            A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// sphere
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
sphere :: sphere(t_floatarg size, t_floatarg slize)
  : GemGluObj(size, slize),
    m_x(0), m_y(0), m_z(0),
    oldStacks(-1), oldSlices(-1),
    oldDrawType(0), oldTexture(0)
{
  int slice = m_numSlices;
  int stack = m_numSlices;

  m_x = new float[slice * stack];
  m_y = new float[slice * stack];
  m_z = new float[slice * stack];

  oldDrawType = m_drawType;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
sphere :: ~sphere()
{
  delete[]m_x;
  delete[]m_y;
  delete[]m_z;
}

/////////////////////////////////////////////////////////
// createSphere
//      actually calculates the values of the vertices
//
/////////////////////////////////////////////////////////
void sphere :: createSphere(GemState *state)
{
  // GLdouble radius=m_size;
  GLint slices=(m_numSlices>0)?m_numSlices:10;
  GLint stacks=(m_numStacks>0)?m_numStacks:10;

  GLfloat rho, drho, theta, dtheta;
  GLint i, j, imin, imax;
  GLenum orientation = true; /* GLU_INSIDE; */
  /* coverity[dead_error_condition] we might want to play with orientation (FIXME) */
  GLfloat nsign = (orientation)?-1.0:1.0;

  TexCoord*texCoords=NULL;
  int texNum=0;
  int texType=0;
  state->get(GemState::_GL_TEX_COORDS, texCoords);
  state->get(GemState::_GL_TEX_TYPE, texType);
  state->get(GemState::_GL_TEX_NUMCOORDS, texNum);

  if(m_x) {
    delete[]m_x;
  }
  m_x=NULL;
  if(m_y) {
    delete[]m_y;
  }
  m_y=NULL;
  if(m_z) {
    delete[]m_z;
  }
  m_z=NULL;

  drho = M_PI / static_cast<GLfloat>(stacks);
  dtheta = 2.0 * M_PI / static_cast<GLfloat>(slices);

  int src;


  m_x = new float[slices * stacks * 3];
  m_y = new float[slices * stacks * 3];
  m_z = new float[slices * stacks * 3];

  src = 0;

  if (!texType) {
    /* draw +Z end as a triangle fan */

    for (j = 0; j <= slices; j++) {
      theta = (j == slices) ? 0.0 : j * dtheta;
      m_x[src] = -sin(theta) * sin(drho);
      m_y[src] = cos(theta) * sin(drho);
      m_z[src] = nsign * cos(drho);
      src++;
    }
  }

  if (texType) {
    imin = 0;
    imax = stacks;
  } else {
    imin = 1;
    imax = stacks - 1;
  }

  /* draw intermediate stacks as quad strips */
  for (i = imin; i < imax; i++) {
    rho = i * drho;
    for (j = 0; j <= slices; j++) {
      theta = (j == slices) ? 0.0 : j * dtheta;
      m_x[src] = -sin(theta) * sin(rho);
      m_y[src] = cos(theta) * sin(rho);
      m_z[src] = nsign * cos(rho);
      src++;
      m_x[src] = -sin(theta) * sin(rho + drho);
      m_y[src] = cos(theta) * sin(rho + drho);
      m_z[src] = nsign * cos(rho + drho);
      src++;

    }
  }

  if (!texType) {
    /* draw -Z end as a triangle fan */

    rho = M_PI - drho;
    for (j = slices; j >= 0; j--) {
      theta = (j == slices) ? 0.0 : j * dtheta;
      m_x[src] = -sin(theta) * sin(rho);
      m_y[src] = cos(theta) * sin(rho);
      m_z[src] = nsign * cos(rho);
      src++;
    }
  }
}
/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void sphere :: renderShape(GemState *state)
{
  GLdouble radius=m_size;
  GLint slices=(m_numSlices>0)?m_numSlices:10;
  GLint stacks=(m_numStacks>0)?m_numStacks:10;

  GLfloat s, t, ds, dt;
  GLint i, j, imin, imax;
  GLenum orientation = true; /* GLU_INSIDE; */
  /* coverity[dead_error_condition] we might want to play with orientation (FIXME) */
  GLfloat nsign = (orientation)?-1.0:1.0;

  int src=0;

  TexCoord*texCoords=NULL;
  int texType=0;
  int texNum=0;
  bool lighting=false;
  state->get(GemState::_GL_TEX_COORDS, texCoords);
  state->get(GemState::_GL_TEX_TYPE, texType);
  state->get(GemState::_GL_TEX_NUMCOORDS, texNum);
  state->get(GemState::_GL_LIGHTING, lighting);

  GLenum type;
  if(m_drawType==GL_DEFAULT_GEM) {
    m_drawType=GL_FILL;
  }
  type = m_drawType;


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

  GLfloat xsize = 1.0, xsize0 = 0.0;
  GLfloat ysize = 1.0, ysize0 = 0.0;
  if(texType && texNum>=3) {
    xsize0 = texCoords[0].s;
    xsize  = texCoords[1].s-xsize0;
    ysize0 = texCoords[1].t;
    ysize  = texCoords[2].t-ysize0;
  }

  ds = 1.0 / slices;
  dt = 1.0 / stacks;

  /* texturing: s goes from 0.0/0.25/0.5/0.75/1.0 at +y/+x/-y/-x/+y axis */
  /* t goes from -1.0/+1.0 at z = -radius/+radius (linear along longitudes) */
  /* cannot use triangle fan on texturing (s coord. at top/bottom tip varies) */

  //if anything changed then the geometry is rebuilt
  if (stacks != oldStacks || slices != oldSlices ||
      m_drawType != oldDrawType || texType!=oldTexture) {

    //call the sphere creation function to fill the array
    createSphere(state);

    oldStacks = stacks;
    oldSlices = slices;
    oldDrawType = m_drawType;
    oldTexture = texType;
  }

  src = 0;
  if (!texType) {
    /* draw +Z end as a triangle fan */
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, nsign * radius);
    for (j = 0; j <= slices; j++) {
      if (lighting) {
	glNormal3f(m_x[src] * nsign, m_y[src] * nsign, m_z[src] * nsign);
      }
      glVertex3f(m_x[src] * radius, m_y[src] * radius, m_z[src] * radius);
      src++;
    }
    glEnd();
  }

  ds = 1.0 / slices;
  dt = 1.0 / stacks;
  t = 1.0;                    /* because loop now runs from 0 */
  if (texType) {
    imin = 0;
    imax = stacks;
  } else {
    imin = 1;
    imax = stacks - 1;
  }

  glPushAttrib(GL_POLYGON_BIT);
  glPolygonMode(GL_FRONT_AND_BACK, type);

  /* draw intermediate stacks as quad strips */
  for (i = imin; i < imax; i++) {
    glBegin(GL_QUAD_STRIP);
    s = 0.0;
    for (j = 0; j <= slices; j++) {

      if (lighting) {
	glNormal3f(m_x[src] * nsign, m_y[src] * nsign, m_z[src] * nsign);
      }
      if(texType) {
	glTexCoord2f(s*xsize+xsize0, t*ysize+ysize0);
      }
      glVertex3f(m_x[src] * radius, m_y[src] * radius, m_z[src] * radius);
      src++;
      if (lighting) {
	glNormal3f(m_x[src] * nsign, m_y[src] * nsign, m_z[src] * nsign);
      }
      if(texType) {
	glTexCoord2f(s*xsize+xsize0, (t - dt)*ysize+ysize0);
      }
      s += ds;
      glVertex3f(m_x[src] * radius, m_y[src] * radius, m_z[src] * radius);
      src++;
    }
    glEnd();
    t -= dt;
  }

  if (!texType) {
    /* draw -Z end as a triangle fan */
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(0.0, 0.0, -radius * nsign);
    s = 1.0;
    t = dt;
    for (j = slices; j >= 0; j--) {
      if (lighting) {
	glNormal3f(m_x[src] * nsign, m_y[src] * nsign, m_z[src] * nsign);
      }
      s -= ds;
      glVertex3f(m_x[src] * radius, m_y[src] * radius, m_z[src] * radius);
      src++;
    }
    glEnd();
  }
  glPopAttrib();
}
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void sphere :: obj_setupCallback(t_class *)
{ }
