////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "circle.h"

#include <math.h>

#include "Base/GemState.h"

const int circle::NUM_PNTS = 100;
GLfloat *circle::m_cos = NULL;
GLfloat *circle::m_sin = NULL;

CPPEXTERN_NEW_WITH_ONE_ARG(circle, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// circle
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
circle :: circle(t_floatarg size)
        : GemShape(size)
{
    m_drawType = GL_POLYGON;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
circle :: ~circle()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void circle :: render(GemState *state)
{
  if(m_drawType==GL_DEFAULT_GEM)m_drawType=GL_POLYGON;
    glNormal3f(0.0f, 0.0f, 1.0f);
    glLineWidth(m_linewidth);
    glBegin(m_drawType);
	    if (state->texture)
	    {
	      GLfloat xsize  = state->texCoords[1].s;
	      GLfloat ysize0 = state->texCoords[2].t;
	      GLfloat ysize  = state->texCoords[1].t;
	        for (int n = 0; n < NUM_PNTS; n++)
	        {
		  glTexCoord2f(xsize*(m_cos[n] + 1) / 2.f, (ysize0-ysize)*(m_sin[n] + 1) / 2.f+ysize);
		        glVertex3f(m_cos[n] * m_size,
			           m_sin[n] * m_size,
			           0.0);
	        }
	    }
	    else
	    {
	        for (int n = 0; n < NUM_PNTS; n++)
		        glVertex3f(m_cos[n] * m_size,
			           m_sin[n] * m_size,
			           0.0);
	    }
    glEnd();
    glLineWidth(1.0);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void circle :: obj_setupCallback(t_class *)
{
    m_cos = new GLfloat [NUM_PNTS];
    m_sin = new GLfloat [NUM_PNTS];
    float TWO_PI = 8.f * (float)atan(1.f);
    // compute sin/cos lookup table
    for(int i = 0; i < NUM_PNTS; i++)
    {
	    m_cos[i] = (float)cos(TWO_PI * (double)i / (double)NUM_PNTS);
	    m_sin[i] = (float)sin(TWO_PI * (double)i / (double)NUM_PNTS);
    }
}

