////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
/////////////////////////////////////////////////////////

#include "square.h"

CPPEXTERN_NEW_WITH_ONE_ARG(square, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// square
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
square :: square(t_floatarg size)
  : GemShape(size)
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
square :: ~square()
{ }

/////////////////////////////////////////////////////////
// renderShape
//
/////////////////////////////////////////////////////////
void square :: renderShape(GemState *state)
{
  if(m_drawType==GL_DEFAULT_GEM) {
    m_drawType=GL_QUADS;
  }
  glNormal3f(0.0f, 0.0f, 1.0f);
  if (m_drawType == GL_LINE_LOOP) {
    glLineWidth(m_linewidth);
  }

  glBegin(m_drawType);

  SetVertex(state, -m_size, -m_size, 0.,  0., 0.,  0);
  SetVertex(state,  m_size, -m_size, 0.,  1., 0.,  1);
  SetVertex(state,  m_size,  m_size, 0.,  1., 1.,  2);
  SetVertex(state, -m_size,  m_size, 0.,  0., 1.,  3);

  glEnd();
}

void square :: obj_setupCallback(t_class *classPtr) {}
