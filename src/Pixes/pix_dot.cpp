////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyleft  (l) 2001 IOhannes m zmölnig
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#if 0
#include "pix_dot.h"

CPPEXTERN_NEW(pix_dot)

static void drawDot(int xx, int yy, unsigned char c, unsigned char *dest)
{
  int x, y;
  unsigned char *pat;

  c = (c>>(8-m_dotdepth));
  pat = m_pattern + c * m_xdot * m_xdot;
  dest = dest + yy * dot_size * m_xsize + xx * dot_size;
  for(y=0; y<m_xdot; y++) {
    for(x=0; x<m_xdot; x++) {
      *dest++ = *pat++;
    }
    pat -= 2;
    for(x=0; x<m_xdot-1; x++) {
      *dest++ = *pat--;
    }
    dest += m_xsize - dot_size + 1;
    pat += m_xdot + 1;
  }
  pat -= m_xdot*2;
  for(y=0; y<m_xdot-1; y++) {
    for(x=0; x<m_xdot; x++) {
      *dest++ = *pat++;
    }
    pat -= 2;
    for(x=0; x<m_xdot-1; x++) {
      *dest++ = *pat--;
    }
    dest += m_xsize - dot_size + 1;
    pat += -m_xdot + 1;
  }
}

/////////////////////////////////////////////////////////
//
// pix_dot
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_dot :: pix_dot()
{
  myImage.xsize=myImage.ysize=myImage.csize=1;
  myImage.data = new unsigned char[1];

  m_dotx = 8;
  m_doty = 8;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_dot :: ~pix_dot()
{
  delete [] blockoffset;
  delete [] blockpos;
  if (myImage.data) delete [] myImage.data;
}


/////////////////////////////////////////////////////////
// draw a basic Dot
//
/////////////////////////////////////////////////////////
void pix_dot :: makePattern()
{
  int i, x, y;
  delete [] blockoffset;
  delete [] blockpos;

  blockxsize = xsize / blockw;
  blockysize = ysize / blockh;
  blocknum = blockw * blockh;

  marginw = xsize - blockw*blockxsize;
  marginh = ysize - blockh*blockysize;

  spacepos = blocknum - 1;

  blockoffset = new int[blocknum];
  blockpos = new int[blocknum];

  for(y=0; y<blockh; y++)
    for(x=0; x<blockw; x++) blockoffset[y*blockw+x] = (y*blockysize*xsize + x*blockxsize)*csize;

  for(i=0; i<blocknum; i++) blockpos[i] = i;
}

/////////////////////////////////////////////////////////
// sizeMess
//
/////////////////////////////////////////////////////////
void pix_dot :: sizeMess(int width, int height)
{

  dotw = (width>0)?width:8;
  doth = (height>0)?height:8;
}


/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_dot :: processFX(imageStruct &image)
{
  unsigned char *src = image.data;
  unsigned char *dest;

  int x, y, xx, yy, i;
  unsigned char *p, *q;

  if (m_force || (myImage.xsize*myImage.ysize*myImage.csize != image.xsize*image.ysize*image.csize)){
    int dataSize = image.xsize * image.ysize * image.csize;
    delete [] myImage.data;
    m_force = false;

    myImage.data = new unsigned char[dataSize];

    makePuzzleBlocks(image.xsize, image.ysize, image.csize);
    shuffle();
  }

  myImage.xsize = image.xsize;
  myImage.ysize = image.ysize;
  myImage.csize = image.csize;
  myImage.type  = image.type;

  dest = myImage.data;
 
  i=0;
  for (y=0; y<blockh; y++){
    for(x=0; x<blockw; x++) {
      p = &src[blockoffset[blockpos[i]]];
      q = &dest[blockoffset[i]];
      if(m_game && spacepos == i) { // leave one rectangle blank (for the puzzle game)
	for(yy=0; yy<blockysize; yy++) {
	  for(xx=0; xx<blockxsize*image.csize; xx++) {
	    q[xx] = 0;
	  }
	  q += image.xsize*image.csize;
	}
      } else {
	for(yy=0; yy<blockysize; yy++) {
	  for(xx=0; xx<blockxsize*image.csize; xx++) {
	    q[xx] = p[xx];
	  }
	  q += image.xsize*image.csize;
	  p += image.xsize*image.csize;
	}
      }
      i++;
    }
  }

  p = src +  blockw * blockxsize;
  q = dest + blockw * blockxsize;

  if(marginw) {
    for(y=0; y<blockh*blockysize; y++) {
      for(x=0; x<marginw; x++) {
	*q++ = *p++;
      }
      p += image.xsize - marginw;
      q += image.xsize - marginw;
    }
  }

  if(marginh) {
    p = src + (blockh * blockysize) * image.xsize;
    q = dest + (blockh * blockysize) * image.xsize;
    memcpy(p, q, marginh*image.xsize*image.csize);
  }

  image.data=myImage.data;
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_dot :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&pix_dot::sizeMessCallback,
  		  gensym("size"), A_FLOAT, A_FLOAT, A_NULL);
  class_addfloat(classPtr, (t_method)&pix_dot::stateMessCallback);
}


void pix_dot :: sizeMessCallback(void *data, t_floatarg width, t_floatarg height)
{
  GetMyClass(data)->sizeMess((int)width, (int)height);  
}

void pix_dot :: stateMessCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->activate(state);
}
#endif
