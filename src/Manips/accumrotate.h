/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_ACCUMROTATE_H_
#define _INCLUDE__GEM_MANIPS_ACCUMROTATE_H_

#include "Base/GemBase.h"
#include "Utils/Matrix.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
        accumrotate

        accumulate a rotation

DESCRIPTION

    Inlet for X - "xVal"
    Inlet for Y - "yVal"
    Inlet for Z - "zVal"

    "reset" - set the matrix to identity

-----------------------------------------------------------------*/
class GEM_EXTERN accumrotate : public GemBase
{
  CPPEXTERN_HEADER(accumrotate, GemBase);

public:

  //////////
  // Constructor
  accumrotate(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~accumrotate();

  //////////
  // When a gem message is received
  virtual void    render(GemState *state);

  //////////
  // The rotation matrix
  Matrix                  m_rotMatrix;

  //////////
  // X value changed
  void            xMess(float val);

  //////////
  // Y value changed
  void            yMess(float val);

  //////////
  // Z value changed
  void            zMess(float val);

  //////////
  // A reset was received
  void            reset();
};

#endif  // for header file
