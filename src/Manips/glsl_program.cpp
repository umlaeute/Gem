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

using namespace gem::utils::gl;

CPPEXTERN_NEW(glsl_program);


namespace
{
  std::string removeArrayBrackets(const std::string name) {
    ssize_t namesize = name.size()-3;
    if(namesize>0 && 0 == name.compare(namesize, 3, "[0]")) {
      return name.substr(0, namesize);
    }
    return name;
  }
GLenum uniform2type(CPPExtern*obj, GLenum type)
{
  /* the base type for a (complex) uniform type;
     determines whether we use glUniform1f or glUniform1i
  */
  switch(type) {
  default:
    break;
  case GL_FLOAT:
  case GL_FLOAT_VEC2:
  case GL_FLOAT_VEC3:
  case GL_FLOAT_VEC4:
  case GL_FLOAT_MAT2:
  case GL_FLOAT_MAT3:
  case GL_FLOAT_MAT4:
    return GL_FLOAT;
  case GL_INT:
  case GL_INT_VEC2:
  case GL_INT_VEC3:
  case GL_INT_VEC4:
    return GL_INT;
  case GL_BOOL:
  case GL_BOOL_VEC2:
  case GL_BOOL_VEC3:
  case GL_BOOL_VEC4:
    return GL_INT;
  case GL_SAMPLER_1D:
  case GL_SAMPLER_2D:
  case GL_SAMPLER_3D:
  case GL_SAMPLER_CUBE:
  case GL_SAMPLER_1D_SHADOW:
  case GL_SAMPLER_2D_SHADOW:
  case GL_SAMPLER_2D_RECT_ARB:
    return GL_INT;
  }
  /* ARB types */
  switch(type) {
  default:
    break;
  case GL_FLOAT:
  case GL_FLOAT_VEC2_ARB:
  case GL_FLOAT_VEC3_ARB:
  case GL_FLOAT_VEC4_ARB:
    return GL_FLOAT;
  case GL_INT:
  case GL_INT_VEC2_ARB:
  case GL_INT_VEC3_ARB:
  case GL_INT_VEC4_ARB:
    return GL_INT;
  case GL_BOOL_ARB:
  case GL_BOOL_VEC2_ARB:
  case GL_BOOL_VEC3_ARB:
  case GL_BOOL_VEC4_ARB:
    return GL_INT;
  case GL_FLOAT_MAT2_ARB:
  case GL_FLOAT_MAT3_ARB:
  case GL_FLOAT_MAT4_ARB:
    return GL_FLOAT;
  case GL_SAMPLER_1D_ARB:
  case GL_SAMPLER_2D_ARB:
  case GL_SAMPLER_3D_ARB:
  case GL_SAMPLER_CUBE_ARB:
  case GL_SAMPLER_1D_SHADOW_ARB:
  case GL_SAMPLER_2D_SHADOW_ARB:
  case GL_SAMPLER_2D_RECT_ARB:
    return GL_INT;
  }
  obj->error("unknown uniform type %d, assuming float", type);
  return GL_FLOAT;
}
size_t uniform2numelements(CPPExtern*obj, GLenum type)
{
  /* the base number of elements for a (complex) uniform type;
  */
  switch(type) {
  default:
    break;
  case GL_FLOAT:
  case GL_INT:
  case GL_BOOL:
    return 1;
  case GL_FLOAT_VEC2:
  case GL_INT_VEC2:
  case GL_BOOL_VEC2:
    return 2;
  case GL_FLOAT_VEC3:
  case GL_INT_VEC3:
  case GL_BOOL_VEC3:
    return 3;
  case GL_FLOAT_VEC4:
  case GL_INT_VEC4:
  case GL_BOOL_VEC4:
    return 4;
  case GL_FLOAT_MAT2:
    return 4;
  case GL_FLOAT_MAT3:
    return 9;
  case GL_FLOAT_MAT4:
    return 16;
  case GL_SAMPLER_1D:
  case GL_SAMPLER_2D:
  case GL_SAMPLER_3D:
  case GL_SAMPLER_CUBE:
  case GL_SAMPLER_1D_SHADOW:
  case GL_SAMPLER_2D_SHADOW:
  case GL_SAMPLER_2D_RECT_ARB:
    return 1;
  }
  /* ARB */
  switch(type) {
  default:
    break;
  case GL_FLOAT:
  case GL_INT:
  case GL_BOOL_ARB:
    return 1;
  case GL_FLOAT_VEC2_ARB:
  case GL_INT_VEC2_ARB:
  case GL_BOOL_VEC2_ARB:
    return 2;
  case GL_FLOAT_VEC3_ARB:
  case GL_INT_VEC3_ARB:
  case GL_BOOL_VEC3_ARB:
    return 3;
  case GL_FLOAT_VEC4_ARB:
  case GL_INT_VEC4_ARB:
  case GL_BOOL_VEC4_ARB:
    return 4;
  case GL_FLOAT_MAT2_ARB:
    return 4;
  case GL_FLOAT_MAT3_ARB:
    return 9;
  case GL_FLOAT_MAT4_ARB:
    return 16;
  case GL_SAMPLER_1D_ARB:
  case GL_SAMPLER_2D_ARB:
  case GL_SAMPLER_3D_ARB:
  case GL_SAMPLER_CUBE_ARB:
  case GL_SAMPLER_1D_SHADOW_ARB:
  case GL_SAMPLER_2D_SHADOW_ARB:
  case GL_SAMPLER_2D_RECT_ARB:
    return 1;
  }

  obj->error("unknown base size for uniform type %d, assuming 1", type);
  return 1;
}
};

