/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

	Starting point for a particle system

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PART_HEAD_H_
#define INCLUDE_PART_HEAD_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

	part_head
    
	Starting point for a particle system

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN part_head : public GemBase
{
    CPPEXTERN_HEADER(part_head, GemBase)

    public:

	    //////////
	    // Constructor
    	part_head(t_floatarg priority);
    	
    	//////////
    	virtual void 	render(GemState *state);

    protected:
    	
    	//////////
    	// Destructor
    	virtual ~part_head();

		//////////
		// The particle group
		int				m_particleGroup;

    	//////////
    	// The speed of the particle system
    	void	    	speedMess(float speed);

    	//////////
    	// The speed of the object
        float	    	m_speed;
	
    private:
    
       	//////////
    	// static member functions
    	static void 	speedMessCallback(void *data, t_floatarg speed);
};

#endif	// for header file
