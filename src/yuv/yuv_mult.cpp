/*
 *  yuv_mult.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "yuv_mult.h"
#define clamp(x) ((x < 16) ? 16 : (x > 235) ? 235 : x)
CPPEXTERN_NEW(yuv_mult)

/////////////////////////////////////////////////////////
//
// yuv_mult
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_mult :: yuv_mult()
{

}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_mult :: ~yuv_mult()
{

}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void yuv_mult :: processDualImage(imageStruct &image, imageStruct &right)
{
    post("yuv_mult: no RGB support :-P");
}



/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void yuv_mult :: processDualYUV(imageStruct &image, imageStruct &right)
{
   long src,h,w;
   int	y1,y2;
   src =0;
   //format is U Y V Y
   for (h=0; h<image.ysize; h++){
    for(w=0; w<image.xsize/2; w++){
        y1 = (image.data[src+1] * right.data[src+1]) >> 8;
        image.data[src+1] = clamp(y1);
        y2 = (image.data[src+3] * right.data[src+3]) >> 8;
        image.data[src+3] = clamp(y2);
        
        src+=4;
    }
    }
}

void yuv_mult ::	processRightYUV(imageStruct &image, imageStruct &right)
{
    post("yuv_mult: processRightYUV");
}
        
void yuv_mult ::	processLeftYUV(imageStruct &image, imageStruct &right)
{
    post("yuv_mult: processLeftYUV");
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_mult :: obj_setupCallback(t_class *classPtr)
{

}
