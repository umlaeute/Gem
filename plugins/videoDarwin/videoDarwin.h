/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Copyright (c) 2002 James Tittle & Chris Clepper    
Copyright (c) 2010 IOhannes m zmoelnig. forum::fÃŒr::umlÃ€ute. IEM. zmoelnig@iem.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

Linux version by Miller Puckette. msp@ucsd.edu
	
-----------------------------------------------------------------*/

#ifndef INCLUDE_VIDEODARWIN_H_
#define INCLUDE_VIDEODARWIN_H_

#include "plugins/video.h"

#ifdef HAVE_CARBON
# include <Carbon/Carbon.h>
#endif

#ifdef HAVE_QUICKTIME
# include <QuickTime/QuickTime.h>
#endif

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
	pix_video
    
  captures a video on Apple machines
    
  KEYWORDS
  pix
    
  -----------------------------------------------------------------*/
namespace gem { namespace plugins {
 class GEM_EXPORT videoDarwin : public video {
  public:
    //////////
    // Constructor
    videoDarwin(void);
    	    	
    //////////
    // Destructor
    virtual ~videoDarwin();

#ifdef HAVE_VIDEODARWIN
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
    virtual bool grabFrame(void);

    //////////
    // Set the video dimensions
    virtual bool setDimen(int x, int y, int leftmargin, int rightmargin, int topmargin, int bottommargin);
    virtual bool setQuality(int d);
    virtual bool setColor(int d);
    virtual bool dialog(std::vector<std::string>dlg);
    virtual std::vector<std::string>dialogs(void);

    virtual bool enumProperties(gem::Properties&readable,
                                gem::Properties&writeable);
    virtual void setProperties(gem::Properties&props);
    virtual void getProperties(gem::Properties&props);



    
  protected:
        void initSeqGrabber();
        void resetSeqGrabber();
        void destroySeqGrabber();
        virtual void    setupCapture();



    int		m_newFrame; 
		bool	m_banged;
		bool	m_auto;
		char	m_filename[80];
		int		m_record;
    
    SeqGrabComponent	m_sg;		// Sequence Grabber Component
    SGChannel			m_vc;			// Video Channel
		SGOutput			m_sgout; //output for writing to disk
		Movie				m_movie;
    short				m_pixelDepth;	//
    int					m_vidXSize;		//
    int					m_vidYSize;		//
    Rect				m_srcRect;		// Capture Rectangle
    GWorldPtr			m_srcGWorld;	// Capture Destination
    PixMapHandle		m_pixMap;	// PixMap Handle for capture image
    Ptr					m_baseAddr;		// Base address of pixel Data
    long				m_rowBytes;		// Row bytes in a row
    int					m_quality;
    int					m_colorspace;
		
		int					m_inputDevice;
		int					m_inputDeviceChannel;
		VideoDigitizerComponent			m_vdig; //gotta have the vdig
		VideoDigitizerError	vdigErr;
		DigitizerInfo       m_vdigInfo; //the info about the VDIG
		
		FSSpec		theFSSpec;
		short		nFileRefNum;
		short		nResID;


    virtual std::vector<std::string>enumerate(void);
    std::vector<std::string>m_devices;
    
    bool setIIDCProperty(OSType specifier, double value);

#endif /*HAVE_VIDEODARWIN */
  }; 
};};

#endif	// for header file
