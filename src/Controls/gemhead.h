/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    The starting point for all graphics trees

    Copyright (c) 1997-1998 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMHEAD_H_
#define INCLUDE_GEMHEAD_H_

#include "Base/CPPExtern.h"

class GemState;
class GemCache;
class GemDag;
class gemBaseLink;

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    gemhead
    
    The starting point for all graphics trees

DESCRIPTION
    
    "bang" - sends out a state list
    
-----------------------------------------------------------------*/
class GEM_EXTERN gemhead : public CPPExtern
{
    	CPPEXTERN_HEADER(gemhead, CPPExtern)

    public:

	    //////////
	    // Constructor
    	gemhead(t_floatarg priority);
    	
    	//////////
    	// A render message
    	void	    	renderGL(GemState *state);
    	    	
    	//////////
    	// Set up the modifying flags and the DAG
    	void	    	startRendering();

    	//////////
    	// Clean up the modifying flags and abort the DAG
    	void	    	stopRendering();

    	//////////
    	// The DAG is incorrect and the rendering in the 
    	//  	chain should stop.
    	void	    	breakDAG();

    protected:
    	
    	//////////
    	// Destructor
    	~gemhead();
    	
	private:

	    t_outlet    	*m_out1;

    	GemCache    	*m_cache;   	    	    // The cache information
    	GemDag    		*m_dag;     	    	    // The dag information
    	
    	void	    	renderOnOff(int state);     // Turn rendering on and off
    	int 	    	m_renderOn;
    	
    	void	    	stopBaseRendering(gemBaseLink *link);
    	void	    	renderBase(gemBaseLink *link, GemState *state);
    	void	    	bangMess();
    	
    	//////////
    	// static member functions
    	static void 	bangMessCallback(void *data);
    	static void 	intMessCallback(void *data, t_floatarg n);
};

#endif	// for header file
