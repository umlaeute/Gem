////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "Base/config.h"

// I hate Microsoft...I shouldn't have to do this!
#ifdef __WIN32__
# pragma warning( disable : 4244 )
# pragma warning( disable : 4305 )
# pragma warning( disable : 4091 )
#endif

#include "m_pd.h"
#include "GemPixUtil.h"

#include <string.h>
#include <ctype.h>

/* this is some magic for debugging:
 * to time execution of a code-block use 
 *   'START_TIMING;' at the beginning of the block and
 *   'STOP_TIMING("something");' at the end of the block
 */
#ifdef __TIMING__
# ifdef __linux__
#  include <sys/time.h>

#  define START_TIMING float mseconds=0.f; \
                     timeval startTime, endTime; \
                     gettimeofday(&startTime, 0)
#  define STOP_TIMING(x) gettimeofday(&endTime, 0); \
                         mseconds = (endTime.tv_sec - startTime.tv_sec)*1000 +\
                                    (endTime.tv_usec - startTime.tv_usec) * 0.001;\
                         post("%s frame time = %f ms", x, mseconds)
# elif defined __APPLE__
#  define START_TIMING float mseconds=0.f; \
                      UnsignedWide start, end;\
	              Microseconds(&start)
#  define STOP_TIMING(x) Microseconds(&end); \
                         mseconds = (float)(end.lo - start.lo) / 1000.f; \
                         post("%s frame time = %f ms", x, mseconds)
# else
#  define START_TIMING
#  define STOP_TIMING(x)
# endif /* timing for OS */
#else
# define START_TIMING
# define STOP_TIMING(x)
#endif /* __TIMING__ */

pixBlock :: pixBlock()
  : newimage(0), newfilm(0)
{}


imageStruct :: imageStruct() 
  : xsize (0),ysize(0),csize(0),
#ifdef GL_UNSIGNED_SHORT_8_8_REV_APPLE
    // or should type be GL_UNSIGNED_INT_8_8_8_8_REV ? i don't know: jmz
    type(GL_UNSIGNED_SHORT_8_8_REV_APPLE), format(GL_YCBCR_422_GEM),
#else
    type(GL_UNSIGNED_BYTE), format(GL_RGBA),
#endif
    notowned(0),data(NULL),pdata(NULL),datasize(0),
#ifdef __APPLE__
    upsidedown(1)
#else
  upsidedown(0)
#endif
{}

imageStruct :: ~imageStruct()
{
  clear();
}

/* align the memory to 128bit (GEM_VECTORALIGNMENT is in GemSIMD.h)
 * this code is taken from pd-devel (written by t.grill)
 * there used to be something in here written by g.geiger
 */
GEM_EXTERN unsigned char* imageStruct::allocate(size_t size) 
{
  if (pdata){
    delete [] pdata;
    pdata=NULL;
  }
  int array_size= size+(GEM_VECTORALIGNMENT/8-1);
  pdata = new unsigned char[array_size];

  int alignment = ((int)pdata)&(GEM_VECTORALIGNMENT/8-1);
  int offset    = (alignment == 0?0:(GEM_VECTORALIGNMENT/8-alignment));
  data = pdata+offset;
  datasize=array_size-offset;
  notowned=0;
  return data; 
}

GEM_EXTERN unsigned char* imageStruct::allocate() 
{
	return allocate(xsize*ysize*csize);  
}

GEM_EXTERN unsigned char* imageStruct::reallocate(size_t size)
{
  if (size>datasize){
      return allocate(size);
  }
  int alignment = ((int)pdata)&(GEM_VECTORALIGNMENT/8-1);
  int offset    = (alignment == 0?0:(GEM_VECTORALIGNMENT/8-alignment));
  data=pdata+offset;
  return data;
}
GEM_EXTERN unsigned char* imageStruct::reallocate() 
{  
	return reallocate(xsize*ysize*csize);  
}
 
GEM_EXTERN void imageStruct::clear() 
{
  if (pdata) { // pdata is always owned by imageStruct
    delete [] pdata;
  }
  data = pdata = NULL;      
  datasize=0;
}


GEM_EXTERN void imageStruct::copy2ImageStruct(imageStruct *to){
    if (!to || !this || !this->data) {
      error("GEM: Someone sent a bogus pointer to copy2ImageStruct");
      if (to) to->data = NULL;
      return;
    }

    // copy the information over
    to->xsize 	= xsize;
    to->ysize 	= ysize;
    to->csize 	= csize;
    to->format 	= format;
    to->type 	= type;
    to->data    = data;
    /* from SIMD-branch: datasize refers to the private pdata
     * thus we shouldn't set it to something else, in order to not break
     * reallocate() and friends...
     */
    //to->datasize= datasize;
    to->upsidedown=upsidedown;
    to->notowned= true; /* but pdata is always owned by us */
}
GEM_EXTERN void imageStruct::info() {
  post("imageStruct\t:%dx%dx%d\n\t\t%X\t(%x) %d\n\t\t%x\t%x\t%d",
       xsize, ysize, csize,
       data, pdata, datasize,
       format, type, notowned);
}

GEM_EXTERN void imageStruct::copy2Image(imageStruct *to) {
    if (!to || !this || !this->data)
    {
        error("GEM: Someone sent a bogus pointer to copy2Image");
        if (to)
	  to->data = NULL;
        return;
    }

    /* copy without new allocation if possible (speedup in convolve ..) */
    to->xsize 	= xsize;
    to->ysize 	= ysize;
    to->csize 	= csize;
    to->format 	= format;
    to->type 	= type;
    to->reallocate();
    to->upsidedown 	= upsidedown;

    memcpy(to->data, data, xsize*ysize*csize);
}

GEM_EXTERN void imageStruct::refreshImage(imageStruct *to) {
    if (!to || !data)
    {
        error("GEM: Someone sent a bogus pointer to refreshImage");
        return;
    }

    // check if we need to reallocate memory
    if (to->xsize != xsize ||
        to->ysize != ysize ||
        to->csize != csize ||
		!to->data)
    {
        to->clear();
        copy2Image(to);
    }
    else
        // copy the data over
        memcpy(to->data, this->data, to->xsize * to->ysize * to->csize);
}


