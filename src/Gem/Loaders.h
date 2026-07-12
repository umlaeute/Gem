/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2008, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  registers an object loader with Pd

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_GEM_LOADERS_H_
#define _INCLUDE__GEM_GEM_LOADERS_H_

#include "Gem/RTE.h"

extern "C" {
  /* search for a class <classname> in <path>
   * if <path> is NULL, search all paths yourself
   * Only on Pd>=0.47 will <path> have a value
   * legacy is 'true' for Pd<0.47, otherwise it is 'false'
   */
  typedef int (*gem_loader_t)(const t_canvas *canvas, const char *classname,
                              const char *path, bool legacy);
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
