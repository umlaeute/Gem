/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A trapezoid

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_SQUARE_H_
#define INCLUDE_SQUARE_H_

#include "Base/GemShape.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    trapezoid
    
    Creates a trapezoid

KEYWORDS
    geo
    
DESCRIPTION
    
-----------------------------------------------------------------*/
class GEM_EXTERN trapezoid : public GemShape
{
    CPPEXTERN_HEADER(trapezoid, GemShape)

    public:

        //////////
        // Constructor
  trapezoid(t_floatarg size, t_floatarg scale);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~trapezoid();
        
      t_inlet *m_topinlet;
      t_float	m_top;

      t_inlet *m_scaleinlet;
      t_float	m_scale_texcoord;

    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);
      static void 	scaleMessCallback(void *data, t_floatarg size);
      static void 	topMessCallback(void *data, t_floatarg size);
};

#endif	// for header file
