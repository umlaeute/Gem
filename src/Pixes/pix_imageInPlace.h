/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Load multiple images into a pix block and texture immediately

    Copyright (c) 1997-1999 Mark Danks
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_IMAGEINPLACE_H_
#define INCLUDE_PIX_IMAGEINPLACE_H_

#include "Pixes/pix_multiimage.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_imageInPlace
    
    Load multiple images into a pix block and texture immediately
    
KEYWORDS
    pix
    
DESCRIPTION

    You can select which file by giving a number.

-----------------------------------------------------------------*/
class GEM_EXTERN pix_imageInPlace : public pix_multiimage
{
    CPPEXTERN_HEADER(pix_imageInPlace, pix_multiimage)

    public:

	    //////////
	    // Constructor
    	pix_imageInPlace(t_symbol *filename, t_floatarg baseImage, t_floatarg topImage, t_floatarg skipRate);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_imageInPlace();

    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);

    	//////////
    	// Do the rendering
    	virtual void 	postrender(GemState *state);

    	//////////
    	virtual void	startRendering();

    	//////////
    	virtual void	stopRendering();

    	//////////
    	// When a preload is received
    	virtual void	preloadMess(t_symbol *filename, int baseImage, int topImage, int skipRate);

    	//////////
    	// When a download is received
    	virtual void	downloadMess();
    	
    	//////////
    	// When a purge is received
    	virtual void	purgeMess();
    	
	//////////
	int				mInPreload;

    private:
    	
    	//////////
    	// static member functions
    	static void 	preloadMessCallback(void *data, t_symbol *filename, t_floatarg baseImage, t_floatarg topImage, t_floatarg skipRate);
    	static void 	downloadImageCallback(void *data);
    	static void 	purgeImageCallback(void *data);
};

#endif	// for header file