GEM_EXTERN int imageStruct::setCsizeByFormat(int setformat) {
  switch(setformat){
    case GL_LUMINANCE:  format=GL_LUMINANCE;  csize=1; type=GL_UNSIGNED_BYTE; break;

#ifndef __APPLE__
    case GL_RGB: format=GL_RGB;
#else
    case GL_RGB:
    case GL_BGR_EXT: format=GL_BGR_EXT;
#endif
      type=GL_UNSIGNED_BYTE; csize =3; break;
    
	case GL_YUV422_GEM:
#ifdef __APPLE__
      type=GL_UNSIGNED_SHORT_8_8_REV_APPLE;
#else
      type=GL_UNSIGNED_BYTE;
#endif
      format=GL_YUV422_GEM; csize=2; break;

#ifndef __APPLE__
    case GL_RGBA:       format=GL_RGBA;     type=GL_UNSIGNED_BYTE;
#else
    case GL_RGBA:
    case GL_BGRA_EXT:   format=GL_BGRA_EXT; type = GL_UNSIGNED_INT_8_8_8_8_REV;
#endif
      csize=4; break;
	  
	default:
#ifdef __APPLE__
	  format=GL_YUV422_GEM; type=GL_UNSIGNED_SHORT_8_8_REV_APPLE; csize=2;
#else
	  format=GL_RGBA; type=GL_UNSIGNED_BYTE; csize=4;
#endif
	  break;
  }
  return csize;
}
GEM_EXTERN int imageStruct::setCsizeByFormat() {
  return setCsizeByFormat(format);
}

void pix_addsat(unsigned char *leftPix, unsigned char *rightPix, size_t datasize)
{
  while(datasize--)
    {           
      *leftPix = CLAMP_HIGH((int)*leftPix + (int)*rightPix);
      leftPix++;
      rightPix++;
    }
}


void pix_sub(unsigned char *leftPix, unsigned char *rightPix, size_t datasize)
{
  while(datasize--){
    *leftPix = CLAMP_LOW((int)*leftPix - (int)*rightPix++);
    leftPix++;
  }
}

GEM_EXTERN void imageStruct::setBlack() {
  size_t i = datasize;
  unsigned char* dummy=pdata;
  switch (format){
  case GL_YCBCR_422_GEM:
    i/=4;
    while(i--){
      *dummy++=128;*dummy++=0;
      *dummy++=128;*dummy++=0;
    }
    break;
  default:
    memset(pdata, 0, datasize);
    break;
  }
}
GEM_EXTERN void imageStruct::setWhite() {
  size_t i = datasize;
  unsigned char* dummy=pdata;
  switch (format){
  case GL_YCBCR_422_GEM:
    i/=4;
    while(i--){
      *dummy++=128;*dummy++=255;
      *dummy++=128;*dummy++=255;
    }
    break;
  default:
    memset(pdata, 1, datasize);
    break;
  }
}
GEM_EXTERN void imageStruct::convertFrom(imageStruct *from, GLenum to_format=0) {
  if (!from || !this || !from->data) {
    error("GEM: Someone sent a bogus pointer to convert");
    return;
  }
  xsize=from->xsize;
  ysize=from->ysize;
  setCsizeByFormat(to_format);
  switch (from->format){
  case GL_RGBA: 
    fromRGBA(from->data);
    break;
  case GL_RGB:  
    fromRGB(from->data);
    break;
  case GL_BGR_EXT:
    fromBGR(from->data);
    break;
  case GL_BGRA_EXT: /* "RGBA" on apple */
    fromBGRA(from->data);
    break;
  case GL_LUMINANCE:
    fromGray(from->data);
    break;
  case GL_YCBCR_422_GEM: // YUV
    fromUYVY(from->data);
    break;
  }
}

GEM_EXTERN void imageStruct::convertTo(imageStruct *to, GLenum fmt=0) {
  if (!to || !this || !this->data) {
    error("GEM: Someone sent a bogus pointer to convert");
    if (to) to->data = NULL;
    return;
  }
  to->xsize=xsize;
  to->ysize=ysize;
  to->setCsizeByFormat(fmt);
  switch (format){
  case GL_RGBA: 
    to->fromRGBA(data);
    break;
  case GL_RGB:  
    to->fromRGB(data);
    break;
  case GL_BGR_EXT:
    to->fromBGR(data);
    break;
  case GL_BGRA_EXT: /* "RGBA" on apple */
    to->fromBGRA(data);
    break;
  case GL_LUMINANCE:
    to->fromGray(data);
    break;
  case GL_YCBCR_422_GEM: // YUV
    to->fromUYVY(data);
    break;
  }
}

GEM_EXTERN void imageStruct::fromRGB(unsigned char *rgbdata) {
  if(!rgbdata)return;
  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  unsigned char *pixels=data;
  switch (format){
  case GL_RGB:
    memcpy(data, rgbdata, pixelnum*csize);
    break;
  case GL_BGR_EXT:
    while(pixelnum--){
      pixels[0]=rgbdata[2];
      pixels[1]=rgbdata[1];
      pixels[2]=rgbdata[0];
      pixels+=3; rgbdata+=3;
    }
    break;
  case GL_RGBA:
    while(pixelnum--){
      *pixels++=*rgbdata++;
      *pixels++=*rgbdata++;
      *pixels++=*rgbdata++;
      *pixels++=255;
    }
    break;
  case GL_BGRA_EXT:
    while(pixelnum--){
      pixels[0]=rgbdata[2];
      pixels[1]=rgbdata[1];
      pixels[2]=rgbdata[0];
      pixels[3]=255;
      pixels+=4;rgbdata+=3;
    }
    break;
  case GL_LUMINANCE:
    while(pixelnum--){
      //      *pixels++=(unsigned char)(rgbdata[0] * 0.3086f + rgbdata[1] * 0.06094f + rgbdata[2] * 0.0820f);
      *pixels++=(rgbdata[0]*79+rgbdata[1]*156+rgbdata[2]*21)>>8;
      rgbdata+=3;
    }
    break;
  case GL_YUV422_GEM:
#if 0
	RGB_to_YCbCr_altivec(rgbdata, pixelnum, pixels);
#else
    pixelnum>>=1;
    while(pixelnum--){
      *pixels++=((RGB2YUV_21*rgbdata[0]+RGB2YUV_22*rgbdata[1]+RGB2YUV_23*rgbdata[2])>>8)+UV_OFFSET; // U
      *pixels++=((RGB2YUV_11*rgbdata[0]+RGB2YUV_12*rgbdata[1]+RGB2YUV_13*rgbdata[2])>>8)+ Y_OFFSET; // Y
      *pixels++=((RGB2YUV_31*rgbdata[0]+RGB2YUV_32*rgbdata[1]+RGB2YUV_33*rgbdata[2])>>8)+UV_OFFSET; // V
      *pixels++=((RGB2YUV_11*rgbdata[3]+RGB2YUV_12*rgbdata[4]+RGB2YUV_13*rgbdata[5])>>8)+ Y_OFFSET; // Y
      rgbdata+=6;
    }
#endif
    break;
  }
}

