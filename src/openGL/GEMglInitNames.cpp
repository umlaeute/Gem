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

#include "GEMglInitNames.h"

CPPEXTERN_NEW( GEMglInitNames);

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglInitNames :: GEMglInitNames()
{
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglInitNames :: ~GEMglInitNames () {}

//////////////////
// extension check
bool GEMglInitNames :: isRunnable(void)
{
  if(GLEW_VERSION_1_1) {
    return true;
  }
  error("your system does not support OpenGL-1.1");
  return false;
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglInitNames :: render(GemState *state)
{
  glInitNames ();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglInitNames :: obj_setupCallback(t_class *classPtr) {}
