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

#include "part_draw.h"

#include <string.h>
#include "Gem/State.h"

#include "papi/papi.h"

CPPEXTERN_NEW(part_draw);

/////////////////////////////////////////////////////////
//
// part_draw
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
part_draw :: part_draw(void)
  : m_drawType(GL_LINES)
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
part_draw :: ~part_draw(void)
{ }

/////////////////////////////////////////////////////////
// renderParticles
//
/////////////////////////////////////////////////////////
void part_draw :: renderParticles(GemState *state)
{
  bool lighting=false;
  state->get(GemState::_GL_LIGHTING, lighting);
  if (lighting)   {
    glDisable(GL_LIGHTING);
  }
  if (m_tickTime > 0.f)   {
    pMove();
  }
  pDrawGroupp(m_drawType);
  if (lighting) {
    glEnable(GL_LIGHTING);
  }
}

/////////////////////////////////////////////////////////
// typeMess
//
/////////////////////////////////////////////////////////
void part_draw :: typeMess(t_symbol*s,int ac,t_atom* av)
{
  if(ac&&A_SYMBOL==av->a_type) {
    t_symbol*s=atom_getsymbolarg(0,ac,av);
    switch(*(s->s_name)) {
    case 'l':
    case 'L':
      m_drawType=GL_LINES;
      break;
    case 'p':
    case 'P':
      m_drawType=GL_POINTS;
      break;
    default:
      error("unknown draw style");
      return;
    }
  } else {
    m_drawType = (int)atom_getfloatarg(0,ac,av);
  }
}


/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void part_draw :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG (classPtr, "draw", typeMess);
}
