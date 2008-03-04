/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an GLSL fragment shader
 
 *  Created by tigital on 11/13/2005.
 *  Copyright 2005 James Tittle.
 
Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GLSL_FRAGMENT_H_
#define INCLUDE_GLSL_FRAGMENT_H_

#include "Manips/glsl_vertex.h"

#define GEM_shader_none  0
#define GEM_shader_GLSL  1
#define GEM_shader_CG    2


/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  glsl_fragment
    
  Loads in a fragment shader
    
  KEYWORDS
  
    
  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN glsl_fragment : public glsl_vertex
{
  CPPEXTERN_HEADER(glsl_fragment, glsl_vertex)
    
    public:
  
  //////////
  // Constructor
  glsl_fragment(void);
  glsl_fragment(t_symbol *filename);

 protected:
    
  //////////
  // Destructor
  virtual ~glsl_fragment();

  ////////
  // extension check
  virtual bool isRunnable(void);

  //////////
  // Print Info about Hardware limits
  virtual void printInfo();


 protected:
	
  //////////
  // static member functions
  static void openMessCallback   (void *data, t_symbol *filename);
  static void printMessCallback  (void *);
};

#endif	// for header file
