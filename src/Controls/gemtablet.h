/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Respond to tablet events
    
    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMTABLET_H_
#define INCLUDE_GEMTABLET_H_

#include "Base/CPPExtern.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    gemtablet
    
    Respond to tablet events

DESCRIPTION
    
    
-----------------------------------------------------------------*/
class GEM_EXTERN gemtablet : public CPPExtern
{
    CPPEXTERN_HEADER(gemtablet, CPPExtern)

    public:

        //////////
        // Constructor
        gemtablet();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~gemtablet();

        //////////
        // tablet motion
        void            tabletMotion(int x, int y, float pressure);

        //////////
        // tablet rotation
        void            tabletRotation(int az, int alt, int twist);

        //////////
        // tablet button
        void            tabletButton(int which, int state, int x, int y);

        //////////
        // The xpos outlet
        t_outlet    	*m_outXPos;

        //////////
        // The ypos outlet
        t_outlet    	*m_outYPos;

        //////////
        // The pressure outlet
        t_outlet    	*m_outPress;

        //////////
        // The azimuth outlet
        t_outlet    	*m_outAz;

        //////////
        // The altitude outlet
        t_outlet    	*m_outAlt;

        //////////
        // The twist outlet
        t_outlet    	*m_outTwist;

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
        static void     tabletMotionCallback(int x, int y, float pressure, void *data);
        static void     tabletRotationCallback(int az, int alt, int twist, void *data);
        static void     tabletButtonCallback(int which, int state, int x, int y, void *data);
};

#endif  // for header file
