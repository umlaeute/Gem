/*
 *  yuv_swap.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "yuv_swap.h"
#define clamp(x) ((x < 16) ? 16 : (x > 235) ? 235 : x)
CPPEXTERN_NEW(yuv_swap)

/////////////////////////////////////////////////////////
//
// yuv_swap
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_swap :: yuv_swap()
{
m_processOnOff=0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_swap :: ~yuv_swap()
{

}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void yuv_swap :: processDualImage(imageStruct &image, imageStruct &right)
{

}



/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////

void yuv_swap :: processDualYUV(imageStruct &image, imageStruct &right)
{
   long src,h,w;
//   int	y1,y2,u,v;
   src =0;
   //a = yuv1[0][i] + (2*yuv2[0][i]) - 235;
   //format is U Y V Y
   for (h=0; h<image.ysize; h++){
    for(w=0; w<image.xsize/2; w++){
        /*
        image.data[src] = right.data[src+U];
        image.data[src+1] = right.data[src+Y];
        image.data[src+2] = right.data[src+V];
        image.data[src+3] = right.data[src+Y];
        */
       if (U) {  
        image.data[src] = right.data[src];}
        if (Y) {
        image.data[src+1] = right.data[src+1];}
        if (V) {
        image.data[src+2] = right.data[src+2];}
        if (Y) {
        image.data[src+3] = right.data[src+3];}
    
        src+=4;
    }
    }
}

void yuv_swap ::	processRightYUV(imageStruct &image, imageStruct &right)
{
post("yuv_swap: processRightGray");
}
        
void yuv_swap ::	processLeftYUV(imageStruct &image, imageStruct &right)
{
post("yuv_swap: processLeftGray");
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_swap :: obj_setupCallback(t_class *classPtr)
{
class_addmethod(classPtr, (t_method)&yuv_swap::yMessCallback,
    	    gensym("Yvalue"), A_FLOAT, A_NULL);
            
class_addmethod(classPtr, (t_method)&yuv_swap::uMessCallback,
    	    gensym("Uvalue"), A_FLOAT, A_NULL);

class_addmethod(classPtr, (t_method)&yuv_swap::vMessCallback,
    	    gensym("Vvalue"), A_FLOAT, A_NULL);

}

void yuv_swap :: yMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->Y=((unsigned char)size);
}

void yuv_swap :: uMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->U=((unsigned char)size);
}

void yuv_swap :: vMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->V=((unsigned char)size);
}