/*
 *  yuv_pixel_average.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "yuv_pixel_average.h"

CPPEXTERN_NEW(yuv_pixel_average)

/////////////////////////////////////////////////////////
//
// yuv_pixel_average
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_pixel_average :: yuv_pixel_average()
{
    m_processOnOff=0;
    m_width =1;
    m_height = 1;
    m_YOut = outlet_new(this->x_obj, 0);
    m_UOut = outlet_new(this->x_obj, 0);
    m_VOut = outlet_new(this->x_obj, 0);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_pixel_average :: ~yuv_pixel_average()
{
    outlet_free(m_YOut);
    outlet_free(m_UOut);
    outlet_free(m_VOut);
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void yuv_pixel_average :: processImage(imageStruct &image)
{

}

/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void yuv_pixel_average :: processYUVImage(imageStruct &image)
{
   long src,h,w,count;
   int	y,u,v;
   int maskw,maskh,upperx,uppery,lowerx,lowery,maxX,maxY;
   
   src =0;
   count = 0;
   y = u = v = 0;
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
        
        u = u + image.data[src];
        y = y + image.data[src+1];
        v = v + image.data[src+2];
        y = y + image.data[src+3];
       
        src+=4;
        count += 2;
    }
    src+= (image.xsize - maskw) * 2;
    }
    
    YVal = y/(count);
    UVal = u/(count/2);
    VVal = v/(count/2);
    
/*
    YVal = image.data[(m_height*m_width)+1];
    UVal = image.data[m_height*m_width];
    VVal = image.data[(m_height*m_width)+2];
*/
    outlet_float(m_YOut, (t_float)YVal);
    outlet_float(m_UOut, (t_float)UVal);
    outlet_float(m_VOut, (t_float)VVal);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_pixel_average :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&yuv_pixel_average::topCallback,
		  gensym("top"), A_DEFFLOAT, A_DEFFLOAT, A_NULL);
 
    class_addmethod(classPtr, (t_method)&yuv_pixel_average::lowerCallback,
		  gensym("lower"), A_DEFFLOAT, A_DEFFLOAT, A_NULL);                  
                                                    
    class_addmethod(classPtr, (t_method)&yuv_pixel_average::dimenCallback,
		  gensym("dimen"), A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

void yuv_pixel_average :: topCallback(void *data, t_floatarg X, t_floatarg Y)
{
  GetMyClass(data)->topX=((long)X);
  GetMyClass(data)->topY=((long)Y);

}

void yuv_pixel_average :: lowerCallback(void *data, t_floatarg X, t_floatarg Y)
{
  GetMyClass(data)->lowX=((long)X);
  GetMyClass(data)->lowY=((long)Y);

}

void yuv_pixel_average :: dimenCallback(void *data, t_floatarg X, t_floatarg Y)
{
  GetMyClass(data)->dimenX=((long)X);
  GetMyClass(data)->dimenY=((long)Y);

}
