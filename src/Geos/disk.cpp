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

/////////////////////////////////////////////////////////
// 2105:forum::für::umläute:2000
/////////////////////////////////////////////////////////
// added the gluPartialDisk
/////////////////////////////////////////////////////////

#include "disk.h"
#include "Gem/State.h"
#include "Gem/Exception.h"

CPPEXTERN_NEW_WITH_GIMME(disk);

/////////////////////////////////////////////////////////
//
// disk
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
disk :: disk(int argc, t_atom *argv)
  : GemGluObj(1.f)
{
  float size = 1.f;
  int numSlices = 10;
  m_innerRadius = 0.f;

  switch(argc) {
  case 3:
    size = atom_getfloat(&argv[0]);
    numSlices = atom_getint(&argv[1]);
    m_innerRadius = atom_getfloat(&argv[2]);
    break;
  case 2:
    size = atom_getfloat(&argv[0]);
    numSlices = atom_getint(&argv[1]);
    break;
  case 1:
    size = atom_getfloat(&argv[0]);
  case 0:
    break;
  default:
    throw(GemException("needs 0, 1, 2 or 3 arguments"));
  }

  sizeMess(size);
  numSlicesMess(numSlices);
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
            gensym("inner"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
disk :: ~disk(void)
{ }

/////////////////////////////////////////////////////////
// innerRadius
//
/////////////////////////////////////////////////////////
void disk :: innerRadius(float radius)
{
  m_innerRadius = radius;
  setModified();
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void disk :: renderShape(GemState *state)
{
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

  GLfloat da, dr;

  /* coverity[dead_error_condition] we might want to play with orientation (FIXME) */
  GLenum orientation = true; /* GLU_INSIDE; */

  TexCoord*texCoords=NULL;
  int texType=0;
  int texNum=0;
  bool lighting=false;
  state->get(GemState::_GL_TEX_COORDS, texCoords);
  state->get(GemState::_GL_TEX_TYPE, texType);
  state->get(GemState::_GL_TEX_NUMCOORDS, texNum);
  state->get(GemState::_GL_LIGHTING, lighting);


  GLboolean normals = (lighting);

  GLfloat xsize = 1.0, xsize0 = 0.0;
  GLfloat ysize = 1.0, ysize0 = 0.0;
  if(texType && texNum>=3) {
    xsize0 = texCoords[0].s;
    xsize  = texCoords[1].s-xsize0;
    ysize0 = texCoords[1].t;
    ysize  = texCoords[2].t-ysize0;
  }

  //gluDisk(m_thing, m_innerRadius, m_size, m_numSlices, m_numSlices);
  GLint loops = m_numSlices;


  /* Normal vectors */
  if (normals) {
    if (!orientation) {
      glNormal3f(0.0, 0.0, +1.0);
    } else {
      glNormal3f(0.0, 0.0, -1.0);
    }
  }

  da = 2.0 * M_PI / m_numSlices;
  dr = (m_size - m_innerRadius) / static_cast<GLfloat>(loops);

  GLfloat dtc = 2.0f * m_size;
  GLfloat sa, ca;
  GLfloat r1 = m_innerRadius;

  glPushAttrib(GL_POLYGON_BIT);
  glPolygonMode(GL_FRONT_AND_BACK, type);

  /* texture of a gluDisk is a cut out of the texture unit square
   * x, y in [-m_size, +m_size]; s, t in [0, 1]
   * (linear mapping)
   */
  GLint l;
  for (l = 0; l < loops; l++) {
    GLfloat r2 = r1 + dr;
    if (!orientation) {
      GLint s;
      glBegin(GL_QUAD_STRIP);
      for (s = 0; s <= m_numSlices; s++) {
	GLfloat a=(s == m_numSlices)?0.0:(s * da);
	sa = sin(a);
	ca = cos(a);
	if(texType) {
	  glTexCoord2f((0.5 + sa * r2 / dtc)*xsize+xsize0,
		       (0.5 + ca * r2 / dtc)*ysize+ysize0);
	}
	glVertex2f(r2 * sa, r2 * ca);
	if(texType) {
	  glTexCoord2f((0.5 + sa * r1 / dtc)*xsize+xsize0,
		       (0.5 + ca * r1 / dtc)*ysize+ysize0);
	}
	glVertex2f(r1 * sa, r1 * ca);
      }
      glEnd();
    } else {
      GLint s;
      glBegin(GL_QUAD_STRIP);
      for (s = m_numSlices; s >= 0; s--) {
	GLfloat a=(s==m_numSlices)?0.0:s * da;
	sa = sin(a);
	ca = cos(a);
	if(texType) {
	  glTexCoord2f((0.5 - sa * r2 / dtc)*xsize+xsize0,
		       (0.5 + ca * r2 / dtc)*ysize+ysize0);
	}
	glVertex2f(r2 * sa, r2 * ca);
	if(texType) {
	  glTexCoord2f((0.5 - sa * r1 / dtc)*xsize+xsize0,
		       (0.5 + ca * r1 / dtc)*ysize+ysize0);
	}
	glVertex2f(r1 * sa, r1 * ca);
      }
      glEnd();
    }
    r1 = r2;
  }
  glPopAttrib();
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void disk :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "inner", innerRadius, float);
}
