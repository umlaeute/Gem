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
void curve :: render(GemState *)
{
    glNormal3f(0.0f, 0.0f, 1.0f);
    glLineWidth(m_linewidth);
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
