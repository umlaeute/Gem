/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2007, Cyrille Henry and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_MESH_SQUARE_H_
#define _INCLUDE__GEM_GEOS_MESH_SQUARE_H_

#include "Base/GemShape.h"
#include <vector>


/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    mesh_square

    Creates a mesh_square

KEYWORDS
    geo

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN mesh_square : public GemShape
{
  CPPEXTERN_HEADER(mesh_square, GemShape);

public:

  //////////
  // Constructor
  mesh_square(t_floatarg sizeX,t_floatarg sizeY);

protected:

  //////////
  // Destructor
  virtual ~mesh_square(void);


  //////////
  // Do the rendering
  virtual void    renderShape(GemState *state);

  void    setSize( int valueX, int valueY );
  void    setSize( int valueXY );
  void    setGridX(int valX);
  void    setGridY(int valY);
  void    getTexCoords(void);

  //////////
  // getStuff
  int             gridX, gridY;
  float           xsize, xsize0, ysize, ysize0;
  int             alreadyInit;

  std::vector<std::vector<std::vector<float> > >          texCoords;
};

#endif  // for header file
