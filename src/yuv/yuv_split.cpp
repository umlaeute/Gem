/*
 *  yuv_split.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "yuv_split.h"
#define clamp(x) ((x < 16) ? 16 : (x > 235) ? 235 : x)
CPPEXTERN_NEW(yuv_split)

/////////////////////////////////////////////////////////
//
// yuv_split
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_split :: yuv_split()
{
m_processOnOff=0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_split :: ~yuv_split()
{

}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void yuv_split :: processDualImage(imageStruct &image, imageStruct &right)
{

}



/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////

void yuv_split :: processDualYUV(imageStruct &image, imageStruct &right)
{
   long src,h,w;
//   int	y1,y2,u,v;
   
   //src =0;
   src = (image.xsize*image.ysize);
   //a = yuv1[0][i] + (2*yuv2[0][i]) - 235;
   //format is U Y V Y
   for (h=0; h<image.ysize/2; h++){
    for(w=0; w<image.xsize/2; w++){
        
        image.data[src] = right.data[src];
        image.data[src+1] = right.data[src+1];
        image.data[src+2] = right.data[src+2];
        image.data[src+3] = right.data[src+3];
       
        src+=4;
    }
    }
}

void yuv_split ::	processRightYUV(imageStruct &image, imageStruct &right)
{
post("yuv_split: processRightGray");
}
        
void yuv_split ::	processLeftYUV(imageStruct &image, imageStruct &right)
{
post("yuv_split: processLeftGray");
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_split :: obj_setupCallback(t_class *classPtr)
{

}
