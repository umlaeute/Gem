/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_SPLINE_PATH_H_
#define _INCLUDE__GEM_MANIPS_SPLINE_PATH_H_

#include "Base/GemPathBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    spline_path


DESCRIPTION


-----------------------------------------------------------------*/
class GEM_EXTERN spline_path : public GemPathBase
{
  CPPEXTERN_HEADER(spline_path, GemPathBase);

public:

  //////////
  // Constructor
  spline_path(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~spline_path();

  //////////
  // When a float val is received
  virtual void lookupFunc(t_float x, t_float *ret, int numDimen, const gem::RTE::Array&array);
};

#endif  // for header file
