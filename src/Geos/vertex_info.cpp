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
CPPEXTERN_NEW_WITH_ONE_ARG(vertex_info, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// vertex_info
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
vertex_info :: vertex_info(t_floatarg size)
        : GemShape(size)
{
      
    m_x=0.f;
    m_y=0.f;
    m_z=0.f;
    m_w=0.f;
    m_previousSize = 0;
    m_vertNum = 0;
    m_vertCount = 0;
    m_Vsize = outlet_new(this->x_obj, 0);
    //m_Csize = outlet_new(this->x_obj, 0);
    //m_VOut = outlet_new(this->x_obj, 0);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
vertex_info :: ~vertex_info()
{ }

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
    /*
    src =0;
    //VertexArray = new float[sizeof(state->VertexArray)];
   // state->VertexArray = new float[sizeof(state->VertexArray)];
   
   if (m_vertNum < 0) m_vertNum = 0;
   if (m_vertNum > size) m_vertNum = size;
   count = m_vertCount;
   
   if (count < 1) count = 1;
   if ((count + m_vertNum-1) > size) count = size - m_vertNum;// -1;
  // post("vertex_info: size %d vertNum %d vertCount %d count %d",size, m_vertNum,m_vertCount,count);
   if (m_vertNum){

        src = (m_vertNum-1) * 4;
        for (i=0; i< count; i++){
        VertexArray[src] += m_x;
        VertexArray[src+1] += m_y;
        VertexArray[src+2] += m_z;
        VertexArray[src+3] += m_w;
        src+=4;
        }
   }else{
   
    src=0;
    for (i=0; i< size; i++){
        VertexArray[src] += m_x;
        VertexArray[src+1] += m_y;
        VertexArray[src+2] += m_z;
        VertexArray[src+3] += m_w;
        src+=4;
    }
    }*/
    //state->VertexArray = VertexArray;
   // memcpy(state->VertexArray, VertexArray, sizeof(VertexArray));
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void vertex_info :: postrender(GemState *state)
{
//m_vertNum = 0;
}
 
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void vertex_info :: obj_setupCallback(t_class *classPtr)
{     class_addmethod(classPtr, (t_method)&vertex_info::offsetMessCallback,
    	    gensym("offset"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&vertex_info::vertexMessCallback,
    	    gensym("vertex"), A_FLOAT, A_FLOAT, A_NULL);
}

void vertex_info :: offsetMessCallback(void *data, t_floatarg x, t_floatarg y, t_floatarg z, t_floatarg w)
{
    GetMyClass(data)->m_x=((float)x);
    GetMyClass(data)->m_y=((float)y);
    GetMyClass(data)->m_z=((float)z);
    GetMyClass(data)->m_w=((float)w);

}

void vertex_info :: vertexMessCallback(void *data,  t_floatarg num, t_floatarg counter)
{
    GetMyClass(data)->m_vertNum=((int)num);
    GetMyClass(data)->m_vertCount=((int)counter);
}
