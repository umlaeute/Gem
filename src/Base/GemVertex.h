
/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    include file for VertexArrays

    Copyright (c) 2004 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/


#ifndef INCLUDE_GEMVERTEX_H_
#define INCLUDE_GEMVERTEX_H_

#include "Base/GemShape.h"

#ifdef __APPLE__
# define glGenVertexArraysGEM glGenVertexArraysAPPLE
# define glIsVertexArrayGEM   glGenVertexArraysAPPLE
# define glBindVertexArrayGEM glGenVertexArraysAPPLE
#else
# define glGenVertexArraysGEM
# define glIsVertexArrayGEM  
# define glBindVertexArrayGEM
#endif

#endif /* INCLUDE_GEMVERTEX_H_ */
