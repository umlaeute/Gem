/*
 *  yuv_adjust.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "yuv_adjust.h"
#define clamp(x) ((x < 0) ? 0 : (x > 255) ? 255 : x)
CPPEXTERN_NEW(yuv_adjust)

/////////////////////////////////////////////////////////
//
// yuv_adjust
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_adjust :: yuv_adjust()
{
//post("yuv_adjust: i am being constructed");
Y=0;
U=0;
V=0;
inletY = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("Yvalue"));
inletU = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("Uvalue"));
inletV = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("Vvalue"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_adjust :: ~yuv_adjust()
{

}

/////////////////////////////////////////////////////////
// messages
//
/////////////////////////////////////////////////////////

void yuv_adjust :: yMess(float size)
{
    Y = (short)size;
    //Y = size;
    setModified();
}

void yuv_adjust :: uMess(float size)
{
    U = (short)size;
   // U = size;
    setModified();
}

void yuv_adjust :: vMess(float size)
{
    V = (short)size;
   // V = size;
    setModified();
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void yuv_adjust :: processImage(imageStruct &image)
{
}


/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void yuv_adjust :: processYUVImage(imageStruct &image)
{
    int h,w;
    long src;

//post("yuv_adjust X:%i",image.xsize);
//post("yuv_adjust Y:%i",image.ysize);
//post("yuv_adjust: i am processing gray image");
src = 0;

//format is U Y V Y

for (h=0; h<image.ysize; h++){
    for(w=0; w<image.xsize/2; w++){
        image.data[src] = clamp( image.data[src] + U );
        image.data[src+1] = clamp( image.data[src+1] + Y );
        image.data[src+2] = clamp( image.data[src+2] + V );
        image.data[src+3] = clamp( image.data[src+3] + Y );

        src+=4;
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
void yuv_adjust :: obj_setupCallback(t_class *classPtr)
{
class_addmethod(classPtr, (t_method)&yuv_adjust::yMessCallback,
    	    gensym("Yvalue"), A_FLOAT, A_NULL);
            
class_addmethod(classPtr, (t_method)&yuv_adjust::uMessCallback,
    	    gensym("Uvalue"), A_FLOAT, A_NULL);

class_addmethod(classPtr, (t_method)&yuv_adjust::vMessCallback,
    	    gensym("Vvalue"), A_FLOAT, A_NULL);

}

void yuv_adjust :: yMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->yMess((float)size);
}

void yuv_adjust :: uMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->uMess((float)size);
}

void yuv_adjust :: vMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->vMess((float)size);
}

