/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

	Set the target color for a particle system

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PART_TARGETCOLOR_H_
#define INCLUDE_PART_TARGETCOLOR_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

	part_targetcolor
    
	Set the target color for a particle system

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN part_targetcolor : public GemBase
{
    CPPEXTERN_HEADER(part_targetcolor, GemBase)

    public:

	    //////////
	    // Constructor
    	part_targetcolor(int argc, t_atom *argv);
    	
    	//////////
    	virtual void 	render(GemState *state);

    protected:
    	
    	//////////
    	// Destructor
    	virtual ~part_targetcolor();

    	//////////
    	// The scale factor angle
    	float	    	m_scale;

		//////////
		// The color vector (RGBA)
		float			m_color[4];

    	//////////
    	// Scale changed
    	void	    	scaleMess(float scale);
    	
    	//////////
    	// Color changed
    	void	    	colorMess(float red, float green, float blue, float alpha);
    	
    private:
    	
    	//////////
    	// static member functions
    	static void 	scaleMessCallback(void *data, t_floatarg scale);
    	static void 	colorMessCallback(void *data, t_symbol *, int argc, t_atom *argv);
};

#endif	// for header file