/////////////////////////////////////////////////////////
//
// glsl_program::t_uniform
//
/////////////////////////////////////////////////////////

struct glsl_program::t_uniform {
  //GLint size;
  GLint loc;
  GLenum type;
  struct {
    std::vector<GLfloat>f;
    std::vector<GLint>i;
  } param; /* the actual member depends on 'type' */
  GLenum paramtype; /* GL_FLOAT or GL_INT */
  size_t paramsize; /* how many elements does single parameter hold (e.g. vec2 => 2); calculated from 'type' */
  GLint arraysize; /* array size (or 1) */
  bool changed;

  t_uniform(void)
    : paramsize(1)
  {  }
  t_uniform(glsl_program*parent, GLint _loc, GLenum _type, GLint _arraysize)
    : loc(_loc)
    , type(_type)
    , arraysize(_arraysize)
    , changed(false)
  {
    paramsize = uniform2numelements(parent, type);
    paramtype = uniform2type(parent, type);
    param.f = std::vector<GLfloat>(arraysize * paramsize);
    param.i = std::vector<GLint>(arraysize * paramsize);
  }

  void applyGL2(void) {
    if(!changed)return;
    // remove flag because the value is going to be in the GL's state soon...
    changed = false;
    GLfloat*floatarray = param.f.data();
    GLint*intarray = param.i.data();
    switch (type) {
      /* float vectors */
    case GL_FLOAT:
      glUniform1fv( loc, arraysize, floatarray );
      break;
    case GL_FLOAT_VEC2:
      glUniform2fv( loc, arraysize, floatarray );
      break;
    case GL_FLOAT_VEC3:
      glUniform3fv( loc, arraysize, floatarray );
      break;
    case GL_FLOAT_VEC4:
      glUniform4fv( loc, arraysize, floatarray );
      break;

      /* int vectors */
    case GL_INT:
      glUniform1iv( loc, arraysize, intarray );
      break;
    case GL_INT_VEC2:
      glUniform2iv( loc, arraysize, intarray );
      break;
    case GL_INT_VEC3:
      glUniform3iv( loc, arraysize, intarray );
      break;
    case GL_INT_VEC4:
      glUniform4iv( loc, arraysize, intarray );
      break;

      /* bool vectors */
    case GL_BOOL:
      glUniform1iv( loc, arraysize, intarray );
      break;
    case GL_BOOL_VEC2:
      glUniform2iv( loc, arraysize, intarray );
      break;
    case GL_BOOL_VEC3:
      glUniform3iv( loc, arraysize, intarray );
      break;
    case GL_BOOL_VEC4:
      glUniform4iv( loc, arraysize, intarray );
      break;

      /* float matrices */
    case GL_FLOAT_MAT2:
      // GL_TRUE = row major order, GL_FALSE = column major
      glUniformMatrix2fv( loc, arraysize, GL_FALSE, floatarray );
      break;
    case GL_FLOAT_MAT3:
      glUniformMatrix3fv( loc, arraysize, GL_FALSE, floatarray );
      break;
    case GL_FLOAT_MAT4:
      glUniformMatrix4fv( loc, arraysize, GL_FALSE, floatarray );
      break;

      /* textures */
    case GL_SAMPLER_1D:
    case GL_SAMPLER_2D:
    case GL_SAMPLER_3D:
    case GL_SAMPLER_CUBE:
    case GL_SAMPLER_1D_SHADOW:
    case GL_SAMPLER_2D_SHADOW:
    case GL_SAMPLER_2D_RECT_ARB:
      glUniform1iv(loc, arraysize, intarray);
      break;

    default:
      break;
    }
  }

