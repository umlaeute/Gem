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

#include "part_size.h"

#include "papi.h"

CPPEXTERN_NEW_WITH_GIMME(part_size)

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
  sizeMess(argc,argv);
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("size"));
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
void part_size :: sizeMess(int argc, t_atom*argv)
{ 
  switch(argc){
  case 1:
    m_size[0]=m_size[1]=m_size[2]=atom_getfloat(argv);
    break;
  case 3:
    m_size[0]=atom_getfloat(argv++);
    m_size[1]=atom_getfloat(argv++);
    m_size[2]=atom_getfloat(argv++);
  default:;
  }
}
/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void part_size :: render(GemState *state)
{
  if (state->tickTime > 0.f)    {
      pSize(m_size[0], m_size[1], m_size[2]);
    }
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void part_size :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&part_size::sizeMessCallback,
		  gensym("size"), A_GIMME, A_NULL);
}
void part_size :: sizeMessCallback(void *data, t_symbol*s, int argc, t_atom*argv)
{
  GetMyClass(data)->sizeMess(argc, argv);
}

