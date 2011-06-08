/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Base Class for Video Capture Plugins

Copyright (c) 2010 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_VIDEOTHREADED_H_
#define INCLUDE_VIDEOTHREADED_H_

#include "video.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
	videoThreaded
    
	a OS-indendent parent-class for retrieving video-frames
	(using a separate grabbing thread)
	
  KEYWORDS
	pix
    
  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem { class GEM_EXTERN videoThreaded : public video {
  public:
  
    //////////
    // Constructor
    // if numlocks>0 we will use a thread to capture the image create <numlocks> mutexes
    // 
    videoThreaded(const std::string name, unsigned int numlocks=1, unsigned int timeout=0);
  
    //////////
    // Destructor
    virtual ~videoThreaded(void);

    //! Start up the video device (called on startRendering)
    /* \return FALSE is something failed, TRUE otherwise
     */
    virtual bool	    	startTransfer(void);

    //! Stop the videoThreaded device (called on stopRendering)
    /* \return TRUE if a transfer was going on, FALSE if the transfer was already stopped
     */
    virtual bool	   	stopTransfer();


    //! Stops the videoThreaded device and if it was running restarts it
    /* \return the return code of startTransfer()
     */
    virtual bool	   	restartTransfer();

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


    /* starts the thread that will call grabFrame() (if threads are requested in the ctor) */
    bool startThread(void);
    /* stops the thread; waits at most "timeout" microseconds; if 0 waits forever; if -1 waits for time specified in ctor */
    bool stopThread(int timeout=-1);

    /* locks the mutex #<id>; 
     * if the mutex does not exist (e.g. no threading), this will simply return
     * the default mutex #0 is locked by default in the getFrame() to protect the m_image ressource
     */
    void lock(unsigned int id=0);
    /* unlocks the mutex #<id>; 
     * if the mutex does not exist (e.g. no threading), this will simply return
     * the default mutex #0 is locked by default in the getFrame() to protect the m_image ressource
     */
    void unlock(unsigned int id=0);

    /* sleep a selected time in usec
     * convenience wrapper around select()
     */
    void usleep(unsigned long usec);



    /** turn on/off "asynchronous"-grabbing
     * default is "true"
     * "asynchronous" means, that the device is constantly grabbing, and grabFrame() returns the current frame
     * non-"continous" means, that the device will only issue a new grab when a frame has read
     *   (thus potentially reducing the CPU-load to what is needed, at the cost of slightly outdated images
     * returns: the old state
     */
    bool grabAsynchronous(bool);

  protected:

  private:
    class PIMPL;
    PIMPL*m_pimpl;
  };};


#endif	// for header file
