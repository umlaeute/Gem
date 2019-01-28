/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Link a GLSL program/shader

 *  Created by tigital on 11/13/2005.
 *  Copyright 2005 tigital.

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_MANIPS_GLSL_PROGRAM_H_
#define _INCLUDE__GEM_MANIPS_GLSL_PROGRAM_H_

#include "Base/GemBase.h"
#include "Utils/GLUtil.h"

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
  virtual void  paramMess(t_symbol*s, int argc, t_atom*argv);

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

  gem::ContextData<GLint>m_maxLength;

  //////////
  // Variables for the, uh, variables
  GLint m_uniformCount;
  struct t_uniform {
    t_symbol*name;
    GLint size;
    GLenum type;
    GLint loc;
    union {
      GLfloat*f;
      GLint*i;
    } param;
    GLint paramsize; /* how many elements does single parameter hold (e.g. vec2 => 2) */
    GLint arraysize; /* array size (or 1) */
    bool changed;
  };
  t_uniform*m_uniform;

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


private:

  //////////
  // static member functions
  static void paramMessCallback (void *data, t_symbol *, int, t_atom*);

  static void intypeMessCallback  (void *, t_symbol*, int, t_atom*);
  static void outtypeMessCallback  (void *, t_symbol*, int, t_atom*);
  static void typeMessCallback  (void *, t_symbol*, int, t_atom*);
};

#endif  // for header file
