/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A vertex_resize

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) G¸nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f¸r::uml‰ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_VERTEX_RESIZE_H_
#define INCLUDE_VERTEX_RESIZE_H_

#include "Base/GemVertex.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    vertex_resize
    
    Creates a vertex_resize

KEYWORDS
    geo
    
DESCRIPTION
    
-----------------------------------------------------------------*/
class GEM_EXTERN vertex_resize : public GemVertex
{
  CPPEXTERN_HEADER(vertex_resize, GemVertex)

    public:

  //////////
  // Constructor
  vertex_resize(t_floatarg);
    	
 protected:
    	
  //////////
  // Destructor
  virtual ~vertex_resize();

  ////////////
  // inlet for the new size
  t_inlet*m_sizeIn;


  //////////
  // manipulation parameters (e.g: "offset")
  virtual void sizeMess(int);
  int	m_size;

  //////////
  // the resized arrays
  GLfloat *m_vertexArray, *m_colorArray, *m_normalArray, *m_texCoordArray;

  GLfloat *m_vertexCache, *m_colorCache, *m_normalCache, *m_texCoordCache;
  int m_sizeCache;

  //////////
  // Do the rendering

  // do the actual processing (subclasses might want to override this)
  virtual void  vertexProcess(int,GLfloat *, int,GLfloat*);

  // call vertexProcess with the apropriate array
  virtual void 	render(GemState *state);

  // revert the State
  virtual void 	postrender(GemState *state);

 private:
        
  static void 	sizeMessCallback(void *data, t_floatarg);
};

#endif	// for header file
