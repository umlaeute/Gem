////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "square.h"

#include "Base/GemState.h"

CPPEXTERN_NEW_WITH_ONE_ARG(square, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// square
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
square :: square(t_floatarg size)
        : GemShape(size)
{
    m_linewidth=1.0;
    m_drawType = GL_QUADS;
    m_blend=0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
square :: ~square()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void square :: render(GemState *state)
{
  if(m_drawType==-1)m_drawType=GL_QUADS;
    glNormal3f(0.0f, 0.0f, 1.0f);
    if (m_drawType == GL_LINE_LOOP)
        glLineWidth(m_linewidth);
        
    if (m_blend) {
        glEnable(GL_POLYGON_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE);
        glHint(GL_POLYGON_SMOOTH_HINT,GL_DONT_CARE); 
    }
    glBegin(m_drawType);

    SetVertix(state, -m_size,  -m_size, 0.0f,0.,0.,0);
    SetVertix(state, m_size,  -m_size, 0.0f,1.,0.,1);
    SetVertix(state, m_size,  m_size, 0.0f,1.,1.,2);
    SetVertix(state, -m_size,  m_size, 0.0f,0.,1.,3);

    glEnd();
    if (m_blend) {
        glDisable(GL_POLYGON_SMOOTH);
        glDisable(GL_BLEND);
    }
}
 
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void square :: obj_setupCallback(t_class *classPtr)
{     class_addmethod(classPtr, (t_method)&square::blendMessCallback,
    	    gensym("blend"), A_FLOAT, A_NULL);
}

void square :: blendMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->m_blend=((int)size);
}
