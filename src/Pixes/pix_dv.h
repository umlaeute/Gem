/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Capture frames from a the IEEE1394(firewire)-device (DVcam) into a pixBlock
(OS independant parent class)

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
	
-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_DV_H_
#define INCLUDE_PIX_DV_H_

#include "Base/GemBase.h"
#include "Base/GemPixUtil.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_video
    
  Loads in a video
    
  KEYWORDS
  pix
    
  DESCRIPTION
    
  -----------------------------------------------------------------*/
class GEM_EXTERN pix_dv : public GemBase
{
  CPPEXTERN_HEADER(pix_dv, GemBase)

    public:

  //////////
  // Constructor
  pix_dv();
    	
 protected:
    	
  //////////
  // Destructor
  virtual ~pix_dv();

  //////////
  // Do the rendering
  virtual void 	render(GemState *state){}

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
  virtual void 	cleanPixBlock();
  //////////
  // make a pixBlock
  virtual void 	makePixBlock();
  
  //////////
  // Start the grabbing
  virtual int   startTransfer(){
    error("GEM: pix_dv: define DV(IEEE1394) for your OS");
    return 0;
  }

  //////////
  // Stop the grabbing
  virtual int  	stopTransfer(){
    return 0;
}

  int           m_haveVideo;
 
  //////////
  // The pixBlock with the current image
  pixBlock    	m_pixBlock;
  imageStruct   m_imageStruct;

  //////////
  // is it PAL or NTSC ?
  int m_norm;
};

#endif	// for header file
