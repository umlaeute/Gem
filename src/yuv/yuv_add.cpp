/*
 *  yuv_add.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "yuv_add.h"
#define clamp(x) ((x < 16) ? 16 : (x > 235) ? 235 : x)
CPPEXTERN_NEW(yuv_add)

/////////////////////////////////////////////////////////
//
// yuv_add
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_add :: yuv_add()
{

}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_add :: ~yuv_add()
{

}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void yuv_add :: processDualImage(imageStruct &image, imageStruct &right)
{
    post("yuv_add: no RGB support :-P");
}



/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void yuv_add :: processDualYUV(imageStruct &image, imageStruct &right)
{
   long src,h,w;
   int	y1,y2;
   int u,v;
   src =0;
   //format is U Y V Y
   for (h=0; h<image.ysize; h++){
    for(w=0; w<image.xsize/2; w++){
        
        u = image.data[src] + (2*right.data[src]) - 255;
        image.data[src] = clamp(u);
        y1 =image.data[src+1] + right.data[src+1];
        image.data[src+1] = clamp(y1);
        v = image.data[src+2] + (2*right.data[src+2]) - 255;
        image.data[src+2] = clamp(v);
        y2 = image.data[src+3] + right.data[src+3];
        image.data[src+3] = clamp(y2);
       
        src+=4;
    }
    }
}

void yuv_add ::	processRightYUV(imageStruct &image, imageStruct &right)
{
    post("yuv_add: processRightYUV");
}
        
void yuv_add ::	processLeftYUV(imageStruct &image, imageStruct &right)
{
    post("yuv_add: processLeftYUV");
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_add :: obj_setupCallback(t_class *classPtr)
{

}
