/*
 *  yuv_blur.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "yuv_blur.h"
#define clamp(x) ((x < 16) ? 16 : (x > 235) ? 235 : x)
CPPEXTERN_NEW(yuv_blur)

/////////////////////////////////////////////////////////
//
// yuv_blur
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_blur :: yuv_blur()
{	long size,src,i;
    
inletBlur = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("blur"));

m_blur = 0;
m_blurH = 240;
m_blurW = 240;
m_blurBpp = 2;
size = 320 * 240 * 2;
saved = new unsigned char [size];
src=0;
for (i=0;i<size/2;i++)
{
saved[src] = 128;
saved[src+1] = 0;
src += 2;
}
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_blur :: ~yuv_blur()
{

}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void yuv_blur :: processImage(imageStruct &image)
{

}


/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void yuv_blur :: processYUVImage(imageStruct &image)
{
       int h,w,hlength;
    long src;

    int rightGain,imageGain, Rgain,Igain,y1,y2;

src = 0;

if (m_blurH != image.ysize || m_blurW != image.xsize || m_blurBpp != image.csize) {

m_blurH = image.ysize;
m_blurW = image.xsize;
m_blurBpp = image.csize;
m_blurSize = m_blurH * m_blurW * m_blurBpp;
delete saved;
saved = new unsigned char [m_blurSize];

}

rightGain = m_blur;
imageGain = 255 - m_blur;
   Rgain = 255/rightGain;
   Igain = 255/imageGain;
   hlength = image.xsize/2;
//size = image.xsize * image.ysize * image.csize;
//saved = new unsigned char [size];
//format is U Y V Y

for (h=0; h<image.ysize; h++){
    for(w=0; w<hlength; w++){
    

      
        y1 = ((image.data[src+1] * imageGain)>>8) + ((saved[src+1] * rightGain)>>8);
        saved[src+1] = (unsigned char)clamp(y1);
        image.data[src+1] = saved[src+1];
 
 
        
        y2 = ((image.data[src+3] * imageGain)>>8) + ((saved[src+3] * rightGain)>>8);;
        saved[src+3] = (unsigned char)clamp(y2);
        image.data[src+3] = saved[src+3];
  
        src += 4;

     
    }
}

//image.data = saved;

/*
        if (image.data[src] > 128) {
            //u = ((image.data[src] - 128) / (255 / G)) + 128;
            image.data[src] = ((image.data[src] - 128) / (gain)) + 128;
            } else
        if (image.data[src] < 128) {
         //  u = 128 - ((128 - image.data[src]) / (255 / G)) ;
         image.data[src] = 128 - ((128 - image.data[src]) / (gain)) ;
           }
       // image.data[src] = (unsigned char)clamp(u);
       
      
        y1 = (image.data[src+1] * G)>>8;
        image.data[src+1] = (unsigned char)clamp(y1);
        

       if (image.data[src+2] > 128) {
         //   v = ((image.data[src+2] - 128) / (255 / G)) + 128;
        image.data[src+2] = ((image.data[src+2] - 128) / (gain)) + 128;
            } else
       if (image.data[src+2] < 128) {
          // v = 128 - ((128 - image.data[src+2]) / (255 / G)) ;
          image.data[src+2] = 128 - ((128 - image.data[src+2]) / (gain)) ;
           }
       // image.data[src+2] = (unsigned char)clamp(v);
        
        y2 = (image.data[src+3] * G)>>8;
        image.data[src+3] = (unsigned char)clamp(y2); 
*/

//delete saved;

}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_blur :: obj_setupCallback(t_class *classPtr)
{

    class_addmethod(classPtr, (t_method)&yuv_blur::blurCallback,
		  gensym("blur"), A_DEFFLOAT, A_NULL);
}

void yuv_blur :: blurCallback(void *data, t_floatarg value)
{
  GetMyClass(data)->m_blur=((long)value);

}