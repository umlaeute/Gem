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

#ifndef GL_BGRA
# define GL_BGRA GL_BGRA_EXT
#endif
#ifndef GL_BGR
# define GL_BGR GL_BGR_EXT
#endif


#if GEM_RAW_GRAY != GL_LUMINANCE
# error GL_RAW_GRAY broken
#endif
#if GEM_RAW_UYVY != GL_YCBCR_422_APPLE
# error GL_RAW_UYVY broken
#endif
#if GEM_RAW_RGB != GL_RGB
# error GL_RAW_RGB broken
#endif
#if GEM_RAW_RGBA != GL_RGBA
# error GL_RAW_RGBA broken
#endif
#if GEM_RAW_BGR != GL_BGR
# error GL_RAW_BGR broken
#endif
#if GEM_RAW_BGRA != GL_BGRA
# error GL_RAW_BGR broken
#endif


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

#define RGBtoUYVY(indata, outdata, xsize, ysize)          \
  switch(m_simd) {                                        \
  case GEM_SIMD_ALTIVEC:                                  \
    RGBtoUYVY_Altivec(indata, outdata, xsize, ysize);     \
    break;                                                \
  case GEM_SIMD_NONE:                                     \
  default:                                                \
    RGBtoUYVY(indata, outdata, xsize, ysize);             \
    break;                                                \
  }

#define RGBAtoUYVY(indata, outdata, xsize, ysize);      \
  switch(m_simd) {                                      \
  case GEM_SIMD_ALTIVEC:                                \
    RGBAtoUYVY_Altivec(indata, outdata, xsize, ysize);  \
    break;                                              \
  case GEM_SIMD_SSE2:                                   \
    RGBAtoUYVY_SSE2(indata, pixels, xsize, ysize);      \
    break;                                              \
  case GEM_SIMD_NONE:                                   \
  default:                                              \
    RGBAtoUYVY(indata, outdata, xsize, ysize);          \
    break;                                              \
  }

#define BGRtoUYVY(indata, outdata, xsize, ysize);     \
  switch(m_simd) {                                    \
  case GEM_SIMD_ALTIVEC:                              \
    BGRtoUYVY_Altivec(indata, outdata, xsize, ysize); \
    break;                                            \
  case GEM_SIMD_NONE:                                 \
  default:                                            \
    BGRtoUYVY(indata, outdata, xsize, ysize);         \
    break;                                            \
  }

#define BGRAtoUYVY(indata, outdata, xsize, ysize)       \
  switch(m_simd) {                                      \
  case GEM_SIMD_ALTIVEC:                                \
    BGRAtoUYVY_Altivec(indata, outdata, xsize, ysize);  \
    break;                                              \
  case GEM_SIMD_NONE:                                   \
  default:                                              \
    BGRAtoUYVY(indata, outdata, xsize, ysize);          \
    break;                                              \
  }

#define I420S16toUYVY(Y, U, V, outdata, xsize, ysize)       \
  switch(m_simd) {                                          \
  case GEM_SIMD_ALTIVEC:                                    \
    I420S16toUYVY_Altivec(Y, U, V, outdata, xsize, ysize);  \
    break;                                                  \
  case GEM_SIMD_NONE:                                       \
  default:                                                  \
    I420S16toUYVY(Y, U, V, outdata, xsize, ysize);          \
    break;                                                  \
  }

#define UYVYtoRGB(indata, outdata, xsize, ysize)    \
  switch(m_simd) {                                  \
  case GEM_SIMD_SSE2:                               \
    UYVYtoRGB_SSE2(indata, outdata, xsize, ysize);  \
    break;                                          \
  case GEM_SIMD_NONE:                               \
  default:                                          \
    UYVYtoRGB(indata, outdata, xsize, ysize);       \
    break;                                          \
  }

#define UYVYtoBGR(indata, outdata, xsize, ysize)    \
  switch(m_simd) {                                  \
  case GEM_SIMD_SSE2:                               \
    UYVYtoBGR_SSE2(indata, outdata, xsize, ysize);  \
    break;                                          \
  case GEM_SIMD_NONE:                               \
  default:                                          \
    UYVYtoBGR(indata, outdata, xsize, ysize);       \
    break;                                          \
  }

