/*
 *  yuv_pixel_isolate.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "yuv_pixel_isolate.h"
#define clamp(x) ((x < 0) ? 0 : (x > 255) ? 255 : x)

/////////////////////////////////////////////////////////
//
// yuv_pixel_isolate
CPPEXTERN_NEW(yuv_pixel_isolate)

/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_pixel_isolate :: yuv_pixel_isolate()
{

}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_pixel_isolate :: ~yuv_pixel_isolate()
{

}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void yuv_pixel_isolate :: processImage(imageStruct &image)
{

}


/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void yuv_pixel_isolate :: processYUVImage(imageStruct &image)
{
    long src,h,w;
    unsigned char Uhi,Ulo,Vhi,Vlo,Yhi,Ylo;
    src =0;

   Yhi = clamp(m_Yvalue + m_Yrange); 
   Ylo = clamp(m_Yvalue - m_Yrange);
   Uhi = clamp(m_Uvalue + m_Urange); 
   Ulo = clamp(m_Uvalue - m_Urange);
   Vhi = clamp(m_Vvalue + m_Vrange); 
   Vlo = clamp(m_Vvalue - m_Vrange);
   //format is U Y V Y
    
    for (h=0; h<image.ysize; h++){
        for(w=0; w<image.xsize/2; w++){
        
            if (!((image.data[src] < Uhi)&&(image.data[src] > Ulo)&&
            (image.data[src+1] < Yhi)&&(image.data[src+1] > Ylo)&&
            (image.data[src+2] < Vhi)&&(image.data[src+2] > Vlo)))
                {
                image.data[src] = 128;
                //image.data[src+1] = m_Yreplace;
            
                image.data[src+2] = 128;
                //image.data[src+3] = m_Yreplace;
                }
        src+=4;
            }
        }

}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_pixel_isolate :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&yuv_pixel_isolate::rangeCallback,
		  gensym("range"), A_DEFFLOAT,A_DEFFLOAT,A_DEFFLOAT, A_NULL);
                  
    class_addmethod(classPtr, (t_method)&yuv_pixel_isolate::valueCallback,
		  gensym("value"), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);
    
    class_addmethod(classPtr, (t_method)&yuv_pixel_isolate::replaceCallback,
		  gensym("replace"), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

void yuv_pixel_isolate :: rangeCallback(void *data, t_floatarg Yval, t_floatarg Uval, t_floatarg Vval)
{
  GetMyClass(data)->m_Yrange=((unsigned char)Yval);
  GetMyClass(data)->m_Urange=((unsigned char)Uval);
  GetMyClass(data)->m_Vrange=((unsigned char)Vval);

}

void yuv_pixel_isolate :: valueCallback(void *data, t_floatarg Yval, t_floatarg Uval, t_floatarg Vval)
{
  GetMyClass(data)->m_Yvalue=((unsigned char)Yval);
  GetMyClass(data)->m_Uvalue=((unsigned char)Uval);
  GetMyClass(data)->m_Vvalue=((unsigned char)Vval);
}

void yuv_pixel_isolate :: replaceCallback(void *data, t_floatarg Yrep, t_floatarg Urep, t_floatarg Vrep)
{
  GetMyClass(data)->m_Yreplace=((unsigned char)Yrep);
  GetMyClass(data)->m_Ureplace=((unsigned char)Urep);
  GetMyClass(data)->m_Vreplace=((unsigned char)Vrep);
}
