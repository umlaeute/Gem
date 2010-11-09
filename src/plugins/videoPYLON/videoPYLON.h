/*-----------------------------------------------------------------

    GEM - Graphics Environment for Multimedia

    Load an video into a pix block

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

    Linux version by Miller Puckette. msp@ucsd.edu
	
-----------------------------------------------------------------*/

#ifndef INCLUDE_VIDEOPYLON_H_
#define INCLUDE_VIDEOPYLON_H_

#include "plugins/video.h"

#if defined HAVE_LIBPYLON
# define HAVE_PYLON
#endif

#ifdef Status
/* ouch: Xlib.h defines "Status" as "int", but Pylon uses "Status" as a
 * variable name
 */
# undef Status
#endif
#ifdef None
# undef None
#endif

#ifdef HAVE_PYLON
# include "pylon/PylonIncludes.h"
# include <pylon/gige/BaslerGigECamera.h>
typedef Pylon::CBaslerGigECamera Camera_t;
#endif
/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
	pix_video
    
    Loads in a video
    
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
namespace gem { class GEM_EXPORT videoPYLON : public video {
    public:
    //////////
    // Constructor
    videoPYLON(void);
    	    	
    //////////
    // Destructor
    virtual ~videoPYLON();

#ifdef HAVE_PYLON
    ////////
    // open the video-device
    virtual bool           openDevice(gem::Properties&writeprops);
    virtual void          closeDevice(void);
    
    //////////
    // Start up the video device
    // [out] int - returns 0 if bad
    bool	    	startTransfer();
    //////////
    // Stop the video device
    // [out] int - returns 0 if bad
    bool	   	stopTransfer();
    
    //////////
    // get the next frame
    bool grabFrame();

    virtual std::vector<std::string>enumerate(void);



  virtual bool enumProperties(gem::Properties&readable,
			      gem::Properties&writeable);
  virtual void setProperties(gem::Properties&writeprops);
  virtual void getProperties(gem::Properties&readprops);

   
  protected:
  class CGrabBuffer;

  Pylon::PylonAutoInitTerm autoInitTerm;
  Pylon::CTlFactory*m_factory;
    
  Pylon::CBaslerGigECamera*m_camera;
  Pylon::CBaslerGigEStreamGrabber*m_grabber;
#ifdef HAVE_LIBPYLONUTILITY
  Pylon::CPixelFormatConverter*m_converter;
#endif

  uint32_t m_numBuffers;
  std::vector<CGrabBuffer*> m_buffers;
  std::map<std::string, Pylon::CDeviceInfo>m_id2device;
#endif /* HAVE_PYLON */

}; 
};

#endif	// for header file
