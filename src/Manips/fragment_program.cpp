////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Created by tigital on 10/16/04.
// Copyright 2004 James Tittle
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) G¬∏nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f¬∏r::uml‚Ä∞ute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "fragment_program.h"
#include "string.h"

CPPEXTERN_NEW_WITH_ONE_ARG(fragment_program, t_symbol *, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// fragment_program
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
fragment_program :: fragment_program(t_symbol *filename) :
  vertex_program()
{
#if !defined GL_ARB_fragment_program && !defined GL_NV_fragment_program 
  post("GEM has been compiled without 'fragment_program' support");
  return;
#endif
  openMess(filename);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
fragment_program :: ~fragment_program()
{
  closeMess();
}


/////////////////////////////////////////////////////////
// openMess
//
/////////////////////////////////////////////////////////
GLint fragment_program :: queryProgramtype(char*program)
{
#ifdef GL_FRAGMENT_PROGRAM_ARB
  if(!strncmp(program,"!!ARBfp1.0",10)){
    m_programTarget=GL_FRAGMENT_PROGRAM_ARB;
    return(GEM_PROGRAM_ARB);
  }
#endif /* GL_FRAGMENT_PROGRAM_ARB */
#ifdef GL_FRAGMENT_PROGRAM_NV
  if(!strncmp(program,"!!FP1.0",7)){
    m_programTarget=GL_FRAGMENT_PROGRAM_NV;
    return(GEM_PROGRAM_NV);
  }
#endif /* GL_FRAGMENT_PROGRAM_NV */

  return GEM_PROGRAM_none;
}

/////////////////////////////////////////////////////////
// printInfo
//
/////////////////////////////////////////////////////////
void fragment_program :: printInfo()
{
	GLint bitnum = 0;
	post("fragment_Program Hardware Info");
	post("==============================");
	
#ifdef GL_MAX_PROGRAM_MATRICES_ARB
	glGetIntegerv( GL_MAX_PROGRAM_MATRICES_ARB, &bitnum );
	post("MAX_PROGRAM_MATRICES: %d", bitnum);
#endif
#ifdef GL_MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB
	glGetIntegerv( GL_MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB, &bitnum );
	post("MAX_PROGRAM_MATRIX_STACK_DEPTH: %d", bitnum);
#endif
#ifdef GL_MAX_TEXTURE_COORDS_ARB
	glGetIntegerv( GL_MAX_TEXTURE_COORDS_ARB, &bitnum );
	post("MAX_TEXTURE_COORDS: %d", bitnum);
#endif 
#ifdef GL_MAX_TEXTURE_IMAGE_UNITS_ARB
	glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &bitnum );
	post("MAX_TEXTURE_IMAGE_UNITS: %d", bitnum);
#endif

#if defined GL_ARB_fragment_program && defined GL_ARB_vertex_program
# ifdef GL_MAX_PROGRAM_INSTRUCTIONS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_INSTRUCTIONS_ARB, &bitnum);
	post("MAX_PROGRAM_INSTRUCTIONS: %d", bitnum);
# endif
# ifdef GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB, &bitnum);
	post("MAX_PROGRAM_NATIVE_INSTRUCTIONS: %d", bitnum);
# endif
# ifdef GL_MAX_PROGRAM_TEMPORARIES_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_TEMPORARIES_ARB, &bitnum);
	post("MAX_PROGRAM_TEMPORARIES: %d", bitnum);
# endif
# ifdef GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB, &bitnum);
	post("MAX_PROGRAM_NATIVE_TEMPORARIES: %d", bitnum);
# endif
# ifdef GL_MAX_PROGRAM_PARAMETERS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_PARAMETERS_ARB, &bitnum);
	post("MAX_PROGRAM_PARAMETERS: %d", bitnum);
# endif
# ifdef GL_MAX_PROGRAM_NATIVE_PARAMETERS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_PARAMETERS_ARB, &bitnum);
	post("MAX_PROGRAM_NATIVE_PARAMETERS: %d", bitnum);
# endif
# ifdef GL_MAX_PROGRAM_ATTRIBS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_ATTRIBS_ARB, &bitnum);
	post("MAX_PROGRAM_ATTRIBS: %d", bitnum);
# endif
# ifdef GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB, &bitnum);
	post("MAX_PROGRAM_NATIVE_ATTRIBS: %d", bitnum);
# endif
# ifdef GL_MAX_PROGRAM_ADDRESS_REGISTERS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_ADDRESS_REGISTERS_ARB, &bitnum);
	post("MAX_PROGRAM_ADDRESS_REGISTERS: %d", bitnum);
# endif
# ifdef GL_MAX_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB, &bitnum);
	post("MAX_PROGRAM_NATIVE_ADDRESS_REGISTERS: %d", bitnum);
# endif
# ifdef GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB, &bitnum);
	post("MAX_PROGRAM_LOCAL_PARAMETERS: %d", bitnum);
# endif
# ifdef GL_MAX_PROGRAM_ENV_PARAMETERS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_ENV_PARAMETERS_ARB, &bitnum);
	post("MAX_PROGRAM_ENV_PARAMETERS: %d", bitnum);
	post("");
# endif
# ifdef GL_PROGRAM_LENGTH_ARB	
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_LENGTH_ARB, &bitnum);
	post("PROGRAM_LENGTH: %d", bitnum);
# endif
# ifdef GL_PROGRAM_FORMAT_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ARB, &bitnum);
	post("PROGRAM_FORMAT: %d", bitnum);
# endif
# ifdef GL_PROGRAM_BINDING_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_BINDING_ARB, &bitnum);
	post("PROGRAM_BINDING: %d", bitnum);
# endif
# ifdef GL_PROGRAM_INSTRUCTIONS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_INSTRUCTIONS_ARB, &bitnum);
	post("PROGRAM_INSTRUCTIONS: %d", bitnum);
# endif
# ifdef GL_PROGRAM_NATIVE_INSTRUCTIONS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_NATIVE_INSTRUCTIONS_ARB, &bitnum);
	post("PROGRAM_NATIVE_INSTRUCTIONS: %d", bitnum);
# endif
# ifdef GL_PROGRAM_TEMPORARIES_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_TEMPORARIES_ARB, &bitnum);
	post("PROGRAM_TEMPORARIES: %d", bitnum);
# endif
# ifdef GL_PROGRAM_NATIVE_TEMPORARIES_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_NATIVE_TEMPORARIES_ARB, &bitnum);
	post("PROGRAM_NATIVE_TEMPORARIES: %d", bitnum);
# endif
# ifdef GL_PROGRAM_PARAMETERS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_PARAMETERS_ARB, &bitnum);
	post("PROGRAM_PARAMETERS: %d", bitnum);
# endif
# ifdef GL_PROGRAM_NATIVE_PARAMETERS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_NATIVE_PARAMETERS_ARB, &bitnum);
	post("PROGRAM_NATIVE_PARAMETERS: %d", bitnum);
# endif
# ifdef GL_PROGRAM_ATTRIBS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_ATTRIBS_ARB, &bitnum);
	post("PROGRAM_ATTRIBS: %d", bitnum);
# endif
# ifdef GL_PROGRAM_NATIVE_ATTRIBS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_NATIVE_ATTRIBS_ARB, &bitnum);
	post("PROGRAM_NATIVE_ATTRIBS: %d", bitnum);
# endif
# ifdef GL_PROGRAM_ADDRESS_REGISTERS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_ADDRESS_REGISTERS_ARB, &bitnum);
	post("PROGRAM_ADDRESS_REGISTERS: %d", bitnum);
# endif
# ifdef GL_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB, &bitnum);
	post("PROGRAM_NATIVE_ADDRESS_REGISTERS: %d", bitnum);
	post("");
# endif
# ifdef GL_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB	
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB, &bitnum);
	post("MAX_PROGRAM_ALU_INSTRUCTIONS: %d", bitnum);
