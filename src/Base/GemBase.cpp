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

/////////////////////////////////////////////////////////
//
// GemBase
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GemBase :: GemBase()
  : gem_amRendering(false), m_cache(NULL), m_modified(true)
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
  if (state && !gem_amRendering)startRendering();
  else if (!state && gem_amRendering)stopRendering();

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

  if (!gem_amRendering){ // init Rendering if not done yet
    startRendering();
    gem_amRendering=true;
  }
  if(state)render(state);
  continueRender(state);
  if(state)postrender(state);
  m_modified=false;
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
// static member function
//
/////////////////////////////////////////////////////////
void GemBase :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&GemBase::gem_MessCallback,
    	    gensym("gem_state"), A_GIMME, A_NULL);
}
void GemBase :: gem_MessCallback(void *data, t_symbol *s, int argc, t_atom *argv)
{
  if (argc==1 && argv->a_type==A_FLOAT){
    GetMyClass(data)->gem_startstopMess(atom_getint(argv));  // start rendering (forget this !?)
  } else if (argc==2 && argv->a_type==A_POINTER && (argv+1)->a_type==A_POINTER){

    GetMyClass(data)->gem_renderMess((GemCache *)argv->a_w.w_gpointer, (GemState *)(argv+1)->a_w.w_gpointer);
  } else {
    error("GEM: wrong arguments....");
  }
}
