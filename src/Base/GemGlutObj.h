/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A Glut object

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  forum::für::umläute

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMGLUTOBJ_H_
#define INCLUDE_GEMGLUTOBJ_H_

#if HAVE_LIBGLUT || MACOSX
#include "Base/GemShape.h"

#ifdef MACOSX
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif	//MACOSX

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GemGluObj
    
    A Glu object

DESCRIPTION

    Inlet for an int - "in2"

    "in2" - the number of slices in the object
        
-----------------------------------------------------------------*/
class GEM_EXTERN GemGlutObj : public GemShape
{
    public:
    
	    //////////
	    // Constructor
    	GemGlutObj(t_floatarg size);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~GemGlutObj();

    	//////////
    	// Create the quadric
    	virtual void	startRendering();
    	
    	//////////
    	// Clean it up
    	virtual void	stopRendering();
    	
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
    	GLUquadricObj 	*m_thing;
    	
        //////////
        t_inlet         *m_sliceInlet;

    	//////////
    	// creation callback
    	static void 	real_obj_setupCallback(t_class *classPtr)
    	    { GemShape::real_obj_setupCallback(classPtr); GemGlutObj::obj_setupCallback(classPtr); }
    	
    private:
    
     	static inline GemGlutObj *GetMyClass(void *data) {return((GemGlutObj *)((Obj_header *)data)->data);}

    	//////////
    	// Static member functions
    	static void 	obj_setupCallback(t_class *classPtr);
    	static void 	numSlicesMessCallback(void *data, t_floatarg numSlices);
};

#endif
#endif	// for header file
