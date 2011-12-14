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
//  random function : lagged fibonacci generator
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
pix_noise :: pix_noise(t_floatarg xsize, t_floatarg ysize) :
  m_banged(false), m_automatic(false),
  m_mode(GL_RGBA),
  m_rand_p(0), m_rand_k(24)
{
	int dataSize;
	if (xsize < 1) xsize = 256;
	if (ysize < 1) ysize = 256;
	int randInit = 307*1319;
	initRandom(randInit);
	
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
// initialize random generator
//
/////////////////////////////////////////////////////////
void pix_noise :: initRandom(int seed)
{
	int i,randval;
	m_rand_p = 0;
	m_rand_k = 24;
	randval = seed;
	for (i=0; i<55; i++) {
		m_rand[i] = seed = seed * 435898247 + 382842987;//random init
	}
	
}

/////////////////////////////////////////////////////////
// debug internal state of generator
//
/////////////////////////////////////////////////////////
void pix_noise :: debug()
{
	post("mrand_p = %i",m_rand_p);
	post("mrand_k = %i",m_rand_k);
	post("mrand[p] = %i",m_rand[m_rand_p]);
	post("mrand[k] = %i",m_rand[m_rand_k]);
	for (int i=0;i<55;i++) {
		post("m_rand[%i] = %i",i,m_rand[i]);//randval = randval * 435898247 + 382842987;
	}
	
}

/////////////////////////////////////////////////////////
// debug generator
//
/////////////////////////////////////////////////////////
unsigned char pix_noise :: random()
{
	// Lagged Fibonacci Generator
	// S[n] = S[n-p]+S[n-k]
	// with p = 55, k = 24
	
	m_rand[m_rand_p] = m_rand[m_rand_p] + m_rand[m_rand_k];
	if (++m_rand_p>54) m_rand_p = 0;
	if (++m_rand_k>54) m_rand_k = 0;
	return (unsigned char)(m_rand[m_rand_p]);
	
	//m_rand[m_rand_ptr] = m_rand[m_rand_ptr_prec] * 435898247 + 382842987;

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
				buffer[0] = random(); // red
				buffer[1] = random(); // green
				buffer[2] = random(); // blue
				buffer[3] = 0;					     // alpha
				buffer+=4;
			}
			break;
		case GL_LUMINANCE:
			counter=picturesize;
			while (counter--) {
				buffer[0] = buffer[1] = buffer[2] = random();	// rgb
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
				buffer[0] = random(); // red
				buffer[1] = random(); // green
				buffer[2] = random(); // blue
				buffer[3] = random(); // alpha
				buffer+=4;
			}
	}
	m_pixBlock.newimage = true;
}

/////////////////////////////////////////////////////////
// trigger
//
/////////////////////////////////////////////////////////
void pix_noise :: bang(void)
{
  m_banged=true;
}
void pix_noise :: autoMess(bool automatic)
{
  m_automatic=automatic;
}
/////////////////////////////////////////////////////////
// RGBMess
//
/////////////////////////////////////////////////////////
void pix_noise :: RGBAMess(void)
{
	m_mode = GL_RGBA;
  bang();
}
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
	class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_noise::seedMessCallback),
					gensym("seed"), A_FLOAT, A_NULL);
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
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_noise::debugMessCallback),
					gensym("debug"), A_NULL);
}

void pix_noise :: autoMessCallback(void *data, t_floatarg on)
{
	GetMyClass(data)->m_automatic=(on!=0);
}

void pix_noise :: seedMessCallback(void *data, t_floatarg seed)
{
	GetMyClass(data)->initRandom((int)seed);
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
void pix_noise :: debugMessCallback(void *data)
{
	GetMyClass(data)->debug();
}
