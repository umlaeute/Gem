/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A curve3d

    Copyright (c) 1997-1998 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

    -----------------------------------------------------------------*/

#ifndef INCLUDE_curve3d_H_
#define INCLUDE_curve3d_H_

#include "Base/GemShape.h"


/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  curve3d
    
  Creates a curve3d

  KEYWORDS
  geo
    
  DESCRIPTION
    
  -----------------------------------------------------------------*/
class GEM_EXTERN curve3d : public GemShape
{
  CPPEXTERN_HEADER(curve3d, GemShape)

    public:

  //////////
  // Constructor
  curve3d(t_floatarg size_X, t_floatarg size_Y);
    	
 protected:
    	
  //////////
  // Destructor
  virtual ~curve3d();

  //////////
  // Do the rendering
  virtual void 	render(GemState *state);
		
  //////////
  static const int NUM_PNTS_MAX;
  static const int NUM_PNTS_CONTROL_MAX;


  int nb_pts_control_X;
  int nb_pts_control_Y;
  int nb_pts_affich_X;
  int nb_pts_affich_Y;
  void resolutionMess(int resX, int resY);
  void gridMess(int gridX, int gridY);
  void setMess(int X,int Y,float posX, float posY,float posZ);

  //////////
  // Vertex vectors
  float			mVectors[3][3];
  
  virtual void	typeMess(t_symbol *type);
 
  GLenum	    	m_drawType;
		
  float			posXYZ[2500][3]; // attention, valeur critique

 private:
  static void		resolutionMessCallback(void *data, t_floatarg resX, t_floatarg resY );
  static void		gridMessCallback(void *data, t_floatarg gridX, t_floatarg gridY );
  static void		setMessCallback(void *data, t_floatarg X,t_floatarg Y,t_floatarg posX,t_floatarg posY,t_floatarg posZ);
  static void 	typeMessCallback(void *data, t_symbol *type);
};

#endif	// for header file
