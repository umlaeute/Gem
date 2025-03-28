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

#include "Gem/GemConfig.h"
#include "GLUtil.h"
#include <map>
#include "Gem/RTE.h"
#include "Gem/ContextData.h"

#include <math.h>

// I hate Microsoft...I shouldn't have to do this!
#ifdef _WIN32
/* disable warnings about unknown pragmas */
# pragma warning( disable : 4068 )
#endif


#define _GL_UNDEFINED -1

namespace {
static const char* _gemglErrorString(GLenum err) {
  switch(err) {
  default: break;
  case GL_NO_ERROR: return "no error (GL)";
  case GL_INVALID_ENUM: return "invalid enumerant (GL)";
  case GL_INVALID_VALUE: return "invalid value (GL)";
  case GL_INVALID_OPERATION: return "invalid operation (GL)";
  case GL_STACK_OVERFLOW: return "stack overflow (GL)";
  case GL_STACK_UNDERFLOW: return "stack underflow (GL)";
  case GL_OUT_OF_MEMORY: return "out of memory (GL)";
  case GL_TABLE_TOO_LARGE: return "table too large (GL)";
  case GL_INVALID_FRAMEBUFFER_OPERATION: return "invalid framebuffer operation (GL)";
    //case GL_INVALID_FRAMEBUFFER_OPERATION_EXT: return "invalid framebuffer operation";

  case GL_CONTEXT_LOST: return "context lost (GL)";
  case GL_RELATIVE_LINE_TO_NV: return "relative line to nv (GL)";

    /* GLU */
#ifdef GLU_INVALID_ENUM
  case GLU_INVALID_ENUM: return "invalid enumerant (GLU)";
#endif
#ifdef GLU_INVALID_VALUE
  case GLU_INVALID_VALUE: return "invalid value (GLU)";
#endif
#ifdef GLU_OUT_OF_MEMORY
  case GLU_OUT_OF_MEMORY: return "out of memory (GLU)";
#endif
#ifdef GLU_INCOMPATIBLE_GL_VERSION
  case GLU_INCOMPATIBLE_GL_VERSION: return "incompatible gl version (GLU)";
#endif
#ifdef GLU_INVALID_OPERATION
  case GLU_INVALID_OPERATION: return "invalid operation (GLU)";
#endif /* GLU */
  }
  return "unknown error";
}

};
const char* gem::utils::gl::glErrorString(GLenum err) {
#ifdef GEM_HAVE_GLU
  return (const char*)gluErrorString(err);
#else
  return _gemglErrorString(err));
#endif
}


const char* gem::utils::gl::glErrorString(void) {
  GLenum err = glGetError();
  if(err != GL_NO_ERROR)
    return gem::utils::gl::glErrorString(err);
  return 0;
}

// if error dump gl errors to debugger string, return error
GLenum gem::utils::gl::glReportError (bool verbose)
{
  GLenum err = glGetError();
  if (verbose && GL_NO_ERROR != err) {
    const char *errStr = gem::utils::gl::glErrorString(err);
    post("GL[0x%X]: %s", err, errStr);
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
  std::map<float, gem::ContextData<GLuint> >idmap;
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
    std::map<float,gem::ContextData<GLuint> >::iterator it = idmap.find(id);
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


/*
 * SGI FREE SOFTWARE LICENSE B (Version 2.0, Sept. 18, 2008)
 * Copyright (C) 1991-2000 Silicon Graphics, Inc. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice including the dates of first publication and
 * either this permission notice or a reference to
 * http://oss.sgi.com/projects/FreeB/
 * shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * SILICON GRAPHICS, INC. BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Except as contained in this notice, the name of Silicon Graphics, Inc.
 * shall not be used in advertising or otherwise to promote the sale, use or
 * other dealings in this Software without prior written authorization from
 * Silicon Graphics, Inc.
 */
#define __glPi 3.14159265358979323846
static void normalize(float v[3])
{
  float r;

  r = sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );
  if (r == 0.0) {
    return;
  }

  v[0] /= r;
  v[1] /= r;
  v[2] /= r;
}

static void cross(float v1[3], float v2[3], float result[3])
{
  result[0] = v1[1]*v2[2] - v1[2]*v2[1];
  result[1] = v1[2]*v2[0] - v1[0]*v2[2];
  result[2] = v1[0]*v2[1] - v1[1]*v2[0];
}

void gem::utils::gl::gluLookAt (GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx, GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy, GLdouble upz)
{
  float forward[3], side[3], up[3];
  GLfloat m[4][4];

  forward[0] = centerx - eyex;
  forward[1] = centery - eyey;
  forward[2] = centerz - eyez;

  up[0] = upx;
  up[1] = upy;
  up[2] = upz;

  normalize(forward);

  /* Side = forward x up */
  cross(forward, up, side);
  normalize(side);

  /* Recompute up as: up = side x forward */
  cross(side, forward, up);

  for(size_t i=0; i<4; i++)
    for(size_t j=0; j<4; j++) {
      m[i][j] = (i==j)?1.:0.;
    }

  m[0][0] = side[0];
  m[1][0] = side[1];
  m[2][0] = side[2];

  m[0][1] = up[0];
  m[1][1] = up[1];
  m[2][1] = up[2];

  m[0][2] = -forward[0];
  m[1][2] = -forward[1];
  m[2][2] = -forward[2];

  glMultMatrixf(&m[0][0]);
  glTranslated(-eyex, -eyey, -eyez);
}

void gem::utils::gl::gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
  GLdouble m[4][4];
  double sine, cotangent, deltaZ;
  double radians = fovy / 2 * __glPi / 180;

  deltaZ = zFar - zNear;
  sine = sin(radians);
  if ((deltaZ == 0) || (sine == 0) || (aspect == 0)) {
    return;
  }
  cotangent = cos(radians) / sine;
  for(size_t i=0; i<4; i++)
    for(size_t j=0; j<4; j++) {
      m[i][j] = (i==j)?1.:0.;
    }
  m[0][0] = cotangent / aspect;
  m[1][1] = cotangent;
  m[2][2] = -(zFar + zNear) / deltaZ;
  m[2][3] = -1;
  m[3][2] = -2 * zNear * zFar / deltaZ;
  m[3][3] = 0;
  glMultMatrixd(&m[0][0]);
}


