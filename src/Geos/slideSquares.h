/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A bunch of sliding squares

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_SLIDESQUARES_H_
#define INCLUDE_SLIDESQUARES_H_

#include "Base/GemShape.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    slideSquares
    
    Creates a bunch of sliding squares

KEYWORDS
    geo
    
DESCRIPTION
    
-----------------------------------------------------------------*/
class GEM_EXTERN slideSquares : public GemShape
{
    CPPEXTERN_HEADER(slideSquares, GemShape)

    public:

        //////////
        // Constructor
		slideSquares(t_floatarg width, t_floatarg height);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~slideSquares();

    	//////////
    	// The height of the object
    	void	    	heightMess(float size);

    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);

    	//////////
    	// How the object should be drawn
    	virtual void	typeMess(t_symbol *type);
		
    	//////////
    	// The height of the object
        GLfloat	    	m_height;

        //////////
        // The height inlet
        t_inlet         *m_inletH;
        
        
        GLvoid 	Slide( int i );
        void	slide_init();
        float	xsize, ysize;
        int	m_blend;

	private:

		//////////
		// Static member functions
    	static void 	heightMessCallback(void *data, t_floatarg size);
        static void 	blendMessCallback(void *data, t_floatarg size);
};

#endif	// for header file
