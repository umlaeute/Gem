/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    The base class for all of the gem objects

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger.
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
	
-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMBASE_H_
#define INCLUDE_GEMBASE_H_

// I hate Microsoft...I shouldn't have to do this!
#ifdef _WINDOWS
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#endif // __APPLE__

#ifndef INCLUDE_CPPEXTERN_H_
#include "Base/CPPExtern.h"
#endif 
#include "Base/GemState.h"

class GemCache;

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GemBase
    
    Base class for gem objects

DESCRIPTION
    
-----------------------------------------------------------------*/
class GEM_EXTERN GemBase : public CPPExtern
{
    protected:
    	
        //////////
        // Constructor
    	GemBase();
    	
    	//////////
    	// Destructor
    	virtual     	~GemBase();

    	//////////
    	virtual void 	render(GemState *state) = 0;

	//////////
	void    continueRender(GemState *state);

    	//////////
    	// After objects below you in the chain have finished.
    	// You should reset all GEM/OpenGL states here.
    	virtual void 	postrender(GemState *)              { ; }

     	//////////
     	// Called when rendering stops
#if 1/*(jmz) this seems to be for gem2pdp*/
	virtual void 	stoprender()			{ realStopRendering(); }
#endif

    	//////////
    	// If you care about the start of rendering
    	virtual void	startRendering()                    { ; }

    	//////////
    	// If you care about the stop of rendering
    	virtual void	stopRendering()    	                { ; }


	//////////
	// has rendering started ?
	bool            gem_amRendering;
    	
    	//////////
    	// If anything in the object has changed
    	virtual void  	setModified();

    	//////////
    	// Don't mess with this unless you know what you are doing.
    	GemCache    	*m_cache;
	//////////
	// check whether this object has changed
	bool             m_modified;

        //////////
        // The outlet
        t_outlet    	*m_out1;

    	//////////
    	// creation callback
    	static void 	real_obj_setupCallback(t_class *classPtr)
    	    { CPPExtern::real_obj_setupCallback(classPtr); GemBase::obj_setupCallback(classPtr); }
    	
    private:
    
    	void	    	realStopRendering();
        void            gem_startstopMess(int state);
	void            gem_renderMess(GemCache* state, GemState* state2);
  	    	
    	static inline GemBase *GetMyClass(void *data) {return((GemBase *)((Obj_header *)data)->data);}

    	friend class    gemhead;
    	static void 	obj_setupCallback(t_class *classPtr);
     	static void 	gem_MessCallback(void *, t_symbol *,int, t_atom*);
   	static void 	renderCallback(GemBase *data, GemState *state);
    	static void 	postrenderCallback(GemBase *data, GemState *state);
#if 1 /*jmz this seems to be for gem2pdp*/
	static void	stoprenderCallback(GemBase *data);	//DH
#endif
};

#endif	// for header file
