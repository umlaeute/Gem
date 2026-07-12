/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_TRANSLATEXYZ_H_
#define _INCLUDE__GEM_MANIPS_TRANSLATEXYZ_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    translateXYZ

    translate a gem object

DESCRIPTION

    Inlet for X - "xVal"
    Inlet for Y - "yVal"
    Inlet for Z - "zVal"

-----------------------------------------------------------------*/
class GEM_EXTERN translateXYZ : public GemBase
{
  CPPEXTERN_HEADER(translateXYZ, GemBase);

public:

  //////////
  // Constructor
  translateXYZ(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~translateXYZ();

  //////////
  // When rendering occurs
  virtual void    render(GemState *state);

  //////////
  // The translation vector (x, y, z)
  float           m_vector[3];

  //////////
  // X value changed
  void            xMess(float val);

  //////////
  // Y value changed
  void            yMess(float val);

  //////////
  // Z value changed
  void            zMess(float val);
};

#endif  // for header file
