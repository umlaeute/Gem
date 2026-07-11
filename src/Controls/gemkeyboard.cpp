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

#include "gemkeyboard.h"

#include "Gem/Event.h"

CPPEXTERN_NEW(gemkeyboard);

/////////////////////////////////////////////////////////
//
// gemkeyboard
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemkeyboard :: gemkeyboard()
{
  m_outKeyVal = outlet_new(this->x_obj, 0);

  // register event callback
  setKeyboardCallback(&gemkeyboard::keyboardCallback, this);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemkeyboard :: ~gemkeyboard()
{
  // remove event callback
  removeKeyboardCallback(&gemkeyboard::keyboardCallback, this);

  outlet_free(m_outKeyVal);
}


/////////////////////////////////////////////////////////
// KeyBoardPressed
//
/////////////////////////////////////////////////////////
void gemkeyboard :: KeyBoardPressed(int val, int state)
{
  if (state==0) {
    return;
  }
  outlet_float(m_outKeyVal, static_cast<t_float>(val));
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemkeyboard :: obj_setupCallback(t_class *)
{ }
void gemkeyboard :: keyboardCallback(const char* w, int x, int y, void *data)
{
  (reinterpret_cast<gemkeyboard*>(data))->KeyBoardPressed(x, y);
}
