/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Capture frames from a the IEEE1394(firewire)-device (DVcam) into a pixBlock (Linux)

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
	
-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_DVLINUX_H_
#define INCLUDE_PIX_DVLINUX_H_

#define HAVE_FIREWIRE

#include "pix_dv.h"
#include <quicktime/dv.h>

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_video
    
  Loads in a video
    
  KEYWORDS
  pix
    
  DESCRIPTION
    
  -----------------------------------------------------------------*/
class GEM_EXTERN pix_dvLinux : public pix_dv
{
  CPPEXTERN_HEADER(pix_dvLinux, GemBase)

    public:

  //////////
  // Constructor
  pix_dvLinux();
	
 protected:
    	
  //////////
  // Destructor
  virtual ~pix_dvLinux();

  //////////
  // Do the rendering
  virtual void 	render(GemState *state);
  	
  //////////
  // Start the grabbing
  virtual int	    	startTransfer();

  //////////
  // Stop the grabbing
  virtual int	   	stopTransfer();

  //-----------------------------------
  // GROUP:	Linux specific video data
  //-----------------------------------	

  dv_grabber_t *m_grabber;
  dv_t         *m_dv;

  int m_port;
  int m_channel;
 
 private:
    	
  //////////
  // static member functions

  static void channelMessCallback(void *data, t_floatarg f);
  static void portMessCallback   (void *data, t_floatarg f);
};

#endif	// for header file
