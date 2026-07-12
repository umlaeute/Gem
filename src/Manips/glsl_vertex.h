/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2005, James Tittle II and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_GLSL_VERTEX_H_
#define _INCLUDE__GEM_MANIPS_GLSL_VERTEX_H_

#include "Base/GemBase.h"
#include "Utils/GLUtil.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  glsl_vertex

  Loads in a GLSL vertex shader

  KEYWORDS

  DESCRIPTION

  -----------------------------------------------------------------*/

#define GLSL_GETPOSTINT(pname) do { \
      GLint data = 0; \
      glGetIntegerv(GL_##pname, &data); \
      post(#pname ": %d", data); \
    } while(0)

class GEM_EXTERN glsl_vertex : public GemBase
{
  CPPEXTERN_HEADER(glsl_vertex, GemBase);

public:

  //////////
  // Constructor
  glsl_vertex(void);
  glsl_vertex(t_symbol* filename);

protected:

  //////////
  // Destructor
  virtual ~glsl_vertex();

  //////////
  // close the shader file
  virtual void closeMess(void);
  //////////
  // open a shader up
  virtual bool openMessGL2(void);
  virtual bool openMessARB(void);
  virtual void openMess(t_symbol* filename);
  virtual void loadShader(void);


  ////////
  // extension check
  virtual bool isRunnable(void);

  //////////
  // Do the rendering
  virtual void render(GemState *state);

  //////////
  // Clear the dirty flag on the pixBlock
  virtual void postrender(GemState *state);

  //////////
  virtual void startRendering();
  virtual void stopRendering();

  //////////
  // Print Info about Hardware limits
  virtual void printInfo();

  //////////
  //
  std::string m_shaderString;
  gem::ContextData<GLuint>m_shaderTarget;
  enum shaderType { NONE=0, ARB=1, GL2=2 };
  gem::ContextData<shaderType>m_shaderType;
  gem::ContextData<GLuint>m_shader;
  gem::ContextData<GLhandleARB>m_shaderARB;

  t_outlet              *m_outShaderID;
  gem::utils::gl::GLuintMap m_idmapper;
  float                 m_idmapped;
};

#endif  // for header file
