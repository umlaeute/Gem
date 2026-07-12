/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_DEPTH_H_
#define _INCLUDE__GEM_MANIPS_DEPTH_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    depth

    Turn on/off depth test

DESCRIPTION

    Inlet for a list - "depthstate"

    "depthstate" - whether to use the Z-buffer

-----------------------------------------------------------------*/
class GEM_EXTERN depth : public GemBase
{
  CPPEXTERN_HEADER(depth, GemBase);

public:

  //////////
  // Constructor
  depth(int, t_atom*);

protected:

  //////////
  // Destructor
  virtual ~depth();

  //////////
  // Do the rendering
  virtual void    render(GemState *state);

  //////////
  // Turn back on depth test
  virtual void    postrender(GemState *state);

  //////////
  // Depth state
  int                     m_state;

  //////////
  // Depth changed
  void            depthMess(int state);
};

#endif  // for header file
