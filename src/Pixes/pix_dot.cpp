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
//    Copyleft (l) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#if 0
#include "pix_dot.h"

CPPEXTERN_NEW(pix_dot)

#define DOTDEPTH 5
#define DOTMAX (1<<DOTDEPTH)

static int state;
static unsigned char *pattern;
static unsigned char *heart_pattern;
static int dots_width;
static int dots_height;
static int dot_size;
static int dot_hsize;
static int *sampx, *sampy;
static int mode = 0;

static void drawDot(int xx, int yy, unsigned char c, unsigned char *dest)
{
  int x, y;
  unsigned char *pat;

  c = (c>>(8-DOTDEPTH));
  pat = pattern + c * dot_hsize * dot_hsize;
  dest = dest + yy * dot_size * m_xsize + xx * dot_size;
  for(y=0; y<dot_hsize; y++) {
    for(x=0; x<dot_hsize; x++) {
      *dest++ = *pat++;
    }
    pat -= 2;
    for(x=0; x<dot_hsize-1; x++) {
      *dest++ = *pat--;
    }
    dest += m_xsize - dot_size + 1;
    pat += dot_hsize + 1;
  }
  pat -= dot_hsize*2;
  for(y=0; y<dot_hsize-1; y++) {
    for(x=0; x<dot_hsize; x++) {
      *dest++ = *pat++;
    }
    pat -= 2;
    for(x=0; x<dot_hsize-1; x++) {
      *dest++ = *pat--;
    }
    dest += m_xsize - dot_size + 1;
    pat += -dot_hsize + 1;
  }
}

inline static unsigned char inline_RGBtoY(int rgb)
{
	int i;

	i = RtoY[(rgb>>16)&0xff];
	i += GtoY[(rgb>>8)&0xff];
	i += BtoY[rgb&0xff];
	return i;
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
  myImage.allocate(1);
  
  dot_size = 8;
  dot_hsize = dot_size / 2;
  dots_width = m_xsize / dot_size;
  dots_hieght = m_ysize / dot_size;
  
  pattern = (unsigned char *)malloc(DOTMAX * dot_hsize * dot_hsize * sizeof(unsigned char));
  if (pattern == NULL) {
    post("pix_dot couldn't make pattern");
    return NULL;
  }
  makePattern();
  // init_sampxy_table()
    int i, j;

    j = m_dot_xsize;
    for(i=0; i<dots_width; i++) {
        sampx[i] = j;// * video_width / m_xsize;
        j += dot_size;
    }
    j = m_dot_xsize;
    for(i=0; i<dots_height; i++) {
        sampy[i] = j;// * video_height / screen_height;
        j += dot_size;
    }
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_dot :: ~pix_dot()
{
  myImage.clear();
}


/////////////////////////////////////////////////////////
// draw a basic Dot
//
/////////////////////////////////////////////////////////
void pix_dot :: makePattern()
{
  int i, x, y, c;
  int u, v;
  double p, q, r;
  unsigned char *pat;
  
  for (i=0; i<DOTMAX; i++)
  {
/* Generated pattern is a quadrant of a disk. */
    pat = pattern + (i+1) * m_xdot * m_xdot - 1;
    r = (0.2 * i / DOTMAX + 0.8) * m_xdot;
    r = r*r;
    for(y=0; y<m_xdot; y++) {
        for(x=0; x<m_xdot; x++) {
            c = 0;
            for(u=0; u<4; u++) {
                p = (double)u/4.0 + y;
                p = p*p;
                for(v=0; v<4; v++) {
                    q = (double)v/4.0 + x;
                    if(p+q*q<r) {
                        c++;
                    }
                }
            }
            c = (c>15)?15:c;
            *pat-- = c<<20 | c<<12 | c<<4;
/* The upper left part of a disk is needed, but generated pattern is a bottom
 * right part. So I spin the pattern. */
        }
    }
  }
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

  int x, y, i, sx, sy;
  unsigned char *p, *q;

  if (myImage.xsize*myImage.ysize*myImage.csize != image.xsize*image.ysize*image.csize){
    int dataSize = image.xsize * image.ysize * image.csize;
    myImage.clear();

    myImage.allocate(dataSize);
  }

  myImage.xsize = image.xsize;
  myImage.ysize = image.ysize;
  myImage.csize = image.csize;
  myImage.type  = image.type;

  dest = myImage.data;
  
  for ( y=0; y<dots_height; y++) {
    sy = sampy[y];
    for ( x=0; x<dots_width; x++){
        sx = sampx[x];
        drawDot(x, y, inline_RGBtoY( src[sy*image.xsize+sx]), dest);
    }
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
