/*
 *  yuv_bandw.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "yuv_bandw.h"

CPPEXTERN_NEW(yuv_bandw)

/////////////////////////////////////////////////////////
//
// yuv_bandw
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_bandw :: yuv_bandw()
{
post("yuv_bandw: i am being constructed");
inletY = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("Yvalue"));
inletU = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("Uvalue"));
inletV = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("Vvalue"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_bandw :: ~yuv_bandw()
{

}

void yuv_bandw :: yMess(float size)
{
    Y = (char)size;
    setModified();
}

void yuv_bandw :: uMess(float size)
{
    U = (char)size;
    setModified();
}

void yuv_bandw :: vMess(float size)
{
    V = (char)size;
    setModified();
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void yuv_bandw :: processImage(imageStruct &image)
{
post("yuv_bandw: i am processing");
}


/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void yuv_bandw :: processYUVImage(imageStruct &image)
{
    int h,w;
    long src;

//post("yuv_bandw X:%i",image.xsize);
//post("yuv_bandw Y:%i",image.ysize);
//post("yuv_bandw: i am processing gray image");
src = 0;

//format is U Y V Y

for (h=0; h<image.ysize; h++){
    for(w=0; w<image.xsize; w++){
    
    //image.data[src] = Y;
    image.data[src] = 128;
    src+=2;
    
    }

}
//post("h %i",h);
//post("w %i",w);
//post("total %i",src);

}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_bandw :: obj_setupCallback(t_class *classPtr)
{
class_addmethod(classPtr, (t_method)&yuv_bandw::yMessCallback,
    	    gensym("Yvalue"), A_FLOAT, A_NULL);
            
class_addmethod(classPtr, (t_method)&yuv_bandw::uMessCallback,
    	    gensym("Uvalue"), A_FLOAT, A_NULL);

class_addmethod(classPtr, (t_method)&yuv_bandw::vMessCallback,
    	    gensym("Vvalue"), A_FLOAT, A_NULL);

}

void yuv_bandw :: yMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->yMess((float)size);
}

void yuv_bandw :: uMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->uMess((float)size);
}

void yuv_bandw :: vMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->vMess((float)size);
}