  void applyARB(void) {
    if(!changed)
      return;
    // remove flag because the value is going to be in the GL's state soon...
    changed = false;

    GLfloat*floatarray = param.f.data();
    GLint*intarray = param.i.data();
    switch (type) {
      /* float vectors */
    case GL_FLOAT:
      glUniform1fARB( loc, floatarray[0] );
      break;
    case GL_FLOAT_VEC2_ARB:
      glUniform2fARB( loc, floatarray[0], floatarray[1] );
      break;
    case GL_FLOAT_VEC3_ARB:
      glUniform3fARB( loc, floatarray[0], floatarray[1], floatarray[2] );
      break;
    case GL_FLOAT_VEC4_ARB:
      glUniform4fARB( loc, floatarray[0], floatarray[1], floatarray[2], floatarray[3] );
      break;

      /* int vectors */
    case GL_INT:
      glUniform1iARB( loc, intarray[0] );
      break;
    case GL_INT_VEC2_ARB:
      glUniform2iARB( loc, intarray[0], intarray[1] );
      break;
    case GL_INT_VEC3_ARB:
      glUniform3iARB( loc, intarray[0], intarray[1], intarray[2] );
      break;
    case GL_INT_VEC4_ARB:
      glUniform4iARB( loc, intarray[0], intarray[1], intarray[2], intarray[3] );
      break;

      /* bool vectors */
    case GL_BOOL_ARB:
      glUniform1fARB( loc, floatarray[0] );
      break;
    case GL_BOOL_VEC2_ARB:
      glUniform2fARB( loc, floatarray[0], floatarray[1] );
      break;
    case GL_BOOL_VEC3_ARB:
      glUniform3fARB( loc, floatarray[0], floatarray[1], floatarray[2] );
      break;
    case GL_BOOL_VEC4_ARB:
      glUniform4fARB( loc, floatarray[0], floatarray[1], floatarray[2], floatarray[3] );
      break;

      /* float matrices */
    case GL_FLOAT_MAT2_ARB:
      // GL_TRUE = row major order, GL_FALSE = column major
      glUniformMatrix2fvARB( loc, 1, GL_FALSE, floatarray );
      break;
    case GL_FLOAT_MAT3_ARB:
      glUniformMatrix3fvARB( loc, 1, GL_FALSE, floatarray );
      break;
    case GL_FLOAT_MAT4_ARB:
      glUniformMatrix4fvARB( loc, 1, GL_FALSE, floatarray );
      break;

      /* textures */
    case GL_SAMPLER_1D_ARB:
    case GL_SAMPLER_2D_ARB:
    case GL_SAMPLER_3D_ARB:
    case GL_SAMPLER_CUBE_ARB:
    case GL_SAMPLER_1D_SHADOW_ARB:
    case GL_SAMPLER_2D_SHADOW_ARB:
    case GL_SAMPLER_2D_RECT_ARB:
      glUniform1iARB(loc, floatarray[0]);
      break;
    default:
      break;
    }
  }
};


/////////////////////////////////////////////////////////
//
// glsl_program
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
glsl_program :: glsl_program()
  : m_program(0)
  , m_programARB(0)
  , m_linked(0)
  , m_numShaders(0)
  , m_outProgramID(0)
  , m_shadermapper("glsl.shader"), m_programmapper("glsl.program")
  , m_programmapped(0.)
  , m_geoInType(GL_TRIANGLES), m_geoOutType(GL_TRIANGLE_STRIP)
  , m_geoOutVertices(-1)
  , m_keepUniforms(true)
{
  int i=0;
  for(i=0; i<MAX_NUM_SHADERS; i++) {
    m_shaderObj[i]=0;
    m_shaderObjARB[i]=0;
  }

  // create an outlet to send texture ID
  m_outProgramID = outlet_new(this->x_obj, &s_float);
}


////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
glsl_program :: ~glsl_program()
{
  m_programmapper.del(m_programmapped);
  m_programmapped=0.;

  if(m_program) {
    glDeleteProgram( m_program );
  }
  m_program=0;
  if(m_programARB) {
    glDeleteObjectARB( m_programARB );
  }
  m_programARB=0;
}


