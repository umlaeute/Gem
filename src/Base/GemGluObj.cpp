////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute
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
GemGluObj :: GemGluObj(t_floatarg size, t_floatarg slices)
    	   : GemShape(size)
{
    m_drawType = (GLenum) GL_FILL;
    m_numSlices=(int)slices;
    if(m_numSlices<=0)m_numSlices=10;
    
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
    inlet_free(m_sliceInlet);
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
  char c=*type->s_name;
  switch (c){
  case 'D': case 'd': // default
    m_drawType = GL_DEFAULT_GEM;
    break;
  case 'L': case 'l': // line
    m_drawType = GL_LINE;
    break;
  case 'F': case 'f': // fill
    m_drawType = GL_FILL;
    break;
  case 'P': case 'p': // point
    m_drawType = GL_POINT;
    break;
  default:
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
