////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// ch@chdh.net
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

#include "gemlist.h"

#include "Base/GemCache.h"
#include "Base/GemMan.h"

CPPEXTERN_NEW(gemlist)

/////////////////////////////////////////////////////////
//
// gemlist
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemlist :: gemlist(void) 
  : m_valide_state(false),
    m_inlet(NULL),
    m_tickTime(-1.f),
    m_lightState(false),
    m_drawType(0)
{
	// create the cold inlet
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("gem_state"), gensym("gem_right"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemlist :: ~gemlist()
{
  inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void gemlist :: render(GemState *state)
{
  m_current_state=*state; //copy current state for later use


	m_valide_state=true;
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void gemlist :: postrender(GemState *)
{
  //	m_valide_state=false;

  // this is to early to reset the m_valide_state
  // when should we call this???
  // TODO : fix this.
}

void gemlist :: sendCacheState(GemCache *cache, GemState*state)
{
  if  ( !GemMan::windowExists() ) {
    // LATER: shouldn't this test for a valid context rather than an existing window??
    //	error("you should not bang the gemlist now"); 
    return;
  }

  if(state) {
		t_atom ap[2];

    GemCache tempCache(NULL);
    if(!cache)
      cache=&tempCache;

		ap->a_type=A_POINTER;
		ap->a_w.w_gpointer=reinterpret_cast<t_gpointer*>(cache);
		(ap+1)->a_type=A_POINTER;
		(ap+1)->a_w.w_gpointer=reinterpret_cast<t_gpointer*>(state);
		outlet_anything(m_out1, gensym("gem_state"), 2, ap);
  }
}


/////////////////////////////////////////////////////////
// bang
//
/////////////////////////////////////////////////////////
void gemlist :: trigger()
{
	if(m_valide_state) {
    // outlet the current state when banged
    sendCacheState(m_cache, &m_current_state);
  } else {
	  // fill in out own state and output
    GemState state;

    if(m_lightState) {
      state.set("gl.lighting", true);
      state.set("gl.smooth", true);
    }
    
    if(m_drawType) {
      state.set("gl.drawtype", m_drawType);
    }

    if(m_tickTime>=0.f) {
      state.set("timing.tick", m_tickTime);
    }

    //    GemMan::fillGemState(state);
    sendCacheState(NULL, &state);
  }
}


///////////////////
// here come some messages for setting up a manual GemState
void gemlist :: ticktimeMess(t_float ticktime)
{
  m_tickTime=ticktime;
}
void gemlist :: lightingMess(bool light)
{
  m_lightState=light;
}
void gemlist :: drawMess(t_atom arg)
{
  if(A_SYMBOL==arg.a_type) {
    t_symbol*type=atom_getsymbol(&arg);
    char c=*type->s_name;
    switch (c){
    case 'D': case 'd': // default
      m_drawType = GL_DEFAULT_GEM;
      break;
    case 'L': case 'l': // line
      m_drawType = GL_LINE;
      break;
    case 'F': case 'f': // fill
      m_drawType = GL_FILL;
      break;
    case 'P': case 'p': // point
      m_drawType = GL_POINT;
      break;
    default:
      m_drawType = static_cast<GLenum>(getGLdefine(&arg));
    }
  }
  else m_drawType=atom_getint(&arg);
}



/////////////////////////////////////////////////////////
// rightRender
//
/////////////////////////////////////////////////////////
void gemlist :: rightRender(GemCache*cache, GemState *state)
{
  if(state) {
    m_current_state=*state;
    m_valide_state=true;
    // get the current state on the right inlet
  } else {
    m_valide_state=false;
  }

  m_cache=cache;

}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemlist :: obj_setupCallback(t_class *classPtr)
{
  class_addbang(classPtr, reinterpret_cast<t_method>(&gemlist::triggerMessCallback));
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemlist::gem_rightMessCallback), gensym("gem_right"), A_GIMME, A_NULL);


  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemlist::ticktimeMessCallback), gensym("ticktime"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemlist::lightingMessCallback), gensym("lighting"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemlist::drawMessCallback), gensym("draw"), A_GIMME, A_NULL);
}

void gemlist::triggerMessCallback(void *data)
{
  GetMyClass(data)->trigger();
}


void gemlist :: ticktimeMessCallback(void *data, t_floatarg time)
{
    GetMyClass(data)->ticktimeMess(time);
}
void gemlist :: lightingMessCallback(void *data, t_floatarg light)
{
    GetMyClass(data)->lightingMess(light>0.f);
}
void gemlist :: drawMessCallback(void *data, int argc, t_atom*argv)
{
  if(argc==1)GetMyClass(data)->drawMess(argv[0]);
}

void gemlist::gem_rightMessCallback(void *data, t_symbol *s, int argc, t_atom *argv)
{
  GemCache*cache=NULL;
  GemState*state=NULL;
  if (argc==1 && argv->a_type==A_FLOAT){
    GetMyClass(data)->rightRender(cache, state);
  } else if (argc==2 && argv->a_type==A_POINTER && (argv+1)->a_type==A_POINTER){
    cache=reinterpret_cast<GemCache*>(argv->a_w.w_gpointer);
    state=reinterpret_cast<GemState*>((argv+1)->a_w.w_gpointer);
    GetMyClass(data)->rightRender( cache, state );
  } else GetMyClass(data)->error("wrong righthand arguments....");
}
