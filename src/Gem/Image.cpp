////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "Gem/GemConfig.h"

// I hate Microsoft...I shouldn't have to do this!
#ifdef _WIN32
# pragma warning( disable : 4244 )
# pragma warning( disable : 4305 )
# pragma warning( disable : 4091 )
#endif

#include <m_pd.h>
#include "Image.h"
#include "GemGL.h"
#include "PixConvert.h"
#include "Utils/Functions.h"
// utility functions from PeteHelpers.h
//#include "Utils/PixPete.h"

#if GEM_GRAY != GL_LUMINANCE
# error Grayscale colortype broken
#endif

#if GEM_YUV != GL_YUV422_GEM
# error YUV colortype broken
#endif

#if GEM_RGBA != GL_RGBA_GEM
# error RGBA colortype broken
#endif

#if GEM_RGB != GL_RGB_GEM
# error RGB colortype broken
#endif

#ifndef GL_BGRA
# define GL_BGRA GL_BGRA_EXT
#endif
#ifndef GL_BGR
# define GL_BGR GL_BGR_EXT
#endif


#include <string.h>
#include <ctype.h>

#include<new>

/* this is some magic for debugging:
 * to time execution of a code-block use
 *   'START_TIMING;' at the beginning of the block and
 *   'STOP_TIMING("something");' at the end of the block
 */
#ifdef __TIMING__
# ifdef __linux__
#  include <sys/time.h>

#  define START_TIMING float mseconds=0.f;      \
  timeval startTime, endTime;                   \
  gettimeofday(&startTime, 0)
#  define STOP_TIMING(x) gettimeofday(&endTime, 0);     \
  mseconds = (endTime.tv_sec - startTime.tv_sec)*1000 + \
    (endTime.tv_usec - startTime.tv_usec) * 0.001;      \
  post("%s frame time = %f ms", x, mseconds)
# elif defined __APPLE__
#  define START_TIMING float mseconds=0.f;      \
  UnsignedWide start, end;                      \
  Microseconds(&start)
#  define STOP_TIMING(x) Microseconds(&end);                    \
  mseconds = static_cast<float>((end.lo - start.lo) / 1000.f);  \
  post("%s frame time = %f ms", x, mseconds)
# else
#  define START_TIMING
#  define STOP_TIMING(x)
# endif /* timing for OS */
#else /* !__TIMING__ */
# define START_TIMING
# define STOP_TIMING(x)
#endif /* __TIMING__ */

#ifndef PERTHREAD
# define PERTHREAD
#endif

#ifdef __VEC__
static int m_simd=3;
#else
static int m_simd=GemSIMD::getCPU();
#endif

namespace {
  size_t type2size(unsigned int type) {
    switch(type) {
    default:
      return 1;
      break;
    case GL_FLOAT:
      return sizeof(GLfloat);
      break;
    case GL_DOUBLE:
      return sizeof(GLdouble);
      break;
    }
    /* this should never happen */
    return 0;
  }
  const char*type2name(unsigned int type) {
    static PERTHREAD char buf[1024];
    switch(type) {
    case GL_BYTE: return "BYTE";
    case GL_UNSIGNED_BYTE: return "UBYTE";
    case GL_SHORT: return "SHORT";
    case GL_UNSIGNED_SHORT: return "USHORT";
    case GL_INT: return "INT";
    case GL_UNSIGNED_INT: return "UINT";
    case GL_FLOAT: return "FLOAT";
    case GL_2_BYTES: return "2BYTES";
    case GL_3_BYTES: return "3BYTES";
    case GL_4_BYTES: return "4BYTES";
    case GL_DOUBLE: return "DOUBLE";
    default:
      break;
    }
    sprintf(buf, "<type:%d>", type);
    return buf;
  }
  const char*format2name(unsigned int format) {
    static PERTHREAD char buf[1024];
    switch(format) {
#ifdef GL_ABGR_EXT
    case GL_ABGR_EXT: return "ABGR";
#endif
#ifdef GL_ARGB_EXT
    case GL_ARGB_EXT: return "ARGB";
#endif
    case GL_BGR: return "BGR";
    case GL_BGRA: return "BGRA";
    case GL_LUMINANCE: return "LUMINANCE";
    case GL_RGB: return "RGB";
    case GL_RGBA: return "RGBA";
    case GL_YUV422_GEM: return "YUV422";
    default: break;
    }
    sprintf(buf, "<format:%d>", format);
    return buf;
  }

  const bool needsReverseOrdering(unsigned int type) {
    const bool isBigEndian =
#ifdef __BIG_ENDIAN__
      true
#else
      false
#endif
      ;
    switch(type) {
    default:
      return false;

    case GL_UNSIGNED_INT_8_8_8_8:
      /* used on LE for RGBA formats */
      return !isBigEndian;
    case GL_UNSIGNED_INT_8_8_8_8_REV:
      /* used on Darwin/BE for RGBA formats */
      return isBigEndian;
    case GL_UNSIGNED_SHORT_8_8_APPLE:
      /* used on Darwin/LE for YUV formats */
      return isBigEndian;
    case GL_UNSIGNED_SHORT_8_8_REV_APPLE:
      /* used on Darwin/BE for YUV formats */
      return !isBigEndian;
    }

    return false;
  }
}

pixBlock :: pixBlock(void)
  : image(imageStruct()), newimage(0), newfilm(0)
{}


imageStruct :: imageStruct(void)
  : xsize (0), ysize(0), csize(0)
