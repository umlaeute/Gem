////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2000 Guenter Geiger geiger@epy.co.at
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_sig2pix.h"

#ifdef unix
#include <unistd.h>
#include <strings.h>
#endif

#include "Base/GemCache.h"

static inline int powerOfTwo(int value)
{
  int x = 1;
  while(x < value) x <<= 1;
  return(x);
}

CPPEXTERN_NEW_WITH_TWO_ARGS(pix_sig2pix, t_float,A_DEFFLOAT,t_float, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// pix_sig2pix
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_sig2pix :: pix_sig2pix(t_float& width, t_float& height)
{
  int i;

  m_pixBlock.image = m_imageStruct;
  m_pixBlock.image.data=NULL;

  dimenMess((int)width, (int)height);	//tigital

  for (i=0; i<3; i++)
    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_signal, &s_signal); /* channels inlet */
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_sig2pix :: ~pix_sig2pix()
{
  cleanImage();
}

void pix_sig2pix :: dimenMess(int width, int height) {
  if (width == 0) width = 256;
  if (height == 0) height = 256;

  if (width>32000)width=256;
  if (height>32000)width=256;

  cleanImage();
  
  m_pixBlock.image.xsize =(GLint) width;
  m_pixBlock.image.ysize = (GLint) height;
  m_pixBlock.image.csize = 4;
  m_pixBlock.image.format = GL_RGBA;
  m_pixBlock.image.type = GL_UNSIGNED_BYTE;

  m_pixsize = m_pixBlock.image.xsize*m_pixBlock.image.ysize;
  m_pixBlock.image.allocate(m_pixsize * m_pixBlock.image.csize);

  clearImage();
}


/////////////////////////////////////////////////////////
void pix_sig2pix :: render(GemState *state)
{
  state->image = &m_pixBlock;
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_sig2pix :: postrender(GemState *state)
{
  m_pixBlock.newimage = 0;
  state->image = NULL;
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_sig2pix :: startRendering()
{
  m_pixBlock.newimage = 1;
}

/////////////////////////////////////////////////////////
// cleanImage
//
/////////////////////////////////////////////////////////
void pix_sig2pix :: cleanImage()
{
  if (!m_pixBlock.image.data) return;

  // release previous data
  m_pixBlock.image.clear();
  m_pixBlock.image.data = NULL;
}

/////////////////////////////////////////////////////////
// clearImage
//
/////////////////////////////////////////////////////////
void pix_sig2pix :: clearImage()
{
  long int dataSize = m_pixsize * m_pixBlock.image.csize;
  if (!m_pixBlock.image.data) return;

  // set data to 0
  memset(m_pixBlock.image.data, 0, dataSize);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

t_int* pix_sig2pix :: perform(t_int* w)
{
  pix_sig2pix *x = GetMyClass((void*)w[1]);
  t_float* in_red =   (t_float*)(w[2]);
  t_float* in_green = (t_float*)(w[3]);
  t_float* in_blue =  (t_float*)(w[4]);
  t_float* in_alpha = (t_float*)(w[5]);
  t_int n = (t_int)(w[6]);

  unsigned char* data = x->m_pixBlock.image.data;

  if (n > x->m_pixsize) n = x->m_pixsize;
  while(n--){
    *data++ = (unsigned char) (*in_red++  *255.0);
    *data++ = (unsigned char) (*in_green++*255.0);
    *data++ = (unsigned char) (*in_blue++ *255.0);
    *data++ = (unsigned char) (*in_alpha++*255.0);
  }

  x->m_pixBlock.newimage = 1;
  return (w+7);
}

void pix_sig2pix :: dspMess(void *data, t_signal** sp)
{
  clearImage();
  dsp_add(perform, 6, data, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec, sp[0]->s_n);
}

/////////////////////////////////////////////////////////
// Callback functions
//
/////////////////////////////////////////////////////////

void pix_sig2pix :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classpix_sig2pix, gensym("pix_sig2pix~"), A_DEFFLOAT, A_DEFFLOAT, A_NULL);

  class_addmethod(classPtr, nullfn, gensym("signal"), A_NULL);
  class_addmethod(classPtr, (t_method)pix_sig2pix::dspMessCallback, 
		  gensym("dsp"), A_NULL);
  class_addmethod(classPtr, (t_method)pix_sig2pix::dimenMessCallback, 
		  gensym("dimen"), A_DEFFLOAT,A_DEFFLOAT, A_NULL);
}


void pix_sig2pix :: dspMessCallback(void *data,t_signal** sp)
{
  GetMyClass(data)->dspMess(data, sp);
}

void pix_sig2pix ::dimenMessCallback(void *data, t_float w, t_float h)
{
  GetMyClass(data)->dimenMess((int)w, (int)h);
}