GEM_EXTERN void imageStruct::fromRGB16(unsigned char *rgb16data) {
  //   B B B B B G G G   G G G R R R R R
  //   R R R R R G G G   G G G B B B B B
  if(!rgb16data)return;
  unsigned short*rgbdata=(unsigned short*)rgb16data;
  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  unsigned char *pixels=data;
  unsigned short rgb;
  switch (format){
  case GL_RGBA:
    while(pixelnum--){
      rgb=*rgbdata++;
      pixels[0]=((rgb>>8)&0xF8);
      pixels[1]=((rgb>>3)&0xFC);
      pixels[2]=((rgb<<3)&0xF8);
      pixels[3]=255;
      pixels+=4;
    }
    break;
  case GL_LUMINANCE:
    while(pixelnum--){
      rgb=*rgbdata++;
      *pixels++=(((rgb>>8)&0xF8)*79+((rgb>>3)&0xFC)*156+((rgb<<3)&0xF8)*21)>>8;
    }
    break;
  case GL_YUV422_GEM:
    pixelnum>>=1;
    while(pixelnum--){
      rgb=*rgbdata++;
      unsigned char r=((rgb>>8)&0xF8);
      unsigned char g=((rgb>>3)&0xFC);
      unsigned char b=((rgb<<3)&0xF8);
      *pixels++=((RGB2YUV_21*r+RGB2YUV_22*g+RGB2YUV_23*b)>>8)+UV_OFFSET; // U
      *pixels++=((RGB2YUV_11*r+RGB2YUV_12*g+RGB2YUV_13*b)>>8)+ Y_OFFSET;  // Y
      *pixels++=((RGB2YUV_31*r+RGB2YUV_32*g+RGB2YUV_33*b)>>8)+UV_OFFSET; // V

      rgb=*rgbdata++;
      r=((rgb>>8)&0xF8);
      g=((rgb>>3)&0xFC);
      b=((rgb<<3)&0xF8);
      *pixels++=((RGB2YUV_11*r+RGB2YUV_12*g+RGB2YUV_13*b)>>8)+ Y_OFFSET;     // Y
    }
    break;
  }
}

GEM_EXTERN void imageStruct::fromRGBA(unsigned char *rgbadata) {
  if(!rgbadata)return;
  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  unsigned char *pixels=data;
  switch (format){
  case GL_RGB:
    while(pixelnum--){
      *pixels++=*rgbadata++;
      *pixels++=*rgbadata++;
      *pixels++=*rgbadata++;
      rgbadata++;
    }
    break;
  case GL_BGR_EXT:
    while(pixelnum--){
      pixels[0]=rgbadata[2];
      pixels[1]=rgbadata[1];
      pixels[2]=rgbadata[0];
      pixels+=3; rgbadata+=4;
    }
    break;
  case GL_RGBA:
    memcpy(data, rgbadata, pixelnum*csize);
    break;
  case GL_BGRA_EXT:
    if(pixels==rgbadata){
      unsigned char dummy=0;
      while(pixelnum--){
	dummy=pixels[2];
	pixels[2]=pixels[0];
	pixels[0]=dummy;
	pixels+=4;
      }
    } else {
      while(pixelnum--){
	pixels[0]=rgbadata[2];
	pixels[1]=rgbadata[1];
	pixels[2]=rgbadata[0];
	pixels[3]=rgbadata[3];
	pixels+=4;rgbadata+=4;
      }
    }
    break;
  case GL_LUMINANCE:
    while(pixelnum--){
      //      *pixels++=(unsigned char)(rgbadata[0] * 0.3086f + rgbadata[1] * 0.6094f + rgbadata[2] * 0.0820f);
      *pixels++=(rgbadata[0]*79+rgbadata[1]*156+rgbadata[2]*21)>>8;
      rgbadata+=4;
    }
    break;
  case GL_YUV422_GEM:
    START_TIMING;
#ifdef __SSE2__
    RGBA_to_UYVY_SSE2(rgbadata, pixelnum, pixels);
#else
    pixelnum>>=1;
    while(pixelnum--){
      *pixels++=((RGB2YUV_21*rgbadata[chRed]+
		  RGB2YUV_22*rgbadata[chGreen]+
		  RGB2YUV_23*rgbadata[chBlue])>>8)+UV_OFFSET; // U
      *pixels++=((RGB2YUV_11*rgbadata[chRed]+
		  RGB2YUV_12*rgbadata[chGreen]+
		  RGB2YUV_13*rgbadata[chBlue])>>8)+ Y_OFFSET; // Y
      *pixels++=((RGB2YUV_31*rgbadata[chRed]+
		  RGB2YUV_32*rgbadata[chGreen]+
		  RGB2YUV_33*rgbadata[chBlue])>>8)+UV_OFFSET; // V
      *pixels++=((RGB2YUV_11*rgbadata[4+chRed]+
		  RGB2YUV_12*rgbadata[4+chGreen]+
		  RGB2YUV_13*rgbadata[4+chBlue])>>8)+ Y_OFFSET; // Y
      rgbadata+=8;
    }
#endif
    STOP_TIMING("RGBA to UYVY");
    break;
  }
}


GEM_EXTERN void imageStruct::fromBGR(unsigned char *bgrdata) {
  if(!bgrdata)return;
  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  unsigned char *pixels=data;
  switch (format){
  case GL_BGR_EXT:
    memcpy(data, bgrdata, pixelnum*csize);
    break;
  case GL_RGB:
    while(pixelnum--){
      pixels[0]=bgrdata[2];
      pixels[1]=bgrdata[1];
      pixels[2]=bgrdata[0];
      pixels+=3; bgrdata+=3;
    }
    break;
  case GL_BGRA_EXT:
    while(pixelnum--){
      *pixels++=*bgrdata++;
      *pixels++=*bgrdata++;
      *pixels++=*bgrdata++;
      *pixels++=255;
    }
    break;
  case GL_RGBA:
    while(pixelnum--){
      pixels[0]=bgrdata[2];
      pixels[1]=bgrdata[1];
      pixels[2]=bgrdata[0];
      pixels[3]=255;
      pixels+=4;bgrdata+=3;
    }
    break;
  case GL_LUMINANCE:
    while(pixelnum--){
      //      *pixels++=(unsigned char)(bgrdata[2] * 0.3086f + bgrdata[1] * 0.06094f + bgrdata[0] * 0.0820f);
      *pixels++=(bgrdata[2]*79+bgrdata[1]*156+bgrdata[0]*21)>>8;
      bgrdata+=3;
    }
    break;
  case GL_YUV422_GEM:
    pixelnum>>=1;
    while(pixelnum--){
      *pixels++=((RGB2YUV_21*bgrdata[2]+RGB2YUV_22*bgrdata[1]+RGB2YUV_23*bgrdata[0])>>8)+UV_OFFSET; // U
      *pixels++=((RGB2YUV_11*bgrdata[2]+RGB2YUV_12*bgrdata[1]+RGB2YUV_13*bgrdata[0])>>8)+ Y_OFFSET; // Y
      *pixels++=((RGB2YUV_31*bgrdata[2]+RGB2YUV_32*bgrdata[1]+RGB2YUV_33*bgrdata[0])>>8)+UV_OFFSET; // V
      *pixels++=((RGB2YUV_11*bgrdata[5]+RGB2YUV_12*bgrdata[4]+RGB2YUV_13*bgrdata[3])>>8)+ Y_OFFSET; // Y
      bgrdata+=6;
    }
    break;
  }
}

