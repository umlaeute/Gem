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

#include "Gem/RTE.h"
#include "Gem/any.h"

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
  GEM_DEPRECATED  bool                 dirty;

  //////////
  // Are we in a display list creation?
  // deprecated: use property 'gl.displaylist' instead
  GEM_DEPRECATED  bool 	    	    inDisplayList;
    	
  //////////
  // Lighting on?
  // deprecated: use property 'gl.lighting' instead
  GEM_DEPRECATED  bool 	    	    lighting;
    	
  //////////
  // Smooth shading (flat is other type)
  // deprecated: use property 'gl.smooth' instead
  GEM_DEPRECATED  bool 	    	    smooth;
    	
  //////////
  // Texture mapping on?
  // 0..off
  // 1..normalized texture
  // 2..rectangle texture
  // deprecated: use property 'gl.tex.type' instead
  GEM_DEPRECATED  int 	    	    texture;
    	
  //////////
  // The image to texture map
  // deprecated: use property 'pix' instead
  GEM_DEPRECATED  pixBlock	        *image;
    	
  //////////
  // Texture coordinates.
  // This can be NULL if there aren't any coordinates
  // deprecated: use property 'gl.tex.coords' instead
  GEM_DEPRECATED  TexCoord    	    *texCoords;
    	
  //////////
  // The number of TexCoords
  // deprecated: use property 'gl.tex.numcoords' instead
  GEM_DEPRECATED  int 	    	    numTexCoords;
  
  //////////
  // The number of multiTexUnits
  //   default = 0, max = 7
  // deprecated: use property 'gl.tex.units' instead
  GEM_DEPRECATED  int 	    	    multiTexUnits;

  //////////
  // Milliseconds since last frame
  // If in Stereoscopic mode, then it is the same number for both left
  //		and right renderings
  // deprecated: use property 'timing.tick' instead
  GEM_DEPRECATED  float				tickTime;

  //////////////////
  // the default draw-type (might be overriden within a Geo)
  // deprecated: use property 'gl.drawtype' instead
  GEM_DEPRECATED  GLenum				drawType;

  //////////
  // how deep is the current stack /* 4 fields for the 4 stacks */
  // deprecated: use property 'gl.stacks' instead
  GEM_DEPRECATED  int stackDepth[4];
	
  ////////////
  //vertex-array data
  // deprecated: use property 'vertex.dirty' instead
  GEM_DEPRECATED  int                 VertexDirty; // the vertex-arrays has changed
  // deprecated: use property 'vertex.array.vertex' instead
  GEM_DEPRECATED  GLfloat				*VertexArray;
  // deprecated: use property 'vertex.array.vertex' instead
  GEM_DEPRECATED  int					VertexArraySize;
  // deprecated: use property 'vertex.array.vertex' instead
  GEM_DEPRECATED  int					VertexArrayStride;

  // deprecated: use property 'vertex.array.color' instead
  GEM_DEPRECATED  GLfloat				*ColorArray;
  // deprecated: use property 'vertex.array.color' instead
  GEM_DEPRECATED  bool		 			HaveColorArray;

  // deprecated: use property 'vertex.array.normal' instead
  GEM_DEPRECATED  GLfloat				*NormalArray;
  // deprecated: use property 'vertex.array.normal' instead
  GEM_DEPRECATED  bool					HaveNormalArray;

  // deprecated: use property 'vertex.array.texcoord' instead
  GEM_DEPRECATED  GLfloat				*TexCoordArray;
  // deprecated: use property 'vertex.array.texcoord' instead
  GEM_DEPRECATED  bool					HaveTexCoordArray;

  //////////
  // Constructor
  GemState();

  //////////
  // Destructor
  virtual ~GemState();

  float texCoordX(int num) const;

  float texCoordY(int num) const;

  /* reset (parts of?) the GemState: to be called from [gemhead] */
  void reset(void);


  /* get a named property */
  /* if the property exists (as the given type), 
   * the value of the 2nd argument is set accordingly and <code>true</code> is returned
   * if the key does not exist (or the type is wrong) the value is not touched and <code>false</code> is returned instead
   */
  virtual bool get(const std::string key, gem::any&value);
  virtual bool get(const t_symbol*key, gem::any&value) { return get(key->s_name, value); }

  template<class T>
    bool get(const std::string key, T&value) {
    try {
      gem::any val;
      if(!get(key,val)) {
        // key not found
        return false;
      }
      value=gem::any_cast<T>(val);
      return true;
    } catch (gem::bad_any_cast) {
      // type problem
    }
    return false;
  };
  template<class T>
    bool get(const t_symbol*key, T&value) {
    return get(key->s_name, value);
  }

  /* set a named property */
  virtual bool set(const std::string key, gem::any value);
  virtual bool set(const t_symbol*key, gem::any value) { return set(key->s_name, value); }
  /* remove a named property */
  virtual bool remove(const std::string key);
  virtual bool remove(const t_symbol*key)  { return remove(key->s_name);}

  // Copy assignment
  GemState& operator=(const GemState&);

 protected:
  GemStateData*data;
};
    	
#endif	// for header file
