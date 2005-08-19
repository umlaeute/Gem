/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A Glu object

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMGLUOBJ_H_
#define INCLUDE_GEMGLUOBJ_H_

// I hate Microsoft...I shouldn't have to do this!
#ifdef __WIN32__
#include <windows.h>
#endif

#include <string.h>


#include <math.h>
#ifndef M_PI
#  define M_PI (3.1415926)
#endif

#include "Base/GemShape.h"

#ifndef GLU_SILHOUETTE
# define GLU_SILHOUETTE 0
#endif

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GemGluObj
    
    A Glu object

DESCRIPTION

    Inlet for an int - "in2"

    "in2" - the number of slices in the object
        
-----------------------------------------------------------------*/
class GEM_EXTERN GemGluObj : public GemShape
{
    public:
    
	    //////////
	    // Constructor
  GemGluObj(t_floatarg size, t_floatarg slices=10);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~GemGluObj();

    	//////////
    	// How the object should be drawn
    	virtual void	typeMess(t_symbol *type);

    	//////////
    	// The number of slices in the quadric
    	void	    	numSlicesMess(int numSlices);

    	//////////
    	// The number of slices
    	int 	    	m_numSlices;
    	
        //////////
        t_inlet         *m_sliceInlet;

    	//////////
    	// creation callback
    	static void 	real_obj_setupCallback(t_class *classPtr)
    	    { GemShape::real_obj_setupCallback(classPtr); GemGluObj::obj_setupCallback(classPtr); }
    	
    private:
    
     	static inline GemGluObj *GetMyClass(void *data) {return((GemGluObj *)((Obj_header *)data)->data);}

    	//////////
    	// Static member functions
    	static void 	obj_setupCallback(t_class *classPtr);
    	static void 	numSlicesMessCallback(void *data, t_floatarg numSlices);
};

#endif	// for header file
