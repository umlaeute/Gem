/*
 *  GEM - Graphics Environment for Multimedia
 *
 *  rubber.cpp
 *  gem_darwin
 *
 *  Created by Jamie Tittle on Sun Jan 19 2003.
 *  Copyright (c) 2003 tigital. All rights reserved.
 *    For information on usage and redistribution, and for a DISCLAIMER OF ALL
 *    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
 *
 */

#include "rubber.h"

#define GRID_SIZE_X  32
#define GRID_SIZE_Y  32

#define CLIP_NEAR  0.0
#define CLIP_FAR   1000.0

#define SPRING_KS  0.3
#define DRAG	   0.5

typedef struct {
    float x[3];
    float v[3];
    float t[2];
    int nail;
} MASS;

typedef struct {
    int i, j;
    float r;
} SPRING;

static int spring_count;

static MASS *mass = NULL;
static SPRING *spring = NULL;

static int grab = -1;	/* index of grabbed mass point */

CPPEXTERN_NEW_WITH_TWO_ARGS(rubber, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// rubber
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
rubber :: rubber( t_floatarg gridX, t_floatarg gridY )
    	     : GemShape(1.0), m_height(0.0),
               m_speed(0), alreadyInit(0)
{
  m_grid_sizeX = (gridX>0.)?(int)gridX:GRID_SIZE_X;
  m_grid_sizeY = (gridY>0.)?(int)gridY:GRID_SIZE_Y;

  //  if (m_height == 0.f)m_height = 1.f;

    // the height inlet
    m_inletH = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("Ht"));
    inletcX = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("cX"));
    inletcY = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("cY"));
    //inletOt = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("Ot"));
    //m_drawType = GL_LINE_LOOP;
    m_drawType = GL_POLYGON;
    alreadyInit = 0;
    m_blend = 0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
rubber :: ~rubber()
{
	 inlet_free(m_inletH);
         inlet_free(inletcX);
         inlet_free(inletcY);
         //inlet_free(inletOt);
         alreadyInit = 0;
}

void rubber :: ctrXMess(float center)
{
    ctrX = center;
    setModified();
}
void rubber :: ctrYMess(float center)
{
    ctrY = center;
    setModified();
}

