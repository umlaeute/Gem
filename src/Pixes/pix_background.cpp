/*
 *  pix_background.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "pix_background.h"
#define clamp(x) ((x < 16) ? 16 : (x > 235) ? 235 : x)
CPPEXTERN_NEW(pix_background)

/////////////////////////////////////////////////////////
//
// pix_background
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_background :: pix_background()
{	long size,src,i;
    
inletBlur = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("blur"));

Yrange = 0;
Urange = 0;
Vrange = 0;
m_blur = 0;
m_reset = 0;
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
pix_background :: ~pix_background()
{
delete saved;
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_background :: processRGBAImage(imageStruct &image)
{
       int h,w,hlength;
    long src,pixsize;
    //int R,G,B,A;
   // unsigned char *pixels=image.data;

src = 0;
pixsize = image.xsize * image.ysize * image.csize;
if (m_blurH != image.ysize || m_blurW != image.xsize || m_blurBpp != image.csize) {

m_blurH = image.ysize;
m_blurW = image.xsize;
m_blurBpp = image.csize;
m_blurSize = m_blurH * m_blurW * m_blurBpp;
delete saved;
saved = new unsigned char [m_blurSize];

}

if (m_reset){
    memcpy(saved,image.data,pixsize);
    m_reset = 0; 
    return;
}

   
   hlength = image.xsize;


for (h=0; h<image.ysize; h++){
    for(w=0; w<hlength; w++){
          
        if (((image.data[src+chRed] > saved[src+chRed] - Urange)&&(image.data[src+chRed] < saved[src+chRed] + Urange))&&
            ((image.data[src+chGreen] > saved[src+chGreen] - Yrange)&&(image.data[src+chGreen] < saved[src+chGreen] + Yrange))&&
            ((image.data[src+chBlue] > saved[src+chBlue] - Vrange)&&(image.data[src+chBlue] < saved[src+chBlue] + Vrange)))
                {
                image.data[src+chRed] = 0;
                image.data[src+chGreen] = 0;
                image.data[src+chBlue] = 0;
                }
        src+=4;
    }
}
m_reset = 0; 

}


/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void pix_background :: processYUVImage(imageStruct &image)
{
       int h,w,hlength;
    long src,pixsize;

src = 0;
pixsize = image.xsize * image.ysize * image.csize;
if (m_blurH != image.ysize || m_blurW != image.xsize || m_blurBpp != image.csize) {

m_blurH = image.ysize;
m_blurW = image.xsize;
m_blurBpp = image.csize;
m_blurSize = m_blurH * m_blurW * m_blurBpp;
delete saved;
saved = new unsigned char [m_blurSize];

}

if (m_reset){
    memcpy(saved,image.data,pixsize);
    m_reset = 0; 
    return;
}

   
   hlength = image.xsize/2;

for (h=0; h<image.ysize; h++){
    for(w=0; w<hlength; w++){
          
        if (((image.data[src] > saved[src] - Urange)&&(image.data[src] < saved[src] + Urange))&&
            ((image.data[src+1] > saved[src+1] - Yrange)&&(image.data[src+1] < saved[src+1] + Yrange))&&
            ((image.data[src+2] > saved[src+2] - Vrange)&&(image.data[src+2] < saved[src+2] + Vrange)))
                {
                image.data[src] = 128;
                image.data[src+1] = 0;
                image.data[src+2] = 128;
                image.data[src+3] = 0;
                }
        src+=4;

     
    }
}
m_reset = 0; 
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_background :: obj_setupCallback(t_class *classPtr)
{


    class_addmethod(classPtr, (t_method)&pix_background::rangeCallback,
		  gensym("range"), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_background::resetCallback,
		  gensym("reset"), A_NULL);
}


void pix_background :: rangeCallback(void *data, t_floatarg Y, t_floatarg U, t_floatarg V)
{
  GetMyClass(data)->Yrange=((int)Y);
  GetMyClass(data)->Urange=((int)U);
  GetMyClass(data)->Vrange=((int)V);

}

void pix_background :: resetCallback(void *data)
{
  GetMyClass(data)->m_reset=1;

}