/*
 *  yuv_clamp.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "yuv_clamp.h"
#define clamp(x) ((x < 0) ? 0 : (x > 255) ? 255 : x)
CPPEXTERN_NEW(yuv_clamp)

/////////////////////////////////////////////////////////
//
// yuv_clamp
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_clamp :: yuv_clamp()
{
Yhi = 255;
Ylo = 0;
Uhi = 255;
Ulo = 0;
Vhi = 255;
Vlo = 0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_clamp :: ~yuv_clamp()
{

}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void yuv_clamp :: processImage(imageStruct &image)
{

}


/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void yuv_clamp :: processYUVImage(imageStruct &image)
{
    long h,w,src;
/*
    clamp(Yhi);
    clamp(Ylo);
    clamp(Uhi);
    clamp(Ulo);
    clamp(Vhi);
    clamp(Vlo);
*/    
src = 0;
    for (h=0; h<image.ysize; h++){
        for(w=0; w<image.xsize/2; w++){
        
        if (image.data[src] < Ulo) {image.data[src] = Ulo;}
        if (image.data[src] > Uhi) {image.data[src] = Uhi;}
        
        if (image.data[src+1] < Ylo) {image.data[src+1] = Ylo;}
        if (image.data[src+1] > Yhi) {image.data[src+1] = Yhi;}
        
        if (image.data[src+2] < Vlo) {image.data[src+2] = Vlo;}
        if (image.data[src+2] > Vhi) {image.data[src+2] = Vhi;}

        if (image.data[src+3] < Ylo) {image.data[src+3] = Ylo;}
        if (image.data[src+3] > Yhi) {image.data[src+3] = Yhi;}

        src+=4;
        
        }
    }

}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_clamp :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&yuv_clamp::YClampCallback,
		  gensym("Y"), A_DEFFLOAT, A_DEFFLOAT, A_NULL);
                  
    class_addmethod(classPtr, (t_method)&yuv_clamp::UClampCallback,
		  gensym("U"), A_DEFFLOAT, A_DEFFLOAT, A_NULL);
                  
    class_addmethod(classPtr, (t_method)&yuv_clamp::UClampCallback,
		  gensym("V"), A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

void yuv_clamp :: YClampCallback(void *data, t_floatarg Loval, t_floatarg Hival)
{
  GetMyClass(data)->Yhi=((long)Hival);
  GetMyClass(data)->Ylo=((long)Loval);
}

void yuv_clamp :: UClampCallback(void *data, t_floatarg Loval, t_floatarg Hival)
{
  GetMyClass(data)->Uhi=((long)Hival);
  GetMyClass(data)->Ulo=((long)Loval);
}

void yuv_clamp :: VClampCallback(void *data, t_floatarg Loval, t_floatarg Hival)
{
  GetMyClass(data)->Vhi=((long)Hival);
  GetMyClass(data)->Vlo=((long)Loval);
}


