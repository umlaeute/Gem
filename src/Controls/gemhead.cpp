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
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "gemhead.h"

#ifdef __APPLE__
#include <Carbon/Carbon.h>
#endif

#include "Base/GemMan.h"
#include "Base/GemState.h"
#include "Base/GemCache.h"
#include "Base/GemBase.h"

#include "Base/GLStack.h"
#include "RTE/MessageCallbacks.h"

CPPEXTERN_NEW_WITH_ONE_ARG(gemhead, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// gemhead
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemhead :: gemhead(t_floatarg priority) : 
  gemreceive(gensym("__gem_render")),
  m_cache(new GemCache(this)), m_renderOn(1)
{
    if(m_fltin)inlet_free(m_fltin);
    m_fltin=NULL;

    setMess(priority);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemhead :: ~gemhead()
{
    if (m_cache)
        stopRendering();
    if(m_cache)delete m_cache;
    m_cache=NULL;
}

/////////////////////////////////////////////////////////
// renderGL
//
/////////////////////////////////////////////////////////
void gemhead :: renderGL(GemState *state)
{
  static const GLfloat a_color[]={0.2,0.2,0.2,1};
  static const GLfloat d_color[]={0.8,0.8,0.8,1};
  static const GLfloat e_color[]={0.0,0.0,0.0,1};
  static const GLfloat s_color[]={0.0,0.0,0.0,1};
  static const GLfloat shininess[]={0.0};

  if (!m_cache || !m_renderOn) return;

  // set the default color and transformation matrix
  glColor4f(1.f, 1.f, 1.f, 1.f);

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  a_color);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  d_color);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, e_color);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, s_color);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

  gem::GLStack*stacks=NULL;
  if(state)
  {
    state->reset();
    // set the state dirty flag
    state->set(GemState::_DIRTY, m_cache->dirty);
    state->VertexDirty=m_cache->vertexDirty;

    state->get(GemState::_GL_STACKS, stacks);
    if(stacks)stacks->push();
  }

  // are we profiling and need to send new images?
  if (GemMan::getProfileLevel() >= 2)
	m_cache->resendImage = 1;

  t_atom ap[2];
  ap->a_type=A_POINTER;
  ap->a_w.w_gpointer=reinterpret_cast<t_gpointer*>(m_cache);  // the cache ?
  (ap+1)->a_type=A_POINTER;
  (ap+1)->a_w.w_gpointer=reinterpret_cast<t_gpointer*>(state);
  outlet_anything(this->m_outlet, gensym("gem_state"), 2, ap);

  m_cache->dirty = false;
  m_cache->vertexDirty=false;
  state->get(GemState::_GL_STACKS, stacks);
  if(stacks)stacks->pop();
}


/////////////////////////////////////////////////////////
// bangMess
//
/////////////////////////////////////////////////////////
void gemhead :: bangMess()
{
    int renderon = m_renderOn;
    // make sure that the window and the cache exist
    if ( !GemMan::windowExists() || !m_cache )
		return;
    
    // make a dummy GemState
    GemState tempState;
    GemMan::fillGemState(tempState);

    m_renderOn = 1;
    renderGL(&tempState);
    m_renderOn = renderon;
    glFlush();
}

/////////////////////////////////////////////////////////
// renderOnOff
//
/////////////////////////////////////////////////////////
void gemhead :: renderOnOff(int state)
{
    m_renderOn = state;
}

/////////////////////////////////////////////////////////
// setPriority
//
/////////////////////////////////////////////////////////
void gemhead :: setMess(t_float priority)
{
  if (priority == 0.)priority=50.;
  if(priority==m_priority)
    return;

  m_priority=priority;

  std::string rcv="__gem_render";
  if(priority<0.f)
    rcv="__gem_render_osd";

  gemreceive::priorityMess(priority);
  gemreceive::nameMess(gensym(rcv.c_str()));
}

void gemhead :: receive(t_symbol*s, int argc, t_atom*argv) {
  if(gensym("gem_state")==s) {
    if(1==argc && A_FLOAT==argv->a_type) {
      int i=atom_getint(argv);
      switch(i) {
      case 0:
        stopRendering();
        break;
      default:
        startRendering();
      }
    } else if (2==argc && A_POINTER==argv[0].a_type && A_POINTER==argv[1].a_type) {
      GemCache*cache=reinterpret_cast<GemCache*>(argv[0].a_w.w_gpointer);
      GemState*state=reinterpret_cast<GemState*>(argv[1].a_w.w_gpointer);
      if(!m_renderOn)
        startRendering();
      renderGL(state);
    }
  } else {
    // not for us...
  }
}


/////////////////////////////////////////////////////////
// outputRenderOnOff
//
/////////////////////////////////////////////////////////
void gemhead :: outputRenderOnOff(int state)
{
  // continue sending out the cache message
  t_atom ap[1];
  SETFLOAT(ap, state);
  outlet_anything(this->m_outlet, gensym("gem_state"), 1, ap);
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void gemhead :: startRendering()
{
  if (m_cache) m_cache->reset(this);
  else         m_cache = new GemCache(this);

  outputRenderOnOff(1);
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void gemhead :: stopRendering()
{
  outputRenderOnOff(0);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemhead :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG0(classPtr, "bang", bangMess);
  CPPEXTERN_MSG1(classPtr, "float", renderOnOff, int);
  CPPEXTERN_MSG1(classPtr, "set", setMess, int);
}
