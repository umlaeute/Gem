/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an GLSL vertex shader
 
 *  Created by tigital on 10/04/2005.
 *  Copyright 2005 James Tittle.
 
Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GLSL_VERTEX_H_
#define INCLUDE_GLSL_VERTEX_H_

#include "Manips/glsl_program.h"

#define GEM_shader_none  0
#define GEM_shader_GLSL  1
#define GEM_shader_CG    2

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  glsl_vertex
    
  Loads in a vertex shader
    
  KEYWORDS

  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN glsl_vertex : public GemBase
{
  CPPEXTERN_HEADER(glsl_vertex, GemBase)
    
    public:
  
  //////////
  // Constructor
  glsl_vertex(void);
  glsl_vertex(t_symbol *filename);

 protected:
    
  //////////
  // Destructor
  virtual ~glsl_vertex();

  //////////
  // close the shader file
  virtual void closeMess(void);
  //////////
  // open a shader up
  virtual void openMess(t_symbol *filename);

  //////////
  // Do the rendering
  virtual void render(GemState *state);

  //////////
  // Clear the dirty flag on the pixBlock
  virtual void postrender(GemState *state);

  //////////
  virtual void startRendering();

  //////////
  // Print Info about Hardware limits
  virtual void printInfo();

  //////////
  //
#if defined GL_VERSION_2_0 || defined GL_ARB_shader_objects
  GLuint		m_shaderType;

  GLuint		m_shaderTarget;
#ifdef GL_VERSION_2_0
  GLuint		m_shader;
#else
  GLhandleARB   m_shader;
#endif // GL_VERSION_2_0
  GLint			m_compiled;
  int			m_size;
  char*			m_shaderString;
  
  GLint			m_shaderID;
#endif
  t_outlet		*m_outShaderID;

 protected:
	
  //////////
  // static member functions
  static void openMessCallback   (void *data, t_symbol *filename);
  static void printMessCallback  (void *);
};

#endif	// for header file
