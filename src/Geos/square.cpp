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
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "square.h"

#include "Gem/State.h"

CPPEXTERN_NEW_WITH_ONE_ARG(square, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// square
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

static float s_vertices[]={
-1, -1, 0,
-1,  1, 0,
 1,  1, 0,

 1,  1, 0,
 1, -1, 0,
-1, -1, 0,
};

static float s_normals[]={
0, 0, -1,
0, 0, -1,
0, 0, -1,

0, 0, -1,
0, 0, -1,
0, 0, -1,
};

static float s_texcoords[]={
0, 1,
0, 0,
1, 0,

1, 0,
1, 1,
0, 1,
};

square :: square(t_floatarg size)
        : GemShapeVBO(size)
{
  std::vector<float>vec;
  vec.assign(s_vertices, s_vertices+sizeof(s_vertices)/sizeof(*s_vertices));
  setArray(VERTEX, vec, 3);
  vec.assign(s_normals, s_normals+sizeof(s_normals)/sizeof(*s_normals));
  setArray(NORMAL, vec, 3);
  vec.assign(s_texcoords, s_texcoords+sizeof(s_texcoords)/sizeof(*s_texcoords));
  setArray(TEXCOORD, vec, 2);
  setDefaultDrawtype(GL_POLYGON);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
square :: ~square()
{ }

/////////////////////////////////////////////////////////
// renderShape
//
/////////////////////////////////////////////////////////
void square :: renderShape(GemState *state)
{
  if(m_drawType==GL_DEFAULT_GEM)m_drawType=GL_QUADS;
    glNormal3f(0.0f, 0.0f, 1.0f);
    if (m_drawType == GL_LINE_LOOP)
        glLineWidth(m_linewidth);

    glBegin(m_drawType);

    SetVertex(state, -m_size, -m_size, 0.0f, 0.,0., 0);
    SetVertex(state,  m_size, -m_size, 0.0f, 1.,0., 1);
    SetVertex(state,  m_size,  m_size, 0.0f, 1.,1., 2);
    SetVertex(state, -m_size,  m_size, 0.0f, 0.,1., 3);

    glEnd();

    if (m_drawType == GL_LINE_LOOP)
        glLineWidth(1.0);
}

void square :: obj_setupCallback(t_class *classPtr){}
