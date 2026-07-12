/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEM_RECTANGLE_H_
#define _INCLUDE__GEM_GEM_RECTANGLE_H_

#include "Gem/ExportDef.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  Rectangle

  abstract rectangle class (to be used e.g. as ROI)

  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem
{
class GEM_EXTERN Rectangle
{
public:

  //////////
  // Constructor
  Rectangle(void);
  float x1, y1,  x2, y2;

  static Rectangle createNormalized(float x1, float y1, float x2, float y2);
};
};

#endif  // for header file
