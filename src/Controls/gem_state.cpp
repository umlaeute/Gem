////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 2009 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "gem_state.h"
#include "Base/GemState.h"

#include "Base/GemCache.h"
#include "Base/GemMan.h"

CPPEXTERN_NEW(gem_state)

/////////////////////////////////////////////////////////
//
// gem_state
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gem_state :: gem_state() :
  m_newcache(NULL), m_oldcache(NULL),
  m_profiling(0)
{
  m_newcache = new GemCache(NULL);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gem_state :: ~gem_state()
{
  if(m_newcache)
    delete m_newcache;
  m_newcache=NULL;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void gem_state :: render(GemState *state)
{
  m_oldcache=m_cache;
  if(state)
  {
    state->reset();

    // set the state dirty flag
    state->dirty = m_newcache->dirty;
    state->VertexDirty=m_newcache->vertexDirty;
  }

  // are we profiling and need to send new images?
  if (m_profiling >= 2)
    m_newcache->resendImage = 1;

  m_cache=m_newcache;
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void gem_state :: postrender(GemState *state)
{
  m_newcache->dirty = 0;
  m_newcache->vertexDirty=0;
  m_cache=m_oldcache;
}

void gem_state :: profileMess(int state)
{
  if(state<0)state=0;
  m_profiling=state;
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gem_state :: obj_setupCallback(t_class *classPtr)
{
   class_addmethod(classPtr, (t_method)&gem_state::profileMessCallback, gensym("profile"), A_FLOAT, A_NULL);
}
void gem_state :: profileMessCallback(void *data, t_floatarg n)
{
    GetMyClass(data)->profileMess((int)n);
}
