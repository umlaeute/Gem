////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 2012, IOhannes m zmölnig and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
////////////////////////////////////////////////////////

#include "Rectangle.h"

/////////////////////////////////////////////////////////
//
// Rectangle
//
namespace gem
{
Rectangle :: Rectangle(void) :
  x1(0.f), y1(0.f), x2(1.f), y2(1.f)
{
}

#define CLAMP01(x) if(x<0.)x=0.;else if(x>1.)x=1.;
Rectangle Rectangle :: createNormalized(float x1, float y1, float x2,
                                        float y2)
{
  Rectangle result;

  float minX=(x1<x2)?x1:x2;
  float maxX=(x1>x2)?x1:x2;
  float minY=(y1<y2)?y1:y2;
  float maxY=(y1>y2)?y1:y2;
  CLAMP01(minX);
  CLAMP01(maxX);
  CLAMP01(minY);
  CLAMP01(maxY);

  result.x1=minX;
  result.x2=maxX;
  result.y1=minY;
  result.y2=maxY;

  return result;
}
};
