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
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_gain.h"

CPPEXTERN_NEW(pix_gain)

/////////////////////////////////////////////////////////
//
// pix_gain
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_gain :: pix_gain()
{
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("ft1"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("vec_gain"));
    m_gain[chRed] = m_gain[chGreen] = m_gain[chBlue] = m_gain[chAlpha] = 1.0f;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_gain :: ~pix_gain()
{ }

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_gain :: processImage(imageStruct &image)
{
    int datasize =  image.xsize * image.ysize;
    unsigned char *pixels = image.data;

	while(datasize--)
	{
		int red   = (int)(pixels[chRed] * m_gain[chRed]);
		int green = (int)(pixels[chGreen] * m_gain[chGreen]);
		int blue  = (int)(pixels[chBlue] * m_gain[chBlue]);
		int alpha = (int)(pixels[chAlpha] * m_gain[chAlpha]);
		pixels[chRed]	= CLAMP(red);
		pixels[chGreen] = CLAMP(green);
		pixels[chBlue]	= CLAMP(blue);
		pixels[chAlpha] = CLAMP(alpha);
		pixels += 4;
	}
}

/////////////////////////////////////////////////////////
// processGrayImage
//
/////////////////////////////////////////////////////////
void pix_gain :: processGrayImage(imageStruct &image)
{
    int datasize =  image.xsize * image.ysize;
    unsigned char *pixels = image.data;

	while (datasize--)
	{
		int gray = (int)(pixels[chGray] * m_gain[chRed]);
		pixels[chGray] = CLAMP(gray);
		pixels++;
	}
}

/////////////////////////////////////////////////////////
// vecGainMess
//
/////////////////////////////////////////////////////////
void pix_gain :: vecGainMess(int argc, t_atom *argv)
{
    if (argc >= 4) m_gain[chAlpha] = atom_getfloat(&argv[3]);
    else if (argc == 3) m_gain[chAlpha] = 1.0;
    else
    {
    	error("GEM: pix_gain: not enough gain values");
    	return;
    }
    m_gain[chRed] = atom_getfloat(&argv[0]);
    m_gain[chGreen] = atom_getfloat(&argv[1]);
    m_gain[chBlue] = atom_getfloat(&argv[2]);
    setPixModified();
}

/////////////////////////////////////////////////////////
// floatGainMess
//
/////////////////////////////////////////////////////////
void pix_gain :: floatGainMess(float gain)
{
    // assumption that the alpha should be one
    m_gain[chAlpha] = 1.0f;
    m_gain[chRed] = m_gain[chGreen] = m_gain[chBlue] = FLOAT_CLAMP(gain);
    setPixModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_gain :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&pix_gain::vecGainMessCallback,
    	    gensym("vec_gain"), A_GIMME, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_gain::floatGainMessCallback,
    	    gensym("ft1"), A_FLOAT, A_NULL);
}
void pix_gain :: vecGainMessCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->vecGainMess(argc, argv);
}
void pix_gain :: floatGainMessCallback(void *data, t_floatarg gain)
{
    GetMyClass(data)->floatGainMess((float)gain);
}
