/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load a video into a pix block

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) G�nther Geiger. geiger@epy.co.at
Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

Linux version by Miller Puckette. msp@ucsd.edu

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_VIDEO_H_
#define INCLUDE_PIX_VIDEO_H_
#include "Base/config.h"

#if defined __VIDEO__NEW  || defined HAVE_DIRECTSHOW
# ifndef DO_AUTO_REGISTER_CLASS
#  define NO_AUTO_REGISTER_CLASS
# endif
#endif

#include "Base/GemBase.h"
#include "Base/GemPixUtil.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_video
    
  Loads in an video
    
  KEYWORDS
  pix
    
  DESCRIPTION

  "dimen" (int, int) - set the x,y dimensions
  "zoom" (int, int) - the zoom factor (1.0 is nominal) (num / denom)
  "bright" (int) - the brightnes
  "contrast" (int) - the contrast
  "hue" (int) - the hue
  "sat" (int) - the saturation
    
  -----------------------------------------------------------------*/
class GEM_EXTERN pix_video : public GemBase
{
  CPPEXTERN_HEADER(pix_video, GemBase)

    public:

  //////////
  // Constructor
  pix_video(t_floatarg num = 0);
    	
 protected:
    	
  //////////
  // Destructor
  virtual ~pix_video();

  //////////
  // Do the rendering
  virtual void 	render(GemState *state);

  //////////
  // Clear the dirty flag on the pixBlock
  virtual void 	postrender(GemState *state);

  //////////
  virtual void	startRendering();

  //////////
  // If you care about the stop of rendering
  virtual void	stopRendering();
    	
  //////////
  // Clean up the pixBlock
  void	    	cleanPixBlock();
    
  //////////
  // Set the video dimensions
  virtual void	dimenMess(int x, int y, int leftmargin = 0, int rightmargin = 0 ,
			  int topmargin = 0 , int bottommargin = 0) {}

  //////////
  // Set the video offset
  virtual void	offsetMess(int x, int y);
    
  //////////
  // Start up the video device
  // [out] int - returns 0 if bad
  virtual int	startTransfer();
    
  //////////
  // Stop the video device
  // [out] int - returns 0 if bad
  virtual int	stopTransfer();
    
  ////////// 
  // swap the image (upside down ?)
  virtual void	swapMess(int state);

  ////////// 
  // enumerate the devices
  virtual void	enumerateMess();
  ////////// 
  // colorspace-message
  virtual void	csMess(int format);
  ////////// 
  // property-dialog
  virtual void	dialogMess(int,t_atom*);

  //-----------------------------------
  // GROUP:	Video data
  //-----------------------------------
    
  //////////
  // If video is connected
  int 	    	m_haveVideo;
    	
  //////////
  // The pixBlock with the current image
  pixBlock    	m_pixBlock;
  imageStruct   m_imageStruct;
    	
  //////////
  // Should swap the pixels?
  int 	    	m_swap;
    	 
  //////////
  // Do we have to color swap?
  int 	    	m_colorSwap;
    	
 private:
    	
  //////////
  // static member functions

  static void dimenMessCallback(void *data, t_symbol *s, int ac, t_atom *av);
  static void offsetMessCallback(void *data, t_floatarg x, t_floatarg y);
  static void swapMessCallback(void *data, t_floatarg state);

  /* dummy callbacks; won't do anything, but are here for future compatibility */
  static void dialogMessCallback(void *data, t_symbol*,int,t_atom*);
  static void enumerateMessCallback(void *data);
  static void csMessCallback(void *data, t_symbol*colorspace);
};

#endif	// for header file
