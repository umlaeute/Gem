/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Respond to space orb events
    
    Copyright (c) 1997-1998 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMORB_H_
#define INCLUDE_GEMORB_H_

#ifndef HELPSYMBOL_BASE
# define HELPSYMBOL_BASE "Gem/"
#endif

#include "Base/CPPExtern.h"

typedef void * OrbHandle;

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    gemorb
    
    Respond to space orb events

DESCRIPTION
    
    
-----------------------------------------------------------------*/
class GEM_EXTERN gemorb : public CPPExtern
{
    CPPEXTERN_HEADER(gemorb, CPPExtern)

    public:

        //////////
        // Constructor
        gemorb(t_floatarg portNumber);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~gemorb();

		//////////
		// Set the timer interval
		// Default is 20.f
		void			setInterval(float interval);

		//////////
		// Set the size of the null region
		// Default is 65
		void			setNullRegion(int region);

		//////////
		// The timer callback
		void			timer();

		//////////
		// Reset the orb
		void			reset();

		//////////
		// The orb handle
		OrbHandle		m_orb;

		//////////
		// The interval between timer callbacks (default is 20.f)
		float			m_interval;

		//////////
		// The buttons
		int				m_but[7];

        //////////
        // The position outlet
        t_outlet    	*m_outPos;

        //////////
        // The rotation outlet
        t_outlet    	*m_outRot;

        //////////
        // The button outlet
        t_outlet    	*m_outBut;

		//////////
		// The clock for the timer
		t_clock			*m_clock;

    private:

        //////////
        // Static member functions
        static void     orbTimerCallback(void *data);
        static void     resetCallback(void *data);
		static void		timerIntervalCallback(void *data, t_floatarg interval);
		static void		nullRegionCallback(void *data, t_floatarg region);
};

#endif  // for header file

