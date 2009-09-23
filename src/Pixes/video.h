/*-----------------------------------------------------------------

    GEM - Graphics Environment for Multimedia

    Load an video into a pix block

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2003 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_VIDEO_H_
#define INCLUDE_VIDEO_H_

#include "Base/GemConfig.h"
#include "Base/GemPixUtil.h"
#ifndef CLASS_PD
# include "m_pd.h" /* for post() and friends */
#endif

#include <vector>

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
	video
    
	a OS-indendent parent-class for retrieving video-frames
	
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
class GEM_EXTERN video {
 public:
  
  //////////
  // Constructor
  video(int format);
  
  //////////
  // Destructor
  virtual ~video();

  ////////
  // open the video-device
  virtual int            openDevice(int devnum, int format=0);
  virtual void          closeDevice(void);
  virtual int           resetDevice(void);

  //////////
  // Start up the video device (called on startRendering)
  // [out] int - returns 0 if bad
  virtual int	    	startTransfer(int format = 0);
  //////////
  // Stop the video device (called on stopRendering)
  // [out] int - returns 0 if bad
  virtual int	   	stopTransfer();

  //////////
  // get the next frame (called when rendering)
  virtual pixBlock *getFrame() = 0;

  //////////
  // Set the video dimensions
  virtual int	    	setDimen(int x, int y, int leftmargin=0, int rightmargin=0,
				 int topmargin=0, int bottommargin=0);
  virtual int	    	setOffset(int x, int y);
  virtual int	    	setSwap(int state);
  virtual int	    	setChannel(int c, float f=0);
  virtual int	    	setNorm(char*n);
  virtual int	    	setDevice(int d);
  virtual int	    	setDevice(char*name);
  virtual int	    	setColor(int);
  virtual int	    	setQuality(int);

 protected:
  bool m_capturing;
  bool m_haveVideo;
  pixBlock m_image;
  
  int m_width;
  int m_height;
  int m_channel;
  int m_norm;
  int m_reqFormat;

  /* specify either devicename XOR devicenum */  
  char*m_devicename;
  int m_devicenum;

  int m_quality;

 public:
  // for pix_video: query whether this backend provides access to this class of devices
  // (e.g. "dv"
  bool provides(const char*name);
 protected:
  // for child-implementations: remember that we provide access to this class of devices
  void provide(const char*name);
 private:
  // for ourself: the list of provided device-classes
  std::vector<const char*>m_providers;
};

#endif	// for header file
