////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "colorSquare.h"
#include <string.h>

#include "Base/GemState.h"

CPPEXTERN_NEW_WITH_ONE_ARG(colorSquare, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// colorSquare
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
colorSquare :: colorSquare(t_floatarg size)
             : GemShape(size)
{
    m_drawType = GL_QUADS;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
            m_color[i][j] = 1.f;
    }

    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_list, gensym("vert0"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_list, gensym("vert1"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_list, gensym("vert2"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_list, gensym("vert3"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
colorSquare :: ~colorSquare()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void colorSquare :: render(GemState *state)
{
    if (!state->lighting) glShadeModel(GL_SMOOTH);

    glNormal3f(0.0f, 0.0f, 1.0f);
    if (m_drawType == GL_LINE_LOOP) glLineWidth(m_linewidth);
    if (state->texture && state->numTexCoords)
    {
        int curCoord = 0;

	    glBegin(m_drawType);
	    	glTexCoord2f(state->texCoords[curCoord].s, state->texCoords[curCoord].t);
            glColor3fv(m_color[0]);
                glVertex3f(-m_size, -m_size, 0.0f);

	        if (state->numTexCoords > 1) curCoord = 1;
	    	glTexCoord2f(state->texCoords[curCoord].s, state->texCoords[curCoord].t);
            glColor3fv(m_color[1]);
                glVertex3f( m_size, -m_size, 0.0f);

	        if (state->numTexCoords > 2) curCoord = 2;
	    	glTexCoord2f(state->texCoords[curCoord].s, state->texCoords[curCoord].t);
            glColor3fv(m_color[2]);
                glVertex3f( m_size,  m_size, 0.0f);

	        if (state->numTexCoords > 3) curCoord = 3;
	    	glTexCoord2f(state->texCoords[curCoord].s, state->texCoords[curCoord].t);
            glColor3fv(m_color[3]);
                glVertex3f(-m_size,  m_size, 0.0f);
	    glEnd();
    }
    else
    {
	    glBegin(m_drawType);
            glColor3fv(m_color[0]);
	        glTexCoord2f(0.0f, 0.0f);
                glVertex3f(-m_size, -m_size, 0.0f);
            glColor3fv(m_color[1]);
	        glTexCoord2f(1.0f, 0.0f);
                glVertex3f( m_size, -m_size, 0.0f);
            glColor3fv(m_color[2]);
	        glTexCoord2f(1.0f, 1.0f);
                glVertex3f( m_size,  m_size, 0.0f);
            glColor3fv(m_color[3]);
	        glTexCoord2f(0.0f, 1.0f);
                glVertex3f(-m_size,  m_size, 0.0f);
	    glEnd();
    }
    if (m_drawType == GL_LINE_LOOP)
        glLineWidth(1.0);
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void colorSquare :: postrender(GemState *state)
{
    if (!state->lighting) glShadeModel(GL_FLAT);
}

/////////////////////////////////////////////////////////
// vertColorMess
//
/////////////////////////////////////////////////////////
void colorSquare :: vertColorMess(int whichVert, float r, float g, float b)
{
    m_color[whichVert][0] = r;
    m_color[whichVert][1] = g;
    m_color[whichVert][2] = b;
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void colorSquare :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&colorSquare::vert0MessCallback,
    	    gensym("vert0"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&colorSquare::vert1MessCallback,
    	    gensym("vert1"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&colorSquare::vert2MessCallback,
    	    gensym("vert2"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&colorSquare::vert3MessCallback,
    	    gensym("vert3"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
}
void colorSquare :: vert0MessCallback(void *data, t_floatarg r, t_floatarg g, t_floatarg b)
{
    GetMyClass(data)->vertColorMess(0, (float)r, (float)g, (float)b);
}
void colorSquare :: vert1MessCallback(void *data, t_floatarg r, t_floatarg g, t_floatarg b)
{
    GetMyClass(data)->vertColorMess(1, (float)r, (float)g, (float)b);
}
void colorSquare :: vert2MessCallback(void *data, t_floatarg r, t_floatarg g, t_floatarg b)
{
    GetMyClass(data)->vertColorMess(2, (float)r, (float)g, (float)b);
}
void colorSquare :: vert3MessCallback(void *data, t_floatarg r, t_floatarg g, t_floatarg b)
{
    GetMyClass(data)->vertColorMess(3, (float)r, (float)g, (float)b);
}
