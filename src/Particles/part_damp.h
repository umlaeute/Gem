/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

	Apply damping to particles

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PART_DAMP_H_
#define INCLUDE_PART_DAMP_H_

#include "papi.h"

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

	part_damp
    
	Apply damping to particles

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN part_damp : public GemBase
{
    CPPEXTERN_HEADER(part_damp, GemBase)

    public:

	    //////////
	    // Constructor
    	part_damp(t_floatarg val1, t_floatarg val2, t_floatarg val3);
    	
    	//////////
    	virtual void 	render(GemState *state);

    protected:
    	
    	//////////
    	// Destructor
    	virtual ~part_damp();

    	//////////
    	// Vector changed
    	void	    	vectorMess(float x, float y, float z);
    	
		//////////
		float			m_vector[3];

    private:
    	
    	//////////
    	// static member functions
    	static void 	vectorMessCallback(void *data, t_floatarg x, t_floatarg y, t_floatarg z);
};

#endif	// for header file
