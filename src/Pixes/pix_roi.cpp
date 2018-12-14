////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2012 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
// this is based on EffecTV by Fukuchi Kentarou
// * Copyright (C) 2001 FUKUCHI Kentarou
//
/////////////////////////////////////////////////////////

#include "pix_roi.h"
#include "Gem/State.h"
#include "Gem/Exception.h"

CPPEXTERN_NEW_WITH_GIMME(pix_roi);

/////////////////////////////////////////////////////////
//
// pix_roi
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
int pix_roi::s_ID = -1;
pix_roi :: pix_roi(int argc, t_atom*argv) :
  m_oldrect(NULL)
{
  if(s_ID<0) {
    s_ID=GemState::getKey("pix.roi.rectangle");
  }

  switch(argc) {
  case 0:
    break;
  case 4:
    m_rectangle=gem::Rectangle::createNormalized(
                  atom_getfloat(argv+0),
                  atom_getfloat(argv+1),
                  atom_getfloat(argv+2),
                  atom_getfloat(argv+3));
    break;
  default:
    throw(GemException("arguments: [<x1> <y1> <x2> <y2>]"));
    break;
  }
}

pix_roi :: ~pix_roi()
{}

void pix_roi :: render(GemState*state)
{
  if(s_ID>=0) {
    m_staterect=m_rectangle;
    state->get((GemState::key_t)s_ID, m_oldrect);
    state->set((GemState::key_t)s_ID, &m_staterect);
  }
}

void pix_roi :: postrender(GemState*state)
{
  if(s_ID>=0) {
    state->set((GemState::key_t)s_ID, m_oldrect);
  }
}

void pix_roi :: roiMess(float x1, float y1, float x2, float y2)
{
  m_rectangle=gem::Rectangle::createNormalized(x1, y1, x2, y2);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_roi :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG4(classPtr, "roi", roiMess, float, float, float, float);
}
