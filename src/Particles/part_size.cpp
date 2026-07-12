////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
////////////////////////////////////////////////////////
#include "part_size.h"
#include "papi/papi.h"

CPPEXTERN_NEW_WITH_GIMME(part_size);

/////////////////////////////////////////////////////////
//
// part_size
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
part_size :: part_size(int argc, t_atom*argv)
{
  m_size[0]=m_size[1]=m_size[2]=1.0f;
  sizeMess(0, argc,argv);
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
            gensym("size"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
part_size :: ~part_size()
{ }

/////////////////////////////////////////////////////////
// sizeMess
//
/////////////////////////////////////////////////////////
void part_size :: sizeMess(t_symbol*s, int argc, t_atom*argv)
{
  switch(argc) {
  case 1:
    m_size[0]=m_size[1]=m_size[2]=atom_getfloat(argv);
    break;
  case 3:
    m_size[0]=atom_getfloat(argv++);
    m_size[1]=atom_getfloat(argv++);
    m_size[2]=atom_getfloat(argv++);
  default:
    ;
  }
}
/////////////////////////////////////////////////////////
// renderParticles
//
/////////////////////////////////////////////////////////
void part_size :: renderParticles(GemState *state)
{
  if (m_tickTime > 0.f)    {
    pSize(m_size[0], m_size[1], m_size[2]);
  }
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void part_size :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG (classPtr, "size", sizeMess);
}
