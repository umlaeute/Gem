/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    scale a gem object

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_SCALEXYZ_H_
#define INCLUDE_SCALEXYZ_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    scaleXYZ
    
    scale a gem object

DESCRIPTION
    
    Inlet for X - "xVal"
    Inlet for Y - "yVal"
    Inlet for Z - "zVal"

-----------------------------------------------------------------*/
class GEM_EXTERN scaleXYZ : public GemBase
{
    CPPEXTERN_HEADER(scaleXYZ, GemBase)

    public:

        //////////
        // Constructor
    	scaleXYZ(int argc, t_atom *argv);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~scaleXYZ();

    	//////////
    	// When rendering occurs
    	virtual void	render(GemState *state);

    	//////////
    	// The scale vector (x, y, z)
    	float	    	m_vector[3];

    	//////////
    	// X value changed
    	void	    	xMess(float val);
    	
    	//////////
    	// Y value changed
    	void	    	yMess(float val);
    	
    	//////////
    	// Z value changed
    	void	    	zMess(float val);
    	
    private:
    	
    	//////////
    	// static member functions
    	static void 	xMessCallback(void *data, t_floatarg val);
    	static void 	yMessCallback(void *data, t_floatarg val);
    	static void 	zMessCallback(void *data, t_floatarg val);
};

#endif	// for header file
