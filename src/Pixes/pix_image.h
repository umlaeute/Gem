/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Load an image into a pix block

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_IMAGE_H_
#define INCLUDE_PIX_IMAGE_H_

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
    CPPEXTERN_HEADER(pix_image, GemBase)

    public:

	    //////////
	    // Constructor
    	pix_image(t_symbol *filename);
    	
        class singleImageCache
        {
            public:
                
                singleImageCache(const char *_imageName)
                        : refCount(0), next(NULL), image(NULL)
                        { imageName = strdup(_imageName); }
                ~singleImageCache()
                            { delete imageName;
                              delete image;
                            }
                int                 refCount;
                singleImageCache    *next;
                imageStruct         *image;
                char                *imageName;
        };
    	
    	//////////
        static singleImageCache  *s_imageCache;

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
    
    	//////////
    	// Display the image
    	void	    	clickMess();
    
	        //-----------------------------------
	        // GROUP:	Image data
	        //-----------------------------------
    
    	//////////
    	// The original image
    	singleImageCache *m_loadedImage;
    	
    	//////////
    	// The pixBlock with the current image
    	pixBlock    	m_pixBlock;
	//////////
	// The current image
	imageStruct     m_imageStruct;
    	
    	//////////
    	char	    	m_filename[80];
    	
    private:
    	
    	//////////
    	// static member functions
    	static void 	openMessCallback(void *data, t_symbol *filename);
    	static void 	clickMessCallback(void *data, t_floatarg , t_floatarg , t_floatarg );
};

#endif	// for header file
