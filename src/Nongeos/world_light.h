/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A world_light

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_WORLD_LIGHT_H_
#define INCLUDE_WORLD_LIGHT_H_

// I hate Microsoft...I shouldn't have to do this!
#ifdef _WINDOWS
#include <windows.h>
#endif

#ifdef MACOSX
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif // MACOSX

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    world_light
    
    Creates a world_light - position is at infinity (can be rotated)

DESCRIPTION

    Inlet for a list - "clrlist"

    "clrlist" - Determines color
    "int" - On/off state
    
-----------------------------------------------------------------*/
class GEM_EXTERN world_light : public GemBase
{
    CPPEXTERN_HEADER(world_light, GemBase)

    public:

        //////////
        // Constructor
    	world_light(t_floatarg lightNum);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~world_light();

    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);

    	//////////
    	// Turn on/off the debugging object
    	void 	    	debugMess(int state);
    	
    	//////////
    	// Turn the light on or off
    	void 	    	lightOnOffMess(int state);
    	
    	//////////
    	// Set the light's color
    	void	    	lightColorMess(float red, float green, float blue);

    	//////////
    	// If you care about the start of rendering
     	virtual void	startRendering();
    	
    	//////////
    	// If you care about the stop of rendering
     	virtual void	stopRendering();
    	
	    //-----------------------------------
	    // GROUP:	Member variables
	    //-----------------------------------
    
    	//////////
    	// The light color
    	GLfloat     	m_color[4];
    	
    	//////////
    	// The position
    	GLfloat     	m_position[4];
		
		//////////
		// If a change occured
		int				m_change;
    	
    	//////////
    	// The on/off state of the light
    	int 	    	m_on;
    	
    	//////////
    	// The on/off state for debugging
    	int 	    	m_debug;
    	
    	//////////
    	// The light number with reference to OpenGL
    	GLenum	    	m_light;

    	//////////
    	GLUquadricObj 	*m_thing;
    	
    private:
    	    
    	//////////
    	// Static member functions
    	static void 	lightColorMessCallback(void *data, t_floatarg red, t_floatarg green, t_floatarg blue);
    	static void 	lightOnOffMessCallback(void *data, t_floatarg n);
    	static void 	debugMessCallback(void *data, t_floatarg n);
};

#endif	// for header file
