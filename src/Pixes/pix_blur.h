/*
 *  pix_blur.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_BLUR_H_
#define _INCLUDE__GEM_PIXES_PIX_BLUR_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_blur



  KEYWORDS
  pix
  yuv

  DESCRIPTION

  old motion blur object.  soon to be an abstraction for pix_convolve ??

  -----------------------------------------------------------------*/

class GEM_EXTERN pix_blur : public GemPixObj
{
  CPPEXTERN_HEADER(pix_blur, GemPixObj);

public:

  //////////
  // Constructor
  pix_blur(void);

protected:

  //////////
  // Destructor
  virtual ~pix_blur(void);

  //////////
  // Do the processing
  virtual void  processRGBAImage(imageStruct &image);
  virtual void  processGrayImage(imageStruct &image);
  virtual void  processYUVImage(imageStruct &image);
#ifdef __VEC__
  virtual void  processYUVAltivec(imageStruct &image);
#endif /* __VEC__ */

  unsigned int  *saved;
  void blurMess(float value);
  float         m_blurf;
  int           m_blurH,m_blurW,m_blurSize,m_blurBpp;

  t_inlet         *inletBlur;
};

#endif
