////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_snap.h"

#include "Base/GemMan.h"
#include "Base/GemCache.h"

CPPEXTERN_NEW_WITH_GIMME(pix_snap)

/////////////////////////////////////////////////////////
//
// pix_snap
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_snap :: pix_snap(int argc, t_atom *argv)
    	  : m_originalImage(NULL)
{
  m_pixBlock.image = m_imageStruct;
  m_pixBlock.image.data = NULL;
  if (argc == 4){
    m_x = (int)atom_getfloat(&argv[0]);
    m_y = (int)atom_getfloat(&argv[1]);
    m_width = (int)atom_getfloat(&argv[2]);
    m_height = (int)atom_getfloat(&argv[3]);
  } else if (argc == 2)	{
    m_x = m_y = 0;
    m_width = (int)atom_getfloat(&argv[0]);
    m_height = (int)atom_getfloat(&argv[1]);
  } else if (argc == 0)	{
    m_x = m_y = 0;
    m_width = m_height = 128;
  } else {
    error("GEM: pix_snap: needs 0, 2, or 4 values");
    m_x = m_y = 0;
    m_width = m_height = 128;
  }
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("vert_pos"));
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("vert_size"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_snap :: ~pix_snap()
{
    cleanImage();
}

/////////////////////////////////////////////////////////
// snapMess
//
/////////////////////////////////////////////////////////
void pix_snap :: snapMess()
{
    if ( !GemMan::windowExists() )
		return;
        
	if (m_width <= 0 || m_height <= 0)
	{
		error("GEM: pix_snap: Illegal size");
		return;
	}
	// do we need to remake the data?
	int makeNew = 0;

    // release previous data
    if (m_originalImage)
    {
		if (m_originalImage->xsize != m_width ||
			m_originalImage->ysize != m_height)
		{
			m_originalImage->clear();
			delete m_originalImage;
			m_originalImage = NULL;
			makeNew = 1;
		}
	}
	else
		makeNew = 1;

    if (makeNew)
	{
		m_originalImage = new imageStruct;
		m_originalImage->xsize = m_width;
		m_originalImage->ysize = m_height;
		m_originalImage->csize = 4;
                #ifndef MACOSX
		m_originalImage->type  = GL_UNSIGNED_BYTE;
		m_originalImage->format = GL_RGBA;
                #else
                m_originalImage->type  = GL_UNSIGNED_INT_8_8_8_8_REV;
		m_originalImage->format = GL_BGRA_EXT;
                #endif
		m_originalImage->allocate(m_originalImage->xsize * m_originalImage->ysize * m_originalImage->csize);
	}

    glReadPixels(m_x, m_y, m_width, m_height,
    	    	 m_originalImage->format, m_originalImage->type, m_originalImage->data);    
           
	if (m_cache)
		m_cache->resendImage = 1;

    post("GEM: pix_snap: snapped image"); 
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_snap :: render(GemState *state)
{
    // if we don't have an image, just return
    if (!m_originalImage)
		return;
    
    // do we need to reload the image?    
    if (m_cache->resendImage)
    {
      m_originalImage->refreshImage(&m_pixBlock.image);
    	m_pixBlock.newimage = 1;
    	m_cache->resendImage = 0;
    }
    
    state->image = &m_pixBlock;
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_snap :: postrender(GemState *state)
{
    m_pixBlock.newimage = 0;
    state->image = NULL;
}

/////////////////////////////////////////////////////////
// sizeMess
//
/////////////////////////////////////////////////////////
void pix_snap :: sizeMess(int width, int height)
{
	m_width = width;
    m_height = height;
}

/////////////////////////////////////////////////////////
// posMess
//
/////////////////////////////////////////////////////////
void pix_snap :: posMess(int x, int y)
{
    m_x = x;
    m_y = y;
}

/////////////////////////////////////////////////////////
// cleanImage
//
/////////////////////////////////////////////////////////
void pix_snap :: cleanImage()
{
    // release previous data
    if (m_originalImage)
    {
      delete m_originalImage;
      m_originalImage = NULL;

      m_pixBlock.image.clear();
    }
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void pix_snap :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&pix_snap::snapMessCallback,
    	    gensym("snap"), A_NULL);

    class_addmethod(classPtr, (t_method)&pix_snap::sizeMessCallback,
    	    gensym("vert_size"), A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_snap::posMessCallback,
    	    gensym("vert_pos"), A_FLOAT, A_FLOAT, A_NULL);
}
void pix_snap :: snapMessCallback(void *data)
{
    GetMyClass(data)->snapMess();
}
void pix_snap :: sizeMessCallback(void *data, t_floatarg width, t_floatarg height)
{
    GetMyClass(data)->sizeMess((int)width, (int)height);
}
void pix_snap :: posMessCallback(void *data, t_floatarg x, t_floatarg y)
{
    GetMyClass(data)->posMess((int)x, (int)y);
}
