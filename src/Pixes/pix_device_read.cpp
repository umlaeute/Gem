////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// Copyright (c) 2007, Thomas Holzmann
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_device_read.h"

#include "Base/GemMan.h"

CPPEXTERN_NEW(pix_device_read)

pix_device_read :: pix_device_read() : 
  m_newfilm(false), m_deviceReader(NULL)
{
  m_deviceReader = &m_kernel.getDeviceReadServer().getPlugin();
}

pix_device_read :: ~pix_device_read()
{
  closeDevice();
}

void pix_device_read :: render(GemState *state)
{
  // get pointer to the frame data
  unsigned char *vioframe_ptr = m_deviceReader->getFrameData();
  
  if( !vioframe_ptr ) return;

  // check if image size changed
  if( m_image.image.xsize != m_deviceReader->getWidth() ||
      m_image.image.ysize != m_deviceReader->getHeight() ||
      m_image.image.csize != m_deviceReader->getColorSize() )
  {
    reallocate_m_image();
  }

  // read frame data into m_image
  unsigned char *image_ptr = m_image.image.data;
  int i = m_image.image.xsize * m_image.image.ysize * m_image.image.csize;
  while(i--)
  {
    *(image_ptr++) = *(vioframe_ptr++);
  }
  
  // set flag if we have a new film
  if(m_newfilm)
  {
    m_image.newfilm = true;
    m_newfilm = false;
  }
  m_image.newimage = true;

  m_image.newimage = true;
  state->image = &m_image;
}

void pix_device_read :: openDevice(t_symbol *name, t_symbol *dev)
{
  closeDevice();

  bool suc=false;

  // get GEM framerate
  m_deviceReader->setFramerate( GemMan::getFramerate() );

  // open device
  if( dev )
    suc = m_deviceReader->openDevice(name->s_name, dev->s_name);
  else
    suc = m_deviceReader->openDevice(name->s_name);

  if( !suc ) error("could not open device %s", dev->s_name);
}

void pix_device_read :: closeDevice()
{
  m_deviceReader->closeDevice();
}

void pix_device_read :: postrender(GemState *state)
{
  if (state && state->image)
    state->image->newimage = 0;
}

void pix_device_read :: forceColorspace(t_symbol *cs)
{
  char c =*cs->s_name;
  switch (c)
  {
    case 'g': case 'G':
      m_deviceReader->forceColorspace(VideoIO_::GRAY);
      break;
    case 'y': case 'Y':
      m_deviceReader->forceColorspace(VideoIO_::YUV422);
      break;
    case 'r': case 'R': 
      if(gensym("RGB")==cs||gensym("rgb")==cs)
        m_deviceReader->forceColorspace(VideoIO_::RGB);
      else
        m_deviceReader->forceColorspace(VideoIO_::RGBA);
      break;
    default:
      error("colorspace must be 'RGBA', 'YUV' or 'Gray'");
  }
}

void pix_device_read :: reallocate_m_image()
{
  // allocate memory for m_image
  m_image.image.xsize = m_deviceReader->getWidth();
  m_image.image.ysize = m_deviceReader->getHeight();
  m_image.image.type = GL_UNSIGNED_BYTE; /// TODO under OSX ?
  m_image.image.csize = m_deviceReader->getColorSize();

  switch( m_deviceReader->getColorspace() )
  {
    case VideoIO_::GRAY:
      m_image.image.format = GL_LUMINANCE;
      break;
    case VideoIO_::YUV422:
      m_image.image.format = GL_YCBCR_422_GEM;
      break;
    case VideoIO_::RGB:
      m_image.image.format = GL_RGB;
      break;
    case VideoIO_::RGBA:
      m_image.image.format = GL_RGBA;
      break;
    default:
      error("error in reallocate_m_image");
  }

  m_image.image.reallocate();
  m_newfilm = true;
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_device_read :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classpix_device_read,
                    gensym("pix_device_read"),A_NULL);

  class_addmethod(classPtr, (t_method)&pix_device_read::openMessCallback,
              gensym("open"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_device_read::startCallback,
              gensym("start"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_device_read::stopCallback,
              gensym("stop"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_device_read::seekCallback,
              gensym("seek"), A_DEFFLOAT, A_NULL);

  class_addmethod(classPtr, (t_method)&pix_device_read::csCallback,
              gensym("forceColorspace"), A_DEFSYM, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_device_read::setDVQualityCallback,
              gensym("quality"), A_FLOAT, A_NULL);
}

void pix_device_read :: openMessCallback(void *data, t_symbol *s, int argc, t_atom*argv)
{
  switch(argc)
  {
    case 2:
      GetMyClass(data)->openDevice( atom_getsymbol(argv),
                                    atom_getsymbol(argv+1) );
      break;
    case 1:
      GetMyClass(data)->openDevice( atom_getsymbol(argv), 0 );
      break;
    default:
      GetMyClass(data)->error("openDevice name [device]");
  }
}

void pix_device_read :: startCallback(void *data, t_floatarg start)
{
  GetMyClass(data)->m_deviceReader->startDevice();
}

void pix_device_read :: stopCallback(void *data, t_floatarg stop)
{
  GetMyClass(data)->m_deviceReader->stopDevice();
}

void pix_device_read :: seekCallback(void *data, t_floatarg seek)
{
  GetMyClass(data)->m_deviceReader->seekDevice( (int)seek );
}

void pix_device_read :: csCallback(void *data, t_symbol *s)
{
  GetMyClass(data)->forceColorspace(s);
}

void pix_device_read :: setDVQualityCallback(void *data, t_floatarg qual)
{
  GetMyClass(data)->m_deviceReader->setDVQuality( (int)qual );
}
