////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

/* -------------------------- setup function ------------------------------ */
#if !defined(MACOSX) && !defined(NT)
#include "../Base/config.h"
#endif

#include "GemMan.h"
#include "../Controls/setup.h"
#include "../Geos/setup.h"
#include "../Nongeos/setup.h"
#include "../Manips/setup.h"
#include "../Particles/setup.h"
#include "../Pixes/setup.h"
//#include "../TV/setup.h"
#include "../openGL/setup.h"
#include "../yuv/setup.h"

// notice that this is not inside of the extern "C"
void MarkEx_setup();

extern "C" {

  GEM_EXTERN void Gem_setup()
  {
    // startup GEM
    GemMan::initGem();

    MarkEx_setup();

    Controls_setup();
    Geos_setup();
    Nongeos_setup();
    Manips_setup();
    Particles_setup();
    Pixes_setup();
    //    TV_setup();
    openGL_setup();
    yuv_setup();
  }

  GEM_EXTERN void gem_setup()
  {
    Gem_setup();
  }

  GEM_EXTERN void GEM_setup()
  {
    Gem_setup();
  }

}   // for extern "C"
