/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Set the alpha values of a pix

    Copyright (c) 1997-1998 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    Copyright (c) 2002 James Tittle & Chris Clepper
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_COLORALPHA_H_
#define INCLUDE_PIX_COLORALPHA_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_coloralpha
    
    Set the alpha values of a pix

KEYWORDS
    pix
    
DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN pix_coloralpha : public GemPixObj
{
    CPPEXTERN_HEADER(pix_coloralpha, GemPixObj)

    public:

	    //////////
	    // Constructor
    	pix_coloralpha();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_coloralpha();

    	//////////
    	// Do the processing
    	virtual void 	processRGBAImage(imageStruct &image);
};

#endif	// for header file
