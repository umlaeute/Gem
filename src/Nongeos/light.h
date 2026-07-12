/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  A light

  -----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_NONGEOS_LIGHT_H_
#define _INCLUDE__GEM_NONGEOS_LIGHT_H_

#include "Nongeos/world_light.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  light

  Creates a light which can be positioned

  DESCRIPTION

  Inlet for a list - "poslist"

  "poslist" - Determines position

  -----------------------------------------------------------------*/
class GEM_EXTERN light : public world_light
{
  CPPEXTERN_HEADER(light, world_light);

public:

  //////////
  // Constructor
  light(t_floatarg lightNum);

protected:

  //////////
  // Destructor
  virtual ~light();

  virtual void    renderDebug();
};

#endif  // for header file
