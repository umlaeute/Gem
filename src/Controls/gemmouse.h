/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Respond to mouse events
    
    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMMOUSE_H_
#define INCLUDE_GEMMOUSE_H_

#ifndef HELPSYMBOL_BASE
# define HELPSYMBOL_BASE "Gem/"
#endif

#include "Base/CPPExtern.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    gemmouse
    
    Respond to mouse events

DESCRIPTION
    
    
-----------------------------------------------------------------*/
class GEM_EXTERN gemmouse : public CPPExtern
{
    CPPEXTERN_HEADER(gemmouse, CPPExtern)

    public:

        //////////
        // Constructor
  gemmouse(int,t_atom*);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~gemmouse();

        //////////
        // mouse motion
        void            mouseMotion(int x, int y);

        //////////
        // mouse button
        void            mouseButton(int which, int state, int x, int y);

        //////////
        // mouse button
        void		mouseWheel(int axis, int value);
        
        //////////
        // The xpos outlet
        t_outlet    	*m_outXPos;

        //////////
        // The ypos outlet
        t_outlet    	*m_outYPos;

        //////////
        // The left button outlet
        t_outlet    	*m_outLBut;

        //////////
        // The middle outlet
        t_outlet    	*m_outMBut;

        //////////
        // The right outlet
        t_outlet    	*m_outRBut;

	//////////
	// coordinate-scales
	// if !=0, the mouse-coordinate is scaled to 0..m_scale
	// if ==0, the mouse-coordinate is scaled to 0..windowsize
	t_float m_scaleX, m_scaleY;

	//////////
	// should Y we scaled separately or like X ?
	bool m_doY;

    private:

        //////////
        // Static member functions
        static void     mouseMotionCallback(int x, int y, void *data);
        static void     mouseButtonCallback(int which, int state, int x, int y, void *data);
        static void	mouseWheelCallback(int axis, int value, void *data);
};

#endif  // for header file
