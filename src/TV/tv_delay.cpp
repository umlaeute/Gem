////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyleft  (l) 2002 IOhannes m zm�lnig
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "tv_delay.h"
#include <string.h>

CPPEXTERN_NEW_WITH_ONE_ARG(tv_delay, t_float,A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// tv_delay
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
tv_delay :: tv_delay(t_float &f)
{
  m_maxframes=(f>0)?(int)f:MAX_FRAMES;
  myImage.xsize=myImage.ysize=myImage.csize=1;
#ifdef IMAGE_CLASS
  myImage.allocate(1*m_maxframes);
#else
  myImage.data = new unsigned char[1*m_maxframes];
#endif
  m_curframe = m_frame = 0;

    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("delay"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
tv_delay :: ~tv_delay()
{
#ifdef IMAGE_CLASS
  myImage.clear();
#else
  delete [] myImage.data;
  myImage.data = NULL;
#endif
}

/////////////////////////////////////////////////////////
// sizeMess
//
/////////////////////////////////////////////////////////
void tv_delay :: delayMess(int frame)
{
  if (frame>=0)m_frame=(frame<m_maxframes)?frame:m_maxframes;
}


/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void tv_delay :: processFX(imageStruct &image)
{
  unsigned char *src = image.data;
  unsigned char *dest;

  unsigned int dataSize = image.xsize * image.ysize * image.csize;
  int readframe;

  if (myImage.xsize*myImage.ysize*myImage.csize != image.xsize*image.ysize*image.csize){
#ifdef IMAGE_CLASS
    myImage.clear();
    myImage.allocate(dataSize*m_maxframes);
#else
    delete [] myImage.data;
    myImage.data = new unsigned char[m_maxframes * dataSize];
#endif

    m_curframe=0;
  }

  myImage.xsize = image.xsize;
  myImage.ysize = image.ysize;
  myImage.csize = image.csize;
  myImage.type  = image.type;

  dest = myImage.data+m_curframe*dataSize;
  readframe=m_curframe-m_frame;
  readframe+=m_maxframes;
  readframe%=m_maxframes;

  // copy the data to the buffer
  //while(dataSize--)*src++=*dest++;
  memcpy(dest, src, dataSize);
  m_curframe++;
  m_curframe%=m_maxframes;  

  image.data=myImage.data+readframe*dataSize;
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void tv_delay :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&tv_delay::delayMessCallback,
  		  gensym("delay"), A_FLOAT, A_NULL);
}


void tv_delay :: delayMessCallback(void *data, t_floatarg frames)
{
  GetMyClass(data)->delayMess((int)frames);  
}
