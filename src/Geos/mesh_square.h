/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A mesh_square

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

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
