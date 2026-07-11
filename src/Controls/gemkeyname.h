/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_CONTROLS_GEMKEYNAME_H_
#define _INCLUDE__GEM_CONTROLS_GEMKEYNAME_H_

#include "Base/CPPExtern.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    gemkeyname

    Respond to keyboard events

DESCRIPTION


-----------------------------------------------------------------*/
class GEM_EXTERN gemkeyname : public CPPExtern
{
  CPPEXTERN_HEADER(gemkeyname, CPPExtern);

public:

  //////////
  // Constructor
  gemkeyname();

protected:

  //////////
  // Destructor
  virtual ~gemkeyname();

  //////////
  // keyname-button
  void            KeyNamePressed(const char* string, int val, int state);

  //////////
  // The key-val outlet
  t_outlet        *m_outKeyVal;

  //////////
  // The key-state outlet
  t_outlet        *m_outKeyState;


private:

  //////////
  // Static member functions
  static void     keynameCallback(const char* x, int y, int z, void *data);
};

#endif  // for header file
