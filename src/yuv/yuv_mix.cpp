/*
 *  yuv_mix.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "yuv_mix.h"
#define clamp(x) ((x < 16) ? 16 : (x > 235) ? 235 : x)
CPPEXTERN_NEW(yuv_mix)

/////////////////////////////////////////////////////////
//
// yuv_mix
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_mix :: yuv_mix()
{
m_processOnOff=0;
imageGain=1;
rightGain=1;
post("yuv_mix created");
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_mix :: ~yuv_mix()
{

}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void yuv_mix :: processDualImage(imageStruct &image, imageStruct &right)
{

}



/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////

void yuv_mix :: processDualYUV(imageStruct &image, imageStruct &right)
{
long h,w, width;
   
      #ifdef ALTIVEC
    /*altivec code starts */
    width = image.xsize/8;
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
        vector	unsigned long v;
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
    vector signed short szero = vec_splat_s16(0);
    vector unsigned char c,one;
    vector signed int UVhi,UVlo,Yhi,Ylo;
    vector signed int UVhiR,UVloR,YhiR,YloR;
    vector signed short gainSub,gain,gainR,d;
    vector unsigned long bitshift;
    vector unsigned char *inData = (vector unsigned char*) image.data;
    vector unsigned char *rightData = (vector unsigned char*) right.data;
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
            hiImage = (vector signed short) vec_sub( hiImage, gainSub );
            loImage = (vector signed short) vec_sub( loImage, gainSub );   
            
            hiRight = (vector signed short) vec_sub( hiRight, gainSub );
            loRight = (vector signed short) vec_sub( loRight, gainSub );   
            
            
            //now vec_mule the UV into two vector ints
            UVhi = vec_mule(gain,hiImage);
            UVlo = vec_mule(gain,loImage);
            
            UVhiR = vec_mule(gainR,hiRight);
            UVloR = vec_mule(gainR,loRight);
            
            //now vec_mulo the Y into two vector ints
            Yhi = vec_mulo(gain,hiImage);
            Ylo = vec_mulo(gain,loImage);
            
            YhiR = vec_mulo(gainR,hiRight);
            YloR = vec_mulo(gainR,loRight);
            
            
            //this is where to do the bitshift/divide due to the resolution
            UVhi = vec_sra(UVhi,bitshift);
            UVlo = vec_sra(UVlo,bitshift);
            Yhi = vec_sra(Yhi,bitshift);
            Ylo = vec_sra(Ylo,bitshift);
            
            UVhiR = vec_sra(UVhiR,bitshift);
            UVloR = vec_sra(UVloR,bitshift);
            YhiR = vec_sra(YhiR,bitshift);
            YloR = vec_sra(YloR,bitshift);
            
            //pack the UV into a single short vector
            UVImage = vec_packs(UVhi,UVlo);
            UVRight = vec_packs(UVhiR,UVloR);

            //pack the Y into a single short vector
            YImage = vec_packs(Yhi,Ylo);
            YRight = vec_packs(YhiR,YloR);
                                        
            
            //vec_adds +128 to U V U V short
            UVImage = vec_adds(UVImage,gainAdd);
            UVRight = vec_adds(UVRight,gainAdd);
            
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
            UVRight = (vector signed short)vec_mule(c,tempRight);
            
            //vec_mulo Y * 1 to short vector Y Y Y Y shorts
            YImage = (vector signed short)vec_mulo(c,tempImage);
            YRight = (vector signed short)vec_mulo(c,tempRight);

            
            //vel_subs UV - 255
            UVRight = (vector signed short)vec_subs(UVRight, d);
            
            //vec_adds UV
            UVTemp = vec_adds(UVImage,UVRight);
            
            //vec_adds Y
            YTemp = vec_adds(YImage,YRight);
            
            hiImage = vec_mergeh(UVTemp,YTemp);
            loImage = vec_mergel(UVTemp,YTemp);
            
            //vec_mergel + vec_mergeh Y and UV
            inData[0] = vec_packsu(hiImage, loImage);        
        
            inData++;
            rightData++;
        }
        
        //stop the cache streams
        vec_dss( 0 );
        vec_dss( 1 );

}  /* end of working altivec function */   
   #else
      
   int	y1,y2;
   int u,v,u1,u2,v1,v2;
   int Rgain, Igain;
   long src =0;
   Rgain = 255/rightGain;
   Igain = 255/imageGain;
   width = image.xsize/2;
   //a = yuv1[0][i] + (2*yuv2[0][i]) - 235;
   //format is U Y V Y
       for (h=0; h<image.ysize; h++){
        for(w=0; w<width; w++){
        
        
         u = (((image.data[src] - 128) * imageGain)>>8)+128;
         u1 = (((right.data[src] - 128) * rightGain)>>8)+128;
         u = u + ((2*u1) -255);
        image.data[src] = (unsigned char)clamp(u);
        
        y1 = ((image.data[src+1] * imageGain)>>8) + ((right.data[src+1] * rightGain)>>8);
        image.data[src+1] = (unsigned char)clamp(y1);
        
       v = (((image.data[src+2] - 128) * imageGain)>>8)+128;
       v1 = (((right.data[src+2] - 128) * rightGain)>>8)+128;
       v = v + (2*v1) - 255;
        image.data[src+2] = (unsigned char)clamp(v);

        y2 = ((image.data[src+3] * imageGain)>>8) + ((right.data[src+3] * rightGain)>>8);;
        image.data[src+3] = (unsigned char)clamp(y2);

        src += 4;
        
        }
      }
    #endif  
}

void yuv_mix ::	processRightYUV(imageStruct &image, imageStruct &right)
{
post("yuv_mix: processRightGray");
}
        
void yuv_mix ::	processLeftYUV(imageStruct &image, imageStruct &right)
{
post("yuv_mix: processLeftGray");
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_mix :: obj_setupCallback(t_class *classPtr)
{

    class_addmethod(classPtr, (t_method)&yuv_mix::gainCallback,
		  gensym("gain"), A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

void yuv_mix :: gainCallback(void *data, t_floatarg X, t_floatarg Y)
{
  GetMyClass(data)->imageGain=((int)X);
  GetMyClass(data)->rightGain=((int)Y);

}
