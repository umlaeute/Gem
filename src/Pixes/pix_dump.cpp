////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//
//  pix_dump
//
//  0409:forum::für::umläute:2000
//  IOhannes m zmoelnig
//  mailto:zmoelnig@iem.at
//
/////////////////////////////////////////////////////////


#include "pix_dump.h"

CPPEXTERN_NEW(pix_dump);

/////////////////////////////////////////////////////////
//
// pix_dump
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_dump :: pix_dump() :
  m_dataOut(0),
  m_xsize(0), m_ysize(0), m_csize(3),
  m_buffer(0),
  m_bufsize(0),
  m_data(0),
  m_bytemode(false),
  m_mode(GEM_RGBA)
{
  m_xsize = 0;
  m_ysize = 0;

  m_bufsize = m_xsize * m_ysize * m_csize;

  m_buffer = new t_atom[m_bufsize];

  m_dataOut = outlet_new(this->x_obj, &s_list);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_dump :: ~pix_dump()
{
  outlet_free(m_dataOut);
  delete [] m_buffer;
  m_buffer = 0;
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_dump :: processImage(imageStruct &image)
{
  m_xsize = image.xsize;
  m_ysize = image.ysize;
  m_csize = image.csize;

  if(m_xsize * m_ysize * m_csize > m_bufsize) {
    // resize the image buffer
    if(m_buffer) {
      delete [] m_buffer;
      m_buffer = 0;
    }
    m_bufsize = m_xsize * m_ysize * m_csize;
    m_buffer = new t_atom[m_bufsize];
  }

  m_data = image.data;
}


namespace {
  template<typename T>
  size_t data4_to_atoms(t_atom*dest, const T*src, size_t n, t_float scale, const int channels[4]) {
    size_t count = 0;
    while(n--) {
      for(size_t i=0; i<4; i++) {
        int ch = channels[i];
        if(ch<0)continue;
        t_float v = static_cast<t_float>(src[channels[ch]]) * scale;
        SETFLOAT(dest, v);
        dest++;
        count++;
      }
      src+=4;
    }
    return count;
  }
  template<typename T>
  size_t data_to_atoms(t_atom*dest, const T*src, size_t N, t_float scale) {
    size_t count = 0;
    for(size_t n=0; n<N; n++) {
      t_float v = static_cast<t_float>(*src++) * scale;
      SETFLOAT(dest+n, v);
      count++;
    }
    return count;
  }
  template<typename T>
  size_t pix2atoms(t_atom*atoms, int mode, t_float scale,
                 const T*pixels, size_t width, size_t height, unsigned int format,
                 size_t x0, size_t y0, size_t rows, size_t cols)
  {
    size_t count = 0;
    const int extrachannel = (GEM_RGBA==mode)?1:0;
    const int channelsRGBA[] = {chRed, chGreen, chBlue, extrachannel?chAlpha:-1};
    const int channelsUYVY[] = {chU, chY0, chV, extrachannel?chY1:-1};
    if(x0 > width || (y0 > height))
      return 0;
    if (x0 + rows > width)
      rows = width - x0;
    if (y0 + cols > height)
      cols = height - y0;


    switch(format) {
    case GEM_GRAY:
      for(size_t r=y0; r<rows; r++) {
        const T*data = pixels + width*r + x0;
        size_t n = data_to_atoms(atoms, data, cols, scale);
        count += n;
        atoms += n;
      }
      break;
    case GEM_YUV:
      for(size_t r=y0; r<rows; r++) {
        const T*data = pixels + (width*r + x0) * 2;
        size_t n = data4_to_atoms(atoms, data, cols>>1, scale, channelsUYVY);
        count += n;
        atoms += n;
      }
      break;
    case GEM_RGB:
      /* TODO: honor chRed,chGreen/chBlue */
      for(size_t r=y0; r<rows; r++) {
        const T*data = pixels + (width*r + x0) * 3;
        size_t n = data_to_atoms(atoms, data, cols * 3, scale);
        count += n;
        atoms += n;
      }
      break;
    case GEM_RGBA:
      for(size_t r=y0; r<rows; r++) {
        const T*data = pixels + (width*r + x0) * 4;
        size_t n = data4_to_atoms(atoms, data, cols, scale, channelsRGBA);
        count += n;
        atoms += n;
      }
      break;
    }
    return count;
  }
};

/////////////////////////////////////////////////////////
// trigger
//
/////////////////////////////////////////////////////////
void pix_dump :: trigger()
{
  if (!m_data) {
    return;
  }

  int n = 0, m = 0;
  int i = 0, j=0;

  int roi_x1=0;
  int roi_x2=m_xsize;
  int roi_y1=0;
  int roi_y2=m_ysize;

  t_float scale = m_bytemode?1:(1./255.);
  int format;
  switch(m_csize) {
  case 1:
    format = GEM_GRAY;
    break;
  case 2:
    format = GEM_YUV;
    break;
  case 3:
    format = GEM_RGB;
    break;
  case 4:
    format = GEM_RGBA;
    break;
  default:
    error("unknown format with csize=%d", m_csize);
    return;
  }

  if ( m_doROI ) {
    roi_x1=m_roi.x1*(0.5+m_xsize);
    roi_x2=m_roi.x2*(0.5+m_xsize);
    roi_y1=m_roi.y1*(0.5+m_ysize);
    roi_y2=m_roi.y2*(0.5+m_ysize);
  }
  size_t count = pix2atoms(m_buffer, m_mode, scale,
                           m_data, m_xsize, m_ysize, format,
                           roi_x1, roi_y1, roi_x2-roi_x1, roi_x2-roi_x1);
  outlet_list(m_dataOut, gensym("list"), count, m_buffer);

}

/////////////////////////////////////////////////////////
// RGBAMess
//
/////////////////////////////////////////////////////////
void pix_dump :: RGBAMess(void)
{
  m_mode = GEM_RGBA;
}
/////////////////////////////////////////////////////////
// RGBMess
//
/////////////////////////////////////////////////////////
void pix_dump :: RGBMess(void)
{
  m_mode = GEM_RGB;
}

void pix_dump :: bytemodeMess(bool v)
{
  m_bytemode=v;
}
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_dump :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG0(classPtr, "bang", trigger);
  CPPEXTERN_MSG1(classPtr, "bytemode", bytemodeMess, bool);
  CPPEXTERN_MSG0(classPtr, "RGBA", RGBAMess);
  CPPEXTERN_MSG0(classPtr, "rgba", RGBAMess);
  CPPEXTERN_MSG0(classPtr, "RGB", RGBMess);
  CPPEXTERN_MSG0(classPtr, "rgb", RGBMess);
}
