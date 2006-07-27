/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  The state to pass among GEM objects

  Copyright (c) 1997-2000 Mark Danks. mark@danks.org
  Copyright (c) Günther Geiger. geiger@epy.co.at
  Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef INCLUDE_GEMSTATE_H_
#define INCLUDE_GEMSTATE_H_

#include "Base/GemExportDef.h"
#include "Base/GemGL.h"

struct pixBlock;
class TexCoord;

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  TexCoord
    

  DESCRIPTION
    
  -----------------------------------------------------------------*/
class GEM_EXTERN TexCoord
{
 public:
  TexCoord() : s(0.f), t(0.f) { }
    TexCoord(float s_, float t_) : s(s_), t(t_) { }
      float   	    s;
      float   	    t;
};


/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  GemState
    
  The state to pass among GEM objects

  DESCRIPTION
    
  -----------------------------------------------------------------*/
class GEM_EXTERN GemState
{
 public:

  //////////
  // Has something changed since the last time?
  int                 dirty;

  //////////
  // Are we in a display list creation?
  int 	    	    inDisplayList;
    	
  //////////
  // Lighting on?
  int 	    	    lighting;
    	
  //////////
  // Smooth shading (flat is other type)
  int 	    	    smooth;
    	
  //////////
  // Texture mapping on?
  // 0..off
  // 1..normalized texture
  // 2..rectangle texture
  int 	    	    texture;
    	
  //////////
  // The image to texture map
  pixBlock	        *image;
    	
  //////////
  // Texture coordinates.
  // This can be NULL if there aren't any coordinates
  TexCoord    	    *texCoords;
    	
  //////////
  // The number of TexCoords
  int 	    	    numTexCoords;

  //////////
  // Milliseconds since last frame
  // If in Stereoscopic mode, then it is the same number for both left
  //		and right renderings
  float				tickTime;

  //////////////////
  // the default draw-type (might be overriden within a Geo)
  GLenum				drawType;

  //////////
  // how deep is the current stack /* 4 fields for the 4 stacks */
  int stackDepth[4];
	
  ////////////
  //vertex-array data
  int                 VertexDirty; // the vertex-arrays has changed

  GLfloat				*VertexArray;
  int					VertexArraySize;
  int					VertexArrayStride;

  GLfloat				*ColorArray;
  int		 			HaveColorArray;

  GLfloat				*NormalArray;
  int					HaveNormalArray;

  GLfloat				*TexCoordArray;
  int					HaveTexCoordArray;

  //////////
  // Constructor
  GemState();

  float texCoordX(int num) {
    if (texture && numTexCoords > num)
      return texCoords[num].s;
    else return 0.;
  }

  float texCoordY(int num) {
    if (texture && numTexCoords > num)
      return texCoords[num].t;
    else return 0.;
  }

  /* reset (parts of?) the GemState: to be called from [gemhead] */
  void reset();

};
    	
#endif	// for header file
