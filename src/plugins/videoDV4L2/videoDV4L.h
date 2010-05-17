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
#include "plugins/video.h"

#if defined HAVE_LIBIEC61883 && defined HAVE_LIBRAW1394 && defined HAVE_LIBDV
# define HAVE_DV
#endif

#ifdef HAVE_DV
#include <libraw1394/raw1394.h>
#include <libiec61883/iec61883.h>
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
namespace gem { class GEM_EXTERN videoDV4L : public video {
    public:
        //////////
        // Constructor
    	videoDV4L(void);
    	    	
    	//////////
    	// Destructor
    	virtual ~videoDV4L();
#ifdef HAVE_DV
	////////
	// open the video-device
	virtual bool           openDevice();
	virtual void          closeDevice(void);
    
  //////////
  // Start up the video device
  // [out] int - returns 0 if bad
  bool	    	startTransfer();
	//////////
  // Stop the video device
  // returns TRUE is transfer was running, FALSE is otherwise
  bool	   	stopTransfer();

	//////////
	// get the next frame
	bool grabFrame(void);

  int decodeFrame(unsigned char*, int);
  static int iec_frame(unsigned char *data,int len, int complete, void *arg);

	//////////
	// Set the video dimensions
	virtual int	  setDevice(char*);
	virtual int	  setDevice(int);
	virtual int	  setColor(int);
	virtual int		setQuality(int);
    
 protected:

  //-----------------------------------
  // GROUP:	Linux specific video data
  //-----------------------------------
  int m_dvfd;

  raw1394handle_t m_raw;
  iec61883_dv_fb_t m_iec;

  ////////
  // the DV-decoder
  dv_decoder_t *m_decoder;

  bool m_parsed;
  uint8_t*m_frame[3];
  int m_pitches[3];
#endif /* HAVE_DV */
}; };

#endif	// for header file