#define CASE2NAME(x) case x: return #x

namespace gem { namespace utils { namespace gl {
const char*pixtype2name (GLenum type) {
  switch(type) {
  CASE2NAME(GL_BYTE);
  CASE2NAME(GL_UNSIGNED_BYTE);
  CASE2NAME(GL_SHORT);
  CASE2NAME(GL_UNSIGNED_SHORT);
  CASE2NAME(GL_INT);
  CASE2NAME(GL_UNSIGNED_INT);
  CASE2NAME(GL_FLOAT);
  CASE2NAME(GL_2_BYTES);
  CASE2NAME(GL_3_BYTES);
  CASE2NAME(GL_4_BYTES);
  CASE2NAME(GL_DOUBLE);

  CASE2NAME(GL_UNSIGNED_BYTE_3_3_2);
  CASE2NAME(GL_UNSIGNED_BYTE_2_3_3_REV);

  CASE2NAME(GL_UNSIGNED_SHORT_4_4_4_4);
  CASE2NAME(GL_UNSIGNED_SHORT_5_5_5_1);
  CASE2NAME(GL_UNSIGNED_SHORT_5_6_5);
  CASE2NAME(GL_UNSIGNED_SHORT_5_6_5_REV);
  CASE2NAME(GL_UNSIGNED_SHORT_4_4_4_4_REV);
  CASE2NAME(GL_UNSIGNED_SHORT_1_5_5_5_REV);
  CASE2NAME(GL_UNSIGNED_SHORT_8_8_APPLE);
  CASE2NAME(GL_UNSIGNED_SHORT_8_8_REV_APPLE);

  CASE2NAME(GL_UNSIGNED_INT_8_8_8_8);
  CASE2NAME(GL_UNSIGNED_INT_10_10_10_2);
  CASE2NAME(GL_UNSIGNED_INT_8_8_8_8_REV);
  CASE2NAME(GL_UNSIGNED_INT_2_10_10_10_REV);
  CASE2NAME(GL_UNSIGNED_INT_24_8);
  CASE2NAME(GL_UNSIGNED_INT_S8_S8_8_8_NV);
  CASE2NAME(GL_UNSIGNED_INT_8_8_S8_S8_REV_NV);
  CASE2NAME(GL_UNSIGNED_INT_10F_11F_11F_REV);
  CASE2NAME(GL_UNSIGNED_INT_5_9_9_9_REV);

  default:
    break;
  }
  return 0;
}


const char*pixformat2name (GLenum format) {
  switch(format) {
    CASE2NAME(GL_RED);
    CASE2NAME(GL_GREEN);
    CASE2NAME(GL_BLUE);
    CASE2NAME(GL_ALPHA);
    CASE2NAME(GL_RGB);
    CASE2NAME(GL_RGBA);
    CASE2NAME(GL_LUMINANCE);
    CASE2NAME(GL_LUMINANCE_ALPHA);

    CASE2NAME(GL_BGR);
    CASE2NAME(GL_BGRA);

#ifdef GL_ABGR_EXT
    CASE2NAME(GL_ABGR_EXT);
#endif
#ifdef GL_ARGB_EXT
    CASE2NAME(GL_ARGB_EXT);
#endif
#ifdef GL_YCBCR_422_APPLE
    CASE2NAME(GL_YCBCR_422_APPLE);
#else
    CASE2NAME(GL_YUV422_GEM);
#endif
  default: break;
  }
  return 0;
}
}}}
