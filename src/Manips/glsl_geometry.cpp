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
//    Copyright (c) G¸nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f¸r::uml‰ute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "glsl_geometry.h"
#include "Base/GemMan.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef __WIN32__
# include <io.h>
#else
# include <unistd.h>
#endif

CPPEXTERN_NEW_WITH_ONE_ARG(glsl_geometry, t_symbol *, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// glsl_geometry
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
glsl_geometry :: glsl_geometry() :
  m_shaderTarget(0),
  m_shader(0),
  m_shaderARB(0),
  m_compiled(0), m_size(0),
  m_shaderString(NULL), 
  m_shaderFilename(NULL),
  m_shaderID(0)
{
  // create an outlet to send shader object ID
  m_outShaderID = outlet_new(this->x_obj, &s_float);
}
glsl_geometry :: glsl_geometry(t_symbol *filename) :
  m_shaderTarget(0),
  m_shader(0),
  m_shaderARB(0),
  m_compiled(0), m_size(0), 
  m_shaderString(NULL), 
  m_shaderFilename(NULL),
  m_shaderID(0)
{
  openMess(filename);

  // create an outlet to send shader object ID
  m_outShaderID = outlet_new(this->x_obj, &s_float);
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
glsl_geometry :: ~glsl_geometry()
{
  closeMess();
}

////////////////////////////////////////////////////////
// closeMess
//
/////////////////////////////////////////////////////////
void glsl_geometry :: closeMess(void)
{
  if(m_shaderString)delete [] m_shaderString;
  m_shaderString=NULL;
  m_size=0;
  if(m_shader)
    glDeleteShader( m_shader );
  if(m_shaderARB)
    glDeleteObjectARB( m_shaderARB );

  m_shader=0;
  m_shaderARB = 0;

  m_compiled=0;
}

////////////////////////////////////////////////////////
// openMess
//
/////////////////////////////////////////////////////////
bool glsl_geometry :: openMessGL2(void)
{
  if (m_shader) {
    glDeleteShader( m_shader );
  }
  m_shader = glCreateShader(m_shaderTarget);

  if (!m_shader)
    {
      error("could not create GLSL shader object");
      return false;
    }
  const char * vs = m_shaderString;
  glShaderSource( m_shader, 1, &vs, NULL );
  glCompileShader( m_shader );
  glGetShaderiv( m_shader, GL_COMPILE_STATUS, &m_compiled );
  if (!m_compiled) {
    GLint	length;
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
  return true;
}

bool glsl_geometry :: openMessARB(void)
{
  if(m_shaderARB)
    glDeleteObjectARB( m_shaderARB );
  m_shaderARB = glCreateShaderObjectARB(m_shaderTarget);

  if (!m_shaderARB)
    {
      error("could not create ARB shader object");
      return false;
    }
  const char * vs = m_shaderString;
  glShaderSourceARB( m_shaderARB, 1, &vs, NULL );
  glCompileShaderARB( m_shaderARB );
  glGetObjectParameterivARB( m_shaderARB, GL_OBJECT_COMPILE_STATUS_ARB, &m_compiled );
  if (!m_compiled) {
    GLint	length;
    GLcharARB* log;
    glGetObjectParameterivARB( m_shaderARB, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length );
    log = (GLcharARB*)malloc( length * sizeof(GLcharARB) );
    glGetInfoLogARB( m_shaderARB, length, NULL, log );
    post("compile Info_log:");
    post("%s", log );
    error("shader not loaded");
    free(log);
    return false;
  }

  return true;
}

void glsl_geometry :: openMess(t_symbol *filename)
{
  if(NULL==filename || NULL==filename->s_name)return;
  if(&s_==filename)return;

  if( !GemMan::windowExists() ) {
    post("cannot load shader now! deferring till later...");
    m_shaderFilename=filename;
    return;
  }

  if(!GLEW_VERSION_2_1 && !GLEW_ARB_geometry_shader4) {
    post("cannot load shader now!");
    return;
  }

  isRunnable();

  char buf[MAXPDSTRING];
  char buf2[MAXPDSTRING];
  char *bufptr=NULL;

  // Clean up any open files
  closeMess();

  int fd=-1;
  if ((fd=open_via_path(canvas_getdir(getCanvas())->s_name, filename->s_name, "", 
                        buf2, &bufptr, MAXPDSTRING, 1))>=0){
    close(fd);
    sprintf(buf, "%s/%s", buf2, bufptr);
  } else
    canvas_makefilename(getCanvas(), filename->s_name, buf, MAXPDSTRING);

  FILE *file = fopen(buf,"r");
  if(file) {
    fseek(file,0,SEEK_END);
    int size = ftell(file);
    m_shaderString = new char[size + 1];
    memset(m_shaderString,0,size + 1);
    fseek(file,0,SEEK_SET);
    fread(m_shaderString,1,size,file);
    fclose(file);
  } else {
    error("could not find shader-file: '%s'", buf);
    return;
    /*
      // assuming that the "filename" is actually a shader-program per se
      m_shaderString = new char[strlen(buf) + 1];
      strcpy(m_shaderString,buf);
    */
  }
  m_size=strlen(m_shaderString);

  if(GLEW_EXT_geometry_shader4)
    openMessGL2();
  else if (GLEW_ARB_geometry_shader4) 
    openMessARB();

  verbose(1, "Loaded file: %s", buf);
  m_shaderFilename=NULL;
}

////////////////////////////////////////////////////////
// extension check
//
/////////////////////////////////////////////////////////
bool glsl_geometry :: isRunnable() {
  if(GLEW_EXT_geometry_shader4) {
    m_shaderTarget = GL_GEOMETRY_SHADER_EXT;
    return true;
  } else if (GLEW_ARB_geometry_shader4) {
    m_shaderTarget = GL_GEOMETRY_SHADER_ARB;
    return true;
  }

  error("need OpenGL-2.0 (or at least the geometry-shader ARB-extension) to run GLSL");
  return false;
}

////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void glsl_geometry :: startRendering()
{
  if(NULL!=m_shaderFilename)
    openMess(m_shaderFilename);

  if (m_shaderString == NULL)
    {
      error("need to load a shader");
      return;
    }
}

////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void glsl_geometry :: render(GemState *state)
{
  if (m_shader || m_shaderARB)
    {
      t_floatuint fi;   
      // send textureID to outlet
      if(m_shader)
        fi.i=m_shader;
      else
        fi.i=m_shaderARB;

      outlet_float(m_outShaderID, fi.f);
    }
}

////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void glsl_geometry :: postrender(GemState *state)
{
}

////////////////////////////////////////////////////////
// printInfo
//
/////////////////////////////////////////////////////////
void glsl_geometry :: printInfo()
{
  if(GLEW_EXT_geometry_shader4 || GLEW_ARB_geometry_shader4) {
    GLint bitnum = 0;
    post("geometry_shader Hardware Info");
    post("============================");
    if(GLEW_EXT_geometry_shader4) {
      glGetIntegerv( GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_EXT, &bitnum );
      post("MAX_GEOMETRY_UNIFORM_COMPONENTS_EXT: %d", bitnum);
      glGetIntegerv( GL_MAX_VARYING_FLOATS, &bitnum );
      post("MAX_VARYING_FLOATS: %d", bitnum);
      glGetIntegerv( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &bitnum );
      post("MAX_COMBINED_TEXTURE_IMAGE_UNITS: %d", bitnum);
      glGetIntegerv( GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_EXT, &bitnum );
      post("MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_EXT: %d", bitnum);
      glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &bitnum );
      post("MAX_TEXTURE_IMAGE_UNITS: %d", bitnum);
      glGetIntegerv( GL_MAX_TEXTURE_COORDS, &bitnum );
      post("MAX_TEXTURE_COORDS: %d", bitnum);
    } else {
      glGetIntegerv( GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_ARB, &bitnum );
      post("MAX_GEOMETRY_UNIFORM_COMPONENTS_ARB: %d", bitnum);
      glGetIntegerv( GL_MAX_VARYING_FLOATS_ARB, &bitnum );
      post("MAX_VARYING_FLOATS: %d", bitnum);
      glGetIntegerv( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB, &bitnum );
      post("MAX_COMBINED_TEXTURE_IMAGE_UNITS: %d", bitnum);
      glGetIntegerv( GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_ARB, &bitnum );
      post("MAX_GEOMETRY_TEXTURE_IMAGE_UNITS: %d", bitnum);
      glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &bitnum );
      post("MAX_TEXTURE_IMAGE_UNITS: %d", bitnum);
      glGetIntegerv( GL_MAX_TEXTURE_COORDS_ARB, &bitnum );
      post("MAX_TEXTURE_COORDS: %d", bitnum);
    }
  } else post("no GLSL support");
}

////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void glsl_geometry :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&glsl_geometry::openMessCallback,
                  gensym("open"), A_SYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&glsl_geometry::printMessCallback,
                  gensym("print"), A_NULL);
}
void glsl_geometry :: openMessCallback(void *data, t_symbol *filename)
{
  GetMyClass(data)->openMess(filename);
}
void glsl_geometry :: printMessCallback(void *data)
{
  GetMyClass(data)->printInfo();
}
