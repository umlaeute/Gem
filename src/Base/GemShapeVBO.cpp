////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "GemShapeVBO.h"
#include "Gem/State.h"
#include "Utils/GLUtil.h"

/////////////////////////////////////////////////////////
//
// a generic GemShapeVBO
//

// outline:
//   we ignore interleaved arrays for now,
//   assuming that this is only used for *small* shapes (the usual Geos)


/////////////////////////////////////////////////////////
// PIMPL
class GemShapeVBO :: PIMPL {
public:
  class VBO {
    std::vector<float>m_data;
    GLuint m_vbo;
    GLenum m_type;
    GLuint m_stride;
  public:
    VBO(arraytype_t type, std::vector<float>&data, GLuint stride=3) :
      m_data(data),
      m_vbo(0),
      m_type(0),
      m_stride(stride)
    {
      switch(type) {
      case VERTEX  : m_type=GL_VERTEX_ARRAY; break;
      case COLOR   : m_type=GL_COLOR_ARRAY; break;
      case NORMAL  : m_type=GL_NORMAL_ARRAY; break;
      case TEXCOORD: m_type=GL_TEXTURE_COORD_ARRAY; break;
      }
    }
    ~VBO(void) {
      if(m_vbo) {
	glBindBuffer(1, m_vbo);
	glDeleteBuffers(1, &m_vbo);
      }
      m_vbo=0;
    }
    void render(void) {
      if(!m_vbo) {
	glGenBuffers(1, &m_vbo);
	if(!m_vbo)return;
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_data.size() * sizeof(float), &m_data[0], GL_STATIC_DRAW);
      }
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      switch(m_type) {
      case GL_VERTEX_ARRAY: glVertexPointer(m_stride, GL_FLOAT, 0, 0); break;
      case GL_NORMAL_ARRAY: glNormalPointer(GL_FLOAT, 0, 0); break;

      case GL_COLOR_ARRAY:  glColorPointer (m_stride, GL_FLOAT, 0, 0); break;
      case GL_TEXTURE_COORD_ARRAY: glTexCoordPointer(m_stride, GL_FLOAT, 0, 0); break;
      };
      glEnableClientState(m_type);
    }
    void postrender(void) {
      if(m_vbo)
	glDisableClientState(m_type);
    }

  };

  std::vector<VBO*> m_arrays;
  unsigned int m_size;
  GLenum m_defaultDrawType;
  PIMPL(void) : 
    m_size(0),
    m_defaultDrawType(GL_POLYGON)
  {
    m_arrays.resize(GemShapeVBO::_LAST);
  }
  ~PIMPL(void) {
    unsigned int i;
    for(i=0; i<m_arrays.size(); i++) {
      if(m_arrays[i])
	delete m_arrays[i];
      m_arrays[i]=NULL;
    }
  }

  void renderVBO(GLenum drawType) {
    unsigned int i;
    for(i=0; i<m_arrays.size(); i++) {
      if(m_arrays[i]) {
	m_arrays[i]->render();
      }
    }
    glDrawArrays(drawType, 0, m_size);
    for(i=0; i<m_arrays.size(); i++) {
      if(m_arrays[i]) {
	m_arrays[i]->postrender();
      }
    }
  }
  void setArray(arraytype_t type, std::vector<float>&values, unsigned int stride) {
    unsetArray(type);
    if(m_arrays.size()<=type) {
      m_arrays.resize(type+1);
    }
    m_arrays[type]=new PIMPL::VBO(type, values, stride);

    if(VERTEX==type || 1>m_size) {
      m_size   = values.size()/stride;
    }
  }
  void unsetArray(arraytype_t type) {
    if(m_arrays.size()<=type) {
      return;
    }
    if(m_arrays[type])
      delete m_arrays[type];
  }

};



/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GemShapeVBO :: GemShapeVBO(t_floatarg size)
  : GemShape(size),
    m_vbo(1),
    m_pimpl(new PIMPL)
{}
GemShapeVBO :: GemShapeVBO()
  : GemShape(),
    m_vbo(1),
    m_pimpl(new PIMPL)
{}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GemShapeVBO :: ~GemShapeVBO()
{
  if(m_pimpl)delete m_pimpl;m_pimpl=0;
}

void GemShapeVBO :: render(GemState *state)
{
  if(m_drawType==GL_DEFAULT_GEM)m_drawType=m_pimpl->m_defaultDrawType;
  if (m_drawType == GL_LINE_LOOP || m_drawType == GL_LINE_STRIP || m_drawType == GL_LINES)
    glLineWidth(m_linewidth);

  if (m_blend) {
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glHint(GL_POLYGON_SMOOTH_HINT,GL_DONT_CARE);
  }

  m_texType=0;
  m_texNum =0;
  m_texCoords=NULL;
  m_lighting=false;

  state->get(GemState::_GL_TEX_COORDS,    m_texCoords);
  state->get(GemState::_GL_TEX_TYPE,      m_texType);
  state->get(GemState::_GL_TEX_NUMCOORDS, m_texNum);
  state->get(GemState::_GL_LIGHTING,      m_lighting);

  switch(m_vbo) {
  case 0: renderShape(state); break;
  case 1: m_pimpl->renderVBO(m_drawType); break;
  default: error("illegal rendermode: VBO=%d", m_vbo);
  }

  // LATER try to restore the original state
  if (m_blend) {
    glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_BLEND);
  }

  if (m_drawType == GL_LINE_LOOP || m_drawType == GL_LINE_STRIP || m_drawType == GL_LINES)
    glLineWidth(1.0);
}

void GemShapeVBO :: vboMess(int mode) {
  switch(mode) {
  case 0:
  case 1:
    m_vbo=mode;
    break;
  default:
    error("allowed VBO-modes: 0, 1");
    break;
  }
}

void GemShapeVBO :: setArray(arraytype_t type, std::vector<float>&values, unsigned int stride) {
  if(values.size()%stride)
    error("trying to set VBO array with %d values but %d per element", values.size(), stride);
  else 
    m_pimpl->setArray(type, values, stride);
}
void GemShapeVBO :: unsetArray(arraytype_t type) {
  m_pimpl->unsetArray(type);
}

void GemShapeVBO :: setDefaultDrawtype(GLenum mode) {
  m_pimpl->m_defaultDrawType=mode;
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void GemShapeVBO :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "vbo", vboMess, int);
}
