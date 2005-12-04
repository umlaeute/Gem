////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Created by tigital on 11/13/2005.
// Copyright 2005 James Tittle
//
// Implementation file
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "glsl_program.h"

#include <string.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <AGL/agl.h>
extern bool HaveValidContext (void);
#endif


CPPEXTERN_NEW(glsl_program)

/////////////////////////////////////////////////////////
//
// glsl_program
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
glsl_program :: glsl_program() :
  m_program(0), 
  m_maxLength(0), m_uniformCount(0),
  m_name(NULL), m_symname(NULL), m_length(NULL), m_size(NULL), m_type(NULL),
  m_param(NULL), m_flag(NULL), m_linked(0),
  m_infoLog(NULL), m_num(0)
{
#if !defined GL_ARB_shader_objects && !defined GL_ARB_shading_language_100
  post("GEM has been compiled without GLSL support");
  return;
#endif
  // create an outlet to send texture ID
  m_outProgramID = outlet_new(this->x_obj, &s_float);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
glsl_program :: ~glsl_program()
{
#ifdef GL_ARB_shader_objects
  if (m_program)
    glDeleteObjectARB( m_program );
  if (m_size) free(m_size);
  if (m_type) free(m_type);
  if (m_length) free(m_length);
  if (m_name) free(m_name);
  if (m_symname) free(m_symname);
  if (m_param)
  {
	for (int i = 0; i < m_uniformCount; i++)
	  free(m_param[i]);
	free(m_param);
  }
  if (m_flag) free(m_flag);
#endif
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void glsl_program :: render(GemState *state)
{
#ifdef GL_ARB_shader_objects
  if (m_linked) {
	glUseProgramObjectARB( m_program );
	for(int i=0; i<=m_uniformCount; i++)
	{
	  if(m_flag[i])
	  {
	    switch (m_type[i])
	    {
	  case 0x1404:  // GL_INT
	    glUniform1iARB( i, (GLint)m_param[i][0] );
		break;
	  case 0x1406:  // GL_FLOAT
	    glUniform1fARB( i, (GLfloat)m_param[i][0] );
		break;
	  case 0x8B50:  // GL_FLOAT_VEC2_ARB
	    glUniform2fARB( i, (GLfloat)m_param[i][0], (GLfloat)m_param[i][1] );
		break;
	  case 0x8B51:  // GL_FLOAT_VEC3_ARB
	    glUniform3fARB( i, (GLfloat)m_param[i][0], (GLfloat)m_param[i][1], 
							(GLfloat)m_param[i][2] );
		break;
	  case 0x8B52:  // GL_FLOAT_VEC4_ARB
	    glUniform4fARB( i, (GLfloat)m_param[i][0], (GLfloat)m_param[i][1], 
							(GLfloat)m_param[i][2], (GLfloat)m_param[i][3] );
		break;
	  case 0x8B53:  // GL_INT_VEC2_ARB
	    glUniform2iARB( i, (GLint)m_param[i][0], (GLint)m_param[i][1] );
		break;
	  case 0x8B54:  // GL_INT_VEC3_ARB
	    glUniform3iARB( i, (GLint)m_param[i][0], (GLint)m_param[i][1], 
							(GLint)m_param[i][2] );
		break;
	  case 0x8B55:  // GL_INT_VEC4_ARB
	    glUniform4iARB( i, (GLint)m_param[i][0], (GLint)m_param[i][1], 
							(GLint)m_param[i][2], (GLint)m_param[i][3] );
		break;
	  case 0x8B5A:  // GL_FLOAT_MAT2_ARB
					// GL_TRUE = row major order, GL_FALSE = column major
	    glUniformMatrix2fvARB( i, 1, GL_FALSE, (GLfloat*)&m_param[i] );
		break;
	  case 0x8B5B:  // GL_FLOAT_MAT3_ARB
	    glUniformMatrix3fvARB( i, 1, GL_FALSE, (GLfloat*)&m_param[i] );
		break;
	  case 0x8B5C:  // GL_FLOAT_MAT4_ARB
	    glUniformMatrix4fvARB( i, 1, GL_FALSE, (GLfloat*)&m_param[i] );
		break;
	  default:
		;
	    }
		// remove flag because the value is in GL's state now...
		m_flag[i]=0;
	  }
	}
  } else {
	    post("GEM: [%s]:  no program linked", m_objectname->s_name);
  }
#endif

  // send program ID to outlet
  outlet_float(m_outProgramID, (t_float)(int)m_program);
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void glsl_program :: postrender(GemState *state)
{
#ifdef GL_ARB_shader_objects
  glUseProgramObjectARB(0);
#endif
}
/////////////////////////////////////////////////////////
// shaderMess
//
/////////////////////////////////////////////////////////
void glsl_program :: paramMess(t_symbol*s,int argc, t_atom *argv)
{
  int i=0;
  for(i=0; i<=m_uniformCount; i++){
    if(s==m_symname[i]){
//      post("uniform parameters #%d", i);
      // don't know what to do with that...
      // sketch:
      //   copy the values into memory and add a flag that we have them for this parameter
      //   in the render cycle use it
	  for (int j=0; j < argc; j++)
	  {
	    m_param[i][j] = atom_getfloat(&argv[j]);
	  }
	  // tell the GL state that this variable has changed next render
	  m_flag[i] = 1;
      return;
    }
  }
  // if we reach this, then no param-name was matching!
  if(i>m_num)error("glsl_program: no method for '%s' (it's not uniform variable)", s->s_name);
}

/////////////////////////////////////////////////////////
// shaderMess
//
/////////////////////////////////////////////////////////
void glsl_program :: shaderMess(int argc, t_atom *argv)
{
#ifdef __APPLE__
	if (!HaveValidContext ()) {
		post("GEM: glsl_program - need window/context to load program");
		return;
	}
#endif
#ifdef GL_ARB_shader_objects
  if (!argc)
  {
    	error("GEM: [%s]: can't link non-existent shaders", m_objectname->s_name);
    	return;
  }

  if(argc>MAX_NUM_SHADERS)
    {
      argc=MAX_NUM_SHADERS;
      post("[%s]: only %d shaders supported; skipping the rest", MAX_NUM_SHADERS);
    }
  for (int i = 0; i < argc; i++)
  {
    m_shaderObj[i] = (t_GLshaderObj)(atom_getint(&argv[i]));
  }
  
//  not sure what to do here:  we don't want to link & re-link every render cycle,
//  but we do want to link when there are new shaders to link...so I made a seperate
//  link message
  m_num = argc;
#endif
}

/////////////////////////////////////////////////////////
// LinkProgram
//
/////////////////////////////////////////////////////////
void glsl_program :: LinkProgram()
{
#ifdef GL_ARB_shader_objects
  post("link %d progs", m_num);
  GLsizei length=0;
  if (!m_num)
  {
    	error("GEM: [%s]: can't link zero shaders", m_objectname->s_name);
    	return;
  }
  
  if (!m_program) m_program = glCreateProgramObjectARB();
  else
  {
    glDeleteObjectARB( m_program );
	m_program = glCreateProgramObjectARB();
  }
  
  for (int i = 0; i < m_num; i++)
  {
    post("attach object %d", i);
    glAttachObjectARB( m_program, m_shaderObj[i] );
  }
  glLinkProgramARB( m_program );
  glGetObjectParameterivARB( m_program, GL_OBJECT_LINK_STATUS_ARB, &m_linked );
  glGetObjectParameterivARB( m_program, GL_OBJECT_INFO_LOG_LENGTH_ARB, &m_maxLength );
  post("getting %d chars for infolog", m_maxLength);
  if(m_infoLog)free(m_infoLog);m_infoLog=NULL;
  m_infoLog = (GLcharARB *) malloc(m_maxLength * sizeof(GLcharARB));
  glGetInfoLogARB( m_program, m_maxLength, &length, m_infoLog );
  if (length)
  {
    post("GEM: [%s]: Info_log:", m_objectname->s_name);
    post("%s", m_infoLog);
  }
  //post("freeing log");
  if(m_infoLog)free(m_infoLog);m_infoLog=NULL;
  
  //
  // If all went well, make the ProgramObject part of the current state
  //
  //post("did we link?");
  if (m_linked) {
    glUseProgramObjectARB( m_program );
  } else {
    glUseProgramObjectARB( 0 );
    post("GEM: [%s]:  Link failed!", m_objectname->s_name);
    return;
  }
  //post("getting variables");
  getVariables();
#ifdef __APPLE__
  // call API to check if linked program is running on hardware or in software emulation
  long int vertexGPUProcessing, fragmentGPUProcessing;
  CGLGetParameter (CGLGetCurrentContext(), kCGLCPGPUVertexProcessing, &vertexGPUProcessing);
  CGLGetParameter (CGLGetCurrentContext(), kCGLCPGPUFragmentProcessing, &fragmentGPUProcessing);
  
  if (vertexGPUProcessing)
    post("[%s]: vertex shader running in hardware", m_objectname->s_name);
  else
    post("[%s]: vertex shader running in software", m_objectname->s_name);
  
  if (fragmentGPUProcessing)
    post("[%s]: fragment shader running in hardware", m_objectname->s_name);
  else
    post("[%s]: fragment shader running in software", m_objectname->s_name);	
#endif //__APPLE__
#endif
}

/////////////////////////////////////////////////////////
// getVariables
//
/////////////////////////////////////////////////////////
void glsl_program :: getVariables()
{
#ifdef GL_ARB_shader_objects
  if(m_linked)
  {
	//
    // Get the number of uniforms, and the length of the longest name.
    //
    glGetObjectParameterivARB( m_program,
			      GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB,
			      &m_maxLength);
    glGetObjectParameterivARB( m_program, GL_OBJECT_ACTIVE_UNIFORMS_ARB,
			      &m_uniformCount); 

    //
    // Allocate arrays to store the answers in. For simplicity, the return
    // from malloc is not checked for NULL.
    //
    if (m_size) free(m_size);
    if (m_type) free(m_type);
    if (m_length) free(m_length);
    if (m_name) free(m_name);
    if (m_symname) free(m_symname);
	if (m_param)
	{
	  for (int i = 0; i < m_uniformCount; i++)
	    free(m_param[i]);
	  free(m_param);
	}
	if (m_flag) free(m_flag);

    m_size   = (GLint *) malloc(m_uniformCount * sizeof(GLint));
    m_type   = (GLenum *) malloc(m_uniformCount * sizeof(GLenum));
    m_length = (GLsizei *) malloc(m_uniformCount * sizeof(GLsizei));
    m_name   = (GLcharARB **) malloc(m_uniformCount * sizeof(GLcharARB *));
    m_symname= (t_symbol**) malloc(m_uniformCount * sizeof(t_symbol *));
	m_param  = (float**) malloc(m_uniformCount * sizeof(float*));
	m_flag   = (int *) malloc(m_uniformCount * sizeof(int*));

    //
    // Loop over glGetActiveUniformARB and store the results away.
    //
    for (int i = 0; i < m_uniformCount; i++) 
	{
	  m_name[i] = (GLcharARB *) malloc(m_maxLength * sizeof(GLcharARB));
	  glGetActiveUniformARB(m_program, i, m_maxLength, &m_length[i],
			      &m_size[i], &m_type[i], m_name[i]);
	  m_symname[i]=gensym(m_name[i]);
	  // allocate maximum size for a param, which is a 4x4 matrix of floats
	  // in the future, only allocate for specific type
	  // also, technically we should handle arrays of matrices, too...sheesh!
	  m_param[i] = (float*) malloc(15 * sizeof(float*));
	  m_flag[i] = 0;
//	  post("[%s]: active uniform variable: %s", m_objectname->s_name, m_name[i]);
    }
  }
#endif
}

/////////////////////////////////////////////////////////
// printInfo
//
/////////////////////////////////////////////////////////
void glsl_program :: printInfo()
{
#ifdef __APPLE__
	if (!HaveValidContext ()) {
		post("GEM: glsl_program - need window/context to load program");
		return;
	}
#endif
	GLint bitnum = 0;
	post("glsl_Program Hardware Info");
	post("============================");
	
	post("");
#ifdef GL_ARB_shader_objects
  if(m_linked)
  {
    for (int i=0; i<m_uniformCount; i++)
	{ 
	  switch (m_type[i])
	  {
	  case 0x1404:
	    post("[%s]: active uniform variable: %s", m_objectname->s_name, m_name[i]);
		post("[%s]:    index = %i, GLtype = GL_INT", m_objectname->s_name, i);
		break;
	  case 0x1406:
	    post("[%s]: active uniform variable: %s", m_objectname->s_name, m_name[i]);
		post("[%s]:    index = %i, GLtype = GL_FLOAT", m_objectname->s_name, i);
		break;
	  case 0x8B50:
	    post("[%s]: active uniform variable: %s", m_objectname->s_name, m_name[i]);
		post("[%s]:    index = %i, GLtype = GL_FLOAT_VEC2_ARB", m_objectname->s_name, i);
		break;
	  case 0x8B51:
	    post("[%s]: active uniform variable: %s", m_objectname->s_name, m_name[i]);
		post("[%s]:    index = %i, GLtype = GL_FLOAT_VEC3_ARB", m_objectname->s_name, i);
		break;
	  case 0x8B52:
	    post("[%s]: active uniform variable: %s", m_objectname->s_name, m_name[i]);
		post("[%s]:    index = %i, GLtype = GL_FLOAT_VEC4_ARB", m_objectname->s_name, i);
		break;
	  case 0x8B53:
	    post("[%s]: active uniform variable: %s", m_objectname->s_name, m_name[i]);
		post("[%s]:    index = %i, GLtype = GL_INT_VEC2_ARB", m_objectname->s_name, i);
		break;
	  case 0x8B54:
	    post("[%s]: active uniform variable: %s", m_objectname->s_name, m_name[i]);
		post("[%s]:    index = %i, GLtype = GL_INT_VEC3_ARB", m_objectname->s_name, i);
		break;
	  case 0x8B55:
	    post("[%s]: active uniform variable: %s", m_objectname->s_name, m_name[i]);
		post("[%s]:    index = %i, GLtype = GL_INT_VEC4_ARB", m_objectname->s_name, i);
		break;
	  case 0x8B56:
	    post("[%s]: active uniform variable: %s", m_objectname->s_name, m_name[i]);
		post("[%s]:    index = %i, GLtype = GL_BOOL_ARB", m_objectname->s_name, i);
		break;
	  case 0x8B57:
	    post("[%s]: active uniform variable: %s", m_objectname->s_name, m_name[i]);
		post("[%s]:    index = %i, GLtype = GL_BOOL_VEC2_ARB", m_objectname->s_name, i);
		break;
	  case 0x8B58:
	    post("[%s]: active uniform variable: %s", m_objectname->s_name, m_name[i]);
		post("[%s]:    index = %i, GLtype = GL_BOOL_VEC3_ARB", m_objectname->s_name, i);
		break;
	  case 0x8B59:
	    post("[%s]: active uniform variable: %s", m_objectname->s_name, m_name[i]);
		post("[%s]:    index = %i, GLtype = GL_BOOL_VEC4_ARB", m_objectname->s_name, i);
		break;
	  case 0x8B5A:
	    post("[%s]: active uniform variable: %s", m_objectname->s_name, m_name[i]);
		post("[%s]:    index = %i, GLtype = GL_FLOAT_MAT2_ARB", m_objectname->s_name, i);
		break;
	  case 0x8B5B:
	    post("[%s]: active uniform variable: %s", m_objectname->s_name, m_name[i]);
		post("[%s]:    index = %i, GLtype = GL_FLOAT_MAT3_ARB", m_objectname->s_name, i);
		break;
	  case 0x8B5C:
	    post("[%s]: active uniform variable: %s", m_objectname->s_name, m_name[i]);
		post("[%s]:    index = %i, GLtype = GL_FLOAT_MAT4_ARB", m_objectname->s_name, i);
		break;
	  default:
        post("[%s]: active uniform variable: %s", m_objectname->s_name, m_name[i]);
		post("[%s]:    index = %i, GLtype = %i", m_objectname->s_name, i, m_type[i]);
	}
    }
  }
#endif
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void glsl_program :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&glsl_program::shaderMessCallback,
		gensym("shader"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&glsl_program::linkCallback,
		gensym("link"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&glsl_program::printMessCallback,
		gensym("print"), A_NULL);
 class_addanything(classPtr, (t_method)&glsl_program::paramMessCallback);
}
void glsl_program :: shaderMessCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->shaderMess(argc, argv);
}
void glsl_program :: linkCallback(void *data)
{
    GetMyClass(data)->LinkProgram();
}
void glsl_program :: printMessCallback(void *data)
{
	GetMyClass(data)->printInfo();
}
void glsl_program :: paramMessCallback(void *data, t_symbol *s, int argc, t_atom *argv)
{
  GetMyClass(data)->paramMess(s, argc, argv);
}
