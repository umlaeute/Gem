/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2009, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_GLSL_GEOMETRY_H_
#define _INCLUDE__GEM_MANIPS_GLSL_GEOMETRY_H_

#include "Manips/glsl_vertex.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  glsl_geometry

  Loads in a GLSL geometry shader

  KEYWORDS

  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN glsl_geometry : public glsl_vertex
{
  CPPEXTERN_HEADER(glsl_geometry, glsl_vertex);

public:

  //////////
  // Constructor
  glsl_geometry(void);
  glsl_geometry(t_symbol* filename);

protected:

  //////////
  // Destructor
  virtual ~glsl_geometry();

  ////////
  // extension check
  virtual bool isRunnable(void);

  //////////
  // Print Info about Hardware limits
  virtual void printInfo();
};

#endif  // for header file
