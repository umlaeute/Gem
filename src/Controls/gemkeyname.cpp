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

#include "gemkeyname.h"

#include "Gem/Event.h"

CPPEXTERN_NEW(gemkeyname);

/////////////////////////////////////////////////////////
//
// gemkeyname
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemkeyname :: gemkeyname()
{
  m_outKeyState = outlet_new(this->x_obj, 0);
  m_outKeyVal = outlet_new(this->x_obj, 0);

  // register event callback
  setKeyboardCallback(&gemkeyname::keynameCallback, this);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemkeyname :: ~gemkeyname()
{
  // remove event callback
  removeKeyboardCallback(&gemkeyname::keynameCallback, this);

  outlet_free(m_outKeyState);
  outlet_free(m_outKeyVal);
}


/////////////////////////////////////////////////////////
// mouseMotion
//
/////////////////////////////////////////////////////////
void gemkeyname :: KeyNamePressed(const char *string, int val, int state)
{
  outlet_symbol(m_outKeyVal, gensym(string));
  outlet_float(m_outKeyState, static_cast<t_float>(state));
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemkeyname :: obj_setupCallback(t_class *)
{ }
void gemkeyname :: keynameCallback(const char *x, int y, int z, void *data)
{
  (reinterpret_cast<gemkeyname*>(data))->KeyNamePressed(x, y, z);
}
