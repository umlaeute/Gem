////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_diff.h"
#include <stdlib.h>

CPPEXTERN_NEW(pix_diff)

/////////////////////////////////////////////////////////
//
// pix_diff
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_diff :: pix_diff()
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_diff :: ~pix_diff()
{ }

/////////////////////////////////////////////////////////
// processDualImage
//
/////////////////////////////////////////////////////////
void pix_diff :: processRGBA_RGBA(imageStruct &image, imageStruct &right)
{
    int datasize = image.xsize * image.ysize;
    unsigned char *leftPix = image.data;
    unsigned char *rightPix = right.data;

    while(datasize--)    {
      leftPix[chRed] =
	abs(leftPix[chRed] - (int)rightPix[chRed]);
      leftPix[chGreen] =
	abs(leftPix[chGreen] - (int)rightPix[chGreen]);
      leftPix[chBlue] =
	abs((int)leftPix[chBlue] - (int)rightPix[chBlue]);
      leftPix += 4;
      rightPix += 4;
    }
}

/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////

void pix_diff :: processYUV_YUV(imageStruct &image, imageStruct &right)
{
   long src,h,w;
   int	y1,y2;
   int u,v;
   src =0;
   //format is U Y V Y
   for (h=0; h<image.ysize; h++){
    for(w=0; w<image.xsize/2; w++){
        
        u = image.data[src] - ((2*right.data[src]) - 255);
        image.data[src] = abs(u);
        y1 =image.data[src+1] - right.data[src+1];
        image.data[src+1] = abs(y1);
        v = image.data[src+2] - ((2*right.data[src+2]) - 255);
        image.data[src+2] = abs(v);
        y2 = image.data[src+3] - right.data[src+3];
        image.data[src+3] = abs(y2);
       
        src+=4;
    }
    }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_diff :: obj_setupCallback(t_class *)
{ }
