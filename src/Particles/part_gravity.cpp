////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "part_gravity.h"

CPPEXTERN_NEW_WITH_THREE_ARGS(part_gravity, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

  /////////////////////////////////////////////////////////
//
// part_gravity
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
part_gravity :: part_gravity(t_floatarg xpos, t_floatarg ypos, t_floatarg zpos)
{
  m_vector[0] = xpos;
  m_vector[1] = ypos;
  m_vector[2] = zpos;

  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("vector"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
part_gravity :: ~part_gravity()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void part_gravity :: render(GemState *state)
{
  if (state->doUpdate)
    {
      pGravity(m_vector[0], m_vector[1], m_vector[2]);
    }
}

/////////////////////////////////////////////////////////
// vectorMess
//
/////////////////////////////////////////////////////////
void part_gravity :: vectorMess(float x, float y, float z)
{
  m_vector[0] = x;
  m_vector[1] = y;
  m_vector[2] = z;
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void part_gravity :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&part_gravity::vectorMessCallback,
		  gensym("vector"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL); 
}
void part_gravity :: vectorMessCallback(void *data, t_floatarg x, t_floatarg y, t_floatarg z)
{
  GetMyClass(data)->vectorMess((float)x, (float)y, (float)z);
}
