/*
 *  yuv_set.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "yuv_set.h"

CPPEXTERN_NEW(yuv_set)

/////////////////////////////////////////////////////////
//
// yuv_set
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_set :: yuv_set()
{
post("yuv_set: i am being constructed");
inletY = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("Yvalue"));
inletU = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("Uvalue"));
inletV = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("Vvalue"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_set :: ~yuv_set()
{

}

void yuv_set :: yMess(float size)
{
    Y = (unsigned char)size;
    setModified();
}

void yuv_set :: uMess(float size)
{
    U = (unsigned char)size;
    setModified();
}

void yuv_set :: vMess(float size)
{
    V = (unsigned char)size;
    setModified();
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void yuv_set :: processImage(imageStruct &image)
{
}


/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void yuv_set :: processYUVImage(imageStruct &image)
{
    int h,w;
    long src;


src = 0;

//format is U Y V Y

for (h=0; h<image.ysize; h++){
    for(w=0; w<image.xsize/2; w++){
    
    image.data[src] =  U;
    if (image.data[src] > 235) {image.data[src] = 235;}
    if (image.data[src] < 16) {image.data[src] = 16;}
    image.data[src+1] =  Y;
    if (image.data[src+1] > 235) {image.data[src+1] = 235;}
    if (image.data[src+1] < 16) {image.data[src+1] = 16;}
    image.data[src+2] = V;
    if (image.data[src+2] > 235) {image.data[src+2] = 235;}
    if (image.data[src+2] < 16) {image.data[src+2] = 16;}
    image.data[src+3] =  Y;
    if (image.data[src+3] > 235) {image.data[src+3] = 235;}
    if (image.data[src+3] < 16) {image.data[src+3] = 16;}
    src+=4;
    
    }

}


}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_set :: obj_setupCallback(t_class *classPtr)
{
class_addmethod(classPtr, (t_method)&yuv_set::yMessCallback,
    	    gensym("Yvalue"), A_FLOAT, A_NULL);
            
class_addmethod(classPtr, (t_method)&yuv_set::uMessCallback,
    	    gensym("Uvalue"), A_FLOAT, A_NULL);

class_addmethod(classPtr, (t_method)&yuv_set::vMessCallback,
    	    gensym("Vvalue"), A_FLOAT, A_NULL);

}

void yuv_set :: yMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->yMess((float)size);
}

void yuv_set :: uMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->uMess((float)size);
}

void yuv_set :: vMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->vMess((float)size);
}

