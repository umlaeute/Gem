////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_test.h"
#include "Gem/State.h"

CPPEXTERN_NEW_WITH_GIMME(pix_test);

namespace
{
static unsigned char bars_RGBA[][3] = {
  // R , G  , B
  {0xFF,0xFF,0xFF}, // white
  {0xFF,0xFF,0x00}, // yellow
  {0x00,0xFF,0xFF}, // cyan
  {0x00,0xFF,0x00}, // green
  {0xFF,0x00,0xFF}, // magenta
  {0xFF,0x00,0x00}, // red
  {0x00,0x00,0xFF}, // blue
  {0x00,0x00,0x00}, // black
};
static unsigned char bars_YUV[][3] = {
  // Y , U  , V
  {0xEB,0x80,0x80}, // white
  {0xD2,0x10,0x92}, // yellow
  {0xAA,0xA6,0x10}, // cyan
  {0x91,0x36,0x22}, // green
  {0x6A,0xCA,0xDE}, // magenta
  {0x51,0x5A,0xF0}, // red
  {0x29,0xF0,0x6E}, // blue
  {0x10,0x80,0x80}, // black
};

static volatile unsigned char getRandom(void)
{
  static unsigned int random_nextseed = 1489853723;
  random_nextseed = random_nextseed * 435898247 + 938284281;
  return (random_nextseed % 0xFF);
}

static void makeSMPTE_RGBA(unsigned int cols, unsigned int rows,
                           unsigned char*DATA, float scale, bool noise)
{
  unsigned char*data=DATA;
  unsigned int r,c;
  unsigned int row0, row1;

  row0=0;
  row1=noise?(rows*2/3):(rows*3/4);
  for(r=row0; r<row1; r++) {
    for(c=0; c<cols; c++) {
      unsigned int idx=c*7/cols;
      data[chRed  ]=bars_RGBA[idx][0]*scale;
      data[chGreen]=bars_RGBA[idx][1]*scale;
      data[chBlue ]=bars_RGBA[idx][2]*scale;
      data[chAlpha]=0xFF;
      data+=4;
    }
  }
  row0=r;
  row1=noise?(rows*3/4):(rows*7/8);
  for(r=row0; r<row1; r++) {
    for(c=0; c<cols; c++) {
      unsigned int grey=c*255/cols;
      data[chRed  ]=grey;
      data[chGreen]=grey;
      data[chBlue ]=grey;
      data[chAlpha]=0xFF;
      data+=4;
    }
  }
  row0=r;
  row1=noise?(rows*5/6):(rows);
  for(r=row0; r<row1; r++) {
    for(c=0; c<cols; c++) {
      unsigned int grey=255-c*255/cols;
      data[chRed  ]=grey;
      data[chGreen]=grey;
      data[chBlue ]=grey;
      data[chAlpha]=0xFF;
      data+=4;
    }
  }
  row0=r;
  row1=rows;
  for(r=0; r<(row1-row0)*cols; r++) {
    unsigned char grey=getRandom();
    data[chRed  ]=grey;
    data[chGreen]=grey;
    data[chBlue ]=grey;
    data[chAlpha]=0xFF;
    data+=4;
  }
}
static void makeSMPTE_RGB(unsigned int cols, unsigned int rows,
                          unsigned char*DATA, float scale, bool noise)
{
  unsigned char*data=DATA;
  unsigned int r,c;
  unsigned int row0, row1;

  row0=0;
  row1=rows*2/3;
  for(r=row0; r<row1; r++) {
    for(c=0; c<cols; c++) {
      unsigned int idx=c*7/cols;
      data[chRed  ]=bars_RGBA[idx][0]*scale;
      data[chGreen]=bars_RGBA[idx][1]*scale;
      data[chBlue ]=bars_RGBA[idx][2]*scale;
      data+=3;
    }
  }
  row0=r;
  row1=rows*3/4;
  for(r=row0; r<row1; r++) {
    for(c=0; c<cols; c++) {
      unsigned int grey=c*255/cols;
      data[chRed  ]=grey;
      data[chGreen]=grey;
      data[chBlue ]=grey;
      data+=3;
    }
  }
  row0=r;
  row1=rows*5/6;
  for(r=row0; r<row1; r++) {
    for(c=0; c<cols; c++) {
      unsigned int grey=255-c*255/cols;
      data[chRed  ]=grey;
      data[chGreen]=grey;
      data[chBlue ]=grey;
      data+=3;
    }
  }
  row0=r;
  row1=rows;
  for(r=0; r<(row1-row0)*cols; r++) {
    unsigned char grey=getRandom();
    data[chRed  ]=grey;
    data[chGreen]=grey;
    data[chBlue ]=grey;
    data+=3;
  }
}
void makeSMPTE_YUV(unsigned int cols, unsigned int rows,
                   unsigned char*DATA, float scale, bool noise)
{
  unsigned char*data=DATA;
  unsigned int r,c;
  unsigned int row0, row1;
  unsigned int halfcols=cols>>1;

  row0=0;
  row1=rows*2/3;
  for(r=row0; r<row1; r++) {
    data=DATA+r*cols*2;
    for(c=0; c<halfcols; c++) {
      unsigned int idx=c*7/halfcols;
      data[chY0]=data[chY1]=bars_YUV[idx][0]*scale;
      data[chU ]=bars_YUV[idx][1];
      data[chV ]=bars_YUV[idx][2];
      data+=4;
    }
  }
  row0=r;
  row1=rows*3/4;
  for(r=row0; r<row1; r++) {
    data=DATA+r*cols*2;
    for(c=0; c<cols; c++) {
      unsigned int grey=c*255/cols;
      data[chY0]=grey;
      data[chU ]=0x80;
      data+=2;
    }
  }
  row0=r;
  row1=rows*5/6;
  for(r=row0; r<row1; r++) {
    data=DATA+r*cols*2;
    for(c=0; c<cols; c++) {
      unsigned int grey=255-c*255/cols;
      data[chY0]=grey;
      data[chU ]=0x80;
      data+=2;
    }
  }
  row0=r;
  row1=rows;
  data=DATA+r*cols*2;
  for(r=0; r<(row1-row0)*cols; r++) {
    unsigned char grey=getRandom();
    data[chY0]=getRandom();
    data[chU ]=0x80;
    data+=2;
  }
}
void makeSMPTE_Grey(unsigned int cols, unsigned int rows,
                    unsigned char*data, float scale, bool noise)
{
  unsigned int r,c;
  unsigned int row0, row1;

  row0=0;
  row1=rows*2/3;
  for(r=row0; r<row1; r++) {
    for(c=0; c<cols; c++) {
      *data++=bars_YUV[c*7/cols][0]*scale;
    }
  }
  row0=r;
  row1=rows*3/4;
  for(r=row0; r<row1; r++) {
    for(c=0; c<cols; c++) {
      *data++=c*255/cols;
    }
  }
  row0=r;
  row1=rows*5/6;
  for(r=row0; r<row1; r++) {
    for(c=0; c<cols; c++) {
      *data++=255-c*255/cols;
    }
  }
  row0=r;
  row1=rows;
  for(r=0; r<(row1-row0)*cols; r++) {
    *data++=getRandom();
  }
}
};

