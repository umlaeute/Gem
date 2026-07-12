/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

// 1307:forum::für::umläute:2000

#ifndef _INCLUDE__GEM_MANIPS_SPECULARRGB_H_
#define _INCLUDE__GEM_MANIPS_SPECULARRGB_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    specularRGB

    specular a gem object

DESCRIPTION

    Inlet for R - "rVal"
    Inlet for G - "gVal"
    Inlet for B - "bVal"

-----------------------------------------------------------------*/
class GEM_EXTERN specularRGB : public GemBase
{
  CPPEXTERN_HEADER(specularRGB, GemBase);

public:

  //////////
  // Constructor
  specularRGB(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~specularRGB();

  //////////
  // Turn back on the color material
  virtual void    postrender(GemState *state);

  //////////
  // When rendering occurs
  virtual void    render(GemState *state);

  //////////
  // The translation vector (r, g, b, a)
  float           m_vector[4];

  //////////
  // R value changed
  void            rMess(float val);

  //////////
  // G value changed
  void            gMess(float val);

  //////////
  // B value changed
  void            bMess(float val);

  //////////
  // A value changed
  void            aMess(float val);
};

#endif  // for header file
