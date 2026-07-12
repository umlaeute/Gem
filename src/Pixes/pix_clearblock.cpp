////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
////////////////////////////////////////////////////////

#include "pix_clearblock.h"

#include "Gem/Cache.h"
#include "Gem/State.h"


CPPEXTERN_NEW(pix_clearblock);

/////////////////////////////////////////////////////////
//
// pix_clearblock
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_clearblock :: pix_clearblock()
  : m_oldcache(NULL)
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_clearblock :: ~pix_clearblock()
{
  if (m_oldcache) {
    stopRendering();
  }
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_clearblock :: render(GemState *state)
{
  // the cache and image should have been set
  if (!m_oldcache || m_oldcache->m_magic!=GEMCACHE_MAGIC) {
    // our cache has not been set or has been destroyed
    startRendering(); // i'm not sure, whether this breaks the default behaviour
    return;
  }
  state->set(GemState::_PIX, static_cast<pixBlock*>(NULL));
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_clearblock :: postrender(GemState *state)
{
#ifdef __GNUC__
# warning should we not reset to the original image?
#endif
  state->set(GemState::_PIX, static_cast<pixBlock*>(NULL));
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_clearblock :: startRendering()
{
  stopRendering();

  if (m_cache && m_cache->m_magic!=GEMCACHE_MAGIC) {
    m_cache=NULL;
  }

  m_oldcache = m_cache;

  if(m_oldcache) {
    m_cache = new GemCache(m_oldcache->m_parent);
  }
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void pix_clearblock :: stopRendering()
{
  if(m_oldcache) {
    if (m_cache) {
      delete m_cache;
    }
    m_cache=NULL;
  }
  m_oldcache=NULL;
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_clearblock :: obj_setupCallback(t_class *)
{ }
