/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2005, James Tittle II and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_GLSL_FRAGMENT_H_
#define _INCLUDE__GEM_MANIPS_GLSL_FRAGMENT_H_

#include "Manips/glsl_vertex.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  glsl_fragment

  Loads in a GLSL fragment shader

  KEYWORDS


  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN glsl_fragment : public glsl_vertex
{
  CPPEXTERN_HEADER(glsl_fragment, glsl_vertex);

public:

  //////////
  // Constructor
  glsl_fragment(void);
  glsl_fragment(t_symbol* filename);

protected:

  //////////
  // Destructor
  virtual ~glsl_fragment();

  ////////
  // extension check
  virtual bool isRunnable(void);

  //////////
  // Print Info about Hardware limits
  virtual void printInfo();
};

#endif  // for header file
