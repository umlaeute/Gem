/*
 *  yuv_difference.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */
#include <stdlib.h>
#include "yuv_difference.h"
#define clamp(x) ((x < 16) ? 16 : (x > 235) ? 235 : x)
CPPEXTERN_NEW(yuv_difference)

/////////////////////////////////////////////////////////
//
// yuv_difference
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_difference :: yuv_difference()
{
m_processOnOff=0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_difference :: ~yuv_difference()
{

}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void yuv_difference :: processDualImage(imageStruct &image, imageStruct &right)
{
    post("yuv_difference: no RGB support :-P");
}



/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////

void yuv_difference :: processDualYUV(imageStruct &image, imageStruct &right)
{
   long src,h,w;
   int	y1,y2;
   int u,v;
   src =0;
   //format is U Y V Y
   for (h=0; h<image.ysize; h++){
    for(w=0; w<image.xsize/2; w++){
        
        u = image.data[src] - ((2*right.data[src]) - 255);
        image.data[src] = abs(u);
        y1 =image.data[src+1] - right.data[src+1];
        image.data[src+1] = abs(y1);
        v = image.data[src+2] - ((2*right.data[src+2]) - 255);
        image.data[src+2] = abs(v);
        y2 = image.data[src+3] - right.data[src+3];
        image.data[src+3] = abs(y2);
       
        src+=4;
    }
    }
}

void yuv_difference ::	processRightYUV(imageStruct &image, imageStruct &right)
{
post("yuv_difference: processRightYUV");
}
        
void yuv_difference ::	processLeftYUV(imageStruct &image, imageStruct &right)
{
post("yuv_difference: processLeftYUV");
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_difference :: obj_setupCallback(t_class *classPtr)
{

}
