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

#ifndef INCLUDE_PIX_VIDEOSGI_H_
#define INCLUDE_PIX_VIDEOSGI_H_

#include <vl/vl.h>

#include "Pixes/pix_video.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_video
    
    Loads in an video
    
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
class GEM_EXTERN pix_video : public GemBase
{
    CPPEXTERN_HEADER(pix_video, GemBase)

    public:

        //////////
        // Constructor
    	pix_video();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_video();

    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);

    	//////////
    	// Clear the dirty flag on the pixBlock
    	virtual void 	postrender(GemState *state);

    	//////////
    	virtual void	startRendering();

    	//////////
    	// If you care about the stop of rendering
    	virtual void	stopRendering();
    	
    	//////////
    	// Clean up the pixBlock
    	void	    	cleanPixBlock();
    
    	//////////
    	// Set the video dimensions
    	void	    	dimenMess(int x, int y);

    	//////////
    	// Set the video offset
    	void	    	offsetMess(int x, int y);
    
    	//////////
    	// Start up the video device
    	// [out] int - returns 0 if bad
    	int	    		startTransfer();
    
    	//////////
    	// Stop the video device
    	// [out] int - returns 0 if bad
    	int	    		stopTransfer();
    
    	////////// 
    	// Stop the video device
    	// [out] int - returns 0 if bad
    	void	    	swapMess(int state);
    
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
    	 
    	//////////
    	// Do we have to color swap?
    	int 	    	m_colorSwap;
    	
		VLServer    	m_svr;
		VLPath      	m_path;
		VLNode      	m_src;
		VLNode	    	m_drn;
		VLBuffer    	m_buffer;

    private:
    	
    	//////////
    	// static member functions

    	static void dimenMessCallback(void *data, t_floatarg x, t_floatarg y);
    	static void offsetMessCallback(void *data, t_floatarg x, t_floatarg y);
    	static void swapMessCallback(void *data, t_floatarg state);
};

#endif	// for header file
