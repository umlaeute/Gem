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

#include "part_veldomain.h"
#include <string.h>

CPPEXTERN_NEW_WITH_GIMME(part_veldomain)

  /////////////////////////////////////////////////////////
//
// part_veldomain
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
part_veldomain :: part_veldomain(int argc, t_atom*argv)
{
  int i=9;
  while(i--)m_arg[i]=0.0;
  if (argc>0)domainMess(atom_getsymbol(argv));
  vectorMess(argc-1, argv+1);
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("symbol"), gensym("domain"));
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("vector"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
part_veldomain :: ~part_veldomain()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void part_veldomain :: render(GemState *state)
{
  if (state->stereo == 0 || state->stereo == 1)
    pVelocityD(m_domain,
	       m_arg[0],m_arg[1],m_arg[2],m_arg[3],m_arg[4],m_arg[5],m_arg[6],m_arg[7],m_arg[8]);
}

void part_veldomain :: domainMess(t_symbol*s){
  char *str=s->s_name;
       if (!strcmp(str,"point"    ))m_domain=PDPoint;
  else if (!strcmp(str,"line"     ))m_domain=PDLine;
  else if (!strcmp(str,"triangle" ))m_domain=PDTriangle;
  else if (!strcmp(str,"plane"    ))m_domain=PDPlane;
  else if (!strcmp(str,"box"      ))m_domain=PDBox;
  else if (!strcmp(str,"sphere"   ))m_domain=PDSphere;
  else if (!strcmp(str,"cylinder" ))m_domain=PDCylinder;
  else if (!strcmp(str,"cone"     ))m_domain=PDCone;
  else if (!strcmp(str,"blob"     ))m_domain=PDBlob;
  else if (!strcmp(str,"disc"     ))m_domain=PDDisc;
  else if (!strcmp(str,"rectangle"))m_domain=PDRectangle;
  else error("GEM: part_veldomain: unknown domain");
}

void part_veldomain :: vectorMess(int argc, t_atom*argv){
  int i=9;
  while(i--)if(argc>i)m_arg[i]=atom_getfloat(argv+i);
}
/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void part_veldomain :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&part_veldomain::vectorMessCallback,
		  gensym("vector"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&part_veldomain::domainMessCallback,
		  gensym("domain"), A_SYMBOL, A_NULL);
}
void part_veldomain :: domainMessCallback(void *data, t_symbol*s)
{
  GetMyClass(data)->domainMess(s);
}
void part_veldomain :: vectorMessCallback(void *data, t_symbol*, int argc, t_atom*argv)
{
  GetMyClass(data)->vectorMess(argc, argv);
}
