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

#include "GEMglPopClientAttrib.h"

CPPEXTERN_NEW ( GEMglPopClientAttrib);

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglPopClientAttrib :: GEMglPopClientAttrib    ()
{
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglPopClientAttrib :: ~GEMglPopClientAttrib () {}

//////////////////
// extension check
bool GEMglPopClientAttrib :: isRunnable(void)
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
void GEMglPopClientAttrib :: render(GemState *state)
{
  glPopClientAttrib ();
}


void GEMglPopClientAttrib :: obj_setupCallback(t_class *classPtr) {}
