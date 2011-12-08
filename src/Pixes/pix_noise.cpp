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
//  based on pix_set
//  random function based on noise~
//  2011
//  Nicolas Montgermont
//  mailto:nicolas_montgermont@yahoo.fr
//
/////////////////////////////////////////////////////////



// generate a uniform noise texture

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
	m_rand = randInit;
	m_automatic = false;
	m_banged = false;
	
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
	generateNoise();	
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
	if (m_automatic || m_banged) {
		m_banged = false;
		generateNoise();
	}
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

/////////////////////////////////////////////////////////
// random generator
//
/////////////////////////////////////////////////////////
unsigned char pix_noise :: pix_random()
{
	m_rand = m_rand * 435898247 + 382842987;
	return ((unsigned char)((m_rand << 24)));
}

/////////////////////////////////////////////////////////
// generate texture
//
/////////////////////////////////////////////////////////
void pix_noise :: generateNoise()
{
	int picturesize = m_pixBlock.image.xsize * m_pixBlock.image.ysize, counter, n;
	unsigned char *buffer = m_pixBlock.image.data;
	
	memset(buffer, 0, picturesize*m_pixBlock.image.csize*sizeof(unsigned char));
	
	switch (m_mode) {
		case GL_RGB:
			counter=picturesize;
			while (counter--) {
				buffer[0] = pix_random(); // red
				buffer[1] = pix_random(); // green
				buffer[2] = pix_random(); // blue
				buffer[3] = 0;					     // alpha
				buffer+=4;
			}
			break;
		case GL_LUMINANCE:
			counter=picturesize;
			while (counter--) {
				buffer[0] = buffer[1] = buffer[2] = pix_random();	// rgb
				buffer[3] = 0;									// alpha
				buffer+=4;
			}
			break;
		case GL_YCBCR_422_GEM:
			// ?
			break;
		default:
			counter=picturesize;
			while (counter--) {
				buffer[0] = pix_random(); // red
				buffer[1] = pix_random(); // green
				buffer[2] = pix_random(); // blue
				buffer[3] = pix_random(); // alpha
				buffer+=4;
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
	class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_noise::autoMessCallback),
					gensym("auto"), A_FLOAT, A_NULL);
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

}

void pix_noise :: autoMessCallback(void *data, t_floatarg on)
{
	GetMyClass(data)->m_automatic=(on!=0);
}

void pix_noise :: bangMessCallback(void *data)
{
	GetMyClass(data)->m_banged=true;
}

void pix_noise :: RGBAMessCallback(void *data)
{
	GetMyClass(data)->m_mode=GL_RGBA;
	GetMyClass(data)->m_banged=true;
}

void pix_noise :: RGBMessCallback(void *data)
{
	GetMyClass(data)->m_mode=GL_RGB;
	GetMyClass(data)->m_banged=true;
}

void pix_noise :: GREYMessCallback(void *data)
{
	GetMyClass(data)->m_mode=GL_LUMINANCE;
	GetMyClass(data)->m_banged=true;
}
void pix_noise :: YUVMessCallback(void *data)
{
  //	GetMyClass(data)->m_mode=GL_YCBCR_422_GEM;
}
void pix_noise :: SETMessCallback(void *data, t_float x, t_float y)
{
    GetMyClass(data)->SETMess((int)x, (int)y);
	GetMyClass(data)->m_banged=true;
}

