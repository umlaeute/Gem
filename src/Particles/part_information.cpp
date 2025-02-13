////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "part_information.h"

#include <string.h>


#include "papi/papi.h"

CPPEXTERN_NEW(part_information);

/////////////////////////////////////////////////////////
//
// part_information
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
part_information :: part_information()
{
  const size_t length = 1024;
  m_position.reserve(length*3);
  m_color.reserve(length*4);
  m_velocity.reserve(length*3);
  m_size.reserve(length*3);
  m_age.reserve(length);

  out_num = outlet_new(this->x_obj, 0);
  out_position = outlet_new(this->x_obj, 0);
  out_color = outlet_new(this->x_obj, 0);
  out_velocity = outlet_new(this->x_obj, 0);
  out_size = outlet_new(this->x_obj, 0);
  out_age = outlet_new(this->x_obj, 0);

  unsigned int i;
  const unsigned int alistsize=sizeof(m_alist)/sizeof(*m_alist);
  for(i=0; i<alistsize; i++) {
    SETFLOAT(m_alist+i, 0.f);
  }
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
part_information :: ~part_information()
{}

/////////////////////////////////////////////////////////
// renderParticles
//
/////////////////////////////////////////////////////////
void part_information :: renderParticles(GemState *state)
{
  const int length = pGetGroupCount();
  if(length < 1) {
    return;
  }
  m_position.reserve(length*3);
  m_color.reserve(length*4);
  m_velocity.reserve(length*3);
  m_size.reserve(length*3);
  m_age.reserve(length);

  float *position = m_position.data();
  float *color    = m_color.data();
  float *velocity = m_velocity.data();
  float *size     = m_size.data();
  float *age      = m_age.data();
  pGetParticles(0, length, position, color, velocity, size, age);
  for(int i = 0; i < length; i++)  {
    SETFLOAT(m_alist+0, position[0]);
    SETFLOAT(m_alist+1, position[1]);
    SETFLOAT(m_alist+2, position[2]);
    position+=3;
    SETFLOAT(m_alist+3, color[0]);
    SETFLOAT(m_alist+4, color[1]);
    SETFLOAT(m_alist+5, color[2]);
    SETFLOAT(m_alist+6, color[3]);
    color+=4;
    SETFLOAT(m_alist+7, velocity[0]);
    SETFLOAT(m_alist+8, velocity[1]);
    SETFLOAT(m_alist+9, velocity[2]);
    velocity+=3;
    SETFLOAT(m_alist+10, size[0]);
    SETFLOAT(m_alist+11, size[1]);
    SETFLOAT(m_alist+12, size[2]);
    size+=3;

    outlet_float(out_age, age[i]);
    outlet_list (out_size, &s_list, 3, m_alist+10);
    outlet_list (out_velocity, &s_list, 3, m_alist+7);
    outlet_list (out_color, &s_list, 4, m_alist+3);
    outlet_list (out_position, &s_list, 3, m_alist+0);
    outlet_float(out_num, i);
    continueRender(state);
  }
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void part_information :: obj_setupCallback(t_class *classPtr)
{}
