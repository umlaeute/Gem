/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Respond to mouse events
    
    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMMOUSE_H_
#define INCLUDE_GEMMOUSE_H_

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
        gemmouse();
    	
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

    private:

        //////////
        // Static member functions
        static void     mouseMotionCallback(int x, int y, void *data);
        static void     mouseButtonCallback(int which, int state, int x, int y, void *data);
};

#endif  // for header file
