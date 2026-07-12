/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_LINEAR_PATH_H_
#define _INCLUDE__GEM_MANIPS_LINEAR_PATH_H_

#include "Base/GemPathBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    linear_path


DESCRIPTION


-----------------------------------------------------------------*/
class GEM_EXTERN linear_path : public GemPathBase
{
  CPPEXTERN_HEADER(linear_path, GemPathBase);

public:

  //////////
  // Constructor
  linear_path(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~linear_path();

  //////////
  // When a float val is received
  virtual void lookupFunc(t_float x, t_float *ret, int numDimen, const gem::RTE::Array&array);
};

#endif  // for header file
