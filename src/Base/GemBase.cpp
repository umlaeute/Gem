////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
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
 * DISABLED: the object is not usable in this context
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

#include "GemBase.h"
#include "Gem/Cache.h"

struct GemBase::PIMPL {

  /* whether the object is internally disabled or not
   * objects are to be disabled, if the system cannot make use of them, e.g. because of unsupported openGL features
   */
  GemBase *parent;
  gem::ContextData<bool>enabled;
  gem::ContextData<enum RenderState>state;
  PIMPL(GemBase *_parent)
    : parent(_parent)
    , enabled(true)
    , state(INIT)
  { }
  PIMPL(PIMPL *p)
    : parent(p->parent)
    , enabled(p->enabled)
    , state(p->state)
  { }
};

/////////////////////////////////////////////////////////
//
// GemBase
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GemBase :: GemBase(void)
  : gem_amRendering(false), m_cache(NULL), m_modified(true)
  , m_out1(NULL)
  , m_pimpl(new PIMPL(this))
{
  m_out1 = outlet_new(this->x_obj, 0);
  pd_bind(&this->x_obj->ob_pd, gensym("__gemBase"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GemBase :: ~GemBase(void)
{
  if (gem_amRendering) {
    stopRendering();
    gem_amRendering=false;
  }

  if (m_out1) {
    outlet_free(m_out1);
  }
  pd_unbind(&this->x_obj->ob_pd, gensym("__gemBase"));
}

/////////////////////////////////////////////////////////
// gem_cacheMess
//
/////////////////////////////////////////////////////////
void GemBase :: gem_startstopMess(int state)
{
  // for now, this is important, as it is the only way to call the stopRendering
#if 1
  if (state && !gem_amRendering) {
    m_pimpl->enabled = isRunnable();
    if(m_pimpl->enabled) {
      startRendering();
      m_pimpl->state=RENDERING;
    }
  } else if (!state && gem_amRendering) {
    if(m_pimpl->enabled) {
      stopRendering();
      m_pimpl->state=ENABLED;
    }
  }

  gem_amRendering=(state!=0);


  // continue sending out the cache message
  t_atom ap[1];
  SETFLOAT(ap, state);
  outlet_anything(this->m_out1, gensym("gem_state"), 1, ap);
#else
  post("gem_startstopMess(%d) called...please report this to the upstream developers",
       state);
#endif
}

/////////////////////////////////////////////////////////
// renderMess
//
/////////////////////////////////////////////////////////
void GemBase :: gem_renderMess(GemCache* cache, GemState*state)
{
  m_cache=cache;
  if(m_cache && m_cache->m_magic!=GEMCACHE_MAGIC) {
    m_cache=NULL;
  }
  if(INIT==m_pimpl->state) {
    if(isRunnable()) {
      m_pimpl->state=ENABLED;
    } else {
      m_pimpl->state=DISABLED;
    }
  }
  if(MODIFIED==m_pimpl->state) {
    stopRendering();
    m_pimpl->state=ENABLED;
  }
  if(ENABLED==m_pimpl->state) {
    startRendering();
    m_pimpl->state=RENDERING;
  }
  if(RENDERING==m_pimpl->state) {
    gem_amRendering=true;
    if(state) {
      render(state);
    }
    continueRender(state);
    if(state) {
      postrender(state);
    }
  }
  m_modified=false;
}

void GemBase :: continueRender(GemState*state)
{
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
void GemBase :: setModified(void)
{
  if (m_cache&& (m_cache->m_magic!=GEMCACHE_MAGIC)) {
    m_cache=NULL;
  }
  if (m_cache) {
    m_cache->dirty = true;
  }
  m_modified=true;
  switch(m_pimpl->state) {
  case DISABLED:
  case INIT:
    break;
  default:
    m_pimpl->state=MODIFIED;
  }
}

/////////////////////////////////////////////////////////
// realStopRendering
//
/////////////////////////////////////////////////////////
void GemBase :: realStopRendering(void)
{
  /* no idea what this function is for; ask the user when it appears */
  post("realStopRendering() called...please report this to the upstream developers");
  stopRendering();
  m_cache = NULL;
  m_pimpl->state=ENABLED;
}



/////////////////////////////////////////////////////////
// disabling the rendering of this object
//
/////////////////////////////////////////////////////////
bool GemBase :: isRunnable(void)
{
  return true;
}

enum GemBase::RenderState GemBase::getState(void)
{
  return m_pimpl->state;
}

#include "Base/GemWindow.h"
void GemBase::beforeDeletion(void)
{
  //post("GemBase to be deleted");
  GemWindow::stopInAllContexts(this);
  CPPExtern::beforeDeletion();
}




/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void GemBase :: obj_setupCallback(t_class *classPtr)
{
  struct _CallbackClass_gemState {
    static void callback(void *data, t_symbol* s, int argc, t_atom *argv)
    {
      if (argc==2 && argv->a_type==A_POINTER && (argv+1)->a_type==A_POINTER) {
        GetMyClass(data)->gem_renderMess(
            reinterpret_cast<GemCache *>(argv->a_w.w_gpointer),
            reinterpret_cast<GemState *>((argv+1)->a_w.w_gpointer));
#if 1
      } else if (argc==1 && argv->a_type==A_FLOAT) {
        GetMyClass(data)->gem_startstopMess(atom_getint(argv));  // start rendering (forget this !?)
#endif
      } else {
        GetMyClass(data)->error("wrong arguments in GemTrigger...");
      }
    }
    explicit _CallbackClass_gemState (struct _class*c)
    {
      class_addmethod(c, reinterpret_cast<t_method>(callback),
                      gensym("gem_state"), A_GIMME, A_NULL);
    }
  };
  _CallbackClass_gemState _CallbackClassInstance_gemState (classPtr);

  struct _CallbackClass_gemContext {
    static void callback(void*data, t_float v0)
    {
      GemBase*obj=GetMyClass(data);
      bool state=(bool)v0;
      if(!state && obj->gem_amRendering) {
        if(obj->m_pimpl->enabled) {
          //obj->post("stop rendering");
          obj->stopRendering();
          obj->m_pimpl->state=obj->ENABLED;
        }
      }
      obj->gem_amRendering=(!state);
    }
    explicit _CallbackClass_gemContext (struct _class*c)
    {
      class_addmethod(c, reinterpret_cast<t_method>(callback),
                      gensym("__gem_context"), A_FLOAT, A_NULL);
    }
  };
  _CallbackClass_gemContext _CallbackClassInstance_gemContext (classPtr);

}
