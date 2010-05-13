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

#include "Base/GemPixUtil.h"

#include <string>
#include "plugins/PluginFactory.h"

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
namespace gem { class GEM_EXTERN video {
 public:
  
  //////////
  // Constructor
  video(bool threaded=true);
  
  //////////
  // Destructor
  virtual ~video(void);

  //! reset the backend, possibly re-enumerating devices
  virtual bool          reset(void);

  //! open the video-device
  /* the exact video device is either the default one or has to be set prior with setDevice()
   * the device need not start streaming yet
   * this comes in handy when determining the correct backend for a certain device
   * \return TRUE when we successfully opened the device and can startTransfer immediately
   */
  virtual bool           openDevice(void);

  //! close the video device, freeing all ressources
  /* once the device has been closed it should be useable by other applications,...
   * this get's called when switching to another backend or when deleting the object
   */
  virtual void          closeDevice(void);

  //! Start up the video device (called on startRendering)
  /* \return FALSE is something failed, TRUE otherwise
   */
  virtual bool	    	startTransfer(void);

  //! Stop the video device (called on stopRendering)
  /* \return TRUE if a transfer was going on, FALSE if the transfer was already stopped
   */
  virtual bool	   	stopTransfer();


  //! Stops the video device and if it was running restarts it
  /* \return the return code of startTransfer()
   */
  virtual bool	   	restartTransfer();

  //! grab a frame (work-horse)
  /* this will be run in a separate thread (if threading is enabled)
   * makes the data accessible in the "m_image" struct!
   * access to m_image MUST be protected by lock() and unlock()
   */
  virtual bool grabFrame(void){/* lock(); m_image.image.data=NULL; unlock(); */};
  

  //! get the next frame (called when rendering)
  /* grab the next frame from the device
   * if no new frame is available, this should set the "newimage" flag to false
   * this gets called from the main thread, 
   * and will either read the frame from the grabbing thread (if there is one), 
   * or directly call grabFrame()
   * you shouldn't need to override this
   * \return the new frame or NULL on error
   */
  virtual pixBlock *getFrame(void);

  //! release a frame (after use)
  /* this gets called at postRender()
   * is you are using DMA or the like, now is the time to release the ressource
   */
  virtual void releaseFrame(void);


  
  bool startThread(void);
  bool stopThread(unsigned int timeout=0);

  void lock(void);
  void unlock(void);





  //////////////////////
  // device settings


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
  //! indicates valid transfer
  /* set this to TRUE in a successfull startTransfer() and to FALSE at stopTransfer() */
  bool m_capturing;
  //! indicates valid device
  /* set this to TRUE after a successfull openDevice(), and to FALSE at closeDevice */
  bool m_haveVideo;
  //! a place to store the image with grabFrame()
  pixBlock m_image;
  
  unsigned int m_width;
  unsigned int m_height;
  int m_channel;
  int m_norm;
  int m_reqFormat;

  /* specify either devicename XOR devicenum */  
  const char*m_devicename;
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

  class PIMPL;
  PIMPL*m_pimpl;
};};

/**
 * \fn REGISTER_VIDEOFACTORY(const char *id, Class videoClass)
 * registers a new class "videoClass" with the video-factory
 *
 * \param id a symbolic (const char*) ID for the given class
 * \param videoClass a class derived from "video"
 */
#define REGISTER_VIDEOFACTORY(id, TYP) static gem::PluginFactoryRegistrar::registrar<TYP, gem::video> fac_video_ ## TYP (gensym(id)->s_name)


/**
 * \fn INIT_VIDEOFACTORY()
 * initialized the factory
 * \note call this before any externals register themselves
 */
#define INIT_VIDEOFACTORY() \
  template<>gem::PluginFactory<gem::video>*gem::PluginFactory<gem::video>::s_factory=NULL; \
  static gem::PluginFactoryRegistrar::dummy<gem::video> fac_videodummy


#endif	// for header file