#ifdef __APPLE__
    // or should type be GL_UNSIGNED_INT_8_8_8_8_REV ? i don't know: jmz
# ifdef __BIG_ENDIAN__
  , type(GL_UNSIGNED_SHORT_8_8_REV_APPLE)
# else
  , type(GL_UNSIGNED_SHORT_8_8_APPLE)
# endif /* __BIG_ENDIAN__ */
  , format(GL_YCBCR_422_GEM)
#else /* !__APPLE__ */
  , type(GL_UNSIGNED_BYTE), format(GL_RGBA)
#endif /* __APPLE__ */
  , notowned(false), data(NULL), pdata(NULL), datasize(0)
  , upsidedown(true)
{}

imageStruct :: imageStruct(const imageStruct&org)
  : xsize(0), ysize(0), csize(0)
  , type(GL_UNSIGNED_BYTE), format(GL_RGBA)
  , notowned(false), data(NULL), pdata(NULL), datasize(0)
  , upsidedown(true)
{
  org.copy2Image(this);
}

imageStruct :: ~imageStruct(void)
{
  clear();
}

/* align the memory to 128bit (GEM_VECTORALIGNMENT is in Utils/SIMD.h)
 * this code is taken from pd-devel (written by t.grill)
 * there used to be something in here written by g.geiger
 */
GEM_EXTERN unsigned char* imageStruct::allocate(size_t size)
{
  if (pdata) {
    delete [] pdata;
    pdata=NULL;
  }

#ifdef __APPLE__
  try {
    data = pdata =  new unsigned char [size];
  } catch ( const std::bad_alloc & e) {
    pd_error(0, "out of memory!");
    data=pdata=NULL;
    datasize=0;
    return NULL;
  }

  datasize=size;
#else
  size_t array_size= size+(GEM_VECTORALIGNMENT/8-1);
  try {
    pdata = new unsigned char[array_size];
  } catch ( const std::bad_alloc & e) {
    pd_error(0, "out of memory!");
    data=pdata=NULL;
    datasize=0;
    return NULL;
  }

  size_t alignment = (reinterpret_cast<size_t>(pdata))&
                     (GEM_VECTORALIGNMENT/8-1);
  size_t offset    = (alignment == 0?0:(GEM_VECTORALIGNMENT/8-alignment));
  data = pdata+offset;
  datasize=array_size-offset;
#endif
  notowned=false;
  //post("created data [%d] @ %x: [%d]@%x", array_size, pdata, datasize, data);
  return data;
}

GEM_EXTERN unsigned char* imageStruct::allocate(void)
{
  return allocate(xsize*ysize*csize*type2size(type));
}

GEM_EXTERN unsigned char* imageStruct::reallocate(size_t size)
{
  if (size>datasize) {
    return allocate(size);
  }
  size_t alignment = (reinterpret_cast<size_t>(pdata))&
                     (GEM_VECTORALIGNMENT/8-1);
  size_t offset    = (alignment == 0?0:(GEM_VECTORALIGNMENT/8-alignment));
  notowned=false;
  data=pdata+offset;
  return data;
}
GEM_EXTERN unsigned char* imageStruct::reallocate(void)
{
  return reallocate(xsize*ysize*csize*type2size(type));
}

GEM_EXTERN void imageStruct::clear(void)
{
  if (pdata) { // pdata is always owned by imageStruct
    delete [] pdata;
  }
  data = pdata = NULL;
  datasize=0;
}


GEM_EXTERN void imageStruct::copy2ImageStruct(imageStruct *to) const
{
  if (!to || !data) {
    pd_error(0, "GEM: Someone sent a bogus pointer to copy2ImageStruct");
    if (to) {
      to->data = NULL;
    }
    return;
  }

  // copy the information over
  to->xsize     = xsize;
  to->ysize     = ysize;
  to->csize     = csize;
  to->format    = format;
  to->type      = type;
  to->data    = data;
  /* from SIMD-branch: datasize refers to the private pdata
   * thus we shouldn't set it to something else, in order to not break
   * reallocate() and friends...
   */
  //to->datasize= datasize;
  to->upsidedown=upsidedown;
  to->notowned= true; /* but pdata is always owned by us */
}
GEM_EXTERN void imageStruct::info(void)
{
  post("imageStruct\t:%dx%dx%d\n\t\t%X\t(%x) %d\n\t\t%x\t%x\t%d",
       xsize, ysize, csize,
       data, pdata, datasize,
       format, type, notowned);
}

static bool copy_imagestruct2imagestruct(const imageStruct*from,
    imageStruct*to)
{

  if(!from || !to) {
    return false;
  }

  /* copy without new allocation if possible (speedup in convolve ..) */
  to->xsize     = from->xsize;
  to->ysize     = from->ysize;
  to->csize     = from->csize;
  to->format    = from->format;
  to->type      = from->type;
  to->reallocate();
  to->upsidedown = from->upsidedown;

  if(!from->data) {
    return false;
  }

  memcpy(to->data, from->data, from->xsize*from->ysize*from->csize*type2size(from->type));
  return true;
}

GEM_EXTERN void imageStruct::copy2Image(imageStruct *to) const
{
  if(!copy_imagestruct2imagestruct(this, to)) {
    pd_error(0, "GEM: Someone sent a bogus pointer to copy2Image");
    if (to) {
      to->data = NULL;
    }
    return;
  }
}

