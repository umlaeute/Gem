/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_EMISSION_H_
#define _INCLUDE__GEM_MANIPS_EMISSION_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    emission

    emission a gem object

DESCRIPTION

    Inlet for a list - "emission"

    "emission" - the RGB emission to set the object to

-----------------------------------------------------------------*/
class GEM_EXTERN emission : public GemBase
{
  CPPEXTERN_HEADER(emission, GemBase);

public:

  //////////
  // Constructor
  emission(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~emission();

  //////////
  // Turn back on the color material
  virtual void    postrender(GemState *state);

  //////////
  // When a gem message is received
  virtual void    render(GemState *state);

  //////////
  // The emission vector (RGBA)
  float           m_emission[4];

  //////////
  // emission changed
  void            emissionMess(float red, float green, float blue,
                               float alpha);

private:

  //////////
  // static member functions
  static void     emissionMessCallback(void *data, t_symbol*, int argc,
                                       t_atom *argv);
};

#endif  // for header file
