////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Created by tigital on 11/13/2005.
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

#include "glsl_fragment.h"

#include <string.h>
#include <stdio.h>

CPPEXTERN_NEW_WITH_ONE_ARG(glsl_fragment, t_symbol *, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// glsl_fragment
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

glsl_fragment :: glsl_fragment(t_symbol *filename) :
  glsl_vertex()
{
  openMess(filename);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
glsl_fragment :: ~glsl_fragment()
{
  closeMess();
}

////////////////////////////////////////////////////////
// extension check
//
/////////////////////////////////////////////////////////
bool glsl_fragment :: isRunnable() {
  if(GLEW_VERSION_2_0) {
    m_shaderTarget = GL_FRAGMENT_SHADER;
    return true;
  } else if (GLEW_ARB_fragment_shader) {
    m_shaderTarget = GL_FRAGMENT_SHADER_ARB;
    return true;
  }

  error("need OpenGL-2.0 (or at least the fragment-shader ARB-extension) to run GLSL");
  return false;
}


/////////////////////////////////////////////////////////
// printInfo
//
/////////////////////////////////////////////////////////
void glsl_fragment :: printInfo()
{
  if(GLEW_VERSION_2_0 || GLEW_ARB_shader_objects) {
    GLint bitnum = 0;
    post("glsl_fragment Hardware Info");
    post("============================");
    if(GLEW_VERSION_2_0) {
      glGetIntegerv( GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &bitnum );
      post("MAX_FRAGMENT_UNIFORM_COMPONENTS: %d", bitnum);
      glGetIntegerv( GL_MAX_TEXTURE_COORDS, &bitnum );
      post("MAX_TEXTURE_COORDS: %d", bitnum);
      glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &bitnum );
      post("MAX_TEXTURE_IMAGE_UNITS: %d", bitnum);
    } else {
      glGetIntegerv( GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB, &bitnum );
      post("MAX_FRAGMENT_UNIFORM_COMPONENTS: %d", bitnum);
      glGetIntegerv( GL_MAX_TEXTURE_COORDS_ARB, &bitnum );
      post("MAX_TEXTURE_COORDS: %d", bitnum);
      glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &bitnum );
      post("MAX_TEXTURE_IMAGE_UNITS: %d", bitnum);
    }
  } else post("no GLSL support");
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void glsl_fragment :: obj_setupCallback(t_class *classPtr)
{
}

