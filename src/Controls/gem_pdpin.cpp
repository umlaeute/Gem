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

#include "gem_pdpin.h"

CPPEXTERN_NEW_WITH_GIMME(gem_pdpin)

/////////////////////////////////////////////////////////
//
// gem_pdpin
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gem_pdpin :: gem_pdpin(int argc, t_atom*argv)
{
    m_inPDP = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("pdp"), gensym("gem_pdp"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gem_pdpin :: ~gem_pdpin()
{
    inlet_free(m_inPDP);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void gem_pdpin :: render(GemState *state)
{
  //  add your pdp packet into the state
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void gem_pdpin :: postrender(GemState *state)
{
  // want to do something ?
}

/////////////////////////////////////////////////////////
// getting the pdp-packet
//
/////////////////////////////////////////////////////////
void gem_pdpin :: pdpMess(t_symbol*s, int p)
{
  // want to do something ?
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gem_pdpin :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classgem_pdpin, gensym("gem_pdp.in"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&gem_pdpin::pdpMessCallback,
		  gensym("gem_pdp"), A_SYMBOL, A_DEFFLOAT, A_NULL);
}

void gem_pdpin :: pdpMessCallback(void *data, t_symbol *s, t_floatarg f)
{
  GetMyClass(data)->pdpMess(s, (int)f);
}

