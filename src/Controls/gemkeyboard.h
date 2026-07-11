/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_CONTROLS_GEMKEYBOARD_H_
#define _INCLUDE__GEM_CONTROLS_GEMKEYBOARD_H_

#include "Base/CPPExtern.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    gemkeyboard

    Respond to keyboard events

DESCRIPTION


-----------------------------------------------------------------*/
class GEM_EXTERN gemkeyboard : public CPPExtern
{
  CPPEXTERN_HEADER(gemkeyboard, CPPExtern);

public:

  //////////
  // Constructor
  gemkeyboard();

protected:

  //////////
  // Destructor
  virtual ~gemkeyboard();

  //////////
  // keyboard-button
  void            KeyBoardPressed(int val, int state);

  //////////
  // The key-val outlet
  t_outlet        *m_outKeyVal;

private:

  //////////
  // Static member functions
  static void     keyboardCallback(const char *w, int x, int y, void *data);
};

#endif  // for header file
