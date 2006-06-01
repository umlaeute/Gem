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

#ifndef INCLUDE_VIDEOV4L_H_
#define INCLUDE_VIDEOV4L_H_

#include "Pixes/video.h"

#ifdef HAVE_VIDEO4LINUX
# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
# include <unistd.h>
# include <string.h>
# include <ctype.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/ioctl.h>
# include <sys/types.h>
# include <sys/time.h>
# include <linux/types.h>
# include <linux/videodev.h>
# include <sys/mman.h>
//#ifdef HAVE_PTHREADS
# include <pthread.h>
//#endif
# define V4L_DEVICENO 0
# define V4L_NBUF 2
# define V4L_COMPOSITEIN 1
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
class GEM_EXTERN videoV4L : public video {
    public:
        //////////
        // Constructor
    	videoV4L(int format=0);
    	    	
    	//////////
    	// Destructor
    	virtual ~videoV4L();

#ifdef HAVE_VIDEO4LINUX
	////////
	// open the video-device
	virtual int            openDevice(int devnum, int format=0){return 1;}
	virtual void          closeDevice(void){}
	virtual int           resetDevice(void){return 1;}
    
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
  	virtual int	    	setDimen(int x, int y, int leftmargin, int rightmargin,
					 int topmargin, int bottommargin);
	virtual int	    	setChannel(int c, float f);
	virtual int	    	setNorm(char*);
	virtual int	    	setDevice(int);
	virtual int	    	setDevice(char*);
	virtual int	    	setColor(int);

    
 protected:

  //-----------------------------------
  // GROUP:	Linux specific video data
  //-----------------------------------
	
  struct video_tuner vtuner;
  struct video_picture vpicture;
  struct video_buffer vbuffer;
  struct video_capability vcap;
  struct video_channel vchannel;
  struct video_audio vaudio;
  struct video_mbuf vmbuf;
  struct video_mmap vmmap[V4L_NBUF];
  int tvfd;
  int frame, last_frame;
  unsigned char *videobuf;
  int skipnext;
  int mytopmargin, mybottommargin;
  int myleftmargin, myrightmargin;

  int m_gotFormat; // the format returned by the v4l-device (not an openGL-format!)
  bool m_colorConvert; // do we have to convert the colour-space manually ?
  
  //////////
  // the capturing thread
  pthread_t m_thread_id;
  bool      m_continue_thread;
  bool      m_frame_ready;
  static void*capturing(void*);

  // rendering might be needed when we are currently not capturing because we cannot (e.g: couldn't open device)
  // although we should. when reopening another device, we might be able to render again...
  // example: we have only 1 video-device /dev/video0;
  // when we try to open /dev/video1 we fail, and m_capturing is set to 0
  // now when rendering is turned on and we want to switch back to /dev/video0 we should reconnect to the good device
  bool      m_rendering; // "true" when rendering is on, false otherwise
  
#endif /* HAVE_VIDEO4LINUX */

};

#endif	// for header file
