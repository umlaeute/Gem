/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an ARB fragment program/shader
 
 *  Created by tigital on 10/16/04.
 *  Copyright 2004 tigital.
 
Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_FRAGMENT_PROGRAM_H_
#define INCLUDE_FRAGMENT_PROGRAM_H_

#include "Manips/vertex_program.h"

#ifndef DONT_USE_ARB
# ifndef GL_ARB_fragment_program
#  define GL_ARB_fragment_program
#  define GL_FRAGMENT_PROGRAM_ARB           0x8804
#  define GL_PROGRAM_ALU_INSTRUCTIONS_ARB   0x8805
#  define GL_PROGRAM_TEX_INSTRUCTIONS_ARB   0x8806
#  define GL_PROGRAM_TEX_INDIRECTIONS_ARB   0x8807
#  define GL_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB 0x8808
#  define GL_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB 0x8809
#  define GL_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB 0x880A
#  define GL_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB 0x880B
#  define GL_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB 0x880C
#  define GL_MAX_PROGRAM_TEX_INDIRECTIONS_ARB 0x880D
#  define GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB 0x880E
#  define GL_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB 0x880F
#  define GL_MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB 0x8810
#  define GL_MAX_TEXTURE_COORDS_ARB         0x8871
#  define GL_MAX_TEXTURE_IMAGE_UNITS_ARB    0x8872
# endif /* GL_ARB_fragment_program */
#endif /* DONT_USE_ARB */

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  fragment_program
    
  Loads in an ARB fragment program/shader
    
  KEYWORDS
  
    
  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN fragment_program : public vertex_program
{
  CPPEXTERN_HEADER(fragment_program, vertex_program)
    
    public:
  
  //////////
  // Constructor
  fragment_program(t_symbol *filename);

 protected:
    
  //////////
  // Destructor
  virtual ~fragment_program();

  //////////
  // which Program do we have (ARB, NV,...)
  virtual GLint queryProgramtype(char*program);

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
