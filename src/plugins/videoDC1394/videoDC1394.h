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

#ifndef INCLUDE_VIDEODC1394_H_
#define INCLUDE_VIDEODC1394_H_
#include "plugins/video.h"


#ifdef HAVE_LIBDC1394
#include "dc1394/dc1394.h"
#endif

#ifdef HAVE_PTHREAD
# include <pthread.h>
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
namespace gem { class GEM_EXTERN videoDC1394 : public video {
    public:
        //////////
        // Constructor
    	videoDC1394(void);
    	    	
    	//////////
    	// Destructor
    	virtual ~videoDC1394();
#ifdef HAVE_LIBDC1394
	////////
	// open the video-device
	virtual int            openDevice(int format=0);
	virtual void          closeDevice(void);
	virtual int           resetDevice(void);
    
    	//////////
    	// Start up the video device
    	// [out] int - returns 0 if bad
    	int	    	startTransfer(int format=0);
	//////////
    	// Stop the video device
    	// [out] int - returns 0 if bad
    	int	   	stopTransfer();

	//////////
	// get the next frame
	pixBlock    *getFrame();

	//////////
	// Set the video dimensions
	virtual int	    	setNorm(char*);
	virtual int	    	setDevice(char*);
	virtual int	    	setDevice(int);
	virtual int	    	setColor(int);
	virtual int		setQuality(int);
    
 protected:

  //-----------------------------------
  // GROUP:	Linux specific video data
  //-----------------------------------


  //////////
  // the capturing thread
  void captureThread(void);
  static void*capturing(void*);
  bool m_continue_thread;
  pthread_t m_thread_id;
  pthread_mutex_t*m_lock;

  dc1394camera_t * m_dccamera;

  dc1394video_frame_t * m_dcframe;
  dc1394_t * m_dc;

  imageStruct m_frame;

#else
  pixBlock    *getFrame(){return NULL;}
#endif /* HAVE_LIBDC1394 */
}; };

#endif	// for header file
