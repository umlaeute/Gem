/*
 *  yuv_bitshift.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "yuv_bitshift.h"
#define clamp(x) ((x < 0) ? 0 : (x > 4) ? 4 : x)
CPPEXTERN_NEW(yuv_bitshift)

/////////////////////////////////////////////////////////
//
// yuv_bitshift
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_bitshift :: yuv_bitshift()
{
m_processOnOff=0;

}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_bitshift :: ~yuv_bitshift()
{

}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void yuv_bitshift :: processImage(imageStruct &image)
{

}


/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void yuv_bitshift :: processYUVImage(imageStruct &image)
{
    int h,w;
    long src;
    src = 0;
    Y = clamp(Y);
    U = clamp(U);
    V = clamp(V);

    //format is U Y V Y

    for (h=0; h<image.ysize; h++){
        for(w=0; w<image.xsize/2; w++){
            image.data[src] = image.data[src+U];
            image.data[src+1] = image.data[src+Y];
            image.data[src+2] = image.data[src+V];
            image.data[src+3] = image.data[src+Y];
            src+=4;
        }
    }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_bitshift :: obj_setupCallback(t_class *classPtr)
{
class_addmethod(classPtr, (t_method)&yuv_bitshift::yMessCallback,
    	    gensym("Yvalue"), A_FLOAT, A_NULL);
            
class_addmethod(classPtr, (t_method)&yuv_bitshift::uMessCallback,
    	    gensym("Uvalue"), A_FLOAT, A_NULL);

class_addmethod(classPtr, (t_method)&yuv_bitshift::vMessCallback,
    	    gensym("Vvalue"), A_FLOAT, A_NULL);

}

void yuv_bitshift :: yMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->Y=((unsigned char)size);
}

void yuv_bitshift :: uMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->U=((unsigned char)size);
}

void yuv_bitshift :: vMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->V=((unsigned char)size);
}


