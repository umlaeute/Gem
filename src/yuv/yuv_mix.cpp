/*
 *  yuv_mix.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "yuv_mix.h"
#define clamp(x) ((x < 16) ? 16 : (x > 235) ? 235 : x)
CPPEXTERN_NEW(yuv_mix)

/////////////////////////////////////////////////////////
//
// yuv_mix
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_mix :: yuv_mix()
{
m_processOnOff=0;
imageGain=1;
rightGain=1;
post("yuv_mix created");
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_mix :: ~yuv_mix()
{

}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void yuv_mix :: processDualImage(imageStruct &image, imageStruct &right)
{

}



/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////

void yuv_mix :: processDualYUV(imageStruct &image, imageStruct &right)
{
   long src,h,w, width;
   int	y1,y2;
   int u,v,u1,u2,v1,v2;
   int Rgain, Igain;
   
   src =0;
   Rgain = 255/rightGain;
   Igain = 255/imageGain;
   width = image.xsize/2;
   //a = yuv1[0][i] + (2*yuv2[0][i]) - 235;
   //format is U Y V Y
   
       for (h=0; h<image.ysize; h++){
        for(w=0; w<width; w++){
        
        
         u = (((image.data[src] - 128) * imageGain)>>8)+128;
         u1 = (((right.data[src] - 128) * rightGain)>>8)+128;
         u = u + ((2*u1) -255);
        image.data[src] = (unsigned char)clamp(u);
        
        y1 = ((image.data[src+1] * imageGain)>>8) + ((right.data[src+1] * rightGain)>>8);
        image.data[src+1] = (unsigned char)clamp(y1);
        
       v = (((image.data[src+2] - 128) * imageGain)>>8)+128;
       v1 = (((right.data[src+2] - 128) * rightGain)>>8)+128;
       v = v + (2*v1) - 255;
        image.data[src+2] = (unsigned char)clamp(v);

        y2 = ((image.data[src+3] * imageGain)>>8) + ((right.data[src+3] * rightGain)>>8);;
        image.data[src+3] = (unsigned char)clamp(y2);

        src += 4;
        
        }
      }
}

void yuv_mix ::	processRightYUV(imageStruct &image, imageStruct &right)
{
post("yuv_mix: processRightGray");
}
        
void yuv_mix ::	processLeftYUV(imageStruct &image, imageStruct &right)
{
post("yuv_mix: processLeftGray");
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_mix :: obj_setupCallback(t_class *classPtr)
{

    class_addmethod(classPtr, (t_method)&yuv_mix::gainCallback,
		  gensym("gain"), A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

void yuv_mix :: gainCallback(void *data, t_floatarg X, t_floatarg Y)
{
  GetMyClass(data)->imageGain=((int)X);
  GetMyClass(data)->rightGain=((int)Y);

}