bool glsl_program :: isRunnable()
{
  if (GLEW_VERSION_2_0 || GLEW_ARB_shader_objects) {
    return true;
  }

  error("openGL-2.0 (or at least ARB shader extensions) needed for GLSL");

  return false;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void glsl_program :: renderGL2()
{
  if(!m_linked)
    return;

  glUseProgram( m_program );
  for(std::map<std::string, t_uniform>::iterator it = m_uniforms.begin(); it != m_uniforms.end(); it++) {
    it->second.applyGL2();
  }
}

void glsl_program :: renderARB()
{
  if(!m_linked)
    return;

  glUseProgramObjectARB( m_programARB );
  for(std::map<std::string, t_uniform>::iterator it = m_uniforms.begin(); it != m_uniforms.end(); it++) {
    it->second.applyARB();
  }
}

void glsl_program :: startRendering()
{
  LinkProgram();
}


void glsl_program :: render(GemState *state)
{
  if(GLEW_VERSION_2_0) {
    renderGL2();
  } else {
    renderARB();
  }
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void glsl_program :: postrender(GemState *state)
{
  if(m_linked) {
    if(GLEW_VERSION_2_0) {
      glUseProgram(0);
    } else {
      glUseProgramObjectARB(0);
    }
  }
}
/////////////////////////////////////////////////////////
// paramMess
//
/////////////////////////////////////////////////////////
void glsl_program :: paramMess(t_symbol*s,int argc, const t_atom *argv)
{
  std::string name = removeArrayBrackets(std::string(s->s_name));
  if (!(m_program || m_programARB)) {
    /* cache the message */
    std::vector<t_atom>vec;
    for(int i=0; i<argc; i++) {
      vec.push_back(argv[i]);
    }
    m_cachedParameters[name] = vec;
  }
  if (!(m_program || m_programARB)) {
    return;
  }

  try {
    t_uniform&uni = m_uniforms.at(name);

    // don't know what to do with that...
    // sketch:
    //   copy the values into memory and add a flag that we have them for this parameter
    //   in the render cycle use it
    const int maxargc = uni.arraysize * uni.paramsize;
    if(argc == 1 && A_SYMBOL == argv[0].a_type) {
      /* read from array */
      t_symbol *s = atom_getsymbol(argv);
      t_garray *a;
      if (!(a = (t_garray *)pd_findbyclass(s, garray_class))) {
        error("no such table '%s' to fill values for uniform variable '%s'", s->s_name, name.c_str());
        return;
      }
      int npoints;
      t_word *vec;
      if (!garray_getfloatwords(a, &npoints, &vec)) {
        error("%s: bad template for uniform '%s'", s->s_name, name.c_str());
        return;
      }
      if(npoints < 0) {
        error("%s: illegal number of elements %d for uniform '%s'", s->s_name, npoints, name.c_str());
        return;
      }
      if(npoints > maxargc)
        npoints = maxargc;
      switch(uni.paramtype) {
      case GL_FLOAT:
        for(int j=0; j<npoints; j++) {
          uni.param.f[j] = vec[j].w_float;
        }
        break;
      case GL_INT:
        for(int j=0; j<npoints; j++) {
          uni.param.i[j] = (GLint)(vec[j].w_float);
        }
        break;
      }
    } else {
      /* read data from message */
      if(argc > maxargc) {
        argc=maxargc;
      }
      switch(uni.paramtype) {
      case GL_FLOAT:
        for (int j=0; j < argc; j++) {
          uni.param.f[j] = atom_getfloat(&argv[j]);
        }
        break;
      case GL_INT:
        for (int j=0; j < argc; j++) {
          uni.param.i[j] = (GLint)atom_getfloat(&argv[j]);
        }
        break;
      }
    }
    // tell the GL state that this variable has changed next render
    uni.changed = true;
    // should we return here?  It only allows one uni.param to be changed
    return;
  } catch (std::out_of_range&) {
    // if we reach this, then no param-name was matching!
    error("no method for '%s' (it's not a uniform variable)", s->s_name);
  }
}
void glsl_program :: keepUniformsMess(bool keep) {
  m_keepUniforms = keep;
}


/////////////////////////////////////////////////////////
// shaderMess
//
/////////////////////////////////////////////////////////
void glsl_program :: shaderMess(int argc, t_atom *argv)
{
  int i;

  if (!argc) {
    error("can't link non-existent shaders");
    return;
  }

  m_numShaders=0;
  for (i = 0; i < argc; i++) {
    if(m_numShaders>=MAX_NUM_SHADERS) {
      post("only %d shaders supported; skipping the rest", MAX_NUM_SHADERS);
      break;
    }
    GLuint ui=0;
    t_float f=atom_getfloat(argv+i);
    try {
      ui=m_shadermapper.get(f);
    } catch(GemException&x) {
      error("unable to get shaderID for %f...skipping!", f);
      continue;
    }
    m_shaderObj[m_numShaders]    = ui;
    m_shaderObjARB[m_numShaders] = ui;//static_cast<GLhandleARB>(fi.i);
    m_numShaders++;
  }
  setModified();
}

/////////////////////////////////////////////////////////
// LinkProgram
//
/////////////////////////////////////////////////////////
bool glsl_program :: LinkGL2()
{
  GLint infoLength;
  GLsizei length=0;
  int i;

  int numVertexShaders = 0;
  int numTessEvalShaders = 0;
  int numTessControlShaders = 0;
  int numGeometryShaders = 0;
  int numFragmentShaders = 0;

  if(m_program) {
    glDeleteProgram( m_program );
    m_programmapper.del(m_programmapped);
    m_programmapped=0.;
    m_program = 0;
  }
  m_program = glCreateProgram();
  for (i = 0; i < m_numShaders; i++) {
    GLint type;
    glAttachShader( m_program, m_shaderObj[i] );
    glGetShaderiv ( m_shaderObj[i], GL_SHADER_TYPE, &type);
    switch(type) {
    case GL_VERTEX_SHADER:
      numVertexShaders++;
      break;
    case  GL_GEOMETRY_SHADER:
      numGeometryShaders++;
      break;
    case  GL_FRAGMENT_SHADER:
      numFragmentShaders++;
      break;
    case  GL_TESS_EVALUATION_SHADER:
      numTessEvalShaders++;
      break;
    case GL_TESS_CONTROL_SHADER:
      numTessControlShaders++;
      break;
    }
  }

  /* setup geometry shader */
  if(numGeometryShaders>0 && glProgramParameteriEXT) {
    glProgramParameteriEXT(m_program,GL_GEOMETRY_INPUT_TYPE_EXT,m_geoInType);
    glProgramParameteriEXT(m_program,GL_GEOMETRY_OUTPUT_TYPE_EXT,m_geoOutType);

    int temp=m_geoOutVertices;
    if(temp<0) {
      glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT,&temp);
    }
    glProgramParameteriEXT(m_program,GL_GEOMETRY_VERTICES_OUT_EXT,temp);
  }

  if(numTessEvalShaders>0 && glPatchParameterfv) {
    float kInnerTessellationLevel = 1.0f;
    float kOuterTessellationLevel = 1.0f;
    const GLfloat innerTessLevels[2] = {
      kInnerTessellationLevel, // inner horizontal
      kInnerTessellationLevel  // inner vertical
    };

    const GLfloat outerTessLevels[4] = {
      kOuterTessellationLevel, // outer left (vertical)
      kOuterTessellationLevel, // outer bottom (horizontal)
      kOuterTessellationLevel, // outer right (vertical)
      kOuterTessellationLevel  // outer top (horizontal)
    };

    // We can define the tessellation levels using glPatchParameter if we don't
    // have a Tessellation Control Shader stage.
    glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, innerTessLevels);
    glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, outerTessLevels);
  }


  GLint linkstatus = 0;
  glLinkProgram( m_program );
  glGetProgramiv( m_program, GL_LINK_STATUS, &linkstatus );
  m_linked = linkstatus;

  glGetProgramiv( m_program, GL_INFO_LOG_LENGTH, &infoLength );
  GLchar *infoLog = new GLchar[infoLength];

  glGetProgramInfoLog( m_program, infoLength, &length, infoLog );

  if (length) {
    post("Info_log:");
    post("%s", infoLog);
  }

  if(infoLog) {
    delete[]infoLog;
  }
  infoLog=0;

  //
  // If all went well, make the ProgramObject part of the current state
  //
  //post("did we link?");
  if (m_linked) {
    glUseProgram( m_program );
  } else {
    glUseProgram( 0 );
    post("Link failed!");
    return false;
  }
  return true;
}
/////////////////////////////////////////////////////////
// LinkProgram
//
/////////////////////////////////////////////////////////
bool glsl_program :: LinkARB()
{
  int i;
  GLsizei length=0;
  GLint infoLength;

  if(m_programARB) {
    glDeleteObjectARB( m_programARB );
    m_programmapper.del(m_programmapped);
    m_programmapped=0.;
    m_programARB = 0;
  }
  m_programARB = glCreateProgramObjectARB();
  for (i = 0; i < m_numShaders; i++) {
    glAttachObjectARB( m_programARB, m_shaderObjARB[i] );
  }

  /* setup geometry shader */
  if(glProgramParameteriEXT) {
    glProgramParameteriEXT(m_program,GL_GEOMETRY_INPUT_TYPE_EXT,m_geoInType);
    glProgramParameteriEXT(m_program,GL_GEOMETRY_OUTPUT_TYPE_EXT,m_geoOutType);

    int temp=m_geoOutVertices;
    if(temp<0) {
      glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT,&temp);
    }
    glProgramParameteriEXT(m_program,GL_GEOMETRY_VERTICES_OUT_EXT,temp);
  }

  glLinkProgramARB( m_programARB );
  GLint linkstatus = 0;
  glGetObjectParameterivARB( m_programARB, GL_OBJECT_LINK_STATUS_ARB,
                             &linkstatus );
  m_linked = linkstatus;

  glGetObjectParameterivARB( m_programARB, GL_OBJECT_INFO_LOG_LENGTH_ARB,
                             &infoLength );

  GLcharARB*infoLogARB = new GLcharARB[infoLength];

  glGetInfoLogARB( m_programARB, infoLength, &length, infoLogARB );

  if (length) {
    post("Info_log:");
    post("%s", infoLogARB);
  }
  //post("freeing log");
  if(infoLogARB) {
    delete[]infoLogARB;
  }
  infoLogARB=0;

  //
  // If all went well, make the ProgramObject part of the current state
  //
  //post("did we link?");
  if (m_linked) {
    glUseProgramObjectARB( m_programARB );
  } else {
    glUseProgramObjectARB( 0 );
    post("ARB Link failed!");
    return false;
  }
  return true;
}

