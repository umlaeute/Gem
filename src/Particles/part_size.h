/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

	Set the initial size

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PART_SIZE_H_
#define INCLUDE_PART_SIZE_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

	part_size
    
	Set the initial size

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN part_size : public GemBase
{
    CPPEXTERN_HEADER(part_size, GemBase)

    public:

	    //////////
	    // Constructor
    	part_size(int,t_atom*);
    	
    	//////////
    	virtual void 	render(GemState *state);

    protected:
    	
    	//////////
    	// Destructor
    	virtual ~part_size();
		
	//////////
	void		sizeMess(int,t_atom*);

	//////////
	float		m_size[3];
	
 private:

	//////////
	// static member functions
	static void	sizeMessCallback(void *data, t_symbol*,int,t_atom*);
};

#endif	// for header file
