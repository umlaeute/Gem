////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
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

#include "sphere.h"

CPPEXTERN_NEW_WITH_ONE_ARG(sphere, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// sphere
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
sphere :: sphere(t_floatarg size, t_floatarg slize)
  : GemGluObj(size, slize)
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
sphere :: ~sphere()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void sphere :: render(GemState *state)
{
  GLdouble radius=m_size;
  GLint slices=m_numSlices;
  GLint stacks=m_numSlices;

  GLfloat rho, drho, theta, dtheta;
  GLfloat x, y, z;
  GLfloat s, t, ds, dt;
  GLint i, j, imin, imax;
  GLboolean normals = (state->lighting)?GL_TRUE:GL_FALSE;
  GLenum orientation = true; /* GLU_INSIDE; */
  GLfloat nsign = (orientation)?-1.0:1.0;

  
  
  GLfloat xsize = 1.0, xsize0 = 0.0;
  GLfloat ysize = 1.0, ysize0 = 0.0;
  if(state->texture){
    xsize0 = state->texCoords[0].s;
    xsize  = state->texCoords[1].s-xsize0;
    ysize0 = state->texCoords[1].t;
    ysize  = state->texCoords[2].t-ysize0;
  }
    
  //    gluSphere(m_thing, m_size, m_numSlices, m_numSlices);
  drho = M_PI / (GLfloat) stacks;
  dtheta = 2.0 * M_PI / (GLfloat) slices;

  /* texturing: s goes from 0.0/0.25/0.5/0.75/1.0 at +y/+x/-y/-x/+y axis */
  /* t goes from -1.0/+1.0 at z = -radius/+radius (linear along longitudes) */
  /* cannot use triangle fan on texturing (s coord. at top/bottom tip varies) */

  if (m_drawType == GL_FILL) {
    if (!state->texture) {
      /* draw +Z end as a triangle fan */
      glBegin(GL_TRIANGLE_FAN);
      glNormal3f(0.0, 0.0, 1.0);
      glVertex3f(0.0, 0.0, nsign * radius);
      for (j = 0; j <= slices; j++) {
	theta = (j == slices) ? 0.0 : j * dtheta;
	x = -sin(theta) * sin(drho);
	y = cos(theta) * sin(drho);
	z = nsign * cos(drho);
	if (normals)
	  glNormal3f(x * nsign, y * nsign, z * nsign);
	glVertex3f(x * radius, y * radius, z * radius);
      }
      glEnd();
    }

    ds = 1.0 / slices;
    dt = 1.0 / stacks;
    t = 1.0;			/* because loop now runs from 0 */
    if (state->texture) {
      imin = 0;
      imax = stacks;
    }
    else {
      imin = 1;
      imax = stacks - 1;
    }

    /* draw intermediate stacks as quad strips */
    for (i = imin; i < imax; i++) {
      rho = i * drho;
      glBegin(GL_QUAD_STRIP);
      s = 0.0;
      for (j = 0; j <= slices; j++) {
	theta = (j == slices) ? 0.0 : j * dtheta;
	x = -sin(theta) * sin(rho);
	y = cos(theta) * sin(rho);
	z = nsign * cos(rho);
	if (normals)
	  glNormal3f(x * nsign, y * nsign, z * nsign);
	if(state->texture)glTexCoord2f(s*xsize+xsize0, t*ysize+ysize0);
	glVertex3f(x * radius, y * radius, z * radius);
	x = -sin(theta) * sin(rho + drho);
	y = cos(theta) * sin(rho + drho);
	z = nsign * cos(rho + drho);
	if (normals)
	  glNormal3f(x * nsign, y * nsign, z * nsign);
	if(state->texture)glTexCoord2f(s*xsize+xsize0, (t - dt)*ysize+ysize0);
	s += ds;
	glVertex3f(x * radius, y * radius, z * radius);
      }
      glEnd();
      t -= dt;
    }

    if (!state->texture) {
      /* draw -Z end as a triangle fan */
      glBegin(GL_TRIANGLE_FAN);
      glNormal3f(0.0, 0.0, -1.0);
      glVertex3f(0.0, 0.0, -radius * nsign);
      rho = M_PI - drho;
      s = 1.0;
      t = dt;
      for (j = slices; j >= 0; j--) {
	theta = (j == slices) ? 0.0 : j * dtheta;
	x = -sin(theta) * sin(rho);
	y = cos(theta) * sin(rho);
	z = nsign * cos(rho);
	if (normals)
	  glNormal3f(x * nsign, y * nsign, z * nsign);
	s -= ds;
	glVertex3f(x * radius, y * radius, z * radius);
      }
      glEnd();
    }
  }
  else if (m_drawType == GL_LINE || m_drawType == GLU_SILHOUETTE) {
    /* draw stack lines */
    for (i = 1; i < stacks; i++) {	/* stack line at i==stacks-1 was missing here */
      rho = i * drho;
      glBegin(GL_LINE_LOOP);
      for (j = 0; j < slices; j++) {
	theta = j * dtheta;
	x = cos(theta) * sin(rho);
	y = sin(theta) * sin(rho);
	z = cos(rho);
	if (normals)
	  glNormal3f(x * nsign, y * nsign, z * nsign);
	glVertex3f(x * radius, y * radius, z * radius);
      }
      glEnd();
    }
    /* draw slice lines */
    for (j = 0; j < slices; j++) {
      theta = j * dtheta;
      glBegin(GL_LINE_STRIP);
      for (i = 0; i <= stacks; i++) {
	rho = i * drho;
	x = cos(theta) * sin(rho);
	y = sin(theta) * sin(rho);
	z = cos(rho);
	if (normals)
	  glNormal3f(x * nsign, y * nsign, z * nsign);
	glVertex3f(x * radius, y * radius, z * radius);
      }
      glEnd();
    }
  }
  else if (m_drawType == GL_POINT) {
    /* top and bottom-most points */
    glBegin(GL_POINTS);
    if (normals)
      glNormal3f(0.0, 0.0, nsign);
    glVertex3d(0.0, 0.0, radius);
    if (normals)
      glNormal3f(0.0, 0.0, -nsign);
    glVertex3d(0.0, 0.0, -radius);

    /* loop over stacks */
    for (i = 1; i < stacks - 1; i++) {
      rho = i * drho;
      for (j = 0; j < slices; j++) {
	theta = j * dtheta;
	x = cos(theta) * sin(rho);
	y = sin(theta) * sin(rho);
	z = cos(rho);
	if (normals)
	  glNormal3f(x * nsign, y * nsign, z * nsign);
	glVertex3f(x * radius, y * radius, z * radius);
      }
    }
    glEnd();
  }
}
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void sphere :: obj_setupCallback(t_class *)
{ }

