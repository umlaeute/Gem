////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "rectangle.h"

#include "Base/GemState.h"
#include <string.h>

CPPEXTERN_NEW_WITH_TWO_ARGS(rectangle, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// rectangle
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
rectangle :: rectangle(t_floatarg width, t_floatarg height)
		   : GemShape(width), m_height(height)
{
    if (m_height == 0.f)
		m_height = 1.f;

    // the height inlet
    m_inletH = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("ft2"));
    m_drawType = GL_QUADS;
    m_blend = 0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
rectangle :: ~rectangle()
{
    inlet_free(m_inletH);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void rectangle :: render(GemState *state)
{
    glNormal3f(0.0f, 0.0f, 1.0f);
    if (m_drawType == GL_LINE_LOOP)
        glLineWidth(m_linewidth);
        
    if (m_blend) {
        glEnable(GL_POLYGON_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE);
        glHint(GL_POLYGON_SMOOTH_HINT,GL_DONT_CARE);
    }

    if (state->texture && state->numTexCoords)
    {
        int curCoord = 0;

	    glBegin(m_drawType);
	    	glTexCoord2f(state->texCoords[curCoord].s, state->texCoords[curCoord].t);
                glVertex3f(-m_size, -m_height, 0.0f);

	        if (state->numTexCoords > 1) curCoord = 1;
	    	glTexCoord2f(state->texCoords[curCoord].s, state->texCoords[curCoord].t);
                glVertex3f( m_size, -m_height, 0.0f);

	        if (state->numTexCoords > 2) curCoord = 2;
	    	glTexCoord2f(state->texCoords[curCoord].s, state->texCoords[curCoord].t);
                glVertex3f( m_size,  m_height, 0.0f);

	        if (state->numTexCoords > 3) curCoord = 3;
	    	glTexCoord2f(state->texCoords[curCoord].s, state->texCoords[curCoord].t);
                glVertex3f(-m_size,  m_height, 0.0f);
                
	    glEnd();
    }
    else
    {
	    glBegin(m_drawType);
	        glTexCoord2f(0.0f, 0.0f);
                glVertex3f(-m_size, -m_height, 0.0f);
	        glTexCoord2f(1.0f, 0.0f);
                glVertex3f( m_size, -m_height, 0.0f);
	        glTexCoord2f(1.0f, 1.0f);
                glVertex3f( m_size,  m_height, 0.0f);
	        glTexCoord2f(0.0f, 1.0f);
                glVertex3f(-m_size,  m_height, 0.0f);
	    glEnd();
    }

    if (m_blend) {
        glDisable(GL_POLYGON_SMOOTH);
        glDisable(GL_BLEND);
    }
}

/////////////////////////////////////////////////////////
// heightMess
//
/////////////////////////////////////////////////////////
void rectangle :: heightMess(float size)
{
    m_height = size;
    setModified();
}

/////////////////////////////////////////////////////////
// typeMess
//
/////////////////////////////////////////////////////////
void rectangle :: typeMess(t_symbol *type)
{
    if (!strcmp(type->s_name, "line")) 
	    m_drawType = GL_LINE_LOOP;
    else if (!strcmp(type->s_name, "fill")) 
	    m_drawType = GL_QUADS;
    else if (!strcmp(type->s_name, "point"))
	    m_drawType = GL_POINTS;
    else
    {
	    error ("GEM: square draw style");
	    return;
    }
    setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void rectangle :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&rectangle::heightMessCallback,
    	    gensym("ft2"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&rectangle::blendMessCallback,
    	    gensym("blend"), A_FLOAT, A_NULL);
}

void rectangle :: heightMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->heightMess((float)size);
}
void rectangle :: blendMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->m_blend=((int)size);
}
