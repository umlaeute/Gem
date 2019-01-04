/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  Interface for the window manager

  Copyright (c) 2019 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

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
