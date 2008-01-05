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
#include "Base/GemState.h"

CPPEXTERN_NEW(gemlist)

/////////////////////////////////////////////////////////
//
// gemlist
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemlist :: gemlist()
{
    m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("gem_state"), gensym("gem_right"));
	// create the cold inlet
	m_valide_state=false;
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
	m_curent_state=state; //copy curent state for later use
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

/////////////////////////////////////////////////////////
// bang
//
/////////////////////////////////////////////////////////
void gemlist :: trigger()
{
	if(m_valide_state)
	{   // outlet the curent state when banged
		t_atom ap[2];
		ap->a_type=A_POINTER;
		ap->a_w.w_gpointer=(t_gpointer *)m_cache;
		(ap+1)->a_type=A_POINTER;
		(ap+1)->a_w.w_gpointer=(t_gpointer *)m_curent_state;
		outlet_anything(this->m_out1, gensym("gem_state"), 2, ap);
	}
	else 
		error("you should not bang the gemlist now");
}

/////////////////////////////////////////////////////////
// rightRender
//
/////////////////////////////////////////////////////////
void gemlist :: rightRender(GemState *state)
{
	m_curent_state=state;
	m_valide_state=true;
	// get the curent state on the right inlet
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemlist :: obj_setupCallback(t_class *classPtr)
{
    class_addbang(classPtr, (t_method)&gemlist::triggerMessCallback);
	class_addmethod(classPtr, (t_method)&gemlist::gem_rightMessCallback, gensym("gem_right"), A_GIMME, A_NULL);
}

void gemlist::triggerMessCallback(void *data)
{
    GetMyClass(data)->trigger();
}


void gemlist::gem_rightMessCallback(void *data, t_symbol *s, int argc, t_atom *argv)
{
  if (argc==1 && argv->a_type==A_FLOAT){
  } else if (argc==2 && argv->a_type==A_POINTER && (argv+1)->a_type==A_POINTER){
    GetMyClass(data)->m_cache = (GemCache*)argv->a_w.w_gpointer;
    GetMyClass(data)->rightRender((GemState *)(argv+1)->a_w.w_gpointer);
  } else GetMyClass(data)->error("wrong righthand arguments....");
}
