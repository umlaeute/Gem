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

#include "pix_threshold.h"

CPPEXTERN_NEW(pix_threshold)

/////////////////////////////////////////////////////////
//
// pix_threshold
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_threshold :: pix_threshold()
{
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("ft1"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("vec_thresh"));
    m_thresh[chRed] = m_thresh[chGreen] = m_thresh[chBlue] = m_thresh[chAlpha] = 0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_threshold :: ~pix_threshold()
{ }

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_threshold :: processImage(imageStruct &image)
{
    int datasize = image.xsize * image.ysize;

    unsigned char *base = image.data;

    while(datasize--)
    {
		if (base[chRed] < m_thresh[chRed]) base[chRed] = 0;
		if (base[chGreen] < m_thresh[chGreen]) base[chGreen] = 0;
		if (base[chBlue] < m_thresh[chBlue]) base[chBlue] = 0;
		if (base[chAlpha] < m_thresh[chAlpha]) base[chAlpha] = 0;
		base += 4;
    }    
}

/////////////////////////////////////////////////////////
// processGrayImage
//
/////////////////////////////////////////////////////////
void pix_threshold :: processGrayImage(imageStruct &image)
{
    int datasize = image.xsize * image.ysize;

    unsigned char *base = image.data;

    while(datasize--)
    {
		if (base[chGray] < m_thresh[chRed]) base[chGray] = 0;
		base++;
    }    
}

/////////////////////////////////////////////////////////
// processYUVImage
//
/////////////////////////////////////////////////////////
void pix_threshold :: processYUVImage(imageStruct &image)
{
    post("pix_threshold: YUV not yet implemented :-(");
}

/////////////////////////////////////////////////////////
// vecThreshMess
//
/////////////////////////////////////////////////////////
void pix_threshold :: vecThreshMess(int argc, t_atom *argv)
{
    if (argc >= 4)
    {
    	m_thresh[chAlpha] = CLAMP(atom_getfloat(&argv[3]) * 255);
    }
    else if (argc == 3) m_thresh[3] = 0;
    else
    {
    	error("GEM: pix_threshold: not enough threshold values");
    	return;
    }
    
    m_thresh[chRed] = CLAMP(atom_getfloat(&argv[0]) * 255);
    m_thresh[chGreen] = CLAMP(atom_getfloat(&argv[1]) * 255);
    m_thresh[chBlue] = CLAMP(atom_getfloat(&argv[2]) * 255);

    setPixModified();
}

/////////////////////////////////////////////////////////
// floatThreshMess
//
/////////////////////////////////////////////////////////
void pix_threshold :: floatThreshMess(float thresh)
{
    m_thresh[chRed] = m_thresh[chGreen] = m_thresh[chBlue] = CLAMP(thresh * 255);
    // assumption that the alpha threshold should be zero
    m_thresh[chAlpha] = 0;
    setPixModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_threshold :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&pix_threshold::vecThreshMessCallback,
    	    gensym("vec_thresh"), A_GIMME, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_threshold::floatThreshMessCallback,
    	    gensym("ft1"), A_FLOAT, A_NULL);
}
void pix_threshold :: vecThreshMessCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->vecThreshMess(argc, argv);
}
void pix_threshold :: floatThreshMessCallback(void *data, t_floatarg thresh)
{
    GetMyClass(data)->floatThreshMess((float)thresh);
}
