////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 2000 Guenter Geiger geiger@epy.co.at
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_pix2sig~.h"
#include "Gem/State.h"

CPPEXTERN_NEW_NAMED(pix_pix2sig, "pix_pix2sig~");

/////////////////////////////////////////////////////////
//
// pix_pix2sig
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_pix2sig :: pix_pix2sig(void)
{
  int i=4;
  while(i--) {
    o_col[i]=outlet_new(this->x_obj, &s_signal);
  }
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_pix2sig :: ~pix_pix2sig()
{
  int i=4;
  while(i--) {
    outlet_free(o_col[i]);
  }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// processImage
/////////////////////////////////////////////////////////
void pix_pix2sig :: render(GemState *state)
{
  pixBlock*img=NULL;
  if(state) {
    state->get(GemState::_PIX, img);
  }
  if(img) {
    img->image.copy2ImageStruct(&m_image);
  }
}

/////////////////////////////////////////////////////////
// signal Performance
namespace {
  template<typename T>
  void perform_pix2sig(t_sample**out, void*data_, unsigned int format, size_t n, t_sample scale) {
    T*data = static_cast<T*>(data_);
    t_sample*out_red   = out[0];
    t_sample*out_green = out[1];
    t_sample*out_blue  = out[2];
    t_sample*out_alpha = out[3];

    switch(format) {
    case GEM_RGBA:
    default:
      while(n--) {
        *(out_red  ++) = scale * static_cast<t_sample>(data[chRed]);
        *(out_green++) = scale * static_cast<t_sample>(data[chGreen]);
        *(out_blue ++) = scale * static_cast<t_sample>(data[chBlue]);
        *(out_alpha++) = scale * static_cast<t_sample>(data[chAlpha]);
        data+=4;
      }
      break;
    case GEM_YUV:
      n/=2;
      while(n--) {
        t_sample y0 = scale * static_cast<t_sample>(data[chY0]);
        t_sample y1 = scale * static_cast<t_sample>(data[chY1]);
        t_sample u  = scale * static_cast<t_sample>(data[chU]);
        t_sample v  = scale * static_cast<t_sample>(data[chV]);
        *(out_red  ++) = y0;
        *(out_red  ++) = y1;
        *(out_green++) = u;
        *(out_green++) = u;
        *(out_blue ++) = v;
        *(out_blue ++) = v;
        *(out_alpha++) = 1.;
        *(out_alpha++) = 1.;
        data+=4;
      }
      break;
    case GEM_GRAY:
      while(n--) {
        t_sample g = scale * static_cast<t_sample>(data[chGray]);
        *(out_red  ++) = g;
        *(out_green++) = g;
        *(out_blue ++) = g;
        *(out_alpha++) = 1.;
        data++;
      }
      break;
    }
  }
};



void pix_pix2sig :: perform(t_sample**out, size_t N)
{
  unsigned char* data = m_image.data;
  long int size   = m_image.xsize * m_image.ysize;
  int n = (N<size)?N:size;

  if (data && n>0) {
    switch(m_image.type) {
    default:
      perform_pix2sig<unsigned char>(out, data, m_image.format, n, 1./255.0);
      break;
    case GL_FLOAT:
      perform_pix2sig<GLfloat>(out, data, m_image.format, n, 1.0);
      break;
    case GL_DOUBLE:
      perform_pix2sig<GLdouble>(out, data, m_image.format, n, 1.0);
      break;
    }
  } else {
    n=N;
    t_sample*out_red   = out[0];
    t_sample*out_green = out[1];
    t_sample*out_blue  = out[2];
    t_sample*out_alpha = out[3];
    while (n--) {
      *out_red++=*out_green++=*out_blue++=*out_alpha++=0;
    }
  }

  return;
}

void pix_pix2sig :: dspMess(void *data, t_signal** sp)
{
  struct DSPCallbackClass {
    static t_int* callback(t_int *w) {
      pix_pix2sig *x = GetMyClass((void*)w[1]);
      t_sample**signals = (t_sample**)w+2;
      int n = (int)(w[6]);
      if(n>0)
        x->perform(signals, n);
      return (w+7);
    }
  };
  DSPCallbackClass cb;

  dsp_add(cb.callback, 6, data, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec,
          sp[3]->s_vec, sp[0]->s_n);
}

/////////////////////////////////////////////////////////
// Callback functions
//
/////////////////////////////////////////////////////////

void pix_pix2sig :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
      reinterpret_cast<t_method>(pix_pix2sig::dspMessCallback),
      gensym("dsp"), A_CANT, A_NULL);
}

void pix_pix2sig :: dspMessCallback(void *data,t_signal** sp)
{
  GetMyClass(data)->dspMess(data, sp);
}