/////////////////////////////////////////////////////////
//
// pix_test
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_test :: pix_test(int argc, t_atom*argv)
  : m_noise(true)
{
  m_pix.image.xsize=m_pix.image.ysize=128;
  switch(argc) {
  case 0:
    break;
  case 1:
    if(A_FLOAT == argv->a_type && ((int)atom_getfloat(argv))>0) {
      m_pix.image.xsize=m_pix.image.ysize=atom_getfloat(argv);
    } else {
      error("usage: pix_test <width=height>]");
    }
    break;
  case 2:
    if(A_FLOAT == argv[0].a_type && A_FLOAT == argv[1].a_type) {
      int i = atom_getfloat(argv);
      if(i>0) {
        m_pix.image.xsize = i;
      }
      i = atom_getfloat(argv+1);
      if(i>0) {
        m_pix.image.ysize = i;
      }
    } else {
      error("usage: pix_test [<width> <height>]");
    }
    break;
  default:
    error("usage: pix_test [<width> [<height>]]");
    break;
  }

  m_pix.image.setFormat(GEM_RGBA);
  m_pix.image.reallocate();
}
pix_test :: pix_test()
{
  m_pix.image.xsize=m_pix.image.ysize=128;
  m_pix.image.setFormat(GEM_RGBA);
  m_pix.image.reallocate();
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_test :: ~pix_test()
{ }

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_test :: render(GemState*state)
{
  bool noise = m_noise;
  float scale=1.;
  int rows=m_pix.image.xsize;
  int cols=m_pix.image.ysize;
  int datasize;
  unsigned char* data=m_pix.image.data;
  switch (m_pix.image.format) {
  case GEM_RGBA:
    makeSMPTE_RGBA(m_pix.image.xsize, m_pix.image.ysize, m_pix.image.data,
                   scale, noise);
    break;
  case GEM_RGB:
    makeSMPTE_RGB(m_pix.image.xsize, m_pix.image.ysize, m_pix.image.data,
                  scale, noise);
    break;
  case GEM_YUV:
    makeSMPTE_YUV(m_pix.image.xsize, m_pix.image.ysize, m_pix.image.data,
                  scale, noise);
    break;
  case GEM_GRAY:
    makeSMPTE_Grey(m_pix.image.xsize, m_pix.image.ysize, m_pix.image.data,
                   scale, noise);
    break;
  }
  //post("image=%d\tfilm=%d", m_pix.newimage,m_pix.newfilm);
  m_pix.newimage=true;
  state->set(GemState::_PIX, &m_pix);
}


void pix_test :: csMess(std::string cs)
{
  std::string color;
  unsigned int fmt=GEM_RGBA;
  char c=0;
  int len = cs.size();
  if(len>0) {
    char col[5];
    int i;
    if(len > 4) {
      len = 4;
    }
    for(i=0; i<len; i++) {
      col[i] = tolower(cs[i]);
    }
    col[len] = 0;
    color = col;
  }
  if ("rgba" == color) {
    fmt=GEM_RGBA;
  } else if ("rgb" == color) {
    fmt=GEM_RGB;
  } else if ("yuv" == color) {
    fmt=GEM_YUV;
  } else if (("grey" == color) || ("gray" == color)) {
    fmt=GEM_GRAY;
  } else {
    error("invalid colorspace '%s'; must be 'rgba', 'yuv' or 'grey'",
          cs.c_str());
    return;
  }
  m_pix.image.setFormat(fmt);
  dimenMess(m_pix.image.xsize, m_pix.image.ysize);

}
void pix_test :: postrender(GemState *state)
{
  m_pix.newimage = false;
  m_pix.newfilm  = false;
  state->set(GemState::_PIX, static_cast<pixBlock*>(0));
}

void pix_test :: dimenMess(unsigned int w, unsigned int h)
{
  m_pix.image.xsize=w;
  m_pix.image.ysize=h;
  if(GEM_YUV == m_pix.image.format && m_pix.image.xsize%2)
    m_pix.image.xsize+=1;
  m_pix.image.reallocate();
  m_pix.newfilm=true;
}
void pix_test :: noiseMess(bool noise) {
  m_noise = noise;
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_test :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG2(classPtr, "dimen", dimenMess, unsigned int, unsigned int);
  CPPEXTERN_MSG1(classPtr, "colorspace", csMess, std::string);
  CPPEXTERN_MSG1(classPtr, "noise", noiseMess, bool);
}
