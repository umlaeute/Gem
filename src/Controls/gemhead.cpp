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

#include "Base/GemMan.h"
#include "Base/GemState.h"
#include "Base/GemCache.h"
#include "Base/GemDag.h"
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
    	 : m_cache(NULL), m_dag(NULL), m_renderOn(1)
{
    // register with Gem
    if (priority == 0)
		priority = 50;
    GemMan::addObj(this, (int)priority);

    m_out1 = outlet_new(this->x_obj, 0);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemhead :: ~gemhead()
{
    GemMan::removeObj(this);
    if (m_cache)
        stopRendering();
    outlet_free(m_out1);
}

/////////////////////////////////////////////////////////
// renderGL
//
/////////////////////////////////////////////////////////
void gemhead :: renderGL(GemState *state)
{
    // the cache should have been set
    if (!m_cache || !m_dag || !m_renderOn) return;
    
	// set the default color and transformation matrix
	glColor4f(1.f, 1.f, 1.f, 1.f);
	glPushMatrix();

    // set the state dirty flag
    state->dirty = m_cache->dirty;

	// are we profiling and need to send new images?
	if (GemMan::m_profile >= 2)
		m_cache->resendImage = 1;

    // do the rendering
    for (int i = 0; i < m_dag->m_numChildren; i++)
    	renderBase(m_dag->m_list[i], state);

    m_cache->dirty = 0;

	glPopMatrix();
}

/////////////////////////////////////////////////////////
// renderBase
//
/////////////////////////////////////////////////////////
void gemhead :: renderBase(gemBaseLink *link, GemState *state)
{
    link->renderFunc(link->data, state);

    for (int i = 0; i < link->numChildren; i++)
    	    renderBase(link->children[i], state);

    link->postrenderFunc(link->data, state);
}

/////////////////////////////////////////////////////////
// bangMess
//
/////////////////////////////////////////////////////////
void gemhead :: bangMess()
{
    // make sure that the window and the cache exist
    if ( !GemMan::windowExists() || !m_cache )
		return;
    
    // make a dummy GemState
    GemState tempState;
	GemMan::fillGemState(tempState);

    renderGL(&tempState);
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
// startRendering
//
/////////////////////////////////////////////////////////
void gemhead :: startRendering()
{
    if (m_cache)
		delete m_cache;
    m_cache = new GemCache(this);
    if (m_dag)
		delete m_dag;
    m_dag = new GemDag;
    
    t_atom cacheArray[2];
    cacheArray[0].a_type = A_POINTER;
    cacheArray[0].a_w.w_gpointer = (t_gpointer *)m_dag;
    cacheArray[1].a_type = A_POINTER;
    cacheArray[1].a_w.w_gpointer = (t_gpointer *)m_cache;
    // send out the dag and cache information
    outlet_anything(this->m_out1, gensym("gem_state"), 2, cacheArray);    
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void gemhead :: stopRendering()
{
    if (!m_dag) return;

    for (int i = 0; i < m_dag->m_numChildren; i++)
    	stopBaseRendering(m_dag->m_list[i]);

    if (m_cache) delete m_cache;
    delete m_dag;
    m_dag = NULL;
    m_cache = NULL;
}

/////////////////////////////////////////////////////////
// stopBaseRendering
//
/////////////////////////////////////////////////////////
void gemhead :: stopBaseRendering(gemBaseLink *link)
{
    link->data->realStopRendering();
    for (int i = 0; i < link->numChildren; i++)
    	stopBaseRendering(link->children[i]);
}

/////////////////////////////////////////////////////////
// breakDAG
//
/////////////////////////////////////////////////////////
void gemhead :: breakDAG()
{
    stopRendering();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemhead :: obj_setupCallback(t_class *classPtr)
{
    class_addbang(classPtr, (t_method)&gemhead::bangMessCallback);    
    class_addfloat(classPtr, (t_method)&gemhead::intMessCallback);    
}
void gemhead :: bangMessCallback(void *data)
{
    GetMyClass(data)->bangMess();
}
void gemhead :: intMessCallback(void *data, t_floatarg n)
{
    GetMyClass(data)->renderOnOff((int)n);
}

