/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    The base functions and structures
    Also includes gemwin header file

    Copyright (c) 1997-2000 Mark Danks.mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMMAN_H_
#define INCLUDE_GEMMAN_H_

#ifdef _WINDOWS
// I hate Microsoft...I shouldn't have to do this!
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/glu.h>
#include <Carbon/Carbon.h>
#include <AGL/agl.h>
#else
#include <GL/glu.h>
#endif // __APPLE__

#include "Base/GemExportDef.h"

class gemhead;
class gemheadLink;
class gemcontrol;
class gemctrlLink;
class GemState;
class WindowInfo;

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GemMan
    
    A static class to create windows, etc.

DESCRIPTION
    
-----------------------------------------------------------------*/
class GEM_EXTERN GemMan
{
    public:

        //////////
        // Should only be called once (usually by GemSetup)
    	static void 	    initGem();

    	//////////
    	static void 	    addCtrl(gemcontrol *obj);   	
    	//////////
    	static void 	    removeCtrl(gemcontrol *obj);

    	//////////
    	static void 	    addObj(gemhead *obj, int priority);   	
    	//////////
    	static void 	    removeObj(gemhead *obj, int priority);

        //////////
    	// Is there a window.
    	static int  	    windowExists()  	    { return 0; }

        //////////
    	// Are we rendering.
    	static int  	    getRenderState()  	    { return(m_rendering); }

	//////////
	// is there a context (has its meaning under X)
	static void         createContext(char* disp);
    	static int  	    contextExists()  	    { return(m_windowContext); }
    	
        //////////
        // If an object needs to know if the window changed.
        // This is important for display lists.
    	static int  	    windowNumber()  	    { return(m_windowNumber); }
    	
    	//////////
    	// reset to the initial state
    	static void 	    resetState();


    	    	
    	//////////
    	// Just send out one frame (if double buffered, will swap buffers)
    	static void 	    render(void *);
        static void	    renderChain(gemheadLink *head, GemState *state);

    	    	
    	//////////
    	// Start a clock to do rendering.
    	static void 	    startRendering();
    	
    	//////////
    	// Stop the clock to do rendering.
    	static void 	    stopRendering();
    	
        //////////
        // Create the window with the current parameters
    	static int 	    createWindow(char* disp = 0);

        //////////
        // Destroy the window
    	static void 	    destroyWindow();
    	
        //////////
        // Swap the buffers.
        // If single buffered, just clears the window
    	static void	    swapBuffers();

    	//////////
    	// Set the frame rate
    	static void 	    frameRate(float framespersecond);

    	//////////
    	// Turn on/off lighting
    	static void 	    lightingOnOff(int state);
    	
	//////////
	// Turn on/off cursor
	static void         cursorOnOff(int state);

	//////////
	// Turn on/off topmost position
	static void         topmostOnOff(int state);


    	//////////
    	// Request a lighting value - it is yours until you free it.
    	// The return can be 0, in which there are too many lights
		// [in] specific - If you want a specific light.  == 0 means that you don't care.
    	static GLenum	    requestLight(int specific = 0);
    	
    	//////////
    	// Free a lighting value
    	static void 	    freeLight(GLenum lightNum);
    	
    	//////////
    	// Print out information
    	static void 	    printInfo();
    	
    	//////////
        // computer and window information
        static int	   m_profile;		// off(0), on(1), w/o image caching(2)
	static int	   m_rendering;

	static float       m_motionBlur;        // motion-blur factor in double-buffer mode
	static int	   texture_rectangle_supported;
	static int	   client_storage_supported;
	static int	   texture_range_supported;
	static int	   texture_yuv_supported;
        static float	   fps;
        static int	   fsaa;
	
        //////////
    	static void 	    fillGemState(GemState &);


	static void checkExtensions();

	static void render1(GemState);
	static void render2(GemState);
 private:
	
    	// gemwin is allowed to modifying "global" window attributes
    	friend class gemwin;
	friend class gemwindow;
    	
 	static double	    m_lastRenderTime;	// the time of the last rendered frame
	
    	// gemwin should not touch the following member variables and member functions
    	static int  	    m_windowNumber;
    	static int  	    m_windowContext;
};

#endif
