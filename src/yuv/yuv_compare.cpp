/*
 *  yuv_compare.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "yuv_compare.h"
#define clamp(x) ((x < 16) ? 16 : (x > 235) ? 235 : x)
CPPEXTERN_NEW(yuv_compare)

/////////////////////////////////////////////////////////
//
// yuv_compare
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_compare :: yuv_compare()
{
m_processOnOff=0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_compare :: ~yuv_compare()
{

}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void yuv_compare :: processDualImage(imageStruct &image, imageStruct &right)
{
    post("yuv_compare: no RGB support :-P");
}


/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////


void yuv_compare :: processDualYUV(imageStruct &image, imageStruct &right)
{
   long src,h,w;
   src =0;

   //format is U Y V Y
    if (m_direction) {
    
    for (h=0; h<image.ysize; h++){
        for(w=0; w<image.xsize/2; w++){
        
            if ((image.data[src+1] < right.data[src+1])&&(image.data[src+3] < right.data[src+3]))
                {
                image.data[src] = right.data[src];
                image.data[src+1] = right.data[src+1];
            
                image.data[src+2] = right.data[src+2];
                image.data[src+3] = right.data[src+3];
                }
        src+=4;
            }
        }
    } else {
        
     for (h=0; h<image.ysize; h++){
        for(w=0; w<image.xsize/2; w++){
        
            if ((image.data[src+1] > right.data[src+1])&&(image.data[src+3] > right.data[src+3]))
                {
                image.data[src] = right.data[src];
                image.data[src+1] = right.data[src+1];
            
                image.data[src+2] = right.data[src+2];
                image.data[src+3] = right.data[src+3];
                }
        src+=4;
            }
        }   
        
    }
}

void yuv_compare ::	processRightYUV(imageStruct &image, imageStruct &right)
{
post("yuv_compare: processRightYUV");
}
        
void yuv_compare ::	processLeftYUV(imageStruct &image, imageStruct &right)
{
post("yuv_compare: processLeftYUV");
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_compare :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&yuv_compare::directionCallback,
		  gensym("direction"), A_DEFFLOAT, A_NULL);
}

void yuv_compare :: directionCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->m_direction=!(!(int)state);
}
