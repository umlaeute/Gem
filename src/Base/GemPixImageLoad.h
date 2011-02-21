/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    GemPixImageLoad.h
       - code to load in and resize an image
       - part of GEM

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/
 
#ifndef INCLUDE_GEMPIXIMAGELOAD_H_
#define INCLUDE_GEMPIXIMAGELOAD_H_

#include "Base/GemExportDef.h"

struct imageStruct;

// image2mem() reads an image file into memory
//   and a pointer to an imageStruct
//       NULL = failure
// 
//       format:
//    	  returns either GL_LUMINANCE or GL_RGBA
// 
//   automatically allocates the memory for the user
//
// This can read TIFF, SGI, and JPG images
//
GEM_EXTERN extern imageStruct *image2mem(const char *filename);

#endif
