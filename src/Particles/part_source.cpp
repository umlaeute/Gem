////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "part_source.h"

#include "papi.h"

CPPEXTERN_NEW_WITH_ONE_ARG(part_source, t_floatarg, A_DEFFLOAT)

  /////////////////////////////////////////////////////////
//
// part_source
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
part_source :: part_source(t_floatarg num)
  : m_numberToAdd(150)
{
  if (num > 0)
    m_numberToAdd = (int)num;
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("numToAdd"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
part_source :: ~part_source()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void part_source :: render(GemState *state)
{
  if (state->stereo == 0 ||
      state->stereo == 1) {
    pSource((float)m_numberToAdd, PDSphere, 0.f, 0.f, 0.f, .02f);
  }
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void part_source :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&part_source::numberMessCallback,
		  gensym("numToAdd"), A_FLOAT, A_NULL);
}
void part_source :: numberMessCallback(void *data, t_floatarg num)
{
  GetMyClass(data)->numberMess((int)num);
}

