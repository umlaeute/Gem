/*
 *  pix_chroma_key.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "pix_chroma_key.h"
#define clamp(x) ((x < 0) ? 0 : (x > 255) ? 255 : x)
CPPEXTERN_NEW(pix_chroma_key)

/////////////////////////////////////////////////////////
//
// pix_chroma_key
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_chroma_key :: pix_chroma_key()
{
m_processOnOff=0;
m_direction = 1;
m_Yrange = m_Urange = m_Vrange = m_Yvalue = m_Uvalue = m_Vvalue = 0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_chroma_key :: ~pix_chroma_key()
{

}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_chroma_key :: processRGBA_RGBA(imageStruct &image, imageStruct &right)
{
   long src,h,w;
   int datasize = image.xsize * image.ysize;
    unsigned char *leftPix = image.data;
    unsigned char *rightPix = right.data;
   unsigned char Ghi,Glo,Bhi,Blo,Rhi,Rlo;
   src =0;

   Rhi = CLAMP(m_Yvalue + m_Yrange); 
   Rlo = CLAMP(m_Yvalue - m_Yrange);
   Ghi = CLAMP(m_Uvalue + m_Urange); 
   Glo = CLAMP(m_Uvalue - m_Urange);
   Bhi = CLAMP(m_Vvalue + m_Vrange); 
   Blo = CLAMP(m_Vvalue - m_Vrange);
   //format is U Y V Y
 //  post("datasize %d",datasize);
    if (m_direction) {
    
    while(datasize--){
        
            if ((leftPix[chBlue] < Bhi)&&(leftPix[chBlue] > Blo)&&
            (leftPix[chRed] < Rhi)&&(leftPix[chRed] > Rlo)&&
            (leftPix[chGreen] < Ghi)&&(leftPix[chGreen] > Glo))
                {
                leftPix[chRed] = rightPix[chRed];
                leftPix[chGreen] = rightPix[chGreen];
            
                leftPix[chBlue] = rightPix[chBlue];
                }
        leftPix+=4;
        rightPix+=4;
            }
        
    } else { //this needs help
        
     while(datasize--){
        
            if (!((leftPix[chBlue] < Bhi)&&(leftPix[chBlue] > Blo)&&
            (leftPix[chRed] < Ghi)&&(leftPix[chRed] > Glo)&&
            (leftPix[chGreen] < Rhi)&&(leftPix[chGreen] > Rlo)))
                {
                leftPix[chRed] = rightPix[chRed];
                leftPix[chGreen] = rightPix[chGreen];
            
                leftPix[chBlue] = rightPix[chBlue];
                }
        leftPix+=4;
        rightPix+=4;
            
        }   
        
    }
    
   // post("datasize %d",datasize);
}


/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void pix_chroma_key :: processYUV_YUV(imageStruct &image, imageStruct &right)
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
    if (m_direction) {
    
    for (h=0; h<image.ysize; h++){
        for(w=0; w<image.xsize/2; w++){
        
            if ((image.data[src] < Uhi)&&(image.data[src] > Ulo)&&
            (image.data[src+1] < Yhi)&&(image.data[src+1] > Ylo)&&
            (image.data[src+2] < Vhi)&&(image.data[src+2] > Vlo))
                {
                image.data[src] = right.data[src];
                image.data[src+1] = right.data[src+1];
            
                image.data[src+2] = right.data[src+2];
                image.data[src+3] = right.data[src+3];
                }
        src+=4;
            }
        }
    } else { //this needs help
        
     for (h=0; h<image.ysize; h++){
        for(w=0; w<image.xsize/2; w++){
        
            if (!((image.data[src] < Uhi)&&(image.data[src] > Ulo)&&
            (image.data[src+1] < Yhi)&&(image.data[src+1] > Ylo)&&
            (image.data[src+2] < Vhi)&&(image.data[src+2] > Vlo)))
                {
                image.data[src] = right.data[src];
                image.data[src+1] = right.data[src+1];
            
                image.data[src+2] = right.data[src+2];
                image.data[src+3] = right.data[src+3];
                }
        src+=4;
            }
        }   
        
    }
}




/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_chroma_key :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&pix_chroma_key::directionCallback,
		  gensym("direction"), A_DEFFLOAT, A_NULL);
                  
    class_addmethod(classPtr, (t_method)&pix_chroma_key::rangeCallback,
		  gensym("range"), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);
                  
    class_addmethod(classPtr, (t_method)&pix_chroma_key::valueCallback,
		  gensym("value"), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

void pix_chroma_key :: directionCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->m_direction=!(!(int)state);
}

//make separate ranges for Y U V
void pix_chroma_key :: rangeCallback(void *data, t_floatarg Yval, t_floatarg Uval,t_floatarg Vval)
{
  GetMyClass(data)->m_Yrange=((unsigned char)Yval);
  GetMyClass(data)->m_Urange=((unsigned char)Uval);
  GetMyClass(data)->m_Vrange=((unsigned char)Vval);
}

void pix_chroma_key :: valueCallback(void *data, t_floatarg Yval, t_floatarg Uval, t_floatarg Vval)
{
  GetMyClass(data)->m_Yvalue=((unsigned char)Yval);
  GetMyClass(data)->m_Uvalue=((unsigned char)Uval);
  GetMyClass(data)->m_Vvalue=((unsigned char)Vval);
}
