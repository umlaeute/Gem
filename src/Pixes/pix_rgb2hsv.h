/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  transform the RGB(A)-channels into HSV(A)

  Copyright (c) 1997-1999 Mark Danks. mark@danks.org
  Copyright (c) G�nther Geiger. geiger@epy.co.at
  Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
  Copyright (c) 2002 James Tittle & Chris Clepper
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_HSV2RGB_H_
#define INCLUDE_PIX_HSV2RGB_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_rgb2hsv
    
  Change pix to greyscale

  KEYWORDS
  pix
    
  DESCRIPTION
   
  -----------------------------------------------------------------*/
class GEM_EXTERN pix_rgb2hsv : public GemPixObj
{
  CPPEXTERN_HEADER(pix_rgb2hsv, GemPixObj)

    public:

  //////////
  // Constructor
  pix_rgb2hsv();
    	
 protected:
    	
  //////////
  // Destructor
  virtual ~pix_rgb2hsv();

  //////////
  // Do the processing
  virtual void 	processRGBAImage(imageStruct &image);
};

#endif	// for header file
