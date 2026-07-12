/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_SHEARXZ_H_
#define _INCLUDE__GEM_MANIPS_SHEARXZ_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    shearXZ

    shear a gem object

DESCRIPTION



-----------------------------------------------------------------*/
class GEM_EXTERN shearXZ : public GemBase
{
  CPPEXTERN_HEADER(shearXZ, GemBase);

public:

  //////////
  // Constructor
  shearXZ(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~shearXZ();

  //////////
  // When rendering occurs
  virtual void    render(GemState *state);


  //////////
  // X value changed
  void            shearMess(float val);

  //shear value
  float                   shear;
};

#endif  // for header file
