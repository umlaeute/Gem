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
  float red, green, blue, grey;
  unsigned char r, g, b, G;

  m_pixRight->image.getRGB(xPos, yPos, &r, &g, &b);
  m_pixRight->image.getGrey(xPos, yPos, &G);

  red   = r / 256.f;
  green = g / 256.f;
  blue  = b / 256.f;
  grey  = G / 256.f;
  
	t_atom atom[3];
	// send out the color information
	outlet_float(m_grayOut, (t_float)grey);
	SETFLOAT(&atom[0], (t_float)red);
	SETFLOAT(&atom[1], (t_float)green);
	SETFLOAT(&atom[2], (t_float)blue);
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
