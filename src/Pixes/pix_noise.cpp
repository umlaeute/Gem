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

/////////////////////////////////////////////////////////
//
//  pix_noise
//
//  2011
//  Nicolas Montgermont
//  mailto:nicolas_montgermont@yahoo.fr
//
/////////////////////////////////////////////////////////



// this is to paint easily your own pictures by passing a package of floats that contain all necessary image data)

#include "pix_noise.h"
#include "Gem/State.h"

#include <string.h>

CPPEXTERN_NEW_WITH_TWO_ARGS(pix_noise, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_noise :: pix_noise(t_floatarg xsize, t_floatarg ysize)
{
  int dataSize;
  if (xsize < 1) xsize = 256;
  if (ysize < 1) ysize = 256;
  int randInit = 307*1319;
  randNow = randInit;

  m_pixBlock.image = m_imageStruct;
  m_pixBlock.image.xsize = (int)xsize;
  m_pixBlock.image.ysize = (int)ysize;
  m_pixBlock.image.csize = 4;
  m_pixBlock.image.format = GL_RGBA;
  m_pixBlock.image.type = GL_UNSIGNED_BYTE;

  dataSize = m_pixBlock.image.xsize * m_pixBlock.image.ysize *
    m_pixBlock.image.csize * sizeof(unsigned char);
  m_pixBlock.image.allocate(dataSize);
  memset(m_pixBlock.image.data, 0, dataSize);
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("data"));
	bangMess();	
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_noise :: ~pix_noise()
{
  cleanPixBlock();
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_noise :: render(GemState *state)
{
  state->set(GemState::_PIX,&m_pixBlock);
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_noise :: startRendering()
{
    m_pixBlock.newimage = 1;
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_noise :: postrender(GemState *state)
{
    m_pixBlock.newimage = 0;
    //state->image = NULL;
}

float pix_noise :: pix_random()
{
	randNow = randNow * 435898247 + 382842987;
	return ((float)((randNow & 0x7fffffff) - 0x40000000)) * (float)(1.0 / 0x40000000);
}

/////////////////////////////////////////////////////////
// DATAMess
//
/////////////////////////////////////////////////////////
void pix_noise :: bangMess()
{
	int picturesize = m_pixBlock.image.xsize * m_pixBlock.image.ysize, counter, n;
	unsigned char *buffer = m_pixBlock.image.data;
	
	//	argc--;
	memset(buffer, 0, picturesize*m_pixBlock.image.csize*sizeof(unsigned char));
	
	switch (m_mode) {
		case GL_RGB:
			//n = argc/3;
			counter=picturesize;
			while (counter--) {
				buffer[0] = (unsigned char)(255.*pix_random()); // red
				buffer[1] = (unsigned char)(255.*pix_random()); // green
				buffer[2] = (unsigned char)(255.*pix_random()); // blue
				buffer[3] = 0;					     // alpha
				//argv+=3; 
				buffer+=4;
			}
			break;
		case GL_LUMINANCE:
			counter=picturesize;
			while (counter--) {
				buffer[0] = buffer[1] = buffer[2] = (unsigned char)(255.*pix_random());	// rgb
				buffer[3] = 0;									// alpha
				//argv++;	
				buffer+=4;
			}
			break;
		case GL_YCBCR_422_GEM:
			// ?
			break;
		default:
			//n = argc/4;
			counter=picturesize;
			while (counter--) {
				buffer[0] = (unsigned char)(255.*pix_random()); // red
				buffer[1] = (unsigned char)(255.*pix_random()); // green
				buffer[2] = (unsigned char)(255.*pix_random()); // blue
				buffer[3] = (unsigned char)(255.*pix_random()); // alpha
				//argv+=4; 
				buffer+=4;
			}
	}
	m_pixBlock.newimage = 1;
}


/////////////////////////////////////////////////////////
// DATAMess
//
/////////////////////////////////////////////////////////
void pix_noise :: DATAMess(int argc, t_atom *argv)
{
  int picturesize = m_pixBlock.image.xsize * m_pixBlock.image.ysize, counter, n;
  unsigned char *buffer = m_pixBlock.image.data;

  //	argc--;
  memset(buffer, 0, picturesize*m_pixBlock.image.csize*sizeof(unsigned char));

  switch (m_mode) {
  case GL_RGB:
    n = argc/3;
    counter=(picturesize<n)?picturesize:n;
    while (counter--) {
      buffer[0] = (unsigned char)(255.*atom_getfloat(&argv[0])); // red
      buffer[1] = (unsigned char)(255.*atom_getfloat(&argv[1])); // green
      buffer[2] = (unsigned char)(255.*atom_getfloat(&argv[2])); // blue
      buffer[3] = 0;					     // alpha
      argv+=3; buffer+=4;
    }
    break;
  case GL_LUMINANCE:
    counter=(picturesize<argc)?picturesize:argc;
    while (counter--) {
      buffer[0] = buffer[1] = buffer[2] = (unsigned char)(255.*atom_getfloat(argv));	// rgb
      buffer[3] = 0;									// alpha
      argv++;	buffer+=4;
    }
    break;
  case GL_YCBCR_422_GEM:
    // ?
    break;
  default:
    n = argc/4;
    counter=(picturesize<n)?picturesize:n;
    while (counter--) {
      buffer[0] = (unsigned char)(255.*atom_getfloat(&argv[0])); // red
      buffer[1] = (unsigned char)(255.*atom_getfloat(&argv[1])); // green
      buffer[2] = (unsigned char)(255.*atom_getfloat(&argv[2])); // blue
      buffer[3] = (unsigned char)(255.*atom_getfloat(&argv[3])); // alpha
      argv+=4; buffer+=4;
    }
  }
  m_pixBlock.newimage = 1;
}



/////////////////////////////////////////////////////////
// RGBAMess
//
/////////////////////////////////////////////////////////
void pix_noise :: RGBAMess(void)
{
	m_mode = GL_RGBA;
}
/////////////////////////////////////////////////////////
// RGBMess
//
/////////////////////////////////////////////////////////
void pix_noise :: RGBMess(void)
{
	m_mode = GL_RGB;
}
/////////////////////////////////////////////////////////
// GREYMess
//
/////////////////////////////////////////////////////////
void pix_noise :: GREYMess(void)
{
	m_mode = GL_LUMINANCE;
}

/////////////////////////////////////////////////////////
// SETMess
//
/////////////////////////////////////////////////////////
void pix_noise :: SETMess(int xsize, int ysize)
{
	int dataSize;
	if ((xsize < 1) || (ysize < 1)) return;
	m_pixBlock.image.clear();
	m_pixBlock.image.xsize = (int)xsize;
	m_pixBlock.image.ysize = (int)ysize;
	m_pixBlock.image.csize = 4;
	m_pixBlock.image.format = GL_RGBA;
	m_pixBlock.image.type = GL_UNSIGNED_BYTE;

	dataSize = m_pixBlock.image.xsize * m_pixBlock.image.ysize
		* 4 * sizeof(unsigned char);
	m_pixBlock.image.allocate(dataSize);
	memset(m_pixBlock.image.data, 0, dataSize);
}

/////////////////////////////////////////////////////////
// cleanPixBlock -- free the pixel buffer memory
//
/////////////////////////////////////////////////////////
void pix_noise :: cleanPixBlock()
{
  m_pixBlock.image.clear();
  m_pixBlock.image.data = NULL;
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_noise :: obj_setupCallback(t_class *classPtr)
{
	class_addbang(classPtr, reinterpret_cast<t_method>(&pix_noise::bangMessCallback));

    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_noise::RGBAMessCallback),
		gensym("RGBA"), A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_noise::RGBMessCallback),
		gensym("RGB"), A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_noise::GREYMessCallback),
		gensym("GREY"), A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_noise::GREYMessCallback),
		gensym("GRAY"), A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_noise::YUVMessCallback),
		gensym("YUV"), A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_noise::RGBAMessCallback),
		gensym("rgba"), A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_noise::RGBMessCallback),
		gensym("rgb"), A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_noise::GREYMessCallback),
		gensym("grey"), A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_noise::GREYMessCallback),
		gensym("gray"), A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_noise::YUVMessCallback),
		gensym("yuv"), A_NULL);
     class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_noise::SETMessCallback),
		gensym("set"), A_FLOAT, A_FLOAT, A_NULL);

    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_noise::DATAMessCallback),
		gensym("data"), A_GIMME, A_NULL);
}

void pix_noise :: bangMessCallback(void *data)
{
	GetMyClass(data)->bangMess();
}

void pix_noise :: RGBAMessCallback(void *data)
{
	GetMyClass(data)->m_mode=GL_RGBA;
}

void pix_noise :: RGBMessCallback(void *data)
{
	GetMyClass(data)->m_mode=GL_RGB;
}

void pix_noise :: GREYMessCallback(void *data)
{
	GetMyClass(data)->m_mode=GL_LUMINANCE;
}
void pix_noise :: YUVMessCallback(void *data)
{
  //	GetMyClass(data)->m_mode=GL_YCBCR_422_GEM;
}
void pix_noise :: SETMessCallback(void *data, t_float x, t_float y)
{
    GetMyClass(data)->SETMess((int)x, (int)y);
}

void pix_noise :: DATAMessCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->DATAMess(argc, argv);
}
