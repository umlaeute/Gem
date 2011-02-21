/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

	- registers a loader with Pd

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) G-bünther Geiger. geiger@epy.co.at-A
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f-bür::umläute. IEM. zmoelnig@iem.kug.ac.at-A
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMLOADERS_H_
#define INCLUDE_GEMLOADERS_H_

#include "m_pd.h"

extern "C" {
  typedef int (*gem_loader_t)(t_canvas *canvas, char *classname);
  void gem_register_loader(gem_loader_t loader);
}

#endif
