/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

	Kill particles that are past a certain time

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PART_KILLOLD_H_
#define INCLUDE_PART_KILLOLD_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

	part_killold
    
	Kill particles that are past a certain time

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN part_killold : public GemBase
{
    CPPEXTERN_HEADER(part_killold, GemBase)

    public:

	    //////////
	    // Constructor
    	part_killold(t_floatarg num);
    	
    	//////////
    	virtual void 	render(GemState *state);

    protected:
    	
    	//////////
    	// Destructor
    	virtual ~part_killold();
		
		//////////
		void			numberMess(float num)	{ m_killAge = num; }

		//////////
		float			m_killAge;
	
	private:

		//////////
		// static member functions
		static void		numberMessCallback(void *data, t_floatarg num);
};

#endif	// for header file
