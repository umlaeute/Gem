/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    GemPixUtil.h
       - contains image functions for pix objects
       - part of GEM

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMPIXUTIL_H_
#define INCLUDE_GEMPIXUTIL_H_

// I hate Microsoft...I shouldn't have to do this!
#ifdef _WINDOWS
# include <windows.h>

# pragma warning( disable : 4244 )
# pragma warning( disable : 4305 )
# pragma warning( disable : 4091 )
#endif

# include "config.h"

#ifdef __APPLE__
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <OpenGL/glext.h>
# include <QuickTime/QuickTime.h>
#else
# include <GL/gl.h>
# ifndef DONT_INCLUDE_GLEXT
/* yhä: in glext.h some GL_YCBCR_* definitions might be hidden */
#  include <GL/glext.h>
# endif
#endif // __APPLE__

#include <string.h>
#include <stdlib.h>



///////////////////////////////////////////////////////////////////////////////
// Color component defines
//
// These should be used to reference the various color channels
///////////////////////////////////////////////////////////////////////////////

/* RGBA */
#ifdef __APPLE__				//tigital
const int chAlpha	= 0;
const int chRed		= 1;
const int chGreen	= 2;
const int chBlue	= 3;
#else
const int chRed		= 0;
const int chGreen	= 1;
const int chBlue	= 2;
const int chAlpha	= 3;
#endif

/* Gray */
const int chGray	= 0;

/* YUV422 */
const int chU           = 0;
const int chY0          = 1;
const int chV           = 2;
const int chY1          = 3;

// packed pixel defines for textures
#ifndef GL_EXT_packed_pixels
#define GL_UNSIGNED_BYTE_3_3_2_EXT          0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4_EXT       0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1_EXT       0x8034
#define GL_UNSIGNED_INT_8_8_8_8_EXT         0x8035
#define GL_UNSIGNED_INT_10_10_10_2_EXT      0x8036
#endif


// windows has this oh so old openGL installed...
#if !defined GL_BGRA && defined GL_BGRA_EXT
# define GL_BGRA GL_BGRA_EXT
#endif
#if !defined GL_BGRA_EXT && defined GL_BGRA
# define GL_BGRA_EXT GL_BGRA
#endif
#if !defined GL_BGRA && !defined GL_BGRA_EXT
# define GL_BGRA 0x80E1
# define GL_BGRA_EXT 0x80E1
#endif

#if !defined GL_BGR && defined GL_BGR_EXT
# define GL_BGR GL_BGR_EXT
#endif
#if !defined GL_BGR_EXT && defined GL_BGR
# define GL_BGR_EXT GL_BGR
#endif
#if !defined GL_BGR && !defined GL_BGR_EXT
# define GL_BGR 0x80E0
# define GL_BGR_EXT 0x80E0
#endif

#ifndef GL_YUV422_GEM

# ifdef GL_YCBCR_422_APPLE
#  define GL_YCBCR_422_GEM GL_YCBCR_422_APPLE
# elif defined GL_CRYCBY_422_NVX
#  define GL_YCBCR_422_GEM GL_CRYCBY_422_NVX
//#  define GL_YCBCR_422_GEM GL_YCRYCB_422_NVX
# elif defined GL_YCRCB_422_SGIX
#  define GL_YCBCR_422_GEM GL_YCRCB_422_SGIX
# endif

# ifndef GL_YCBCR_422_GEM
#  define GL_YCBCR_422_GEM 0x85B9
# endif

# define GL_YUV422_GEM GL_YCBCR_422_GEM

#endif /* GL_YUV422_GEM */

#if !defined(GL_TEXTURE_RECTANGLE_EXT) && defined(GL_TEXTURE_RECTANGLE_NV)
#define GL_TEXTURE_RECTANGLE_EXT GL_TEXTURE_RECTANGLE_NV
#endif


// basic helper functions, like CLAMP and powerOfTwo
#include "Base/GemFuncUtil.h"

// utility functions from PeteHelpers.h
#include "Base/GemPixPete.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    imageStruct
    
    The basic image structure

-----------------------------------------------------------------*/
// we now have a class "imageStruct";
// since i need to compile some of the sources with an older version of Gem
// there is a  new define here:
#define IMAGE_CLASS

struct GEM_EXTERN imageStruct
{
  imageStruct() : xsize (0),ysize(0),csize(0),
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

  ~imageStruct() { clear(); }
  void info();
  //////////
  // columns
  unsigned char* allocate(int size);
  unsigned char* allocate();

  // if we have allocated some space already, only re-allocate when needed.
  unsigned char* reallocate(int size);
  unsigned char* reallocate();
 
  // delete the buffer (if it is ours)
  void clear();


  //////////
  // dimensions of the image
  GLint             xsize;
  GLint   	    ysize;

  //////////
  // (average) width of 1 pixel (LUMINANCE = 1, RGBA = 4, YUV = 2)
  GLint   	    csize;

  //////////
  // data type - always GL_UNSIGNED_BYTE (except for OS X)
  GLenum          type;

  //////////
  // the format - either GL_RGBA, GL_LUMINANCE
  // or GL_YCBCR_422_GEM (which is on mac-computers GL_YCBCR_422_APPLE)
  GLenum          format;
  
  //////////
  // is this owned by us (? what about underscores ?)
  int notowned;
  
  //////////
  // gets a pixel
  /* X,Y are the coordinates
   * C is the offset in the interleaved data (like chRed==0 for red)
   * you should use chRed instead of 0 (because it might not be 0)
   */
  // heck, why are X&Y swapped ?? (JMZ)
  inline unsigned char GetPixel(int Y, int X, int C)
  { return(data[Y * xsize * csize + X * csize + C]); }
  
