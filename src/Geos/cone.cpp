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

#include "cone.h"

CPPEXTERN_NEW_WITH_ONE_ARG(cone, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// cone
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
cone :: cone(t_floatarg size)
      : GemGluObj(size)
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
cone :: ~cone()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void cone :: render(GemState *state)
{
    // lighting
    if (state->lighting)
    {
    	// normals
    	if (state->smooth) gluQuadricNormals(m_thing, (GLenum)GLU_SMOOTH);
    	else gluQuadricNormals(m_thing, (GLenum)GLU_FLAT);
    }
    
    // texture mapping
    if (state->texture) gluQuadricTexture(m_thing, GL_TRUE);
    else gluQuadricTexture(m_thing, GL_FALSE);
    
    gluQuadricDrawStyle(m_thing, m_drawType);
    gluCylinder(m_thing, m_size, 0, m_size * 2, m_numSlices, m_numSlices);
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void cone :: obj_setupCallback(t_class *)
{ }
