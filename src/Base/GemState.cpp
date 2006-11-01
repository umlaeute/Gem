////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "GemState.h"

#include "Base/GemGL.h"


/////////////////////////////////////////////////////////
//
// GemState
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GemState :: GemState()
		  : dirty(0), inDisplayList(0), lighting(0), smooth(0), texture(0),
                    image(0), texCoords(0), numTexCoords(0), multiTexUnits(0),
                    tickTime(50.f), drawType(0),
                    VertexArray(0), VertexArraySize(0), VertexArrayStride(0),
                    ColorArray(0), HaveColorArray(0),
                    NormalArray(0), HaveNormalArray(0),
                    TexCoordArray(0), HaveTexCoordArray(0)
{
  stackDepth[0]=stackDepth[1]=stackDepth[2]=stackDepth[3]=1; // 1 is the current matrix
}

void GemState :: reset() {
  image = 0;
  VertexArray = 0;
  VertexArraySize = 0;
  ColorArray = 0;
  NormalArray = 0;
  TexCoordArray = 0;
  HaveColorArray = 0;
  HaveNormalArray = 0;
  HaveTexCoordArray = 0;
  drawType = 0;
}
