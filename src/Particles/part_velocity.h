/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

	Set the initial size

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PART_VELOCITY_H_
#define INCLUDE_PART_VELOCITY_H_

#include "Particles/part_size.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

	part_velocity
    
	Set the initial velocity

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN part_velocity : public part_size
{
    CPPEXTERN_HEADER(part_velocity, GemBase)

    public:

    //////////
    // Constructor
    part_velocity(int,t_atom*);
    	
    //////////
    virtual void 	render(GemState *state);

    protected:
    	
    //////////
    // Destructor
    virtual ~part_velocity(){}
	
 private:

    //////////
    // static member functions
    static void	sizeMessCallback(void *data, t_symbol*,int,t_atom*);
};

#endif	// for header file
