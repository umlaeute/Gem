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

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    imageStruct
    
    The basic image structure

-----------------------------------------------------------------*/
struct GEM_EXTERN imageStruct
{
    //////////
    // columns
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

    //////////
    // the actual image data
    unsigned char   *data;
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
//////////
// gets a pixel
inline unsigned char GetPixel(imageStruct *IMG, int Y, int X, int C)
    { return(IMG->data[Y * IMG->xsize * IMG->csize + X * IMG->csize + C]); }

//////////
// sets a pixel
inline void SetPixel(imageStruct *IMG, int Y, int X, int C, unsigned char VAL)
    { IMG->data[Y * IMG->xsize * IMG->csize + X * IMG->csize + C] = VAL; }


#endif
