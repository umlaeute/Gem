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

#ifndef INCLUDE_GEM_PDPOUT_H_
#define INCLUDE_GEM_PDPOUT_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    gem_pdpout
    
      - forks off one GemState object as pdp packet

DESCRIPTION
    
     - bridge between Gem and pdp
    
-----------------------------------------------------------------*/
class GEM_EXTERN gem_pdpout : public GemBase
{
    CPPEXTERN_HEADER(gem_pdpout, GemBase)

    public:

        //////////
        // Constructor
  gem_pdpout(int argc, t_atom*argv);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~gem_pdpout();

	//////////
	// convert the GemState to something pdp understands
    	void 	render(GemState*);

	//////////
	// after (Gem)execution of the objects below, this gets called
	void postrender(GemState*);

    	//////////
	// an outlet for a pdp packet
        t_outlet        *m_outPDP;	    // output the pdp-package...
};

#endif	// for header file
