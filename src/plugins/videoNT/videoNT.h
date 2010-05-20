/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an video into a pix block

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2010 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

Linux version by Miller Puckette. msp@ucsd.edu
	
-----------------------------------------------------------------*/

#ifndef INCLUDE_VIDEONT_H_
#define INCLUDE_VIDEONT_H_

#include "plugins/video.h"

#ifdef HAVE_VFW_H
# include <vfw.h>
#endif

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
	pix_video
    
  captures a video on NT machines
    
  KEYWORDS
  pix
    
  -----------------------------------------------------------------*/
namespace gem { class GEM_EXTERN videoNT : public video {
  public:
    //////////
    // Constructor
    videoNT(void);
    	    	
    //////////
    // Destructor
    virtual ~videoNT();

#ifdef HAVE_VFW_H
    ////////
    // open the video-device
    virtual bool           openDevice(void);
    virtual void          closeDevice(void);
    
    //////////
    // Start up the video device
    // [out] int - returns 0 if bad
    bool startTransfer();
    //////////
    // Stop the video device
    // [out] int - returns 0 if bad
    bool stopTransfer();

    //////////
    // get the next frame
    bool grabFrame(void);

    //////////
    // Set the video dimensions
    virtual bool	    	setDimen(int x, int y, int leftmargin, int rightmargin, int topmargin, int bottommargin);
    virtual bool	    	setColor(int);

  protected:
    HWND		m_hWndC;
    void		videoFrame(LPVIDEOHDR lpVHdr);
  private:
    static void videoFrameCallback(HWND hWnd, LPVIDEOHDR lpVHdr);

#endif /*HAVE_VFW_H */
  }; 
};

#endif	// for header file