GEM_EXTERN void imageStruct::fromBGRA(unsigned char *bgradata) {
  if(!bgradata)return;
  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  unsigned char *pixels=data;
  switch (format){
  case GL_BGR_EXT:
    while(pixelnum--){
      *pixels++=*bgradata++;
      *pixels++=*bgradata++;
      *pixels++=*bgradata++;
      bgradata++;
    }
    break;
  case GL_RGB:
    while(pixelnum--){
      pixels[0]=bgradata[2];
      pixels[1]=bgradata[1];
      pixels[2]=bgradata[0];
      pixels+=3; bgradata+=4;
    }
    break;
  case GL_BGRA_EXT:
    memcpy(data, bgradata, pixelnum*csize);
    break;
  case GL_RGBA:
    if(bgradata==data){
      // in place conversion
      unsigned char dummy=0;
      while(pixelnum--){
	dummy    =pixels[2];
	pixels[2]=pixels[0];
	pixels[0]=dummy;
	pixels+=4;
      } 
    } else {
      while(pixelnum--){
	pixels[0]=bgradata[2];
	pixels[1]=bgradata[1];
	pixels[2]=bgradata[0];
	pixels[3]=bgradata[3];
	pixels+=4;bgradata+=4;
      }
    }
    break;
  case GL_LUMINANCE:
    while(pixelnum--){
      //      *pixels++=(unsigned char)(bgradata[2] * 0.3086f + bgradata[1] * 0.06094f + bgradata[0] * 0.0820f);
      *pixels++=(bgradata[2]*79+bgradata[1]*156+bgradata[0]*21)>>8;

      bgradata+=4;
    }
    break;
  case GL_YUV422_GEM:
    START_TIMING;
#ifdef __VEC__
	BGRA_to_YCbCr_altivec(bgradata,pixelnum,pixels);
#else
    pixelnum>>=1;
    while(pixelnum--){
      *pixels++=((RGB2YUV_21*bgradata[chRed]+
		  RGB2YUV_22*bgradata[chGreen]+
		  RGB2YUV_23*bgradata[chBlue])>>8)+UV_OFFSET; // U
      *pixels++=((RGB2YUV_11*bgradata[chRed]+
		  RGB2YUV_12*bgradata[chGreen]+
		  RGB2YUV_13*bgradata[chBlue])>>8)+ Y_OFFSET; // Y
      *pixels++=((RGB2YUV_31*bgradata[chRed]+
		  RGB2YUV_32*bgradata[chGreen]+
		  RGB2YUV_33*bgradata[chBlue])>>8)+UV_OFFSET; // V
      *pixels++=((RGB2YUV_11*bgradata[4+chRed]+
		  RGB2YUV_12*bgradata[4+chGreen]+
		  RGB2YUV_13*bgradata[4+chBlue])>>8)+ Y_OFFSET; // Y
      bgradata+=8;
    }
#endif
    STOP_TIMING("BGRA_to_YCbCr");
    break;
  }
}

GEM_EXTERN void imageStruct::fromGray(unsigned char *greydata) {
  if(!greydata)return;
  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  unsigned char *pixels=data;
  register unsigned char grey=0;
  switch (format){
  case GL_RGB:
  case GL_BGR_EXT:
    while(pixelnum--){
      grey=*greydata++;
      *pixels++=grey;
      *pixels++=grey;
      *pixels++=grey;
      greydata++;
    }
    break;
  case GL_RGBA:
  case GL_BGRA_EXT:
    while(pixelnum--){
      grey=*greydata++;
      pixels[chRed]=grey;
      pixels[chGreen]=grey;
      pixels[chBlue]=grey;
      pixels[chAlpha]=255;
      pixels+=4;
    }
    break;
  case GL_LUMINANCE:
    memcpy(pdata, greydata, pixelnum);
    break;
  case GL_YUV422_GEM:
    pixelnum>>=1;
    while(pixelnum--){
      pixels[chY0]=*greydata++;
      pixels[chY1]=*greydata++;
      pixels[chU]=pixels[chV]=128;
      pixels+=4;      
    }
    break;
  }
}

GEM_EXTERN void imageStruct::fromGray(short *greydata) {
  if(!greydata)return;
  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  unsigned char *pixels=data;
  register short grey=0;
  switch (format){
  case GL_RGB:
  case GL_BGR_EXT:
    while(pixelnum--){
      grey=*greydata++;
      *pixels++=grey;
      *pixels++=grey;
      *pixels++=grey;
      greydata++;
    }
    break;
  case GL_RGBA:
  case GL_BGRA_EXT:
    while(pixelnum--){
      grey=*greydata++;
      pixels[chRed]=grey;
      pixels[chGreen]=grey;
      pixels[chBlue]=grey;
      pixels[chAlpha]=255;
      pixels+=4;
    }
    break;
  case GL_LUMINANCE:
    memcpy(pdata, greydata, pixelnum);
    break;
  case GL_YUV422_GEM:
    pixelnum>>=1;
    while(pixelnum--){
      pixels[chY0]=(*greydata++)>>7;
      pixels[chY1]=(*greydata++)>>7;
      pixels[chU]=pixels[chV]=128;
      pixels+=4;      
    }
    break;
  }
}

