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

#include "vertex_offset.h"

#include "Base/GemState.h"
#include "string.h"
CPPEXTERN_NEW_WITH_ONE_ARG(vertex_offset, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// vertex_offset
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
vertex_offset :: vertex_offset(t_floatarg size)
        : GemShape(size)
{
      
    m_x=0.f;
    m_y=0.f;
    m_z=0.f;
    m_w=0.f;
    m_previousSize = 0;
    m_vertNum = 0;
     m_vertCount = 0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
vertex_offset :: ~vertex_offset()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void vertex_offset :: render(GemState *state)
{
   
   
    int i,size,src,count;
    GLfloat *VertexArray;
    
    VertexArray =state->VertexArray;
    if (state->VertexArray == NULL || state->VertexArraySize <= 0){
     //   post("vertex_offset: no vertex array!");
        return;
    }
    
    if (state->VertexArray == NULL ){
        post("vertex_offset: no color array!");
        return;
    }
    
    size = state->VertexArraySize;
    src =0;
    //VertexArray = new float[sizeof(state->VertexArray)];
   // state->VertexArray = new float[sizeof(state->VertexArray)];
   
   if (m_vertNum < 0) m_vertNum = 0;
   if (m_vertNum > size) m_vertNum = size;
   count = m_vertCount;
   
   if (count < 1) count = 1;
   if ((count + m_vertNum-1) > size) count = size - m_vertNum;// -1;
  // post("vertex_offset: size %d vertNum %d vertCount %d count %d",size, m_vertNum,m_vertCount,count);
  
  //needs some altivec 
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
    }
    //state->VertexArray = VertexArray;
   // memcpy(state->VertexArray, VertexArray, sizeof(VertexArray));
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void vertex_offset :: postrender(GemState *state)
{
//m_vertNum = 0;
}
 
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void vertex_offset :: obj_setupCallback(t_class *classPtr)
{     class_addmethod(classPtr, (t_method)&vertex_offset::offsetMessCallback,
    	    gensym("offset"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&vertex_offset::vertexMessCallback,
    	    gensym("vertex"), A_FLOAT, A_FLOAT, A_NULL);
}

void vertex_offset :: offsetMessCallback(void *data, t_floatarg x, t_floatarg y, t_floatarg z, t_floatarg w)
{
    GetMyClass(data)->m_x=((float)x);
    GetMyClass(data)->m_y=((float)y);
    GetMyClass(data)->m_z=((float)z);
    GetMyClass(data)->m_w=((float)w);

}

void vertex_offset :: vertexMessCallback(void *data,  t_floatarg num, t_floatarg counter)
{
    GetMyClass(data)->m_vertNum=((int)num);
    GetMyClass(data)->m_vertCount=((int)counter);
}
