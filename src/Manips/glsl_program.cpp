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

CPPEXTERN_NEW(glsl_program)

/////////////////////////////////////////////////////////
//
// glsl_program
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
glsl_program :: glsl_program()
#ifdef GL_ARB_shader_objects
  :
m_program(0), 
m_maxLength(0), m_infoLength(0), m_uniformCount(0),
m_symname(NULL), m_size(NULL), m_type(NULL),
m_param(NULL), m_flag(NULL), m_linked(0), m_wantLink(false),
m_infoLog(NULL), m_num(0)
#endif
{
#if !defined GL_ARB_shader_objects
  post("GEM has been compiled without GLSL support");
  return;
#endif
  // create an outlet to send texture ID
  m_outProgramID = outlet_new(this->x_obj, &s_float);
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
glsl_program :: ~glsl_program()
{
#ifdef GL_ARB_shader_objects
  if (m_program)glDeleteObjectARB( m_program ); m_program=0;
  destroyArrays();
#endif
}

void glsl_program :: destroyArrays() {
#ifdef GL_ARB_shader_objects
  int i;
  //  if (m_program) glDeleteObjectARB( m_program ); m_program=0;

  if (m_param)
    {
      for (i = 0; i < m_uniformCount; i++){
        if(m_param[i])delete[]m_param[i];
        m_param[i]=NULL;
      }
    }

  if (m_size)   delete[]m_size;   m_size   =NULL;
  if (m_type)   delete[]m_type;   m_type   =NULL;
  if (m_symname)delete[]m_symname;m_symname=NULL;
  if (m_flag)   delete[]m_flag;   m_flag   =NULL;
  if (m_param)  delete[]m_param;  m_param  =NULL;
#endif
}
void glsl_program :: createArrays() {
#ifdef GL_ARB_shader_objects
  int i;

  m_size   = new GLint     [m_uniformCount];
  m_type   = new GLenum    [m_uniformCount];
  m_symname= new t_symbol* [m_uniformCount];
  m_param  = new float*    [m_uniformCount];
  m_flag   = new int       [m_uniformCount];

  // allocate maximum size for a param, which is a 4x4 matrix of floats
  // in the future, only allocate for specific type
  // also, technically we should handle arrays of matrices, too...sheesh!
  for (i = 0; i < m_uniformCount; i++) {
    int j=0;
    m_size   [i] = 0;
    m_type   [i] = 0;
    m_symname[i] = 0;
    m_param  [i] = new float[16];
    m_flag   [i] = 0;
    for(j=0; j<16; j++)m_param[i][j]=0;
  }
#endif
}
/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void glsl_program :: render(GemState *state)
{
#ifdef GL_ARB_shader_objects
  if(m_wantLink){
    m_wantLink=0;
    LinkProgram();
  }
  if (m_linked) {
    glUseProgramObjectARB( m_program );
    for(int i=0; i<m_uniformCount; i++)
      {
        if(m_flag[i])
	  {
	    switch (m_type[i])
              {
              case GL_INT:
                glUniform1iARB( i, (GLint)m_param[i][0] );
		break;
              case GL_FLOAT:
                glUniform1fARB( i, (GLfloat)m_param[i][0] );
		break;
              case GL_FLOAT_VEC2_ARB:
                glUniform2fARB( i, (GLfloat)m_param[i][0], (GLfloat)m_param[i][1] );
		break;
              case GL_FLOAT_VEC3_ARB:
                glUniform3fARB( i, (GLfloat)m_param[i][0], (GLfloat)m_param[i][1], 
                                (GLfloat)m_param[i][2] );
		break;
              case GL_FLOAT_VEC4_ARB:
                glUniform4fARB( i, (GLfloat)m_param[i][0], (GLfloat)m_param[i][1], 
                                (GLfloat)m_param[i][2], (GLfloat)m_param[i][3] );
		break;
              case GL_INT_VEC2_ARB:
                glUniform2iARB( i, (GLint)m_param[i][0], (GLint)m_param[i][1] );
		break;
              case GL_INT_VEC3_ARB:
                glUniform3iARB( i, (GLint)m_param[i][0], (GLint)m_param[i][1], 
                                (GLint)m_param[i][2] );
		break;
              case GL_INT_VEC4_ARB:
                glUniform4iARB( i, (GLint)m_param[i][0], (GLint)m_param[i][1], 
                                (GLint)m_param[i][2], (GLint)m_param[i][3] );
		break;
              case GL_FLOAT_MAT2_ARB:
                // GL_TRUE = row major order, GL_FALSE = column major
                glUniformMatrix2fvARB( i, 1, GL_FALSE, (GLfloat*)&m_param[i] );
		break;
              case GL_FLOAT_MAT3_ARB:
                glUniformMatrix3fvARB( i, 1, GL_FALSE, (GLfloat*)&m_param[i] );
		break;
              case GL_FLOAT_MAT4_ARB:
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
    /* JMZ: this is really annoying... */
    //post("GEM: [%s]:  no program linked", m_objectname->s_name);
  }
  // send program ID to outlet
  outlet_float(m_outProgramID, (t_float)(int)m_program);
#endif
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
#ifdef GL_ARB_shader_objects
  int i=0, j=0;
  if (m_program){
    for(i=0; i<m_uniformCount; i++){
      if(s==m_symname[i]){
        //      post("uniform parameters #%d", i);
        // don't know what to do with that...
        // sketch:
        //   copy the values into memory and add a flag that we have them for this parameter
        //   in the render cycle use it
        for (j=0; j < argc; j++)
	  {
	    m_param[i][j] = atom_getfloat(&argv[j]);
	  }
        // tell the GL state that this variable has changed next render
        m_flag[i] = 1;
        setModified();
        // should we return here?  It only allows one m_param[i] to be changed
        return;
      }
    }
    // if we reach this, then no param-name was matching!
    if(i>m_num)error("glsl_program: no method for '%s' (it's not uniform variable)", s->s_name);
  }
#endif
}

/////////////////////////////////////////////////////////
// shaderMess
//
/////////////////////////////////////////////////////////
void glsl_program :: shaderMess(int argc, t_atom *argv)
{
#ifdef GL_ARB_shader_objects
  int i;

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
  for (i = 0; i < argc; i++)
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
  int i;
  GLsizei length=0;
  if (!m_num)
    {
      error("GEM: [%s]: can't link zero shaders", m_objectname->s_name);
      return;
    }

  if(m_program) {
    glDeleteObjectARB( m_program );
    m_program = 0;
  }
  m_program = glCreateProgramObjectARB();
  for (i = 0; i < m_num; i++)
    {
      glAttachObjectARB( m_program, m_shaderObj[i] );
    }
  glLinkProgramARB( m_program );
  glGetObjectParameterivARB( m_program, GL_OBJECT_LINK_STATUS_ARB, &m_linked );

  glGetObjectParameterivARB( m_program, GL_OBJECT_INFO_LOG_LENGTH_ARB, &m_infoLength );
  if(m_infoLog)delete[]m_infoLog;m_infoLog=NULL;
  m_infoLog = new GLcharARB[m_infoLength];

  glGetInfoLogARB( m_program, m_infoLength, &length, m_infoLog );
  if (length)
    {
      post("GEM: [%s]: Info_log:", m_objectname->s_name);
      post("%s", m_infoLog);
    }
  //post("freeing log");
  if(m_infoLog)delete[]m_infoLog;m_infoLog=NULL;
  
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
# ifdef __APPLE__
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
# endif //__APPLE__
#endif
}

/////////////////////////////////////////////////////////
// getVariables
//
/////////////////////////////////////////////////////////
void glsl_program :: getVariables()
{
#ifdef GL_ARB_shader_objects
  if(!m_linked)return;
  int i;
  //
  // Allocate arrays to store the answers in. For simplicity, the return
  // from malloc is not checked for NULL.
  //
  destroyArrays();
  //
  // Get the number of uniforms, and the length of the longest name.
  //
  glGetObjectParameterivARB( m_program,
                             GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB,
                             &m_maxLength);
  glGetObjectParameterivARB( m_program, GL_OBJECT_ACTIVE_UNIFORMS_ARB,
                             &m_uniformCount);
  createArrays();
  
  //
  // Loop over glGetActiveUniformARB and store the results away.
  //
  GLcharARB *name=new GLcharARB[m_maxLength];
  GLsizei    length=0;
  for (i = 0; i < m_uniformCount; i++) 
    {
      /*
        glGetActiveUniformARB(m_program, i, m_maxLength, &length,
        &m_size[i], &m_type[i], name);
      */
      glGetActiveUniformARB(m_program, i, m_maxLength, &length, &m_size[i], &m_type[i], name);
      m_symname[i]=gensym(name);
      //	  post("[%s]: active uniform variable: %s", m_objectname->s_name, name);
    }
  delete[]name;
#endif
}

/////////////////////////////////////////////////////////
// printInfo
//
/////////////////////////////////////////////////////////
void glsl_program :: printInfo()
{
#ifdef GL_ARB_shader_objects
  int i;

  if(!m_linked) 
    {
      pd_error(x_obj, "no GLSL-program linked");
      return;
    }

  GLint bitnum = 0;
  post("glsl_Program Hardware Info");
  post("============================");
	
  post("");
  for (i=0; i<m_uniformCount; i++)
    { 
      startpost("[%s]: uvar#%d: \"%s\": ",  
                m_objectname->s_name, i, m_symname[i]->s_name);
      switch (m_type[i])
        {
        case 0x1404:
          post("GL_INT");
          break;
        case 0x1406:
          post("GL_FLOAT");
          break;
        case 0x8B50:
          post("GL_FLOAT_VEC2_ARB");
          break;
        case 0x8B51:
          post("GL_FLOAT_VEC3_ARB");
          break;
        case 0x8B52:
          post("GL_FLOAT_VEC4_ARB");
          break;
        case 0x8B53:
          post("GL_INT_VEC2_ARB");
          break;
        case 0x8B54:
          post("GL_INT_VEC3_ARB");
          break;
        case 0x8B55:
          post("GL_INT_VEC4_ARB");
          break;
        case 0x8B56:
          post("GL_BOOL_ARB");
          break;
        case 0x8B57:
          post("GL_BOOL_VEC2_ARB");
          break;
        case 0x8B58:
          post("GL_BOOL_VEC3_ARB");
          break;
        case 0x8B59:
          post("GL_BOOL_VEC4_ARB");
          break;
        case 0x8B5A:
          post("GL_FLOAT_MAT2_ARB");
          break;
        case 0x8B5B:
          post("GL_FLOAT_MAT3_ARB");
          break;
        case 0x8B5C:
          post("GL_FLOAT_MAT4_ARB");
          break;
        default:
          post("unknown (0x%X)", m_type[i]);
          break;
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
void glsl_program :: linkCallback(void *data, t_symbol*, int argc, t_atom*argv)
{
  if(argc)
    GetMyClass(data)->shaderMess(argc, argv);
#ifdef GL_ARB_shader_objects
  GetMyClass(data)->m_wantLink=1;
#endif
}
void glsl_program :: printMessCallback(void *data)
{
  GetMyClass(data)->printInfo();
}
void glsl_program :: paramMessCallback(void *data, t_symbol *s, int argc, t_atom *argv)
{
  GetMyClass(data)->paramMess(s, argc, argv);
}
