/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    GemPixUtil.h
       - contains image functions for pix objects
       - part of GEM

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMPIXUTIL_H_
#define INCLUDE_GEMPIXUTIL_H_

// I hate Microsoft...I shouldn't have to do this!
#ifdef _WINDOWS
#include <windows.h>
#endif

#include <GL/gl.h>

// packed pixel defines for textures
#ifndef GL_EXT_packed_pixels
#define GL_UNSIGNED_BYTE_3_3_2_EXT          0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4_EXT       0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1_EXT       0x8034
#define GL_UNSIGNED_INT_8_8_8_8_EXT         0x8035
#define GL_UNSIGNED_INT_10_10_10_2_EXT      0x8036
#endif

#include "Base/GemFuncUtil.h"
#include "config.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    imageStruct
    
    The basic image structure

-----------------------------------------------------------------*/
struct GEM_EXTERN imageStruct
{

  imageStruct() : xsize (0), ysize(0),csize(0),data(0),pdata(0),notowned(0) {}

  ~imageStruct() { clear(); }
    //////////
    // columns
  unsigned char* allocate(int size) {
    if (pdata) delete [] pdata;
#if 1
    pdata = new unsigned char[size];
    data = (unsigned char*) ((((unsigned int)pdata)+31)& (~31));
#else
    data = pdata =  new unsigned char [size];
#endif
    return data; 
  }

  void clear() {
    if (!notowned && pdata) {
      delete [] pdata;
      data = pdata = 0;      
    }
    xsize = ysize = csize = 0;
  }

    GLint           xsize;

    //////////
    // rows
    GLint   	    ysize;

    //////////
    // color (LUMINANCE = 1, RGBA = 4)
    GLint   	    csize;

    //////////
    // the format - either GL_RGBA or GL_LUMINANCE
    GLenum          format;

    //////////
     // data type - always UNSIGNED_BYTE
    GLenum          type;


  int notowned;
    //////////
    // the actual image data

  //////////
  // gets a pixel
  inline unsigned char GetPixel(int Y, int X, int C)
  { return(data[Y * xsize * csize + X * csize + C]); }
  
  //////////
  // sets a pixel
  inline void SetPixel(int Y, int X, int C, unsigned char VAL)
  { data[Y * xsize * csize + X * csize + C] = VAL; }

  void copy2Image(imageStruct *to);
  void refreshImage(imageStruct *to);

    unsigned char   *data;
  private:
    unsigned char   *pdata;
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


///////////////////////////////////////////////////////////////////////////////
// Color component defines
//
// These should be used to reference the various color channels
///////////////////////////////////////////////////////////////////////////////
const int chRed		= 0;
const int chGreen	= 1;
const int chBlue	= 2;
const int chAlpha	= 3;

const int chGray	= 0;

///////////////////////////////////////////////////////////////////////////////
// Pixel access functions
//
///////////////////////////////////////////////////////////////////////////////


//
// Accelerated Pixel Manipulations
//

int detect_mmx( void );


#if HAVE_MMX


#define MMXDONE asm volatile ("emms  \n\t")

#define MMXSTART   \
asm volatile ( \
        "push %ebx \n\t"                    \
        "mov       %esp, %ebx \n\t"    \
        "sub          $4, %ebx \n\t"  \
        "and        $-32, %ebx \n\t"  \
        "mov     %esp, (%ebx) \n\t"  \
        "mov       %ebx, %esp \n\t"  \
        "pop %ebx")

#define ADD8(a,b) \
asm volatile (\
   "movq    (%1), %%mm1 \n\t"       \
   "paddusb (%0), %%mm1 \n\t"       \
   "movq    %%mm1, (%0) \n\t"       \
   : : "r" (a), "r" (b))

#define SUB8(a,b) \
asm volatile (\
       "movq    (%0), %%mm1 \n\t"   \
       "psubusb (%1), %%mm1 \n\t"   \
       "movq    %%mm1, (%0) \n\t"   \
       : : "r" (a), "r" (b))

#define ABSDIFF8(a,b) \
asm volatile (\
       "movq    (%0), %%mm1 \n\t"   \
       "movq    (%1), %%mm2 \n\t"   \
       "psubusb (%1), %%mm1 \n\t"   \
       "psubusb (%0), %%mm2 \n\t"   \
       "por    %%mm2, %%mm1 \n\t"   \
       "movq    %%mm1, (%0) \n\t"   \
	      : : "r" (a), "r" (b))


#define MULT4_b_s(a,b) \
asm volatile (\
       "pxor      %%mm0, %%mm0 \n\t" \
       "movq    (%0), %%mm1 \n\t"    \
       "movq    (%1), %%mm2 \n\t"    \
       "punpcklbw %%mm0, %%mm1 \n\t" \
       "pmullw    %%mm2, %%mm1 \n\t" \
       "packuswb  %%mm0, %%mm1 \n\t" \
       "movq    %%mm1, (%0) \n\t"   \
       : : "r" (a), "r" (b))



#else // non MMX versions

#define MMXSTART
#define MMXDONE

#define ADD8(a,b) \
 ((unsigned char*)(a))[0] = CLAMP_HIGH((int)((unsigned char*)(a))[0] + ((unsigned char*)(b))[0]);\
 ((unsigned char*)(a))[1] = CLAMP_HIGH((int)((unsigned char*)(a))[1] + ((unsigned char*)(b))[1]);\
 ((unsigned char*)(a))[2] = CLAMP_HIGH((int)((unsigned char*)(a))[2] + ((unsigned char*)(b))[2]);\
 ((unsigned char*)(a))[3] = CLAMP_HIGH((int)((unsigned char*)(a))[3] + ((unsigned char*)(b))[3]);\
 ((unsigned char*)(a))[4] = CLAMP_HIGH((int)((unsigned char*)(a))[4] + ((unsigned char*)(b))[4]);\
 ((unsigned char*)(a))[5] = CLAMP_HIGH((int)((unsigned char*)(a))[5] + ((unsigned char*)(b))[5]);\
 ((unsigned char*)(a))[6] = CLAMP_HIGH((int)((unsigned char*)(a))[6] + ((unsigned char*)(b))[6]);\
 ((unsigned char*)(a))[7] = CLAMP_HIGH((int)((unsigned char*)(a))[7] + ((unsigned char*)(b))[7]);


#define SUB8(a,b) \
 ((unsigned char*)(a))[0] = CLAMP_LOW((int)((unsigned char*)(a))[0] - ((unsigned char*)(b))[0]);\
 ((unsigned char*)(a))[1] = CLAMP_LOW((int)((unsigned char*)(a))[1] - ((unsigned char*)(b))[1]);\
 ((unsigned char*)(a))[2] = CLAMP_LOW((int)((unsigned char*)(a))[2] - ((unsigned char*)(b))[2]);\
 ((unsigned char*)(a))[3] = CLAMP_LOW((int)((unsigned char*)(a))[3] - ((unsigned char*)(b))[3]);\
 ((unsigned char*)(a))[4] = CLAMP_LOW((int)((unsigned char*)(a))[4] - ((unsigned char*)(b))[4]);\
 ((unsigned char*)(a))[5] = CLAMP_LOW((int)((unsigned char*)(a))[5] - ((unsigned char*)(b))[5]);\
 ((unsigned char*)(a))[6] = CLAMP_LOW((int)((unsigned char*)(a))[6] - ((unsigned char*)(b))[6]);\
 ((unsigned char*)(a))[7] = CLAMP_LOW((int)((unsigned char*)(a))[7] - ((unsigned char*)(b))[7]);

#endif // MMX





#endif
