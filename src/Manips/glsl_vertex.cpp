////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Created by tigital on 10/04/2005.
// Copyright 2005 James Tittle
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "glsl_vertex.h"
#include "Gem/Manager.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef _WIN32
# include <io.h>
# define close _close
#else
# include <unistd.h>
#endif

CPPEXTERN_NEW_WITH_ONE_ARG(glsl_vertex, t_symbol *, A_DEFSYM);

/////////////////////////////////////////////////////////
//
// glsl_vertex
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
glsl_vertex :: glsl_vertex() :
  m_shaderString(""),
  m_shaderTarget(0),
  m_shader(0),
  m_shaderARB(0),
  m_outShaderID(0),
  m_idmapper("glsl.shader"),
  m_idmapped(0.)
{
  // create an outlet to send shader object ID
  m_outShaderID = outlet_new(this->x_obj, &s_float);
}
glsl_vertex :: glsl_vertex(t_symbol *filename) :
  m_shaderTarget(0),
  m_shader(0),
  m_shaderARB(0),
  m_idmapper("glsl.shader"),
  m_idmapped(0.)
{
  openMess(filename);

  // create an outlet to send shader object ID
  m_outShaderID = outlet_new(this->x_obj, &s_float);
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
glsl_vertex :: ~glsl_vertex()
{
  closeMess();
}

////////////////////////////////////////////////////////
// closeMess
//
/////////////////////////////////////////////////////////
void glsl_vertex :: closeMess(void)
{
  m_shaderString="";
  if(m_shader) {
    glDeleteShader( m_shader );
  }
  if(m_shaderARB) {
    glDeleteObjectARB( m_shaderARB );
  }

  m_idmapper.del(m_idmapped);
  m_idmapped=0.;

  m_shader=0;
  m_shaderARB = 0;
}

////////////////////////////////////////////////////////
// openMess
//
/////////////////////////////////////////////////////////
bool glsl_vertex :: openMessGL2(void)
{
  if (m_shader) {
    glDeleteShader( m_shader );
    m_idmapper.del(m_idmapped);
    m_idmapped=0.;
  }
  m_shader = glCreateShader(m_shaderTarget);

  if (!m_shader) {
    error("could not create GLSL shader object");
    return false;
  }
  const char * vs = m_shaderString.c_str();
  glShaderSource( m_shader, 1, &vs, NULL );
  glCompileShader( m_shader );
  GLint compiled = 0;
  glGetShaderiv( m_shader, GL_COMPILE_STATUS, &compiled );
  if (!compiled) {
    GLint       length;
    GLchar* log;
    glGetShaderiv( m_shader, GL_INFO_LOG_LENGTH, &length );
    log = (GLchar*)malloc( length * sizeof(GLchar) );
    glGetShaderInfoLog( m_shader, length, NULL, log );
    post("compile Info_log:");
    post("%s", log );
    error("shader not loaded");
    free(log);
    return false;
  }
  if(m_shader) {
    t_atom a;
    m_idmapped=m_idmapper.set(m_shader, m_idmapped);
    SETFLOAT(&a, m_idmapped);
    outlet_list(m_outShaderID, gensym("list"), 1, &a);
  }
  return true;
}

bool glsl_vertex :: openMessARB(void)
{
  if(m_shaderARB) {
    glDeleteObjectARB( m_shaderARB );
    m_idmapper.del(m_idmapped);
    m_idmapped=0.;
  }
  m_shaderARB = glCreateShaderObjectARB(m_shaderTarget);

  if (!m_shaderARB) {
    error("could not create ARB shader object");
    return false;
  }
  const char * vs = m_shaderString.c_str();
  glShaderSourceARB( m_shaderARB, 1, &vs, NULL );
  glCompileShaderARB( m_shaderARB );
  GLint compiled = 0;
  glGetObjectParameterivARB( m_shaderARB, GL_OBJECT_COMPILE_STATUS_ARB,
                             &compiled );
  if (!compiled) {
    GLint       length;
    GLcharARB* log;
    glGetObjectParameterivARB( m_shaderARB, GL_OBJECT_INFO_LOG_LENGTH_ARB,
                               &length );
    log = (GLcharARB*)malloc( length * sizeof(GLcharARB) );
    glGetInfoLogARB( m_shaderARB, length, NULL, log );
    post("compile Info_log:");
    post("%s", log );
    error("shader not loaded");
    free(log);
    return false;
  }
  if(m_shaderARB) {
    t_atom a;
    m_idmapped=m_idmapper.set(m_shaderARB, m_idmapped);
    SETFLOAT(&a, m_idmapped);
    outlet_list(m_outShaderID, gensym("list"), 1, &a);
  }

  return true;
}


void glsl_vertex :: openMess(t_symbol *filename)
{
  if(NULL==filename || NULL==filename->s_name) {
    return;
  }
  if(&s_==filename) {
    return;
  }

  std::string fn = findFile(filename->s_name);
  const char*fname=fn.c_str();

  FILE *file = fopen(fname,"rb");
  if(file) {
    fseek(file,0,SEEK_END);
    long size = ftell(file);
    if(size<0) {
      fclose(file);
      error("error reading filesize");
      return;
    }
    char*shaderString = new char[size + 1];
    memset(shaderString, 0, size + 1);
    fseek(file,0,SEEK_SET);
    size_t count=fread(shaderString,1,size,file);
    shaderString[size]='\0';

    int err=ferror(file);
    fclose(file);
    if(err) {
      error("error %d reading file (%d<%d)", err, count, size);
      delete[]shaderString;
      return;
    }
    m_shaderString = shaderString;
  } else {
    error("could not find shader-file: '%s'", fname);
    return;
  }
  verbose(1, "loaded shader file '%s'", fname);
  setModified();
}

void glsl_vertex :: loadShader()
{
  if(!isRunnable()) {
    return;
  }
  if(m_shaderString.empty()) {
    closeMess();
    return;
  }

  GLuint type = m_shaderType;
  switch(type) {
  case GL2:
    openMessGL2();
    break;
  case ARB:
    openMessARB();
    break;
  default:
    break;
  }
}

////////////////////////////////////////////////////////
// extension check
//
/////////////////////////////////////////////////////////
bool glsl_vertex :: isRunnable()
{
  if(GLEW_VERSION_2_0) {
    m_shaderTarget = GL_VERTEX_SHADER;
    m_shaderType = GL2;
    return true;
  } else if (GLEW_ARB_vertex_shader) {
    m_shaderTarget = GL_VERTEX_SHADER_ARB;
    m_shaderType = ARB;
    return true;
  }

  error("need OpenGL-2.0 (or at least the vertex-shader ARB-extension) to run GLSL");
  return false;
}

////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void glsl_vertex :: startRendering()
{
  loadShader();

  if (m_shaderString.empty()) {
    error("need to load a shader");
    return;
  }
}
void glsl_vertex :: stopRendering() {
  if(m_shader) {
    glDeleteShader( m_shader );
  }
  GLhandleARB shaderARB = m_shaderARB;
  if(shaderARB) {
    glDeleteObjectARB( shaderARB );
  }

  m_idmapper.del(m_idmapped);
  m_idmapped=0.;

  m_shader=0;
  m_shaderARB = 0;
}

////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void glsl_vertex :: render(GemState *state)
{
}

////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void glsl_vertex :: postrender(GemState *state)
{
}

////////////////////////////////////////////////////////
// printInfo
//
/////////////////////////////////////////////////////////
void glsl_vertex :: printInfo()
{
  if(getState()==INIT) {
    verbose(0, "not initialized yet with a valid context");
    return;
  }

  if(GLEW_VERSION_2_0 || GLEW_ARB_vertex_shader) {
    post("vertex shader - Hardware Info");
    post("=============================");
    if(GLEW_VERSION_2_0) {
      GLSL_GETPOSTINT( MAX_VERTEX_ATTRIBS );
      GLSL_GETPOSTINT( MAX_VERTEX_UNIFORM_COMPONENTS );
      GLSL_GETPOSTINT( MAX_VARYING_FLOATS );
      GLSL_GETPOSTINT( MAX_COMBINED_TEXTURE_IMAGE_UNITS );
      GLSL_GETPOSTINT( MAX_VERTEX_TEXTURE_IMAGE_UNITS );
      GLSL_GETPOSTINT( MAX_TEXTURE_IMAGE_UNITS );
      GLSL_GETPOSTINT( MAX_TEXTURE_COORDS );

      if(m_shader) {
        GLint shader = m_shader;
        post("compiled last shader to ID: %d", shader);
      }
    } else {
      GLSL_GETPOSTINT( MAX_VERTEX_ATTRIBS_ARB );
      GLSL_GETPOSTINT( MAX_VERTEX_UNIFORM_COMPONENTS_ARB );
      GLSL_GETPOSTINT( MAX_VARYING_FLOATS_ARB );
      GLSL_GETPOSTINT( MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB );
      GLSL_GETPOSTINT( MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB );
      GLSL_GETPOSTINT( MAX_TEXTURE_IMAGE_UNITS_ARB );
      GLSL_GETPOSTINT( MAX_TEXTURE_COORDS_ARB );
      if(m_shaderARB) {
        GLhandleARB shader = m_shaderARB;
        post("compiled last shaderARB to ID: %d", shader);
      }
    }
  } else {
    post("no GLSL support");
  }
}

////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void glsl_vertex :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG0(classPtr, "print", printInfo);
  CPPEXTERN_MSG1(classPtr, "open", openMess, t_symbol*);
}
