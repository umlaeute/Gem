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


#include "pix_write.h"

#include "Base/GemMan.h"
#include "Base/GemCache.h"

#include <stdio.h>
#include <string.h>

CPPEXTERN_NEW_WITH_GIMME(pix_write)

  /////////////////////////////////////////////////////////
//
// pix_write
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_write :: pix_write(int argc, t_atom *argv)
  : m_originalImage(NULL)
{
  m_xoff = m_yoff = 0;
  m_width = m_height = 0;
  if (argc == 4) {
    m_xoff = (int)atom_getfloat(&argv[0]);
    m_yoff = (int)atom_getfloat(&argv[1]);
    m_width = (int)atom_getfloat(&argv[2]);
    m_height = (int)atom_getfloat(&argv[3]);
  } else if (argc == 2) {
    m_width = (int)atom_getfloat(&argv[0]);
    m_height = (int)atom_getfloat(&argv[1]);
  } else if (argc != 0){
    error("GEM: pix_write: needs 0, 2, or 4 values");
    m_xoff = m_yoff = 0;
    m_width = m_height = 128;
  }

  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("vert_pos"));
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("vert_size"));

  m_automatic = false;
  m_autocount = 0;
  m_filetype=0;
  sprintf(m_pathname, "gem");
    
  m_banged = false;
     
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_write :: ~pix_write()
{
  cleanImage();
}

/////////////////////////////////////////////////////////
// writeMess
//
/////////////////////////////////////////////////////////
void pix_write :: doWrite()
{
  if ( !GemMan::windowExists() )
    return;
  
  int width  = (m_width > 0)?m_width :GemMan::m_width;
  int height = (m_height> 0)?m_height:GemMan::m_height;

  // do we need to remake the data?
  int makeNew = 0;
  
  // release previous data
  if (m_originalImage)
    {
      if (m_originalImage->xsize != width ||
	  m_originalImage->ysize != height)
	{
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
      m_originalImage->xsize = width;
      m_originalImage->ysize = height;
      m_originalImage->type  = GL_UNSIGNED_BYTE;

      m_originalImage->csize = 3;
      m_originalImage->format = GL_RGB;

      m_originalImage->allocate(m_originalImage->xsize * m_originalImage->ysize * m_originalImage->csize);
    }
  
  glReadPixels(m_xoff, m_yoff, width, height,
	       m_originalImage->format, m_originalImage->type, m_originalImage->data); 
 
  mem2image(m_originalImage, m_filename, m_filetype);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_write :: render(GemState *state)
{
  if (m_automatic || m_banged) {
    char *extension;
    if (m_filetype<0)m_filetype=0;
    if (m_filetype==0) extension="tif";
    else {
#if 0
      post("pix_write: you can only write TIFF-files ! (forcing to TIFF)");
      m_filetype=0;
      extension="tif";
#else
      extension="jpg";
#endif
    }
    
    sprintf(m_filename, "%s%d.%s", m_pathname, m_autocount+10000, extension);
    m_filename[strlen(m_pathname)]-=1;
    
    m_autocount++;
    m_banged = false;
    doWrite();
  }
}


/////////////////////////////////////////////////////////
// sizeMess
//
/////////////////////////////////////////////////////////
void pix_write :: sizeMess(int width, int height)
{
  m_width = width;
  m_height = height;
}

/////////////////////////////////////////////////////////
// posMess
//
/////////////////////////////////////////////////////////
void pix_write :: posMess(int x, int y)
{
  m_xoff = x;
  m_yoff = y;
}

void pix_write :: fileMess(int argc, t_atom *argv)
{
  char *extension = ".tif";
  if (argc) {
    if (argv->a_type == A_SYMBOL) {
      atom_string(argv++, m_pathname, 80);
      argc--;
      sprintf(m_filename, "%s.%s", m_pathname, extension);
    }
    if (argc>0)
      m_filetype = atom_getint(argv);
  }

  m_autocount = 0;

  //  GetMyClass(data)->doWrite();
}

/////////////////////////////////////////////////////////
// cleanImage
//
/////////////////////////////////////////////////////////
void pix_write :: cleanImage()
{
  // release previous data
  if (m_originalImage)
    {
      delete m_originalImage;
      m_originalImage = NULL;
    }
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void pix_write :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&pix_write::fileMessCallback,
		  gensym("file"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_write::autoMessCallback,
		  gensym("auto"), A_FLOAT, A_NULL);
  class_addbang(classPtr, (t_method)&pix_write::bangMessCallback);

  class_addmethod(classPtr, (t_method)&pix_write::sizeMessCallback,
		  gensym("vert_size"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_write::posMessCallback,
		  gensym("vert_pos"), A_FLOAT, A_FLOAT, A_NULL);
}

void pix_write :: fileMessCallback(void *data, t_symbol *s, int argc, t_atom *argv)
{
  GetMyClass(data)->fileMess(argc, argv);
}
void pix_write :: autoMessCallback(void *data, t_floatarg on)
{
  GetMyClass(data)->m_automatic=(on!=0);
}
void pix_write :: bangMessCallback(void *data)
{
  GetMyClass(data)->m_banged=true;
}

void pix_write :: sizeMessCallback(void *data, t_floatarg width, t_floatarg height)
{
  GetMyClass(data)->sizeMess((int)width, (int)height);
}
void pix_write :: posMessCallback(void *data, t_floatarg x, t_floatarg y)
{
  GetMyClass(data)->posMess((int)x, (int)y);
}
