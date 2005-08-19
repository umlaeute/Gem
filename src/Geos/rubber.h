/*
 *  GEM - Graphics Environment for Multimedia
 *
 *  rubber.h
 *  gem_darwin
 *
 *  Created by Jamie Tittle on Sun Jan 19 2003.
 *  Copyright (c) 2003 tigital. All rights reserved.
 *    For information on usage and redistribution, and for a DISCLAIMER OF ALL
 *    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
 *
 */
#ifndef INCLUDE_RUBBER_H_
#define INCLUDE_RUBBER_H_

#include "Base/GemShape.h"
#include "Base/GemState.h"
#include "Base/GemMan.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifdef __ppc__
#include "Base/GemFuncUtil.h"
#undef sqrt
#define sqrt fast_sqrtf
#endif

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    rubber
    
    based on the SGI demo distort

KEYWORDS
    geo
    
DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN rubber : public GemShape
{
  CPPEXTERN_HEADER(rubber, GemShape)

    public:

  //////////
  // Constructor
  rubber( t_floatarg width, t_floatarg height);
    	
 protected:
    	
  //////////
  // Destructor
  virtual ~rubber();

  //////////
  // The height of the object
  float		ctrX, ctrY;
  void	    	heightMess(float height);
  void		ctrXMess(float center);
  void		ctrYMess(float center);
  //void		otherMess(float other);        
  //////////
  // Do the rendering
  virtual void 	render(GemState *state);
  
  //////////
  // How the object should be drawn
  virtual void	typeMess(t_symbol *type);
        
  virtual void	rubber_init();
  virtual void	rubber_dynamics();
  virtual void	rubber_bang();
  virtual int	rubber_grab();
		
  //////////
  // The height of the object
  GLfloat	    	m_height;

  //////////
  // The height inlet
  t_inlet         *m_inletH;
  t_inlet		*inletcX;
  t_inlet		*inletcY;
  //t_inlet		*inletOt;
  
  //////////
  // Stuff
  
  int		m_speed;
  int		m_blend;
  int		alreadyInit;
  float 	xsize, ysize, ysize0;

  // number of grid-segments in X/Y direction (defaults: 32);
  int           m_grid_sizeX,m_grid_sizeY;
    
 private:

  //////////
  // static member functions
  static void	bangMessCallback(void *data);
  static void 	heightMessCallback(void *data, t_floatarg height);
  static void 	ctrXMessCallback(void *data, t_floatarg center);
  static void 	ctrYMessCallback(void *data, t_floatarg center);
  static void 	blendMessCallback(void *data, t_floatarg blend);
};

#endif	// for header file
