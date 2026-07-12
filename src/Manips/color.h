/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_COLOR_H_
#define _INCLUDE__GEM_MANIPS_COLOR_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    color

    color a gem object

DESCRIPTION

    Inlet for a list - "color"

    "color" - the RGB color to set the object to

-----------------------------------------------------------------*/
class GEM_EXTERN color : public GemBase
{
  CPPEXTERN_HEADER(color, GemBase);

public:

  //////////
  // Constructor
  color(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~color();

  //////////
  // When a gem message is received
  virtual void    render(GemState *state);

  //////////
  // The color vector (RGBA)
  float           m_color[4];

  //////////
  // Color changed
  void            colorMess(float red, float green, float blue, float alpha);

private:

  //////////
  // static member functions
  static void     colorMessCallback(void *data, t_symbol*, int argc,
                                    t_atom *argv);
};

#endif  // for header file
