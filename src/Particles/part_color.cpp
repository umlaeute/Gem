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

#include "part_color.h"

#include "papi.h"

CPPEXTERN_NEW(part_color)

/////////////////////////////////////////////////////////
//
// part_color
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
part_color :: part_color()
{
	color1Mess(1.f, 1.f, 1.f);
	color2Mess(1.f, 1.f, 1.f);
	
    // create the new inlet
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("color1"));

    // create the new inlet
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("color2"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
part_color :: ~part_color()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void part_color :: render(GemState *state)
{
	if (state->stereo == 0 ||
		state->stereo == 1)
	{
		pColorD(1.0f, PDLine, m_color1[0], m_color1[1], m_color1[2],
							  m_color2[0], m_color2[1], m_color2[2]);
	}
}

/////////////////////////////////////////////////////////
// color1Mess
//
/////////////////////////////////////////////////////////
void part_color :: color1Mess(float red, float green, float blue)
{
    m_color1[0] = red;
    m_color1[1] = green;
    m_color1[2] = blue;
    setModified();
}

/////////////////////////////////////////////////////////
// color2Mess
//
/////////////////////////////////////////////////////////
void part_color :: color2Mess(float red, float green, float blue)
{
    m_color2[0] = red;
    m_color2[1] = green;
    m_color2[2] = blue;
    setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void part_color :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&part_color::color1MessCallback,
    	    gensym("color1"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL); 
    class_addmethod(classPtr, (t_method)&part_color::color2MessCallback,
    	    gensym("color2"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL); 
}
void part_color :: color1MessCallback(void *data, t_floatarg r, t_floatarg g, t_floatarg b)
{
    GetMyClass(data)->color1Mess(r, g, b);
}
void part_color :: color2MessCallback(void *data, t_floatarg r, t_floatarg g, t_floatarg b)
{
    GetMyClass(data)->color2Mess(r, g, b);
}
