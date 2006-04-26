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
    		image(0), texCoords(0), numTexCoords(0),
		    numVertexColors(0), stereo(0), tickTime(50.f)
{
  stackDepth[0]=stackDepth[1]=stackDepth[2]=stackDepth[3]=1; // 1 is the current matrix
 }

void GemState::setColor(int num)
{
  if (numVertexColors > num)
    glColor3fv(vertexColors[num]); 
}
