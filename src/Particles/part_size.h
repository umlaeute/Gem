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
    	part_size(t_floatarg num);
    	
    	//////////
    	virtual void 	render(GemState *state);

    protected:
    	
    	//////////
    	// Destructor
    	virtual ~part_size();
		
		//////////
		void			numberMess(float num)		{ m_size = num; }

		//////////
		float			m_size;
	
	private:

		//////////
		// static member functions
		static void		numberMessCallback(void *data, t_floatarg num);
};

#endif	// for header file
