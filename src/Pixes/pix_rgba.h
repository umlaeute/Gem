/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    age an image

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    Copyright (c) 2002 James Tittle & Chris Clepper
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

    this is based on EffecTV by Fukuchi Kentauro
    * Copyright (C) 2001 FUKUCHI Kentarou

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_RGBA_H_
#define INCLUDE_PIX_RGBA_H_

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#endif

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_rgba
    
    Change pix from "any" color-space to GL_RGBA

KEYWORDS
    pix
    
DESCRIPTION
   
-----------------------------------------------------------------*/

class GEM_EXTERN pix_rgba : public GemPixObj
{
    CPPEXTERN_HEADER(pix_rgba, GemPixObj)

    public:

    //////////
    // Constructor
    pix_rgba();
    	
 protected:
    	
    //////////
    // Destructor
    virtual ~pix_rgba();

    //////////
    // Do the processing
    void 	processImage(imageStruct &image);

    imageStruct m_image;
 private:
};

#endif	// for header file
