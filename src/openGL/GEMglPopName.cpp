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

#include "GEMglPopName.h"

CPPEXTERN_NEW ( GEMglPopName);

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglPopName :: GEMglPopName    ()
{
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglPopName :: ~GEMglPopName () {}

//////////////////
// extension check
bool GEMglPopName :: isRunnable(void)
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
void GEMglPopName :: render(GemState *state)
{
  glPopName ();
}

/////////////////////////////////////////////////////////
// static member functions
//
void GEMglPopName :: obj_setupCallback(t_class *classPtr) {}
