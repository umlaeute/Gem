/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Base Class for Video Capture Plugins

Copyright (c) 2010 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_VIDEO_H_
#define INCLUDE_VIDEO_H_

#include "Base/GemPixUtil.h"

#include <string>
#include "plugins/PluginFactory.h"

#include "Base/Properties.h"

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
    // if numlocks>0 we will use a thread to capture the image create <numlocks> mutexes
    // 
    video(const std::string name, unsigned int numlocks=1, unsigned int timeout=0);
  
    //////////
    // Destructor
    virtual ~video(void);

    //! open the device (calls openDevice())
    bool open(void);
    //! close the device (calls closeDevice())
    void close(void);
    //! start the transmission (calls startTransfer())
    bool start(void);
    //! stop the transmission (calls stopTransfer())
    bool stop(void);

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
    virtual bool grabFrame(void){/* lock(); m_image.image.data=NULL; unlock(); */ return false; };
  

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

    //////////////////////
    // device settings

    //////////
    // changes the device
    // after the device has been set, the caller has to restart 
    // (close() the current handle, try open() with the new settings)
    // normally you shouldn't need to override these
    virtual bool	    	setDevice(int d);
    virtual bool	    	setDevice(const std::string);


    //////////
    // Set the video dimensions and other stuff
    // implement what's possible
    virtual bool	    	setDimen(int x, int y, int leftmargin=0, int rightmargin=0,
                                 int topmargin=0, int bottommargin=0);

    virtual bool	    	setChannel(int c, float f=0);
    virtual bool	    	setNorm(const std::string);
    virtual bool	    	setColor(int);
    virtual bool	    	setQuality(int);

    // set stuff via a system dialog (highly system specific)
    // implement if the system provides an API for this
    virtual bool	    	dialog(std::vector<std::string>names=std::vector<std::string>());

    // provide a list of devices this backend can handle */
    virtual std::vector<std::string>enumerate(void);

    // get's the name of the backend (e.g. "v4l")
    const std::string getName(void);

  protected:
    //! indicates valid transfer (automatically set in start()/stop())
    bool m_capturing;
    //! indicates valid device (automatically set in open()/close())
    bool m_haveVideo;
    //! a place to store the image with grabFrame()
    pixBlock m_image;
  
    unsigned int m_width;
    unsigned int m_height;
    int m_channel;
    float m_frequency;
    int m_norm;
    int m_reqFormat;

    /* specify either devicename XOR devicenum */  
    std::string m_devicename;
    int m_devicenum;

    int m_quality;

  public:
    // for pix_video: query whether this backend provides access to this class of devices
    // (e.g. "dv"
    bool provides(const std::string);
    // get a list of all provided devices
    std::vector<std::string>provides(void);

    /**
     * list all properties the currently opened device supports
     * after calling, "readable" will hold a list of all properties that can be read
     * and "writeable" will hold a list of all properties that can be set
     * if the enumeration fails, this returns <code>false</code>
     */
    virtual bool enumProperties(std::vector<std::string>&readable,std::vector<std::string>&writeable);

    /**
     * set a number of properties (as defined by "props")
     * the "props" may hold properties not supported by the currently opened device,
     *  which is legal; in this case the superfluous properties are simply ignored
     * this function MAY modify the props; 
     * namely one-shot properties (e.g. "do-white-balance-now") should be removed from the props
     */
    virtual void setProperties(gem::Properties&props);

    /**
     * get the current value of the given properties from the device
     * if props holds properties that can not be read from the device, they are set to UNSET 
     */
    virtual void getProperties(gem::Properties&props);

  protected:
    // for child-implementations: remember that we provide access to this class of devices
    void provide(const std::string);
  private:
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
#define INIT_VIDEOFACTORY()                                             \
  static gem::PluginFactoryRegistrar::dummy<gem::video> fac_videodummy


#endif	// for header file
