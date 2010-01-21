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

class GemStateData;
class GEM_EXTERN GemState
{
 public:

  //////////
  // Has something changed since the last time?
  // deprecated: use property 'dirty' instead
  GEM_DEPRECATED  int                 dirty;

  //////////
  // Are we in a display list creation?
  // deprecated: use property '' instead
  GEM_DEPRECATED  int 	    	    inDisplayList;
    	
  //////////
  // Lighting on?
  // deprecated: use property '' instead
  GEM_DEPRECATED  int 	    	    lighting;
    	
  //////////
  // Smooth shading (flat is other type)
  // deprecated: use property '' instead
  GEM_DEPRECATED  int 	    	    smooth;
    	
  //////////
  // Texture mapping on?
  // 0..off
  // 1..normalized texture
  // 2..rectangle texture
  // deprecated: use property '' instead
  GEM_DEPRECATED  int 	    	    texture;
    	
  //////////
  // The image to texture map
  // deprecated: use property '' instead
  GEM_DEPRECATED  pixBlock	        *image;
    	
  //////////
  // Texture coordinates.
  // This can be NULL if there aren't any coordinates
  // deprecated: use property '' instead
  GEM_DEPRECATED  TexCoord    	    *texCoords;
    	
  //////////
  // The number of TexCoords
  // deprecated: use property '' instead
  GEM_DEPRECATED  int 	    	    numTexCoords;
  
  //////////
  // The number of multiTexUnits
  //   default = 0, max = 7
  // deprecated: use property '' instead
  GEM_DEPRECATED  int 	    	    multiTexUnits;

  //////////
  // Milliseconds since last frame
  // If in Stereoscopic mode, then it is the same number for both left
  //		and right renderings
  // deprecated: use property '' instead
  GEM_DEPRECATED  float				tickTime;

  //////////////////
  // the default draw-type (might be overriden within a Geo)
  // deprecated: use property '' instead
  GEM_DEPRECATED  GLenum				drawType;

  //////////
  // how deep is the current stack /* 4 fields for the 4 stacks */
  // deprecated: use property '' instead
  GEM_DEPRECATED  int stackDepth[4];
	
  ////////////
  //vertex-array data
  // deprecated: use property '' instead
  GEM_DEPRECATED  int                 VertexDirty; // the vertex-arrays has changed
  // deprecated: use property '' instead
  GEM_DEPRECATED  GLfloat				*VertexArray;
  // deprecated: use property '' instead
  GEM_DEPRECATED  int					VertexArraySize;
  // deprecated: use property '' instead
  GEM_DEPRECATED  int					VertexArrayStride;

  // deprecated: use property '' instead
  GEM_DEPRECATED  GLfloat				*ColorArray;
  // deprecated: use property '' instead
  GEM_DEPRECATED  int		 			HaveColorArray;

  // deprecated: use property '' instead
  GEM_DEPRECATED  GLfloat				*NormalArray;
  // deprecated: use property '' instead
  GEM_DEPRECATED  int					HaveNormalArray;

  // deprecated: use property '' instead
  GEM_DEPRECATED  GLfloat				*TexCoordArray;
  // deprecated: use property '' instead
  GEM_DEPRECATED  int					HaveTexCoordArray;

  //////////
  // Constructor
  GemState();

  //////////
  // Destructor
  ~GemState();

  float texCoordX(int num) const;

  float texCoordY(int num) const;

  /* reset (parts of?) the GemState: to be called from [gemhead] */
  void reset(void);


  /* get a named property */
  /* if the property exists (as the given type), 
   * the value of the 2nd argument is set accordingly and <code>true</code> is returned
   * if the key does not exist (or the type is wrong) the value is not touched and <code>false</code> is returned instead
   */
  virtual bool get(const char*key, long&value);
  virtual bool get(const char*key, double&value);
  virtual bool get(const char*key, char*&value);
  /* getter of a pointer to a (shared) shallow copy */
  virtual bool get(const char*key, void*&value, size_t&length);
  /* raw accessor; you have to make sure that you cast it to the correct type
   * use at your own risk
   */
  virtual bool get(const char*key, void*&value);

  /* set a named property */
  virtual bool set(const char*key, const long value);
  virtual bool set(const char*key, const double value);
  virtual bool set(const char*key, const char*value);

  /* the data will be saved in a shallow copy */
  virtual bool set(const char*key, const void*value, const size_t length);

  /* raw pointer setter */
  virtual bool set(const char*key, const void*value);


  /* remove a named property */
  virtual bool remove(const char*key);

 protected:
  GemStateData*data;
};
    	
#endif	// for header file