GEM_EXTERN void imageStruct::refreshImage(imageStruct *to) const
{
  if (!to || !data) {
    pd_error(0, "GEM: Someone sent a bogus pointer to refreshImage");
    return;
  }

  // check if we need to reallocate memory
  if (to->xsize != xsize ||
      to->ysize != ysize ||
      to->csize != csize ||
      to->type != type ||
      !to->data) {
    to->clear();
    copy2Image(to);
  } else
    // copy the data over
  {
    memcpy(to->data, this->data, to->xsize * to->ysize * to->csize * type2size(to->type));
  }
}

imageStruct&imageStruct::operator=(const imageStruct&org)
{
  copy_imagestruct2imagestruct(&org, this);
  return *this;
}


GEM_EXTERN int imageStruct::setCsizeByFormat(int setformat)
{
  switch(setformat) {
  case GL_LUMINANCE:
    format=setformat;
    type=GL_UNSIGNED_BYTE;
    csize=1;
    break;

  case GL_YUV422_GEM:
    format=setformat;
    type =
#ifdef __APPLE__
# ifdef __BIG_ENDIAN__
      GL_UNSIGNED_SHORT_8_8_REV_APPLE
# else
      GL_UNSIGNED_SHORT_8_8_APPLE
# endif
#else
      GL_UNSIGNED_BYTE
#endif
      ;
    csize=2;
    break;

  case GL_RGB:
  case GL_BGR:
    format=setformat;
    type=GL_UNSIGNED_BYTE;
    csize=3;
    break;

  case GL_RGBA:
  case GL_BGRA:
  default:
    format=setformat;
    type =
#ifdef __APPLE__
# ifdef __BIG_ENDIAN__
      GL_UNSIGNED_INT_8_8_8_8_REV
# else
      GL_UNSIGNED_INT_8_8_8_8
# endif
#else
# ifdef __BIG_ENDIAN__
      GL_UNSIGNED_INT_8_8_8_8_REV
# else
      GL_UNSIGNED_BYTE
# endif
#endif
      ;
    csize=4;
    break;
  }

  return csize;
}
GEM_EXTERN int imageStruct::setCsizeByFormat(void)
{
  return setCsizeByFormat(format);
}

void pix_addsat(unsigned char *leftPix, unsigned char *rightPix,
                size_t datasize)
{
  while(datasize--) {
    *leftPix = CLAMP_HIGH(
      static_cast<int>(*leftPix) +
      static_cast<int>(*rightPix));
    leftPix++;
    rightPix++;
  }
}


void pix_sub(unsigned char *leftPix, unsigned char *rightPix,
             size_t datasize)
{
  while(datasize--) {
    *leftPix = CLAMP_LOW(
      static_cast<int>(*leftPix) -
      static_cast<int>(*rightPix++));
    leftPix++;
  }
}

GEM_EXTERN void imageStruct::setBlack(void)
{
  size_t i = datasize;
  unsigned char* dummy=data;
  if(!data) {
    return;
  }
  switch (format) {
  case GL_YCBCR_422_GEM:
    i/=4;
    while(i--) {
      *dummy++=128;
      *dummy++=0;
      *dummy++=128;
      *dummy++=0;
    }
    break;
  default:
    memset(data, 0, datasize);
    break;
  }
}
GEM_EXTERN void imageStruct::setWhite(void)
{
  size_t i = datasize;
  unsigned char* dummy=data;
  if(!data) {
    return;
  }
  switch (format) {
  case GL_YCBCR_422_GEM:
    i/=4;
    while(i--) {
      *dummy++=128;
      *dummy++=255;
      *dummy++=128;
      *dummy++=255;
    }
    break;
  default:
    memset(data, 1, datasize);
    break;
  }
}
GEM_EXTERN bool imageStruct::convertFrom(const imageStruct *from,
    unsigned int to_format)
{
  if (!from || !from->data) {
    pd_error(0, "GEM: Someone sent a bogus pointer to convert");
    return false;
  }
  switch(from->type) {
  default:
    break;
  case GL_FLOAT:
  case GL_DOUBLE:
    pd_error(0, "GEM: Cannot convert from %s image data!", type2name(from->type));
    return false;
  }
  xsize=from->xsize;
  ysize=from->ysize;

  if(to_format>0) {
    setCsizeByFormat(to_format);
  }

  upsidedown=from->upsidedown;

  bool fixOrder = needsReverseOrdering(from->type);

  switch (from->format) {
  default:
    pd_error(0, "%s: unable to convert from %s", __FUNCTION__, format2name(from->format));
    break;
  case GL_RGBA:
    if (fixOrder)
      return fromABGR(from->data);
    else
      return fromRGBA(from->data);
    break;
  case GL_BGRA: /* "RGBA" on apple */
    if (fixOrder)
      return fromARGB(from->data);
    else
      return fromBGRA(from->data);
    break;
  case GL_RGB:
    return fromRGB(from->data);
  case GL_BGR:
    return fromBGR(from->data);
  case GL_LUMINANCE:
    return fromGray(from->data);
  case GL_YCBCR_422_GEM: // YUV
    return fromUYVY(from->data);
  }
  return false;
}

GEM_EXTERN bool imageStruct::convertTo(imageStruct *to, unsigned int fmt) const
{
  if (!to || !data) {
    pd_error(0, "GEM: Someone sent a bogus pointer to convert");
    if (to) {
      to->data = NULL;
    }
    return false;
  }
  return to->convertFrom(this, fmt);
}

