/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Base Class for Video Capture Plugins

Copyright (c) 2010 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_VIDEOGRABBER_H_
#define INCLUDE_VIDEOGRABBER_H_

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
	videoGrabber
    
	a OS-indendent class for retrieving video-frames
	(using a separate grabbing thread if possible)

  this object is a frontend for other video-plugins
  
	
  KEYWORDS
	pix
    
  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem { class GEM_EXTERN videoGrabber {
  private:
    class PIMPL;
    PIMPL*m_pimpl;

  public:
  
    //////////
    // Constructor
    // 
    videoGrabber(void);
  
    //////////
    // Destructor
    virtual ~videoGrabber(void);

    virtual bool open(gem::Properties&props, std::string backend=std::string());
    virtual bool close();

    //! Start up the video device (called on startRendering)
    /* \return FALSE is something failed, TRUE otherwise
     */
    virtual bool	    	start(void);

    //! Stop the videoGrabber device (called on stopRendering)
    /* \return TRUE if a transfer was going on, FALSE if the transfer was already stopped
     */
    virtual bool	   	stop();

    //! get the next frame (called when rendering)
    /* grab the next frame from the device
     * if no new frame is available, this should set the "newimage" flag to false
     * this gets called from the main thread, 
     * and will either read the frame from the grabbing thread (if there is one; locking mutex #0), 
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

    /** turn on/off "asynchronous"-grabbing
     * default is "true"
     * "asynchronous" means, that the device is constantly grabbing, and grabFrame() returns the current frame
     * non-"continous" means, that the device will only issue a new grab when a frame has read
     *   (thus potentially reducing the CPU-load to what is needed, at the cost of slightly outdated images
     * returns: the old state
     */
    bool grabAsynchronous(bool);

  protected:
  };};


#endif	// for header file
