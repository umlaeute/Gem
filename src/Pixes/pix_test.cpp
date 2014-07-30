////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
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

CPPEXTERN_NEW(pix_test);

/////////////////////////////////////////////////////////
//
// pix_test
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_test :: pix_test()
{
  m_pix.image.xsize=m_pix.image.ysize=128;
  m_pix.image.setCsizeByFormat(GL_RGBA_GEM);
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
  unsigned char off=128;
  int rows=m_pix.image.xsize;
  int cols=m_pix.image.ysize;
  int datasize;
  unsigned char* data=m_pix.image.data;
  switch (m_pix.image.format){
  case GL_RGBA:
    while(rows--){
      int col=cols/2;
      while(col--){
	data[0]=data[2]=off;
	data[1]=0;  data[3]=255;
	data+=4;
	data[0]=data[1]=data[2]=0;data[3]=255;
	data+=4;
      }
    }
    break;
  case GL_YUV422_GEM:
    datasize=m_pix.image.xsize*m_pix.image.ysize*m_pix.image.csize;
    while(datasize--)*data++=off;
    break;
    while(rows--){
      int col=cols;
      while(col--){
	*data++=off;
	*data++=128;
      }
    }
    break;
  case GL_LUMINANCE:
    datasize=rows*cols;
    while(datasize-->0) {
      *data++=off;
    }
  }
  m_pix.newimage=true;
  state->set(GemState::_PIX, &m_pix);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_test :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG2(classPtr, "dimen", dimenMess, unsigned int, unsigned int);
  CPPEXTERN_MSG1(classPtr, "colorspace", csMess, std::string);
}
void pix_test :: csMess(std::string cs)
{
  GLenum fmt=GL_RGBA_GEM;
  char c=0;
  if(cs.size()>0)
    c=tolower(cs[0]);
  switch(c) {
  case 'r': fmt=GL_RGBA_GEM;  break;
  case 'y': fmt=GL_YUV422_GEM;break;
  case 'g': fmt=GL_LUMINANCE; break;
  default:
    error("invalid colorspace '%s'; must be 'rgba', 'yuv' or 'grey'", cs.c_str());
    return;
  }
  m_pix.image.setCsizeByFormat(fmt);
  m_pix.image.reallocate();
  m_pix.newfilm=true;
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
  m_pix.image.reallocate();
  m_pix.newfilm=true;
}
