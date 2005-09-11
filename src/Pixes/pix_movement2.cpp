////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyrigth (c) 2004 Jakob Leiner & Theresa Rienmüller
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_movement2.h"
#include <string.h>
#include <math.h>

CPPEXTERN_NEW_WITH_TWO_ARGS(pix_movement2, t_float,A_DEFFLOAT,t_float, A_DEFFLOAT)

/*------------------------------------------------------------
  Constructor
  initializes the pixBlocks and pixBlobs
  ------------------------------------------------------------*/
  pix_movement2 :: pix_movement2(t_float lotresh, t_float hitresh): 
    m_frameIndex(0),
    m_storeBackground(true), m_resetTreshold(true)
{
  int i=3;
  while(i--){
    m_frame[i].xsize=0;
    m_frame[i].ysize=0;
    m_frame[i].setCsizeByFormat(GL_LUMINANCE);
    m_frame[i].reallocate();
  }
  m_output.xsize=0;
  m_output.ysize=0;
  m_output.setCsizeByFormat(GL_LUMINANCE);
  m_output.reallocate();

  m_background.xsize=0;
  m_background.ysize=0;
  m_background.setCsizeByFormat(GL_LUMINANCE);
  m_background.reallocate();

  m_treshold.xsize=0;
  m_treshold.ysize=0;
  m_treshold.setCsizeByFormat(GL_LUMINANCE);
  m_treshold.reallocate();

  m_lowtresh=CLAMP(255.f*MIN(lotresh, hitresh));
  m_tresh=CLAMP(255.f*MAX(lotresh, hitresh));
  if(m_tresh==0)   m_tresh=150;
  if(m_lowtresh==0)m_lowtresh=100;

  /*
    m_tresh = 150;
    m_lowtresh = 100;
  */

  m_lowtreshInlet=inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("low_tresh"));
  m_treshInlet=inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("hi_tresh"));
}

pix_movement2 :: ~pix_movement2(){
  if(m_treshInlet)inlet_free(m_treshInlet);
  if(m_lowtreshInlet)inlet_free(m_lowtreshInlet);
}

/*------------------------------------------------------------
  processImage
  calculates the difference image between three frames
  if the difference is greater than the treshhold, the concerning
  pixel is set to 255
  ------------------------------------------------------------*/
