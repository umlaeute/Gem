/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an ARB fragment program/shader
 
 *  Created by tigital on 10/16/04.
 *  Copyright 2004 tigital.
 
Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) G¬∏nther Geiger. geiger@epy.co.at
Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f¬∏r::uml‚Ä∞ute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_FRAGMENT_PROGRAM_H_
#define INCLUDE_FRAGMENT_PROGRAM_H_

#include <string.h>
#include <stdio.h>

#include "Base/config.h"

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  fragment_program
    
  Loads in an ARB fragment program/shader
    
  KEYWORDS
  
    
  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN fragment_program : public GemBase
{
  CPPEXTERN_HEADER(fragment_program, GemBase)
    
    public:
  
  //////////
  // Constructor
  fragment_program(t_symbol *filename);

 protected:
    
  //////////
  // Destructor
  virtual ~fragment_program();

  //////////
  // close the program file
  virtual void closeMess(void){}
  //////////
  // open a program up
  virtual void openMess(t_symbol *filename);

  //////////
  // Do the rendering
  virtual void render(GemState *state);


  //////////
  // Clear the dirty flag on the pixBlock
  virtual void postrender(GemState *state);

  //////////
  virtual void startRendering();

  //////////
  // Delete texture object
  virtual void stopRendering() {}
  
  //////////
  // Print Info about Hardware limits
  virtual void printInfo();

  //////////
  // the current file
  t_symbol *x_filename;
	
  //////////
  //
  GLuint 	    m_programTarget;
  GLuint        m_programID;
  char*			m_programString;
  int			m_size;

  //////////
  // a outlet for information like #frames and "reached end"
  t_outlet     *m_outNumFrames;
  t_outlet     *m_outEnd;

 protected:
	
  //////////
  // static member functions
  static void openMessCallback   (void *data, t_symbol *filename);
  static void printMessCallback  (void *);
};

#endif	// for header file
