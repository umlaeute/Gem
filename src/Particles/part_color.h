/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

	Set the color for a particle system

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PART_COLOR_H_
#define INCLUDE_PART_COLOR_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

	part_color
    
	Set the color for a particle system

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN part_color : public GemBase
{
    CPPEXTERN_HEADER(part_color, GemBase)

    public:

	    //////////
	    // Constructor
    	part_color();
    	
    	//////////
    	virtual void 	render(GemState *state);

    protected:
    	
    	//////////
    	// Destructor
    	virtual ~part_color();

		//////////
		// One end of the range
		float			m_color1[3];

		//////////
		// Other end of the range
		float			m_color2[3];

    	//////////
    	// Color changed
    	void	    	color1Mess(float red, float green, float blue);
    	
    	//////////
    	// Color changed
    	void	    	color2Mess(float red, float green, float blue);
    	
    private:
    	
    	//////////
    	// static member functions
    	static void 	color1MessCallback(void *data, t_floatarg r, t_floatarg g, t_floatarg b);
    	static void 	color2MessCallback(void *data, t_floatarg r, t_floatarg g, t_floatarg b);
};

#endif	// for header file
