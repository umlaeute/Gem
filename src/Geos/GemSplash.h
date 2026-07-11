/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2006, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_GEMSPLASH_H_
#define _INCLUDE__GEM_GEOS_GEMSPLASH_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GemSplash

    Creates a GemSplash

KEYWORDS
    geo

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN GemSplash : public GemBase
{
  CPPEXTERN_HEADER(GemSplash, GemBase);

public:

  //////////
  // Constructor
  GemSplash();

protected:

  //////////
  // Destructor
  virtual ~GemSplash();

  //////////
  // Do the rendering
  virtual void    render(GemState *state);
};

#endif  // for header file
