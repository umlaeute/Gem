////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_gain.h"

CPPEXTERN_NEW(pix_gain)
  
/////////////////////////////////////////////////////////
//
// pix_gain
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_gain :: pix_gain()
{
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("ft1"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("vec_gain"));
    m_gain[chRed] = m_gain[chGreen] = m_gain[chBlue] = m_gain[chAlpha] = 1.0f;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_gain :: ~pix_gain()
{ }

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_gain :: processRGBAImage(imageStruct &image)
{
    int datasize =  image.xsize * image.ysize;
    unsigned char *pixels = image.data;
    short R,G,B,A;
    int red,green,blue,alpha;
    R   = int( 256 * m_gain[chRed]);
    G = int(256 * m_gain[chGreen]);
    B  = int(256 * m_gain[chBlue]);
    A = int(256 * m_gain[chAlpha]);

	while(datasize--)
	{
        //old float code
		//int red   = (int)(pixels[chRed] * m_gain[chRed]);
		//int green = (int)(pixels[chGreen] * m_gain[chGreen]);
		//int blue  = (int)(pixels[chBlue] * m_gain[chBlue]);
                //int alpha = (int)(pixels[chAlpha] * m_gain[chAlpha]);
                red = (pixels[chRed] * R)>>8;
		pixels[chRed]	= CLAMP(red);
                green = (pixels[chGreen] * G)>>8;
		pixels[chGreen] = CLAMP(green);
                blue = (pixels[chBlue] * B)>>8;
		pixels[chBlue]	= CLAMP(blue);
                alpha = (pixels[chAlpha] * A)>>8;
		pixels[chAlpha] = CLAMP(alpha);
		pixels += 4;
	}
}

/////////////////////////////////////////////////////////
// processGrayImage
//
/////////////////////////////////////////////////////////
void pix_gain :: processGrayImage(imageStruct &image)
{
    int datasize =  image.xsize * image.ysize;
    unsigned char *pixels = image.data;

	while (datasize--)
	{
		int gray = (int)(pixels[chGray] * m_gain[chRed]);
		pixels[chGray] = CLAMP(gray);
		pixels++;
	}
}
/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void pix_gain :: processYUVImage(imageStruct &image)
{
  int h,w,width;
    long src;
    int y1,y2,u,v;

    short Y=(short)(m_gain[1] * 255);
    short U=(short)(m_gain[2] * 255);
    short V=(short)(m_gain[3] * 255);
    src = 0;
    width = image.xsize/2;
    for (h=0; h<image.ysize; h++){
        for(w=0; w<width; w++){
        
      u = (((image.data[src] - 128) * U)>>8)+128;
        image.data[src] = (unsigned char)CLAMP(u);
        
        y1 = (image.data[src+1] * Y)>>8;
        image.data[src+1] = (unsigned char)CLAMP(y1);
        
       v = (((image.data[src+2] - 128) * V)>>8)+128;
        image.data[src+2] = (unsigned char)CLAMP(v);

        y2 = (image.data[src+3] * Y)>>8;
        image.data[src+3] = (unsigned char)CLAMP(y2);
       
        src+=4;
        }
    }
}

#ifdef __MMX__
/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_gain :: processRGBAMMX(imageStruct &image)
{

  short  R = int(256 * m_gain[chRed]);
  short  G = int(256 * m_gain[chGreen]);
  short  B = int(256 * m_gain[chBlue]);
  short  A = int(256 * m_gain[chAlpha]);


  /* the MMX code goes easily into clipping, 
   * since we are using (short) instead of (int)
   */
  if((R>256)||(G>256)||(B>256)||(B>256))processRGBAImage(image);
  
  register int pixsize = (image.ysize * image.xsize)>>1;

  register __m64 gain_64 = _mm_setr_pi16(R, G, B, A);
  register __m64*data_p= (__m64*)image.data;
  register __m64 null_64 = _mm_setzero_si64();
  register __m64 a0,a1;
  _mm_empty();

  while(pixsize--) {
    a1 = data_p[0];
    
    a0=_mm_unpacklo_pi8(a1, null_64);
    a1=_mm_unpackhi_pi8(a1, null_64);

    a0 = _mm_mullo_pi16(a0, gain_64);
    a1 = _mm_mullo_pi16(a1, gain_64);

    a0 = _mm_srai_pi16(a0, 8);
    a1 = _mm_srai_pi16(a1, 8);

    data_p[0]=_mm_packs_pi16(a0, a1);
    data_p++;      
  }
  _mm_empty();
}
#endif /* __MMX__ */



#ifdef __VEC__
void pix_gain :: processYUV_Altivec(imageStruct &image)
{
 int h,w,width,height;
    /*altivec code starts */
    width = image.xsize/8;
    height = image.ysize;
    union
    {
        short	elements[8];
        vector	signed short v;
    }shortBuffer;
    
    union
    {
        unsigned long	elements[8];
        vector	unsigned int v;
    }bitBuffer;
    
    
    register vector signed short d, hiImage, loImage, YImage, UVImage;
    vector unsigned char zero = vec_splat_u8(0);
    register vector signed int UVhi,UVlo,Yhi,Ylo;
    register vector signed short c,gain;
    register vector unsigned int bitshift;
    vector unsigned char *inData = (vector unsigned char*) image.data;

    
    shortBuffer.elements[0] = 128;
    shortBuffer.elements[1] = 0;
    shortBuffer.elements[2] = 128;
    shortBuffer.elements[3] = 0;
    shortBuffer.elements[4] = 128;
    shortBuffer.elements[5] = 0;
    shortBuffer.elements[6] = 128;
    shortBuffer.elements[7] = 0;
    
        c = shortBuffer.v;
    
    shortBuffer.elements[0] =(short) (m_gain[1]*255);
    gain = shortBuffer.v; 
    gain =  vec_splat(gain, 0 );  


    bitBuffer.elements[0] = 8;

    //Load it into the vector unit
    bitshift = bitBuffer.v;
    bitshift = vec_splat(bitshift,0); 
     
    shortBuffer.elements[0] = 128;
   
    //Load it into the vector unit
    d = shortBuffer.v;
    d = (vector signed short)vec_splat((vector signed short)d,0);
    
    #ifndef PPC970
   	UInt32			prefetchSize = GetPrefetchConstant( 16, 1, 256 );
	vec_dst( inData, prefetchSize, 0 );
    #endif
        
    for ( h=0; h<height; h++){
        for (w=0; w<width; w++)
        {
        #ifndef PPC970
	vec_dst( inData, prefetchSize, 0 );
        #endif
            //interleaved U Y V Y chars
            
            //expand the UInt8's to short's
            hiImage = (vector signed short) vec_mergeh( zero, inData[0] );
            loImage = (vector signed short) vec_mergel( zero, inData[0] );
            
            //vec_subs -128
            hiImage = (vector signed short) vec_sub( hiImage, c );
            loImage = (vector signed short) vec_sub( loImage, c );   
            
            //now vec_mule the UV into two vector ints
            UVhi = vec_mule(gain,hiImage);
            UVlo = vec_mule(gain,loImage);
            
            //now vec_mulo the Y into two vector ints
            Yhi = vec_mulo(gain,hiImage);
            Ylo = vec_mulo(gain,loImage);
            
            //this is where to do the bitshift/divide due to the resolution
            UVhi = vec_sra(UVhi,bitshift);
            UVlo = vec_sra(UVlo,bitshift);
            Yhi = vec_sra(Yhi,bitshift);
            Ylo = vec_sra(Ylo,bitshift);
            
            //pack the UV into a single short vector
            UVImage = vec_packs(UVhi,UVlo);
            
            //pack the Y into a single short vector
            YImage = vec_packs(Yhi,Ylo);
                                            
            
            //vec_adds +128 to U V U V short
            UVImage = vec_adds(UVImage,d);
            
            //vec_mergel + vec_mergeh Y and UV
            hiImage =  vec_mergeh(UVImage,YImage);
            loImage =  vec_mergel(UVImage,YImage);
            
            //pack back to 16 chars
            inData[0] = vec_packsu(hiImage, loImage);
            
          
            inData++;
        }
        #ifndef PPC970
        vec_dss( 0 );
        #endif
    }  /* end of working altivec function */
}
#endif /* __VEC__ */

/////////////////////////////////////////////////////////
// vecGainMess
//
/////////////////////////////////////////////////////////
void pix_gain :: vecGainMess(int argc, t_atom *argv)
{
    if (argc >= 4) m_gain[chAlpha] = atom_getfloat(&argv[3]);
    else if (argc == 3) m_gain[chAlpha] = 1.0;
    else if (argc == 1) m_gain[chRed] = m_gain[chGreen] = m_gain[chBlue] = m_gain[chAlpha] = atom_getfloat(argv);
    else {
      error("GEM: pix_gain: not enough gain values");
      return;
    }
    m_gain[chRed] = atom_getfloat(&argv[0]);
    m_gain[chGreen] = atom_getfloat(&argv[1]);
    m_gain[chBlue] = atom_getfloat(&argv[2]);
    setPixModified();
}

/////////////////////////////////////////////////////////
// floatGainMess
//
/////////////////////////////////////////////////////////
void pix_gain :: floatGainMess(float gain)
{
    // assumption that the alpha should be one
    m_gain[chAlpha] = 1.0f;
    m_gain[chRed] = m_gain[chGreen] = m_gain[chBlue] = gain;
    setPixModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_gain :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&pix_gain::vecGainMessCallback,
    	    gensym("vec_gain"), A_GIMME, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_gain::floatGainMessCallback,
    	    gensym("ft1"), A_FLOAT, A_NULL);
}
void pix_gain :: vecGainMessCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->vecGainMess(argc, argv);
}
void pix_gain :: floatGainMessCallback(void *data, t_floatarg gain)
{
    GetMyClass(data)->floatGainMess((float)gain);
}
