/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    specular a gem object

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/


// 1307:forum::f�r::uml�ute:2000

#ifndef INCLUDE_SPECULAR_H_
#define INCLUDE_SPECULAR_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    specular
    
    specular a gem object

DESCRIPTION

    Inlet for a list - "specular"

    "specular" - the RGB specular to set the object to
    
-----------------------------------------------------------------*/
class GEM_EXTERN specular : public GemBase
{
    CPPEXTERN_HEADER(specular, GemBase)

    public:

	    //////////
	    // Constructor
    	specular(int argc, t_atom *argv);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~specular();

    	//////////
    	// Turn back on the color material
    	virtual void 	postrender(GemState *state);

    	//////////
    	// When a gem message is received
    	virtual void	render(GemState *state);

    	//////////
    	// The specular vector (RGBA)
    	float	    	m_specular[4];

    	//////////
    	// specular changed
    	void	    	specularMess(float red, float green, float blue, float alpha);
    	
    private:
    	
    	//////////
    	// static member functions
    	static void 	specularMessCallback(void *data, t_symbol *, int argc, t_atom *argv);
};

#endif	// for header file
