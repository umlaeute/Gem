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

#include "part_head.h"
#include "papi/papi.h"

CPPEXTERN_NEW_WITH_ONE_ARG(part_head, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// part_head
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
part_head :: part_head(t_floatarg numParts)
  : m_speed(1.f)
{
  if (numParts <= 0) {
    numParts = 1000.f;
  }
  m_particleGroup = pGenParticleGroups(1, (int)numParts);
  m_particlestate = pStateCreate();
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
part_head :: ~part_head(void)
{
  if (m_particleGroup < 0) {
    pDeleteParticleGroups(m_particleGroup, 1);
  }
}

/////////////////////////////////////////////////////////
// renderParticles
//
/////////////////////////////////////////////////////////
void part_head :: renderParticles(GemState *state)
{
  if (m_particleGroup < 0) {
    return;
  }
  pStateActivate(m_particlestate);

  // The original default was 50.f milliseconds (20 fps)
  pTimeStep((m_tickTime / 50.f) * m_speed);

  pCurrentGroup(m_particleGroup);
}

/////////////////////////////////////////////////////////
// speedMess
//
/////////////////////////////////////////////////////////
void part_head :: speedMess(float speed)
{
  m_speed = (speed < 0.001f) ? 0.0001f : speed;
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void part_head :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "speed", speedMess, float);
}
