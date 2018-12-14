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
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "part_velocity.h"


#include <string.h>

CPPEXTERN_NEW_WITH_GIMME(part_velocity);

/////////////////////////////////////////////////////////
//
// part_velocity
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
part_velocity :: part_velocity(int argc, t_atom*argv) :
  m_domain(PDPoint)
{
  int i=9;
  while(i--) {
    m_arg[i]=0.0;
  }
  if (argc>0) {
    if (argv->a_type==A_SYMBOL) {
      domainMess(atom_getsymbol(argv)->s_name);
      argv++;
      argc--;
    }
    vectorMess(0, argc, argv);
  }
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("symbol"),
            gensym("domain"));
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"),
            gensym("vector"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
part_velocity :: ~part_velocity(void)
{ }

/////////////////////////////////////////////////////////
// renderParticles
//
/////////////////////////////////////////////////////////
void part_velocity :: renderParticles(GemState *state)
{
  if (m_tickTime > 0.f)
    pVelocityD(m_domain,
               m_arg[0],m_arg[1],m_arg[2],m_arg[3],m_arg[4],m_arg[5],m_arg[6],m_arg[7],
               m_arg[8]);
}

void part_velocity :: domainMess(const std::string&str)
{
  if(0) {
  } else if ("point"    ==str) {
    m_domain=PDPoint;
  } else if ("line"     ==str) {
    m_domain=PDLine;
  } else if ("triangle" ==str) {
    m_domain=PDTriangle;
  } else if ("plane"    ==str) {
    m_domain=PDPlane;
  } else if ("box"      ==str) {
    m_domain=PDBox;
  } else if ("sphere"   ==str) {
    m_domain=PDSphere;
  } else if ("cylinder" ==str) {
    m_domain=PDCylinder;
  } else if ("cone"     ==str) {
    m_domain=PDCone;
  } else if ("blob"     ==str) {
    m_domain=PDBlob;
  } else if ("disc"     ==str) {
    m_domain=PDDisc;
  } else if ("rectangle"==str) {
    m_domain=PDRectangle;
  } else {
    error("unknown domain");
  }
}

void part_velocity :: vectorMess(t_symbol*s, int argc, t_atom*argv)
{
  int i=9;
  while(i--)if(argc>i) {
      m_arg[i]=atom_getfloat(argv+i);
    }
}
/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void part_velocity :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG (classPtr, "vector", vectorMess);
  CPPEXTERN_MSG1(classPtr, "domain", domainMess, std::string);
}
