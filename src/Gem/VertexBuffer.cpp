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

#include "VertexBuffer.h"

/* for post(), error(),... */
#include <m_pd.h>

gem::VertexBuffer:: VertexBuffer()
  : size(0) /* gemvertexbuffer, model, multimodel */
  , dimen(0) /* gemvertexbuffer, model, multimodel, scopeXYZ~ */
  , vbo(0) /* gemvertexbuffer, model, multimodel, scopeXYZ~ */
  , array(NULL) /* gemvertexbuffer, model, multimodel, scopeXYZ~ */
  , dirty(false) /* gemvertexbuffer, model, multimodel */
  , enabled(false) /* gemvertexbuffer, model, multimodel, scopeXYZ~ */
  , attrib_index(0) /* gemvertexbuffer */
  , attrib_name("") /* gemvertexbuffer */
  , attrib_array("") /* gemvertexbuffer */
  , offset(0) /* gemvertexbuffer */
{ }
gem::VertexBuffer:: VertexBuffer (unsigned int size_,
                                  unsigned int dimen_)
  : size(0)
  , dimen(dimen_)
  , vbo(0)
  , array(NULL)
  , dirty(false)
  , enabled(false)
  , attrib_index(0)
  , attrib_name("")
  , attrib_array("")
  , offset(0)
{
  resize(size_);
}
gem::VertexBuffer:: VertexBuffer (const gem::VertexBuffer&vb)
  : size(0)
  , dimen(vb.dimen)
  , vbo(vb.vbo)
  , array(NULL)
  , dirty(false)
  , enabled(vb.enabled)
  , attrib_index(vb.attrib_index)
  , attrib_name(vb.attrib_name)
  , attrib_array(vb.attrib_array)
  , offset(vb.offset)
{
  resize(vb.size);
  // TODO: shouldn't we copy the data from vb?
}

gem::VertexBuffer:: ~VertexBuffer (void)
{
  //::post("destroying VertexBuffer[%p] with %dx%d elements at %p", this, size, dimen, array);
  destroy();

  if(array) {
    delete[]array;
  }
  array=NULL;
}
void gem::VertexBuffer:: resize (unsigned int size_)
{
  float*tmp=NULL;
  try {
    tmp=new float[size_*dimen];
  } catch (std::bad_alloc& ba)  {
    pd_error(0, "vertexbuffer resize failed: %s ", ba.what());
    return;
  }
  if(array) {
    delete[]array;
    array=0;
  }
  array=tmp;
  size=size_;

  unsigned int i;
  for(i=0; i<size*dimen; i++) {
    array[i]=0;
  }
  dirty=true;
}

/* gemvertexbuffer, model, multimodel */
bool gem::VertexBuffer:: create (void)
{
  if(!(glGenBuffers && glBufferData && glBindBuffer)) {
    return false;
  }
  if(!vbo) {
    glGenBuffers(1, &vbo);
  }
  if(vbo) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size * dimen * sizeof(float), array,
                 GL_DYNAMIC_DRAW);
  }
  return (0!=vbo);
}
bool gem::VertexBuffer:: render (void)
{
  if(!(glBufferData && glBindBuffer)) {
    return false;
  }
  // render from the VBO
  //::post("VertexBuffer::render: %d?", enabled);
  if ( enabled ) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    if ( dirty ) {
      //::post("push vertex %p\n", this);
      glBufferData(GL_ARRAY_BUFFER, size * dimen * sizeof(float), array,
                   GL_DYNAMIC_DRAW);
      dirty = false;
    }
  }
  return enabled;
}
void gem::VertexBuffer:: destroy (void)
{
  if ( vbo ) {
    glBindBuffer(1, vbo);
    glDeleteBuffers(1, &vbo);
  }
  vbo=0;
}


gem::VBO::VBO(GLenum type, unsigned char dimen)
  : m_vbo(0)
  , m_size(0)
  , m_dimen(dimen)
  , m_type(type)
  , m_valid(false)
{
  if(!m_dimen) {
    switch(m_type) {
    case GL_VERTEX_ARRAY:
    case GL_NORMAL_ARRAY:
      m_dimen = 3;
      break;
    case GL_COLOR_ARRAY:
      m_dimen = 4;
      break;
    case GL_TEXTURE_COORD_ARRAY:
      m_dimen = 2;
      break;
    }
  }
}
int gem::VBO::render(void) const
{
  if(!m_valid || !m_vbo)
    return 0;
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

  switch(m_type) {
  case GL_VERTEX_ARRAY:
    glVertexPointer(m_dimen, GL_FLOAT, 0, 0);
    break;
  case GL_NORMAL_ARRAY:
    glNormalPointer(GL_FLOAT, 0, 0);
    break;
  case GL_COLOR_ARRAY:
    glColorPointer(m_dimen, GL_FLOAT, 0, 0);
    break;
  case GL_TEXTURE_COORD_ARRAY:
    glTexCoordPointer(m_dimen, GL_FLOAT, 0, 0);
    break;
  default:
    return -1;
  }
  glEnableClientState(m_type);
  return m_size;
}

bool gem::VBO::update(size_t argc, const float* argv)
{
  m_valid = false;

  if(!argc || !argv) {
    return false;
  }

  if(!(glGenBuffers && glBindBuffer && glBufferData)) {
    return false;
  }
  if(!m_vbo) {
    glGenBuffers(1, &m_vbo);
    m_size = 0;
  }
  if(!m_vbo) {
    return false;
  }

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  if(argc>m_size || !glBufferSubData) {
    glBufferData(GL_ARRAY_BUFFER, argc * m_dimen * sizeof(float), argv, GL_DYNAMIC_DRAW);
    m_size = argc;
  } else {
    glBufferSubData(GL_ARRAY_BUFFER, 0, argc * m_dimen * sizeof(float), argv);
  }
  m_valid = true;
  return m_valid;
}
void gem::VBO::destroy(void)
{
  if(m_vbo) {
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glDeleteBuffers(1, &m_vbo);
  }
  m_vbo = 0;
  m_size = 0;
  m_valid = false;
}
