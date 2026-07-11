/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2019, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */
#ifndef _INCLUDE__GEM_CONTROLS_GEMMANAGER_H_
#define _INCLUDE__GEM_CONTROLS_GEMMANAGER_H_

#include "Base/CPPExtern.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  gemmanager

  interface with the global gem-state

  DESCRIPTION

  Access to GemMan.

  "dimen"   - set the current window-size to w/h

  -----------------------------------------------------------------*/
class GEM_EXTERN gemmanager : public CPPExtern
{
  CPPEXTERN_HEADER(gemmanager, CPPExtern);

public:

  //////////
  // Constructor
  gemmanager(void);

private:

  //////////
  // Destructor
  virtual       ~gemmanager();
  void          dimenMess(int width, int height);
};

#endif  // for header file
