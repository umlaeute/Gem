////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 2019, IOhannes m zmölnig and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
/////////////////////////////////////////////////////////

#include "glsl_tesseval.h"

CPPEXTERN_NEW_WITH_ONE_ARG(glsl_tesseval, t_symbol*, A_DEFSYMBOL);

/////////////////////////////////////////////////////////
//
// glsl_tesseval
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
glsl_tesseval :: glsl_tesseval(t_symbol* filename)
  : glsl_vertex()
{
  openMess(filename);
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
glsl_tesseval :: ~glsl_tesseval()
{
  closeMess();
}

////////////////////////////////////////////////////////
// extension check
//
/////////////////////////////////////////////////////////
bool glsl_tesseval :: isRunnable()
{
  if(GLEW_VERSION_4_0) {
    m_shaderTarget = GL_TESS_EVALUATION_SHADER;
    m_shaderType = GL2;
    return true;
  }

  error("need OpenGL-4.0 to run GLSL tessellation");
  return false;
}

////////////////////////////////////////////////////////
// printInfo
//
/////////////////////////////////////////////////////////
void glsl_tesseval :: printInfo()
{
  if(getState()==INIT) {
    verbose(0, "not initialized yet with a valid context");
    return;
  }
  if(GLEW_VERSION_4_0) {
    post("tessellation evaluation shader - Hardware Info");
    post("=============================================");

    GLSL_GETPOSTINT( MAX_TESS_GEN_LEVEL );
    GLSL_GETPOSTINT( MAX_PATCH_VERTICES );
    GLSL_GETPOSTINT( MAX_TESS_PATCH_COMPONENTS );

    GLSL_GETPOSTINT( MAX_TESS_EVALUATION_UNIFORM_COMPONENTS );
    GLSL_GETPOSTINT( MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS );
    GLSL_GETPOSTINT( MAX_TESS_EVALUATION_OUTPUT_COMPONENTS );
    GLSL_GETPOSTINT( MAX_TESS_EVALUATION_UNIFORM_BLOCKS );
    GLSL_GETPOSTINT( MAX_TESS_EVALUATION_INPUT_COMPONENTS );
    GLSL_GETPOSTINT( MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS );

    if(m_shader) {
      GLint shader = m_shader;
      post("compiled last shader to ID: %d", shader);
    }
  } else {
    post("no GLSL support");
  }
}

////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void glsl_tesseval :: obj_setupCallback(t_class *classPtr)
{
}
