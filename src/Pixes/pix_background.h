/*
 *  pix_background.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_pix_background_H_ 
#define INCLUDE_pix_background_H_ 

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_background
    
    

  KEYWORDS
  pix
  yuv
    
  DESCRIPTION

  template for yuv_ objects
   
  -----------------------------------------------------------------*/

class GEM_EXTERN pix_background : public GemPixObj
{
  CPPEXTERN_HEADER(pix_background, GemPixObj)

    public:

  //////////
  // Constructor
  pix_background();
    	
 protected:
    	
  //////////
  // Destructor
  virtual ~pix_background();

  //////////
  // Do the processing
  virtual void 	processRGBAImage(imageStruct &image);
  virtual void 	processGrayImage(imageStruct &image);
  virtual void 	processYUVImage(imageStruct &image);
        
  //////////
  // Do the YUV Altivec processing
  virtual void 	processYUVImageAltivec(imageStruct &image);
        
  unsigned char  *saved;
  int		m_Yrange,m_Urange,m_Vrange;
  int		m_blur,m_blurH,m_blurW,m_blurSize,m_blurBpp;
  t_inlet      *inletBlur;
  int		m_reset;

        
 private:
    
  //////////
  // Static member functions
    	
  static void rangeCallback       (void *data, t_floatarg Y, t_floatarg U, t_floatarg V);
  //  static void rangeNCallback      (void *data, t_floatarg Y, t_floatarg U, t_floatarg V);
  static void rangeNCallback      (void *data, t_symbol*,int,t_atom*);
  static void resetCallback       (void *data);


};

#endif

