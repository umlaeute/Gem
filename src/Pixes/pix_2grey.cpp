////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_2grey.h"

CPPEXTERN_NEW(pix_2grey)

/////////////////////////////////////////////////////////
//
// pix_2grey
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_2grey :: pix_2grey()
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_2grey :: ~pix_2grey()
{ }

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_2grey :: processRGBAImage(imageStruct &image)
{
  unsigned char *pixels = image.data;
  int count = image.ysize * image.xsize;
   
     while (count--)    {
     int grey = (pixels[chRed] * 79 + pixels[chGreen] * 155
      + pixels[chBlue] * 21)>>8;
     pixels[chRed] = pixels[chGreen] = pixels[chBlue] = (unsigned char)grey;
     pixels += 4;
     }  
}

void pix_2grey :: processYUVImage(imageStruct &image)
{
  unsigned char *pixels = image.data;
  int count = image.ysize * image.xsize / 2;
 
  while (count--)    {
    pixels[chU]=127; pixels[chV]=127;
    pixels+=4;
  }
}

#ifdef __MMX__
void pix_2grey :: processRGBAMMX(imageStruct &image){


}

void pix_2grey :: processYUVMMX(imageStruct &image){
 register int pixsize = (image.ysize * image.xsize)>>2;

 register __m64 mask_64   = _mm_set_pi8(0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00);
 register __m64 offset_64 = _mm_setr_pi8(0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00);
 register __m64*data_p= (__m64*)image.data;
 _mm_empty();

 register __m64 pixel;

 while(pixsize--) {
   
   pixel=data_p[0];
   pixel = _mm_and_si64(pixel, mask_64);
   pixel = _mm_add_pi8 (pixel, offset_64);
   *data_p++=pixel;
 }
 _mm_empty();

}
#endif

#ifdef __VEC__
void pix_2grey :: processRGBAAltivec(imageStruct &image)
{
 
  union{
    unsigned char		c[16];
    vector unsigned char	v;
  }charBuffer;
  
  union{
    unsigned short		s[8];
    vector unsigned short	v;
  }shortBuffer;
  
  vector unsigned char 	factors;
  vector unsigned short	oddS, evenS, one,lo,hi,lo1,hi1;
  vector unsigned int	R, G, B, A,shift,grey;
  vector unsigned char *pixels = (vector unsigned char *)image.data;
  int count = image.ysize * image.xsize / 4;
 
  #ifndef PPC970
   	UInt32			prefetchSize = GetPrefetchConstant( 16, 1, 256 );
	vec_dst( pixels, prefetchSize, 0 );
	vec_dst( pixels, prefetchSize, 1 );
  #endif
 
  charBuffer.c[0] = 1;
  charBuffer.c[1] = 79;
  charBuffer.c[2] = 155;
  charBuffer.c[3] = 21;
  charBuffer.c[4] = 1;
  charBuffer.c[5] = 79;
  charBuffer.c[6] = 155;
  charBuffer.c[7] = 21;
  charBuffer.c[8] = 1;
  charBuffer.c[9] = 79;
  charBuffer.c[10] = 155;
  charBuffer.c[11] = 21;
  charBuffer.c[12] = 1;
  charBuffer.c[13] = 79;
  charBuffer.c[14] = 155;
  charBuffer.c[15] = 21;
  
  factors = charBuffer.v;
  
  shortBuffer.s[0] = 4;
  shortBuffer.s[1] = 0;
  shortBuffer.s[2] = 1;
  shortBuffer.s[3] = 2;
  shortBuffer.s[4] = 5;
  shortBuffer.s[5] = 0;
  shortBuffer.s[6] = 0;
  shortBuffer.s[7] = 0;
  
  shortBuffer.s[0] = 1;
  one = vec_splat(shortBuffer.v,0);
  shift = vec_splat_u32(8); 
  
  while (count--)    {
    #ifndef PPC970
	vec_dst( pixels, prefetchSize, 0 );
        vec_dst( pixels+256, prefetchSize, 1 );
    #endif    
    
    //mult to short vector of R and B
    oddS = vec_mulo(pixels[0],factors);
    //mult to a short vector of A and G
    evenS = vec_mule(pixels[0],factors);
    
    //get separate R G B A vectors
    R = vec_mule(oddS,one);
    B = vec_mulo(oddS,one);
    A = vec_mule(evenS,one);
    G = vec_mulo(evenS,one);
    
    //sum the values
    grey = vec_adds(R,(vec_adds(G,B)));
    //bitshift down 8 bits
    grey = vec_sra(grey, shift);
    
    lo = vec_packsu(grey,grey);
    hi = vec_packsu(grey,grey);
    
    lo1 = vec_mergel(lo,hi);
    hi1 = vec_mergeh(lo,hi);
    
    lo = vec_mergel(lo1,hi1);
    hi = vec_mergeh(lo1,hi1);
    
    pixels[0] = vec_packsu(hi,lo);
    pixels++;
   
  }
  # ifndef PPC970
    //stop the cache streams
    vec_dss( 0 );
    vec_dss( 1 );
  # endif
  
}

void pix_2grey :: processYUVAltivec(imageStruct &image)
{
  union{
    unsigned char		c[16];
    vector unsigned char	v;
  }charBuffer;
  
  vector unsigned char *pixels = (vector unsigned char *)image.data;
  int count = image.ysize * image.xsize / 8;
  vector unsigned char	permmask,fill;
  
  charBuffer.c[0] = 127;
  fill = charBuffer.v;
  fill = vec_splat(fill,0);
  
  charBuffer.c[0] = 16;
  charBuffer.c[1] = 1;
  charBuffer.c[2] = 17;
  charBuffer.c[3] = 3;
  charBuffer.c[4] = 18;
  charBuffer.c[5] = 5;
  charBuffer.c[6] = 19;
  charBuffer.c[7] = 7;
  charBuffer.c[8] = 20;
  charBuffer.c[9] = 9;
  charBuffer.c[10] = 21;
  charBuffer.c[11] = 11;
  charBuffer.c[12] = 22;
  charBuffer.c[13] = 13;
  charBuffer.c[14] = 23;
  charBuffer.c[15] = 15;
  
  permmask = charBuffer.v;
  
  while (count--)    {
   pixels[0] = vec_perm(pixels[0],fill,permmask);
   pixels++;
   
  }
}
#endif //altivec function


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_2grey :: obj_setupCallback(t_class *)
{
  class_addcreator((t_newmethod)_classpix_2grey, 
		   gensym("pix_2gray"), A_NULL);
}
