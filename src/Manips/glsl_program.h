/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2005, James Tittle II and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_GLSL_PROGRAM_H_
#define _INCLUDE__GEM_MANIPS_GLSL_PROGRAM_H_

#include "Base/GemBase.h"
#include "Utils/GLUtil.h"
#include <map>

#define MAX_NUM_SHADERS 32


/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  glsl_program

  Link in a GLSL program/shader

  KEYWORDS


  DESCRIPTION

  -----------------------------------------------------------------*/

class GEM_EXTERN glsl_program : public GemBase
{
  CPPEXTERN_HEADER(glsl_program, GemBase);

public:

  //////////
  // Constructor
  glsl_program(void);

protected:

  //////////
  // Destructor
  virtual ~glsl_program(void);


  //////////
  // check openGL-extensions
  virtual bool  isRunnable(void);
  virtual void  startRendering(void);
  virtual void  stopRendering(void);

  //////////
  // Do the rendering
  virtual void  renderGL2(void);
  virtual void  renderARB(void);
  virtual void  render(GemState *state);

  //////////
  // Clean up after rendering
  virtual void  postrender(GemState *state);

  //////////
  // parameters to the glsl-program
  virtual void  paramMess(t_symbol*s, int argc, const t_atom*argv);

  //////////
  // shader message
  virtual void  shaderMess(int argc, t_atom *argv);
  void  shaderMess(t_symbol*, int argc, t_atom *argv)
  {
    shaderMess(argc, argv);
  }
  virtual void  linkMess(t_symbol*, int, t_atom *);

  //////////
  // Do the linking
  virtual bool  LinkGL2(void);
  virtual bool  LinkARB(void);
  virtual void  LinkProgram(void);
  virtual void  UnlinkProgram(void);

  //////////
  // What can we play with?
  virtual void  getVariables(void);

  void createArrays(void);
  void destroyArrays(void);

  //////////
  // Print Info about Hardware limits
  virtual void printInfo(void);

  gem::ContextData<GLuint>m_program;
  GLuint                m_shaderObj[MAX_NUM_SHADERS];

  gem::ContextData<GLhandleARB>m_programARB;
  GLhandleARB           m_shaderObjARB[MAX_NUM_SHADERS];

  //////////
  // Variables for the, uh, variables
  struct t_uniform;
  std::map<std::string, t_uniform>m_uniforms;

  gem::ContextData<GLint>m_linked;
  int m_numShaders;

  t_outlet *m_outProgramID;

  gem::utils::gl::GLuintMap m_shadermapper, m_programmapper;
  float m_programmapped;


  ////////
  // for geometry shaders we need a bit more...
  virtual void intypeMess(GLuint intype);
  GLuint m_geoInType;
  virtual void outtypeMess(GLuint outtype);
  GLuint m_geoOutType;
  virtual void outverticesMess(GLint);
  GLint  m_geoOutVertices;

  std::map<std::string, std::vector<t_atom> >m_cachedParameters;

  virtual void keepUniformsMess(bool);
  bool m_keepUniforms; /* should we keep uniforms across reloading of shaders?)*/


private:

  //////////
  // static member functions
  static void paramMessCallback (void *data, t_symbol*, int, t_atom*);

  static void intypeMessCallback  (void *, t_symbol*, int, t_atom*);
  static void outtypeMessCallback  (void *, t_symbol*, int, t_atom*);
  static void typeMessCallback  (void *, t_symbol*, int, t_atom*);
};

#endif  // for header file
