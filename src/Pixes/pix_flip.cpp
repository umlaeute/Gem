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
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_flip.h"

CPPEXTERN_NEW(pix_flip)

/////////////////////////////////////////////////////////
//
// pix_flip
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_flip :: pix_flip()
          : m_flip(NONE)
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_flip :: ~pix_flip()
{ }

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_flip :: processImage(imageStruct &image)
{
    // eventually should do this inline, but in the interest of getting it done...
    imageStruct tempImg;
    image.copy2Image(&tempImg);

    int ySrcStride = image.xsize * image.csize;
    int yDstStride = image.xsize * image.csize;
    int xSrcStride = image.csize;
    int xDstStride = image.csize;
    unsigned char *srcLine = tempImg.data;
    unsigned char *dstLine = image.data;

    switch(m_flip)
    {
    	case(HOR):
            srcLine = tempImg.data + ySrcStride - xSrcStride;
            xSrcStride = -xSrcStride;
    	    break;
    	case(VERT):
            srcLine = tempImg.data + ySrcStride * image.ysize - ySrcStride;
            ySrcStride = -ySrcStride;
    	    break;
    	case(BOTH):
            srcLine = tempImg.data + ySrcStride * image.ysize - xSrcStride;
            xSrcStride = -xSrcStride;
            ySrcStride = -ySrcStride;
    	    break;
    	default:
            return;
    	    // break;
    }
	int ySize = image.ysize;
	int xHold = image.xsize;
    while(ySize--)
    {
        unsigned char *srcPixels = srcLine;
        unsigned char *dstPixels = dstLine;
		int xSize = xHold;
        while(xSize--)
        {
    	    dstPixels[chRed] = srcPixels[chRed];
    	    dstPixels[chGreen] = srcPixels[chGreen];
    	    dstPixels[chBlue] = srcPixels[chBlue];
    	    dstPixels[chAlpha] = srcPixels[chAlpha];
            dstPixels += xDstStride;
            srcPixels += xSrcStride;
        }
        dstLine += yDstStride;
        srcLine += ySrcStride;
    }    
}

/////////////////////////////////////////////////////////
// flipMess
//
/////////////////////////////////////////////////////////
void pix_flip :: flipMess(FlipType type)
{
    m_flip = type;
    setPixModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_flip :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&pix_flip::horMessCallback,
    	    gensym("horizontal"), A_NULL);
    class_addmethod(classPtr, (t_method)&pix_flip::vertMessCallback,
    	    gensym("vertical"), A_NULL);
    class_addmethod(classPtr, (t_method)&pix_flip::bothMessCallback,
    	    gensym("both"), A_NULL);
    class_addmethod(classPtr, (t_method)&pix_flip::noneMessCallback,
    	    gensym("none"), A_NULL);
}
void pix_flip :: horMessCallback(void *data)
{
    GetMyClass(data)->flipMess(HOR);
}
void pix_flip :: vertMessCallback(void *data)
{
    GetMyClass(data)->flipMess(VERT);
}
void pix_flip :: bothMessCallback(void *data)
{
    GetMyClass(data)->flipMess(BOTH);
}
void pix_flip :: noneMessCallback(void *data)
{
    GetMyClass(data)->flipMess(NONE);
}
