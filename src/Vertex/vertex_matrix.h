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

#ifndef INCLUDE_VERTEX_MATRIX_H_
#define INCLUDE_VERTEX_MATRIX_H_

#include "vertex_scale.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    vertex_matrix
    
    Creates a vertex_matrix

KEYWORDS
    geo
    
DESCRIPTION
    
-----------------------------------------------------------------*/
class GEM_EXTERN vertex_matrix : public vertex_scale
{
  CPPEXTERN_HEADER(vertex_matrix, vertex_scale)

    public:

  //////////
  // Constructor
  vertex_matrix(int, t_atom*);
    	
 protected:
    	
  //////////
  // Destructor
  virtual ~vertex_matrix();

  virtual void paramMess(int,t_atom*);
  GLfloat	m_00,m_01,m_02,m_03;
  GLfloat	m_10,m_11,m_12,m_13;
  GLfloat	m_20,m_21,m_22,m_23;
  GLfloat	m_30,m_31,m_32,m_33;

  
  //////////
  // Do the rendering
  virtual void  vertexProcess(int,GLfloat *);

};

#endif	// for header file
