////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
 
#include "pix_offset.h"

CPPEXTERN_NEW(pix_offset)
 
/////////////////////////////////////////////////////////
//
// pix_offset
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_offset :: pix_offset()
{
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("ft1"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("vec_offset"));
    m_offset[chRed] = m_offset[chGreen] = m_offset[chBlue] = m_offset[chAlpha] = 0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_offset :: ~pix_offset()
{ }

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_offset :: processRGBAImage(imageStruct &image)
{
  int datasize =  image.xsize * image.ysize;
  unsigned char *pixels = image.data;

  unsigned char o_red=m_offset[chRed], o_green=m_offset[chGreen], o_blue=m_offset[chBlue], o_alpha=m_offset[chAlpha];

  while(datasize--) {
    //		int red   = (int)(pixels[chRed] * m_offset[]);
    //		int green = (int)(pixels[chGreen] * m_offset[chGreen]);
    //		int blue  = (int)(pixels[chBlue] * m_offset[chBlue]);
    //		int alpha = (int)(pixels[chAlpha] * m_offset[chAlpha]);
    pixels[chRed]   += o_red;
    pixels[chGreen] += o_green;
    pixels[chBlue]  += o_blue;
    pixels[chAlpha] += o_alpha;
    pixels += 4;
  }
}

/////////////////////////////////////////////////////////
// processGrayImage
//
/////////////////////////////////////////////////////////
void pix_offset :: processGrayImage(imageStruct &image)
{
    int datasize =  image.xsize * image.ysize*image.csize;
    unsigned char *pixels = image.data;
    unsigned char m_grey=m_offset[chRed];

    while(datasize--)*pixels++ += m_grey;
}

/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void pix_offset :: processYUVImage(imageStruct &image)
{
 
#ifdef __VEC__
//post("altivec");
processYUV_Altivec(image);
return;
#else 
    int h,w;
    long src;

src = 0;


//format is U Y V Y

for (h=0; h<image.ysize; h++){
    for(w=0; w<image.xsize/2; w++){
        image.data[src] = CLAMP( image.data[src] + U );
        image.data[src+1] = CLAMP( image.data[src+1] + Y );
        image.data[src+2] = CLAMP( image.data[src+2] + V );
        image.data[src+3] = CLAMP( image.data[src+3] + Y );

        src+=4;
    }
}
#endif
}


/* more optimized version - unrolled and load-hoisted */
void pix_offset :: processYUV_Altivec(imageStruct &image)
{
#ifdef __VEC__
    register int h,w,width,height;
width = image.xsize/16; //for altivec
height = image.ysize;
//format is U Y V Y
// start of working altivec function 
    union
    {
        short	elements[8];
        vector	signed short v;
    }transferBuffer;
    
    register vector signed short c, hi, lo;
    register vector signed short hi1, lo1;
    register vector signed short loadhi, loadhi1, loadlo, loadlo1;
    register vector unsigned char zero = vec_splat_u8(0);
    register vector unsigned char *inData = (vector unsigned char*) image.data;

    //Write the pixel (pair) to the transfer buffer
    //transferBuffer.i = (U << 24) | (Y << 16) | (V << 8 ) | Y;
    transferBuffer.elements[0] = U;
    transferBuffer.elements[1] = Y;
    transferBuffer.elements[2] = V;
    transferBuffer.elements[3] = Y;
    transferBuffer.elements[4] = U;
    transferBuffer.elements[5] = Y;
    transferBuffer.elements[6] = V;
    transferBuffer.elements[7] = Y;

    //Load it into the vector unit
    c = transferBuffer.v;

    
    #ifndef PPC970
   	UInt32			prefetchSize = GetPrefetchConstant( 16, 1, 256 );
	vec_dst( inData, prefetchSize, 0 );
        vec_dst( inData+16, prefetchSize, 1 );
        vec_dst( inData+32, prefetchSize, 2 );
        vec_dst( inData+64, prefetchSize, 3 );
      #endif  
     
    //expand the UInt8's to short's
    loadhi = (vector signed short) vec_mergeh( zero, inData[0] );
    loadlo = (vector signed short) vec_mergel( zero, inData[0] );
           
    loadhi1 = (vector signed short) vec_mergeh( zero, inData[1] );
    loadlo1 = (vector signed short) vec_mergel( zero, inData[1] );  \
           
        
    for ( h=0; h<height; h++){
        for (w=0; w<width; w++){
        
        #ifndef PPC970
	vec_dst( inData, prefetchSize, 0 );
        vec_dst( inData+16, prefetchSize, 1 );
        vec_dst( inData+32, prefetchSize, 2 );
        vec_dst( inData+64, prefetchSize, 3 );
        #endif
                    
            //add the constant to it
            hi = vec_add( loadhi, c );
            lo = vec_add( loadlo, c );
            
            hi1 = vec_add( loadhi1, c );
            lo1 = vec_add( loadlo1, c );
            
            
            //expand the UInt8's to short's
            loadhi = (vector signed short) vec_mergeh( zero, inData[2] );
            loadlo = (vector signed short) vec_mergel( zero, inData[2] );
           
            
            loadhi1 = (vector signed short) vec_mergeh( zero, inData[3] );
            loadlo1 = (vector signed short) vec_mergel( zero, inData[3] );
            
            //pack the result back down, with saturation
            inData[0] = vec_packsu( hi, lo );
            inData++;
            
            
            inData[0] = vec_packsu( hi1, lo1 );
            inData++;
              

        }
        
       
       
    }
    
        //
        // finish the last iteration after the loop
        //
        hi = vec_add( loadhi, c );
        lo = vec_add( loadlo, c );
            
        hi1 = vec_add( loadhi1, c );
        lo1 = vec_add( loadlo1, c );
            
        //pack the result back down, with saturation
        inData[0] = vec_packsu( hi, lo );
            
        inData++;
            
        inData[0] = vec_packsu( hi1, lo1 );
            
        inData++;
    
    #ifndef PPC970
    vec_dss( 0 );
    vec_dss( 1 );
    vec_dss( 2 );
    vec_dss( 3 );  //end of working altivec function 
    #endif
#endif
}


/////////////////////////////////////////////////////////
// vecOffsetMess
//
/////////////////////////////////////////////////////////
void pix_offset :: vecOffsetMess(int argc, t_atom *argv)
{
    if (argc >= 4) m_offset[chAlpha] = (int)(255*atom_getfloat(&argv[3]));
    else if (argc == 3) m_offset[chAlpha] = 0;
    else
    {
    	error("GEM: pix_offset: not enough offset values");
    	return;
    }
    m_offset[chRed]   = (int)(255*atom_getfloat(&argv[0]));
    m_offset[chGreen] = (int)(255*atom_getfloat(&argv[1]));
    m_offset[chBlue]  = (int)(255*atom_getfloat(&argv[2]));
    Y =(short)(255*atom_getfloat(&argv[0]));
    U = (short)(255*atom_getfloat(&argv[1]));
    V = (short)(255*atom_getfloat(&argv[2]));
    setPixModified();
}

/////////////////////////////////////////////////////////
// floatOffsetMess
//
/////////////////////////////////////////////////////////
void pix_offset :: floatOffsetMess(float foffset)
{
    // assumption that the alpha should be one
    m_offset[chAlpha] = 0;
    m_offset[chRed] = m_offset[chGreen] = m_offset[chBlue] = (int)(255*foffset);
    Y = U = V = (short)(255*foffset);
    setPixModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_offset :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&pix_offset::vecOffsetMessCallback,
    	    gensym("vec_offset"), A_GIMME, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_offset::floatOffsetMessCallback,
    	    gensym("ft1"), A_FLOAT, A_NULL);
}
void pix_offset :: vecOffsetMessCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->vecOffsetMess(argc, argv);
}
void pix_offset :: floatOffsetMessCallback(void *data, t_floatarg offset)
{
    GetMyClass(data)->floatOffsetMess((float)offset);
}
