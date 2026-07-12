/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_BASE_GEMPATHBASE_H_
#define _INCLUDE__GEM_BASE_GEMPATHBASE_H_

#include "Base/CPPExtern.h"
#include "RTE/Array.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GemPathBase


DESCRIPTION
    Base class for paths

-----------------------------------------------------------------*/
class GEM_EXTERN GemPathBase : public CPPExtern
{
  CPPEXTERN_HEADER(GemPathBase, CPPExtern);

public:

  //////////
  // Constructor
  GemPathBase(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~GemPathBase();

  //////////
  // When an open is received
  virtual void    openMess(t_symbol* arrayname);

  //////////
  // When a float val is received
  virtual void    floatMess(t_float val);

  ///////////
  // do the actual interpolation
  virtual void lookupFunc(t_float x, t_float *ret, int numDimen, const gem::RTE::Array&array) = 0;

  //////////
  // The number of dimensions
  int             m_numDimens;

  //////////
  // The array
  t_symbol      *m_arrayname;
  bool           m_warnedNonExistent;

  gem::RTE::Array m_array;

  //////////
  // The outlet
  t_outlet        *m_out1;
};

#endif  // for header file