GEM_EXTERN void imageStruct::fromYV12(unsigned char*yuvdata) {
  if(!yuvdata)return;
  size_t pixelnum=xsize*ysize;
  fromYV12((yuvdata), yuvdata+(pixelnum+(pixelnum>>2)), yuvdata+(pixelnum));
}
GEM_EXTERN void imageStruct::fromYV12(unsigned char*Y, unsigned char*U, unsigned char*V) {
  // planar: 8bit Y-plane + 8bit 2x2-subsampled V- and U-planes
  if(!U && !V)fromGray(Y);
  if(!Y || !U || !V)return;

  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  switch (format){
  case GL_LUMINANCE:
    memcpy(pdata, Y, pixelnum);
    break;
  case GL_RGB:  case GL_BGR_EXT: // of course this is stupid, RGB isn't BGR
    {
      unsigned char *pixels1=data;
      unsigned char *pixels2=data+xsize*3;

      unsigned char*py1=Y;
      unsigned char*py2=Y+xsize; // plane_1 is luminance (csize==1)
      unsigned char*pv=(format==GL_BGR_EXT)?U:V;
      unsigned char*pu=(format==GL_RGB)?U:V;

      int y, u, v, yy;
      int uv_r, uv_g, uv_b;
      int row=ysize>>1;
      int cols=xsize>>1;
      yy=128;
      while(row--){
	int col=cols;
	while(col--){
	  u=*pu++ -UV_OFFSET;
	  v=*pv++ -UV_OFFSET;
	  uv_r=YUV2RGB_12*u+YUV2RGB_13*v;
	  uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
	  uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

	  // 1st row - 1st pixel
	  y=YUV2RGB_11*(*py1++ -Y_OFFSET);
	  *pixels1++ = CLAMP((y + uv_b) >> 8); // b
	  *pixels1++ = CLAMP((y + uv_g) >> 8); // g
	  *pixels1++ = CLAMP((y + uv_r) >> 8); // r

	  // 1st row - 2nd pixel
	  y=YUV2RGB_11*(*py1++ -Y_OFFSET);
	  *pixels1++ = CLAMP((y + uv_b) >> 8); // b
	  *pixels1++ = CLAMP((y + uv_g) >> 8); // g
	  *pixels1++ = CLAMP((y + uv_r) >> 8); // r

	  // 2nd row - 1st pixel
	  y=YUV2RGB_11*(*py2++ -Y_OFFSET);
	  *pixels2++ = CLAMP((y + uv_b) >> 8); // b
	  *pixels2++ = CLAMP((y + uv_g) >> 8); // g
	  *pixels2++ = CLAMP((y + uv_r) >> 8); // r

	  // 2nd row - 2nd pixel
	  y=YUV2RGB_11*(*py2++ -Y_OFFSET);
	  *pixels2++ = CLAMP((y + uv_b) >> 8); // b
	  *pixels2++ = CLAMP((y + uv_g) >> 8); // g
	  *pixels2++ = CLAMP((y + uv_r) >> 8); // r
	}
	pixels1+=xsize*csize;	pixels2+=xsize*csize;
	py1+=xsize*1;	py2+=xsize*1;
      }
    }
    break;
  case GL_RGBA:
  case GL_BGRA_EXT:
    {
      unsigned char *pixels1=data;
      unsigned char *pixels2=data+xsize*4;

      unsigned char*py1=Y;//yuvdata;
      unsigned char*py2=Y+xsize;//yuvdata+xsize; // plane_1 is luminance (csize==1)
      unsigned char*pv=(format==GL_BGRA_EXT)?V:U;
      unsigned char*pu=(format==GL_RGBA)?V:U;
 
      int y, u, v, yy;
      int uv_r, uv_g, uv_b;
      int row=ysize>>1;
      int cols=xsize>>1;
      yy=128;
      while(row--){
	int col=cols;
	while(col--){
	  u=*pu++-UV_OFFSET;
	  v=*pv++-UV_OFFSET;
	  uv_r=YUV2RGB_12*u+YUV2RGB_13*v;
	  uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
	  uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

	  // 1st row - 1st pixel
	  y=YUV2RGB_11*(*py1++ -Y_OFFSET);
	  *pixels1++ = CLAMP((y + uv_b) >> 8); // b
	  *pixels1++ = CLAMP((y + uv_g) >> 8); // g
	  *pixels1++ = CLAMP((y + uv_r) >> 8); // r
	  *pixels1++ = 255; // a

	  // 1st row - 2nd pixel
	  y=YUV2RGB_11*(*py1++ -Y_OFFSET);
	  *pixels1++ = CLAMP((y + uv_b) >> 8); // b
	  *pixels1++ = CLAMP((y + uv_g) >> 8); // g
	  *pixels1++ = CLAMP((y + uv_r) >> 8); // r
	  *pixels1++ = 255; // a

	  // 2nd row - 1st pixel
	  y=YUV2RGB_11*(*py2++ -Y_OFFSET);
	  *pixels2++ = CLAMP((y + uv_b) >> 8); // b
	  *pixels2++ = CLAMP((y + uv_g) >> 8); // g
	  *pixels2++ = CLAMP((y + uv_r) >> 8); // r
	  *pixels2++ = 255; // a

	  // 2nd row - 2nd pixel
	  y=YUV2RGB_11*(*py2++ -Y_OFFSET);
	  *pixels2++ = CLAMP((y + uv_b) >> 8); // b
	  *pixels2++ = CLAMP((y + uv_g) >> 8); // g
	  *pixels2++ = CLAMP((y + uv_r) >> 8); // r
	  *pixels2++ = 255; // a
	}
	pixels1+=xsize*csize;	pixels2+=xsize*csize;
	py1+=xsize*1;	py2+=xsize*1;
      }
    }
    break;
  case GL_YUV422_GEM:
    {
      unsigned char *pixels1=data;
      unsigned char *pixels2=data+xsize*csize;
      unsigned char*py1=Y;
      unsigned char*py2=Y+xsize; // plane_1 is luminance (csize==1)
      unsigned char*pu=U;
      unsigned char*pv=V;
      int row=ysize>>1;
      int cols=xsize>>1;
      unsigned char u, v;
      /* this is only re-ordering of the data */
      while(row--){
		int col=cols;
		while(col--){
			// yuv422 is U Y0 V Y1
			u=*pu++;	  v=*pv++;
			*pixels1++=u;
			*pixels1++=*py1++;
			*pixels1++=v;
			*pixels1++=*py1++;
			*pixels2++=u;
			*pixels2++=*py2++;
			*pixels2++=v;
			*pixels2++=*py2++;	  
		}
		pixels1+=xsize*csize;	pixels2+=xsize*csize;
		py1+=xsize*1;	py2+=xsize*1;
      }
    }
    break;
  }
}
//  for gem2pdp
GEM_EXTERN void imageStruct::fromYV12(short*yuvdata) {
  if(!yuvdata)return;
  int pixelnum=xsize*ysize;
  fromYV12((yuvdata), yuvdata+(pixelnum+(pixelnum>>2)), yuvdata+(pixelnum));
}
GEM_EXTERN void imageStruct::fromYV12(short*Y, short*U, short*V) {
  // planar: 8bit Y-plane + 8bit 2x2-subsampled V- and U-planes
  if(!U && !V)fromGray((unsigned char*)(*Y>>7));
  if(!Y || !U || !V)return;

  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  switch (format){
  case GL_LUMINANCE:
    memcpy(pdata, Y, pixelnum);
    break;
  case GL_RGB:  case GL_BGR_EXT: // of course this is stupid, RGB isn't BGR
    {
      unsigned char *pixels1=data;
      unsigned char *pixels2=data+xsize*csize;

      short*py1=Y;
      short*py2=Y+xsize; // plane_1 is luminance (csize==1)
      short*pv=V;//(format==GL_BGR_EXT)?V:U;
      short*pu=U;//(format==GL_RGB)?V:U;

      int y, u, v, yy;
      int uv_r, uv_g, uv_b;
      int row=ysize>>1;
      int cols=xsize>>1;
      yy=128;
      while(row--){
	int col=cols;
	while(col--){
	  // these are from http://www.poynton.com/notes/colour_and_gamma/ColorFAQ.html#RTFToC30
	  u=(*pu++)>>8;
	  v=(*pv++)>>8;
	  uv_r=YUV2RGB_12*u+YUV2RGB_13*v;
	  uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
	  uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

	  // 1st row - 1st pixel
	  y=YUV2RGB_11*((*py1++)>>7); // what about the "16"-offset ?
#ifndef __APPLE__
	  pixels1[chRed  ] = CLAMP((y + uv_r) >> 8); // r
	  pixels1[chGreen] = CLAMP((y + uv_g) >> 8); // g
	  pixels1[chBlue ] = CLAMP((y + uv_b) >> 8); // b
	  pixels1+=3;

	  // 1st row - 2nd pixel
	  y=YUV2RGB_11*((*py1++)>>7);
	  pixels1[chRed  ] = CLAMP((y + uv_r) >> 8); // r
	  pixels1[chGreen] = CLAMP((y + uv_g) >> 8); // g
	  pixels1[chBlue ] = CLAMP((y + uv_b) >> 8); // b
	  pixels1+=3;

	  // 2nd row - 1st pixel
	  y=YUV2RGB_11*((*py2++)>>7);
	  pixels2[chRed  ] = CLAMP((y + uv_r) >> 8); // r
	  pixels2[chGreen] = CLAMP((y + uv_g) >> 8); // g
	  pixels2[chBlue ] = CLAMP((y + uv_b) >> 8); // b
	  pixels2+=3;

	  // 2nd row - 2nd pixel
	  y=YUV2RGB_11*((*py2++)>>7);
	  pixels2[chRed  ] = CLAMP((y + uv_r) >> 8); // r
	  pixels2[chGreen] = CLAMP((y + uv_g) >> 8); // g
	  pixels2[chBlue ] = CLAMP((y + uv_b) >> 8); // b
	  pixels2+=3;

#else
	  pixels1[2 ] = CLAMP((y + uv_r) >> 8); // r
	  pixels1[1] = CLAMP((y + uv_g) >> 8); // g
	  pixels1[0] = CLAMP((y + uv_b) >> 8); // b
	  pixels1+=3;

	  // 1st row - 2nd pixel
	  y=YUV2RGB_11*((*py1++)>>7);
	  pixels1[2 ] = CLAMP((y + uv_r) >> 8); // r
	  pixels1[1] = CLAMP((y + uv_g) >> 8); // g
	  pixels1[0] = CLAMP((y + uv_b) >> 8); // b
	  pixels1+=3;

	  // 2nd row - 1st pixel
	  y=YUV2RGB_11*((*py2++)>>7);
	  pixels2[2 ] = CLAMP((y + uv_r) >> 8); // r
	  pixels2[1] = CLAMP((y + uv_g) >> 8); // g
	  pixels2[0 ] = CLAMP((y + uv_b) >> 8); // b
	  pixels2+=3;

	  // 2nd row - 2nd pixel
	  y=YUV2RGB_11*((*py2++)>>7);
	  pixels2[2 ] = CLAMP((y + uv_r) >> 8); // r
	  pixels2[1] = CLAMP((y + uv_g) >> 8); // g
	  pixels2[0] = CLAMP((y + uv_b) >> 8); // b
	  pixels2+=3;
#endif

	}
	pixels1+=xsize*csize;	pixels2+=xsize*csize;
	py1+=xsize*1;	py2+=xsize*1;
      }
    }
    break;
  case GL_RGBA:
  case GL_BGRA_EXT:
    {
      unsigned char *pixels1=data;
      unsigned char *pixels2=data+xsize*csize;

      short*py1=Y;//yuvdata;
      short*py2=Y+xsize;//yuvdata+xsize; // plane_1 is luminance (csize==1)
      short*pv=V;//(format==GL_BGRA_EXT)?U:V;
      short*pu=U;//(format==GL_RGBA)?U:V;

      int y, u, v, yy;
      int uv_r, uv_g, uv_b;
      int row=ysize>>1;
      int cols=xsize>>1;
      yy=128;
      while(row--){
	int col=cols;
	while(col--){
	  u=(*pu++)>>8;
	  v=(*pv++)>>8;
	  uv_r=YUV2RGB_12*u+YUV2RGB_13*v;
	  uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
	  uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

	  // 1st row - 1st pixel
	  y=YUV2RGB_11*((*py1++)>>7); // what about the "16"-offset ?
	  pixels1[chRed  ] = CLAMP((y + uv_r) >> 8); // r
	  pixels1[chGreen] = CLAMP((y + uv_g) >> 8); // g
	  pixels1[chBlue ] = CLAMP((y + uv_b) >> 8); // b
	  pixels1[chAlpha] = 255; // a
	  pixels1+=4;

	  // 1st row - 2nd pixel
	  y=YUV2RGB_11*((*py1++)>>7);
	  pixels1[chRed  ] = CLAMP((y + uv_r) >> 8); // r
	  pixels1[chGreen] = CLAMP((y + uv_g) >> 8); // g
	  pixels1[chBlue ] = CLAMP((y + uv_b) >> 8); // b
	  pixels1[chAlpha] = 255; // a
	  pixels1+=4;

	  // 2nd row - 1st pixel
	  y=YUV2RGB_11*((*py2++)>>7);
	  pixels2[chRed  ] = CLAMP((y + uv_r) >> 8); // r
	  pixels2[chGreen] = CLAMP((y + uv_g) >> 8); // g
	  pixels2[chBlue ] = CLAMP((y + uv_b) >> 8); // b
	  pixels2[chAlpha] = 255; // a
	  pixels2+=4;

	  // 2nd row - 2nd pixel
	  y=YUV2RGB_11*((*py2++)>>7);
	  pixels2[chRed  ] = CLAMP((y + uv_r) >> 8); // r
	  pixels2[chGreen] = CLAMP((y + uv_g) >> 8); // g
	  pixels2[chBlue ] = CLAMP((y + uv_b) >> 8); // b
	  pixels2[chAlpha] = 255; // a
	  pixels2+=4;
	}
	pixels1+=xsize*csize;	pixels2+=xsize*csize;
	py1+=xsize*1;	py2+=xsize*1;
      }
    }

    break;
  case GL_YUV422_GEM:
    {
      START_TIMING;
#ifdef __VEC__
	YV12_to_YUV422_altivec(Y, U, V, data, xsize, ysize);
#else
      unsigned char *pixels1=data;
      unsigned char *pixels2=data+xsize*csize;
      short*py1=Y;
      short*py2=Y+xsize; // plane_1 is luminance (csize==1)
      short*pu=U;
      short*pv=V;
      int row=ysize>>1;
      int cols=xsize>>1;
      unsigned char u, v;
      /* this is only re-ordering of the data */
      while(row--){
		int col=cols;
		while(col--){
			// yuv422 is U Y0 V Y1
			u=((*pu++)>>8)+128;	  v=((*pv++)>>8)+128;
			*pixels1++=u;
			*pixels1++=(*py1++)>>7;
			*pixels1++=v;
			*pixels1++=(*py1++)>>7;
			*pixels2++=u;
			*pixels2++=(*py2++)>>7;
			*pixels2++=v;
			*pixels2++=(*py2++)>>7;	  
		}
		pixels1+=xsize*csize;	pixels2+=xsize*csize;
		py1+=xsize*1;	py2+=xsize*1;
      }
#endif
      STOP_TIMING("YV12_to_YUV422");
    }
    break;
  }
}

