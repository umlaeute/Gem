////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "Base/config.h"
#if HAVE_LIBGLUT
#include "GemGlutObj.h"

/////////////////////////////////////////////////////////
//
// GemGlutObj
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GemGlutObj :: GemGlutObj(t_floatarg size)
    	   : GemShape(size), m_numSlices(10), m_thing(NULL)
{
//    m_drawType = Glut_FILL;
    
    // the number of slices
//    m_sliceInlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("numslices"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GemGlutObj :: ~GemGlutObj()
{
    // in case we are deleted while still running
    stopRendering();
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void GemGlutObj :: startRendering()
{
    if (m_thing)
		stopRendering();
//    m_thing = GlutNewQuadric();
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void GemGlutObj :: stopRendering()
{
    if (m_thing){;}
//		GlutDeleteQuadric(m_thing);
    m_thing = NULL;
}

/////////////////////////////////////////////////////////
// numSlicesMess
//
/////////////////////////////////////////////////////////
void GemGlutObj :: numSlicesMess(int numSlices)
{
    m_numSlices = (numSlices < 2) ? 2 : numSlices;
    setModified();
}

/////////////////////////////////////////////////////////
// typeMess
//
/////////////////////////////////////////////////////////
void GemGlutObj :: typeMess(t_symbol *type)
{
#if 0
    if (!strcmp(type->s_name, "line")) 
	    m_drawType = Glut_LINE;
    else if (!strcmp(type->s_name, "fill")) 
	    m_drawType = Glut_FILL;
    else if (!strcmp(type->s_name, "point"))
	    m_drawType = Glut_POINT;
    else
    {
	    error ("GEM: GemGlutObj draw style");
	    return;
    }
#endif
    setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void GemGlutObj :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&GemGlutObj::numSlicesMessCallback,
    	    gensym("numslices"), A_FLOAT, A_NULL); 
}
void GemGlutObj :: numSlicesMessCallback(void *data, t_floatarg numSlices)
{
    GetMyClass(data)->numSlicesMess((int)numSlices);
}

#endif

