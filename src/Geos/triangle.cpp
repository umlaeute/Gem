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

#include "triangle.h"
#include <string.h>

#include "Base/GemState.h"

CPPEXTERN_NEW_WITH_ONE_ARG(triangle, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// triangle
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
triangle :: triangle(t_floatarg size)
          : GemShape(size)
{
    m_drawType = GL_TRIANGLES;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
triangle :: ~triangle()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void triangle :: render(GemState *state)
{
    glNormal3f(0.0f, 0.0f, 1.0f);
    if (m_drawType == GL_LINE_LOOP)
        glLineWidth(m_linewidth);

    if (state->texture && state->numTexCoords)
    {
        int curCoord = 0;
	    glBegin(m_drawType);
	        glTexCoord2f(state->texCoords[curCoord].s, state->texCoords[curCoord].t);
   	        glVertex3f(-m_size, -m_size, 0.f);

	        if (state->numTexCoords > 1) curCoord = 1;
	    	glTexCoord2f(state->texCoords[curCoord].s, state->texCoords[curCoord].t);
   	        glVertex3f(m_size, -m_size, 0.f);

	        if (state->numTexCoords > 2) curCoord = 2;
	    	glTexCoord2f(state->texCoords[curCoord].s, state->texCoords[curCoord].t);
   	        glVertex3f(0.f,	m_size, 0.f);
	    glEnd();
    }
    else
    {
	    glBegin(m_drawType);
    	        glTexCoord2f(0.f, 0.f); glVertex3f(-m_size, -m_size, 0.f);
    	        glTexCoord2f(1.f, 0.f); glVertex3f( m_size, -m_size, 0.f);
    	        glTexCoord2f(.5f, 1.f); glVertex3f( 0.f,     m_size, 0.f);
	    glEnd();
    }
    if (m_drawType == GL_LINE_LOOP)
        glLineWidth(1.0);
}

/////////////////////////////////////////////////////////
// typeMess
//
/////////////////////////////////////////////////////////
void triangle :: typeMess(t_symbol *type)
{
    if (!strcmp(type->s_name, "line")) 
	    m_drawType = GL_LINE_LOOP;
    else if (!strcmp(type->s_name, "fill")) 
	    m_drawType = GL_TRIANGLES;
    else if (!strcmp(type->s_name, "point"))
	    m_drawType = GL_POINTS;
    else
    {
	    error ("GEM: triangle: draw style");
	    return;
    }
    setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void triangle :: obj_setupCallback(t_class *)
{ }

