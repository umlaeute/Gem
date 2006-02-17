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

#ifdef __APPLE__
#include <AGL/agl.h>
extern bool HaveValidContext (void);
#endif

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
#ifdef GL_ARB_shader_objects
  m_shaderTarget = GL_FRAGMENT_SHADER_ARB;
  openMess(filename);
#endif
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
glsl_fragment :: ~glsl_fragment()
{
  closeMess();
}

/////////////////////////////////////////////////////////
// printInfo
//
/////////////////////////////////////////////////////////
void glsl_fragment :: printInfo()
{
#ifdef GL_ARB_fragment_shader
# ifdef __APPLE__
	if (!HaveValidContext ()) {
		post("GEM:[%s] needs window/context to load shader", m_objectname->s_name);
		return;
	}
# endif
	GLint bitnum = 0;
	post("glsl_fragment Hardware Info");
	post("============================");
# ifdef GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB
	glGetIntegerv( GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB, &bitnum );
	post("MAX_FRAGMENT_UNIFORM_COMPONENTS: %d", bitnum);
# endif
# ifdef GL_MAX_TEXTURE_COORDS_ARB
	glGetIntegerv( GL_MAX_TEXTURE_COORDS_ARB, &bitnum );
	post("MAX_TEXTURE_COORDS: %d", bitnum);
# endif 
# ifdef GL_MAX_TEXTURE_IMAGE_UNITS_ARB
	glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &bitnum );
	post("MAX_TEXTURE_IMAGE_UNITS: %d", bitnum);
# endif
#endif /* GL_ARB_fragment_shader */
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void glsl_fragment :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&glsl_fragment::openMessCallback,
		  gensym("open"), A_SYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&glsl_fragment::printMessCallback,
		  gensym("print"), A_NULL);
}
void glsl_fragment :: openMessCallback(void *data, t_symbol *filename)
{
	    GetMyClass(data)->openMess(filename);
}
void glsl_fragment :: printMessCallback(void *data)
{
	GetMyClass(data)->printInfo();
}
