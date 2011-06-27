/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Load an image into a pix block

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_IMAGE_H_
#define INCLUDE_PIX_IMAGE_H_
#include "Base/GemConfig.h"

#include <string.h>

#include "Base/GemBase.h"
#include "Base/GemPixUtil.h"
#include "Base/GemPixImageLoad.h"


/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_image
    
    Loads in an image
    
KEYWORDS
    pix
    
DESCRIPTION

    "open" - opens a file
    
-----------------------------------------------------------------*/
class GEM_EXTERN pix_image : public GemBase
{
    CPPEXTERN_HEADER(pix_image, GemBase);

    public:

	    //////////
	    // Constructor
    	pix_image(t_symbol *filename);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_image();

    	//////////
    	// When an open is received
    	virtual void	openMess(t_symbol *filename);
    	
    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);

    	//////////
    	// Clear the dirty flag on the pixBlock
    	virtual void 	postrender(GemState *state);

    	//////////
    	virtual void	startRendering();

    	//////////
    	// Clean up the image and the pixBlock
    	void	    	cleanImage();
    
        //-----------------------------------
        // GROUP:	Image data
        //-----------------------------------

    	//////////
    	// do we want threaded reading (default: yes);
    	virtual void	threadMess(bool onoff);
	bool m_wantThread;
 
        //////////
        // the full filename of the image
        char            m_filename[MAXPDSTRING];
   
    	//////////
    	// The original image
    	imageStruct     *m_loadedImage;
    	
    	//////////
    	// The pixBlock with the current image
    	pixBlock    	m_pixBlock;
	//////////
	// The current image
	imageStruct     m_imageStruct;

	void     loaded(unsigned int ID, 
			imageStruct*img,
			const gem::Properties&props);
    	    	
    private:
    	
    	//////////
    	// static member functions
    	static void 	openMessCallback(void *data, t_symbol *filename);
    	static void 	threadMessCallback(void *data, t_floatarg f);

	static void     loadCallback(void*data, 
				     unsigned int ID, 
				     imageStruct*img,
				     const gem::Properties&props);
};

#endif	// for header file
