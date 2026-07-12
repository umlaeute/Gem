/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2000, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_SPECULAR_H_
#define _INCLUDE__GEM_MANIPS_SPECULAR_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    specular

    specular a gem object

DESCRIPTION

    Inlet for a list - "specular"

    "specular" - the RGB specular to set the object to

-----------------------------------------------------------------*/
class GEM_EXTERN specular : public GemBase
{
  CPPEXTERN_HEADER(specular, GemBase);

public:

  //////////
  // Constructor
  specular(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~specular();

  //////////
  // Turn back on the color material
  virtual void    postrender(GemState *state);

  //////////
  // When a gem message is received
  virtual void    render(GemState *state);

  //////////
  // The specular vector (RGBA)
  float           m_specular[4];

  //////////
  // specular changed
  void            specularMess(float red, float green, float blue,
                               float alpha);

private:

  //////////
  // static member functions
  static void     specularMessCallback(void *data, t_symbol*, int argc,
                                       t_atom *argv);
};

#endif  // for header file
