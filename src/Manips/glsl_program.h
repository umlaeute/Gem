/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Link a GLSL program/shader
 
 *  Created by tigital on 11/13/2005.
 *  Copyright 2005 tigital.
 
Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GLSL_PROGRAM_H_
#define INCLUDE_GLSL_PROGRAM_H_

#include "Base/GemBase.h"

#ifdef __APPLE__
# define t_GLshaderObj GLhandleARB*
#else
# define t_GLshaderObj GLhandleARB
#endif


/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  glsl_program
    
  Link in a GLSL program/shader
    
  KEYWORDS
  
    
  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN glsl_program : public GemBase
{
  CPPEXTERN_HEADER(glsl_program, GemBase)
    
    public:
  
  //////////
  // Constructor
  glsl_program();

 protected:
    
  //////////
  // Destructor
  virtual ~glsl_program();
    	
  //////////
  // Do the rendering
  virtual void 	render(GemState *state);
  
  //////////
  // Clean up after rendering
  virtual void 	postrender(GemState *state);
  
  //////////
  // shader message
  virtual void 	shaderMess(int argc, t_atom *argv);
  
  //////////
  // Do the linking
  virtual void 	LinkProgram();
  
  //////////
  // What can we play with?
  virtual void 	getVariables();
  
  //////////
  // Print Info about Hardware limits
  virtual void printInfo();
  
  GLhandleARB		m_program;
  t_GLshaderObj		m_shaderObj[32];
  GLint				m_maxLength;
  GLint				m_uniformCount;
  GLcharARB			**m_name;
  GLsizei			*m_length;
  GLint				*m_size;
  GLenum			*m_type;
  GLint				m_linked;
  GLcharARB*		m_infoLog;
  int				m_num;

 private:
	
  //////////
  // static member functions
  static void shaderMessCallback (void *data, t_symbol *, int, t_atom*);
  static void openMessCallback   (void *data, t_symbol *filename);
  static void linkCallback  (void *);
  static void printMessCallback  (void *);
};

#endif	// for header file
