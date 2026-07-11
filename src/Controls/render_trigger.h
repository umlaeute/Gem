/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_CONTROLS_RENDER_TRIGGER_H_
#define _INCLUDE__GEM_CONTROLS_RENDER_TRIGGER_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    render_trigger

DESCRIPTION

    Send out a bang on pre and post render

-----------------------------------------------------------------*/
class GEM_EXTERN render_trigger : public GemBase
{
  CPPEXTERN_HEADER(render_trigger, GemBase);

public:

  //////////
  // Constructor
  render_trigger();

protected:

  //////////
  // Destructor
  virtual ~render_trigger();

  //////////
  // Push the current state
  virtual void    render(GemState *state);

  //////////
  // Pop the state
  virtual void    postrender(GemState *state);

  //////////
  t_outlet        *m_preOut;          // bang for the pre render

  //////////
  t_outlet        *m_postOut;         // bang for the post render
};

#endif  // for header file
