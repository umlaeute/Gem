/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Send out a bang on pre and post render
    
    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_PDPIN_H_
#define INCLUDE_GEM_PDPIN_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    gem_pdpin
    
    Bridge between pdp and Gem

DESCRIPTION
    
     inserts one pdp-packet into the gem-state
    
-----------------------------------------------------------------*/
class GEM_EXTERN gem_pdpin : public GemBase
{
    CPPEXTERN_HEADER(gem_pdpin, GemBase)

    public:

        //////////
        // Constructor
  gem_pdpin(int argc, t_atom*argv);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~gem_pdpin();

    	//////////
    	// Push the current state
    	virtual void 	render(GemState *state);

    	//////////
    	// Pop the state
    	virtual void 	postrender(GemState *state);


	//////////
	// get the pdp-packet
	void pdpMess(t_symbol*, int);


	//////////
	// the inlet for the pdp-packet
	t_inlet *m_inPDP;

 private:
    	
	//////////
	// static member functions
	static void   pdpMessCallback(void *data, t_symbol*s, t_floatarg f);	

};

#endif	// for header file
