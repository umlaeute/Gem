/*
 *  GEM - Graphics Environment for Multimedia
 *
 *  ripple.h
 *  gem_darwin
 *
 *  Created by Jamie Tittle on Sun Jan 19 2003.
 *  Copyright (c) 2003 tigital. All rights reserved.
 *    For information on usage and redistribution, and for a DISCLAIMER OF ALL
 *    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
 *
 */
#ifndef INCLUDE_RIPPLE_H_
#define INCLUDE_RIPPLE_H_

// I hate Microsoft...I shouldn't have to do this!
#ifdef _WINDOWS
#include <windows.h>
#endif

#ifdef MACOSX
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif // MACOSX

#include "Base/GemShape.h"

#define RIPPLE_LENGTH     2048
#define RIPPLE_CYCLES     18
#define RIPPLE_AMPLITUDE  0.125
#define RIPPLE_STEP	  7
#define RIPPLE_COUNT	  7

#define GRID_SIZE_X   32
#define GRID_SIZE_Y   32

#define CLIP_NEAR  0.0
#define CLIP_FAR   1000.0

typedef struct {	/* precomputed displacement vector table */
  float dx[2];
  int r;		/* distance from origin, in pixels */
} RIPPLE_VECTOR;

typedef struct {	/* precomputed ripple amplitude table */
  float amplitude;
} RIPPLE_AMP;

typedef struct {
  float x[2];		/* initial vertex location */
  float t[2];		/* texture coordinate */
  float dt[2];		/* default texture coordinate */
} RIPPLE_VERTEX;

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    ripple
    
    based on the SGI demo distort

KEYWORDS
    geo
    
DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN ripple : public GemShape
{
  CPPEXTERN_HEADER(ripple, GemShape)

    public:

  //////////
  // Constructor
  ripple( t_floatarg width, t_floatarg height);
    	
 protected:
    	
  //////////
  // Destructor
  virtual ~ripple();

  //////////
  // The height of the object
  short		size, ctrX, ctrY;
  void	    	heightMess(float height);
  void		ctrXMess(float center);
  void		ctrYMess(float center);
  //void		fovMess(float size);        
  //////////
  // Do the rendering
  virtual void 	render(GemState *state);
  
    	//////////
    	// How the object should be drawn
    	virtual void	typeMess(t_symbol *type);
        void	ripple_dynamics();
        void	ripple_init();
        float	ripple_distance( int gx, int gy, int cx, int cy);
        int	ripple_max_distance( int gx, int gy );
        void	ripple_grab();
        void	ripple_bang();
        void	precalc_ripple_vector();
        void	precalc_ripple_amp();
		
    	//////////
    	// The height of the object
        GLfloat	    	m_height;

        //////////
        // The height inlet
        t_inlet         *m_inletH;
        t_inlet		*inletcX;
        t_inlet		*inletcY;
        t_inlet		*inletfov;
  
  //////////
  // getStuff

  
  int 		m_size;
  int		alreadyInit;
  int		win_size_x;
  int		win_size_y;
  int		xyratio;
  int		m_blend;
  float		xsize, ysize;
  float		fov;
  RIPPLE_VECTOR ripple_vector[GRID_SIZE_X][GRID_SIZE_Y];
  RIPPLE_AMP ripple_amp[RIPPLE_LENGTH];
  RIPPLE_VERTEX ripple_vertex[GRID_SIZE_X][GRID_SIZE_Y];

  int cx[RIPPLE_COUNT];
  int cy[RIPPLE_COUNT];
  int t[RIPPLE_COUNT];
  int max[RIPPLE_COUNT];

  int ripple_max;
  int grab;	/* index of grabbed mass point */

 private:

  //////////
  // static member functions
  static void	bangMessCallback(void *data);
  static void 	heightMessCallback(void *data, t_floatarg height);
  static void 	ctrXMessCallback(void *data, t_floatarg center);
  static void 	ctrYMessCallback(void *data, t_floatarg center);
  static void 	fovMessCallback(void *data, t_floatarg size);
  static void 	blendMessCallback(void *data, t_floatarg size);
};

#endif	// for header file
