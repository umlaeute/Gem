////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "part_sink.h"


#include <string.h>

CPPEXTERN_NEW_WITH_GIMME(part_sink);

/////////////////////////////////////////////////////////
//
// part_sink
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
part_sink :: part_sink(int argc, t_atom*argv)
  : m_kill(false), m_domain(PDSphere)
{
  int i=9;
  while(i--) {
    m_arg[i]=0.0;
  }
  m_arg[3]=0.2f;

  if (argc>0) {
    if (argv->a_type==A_SYMBOL) {
      domainMess(atom_getsymbol(argv)->s_name);
      argv++;
      argc--;
    }
    vectorMess(0, argc, argv);
  }

  //inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("kill"));
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("symbol"),
            gensym("domain"));
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"),
            gensym("vector"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
part_sink :: ~part_sink()
{ }
/////////////////////////////////////////////////////////
// vel.domain
//
/////////////////////////////////////////////////////////
void part_sink :: domainMess(const std::string&str)
{
  if(0) {
  } else if (str == "point"    ) {
    m_domain=PDPoint;
  } else if (str == "line"     ) {
    m_domain=PDLine;
  } else if (str == "triangle" ) {
    m_domain=PDTriangle;
  } else if (str == "plane"    ) {
    m_domain=PDPlane;
  } else if (str == "box"      ) {
    m_domain=PDBox;
  } else if (str == "sphere"   ) {
    m_domain=PDSphere;
  } else if (str == "cylinder" ) {
    m_domain=PDCylinder;
  } else if (str == "cone"     ) {
    m_domain=PDCone;
  } else if (str == "blob"     ) {
    m_domain=PDBlob;
  } else if (str == "disc"     ) {
    m_domain=PDDisc;
  } else if (str == "rectangle") {
    m_domain=PDRectangle;
  } else {
    error("unknown domain '%s'", str.c_str());
  }
}
void part_sink :: killMess(int kill)
{
  m_kill=kill>0;
}
void part_sink :: vectorMess(t_symbol*s, int argc, t_atom*argv)
{
  int i=9;
  while(i--)if(argc>i) {
      m_arg[i]=atom_getfloat(argv+i);
    }
}

/////////////////////////////////////////////////////////
// renderParticles
//
/////////////////////////////////////////////////////////
void part_sink :: renderParticles(GemState *state)
{
  if (m_tickTime > 0.f) {
    pSink(m_kill, m_domain,
          m_arg[0],m_arg[1],m_arg[2],m_arg[3],m_arg[4],m_arg[5],m_arg[6],m_arg[7],
          m_arg[8]);
  }
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void part_sink :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "kill", killMess, int);
  CPPEXTERN_MSG1(classPtr, "domain", domainMess, std::string);
  CPPEXTERN_MSG (classPtr, "vector", vectorMess);
}