GEM_EXTERN void imageStruct::fromUYVY(unsigned char *yuvdata) {
  // this is the yuv-format with Gem
  if(!yuvdata)return;
  data=pdata;
  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  unsigned char *pixels=pdata;
  switch (format){
  case GL_YUV422_GEM:
    memcpy(data, yuvdata, pixelnum*csize);
    break;
  case GL_LUMINANCE:
    pixelnum>>=1;
    while(pixelnum--){
      *pixels++=yuvdata[1];
      *pixels++=yuvdata[3];
      yuvdata+=4;
    }
    break;
  case GL_RGB:
  case GL_BGR:
    {
      unsigned char *pixels=data;
      int y, u, v;
      int uv_r, uv_g, uv_b;
      START_TIMING;
#ifdef __SSE2__
      UYVY_to_RGB_SSE2(yuvdata, pixelnum, pixels);
#else
      pixelnum>>=1;

      while(pixelnum--){
	u=yuvdata[0]-UV_OFFSET;
	v=yuvdata[2]-UV_OFFSET;
	uv_r=YUV2RGB_12*u+YUV2RGB_13*v;
	uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
	uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

	// 1st pixel
	y=YUV2RGB_11*(yuvdata[1] -Y_OFFSET);
	pixels[chRed]   = CLAMP((y + uv_r) >> 8); // r
	pixels[chGreen] = CLAMP((y + uv_g) >> 8); // g
	pixels[chBlue]  = CLAMP((y + uv_b) >> 8); // b
	pixels+=3;
	// 2nd pixel
	y=YUV2RGB_11*(yuvdata[3] -Y_OFFSET);
	pixels[chRed]   = CLAMP((y + uv_r) >> 8); // r
	pixels[chGreen] = CLAMP((y + uv_g) >> 8); // g
	pixels[chBlue]  = CLAMP((y + uv_b) >> 8); // b
	pixels+=3;

	yuvdata+=4;
      }
#endif
      STOP_TIMING("YUV2RGB");
    }
    break;
  case GL_RGBA:
  case GL_BGRA: /* ==GL_BGRA_EXT */
    {
      START_TIMING;
#if 0
	  YUV422_to_BGRA_altivec( yuvdata, pixelnum, data);
#elif defined __SSE2__
	  UYVY_to_RGBA_SSE2(yuvdata, pixelnum, data);
#else
      unsigned char *pixels=data;
      int y, u, v;
      int uv_r, uv_g, uv_b;
      pixelnum>>=1;
      while(pixelnum--){
	    u=yuvdata[0]-UV_OFFSET;
		v=yuvdata[2]-UV_OFFSET;
		uv_r=YUV2RGB_12*v+YUV2RGB_13*v;
		uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
		uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

		// 1st pixel
		y=YUV2RGB_11*(yuvdata[1] -Y_OFFSET);
		pixels[chRed]   = CLAMP((y + uv_r) >> 8); // r
		pixels[chGreen] = CLAMP((y + uv_g) >> 8); // g
		pixels[chBlue]  = CLAMP((y + uv_b) >> 8); // b
		pixels[chAlpha] = 255;
		pixels+=4;
		// 2nd pixel
		y=YUV2RGB_11*(yuvdata[3] -Y_OFFSET);
		pixels[chRed]   = CLAMP((y + uv_r) >> 8); // r
		pixels[chGreen] = CLAMP((y + uv_g) >> 8); // g
		pixels[chBlue]  = CLAMP((y + uv_b) >> 8); // b
		pixels[chAlpha] = 255;
		pixels+=4;

		yuvdata+=4;
      }
#endif
      STOP_TIMING("UYVY_to_RGBA/BGRA");
    }
    break;
  }
}

