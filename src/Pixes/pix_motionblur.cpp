/*
 *  pix_motionblur.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "pix_motionblur.h"
CPPEXTERN_NEW(pix_motionblur)
 
/////////////////////////////////////////////////////////
//
// pix_motionblur
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_motionblur :: pix_motionblur()
{	long size,src,i;
    
inletmotionblur = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("motionblur"));

m_motionblur = 0;
m_motionblurH = 240;
m_motionblurW = 240;
m_motionblurBpp = 2;
size = 320 * 240 * 4;
saved = new signed int [size];
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
pix_motionblur :: ~pix_motionblur()
{
delete saved;
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_motionblur :: processRGBAImage(imageStruct &image)
{
       int h,w,height,width;
    long src;
    register int R,R1,G,G1,B,B1; //too many for x86?  i really don't know or care
    int rightGain,imageGain;
    unsigned char *pixels=image.data;
    int Blue,Red,Green;

src = 0;
Blue=chBlue;
Red=chRed;
Green=chGreen;
if (m_motionblurH != image.ysize || m_motionblurW != image.xsize || m_motionblurBpp != image.csize) {

    m_motionblurH = image.ysize;
    m_motionblurW = image.xsize;
    m_motionblurBpp = image.csize;
    m_motionblurSize = m_motionblurH * m_motionblurW * m_motionblurBpp;
    delete saved;
    saved = new int [m_motionblurSize];
}

rightGain = CLAMP((int)(m_motionblur * 255.));
imageGain = CLAMP((int)(255. - (m_motionblur * 255.)));
height = image.ysize;
width = image.xsize;


for (h=0; h<height; h++){
    for(w=0; w<width; w++){


        R = pixels[src+Red];
        R1 = saved[src+Red];
        G = pixels[src+Green];
        G1 = saved[src+Green];
        B = pixels[src+Blue];
        B1 = saved[src+Blue];
        
        R = R * imageGain;
        R1 = R1 * rightGain;
        G = G * imageGain;
        G1 = G1 * rightGain;
        B = B * imageGain;
        B1 = B1 * rightGain;

        
        R = R + R1;
        G = G + G1;
        B = B + B1;
        
        R1 = R>>8;
        G1 = G>>8;
        B1 = B>>8;
        
        
        saved[src+Red] = (unsigned char)R1;
        saved[src+Green] = (unsigned char)G1;
        saved[src+Blue] = (unsigned char)B1;
        
        pixels[src+Red] = (unsigned char)R1;
        pixels[src+Green] = (unsigned char)G1;
        pixels[src+Blue] = (unsigned char)B1;
        
        src += 4;

      
    }
}

}


/////////////////////////////////////////////////////////
// do the YUV processing here
// -- note this is scheduled for PPC, 
//    if you use another CPU write another function for it
/////////////////////////////////////////////////////////
void pix_motionblur :: processYUVImage(imageStruct &image)
{

if (m_motionblurH != image.ysize || m_motionblurW != image.xsize || m_motionblurBpp != image.csize) {

m_motionblurH = image.ysize;
m_motionblurW = image.xsize;
m_motionblurBpp = image.csize;
m_motionblurSize = m_motionblurH * m_motionblurW * m_motionblurBpp;
delete saved;
saved = new signed int [m_motionblurSize];

}

#ifdef ALTIVEC
processYUVAltivec(image);
return;
#else
     int h,w,hlength;
    register long src,dst;

    register int rightGain,imageGain;
    register int y1,y1a,y2,y2a,y1res,y2res,u,u1,v,v1;
    register int loadU,loadV,loadY1, loadY2,loadU1,loadV1,loadY1a, loadY2a;
    
src = 0;
dst = 0;


loadU = image.data[src];
       loadU1 = saved[src]; 
       loadY1 = image.data[src+1] ;
       loadY1a = saved[src+1];
   
       loadV = image.data[src+2];
       loadV1 = saved[src+2]; 
       loadY2 = image.data[src+3];
       loadY2a = saved[src+3] ;
       src += 4;

rightGain = CLAMP((int)(m_motionblur * 235.));
imageGain = CLAMP((int)(255. - (m_motionblur * 235.)));
hlength = image.xsize/2;

//unroll this, add register temps and schedule the ops better to remove the data depedencies
for (h=0; h<image.ysize-1; h++){
    for(w=0; w<hlength; w++){
     
       u  = loadU - 128;
       u1 = loadU1 >> 8;
       v = loadV - 128;
       v1 = loadV1>>8;
       
       y1  = loadY1 * imageGain;
       y1a = loadY1a * rightGain; 
       y2 = loadY2 * imageGain;
       y2a = loadY2a  * rightGain; 
        u *= imageGain;
       u1 *= rightGain;
      
       v *= imageGain;
       v1 *= rightGain;
       
       
       loadU = (int)image.data[src]; 
       loadU1 = (int)saved[src]; 
       loadY1 = (int)image.data[src+1] ;
       loadY1a = (int)saved[src+1];
   
       loadV = (int)image.data[src+2];
       loadV1 = (int)saved[src+2]; 
       loadY2 = (int)image.data[src+3];
       loadY2a = (int)saved[src+3] ;
       
      
       y1a = y1a>>8;
       y2a = y2a>>8;
       
       u += u1; 
       v += v1;
       saved[dst] = u;
       saved[dst+2] = v;
       u = u>>8; 
       v = v>>8;
       u += 128;
       v += 128;
       
       
       y1res = y1 + y1a;
       y2res = y2 + y2a;
       

      saved[dst+1] = y1res;
      saved[dst+3] = y2res;
      
      y1res = y1res >> 8; //shift to 16bit to store? 
     
      y2res = y2res >> 8;
     
      image.data[dst] = (unsigned char)u;
      image.data[dst+2] = (unsigned char)v;
      image.data[dst+1] =(unsigned char)y1res;
      image.data[dst+3] = (unsigned char)y2res;
      src+=4;dst+=4;
       
    }
}
#endif
}


/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////


void pix_motionblur :: processYUVAltivec(imageStruct &image)
{
#ifdef ALTIVEC
int h,w,width;
signed short rightGain,imageGain;
/*altivec code starts */
    width = image.xsize/8;
    rightGain = (signed short)(235. * m_motionblur);
    imageGain = (signed short) (255. - (235. * m_motionblur));
    union
    {
        
        signed short	elements[8];
        
        vector	signed short v;
    }shortBuffer;
    
        union
    {
        
        unsigned int	elements[4];
        
        vector	unsigned int v;
    }bitBuffer;
     
   
    
    register vector signed short gainAdd, hiImage, loImage,hiRight,loRight, YImage, UVImage; 
    register vector unsigned char zero = vec_splat_u8(0);
    register vector signed int UVhi,UVlo,Yhi,Ylo;
    register vector signed int UVhiR,UVloR,YhiR,YloR;
    register vector signed short gainSub,gain,gainR;//,d;
    register vector unsigned int bitshift;
    vector unsigned char *inData = (vector unsigned char*) image.data;
    vector unsigned char *rightData = (vector unsigned char*) saved;
    

    
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
            
            //subtract 128 from UV
            
            hiImage = vec_subs(hiImage,gainSub);
            loImage = vec_subs(loImage,gainSub);
            
            hiRight = vec_subs(hiRight,gainSub);
            loRight = vec_subs(loRight,gainSub);
            
            //now vec_mule the UV into two vector ints
            //change sone to gain
            UVhi = vec_mule(gain,hiImage);
            UVlo = vec_mule(gain,loImage);
            
            UVhiR = vec_mule(gainR,hiRight);
            UVloR = vec_mule(gainR,loRight);
            
            //now vec_mulo the Y into two vector ints
            Yhi = vec_mulo(gain,hiImage);
            Ylo = vec_mulo(gain,loImage);
            
            YhiR = vec_mulo(gainR,hiRight);
            YloR = vec_mulo(gainR,loRight);
            
             
            //this is where to do the add and bitshift due to the resolution
            //add UV
            UVhi = vec_adds(UVhi,UVhiR);
            UVlo = vec_adds(UVlo,UVloR);
        
            Yhi = vec_adds(Yhi,YhiR);
            Ylo = vec_adds(Ylo,YloR);
            
            //bitshift UV
            UVhi = vec_sra(UVhi,bitshift);
            UVlo = vec_sra(UVlo,bitshift);
            
            Yhi = vec_sra(Yhi,bitshift);
            Ylo = vec_sra(Ylo,bitshift);
            
            
            
            //pack the UV into a single short vector
            UVImage =  vec_packs(UVhi,UVlo);

            //pack the Y into a single short vector
            YImage =  vec_packs(Yhi,Ylo);
                   
            //vec_mergel + vec_mergeh Y and UV
            hiImage =  vec_mergeh(UVImage,YImage);
            loImage =  vec_mergel(UVImage,YImage);
            
            //add 128 offset back
            hiImage = vec_adds(hiImage,gainSub);
            loImage = vec_adds(loImage,gainSub);
            
            //vec_mergel + vec_mergeh Y and UV
            rightData[0] = (vector unsigned char)vec_packsu(hiImage, loImage);
            inData[0] = (vector unsigned char)vec_packsu(hiImage, loImage);        
         
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
void pix_motionblur :: obj_setupCallback(t_class *classPtr)
{

    class_addmethod(classPtr, (t_method)&pix_motionblur::motionblurCallback,
		  gensym("motionblur"), A_DEFFLOAT, A_NULL);
}

void pix_motionblur :: motionblurCallback(void *data, t_floatarg value)
{
  GetMyClass(data)->m_motionblur=(value);

}

