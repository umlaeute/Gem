/*
 *  yuv_subtract.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "yuv_subtract.h"
#define clamp(x) ((x < 16) ? 16 : (x > 235) ? 235 : x)
CPPEXTERN_NEW(yuv_subtract)

/////////////////////////////////////////////////////////
//
// yuv_subtract
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_subtract :: yuv_subtract()
{
m_processOnOff=0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_subtract :: ~yuv_subtract()
{

}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void yuv_subtract :: processDualImage(imageStruct &image, imageStruct &right)
{
    post("yuv_subtract: no RGB support :-P");
}



/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////

void yuv_subtract :: processDualYUV(imageStruct &image, imageStruct &right)
{
   long src,h,w;
   int	y1,y2;
   int u,v;
   src =0;
   //format is U Y V Y
   for (h=0; h<image.ysize; h++){
    for(w=0; w<image.xsize/2; w++){
        
        u = image.data[src] - ((2*right.data[src]) - 255);
        image.data[src] = clamp(u);

        y1 =image.data[src+1] - right.data[src+1];
        image.data[src+1] = clamp(y1);
        v = image.data[src+2] - ((2*right.data[src+2]) - 255);
        image.data[src+2] = clamp(v);

        y2 = image.data[src+3] - right.data[src+3];
        image.data[src+3] = clamp(y2);
       
        src+=4;
    }
    }
}

void yuv_subtract ::	processRightYUV(imageStruct &image, imageStruct &right)
{
    post("yuv_subtract: processRightYUV");
}
        
void yuv_subtract ::	processLeftYUV(imageStruct &image, imageStruct &right)
{
    post("yuv_subtract: processLeftYUV");
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_subtract :: obj_setupCallback(t_class *classPtr)
{

}
