/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A vertex_scale

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) G¸nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f¸r::uml‰ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_VERTEX_SCALE_H_
#define INCLUDE_VERTEX_SCALE_H_

#include "Base/GemVertex.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    vertex_scale
    
    Creates a vertex_scale

KEYWORDS
    geo
    
DESCRIPTION
    
-----------------------------------------------------------------*/
class GEM_EXTERN vertex_scale : public GemVertex
{
  CPPEXTERN_HEADER(vertex_scale, GemVertex)

    public:

  //////////
  // Constructor
  vertex_scale(int, t_atom*);
    	
 protected:
    	
  //////////
  // Destructor
  virtual ~vertex_scale();

  //////////
  // manipulation parameters (e.g: "offset")
  virtual void paramMess(int,t_atom*);
  float	m_x,m_y,m_z,m_w;
  
  //////////
  // which vertices to manipulate
  virtual void vertexMess(int offset, int count);
  int  m_offset,m_count;

  ////////////
  // which array to manipulate
  virtual void typeMess(int,t_atom*);
  bool m_vertex, m_color, m_normal, m_texture;

  ////////////
  // inlets for the manipulation-parameters and the vertex-range
  t_inlet*m_parmIn, *m_vertIn;
  
  //////////
  // Do the rendering

  // do the actual processing (subclasses might want to override this)
  virtual void  vertexProcess(int,GLfloat *);

  // call vertexProcess with the apropriate array
  virtual void 	render(GemState *state);


  // this is protected because subclasses might want to use it
  static void 	paramMessCallback(void *data, t_symbol*, int, t_atom*);

 private:
        
  static void 	typeMessCallback(void *data, t_symbol*, int, t_atom*);
  static void 	vertexMessCallback(void *data, t_floatarg num, t_floatarg counter);
};

#endif	// for header file
