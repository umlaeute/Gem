
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

#include "part_velocity.h"

#include "papi.h"

CPPEXTERN_NEW_WITH_GIMME(part_velocity)

/////////////////////////////////////////////////////////
//
// part_velocity
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
part_velocity :: part_velocity(int argc, t_atom*argv) : part_size(argc, argv)
{}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void part_velocity :: render(GemState *state)
{
  if (state->stereo == 0 ||
      state->stereo == 1)    {
      pVelocity(m_size[0], m_size[1], m_size[2]);
    }
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void part_velocity :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&part_velocity::sizeMessCallback,
		  gensym("size"), A_GIMME, A_NULL);
}
void part_velocity :: sizeMessCallback(void *data, t_symbol*s, int argc, t_atom*argv)
{
  GetMyClass(data)->sizeMess(argc, argv);
}

