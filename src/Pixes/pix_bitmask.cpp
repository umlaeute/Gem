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

#include "pix_bitmask.h"

CPPEXTERN_NEW(pix_bitmask)

/////////////////////////////////////////////////////////
//
// pix_bitmask
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_bitmask :: pix_bitmask()
{
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("ft1"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("vec_mask"));

    m_mask[chRed] = m_mask[chGreen] = m_mask[chBlue] = m_mask[chAlpha] = 255;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_bitmask :: ~pix_bitmask()
{ }

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_bitmask :: processImage(imageStruct &image)
{
    int datasize = image.xsize * image.ysize;
	unsigned char *pixels = image.data;
    
	while(datasize--)
	{
		pixels[chRed] &= m_mask[chRed];
		pixels[chGreen] &= m_mask[chGreen];
		pixels[chBlue] &= m_mask[chBlue];
		pixels[chAlpha] &= m_mask[chAlpha];
		pixels += 4;
	}
}

/////////////////////////////////////////////////////////
// processGrayImage
//
/////////////////////////////////////////////////////////
void pix_bitmask :: processGrayImage(imageStruct &image)
{
    int datasize = image.xsize * image.ysize;
	unsigned char *pixels = image.data;
    
	while(datasize--)
	{
		pixels[chGray] &= m_mask[chRed];
		pixels++;
	}
}

/////////////////////////////////////////////////////////
// vecMaskMess
//
/////////////////////////////////////////////////////////
void pix_bitmask :: vecMaskMess(int argc, t_atom *argv)
{
    if (argc >= 4) m_mask[chAlpha] = atom_getint(&argv[3]);
    else if (argc == 3) m_mask[chAlpha] = 255;
    else
    {
    	error("GEM: pix_bitmask: not enough mask values");
    	return;
    }
    m_mask[chRed] = atom_getint(&argv[0]);
    m_mask[chGreen] = atom_getint(&argv[1]);
    m_mask[chBlue] = atom_getint(&argv[2]);
    setPixModified();
}

/////////////////////////////////////////////////////////
// intMaskMess
//
/////////////////////////////////////////////////////////
void pix_bitmask :: intMaskMess(int mask)
{
    // assumption that the alpha should be 255
    m_mask[chAlpha] = 255;
    m_mask[chRed] = m_mask[chGreen] = m_mask[chBlue] = mask;
    setPixModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_bitmask :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&pix_bitmask::vecMaskMessCallback,
    	    gensym("vec_mask"), A_GIMME, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_bitmask::floatMaskMessCallback,
    	    gensym("ft1"), A_FLOAT, A_NULL);
}
void pix_bitmask :: vecMaskMessCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->vecMaskMess(argc, argv);
}
void pix_bitmask :: floatMaskMessCallback(void *data, t_floatarg mask)
{
    GetMyClass(data)->intMaskMess((int)mask);
}
