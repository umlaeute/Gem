////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_crop.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS(pix_crop, t_float,A_DEFFLOAT,t_float, A_DEFFLOAT, t_float,A_DEFFLOAT,t_float, A_DEFFLOAT)

  /////////////////////////////////////////////////////////
//
// pix_crop
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_crop :: pix_crop(t_floatarg x=0, t_floatarg y=0, t_floatarg w=64, t_floatarg h=64)
{
  m_data = NULL;
  m_size = 0;

  offsetMess(x,y);
  if (w < 1.f)w=64.f;
  if (h < 1.f)h=64.f;
  dimenMess(w,h);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_crop :: ~pix_crop()
{
  if (m_data) delete [] m_data;
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_crop :: processImage(imageStruct &image)
{
  int x=(wantSizeX<image.xsize)?wantSizeX:image.xsize;
  int y=(wantSizeY<image.ysize)?wantSizeY:image.ysize;

  if (x*y*image.csize>m_size){
    if (m_data)delete[]m_data;
    m_size = wantSizeX*wantSizeY*image.csize;
    m_data = new unsigned char [m_size];
  }

  int offX=((offsetX%x)+x)%x;
  int offY=((offsetY%y)+y)%y;


  int i=0;
  while(i<y){
    unsigned char *newdata = m_data+(x*i)*image.csize;
    unsigned char *olddata = image.data+(((offY+i)%image.ysize)*image.xsize+offX)*image.csize;
    int j=x*image.csize;
    while(j--){
      *newdata++=*olddata++;
    }
    i++;
  }

  image.data   = m_data;
  image.xsize  = x;
  image.ysize  = y;
}


void pix_crop :: dimenMess(int x, int y){
  if(x<1)x=1;
  if(y<1)y=1;
  
  wantSizeX=x;
  wantSizeY=y;
  
  setPixModified();
}
void pix_crop :: offsetMess(int x, int y){
  offsetX=x;
  offsetY=y;
  
  setPixModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_crop :: obj_setupCallback(t_class *classPtr){
  class_addmethod(classPtr, (t_method)&pix_crop::dimenMessCallback, 
		  gensym("dimen"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_crop::offsetMessCallback, 
		  gensym("offset"), A_FLOAT, A_FLOAT, A_NULL);
}

void pix_crop :: dimenMessCallback(void *data, t_float x, t_float y){
  GetMyClass(data)->dimenMess((int)x, (int)y);
}
void pix_crop :: offsetMessCallback(void *data, t_float x, t_float y){
  GetMyClass(data)->offsetMess((int)x, (int)y);
}
