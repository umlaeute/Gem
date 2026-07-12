/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_DIFFUSE_H_
#define _INCLUDE__GEM_MANIPS_DIFFUSE_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    diffuse

    diffuse a gem object

DESCRIPTION

    Inlet for a list - "diffuse"

    "diffuse" - the RGB diffuse to set the object to

-----------------------------------------------------------------*/
class GEM_EXTERN diffuse : public GemBase
{
  CPPEXTERN_HEADER(diffuse, GemBase);

public:

  //////////
  // Constructor
  diffuse(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~diffuse();

  //////////
  // Turn back on the color material
  virtual void    postrender(GemState *state);

  //////////
  // When a gem message is received
  virtual void    render(GemState *state);

  //////////
  // The diffuse vector (RGBA)
  float           m_diffuse[4];

  //////////
  // diffuse changed
  void            diffuseMess(float red, float green, float blue,
                              float alpha);

private:

  //////////
  // static member functions
  static void     diffuseMessCallback(void *data, t_symbol*, int argc,
                                      t_atom *argv);
};

#endif  // for header file