void glsl_program :: LinkProgram()
{
  bool success=false;
  if (!m_numShaders) {
    error("can't link zero shaders");
    return;
  }

  if(GLEW_VERSION_2_0) {
    success=LinkGL2();
  } else {
    success=LinkARB();
  }


  if(!success) {
    return;
  }

  //post("getting variables");
  getVariables();

  if(m_program || m_programARB) {
    for (std::map<std::string, std::vector<t_atom> >::const_iterator it = m_cachedParameters.begin();
         it != m_cachedParameters.end();
         it++)
      {
        t_symbol*s = gensym(it->first.c_str());
        paramMess(s, it->second.size(), it->second.data());
      }
    m_cachedParameters.clear();
  }


#ifdef __APPLE__
  // call API to check if linked program is running on hardware or in software emulation
  GemCGLint vertexGPUProcessing, fragmentGPUProcessing;
  CGLGetParameter (CGLGetCurrentContext(), kCGLCPGPUVertexProcessing,
                   &vertexGPUProcessing);
  CGLGetParameter (CGLGetCurrentContext(), kCGLCPGPUFragmentProcessing,
                   &fragmentGPUProcessing);

  post("vertex shader running in %sware", vertexGPUProcessing?"hard":"soft");
  post("fragment shader running in %sware",
       fragmentGPUProcessing?"hard":"soft");
#endif //__APPLE__


  // send program ID to outlet
  /* JMZ: shouldn't this be only done, when we have a linked program? */
  t_atom a;
  if(GLEW_VERSION_2_0) {
    m_programmapped=m_programmapper.set(m_program, m_programmapped);
  } else {
    m_programmapped=m_programmapper.set(m_programARB, m_programmapped);
  }
  SETFLOAT(&a, m_programmapped);
  outlet_list(m_outProgramID, 0, 1, &a);

}