GEM_EXTERN void imageStruct::fromYUY2(unsigned char *yuvdata) { // YUYV
  if(!yuvdata)return;
  data=pdata;
  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  unsigned char *pixels=pdata;
  switch (format){
  case GL_YUV422_GEM:
    pixelnum>>=1;
    while(pixelnum--){
      pixels[0]=yuvdata[1]; // u
      pixels[1]=yuvdata[0]; // y
      pixels[2]=yuvdata[3]; // v
      pixels[3]=yuvdata[2]; // y
      pixels+=4;
      yuvdata+=4;
    }
    break;
  case GL_LUMINANCE:
    pixelnum>>=1;
    while(pixelnum--){
      *pixels++=yuvdata[0];
      *pixels++=yuvdata[2];
      yuvdata+=4;
    }
    break;
  case GL_RGB:
  case GL_BGR:
    {
      unsigned char *pixels=data;
      int y, u, v;
      int uv_r, uv_g, uv_b;
      pixelnum>>=1;

      while(pixelnum--){
	u=yuvdata[1]-UV_OFFSET;
	v=yuvdata[3]-UV_OFFSET;
	uv_r=YUV2RGB_12*u+YUV2RGB_13*v;
	uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
	uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

	// 1st pixel
	y=YUV2RGB_11*(yuvdata[0] -Y_OFFSET);
	pixels[chRed]   = CLAMP((y + uv_r) >> 8); // r
	pixels[chGreen] = CLAMP((y + uv_g) >> 8); // g
	pixels[chBlue]  = CLAMP((y + uv_b) >> 8); // b
	pixels+=3;
	// 2nd pixel
	y=YUV2RGB_11*(yuvdata[2] -Y_OFFSET);
	pixels[chRed]   = CLAMP((y + uv_r) >> 8); // r
	pixels[chGreen] = CLAMP((y + uv_g) >> 8); // g
	pixels[chBlue]  = CLAMP((y + uv_b) >> 8); // b
	pixels+=3;

	yuvdata+=4;
      }
    }
    break;
  case GL_RGBA:
  case GL_BGRA: /* ==GL_BGRA_EXT */
    {
      unsigned char *pixels=data;
      int y, u, v;
      int uv_r, uv_g, uv_b;
      pixelnum>>=1;
      while(pixelnum--){
	u=yuvdata[1]-UV_OFFSET;
	v=yuvdata[3]-UV_OFFSET;
	uv_r=YUV2RGB_12*u+YUV2RGB_13*v;
	uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
	uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

	// 1st pixel
	y=YUV2RGB_11*(yuvdata[0] -Y_OFFSET);
	pixels[chRed]   = CLAMP((y + uv_r) >> 8); // r
	pixels[chGreen] = CLAMP((y + uv_g) >> 8); // g
	pixels[chBlue]  = CLAMP((y + uv_b) >> 8); // b
	pixels[chAlpha] = 255;
	pixels+=4;
	// 2nd pixel
	y=YUV2RGB_11*(yuvdata[2] -Y_OFFSET);
	pixels[chRed]   = CLAMP((y + uv_r) >> 8); // r
	pixels[chGreen] = CLAMP((y + uv_g) >> 8); // g
	pixels[chBlue]  = CLAMP((y + uv_b) >> 8); // b
	pixels[chAlpha] = 255;
	pixels+=4;

	yuvdata+=4;
      }
    }
    break;
  }
}