# endif
# ifdef GL_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB, &bitnum);
	post("MAX_PROGRAM_TEX_INSTRUCTIONS: %d", bitnum);
# endif
# ifdef GL_MAX_PROGRAM_TEX_INDIRECTIONS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_TEX_INDIRECTIONS_ARB, &bitnum);
	post("MAX_PROGRAM_TEX_INDIRECTIONS: %d", bitnum);
# endif
# ifdef GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB, &bitnum);
	post("MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS: %d", bitnum);
# endif
# ifdef GL_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB, &bitnum);
	post("MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS: %d", bitnum);
	post("");
# endif
# ifdef GL_PROGRAM_ALU_INSTRUCTIONS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_ALU_INSTRUCTIONS_ARB, &bitnum);
	post("PROGRAM_ALU_INSTRUCTIONS: %d", bitnum);
# endif
# ifdef GL_PROGRAM_TEX_INSTRUCTIONS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_TEX_INSTRUCTIONS_ARB, &bitnum);
	post("PROGRAM_TEX_INSTRUCTIONS: %d", bitnum);
# endif
# ifdef GL_PROGRAM_TEX_INDIRECTIONS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_TEX_INDIRECTIONS_ARB, &bitnum);
	post("PROGRAM_TEX_INDIRECTIONS: %d", bitnum);
# endif
# ifdef GL_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB, &bitnum);
	post("PROGRAM_NATIVE_ALU_INSTRUCTIONS: %d", bitnum);
# endif
# ifdef GL_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB, &bitnum);
	post("PROGRAM_NATIVE_TEX_INSTRUCTIONS: %d", bitnum);
# endif
# ifdef GL_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB
	glGetProgramivARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB, &bitnum);
	post("PROGRAM_NATIVE_TEX_INDIRECTIONS: %d", bitnum);
# endif
#endif /* GL_ARB_fragment_program */
	post("");

}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void fragment_program :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&fragment_program::openMessCallback,
		  gensym("open"), A_SYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&fragment_program::printMessCallback,
		  gensym("print"), A_NULL);
}
void fragment_program :: openMessCallback(void *data, t_symbol *filename)
{
	    GetMyClass(data)->openMess(filename);
}
void fragment_program :: printMessCallback(void *data)
{
	GetMyClass(data)->printInfo();
}
