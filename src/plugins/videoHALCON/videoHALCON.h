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

#ifndef INCLUDE_VIDEOHALCON_H_
#define INCLUDE_VIDEOHALCON_H_

#include "plugins/video.h"

#if defined HAVE_LIBHALCON
# define HAVE_HALCON
#endif

#ifdef HAVE_HALCON
#  include <libv4l1.h> 
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
namespace gem { class GEM_EXTERN videoHALCON : public video {
    public:
        //////////
        // Constructor
    	videoHALCON(void);
    	    	
    	//////////
    	// Destructor
    	virtual ~videoHALCON();

#ifdef HAVE_HALCON
	////////
	// open the video-device
	virtual bool           openDevice(void);
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

	//////////
	// Set the video dimensions
  virtual bool	    	setDimen(int x, int y, int leftmargin, int rightmargin, int topmargin, int bottommargin);
	virtual bool	    	setChannel(int c, float f);
	virtual bool	    	setNorm(const std::string);
	virtual bool	    	setColor(int);

  virtual std::vector<std::string>enumerate(void);
   
 protected:

#endif /* HAVE_HALCON */

}; 
};

#endif	// for header file
