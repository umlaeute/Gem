////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) G¸nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f¸r::uml‰ute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_mix.h"

CPPEXTERN_NEW(pix_mix)

/////////////////////////////////////////////////////////
//
// pix_mix
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_mix :: pix_mix()
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_mix :: ~pix_mix()
{ }

/////////////////////////////////////////////////////////
// processDualImage
//
/////////////////////////////////////////////////////////
void pix_mix :: processRGBA_RGBA(imageStruct &image, imageStruct &right)
{

    int datasize = image.xsize * image.ysize;
    unsigned char *leftPix = image.data;
    unsigned char *rightPix = right.data;
   // int A,R,G,B;

    while(datasize--)
    {
        leftPix[chRed] = (leftPix[chRed] * imageGain)>>8;
        rightPix[chRed] = (rightPix[chRed] * rightGain)>>8;
    	leftPix[chRed] =
			CLAMP_HIGH((int)leftPix[chRed] + (int)rightPix[chRed]);
        leftPix[chGreen] = (leftPix[chGreen] * imageGain)>>8;
        rightPix[chGreen] = (rightPix[chGreen] * rightGain)>>8;
    	leftPix[chGreen] =
			CLAMP_HIGH((int)leftPix[chGreen] + (int)rightPix[chGreen]);
        leftPix[chBlue] = (leftPix[chBlue] * imageGain)>>8;
        rightPix[chBlue] = (rightPix[chBlue] * rightGain)>>8;
    	leftPix[chBlue] =
			CLAMP_HIGH((int)leftPix[chBlue] + (int)rightPix[chBlue]);
        leftPix += 4;
		rightPix += 4;
    }

}



/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void pix_mix :: processYUV_YUV(imageStruct &image, imageStruct &right)
{
    int	y1,y2;
   int u,v,u1,v1;
   long width,h,w;
   long src =0;
 
   width = image.xsize/2;
   
   //format is U Y V Y
       for (h=0; h<image.ysize; h++){
        for(w=0; w<width; w++){
        
        
         u = (((image.data[src] - 128) * imageGain)>>8)+128;
         u1 = (((right.data[src] - 128) * rightGain)>>8)+128;
         u = u + ((2*u1) -255);
        image.data[src] = (unsigned char)CLAMP(u);
        
        y1 = ((image.data[src+1] * imageGain)>>8) + ((right.data[src+1] * rightGain)>>8);
        image.data[src+1] = (unsigned char)CLAMP(y1);
        
       v = (((image.data[src+2] - 128) * imageGain)>>8)+128;
       v1 = (((right.data[src+2] - 128) * rightGain)>>8)+128;
       v = v + (2*v1) - 255;
        image.data[src+2] = (unsigned char)CLAMP(v);

        y2 = ((image.data[src+3] * imageGain)>>8) + ((right.data[src+3] * rightGain)>>8);;
        image.data[src+3] = (unsigned char)CLAMP(y2);

        src += 4;
        
        }
      }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_mix :: obj_setupCallback(t_class *classPtr)
{ 
    class_addmethod(classPtr, (t_method)&pix_mix::gainCallback,
		  gensym("gain"), A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

void pix_mix :: gainCallback(void *data, t_floatarg X, t_floatarg Y)
{
  GetMyClass(data)->imageGain=((int)X);
  GetMyClass(data)->rightGain=((int)Y);

}