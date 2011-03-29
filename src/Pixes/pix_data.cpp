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

CPPEXTERN_NEW(pix_data);

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

  int maxX= m_pixRight->image.xsize;
  int maxY= m_pixRight->image.ysize;

  int xPos = static_cast<int>(m_position[0] * maxX);
  int yPos = static_cast<int>(m_position[1] * maxY);
  t_float red, green, blue, alpha, grey;
  unsigned char r, g, b, a, G;

  if(xPos<0)xPos=0; if(xPos>=maxX)xPos=maxX-1;
  if(yPos<0)yPos=0; if(yPos>=maxY)yPos=maxY-1;

  m_pixRight->image.getRGB(xPos, yPos, &r, &g, &b, &a);
  m_pixRight->image.getGrey(xPos, yPos, &G);

  red   = r / 255.;
  green = g / 255.;
  blue  = b / 255.;
  alpha = a / 255.;
  grey  = G / 255.;
  
  t_atom atom[4];
  // send out the color information
  outlet_float(m_grayOut, grey);
  SETFLOAT(&atom[0], red);
  SETFLOAT(&atom[1], green);
  SETFLOAT(&atom[2], blue);
  SETFLOAT(&atom[3], alpha);
  outlet_list(m_colorOut, gensym("list"), 4, atom);	
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_data :: obj_setupCallback(t_class *classPtr)
{
    class_addbang(classPtr, reinterpret_cast<t_method>(&pix_data::triggerMessCallback));
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_data::xPosCallback),
    	    gensym("xPos"), A_FLOAT, A_NULL); 
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_data::yPosCallback),
    	    gensym("yPos"), A_FLOAT, A_NULL); 
}
void pix_data :: triggerMessCallback(void *data)
{
    GetMyClass(data)->trigger();
}
void pix_data :: xPosCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->xPos(FLOAT_CLAMP(val));
}
void pix_data :: yPosCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->yPos(FLOAT_CLAMP(val));
}
