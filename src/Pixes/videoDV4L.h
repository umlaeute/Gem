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

#ifndef INCLUDE_VIDEODV4L_H_
#define INCLUDE_VIDEODV4L_H_

#include "Base/config.h"
#ifdef HAVE_LIBDV
#define HAVE_DV
#endif

#include "Pixes/video.h"
#ifdef HAVE_DV
// you will have to add "/usr/src/linux/drivers" to your include-path
#include <ieee1394/dv1394.h>
#include <libdv/dv.h>


#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <pthread.h>
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
class GEM_EXTERN videoDV4L : public video {
    public:
        //////////
        // Constructor
    	videoDV4L(int format=0);
    	    	
    	//////////
    	// Destructor
    	virtual ~videoDV4L();
#ifdef HAVE_DV
	////////
	// open the video-device
	virtual int            openDevice(int devnum, int format=0);
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
	virtual int	    	setDevice(int);
	virtual int	    	setColor(int);
	virtual int		setQuality(int);
    
 protected:

  //-----------------------------------
  // GROUP:	Linux specific video data
  //-----------------------------------
  int dvfd;
  unsigned char *videobuf;
  unsigned char *decodedbuf;
  bool m_frame_ready;
  int  m_frame, m_lastframe;


  //////////
  // the capturing thread
  static void*capturing(void*);
  bool m_continue_thread;
  pthread_t m_thread_id;

  int m_framesize;
  unsigned char *m_mmapbuf;

  ////////
  // the DV-decoder
  dv_decoder_t *m_decoder;
#else
  pixBlock    *getFrame(){return NULL;}
#endif /* HAVE_DV */
};

#endif	// for header file