GEM_EXTERN bool imageStruct::fromRGB(const unsigned char *rgbdata)
{
  if(!rgbdata) {
    return false;
  }
  setCsizeByFormat();
  reallocate();
  unsigned char *pixels=data;
  switch (format) {
  default:
    pd_error(0, "%s: unable to convert to %s", __FUNCTION__, format2name(format));
    return false;
  case GL_RGB:
    memcpy(data, rgbdata, xsize*ysize*csize);
    break;
  case GL_BGR:
    RGBtoBGR(rgbdata, data, xsize, ysize);
    break;
  case GL_RGBA:
    RGBtoRGBA(rgbdata, data, xsize, ysize);
    break;
  case GL_BGRA:
    RGBtoBGRA(rgbdata, data, xsize, ysize);
    break;
  case GL_LUMINANCE:
    RGBtoY(rgbdata, data, xsize, ysize);
    break;
  case GL_YUV422_GEM:
    RGBtoUYVY(rgbdata, data, xsize, ysize);
    break;
  }
  return true;
}

GEM_EXTERN bool imageStruct::fromRGB16(const unsigned char *rgb16data)
{
  //   B B B B B G G G   G G G R R R R R
  //   R R R R R G G G   G G G B B B B B
  if(!rgb16data) {
    return false;
  }
  const unsigned short*rgbdata=(const unsigned short*)rgb16data;
  setCsizeByFormat();
  reallocate();
  switch (format) {
  default:
    pd_error(0, "%s: unable to convert to %s", __FUNCTION__, format2name(format));
    return false;
  case GL_RGBA:
    RGB16toRGBA(rgb16data, data, xsize, ysize);
    break;
  case GL_LUMINANCE:
    RGB16toY(rgb16data, data, xsize, ysize);
    break;
  case GL_YUV422_GEM:
    RGB16toUYVY(rgb16data, data, xsize, ysize);
    break;
  }
  return true;
}

GEM_EXTERN bool imageStruct::fromRGBA(const unsigned char *rgbadata)
{
  if(!rgbadata) {
    return false;
  }
  setCsizeByFormat();
  reallocate();
  unsigned char *pixels=data;
  switch (format) {
  default:
    pd_error(0, "%s: unable to convert to %s", __FUNCTION__, format2name(format));
    return false;
  case GL_RGB:
    RGBAtoRGB(rgbadata, data, xsize, ysize);
    break;
  case GL_BGR:
    RGBAtoBGR(rgbadata, data, xsize, ysize);
    break;
  case GL_RGBA:
    memcpy(data, rgbadata, xsize*ysize*csize);
    break;
  case GL_ABGR_EXT:
    RGBAtoABGR(rgbadata, data, xsize, ysize);
    break;
  case GL_BGRA:
    RGBAtoBGRA(rgbadata, data, xsize, ysize);
    break;
  case GL_LUMINANCE:
    RGBAtoY(rgbadata, data, xsize, ysize);
    break;
  case GL_YUV422_GEM:
    START_TIMING;
    switch(m_simd) {
#ifdef __VEC__
    case GEM_SIMD_ALTIVEC:
      BGRA_to_YCbCr_altivec(rgbadata,xsize*ysize,pixels);
      break;
#endif
#ifdef __SSE2__
    case GEM_SIMD_SSE2:
      RGBA_to_UYVY_SSE2(rgbadata,xsize*ysize,pixels);
      break;
#endif
    case GEM_SIMD_NONE:
    default:
      RGBAtoUYVY(rgbadata, data, xsize, ysize);
    }
    STOP_TIMING("RGBA to UYVY");
    break;
  }
  return true;
}


GEM_EXTERN bool imageStruct::fromBGR(const unsigned char *bgrdata)
{
  if(!bgrdata) {
    return false;
  }
  setCsizeByFormat();
  reallocate();
  switch (format) {
  default:
    pd_error(0, "%s: unable to convert to %s", __FUNCTION__, format2name(format));
    return false;
  case GL_BGR:
    memcpy(data, bgrdata, xsize*ysize*csize);
    break;
  case GL_RGB:
    BGRtoRGB(bgrdata, data, xsize, ysize);
    break;
  case GL_BGRA:
    BGRtoBGRA(bgrdata, data, xsize, ysize);
    break;
  case GL_RGBA:
    BGRtoRGBA(bgrdata, data, xsize, ysize);
    break;
  case GL_LUMINANCE:
    BGRtoY(bgrdata, data, xsize, ysize);
    break;
  case GL_YUV422_GEM:
    BGRtoUYVY(bgrdata, data, xsize, ysize);
    break;
  }
  return true;
}

GEM_EXTERN bool imageStruct::fromBGRA(const unsigned char *bgradata)
{
  if(!bgradata) {
    return false;
  }
  setCsizeByFormat();
  reallocate();
  switch (format) {
  default:
    pd_error(0, "%s: unable to convert to %s", __FUNCTION__, format2name(format));
    return false;
  case GL_BGR:
    BGRAtoBGR(bgradata, data, xsize, ysize);
    break;
  case GL_RGB:
    BGRAtoBGR(bgradata, data, xsize, ysize);
    break;
  case GL_BGRA:
    memcpy(data, bgradata, xsize*ysize*csize);
    break;
  case GL_RGBA:
    BGRAtoRGBA(bgradata, data, xsize, ysize);
    break;
  case GL_LUMINANCE:
    BGRAtoY(bgradata, data, xsize, ysize);
    break;
  case GL_YUV422_GEM:
    START_TIMING;
    switch(m_simd) {
#ifdef __VEC__
    case GEM_SIMD_ALTIVEC:
      BGRA_to_YCbCr_altivec(bgradata,xsize*ysize,pixels);
      break;
#endif
    case GEM_SIMD_NONE:
    default:
      BGRAtoUYVY(bgradata, data, xsize, ysize);
    }
    STOP_TIMING("BGRA_to_YCbCr");
    break;
  }
  return true;
}