void pix_movement2 :: processImage(imageStruct &image){
  int size = image.xsize * image.ysize;
  int i;
  // 0. check if we have a new dimension
  bool firstTime = ((image.xsize!=m_frame[0].xsize) || (image.ysize!=m_frame[0].ysize));
  if (firstTime){
    m_storeBackground=true;
    m_resetTreshold=true;
    m_output.xsize = image.xsize;
    m_output.ysize = image.ysize;
    m_output.reallocate();

    m_background.xsize = image.xsize;
    m_background.ysize = image.ysize;
    m_background.reallocate();
    
    m_treshold.xsize = image.xsize;
    m_treshold.ysize = image.ysize;
    m_treshold.reallocate();

    i = 3;
    while(i--){
      m_frame[i].xsize=image.xsize;
      m_frame[i].ysize=image.ysize;
      m_frame[i].reallocate();
    }
    m_frameIndex=0;
  }
  if(m_resetTreshold){
    // we can do this, because we are in Greyscale-mode
    memset(m_treshold.data, m_tresh, m_treshold.xsize * m_treshold.ysize);
    m_resetTreshold=false;
  }

  // 1. store the current frame as gray-image in the apropriate buffer
  switch (image.format) {
  case GL_RGBA:         m_frame[m_frameIndex].fromRGBA(image.data); break;
  case GL_BGRA_EXT:     m_frame[m_frameIndex].fromBGRA(image.data); break;
  case GL_YCBCR_422_GEM:m_frame[m_frameIndex].fromUYVY(image.data); break;
  case GL_LUMINANCE:    m_frame[m_frameIndex].fromGray(image.data); break;
  default: error("pix_movement2: no method for this kind of color"); return;
  }

  // 2. if this is the first time, copy the current frame to the other frames as well
  if(m_storeBackground){
    m_storeBackground=false;
    m_background.fromGray(m_frame[0].data);
    for (i = 1; i < 3; i++)m_frame[i].fromGray(m_frame[0].data);
    // use our (black) "output"-image as "image" and return
    m_output.copy2ImageStruct(&image);

    if(firstTime)return;
  }
  // 3. now calc the difference
  unsigned char* cur   = m_frame[m_frameIndex].data;
  unsigned char* old2  = m_frame[(m_frameIndex+1)%3].data;
  unsigned char* old1  = m_frame[(m_frameIndex+2)%3].data;
  unsigned char* out   = m_output.data;
  unsigned char* tresh = m_treshold.data;
  unsigned char* back  = m_background.data;

  m_frameIndex++; m_frameIndex%=3;

  for(i=0; i<size; i++){

    if ((abs(cur[i]-old1[i]) > tresh[i]) &&
	(abs(cur[i]-old2[i]) > tresh[i])){
      out[i] = 255;
    } else {
      out[i] = 0;
      if(abs(cur[i]-back[i])>tresh[i]){
	out[i] = 255;
      }

      if(tresh[i] < m_lowtresh)tresh[i] = m_lowtresh;

      tresh[i] = ((256-26)*tresh[i]+(26)*5*abs(cur[i]-back[i]))>>8;
      back [i] = (26*back [i]+(256-26)*cur[i])>>8;
    }
  }
  m_output.upsidedown = image.upsidedown;
  m_output.copy2ImageStruct(&image);
}

/*------------------------------------------------------------
  treshMess
  ------------------------------------------------------------*/
void pix_movement2 :: treshMess(int tresh){
  if(tresh < (int)m_lowtresh){
    post("[pix_movement2]: high treshold (%d) must not be less than low treshold(%d)", tresh, m_lowtresh);
    return;
  }
  m_tresh = CLAMP(tresh);
  m_resetTreshold=true;
}

/*------------------------------------------------------------
  lowTreshMess
  ------------------------------------------------------------*/
void pix_movement2 :: lowTreshMess(int tresh){
  if(tresh > (int)m_tresh){
    post("[pix_movement2]: low treshold (%d) must not be be greater than high treshold(%d)", tresh, m_tresh);
    return;
  }
  m_lowtresh = CLAMP(tresh);
}

/*------------------------------------------------------------
  bangMess
  ------------------------------------------------------------*/
void pix_movement2 :: bangMess(){
  m_storeBackground = true;
}

/*------------------------------------------------------------

static member functions

------------------------------------------------------------*/
void pix_movement2 :: obj_setupCallback(t_class*classPtr){
  class_addmethod(classPtr, (t_method)&pix_movement2::lowTreshMessCallback,
		  gensym("low_tresh"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_movement2::treshMessCallback,
		  gensym("hi_tresh"), A_FLOAT, A_NULL);
  class_addbang(classPtr, (t_method)&pix_movement2::bangMessCallback);
}

/*------------------------------------------------------------
  treshMessCallback
  ------------------------------------------------------------*/
void pix_movement2 :: treshMessCallback(void *data, t_floatarg tresh)
{
  GetMyClass(data)->treshMess((int)(255*tresh));
}
/*------------------------------------------------------------
  lowTreshMessCallback
  ------------------------------------------------------------*/
void pix_movement2 :: lowTreshMessCallback(void *data, t_floatarg tresh)
{
  GetMyClass(data)->lowTreshMess((int)(255*tresh));
}
/*------------------------------------------------------------
  bangMessCallback
  ------------------------------------------------------------*/
void pix_movement2 :: bangMessCallback(void *data)
{
  GetMyClass(data)->bangMess();
}