/////////////////////////////////////////////////////////
// getVariables
//
/////////////////////////////////////////////////////////
void glsl_program :: getVariables()
{
  if(!m_linked) {
    return;
  }
  //
  // Get the number of uniforms, and the length of the longest name.
  //
  GLint maxLength = 0;
  GLint uniformcount = 0;
  if(GLEW_VERSION_2_0) {
    glGetProgramiv( m_program,
                    GL_ACTIVE_UNIFORM_MAX_LENGTH,
                    &maxLength);
    glGetProgramiv( m_program, GL_ACTIVE_UNIFORMS,
                    &uniformcount);
  } else if (GLEW_ARB_shader_objects) {
    glGetObjectParameterivARB( m_programARB,
                               GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB,
                               &maxLength);
    glGetObjectParameterivARB( m_programARB, GL_OBJECT_ACTIVE_UNIFORMS_ARB,
                               &uniformcount);
  }
  //
  // Loop over the ActiveUniform's and store the results away.
  //
  GLchar *nameGL=new GLchar[maxLength];
  GLcharARB *nameARB=new GLcharARB[maxLength];
  std::string name;
  GLsizei    length=0;
  std::map<std::string, t_uniform>olduniforms(m_uniforms);
  m_uniforms.clear();

  for (GLuint i = 0; i < uniformcount; i++) {
    GLint loc, size, arraysize=1;
    GLenum type;
    if(GLEW_VERSION_2_0) {
      glGetActiveUniform(m_program, i, maxLength, &length, &size,
                         &type, nameGL);
      loc = glGetUniformLocation( m_program, nameGL );
      name=(char*)nameGL;
      if (glGetActiveUniformsiv) {
        glGetActiveUniformsiv(m_program, 1, &i, GL_UNIFORM_SIZE, &arraysize);
      }
    } else if (GLEW_ARB_shader_objects) {
      glGetActiveUniformARB(m_programARB, i, maxLength, &length, &size,
                            &type, nameARB);
      loc = glGetUniformLocationARB( m_programARB, nameARB );
      name=(char*)nameARB;
      arraysize = 1;
    }

    name = removeArrayBrackets(name);
    std::map<std::string, t_uniform>::const_iterator it = olduniforms.find(name);
    if (olduniforms.end() == it || !m_keepUniforms) {
      m_uniforms[name] = t_uniform(this, loc, type, arraysize);
    } else {
      // we already have this uniform cached.
      // check if it is compatible, and if so, use the old one
      const t_uniform &old = it->second;
      if ((old.type == type) && (old.arraysize == arraysize)) {
        m_uniforms[name] = old;
        t_uniform &uni = m_uniforms[name];
        uni.loc = loc; /* the location might have changed, so update it */
        uni.changed = true; /* make sure to apply the value on next render */
      } else {
        m_uniforms[name] = t_uniform(this, loc, type, arraysize);
      }
    }
  }
  delete[]nameGL;
  delete[]nameARB;
}

