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
//    Copyright (c) G¸nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f¸r::uml‰ute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "vertex_program.h"

#include <string.h>
#include <stdio.h>

#ifdef __APPLE__
#include <AGL/agl.h>
extern bool HaveValidContext (void);
#endif

CPPEXTERN_NEW_WITH_ONE_ARG(vertex_program, t_symbol *, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// vertex_program
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
vertex_program :: vertex_program() :
  m_programType(GEM_PROGRAM_none), 
  m_programID(0), 
  m_programString(NULL), m_size(0)
{
}
vertex_program :: vertex_program(t_symbol *filename) :
  m_programType(GEM_PROGRAM_none), 
  m_programID(0), 
  m_programString(NULL), m_size(0)
{
  openMess(filename);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
vertex_program :: ~vertex_program()
{
  closeMess();
}


/////////////////////////////////////////////////////////
// closeMess
//
/////////////////////////////////////////////////////////
void vertex_program :: closeMess(void)
{
  delete [] m_programString;
  m_programString=NULL;
  m_size=0;
  if(m_programID){
    switch(m_programType){
#ifdef GL_NV_vertex_program
    case(GEM_PROGRAM_NV):
      glDeleteProgramsNV(1,&m_programID);
      break;
#endif
#ifdef GL_ARB_vertex_program
    case(GEM_PROGRAM_ARB):
      glDeleteProgramsARB(1,&m_programID);
      break;
#endif
    default:
      break;
    }
  }

  m_programID=0;
  m_programType=GEM_PROGRAM_none;
}
/////////////////////////////////////////////////////////
// openMess
//
/////////////////////////////////////////////////////////
GLint vertex_program :: queryProgramtype(char*program)
{
#ifdef GL_VERTEX_PROGRAM_ARB
  if(!strncmp(program,"!!ARBvp1.0",10)){
    m_programTarget=GL_VERTEX_PROGRAM_ARB;
    return(GEM_PROGRAM_ARB);
  }
#endif /* GL_VERTEX_PROGRAM_ARB */
#ifdef GL_VERTEX_PROGRAM_NV
  if(!strncmp(program,"!!VP1.0",7)){
    m_programTarget=GL_VERTEX_PROGRAM_NV;
    return(GEM_PROGRAM_NV);
  }
#endif /* GL_VERTEX_PROGRAM_NV */

  return GEM_PROGRAM_none;
}


void vertex_program :: openMess(t_symbol *filename)
{
  if(NULL==filename || NULL==filename->s_name)return;
#ifdef __APPLE__
  if (!HaveValidContext ()) {
    post("GEM: [%s] - need window/context to load program", m_objectname->s_name);
    return;
  }
#endif

  char buf[MAXPDSTRING];
  canvas_makefilename(getCanvas(), filename->s_name, buf, MAXPDSTRING);

  // Clean up any open files
  closeMess();

  //char *data;
  FILE *file = fopen(buf,"r");
  if(file) {
    fseek(file,0,SEEK_END);
    int size = ftell(file);
    m_programString = new char[size + 1];
    memset(m_programString,0,size + 1);
    fseek(file,0,SEEK_SET);
    fread(m_programString,1,size,file);
    fclose(file);
  } else {
    m_programString = new char[strlen(buf) + 1];
    strcpy(m_programString,buf);
  }
  m_size=strlen(m_programString);
  m_programType=queryProgramtype(m_programString);
  if(m_programType==GEM_PROGRAM_none){
    m_programID = 0;
    char *s = m_programString;
    while(*s && *s != '\n') s++;
    *s = '\0';
    post("[%s]: unknown program header \"%s\" or error open \"%s\" file\n",
	 m_objectname->s_name,
	 m_programString,filename->s_name);
    
    delete m_programString; m_programString=NULL;
    m_size=0;
    return;
  }

  post("[%s]: Loaded file: %s\n", m_objectname->s_name, buf);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void vertex_program :: LoadProgram(void){
  if(NULL==m_programString)return;
  GLint error=-1;

  switch(m_programType){
#ifdef GL_NV_vertex_program
  case  GEM_PROGRAM_NV:
    if (m_programID==0)
      {
	glEnable(m_programTarget);
	glGenProgramsNV(1, &m_programID);
	glBindProgramNV(m_programTarget, m_programID);
	glLoadProgramNV(m_programTarget, m_programID, m_size, (GLubyte*)m_programString);
	glGetIntegerv(GL_PROGRAM_ERROR_POSITION_NV, &error);
      } else {
        glEnable(m_programTarget);
	glBindProgramNV(m_programTarget, m_programID);
	return;
    }
    break;
#endif /* GL_NV_vertex_program */
#ifdef GL_ARB_vertex_program
  case  GEM_PROGRAM_ARB:
    if (m_programID==0)
      {
	glEnable(m_programTarget);
	glGenProgramsARB(1, &m_programID);
	glBindProgramARB( m_programTarget, m_programID);
	glProgramStringARB( m_programTarget, GL_PROGRAM_FORMAT_ASCII_ARB, m_size, m_programString);
        glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &error);
      } else {
        glEnable(m_programTarget);
	glBindProgramARB(m_programTarget, m_programID);
	return;
    }
    break;
#endif /* GL_ARB_vertex_program */
  default:
    return;
  }

  if(error != -1) {
    int line = 0;
    char *s = m_programString;
    while(error-- && *s) if(*s++ == '\n') line++;
    while(s >= m_programString && *s != '\n') s--;
    char *e = ++s;
    while(*e != '\n' && *e != '\0') e++;
    *e = '\0';
    post("vertex_program:  program error at line %d:\n\"%s\"\n",line,s);
#ifdef GL_PROGRAM_ERROR_STRING_ARB
    post("vertex_program:  %s\n", glGetString(GL_PROGRAM_ERROR_STRING_ARB));
#endif /* GL_PROGRAM_ERROR_STRING_ARB */
  }

#if defined GL_ARB_vertex_program && defined GL_PROGRAM_UNDER_NATIVE_LIMITS_ARB
  GLint bitnum;
  glGetProgramivARB( m_programTarget, GL_PROGRAM_UNDER_NATIVE_LIMITS_ARB, &bitnum);
  if (!bitnum)
    post("GEM: vertex_program not within native limits!!!");
#endif

}


void vertex_program :: startRendering()
{
  if (m_programString == NULL)
    {
      error("[%s]: need to load a program", m_objectname->s_name);
      return;
    }

  LoadProgram();
}

void vertex_program :: render(GemState *state)
{
  LoadProgram();
  glProgramEnvParameter4fvARB(m_programTarget, m_envNum, m_param);
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void vertex_program :: postrender(GemState *state)
{
  switch(m_programType){
  case  GEM_PROGRAM_NV:  case  GEM_PROGRAM_ARB:
    glDisable( m_programTarget );
    break;
  default:
    break;
  }
}

/////////////////////////////////////////////////////////
// printInfo
//
/////////////////////////////////////////////////////////
void vertex_program :: printInfo()
{
#ifdef __APPLE__
	if (!HaveValidContext ()) {
		post("GEM: vertex_program - need window/context to load program");
		return;
	}
#endif
#ifdef GL_ARB_vertex_program
	GLint bitnum = 0;
	post("Vertex_Program Hardware Info");
	post("============================");
	glGetIntegerv( GL_MAX_VERTEX_ATTRIBS_ARB, &bitnum );
	post("MAX_VERTEX_ATTRIBS: %d", bitnum);
	glGetIntegerv( GL_MAX_PROGRAM_MATRICES_ARB, &bitnum );
	post("MAX_PROGRAM_MATRICES: %d", bitnum);
	glGetIntegerv( GL_MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB, &bitnum );
	post("MAX_PROGRAM_MATRIX_STACK_DEPTH: %d", bitnum);
	
	glGetProgramivARB( GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_INSTRUCTIONS_ARB, &bitnum);
	post("MAX_PROGRAM_INSTRUCTIONS: %d", bitnum);
	glGetProgramivARB( GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB, &bitnum);
	post("MAX_PROGRAM_NATIVE_INSTRUCTIONS: %d", bitnum);
	glGetProgramivARB( GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_TEMPORARIES_ARB, &bitnum);
	post("MAX_PROGRAM_TEMPORARIES: %d", bitnum);
	glGetProgramivARB( GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB, &bitnum);
	post("MAX_PROGRAM_NATIVE_TEMPORARIES: %d", bitnum);
	glGetProgramivARB( GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_PARAMETERS_ARB, &bitnum);
	post("MAX_PROGRAM_PARAMETERS: %d", bitnum);
	glGetProgramivARB( GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_PARAMETERS_ARB, &bitnum);
	post("MAX_PROGRAM_NATIVE_PARAMETERS: %d", bitnum);
	glGetProgramivARB( GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_ATTRIBS_ARB, &bitnum);
	post("MAX_PROGRAM_ATTRIBS: %d", bitnum);
	glGetProgramivARB( GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB, &bitnum);
	post("MAX_PROGRAM_NATIVE_ATTRIBS: %d", bitnum);
	glGetProgramivARB( GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_ADDRESS_REGISTERS_ARB, &bitnum);
	post("MAX_PROGRAM_ADDRESS_REGISTERS: %d", bitnum);
	glGetProgramivARB( GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB, &bitnum);
	post("MAX_PROGRAM_NATIVE_ADDRESS_REGISTERS: %d", bitnum);
	glGetProgramivARB( GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB, &bitnum);
	post("MAX_PROGRAM_LOCAL_PARAMETERS: %d", bitnum);
	glGetProgramivARB( GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_ENV_PARAMETERS_ARB, &bitnum);
	post("MAX_PROGRAM_ENV_PARAMETERS: %d", bitnum);
	post("");
	
	glGetProgramivARB( GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_INSTRUCTIONS_ARB, &bitnum);
	post("PROGRAM_INSTRUCTIONS: %d", bitnum);
	glGetProgramivARB( GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_NATIVE_INSTRUCTIONS_ARB, &bitnum);
	post("PROGRAM_NATIVE_INSTRUCTIONS: %d", bitnum);
	glGetProgramivARB( GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_TEMPORARIES_ARB, &bitnum);
	post("PROGRAM_TEMPORARIES: %d", bitnum);
	glGetProgramivARB( GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_NATIVE_TEMPORARIES_ARB, &bitnum);
	post("PROGRAM_NATIVE_TEMPORARIES: %d", bitnum);
	glGetProgramivARB( GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_PARAMETERS_ARB, &bitnum);
	post("PROGRAM_PARAMETERS: %d", bitnum);
	glGetProgramivARB( GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_NATIVE_PARAMETERS_ARB, &bitnum);
	post("PROGRAM_NATIVE_PARAMETERS: %d", bitnum);
	glGetProgramivARB( GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_ATTRIBS_ARB, &bitnum);
	post("PROGRAM_ATTRIBS: %d", bitnum);
	glGetProgramivARB( GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_NATIVE_ATTRIBS_ARB, &bitnum);
	post("PROGRAM_NATIVE_ATTRIBS: %d", bitnum);
	glGetProgramivARB( GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_ADDRESS_REGISTERS_ARB, &bitnum);
	post("PROGRAM_ADDRESS_REGISTERS: %d", bitnum);
	glGetProgramivARB( GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB, &bitnum);
	post("PROGRAM_NATIVE_ADDRESS_REGISTERS: %d", bitnum);
#endif /* GL_ARB_vertex_program */
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void vertex_program :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&vertex_program::openMessCallback,
		  gensym("open"), A_SYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&vertex_program::printMessCallback,
		  gensym("print"), A_NULL);
}
void vertex_program :: openMessCallback(void *data, t_symbol *filename)
{
	    GetMyClass(data)->openMess(filename);
}
void vertex_program :: printMessCallback(void *data)
{
	GetMyClass(data)->printInfo();
}
