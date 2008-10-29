/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A video class which uses an SGI Indycam

    Copyright (c) 1997-1998 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_INDYCAM_H_
#define INCLUDE_PIX_INDYCAM_H_

#include "Pixes/pix_videoOS.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    video
    
    Loads in an video
    
KEYWORDS
    pix
    
DESCRIPTION

    "zoom" (int, int) - the zoom factor (1.0 is nominal) (num / denom)
    "bright" (int) - the brightnes
    "contrast" (int) - the contrast
    "hue" (int) - the hue
    "sat" (int) - the saturation
    
-----------------------------------------------------------------*/
class pix_indycam : public pix_videoOS
{
    CPPEXTERN_HEADER(pix_indycam, pix_videoOS)

    public:

	//////////
	// Constructor
    	pix_indycam();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_indycam();

	    //-----------------------------------
	    // GROUP:	Control functions
	    //-----------------------------------

    	//////////
    	// Set the video zoom factor
    	void	    	zoomMess(int num, int denom);
    
    	//////////
    	// Set the video brightness
    	void	    	brightMess(int val);
    
    	//////////
    	// Set the video contrast
    	void	    	contrastMess(int val);
    
    	//////////
    	// Set the video hue
    	void	    	hueMess(int val);
    
    	//////////
    	// Set the video sat
    	void	    	satMess(int val);
    
    private:
    	
    	//////////
    	// static member functions
    	static void	zoomMessCallback(void *data, t_floatarg num, t_floatarg denum);
    	static void	brightMessCallback(void *data, t_floatarg val);
    	static void	contrastMessCallback(void *data, t_floatarg val);
    	static void	hueMessCallback(void *data, t_floatarg val);
    	static void	satMessCallback(void *data, t_floatarg val);
};

#endif	// for header file
