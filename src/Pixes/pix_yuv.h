/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    age an image

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    Copyright (c) 2002 James Tittle & Chris Clepper
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

    this is based on EffecTV by Fukuchi Kentauro
    * Copyright (C) 2001 FUKUCHI Kentarou

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_YUV_H_
#define INCLUDE_PIX_YUV_H_

#ifdef MACOSX
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#endif

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_yuv
    
    Change pix from "any" color-space to GL_RGBA

KEYWORDS
    pix
    
DESCRIPTION
   
-----------------------------------------------------------------*/

class GEM_EXTERN pix_yuv : public GemPixObj
{
    CPPEXTERN_HEADER(pix_yuv, GemPixObj)

    public:

	    //////////
	    // Constructor
    	pix_yuv();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_yuv();

    	//////////
    	// Do the processing
    	void 	processImage(imageStruct &image);

	void render(GemState *);
	void postrender(GemState *);

	unsigned char *m_data;
	int            m_size;

 private:
};

#endif	// for header file
