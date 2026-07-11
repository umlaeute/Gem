/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2010, Cyrille Henry and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_MESH_LINE_H_
#define _INCLUDE__GEM_GEOS_MESH_LINE_H_

#include "Base/GemShape.h"
#include <vector>

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    mesh_line

    Creates a mesh_line

KEYWORDS
    geo

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN mesh_line : public GemShape
{
  CPPEXTERN_HEADER(mesh_line, GemShape);

public:

  //////////
  // Constructor
  mesh_line(t_floatarg sizeX);

protected:

  //////////
  // Destructor
  virtual ~mesh_line(void);


  //////////
  // Do the rendering
  virtual void   renderShape(GemState *state);

  void    setGrid( int valueX);
  void    getTexCoords(void);

  //////////
  // getStuff
  int     gridX;
  float    xsize, xsize0;
  int     alreadyInit;
  std::vector<float>    texCoords;
};

#endif  // for header file
