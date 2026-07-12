/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2000, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_SHININESS_H_
#define _INCLUDE__GEM_MANIPS_SHININESS_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    shininess

    shininess a gem object

DESCRIPTION

    Inlet for a float - "shininess"

    "shininess" - the shininess to set the object to

-----------------------------------------------------------------*/
class GEM_EXTERN shininess : public GemBase
{
  CPPEXTERN_HEADER(shininess, GemBase);

public:

  //////////
  // Constructor
  shininess(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~shininess();

  //////////
  // Turn back on the color material
  virtual void    postrender(GemState *state);

  //////////
  // When a gem message is received
  virtual void    render(GemState *state);

  //////////
  // the shininess
  float           m_shininess;

  //////////
  // shininess changed
  void            shininessMess(float val);
};

#endif  // for header file
