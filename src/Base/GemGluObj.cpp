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

#include "GemGluObj.h"

/////////////////////////////////////////////////////////
//
// GemGluObj
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GemGluObj :: GemGluObj(t_floatarg size)
    	   : GemShape(size), m_numSlices(10), m_thing(NULL)
{
    m_drawType = (GLenum) GLU_FILL;
    
    // the number of slices
    m_sliceInlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("numslices"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GemGluObj :: ~GemGluObj()
{
    // in case we are deleted while still running
    stopRendering();
    inlet_free(m_sliceInlet);
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void GemGluObj :: startRendering()
{
    if (m_thing)
		stopRendering();
    m_thing = gluNewQuadric();
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void GemGluObj :: stopRendering()
{
    if (m_thing)
		gluDeleteQuadric(m_thing);
    m_thing = NULL;
}

/////////////////////////////////////////////////////////
// numSlicesMess
//
/////////////////////////////////////////////////////////
void GemGluObj :: numSlicesMess(int numSlices)
{
    m_numSlices = (numSlices < 2) ? 2 : numSlices;
    setModified();
}

/////////////////////////////////////////////////////////
// typeMess
//
/////////////////////////////////////////////////////////
void GemGluObj :: typeMess(t_symbol *type)
{
    if (!strcmp(type->s_name, "line")) 
	    m_drawType = (GLenum) GLU_LINE;
    else if (!strcmp(type->s_name, "fill")) 
	    m_drawType = (GLenum) GLU_FILL;
    else if (!strcmp(type->s_name, "point"))
	    m_drawType = (GLenum) GLU_POINT;
    else
    {
	    error ("GEM: GemGluObj draw style");
	    return;
    }
    setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void GemGluObj :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&GemGluObj::numSlicesMessCallback,
    	    gensym("numslices"), A_FLOAT, A_NULL); 
}
void GemGluObj :: numSlicesMessCallback(void *data, t_floatarg numSlices)
{
    GetMyClass(data)->numSlicesMess((int)numSlices);
}

