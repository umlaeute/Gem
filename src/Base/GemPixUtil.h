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
#include <windows.h>
#endif

#ifdef MACOSX
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <QuickTime/QuickTime.h>
#else
#include <GL/gl.h>
#ifndef NT
#include "config.h"
#endif // NT
#endif // MACOSX

#include <string.h>
#include <stdlib.h>

// packed pixel defines for textures
#ifndef GL_EXT_packed_pixels
#define GL_UNSIGNED_BYTE_3_3_2_EXT          0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4_EXT       0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1_EXT       0x8034
#define GL_UNSIGNED_INT_8_8_8_8_EXT         0x8035
#define GL_UNSIGNED_INT_10_10_10_2_EXT      0x8036
#endif

#ifdef GL_YCBCR_422_APPLE
#define GL_YCBCR_422_GEM GL_YCBCR_422_APPLE
#elif defined GL_CRYCBY_422_NVX
#define GL_YCBCR_422_GEM GL_CRYCBY_422_NVX
//#define GL_YCBCR_422_GEM GL_YCRYCB_422_NVX
#elif defined GL_YCRCB_422_SGIX
#define GL_YCBCR_422_GEM GL_YCRCB_422_SGIX
#endif

#ifndef GL_YCBCR_422_GEM
#define GL_YCBCR_422_GEM 0x85B9
#endif

#define GL_YUV422_GEM GL_YCBCR_422_GEM

#if !defined(GL_TEXTURE_RECTANGLE_EXT) && defined(GL_TEXTURE_RECTANGLE_NV)
#define GL_TEXTURE_RECTANGLE_EXT GL_TEXTURE_RECTANGLE_NV
#endif


#include "Base/GemFuncUtil.h"

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
    notowned(0),data(0),pdata(0),datasize(0)
{}

  ~imageStruct() { clear(); }
  void info();
  //////////
  // columns
  unsigned char* allocate(int size) {
    if (pdata&&!notowned)
      delete [] pdata;
#if 1
    pdata = new unsigned char[size+31];
    data = (unsigned char*) ((((unsigned int)pdata)+31)& (~31));
#else
    data = pdata =  new unsigned char [size];
#endif
    datasize=size;
    notowned=0;
    return data; 
  }
  unsigned char* allocate() {  return allocate(xsize*ysize*csize);  }

  // if we have allocated some space already, only re-allocate when needed.
  unsigned char* reallocate(int size) {
    if (size>datasize)
      return allocate(size);
    return data;
  }
  unsigned char* reallocate() {  return reallocate(xsize*ysize*csize);  }
 
  void clear() {
    if (!notowned && pdata) {
      delete [] pdata;
      data = pdata = 0;      
    }
    xsize = ysize = csize = 0;
    datasize=0;
  }
  
  GLint             xsize;

  //////////
  // rows
  GLint   	    ysize;

  //////////
  // color (LUMINANCE = 1, RGBA = 4)
  GLint   	    csize;

  //////////
  // data type - always GL_UNSIGNED_BYTE (except for OS X)
  GLenum          type;

  //////////
  // the format - either GL_RGBA, GL_BGRA_EXT, or GL_LUMINANCE
  // or GL_YCBCR_422_GEM (which is on mac-computers GL_YCBCR_422_APPLE)
  GLenum          format;
  
  //////////
  // is this owned by us (?)
  int notowned;
  //////////
  // the actual image data
  
  //////////
  // gets a pixel
  /* X,Y are the coordinates
   * C is the offset in the interleaved data (like chRed==0 for red)
   * you should use chRed instead of 0 (because it might not be 0)
   */
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
  void fromGray   (unsigned char* orgdata);
  void fromUYVY   (unsigned char* orgdata);
  void fromYUY2   (unsigned char* orgdata); // YUYV
  void fromYVYU   (unsigned char* orgdata);
  void fromYV12   (unsigned char* orgdata);

  void fromYUV422 (unsigned char* orgdata){fromUYVY(orgdata);}
  void fromYUV420P(unsigned char* orgdata){fromYV12(orgdata);}

  unsigned char   *data;
  private:
  unsigned char   *pdata;
  /* datasize should be private */
  int             datasize;
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

///////////////////////////////////////////////////////////////////////////////
// Color component defines
//
// These should be used to reference the various color channels
///////////////////////////////////////////////////////////////////////////////

/* RGBA */
#ifdef MACOSX				//tigital
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

#endif // GEMPIXUTIL_H_