GEM_EXTERN void imageStruct::fromYVYU(unsigned char *yuvdata) {
  // this is the yuv-format with Gem
  if(!yuvdata)return;
  data=pdata;
  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  unsigned char *pixels=pdata;
  switch (format){
  case GL_YUV422_GEM:
    pixelnum>>=1;
    while(pixelnum--){
      pixels[chU]=yuvdata[1]; // u
      pixels[chY0]=yuvdata[0]; // y
      pixels[chV]=yuvdata[3]; // v
      pixels[chY1]=yuvdata[2]; // y
      pixels+=4;
      yuvdata+=4;
    }
    break;
  case GL_LUMINANCE:
    pixelnum>>=1;
    while(pixelnum--){
      *pixels++=yuvdata[0];
      *pixels++=yuvdata[2];
      yuvdata+=4;
    }
    break;
  case GL_RGB:  case GL_BGR:
    {
      unsigned char *pixels=data;
      int y, u, v;
      int uv_r, uv_g, uv_b;
      pixelnum>>=1;

      while(pixelnum--){
	u=yuvdata[3]-UV_OFFSET;
	v=yuvdata[1]-UV_OFFSET;
	uv_r=YUV2RGB_12*u+YUV2RGB_13*v;
	uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
	uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

	// 1st pixel
	y=YUV2RGB_11*(yuvdata[0] -Y_OFFSET);
	pixels[chRed]   = CLAMP((y + uv_r) >> 8); // r
	pixels[chGreen] = CLAMP((y + uv_g) >> 8); // g
	pixels[chBlue]  = CLAMP((y + uv_b) >> 8); // b
	pixels+=3;
	// 2nd pixel
	y=YUV2RGB_11*(yuvdata[2] -Y_OFFSET);
	pixels[chRed]   = CLAMP((y + uv_r) >> 8); // r
	pixels[chGreen] = CLAMP((y + uv_g) >> 8); // g
	pixels[chBlue]  = CLAMP((y + uv_b) >> 8); // b
	pixels+=3;

	yuvdata+=4;
      }
    }
    break;
  case GL_RGBA:
  case GL_BGRA: /* ==GL_BGRA_EXT */
    {
      unsigned char *pixels=data;
      int y, u, v;
      int uv_r, uv_g, uv_b;
      pixelnum>>=1;
      while(pixelnum--){
	u=yuvdata[3]-UV_OFFSET;
	v=yuvdata[1]-UV_OFFSET;
	uv_r=YUV2RGB_12*u+YUV2RGB_13*v;
	uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
	uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

	// 1st pixel
	y=YUV2RGB_11*(yuvdata[0] -Y_OFFSET);
	pixels[chRed]   = CLAMP((y + uv_r) >> 8); // r
	pixels[chGreen] = CLAMP((y + uv_g) >> 8); // g
	pixels[chBlue]  = CLAMP((y + uv_b) >> 8); // b
	pixels[chAlpha] = 255;
	pixels+=4;
	// 2nd pixel
	y=YUV2RGB_11*(yuvdata[2] -Y_OFFSET);
	pixels[chRed]   = CLAMP((y + uv_r) >> 8); // r
	pixels[chGreen] = CLAMP((y + uv_g) >> 8); // g
	pixels[chBlue]  = CLAMP((y + uv_b) >> 8); // b
	pixels[chAlpha] = 255;
	pixels+=4;

	yuvdata+=4;
      }
    }
    break;
  }
}

GEM_EXTERN extern int getPixFormat(char*cformat){
  char c=tolower(*cformat);
  switch(c){
  case 'g': return GL_LUMINANCE;
  case 'y': return GL_YUV422_GEM;
  case 'r': 
#ifndef __APPLE__
    return GL_RGBA;
#else
    return GL_BGRA_EXT;
#endif
  }
  return 0;
}

/* swap the Red and Blue channel _in-place_ */
GEM_EXTERN void imageStruct::swapRedBlue() {
  size_t pixelnum=xsize*ysize;
  unsigned char *pixels=data;
  unsigned char dummy=0;
  switch (format){
  case GL_YUV422_GEM:
    pixelnum>>=1;
    while(pixelnum--){
      dummy=pixels[chU];
      pixels[chU]=pixels[chV]; // u
      pixels[chV]=dummy; // v
      pixels+=4;
    }
    break;
  case GL_RGB:  case GL_BGR:
    while(pixelnum--){
      dummy=pixels[chRed];
      pixels[chRed]=pixels[chBlue];
      pixels[chBlue]=dummy;
      pixels+=3;
    }
    break;
  case GL_RGBA:
  case GL_BGRA: /* ==GL_BGRA_EXT */
    while(pixelnum--){
      dummy=pixels[chRed];
      pixels[chRed]=pixels[chBlue];
      pixels[chBlue]=dummy;
      pixels+=4;
    }
    break;
  }
}
