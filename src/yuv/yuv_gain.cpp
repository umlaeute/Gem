/*
 *  yuv_gain.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "yuv_gain.h"
#define clamp(x) ((x < 0) ? 0 : (x > 255) ? 255 : x)
CPPEXTERN_NEW(yuv_gain)

/////////////////////////////////////////////////////////
//
// yuv_gain
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_gain :: yuv_gain()
{
inletGain = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("Gainvalue"));
G = 1;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_gain :: ~yuv_gain()
{

}

void yuv_gain :: gainMess(float gain)
{
    G = gain;
    setModified();
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void yuv_gain :: processImage(imageStruct &image)
{

}

/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void yuv_gain :: processYUVImage(imageStruct &image)
{
    int h,w,width;
    long src;
    float y1,y2,u,v;
    
    width = image.xsize/2;
    for (h=0; h<image.ysize; h++){
        for(w=0; w<width; w++){
    
        //U
        if (image.data[src] > 128) {
            u = (image.data[src] - ((image.data[src] - 128) * (1 - G)));
            }
        if (image.data[src] < 128) {
           u = ((128 - image.data[src]) * (1 - G)) + image.data[src];
           }
        image.data[src] = (unsigned char)clamp(u);

        //Y1
        y1 = image.data[src+1] * G;
        image.data[src+1] = (unsigned char)clamp(y1);

       //V
       //v = image.data[src+2] * G;
       if (image.data[src+2] > 128) {
            v = ((float)image.data[src+2] - ((image.data[src+2] - 128) * (1 - G)));
            }
       if (image.data[src+2] < 128) {
            v = (((128 - image.data[src+2]) * (1 - G)) + image.data[src+2]);
            }
        image.data[src+2] = (unsigned char)clamp(v);

        //Y2
        y2 = image.data[src+3] * G;
       image.data[src+3] = (unsigned char)clamp(y2);
        src+=4;
    
        }
    }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_gain :: obj_setupCallback(t_class *classPtr)
{
class_addmethod(classPtr, (t_method)&yuv_gain::gainMessCallback,
    	    gensym("Gainvalue"), A_FLOAT, A_NULL);
}

void yuv_gain :: gainMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->gainMess((float)size);
}

