////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "curve.h"

#include <string.h>

CPPEXTERN_NEW_WITH_ONE_ARG(curve, t_floatarg, A_FLOAT)

static float tex[2][2][2] =
{
  { {0, 0},
    {1, 0}},
  { {0, 1},
    {1, 1}}
};

/////////////////////////////////////////////////////////
//
// curve
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
curve :: curve(t_floatarg numInputs)
	   : polygon(numInputs)
{
    m_drawType = GL_LINE_STRIP;
    m_resolution = 30;

    m_texCoords[0][0]=0;m_texCoords[0][1]=0;
    m_texCoords[1][0]=1;m_texCoords[1][1]=0;
    m_texCoords[2][0]=0;m_texCoords[2][1]=1;
    m_texCoords[3][0]=1;m_texCoords[3][1]=1;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
curve :: ~curve()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void curve :: render(GemState *state)
{
    glNormal3f(0.0f, 0.0f, 1.0f);
    glLineWidth(m_linewidth);
    if(state->texture){
      m_texCoords[0][0]=state->texCoords[0].s;m_texCoords[0][1]=state->texCoords[0].t;
      m_texCoords[1][0]=state->texCoords[1].s;m_texCoords[1][1]=state->texCoords[1].t;
      m_texCoords[2][0]=state->texCoords[2].s;m_texCoords[2][1]=state->texCoords[2].t;
      m_texCoords[3][0]=state->texCoords[3].s;m_texCoords[3][1]=state->texCoords[3].t;

      glEnable(GL_MAP1_TEXTURE_COORD_2);
      glMap1f(GL_MAP1_TEXTURE_COORD_2, 0,   1,   2, m_numInputs, &m_texCoords[0][0]);
    }
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, m_numInputs, &(m_vert[0][0]));
    glEnable(GL_MAP1_VERTEX_3);
    glBegin(m_drawType);
	    for (int n = 0; n < m_resolution; n++)
	        glEvalCoord1f((GLfloat)n/(float)m_resolution);
    glEnd();
    glLineWidth(1.0);
}

/////////////////////////////////////////////////////////
// resolutionMess
//
/////////////////////////////////////////////////////////
void curve :: resolutionMess(int resolution)
{
    m_resolution = (resolution < 1) ? 1 : resolution;
    setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void curve :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&curve::resolutionMessCallback,
    	    gensym("res"), A_FLOAT, A_NULL);
}
void curve :: resolutionMessCallback(void *data, t_floatarg res)
{
    GetMyClass(data)->resolutionMess((int)res);
}
