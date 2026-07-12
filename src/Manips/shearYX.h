/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_SHEARYX_H_
#define _INCLUDE__GEM_MANIPS_SHEARYX_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    shearYX

    shear a gem object

DESCRIPTION



-----------------------------------------------------------------*/
class GEM_EXTERN shearYX : public GemBase
{
  CPPEXTERN_HEADER(shearYX, GemBase);

public:

  //////////
  // Constructor
  shearYX(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~shearYX();

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
