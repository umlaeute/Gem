/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

	Draw a particle group

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PART_DRAW_H_
#define INCLUDE_PART_DRAW_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

	part_draw
    
	Draw a particle group

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN part_draw : public GemBase
{
    CPPEXTERN_HEADER(part_draw, GemBase)

    public:

	    //////////
	    // Constructor
    	part_draw();
    	
    	//////////
    	virtual void 	render(GemState *state);

    protected:
    	
    	//////////
    	// Destructor
    	virtual ~part_draw();

    	//////////
    	// How the object should be drawn
		void			typeMess(int,t_atom*);
    	
		//////////
		int				m_drawType;

	private:

		//////////
		// static member functions
		static void 	typeMessCallback(void *data,  t_symbol *s,int,t_atom *);
};

#endif	// for header file
