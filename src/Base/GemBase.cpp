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

#include "GemBase.h"
#include "GemCache.h"
#include "GemDag.h"

/////////////////////////////////////////////////////////
//
// GemBase
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GemBase :: GemBase()
    	 : m_cache(NULL)
{
    m_out1 = outlet_new(this->x_obj, 0);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GemBase :: ~GemBase()
{
    if (m_cache)
        m_cache->breakDAG();
    if (m_out1)
        outlet_free(m_out1);
}

/////////////////////////////////////////////////////////
// gem_cacheMess
//
/////////////////////////////////////////////////////////
void GemBase :: gem_dagCacheMess(GemDag *dagPtr, GemCache *cachePtr)
{
    dagPtr->addChild(this, &GemBase::renderCallback, &GemBase::postrenderCallback, &GemBase::stoprenderCallback);
   
    m_cache = cachePtr;
    if (m_cache) startRendering();
    else return;

    // continue sending out the cache message
    t_atom cacheArray[2];
    cacheArray[0].a_type = A_POINTER;
    cacheArray[0].a_w.w_gpointer = (t_gpointer *)dagPtr;
    cacheArray[1].a_type = A_POINTER;
    cacheArray[1].a_w.w_gpointer = (t_gpointer *)m_cache;
    outlet_anything(this->m_out1, gensym("gem_state"), 2, cacheArray);

    // tell the dag that the child is done with its part of the chain
    dagPtr->childDone(this);
}

/////////////////////////////////////////////////////////
// setModified
//
/////////////////////////////////////////////////////////
void GemBase :: setModified()
{
    if (m_cache) m_cache->dirty = 1;
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
// static member function
//
/////////////////////////////////////////////////////////
void GemBase :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&GemBase::gem_dagCacheMessCallback,
    	    gensym("gem_state"), A_POINTER, A_POINTER, A_NULL);
}
void GemBase :: gem_dagCacheMessCallback(void *data, void *gem_dag, void *gem_cache)
{
    GetMyClass(data)->gem_dagCacheMess((GemDag *)gem_dag, (GemCache *)gem_cache);
}
void GemBase :: renderCallback(GemBase *data, GemState *state)
{
    data->render(state);
}
void GemBase :: postrenderCallback(GemBase *data, GemState *state)
{
    data->postrender(state);
}
void GemBase :: stoprenderCallback(GemBase *data)
{
    data->stoprender();
}
