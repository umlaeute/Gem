/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    The base class for all of the gem objects

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger.
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
	
-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMBASE_H_
#define INCLUDE_GEMBASE_H_

// I hate Microsoft...I shouldn't have to do this!
#ifdef _WINDOWS
#include <windows.h>
#endif

#include <GL/gl.h>

#ifndef INCLUDE_CPPEXTERN_H_
#include "Base/CPPExtern.h"
#endif INCLUDE_CPPEXTERN_H_
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
    	// After objects below you in the chain have finished.
    	// You should reset all GEM/OpenGL states here.
    	virtual void 	postrender(GemState *)              { ; }

     	//////////
     	// Called when rendering stops
     	virtual void 	stoprender()			{ realStopRendering(); }

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
    	void	    	setModified();

    	//////////
    	// Don't mess with this unless you know what you are doing.
    	GemCache    	*m_cache;

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
	static void	stoprenderCallback(GemBase *data);	//DH
};

#endif	// for header file
