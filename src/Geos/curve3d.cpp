////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Curve3D : create a besier surface
// made by Cyrille Henry
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "curve3d.h"

#include "Base/GemState.h"
#include "Base/Matrix.h"
#include "Base/GemFuncUtil.h"
#include <string.h>

CPPEXTERN_NEW_WITH_TWO_ARGS(curve3d, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// curve3d
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
curve3d :: curve3d(t_floatarg sizeX,t_floatarg sizeY )
  : GemShape(1),
    nb_pts_control_X(2), nb_pts_control_Y(2),
    nb_pts_affich_X (30), nb_pts_affich_Y (30),
    m_drawType(1),
    m_posXYZ(NULL)
{
  int i, j, a;

  nb_pts_control_X = MAX(2, (int)sizeX);
  nb_pts_control_Y = MAX(2, (int)sizeY);

  m_posXYZ = new t_float3[nb_pts_control_X*nb_pts_control_Y];

  if(!m_posXYZ){
    nb_pts_control_X=0;
    nb_pts_control_Y=0;
  }

  for (i=0; i < nb_pts_control_X; i++)
    for (j=0; j < nb_pts_control_Y; j++) {
      a= i + j * nb_pts_control_X;
      m_posXYZ[a].x= (float)i/nb_pts_control_X;
      m_posXYZ[a].y= (float)j/nb_pts_control_Y;
      m_posXYZ[a].z= 0.0;
    }

}

//////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
curve3d :: ~curve3d()
{ 
  if(m_posXYZ)delete[]m_posXYZ;
}

//////////////////////////////////////////////////////////
// setMess
//
/////////////////////////////////////////////////////////
void curve3d :: setMess(int X, int Y, float posX, float posY, float posZ){
  if ((X>=0)&(X<nb_pts_control_X)&(Y>=0)&(Y<nb_pts_control_Y))    {
    m_posXYZ[X+Y*nb_pts_control_X].x=posX;
    m_posXYZ[X+Y*nb_pts_control_X].y=posY;
    m_posXYZ[X+Y*nb_pts_control_X].z=posZ;
    setModified();
  }
}

//////////////////////////////////////////////////////////
// resolutionMess
//
/////////////////////////////////////////////////////////
void curve3d :: resolutionMess(int resolutionX, int resolutionY){
  int x=0;
  int y=0;

  nb_pts_control_X = (resolutionX < 2) ? 2 : resolutionX;
  nb_pts_control_Y = (resolutionY < 2) ? 2 : resolutionY;
  if(m_posXYZ)delete[]m_posXYZ;
  m_posXYZ = new t_float3[nb_pts_control_X*nb_pts_control_Y];
  if(!m_posXYZ){
    nb_pts_control_X=0;
    nb_pts_control_Y=0;
  }
  for (x=0; x < nb_pts_control_X; x++)
    for (y=0; y < nb_pts_control_Y; y++) {
      int a= x + y * nb_pts_control_X;
      m_posXYZ[a].x= (float)x/nb_pts_control_X;
      m_posXYZ[a].y= (float)y/nb_pts_control_Y;
      m_posXYZ[a].z= 0.0;
    }
  
  setModified();
}

//////////////////////////////////////////////////////////
// gridMess
//
/////////////////////////////////////////////////////////
void curve3d :: gridMess(int gridX, int gridY){
  nb_pts_affich_X = (gridX < 2) ? 2 : gridX;
  nb_pts_affich_Y = (gridY < 2) ? 2 : gridY;
  setModified();
}

//////////////////////////////////////////////////////////
// typeMess
//
/////////////////////////////////////////////////////////
void curve3d :: typeMess(t_symbol *type){
  if (!strcmp(type->s_name, "line")) 
    m_drawType = 0;
  else if (!strcmp(type->s_name, "fill")) 
    m_drawType = 1;
  else if (!strcmp(type->s_name, "point"))
    m_drawType = 2;
  else if (!strcmp(type->s_name, "line1"))
    m_drawType = 3;
  else if (!strcmp(type->s_name, "line2"))
    m_drawType = 4;
  else if (!strcmp(type->s_name, "line3"))
    m_drawType = 5;
  else if (!strcmp(type->s_name, "line4"))
    m_drawType = 6; 
  else if (!strcmp(type->s_name, "control_fill"))
    m_drawType = 7; 
  else if (!strcmp(type->s_name, "control_point"))
    m_drawType = 8; 
  else if (!strcmp(type->s_name, "control_line"))
    m_drawType = 9; 
  else if (!strcmp(type->s_name, "control_line1"))
    m_drawType = 10; 
  else if (!strcmp(type->s_name, "control_line2"))
    m_drawType = 11; 
  else if (!strcmp(type->s_name, "default"))
    m_drawType = 1;
  else    {
    error ("GEM : invalid draw style for curve3d?");
    return;
  }
  setModified();
}

//////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void curve3d :: render(GemState *state){
  if(m_drawType==GL_DEFAULT_GEM)m_drawType=1;
  float norm[3];
  int n, m;

  glNormal3f(0.0f, 0.0f, 1.0f);
  glLineWidth(m_linewidth);
  glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, (GLint)nb_pts_control_X, 0,1,(GLint)(3*nb_pts_control_X),(GLint)nb_pts_control_Y, &(m_posXYZ[0].x));
  glEnable(GL_MAP2_VERTEX_3);

  GLfloat xsize = 1.0f;
  GLfloat ysize = 1.0f;
  GLfloat ysize0= 0.0f;

  if (state->texture && state->numTexCoords>=3)
    {
      xsize  = state->texCoords[1].s;
      ysize0 = state->texCoords[2].t;
      ysize  = state->texCoords[1].t;
    }
  GLfloat ysizediff = ysize0 - ysize;

  switch (m_drawType){
    //line
  case 0:    { 
    if (state->texture)	{
      for (n = 0; n < nb_pts_affich_X+1; n++)   {
	glBegin(GL_LINE_STRIP);
	for (m = 0; m  < nb_pts_affich_Y+1; m++){	
	  glTexCoord2f(xsize*(GLfloat)n/(float)nb_pts_affich_X, ysize+ysizediff*(GLfloat)m/(float)nb_pts_affich_Y);
	  glEvalCoord2f((GLfloat)n/(float)nb_pts_affich_X, (GLfloat)m/(float)nb_pts_affich_Y);
	}
	glEnd();
      }
      for(m = 0; m < nb_pts_affich_Y+1; m++) {
	glBegin(GL_LINE_STRIP);
	for(n = 0; n  < nb_pts_affich_X+1; n++){
	  glTexCoord2f(xsize*(GLfloat)n/(float)nb_pts_affich_X, ysize+ysizediff*(GLfloat)m/(float)nb_pts_affich_Y);
	  glEvalCoord2f((GLfloat)n/(float)nb_pts_affich_X, (GLfloat)m/(float)nb_pts_affich_Y);
	}
	glEnd();
      }
    }  else {
	  for(n = 0; n < nb_pts_affich_X+1; n++)  {
	    glBegin(GL_LINE_STRIP);
	    for(m = 0; m  < nb_pts_affich_Y+1; m++){	
	      glEvalCoord2f((GLfloat)n/(float)nb_pts_affich_X, (GLfloat)m/(float)nb_pts_affich_Y);
	    }
	    glEnd();
	  }
	  for(m = 0; m < nb_pts_affich_Y+1; m++)  {
	    glBegin(GL_LINE_STRIP);
	    for(n = 0; n  < nb_pts_affich_X+1; n++){	
	      glEvalCoord2f((GLfloat)n/(float)nb_pts_affich_X, (GLfloat)m/(float)nb_pts_affich_Y);
	    }
	    glEnd();
	  }
    }
  }
    break;
    // fill 
  case 1:
    {
      if (state->texture)
	for(n = 0; n < nb_pts_affich_X; n++) {
	  glBegin(GL_TRIANGLE_STRIP);
	  for(m = 0; m  < nb_pts_affich_Y+1; m++)   {	
	    glTexCoord2f(xsize*(GLfloat)n/(float)nb_pts_affich_X, ysize+ysizediff*(GLfloat)m/(float)nb_pts_affich_Y);
	    glEvalCoord2f((GLfloat)n/(float)nb_pts_affich_X, (GLfloat)m/(float)nb_pts_affich_Y);
	    glTexCoord2f(xsize*(GLfloat)(n+1)/(float)nb_pts_affich_X, ysize+ysizediff*(GLfloat)m/(float)nb_pts_affich_Y);
	    glEvalCoord2f((GLfloat)(n+1)/(float)nb_pts_affich_X, (GLfloat)m/(float)nb_pts_affich_Y);
	  }
	  glEnd();
	}
      else
	for(n = 0; n < nb_pts_affich_X; n++) {
	  glBegin(GL_TRIANGLE_STRIP);
	  for(m = 0; m  < nb_pts_affich_Y+1; m++)  {	
	    glEvalCoord2f((GLfloat)n/(float)nb_pts_affich_X, (GLfloat)m/(float)nb_pts_affich_Y);
	    glEvalCoord2f((GLfloat)(n+1)/(float)nb_pts_affich_X, (GLfloat)m/(float)nb_pts_affich_Y);
	  }
	  glEnd();
	}
    }
    break;
  case 2:
    //point
    {
      glBegin(GL_POINTS);
      if (state->texture)
	for(n = 0; n < nb_pts_affich_X+1; n++) {
	  for(m = 0; m  < nb_pts_affich_Y+1; m++) {
	    glTexCoord2f(xsize*(GLfloat)n/(float)nb_pts_affich_X, ysize+ysizediff*(GLfloat)m/(float)nb_pts_affich_Y);
	    glEvalCoord2f((GLfloat)n/(float)nb_pts_affich_X, (GLfloat)m/(float)nb_pts_affich_Y);
	  }
	}
      else
	for(n = 0; n < nb_pts_affich_X+1; n++) {
	  for(m = 0; m  < nb_pts_affich_Y+1; m++)
	    glEvalCoord2f((GLfloat)n/(float)nb_pts_affich_X, (GLfloat)m/(float)nb_pts_affich_Y);
	}
      glEnd();
    }
    break;

  case 3:
    // line1
    {
      if (state->texture)
	for(n = 0; n < nb_pts_affich_X; n++) {
	  glBegin(GL_LINE_STRIP);
	  for(m = 0; m  < nb_pts_affich_Y; m++)  {	
	    glTexCoord2f(xsize*(GLfloat)n/(float)nb_pts_affich_X, ysize+ysizediff*(GLfloat)m/(float)nb_pts_affich_Y);
	    glEvalCoord2f((GLfloat)n/(float)nb_pts_affich_X, (GLfloat)m/(float)nb_pts_affich_Y);
	  }
	  glEnd();
	}
      else
	for(n = 0; n < nb_pts_affich_X; n++) {
	  glBegin(GL_LINE_STRIP);
	  for(m = 0; m  < nb_pts_affich_Y; m++)   {	
	    glEvalCoord2f((GLfloat)n/(float)nb_pts_affich_X, (GLfloat)m/(float)nb_pts_affich_Y);	
	  }
	  glEnd();
	}
    }
    break;

  case 4:
    //line2
    {
      if (state->texture)
	for(m = 0; m < nb_pts_affich_Y+1; m++) {
	  glBegin(GL_LINE_STRIP);
	  for(n = 0; n  < nb_pts_affich_X+1; n++)  {	
	    glTexCoord2f(xsize*(GLfloat)n/(float)nb_pts_affich_X,ysize+ysizediff*(GLfloat)m/(float)nb_pts_affich_Y);
	    glEvalCoord2f((GLfloat)n/(float)nb_pts_affich_X, (GLfloat)m/(float)nb_pts_affich_Y);
	  }
	  glEnd();
	}
      else
	for(m = 0; m < nb_pts_affich_Y+1; m++) {
	  glBegin(GL_LINE_STRIP);
	  for(n = 0; n  < nb_pts_affich_X+1; n++)  {	
	    glTexCoord2f(xsize*(GLfloat)n/(float)nb_pts_affich_X, ysize+ysizediff*(GLfloat)m/(float)nb_pts_affich_Y);
	    glEvalCoord2f((GLfloat)n/(float)nb_pts_affich_X, (GLfloat)m/(float)nb_pts_affich_Y);
	  }
	  glEnd();
	}
    }
    break;

  case 5:
    //line3
    {
      if (state->texture)
	for(n = 0; n < nb_pts_affich_X; n++) {
	  glBegin(GL_LINES);
	  for(m = 0; m  < nb_pts_affich_Y; m++)
	    {
	      glTexCoord2f(xsize*(GLfloat)n/(float)nb_pts_affich_X, ysize+ysizediff*(GLfloat)m/(float)nb_pts_affich_Y);
	      glEvalCoord2f((GLfloat)n/(float)nb_pts_affich_X, (GLfloat)m/(float)nb_pts_affich_Y);
	    }
	  glEnd();
	}
      else
	for(n = 0; n < nb_pts_affich_X; n++) {
	  glBegin(GL_LINES);
	  for(m = 0; m  < nb_pts_affich_Y; m++)  {	
	    glEvalCoord2f((GLfloat)n/(float)nb_pts_affich_X, (GLfloat)m/(float)nb_pts_affich_Y);	
	  }
	  glEnd();
	}
    }
    break;

  case 6:
    //line4
    {
      if (state->texture)
	for(m = 0; m < nb_pts_affich_Y; m++)
	  {
	    glBegin(GL_LINES);
	    for(n = 0; n  < nb_pts_affich_X; n++) {	
	      glTexCoord2f(xsize*(GLfloat)n/(float)nb_pts_affich_X, ysize+ysizediff*(GLfloat)m/(float)nb_pts_affich_Y);
	      glEvalCoord2f((GLfloat)n/(float)nb_pts_affich_X, (GLfloat)m/(float)nb_pts_affich_Y);
	    }
	    glEnd();
	  }
      else
	for(m = 0; m < nb_pts_affich_Y; m++) {
	  glBegin(GL_LINES);
	  for(n = 0; n  < nb_pts_affich_X; n++) {	
	    glEvalCoord2f((GLfloat)n/(float)nb_pts_affich_X, (GLfloat)m/(float)nb_pts_affich_Y);	
	  }
	  glEnd();
	}
    }
    break;

  case 7:
    // control_fill 
    {	
      if (state->texture)
	for(n = 0; n < nb_pts_control_X-1; n++)
	  for(m = 0; m  < nb_pts_control_Y-1; m++)   {
	    Matrix::generateNormal((GLfloat*)&m_posXYZ[n+m*nb_pts_control_X],
                                   (GLfloat*)&m_posXYZ[n+m*nb_pts_control_X+1], 
                                   (GLfloat*)&m_posXYZ[n+(m+1)*nb_pts_control_X], 
                                   norm);
	    glNormal3fv(norm);

	    glBegin(GL_TRIANGLE_STRIP);
	    glTexCoord2f(xsize*(GLfloat)n/(nb_pts_control_X-1),
                         ysize+ysizediff*(GLfloat)m/(nb_pts_control_Y-1));
	    glVertex3fv((GLfloat*)&m_posXYZ[n+m*nb_pts_control_X]);	    
	    glTexCoord2f(xsize*(GLfloat)(n+1)/(nb_pts_control_X-1),
                         ysize+ysizediff*(GLfloat)m/(nb_pts_control_Y-1));
	    glVertex3fv((GLfloat*)&m_posXYZ[n+m*nb_pts_control_X+1]);
	    glTexCoord2f(xsize*(GLfloat)n/(nb_pts_control_X-1),
                         ysize+ysizediff*(GLfloat)(m+1)/(nb_pts_control_Y-1));
	    glVertex3fv((GLfloat*)&m_posXYZ[n+(m+1)*nb_pts_control_X]);
	    glEnd();

	    Matrix::generateNormal((GLfloat*)&m_posXYZ[n+1+(m+1)*nb_pts_control_X],
                                   (GLfloat*)&m_posXYZ[n+1+m*nb_pts_control_X], 
                                   (GLfloat*)&m_posXYZ[n+(m+1)*nb_pts_control_X], norm);
	    glNormal3fv(norm);
	    glBegin(GL_TRIANGLE_STRIP);
	    glTexCoord2f(xsize*(GLfloat)(n+1)/(nb_pts_control_X-1),
                         ysize+ysizediff*(GLfloat)(m+1)/(nb_pts_control_Y-1));
	    glVertex3fv((GLfloat*)&m_posXYZ[n+1+(m+1)*nb_pts_control_X]);
	    glTexCoord2f(xsize*(GLfloat)(n+1)/(nb_pts_control_X-1),
                         ysize+ysizediff*(GLfloat)m/(nb_pts_control_Y-1));
	    glVertex3fv((GLfloat*)&m_posXYZ[n+1+m*nb_pts_control_X]);
	    glTexCoord2f(xsize*(GLfloat)n/(nb_pts_control_X-1),
                         ysize+ysizediff*(GLfloat)(m+1)/(nb_pts_control_Y-1));
	    glVertex3fv((GLfloat*)&m_posXYZ[n+(m+1)*nb_pts_control_X]);
	    glEnd();
	  }
      else
	for(n = 0; n < nb_pts_control_X-1; n++)
	  for(m = 0; m  < nb_pts_control_Y-1; m++)    {
	    Matrix::generateNormal((GLfloat*)&m_posXYZ[n+m*nb_pts_control_X],
                                   (GLfloat*)&m_posXYZ[n+m*nb_pts_control_X+1], 
                                   (GLfloat*)&m_posXYZ[n+(m+1)*nb_pts_control_X], 
                                   norm);
	    glNormal3fv(norm);

	    glBegin(GL_TRIANGLE_STRIP);
	    glVertex3fv((GLfloat*)&m_posXYZ[n+m*nb_pts_control_X]);
	    glVertex3fv((GLfloat*)&m_posXYZ[n+m*nb_pts_control_X+1]);
	    glVertex3fv((GLfloat*)&m_posXYZ[n+(m+1)*nb_pts_control_X]);
	    glEnd();

	    Matrix::generateNormal((GLfloat*)&m_posXYZ[n+1+(m+1)*nb_pts_control_X],
                                   (GLfloat*)&m_posXYZ[n+(1+m)*nb_pts_control_X],
                                   (GLfloat*)&m_posXYZ[n+1+m*nb_pts_control_X], 
                                   norm);
	    glNormal3fv(norm);
	    glBegin(GL_TRIANGLE_STRIP);
	    glVertex3fv((GLfloat*)&m_posXYZ[n+1+(m+1)*nb_pts_control_X]);
	    glVertex3fv((GLfloat*)&m_posXYZ[n+(m+1)*nb_pts_control_X]);
	    glVertex3fv((GLfloat*)&m_posXYZ[n+1+m*nb_pts_control_X]);
	    glEnd();
	  }
    }
    break;

  case 8:
    // point de control de la structure (control_point)
    {
      if (state->texture)
	for(n = 0; n < nb_pts_control_X; n++)
	  for(m = 0; m  < nb_pts_control_Y; m++)   {	
	    glBegin(GL_POINTS);
	    glTexCoord2f(xsize*(GLfloat)n/(nb_pts_control_X-1),
                         ysize+ysizediff*(GLfloat)m/(nb_pts_control_Y-1));
	    glVertex3fv((GLfloat*)&m_posXYZ[n+m*nb_pts_control_X]);
	    glEnd();
	  }
      else
	for(n = 0; n < nb_pts_control_X; n++)
	  for(m = 0; m  < nb_pts_control_Y; m++) {	
	    glBegin(GL_POINTS);	
	    glVertex3fv((GLfloat*)&m_posXYZ[n+m*nb_pts_control_X]);
	    glEnd();
	  }
    }
    break;

  case 9:
    //control_line
    {
      if (state->texture){
	for(n = 0; n < nb_pts_control_X; n++)   {
	  glBegin(GL_LINE_STRIP);
	  for(m = 0; m  < nb_pts_control_Y; m++){	
	    glTexCoord2f(xsize*(GLfloat)n/(nb_pts_control_X-1),
                         ysize+ysizediff*(GLfloat)m/(nb_pts_control_Y-1));
	    glVertex3fv((GLfloat*)&m_posXYZ[n+m*nb_pts_control_X]);
	  }
	  glEnd();
	}

	for(m = 0; m < nb_pts_control_Y; m++)  {
	  glBegin(GL_LINE_STRIP);
	  for(n = 0; n  < nb_pts_control_X; n++){
	    glTexCoord2f(xsize*(GLfloat)n/(nb_pts_control_X-1),
                         ysize+ysizediff*(GLfloat)m/(nb_pts_control_Y-1));
	    glVertex3fv((GLfloat*)&m_posXYZ[n+m*nb_pts_control_X]);
	  }
	  glEnd();
	}
      }  else	{
	for(n = 0; n < nb_pts_control_X; n++)  {
	  glBegin(GL_LINE_STRIP);
	  for(m = 0; m  < nb_pts_control_Y; m++)
	    glVertex3fv((GLfloat*)&m_posXYZ[n+m*nb_pts_control_X]);
	  glEnd();
	}
	for(m = 0; m < nb_pts_control_Y; m++)  {
	  glBegin(GL_LINE_STRIP);
	  for(n = 0; n  < nb_pts_control_X; n++)	
	    glVertex3fv((GLfloat*)&m_posXYZ[n+m*nb_pts_control_X]);				
	  glEnd();
	}
      }
    }
    break;

  case 11:
    //control_line2
    {
      if (state->texture)
	for(m = 0; m < nb_pts_control_Y; m++) {
	  glBegin(GL_LINE_STRIP);
	  for(n = 0; n  < nb_pts_control_X; n++) {
	    glTexCoord2f(xsize*(GLfloat)n/(nb_pts_control_X-1),
                         ysize+ysizediff*(GLfloat)m/(nb_pts_control_Y-1));
	    glVertex3fv((GLfloat*)&m_posXYZ[n+m*nb_pts_control_X]);			
	  }
	  glEnd();
	}
      else
	for(m = 0; m < nb_pts_control_Y; m++) {
	  glBegin(GL_LINE_STRIP);
	  for(n = 0; n  < nb_pts_control_X; n++)
	    glVertex3fv((GLfloat*)&m_posXYZ[n+m*nb_pts_control_X]);	
	  glEnd();
	}
    }
    break;

  case 10:
    //control_line1
    {
      if (state->texture)
	for(n = 0; n < nb_pts_control_X; n++) {
	  glBegin(GL_LINE_STRIP);
	  for(m = 0; m  < nb_pts_control_Y; m++)  {	
	    glTexCoord2f(xsize*(GLfloat)n/(nb_pts_control_X-1),
                         ysize+ysizediff*(GLfloat)m/(nb_pts_control_Y-1));
	    glVertex3fv((GLfloat*)&m_posXYZ[n+m*nb_pts_control_X]);			
	  }
	  glEnd();
	}
      else
	for(n = 0; n < nb_pts_control_X; n++) {
	  glBegin(GL_LINE_STRIP);
	  for(m = 0; m  < nb_pts_control_Y; m++)
	    glVertex3fv((GLfloat*)&m_posXYZ[n+m*nb_pts_control_X]);
	  glEnd();
	}
    }
    return;
  }
}

//////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void curve3d :: obj_setupCallback(t_class *classPtr){
  class_addmethod(classPtr, (t_method)&curve3d::resolutionMessCallback,
		  gensym("res"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&curve3d::gridMessCallback,
		  gensym("grid"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&curve3d::setMessCallback,
		  gensym("set"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&curve3d::typeMessCallback,
		  gensym("draw"), A_SYMBOL, A_NULL);
}

void curve3d :: resolutionMessCallback(void *data, t_floatarg resX, t_floatarg resY)
{
  GetMyClass(data)->resolutionMess((int)resX, (int)resY);
}
void curve3d :: gridMessCallback(void *data, t_floatarg gridX, t_floatarg gridY)
{
  GetMyClass(data)->gridMess((int)gridX, (int)gridY);
}
void curve3d :: setMessCallback(void *data, float X, float Y, float posX, float posY, float posZ)
{
  GetMyClass(data)->setMess((int)X, (int)Y, posX, posY, posZ);
}
void curve3d :: typeMessCallback(void *data, t_symbol *type)
{
  GetMyClass(data)->typeMess(type);
}
