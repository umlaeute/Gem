/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  Parent class for various output-modules

  Copyright (c) 1997-200 Mark Danks.
  Copyright (c) G�nther Geiger. geiger@epy.co.at
  Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef INCLUDE_GEMOUTPUT_H_
#define INCLUDE_GEMOUTPUT_H_

#include "Base/CPPExtern.h"

#if defined _WINDOWS
# include <windows.h>
# include <GL/gl.h>
#elif defined __APPLE__
# include <OpenGL/gl.h>
#elif defined __unix__
# include <GL/gl.h>
#endif // OS

#include "GemState.h"

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
    on "render" the following happens:
      1. preRender() is called, where the output-objects sets the display-context
      2. callback the rendering-function: doRender()
      3. postRender() for objects that want to grab the pixel-data and send it e.g. to a video-out


  "render" - render to this context
  "view", "perspec" - set up the camera
  "color", "fog", ... - set up the lighting/color/...


  -----------------------------------------------------------------*/
class GEM_EXTERN GemOutput : public CPPExtern
{
  CPPEXTERN_HEADER(GemOutput, CPPExtern)
    
    public:
    
  //////////
  // Constructor
  GemOutput();

 protected:

  //////////
  // Destructor
  virtual     	~GemOutput();


  ////////////
  // an outlet to get information back to the user...
  t_outlet *m_outlet;

  ////////////
  // output information about this "window"
  virtual void      infoMess();

  ////////////
  // whenever a "render"-message is received from gemcontrol 
  // the rendering is setup and done...
  void		renderMess();

  ////////////
  // reset the gem-state (lighting,...)
  virtual void  fillGemState(GemState&);
  ////////////
  // make the context owned by this instance the current one
  virtual void  makeCurrent()=0; // get the current context
  ////////////
  // call the render-callbacks from GemMan
  virtual void  doRender  (GemState currentState); // this does the actual rendering
  ////////////
  // after the rendering is done...
  virtual void  postRender(GemState currentState)=0; // e.g. swap buffers

  unsigned int m_width, m_height; // dimension of the rendering context
  bool m_outputState;   // whether we can output (and thus want to render) or not
  bool m_outputContext; // whether we have a valid rendering context


  int m_buffer; // buffer mode (single or double)
  virtual void bufferMess(int buffer);


  /* the viewpoint */
  float	m_perspect[6]; // values for the perspective matrix
  virtual void	perspectiveMess(t_float, t_float, t_float, t_float, t_float, t_float);

  float	m_lookat[9];	  // values for the lookat matrix
  virtual void	viewMess(t_float, t_float, t_float, t_float, t_float, t_float, t_float, t_float, t_float);

  /* the overall material */
  GLfloat m_clear_color[4];	// the frame buffer clear
  virtual void 	  colorMess(t_float red, t_float green, t_float blue, t_float alpha);

  GLbitfield m_clear_mask;	// the clear bitmask
  virtual void 	     clearmaskMess(t_float bitmask);

  GLfloat m_mat_ambient[4];	// default ambient material
  virtual void 	  ambientMess(t_float red, t_float green, t_float blue, t_float alpha);

  GLfloat m_mat_specular[4];	// default specular material
  virtual void 	  specularMess(t_float red, t_float green, t_float blue, t_float alpha);

  GLfloat m_mat_shininess;	// default shininess material
  virtual void 	  shininessMess(t_float val);

  /* fog */ 
  virtual void  fogDensityMess(t_float density);
  virtual void  fogRangeMess(t_float start, t_float stop);
  virtual void 	fogColorMess(t_float red, t_float green, t_float blue, t_float alpha);
  virtual void	fogModeMess(int val);

  float	   m_fog;		// fog density
  enum FOG_TYPE { FOG_OFF = 0, FOG_LINEAR, FOG_EXP, FOG_EXP2 };
  FOG_TYPE m_fogMode;		// what kind of fog we have
  GLfloat  m_fogColor[4];	// colour of the fog
  float	   m_fogStart;		// start of the linear fog
  float	   m_fogEnd;		// start of the linear fog

  virtual  void	   lightMess(int val);
  bool     m_lightState;


  int               m_stereo;                   // stereo-mode
  GLfloat	    m_stereoSep;		// stereo seperation
  GLfloat	    m_stereoFocal;		// distance to focal point
  bool	            m_stereoLine;		// draw a line between 2 stereo-screens


  virtual void resetState();
  virtual void resetValues();

 private:
    
  //////////
  // Static member functions
  static void 	renderMessCallback (void *data);
  static void	resetMessCallback(void *);

  static void	infoMessCallback(void *);

  //////////
  // change the rendering somehow

  /* camera settings */
  static void	perspectiveMessCallback(void *, t_symbol *, int argc, t_atom *argv);
  static void	viewMessCallback(void *, t_symbol *, int argc, t_atom *argv);
  static void	bufferMessCallback(void *, t_floatarg val);

  /* lights and colors */
  static void	lightMessCallback(void *, t_floatarg val);
  static void 	colorMessCallback(void *data, t_symbol*, int, t_atom*);
  static void 	clearmaskMessCallback(void *data, t_floatarg bitmask);
  static void 	ambientMessCallback(void *data, t_symbol*, int, t_atom*);
  static void 	specularMessCallback(void *data, t_symbol*, int, t_atom*);
  static void 	shininessMessCallback(void *data, t_floatarg val);
  static void	fogMessCallback(void *, t_symbol *, int argc, t_atom *argv);
  static void 	fogColorMessCallback(void *, t_symbol*, int, t_atom*);
  static void	fogModeMessCallback(void *, t_floatarg val);
};

#endif	// for header file
