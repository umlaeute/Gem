// clang-format off
/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2005, Pierre-Olivier Charlebois and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */
// clang-format on

#ifndef _INCLUDE__GEM_NONGEOS_SPOT_LIGHT_H_
#define _INCLUDE__GEM_NONGEOS_SPOT_LIGHT_H_

#include "Nongeos/world_light.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  spot_light

  Creates a spot_light - position is at infinity (can be rotated)

  DESCRIPTION

  Inlet for a list - "clrlist"

  "clrlist" - Determines color
  "int" - On/off state

  -----------------------------------------------------------------*/
class GEM_EXTERN spot_light : public world_light
{
  CPPEXTERN_HEADER(spot_light, world_light);

public:

  //////////
  // Constructor
  spot_light(t_floatarg lightNum);

protected:

  //////////
  // Destructor
  virtual ~spot_light();

  //////////
  // Do the rendering
  virtual void  render(GemState *state);

  virtual void  renderDebug();


  //////////
  // Set the light's parameters (linear attunation, cone angle, decay exponent
  void          lightParamMess(float linAtt, float cutoff, float exponent);

  //-----------------------------------
  // GROUP:     Member variables
  //-----------------------------------

  // global lighting parameters
  GLfloat constantAttenuation;
  GLfloat linearAttenuation;
  GLfloat quadraticAttenuation;
  GLfloat spotCutoff;
  GLfloat spotExponent;

  GLfloat spotDirection[3];
};



// Define constant position and direction

#endif  // for header file
