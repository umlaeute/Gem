/*
 *  yuv_videoDarwin.h
 *  gem_darwin
 *
 *  Created by James Tittle & Chris Clepper on Thu Aug 1 2002.
    Copyright (c) 2002 James Tittle & Chris Clepper
 *
 */
#ifndef INCLUDE_YUV_VIDEODARWIN_H_
#define INCLUDE_YUV_VIDEODARWIN_H_

#include <Carbon/Carbon.h>
#include <QuickTime/Quicktime.h>

#include "yuv_video.h"
 
/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
	yuv_video
    
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
class GEM_EXTERN yuv_videoDarwin : public yuv_video
{
    CPPEXTERN_HEADER(yuv_videoDarwin, GemBase)

    public:

        //////////
        // Constructor
    	yuv_videoDarwin(t_floatarg w = 320, t_floatarg h = 240);

    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~yuv_videoDarwin();

    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);

    	//////////
    	// Clear the dirty flag on the pixBlock
    	virtual void 	postrender(GemState *state);

    	//////////
    	virtual void	startRendering();

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

        //-----------------------------------
        // GROUP:	Video data
        //-----------------------------------
    
     	//////////
    	// If video is connected
    	int 	    	m_haveVideo;
    	
    	//////////
    	// The pixBlock with the current image
    	pixBlock    	m_pixBlock;
    	
    	//////////
    	// Should swap the pixels?
    	int 	    	m_swap;
    	int		m_newFrame; 
    	//////////
    	// Do we have to color swap?
    	int 	    	m_colorSwap;
        
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

    private:
    	
    	//////////
    	// static member functions
};

#endif