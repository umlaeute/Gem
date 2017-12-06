/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  an abstract rectangle representation

  Copyright (c) 1997-1999 Mark Danks. mark@danks.org
  Copyright (c) Günther Geiger. geiger@epy.co.at
  Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

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
