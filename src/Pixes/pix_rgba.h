/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    age an image

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

    this is based on EffecTV by Fukuchi Kentauro
    * AgingTV - film-aging effect.
    * Copyright (C) 2001 FUKUCHI Kentarou

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_RGBA_H_
#define INCLUDE_PIX_RGBA_H_

#include "Pixes/pix_fx.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_rgba
    
    Change pix from "any" color-space to GL_RGBA

KEYWORDS
    pix
    
DESCRIPTION
   
-----------------------------------------------------------------*/

class GEM_EXTERN pix_rgba : public pix_fx
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
    	void 	processFX(imageStruct &image);

	unsigned char *m_data;
	int            m_size;

 private:
};

#endif	// for header file