void rubber :: rubber_init()
{
  int i, j;
  int k;
  int m;
  glEnable(GL_DEPTH_TEST);

  if (mass != NULL)delete[]mass;    mass=NULL;
    
  mass = new MASS[m_grid_sizeX*m_grid_sizeY];
  if (mass == NULL)    {
    post("rubber: Can't allocate memory.\n");	
    exit(-1);
  }

  k = 0;
  for (i = 0; i < m_grid_sizeX; i++)
    for (j = 0; j < m_grid_sizeY; j++)
      {
      mass[k].nail = (i == 0 || j == 0 || i == m_grid_sizeX - 1
		      || j == m_grid_sizeY - 1);
      mass[k].x[0] = i/(m_grid_sizeX - 1.0);
      mass[k].x[1] = j/(m_grid_sizeY - 1.0);
      mass[k].x[2] = -(CLIP_FAR - CLIP_NEAR)/2.0;
      /*
      post("mass[%d].nail = %d",k,mass[k].nail);
      post("mass[%d].x[0] = %f",k,mass[k].x[0]);
      post("mass[%d].x[1] = %f",k,mass[k].x[1]);
      post("mass[%d].x[2] = %f\n",k,mass[k].x[2]);
      */
      mass[k].v[0] = 0.0;
      mass[k].v[1] = 0.0;
      mass[k].v[2] = 0.0;

      mass[k].t[0] = xsize*( i/(m_grid_sizeX - 1.0) );
      mass[k].t[1] = (ysize0-ysize)*( j/(m_grid_sizeY - 1.0) )+ysize;
      /*
      post("mass[%d].t[0] = %f",k,mass[k].t[0]);
      post("mass[%d].t[1] = %f\n",k,mass[k].t[1]);
      */
      k++;
    }

  if (spring != NULL)delete[]spring;
  spring_count = (m_grid_sizeX - 1)*(m_grid_sizeY - 2)
    + (m_grid_sizeY - 1)*(m_grid_sizeX - 2);
    
  //spring = (SPRING *) malloc(sizeof(SPRING)*spring_count);
  spring=new SPRING[spring_count];
  if (spring == NULL)    {
    post("rubber: Can't allocate memory.\n");	
    exit(-1);
  }
  
  k = 0;
  for (i = 1; i < m_grid_sizeX - 1; i++)
    for (j = 0; j < m_grid_sizeY - 1; j++)
    {
      m = m_grid_sizeY*i + j;
      spring[k].i = m;
      spring[k].j = m + 1;
      spring[k].r = 0;
      k++;
    }

  for (j = 1; j < m_grid_sizeY - 1; j++)
    for (i = 0; i < m_grid_sizeX - 1; i++)
    {
      m = m_grid_sizeY*i + j;
      spring[k].i = m;
      spring[k].j = m + m_grid_sizeX;
      spring[k].r = 0.0;
      k++;
    }
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void rubber :: render(GemState *state)
{
    int k, i, j;
    if (m_drawType == GL_LINE_LOOP)glLineWidth(m_linewidth);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glScalef(m_size, m_size, m_size);
    glTranslatef(-1.0, -1.0, 0.0);

    glOrtho(-0.5, 0.5, -0.5, 0.5, CLIP_NEAR, CLIP_FAR);
    //glOrtho(0.5, GemMan::m_width - 0.5, 0.5, GemMan::m_height - 0.5, CLIP_NEAR, CLIP_FAR);
    
    if (m_blend) {
        glEnable(GL_POLYGON_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE);
        glHint(GL_POLYGON_SMOOTH_HINT,GL_DONT_CARE);
    }
    
    glNormal3f(0.0f, 0.0f, 1.0f);
     
    if (state->texture && state->numTexCoords>=3) {

      if ((xsize  != state->texCoords[1].s) ||
	  (ysize  != state->texCoords[1].t) ||
	  (ysize0 != state->texCoords[2].t))
	alreadyInit = 0;
    
      if (!alreadyInit)  {
	xsize  = state->texCoords[1].s;
	ysize0 = state->texCoords[2].t;
	ysize  = state->texCoords[1].t;

	rubber_init();
	alreadyInit = 1;
      }

      k = 0;
      for (i = 0; i < m_grid_sizeX - 1; i++)  {
	for (j = 0; j < m_grid_sizeY - 1; j++){
	  glBegin(GL_POLYGON);
	  glTexCoord2fv(mass[k].t);
	  glVertex3fv(mass[k].x);
	  glTexCoord2fv(mass[k + 1].t);
	  glVertex3fv(mass[k + 1].x);
	  glTexCoord2fv(mass[k + m_grid_sizeY + 1].t);
	  glVertex3fv(mass[k + m_grid_sizeY + 1].x);
	  glTexCoord2fv(mass[k + m_grid_sizeY].t);
	  glVertex3fv(mass[k + m_grid_sizeY].x);
	  glEnd();
	  k++;
	}
	k++;
      }
      rubber_dynamics();
    } else {
      if (!alreadyInit) {
	rubber_init();
	alreadyInit = 1;
      }
      
      k = 0;
      for (i = 0; i < m_grid_sizeX - 1; i++)  {
	for (j = 0; j < m_grid_sizeY - 1; j++){
	  glBegin(m_drawType);
	  glTexCoord2fv(mass[k].t);
	  glVertex3fv(mass[k].x);
	  glTexCoord2fv(mass[k + 1].t);
	  glVertex3fv(mass[k + 1].x);
	  glTexCoord2fv(mass[k + m_grid_sizeY + 1].t);
	  glVertex3fv(mass[k + m_grid_sizeY + 1].x);
	  glTexCoord2fv(mass[k + m_grid_sizeY].t);
	  glVertex3fv(mass[k + m_grid_sizeY].x);
	  glEnd();
	  k++;
	}
	k++;
      }
      rubber_dynamics();
      /*for (k = 0; k < spring_count; k++)
        {
	glBegin(GL_LINES);
	glVertex3fv(mass[spring[k].i].x);
	glVertex3fv(mass[spring[k].j].x);
	glEnd();
        }*/
    }
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
/*
	Do the dynamics simulation for the next frame.
*/

void rubber :: rubber_dynamics()
{
  int k;
  float d[3];
  int i, j;
  float l;
  float a;

  /* calculate all the spring forces acting on the mass points */

  for (k = 0; k < spring_count; k++)
  {
    i = spring[k].i;
    j = spring[k].j;

    d[0] = mass[i].x[0] - mass[j].x[0];
    d[1] = mass[i].x[1] - mass[j].x[1];
    d[2] = mass[i].x[2] - mass[j].x[2];

    l = sqrt(d[0]*d[0] + d[1]*d[1] + d[2]*d[2]);
    if (l != 0.0)
    {
      d[0] /= l;
      d[1] /= l;
      d[2] /= l;

      a = l - spring[k].r;

      mass[i].v[0] -= d[0]*a*SPRING_KS;
      mass[i].v[1] -= d[1]*a*SPRING_KS;
      mass[i].v[2] -= d[2]*a*SPRING_KS;

      mass[j].v[0] += d[0]*a*SPRING_KS;
      mass[j].v[1] += d[1]*a*SPRING_KS;
      mass[j].v[2] += d[2]*a*SPRING_KS;
    }
  }

  /* update the state of the mass points */

  for (k = 0; k < m_grid_sizeX*m_grid_sizeY; k++)
    if (!mass[k].nail)
    {
      mass[k].x[0] += mass[k].v[0];
      mass[k].x[1] += mass[k].v[1];
      mass[k].x[2] += mass[k].v[2];
      
      mass[k].v[0] *= (1.0 - DRAG);
      mass[k].v[1] *= (1.0 - DRAG);
      mass[k].v[2] *= (1.0 - DRAG);

      if (mass[k].x[2] > -CLIP_NEAR - 0.01)mass[k].x[2] = -CLIP_NEAR - 0.01;
      if (mass[k].x[2] < -CLIP_FAR + 0.01) mass[k].x[2] = -CLIP_FAR  + 0.01;
    }

  /* if a mass point is grabbed, attach it to the mouse */

  if (grab != -1 && !mass[grab].nail)
  {
    mass[grab].x[0] = ctrX;
    mass[grab].x[1] = ctrY;
    mass[grab].x[2] = (CLIP_FAR - CLIP_NEAR)/4.0*m_height; /* jmz: added height */
  }
}

/*
	Return the index of the mass point that's nearest to the
	given screen coordinate.
*/

int rubber :: rubber_grab()
{
  float dx[2];
  float d;
  float min_d=0;
  int min_i=0;
  int i;

  for (i = 0; i < m_grid_sizeX*m_grid_sizeY; i++)
  {
    dx[0] = mass[i].x[0] - ctrX;
    dx[1] = mass[i].x[1] - ctrY;
    d = sqrt(dx[0]*dx[0] + dx[1]*dx[1]);
    if (i == 0 || d < min_d)
    {
      min_i = i;
      min_d = d;
    }
  }

  return min_i;
}

/////////////////////////////////////////////////////////
// bangMess
//
/////////////////////////////////////////////////////////
void rubber :: rubber_bang()
{
  grab=(grab+1)?-1:rubber_grab();
}

/////////////////////////////////////////////////////////
// heightMess
//
/////////////////////////////////////////////////////////
void rubber :: heightMess(float height)
{
  m_height = (height-1.0)*2.0;
  setModified();
}

/////////////////////////////////////////////////////////
// typeMess
//
/////////////////////////////////////////////////////////
void rubber :: typeMess(t_symbol *type)
{
  char c=*type->s_name;
  switch(c){
  case 'l': case 'L':   m_drawType = GL_LINE_LOOP; break;
  case 'd': case 'D': // default
  case 'f': case 'F':   m_drawType = GL_POLYGON; break;
  case 'p': case 'P':   m_drawType = GL_POINTS; break;
  default:
    error ("GEM: square draw style");
    return;
  }
  setModified();
}
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void rubber :: obj_setupCallback(t_class *classPtr)
{
    class_addbang(classPtr, (t_method)&rubber::bangMessCallback);
    class_addmethod(classPtr, (t_method)&rubber::heightMessCallback,
    	    gensym("Ht"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&rubber::ctrXMessCallback,
    	    gensym("cX"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&rubber::ctrYMessCallback,
    	    gensym("cY"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&rubber::blendMessCallback,
    	    gensym("blend"), A_FLOAT, A_NULL);
}
void rubber :: bangMessCallback(void *data)
{
    GetMyClass(data)->rubber_bang();
}
void rubber :: heightMessCallback(void *data, t_floatarg height)
{
    GetMyClass(data)->heightMess((float)height);
}
void rubber :: ctrXMessCallback(void *data, t_floatarg center)
{
    GetMyClass(data)->ctrXMess((float)center);
}
void rubber :: ctrYMessCallback(void *data, t_floatarg center)
{
    GetMyClass(data)->ctrYMess((float)center);
}
void rubber :: blendMessCallback(void *data, t_floatarg blend)
{
    GetMyClass(data)->m_blend=((int)blend);
}

