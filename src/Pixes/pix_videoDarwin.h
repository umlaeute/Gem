/*
 *  pix_videoDarwin.h
 *  gem_darwin
 *
 *  Created by James Tittle & Chris Clepper on Thu Aug 1 2002.
    Copyright (c) 2002 James Tittle & Chris Clepper
 *
 */

 
#ifndef INCLUDE_PIX_VIDEODARWIN_H_
#define INCLUDE_PIX_VIDEODARWIN_H_

#include <Carbon/Carbon.h>
#include <QuickTime/Quicktime.h>

#include "pix_video.h"
 
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
    "bright" (int) - the brightness
    "contrast" (int) - the contrast
    "hue" (int) - the hue
    "sat" (int) - the saturation
    
-----------------------------------------------------------------*/
class GEM_EXTERN pix_videoDarwin : public pix_video
{
    CPPEXTERN_HEADER(pix_videoDarwin, GemBase)

    public:

        //////////
        // Constructor
    	pix_videoDarwin(t_floatarg w, t_floatarg h );

    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_videoDarwin();

    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);

    	//////////
    	// Clear the dirty flag on the pixBlock
    	virtual void 	postrender(GemState *state);

    	//////////
    	virtual void	startRendering();
        
          //////////
          // Start up the video device
          // [out] int - returns 0 if bad
        virtual int	startTransfer();

        //////////
        // Stop the video device
        // [out] int - returns 0 if bad
        virtual int	stopTransfer();

        virtual void csMess(int format);

        //////////
        // property-dialog
        virtual void	dialogMess(int,t_atom*);
  
	//-----------------------------------
	// GROUP:	Macintosh specific video data
	//-----------------------------------
        void			tick();
        void			panelMess();
        void			makeOffscreen();
        void			makeVideoChannel();
        void			setupVideoChannel();
        void			disposeOffscreen();
        void			disposeVideoChannel();
        //OSErr	videoFrame(SGChannel c, short bufferNum, Boolean *done);
        
        void InitSeqGrabber();
        void resetSeqGrabber();
        void destroySeqGrabber();
        void DoVideoSettings();
        static pascal Boolean SeqGrabberModalFilterProc (DialogPtr theDialog, const EventRecord *theEvent, short *itemHit, long refCon); 
        void	    	dimenMess(int x, int y, int leftmargin, int rightmargin,
    	    	    	    int topmargin, int bottommargin);

        //-----------------------------------
        // GROUP:	Video data
        //-----------------------------------
    
    	int		m_newFrame; 
        
        SeqGrabComponent	m_sg;		// Sequence Grabber Component
        SGChannel		m_vc;			// Video Channel
        short			m_pixelDepth;	//
        int			m_vidXSize;		//
        int			m_vidYSize;		//
        Rect			m_srcRect;		// Capture Rectangle
        GWorldPtr		m_srcGWorld;	// Capture Destination
        PixMapHandle		m_pixMap;	// PixMap Handle for capture image
        Ptr			m_baseAddr;		// Base address of pixel Data
        long			m_rowBytes;		// Row bytes in a row
        int			m_quality;
        int			m_colorspace;
    private:
    	
    	//////////
    	// static member functions
        static void qualityCallback(void *data, t_floatarg X);
        static void resetCallback(void *data);
        static void dialogCallback(void *data);
        static void colorspaceCallback(void *data, t_symbol *cs);
        static void csMessCallback(void *data, t_symbol *cs);
};

#endif
