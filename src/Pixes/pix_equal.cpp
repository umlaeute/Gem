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
//    Copyright (c) 2012 Ricardo Fabbri. labmacambira.sf.net rfabbri@gmail.com
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
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("vec_lower"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("vec_upper"));
    m_upper[0] = m_upper[1] =  m_upper[2] = m_upper[3] = 255;
    m_lower[0] = m_lower[1] = m_lower[2] = m_lower[3] = 0;
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
      unsigned char pixval = 
        (base[chRed] <= m_upper[chRed] && base[chRed] >= m_lower[chRed] &&
        base[chGreen] <= m_upper[chGreen] && base[chGreen] >= m_lower[chGreen] &&
        base[chBlue] <= m_upper[chBlue] && base[chBlue] >= m_lower[chBlue])*255;

      base[chRed] = base[chGreen] = base[chBlue] = pixval;
      base[chAlpha] = 255;
      base += 4;
    }
}

void pix_equal :: vecUpperBoundMess(int argc, t_atom *argv)
{
    if (argc >= 4)
    {
    	m_upper[chAlpha] = (atom_getfloat(&argv[3]));
    }
    else if (argc != 3)
    {
    	error("not enough upper bound values");
    	return;
    }
    
    m_upper[chRed] = (atom_getfloat(&argv[0]));
    m_upper[chGreen] = (atom_getfloat(&argv[1]));
    m_upper[chBlue] = (atom_getfloat(&argv[2]));
    setPixModified();
}

void pix_equal :: vecLowerBoundMess(int argc, t_atom *argv)
{
    if (argc >= 4)
    {
    	m_lower[chAlpha] = (atom_getfloat(&argv[3]));
    }
    else if (argc != 3)
    {
    	error("not enough lower bound values");
    	return;
    }
    
    m_lower[chRed] = (atom_getfloat(&argv[0]));
    m_lower[chGreen] = (atom_getfloat(&argv[1]));
    m_lower[chBlue] = (atom_getfloat(&argv[2]));
    setPixModified();
}

void pix_equal :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_equal::vecLowerMessCallback),
    	    gensym("vec_lower"), A_GIMME, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_equal::vecUpperMessCallback),
    	    gensym("vec_upper"), A_GIMME, A_NULL);
}
void pix_equal :: vecUpperMessCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->vecUpperBoundMess(argc, argv);
}
void pix_equal :: vecLowerMessCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->vecLowerBoundMess(argc, argv);
}