  //////////
  // sets a pixel
  /* while X and Y should be clear (coordinates), 
   * C is the offset (like chRed==0 for red).
   * VAL is the value to set.
   */
  inline void SetPixel(int Y, int X, int C, unsigned char VAL)
  { data[Y * xsize * csize + X * csize + C] = VAL; }

  /* following will set the whole image-data to either black or white
   * the size of the image-data is NOT xsize*ysize*csize but datasize
   * this is mostly slower
   * i have put the datasize into private (like pdata) (bad idea?)
   */
  void setBlack();
  void setWhite();

  /* certain formats are bound to certain csizes,
   * it's quite annoying to have to think again and again (ok, not much thinking)
   * so we just give the format (like GL_LUMINANCE) 
   * and it will set the image format to this format
   * and set and return the correct csize (like 1)
   * if no format is given the current format is used
   */
  int setCsizeByFormat(int format);
  int setCsizeByFormat();

  
  /* various copy functions
   * sometimes we want to copy the whole image (including pixel-data),
   * but often it is enough to just copy the meta-data (without pixel-data)
   * into a new imageStruct
   */
  void copy2Image(imageStruct *to);
  void copy2ImageStruct(imageStruct *to); // copy the imageStruct (but not the actual data)
  /* this is a sort of better copy2Image, 
   * which only copies the imageStruct-data if it is needed
   */
  void refreshImage(imageStruct *to);

  ///////////////////////////////////////////////////////////////////////////////
  // acquiring data including colour-transformations
  // should be accelerated if possible
  /* i wonder whether this is the right place to put these routines
   * they should be stored somewhere centrally
   * (because maybe a lot of objects would like them) (like [pix_rgba]...)
   * but it might be better to put them (the actual conversion routines) into
   * separate files (a separate library?)
   * orgdata points to the actual data in the given format
   * the datasize will be read from image.xsize, image.ysize
   * the dest-format will be given by image.format
   *   this is maybe not really clean (the meta-data is stored in the destination, 
   *   while the source has no meta-data of its own)
   */
  void fromRGB    (unsigned char* orgdata);
  void fromRGBA   (unsigned char* orgdata);
  void fromBGR    (unsigned char* orgdata);
  void fromBGRA   (unsigned char* orgdata);
  void fromRGB16  (unsigned char* orgdata);
  void fromGray   (unsigned char* orgdata);
  void fromUYVY   (unsigned char* orgdata);
  void fromYUY2   (unsigned char* orgdata); // YUYV
  void fromYVYU   (unsigned char* orgdata);
  /* planar YUV420: this is rather generic and not really YV12 only */
  void fromYV12   (unsigned char* Y, unsigned char*U, unsigned char*V);
  /* assume that the planes are near each other: YVU */
  void fromYV12   (unsigned char* orgdata);
  /* overloading the above two in order to accept pdp YV12 packets */
  void fromYV12   (short* Y, short*U, short*V);
  void fromYV12   (short* orgdata);
  
  /* altivec functions */
#ifdef __VEC__
  void RGB_to_YCbCr_altivec(unsigned char *rgbdata, int RGB_size, 
							unsigned char *pixels);
  void RGBA_to_YCbCr_altivec(unsigned char *rgbadata, int RGBA_size, 
							 unsigned char *pixels);
  void BGR_to_YCbCr_altivec(unsigned char *bgrdata, int BGR_size, 
							unsigned char *pixels);
  void BGRA_to_YCbCr_altivec(unsigned char *bgradata, unsigned int BGRA_size, 
							 unsigned char *pixels);
  void YUV422_to_BGRA_altivec(unsigned char *yuvdata, unsigned int pixelnum,
                             unsigned char *pixels);
  void YV12_to_RGB_altivec(short*Y, short*U, short*V, int pixelnum);
  void YV12_to_RGBA_altivec(short*Y, short*U, short*V, int pixelnum);
  void YV12_to_YUV422_altivec(short*Y, short*U, short*V, int pixelnum);
#endif
  
  /* aliases */
  void fromYUV422 (unsigned char* orgdata){fromUYVY(orgdata);}
  void fromYUV420P(unsigned char* orgdata){fromYV12(orgdata);}
  void fromYUV420P(unsigned char*Y,unsigned char*U,unsigned char*V){fromYV12(Y,U,V);}

  // "data" points to the image.
  // the memory could(!) be reserved by this class or someone else
  // "notowned" should be set to "1", if "data" points to foreign memory
  // "data" is not freed directly, when the destructor is called
  unsigned char   *data;    // the pointer to the data
  private:
  // "pdata" is the private data, and is the memory reserved by this class
  // this data is freed when the destructor is called
  unsigned char   *pdata;
  // "datasize" is the size of data reserved at "pdata"    
  int             datasize;
  
  public:
  //////////
  // true if the image is flipped horizontally
  GLboolean       upsidedown;

};

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pixBlock
    
    The pix block structure

-----------------------------------------------------------------*/
struct GEM_EXTERN pixBlock
{
    //////////
    // the block's image
    imageStruct     image;

    //////////
    // is this a newimage since last time?
    //	ie, has it been refreshed
    int     	    newimage;
    
    //////////
    // keeps track of when new films are loaded
    //	useful for rectangle_textures on OSX
    int		    newfilm;
};

///////////////////////////////////////////////////////////////////////////////
// imageStruct utility functions
//
///////////////////////////////////////////////////////////////////////////////
//////////
// copies all of the data over and mallocs memory
GEM_EXTERN extern void copy2Image(imageStruct *to, imageStruct *from);

//////////
// assumes that it only has to refresh the data
GEM_EXTERN extern void refreshImage(imageStruct *to, imageStruct *from);

GEM_EXTERN extern int getPixFormat(char*);
#endif // GEMPIXUTIL_H_
