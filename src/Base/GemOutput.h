/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  Parent class for various output-modules

  Copyright (c) 1997-200 Mark Danks.
  Copyright (c) Günther Geiger. geiger@epy.co.at
  Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef INCLUDE_GEMOUTPUT_H_
#define INCLUDE_GEMOUTPUT_H_

#include "Base/CPPExtern.h"

#define gem_control void*
/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  GemOutput
    
  an output module

  DESCRIPTION
    
  this is the parent module for various output-objects, like [gemwindow]

  children are to be connected to the outlet of [gemcontrol]
  each render-cycle the [gemcontrol] emmits a "gem_render" message which holds 
  a callback-pointer to the actual rendering code

  what is going on:
    on "create" the output-device (e.g.: window) is created
    on "destroy" the output-device (e.g.: window) is closed
    on "render" the following happens:
      1. preRender() is called, where the output-objects sets the display-context
      2. callback the rendering-function
      3. postRender() for objects that want to grab the pixel-data and send it e.g. to a video-out


  "render" - render to this window
  "create" - create a graphics window
  "destroy" - destroy the graphics window

  -----------------------------------------------------------------*/
class GEM_EXTERN GemOutput : public CPPExtern
{
  CPPEXTERN_HEADER(GemOutput, CPPExtern)
    
    public:
    
  //////////
  // Constructor
  GemOutput();
    	
 private:

  //////////
  // Destructor
  virtual     	~GemOutput();

  void		renderMess(gem_control gc);
  virtual void  preRender (gem_control gc)=0;
  virtual void  postRender(gem_control gc){};

  virtual void 	createMess();
  virtual void  destroyMess();

 private:
    
  //////////
  // Static member functions
  static void 	renderMessCallback (void *data, t_gpointer*);
  static void 	createMessCallback (void *data);
  static void 	destroyMessCallback(void *data);
};

#endif	// for header file
