/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

        - registers a loader with Pd

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_GEM_LOADERS_H_
#define _INCLUDE__GEM_GEM_LOADERS_H_

#include "Gem/RTE.h"

extern "C" {
  /* search for a class <classname> in <path>
   * if <path> is NULL, search all paths yourself
   * Only on Pd>=0.47 will <path> have a value */
  typedef int (*gem_loader_t)(const t_canvas *canvas, const char *classname,
                              const char *path);
  /* register a loader that respects the path (any file-based loader)
   * In Pd<0.47 this loader will be called with a path==NULL, and the loader needs to iterate
   * over the paths on its own (e.g. using canvas_open())
   * In Pd>=0.47 this loader will only be called with path!=NULL
   */
  void gem_register_loader(gem_loader_t loader);
  /* registers a loader that ignores the path (path will always be set to NULL)
   * for Pd>=0.47 this loader will be called after all the path-accepting loaders
   */
  void gem_register_loader_nopath(gem_loader_t loader);
}

#endif
