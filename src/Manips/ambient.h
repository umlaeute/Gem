/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2000, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_AMBIENT_H_
#define _INCLUDE__GEM_MANIPS_AMBIENT_H_

#include "Base/GemBase.h"
#include "Gem/GemGL.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    ambient

    ambient a gem object

DESCRIPTION

    Inlet for a list - "ambient"

    "ambient" - the RGB ambient to set the object to

-----------------------------------------------------------------*/
class GEM_EXTERN ambient : public GemBase
{
  CPPEXTERN_HEADER(ambient, GemBase);

public:

  //////////
  // Constructor
  ambient(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~ambient();

  //////////
  // Turn back on the color material
  virtual void    postrender(GemState *state);

  //////////
  // When a gem message is received
  virtual void    render(GemState *state);

  //////////
  // The ambient vector (RGBA)
  float           m_ambient[4];

  //////////
  // ambient changed
  void            ambientMess(float red, float green, float blue,
                              float alpha);

private:

  //////////
  // static member functions
  static void     ambientMessCallback(void *data, t_symbol*, int argc,
                                      t_atom *argv);
};

#endif  // for header file
