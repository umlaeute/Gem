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

#include "Base/config.h"
#include "Base/GemShape.h"

#ifdef MACOSX
#include <GLUT/glut.h>
#else
#ifdef HAVE_LIBGLUT
#include <GL/glut.h>
#endif
#endif

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GemGlutObj
    
    A Glut object

DESCRIPTION
    not every one has Glut installed,
    therefor, the Glut-routines itself have to be #ifdef-ed, like
#ifdef HAVE_LIBGLUT
  glutSolidTeapot(10.0);
#endif

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
    	// creation callback
    	static void 	real_obj_setupCallback(t_class *classPtr)
    	    { GemShape::real_obj_setupCallback(classPtr); GemGlutObj::obj_setupCallback(classPtr); }
    	
    private:
    
     	static inline GemGlutObj *GetMyClass(void *data) {return((GemGlutObj *)((Obj_header *)data)->data);}

    	//////////
    	// Static member functions
    	static void 	obj_setupCallback(t_class *classPtr);
};
#endif	// for header file
