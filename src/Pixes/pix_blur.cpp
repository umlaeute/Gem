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
    int R,G,B;
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



if (m_blurH != image.ysize || m_blurW != image.xsize || m_blurBpp != image.csize) {

m_blurH = image.ysize;
m_blurW = image.xsize;
m_blurBpp = image.csize;
m_blurSize = m_blurH * m_blurW * m_blurBpp;
delete saved;
saved = new unsigned char [m_blurSize];

}

#ifdef ALTIVEC
processYUVAltivec(image);
return;
#else
src = 0;
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
#endif
}


/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void pix_blur :: processYUVAltivec(imageStruct &image)
{
#ifdef ALTIVEC
int h,w,width;
short rightGain,imageGain;
/*altivec code starts */
    width = image.xsize/8;
    rightGain = m_blur;
    imageGain = 255 - m_blur;
    union
    {
        //unsigned int	i;
        short	elements[8];
        //vector signed char v;
        vector	signed short v;
    }shortBuffer;
    
        union
    {
        //unsigned int	i;
        unsigned long	elements[8];
        //vector signed char v;
        vector	unsigned int v;
    }bitBuffer;
    
        union
    {
        //unsigned int	i;
        unsigned char	elements[16];
        //vector signed char v;
        vector	unsigned char v;
    }charBuffer;
    
    //vector unsigned char c;
    vector signed short gainAdd, hiImage, loImage,hiRight,loRight, YImage, UVImage,YRight, UVRight, UVTemp, YTemp;
    vector unsigned char zero = vec_splat_u8(0);
    vector signed short sone = vec_splat_s16(1);
    vector unsigned char c,one;
    vector signed int UVhi,UVlo,Yhi,Ylo;
    vector signed int UVhiR,UVloR,YhiR,YloR;
    vector signed short gainSub,gain,gainR,d;
    vector unsigned int bitshift;
    vector unsigned char *inData = (vector unsigned char*) image.data;
    vector unsigned char *rightData = (vector unsigned char*) saved;
    vector unsigned char tempImage,tempRight;
    
    //Write the pixel (pair) to the transfer buffer
    charBuffer.elements[0] = 2;
    charBuffer.elements[1] = 1;
    charBuffer.elements[2] = 2;
    charBuffer.elements[3] = 1;
    charBuffer.elements[4] = 2;
    charBuffer.elements[5] = 1;
    charBuffer.elements[6] = 2;
    charBuffer.elements[7] = 1;
    charBuffer.elements[8] = 2;
    charBuffer.elements[9] = 1;
    charBuffer.elements[10] = 2;
    charBuffer.elements[11] = 1;
    charBuffer.elements[12] = 2;
    charBuffer.elements[13] = 1;
    charBuffer.elements[14] = 2;
    charBuffer.elements[15] = 1;


    //Load it into the vector unit
    c = charBuffer.v;
    
    one =  vec_splat_u8( 1 );
    
    shortBuffer.elements[0] = 255;
   
    //Load it into the vector unit
    d = shortBuffer.v;
    d = (vector signed short)vec_splat((vector signed short)d,0);
    
    shortBuffer.elements[0] = 128;
    shortBuffer.elements[1] = 0;
    shortBuffer.elements[2] = 128;
    shortBuffer.elements[3] = 0;
    shortBuffer.elements[4] = 128;
    shortBuffer.elements[5] = 0;
    shortBuffer.elements[6] = 128;
    shortBuffer.elements[7] = 0;
    
        gainSub = shortBuffer.v;
     
    shortBuffer.elements[0] = imageGain;
    gain = shortBuffer.v; 
    gain =  vec_splat(gain, 0 );  

    shortBuffer.elements[0] = rightGain;
    gainR = shortBuffer.v; 
    gainR =  vec_splat(gainR, 0 ); 

    bitBuffer.elements[0] = 8;

    //Load it into the vector unit
    bitshift = bitBuffer.v;
    bitshift = vec_splat(bitshift,0); 
     
    shortBuffer.elements[0] = 128;
   
    //Load it into the vector unit
    gainAdd = shortBuffer.v;
    gainAdd = (vector signed short)vec_splat((vector signed short)gainAdd,0);

   	UInt32			prefetchSize = GetPrefetchConstant( 16, 1, 256 );
	vec_dst( inData, prefetchSize, 0 );
	vec_dst( rightData, prefetchSize, 1 );
        
    for ( h=0; h<image.ysize; h++){
        for (w=0; w<width; w++)
        {
        
	vec_dst( inData, prefetchSize, 0 );
        vec_dst( rightData, prefetchSize, 1 );

            //interleaved U Y V Y chars
            
            //expand the UInt8's to short's
            hiImage = (vector signed short) vec_mergeh( zero, inData[0] );
            loImage = (vector signed short) vec_mergel( zero, inData[0] );
            
            hiRight = (vector signed short) vec_mergeh( zero, rightData[0] );
            loRight = (vector signed short) vec_mergel( zero, rightData[0] );
            
            //vec_subs -128
          //  hiImage = (vector signed short) vec_sub( hiImage, gainSub );
           // loImage = (vector signed short) vec_sub( loImage, gainSub );   
            
          //  hiRight = (vector signed short) vec_sub( hiRight, gainSub );
          //  loRight = (vector signed short) vec_sub( loRight, gainSub );   
            
            
            //now vec_mule the UV into two vector ints
            UVhi = vec_mule(sone,hiImage);
            UVlo = vec_mule(sone,loImage);
            
            UVhiR = vec_mule(sone,hiRight);
            UVloR = vec_mule(sone,loRight);
            
            //now vec_mulo the Y into two vector ints
            Yhi = vec_mulo(gain,hiImage);
            Ylo = vec_mulo(gain,loImage);
            
            YhiR = vec_mulo(gainR,hiRight);
            YloR = vec_mulo(gainR,loRight);
            
            
            //this is where to do the bitshift/divide due to the resolution
           // UVhi = vec_sra(UVhi,bitshift);
          //  UVlo = vec_sra(UVlo,bitshift);
            Yhi = vec_sra(Yhi,bitshift);
            Ylo = vec_sra(Ylo,bitshift);
            
          //  UVhiR = vec_sra(UVhiR,bitshift);
          //  UVloR = vec_sra(UVloR,bitshift);
            YhiR = vec_sra(YhiR,bitshift);
            YloR = vec_sra(YloR,bitshift);
            
            //pack the UV into a single short vector
            UVImage = vec_packs(UVhi,UVlo);
            UVRight = vec_packs(UVhiR,UVloR);

            //pack the Y into a single short vector
            YImage = vec_packs(Yhi,Ylo);
            YRight = vec_packs(YhiR,YloR);
                                        
            
            //vec_adds +128 to U V U V short
           // UVImage = vec_adds(UVImage,gainAdd);
          //  UVRight = vec_adds(UVRight,gainAdd);
            
            //vec_mergel + vec_mergeh Y and UV
            hiImage =  vec_mergeh(UVImage,YImage);
            loImage =  vec_mergel(UVImage,YImage);
            hiRight =  vec_mergeh(UVRight,YRight);
            loRight =  vec_mergel(UVRight,YRight);
            
            //pack back to 16 chars and prepare for add
            tempImage = vec_packsu(hiImage, loImage);
            tempRight = vec_packsu(hiRight, loRight);
        
            //vec_mule UV * 2 to short vector U V U V shorts
            UVImage = (vector signed short)vec_mule(one,tempImage);
            UVRight = (vector signed short)vec_mule(zero,tempRight);
            
            //vec_mulo Y * 1 to short vector Y Y Y Y shorts
            YImage = (vector signed short)vec_mulo(c,tempImage);
            YRight = (vector signed short)vec_mulo(c,tempRight);

            
            //vel_subs UV - 255
           // UVRight = (vector signed short)vec_subs(UVRight, d);
            
            //vec_adds UV
            UVTemp = vec_adds(UVImage,UVRight);
            
            //vec_adds Y
            YTemp = vec_adds(YImage,YRight);
            
            hiImage = vec_mergeh(UVTemp,YTemp);
            loImage = vec_mergel(UVTemp,YTemp);
            
            //vec_mergel + vec_mergeh Y and UV
            rightData[0] = vec_packsu(hiImage, loImage);
            inData[0] = vec_packsu(hiImage, loImage);        
        
            inData++;
            rightData++;
        }
       }
       //stop the cache streams
        vec_dss( 0 );
        vec_dss( 1 );
         /* end of working altivec function */   
         
#endif
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