#define UYVYtoRGBA(indata, outdata, xsize, ysize)   \
  switch(m_simd) {                                  \
  case GEM_SIMD_SSE2:                               \
    UYVYtoRGBA_SSE2(indata, outdata, xsize, ysize); \
    break;                                          \
  case GEM_SIMD_NONE:                               \
  default:                                          \
    UYVYtoRGBA(indata, outdata, xsize, ysize);      \
    break;                                          \
  }

#define UYVYtoBGRA(indata, outdata, xsize, ysize)       \
  switch(m_simd) {                                      \
  case GEM_SIMD_ALTIVEC:                                \
    UYVYtoBGRA_Altivec(indata, outdata, xsize, ysize);  \
    break;                                              \
  case GEM_SIMD_NONE:                                   \
  default:                                              \
    UYVYtoBGRA(indata, outdata, xsize, ysize);          \
    break;                                              \
  }

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

  const unsigned char format2csize(int fmt) {
    switch(fmt) {
    case GL_LUMINANCE:
      return 1;
    case GL_YUV422_GEM:
      return 2;
    case GL_RGB:
    case GL_BGR:
      return 3;
    case GL_RGBA:
    case GL_BGRA:
#ifdef GL_ABGR_EXT
    case GL_ABGR_EXT:
#endif
#ifdef GL_ARGB_EXT
    case GL_ARGB_EXT:
#endif
      return 4;
    default:
      break;
    }
    /* default */
    return 4;
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
  , format(GL_YUV422_GEM)
#else /* !__APPLE__ */
  , type(GL_UNSIGNED_BYTE), format(GL_RGBA)
#endif /* __APPLE__ */
  , not_owned(false), data(NULL), pdata(NULL), datasize(0)
  , upsidedown(true)
{}

imageStruct :: imageStruct(const imageStruct&org)
  : xsize(0), ysize(0), csize(0)
  , type(GL_UNSIGNED_BYTE), format(GL_RGBA)
  , not_owned(false), data(NULL), pdata(NULL), datasize(0)
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
  not_owned=false;
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
  not_owned=false;
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
  to->not_owned= true; /* but pdata is always owned by us */
}
GEM_EXTERN void imageStruct::info(void)
{
  post("imageStruct\t:%dx%dx%d\n\t\t%X\t(%x) %d\n\t\t%x\t%x\t%d",
       xsize, ysize, csize,
       data, pdata, datasize,
       format, type, not_owned);
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


GEM_EXTERN int imageStruct::setFormat(int setformat)
{
  csize = format2csize(setformat);
  switch(setformat) {
  case GL_LUMINANCE:
    format=setformat;
    type=GL_UNSIGNED_BYTE;
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
    break;

  case GL_RGB:
  case GL_BGR:
    format=setformat;
    type=GL_UNSIGNED_BYTE;
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
    break;
  }

  return csize;
}

GEM_EXTERN int imageStruct::setFormat(void)
{
  return setFormat(format);
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
  case GL_YUV422_GEM:
    i/=4;
    while(i--) {
      *dummy++=UV_OFFSET;
      *dummy++=0;
      *dummy++=UV_OFFSET;
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
  case GL_YUV422_GEM:
    i/=4;
    while(i--) {
      *dummy++=UV_OFFSET;
      *dummy++=255;
      *dummy++=UV_OFFSET;
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
    pd_error(0, "GEM: Someone sent a bogus pointer to convert from");
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
    setFormat(to_format);
  }

  upsidedown=from->upsidedown;

  bool reverse = needsReverseOrdering(from->type);

  switch (from->format) {
  default:
    pd_error(0, "%s: unable to convert from %s", __FUNCTION__, format2name(from->format));
    break;
  case GL_RGBA:
    if (reverse)
      return fromABGR(from->data);
    else
      return fromRGBA(from->data);
    break;
  case GL_BGRA: /* "RGBA" on apple */
    if (reverse)
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
  case GL_YUV422_GEM: {
    if (reverse)
      return fromYVYU(from->data); // TODO
    else
      return fromUYVY(from->data);
    }
  }
  return false;
}

GEM_EXTERN bool imageStruct::convertTo(imageStruct *to, unsigned int fmt) const
{
  if (!to || !data) {
    pd_error(0, "GEM: Someone sent a bogus pointer to convert to");
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
  setFormat();
  reallocate();
  bool reverse = needsReverseOrdering(type);

  unsigned char *pixels=data;
  switch (format) {
  default:
    pd_error(0, "%s: unable to convert to %s", __FUNCTION__, format2name(format));
    return false;
  case GL_RGB:
    RGBtoRGB(rgbdata, data, xsize, ysize);
    break;
  case GL_BGR:
    RGBtoBGR(rgbdata, data, xsize, ysize);
    break;
  case GL_RGBA:
    if(reverse)
      RGBtoABGR(rgbdata, data, xsize, ysize);
    else
      RGBtoRGBA(rgbdata, data, xsize, ysize);
    break;
  case GL_BGRA:
    if(reverse)
      RGBtoARGB(rgbdata, data, xsize, ysize);
    else
      RGBtoBGRA(rgbdata, data, xsize, ysize);
    break;
  case GL_LUMINANCE:
    RGBtoY(rgbdata, data, xsize, ysize);
    break;
  case GL_YUV422_GEM:
    if(reverse)
      RGBtoYVYU(rgbdata, data, xsize, ysize);
    else
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
  setFormat();
  reallocate();
  bool reverse = needsReverseOrdering(type);

  switch (format) {
  default:
    pd_error(0, "%s: unable to convert to %s", __FUNCTION__, format2name(format));
    return false;
  case GL_RGBA:
    if(reverse)
      RGB16toABGR(rgb16data, data, xsize, ysize);
    else
      RGB16toRGBA(rgb16data, data, xsize, ysize);
    break;
  case GL_LUMINANCE:
    RGB16toY(rgb16data, data, xsize, ysize);
    break;
  case GL_YUV422_GEM:
    if(reverse)
      RGB16toYVYU(rgb16data, data, xsize, ysize);
    else
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
  setFormat();
  reallocate();
  bool reverse = needsReverseOrdering(type);

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
    if(reverse)
      RGBAtoABGR(rgbadata, data, xsize, ysize);
    else
      RGBAtoRGBA(rgbadata, data, xsize, ysize);
    break;
  case GL_ABGR_EXT:
    if(reverse)
      RGBAtoRGBA(rgbadata, data, xsize, ysize);
    else
      RGBAtoABGR(rgbadata, data, xsize, ysize);
    break;
  case GL_BGRA:
    if(reverse)
      RGBAtoARGB(rgbadata, data, xsize, ysize);
    else
      RGBAtoBGRA(rgbadata, data, xsize, ysize);
    break;
  case GL_LUMINANCE:
    RGBAtoY(rgbadata, data, xsize, ysize);
    break;
  case GL_YUV422_GEM:
    START_TIMING;
    if(reverse)
      RGBAtoYVYU(rgbadata, data, xsize, ysize);
    else
      RGBAtoUYVY(rgbadata, data, xsize, ysize);
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
  setFormat();
  reallocate();
  bool reverse = needsReverseOrdering(type);

  switch (format) {
  default:
    pd_error(0, "%s: unable to convert to %s", __FUNCTION__, format2name(format));
    return false;
  case GL_BGR:
    BGRtoBGR(bgrdata, data, xsize, ysize);
    break;
  case GL_RGB:
    BGRtoRGB(bgrdata, data, xsize, ysize);
    break;
  case GL_BGRA:
    if(reverse)
      BGRtoARGB(bgrdata, data, xsize, ysize);
    else
      BGRtoBGRA(bgrdata, data, xsize, ysize);
    break;
  case GL_RGBA:
    if(reverse)
      BGRtoABGR(bgrdata, data, xsize, ysize);
    else
      BGRtoRGBA(bgrdata, data, xsize, ysize);
    break;
  case GL_LUMINANCE:
    BGRtoY(bgrdata, data, xsize, ysize);
    break;
  case GL_YUV422_GEM:
    if(reverse)
      BGRtoYVYU(bgrdata, data, xsize, ysize);
    else
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
  setFormat();
  reallocate();
  bool reverse = needsReverseOrdering(type);

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
    if(reverse)
      BGRAtoARGB(bgradata, data, xsize, ysize);
    else
      BGRAtoBGRA(bgradata, data, xsize, ysize);
    break;
  case GL_RGBA:
    if(reverse)
      BGRAtoABGR(bgradata, data, xsize, ysize);
    else
      BGRAtoRGBA(bgradata, data, xsize, ysize);
    break;
  case GL_LUMINANCE:
    BGRAtoY(bgradata, data, xsize, ysize);
    break;
  case GL_YUV422_GEM:
    START_TIMING;
    if(reverse)
      BGRAtoYVYU(bgradata, data, xsize, ysize);
    else
      BGRAtoUYVY(bgradata, data, xsize, ysize);
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
  setFormat();
  reallocate();
  bool reverse = needsReverseOrdering(type);

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
    if(reverse)
      ABGRtoRGBA(abgrdata, data, xsize, ysize);
    else
      ABGRtoABGR(abgrdata, data, xsize, ysize);
    break;
  case GL_BGRA:
    if(reverse)
      ABGRtoARGB(abgrdata, data, xsize, ysize);
    else
      ABGRtoBGRA(abgrdata, data, xsize, ysize);
    break;
  case GL_RGBA:
    if(reverse)
      ABGRtoABGR(abgrdata, data, xsize, ysize);
    else
      ABGRtoRGBA(abgrdata, data, xsize, ysize);
    break;
  case GL_LUMINANCE:
    ABGRtoY(abgrdata, data, xsize, ysize);
    break;
  case GL_YUV422_GEM:
    if(reverse)
      ABGRtoYVYU(abgrdata, data, xsize, ysize);
    else
    ABGRtoUYVY(abgrdata, data, xsize, ysize);
    break;
  }
  return true;
}

GEM_EXTERN bool imageStruct::fromARGB(const unsigned char *argbdata)
{
  if(!argbdata) {
    return false;
  }
  setFormat();
  reallocate();
  bool reverse = needsReverseOrdering(type);

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
    if(reverse)
      ARGBtoBGRA(argbdata, data, xsize, ysize);
    else
      ARGBtoARGB(bgradata, data, xsize, ysize);
    break;
#endif
  case GL_BGRA:
    if(reverse)
      ARGBtoARGB(argbdata, data, xsize, ysize);
    else
      ARGBtoBGRA(argbdata, data, xsize, ysize);
    break;
  case GL_RGBA:
    if(reverse)
      ARGBtoABGR(argbdata, data, xsize, ysize);
    else
      ARGBtoRGBA(argbdata, data, xsize, ysize);
    break;
  case GL_LUMINANCE:
    ARGBtoY(argbdata, data, xsize, ysize);
    break;
  case GL_YUV422_GEM:
    if(reverse)
      ARGBtoYVYU(argbdata, data, xsize, ysize);
    else
      ARGBtoUYVY(argbdata, data, xsize, ysize);
    break;
  }
  return true;
}

GEM_EXTERN bool imageStruct::fromGray(const unsigned char *greydata)
{
  if(!greydata) {
    return false;
  }
  setFormat();
  reallocate();
  bool reverse = needsReverseOrdering(type);

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
    if(reverse)
      YtoABGR(greydata, data, xsize, ysize);
    else
      YtoRGBA(greydata, data, xsize, ysize);
    break;
  case GL_BGRA:
    if(reverse)
      YtoARGB(greydata, data, xsize, ysize);
    else
      YtoBGRA(greydata, data, xsize, ysize);
    break;
  case GL_LUMINANCE:
    YtoY(greydata, data, xsize, ysize);
    break;
  case GL_YUV422_GEM:
    if(reverse)
      YtoYVYU(greydata, data, xsize, ysize);
    else
      YtoUYVY(greydata, data, xsize, ysize);
    break;
  }
  return true;
}

GEM_EXTERN bool imageStruct::fromGray(const short *greydata_)
{
  const unsigned short*greydata = (const unsigned short*)greydata_;
  if(!greydata) {
    return false;
  }
  setFormat();
  reallocate();
  bool reverse = needsReverseOrdering(type);

  switch (format) {
  default:
    pd_error(0, "%s: unable to convert to %s", __FUNCTION__, format2name(format));
    return false;
  case GL_RGB:
    Yu16toRGB(greydata, data, xsize, ysize);
    break;
  case GL_BGR:
    Yu16toBGR(greydata, data, xsize, ysize);
    break;
  case GL_RGBA:
    if(reverse)
      Yu16toABGR(greydata, data, xsize, ysize);
    else
      Yu16toRGBA(greydata, data, xsize, ysize);
    break;
  case GL_BGRA:
    if(reverse)
      Yu16toARGB(greydata, data, xsize, ysize);
    else
      Yu16toBGRA(greydata, data, xsize, ysize);
    break;
  case GL_LUMINANCE:
    Yu16toY(greydata, data, xsize, ysize);
    break;
  case GL_YUV422_GEM:
    if(reverse)
      Yu16toYVYU(greydata, data, xsize, ysize);
    else
      Yu16toUYVY(greydata, data, xsize, ysize);
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

  setFormat();
  reallocate();
  bool reverse = needsReverseOrdering(type);

  switch (format) {
  default:
    pd_error(0, "%s: unable to convert to %s", __FUNCTION__, format2name(format));
    return false;
  case GL_LUMINANCE:
    I420toY(Y, U, V, data, xsize, ysize);
    break;
  case GL_RGB:
    I420toRGB(Y, U, V, data, xsize, ysize);
    break;
  case GL_BGR:
    I420toBGR(Y, U, V, data, xsize, ysize);
    break;
  case GL_RGBA:
    if(reverse)
      I420toABGR(Y, U, V, data, xsize, ysize);
    else
      I420toRGBA(Y, U, V, data, xsize, ysize);
    break;
  case GL_BGRA:
    if(reverse)
      I420toARGB(Y, U, V, data, xsize, ysize);
    else
      I420toBGRA(Y, U, V, data, xsize, ysize);
    break;
  case GL_YUV422_GEM:
    if(reverse)
      I420toYVYU(Y, U, V, data, xsize, ysize);
    else
      I420toUYVY(Y, U, V, data, xsize, ysize);
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

  setFormat();
  reallocate();
  bool reverse = needsReverseOrdering(type);

  switch (format) {
  default:
    pd_error(0, "%s: unable to convert to %s", __FUNCTION__, format2name(format));
    return false;
  case GL_LUMINANCE:
    I420S16toY(Y, U, V, data, xsize, ysize);
    break;
  case GL_RGB:
    I420S16toRGB(Y, U, V, data, xsize, ysize);
    break;
  case GL_BGR:
    I420S16toBGR(Y, U, V, data, xsize, ysize);
    break;
  case GL_RGBA:
    if(reverse)
      I420S16toABGR(Y, U, V, data, xsize, ysize);
    else
      I420S16toRGBA(Y, U, V, data, xsize, ysize);
    break;
  case GL_BGRA:
    if(reverse)
      I420S16toARGB(Y, U, V, data, xsize, ysize);
    else
      I420S16toBGRA(Y, U, V, data, xsize, ysize);
    break;
  case GL_YUV422_GEM: {
    START_TIMING;
    if(reverse)
      I420S16toYVYU(Y, U, V, data, xsize, ysize);
    else
      I420S16toUYVY(Y, U, V, data, xsize, ysize);
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
  setFormat();
  reallocate();

  bool reverse = needsReverseOrdering(type);

  switch (format) {
  default:
    pd_error(0, "%s: unable to convert to %s", __FUNCTION__, format2name(format));
    return false;
  case GL_YUV422_GEM:
    if(reverse) {
      UYVYtoYVYU(yuvdata, data, xsize, ysize);
    } else {
      UYVYtoUYVY(yuvdata, data, xsize, ysize);
    }
    break;
  case GL_LUMINANCE:
    UYVYtoY(yuvdata, data, xsize, ysize);
    break;
  case GL_RGB: {
    START_TIMING;
    UYVYtoRGB(yuvdata, data, xsize, ysize);
    STOP_TIMING("YUV2RGB");
  }
    break;
  case GL_BGR: {
    START_TIMING;
    UYVYtoBGR(yuvdata, data, xsize, ysize);
    STOP_TIMING("YUV2BGR");
  }
    break;
  case GL_RGBA: {
    START_TIMING;
    if(reverse) {
      UYVYtoABGR(yuvdata, data, xsize, ysize);
    } else {
      UYVYtoRGBA(yuvdata, data, xsize, ysize);
    }
    STOP_TIMING("UYVY_to_RGBA");
  }
    break;
  case GL_BGRA: {
    START_TIMING;
    if(reverse) {
      UYVYtoARGB(yuvdata, data, xsize, ysize);
    } else {
      UYVYtoBGRA(yuvdata, data, xsize, ysize);
    }
    STOP_TIMING("UYVY_to_BGRA");
  }
    break;
  }
  return true;
}

GEM_EXTERN bool imageStruct::fromYUY2(const unsigned char*yuvdata)   // YUYV
{
  if(!yuvdata) {
    return false;
  }
  setFormat();
  reallocate();
  bool reverse = needsReverseOrdering(type);

  switch (format) {
  default:
    pd_error(0, "%s: unable to convert to %s", __FUNCTION__, format2name(format));
    return false;
  case GL_YUV422_GEM:
    if(reverse)
      YUYVtoYVYU(yuvdata, data, xsize, ysize);
    else
      YUYVtoUYVY(yuvdata, data, xsize, ysize);
    break;
  case GL_LUMINANCE:
    YUYVtoY(yuvdata, data, xsize, ysize);
    break;
  case GL_RGB:
    YUYVtoRGB(yuvdata, data, xsize, ysize);
    break;
  case GL_BGR:
    YUYVtoBGR(yuvdata, data, xsize, ysize);
    break;
  case GL_RGBA:
    if(reverse)
      YUYVtoABGR(yuvdata, data, xsize, ysize);
    else
      YUYVtoRGBA(yuvdata, data, xsize, ysize);
    break;
  case GL_BGRA:
    if(reverse)
      YUYVtoARGB(yuvdata, data, xsize, ysize);
    else
      YUYVtoBGRA(yuvdata, data, xsize, ysize);
    break;
  }
  return true;
}

GEM_EXTERN bool imageStruct::fromYVYU(const unsigned char *yuvdata)
{
  if(!yuvdata) {
    return false;
  }
  setFormat();
  bool reverse = needsReverseOrdering(type);

  reallocate();
  switch (format) {
  default:
    pd_error(0, "%s: unable to convert to %s", __FUNCTION__, format2name(format));
    return false;
  case GL_YUV422_GEM:
    if(reverse)
      YVYUtoYVYU(yuvdata, data, xsize, ysize);
    else
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
    if(reverse)
      YVYUtoABGR(yuvdata, data, xsize, ysize);
    else
      YVYUtoRGBA(yuvdata, data, xsize, ysize);
    break;
  case GL_BGRA:
    if(reverse)
      YVYUtoARGB(yuvdata, data, xsize, ysize);
    else
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

namespace {
  template <class T>
  void _yuv2rgb(const T y, const T u, const T v,
                T&r, T&g, T&b) {
    const T Y = y*static_cast<T>(YUV2RGB_11);
    r = CLAMP(Y + u*static_cast<T>(YUV2RGB_12) + v*static_cast<T>(YUV2RGB_13));
    g = CLAMP(Y + u*static_cast<T>(YUV2RGB_22) + v*static_cast<T>(YUV2RGB_23));
    b = CLAMP(Y + u*static_cast<T>(YUV2RGB_32) + v*static_cast<T>(YUV2RGB_33));
  }
  template<>
  void _yuv2rgb(const unsigned char y, const unsigned char u, const unsigned char v,
                unsigned char&r, unsigned char&g, unsigned char&b) {
    const int Y = YUV2RGB_11*(y - Y_OFFSET);
    const int U = u - UV_OFFSET;
    const int V = v - UV_OFFSET;
    const int uv_r=YUV2RGB_12*U + YUV2RGB_13*V;
    const int uv_g=YUV2RGB_22*U + YUV2RGB_23*V;
    const int uv_b=YUV2RGB_32*U + YUV2RGB_33*V;
    r = CLAMP((Y + uv_r) >> 8);
    g = CLAMP((Y + uv_g) >> 8);
    b = CLAMP((Y + uv_b) >> 8);
  }

  template <class T>
  void _getRGB(const T*data, unsigned int format, size_t position, T&red, T&green, T&blue, T&alpha) {
    int csize = format2csize(format);
    const T*pixels = data+position*csize;
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
      /* ARGB */
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
    case GL_YUV422_GEM:
      pixels = data + ((position>>1)<<1)*csize;
      _yuv2rgb(pixels[(position%2)?chY1:chY0], pixels[chU], pixels[chV], red, green, blue);
      break;
    default:
      break;
    }
  }
};

GEM_EXTERN bool imageStruct::getRGB(int X, int Y, unsigned char*r,
                                    unsigned char*g, unsigned char*b, unsigned char*a) const
{
  bool reverse = needsReverseOrdering(type);
  unsigned char red=0, green=0, blue=0, alpha=255;
  int position = (X+(upsidedown?(ysize-Y-1):Y)*xsize);
  _getRGB(data, format, position, red, green, blue, alpha);
  if(r) *r=red;
  if(g) *g=green;
  if(b) *b=blue;
  if(a) *a=alpha;
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
