////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "part_vertex.h"

#include "papi.h"

CPPEXTERN_NEW_WITH_THREE_ARGS(part_vertex, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

  /////////////////////////////////////////////////////////
//
// part_vertex
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
part_vertex :: part_vertex(t_floatarg x, t_floatarg y, t_floatarg z)
  : m_x(x), m_y(y), m_z(z)
{
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym(""));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
part_vertex :: ~part_vertex()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void part_vertex :: render(GemState *state)
{
  if (state->stereo == 0 ||
      state->stereo == 1) {

    pVertex(m_x, m_y, m_z);
  }
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void part_vertex :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&part_vertex::posMessCallback,
		  gensym(""), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
}
void part_vertex :: posMessCallback(void *data, t_floatarg x, t_floatarg y, t_floatarg z)
{
  GetMyClass(data)->posMess(x, y, z);
}

