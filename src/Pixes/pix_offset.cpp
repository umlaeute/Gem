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

#include "pix_offset.h"

CPPEXTERN_NEW(pix_offset)

/////////////////////////////////////////////////////////
//
// pix_offset
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_offset :: pix_offset()
{
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("ft1"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("vec_offset"));
    m_offset[chRed] = m_offset[chGreen] = m_offset[chBlue] = m_offset[chAlpha] = 0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_offset :: ~pix_offset()
{ }

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_offset :: processRGBAImage(imageStruct &image)
{
  int datasize =  image.xsize * image.ysize;
  unsigned char *pixels = image.data;

  unsigned char o_red=m_offset[chRed], o_green=m_offset[chGreen], o_blue=m_offset[chBlue], o_alpha=m_offset[chAlpha];

  while(datasize--) {
    //		int red   = (int)(pixels[chRed] * m_offset[]);
    //		int green = (int)(pixels[chGreen] * m_offset[chGreen]);
    //		int blue  = (int)(pixels[chBlue] * m_offset[chBlue]);
    //		int alpha = (int)(pixels[chAlpha] * m_offset[chAlpha]);
    pixels[chRed]   += o_red;
    pixels[chGreen] += o_green;
    pixels[chBlue]  += o_blue;
    pixels[chAlpha] += o_alpha;
    pixels += 4;
  }
}

/////////////////////////////////////////////////////////
// processGrayImage
//
/////////////////////////////////////////////////////////
void pix_offset :: processGrayImage(imageStruct &image)
{
    int datasize =  image.xsize * image.ysize*image.csize;
    unsigned char *pixels = image.data;
    unsigned char m_grey=m_offset[chRed];

    while(datasize--)*pixels++ += m_grey;
}

/////////////////////////////////////////////////////////
// vecOffsetMess
//
/////////////////////////////////////////////////////////
void pix_offset :: vecOffsetMess(int argc, t_atom *argv)
{
    if (argc >= 4) m_offset[chAlpha] = (int)(255*atom_getfloat(&argv[3]));
    else if (argc == 3) m_offset[chAlpha] = 0;
    else
    {
    	error("GEM: pix_offset: not enough offset values");
    	return;
    }
    m_offset[chRed]   = (int)(255*atom_getfloat(&argv[0]));
    m_offset[chGreen] = (int)(255*atom_getfloat(&argv[1]));
    m_offset[chBlue]  = (int)(255*atom_getfloat(&argv[2]));
    setPixModified();
}

/////////////////////////////////////////////////////////
// floatOffsetMess
//
/////////////////////////////////////////////////////////
void pix_offset :: floatOffsetMess(float foffset)
{
    // assumption that the alpha should be one
    m_offset[chAlpha] = 0;
    m_offset[chRed] = m_offset[chGreen] = m_offset[chBlue] = (int)(255*foffset);
    setPixModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_offset :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&pix_offset::vecOffsetMessCallback,
    	    gensym("vec_offset"), A_GIMME, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_offset::floatOffsetMessCallback,
    	    gensym("ft1"), A_FLOAT, A_NULL);
}
void pix_offset :: vecOffsetMessCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->vecOffsetMess(argc, argv);
}
void pix_offset :: floatOffsetMessCallback(void *data, t_floatarg offset)
{
    GetMyClass(data)->floatOffsetMess((float)offset);
}
