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
    m_drawType = GL_QUADS;
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
    glNormal3f(0.0f, 0.0f, 1.0f);
    if (m_drawType == GL_LINE_LOOP)
        glLineWidth(m_linewidth);
    
    glBegin(m_drawType);

    SetVertix(state, -m_size,  -m_size, 0.0f,0.,0.,0);
    SetVertix(state, m_size,  -m_size, 0.0f,1.,0.,1);
    SetVertix(state, m_size,  m_size, 0.0f,1.,1.,2);
    SetVertix(state, -m_size,  m_size, 0.0f,0.,1.,3);

    glEnd();

    if (m_drawType == GL_LINE_LOOP)
        glLineWidth(1.0);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void square :: obj_setupCallback(t_class *)
{ }
