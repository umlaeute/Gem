/*
 *  yuv_mask.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "yuv_mask.h"
#define clamp(x) ((x < 16) ? 16 : (x > 235) ? 235 : x)
CPPEXTERN_NEW(yuv_mask)

/////////////////////////////////////////////////////////
//
// yuv_mask
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_mask :: yuv_mask()
{
m_processOnOff=0;
topX = 0;
topY = 0;
dimenX = 0;
dimenY = 0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_mask :: ~yuv_mask()
{

}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void yuv_mask :: processDualImage(imageStruct &image, imageStruct &right)
{

}



/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////

void yuv_mask :: processDualYUV(imageStruct &image, imageStruct &right)
{
   long src,h,w;
//   int	y1,y2, u,v;
   int maskw,maskh,upperx,uppery,lowerx,lowery,maxX,maxY;
   
   src =0;
   maskw=dimenX;
   maskh=dimenY;
   
   if (topX % 2 == 1) {topX = topX + 1;}
   if (maskw % 2 == 1) {maskw = maskw + 1;} //only even pixels on the x axis
   if (maskw + topX < 0) {maskw = 0;}
//   if (maskw < 0) {maskw = 0;}
   if (maskw + topX > image.xsize) {maskw = image.xsize - topX;}
   if (maskh + topY < 0) {maskh = 0;}
//   if (maskh < 0) {maskh = 0;}
   if (maskh + topY > image.ysize) {maskh = image.ysize - topY;}
   
   maxX = image.xsize-maskw;
   maxY = image.ysize-maskh;
   upperx = ((topX < 0) ? 0 : (topX > maxX) ? maxX : topX)*2;
   uppery = ((topY < 0) ? 0 : (topY > maxY) ? maxY : topY);
   lowerx = ((lowX < 0) ? 0 : (lowX > image.xsize) ? image.xsize : lowX)*2;
   lowery = ((lowY < 0) ? 0 : (lowY > image.ysize) ? image.ysize : lowY);   
   
   
   
   src = (image.xsize * uppery * 2) + upperx;

   //format is U Y V Y
   for (h=0; h<maskh; h++){
   
    for(w=0; w<maskw/2; w++){
        
        image.data[src] = right.data[src];
        image.data[src+1] = right.data[src+1];
        image.data[src+2] = right.data[src+2];
        image.data[src+3] = right.data[src+3];
       
        src+=4;
    }
    src+= (image.xsize - maskw) * 2;
    }
    
    //invert
    /*
    tempbuf = right
    for loops
    tempbuf[src] = image.data[src]
    image = tempbuf
    */
    
}

void yuv_mask ::	processRightYUV(imageStruct &image, imageStruct &right)
{
post("yuv_mask: processRightGray");
}
        
void yuv_mask ::	processLeftYUV(imageStruct &image, imageStruct &right)
{
post("yuv_mask: processLeftGray");
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_mask :: obj_setupCallback(t_class *classPtr)
{

    class_addmethod(classPtr, (t_method)&yuv_mask::topCallback,
		  gensym("top"), A_DEFFLOAT, A_DEFFLOAT, A_NULL);
 
    class_addmethod(classPtr, (t_method)&yuv_mask::lowerCallback,
		  gensym("lower"), A_DEFFLOAT, A_DEFFLOAT, A_NULL);                  
                                                    
    class_addmethod(classPtr, (t_method)&yuv_mask::dimenCallback,
		  gensym("dimen"), A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

void yuv_mask :: topCallback(void *data, t_floatarg X, t_floatarg Y)
{
  GetMyClass(data)->topX=((long)X);
  GetMyClass(data)->topY=((long)Y);

}

void yuv_mask :: lowerCallback(void *data, t_floatarg X, t_floatarg Y)
{
  GetMyClass(data)->lowX=((long)X);
  GetMyClass(data)->lowY=((long)Y);

}

void yuv_mask :: dimenCallback(void *data, t_floatarg X, t_floatarg Y)
{
  GetMyClass(data)->dimenX=((long)X);
  GetMyClass(data)->dimenY=((long)Y);

}
