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
void pix_pix2sig :: filltypeMess(t_symbol*s, int argc, t_atom*argv) {
  if(!argc || A_SYMBOL != argv->a_type) {
    error("usage: %s <type> [<params>]", s->s_name);
    return;
  }
  std::string type(atom_getsymbol(argv)->s_name);
  filltype_t filltype = INVALID;
  if (0) {
  } else if("clear" == type) {
    filltype = CLEAR;
  } else if("fill" == type) {
    filltype = FILL;
  } else if("line" == type) {
    filltype = LINE;
  } else if("waterfall" == type) {
    filltype = WATERFALL;
  }

  if (INVALID == filltype) {
    error("invalid %s '%s' (must be one of 'clear', 'fill', 'line' or 'waterfall')", s->s_name, type.c_str());
    return;
  }
  if(WATERFALL == filltype) {
    switch(argc) {
    case 1:
      m_line = 0;
      break;
    case 2:
      if(A_FLOAT == argv[1].a_type) {
        m_line = atom_getfloat(argv+1);
        break;
      }
        /* fallthrough */
    default:
      error("usage: %s %s [<line>]", s->s_name, type.c_str());
      return;
    }
  } else {
    switch(argc) {
    case 1:
      break;
    default:
      error("usage: %s %s (no arguments!)", s->s_name, type.c_str());
      return;
    }
  }
  m_fillType = filltype;
  m_offset = 0;
}



/////////////////////////////////////////////////////////
// signal Performance
namespace {
  template<typename T>
  void perform_pix2sig(t_sample**out, void*data_, unsigned int format, size_t N, t_sample scale) {
    T*data = static_cast<T*>(data_);
    t_sample*out_red   = out[0];
    t_sample*out_green = out[1];
    t_sample*out_blue  = out[2];
    t_sample*out_alpha = out[3];
    size_t n = N;

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
    out[0] += N;
    out[1] += N;
    out[2] += N;
    out[3] += N;
  }
};



void pix_pix2sig :: perform(t_sample**out, size_t N)
{
  unsigned char* data = m_image.data;
  const size_t width = m_image.xsize;
  const size_t height = m_image.ysize;
  const size_t csize = (m_image.csize > 0)?m_image.csize:0;
  const int type = m_image.type;
  const int format = m_image.format;
  size_t pixsize = width * height;
  size_t chansize = 0;
  size_t offset = 0;
  t_sample*outsignal[] = {
    out[0], out[1], out[2], out[3]
  };

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

  int line=m_line;
  if (line<0)
    line = height+line;

  size_t count = N;

  switch(m_fillType) {
  case CLEAR:
    m_offset = 0;
    break;
  case FILL:
    break;
  case LINE:
    if (m_offset%width)
      m_offset = 0;
    if (count >= width)
      count = width;
    break;
  case WATERFALL:
    if((line > height) || line < 0)
      goto cleanup;
    m_offset = line * width;
    if (count >= width)
      count = width;
    break;
  }

  if (m_offset + count > pixsize)
    count = pixsize - m_offset;

  offset = m_offset * csize;
#if 0
  post("data[%p + %d(%d)] -> %dx%d count=%d vecsize=%d pixsize=%d chansize=%d",
       data, (int)m_offset, (int)offset,
       (int)width, (int)height,
       (int)count, (int)N, (int)pixsize, (int)chansize);
#endif
  if (data && count>0) {
    switch(m_image.type) {
    default:
      perform_pix2sig<unsigned char>(outsignal, data + offset*chansize, m_image.format, count, 1./255.0);
      break;
    case GL_FLOAT:
      perform_pix2sig<GLfloat>(outsignal, data + offset*chansize, m_image.format, count, 1.0);
      break;
    case GL_DOUBLE:
      perform_pix2sig<GLdouble>(outsignal, data + offset*chansize, m_image.format, count, 1.0);
      break;
    }
    N -= count;
  }

 cleanup:
  if(N) {
    t_sample*out_red   = outsignal[0];
    t_sample*out_green = outsignal[1];
    t_sample*out_blue  = outsignal[2];
    t_sample*out_alpha = outsignal[3];
    for(size_t remn=0; remn<N; remn++) {
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
  CPPEXTERN_MSG (classPtr, "mode", filltypeMess);
}
