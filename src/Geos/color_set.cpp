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

#include "color_set.h"

#include "Base/GemState.h"
#include "string.h"
CPPEXTERN_NEW_WITH_ONE_ARG(color_set, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// color_set
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
color_set :: color_set(t_floatarg size)
        : GemShape(size)
{
      
    m_Red=1.f;
    m_Green=1.f;
    m_Blue=1.f;
    m_Alpha=1.f;
    m_previousSize = 0;
    m_colorNum = 0;
    m_colorCount = 0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
color_set :: ~color_set()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void color_set :: render(GemState *state)
{
   
   
    int i,size,src,count;
    GLfloat *ColorArray;
    
    
    if (state->VertexArray == NULL || state->VertexArraySize <= 0){
        post("color_set: no vertex array!");
        return;
    }
    
    if (state->ColorArray == NULL ){
        post("color_set: no color array!");
        return;
    }
    ColorArray =state->ColorArray;
    size = state->VertexArraySize;
    src =0;
    //ColorArray = new float[sizeof(state->ColorArray)];
   // state->ColorArray = new float[sizeof(state->VertexArray)];
   
   if (m_colorNum < 0) m_colorNum = 0;
   if (m_colorNum > size) m_colorNum = size;
   
   count = m_colorCount;
   
   if (count < 1) count = 1;
   if ((count + m_colorNum-1) > size) count = size - m_colorNum;
   if (m_colorNum){

        src = (m_colorNum-1) * 4;
        for (i=0; i< count; i++){
            ColorArray[src] = m_Red;
            ColorArray[src+1] = m_Green;
            ColorArray[src+2] = m_Blue;
            ColorArray[src+3] = m_Alpha;
            src+=4;
        }
   }else{
   
    src=0;
    for (i=0; i< size; i++){
        ColorArray[src] = m_Red;
        ColorArray[src+1] = m_Green;
        ColorArray[src+2] = m_Blue;
        ColorArray[src+3] = m_Alpha;
        src+=4;
    }
    }
    //state->ColorArray = ColorArray;
   // memcpy(state->ColorArray, ColorArray, sizeof(ColorArray));
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void color_set :: postrender(GemState *state)
{
//m_colorNum = 0;
}
 
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void color_set :: obj_setupCallback(t_class *classPtr)
{     class_addmethod(classPtr, (t_method)&color_set::colorMessCallback,
    	    gensym("color"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
      class_addmethod(classPtr, (t_method)&color_set::numberMessCallback,
    	    gensym("number"), A_FLOAT, A_FLOAT, A_NULL);
}

void color_set :: colorMessCallback(void *data, t_floatarg R, t_floatarg G, t_floatarg B, t_floatarg A)
{
    GetMyClass(data)->m_Red=((float)R);
    GetMyClass(data)->m_Green=((float)G);
    GetMyClass(data)->m_Blue=((float)B);
    GetMyClass(data)->m_Alpha=((float)A);
}

void color_set :: numberMessCallback(void *data,  t_floatarg num, t_floatarg counter)
{
    GetMyClass(data)->m_colorNum=((int)num);
    GetMyClass(data)->m_colorCount=((int)counter);
}

