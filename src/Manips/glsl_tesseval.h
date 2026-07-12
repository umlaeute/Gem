/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2019, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_GLSL_TESSEVAL_H_
#define _INCLUDE__GEM_MANIPS_GLSL_TESSEVAL_H_

#include "Manips/glsl_vertex.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  glsl_tesseval

  Loads in a GLSL tesselation evaluation shader

  KEYWORDS

  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN glsl_tesseval : public glsl_vertex
{
  CPPEXTERN_HEADER(glsl_tesseval, glsl_vertex);

public:

  //////////
  // Constructor
  glsl_tesseval(void);
  glsl_tesseval(t_symbol* filename);

protected:

  //////////
  // Destructor
  virtual ~glsl_tesseval();

  ////////
  // extension check
  virtual bool isRunnable(void);

  //////////
  // Print Info about Hardware limits
  virtual void printInfo();
};

#endif  // for header file
