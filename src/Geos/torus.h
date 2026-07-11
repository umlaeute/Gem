/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2005, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/////////////////////////////////////////////////////////
// added the gluTorus
/////////////////////////////////////////////////////////

#ifndef _INCLUDE__GEM_GEOS_TORUS_H_
#define _INCLUDE__GEM_GEOS_TORUS_H_

#include "Base/GemGluObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
        torus

    Creates a torus

KEYWORD
    geo

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN torus : public GemGluObj
{
  CPPEXTERN_HEADER(torus, GemGluObj);

public:

  //////////
  // Constructor
  torus(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~torus();

  //////////
  // Inner radius of the torus
  float                   m_innerRadius;

  //////////
  // Set the inner radius
  void                    innerRadius(float radius);

  //////////
  // Do the rendering
  virtual void    renderShape(GemState *state);

};

#endif  // for header file
