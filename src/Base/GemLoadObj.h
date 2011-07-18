/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

	- loads in an alias|wavefront model file

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMLOADOBJ_H_
#define INCLUDE_GEMLOADOBJ_H_

#include "Gem/ExportDef.h"

class GemModelData;

//////////
// Load in an alias|wavefront model
// [out] GemModel * - equals NULL if failure
GEM_EXTERN extern GemModelData *loadWaveFrontModel(const char *filename);


#endif
