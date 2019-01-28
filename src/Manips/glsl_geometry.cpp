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

#include "glsl_geometry.h"

CPPEXTERN_NEW_WITH_ONE_ARG(glsl_geometry, t_symbol *, A_DEFSYM);

/////////////////////////////////////////////////////////
//
// glsl_geometry
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
glsl_geometry :: glsl_geometry(t_symbol *filename)
  : glsl_vertex()
{
  openMess(filename);
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
// extension check
//
/////////////////////////////////////////////////////////
bool glsl_geometry :: isRunnable()
{
  if(GLEW_EXT_geometry_shader4) { // GLEW_VERSION_2_1 ??
    m_shaderTarget = GL_GEOMETRY_SHADER_EXT;
    m_shaderType = GL2;
    return true;
  } else if (GLEW_ARB_geometry_shader4) {
    m_shaderTarget = GL_GEOMETRY_SHADER_ARB;
    m_shaderType = ARB;
    return true;
  }

  error("need OpenGL-2.1 (or at least the geometry-shader ARB-extension) to run GLSL");
  return false;
}

////////////////////////////////////////////////////////
// printInfo
//
/////////////////////////////////////////////////////////
void glsl_geometry :: printInfo()
{
  if(getState()==INIT) {
    verbose(0, "not initialized yet with a valid context");
    return;
  }
  if(GLEW_EXT_geometry_shader4 || GLEW_ARB_geometry_shader4) {
    GLint bitnum = 0;
    post("glsl_geometry Hardware Info");
    post("===========================");
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
      if(m_shader) {
        GLint shader = m_shader;
        post("compiled last shader to ID: %d", shader);
      }
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
void glsl_geometry :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "open", openMess, t_symbol*);
  CPPEXTERN_MSG0(classPtr, "print", printInfo);
}
