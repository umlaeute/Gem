////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) G¸nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f¸r::uml‰ute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "vertex_info.h"

#include "Base/GemState.h"
#include "string.h"
CPPEXTERN_NEW(vertex_info)

/////////////////////////////////////////////////////////
//
// vertex_info
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
vertex_info :: vertex_info() : m_previousSize(0), m_vertNum(0), m_vertCount(0)
{
  m_Vsize = outlet_new(this->x_obj, 0);
  //m_Csize = outlet_new(this->x_obj, 0);
  //m_VOut = outlet_new(this->x_obj, 0);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
vertex_info :: ~vertex_info()
{
  if(m_Vsize)outlet_free(m_Vsize);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void vertex_info :: render(GemState *state)
{
    int i,size,src,count;
    GLfloat *VertexArray;
    
    VertexArray =state->VertexArray;
    if (state->VertexArray == NULL || state->VertexArraySize <= 0){
        post("vertex_info: no vertex array!");
        return;
    }
    
    if (state->VertexArray == NULL ){
        post("vertex_info: no color array!");
        return;
    }
    
    size = state->VertexArraySize;
    
    outlet_float(m_Vsize, (t_float)size);
}
 
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void vertex_info :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&vertex_info::vertexMessCallback,
    	    gensym("vertex"), A_FLOAT, A_FLOAT, A_NULL);
}

void vertex_info :: vertexMessCallback(void *data,  t_floatarg num, t_floatarg counter)
{
    GetMyClass(data)->m_vertNum=((int)num);
    GetMyClass(data)->m_vertCount=((int)counter);
}
