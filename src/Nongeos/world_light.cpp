////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "world_light.h"

#include "Base/GemMan.h"

CPPEXTERN_NEW_WITH_ONE_ARG(world_light, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// world_light
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
world_light :: world_light(t_floatarg lightNum)
			 : m_debug(0), m_thing(NULL)
{
    m_color[0] = m_color[1] = m_color[2] = m_color[3] = 1.0;

    m_position[0] = 0.0;
    m_position[1] = 0.0;
    m_position[2] = 2.0;
    m_position[3] = 0.0;

	int num;
	if (lightNum < 1.f)
		num = 0;
	else
		num = (int)lightNum;
	m_light = GemMan::requestLight(num);
	m_on = 1;
	m_change = 1;
	    
    // create the color inlet
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("clrlist"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
world_light :: ~world_light()
{
	stopRendering();
    if (m_light)
		GemMan::freeLight(m_light);
}

/////////////////////////////////////////////////////////
// lightOnOffMess
//
/////////////////////////////////////////////////////////
void world_light :: lightOnOffMess(int state)
{
    m_on = state;
	m_change = 1;
    setModified();
}

/////////////////////////////////////////////////////////
// debugMess
//
/////////////////////////////////////////////////////////
void world_light :: debugMess(int state)
{
    m_debug = state;
	m_change = 1;
    setModified();
}

/////////////////////////////////////////////////////////
// lightColorMess
//
/////////////////////////////////////////////////////////
void world_light :: lightColorMess(float red, float green, float blue)
{
    m_color[0] = red;
    m_color[1] = green;
    m_color[2] = blue;
	m_change = 1;
    setModified();
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void world_light :: startRendering()
{
    if (m_thing)
		stopRendering();
    m_thing = gluNewQuadric();
	gluQuadricTexture(m_thing, GL_FALSE);
	gluQuadricDrawStyle(m_thing, (GLenum) GLU_FILL);
	m_change = 1;
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void world_light :: stopRendering()
{
    if (m_thing)
		gluDeleteQuadric(m_thing);
    m_thing = NULL;
	if (m_light)
		glDisable(m_light);
	m_change = 1;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void world_light :: render(GemState *state)
{
    if (!m_light)
		return;

	if (m_change)
	{
		m_change = 0;
		if ( !m_on )
		{
    		glDisable(m_light);
    		return;
		}

		glEnable(m_light);
		glLightfv(m_light, GL_DIFFUSE,  m_color);
		glLightfv(m_light, GL_SPECULAR, m_color);
	}

	if (m_on)
	{
		glLightfv(m_light, GL_POSITION, m_position);
		if (m_debug)
		{
			glPushMatrix();
				glDisable(GL_LIGHTING);
				glColor3fv(m_color);
				glTranslatef(m_position[0], m_position[1], m_position[2]);
				const float size = .2f;
				if (m_position[3] == 0.f)
					gluCylinder(m_thing, size, 0, size * 2.f, 10, 10);
				else
					gluSphere(m_thing, size, 10, 10);
				glEnable(GL_LIGHTING);
			glPopMatrix();
		}
	}
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void world_light :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&world_light::lightColorMessCallback,
    	    gensym("clrlist"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&world_light::debugMessCallback,
    	    gensym("debug"), A_FLOAT, A_NULL);
    class_addfloat(classPtr, (t_method)&world_light::lightOnOffMessCallback);
}
void world_light :: lightColorMessCallback(void *data, t_floatarg red, t_floatarg green, t_floatarg blue)
{
    GetMyClass(data)->lightColorMess((float)red, (float)green, (float)blue);
}
void world_light :: lightOnOffMessCallback(void *data, t_floatarg n)
{
    GetMyClass(data)->lightOnOffMess((int)n);
}
void world_light :: debugMessCallback(void *data, t_floatarg n)
{
    GetMyClass(data)->debugMess((int)n);
}
