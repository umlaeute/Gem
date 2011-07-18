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

#include "plugins/video.h"

#if defined HAVE_LIBV4L1 || defined HAVE_LINUX_VIDEODEV_H
# define HAVE_VIDEO4LINUX
#endif

#ifdef HAVE_VIDEO4LINUX

# ifdef HAVE_LINUX_VIDEODEV_H
#  include <linux/videodev.h>
# endif /* HAVE_LINUX_VIDEODEV_H */

# ifdef HAVE_LIBV4L1
#  include <libv4l1.h> 
# endif /* HAVE_LIBV4L1 */

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
namespace gem { class GEM_EXPORT videoV4L : public video {
    public:
        //////////
        // Constructor
    	videoV4L(void);
    	    	
    	//////////
    	// Destructor
    	virtual ~videoV4L();

#ifdef HAVE_VIDEO4LINUX
	////////
	// open the video-device
	virtual bool           openDevice(gem::Properties&props);
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

	virtual bool enumProperties(gem::Properties&readable,
				    gem::Properties&writeable);
	virtual void setProperties(gem::Properties&props);
	virtual void getProperties(gem::Properties&props);

	virtual bool	    	setColor(int);

	virtual std::vector<std::string>enumerate(void);
   
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

  unsigned int errorcount;
  
#endif /* HAVE_VIDEO4LINUX */

}; 
};

#endif	// for header file
