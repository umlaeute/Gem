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

#include "Cache.h"
#include "Controls/gemhead.h"

/////////////////////////////////////////////////////////
//
// GemCache
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GemCache :: GemCache(gemhead *parent)
  : dirty(true), resendImage(false), vertexDirty(false),
    m_parent(parent), m_magic(GEMCACHE_MAGIC)
{
}
GemCache :: GemCache(const GemCache&org)
  : dirty(org.dirty), resendImage(org.resendImage),
    vertexDirty(org.vertexDirty),
    m_parent(org.m_parent), m_magic(GEMCACHE_MAGIC)
{
}
void GemCache :: reset(gemhead *parent)
{
  dirty      =true;
  resendImage=false;
  vertexDirty=false;
  m_parent   =parent;
  m_magic    =GEMCACHE_MAGIC;
}
/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GemCache :: ~GemCache(void)
{
  m_magic=0xFFFFFFF;
  m_parent=NULL;
}

GemCache&GemCache::operator=(const GemCache&org)
{
  dirty=org.dirty;
  resendImage=org.resendImage;
  vertexDirty=org.vertexDirty;
  m_parent=org.m_parent;
  m_magic=GEMCACHE_MAGIC;
  return *this;
}
