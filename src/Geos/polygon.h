/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_POLYGON_H_
#define _INCLUDE__GEM_GEOS_POLYGON_H_

#include "Base/GemBase.h"
#include "Base/GemShape.h"


/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    polygon

    Creates a polygon

KEYWORDS
    geo

DESCRIPTION

    Inlet for a list - "vert_1"
    ...
    Inlet for a list - "vert_9"

    "list" - The first vertex
    "vert_1" - The second vertex
    ...
    "vert_9" - The tenth vertex

-----------------------------------------------------------------*/
class GEM_EXTERN polygon : public GemShape
{
  CPPEXTERN_HEADER(polygon, GemShape);

public:

  //////////
  // Constructor
  polygon(t_floatarg numInputs);

protected:

  //////////
  // Destructor
  virtual ~polygon();

  //////////
  // Do the renderShapeing
  virtual void    renderShape(GemState *state);

  //////////
  // Set the vertices
  void            setVert(int whichOne, float x, float y, float z);

  void            listMess(int,t_atom*);

  //-----------------------------------
  // GROUP:   Member variables
  //-----------------------------------

  //////////
  // The vertices
  void createVertices(int);
  int             m_numVertices;
  float  *m_vertarray;
  float **m_vert;

  //////////
  // The number of inlets (one per vertex)
  int             m_numInputs;
  t_inlet**m_inlet;

private:
  //////////
  // Static member functions
  static void     vertCallback(void *data, t_symbol* type, int argc,
                               t_atom*argv);
  static void     listCallback(void *data, t_symbol* type, int argc,
                               t_atom*argv);
};

#endif  // for header file
