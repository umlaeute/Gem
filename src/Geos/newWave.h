/*
 *  GEM - Graphics Environment for Multimedia
 *
 *  newWave.h
 *  gem_darwin
 *
 *  Created by Jamie Tittle on Thu Oct 10 2002.
 *  Copyright (c) 2002 tigital. All rights reserved.
 *    For information on usage and redistribution, and for a DISCLAIMER OF ALL
 *    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
 *
 */
#ifndef INCLUDE_NEWWAVE_H_
#define INCLUDE_NEWWAVE_H_

// I hate Microsoft...I shouldn't have to do this!
#ifdef _WINDOWS
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif // __APPLE__

#include "Base/GemShape.h"
#define MAXGRID 63

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    newWave
    
    based on the SGI demo New Wave

KEYWORDS
    geo
    
DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN newWave : public GemShape
{
  CPPEXTERN_HEADER(newWave, GemShape)

    public:

  //////////
  // Constructor
  newWave( t_floatarg width, t_floatarg height);
    	
 protected:
    	
  //////////
  // Destructor
  virtual ~newWave();

  //////////
  // The height of the object
  short		size, mode, speed;
  void	    	heightMess(float size);
  void		modeMess(float mode);
  void		speedMess(float speed);
  //void		otherMess(float other);        
  //////////
  // Do the rendering
  virtual void 	render(GemState *state);
  
    	//////////
    	// How the object should be drawn
    	virtual void	typeMess(t_symbol *type);
		
    	//////////
    	// The height of the object
        GLfloat	    	m_height;

        //////////
        // The height inlet
        t_inlet         *m_inletH;
        t_inlet		*inletM;
        t_inlet		*inletSp;
        //t_inlet		*inletOt;
  
  //////////
  // getStuff
  void		getforce(void);
  void 		getvelocity(void);
  void 		getposition(void);
  void		getTexCoords(void);
  void		setSize( int value );
  void		setSpeed( float value );

  void copy( float vec0[3], float vec1[3]);
  void sub(float vec0[3], float vec1[3], float vec2[3]);
  void add(float vec0[3], float vec1[3], float vec2[3]);
  void scalDiv(float vec[3], float c);
  void cross(float vec0[3], float vec1[3], float vec2[3]);
  void norm(float vec[3]);
  void set(float vec[3], float x, float y, float z);
  void reset( int value );
  void setOther( int value );
  
  void getFaceNorms( void );
  void getVertNorms( void );
  void getFaceNormSegs( void );
  
  int 		m_size;
  int		m_speed;
  int		m_blend;
  float		xsize, ysize;
  
  float force[MAXGRID][MAXGRID],
      veloc[MAXGRID][MAXGRID],
      posit[MAXGRID][MAXGRID],
      vertNorms[MAXGRID][MAXGRID][3],
      faceNorms[2][MAXGRID][MAXGRID][3],
      faceNormSegs[2][2][MAXGRID][MAXGRID][3];
  int alreadyInit;
  float texCoords[MAXGRID][MAXGRID][2];

 private:

  //////////
  // static member functions
  static void 	heightMessCallback(void *data, t_floatarg size);
  static void 	modeMessCallback(void *data, t_floatarg mode);
  static void 	speedMessCallback(void *data, t_floatarg speed);
  static void 	blendMessCallback(void *data, t_floatarg size);
};

#endif	// for header file
