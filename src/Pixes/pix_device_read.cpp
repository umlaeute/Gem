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

CPPEXTERN_NEW(pix_device_read)

pix_device_read :: pix_device_read() : 
  m_deviceReader(NULL)
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
  
  
  if( vioframe_ptr == NULL)
  {
    post("Could not get frame data.");
    return;
  }
  else
    post("Got frame data.");
    
  // check if image size changed
  if( m_image.image.xsize != m_deviceReader->getWidth() ||
      m_image.image.ysize != m_deviceReader->getHeight() ||
      m_image.image.csize != m_deviceReader->getColorSize() )
    reallocate_m_image();
  
  // read frame data into m_image
  unsigned char *image_ptr = m_image.image.data;
  int i = m_image.image.xsize * m_image.image.ysize * m_image.image.csize ;
  while(i--)
  {
    *(image_ptr++) = *(vioframe_ptr++);
  }
  
//   // set flag if we have a new film
//   if(m_newfilm)
//   {
//     m_image.newfilm = true;
//     m_newfilm = false;
//   }
//   m_image.newimage = true;

  m_image.newimage = true;
  state->image = &m_image;
}

void pix_device_read :: openDevice(t_symbol *dev)
{
  closeDevice();
  
  // open device
  if(!(m_deviceReader->openDevice(dev->s_name)))
  {
    error("pix_device_read: could not open device %s", dev->s_name);
    return;
  }
}

void pix_device_read :: openDevice(int dev)
{
  closeDevice();
  
  // open device
  if(!(m_deviceReader->openDevice(dev)))
  {
    error("pix_device_read: could not open device %d", dev);
    return;
  }
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

void pix_device_read :: channelMess(int channel, t_float freq)
{
  if(m_deviceReader)
    m_deviceReader->setChannel(channel);  ///TODO auch noch freq setting
}

void pix_device_read :: normMess(t_symbol *s)
{
  if(m_deviceReader)
    m_deviceReader->setTVNorm(s->s_name);
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

void pix_device_read :: enumerateMess()
{
  error("enumerate not supported on this OS");
}

void pix_device_read :: qualityMess(int dev) 
{
//   if (m_deviceReader)
//     m_deviceReader->setQuality(dev);
}

void pix_device_read :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classpix_device_read,gensym("pix_video"),A_NULL);

   class_addmethod(classPtr, (t_method)&pix_device_read::openMessCallback,
              gensym("open"), A_DEFSYM, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_device_read::startCallback,
                  gensym("start"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_device_read::stopCallback,
                  gensym("stop"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_device_read::seekCallback,
                  gensym("seek"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_device_read::csCallback,
                  gensym("forceColorspace"), A_DEFSYM, A_NULL);

  
    class_addmethod(classPtr, (t_method)&pix_device_read::normMessCallback,
    	    gensym("norm"), A_SYMBOL, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_device_read::channelMessCallback,
    	    gensym("channel"), A_GIMME, A_NULL);
     class_addmethod(classPtr, (t_method)&pix_device_read::enumerateMessCallback,
    	    gensym("enumerate"), A_NULL);
    class_addmethod(classPtr, (t_method)&pix_device_read::qualityMessCallback,
	    gensym("quality"), A_FLOAT, A_NULL);
}

void pix_device_read :: openMessCallback(void *data, t_symbol*s)
{
  GetMyClass(data)->openDevice(s);
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
  GetMyClass(data)->m_deviceReader->seekDevice();
}

void pix_device_read :: csCallback(void *data, t_symbol *s)
{
  GetMyClass(data)->forceColorspace(s);
}

void pix_device_read :: normMessCallback(void *data, t_symbol*s)
{
  GetMyClass(data)->normMess(s);
}
void pix_device_read :: channelMessCallback(void *data, t_symbol*s, int argc, t_atom*argv)
{
  if (argc!=1&&argc!=2)return;
  int chan = atom_getint(argv);
  t_float freq = (argc==1)?0:atom_getfloat(argv+1);
  GetMyClass(data)->channelMess((int)chan, freq);
}

void pix_device_read :: enumerateMessCallback(void *data)
{
  GetMyClass(data)->enumerateMess();
}
void pix_device_read :: qualityMessCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->qualityMess((int)state);
}
