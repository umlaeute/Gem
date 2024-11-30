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
#include "Gem/GemGL.h"

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
  m_buffer(0), m_bufsize(0),
  m_bytemode(false),
  m_mode(GEM_RGBA)
{
  m_image.data = 0;
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
  size_t bufsize = image.xsize * image.ysize * image.csize;
  image.copy2ImageStruct(&m_image);
  if(bufsize > m_bufsize) {
    // resize the image buffer
    if(m_buffer) {
      delete [] m_buffer;
      m_buffer = 0;
    }
    m_bufsize = bufsize;
    m_buffer = new t_atom[m_bufsize];
  }
}
void pix_dump :: processFloat32(imageStruct &image)
{
  processImage(image);
}
void pix_dump :: processFloat64(imageStruct &image)
{
  processImage(image);
}


namespace {
  template<typename T>
  static inline
  size_t data4_to_atoms(t_atom*dest, const T*src, size_t n, t_float scale, const int channels[4]) {
    size_t count = 0;
    while(n--) {
      for(size_t i=0; i<4; i++) {
        int ch = channels[i];
        if(ch<0)continue;
        t_float v = static_cast<t_float>(src[ch]) * scale;
        SETFLOAT(dest, v);
        dest++;
        count++;
      }
      src+=4;
    }
    return count;
  }
  template<typename T>
  static inline
  size_t data3_to_atoms(t_atom*dest, const T*src, size_t n, t_float scale, const int channels[3]) {
    size_t count = 0;
    while(n--) {
      for(size_t i=0; i<3; i++) {
        int ch = channels[i];
        //if(ch<0)continue;
        t_float v = static_cast<t_float>(src[ch]) * scale;
        SETFLOAT(dest, v);
        dest++;
        count++;
      }
      src+=3;
    }
    return count;
  }
  template<typename T>
  static inline
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
  static inline
  size_t pix2atoms(t_atom*atoms, int mode, t_float scale,
                   const imageStruct&image,
                   size_t x0, size_t y0, size_t cols, size_t rows)
  {
    size_t count = 0;
    const T*pixels = static_cast<const T*>(static_cast<const void*>(image.data));
    size_t width = image.ysize;
    size_t height = image.xsize;

    const int extrachannel = (GEM_RGBA==mode)?1:0;
    const int channelsRGBA[] = {chRed, chGreen, chBlue, extrachannel?chAlpha:-1};
    const int channelsRGB [] = {chRed, chGreen, chBlue};
    const int channelsUYVY[] = {chU, chY0, chV, extrachannel?chY1:-1};
    if(x0 > width || (y0 > height))
      return 0;
    if (x0 + cols > width)
      cols = width - x0;
    if (y0 + rows > height)
      rows = height - y0;

    switch(image.format) {
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
      for(size_t r=y0; r<rows; r++) {
        const T*data = pixels + (width*r + x0) * 3;
        size_t n = data3_to_atoms(atoms, data, cols * 3, scale, channelsRGB);
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
  if (!m_image.data) {
    return;
  }

  int n = 0, m = 0;
  int i = 0, j=0;

  int roi_x1=0;
  int roi_x2=m_image.xsize;
  int roi_y1=0;
  int roi_y2=m_image.ysize;

  t_float scale = m_bytemode?1:(1./255.);

  if ( m_doROI ) {
    roi_x1=m_roi.x1*(0.5+m_image.xsize);
    roi_x2=m_roi.x2*(0.5+m_image.xsize);
    roi_y1=m_roi.y1*(0.5+m_image.ysize);
    roi_y2=m_roi.y2*(0.5+m_image.ysize);
  }

  size_t count = 0;
  switch(m_image.type) {
  case GL_FLOAT:
    count = pix2atoms<GLfloat>(m_buffer, m_mode, 1., m_image,
                               roi_x1, roi_y1, roi_x2-roi_x1, roi_x2-roi_x1);
    break;
  case GL_DOUBLE:
    count = pix2atoms<GLdouble>(m_buffer, m_mode, 1., m_image,
                                roi_x1, roi_y1, roi_x2-roi_x1, roi_x2-roi_x1);
    break;
  default:
    count = pix2atoms<unsigned char>(m_buffer, m_mode, scale, m_image,
                                     roi_x1, roi_y1, roi_x2-roi_x1, roi_x2-roi_x1);
  }
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
