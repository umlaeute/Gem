/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    GemGLUtil.h
       - contains functions for graphics
       - part of GEM

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMGLUTIL_H_
#define INCLUDE_GEMGLUTIL_H_

//<<<<<<< GemGLUtil.h
#include "Base/GemBase.h"
//=======
#include "Base/GemExportDef.h"
#ifndef MACOSX
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif
//>>>>>>> 1.2

GEM_EXTERN extern int           getGLdefine(char *name);
GEM_EXTERN extern int           getGLdefine(t_symbol *name);
GEM_EXTERN extern int           getGLdefine(t_atom *name);
GEM_EXTERN extern int           getGLbitfield(int argc, t_atom *argv);
#endif  // for header file

