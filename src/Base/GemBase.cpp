////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////


/*
 * m_state: context dependent initialization state
 *
 * states:
 *
 * INIT: a context has just been created; initialize the object for this context
 * DISABLED: the object is not useable in this context
 * ENABLED: the object can run in this context; however, no context-ressources have been allocated yet (needs startRendering())
 * RENDERING: startRendering() has been called and we can render this object just fine...
 * MODIFIED: the object has been modified, and need to free it's context-ressources (stopRendering()) and request new ones (startRendering())
 *
 * state-change triggers:
 * reset(): -> INIT
 * setModified(): -> MODIFIED
 *
 * state-changers:
 * isRunnable(); INIT -> ENABLED|DISABLED
 * startRendering(): ENABLED->RENDERING
 * stopRendering(): MODIFIED->ENABLED
 *
 * 0..INIT ( => isRunnable() -> (startRendering() -> RENDERING) (-> DISABLED))
 * 1..RENDERING ( stopRendering() -> STOPPED)
 * 2..STOPPED ( -> INIT) (startRendering() -> RENDERING)
 * 3..DISABLED ( -> INIT)
 *
 *---
 * INIT -> isRunnable() -> (DISABLED) (ENABLED)
 * reset(): DISABLED -> INIT
 * ENABLED -> startRendering() -> RENDERING
 * setModified(): ENABLED
 * STOPPING -> stopRendering() -> ENABLED

 * RENDERING: render()

 * isRunnable() has to be called once for each (new) context
 * startRendering() has to be called for each context when new IDs are to be generated
 * stopRendering() has to be called for each context to free IDs

 * we need a mechanism to reset a context (e.g. because a context is destroyed and it's ID might be reused)
 */

#define RENDERSTATE_DISABLED -1
#define RENDERSTATE_INIT      0
#define RENDERSTATE_ENABLED   1
#define RENDERSTATE_RENDERING 2
#define RENDERSTATE_MODIFIED  3


#include "GemBase.h"
#include "GemCache.h"

/////////////////////////////////////////////////////////
//
// GemBase
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GemBase :: GemBase()
  : gem_amRendering(false), m_cache(NULL), m_modified(true),
    m_enabled(true)
{
  m_out1 = outlet_new(this->x_obj, 0);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GemBase :: ~GemBase()
{
  if (gem_amRendering){
    stopRendering();
    gem_amRendering=false;
  }

    if (m_out1)
        outlet_free(m_out1);
}

/////////////////////////////////////////////////////////
// gem_cacheMess
//
/////////////////////////////////////////////////////////
void GemBase :: gem_startstopMess(int state)
{
  if (state && !gem_amRendering){
    m_enabled = isRunnable();
    if(m_enabled) {
      startRendering();
    }
  }
  else if (!state && gem_amRendering){
    if(m_enabled) {
      stopRendering();
    }
  }

  gem_amRendering=(state!=0);


  // continue sending out the cache message
  t_atom ap[1];
  SETFLOAT(ap, state);
  outlet_anything(this->m_out1, gensym("gem_state"), 1, ap);
}

/////////////////////////////////////////////////////////
// renderMess
//
/////////////////////////////////////////////////////////
void GemBase :: gem_renderMess(GemCache* cache, GemState*state)
{
  m_cache=cache;
  if(m_cache->m_magic!=GEMCACHE_MAGIC)
    m_cache=NULL;
#ifdef GEM_MULTICONTEXT
  if(RENDERSTATE_INIT==m_state) {
    if(isRunnable()) {
      m_state=RENDERSTATE_ENABLED;
    } else {
      m_state=RENDERSTATE_DISABLED;
    }
  }
  if(RENDERSTATE_MODIFIED==m_state) {
    stopRendering();
    m_state=RENDERSTATE_ENABLED;
  }
  if(RENDERSTATE_ENABLED==m_state) {
    startRendering();
    m_state=RENDERSTATE_RENDERING;
  }
  if(RENDERSTATE_RENDERING==m_state) {
    if(state)render(state);
    continueRender(state);
    if(state)postrender(state);
  }
#else
  if (!gem_amRendering){ // init Rendering if not done yet
    m_enabled=isRunnable();

    if(m_enabled)
      startRendering();

    gem_amRendering=true;
  }
  if(m_enabled) {
    if(state)render(state);
    continueRender(state);
    if(state)postrender(state);
  } else {
    continueRender(state);
  }

  m_modified=false;
#endif
}

void GemBase :: continueRender(GemState*state){
  t_atom ap[2];
  ap->a_type=A_POINTER;
  ap->a_w.w_gpointer=(t_gpointer *)m_cache;  // the cache ?
  (ap+1)->a_type=A_POINTER;
  (ap+1)->a_w.w_gpointer=(t_gpointer *)state;
  outlet_anything(this->m_out1, gensym("gem_state"), 2, ap);
}



/////////////////////////////////////////////////////////
// setModified
//
/////////////////////////////////////////////////////////
void GemBase :: setModified()
{
  if (m_cache&& (m_cache->m_magic!=GEMCACHE_MAGIC))
    m_cache=NULL;
  if (m_cache) m_cache->dirty = 1;
  m_modified=true;
}

/////////////////////////////////////////////////////////
// realStopRendering
//
/////////////////////////////////////////////////////////
void GemBase :: realStopRendering()
{
    stopRendering();
    m_cache = NULL;
}



/////////////////////////////////////////////////////////
// disabling the rendering of this object
//
/////////////////////////////////////////////////////////
bool GemBase :: isRunnable()
{
  return true;
}




/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void GemBase :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, reinterpret_cast<t_method>(&GemBase::gem_MessCallback),
    	    gensym("gem_state"), A_GIMME, A_NULL);
}
void GemBase :: gem_MessCallback(void *data, t_symbol *s, int argc, t_atom *argv)
{
  if (argc==1 && argv->a_type==A_FLOAT){
    GetMyClass(data)->gem_startstopMess(atom_getint(argv));  // start rendering (forget this !?)
  } else if (argc==2 && argv->a_type==A_POINTER && (argv+1)->a_type==A_POINTER){

    GetMyClass(data)->gem_renderMess((GemCache *)argv->a_w.w_gpointer, (GemState *)(argv+1)->a_w.w_gpointer);
  } else {
    GetMyClass(data)->error("wrong arguments...");
  }
}