/////////////////////////////////////////////////////////
// printInfo
//
/////////////////////////////////////////////////////////
void glsl_program :: printInfo()
{
  if(!m_linked) {
    error("no GLSL-program linked");
    return;
  }

  post("glsl_program Info");
  post("=================");

  if(GLEW_VERSION_2_0) {
    GLuint program = m_program;
    startpost("linked shaders");
    for (int i = 0; i < m_numShaders; i++)  {
      startpost( " %d", m_shaderObj[i] );
    }
    post("-> %d", program);
  } else {
    GLhandleARB program = m_programARB;
    startpost("linked ARB-shaders");
    for (int i = 0; i < m_numShaders; i++)  {
      startpost( " %d", m_shaderObjARB[i] );
    }
    post("-> %d", program);
  }

  post("");
  for(std::map<std::string, t_uniform>::const_iterator it = m_uniforms.begin(); it != m_uniforms.end(); it++) {
    const t_uniform &uni = it->second;
    startpost("uniform#%d: \"%s\": ",
              uni.loc, it->first.c_str());
#define SWITCHPOST(label) case label: post("%s", #label); break
    switch (uni.type) {
      SWITCHPOST(GL_FLOAT);
      SWITCHPOST(GL_FLOAT_VEC2);
      SWITCHPOST(GL_FLOAT_VEC3);
      SWITCHPOST(GL_FLOAT_VEC4);

      SWITCHPOST(GL_INT);
      SWITCHPOST(GL_INT_VEC2);
      SWITCHPOST(GL_INT_VEC3);
      SWITCHPOST(GL_INT_VEC4);

      SWITCHPOST(GL_BOOL);
      SWITCHPOST(GL_BOOL_VEC2);
      SWITCHPOST(GL_BOOL_VEC3);
      SWITCHPOST(GL_BOOL_VEC4);

      SWITCHPOST(GL_FLOAT_MAT2);
      SWITCHPOST(GL_FLOAT_MAT3);
      SWITCHPOST(GL_FLOAT_MAT4);

      SWITCHPOST(GL_SAMPLER_1D);
      SWITCHPOST(GL_SAMPLER_2D);
      SWITCHPOST(GL_SAMPLER_3D);
      SWITCHPOST(GL_SAMPLER_CUBE);
      SWITCHPOST(GL_SAMPLER_1D_SHADOW);
      SWITCHPOST(GL_SAMPLER_2D_SHADOW);
    //      SWITCHPOST(GL_SAMPLER_2D_RECT);
    default:
      switch(uni.type) {
        //    SWITCHPOST(GL_FLOAT_ARB);
        SWITCHPOST(GL_FLOAT_VEC2_ARB);
        SWITCHPOST(GL_FLOAT_VEC3_ARB);
        SWITCHPOST(GL_FLOAT_VEC4_ARB);

        //    SWITCHPOST(GL_INT_ARB);
        SWITCHPOST(GL_INT_VEC2_ARB);
        SWITCHPOST(GL_INT_VEC3_ARB);
        SWITCHPOST(GL_INT_VEC4_ARB);

        SWITCHPOST(GL_BOOL_ARB);
        SWITCHPOST(GL_BOOL_VEC2_ARB);
        SWITCHPOST(GL_BOOL_VEC3_ARB);
        SWITCHPOST(GL_BOOL_VEC4_ARB);

        SWITCHPOST(GL_FLOAT_MAT2_ARB);
        SWITCHPOST(GL_FLOAT_MAT3_ARB);
        SWITCHPOST(GL_FLOAT_MAT4_ARB);

        SWITCHPOST(GL_SAMPLER_1D_ARB);
        SWITCHPOST(GL_SAMPLER_2D_ARB);
        SWITCHPOST(GL_SAMPLER_3D_ARB);
        SWITCHPOST(GL_SAMPLER_CUBE_ARB);
        SWITCHPOST(GL_SAMPLER_1D_SHADOW_ARB);
        SWITCHPOST(GL_SAMPLER_2D_SHADOW_ARB);
        SWITCHPOST(GL_SAMPLER_2D_RECT_ARB);
      default:
        post("unknown uniform type (0x%X)", uni.type);
        break;
      }
      break;
    }
  }
}

