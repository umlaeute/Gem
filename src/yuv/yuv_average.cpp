/*
 *  yuv_average.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "yuv_average.h"
#define clamp(x) ((x < 16) ? 16 : (x > 235) ? 235 : x)
CPPEXTERN_NEW(yuv_average)

/////////////////////////////////////////////////////////
//
// yuv_average
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_average :: yuv_average()
{
m_processOnOff=0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_average :: ~yuv_average()
{

}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void yuv_average :: processDualImage(imageStruct &image, imageStruct &right)
{
    post("yuv_average: no RGB support :-P");
}



/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////

void yuv_average :: processDualYUV(imageStruct &image, imageStruct &right)
{
   long src,h,w;
   int	y1,y2;
   int u,v;
   src =0;
   //a = yuv1[0][i] + (2*yuv2[0][i]) - 235;
   //format is U Y V Y
   for (h=0; h<image.ysize; h++){
    for(w=0; w<image.xsize/2; w++){
        
        //u = image.data[src] + ((2*right.data[src]) - 255);
        u =image.data[src] + right.data[src];
        image.data[src] = u/2;

        y1 =image.data[src+1] + right.data[src+1];
        image.data[src+1] = y1/2;

        v =image.data[src+2] + right.data[src+2];
        image.data[src+2] = v/2;

        y2 = image.data[src+3] + right.data[src+3];
        image.data[src+3] = y2/2;
       
        src+=4;
    }
    }
}

void yuv_average ::	processRightYUV(imageStruct &image, imageStruct &right)
{
post("yuv_average: processRightYUV");
}
        
void yuv_average ::	processLeftYUV(imageStruct &image, imageStruct &right)
{
post("yuv_average: processLeftYUV");
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_average :: obj_setupCallback(t_class *classPtr)
{

}
