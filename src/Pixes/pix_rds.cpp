////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// This object is an based on the RandomDotStereoTV effect from EffecTV
// Originally written by Fukuchi Kentarou
// Copyright (C) 2002 FUKUCHI Kentarou                         
//
// ported by tigital@mac.com
//
// Implementation file
//
//    Copyleft (l) 2003 James Tittle
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "pix_rds.h"
#include <stdlib.h>

CPPEXTERN_NEW(pix_rds)

#define inline_fastrand() (fastrand_val=rand()*1103515245+12345)

/////////////////////////////////////////////////////////
//
// pix_rds
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_rds :: pix_rds()
{
    myImage.xsize=myImage.ysize=myImage.csize=1;
    myImage.allocate(1);
    stride = 40;
    method = 0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_rds :: ~pix_rds()
{
  myImage.clear();
}

/////////////////////////////////////////////////////////
// processRGBAImage
//
/////////////////////////////////////////////////////////
void pix_rds :: processRGBAImage(imageStruct &image)
{
    int x, y, i;
    unsigned int *target;
    unsigned int *src = (unsigned int*)image.data;
    unsigned int *dest;
    unsigned int v;
    unsigned int R, G, B;

    if (myImage.xsize*myImage.ysize*myImage.csize != image.xsize*image.ysize*image.csize){
        int dataSize = image.xsize * image.ysize * image.csize;
        myImage.clear();

        myImage.allocate(dataSize);
    }

    myImage.xsize = image.xsize;
    myImage.ysize = image.ysize;
    myImage.csize = image.csize;
    myImage.type  = image.type;

    dest = (unsigned int*)myImage.data;

    memset(dest, 0, image.xsize * image.ysize * image.csize);
    target = dest;

    if(method) {
        for(y=0; y<image.ysize; y++) {
            for(i=0; i<stride; i++) {
                if(inline_fastrand()&0xc0000000)
                    continue;

                x = image.xsize / 2 + i;
                *(dest + x) = 0xffffff;
	
                while(x + stride/2 < image.xsize) {
                    v = *(src + x + stride/2);
                    R = (v&0xff0000)>>(16+6);
                    G = (v&0xff00)>>(8+6);
                    B = (v&0xff)>>7;
                    x += stride + R + G + B;
                    if(x >= image.xsize) break;
                    *(dest + x) = 0xffffff;
                }

                x = image.xsize / 2 + i;
                while(x - stride/2 >= 0) {
                    v = *(src + x - stride/2);
                    R = (v&0xff0000)>>(16+6);
                    G = (v&0xff00)>>(8+6);
                    B = (v&0xff)>>7;
                    x -= stride + R + G + B;
                    if(x < 0) break;
                    *(dest + x) = 0xffffff;
                }
            }
            src += image.xsize;
            dest += image.xsize;
        }
    } else {
        for(y=0; y<image.ysize; y++) {
            for(i=0; i<stride; i++) {
                if(inline_fastrand()&0xc0000000)
                    continue;

                x = image.xsize / 2 + i;
                *(dest + x) = 0xffffff;
	
                while(x + stride/2 < image.xsize) {
                    v = *(src + x + stride/2);
                    R = (v&0xff0000)>>(16+6);
                    G = (v&0xff00)>>(8+6);
                    B = (v&0xff)>>7;
                    x += stride - R - G - B;
                    if(x >= image.xsize) break;
                    *(dest + x) = 0xffffff;
                }

                x = image.xsize / 2 + i;
                while(x - stride/2 >= 0) {
                    v = *(src + x - stride/2);
                    R = (v&0xff0000)>>(16+6);
                    G = (v&0xff00)>>(8+6);
                    B = (v&0xff)>>7;
                    x -= stride - R - G - B;
                    if(x < 0) break;
                    *(dest + x) = 0xffffff;
                }
            }
            src += image.xsize;
            dest += image.xsize;
        }
    }

    target += image.xsize + (image.xsize - stride) / 2;
    for(y=0; y<4; y++) {
        for(x=0; x<4; x++) {
            target[x] = 0xff0000;
            target[x+stride] = 0xff0000;
        }
        target += image.xsize;
    }
    image.data = myImage.data;
}

/////////////////////////////////////////////////////////
// processYUVImage
//
/////////////////////////////////////////////////////////
void pix_rds :: processYUVImage(imageStruct &image)
{
    post("pix_rds:  YUV under construction");
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_rds :: obj_setupCallback(t_class *classPtr)
{
  class_addfloat(classPtr, (t_method)&pix_rds::methMessCallback);
}

void pix_rds :: methMessCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->method=((int)state);
}
