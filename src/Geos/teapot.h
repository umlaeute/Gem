/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A teapot

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_TEAPOT_H_
#define INCLUDE_TEAPOT_H_

#include "Base/GemGluObj.h"


/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
	teapot
    
    Creates a teapot

KEYWORD
    geo
    
DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN teapot : public GemGluObj
{
    CPPEXTERN_HEADER(teapot, GemGluObj)

    public:

	    //////////
	    // Constructor
  teapot(t_floatarg size, t_floatarg slice);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~teapot();

    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);


	GLfloat m_texCoords[4][2];

};

#endif	// for header file
