////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_data.h"

CPPEXTERN_NEW(pix_data)

/////////////////////////////////////////////////////////
//
// pix_data
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_data :: pix_data()
{
    // create the new inlet for the X position
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("xPos"));
    // create the new inlet for the Y position
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("yPos"));

    m_colorOut = outlet_new(this->x_obj, 0);
    m_grayOut = outlet_new(this->x_obj, 0);

    m_position[0] = m_position[1] = 0.f;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_data :: ~pix_data()
{
	outlet_free(m_colorOut);
	outlet_free(m_grayOut);
}

/////////////////////////////////////////////////////////
// trigger
//
/////////////////////////////////////////////////////////
void pix_data :: trigger()
{
	// if we don't have a "right hand" image, then just return
	if (!m_pixRight || !m_pixRight->image.data)
		return;

	int xPos = (int)(m_position[0] * (float)m_pixRight->image.xsize);
	int yPos = (int)(m_position[1] * (float)m_pixRight->image.ysize);
	int position = yPos * m_pixRight->image.xsize * m_pixRight->image.csize +
					xPos * m_pixRight->image.csize;

	unsigned char *data = &m_pixRight->image.data[position];
	float color[3];

	// is this a gray8 or RGBA?
	switch(m_pixRight->image.csize)
	{
		// Gray scale
		case(1):
			color[0] = color[1] = color[2] = data[chGray] / 256.f;
			break;
        // YUV
        case(2):
            error("GEM: pix_data: YUV not yet implemented");	//tigital
            return;
            break;
		// RGB
		case(3):
			color[0] = data[chRed] / 256.f;
			color[1] = data[chGreen] / 256.f;
			color[2] = data[chBlue] / 256.f;
			break;
		// RGBA
		case(4):
			color[0] = data[chRed] / 256.f;
			color[1] = data[chGreen] / 256.f;
			color[2] = data[chBlue] / 256.f;
			break;
		default :
			error("GEM: pix_data: unknown image format");
			return;
//			break;
	}

	float grayVal = (color[0] + color[1] + color[2]) / 3.f;
	t_atom atom[3];

	// send out the color information
	outlet_float(m_grayOut, (t_float)grayVal);
	SETFLOAT(&atom[0], (t_float)color[0]);
	SETFLOAT(&atom[1], (t_float)color[1]);
	SETFLOAT(&atom[2], (t_float)color[2]);
	outlet_list(m_colorOut, gensym("list"), 3, atom);	
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_data :: obj_setupCallback(t_class *classPtr)
{
    class_addbang(classPtr, (t_method)&pix_data::triggerMessCallback);
    class_addmethod(classPtr, (t_method)&pix_data::xPosCallback,
    	    gensym("xPos"), A_FLOAT, A_NULL); 
    class_addmethod(classPtr, (t_method)&pix_data::yPosCallback,
    	    gensym("yPos"), A_FLOAT, A_NULL); 
}
void pix_data :: triggerMessCallback(void *data)
{
    GetMyClass(data)->trigger();
}
void pix_data :: xPosCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->xPos(FLOAT_CLAMP((float)val));
}
void pix_data :: yPosCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->yPos(FLOAT_CLAMP((float)val));
}
