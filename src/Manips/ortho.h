/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_ORTHO_H_
#define _INCLUDE__GEM_MANIPS_ORTHO_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
        ortho

        Use orthogonal viewing

DESCRIPTION

    Inlet for a list - "orthostate"

    "orthostate" - whether to go orthogonal

-----------------------------------------------------------------*/
class GEM_EXTERN ortho : public GemBase
{
  CPPEXTERN_HEADER(ortho, GemBase);

public:

  //////////
  // Constructor
  ortho();

protected:

  //////////
  // Destructor
  virtual ~ortho();

  //////////
  // Do the rendering
  virtual void    render(GemState *state);

  //////////
  // Turn back on depth test
  virtual void    postrender(GemState *state);

  //////////
  // Ortho state
  int                     m_state;

  //////////
  // Ortho changed
  void            orthoMess(int state);

  //////////
  // Compatibility
  int                     m_compat;
  void            compatMess(int state);
};

#endif  // for header file
