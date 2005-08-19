/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  Convert 4 signals (R~, G~, B~, A¨) into a pixBuf

  Copyright (c) 2000 Günther Geiger. geiger@epy.co.at
  Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_SIG2PIX_H_
#define INCLUDE_PIX_SIG2PIX_H_

#include <string.h>

#include "Base/GemBase.h"
#include "Base/GemPixUtil.h"
#include "Base/GemPixImageLoad.h"
#include "Base/GemCache.h"
#include <math.h>

#ifdef unix
#include <unistd.h>
#include <strings.h>
#endif

#ifdef __ppc__
#include "Base/GemFuncUtil.h"
#undef sqrt
#define sqrt fast_sqrtf
#endif

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_sig2pix
    
  Interprete 4 signals as RGBA-Values of a pixel-image
    
  KEYWORDS
  pix
    
  DESCRIPTION

  "dimen" -- change pix_buf dimension
  4 inlets eat signals : R~, G~, B~, A~
  creation: width, height in pixels
    
  -----------------------------------------------------------------*/
class GEM_EXTERN pix_sig2pix : public GemBase
{
  CPPEXTERN_HEADER(pix_sig2pix, GemBase)

    public:

  //////////
  // Constructor
  pix_sig2pix(t_floatarg width, t_floatarg height);
    	
 protected:
    	
  //////////
  // Destructor
  virtual      ~pix_sig2pix();

  //////////
  
  // DSP perform
  static t_int* perform(t_int* w);

  //////////
  // Do the rendering
  virtual void 	render(GemState *state);

  //////////
  // Clear the dirty flag on the pixBlock
  virtual void 	postrender(GemState *state);

  //////////
  virtual void	startRendering();

  //////////
  // setting dimension and colourspace
  virtual void  dimenMess(int w, int h);
  virtual void  csMess(GLint cs);

  //////////
  // DSP-Message
  virtual void  dspMess(void *data, t_signal** sp);

  //-----------------------------------
  // GROUP:	Image data
  //-----------------------------------
    
  //////////
  // The pixBlock with the current image
  pixBlock    	m_pixBlock;
  imageStruct   m_imageStruct;
  int           m_pixsize;

  int           m_width, m_height;

  //////////////
  // which colorspace do we want ? currently only GL_RGBA
  GLint         m_reqFormat;

 private:
    	
  //////////
  // static member functions
  static void   dspMessCallback(void* data,t_signal** sp);
  static void   dimenMessCallback(void *data, t_float w, t_float h);
  static void   csMessCallback(void *data, t_symbol*s);
};

#endif	// for header file
