/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

	Have the particles follow each other

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PART_FOLLOW_H_
#define INCLUDE_PART_FOLLOW_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

	part_follow
    
	Have the particles follow each other

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN part_follow : public GemBase
{
    CPPEXTERN_HEADER(part_follow, GemBase)

    public:

	    //////////
	    // Constructor
    	part_follow(t_floatarg num);
    	
    	//////////
    	virtual void 	render(GemState *state);

    protected:
    	
    	//////////
    	// Destructor
    	virtual ~part_follow();
		
		//////////
		void			numberMess(float num)	{ m_accel = num; }

		//////////
		float			m_accel;
	
	private:

		//////////
		// static member functions
		static void		numberMessCallback(void *data, t_floatarg num);
};

#endif	// for header file