GEM_EXTERN bool imageStruct::fromABGR(const unsigned char *abgrdata)
{
  if(!abgrdata) {
    return false;
  }
  setCsizeByFormat();
  reallocate();
  switch (format) {
  default:
    pd_error(0, "%s: unable to convert to %s", __FUNCTION__, format2name(format));
    return false;
  case GL_BGR:
    ABGRtoBGR(abgrdata, data, xsize, ysize);
    break;
  case GL_RGB:
    ABGRtoRGB(abgrdata, data, xsize, ysize);
    break;
  case GL_ABGR_EXT:
    memcpy(data, abgrdata, xsize*ysize*csize);
    break;
  case GL_BGRA:
    ABGRtoBGRA(abgrdata, data, xsize, ysize);
    break;
  case GL_RGBA:
    ABGRtoRGBA(abgrdata, data, xsize, ysize);
    break;
  case GL_LUMINANCE:
    ABGRtoY(abgrdata, data, xsize, ysize);
    break;
  case GL_YUV422_GEM:
    START_TIMING;
    switch(m_simd) {
    case GEM_SIMD_NONE:
    default:
      ABGRtoUYVY(abgrdata, data, xsize, ysize);
    }
    STOP_TIMING("ABGR_to_YCbCr");
    break;
  }
  return true;
}

GEM_EXTERN bool imageStruct::fromARGB(const unsigned char *argbdata)
{
  if(!argbdata) {
    return false;
  }
  setCsizeByFormat();
  reallocate();
  switch (format) {
  default:
    pd_error(0, "%s: unable to convert to %s", __FUNCTION__, format2name(format));
    return false;
  case GL_BGR:
    ARGBtoBGR(argbdata, data, xsize, ysize);
    break;
  case GL_RGB:
    ARGBtoRGB(argbdata, data, xsize, ysize);
    break;
#if 0
  case GL_ARGB_EXT:
    memcpy(data, argbdata, xsize*ysize*csize);
    break;
#endif
  case GL_BGRA:
    post("%s:%d:: %s", __FILE__, __LINE__, __FUNCTION__);
    ARGBtoBGRA(argbdata, data, xsize, ysize);
    break;
  case GL_RGBA:
    ARGBtoRGBA(argbdata, data, xsize, ysize);
    break;
  case GL_LUMINANCE:
    ARGBtoY(argbdata, data, xsize, ysize);
    break;
  case GL_YUV422_GEM:
    START_TIMING;
    switch(m_simd) {
    case GEM_SIMD_NONE:
    default:
      ARGBtoUYVY(argbdata, data, xsize, ysize);
    }
    STOP_TIMING("ARGB_to_YCbCr");
    break;
  }
  return true;
}

GEM_EXTERN bool imageStruct::fromGray(const unsigned char *greydata)
{
  if(!greydata) {
    return false;
  }
  setCsizeByFormat();
  reallocate();
  switch (format) {
  default:
    pd_error(0, "%s: unable to convert to %s", __FUNCTION__, format2name(format));
    return false;
  case GL_RGB:
    YtoRGB(greydata, data, xsize, ysize);
    break;
  case GL_BGR:
    YtoBGR(greydata, data, xsize, ysize);
    break;
  case GL_RGBA:
    YtoRGBA(greydata, data, xsize, ysize);
    break;
  case GL_BGRA:
    YtoBGRA(greydata, data, xsize, ysize);
    break;
  case GL_LUMINANCE:
    memcpy(data, greydata, xsize*ysize);
    break;
  case GL_YUV422_GEM:
    YtoUYVY(greydata, data, xsize, ysize);
    break;
  }
  return true;
}

GEM_EXTERN bool imageStruct::fromGray(const short *greydata_)
{
  const unsigned char*greydata = (const unsigned char*)greydata_;
  if(!greydata) {
    return false;
  }
  setCsizeByFormat();
  reallocate();
  switch (format) {
  default:
    pd_error(0, "%s: unable to convert to %s", __FUNCTION__, format2name(format));
    return false;
  case GL_RGB:
    YtoRGB(greydata, data, xsize, ysize);
    break;
  case GL_BGR:
    YtoBGR(greydata, data, xsize, ysize);
    break;
  case GL_RGBA:
    YtoRGBA(greydata, data, xsize, ysize);
    break;
  case GL_BGRA:
    YtoBGRA(greydata, data, xsize, ysize);
    break;
  case GL_LUMINANCE:
    memcpy(data, greydata, xsize*ysize);
    break;
  case GL_YUV422_GEM:
    YtoUYVY(greydata, data, xsize, ysize);
    break;
  }
  return true;
}

