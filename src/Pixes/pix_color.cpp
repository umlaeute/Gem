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

#include "pix_color.h"

CPPEXTERN_NEW(pix_color)

/////////////////////////////////////////////////////////
//
// pix_color
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_color :: pix_color()
{
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("vec_gain"));
    m_color[0] = m_color[1] = m_color[2] = 255;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_color :: ~pix_color()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_color :: processImage(imageStruct &image)
{
    int i = image.xsize * image.ysize;
    
    unsigned char *base = image.data;
	while (i--)
    {
      base[chRed] = m_color[0];
      base[chGreen] = m_color[1];
      base[chBlue] = m_color[2];
      base += 4;
    }
}

void pix_color :: processYUVImage(imageStruct &image)
{
    post("pix_color: YUV not yet implemented :-(");
}
/////////////////////////////////////////////////////////
// vecGainMess
//
/////////////////////////////////////////////////////////
void pix_color :: vecGainMess(float red, float green, float blue)
{
    m_color[0] = CLAMP(red * 255);
    m_color[1] = CLAMP(green * 255);
    m_color[2] = CLAMP(blue * 255);
    setPixModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_color :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&pix_color::vecGainMessCallback,
    	    gensym("vec_gain"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
}
void pix_color :: vecGainMessCallback(void *data, t_floatarg red, t_floatarg green, t_floatarg blue)
{
    GetMyClass(data)->vecGainMess((float)red, (float)green, (float)blue);
}
