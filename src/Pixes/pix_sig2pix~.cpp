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

#include "pix_sig2pix~.h"
#include "Gem/State.h"
#include "Utils/Functions.h"

CPPEXTERN_NEW_NAMED_WITH_TWO_ARGS(pix_sig2pix, "pix_sig2pix~", t_float,A_DEFFLOAT,t_float,
    A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// pix_sig2pix
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_sig2pix :: pix_sig2pix(t_floatarg width, t_floatarg height)
  : m_pixsize(0)
  , m_width(0), m_height(0)
  , m_reqFormat(GL_RGBA)
  , m_reqType(0)
{
  dimenMess((int)width, (int)height);   //tigital

  int i;
  for (i=0; i<3; i++) {
    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_signal,
              &s_signal);  /* channels inlet */
  }
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_sig2pix :: ~pix_sig2pix()
{}

void pix_sig2pix :: dimenMess(int width, int height)
{
  if (width>32000) {
    width=8;
  }
  if (height>32000) {
    height=8;
  }
  if (width  < 0) {
    width  = 0;
  }
  if (height < 0) {
    height = 0;
  }

  m_width =width;
  m_height=height;

  if (width  == 0) {
    width = 8;
  }
  if (height == 0) {
    height = 8;
  }

  m_pixBlock.image.xsize =(GLint) width;
  m_pixBlock.image.ysize = (GLint) height;
  m_pixBlock.image.setCsizeByFormat(m_reqFormat);
  if(m_reqType)
    m_pixBlock.image.type = m_reqType;

  m_pixsize = m_pixBlock.image.xsize*m_pixBlock.image.ysize;
  m_pixBlock.image.reallocate();
  m_pixBlock.image.setBlack();
}

void pix_sig2pix :: csMess(GLint cs)
{
  m_reqFormat=cs;
  m_pixBlock.image.setCsizeByFormat(m_reqFormat);
  m_pixsize = m_pixBlock.image.xsize*m_pixBlock.image.ysize;
  m_pixBlock.image.reallocate();
  m_pixBlock.image.setBlack();
}


/////////////////////////////////////////////////////////
void pix_sig2pix :: render(GemState *state)
{
  state->set(GemState::_PIX,&m_pixBlock);
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_sig2pix :: postrender(GemState *state)
{
  m_pixBlock.newimage = 0;
  state->set(GemState::_PIX, static_cast<pixBlock*>(NULL));
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
// static member function
//
/////////////////////////////////////////////////////////

namespace {
  template<typename T>
  void perform_sig2pix(t_sample**in, void*data_, unsigned int format, size_t n, t_sample scale) {
    T*data = static_cast<T*>(data_);
    t_sample*in_red = in[0];
    t_sample*in_green = in[1];
    t_sample*in_blue = in[2];
    t_sample*in_alpha = in[3];

    switch(format) {
    case GL_RGBA:
    default:
      while(n--) {
        data[chRed]   = static_cast<T> (scale*(*in_red++));
        data[chGreen] = static_cast<T> (scale*(*in_green++));
        data[chBlue]  = static_cast<T> (scale*(*in_blue++));
        data[chAlpha] = static_cast<T> (scale*(*in_alpha++));
        data+=4;
      }
      break;
    case GEM_YUV:
      n/=2;
      while(n--) {
        data[chY0] = static_cast<T> (scale*(*in_red++));
        data[chU ] = static_cast<T> (scale*(*in_green++));
        in_green++;
        data[chY1] = static_cast<T> (scale*(*in_red++));
        data[chV ] = static_cast<T> (scale*(*in_blue++));
        in_blue++;

        data+=4;
      }
      break;
    case GEM_GRAY:
      while(n--) {
        *data++ = static_cast<T> (scale*(*in_red++));
      }
      break;
    }
  }
};

void pix_sig2pix :: perform(t_sample**signals, int n)
{
  unsigned char* data = m_pixBlock.image.data;

  if (n > m_pixsize) {
    n = m_pixsize;
  }
  if (n>0) {
    switch(m_pixBlock.image.type) {
    default:
      perform_sig2pix<unsigned char>(signals, data, m_pixBlock.image.format, n, 255.0);
      break;
    case GL_FLOAT:
      perform_sig2pix<GLfloat>(signals, data, m_pixBlock.image.format, n, 1.0);
      break;
    case GL_DOUBLE:
      perform_sig2pix<GLdouble>(signals, data, m_pixBlock.image.format, n, 1.0);
      break;
    }
    m_pixBlock.newimage = 1;
  }
}

void pix_sig2pix :: dspMess(void *data, t_signal** sp)
{
  struct DSPCallbackClass {
    static t_int* callback(t_int *w) {
        pix_sig2pix *x = GetMyClass((void*)w[1]);
        t_sample**signals = (t_sample**)w+2;
        int n = (int)(w[6]);
        x->perform(signals, n);
        return (w+7);
    }
  };
  DSPCallbackClass cb;

  if (m_width==0 && m_height==0) {
    int w = powerOfTwo((int)sqrt((double)sp[0]->s_n));
    int h = (w)?(sp[0]->s_n / w):0;
    dimenMess(w, h);
    m_width = 0;
    m_height= 0;
  }
  m_pixBlock.image.setBlack();
  dsp_add(cb.callback, 6, data, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec,
          sp[3]->s_vec, sp[0]->s_n);
}

void pix_sig2pix :: typeMess(std::string type) {
  if("BYTE" == type) {
    m_reqType = 0;
  } else if ("FLOAT" == type) {
    m_reqType = GL_FLOAT;
  } else if ("DOUBLE" == type) {
    m_reqType = GL_DOUBLE;
  } else {
    error("invalid type '%s': must be 'BYTE', 'FLOAT' or 'DOUBLE'", type.c_str());
    return;
  }
  dimenMess(m_width, m_height);
}

/////////////////////////////////////////////////////////
// Callback functions
//
/////////////////////////////////////////////////////////

void pix_sig2pix :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, nullfn, gensym("signal"), A_NULL);
  class_addmethod(classPtr,
      reinterpret_cast<t_method>(pix_sig2pix::dspMessCallback),
      gensym("dsp"), A_CANT, A_NULL);
  class_addmethod(classPtr,
      reinterpret_cast<t_method>(pix_sig2pix::csMessCallback),
      gensym("colorspace"), A_DEFSYMBOL, A_NULL);
  CPPEXTERN_MSG2(classPtr, "dimen", dimenMess, int, int);
  CPPEXTERN_MSG1(classPtr, "type", typeMess, std::string);
}


void pix_sig2pix :: dspMessCallback(void *data,t_signal** sp)
{
  GetMyClass(data)->dspMess(data, sp);
}

void pix_sig2pix ::csMessCallback(void *data, t_symbol*s)
{
  int cs = getPixFormat(s->s_name);
  if(cs>0) {
    GetMyClass(data)->csMess(cs);
  } else {
    GetMyClass(data)->error("colorspace must be Grey, YUV or RGBA");
  }
}
