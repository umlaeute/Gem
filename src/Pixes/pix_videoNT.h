/*-----------------------------------------------------------------

    GEM - Graphics Environment for Multimedia

    Load an video into a pix block

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_VIDEONT_H_
#define INCLUDE_PIX_VIDEONT_H_

#include "Pixes/pix_videoOS.h"
#include <vfw.h>

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
class GEM_EXTERN pix_videoNT : public pix_videoOS
{
    CPPEXTERN_HEADER(pix_videoNT, GemBase)

    public:

        //////////
        // Constructor
    	pix_videoNT(t_floatarg num);

    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_videoNT();

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
    	// Start up the video device
    	// [out] int - returns 0 if bad
    	int	    	startTransfer();
    
    	//////////
    	// Stop the video device
    	// [out] int - returns 0 if bad
    	int	    	stopTransfer();
    
    	////////// 
    	// set the colorspace
    	void	    	csMess(int state);

	//-----------------------------------
	// GROUP:	Video data
	//-----------------------------------

	HWND		m_hWndC;
	void		videoFrame(LPVIDEOHDR lpVHdr);
	int		m_vidXSize;
	int		m_vidYSize;
	int		m_newFrame;

    private:
    	
    	//////////
    	// static member functions

    	static void dimenMessCallback(void *data, t_floatarg x, t_floatarg y);
    	static void csMessCallback(void *data, t_symbol*);

	static void videoFrameCallback(HWND hWnd, LPVIDEOHDR lpVHdr);
};

#endif	// for header file