void glsl_program:: intypeMess(GLuint intype)
{
  m_geoInType=intype;
  if(m_program && glProgramParameteriEXT) {
    glProgramParameteriEXT(m_program,GL_GEOMETRY_INPUT_TYPE_EXT,m_geoInType);
  }
}
void glsl_program:: outtypeMess(GLuint outtype)
{
  m_geoOutType=outtype;
  if(m_program && glProgramParameteriEXT) {
    glProgramParameteriEXT(m_program,GL_GEOMETRY_OUTPUT_TYPE_EXT,m_geoOutType);
  }
}
void glsl_program:: outverticesMess(GLint vertices)
{
  m_geoOutVertices=vertices;
  if(m_program && glProgramParameteriEXT) {
    int temp=m_geoOutVertices;

    if(temp<0) {
      glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT,&temp);
    }
    glProgramParameteriEXT(m_program,GL_GEOMETRY_VERTICES_OUT_EXT,temp);
  }
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void glsl_program :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG (classPtr, "shader", shaderMess);
  CPPEXTERN_MSG (classPtr, "link", linkMess);
  CPPEXTERN_MSG0(classPtr, "print", printInfo);
  CPPEXTERN_MSG1(classPtr, "keepuniforms", keepUniformsMess, bool);

  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&glsl_program::intypeMessCallback),
                  gensym("geometry_intype"), A_GIMME, A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&glsl_program::outtypeMessCallback),
                  gensym("geometry_outtype"), A_GIMME, A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&glsl_program::typeMessCallback),
                  gensym("geometry_type"), A_GIMME, A_NULL);

  CPPEXTERN_MSG1(classPtr, "geometry_outvertices", outverticesMess, int);

  class_addanything(classPtr,
                    reinterpret_cast<t_method>(&glsl_program::paramMessCallback));
}
void glsl_program :: linkMess(t_symbol*, int argc, t_atom*argv)
{
  if(argc) {
    shaderMess(argc, argv);
  }
}
void glsl_program :: paramMessCallback(void *data, t_symbol* s, int argc,
                                       t_atom *argv)
{
  GetMyClass(data)->paramMess(s, argc, argv);
}
void glsl_program :: intypeMessCallback(void *data, t_symbol* s, int argc,
                                        t_atom *argv)
{
  if(argc==1) {
    GetMyClass(data)->intypeMess(static_cast<GLenum>(getGLdefine(argv)));
  } else {
    GetMyClass(data)->error("input-type must be exactly one parameter");
  }
}
void glsl_program :: outtypeMessCallback(void *data, t_symbol* s, int argc,
    t_atom *argv)
{
  if(argc==1) {
    GetMyClass(data)->outtypeMess(static_cast<GLenum>(getGLdefine(argv)));
  } else {
    GetMyClass(data)->error("output type must be exactly one parameter");
  }
}
void glsl_program :: typeMessCallback(void *data, t_symbol* s, int argc,
                                      t_atom *argv)
{
  if(argc==2) {
    GetMyClass(data)->intypeMess (static_cast<GLenum>(getGLdefine(argv+0)));
    GetMyClass(data)->outtypeMess(static_cast<GLenum>(getGLdefine(argv+1)));
  } else {
    GetMyClass(
      data)->error("geometry_type must have exactly two parameters (input-type & output-type)");
  }
}
