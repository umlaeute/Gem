/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    shininess a gem object

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_SHININESS_H_
#define INCLUDE_SHININESS_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    shininess
    
    shininess a gem object

DESCRIPTION

    Inlet for a float - "shininess"

    "shininess" - the shininess to set the object to
    
-----------------------------------------------------------------*/
class GEM_EXTERN shininess : public GemBase
{
    CPPEXTERN_HEADER(shininess, GemBase)

    public:

	    //////////
	    // Constructor
    	shininess(int argc, t_atom *argv);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~shininess();

    	//////////
    	// Turn back on the color material
    	virtual void 	postrender(GemState *state);

    	//////////
    	// When a gem message is received
    	virtual void	render(GemState *state);

    	//////////
    	// the shininess
    	float	    	m_shininess;

    	//////////
    	// shininess changed
    	void	    	shininessMess(float val);
    	
    private:
    	
    	//////////
    	// static member functions
    	static void 	shininessMessCallback(void *data, t_floatarg val);
};

#endif	// for header file
