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

#ifdef MACOSX
#import <Carbon/Carbon.h>
#endif

#include "Base/GemMan.h"
#include "Base/GemState.h"
#include "Base/GemCache.h"
#include "Base/GemBase.h"

CPPEXTERN_NEW_WITH_ONE_ARG(gemhead, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// gemhead
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemhead :: gemhead(t_floatarg priority)
    	 : m_cache(NULL), m_renderOn(1)
{
    // register with Gem
    if (priority == 0)
		priority = 50;
    m_priority=(int)priority;
    GemMan::addObj(this, m_priority);

    m_cache = new GemCache(this);
    m_out1 = outlet_new(this->x_obj, 0);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemhead :: ~gemhead()
{
    GemMan::removeObj(this, m_priority);
    if (m_cache)
        stopRendering();
    if(m_cache)delete m_cache;
    outlet_free(m_out1);
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

    glPushMatrix();
    if(state)
      {
	// set the state dirty flag
	state->dirty = m_cache->dirty;

	// clear the state->image (might be still there from previous [gemhead]s)
	state->image = 0;
      }

    // are we profiling and need to send new images?
    if (GemMan::m_profile >= 2)
      m_cache->resendImage = 1;

    t_atom ap[2];
    ap->a_type=A_POINTER;
    ap->a_w.w_gpointer=(t_gpointer *)m_cache;  // the cache ?
    (ap+1)->a_type=A_POINTER;
    (ap+1)->a_w.w_gpointer=(t_gpointer *)state;
   outlet_anything(this->m_out1, gensym("gem_state"), 2, ap);

    m_cache->dirty = 0;

    glPopMatrix();
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
void gemhead :: setMess(int priority)
{
  if (priority == 0)priority=50;
  GemMan::removeObj(this, m_priority);
  GemMan::addObj(this, (int)priority);
  m_priority=priority;
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
  outlet_anything(this->m_out1, gensym("gem_state"), 1, ap);
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
    class_addbang(classPtr, (t_method)&gemhead::bangMessCallback);
    class_addfloat(classPtr, (t_method)&gemhead::intMessCallback);
    class_addmethod(classPtr, (t_method)&gemhead::setMessCallback,
		    gensym("set"), A_FLOAT, A_NULL);
}
void gemhead :: bangMessCallback(void *data)
{
    GetMyClass(data)->bangMess();
}
void gemhead :: intMessCallback(void *data, t_floatarg n)
{
    GetMyClass(data)->renderOnOff((int)n);
}
void gemhead :: setMessCallback(void *data, t_floatarg n)
{
    GetMyClass(data)->setMess((int)n);
}
