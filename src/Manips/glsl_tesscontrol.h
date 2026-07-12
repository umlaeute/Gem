/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2019, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_GLSL_TESSCONTROL_H_
#define _INCLUDE__GEM_MANIPS_GLSL_TESSCONTROL_H_

#include "Manips/glsl_vertex.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  glsl_tesscontrol

  Loads in a GLSL tesselation control shader

  KEYWORDS

  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN glsl_tesscontrol : public glsl_vertex
{
  CPPEXTERN_HEADER(glsl_tesscontrol, glsl_vertex);

public:

  //////////
  // Constructor
  glsl_tesscontrol(void);
  glsl_tesscontrol(t_symbol* filename);

protected:

  //////////
  // Destructor
  virtual ~glsl_tesscontrol();

  ////////
  // extension check
  virtual bool isRunnable(void);

  //////////
  // Print Info about Hardware limits
  virtual void printInfo();
};

#endif  // for header file
