/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load a video into a pix block

cgc
-----------------------------------------------------------------*/

#ifndef INCLUDE_yuv_video_H_
#define INCLUDE_yuv_video_H_

#include "Base/GemBase.h"
#include "Base/GemPixUtil.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  yuv_video
    
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
class GEM_EXTERN yuv_video : public GemBase
{
  CPPEXTERN_HEADER(yuv_video, GemBase)

    public:

  //////////
  // Constructor
  yuv_video(t_floatarg nuwm = 0);
    	
 protected:
    	
  //////////
  // Destructor
  virtual ~yuv_video();

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
			  int topmargin = 0 , int bottommargin = 0);

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
  // Stop the video device
  // [out] int - returns 0 if bad
  virtual void	swapMess(int state);
    
  //-----------------------------------
  // GROUP:	Video data
  //-----------------------------------
    
  //////////
  // If video is connected
  int 	    	m_haveVideo;
    	
  //////////
  // The pixBlock with the current image
  pixBlock    	m_pixBlock;
    	
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
};

#endif	// for header file
