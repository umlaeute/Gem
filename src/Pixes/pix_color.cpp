////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_color.h"

CPPEXTERN_NEW(pix_color)

/////////////////////////////////////////////////////////
//
// pix_color
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_color :: pix_color()
{
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("vec_gain"));
    m_color[0] = m_color[1] = m_color[2] = 255;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_color :: ~pix_color()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_color :: processRGBAImage(imageStruct &image)
{
    int i = image.xsize * image.ysize;
    
    unsigned char *base = image.data;
	while (i--)
    {
      base[chRed] = m_color[0];
      base[chGreen] = m_color[1];
      base[chBlue] = m_color[2];
      base += 4;
    }
}
void pix_color :: processGrayImage(imageStruct &image)
{
  int i = image.xsize * image.ysize;
  unsigned char grey=(m_color[0]*79+m_color[1]*156+m_color[2]*21)>>8;
  unsigned char *base = image.data;
  while (i--)*base++=grey;
}
void pix_color :: processYUVImage(imageStruct &image)
{
  int i = image.xsize * image.ysize / 2;
  unsigned char u =((-43*m_color[0]- 85*m_color[1]+128*m_color[2])>>8)+128;
  unsigned char y =(  77*m_color[0]+150*m_color[1]+ 29*m_color[2])>>8;
  unsigned char v =((128*m_color[0]-107*m_color[1]- 21*m_color[2])>>8)+128;

  unsigned char *base = image.data;
  while (i--){
    base[chU]=u; base[chY0]=y;
    base[chV]=v; base[chY1]=y;
    base+=4;
  }
}
/////////////////////////////////////////////////////////
// vecGainMess
//
/////////////////////////////////////////////////////////
void pix_color :: vecGainMess(float red, float green, float blue, float alpha)
{
    m_color[0] = CLAMP(red   * 255);
    m_color[1] = CLAMP(green * 255);
    m_color[2] = CLAMP(blue  * 255);
    m_color[3] = CLAMP(alpha * 255);
   setPixModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_color :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classpix_color, 
		   gensym("pix_colour"), A_NULL);
    class_addmethod(classPtr, (t_method)&pix_color::vecGainMessCallback,
    	    gensym("vec_gain"), A_GIMME, A_NULL);
}
void pix_color :: vecGainMessCallback(void *data, t_symbol*, int argc, t_atom*argv)
{
  float r=0.0, g=0.0, b=0.0;
  float a=1.0;
  switch(argc){
  case 1:
    r=g=b=atom_getfloat(argv); break;
  case 4:
    a=atom_getfloat(argv+3);
  case 3:
    r=atom_getfloat(argv+0);
    g=atom_getfloat(argv+1);
    b=atom_getfloat(argv+2);
    break;
  default:
    error("pix_color: color must be 1, 3 or 4 values");
    return;
  }
   
  GetMyClass(data)->vecGainMess(r, g, b, a);
}
