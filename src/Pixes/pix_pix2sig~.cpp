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
  : m_fillType(CLEAR)
  , m_line(0)
  , m_offset(0)
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
void pix_pix2sig :: filltypeMess(std::string type, int line) {
  if (0) {
  } else if("clear" == type) {
    m_fillType = CLEAR;
  } else if("fill" == type) {
    m_fillType = FILL;
  } else if("line" == type) {
    m_fillType = LINE;
  } else if("waterfall" == type) {
    m_fillType = WATERFALL;
  } else {
    error("invalid mode '%s'", type.c_str());
    return;
  }
  switch(m_fillType) {
  case WATERFALL:
    m_line = line;
    break;
  default:
    if (line)
      error("ignoring line (%d) for mode '%s'", line, type.c_str());
  }
  m_offset = 0;
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
  const size_t width = m_image.xsize;
  const size_t height = m_image.ysize;
  const int csize = m_image.csize;
  const int type = m_image.type;
  const int format = m_image.format;
  size_t pixsize = width * height;
  size_t chansize = 0;
  switch(type) {
  default:
    chansize=sizeof(unsigned char);
    break;
  case GL_FLOAT:
    chansize=sizeof(GLfloat);
    break;
  case GL_DOUBLE:
    chansize=sizeof(GLdouble);
    break;
  }
  if (m_offset >= pixsize)
    m_offset = 0;

  size_t count = N;

  switch(m_fillType) {
  case CLEAR:
    m_offset = 0;
    break;
  case FILL: break;
  case LINE:
    if (m_offset%width)
      m_offset = 0;
    if (count >= width)
      count = width;
    else {
    }
    break;
  case WATERFALL:
    if(m_line>=0) {
      m_offset = m_line * width;
    } else {
      m_offset = (height+m_line) * width;
    }
    if (count >= width)
      count = width;
    break;
  }


  if (m_offset + count > pixsize)
    count = pixsize - m_offset;

  ssize_t offset = m_offset * csize;

  //post("data[%p + %d] -> vecsize=%d pixsize=%d", data, m_offset, count, pixsize);
  if (data && count>0) {
    switch(m_image.type) {
    default:
      perform_pix2sig<unsigned char>(out, data + offset*chansize, m_image.format, count, 1./255.0);
      break;
    case GL_FLOAT:
      perform_pix2sig<GLfloat>(out, data + offset*chansize, m_image.format, count, 1.0);
      break;
    case GL_DOUBLE:
      perform_pix2sig<GLdouble>(out, data + offset*chansize, m_image.format, count, 1.0);
      break;
    }
    N -= count;
  }

  if(N) {
    t_sample*out_red   = out[0];
    t_sample*out_green = out[1];
    t_sample*out_blue  = out[2];
    t_sample*out_alpha = out[3];
    for(count=0; count<N; count++) {
      *out_red++=*out_green++=*out_blue++=*out_alpha++=0;
    }
  }

  switch(m_fillType) {
  case CLEAR: default:
    m_offset = 0;
    break;
  case FILL:
    m_offset += count;
    break;
  case LINE:
    m_offset += width;
    break;
  }

  return;
}

void pix_pix2sig :: dspMess(t_signal** sp)
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

  dsp_add(cb.callback, 6, this->x_obj, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec,
          sp[3]->s_vec, sp[0]->s_n);
}

/////////////////////////////////////////////////////////
// Callback functions
//
/////////////////////////////////////////////////////////

void pix_pix2sig :: obj_setupCallback(t_class *classPtr)
{
  struct dspCallbackClass {
    static void callback(void*data, t_signal** sp) {
      GetMyClass(data)->dspMess(sp);
    }
  };
  dspCallbackClass dspCB;
  class_addmethod(classPtr, reinterpret_cast<t_method>(dspCB.callback), gensym("dsp"), A_CANT, 0);
}
