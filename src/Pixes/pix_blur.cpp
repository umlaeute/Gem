/*
 *  pix_blur.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "pix_blur.h"
#define clamp(x) ((x < 16) ? 16 : (x > 235) ? 235 : x)
CPPEXTERN_NEW(pix_blur)

/////////////////////////////////////////////////////////
//
// pix_blur
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_blur :: pix_blur()
{	long size,src,i;
    
inletBlur = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("blur"));

m_blur = 0;
m_blurH = 240;
m_blurW = 240;
m_blurBpp = 2;
size = 320 * 240 * 4;
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
pix_blur :: ~pix_blur()
{
delete saved;
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_blur :: processRGBAImage(imageStruct &image)
{
       int h,w,hlength;
    long src;
    int R,G,B,A;
    int rightGain,imageGain, Rgain,Igain;
    unsigned char *pixels=image.data;

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
   hlength = image.xsize;


for (h=0; h<image.ysize; h++){
    for(w=0; w<hlength; w++){
    

      
        R = ((pixels[src+chRed] * imageGain)>>8) + ((saved[src+chRed] * rightGain)>>8);
        saved[src+chRed] = (unsigned char)clamp(R);
        pixels[src+chRed] = saved[src+chRed];
        
         G = ((pixels[src+chGreen] * imageGain)>>8) + ((saved[src+chGreen] * rightGain)>>8);;
        saved[src+chGreen] = (unsigned char)clamp(G);
        pixels[src+chGreen] = saved[src+chGreen];
 
         B = ((pixels[src+chBlue] * imageGain)>>8) + ((saved[src+chBlue] * rightGain)>>8);;
        saved[src+chBlue] = (unsigned char)clamp(B);
        pixels[src+chBlue] = saved[src+chBlue];

        src += 4;

     
    }
}

}


/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void pix_blur :: processYUVImage(imageStruct &image)
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

}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_blur :: obj_setupCallback(t_class *classPtr)
{

    class_addmethod(classPtr, (t_method)&pix_blur::blurCallback,
		  gensym("blur"), A_DEFFLOAT, A_NULL);
}

void pix_blur :: blurCallback(void *data, t_floatarg value)
{
  GetMyClass(data)->m_blur=((long)value);

}

