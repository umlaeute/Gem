/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEM_CACHE_H_
#define _INCLUDE__GEM_GEM_CACHE_H_

#include "Gem/ExportDef.h"

class gemhead;

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GemCache

    The cache to pass among GEM objects

DESCRIPTION

-----------------------------------------------------------------*/
#define GEMCACHE_MAGIC 0x1234567
class GEM_EXTERN GemCache
{
public:

  //////////
  // Constructor
  explicit GemCache(gemhead *parent);

  explicit GemCache(const GemCache&);

  //////////
  // Destructor
  virtual ~GemCache(void);


  virtual GemCache& operator=(const GemCache&);

  //////////
  // Was a modification made which will void a display list?
  bool                dirty;

  //////////
  // Should the image be resent?
  bool                resendImage;

  //////////
  // has the Vertex-Array changed?
  bool                vertexDirty;

  //////////
  // re-set (like creation, but without instantiating
  void reset(gemhead*parent);

  //////////
  gemhead                 *m_parent;

  //////////
  // indicates a valid cache
  int m_magic;
};

#endif  // for header file
