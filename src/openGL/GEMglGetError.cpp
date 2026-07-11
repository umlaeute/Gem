////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
// this file has been generated...
////////////////////////////////////////////////////////

#include "GEMglGetError.h"

CPPEXTERN_NEW ( GEMglGetError );

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglGetError :: GEMglGetError  ()
{
  m_outlet = outlet_new(this->x_obj, 0);
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglGetError :: ~GEMglGetError ()
{
  outlet_free(m_outlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglGetError :: render(GemState *state)
{
  GLenum i=glGetError();
  outlet_float(m_outlet, static_cast<t_float>(i));
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglGetError :: obj_setupCallback(t_class *classPtr) {}