GEM_EXTERN bool imageStruct::fromYU12(const unsigned char*yuvdata)
{
  if(!yuvdata) {
    return false;
  }
  size_t pixelnum=xsize*ysize;
  return fromYV12((yuvdata), yuvdata+(pixelnum), yuvdata+(pixelnum+(pixelnum>>2)));
}
GEM_EXTERN bool imageStruct::fromYV12(const unsigned char*yuvdata)
{
  if(!yuvdata) {
    return false;
  }
  size_t pixelnum=xsize*ysize;
  return fromYV12((yuvdata), yuvdata+(pixelnum+(pixelnum>>2)), yuvdata+(pixelnum));
}
GEM_EXTERN bool imageStruct::fromYV12(const unsigned char*Y,
                                      const unsigned char*U, const unsigned char*V)
{
  // planar: 8bit Y-plane + 8bit 2x2-subsampled V- and U-planes
  if(!U && !V) {
    return fromGray(Y);
  }
  if(!Y || !U || !V) {
    return false;
  }

  setCsizeByFormat();
  reallocate();
  switch (format) {
  default:
    pd_error(0, "%s: unable to convert to %s", __FUNCTION__, format2name(format));
    return false;
  case GL_LUMINANCE:
    memcpy(data, Y, xsize*ysize);
    break;
  case GL_RGB:
    YUV420PtoRGB(Y, U, V, data, xsize, ysize);
    break;
  case GL_BGR:
    YUV420PtoBGR(Y, U, V, data, xsize, ysize);
    break;
  case GL_RGBA:
    YUV420PtoRGBA(Y, U, V, data, xsize, ysize);
    break;
  case GL_BGRA:
    YUV420PtoBGRA(Y, U, V, data, xsize, ysize);
    break;
  case GL_YUV422_GEM:
    YUV420PtoUYVY(Y, U, V, data, xsize, ysize);
    break;
  }
  return true;
}
//  for gem2pdp
GEM_EXTERN bool imageStruct::fromYV12(const short*yuvdata)
{
  if(!yuvdata) {
    return false;
  }
  int pixelnum=xsize*ysize;
  return fromYV12((yuvdata), yuvdata+(pixelnum+(pixelnum>>2)), yuvdata+(pixelnum));
}
GEM_EXTERN bool imageStruct::fromYV12(const short*Y, const short*U,
                                      const short*V)
{
  // planar: 8bit Y-plane + 8bit 2x2-subsampled V- and U-planes
  if(!U && !V) {
    return fromGray(Y);
  }
  if(!Y || !U || !V) {
    return false;
  }

  setCsizeByFormat();
  reallocate();
  switch (format) {
  default:
    pd_error(0, "%s: unable to convert to %s", __FUNCTION__, format2name(format));
    return false;
  case GL_LUMINANCE:
    memcpy(data, Y, xsize*ysize);
    break;
  case GL_RGB:
  case GL_BGR: {
    // of course this is stupid, RGB isn't BGR
    unsigned char *pixels1=data;
    unsigned char *pixels2=data+xsize*csize;

    const short*py1=Y;
    const short*py2=Y+xsize; // plane_1 is luminance (csize==1)
    const short*pv=V;//(format==GL_BGR)?V:U;
    const short*pu=U;//(format==GL_RGB)?V:U;

    int row=ysize>>1;
    int cols=xsize>>1;
    while(row--) {
      int col=cols;
      while(col--) {
        int y, u, v;
        int uv_r, uv_g, uv_b;
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
      pixels1+=xsize*csize;
      pixels2+=xsize*csize;
      py1+=xsize*1;
      py2+=xsize*1;
    }
  }
  break;
  case GL_RGBA:
  case GL_BGRA: {
    unsigned char *pixels1=data;
    unsigned char *pixels2=data+xsize*csize;

    const short*py1=Y;//yuvdata;
    const short*py2=Y+xsize;//yuvdata+xsize; // plane_1 is luminance (csize==1)
    const short*pv=V;//(format==GL_BGRA)?U:V;
    const short*pu=U;//(format==GL_RGBA)?U:V;

    int row=ysize>>1;
    int cols=xsize>>1;
    while(row--) {
      int col=cols;
      while(col--) {
        int y, u, v;
        int uv_r, uv_g, uv_b;
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
      pixels1+=xsize*csize;
      pixels2+=xsize*csize;
      py1+=xsize*1;
      py2+=xsize*1;
    }
  }

  break;
  case GL_YUV422_GEM: {
    START_TIMING;
    switch(m_simd) {
#ifdef __VEC__
    case GEM_SIMD_ALTIVEC:
      YV12_to_YUV422_altivec(Y, U, V, data, xsize, ysize);
      break;
#endif
    case GEM_SIMD_NONE:
    default:
      unsigned char *pixels1=data;
      unsigned char *pixels2=data+xsize*csize;
      const short*py1=Y;
      const short*py2=Y+xsize; // plane_1 is luminance (csize==1)
      const short*pu=U;
      const short*pv=V;
      int row=ysize>>1;
      int cols=xsize>>1;
      unsigned char u, v;
      /* this is only re-ordering of the data */
      while(row--) {
        int col=cols;
        while(col--) {
          // yuv422 is U Y0 V Y1
          u=((*pu++)>>8)+128;
          v=((*pv++)>>8)+128;
          *pixels1++=u;
          *pixels1++=(*py1++)>>7;
          *pixels1++=v;
          *pixels1++=(*py1++)>>7;
          *pixels2++=u;
          *pixels2++=(*py2++)>>7;
          *pixels2++=v;
          *pixels2++=(*py2++)>>7;
        }
        pixels1+=xsize*csize;
        pixels2+=xsize*csize;
        py1+=xsize*1;
        py2+=xsize*1;
      }
    }
    STOP_TIMING("YV12_to_YUV422");
  }
  break;
  }
  return true;
}

GEM_EXTERN bool imageStruct::fromUYVY(const unsigned char *yuvdata)
{
  // this is the yuv-format with Gem
  if(!yuvdata) {
    return false;
  }
  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  unsigned char *pixels=data;
  switch (format) {
  default:
    pd_error(0, "%s: unable to convert to %s", __FUNCTION__, format2name(format));
    return false;
  case GL_YUV422_GEM:
    memcpy(data, yuvdata, xsize*ysize*csize);
    break;
  case GL_LUMINANCE:
    UYVYtoY(yuvdata, data, xsize, ysize);
    break;
  case GL_RGB:
  case GL_BGR: {
    unsigned char *pixels=data;
    START_TIMING;
    switch(m_simd) {
#ifdef __SSE2__
    case GEM_SIMD_SSE2:
      UYVY_to_RGB_SSE2(yuvdata, xsize*ysize, pixels);
      break;
#endif
    case GEM_SIMD_NONE:
    default:
      if(GL_BGR == format)
        UYVYtoBGR(yuvdata, pixels, xsize, ysize);
      else
        UYVYtoRGB(yuvdata, pixels, xsize, ysize);
    }
    STOP_TIMING("YUV2RGB");
  }
  break;
  case GL_RGBA:
  case GL_BGRA: {
    START_TIMING;
    switch(m_simd) {
#ifdef __VEC__
    case GEM_SIMD_ALTIVEC:
      YUV422_to_BGRA_altivec( yuvdata, xsize*ysize*2, data);
      break;
#endif
#ifdef __SSE2__
    case GEM_SIMD_SSE2:
      UYVY_to_RGBA_SSE2(yuvdata, xsize*ysize, data);
      break;
#endif
    case GEM_SIMD_NONE:
    default:
      if(GL_BGRA == format)
        UYVYtoBGRA(yuvdata, pixels, xsize, ysize);
      else
        UYVYtoRGBA(yuvdata, pixels, xsize, ysize);
    }
    STOP_TIMING("UYVY_to_RGBA/BGRA");
  }
  break;
  }
  return true;
}

GEM_EXTERN bool imageStruct::fromYUY2(const unsigned char
                                      *yuvdata)   // YUYV
{
  if(!yuvdata) {
    return false;
  }
  size_t pixelnum=xsize*ysize;
  setCsizeByFormat();
  reallocate();
  unsigned char *pixels=data;
  switch (format) {
  default:
    pd_error(0, "%s: unable to convert to %s", __FUNCTION__, format2name(format));
    return false;
  case GL_YUV422_GEM:
    YUYVtoUYVY(yuvdata, pixels, xsize, ysize);
    break;
  case GL_LUMINANCE:
    YUYVtoY(yuvdata, pixels, xsize, ysize);
    break;
  case GL_RGB:
    YUYVtoRGB(yuvdata, pixels, xsize, ysize);
    break;
  case GL_BGR:
    YUYVtoBGR(yuvdata, pixels, xsize, ysize);
    break;
  case GL_RGBA:
    YUYVtoRGBA(yuvdata, pixels, xsize, ysize);
    break;
  case GL_BGRA:
    YUYVtoBGRA(yuvdata, pixels, xsize, ysize);
    break;
  }
  return true;
}

GEM_EXTERN bool imageStruct::fromYVYU(const unsigned char *yuvdata)
{
  // this is the yuv-format with Gem
  if(!yuvdata) {
    return false;
  }
  setCsizeByFormat();
  reallocate();
  switch (format) {
  default:
    pd_error(0, "%s: unable to convert to %s", __FUNCTION__, format2name(format));
    return false;
  case GL_YUV422_GEM:
    YVYUtoUYVY(yuvdata, data, xsize, ysize);
    break;
  case GL_LUMINANCE:
    YVYUtoY(yuvdata, data, xsize, ysize);
    break;
  case GL_RGB:
    YVYUtoRGB(yuvdata, data, xsize, ysize);
    break;
  case GL_BGR:
    YVYUtoBGR(yuvdata, data, xsize, ysize);
    break;
  case GL_RGBA:
    YVYUtoRGBA(yuvdata, data, xsize, ysize);
    break;
  case GL_BGRA:
    YVYUtoBGRA(yuvdata, data, xsize, ysize);
    break;
  }
  return true;
}

GEM_EXTERN extern int getPixFormat(const char*cformat)
{
  char c=tolower(*cformat);
  switch(c) {
  case 'g':
    return GL_LUMINANCE;
  case 'y':
    return GL_YUV422_GEM;
  case 'r':
    return GL_RGBA_GEM;
  }
  return 0;
}


/* flip the image if it is upside down */
GEM_EXTERN void imageStruct::fixUpDown(void)
{
  if(upsidedown) {
    return;  /* everything's fine! */
  }

  int linewidth = xsize*csize;
  unsigned char*line = new unsigned char[linewidth];

  int y1=ysize-1;
  for(int y0=0; y0<ysize/2; y0++, y1--) {
    unsigned char*line0=data+y0*linewidth;
    unsigned char*line1=data+y1*linewidth;
    memcpy(line, line0, linewidth);
    memcpy(line0, line1, linewidth);
    memcpy(line1, line, linewidth);
  }

  delete[]line;

  upsidedown=true;
}

/* swap the Red and Blue channel _in-place_ */
GEM_EXTERN void imageStruct::swapRedBlue(void)
{
  size_t pixelnum=xsize*ysize;
  unsigned char *pixels=data;
  switch (format) {
  case GL_YUV422_GEM:
    pixelnum>>=1;
    while(pixelnum--) {
      unsigned char dummy=pixels[chU];
      pixels[chU]=pixels[chV]; // u
      pixels[chV]=dummy; // v
      pixels+=4;
    }
    break;
  case GL_RGB:
  case GL_BGR:
    while(pixelnum--) {
      unsigned char dummy=pixels[chRed];
      pixels[chRed]=pixels[chBlue];
      pixels[chBlue]=dummy;
      pixels+=3;
    }
    break;
  case GL_RGBA:
  case GL_BGRA:
    while(pixelnum--) {
      unsigned char dummy=pixels[chRed];
      pixels[chRed]=pixels[chBlue];
      pixels[chBlue]=dummy;
      pixels+=4;
    }
    break;
  }
}


GEM_EXTERN bool imageStruct::getRGB(int X, int Y, unsigned char*r,
                                    unsigned char*g, unsigned char*b, unsigned char*a) const
{
  unsigned char red=0, green=0, blue=0, alpha=255;
  int position = (X+(upsidedown?(ysize-Y-1):Y)*xsize);
  const unsigned char*pixels=data+position*csize;

  switch(format) {
  case GL_LUMINANCE:
    red=green=blue=pixels[0];
    alpha=255;
    break;
  case GL_RGB:
    red=pixels[0];
    green=pixels[1];
    blue=pixels[2];
    break;
  case GL_BGR:
    red=pixels[0];
    green=pixels[1];
    blue=pixels[2];
    break;
  case GL_RGBA:
    red=pixels[0];
    green=pixels[1];
    blue=pixels[2];
    alpha=pixels[3];
    break;
  case GL_BGRA:
#ifdef __APPLE__
    red=pixels[1];
    green=pixels[2];
    blue=pixels[3];
    alpha=pixels[0];
#else
    red=pixels[2];
    green=pixels[1];
    blue=pixels[0];
    alpha=pixels[3];
#endif
    break;
  case GL_YUV422_GEM: {
    position = (((X+(upsidedown?(ysize-Y-1):Y)*xsize)>>1)<<1);
    pixels=data+position*csize;
    int y=YUV2RGB_11*(pixels[(X%2)?chY1:chY0]-Y_OFFSET);
    int u=pixels[chU] - UV_OFFSET;
    int v=pixels[chV] - UV_OFFSET;
    int uv_r=YUV2RGB_12*u+YUV2RGB_13*v;
    int uv_g=YUV2RGB_22*u+YUV2RGB_23*v;
    int uv_b=YUV2RGB_32*u+YUV2RGB_33*v;

    red =   CLAMP((y + uv_r) >> 8);
    green = CLAMP((y + uv_g) >> 8);
    blue =  CLAMP((y + uv_b) >> 8);
  }
  break;
  default:
    break;
  }
  if(r) {
    *r=red;
  }
  if(g) {
    *g=green;
  }
  if(b) {
    *b=blue;
  }
  if(a) {
    *a=alpha;
  }
  return true;
}
GEM_EXTERN bool imageStruct::getGrey(int X, int Y, unsigned char*g) const
{
  unsigned char grey=0;
  int position = (X+(upsidedown?(ysize-Y-1):Y)*xsize);
  const unsigned char*pixels=data+position*csize;
  switch(format) {
  case GL_LUMINANCE:
    grey=pixels[0];
    break;
  case GL_RGB:
    grey=(pixels[0]*RGB2GRAY_RED+pixels[1]*RGB2GRAY_GREEN
          +pixels[2]*RGB2GRAY_BLUE)>>8;
    break;
  case GL_BGR:
    grey=(pixels[2]*RGB2GRAY_RED+pixels[1]*RGB2GRAY_GREEN
          +pixels[0]*RGB2GRAY_BLUE)>>8;
    break;
  case GL_RGBA:
    grey=(pixels[0]*RGB2GRAY_RED+pixels[1]*RGB2GRAY_GREEN
          +pixels[2]*RGB2GRAY_BLUE)>>8;
    break;
  case GL_BGRA:
    grey=(pixels[2]*RGB2GRAY_RED+pixels[1]*RGB2GRAY_GREEN
          +pixels[0]*RGB2GRAY_BLUE)>>8;
    break;
  case GL_YUV422_GEM: {
    position = (((X+(upsidedown?(ysize-Y-1):Y)*xsize)>>1)<<1);
    pixels=data+position*csize;
    grey = CLAMP(pixels[((X%2)?chY1:chY0)]-Y_OFFSET);
  }
  break;
  default:
    break;
  }
  if(g) {
    *g=grey;
  }
  return true;
}
GEM_EXTERN bool imageStruct::getYUV(int X, int Y, unsigned char*y,
                                    unsigned char*u, unsigned char*v) const
{
  unsigned char luma=0, chromaU=128, chromaV=128;
  int position = (X+(upsidedown?(ysize-Y-1):Y)*xsize);
  const unsigned char*pixels=data+position*csize;
  switch(format) {
  case GL_LUMINANCE:
    luma=pixels[0];
    break;
  case GL_RGB:
  case GL_BGR:
    pd_error(0, "getYUV not implemented for RGB");
    return false;
  case GL_RGBA:
  case GL_BGRA:
    pd_error(0, "getYUV not implemented for RGBA");
    return false;
  case GL_YUV422_GEM:
    position = (((X+(upsidedown?(ysize-Y-1):Y)*xsize)>>1)<<1);
    pixels=data+position*csize;
    luma=pixels[((X%2)?chY1:chY0)];
    chromaU=pixels[chU];
    chromaV=pixels[chV];
    break;
  default:
    break;
  }
  if(y) {
    *y=luma;
  }
  if(u) {
    *u=chromaU;
  }
  if(v) {
    *v=chromaV;
  }
  return true;
}
