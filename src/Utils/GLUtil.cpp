////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2014 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "GLUtil.h"
#include <map>
#include "Gem/RTE.h"

// I hate Microsoft...I shouldn't have to do this!
#ifdef _WIN32
/* disable warnings about unknown pragmas */
# pragma warning( disable : 4068 )
#endif


#define _GL_UNDEFINED -1

// if error dump gl errors to debugger string, return error
GLenum gem::utils::gl::glReportError (bool verbose)
{
  GLenum err = glGetError();
  if (verbose && GL_NO_ERROR != err) {
    post("GL: %s",(char*)gluErrorString(err));
  }
  // ensure we are returning an OSStatus noErr if no error condition
  if (err == GL_NO_ERROR) {
    return 0;
  } else {
    return err;
  }
}

#warning TODO: use gem::ContextData
using namespace gem::utils::gl;
struct gem::utils::gl::GLuintMap::PIMPL {
  std::map<float, GLuint>idmap;
  float nextfloat;
  PIMPL(void)
    : nextfloat(-0.5)
  {}
  float getNewKey()
  {
    nextfloat+=1.0;
    while(idmap.count(nextfloat>0)) {
      nextfloat+=1.0;
    }
    return nextfloat;
  }
  void del(float id)
  {
    std::map<float,GLuint>::iterator it = idmap.find(id);
    if(idmap.end() != it) {
      idmap.erase(it);
    }
  }

  static std::map<std::string, PIMPL*>s_pool;
  static PIMPL*getFromPool(const std::string&name)
  {
    if(s_pool.count(name)>0) {
      return s_pool[name];
    }
    PIMPL*pimpl=new PIMPL();
    s_pool[name]=pimpl;
    return pimpl;
  }
};
std::map<std::string, GLuintMap::PIMPL*>GLuintMap::PIMPL::s_pool;

GLuintMap::GLuintMap(const std::string&name)
  : m_pimpl(PIMPL::getFromPool(name))
{}

GLuintMap::~GLuintMap()
{}
GLuint GLuintMap::get(float f)
{
  if(m_pimpl->idmap.count(f)>0) {
    GLuint i = m_pimpl->idmap[f];
    return i;
  } else {
    throw(GemException("illegal key"));
  }
  return 0;
}
float GLuintMap::set(GLuint i, float f)
{
  if(f==0.f) {
    f=m_pimpl->getNewKey();
  }
  m_pimpl->idmap[f]=i;
  return f;
}
void GLuintMap::del(float f)
{
  m_pimpl->del(f);
}
