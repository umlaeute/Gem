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
#ifndef _INCLUDE__GEM_GEOS_NEWWAVE_H_
#define _INCLUDE__GEM_GEOS_NEWWAVE_H_

#include "Base/GemShape.h"
#include "Utils/Functions.h"

#ifdef __ppc__
#undef sqrt
#define sqrt fast_sqrtf
#endif

#define MAXGRID 600
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
  CPPEXTERN_HEADER(newWave, GemShape);

    public:

  //////////
  // Constructor
  newWave( int, t_atom* );

 protected:

  //////////
  // Destructor
  virtual ~newWave();

  //////////
  // The height of the object
  int		gridX, gridY;
  void	    heightMess(float size);
  void		modeMess(int mode);
  void		forceMess(float posX, float posY, float valforce);
  void		positionMess(float posX, float posY, float posZ);
  void		textureMess(int mode);
  void		bangMess();

  //////////
  // Do the rendering
  virtual void 	renderShape(GemState *state);

  //////////
  // The height of the object
  GLfloat m_height;

  //////////
  // The height&mode inlet
  t_inlet*m_inletH,*m_inletM;

  //////////
  // getStuff
  void	getforce(void);
  void	getvelocity(void);
  void	getposition(void);
  void	savepos(void);
  void	getK(void);
  void	getdamp(void);
  void	noise(float);
  void	getTexCoords(void);
  void	setSize( int valueX, int valueY );

  void 	setK1Mess(float K);
  void 	setD1Mess(float D);
  void 	setK2Mess(float K);
  void 	setD2Mess(float D);
  void 	setK3Mess(float K);
  void 	setD3Mess(float D);

  void	position( float posX, float posY, float posZ );
  void	setforce( float posX, float posY, float valforce);

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

  float	xsize, xsize0, ysize, ysize0;
  float	K1, D1, K2, D2, K3, D3;

  int alreadyInit;
  int m_textureMode; // how to texture...


  float force[MAXGRID][MAXGRID],
      veloc[MAXGRID][MAXGRID],
      posit[MAXGRID][MAXGRID],
      positold[MAXGRID][MAXGRID],
      vertNorms[MAXGRID][MAXGRID][3],
      faceNorms[2][MAXGRID][MAXGRID][3],
      faceNormSegs[2][2][MAXGRID][MAXGRID][3];

  float texCoords[MAXGRID][MAXGRID][2];
};

#endif	// for header file
