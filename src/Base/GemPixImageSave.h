/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Gempiximagesave.h
       - code to save an image
       - part of GEM

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/
 
#ifndef INCLUDE_GEMPIXIMAGESAVE_H_
#define INCLUDE_GEMPIXIMAGESAVE_H_

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
// This can write TIFF, JPG and other images (depending on which backends are available
// legacy: type=0 -> TIFF; type>0 -> JPEG and (quality:=type)
//
GEM_EXTERN extern int mem2image(imageStruct *image, const char *filename, const int type);


#endif
