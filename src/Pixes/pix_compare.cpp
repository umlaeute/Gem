/*
 *  pix_compare.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon May 26 2003.
 *  Copyright (c) 2003.  All rights reserved.
 *
 */

#include "pix_compare.h"
CPPEXTERN_NEW(pix_compare)

/////////////////////////////////////////////////////////
//
// pix_compare
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_compare :: pix_compare()
{
  //m_processOnOff=0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_compare :: ~pix_compare()
{}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_compare :: processRGBA_RGBA(imageStruct &image, imageStruct &right)
{
  long src,h,w;
  src =0;
  if (m_direction) {
    for (h=0; h<image.ysize; h++){
      for(w=0; w<image.xsize; w++){
	if ((90*image.data[src+chRed]+115*image.data[src+chGreen]+51*image.data[src+chBlue]) <
	    (90*right.data[src+chRed]+115*right.data[src+chGreen]+51*right.data[src+chBlue]))
	  {
	    image.data[src+chRed] = right.data[src+chRed];
	    image.data[src+chBlue] = right.data[src+chBlue];
	    image.data[src+chGreen] = right.data[src+chGreen];
	  }
	src+=4;
      }
    }
  } else {
    for (h=0; h<image.ysize; h++){
      for(w=0; w<image.xsize; w++){
	if ((90*image.data[src+chRed]+115*image.data[src+chGreen]+51*image.data[src+chBlue]) >
	    (90*right.data[src+chRed]+115*right.data[src+chGreen]+51*right.data[src+chBlue]))
	  {
	    image.data[src+chRed] = right.data[src+chRed];
	    image.data[src+chBlue] = right.data[src+chBlue];
	    image.data[src+chGreen] = right.data[src+chGreen];
	  }
	src+=4;
      }
    }   
  };
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_compare :: processGray_Gray(imageStruct &image, imageStruct &right)
{
  long src,h,w;
  src =0;
  if (m_direction) {
    for (h=0; h<image.ysize; h++){
      for(w=0; w<image.xsize; w++){
	if (image.data[src+chGray] < right.data[src+chGray]) {
	  image.data[src+chGray] = right.data[src+chGray];
	}
	src++;
      }
    }
  } else {
    for (h=0; h<image.ysize; h++){
      for(w=0; w<image.xsize; w++){
	if (image.data[src+chGray]>right.data[src+chGray])
	  {
	    image.data[src+chGray] = right.data[src+chGray];
	  }
	src++;
      }
    } 
  }
}

/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void pix_compare :: processYUV_YUV(imageStruct &image, imageStruct &right)
{
#ifdef __VEC__
  processYUVAltivec(image,right);
  return;
#else
  long src,h,w;
  src =0;

  //format is U Y V Y
  if (m_direction) {    
    for (h=0; h<image.ysize; h++){
      for(w=0; w<image.xsize/2; w++){
	if ((image.data[src+1] < right.data[src+1])&&(image.data[src+3] < right.data[src+3]))
	  {
	    image.data[src] = right.data[src];
	    image.data[src+1] = right.data[src+1];
	    image.data[src+2] = right.data[src+2];
	    image.data[src+3] = right.data[src+3];
	  }
        src+=4;
      }
    }
  } else {
    for (h=0; h<image.ysize; h++){
      for(w=0; w<image.xsize/2; w++){
	if ((image.data[src+1] > right.data[src+1])&&(image.data[src+3] > right.data[src+3]))
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
#endif //ALTIVEC
}

/////////////////////////////////////////////////////////
// do the Altivec YUV processing here
//
/////////////////////////////////////////////////////////

void pix_compare :: processYUVAltivec(imageStruct &image, imageStruct &right)
{
#ifdef __VEC__
register int h,w,i,j,width;

    h = image.ysize;
    w = image.xsize/8;
    width = image.xsize/8;
    
    //check to see if the buffer isn't 16byte aligned (highly unlikely)
    if (image.ysize*image.xsize % 16 != 0){
        post("pix_compare: image not properly aligned for Altivec");
        return;
        }

    register vector unsigned short	UVres1, Yres1, UVres2, Yres2;//interleave;
    register vector unsigned short	hiImage, loImage;
    register vector bool short		Ymask1;
    register vector unsigned char	one = vec_splat_u8(1);

    vector unsigned char	*inData = (vector unsigned char*) image.data;
    vector unsigned char	*rightData = (vector unsigned char*) right.data;
   
    #ifndef PPC970
    //setup the cache prefetch -- A MUST!!!
    UInt32			prefetchSize = GetPrefetchConstant( 16, 1, 256 );
    vec_dst( inData, prefetchSize, 0 );
    vec_dst( rightData, prefetchSize, 1 );
    #endif
    if (m_direction) {
    
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
            
         //compare the Y values   
         Ymask1 = vec_cmpgt(Yres1,Yres2);
         
         //bitwise comparison and move using the result of the comparison as a mask
         Yres1 = vec_sel(Yres2,Yres1,Ymask1);
         
         UVres1 = vec_sel(UVres2,UVres1,Ymask1);
         
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
    }else{
    
    for ( i=0; i<h; i++){
        for (j=0; j<w; j++)
        {
        #ifndef PPC970
        vec_dst( inData, prefetchSize, 0 );
        vec_dst( rightData, prefetchSize, 1 );
        #endif
        
        UVres1 = (vector unsigned short)vec_mule(one,inData[0]);
        UVres2 = (vector unsigned short)vec_mule(one,rightData[0]);
            
        //vec_mulo Y * 1 to short vector Y Y Y Y shorts
        Yres1 = (vector unsigned short)vec_mulo(one,inData[0]);
        Yres2 = (vector unsigned short)vec_mulo(one,rightData[0]);
            
         Ymask1 = vec_cmplt(Yres1,Yres2);
         
         Yres1 = vec_sel(Yres2,Yres1,Ymask1);
         
         UVres1 = vec_sel(UVres2,UVres1,Ymask1);
         
         hiImage = vec_mergeh(UVres1,Yres1);
         loImage = vec_mergel(UVres1,Yres1);
         
         inData[0] = vec_packsu(hiImage,loImage);
         
            inData++;
            rightData++;
        
        }
        #ifndef PPC970
        vec_dss(1);
        vec_dss(0);
        #endif
    }

    
    }
#endif
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_compare :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&pix_compare::directionCallback,
		  gensym("direction"), A_DEFFLOAT, A_NULL);
}

void pix_compare :: directionCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->m_direction=!(!(int)state);
}
