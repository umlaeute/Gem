/*
 *  pix_background.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "pix_background.h"
#include <string.h>

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
{
  long size,src,i;
    
  inletBlur = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("blur"));

    m_Yrange = 0;
    m_Urange = 0;
    m_Vrange = 0;
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
if(saved)delete saved;
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_background :: processRGBAImage(imageStruct &image)
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
    if(saved)delete saved;
    saved = new unsigned char [m_blurSize];
  }

  if (m_reset){
    memcpy(saved,image.data,pixsize);
    m_reset = 0; 
  }

  hlength = image.xsize;


  for (h=0; h<image.ysize; h++){
    for(w=0; w<hlength; w++){
      if (((image.data[src+chRed] > saved[src+chRed] - m_Urange)&&(image.data[src+chRed] < saved[src+chRed] + m_Urange))&&
	  ((image.data[src+chGreen] > saved[src+chGreen] - m_Yrange)&&(image.data[src+chGreen] < saved[src+chGreen] + m_Yrange))&&
	  ((image.data[src+chBlue] > saved[src+chBlue] - m_Vrange)&&(image.data[src+chBlue] < saved[src+chBlue] + m_Vrange)))
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

void pix_background :: processGrayImage(imageStruct &image)
{
  int i, h,w,hlength;
  long src,pixsize;
  unsigned char newpix, oldpix, *npixes, *opixes;

  src = 0;
  pixsize = image.xsize * image.ysize * image.csize;
  if (m_blurH != image.ysize || m_blurW != image.xsize || m_blurBpp != image.csize) {
    m_blurH = image.ysize;
    m_blurW = image.xsize;
    m_blurBpp = image.csize;
    m_blurSize = m_blurH * m_blurW * m_blurBpp;
    if(saved)delete saved;
    saved = new unsigned char [m_blurSize];
  }

  if (m_reset){
    memcpy(saved,image.data,pixsize);
    m_reset = 0; 
  }

  npixes=image.data;
  opixes=saved;
  i=pixsize;
  while(i--){
    newpix=*npixes++;
    oldpix=*opixes++;
    if((newpix>oldpix-m_Urange)&&(newpix<oldpix+m_Urange))npixes[-1]=0;
  }
  m_reset = 0; 
}
/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void pix_background :: processYUVImage(imageStruct &image)
{
#ifdef ALTIVEC
processYUVImageAltivec(image);
return;
#else
       int h,w,hlength;
    long src,pixsize;

src = 0;
pixsize = image.xsize * image.ysize * image.csize;
if (m_blurH != image.ysize || m_blurW != image.xsize || m_blurBpp != image.csize) {

m_blurH = image.ysize;
m_blurW = image.xsize;
m_blurBpp = image.csize;
m_blurSize = m_blurH * m_blurW * m_blurBpp;
if(saved)delete saved;
saved = new unsigned char [m_blurSize];

}

if (m_reset){
    memcpy(saved,image.data,pixsize);
    m_reset = 0; 
   // return;
}

   
   hlength = image.xsize/2;

for (h=0; h<image.ysize; h++){
    for(w=0; w<hlength; w++){
          
        if (((image.data[src] > saved[src] - m_Urange)&&(image.data[src] < saved[src] + m_Urange))&&
            ((image.data[src+1] > saved[src+1] - m_Yrange)&&(image.data[src+1] < saved[src+1] + m_Yrange))&&
            ((image.data[src+2] > saved[src+2] - m_Vrange)&&(image.data[src+2] < saved[src+2] + m_Vrange)))
                {
                image.data[src] = 128;
                image.data[src+1] = 0;
                image.data[src+2] = 128;
                image.data[src+3] = 0;
                }
        src+=4;

     
    }
}
#endif 
m_reset = 0; 
}

/////////////////////////////////////////////////////////
// the killer go fast stuff goes in here
//
/////////////////////////////////////////////////////////
void pix_background :: processYUVImageAltivec(imageStruct &image)
{
#ifdef ALTIVEC
register int h,w,i,j,width;
int pixsize = image.xsize * image.ysize * image.csize;
    h = image.ysize;
    w = image.xsize/8;
    width = image.xsize/8;
    
    //check to see if the buffer isn't 16byte aligned (highly unlikely)
    if (image.ysize*image.xsize % 16 != 0){
        post("pix_background: image not properly aligned for Altivec - try something SD or HD maybe?");
        return;
        }
    
    union{
        unsigned short		s[8];
        vector unsigned short	v;
    }shortBuffer;
    
    if (m_blurH != image.ysize || m_blurW != image.xsize || m_blurBpp != image.csize) {

        m_blurH = image.ysize;
        m_blurW = image.xsize;
        m_blurBpp = image.csize;
        m_blurSize = m_blurH * m_blurW * m_blurBpp;
        if(saved)delete saved;
        saved = new unsigned char [m_blurSize];

    }
    
    if (m_reset){
    memcpy(saved,image.data,pixsize);
    m_reset = 0; 
    }
    
    register vector unsigned short	UVres1, Yres1, UVres2, Yres2;//interleave;
    register vector unsigned short	hiImage, loImage;
    register vector unsigned short	Yrange, UVrange, Yblank,UVblank,blank;
    register vector bool short		Ymasklo,Ymaskhi,  UVmaskhi;
    register vector unsigned short	Yhi,Ylo,UVhi,UVlo; 
    register vector unsigned char	one = vec_splat_u8(1);
    register vector unsigned short	sone = vec_splat_u16(1);
    register vector unsigned int			Uhi, Ulo, Vhi, Vlo,Ures,Vres;
    register vector bool int 			Umasklo, Umaskhi, Vmaskhi, Vmasklo;

    vector unsigned char	*inData = (vector unsigned char*) image.data;
    vector unsigned char	*rightData = (vector unsigned char*) saved;
    
    shortBuffer.s[0] =  m_Yrange;
    Yrange = shortBuffer.v;
    Yrange = vec_splat(Yrange,0);
    
    shortBuffer.s[0] = 128;
    shortBuffer.s[1] = 0;
    shortBuffer.s[2] = 128;
    shortBuffer.s[3] = 0;
    shortBuffer.s[4] = 128;
    shortBuffer.s[5] = 0;
    shortBuffer.s[6] = 128;
    shortBuffer.s[7] = 0;
    blank = shortBuffer.v;
    
    shortBuffer.s[0] =  0;
    Yblank = shortBuffer.v;
    Yblank = vec_splat(Yblank,0);
    
    shortBuffer.s[0] =  128;
    UVblank = shortBuffer.v;
    UVblank = vec_splat(UVblank,0);
    
    shortBuffer.s[0] = m_Urange;
    shortBuffer.s[1] = m_Vrange;
    shortBuffer.s[2] = m_Urange;
    shortBuffer.s[3] = m_Vrange;
    shortBuffer.s[4] = m_Urange;
    shortBuffer.s[5] = m_Vrange;
    shortBuffer.s[6] = m_Urange;
    shortBuffer.s[7] = m_Vrange;
    UVrange = shortBuffer.v;
    
    
    //setup the cache prefetch -- A MUST!!!
    UInt32			prefetchSize = GetPrefetchConstant( 16, 1, 256 );
    #ifndef PPC970 // yeah you read that right
    vec_dst( inData, prefetchSize, 0 );
    vec_dst( rightData, prefetchSize, 1 );
    //post("pix_background: G4 not G5");
    #endif //PPC970
    
    for ( i=0; i<h; i++){
        for (j=0; j<w; j++)
        {
        #ifndef PPC970
        //this function is probably memory bound on most G4's -- what else is new?
        vec_dst( inData, prefetchSize, 0 );
        vec_dst( rightData, prefetchSize, 1 );
        #endif
        //separate the U and V from Y
        UVres1 = (vector unsigned short)vec_mule(one,inData[0]);
        UVres2 = (vector unsigned short)vec_mule(one,rightData[0]);
            
        //vec_mulo Y * 1 to short vector Y Y Y Y shorts
        Yres1 = (vector unsigned short)vec_mulo(one,inData[0]);
        Yres2 = (vector unsigned short)vec_mulo(one,rightData[0]);
        
        Yhi = vec_adds(Yres2,Yrange);
        Ylo = vec_subs(Yres2,Yrange);
        
        //go to ints for comparison
        UVhi = vec_adds(UVres2,UVrange);
        UVlo = vec_subs(UVres2,UVrange);
        
        Uhi = vec_mule(sone,UVhi);
        Ulo = vec_mule(sone,UVlo);
        
        Vhi = vec_mulo(sone,UVhi);
        Vlo = vec_mulo(sone,UVlo);
        
        Ures = vec_mule(sone,UVres1);
         Vres = vec_mulo(sone,UVres1);
         
         Umasklo = vec_cmpgt(Ures,Ulo);
         Umaskhi = vec_cmplt(Ures,Uhi);
         
         Vmasklo = vec_cmpgt(Vres,Vlo);
         Vmaskhi = vec_cmplt(Vres,Vhi);
         
         Umaskhi = vec_and(Umaskhi,Umasklo);
         
         Vmaskhi = vec_and(Vmaskhi,Vmasklo);
         
         Umasklo = vec_and(Umaskhi,Vmaskhi);
         Vmasklo = vec_and(Umaskhi,Vmaskhi);
         
         hiImage = (vector unsigned short)vec_mergeh(Umasklo,Vmasklo);
         loImage = (vector unsigned short)vec_mergel(Umasklo,Vmasklo);
         
         //pack it back down to bool short
         UVmaskhi = (vector bool short)vec_packsu(hiImage,loImage);
         
         Ymasklo = vec_cmpgt(Yres1,Ylo);
         Ymaskhi = vec_cmplt(Yres1,Yhi);
         
         Ymaskhi = vec_and(Ymaskhi,Ymasklo);
         
         Ymaskhi = vec_and(Ymaskhi,UVmaskhi);
         UVmaskhi = vec_and(Ymaskhi,UVmaskhi);
         
         //bitwise comparison and move using the result of the comparison as a mask
         Yres1 = vec_sel(Yres1,Yblank,Ymaskhi);
         
         //UVres1 = vec_sel(UVres1,UVres2,UVmaskhi);
         UVres1 = vec_sel(UVres1,UVblank,UVmaskhi);
         
         //merge the Y and UV back together
         hiImage = vec_mergeh(UVres1,Yres1);
         loImage = vec_mergel(UVres1,Yres1);
         
         //pack it back down to unsigned char to store
         inData[0] = vec_packsu(hiImage,loImage);
         
         inData++;
         rightData++;
        
        }
        #ifndef PPC970
        vec_dss(1);
        vec_dss(0);
        #endif
    }
    
#endif //ALTIVEC
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
  GetMyClass(data)->m_Yrange=((int)Y);
  GetMyClass(data)->m_Urange=((int)U);
  GetMyClass(data)->m_Vrange=((int)V);

}

void pix_background :: resetCallback(void *data)
{
  GetMyClass(data)->m_reset=1;

}
