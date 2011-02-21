/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Get pixel information

    Copyright (c) 1997-1998 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    Copyright (c) 2002 James Tittle & Chris Clepper
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_DATA_H_
#define INCLUDE_PIX_DATA_H_

#include "Base/GemPixDualObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

    pix_data
    
	Get pixel information

KEYWORDS
    pix
    
DESCRIPTION

    Inlet for a float - "xPos"
    Inlet for a float - "yPos"

	The x and y position should be normalized between 0 and 1.

-----------------------------------------------------------------*/
class GEM_EXTERN pix_data : public GemPixDualObj
{
    CPPEXTERN_HEADER(pix_data, GemPixDualObj)

    public:

	    //////////
    	// Constructor
    	pix_data();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_data();

    	//////////
    	// All we want is the pixel information, so this is a complete override.
    	virtual void 	render(GemState *state)			{}

    	//////////
        virtual void 	processImage(imageStruct &)		{}

    	//////////
    	virtual void 	processDualImage(imageStruct &, imageStruct &)	{}

        //////////
        void			trigger();

        //////////
        void			xPos(float pos)			{ m_position[0] = pos; }

        //////////
        void			yPos(float pos)			{ m_position[1] = pos; }

        //////////
        float			m_position[2];

        //////////
        // The color outlet
        t_outlet    	*m_colorOut;

        //////////
        // The grayscale outlet
        t_outlet    	*m_grayOut;

	private:

        //////////
        // Static member callbacks
	static void		triggerMessCallback(void *data);
	static void		xPosCallback(void *data, t_floatarg pos);
	static void		yPosCallback(void *data, t_floatarg pos);
};

#endif	// for header file
