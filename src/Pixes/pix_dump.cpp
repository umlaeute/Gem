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

CPPEXTERN_NEW_WITH_TWO_ARGS(pix_dump, t_floatarg, A_DEFFLOAT, t_floatarg,
                            A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// pix_dump
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_dump :: pix_dump(t_floatarg fx, t_floatarg fy) :
  m_dataOut(0),
  xsize(0), ysize(0),
  m_xsize(0), m_ysize(0), m_csize(3),
  m_buffer(0),
  m_bufsize(0),
  oldimagex(0), oldimagey(0),
  m_xstep(1), m_ystep(1),
  m_data(0),
  m_bytemode(false),
  m_mode(GEM_RGBA)
{
  xsize = static_cast<int>(fx);
  ysize = static_cast<int>(fy);

  if (xsize < 0) {
    xsize = 0;
  }
  if (ysize < 0) {
    ysize = 0;
  }

  m_xsize = xsize;
  m_ysize = ysize;

  oldimagex = xsize;
  oldimagey = ysize;

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
  delete[]m_buffer;
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_dump :: processImage(imageStruct &image)
{
  int x = m_xsize, y = m_ysize, c = m_csize;

  if (image.xsize != oldimagex) {
    oldimagex = image.xsize;
    m_xsize = ((!xsize) || (xsize > oldimagex))?oldimagex:xsize;
  }
  if (image.ysize != oldimagey) {
    oldimagey = image.ysize;
    m_ysize = ((!ysize) || (ysize > oldimagey))?oldimagey:ysize;
  }

  if (image.csize != m_csize) {
    m_csize = image.csize;
  }

  if ( (m_xsize != x) || (m_ysize != y) || (m_csize != c) ) {
    // resize the image buffer
    if(m_buffer) {
      delete [] m_buffer;
    }
    m_bufsize = m_xsize * m_ysize * m_csize;
    m_buffer = new t_atom[m_bufsize];

    m_xstep = m_csize * (static_cast<float>(image.xsize)/static_cast<float>
                         (m_xsize));
    m_ystep = m_csize * (static_cast<float>(image.ysize)/static_cast<float>
                         (m_ysize)) * image.xsize;
  }

  m_data = image.data;
}

/////////////////////////////////////////////////////////
// processYUVImage
//
/////////////////////////////////////////////////////////
void pix_dump :: processYUVImage(imageStruct &image)
{
  int x = m_xsize, y = m_ysize, c = m_csize;

  if (image.xsize != oldimagex) {
    oldimagex = image.xsize;
    m_xsize = ((!xsize) || (xsize > oldimagex))?oldimagex:xsize;
  }
  if (image.ysize != oldimagey) {
    oldimagey = image.ysize;
    m_ysize = ((!ysize) || (ysize > oldimagey))?oldimagey:ysize;
  }

  if (image.csize != m_csize) {
    m_csize = image.csize;
  }

  if ( (m_xsize != x) || (m_ysize != y) || (m_csize != c) ) {
    // resize the image buffer
    if(m_buffer) {
      delete [] m_buffer;
    }
    m_bufsize = m_xsize * m_ysize * m_csize;
    m_buffer = new t_atom[m_bufsize];

    m_xstep = m_csize * (static_cast<float>(image.xsize)/static_cast<float>
                         (m_xsize));
    m_ystep = m_csize * (static_cast<float>(image.ysize)/static_cast<float>
                         (m_ysize)) * image.xsize;
  }

  m_data = image.data;
}

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

  unsigned char *buffer = m_data;

  if ( m_doROI ) {
    roi_x1=m_roi.x1*(0.5+m_xsize);
    roi_x2=m_roi.x2*(0.5+m_xsize);
    roi_y1=m_roi.y1*(0.5+m_ysize);
    roi_y2=m_roi.y2*(0.5+m_ysize);

    buffer = m_data + m_csize*(( i / (roi_x2-roi_x1) + roi_y1 ) * m_xsize +
                               (i % (roi_x2-roi_x1)) + roi_x1);
  }
  n=roi_x1;
  m=roi_y1;

  int picturesize = (roi_x2-roi_x1)*(roi_y2-roi_y1);

  unsigned char *data, *line;

  data = line = buffer;
  switch(m_csize) {
  case 4:
    while (picturesize-- > 0) {
      if (!m_bytemode) {
        float r, g, b;
        r = static_cast<float>(data[chRed]) / 255.f;
        SETFLOAT(&m_buffer[i], r);
        i++;
        g = static_cast<float>(data[chGreen]) / 255.f;
        SETFLOAT(&m_buffer[i], g);
        i++;
        b = static_cast<float>(data[chBlue]) / 255.f;
        SETFLOAT(&m_buffer[i], b);
        i++;
        if ( m_mode == GEM_RGBA ) {
          float a = static_cast<float>(data[chAlpha]) / 255.f;
          SETFLOAT(&m_buffer[i], a);
          i++;
        }
      } else {
        unsigned char r, g, b;
        r = static_cast<unsigned char>(data[chRed]);
        SETFLOAT(&m_buffer[i], r);
        i++;
        g = static_cast<unsigned char>(data[chGreen]);
        SETFLOAT(&m_buffer[i], g);
        i++;
        b = static_cast<unsigned char>(data[chBlue]);
        SETFLOAT(&m_buffer[i], b);
        i++;
        if ( m_mode == GEM_RGBA ) {
          unsigned char a = static_cast<unsigned char>(data[chAlpha]);
          SETFLOAT(&m_buffer[i], a);
          i++;
        }
      }
      j++;
      if ( m_doROI ) {
        data = m_data + m_csize*(( j / (roi_x2-roi_x1) + roi_y1 ) * m_xsize +
                                 (j % (roi_x2-roi_x1)) + roi_x1) ;
      } else {
        data+=4;
      }
    }
    break;
  case 2:
    while (n < m_ysize) {
      while (m < m_xsize/2) {
        if (!m_bytemode) {
          float y,u,v;
          u = static_cast<float>(data[0]) / 255.f;
          SETFLOAT(&m_buffer[i], u);
          i++;
          y = static_cast<float>(data[1]) / 255.f;
          SETFLOAT(&m_buffer[i], y);
          i++;
          v = static_cast<float>(data[2]) / 255.f;
          SETFLOAT(&m_buffer[i], v);
          i++;
          if ( m_mode == GEM_RGBA ) {
            float y1 = static_cast<float>(data[3]) / 255.f;
            SETFLOAT(&m_buffer[i], y1);
            i++;
          }
        } else {
          unsigned char y,u,v;
          u = static_cast<unsigned char>(data[0]);
          SETFLOAT(&m_buffer[i], u);
          i++;
          y = static_cast<unsigned char>(data[1]);
          SETFLOAT(&m_buffer[i], y);
          i++;
          v = static_cast<unsigned char>(data[2]);
          SETFLOAT(&m_buffer[i], v);
          i++;
          if ( m_mode == GEM_RGBA ) {
            unsigned char y1 = static_cast<unsigned char>(data[3]);
            SETFLOAT(&m_buffer[i], y1);
            i++;
          }
        }
        m++;
        data = line + static_cast<int>(m_xstep * static_cast<float>(m));
      }
      m = 0;
      n++;
      line = m_data + static_cast<int>(m_ystep*n);
      data = line;
    }
  case 1:
  default:
    int datasize=m_xsize*m_ysize*m_csize/4;
    int leftover=m_xsize*m_ysize*m_csize-datasize*4;
    while (datasize--) {
      if ( !m_bytemode ) {
        float v;
        v = static_cast<float>(data[0]) / 255.f;
        SETFLOAT(&m_buffer[i], v);
        v = static_cast<float>(data[1]) / 255.f;
        SETFLOAT(&m_buffer[i+1], v);
        v = static_cast<float>(data[2]) / 255.f;
        SETFLOAT(&m_buffer[i+2], v);
        i+=3;
        if ( m_mode == GEM_RGBA ) {
          v = static_cast<float>(data[3]) / 255.f;
          SETFLOAT(&m_buffer[i], v);
          i++;
        }
        if ( m_doROI ) {
          j++;
          data = m_data + m_csize*(( j / (roi_x2-roi_x1) + roi_y1 ) * m_xsize +
                                   (j % (roi_x2-roi_x1)) + roi_x1) ;
        } else {
          data+=4;
        }
      } else {
        unsigned char v;
        v = static_cast<unsigned char>(data[0]);
        SETFLOAT(&m_buffer[i], v);
        v = static_cast<unsigned char>(data[1]);
        SETFLOAT(&m_buffer[i+1], v);
        v = static_cast<unsigned char>(data[2]);
        SETFLOAT(&m_buffer[i+2], v);
        i+=3;
        if ( m_mode == GEM_RGBA ) {
          v = static_cast<unsigned char>(data[3]);
          SETFLOAT(&m_buffer[i], v);
          i++;
        }
        if ( m_doROI ) {
          j++;
          data = m_data + m_csize*(( j / (roi_x2-roi_x1) + roi_y1 ) * m_xsize +
                                   (j % (roi_x2-roi_x1)) + roi_x1) ;
        } else {
          data+=4;
        }
      }
    }

    if ( !m_bytemode ) {
      while (leftover--) {
        float v = static_cast<float>(*data++) / 255.f;
        SETFLOAT(&m_buffer[i], v);
        i++;
      }
    } else {
      while (leftover--) {
        unsigned char v = static_cast<unsigned char>(*data++);
        SETFLOAT(&m_buffer[i], v);
        i++;
      }
    }
  }
  outlet_list(m_dataOut, gensym("list"), i, m_buffer);
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
