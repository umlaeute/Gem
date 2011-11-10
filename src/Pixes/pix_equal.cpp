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
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_equal.h"

CPPEXTERN_NEW(pix_equal);

/////////////////////////////////////////////////////////
//
// pix_equal
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_equal :: pix_equal()
{
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("vec_thresh"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("ft1"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("ft2"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_equal :: ~pix_equal()
{ }

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_equal :: processRGBAImage(imageStruct &image)
{
    int datasize = image.xsize * image.ysize;

    unsigned char *base = image.data;

    while(datasize--) {
      base[chRed] = (base[chRed] <= m_upper[chRed] && base[chRed] >= m_lower[chRed])*255;
      base[chGreen] = (base[chGreen] <= m_upper[chGreen] && base[chGreen] >= m_lower[chGreen])*255;
      base[chBlue] = (base[chBlue] <= m_upper[chBlue] && base[chBlue] >= m_lower[chBlue])*255;
      base[chAlpha] = (base[chAlpha] <= m_upper[chAlpha] && base[chAlpha] >= m_lower[chAlpha])*255;
      base += 4;
    }
}

/////////////////////////////////////////////////////////
// processGrayImage
//
/////////////////////////////////////////////////////////
//void pix_equal :: processGrayImage(imageStruct &image)
//{
//    int datasize = image.xsize * image.ysize;

//    unsigned char *base = image.data;

//    while(datasize--)
//    {
//    if (base[chGray] < m_thresh[chRed]) base[chGray] = 0;
//    base++;
//    }    
//}

/////////////////////////////////////////////////////////
// vecThreshMess
//
/////////////////////////////////////////////////////////

void pix_equal :: vecUpperMess(int argc, t_atom *argv)
{
    if (argc >= 4)
    {
    	m_upper[chAlpha] = CLAMP(atom_getfloat(&argv[3]));
    }
    else if (argc == 3) m_upper[3] = 0;
    else
    {
    	error("not enough threshold values");
    	return;
    }
    
    m_upper[chRed] = CLAMP(atom_getfloat(&argv[0]));
    m_upper[chGreen] = CLAMP(atom_getfloat(&argv[1]));
    m_upper[chBlue] = CLAMP(atom_getfloat(&argv[2]));
    m_Y = CLAMP(atom_getfloat(&argv[0]));
    setPixModified();
}

void pix_equal :: vecUpperMess(int argc, t_atom *argv)
{
    if (argc >= 4)
    {
    	m_lower[chAlpha] = CLAMP(atom_getfloat(&argv[3]));
    }
    else if (argc == 3) m_lower[3] = 0;
    else
    {
    	error("not enough threshold values");
    	return;
    }
    
    m_lower[chRed] = CLAMP(atom_getfloat(&argv[0]));
    m_lower[chGreen] = CLAMP(atom_getfloat(&argv[1]));
    m_lower[chBlue] = CLAMP(atom_getfloat(&argv[2]));
    m_Y = CLAMP(atom_getfloat(&argv[0]));
    setPixModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_equal :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_equal::vecUpperMessCallback),
    	    gensym("vec_upper"), A_GIMME, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_equal::vecLowerMessCallback),
    	    gensym("vec_lower"), A_GIMME, A_NULL);
}
void pix_equal :: vecUpperMessCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->vecUpperMess(argc, argv);
}
void pix_equal :: vecLowerMessCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->vecLowerMess(argc, argv);
}
