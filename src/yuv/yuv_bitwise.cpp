/*
 *  yuv_bitwise.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "yuv_bitwise.h"
#define clamp(x) ((x < 16) ? 16 : (x > 235) ? 235 : x)
CPPEXTERN_NEW(yuv_bitwise)

/////////////////////////////////////////////////////////
//
// yuv_bitwise
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_bitwise :: yuv_bitwise()
{
m_mode = 0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_bitwise :: ~yuv_bitwise()
{

}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void yuv_bitwise :: processDualImage(imageStruct &image, imageStruct &right)
{
    post("yuv_bitwise: no RGB support :-P");
}



/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void yuv_bitwise :: processDualYUV(imageStruct &image, imageStruct &right)
{
   long src,h,w;

   
   src =0;
   //format is U Y V Y
   
   if (m_mode == 0){
   
   for (h=0; h<image.ysize; h++){
    for(w=0; w<image.xsize/2; w++){
        

      image.data[src+1] = image.data[src+1] & right.data[src+1];
        
       image.data[src+3]= image.data[src+3] & right.data[src+3];
       
        src+=4;
        }
    }
    } else if (m_mode == 1) {
       for (h=0; h<image.ysize; h++){
        for(w=0; w<image.xsize/2; w++){
        
        image.data[src+1] = image.data[src+1] | right.data[src+1];
        image.data[src+3]= image.data[src+3] | right.data[src+3];

        src+=4;
        }
    }
    } else if (m_mode == 2) {
       for (h=0; h<image.ysize; h++){
        for(w=0; w<image.xsize/2; w++){
        
        image.data[src+1] = image.data[src+1] ^ right.data[src+1];
        image.data[src+3]= image.data[src+3] ^ right.data[src+3];

        src+=4;
        }
    }
    } else if (m_mode == 3){
   
   for (h=0; h<image.ysize; h++){
    for(w=0; w<image.xsize/2; w++){
        

        image.data[src] = image.data[src] ^ ((2*right.data[src]) - 255);
        image.data[src+1] = image.data[src+1] & right.data[src+1];
        image.data[src+2] = image.data[src+2] ^ ((2*right.data[src+2]) - 255);
        image.data[src+3]= image.data[src+3] & right.data[src+3];
       
        src+=4;
        }
    }
    } else if (m_mode == 4) {
       for (h=0; h<image.ysize; h++){
        for(w=0; w<image.xsize/2; w++){
        
        image.data[src] = image.data[src] | ((2*right.data[src]) - 255);
        image.data[src+1] = image.data[src+1] | right.data[src+1];
        image.data[src+2] = image.data[src+2] | ((2*right.data[src+2]) - 255);
        image.data[src+3]= image.data[src+3] | right.data[src+3];

        src+=4;
        }
    }
    } else if (m_mode == 5) {
       for (h=0; h<image.ysize; h++){
        for(w=0; w<image.xsize/2; w++){
        
        image.data[src] = image.data[src] ^ ((2*right.data[src]) - 255);
        image.data[src+1] = image.data[src+1] ^ right.data[src+1];
        image.data[src+2] = image.data[src+2] ^ ((2*right.data[src+2]) - 255);
        image.data[src+3]= image.data[src+3] ^ right.data[src+3];

        src+=4;
        }
    }
    } 
}

void yuv_bitwise ::	processRightYUV(imageStruct &image, imageStruct &right)
{
    post("yuv_bitwise: processRightYUV");
}
        
void yuv_bitwise ::	processLeftYUV(imageStruct &image, imageStruct &right)
{
    post("yuv_bitwise: processLeftYUV");
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_bitwise :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&yuv_bitwise::modeCallback,
		  gensym("mode"), A_DEFFLOAT, A_NULL);
}

void yuv_bitwise :: modeCallback(void *data, t_floatarg value)
{
    GetMyClass(data)->m_mode=((int)value);
}
